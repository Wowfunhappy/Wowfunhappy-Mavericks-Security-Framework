/*
 * Copyright (c) 2014,2015 Apple Inc. All rights reserved.
 * 
 * corecrypto Internal Use License Agreement
 * 
 * IMPORTANT:  This Apple corecrypto software is supplied to you by Apple Inc. ("Apple")
 * in consideration of your agreement to the following terms, and your download or use
 * of this Apple software constitutes acceptance of these terms.  If you do not agree
 * with these terms, please do not download or use this Apple software.
 * 
 * 1.	As used in this Agreement, the term "Apple Software" collectively means and
 * includes all of the Apple corecrypto materials provided by Apple here, including
 * but not limited to the Apple corecrypto software, frameworks, libraries, documentation
 * and other Apple-created materials. In consideration of your agreement to abide by the
 * following terms, conditioned upon your compliance with these terms and subject to
 * these terms, Apple grants you, for a period of ninety (90) days from the date you
 * download the Apple Software, a limited, non-exclusive, non-sublicensable license
 * under Apple’s copyrights in the Apple Software to make a reasonable number of copies
 * of, compile, and run the Apple Software internally within your organization only on
 * devices and computers you own or control, for the sole purpose of verifying the
 * security characteristics and correct functioning of the Apple Software; provided
 * that you must retain this notice and the following text and disclaimers in all
 * copies of the Apple Software that you make. You may not, directly or indirectly,
 * redistribute the Apple Software or any portions thereof. The Apple Software is only
 * licensed and intended for use as expressly stated above and may not be used for other
 * purposes or in other contexts without Apple's prior written permission.  Except as
 * expressly stated in this notice, no other rights or licenses, express or implied, are
 * granted by Apple herein.
 * 
 * 2.	The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED WARRANTIES
 * OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, REGARDING
 * THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS,
 * SYSTEMS, OR SERVICES. APPLE DOES NOT WARRANT THAT THE APPLE SOFTWARE WILL MEET YOUR
 * REQUIREMENTS, THAT THE OPERATION OF THE APPLE SOFTWARE WILL BE UNINTERRUPTED OR
 * ERROR-FREE, THAT DEFECTS IN THE APPLE SOFTWARE WILL BE CORRECTED, OR THAT THE APPLE
 * SOFTWARE WILL BE COMPATIBLE WITH FUTURE APPLE PRODUCTS, SOFTWARE OR SERVICES. NO ORAL
 * OR WRITTEN INFORMATION OR ADVICE GIVEN BY APPLE OR AN APPLE AUTHORIZED REPRESENTATIVE
 * WILL CREATE A WARRANTY. 
 * 
 * 3.	IN NO EVENT SHALL APPLE BE LIABLE FOR ANY DIRECT, SPECIAL, INDIRECT, INCIDENTAL
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) ARISING
 * IN ANY WAY OUT OF THE USE, REPRODUCTION, COMPILATION OR OPERATION OF THE APPLE
 * SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING
 * NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 * 4.	This Agreement is effective until terminated. Your rights under this Agreement will
 * terminate automatically without notice from Apple if you fail to comply with any term(s)
 * of this Agreement.  Upon termination, you agree to cease all use of the Apple Software
 * and destroy all copies, full or partial, of the Apple Software. This Agreement will be
 * governed and construed in accordance with the laws of the State of California, without
 * regard to its choice of law rules.
 * 
 * You may report security issues about Apple products to product-security@apple.com,
 * as described here:  https://www.apple.com/support/security/.  Non-security bugs and
 * enhancement requests can be made via https://bugreport.apple.com as described
 * here: https://developer.apple.com/bug-reporting/
 *
 * EA1350 
 * 10/5/15
 */


#ifndef corecrypto_cc_memory_h
#define corecrypto_cc_memory_h

// =============================================================================
//   Declare workspace in HEAP
// =============================================================================
#if CC_USE_HEAP_FOR_WORKSPACE

#include <stdlib.h>

// Malloc/free functions to be used
CC_INLINE void	*cc_malloc(size_t s)
{
    void	*p=malloc(s);
    cc_assert(p!=NULL);
    /* Propagating the error is not always possible so 
     we just don't allow allocation failure */
    if (p==NULL) abort();
    return p;
}
CC_INLINE void	 cc_free(void *p)
{
    free(p);
}

#define CC_DECL_WORKSPACE(ws,n) \
    cc_unit* tmp_##__FILE__##__LINE__=cc_malloc(ccn_sizeof_n((n))); \
    cc_ws _ws={&tmp_##__FILE__##__LINE__[0],&tmp_##__FILE__##__LINE__[(n)]}; \
    cc_ws_t ws=&_ws;

// Free and reset pointers to avoid future reference
#define CC_FREE_WORKSPACE(ws) \
    cc_free(ws->start); \
    ws->start=NULL;     \
    ws->end=NULL;

// =============================================================================
//   Declare workspace in STACK
// =============================================================================

#else  // not CC_USE_HEAP_FOR_WORKSPACE

// Declare a variable in stack and use its address
// Could use alloca but alloca is not so portable
#define CC_DECL_WORKSPACE(ws,n) \
    cc_unit tmp_##__FILE__##__LINE__[(n)]; \
    cc_ws _ws={&tmp_##__FILE__##__LINE__[0],&tmp_##__FILE__##__LINE__[(n)]}; \
    cc_ws_t ws=&_ws;

// Reset pointers to avoid future reference
#define CC_FREE_WORKSPACE(_ws) \
    ws->start=NULL;     \
    ws->end=NULL;

#endif  // CC_USE_HEAP_FOR_WORKSPACE

// =============================================================================
//   Common
// =============================================================================

#define CC_CLEAR_AND_FREE_WORKSPACE(ws) { \
    assert(ws->start<ws->end); \
    cc_clear(ws->end-ws->start,ws->start); \
    CC_FREE_WORKSPACE(ws);}

#endif // corecrypto_cc_memory_h
