//
//  der_number.c
//  utilities
//
//  Created by Mitch Adler on 6/19/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//


#include <stdio.h>

#include "utilities/SecCFRelease.h"
#include "utilities/der_plist.h"
#include "utilities/der_plist_internal.h"

#include <corecrypto/ccder.h>
#include <CoreFoundation/CoreFoundation.h>


const uint8_t* der_decode_number(CFAllocatorRef allocator, CFOptionFlags mutability,
                                 CFNumberRef* number, CFErrorRef *error,
                                 const uint8_t* der, const uint8_t *der_end)
{
    if (NULL == der)
        return NULL;

    size_t payload_size = 0;
    const uint8_t *payload = ccder_decode_tl(CCDER_INTEGER, &payload_size, der, der_end);

    if (NULL == payload || (der_end - payload) < payload_size) {
        SecCFDERCreateError(kSecDERErrorUnknownEncoding, CFSTR("Unknown number encoding"), NULL, error);
        return NULL;
    }
    if (payload_size > sizeof(long long)) {
        SecCFDERCreateError(kSecDERErrorUnsupportedNumberType, CFSTR("Number too large"), NULL, error);
        return NULL;

    }

    long long value = 0;
    
    if (payload_size > 0) {
        if ((*payload & 0x80) == 0x80)
            value = -1; // Negative integers fill with 1s so we end up negative.
        
        const uint8_t* const payload_end = payload + payload_size;
        
        for (const uint8_t *payload_byte = payload;
             payload_byte < payload_end;
             ++payload_byte) {
            value <<= 8;
            value |= *payload_byte;
        }
    }

    *number = CFNumberCreate(allocator, kCFNumberLongLongType, &value);
    
    if (*number == NULL) {
        SecCFDERCreateError(kSecDERErrorAllocationFailure, CFSTR("Number allocation failed"), NULL, error);
        return NULL;
    }
    
    return payload + payload_size;
}


static inline uint8_t byte_of(size_t byteNumber, long long value)
{
    return value >> (8 * (byteNumber - 1));
}

static inline size_t bytes_when_encoded(long long value)
{
    size_t bytes_encoded = sizeof(long long);
    
    uint8_t first_byte = byte_of(bytes_encoded, value);
    
    // Skip initial 0xFFs or 0x00
    if (first_byte == 0xFF || first_byte == 0x00) {
        do {
            --bytes_encoded;
        } while (bytes_encoded > 1 && (byte_of(bytes_encoded, value) == first_byte));
        
        if ((first_byte & 0x80) != (byte_of(bytes_encoded, value) & 0x80))
            bytes_encoded += 1;
    }
    
    return bytes_encoded;
}

size_t der_sizeof_number(CFNumberRef data, CFErrorRef *error)
{
    long long value;
    if (!CFNumberGetValue(data, kCFNumberLongLongType, &value))
        return 0;
    
    return ccder_sizeof(CCDER_INTEGER, bytes_when_encoded(value));
}

uint8_t* der_encode_number(CFNumberRef number, CFErrorRef *error,
                           const uint8_t *der, uint8_t *der_end)
{
    long long value;
    if (!CFNumberGetValue(number, kCFNumberLongLongType, &value))
        return NULL;
    
    size_t first_byte_to_include = bytes_when_encoded(value);
    
    if (!der_end || der_end - der < first_byte_to_include)
        return NULL;

    // Put the bytes we should include on the end.
    for(int bytes_included = 0; bytes_included < first_byte_to_include; ++bytes_included)
    {
        --der_end;
        *der_end = value & 0xFF;
        value >>= 8;
    }

    return ccder_encode_tl(CCDER_INTEGER, first_byte_to_include, der, der_end);

}
