/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* ----- BEGIN LICENSE BLOCK -----
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is Netscape Communications Corporation.
 * Portions created by Netscape Communications Corporation are
 * Copyright (C) 1998 Netscape Communications Corporation. All
 * Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either of the GNU General Public License Version 2 or later (the "GPL"),
 * or the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the LGPL or the GPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ----- END LICENSE BLOCK ----- */

#ifndef lcglue_h___
#define lcglue_h___

#include "prtypes.h"
#include "jni.h"
#include "jsdbgapi.h"
#include "nsError.h"

#include "nsIThreadManager.h"
#include "nsISecurityContext.h"

struct JVMSecurityStack {
  void        **pNSIPrincipaArray;
  int           numPrincipals;
  void         *pNSISecurityContext;
  JSStackFrame *pJavaToJSFrame;
  JSStackFrame *pJSToJavaFrame;
  JVMSecurityStack *next;
  JVMSecurityStack *prev;
};
typedef struct JVMSecurityStack JVMSecurityStack;

/**
 * JVMContext is maintained as thread local storage. The current thread's
 * context is accessed by calling GetJVMContext().
 */
struct JVMContext {
	JNIEnv					*proxyEnv;					/* thread local proxy JNI */
	JVMSecurityStack		*securityStack;				/* thread local security stack. */
	JSJavaThreadState		*jsj_env;					/* thread local JavaScript execution env. */
	JSContext				*js_context;				/* thread local JavaScript context. */
	JSStackFrame			*js_startframe;				/* thread local JavaScript stack frame. */
    void                    *java_applet_obj;
};

JVMContext* GetJVMContext();
void JVM_InitLCGlue(void);      // in lcglue.cpp
extern "C" void* 
ConvertNSIPrincipalToNSPrincipalArray(JNIEnv *pJNIEnv, JSContext *pJSContext, void  **ppNSIPrincipalArrayIN, int numPrincipals, void *pNSISecurityContext);

#endif /* lcglue_h___ */
