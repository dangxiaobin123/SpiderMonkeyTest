//
//  main.m
//  debug-objc
//
//  Created by chenying on 16/8/2.
//
//

#import <Foundation/Foundation.h>
#import "XXObject.h"

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        // insert code here...
        NSLog(@"Hello, World!");
        
        XXObject *obj = [[XXObject alloc]init];
        [obj hello];
    }
    return 0;
}
