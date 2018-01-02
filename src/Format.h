/**
 * @file    Format.h
 * @ingroup CrashHandler
 * @brief   Basic integer to string formatter.
 *
 * Copyright (c) 2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
*/
#pragma once

#include <stddef.h>

/// signal-safe integer to array formatter
void formatInteger(const int aValue, char* aBuffer, size_t aBufferSize);
