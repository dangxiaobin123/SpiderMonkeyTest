---
layout: post
title:  "debug-objc工程"
date:   2016-08-02 14:45:04 +0800
categories: jekyll update
---

##  objc源码下载

**[工程include文件参考链接](https://github.com/RetVal/objc-runtime)

**[首先下载最新源码runtime](http://opensource.apple.com//tarballs/objc4/)

##	编译报错，冷静！

*下面介绍如何处理错误

	1. objc-os.h dyld_priv.h not found.需要将include文件夹整个引入项目工程
	2. 在Search Paths里面加入inclue路径  直接拖拽也可以，或者填写"$(SRCROOT)/include"

	3.接下来报错objc-os.h lock_private.h not found. //直接注释掉

	4.os_lock_handoff_s unknown type:  找到这个typedef 添加之

	5.pthread_priority_t unknown type: 找这个typedef unsigned long pthread_priority_t; 添加之

	6.objc-auto.mm  CRSetCrashLogMessage2:在preprocessing里面添加LIBC_NO_LIBCRASHREPORTERCLIENT

	7.objc-errors.mm _simple.h not found. //直接注释掉 ASL_LEVEL_ERR. //直接注释掉

	8.objc-runtime-new.mm dyld_get_program_sdk_version() 找到这个extern声明 添加之

	9.objc-opt.mm objc_protocolopt_t //直接注释掉 return nil;

	10.setjmp.h "i386/setjmp.h"  这个是处理器选择x86_64

	11.can't open order file libobjc.order: Order File删掉路径，保留文件名

	12.library not found for -lCrashReporterClient:  Linking-Other Linker Flags:删除-lCrashReporterClient

***GAME OVER: SUCCESS!

##	开始调试

1.创建debug-objc的target，Command Line Tool的项目模版就可以

2.在objc-runtime-new.mm的lookUpImpOrForward里面打断点然后运行可以看到进入objc-runtime里了

$如果没进来，别急

*在Build Phases里面 Target Dependencies添加objc的target

*在Build Phases里面 Link Binary Libraries添加libobjc.A.dylib的库

[附送一个objc消息发送调试](http://www.open-open.com/lib/view/open1461650361464.html)

