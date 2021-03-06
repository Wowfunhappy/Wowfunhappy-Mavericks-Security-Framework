/*
 * Copyright (c) 2000-2001 Apple Computer, Inc. All Rights Reserved.
 * 
 * The contents of this file constitute Original Code as defined in and are
 * subject to the Apple Public Source License Version 1.2 (the 'License').
 * You may not use this file except in compliance with the License. Please obtain
 * a copy of the License at http://www.apple.com/publicsource and read it before
 * using this file.
 * 
 * This Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS
 * OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT. Please see the License for the
 * specific language governing rights and limitations under the License.
 */


//
// cryptkitcsp.h - AlgorithmFactory for CryptKit CSP
//

#ifdef	CRYPTKIT_CSP_ENABLE

#ifndef _H_CRYPTKITCSPLIB
#define _H_CRYPTKITCSPLIB

#include <security_cdsa_plugin/CSPsession.h>
#include "AppleCSP.h"

/* Can't include AppleCSPSession.h due to circular dependency */
class AppleCSPSession;

class CryptKitFactory : public AppleCSPAlgorithmFactory {
public:
    CryptKitFactory(Allocator *normAlloc = NULL, Allocator *privAlloc = NULL);
	~CryptKitFactory();
	
    bool setup(
		AppleCSPSession &session,
		CSPFullPluginSession::CSPContext * &cspCtx, 
		const Context &context);

    static void setNormAllocator(Allocator *alloc)
    { assert(!normAllocator); normAllocator = alloc; }
    static void setPrivAllocator(Allocator *alloc)
    { assert(!privAllocator); privAllocator = alloc; }

    // memory allocators
    static Allocator *normAllocator;
    static Allocator *privAllocator;
    
};
		
/* convenience for clients.... */
#include "CryptKitSpace.h"

#endif //_H_CRYPTKITCSPLIB
#endif	/* CRYPTKIT_CSP_ENABLE */
