---
layout: post
title:  "class-dump"
date:   2015-12-18 13:38:04 +0800
categories: jekyll update
---


[class-dump源码工程](http://stevenygard.com)


![Smaller icon](http://25.io/smaller/favicon.ico "Title here")


今天我们来谈谈class-dump.
首先从官网下载下来最新版class-dump，然后编译。
拿出来class-dump运行文件，然后放在/usr / local / bin下

`class-dump -H /Applications/Calculator.app -o /Users/Rio/Desktop/heads`

上面的这个命令`-H`后面是可执行文件  `-o`后面是产生的.h文件存放路径

class-dump 作用对象必须是未经加密的可执行文件，而从 App Store 下载的 App 都是经过签名加密的，可执行文件被加上了一层“壳”。可以使用 AppCrackr 来自动砸壳。

