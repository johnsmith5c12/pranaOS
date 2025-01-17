/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes 
#include <base/EnumBits.h>
#include <base/Types.h>

#ifdef KERNEL
#    include <libc/limits.h>
#else
#    include <limits.h>
#endif

struct [[gnu::packed]] InodeWatcherEvent {
    enum class Type : u32 {
        Invalid = 0,
        MetadataModified = 1 << 0,
        ContentModified = 1 << 1,
        Deleted = 1 << 2,
        ChildCreated = 1 << 3,
        ChildDeleted = 1 << 4,
    };

    int watch_descriptor { 0 };
    Type type { Type::Invalid };
    size_t name_length { 0 };
    const char name[];
};

BASE_ENUM_BITWISE_OPERATORS(InodeWatcherEvent::Type);

constexpr unsigned MAXIMUM_EVENT_SIZE = sizeof(InodeWatcherEvent) + NAME_MAX + 1;