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

#include "nsIServiceManager.h"
#include "nsCookieManager.h"
#include "nsCRT.h"
#include "nsCookies.h"
#include "nsCookie.h"
#include "nsIGenericFactory.h"
#include "nsXPIDLString.h"
#include "nsIScriptGlobalObject.h"

////////////////////////////////////////////////////////////////////////////////

class nsCookieEnumerator : public nsISimpleEnumerator
{
    public:

        NS_DECL_ISUPPORTS

        nsCookieEnumerator() : mCookieCount(0) 
        {
            NS_INIT_REFCNT();
        }

        NS_IMETHOD HasMoreElements(PRBool *result) 
        {
            *result = COOKIE_Count() > mCookieCount;
            return NS_OK;
        }

        NS_IMETHOD GetNext(nsISupports **result) 
        {
          char *name;
          char *value;
          PRBool isDomain;
          char * host;
          char * path;
          PRBool isSecure;
          PRUint64 expires;
          nsresult rv = COOKIE_Enumerate
            (mCookieCount++, &name, &value, &isDomain, &host, &path, &isSecure, &expires);
          if (NS_SUCCEEDED(rv)) {
            nsICookie *cookie =
              new nsCookie(name, value, isDomain, host, path, isSecure, expires);
            *result = cookie;
            NS_ADDREF(*result);
          } else {
            *result = nsnull;
          }
          return rv;
        }

        virtual ~nsCookieEnumerator() 
        {
        }

    protected:
        PRInt32 mCookieCount;
};

NS_IMPL_ISUPPORTS1(nsCookieEnumerator, nsISimpleEnumerator);


////////////////////////////////////////////////////////////////////////////////
// nsCookieManager Implementation

NS_IMPL_ISUPPORTS2(nsCookieManager, nsICookieManager, nsISupportsWeakReference);

nsCookieManager::nsCookieManager()
{
  NS_INIT_REFCNT();
}

nsCookieManager::~nsCookieManager(void)
{
}

nsresult nsCookieManager::Init()
{
  COOKIE_Read();
  return NS_OK;
}

NS_IMETHODIMP nsCookieManager::RemoveAll(void) {
  ::COOKIE_RemoveAll();
  return NS_OK;
}

NS_IMETHODIMP nsCookieManager::GetEnumerator(nsISimpleEnumerator * *entries)
{
    *entries = nsnull;

    nsCookieEnumerator* cookieEnum = new nsCookieEnumerator();
    if (cookieEnum == nsnull)
        return NS_ERROR_OUT_OF_MEMORY;
    NS_ADDREF(cookieEnum);
    *entries = cookieEnum;
    return NS_OK;
}

NS_IMETHODIMP nsCookieManager::Remove
  (const char* host, const char* name, const char* path, const PRBool permanent) {
  ::COOKIE_Remove(host, name, path, permanent);
  return NS_OK;
}
