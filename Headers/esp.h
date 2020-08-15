#ifndef __ESP_H__
#define __ESP_H__

#include <sandbox/private.h>
#include <xpc/private.h>

int __esp_enabled();
int __esp_check(char *key, xpc_object_t dict);

//From https://github.com/darlinghq/darling-security/blob/0db0acec470cea7b66bfd75838e0c136d20a286c/include/esp.h
#define __esp_check_ns(a, b) -1
#define __esp_notify_ns(a, b)

#endif