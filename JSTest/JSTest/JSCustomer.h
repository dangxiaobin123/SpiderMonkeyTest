/**
 * JSCustomer.h - Example for my tutorial : Scripting C++ with JavaScript
 * (c) 2002 - Franky Braem
 * http://www.braem17.yucom.be
 */

#ifndef _JSCustomer_H
#define _JSCustomer_H

#include "Customer.h"

class JSCustomer
{
public:
	/**
	 * Constructor
	 */
	JSCustomer() : m_pCustomer(NULL)
	{
	}
	
	/**
	 * Destructor
	 */
	virtual ~JSCustomer()
	{
		delete m_pCustomer;
		m_pCustomer = NULL;
	}

	/**
	 * JSGetProperty - Callback for retrieving properties
	 */
	static JSBool JSGetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

	/**
	 * JSSetProperty - Callback for setting properties
	 */
	static JSBool JSSetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

	/**
	 * JSConstructor - Callback for when a wxCustomer object is created
	 */
	static JSBool JSConstructor(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	
	/**
	 * JSDestructor - Callback for when a wxCustomer object is destroyed
	 */
	static void JSDestructor(JSContext *cx, JSObject *obj);

	/**
	 * JSInit - Create a prototype for wxCustomer
	 */
	static JSObject* JSInit(JSContext *cx, JSObject *obj, JSObject *proto = NULL);

    static JSBool computeReduction(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

	static JSClass Customer_class;

    void setCustomer(Customer *customer)
	{
		m_pCustomer = customer; 
	}
    
	Customer* getCustomer() 
	{
		return m_pCustomer; 
	}

protected:

private:

	Customer *m_pCustomer;

    static JSPropertySpec Customer_properties[];
	static JSFunctionSpec Customer_methods[];

	enum
	{
		name_prop,
		age_prop
	};
};

#endif //_JSCustomer_H
