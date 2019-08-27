//
//  ViewController.m
//  test_cpphttplib_by_app
//
//  Created by yu.zuo on 2019/8/27.
//  Copyright © 2019 yu.zuo. All rights reserved.
//

#import "ViewController.h"
#include "http/test_http.hpp"


@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    test_ssl();
}


@end
