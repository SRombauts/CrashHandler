/**
 * @file    Format.c
 * @ingroup CrashHandler
 * @brief   Basic integer to string formatter.
 *
 * Copyright (c) 2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
*/

#include "Format.h"

void reverseString(char* aBuffer, size_t aSize) {
    char* p;
    char* q;
    char c;
    for (p = aBuffer, q = p + (aSize-1); p < q; ++p, --q) {
        c = *p, *p = *q, *q = c;
    }
}

/// signal-safe integer to array formatter
void formatInteger(const int aValue, char* aBuffer, size_t aBufferSize) {
    static const char digits[] = "0123456789";
    size_t Size = 0;
    size_t MaxSize;
    unsigned int Value;

    if (aValue < 0) {
        Value = -aValue;
        MaxSize = aBufferSize - 2;
    } else {
        Value = aValue;
        MaxSize = aBufferSize - 1;
    }

    // Format integer to string in reverse order
    do {
        aBuffer[Size++] = digits[Value % 10];
        Value /= 10;
    } while ((Value > 0) && (Size < MaxSize));

    // Add sign and null terminate string
    if (aValue < 0)
    {
        aBuffer[Size++] = '-';
    }
    aBuffer[Size] = '\0';
 
    // Reverse string
    reverseString(aBuffer, Size);
}
