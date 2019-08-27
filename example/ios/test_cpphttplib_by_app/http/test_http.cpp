//
//  test_http.cpp
//  test_cpphttplib_by_app
//
//  Created by yu.zuo on 2019/8/27.
//  Copyright © 2019 yu.zuo. All rights reserved.
//

#include <iostream>
#include "test_http.hpp"
#include "httplib.h"


//#define CA_CERT_FILE "./ca-bundle.crt"
#define CA_CERT_FILE "ca-bundle.crt"
using namespace std;

void test_https() {
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    httplib::SSLClient cli("httpbin.org");
    //    httplib::SSLClient cli("localhost", 8080);
    // httplib::SSLClient cli("google.com");
    // httplib::SSLClient cli("www.youtube.com");
    cli.set_ca_cert_path(CA_CERT_FILE);
    cli.enable_server_certificate_verification(true);
#else
    httplib::Client cli("localhost", 8080);
#endif
    
    auto res = cli.Get("/get");
    if (res) {
        cout << res->status << endl;
        cout << res->get_header_value("Content-Type") << endl;
        cout << res->body << endl;
    } else {
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        auto result = cli.get_openssl_verify_result();
        if (result) {
            cout << "verify error: " << X509_verify_cert_error_string(result) << endl;
        }
#endif
    }
    
    return ;
}

using namespace httplib;
void test_http2()
{
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    httplib::SSLClient cli("httpbin.org");
    //    httplib::SSLClient cli("localhost", 8080);
    // httplib::SSLClient cli("google.com");
    // httplib::SSLClient cli("www.youtube.com");
    cli.set_ca_cert_path(CA_CERT_FILE);
    cli.enable_server_certificate_verification(true);
#else
    httplib::Client cli("192.168.4.193", 9000);
#endif
    //    Request req;
    //    req.method = "POST";
    //    req.path = "/multipart";
    
    std::string host_and_port;
    host_and_port += "192.168.4.193";
    host_and_port += ":";
    host_and_port += std::to_string(9000);
    
    Headers headers;
    //    headers.emplace("Host", host_and_port.c_str());
    headers.emplace("Accept", "*/*");
    headers.emplace("User-Agent", "cpp-httplib/0.1");
    //    headers.emplace("Content-Type", "multipart/form-data; boundary=----WebKitFormBoundarysBREP3G013oUrLB4");
    
    std::string body =
    "------WebKitFormBoundarysBREP3G013oUrLB4\r\n"
    "Content-Disposition: form-data; name=\"uid\"\r\n"
    "\r\n"
    "aaa\r\n"
    "------WebKitFormBoundarysBREP3G013oUrLB4\r\n"
    "Content-Disposition: form-data; name=\"name\"\r\n"
    "\r\n"
    "aaa\r\n"
    "------WebKitFormBoundarysBREP3G013oUrLB4\r\n";
    
    
    
    
    //    std::string body;
    ////    body = "----------------------------808752360867887258287386\r\n"
    ////    "Content-Disposition: form-data; name=\"uid\"\r\n"
    ////    "\r\n"
    ////    "zuoyu1\r\n"
    ////    "----------------------------808752360867887258287386\r\n"
    ////    "Content-Disposition: form-data; name=\"name\"\r\n"
    ////    "\r\n"
    ////    "zuoyu1name\r\n"
    ////    "----------------------------808752360867887258287386\r\n"
    ////    "\r\n";
    //    body = "----------------------------808752360867887258287386\r\nContent-Disposition: form-data; name=\"uid\"\r\n\r\nzuoyu1\r\n----------------------------808752360867887258287386\r\nContent-Disposition: form-data; name=\"name\"\r\n\r\nzuoyu1name\r\n----------------------------808752360867887258287386\r\n\r\n";
    //    auto res = cli.Post("/auth/token?appkey=5cdir6tjdujot", body, "multipart/form-data; boundary=----------------------------808752360867887258287386");
    auto res = cli.Post("/auth/token?appkey=5cdir6tjdujot", headers, body, "multipart/form-data; boundary=----WebKitFormBoundarysBREP3G013oUrLB4");
    if (res) {
        cout << res->status << endl;
        cout << res->get_header_value("Content-Type") << endl;
        cout << res->body << endl;
    } else {
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        auto result = cli.get_openssl_verify_result();
        if (result) {
            cout << "verify error: " << X509_verify_cert_error_string(result) << endl;
        }
#endif
    }
}

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h>
#endif

#ifdef CPPHTTPLIB_ZLIB_SUPPORT
#include <zlib.h>
#endif

void test_ssl()
{
    SSL_CTX* ctx_ = SSL_CTX_new(SSLv23_client_method());
    SSL *ssl = nullptr;
    ssl = SSL_new(ctx_);

    int ret = SSL_CTX_load_verify_locations(ctx_, CA_CERT_FILE, nullptr);
    if (!ret) {
        //failed.
        int err = SSL_get_error(ssl, ret);
        std::cout << err << std::endl;
    }
    std::cout << ret << std::endl;
    
}
