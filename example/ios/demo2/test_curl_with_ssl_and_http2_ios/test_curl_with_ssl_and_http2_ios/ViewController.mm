//
//  ViewController.m
//  test_curl_with_ssl_and_http2_ios
//
//  Created by zuoyu on 2020/5/27.
//  Copyright © 2020 zuoyu. All rights reserved.
//

#import "ViewController.h"

#include "test_http2.hpp"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    test_curl_http2();
}


@end
