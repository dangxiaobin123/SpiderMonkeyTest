/**
 * JSCustomer.cpp - Example for my tutorial : Scripting C++ with JavaScript
 * (c) 2002 - Franky Braem
 * http://www.braem17.yucom.be
 */

#include <string>

#define XP_PC
#include <jsapi.h>

//#include "Customer.h"
#include "JSCustomer.h"

JSPropertySpec JSCustomer::Customer_properties[] = 
{ 
    { "name", name_prop, JSPROP_ENUMERATE },
    { "age", age_prop, JSPROP_ENUMERATE },
	{ 0 }
};

JSFunctionSpec JSCustomer::Customer_methods[] = 
{
    { "computeReduction", computeReduction, 1, 0, 0 },
	{ 0, 0, 0, 0, 0 }
};

JSClass JSCustomer::Customer_class = 
{ 
	"Customer", JSCLASS_HAS_PRIVATE, JS_PropertyStub, JS_PropertyStub,
	JSCustomer::JSGetProperty, JSCustomer::JSSetProperty,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JSCustomer::JSDestructor
};

JSBool JSCustomer::JSGetProperty(JSContext *cx, JSObject *obj, jsval id,
								 jsval *vp)
{
    if (JSVAL_IS_INT(id)) 
	{
		JSCustomer *p = (JSCustomer *) JS_GetPrivate(cx, obj);
		Customer *customer = p->getCustomer();
        switch (JSVAL_TO_INT(id)) 
		{
		case name_prop:
			{
				std::string name = customer->GetName();
				JSString *str = JS_NewStringCopyN(cx, name.c_str(), name.length());
				*vp = STRING_TO_JSVAL(str);
				break;
			}
		case age_prop:
			*vp = INT_TO_JSVAL(customer->GetAge());
			break;
        }
    }
    return JS_TRUE;
}

JSBool JSCustomer::JSSetProperty(JSContext *cx, JSObject *obj, jsval id, 
								 jsval *vp)
{
    if (JSVAL_IS_INT(id)) 
	{
		JSCustomer *p = (JSCustomer *) JS_GetPrivate(cx, obj);
		Customer *customer = p->getCustomer();

        switch (JSVAL_TO_INT(id)) 
		{
		case name_prop:
			{
				JSString *str = JS_ValueToString(cx, *vp);
				std::string name = JS_GetStringBytes(str);
				customer->SetName(name);
				break;
			}
		case age_prop:
			customer->SetAge(JSVAL_TO_INT(*vp));
			break;
		}
    }
    return JS_TRUE;
}

JSBool JSCustomer::JSConstructor(JSContext *cx, JSObject *obj, uintN argc,
								 jsval *argv, jsval *rval)
{
	JSCustomer *priv = new JSCustomer();
	priv->setCustomer(new Customer());
	JS_SetPrivate(cx, obj, (void *) priv);

	return JS_TRUE;
}

void JSCustomer::JSDestructor(JSContext *cx, JSObject *obj)
{
	JSCustomer *priv = (JSCustomer*) JS_GetPrivate(cx, obj);
	delete priv;
	priv = NULL;
}

JSObject *JSCustomer::JSInit(JSContext *cx, JSObject *obj, JSObject *proto)
{
    JSObject *newProtoObj = JS_InitClass(cx, obj, proto, &Customer_class, 
							  			 JSCustomer::JSConstructor, 0,
										 NULL, JSCustomer::Customer_methods,
										 NULL, NULL);
	JS_DefineProperties(cx, newProtoObj, JSCustomer::Customer_properties);
    return newProtoObj;
}

JSBool JSCustomer::computeReduction(JSContext *cx, JSObject *obj, uintN argc,
									jsval *argv, jsval *rval)
{
     JSCustomer *p = (JSCustomer*) JS_GetPrivate(cx, obj);
     if ( p->getCustomer()->GetAge() < 25 )
       *rval = INT_TO_JSVAL(10);
     else
       *rval = INT_TO_JSVAL(5);
     return JS_TRUE;
}

