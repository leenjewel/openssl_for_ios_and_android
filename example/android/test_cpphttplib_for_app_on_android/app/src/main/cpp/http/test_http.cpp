//
//  test_http.cpp
//  test_cpphttplib_by_app
//
//  Created by yu.zuo on 2019/8/27.
//  Copyright © 2019 yu.zuo. All rights reserved.
//

#include <iostream>
#include "test_http.h"
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
void test_https(const char* cert_file_name)
{
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    httplib::SSLClient cli("httpbin.org");
    //    httplib::SSLClient cli("localhost", 8080);
    // httplib::SSLClient cli("google.com");
    // httplib::SSLClient cli("www.youtube.com");
    cli.set_ca_cert_path(cert_file_name);
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
    int a = OPENSSL_API_COMPAT;
    std::cout << a << std::endl;
    
    const SSL_METHOD* method = SSLv23_client_method();
    if(NULL == method) {
        std::cout << "method is error." << std::endl;
    }
    
    SSL_CTX* ctx_ = SSL_CTX_new(method);
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

#include <fstream>

void test_file_op()
{
    ifstream f(CA_CERT_FILE);
    if (f.is_open()) {
        cout << "f is open." << endl;
    }
    else{
        cout << "f is not open." << endl;
    }
    
    ifstream f2;
    f2.open(CA_CERT_FILE);
    if (f2.is_open()) {
        cout << "f2 is open." << endl;
    }
    else{
        cout << "f2 is not open." << endl;
    }
}

#include <cstdio>
#include <cerrno>

void test_file_op_on_c()
{
    std::FILE* f = fopen(CA_CERT_FILE, "r");
    if (!f) {
        int ret = errno;
        cout << "f is not open. " << ret << endl;
    }
    else {
        cout << "f is open." << endl;
    }
}
void test_file_op_on_c2(const char* filename)
{
    std::FILE* f = fopen(filename, "r");
    if (!f) {
        int ret = errno;
        cout << "f is not open. " << ret << endl;
    }
    else {
        cout << "f is open." << endl;
    }
}
