/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/Prelude.h>
#include "archs/Memory.h"

namespace Arch::x86_32
{

#define PAGE_DIRECTORY_INDEX(vaddr) ((vaddr) >> 22)
#define PAGE_TABLE_INDEX(vaddr) (((vaddr) >> 12) & 0x03ff)

#define PAGE_TABLE_ENTRY_COUNT 1024
#define PAGE_DIRECTORY_ENTRY_COUNT 1024

union PACKED PageTableEntry
{
    struct PACKED
    {
        bool Present : 1;
        bool Write : 1;
        bool User : 1;
        bool PageLevelWriteThrough : 1;
        bool PageLevelCacheDisable : 1;
        bool Accessed : 1;
        bool Dirty : 1;
        bool Pat : 1;
        uint32_t Ignored : 4;
        uint32_t PageFrameNumber : 20;
    };

    uint32_t as_uint;
};

struct PACKED PageTable
{
    PageTableEntry entries[PAGE_TABLE_ENTRY_COUNT];
};


}