//
//  main.cpp
//  JSTest
//
//  Created by 陈鹰 on 14-4-10.
//  Copyright (c) 2014年 chen ying. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "jsapi.h"

static JSBool ccLog(JSContext *cx, uint32_t argc, jsval *vp)
{
    std::cout<<"TC去你大爷的啊"<<std::endl;
    return 0;
}

static JSBool TCMyFunction(JSContext *cx, uint32_t argc, jsval *vp)
{
    std::cout<<"TC哈哈哈哈哈"<<std::endl;
    return 0;
}

static JSFunctionSpec myjs_global_functions[] = {
    JS_FS("system", ccLog, 1, 0),
    JS_FS("TCMyFunction", TCMyFunction, 1, 0),
    JS_FS_END
};




/* The class of the global object. */
static JSClass global_class = { "global",
    JSCLASS_NEW_RESOLVE | JSCLASS_GLOBAL_FLAGS,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_StrictPropertyStub,
    JS_EnumerateStub,
    JS_ResolveStub,
    JS_ConvertStub,
    nullptr,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

/* The error reporter callback. */
void reportError(JSContext *cx, const char *message, JSErrorReport *report) {
    fprintf(stderr, "%s:%u:%s\n",
            report->filename ? report->filename : "[no filename]",
            (unsigned int) report->lineno,
            message);
}

int run(JSContext *cx) {
    /* Enter a request before running anything in the context */
    JSAutoRequest ar(cx);
    
    /* Create the global object in a new compartment. */
    JSObject *global = JS_NewGlobalObject(cx, &global_class, nullptr);
    if (!global)
        return 1;
    
    /* Set the context's global */
    JSAutoCompartment ac(cx, global);
    JS_SetGlobalObject(cx, global);
    
    /* Populate the global object with the standard globals, like Object and Array. */
    if (!JS_InitStandardClasses(cx, global))
        return 1;
    
    if (!JS_DefineFunctions(cx, global, myjs_global_functions))
        return false;

    /* Your application code here. This may include JSAPI calls to create your own custom JS objects and run scripts. */
//    std::string script = "var today = Date(); today.toString();  system(/""echo hello world/"");";
    std::string script = "var today = Date(); today.toString();  TCMyFunction(/""echo hello world/"");";
    jsval rval;
    uint lineno = 0;
    JSBool ok = JS_EvaluateScript(cx, global, script.c_str(), script.length(), nullptr, lineno, &rval);

    return 0;
}

int main(int argc, const char *argv[]) {
    /* Initialize the JS engine -- new/required as of SpiderMonkey 31. */
    if (!JS_Init(0))
        return 1;
    
    /* Create a JS runtime. */
    JSRuntime *rt = JS_NewRuntime(8L * 1024L * 1024L);
//    JSRuntime *rt = JS_NewRuntime(8L * 1024L * 1024L, JS_NO_HELPER_THREADS);
    if (!rt)
        return 1;
    
    /* Create a context. */
    JSContext *cx = JS_NewContext(rt, 8192);
    if (!cx)
        return 1;
    JS_SetOptions(cx, JSOPTION_VAROBJFIX);
    JS_SetErrorReporter(cx, reportError);
    
    int status = run(cx);
    
    JS_DestroyContext(cx);
    JS_DestroyRuntime(rt);
    
    /* Shut down the JS engine. */
    JS_ShutDown();
    
    return status;
}