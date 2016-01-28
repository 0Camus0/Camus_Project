//
//  ViewController.m
//  Camus_Project
//
//  Created by Daniel on 1/25/16.
//  Copyright © 2016 Camus. All rights reserved.
//

#import "ViewController.h"
#include <Utils/Log.h>

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    LogPrintDebug("ViewController viewDidLoad");

    // Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    LogPrintDebug("ViewController didReceiveMemoryWarning");
    // Dispose of any resources that can be recreated.
}

@end
