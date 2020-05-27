//
//  test_http2.cpp
//  test_http2
//
//  Created by zuoyu on 2020/5/21.
//  Copyright © 2020 zuoyu. All rights reserved.
//

#include "test_http2.hpp"
#include "certificate.h"

#include <string>

#include <curl/curl.h>

static void test1();
static void test2();
static void test3();
void test_curl_http2()
{
    test2();
}

/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2019, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
/* <DESC>
 * Multiplexed HTTP/2 downloads over a single connection
 * </DESC>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* somewhat unix-specific */
#include <sys/time.h>
#include <unistd.h>

/* curl stuff */
#include <curl/curl.h>

#ifndef CURLPIPE_MULTIPLEX
/* This little trick will just make sure that we don't enable pipelining for
   libcurls old enough to not have this symbol. It is _not_ defined to zero in
   a recent libcurl header. */
#define CURLPIPE_MULTIPLEX 0
#endif

struct transfer {
  CURL *easy;
  unsigned int num;
  FILE *out;
};

#define NUM_HANDLES 1000

static
void dump(const char *text, int num, unsigned char *ptr, size_t size,
          char nohex)
{
  size_t i;
  size_t c;

  unsigned int width = 0x10;

  if(nohex)
    /* without the hex output, we can fit more on screen */
    width = 0x40;

  fprintf(stderr, "%d %s, %lu bytes (0x%lx)\n",
          num, text, (unsigned long)size, (unsigned long)size);

  for(i = 0; i<size; i += width) {

    fprintf(stderr, "%4.4lx: ", (unsigned long)i);

    if(!nohex) {
      /* hex not disabled, show it */
      for(c = 0; c < width; c++)
        if(i + c < size)
          fprintf(stderr, "%02x ", ptr[i + c]);
        else
          fputs("   ", stderr);
    }

    for(c = 0; (c < width) && (i + c < size); c++) {
      /* check for 0D0A; if found, skip past and start a new line of output */
      if(nohex && (i + c + 1 < size) && ptr[i + c] == 0x0D &&
         ptr[i + c + 1] == 0x0A) {
        i += (c + 2 - width);
        break;
      }
      fprintf(stderr, "%c",
              (ptr[i + c] >= 0x20) && (ptr[i + c]<0x80)?ptr[i + c]:'.');
      /* check again for 0D0A, to avoid an extra \n if it's at width */
      if(nohex && (i + c + 2 < size) && ptr[i + c + 1] == 0x0D &&
         ptr[i + c + 2] == 0x0A) {
        i += (c + 3 - width);
        break;
      }
    }
    fputc('\n', stderr); /* newline */
  }
}

static
int my_trace(CURL *handle, curl_infotype type,
             char *data, size_t size,
             void *userp)
{
  const char *text;
  struct transfer *t = (struct transfer *)userp;
  unsigned int num = t->num;
  (void)handle; /* prevent compiler warning */

  switch(type) {
  case CURLINFO_TEXT:
    fprintf(stderr, "== %u Info: %s", num, data);
    /* FALLTHROUGH */
  default: /* in case a new one is introduced to shock us */
    return 0;

  case CURLINFO_HEADER_OUT:
    text = "=> Send header";
    break;
  case CURLINFO_DATA_OUT:
    text = "=> Send data";
    break;
  case CURLINFO_SSL_DATA_OUT:
    text = "=> Send SSL data";
    break;
  case CURLINFO_HEADER_IN:
    text = "<= Recv header";
    break;
  case CURLINFO_DATA_IN:
    text = "<= Recv data";
    break;
  case CURLINFO_SSL_DATA_IN:
    text = "<= Recv SSL data";
    break;
  }

  dump(text, num, (unsigned char *)data, size, 1);
  return 0;
}

static void setup(struct transfer *t, int num)
{
  char filename[128];
  CURL *hnd;

  hnd = t->easy = curl_easy_init();

  snprintf(filename, 128, "dl-%d", num);

  t->out = fopen(filename, "wb");

  /* write to this file */
  curl_easy_setopt(hnd, CURLOPT_WRITEDATA, t->out);

  /* set the same URL */
  curl_easy_setopt(hnd, CURLOPT_URL, "https://curl.haxx.se/libcurl/c/http2-download.html");

  /* please be verbose */
  curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);
  curl_easy_setopt(hnd, CURLOPT_DEBUGFUNCTION, my_trace);
  curl_easy_setopt(hnd, CURLOPT_DEBUGDATA, t);

  /* HTTP/2 please */
//  curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);

  /* we use a self-signed test server, skip verification during debugging */
  curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);

#if (CURLPIPE_MULTIPLEX > 0)
  /* wait for pipe connection to confirm */
  curl_easy_setopt(hnd, CURLOPT_PIPEWAIT, 1L);
#endif
}

/*
 * Download many transfers over HTTP/2, using the same connection!
 */
static int download_many_transfers(int argc, const char **argv)
{
  struct transfer trans[NUM_HANDLES];
  CURLM *multi_handle;
  int i;
  int still_running = 0; /* keep number of running handles */
  int num_transfers;
  if(argc > 1) {
    /* if given a number, do that many transfers */
    num_transfers = atoi(argv[1]);
    if((num_transfers < 1) || (num_transfers > NUM_HANDLES))
      num_transfers = 3; /* a suitable low default */
  }
  else
    num_transfers = 1; /* suitable default */

  /* init a multi stack */
  multi_handle = curl_multi_init();

  for(i = 0; i < num_transfers; i++) {
    setup(&trans[i], i);

    /* add the individual transfer */
    curl_multi_add_handle(multi_handle, trans[i].easy);
  }

  curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);

  /* we start some action by calling perform right away */
  curl_multi_perform(multi_handle, &still_running);

  while(still_running) {
    struct timeval timeout;
    int rc; /* select() return code */
    CURLMcode mc; /* curl_multi_fdset() return code */

    fd_set fdread;
    fd_set fdwrite;
    fd_set fdexcep;
    int maxfd = -1;

    long curl_timeo = -1;

    FD_ZERO(&fdread);
    FD_ZERO(&fdwrite);
    FD_ZERO(&fdexcep);

    /* set a suitable timeout to play around with */
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    curl_multi_timeout(multi_handle, &curl_timeo);
    if(curl_timeo >= 0) {
      timeout.tv_sec = curl_timeo / 1000;
      if(timeout.tv_sec > 1)
        timeout.tv_sec = 1;
      else
        timeout.tv_usec = (curl_timeo % 1000) * 1000;
    }

    /* get file descriptors from the transfers */
    mc = curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);

    if(mc != CURLM_OK) {
      fprintf(stderr, "curl_multi_fdset() failed, code %d.\n", mc);
      break;
    }

    /* On success the value of maxfd is guaranteed to be >= -1. We call
       select(maxfd + 1, ...); specially in case of (maxfd == -1) there are
       no fds ready yet so we call select(0, ...) --or Sleep() on Windows--
       to sleep 100ms, which is the minimum suggested value in the
       curl_multi_fdset() doc. */

    if(maxfd == -1) {
#ifdef _WIN32
      Sleep(100);
      rc = 0;
#else
      /* Portable sleep for platforms other than Windows. */
      struct timeval wait = { 0, 100 * 1000 }; /* 100ms */
      rc = select(0, NULL, NULL, NULL, &wait);
#endif
    }
    else {
      /* Note that on some platforms 'timeout' may be modified by select().
         If you need access to the original value save a copy beforehand. */
      rc = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout);
    }

    switch(rc) {
    case -1:
      /* select error */
      break;
    case 0:
    default:
      /* timeout or readable/writable sockets */
      curl_multi_perform(multi_handle, &still_running);
      break;
    }
  }

  for(i = 0; i < num_transfers; i++) {
    curl_multi_remove_handle(multi_handle, trans[i].easy);
    curl_easy_cleanup(trans[i].easy);
  }

  curl_multi_cleanup(multi_handle);

  return 0;
}

static void test1()
{
    int argc = 1;
    const char* p1 = "1";
    const char* p2 = "2";
    const char *argv[] = {p1, p2};
    
    download_many_transfers(argc, argv);
}

struct MemoryObject {
    char* data;
    size_t size;
};
    static size_t write_callback(void *content, size_t size, size_t nmemb, void *out)
    {
        if (1 != size) {
//            spdlog::warn("[{0}][{1}]", __FUNCTION__, __LINE__);
            return 0;
        }
        MemoryObject* obj = (MemoryObject*)out;
        size_t byte_size = size * nmemb;
        size_t old_size = obj->size;
        if (0 == obj->size) {
            obj->size = byte_size;
            obj->data = (char*)malloc(obj->size);
        } else {
            obj->size += byte_size;
            obj->data = (char*)realloc(obj->data, obj->size);
        }
        memcpy(obj->data + old_size, content, nmemb);
//        spdlog::trace("[{0}][{1}][{2}]", __FUNCTION__, __LINE__, std::string(obj->data, obj->size));
        return byte_size;
//        std::string&& str = std::string((char*)content, size * nmemb);
//        spdlog::warn("[{0}][{1}][{2}]", __FUNCTION__, __LINE__, str);
//        (*(std::stringstream*)out) << std::string((char*)content, size * nmemb);
//        return size * nmemb;
    }

#include <openssl/err.h>
#include <openssl/ssl.h>
static CURLcode sslctx_function(CURL *curl, void *sslctx, void *parm)
{
  CURLcode rv = CURLE_ABORTED_BY_CALLBACK;

  /** This example uses two (fake) certificates **/
  BIO *cbio = BIO_new_mem_buf(pem_from_haxx_se, sizeof(pem_from_haxx_se));
  X509_STORE  *cts = SSL_CTX_get_cert_store((SSL_CTX *)sslctx);
  int i;
  STACK_OF(X509_INFO) *inf;
  (void)curl;
  (void)parm;

  if(!cts || !cbio) {
    return rv;
  }

  inf = PEM_X509_INFO_read_bio(cbio, NULL, NULL, NULL);

  if(!inf) {
    BIO_free(cbio);
    return rv;
  }

  for(i = 0; i < sk_X509_INFO_num(inf); i++) {
    X509_INFO *itmp = sk_X509_INFO_value(inf, i);
    if(itmp->x509) {
      X509_STORE_add_cert(cts, itmp->x509);
    }
    if(itmp->crl) {
      X509_STORE_add_crl(cts, itmp->crl);
    }
  }

  sk_X509_INFO_pop_free(inf, X509_INFO_free);
  BIO_free(cbio);

  rv = CURLE_OK;
  return rv;
}
static void test_http2_get()
{
//    std::string url = "http://gcc.gnu.org/onlinedocs/cpp/";
//    std::string url = "http://nghttp2.org";
//    std::string url = "https://www.example.com/";
    std::string url = "https://www.sina.com.cn";
//    std::string url = "https://www.baidu.com";
    bool enabled = true;
    bool is_http_2 = true;
    MemoryObject response_header;
    response_header.data = nullptr;
    response_header.size = 0;
    MemoryObject response_body;
    response_body.data = nullptr;
    response_body.size = 0;
    //        std::stringstream out;
    
    CURL* curl = curl_easy_init();
    
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, *write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response_body);
    //        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&out);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, *write_callback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void*)&response_header);
    
    /* HTTP/2 please */
    if (is_http_2) {
        curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);
    }
    
    if (enabled) {
        curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
        
        curl_easy_setopt(curl, CURLOPT_CAINFO, NULL);
        curl_easy_setopt(curl, CURLOPT_CAPATH, NULL);
        
        curl_easy_setopt(curl, CURLOPT_SSL_CTX_FUNCTION, *sslctx_function);
        
        static std::string https("https://");
        if (url.substr(0, 8) == https) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        } else {
            std::string url_with_https(https + url);
            curl_easy_setopt(curl, CURLOPT_URL, url_with_https.c_str());
        }
        
    } else {
        static std::string http("http://");
        if (url.substr(0, 7) == http) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        } else {
            std::string url_with_http(http + url);
            curl_easy_setopt(curl, CURLOPT_URL, url_with_http.c_str());
        }
    }
    
    
//    if (before_callback) {
//        before_callback(curl);
//    }
    
    std::string result;
    CURLcode ret = curl_easy_perform(curl);
    if(ret != CURLE_OK) {
        int a  =0;
        
    } else {
        result = std::string(response_header.data, response_header.size);

        }
    
    curl_easy_cleanup(curl);
}


static void test2()
{
    test_http2_get();
}

static size_t writefunction(void *ptr, size_t size, size_t nmemb, void *stream)
{
  fwrite(ptr, size, nmemb, (FILE *)stream);
  return (nmemb*size);
}
int test_curl_ca_cert(void)
{
  CURL *ch;
  CURLcode rv;

  curl_global_init(CURL_GLOBAL_ALL);
  ch = curl_easy_init();
  curl_easy_setopt(ch, CURLOPT_VERBOSE, 0L);
  curl_easy_setopt(ch, CURLOPT_HEADER, 0L);
  curl_easy_setopt(ch, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(ch, CURLOPT_NOSIGNAL, 1L);
  curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, *writefunction);
  curl_easy_setopt(ch, CURLOPT_WRITEDATA, stdout);
  curl_easy_setopt(ch, CURLOPT_HEADERFUNCTION, *writefunction);
  curl_easy_setopt(ch, CURLOPT_HEADERDATA, stderr);
  curl_easy_setopt(ch, CURLOPT_SSLCERTTYPE, "PEM");
  curl_easy_setopt(ch, CURLOPT_SSL_VERIFYPEER, 1L);
//    curl_easy_setopt(ch, CURLOPT_SSL_VERIFYPEER, 0L);
//    curl_easy_setopt(ch, CURLOPT_SSL_VERIFYHOST, 0L);
//  curl_easy_setopt(ch, CURLOPT_URL, "https://www.example.com/");
    curl_easy_setopt(ch, CURLOPT_URL, "https://www.github.com/");

  /* Turn off the default CA locations, otherwise libcurl will load CA
   * certificates from the locations that were detected/specified at
   * build-time
   */
  curl_easy_setopt(ch, CURLOPT_CAINFO, NULL);
  curl_easy_setopt(ch, CURLOPT_CAPATH, NULL);

  /* first try: retrieve page without ca certificates -> should fail
   * unless libcurl was built --with-ca-fallback enabled at build-time
   */
  rv = curl_easy_perform(ch);
  if(rv == CURLE_OK)
    printf("*** transfer succeeded ***\n");
  else
    printf("*** transfer failed ***\n");

  /* use a fresh connection (optional)
   * this option seriously impacts performance of multiple transfers but
   * it is necessary order to demonstrate this example. recall that the
   * ssl ctx callback is only called _before_ an SSL connection is
   * established, therefore it will not affect existing verified SSL
   * connections already in the connection cache associated with this
   * handle. normally you would set the ssl ctx function before making
   * any transfers, and not use this option.
   */
  curl_easy_setopt(ch, CURLOPT_FRESH_CONNECT, 1L);

  /* second try: retrieve page using cacerts' certificate -> will succeed
   * load the certificate by installing a function doing the necessary
   * "modifications" to the SSL CONTEXT just before link init
   */
  curl_easy_setopt(ch, CURLOPT_SSL_CTX_FUNCTION, *sslctx_function);
  rv = curl_easy_perform(ch);
  if(rv == CURLE_OK)
    printf("*** transfer succeeded ***\n");
  else
    printf("*** transfer failed ***\n");

  curl_easy_cleanup(ch);
  curl_global_cleanup();
  return rv;
}
static void test3()
{
    test_curl_ca_cert();
}
