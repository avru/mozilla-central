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

#include "nsImgManager.h"
#include "nsImages.h"
#include "nsIDocument.h"
#include "nsIContent.h"
#include "nsCOMPtr.h"
#include "nsXPIDLString.h"
#include "nsIURI.h"

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// nsImgManager Implementation

NS_IMPL_ISUPPORTS2(nsImgManager, 
                   nsIImgManager, 
                   nsIContentPolicy);

nsImgManager::nsImgManager()
{
    NS_INIT_REFCNT();
}

nsImgManager::~nsImgManager(void)
{
}

nsresult nsImgManager::Init()
{
    IMAGE_RegisterPrefCallbacks();
    return NS_OK;
}


NS_IMETHODIMP nsImgManager::Block(const char * imageURL)
{
    ::IMAGE_Block(imageURL);
    return NS_OK;
}

// nsIContentPolicy Implementation
NS_IMETHODIMP nsImgManager::ShouldLoad(PRInt32 aContentType, 
                                       nsIURI *aContentLoc,
                                       nsISupports *aContext,
                                       nsIDOMWindow *aWindow,
                                       PRBool *_retval)
{
    *_retval = PR_TRUE;
    nsresult rv = NS_OK;

    // we can't do anything w/ out these.
    if (!aContentLoc || !aContext) return rv;

    switch (aContentType) {
        case nsIContentPolicy::IMAGE:   
        {
            nsCOMPtr<nsIURI> baseURI;
            nsCOMPtr<nsIDocument> doc;
            nsCOMPtr<nsIContent> content(do_QueryInterface(aContext));
            NS_ASSERTION(content, "no content avail");
            if (content) {
                rv = content->GetDocument(*getter_AddRefs(doc));
                if (NS_FAILED(rv) || !doc) return rv;

                rv = doc->GetBaseURL(*getter_AddRefs(baseURI));
                if (NS_FAILED(rv) || !baseURI) return rv;

                // we only care about HTTP images
                PRBool httpType = PR_TRUE;
                rv = baseURI->SchemeIs("http", &httpType);
                if (NS_FAILED(rv) || !httpType) {
                    // check HTTPS as well
                    rv = baseURI->SchemeIs("https", &httpType);
                    if (NS_FAILED(rv) || !httpType) return rv;                    
                }

                rv = aContentLoc->SchemeIs("http", &httpType);
                if (NS_FAILED(rv) || !httpType) {
                    // check HTTPS as well
                    rv = aContentLoc->SchemeIs("https", &httpType);
                    if (NS_FAILED(rv) || !httpType) return rv;                    
                }

                nsXPIDLCString baseHost;
                rv = baseURI->GetHost(getter_Copies(baseHost));
                if (NS_FAILED(rv) || !baseHost) return rv;

                nsXPIDLCString host;
                rv = aContentLoc->GetHost(getter_Copies(host));
                if (NS_FAILED(rv) || !host) return rv;

                return ::IMAGE_CheckForPermission(host, baseHost,
                                                  _retval);
            }
        }
        break;
    }
    return NS_OK;
}

NS_IMETHODIMP nsImgManager::ShouldProcess(PRInt32 aContentType,
                                          nsIURI *aDocumentLoc,
                                          nsISupports *aContext,
                                          nsIDOMWindow *aWindow,
                                          PRBool *_retval) {
  return NS_ERROR_NOT_IMPLEMENTED;
}
