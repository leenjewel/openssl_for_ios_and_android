//
//  ViewController.m
//  test_cpphttplib_by_app
//
//  Created by yu.zuo on 2019/8/27.
//  Copyright © 2019 yu.zuo. All rights reserved.
//

#import "ViewController.h"
#include "http/test_http.hpp"

#define OC_CA_CERT_FILE @"ca-bundle.crt"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
//    test_file_op_on_c();
//    [self getAllDirs];
    NSString* fileName = [self getAppPackageResourcePath:OC_CA_CERT_FILE];
//    test_file_op_on_c2([fileName UTF8String]);
    test_https([fileName UTF8String]);
}


//ref: https://www.jianshu.com/p/be80c46ab731
//ref: https://blog.csdn.net/kwuwei/article/details/38421247
- (void)getAllDirs {
    //==============================================================================
    
    // 资源目录
    
    NSString *resourceDir = [[NSBundle mainBundle] resourcePath];
    NSLog(@"resourceDir = %@\n", resourceDir);
    
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSDirectoryEnumerator *enumerator = [fileManager enumeratorAtPath:resourceDir];
    //遍历属性
    NSString *fileName;
    //下面这个方法最为关键 可以给fileName赋值，获得文件名（带文件后缀）。
    while (fileName = [enumerator nextObject]) {
        //跳过子路径
        [enumerator skipDescendants];
        //获取文件属性
        //enumerator.fileAttributes 的后面可以用点语法点出许多许多的属性。
        NSLog(@"%@",enumerator.fileAttributes);
        NSLog(@"%@",enumerator.directoryAttributes);
    }
    NSArray* fileList = [fileManager contentsOfDirectoryAtPath:resourceDir error:nil];
    NSLog(@"fileList = %@\n", fileList);
    //==============================================================================
    
    // 获取程序Documents目录路径
    // 目录列表
    
    NSArray *docDir = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSLog(@"docDir = %@\n", docDir);
    // 根目录
    
    NSString *docRootDir = [docDir objectAtIndex:0];
    NSLog(@"docRootDir = %@\n", docRootDir);
    //==============================================================================
    
    // 获取程序Library目录路径
    // 目录列表
    
    NSArray *libDir = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
    NSLog(@"libDir = %@\n", libDir);
    // 根目录
    
    NSString *libRootDir = [libDir objectAtIndex:0];
    NSLog(@"libRootDir = %@\n", libRootDir);
    
    //==============================================================================
    
    // 获取程序caches目录
    NSArray *cacheDir = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSLog(@"cacheDir = %@\n", cacheDir);
    NSString *cacheRootDir = [cacheDir objectAtIndex:0] ;
    NSLog(@"cacheRootDir = %@\n", cacheRootDir);
    //==============================================================================
    

    
    //==============================================================================
    // 获取程序app文件所在目录路径
    
    NSString *homeDir = NSHomeDirectory();
    NSLog(@"homeDir = %@\n", homeDir);
    //==============================================================================
    
    // 获取程序tmp目录路径
    
    NSString *tempDir = NSTemporaryDirectory();
    NSLog(@"tempDir = %@\n", tempDir);
    
    //==============================================================================
    
    // 获取程序应用包路径
    
    NSString * path = [[NSBundle mainBundle] resourcePath];
    NSLog(@"path = %@\n", path);
    // 或
    
    NSString * path2 = [[NSBundle mainBundle] pathForResource: @"info" ofType: @"txt"];
    NSLog(@"path2 = %@\n", path2);
    //==============================================================================
    //==============================================================================
    //==============================================================================
}

- (NSString*)getAppPackageResourcePath:(NSString*)resourceName
{
    NSString *resourceDir = [[NSBundle mainBundle] resourcePath];
    NSLog(@"resourceDir = %@\n", resourceDir);
    NSString* path = [NSString stringWithFormat:@"%@/%@", resourceDir, resourceName];
    NSLog(@"path = %@\n", path);
    return path;
}

@end
