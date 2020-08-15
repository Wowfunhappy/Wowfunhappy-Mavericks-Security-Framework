#ifndef _LIB_AKS_H_
#define _LIB_AKS_H_

#include <IOKit/IOReturn.h>

enum keybag_state {
    keybag_state_unlocked = 0,
    keybag_state_locked = 1 << 0,
    keybag_state_no_pin = 1 << 1,
    keybag_state_been_unlocked = 1 << 2,
};
typedef uint32_t keybag_state_t;
typedef int32_t keybag_handle_t;

static kern_return_t aks_get_lock_state(keybag_handle_t handle, keybag_state_t *state) {
    if (state) *state = keybag_state_no_pin & keybag_state_been_unlocked;
    return kIOReturnSuccess;
}

#endif


//Everything below added by Wowfunhappy; trying things blindly to try to make project compile.

#define session_keybag_handle 0
#define device_keybag_handle 0

#define kAKSAssertTypeProfile 1
#define kAKSAssertTypeOther 2
typedef int32_t AKSAssertionType_t;
extern kern_return_t aks_assert_hold(keybag_handle_t keybagHandle, AKSAssertionType_t lockAssertType, uint64_t timeout);
extern kern_return_t aks_assert_drop(keybag_handle_t keybagHandle, AKSAssertionType_t lockAssertType);