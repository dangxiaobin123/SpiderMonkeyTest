---
layout: post
title:  "关于Block循环引用的问题"
date:   2015-11-30 17:16:04 +0800
categories: jekyll update
---

####  Block是什么？

首先探究下Block的实现原理，由于Objective-C是C语言的超集，OC中的NSObject对象其实是由C语言的struct+isa指针实现的，Block的内部实现估计也一样，以下三篇Blog对Block的实现机制做了详细研究

1. [A look inside blocks: Episode 1](http://www.galloway.me.uk/2012/10/a-look-inside-blocks-episode-1/)
2. [A look inside blocks: Episode 2](http://www.galloway.me.uk/2012/10/a-look-inside-blocks-episode-2/)
3. [A look inside blocks: Episode 3](http://www.galloway.me.uk/2012/10/a-look-inside-blocks-episode-3/)

		struct __block_impl {
		  void *isa;
		  int Flags;
		  int Reserved;
		  void *FuncPtr;
		};

这里的isa可能指向三种类型之一的Block:

* __NSGlobalBlock__：全局类型Block，在编译器就已经确定，直接放在代码段__TEXT上。
* __NSStackBlock__：位于栈上分配的Block。
* __NSMallocBlock__：位于堆上分配的Block。


*[@Block热身题](http://blog.parse.com/learn/engineering/objective-c-blocks-quiz/)*


####	Block循环引用
由于我们很多行为会导致Block的copy，而当Block被copy时，会对block中用到的对象产生强引用(ARC下)或者引用计数加一(MRC下)。

* ex.1
	
		@property(nonatomic, readwrite, copy) completionBlock completionBlock;
		
		//========================================
		self.completionBlock = ^ {
		        if (self.success) {
		            self.success(self.responseData);
		        }
		    }
		};



对象有一个Block属性，Block属性中又引用了对象的其他成员变量，那么就会对这个变量本身产生强引用，那么变量本身和他自己的Block属性就形成了循环引用。在ARC下需要用weak来修饰self ，
____weak typeof(self) weakSelf = self;
在MRC下就用____block typeof(self) weakSelf = self;

```
待续...
```

####	Block扩展
下面我们通过观察C函数的生成中间编译代码，来了解Block实现原理

main.m

{% highlight ruby %}

#include <stdio.h>
int main(int argc, char * argv[]) {
    @autoreleasepool {
        int age = 10;
        void (^block)() = ^{
            printf("%d",age);
        };
        block();//
    }
    return 0;
}

{% endhighlight %}


* clang -rewrite-objc main.m之后

{% highlight ruby %}

static void __main_block_func_0(struct __main_block_impl_0 *__cself) {
 	int age = __cself->age; // bound by copy
    printf("%d",age);
   }


int main(int argc, char * argv[]) {
    /* @autoreleasepool */ { __AtAutoreleasePool __autoreleasepool; 
        int age = 10;
        void (*block)() = ((void (*)())&__main_block_impl_0((void *)__main_block_func_0, &__main_block_desc_0_DATA, age));
        age = 30;
        ((void (*)(__block_impl *))((__block_impl *)block)->FuncPtr)((__block_impl *)block);
    }
    return 0;
}

{% endhighlight %}


- 可以看出Block体被单独抽出来成为一个C函数。并且调用的时候是指传递。所以就解释了改变不了age变量的值

{% highlight ruby %}

int main(int argc, char * argv[]) {
    @autoreleasepool {
        __weak int age = 10;
        void (^block)() = ^{
            printf("%d",age);
        };
        age  = 30;
        block();//30
    }
    return 0;
}

{% endhighlight %}

* clang -rewrite-objc main.m之后

{% highlight ruby %}

static void __main_block_func_0(struct __main_block_impl_0 *__cself) {
 	__Block_byref_age_0 *age = __cself->age; // bound by ref
          (age->__forwarding->age)+=1;
   }


int main(int argc, char * argv[]) {
  /* @autoreleasepool */ { __AtAutoreleasePool __autoreleasepool; 
      __attribute__((__blocks__(byref))) __Block_byref_age_0 age = {(void*)0,(__Block_byref_age_0 *)&age, 0, sizeof(__Block_byref_age_0), 10};
      void (*block)() = ((void (*)())&__main_block_impl_0((void *)__main_block_func_0, &__main_block_desc_0_DATA, (__Block_byref_age_0 *)&age, 570425344));
      (age.__forwarding->age) = 30;
      ((void (*)(__block_impl *))((__block_impl *)block)->FuncPtr)((__block_impl *)block);
  }
  return 0;
}

{% endhighlight %}


- 此时调用的时候是引用传递。所以就可以改变age变量的值



