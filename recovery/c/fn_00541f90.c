/* Preferred-image VA 00541f90, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_00540a50(u32 club, u32 direction, u32 require_flag);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_CDECL cm_fn_00541f90(u32 club, volatile u32 output, volatile u32 extended)
{
    u8 work[0x204];
    u32 flag, pointer, result, source, item, total, index, destination, value, version;
    u8 count, cursor, limit;
    if (club == 0) {
        cm_fn_00944cff(0x009b5ac8u, 0, 0, work + 0x104, work + 4);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x104, work + 4, 0x45c7u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    flag = extended;
    limit = flag == 0 ? 50 : 100;
    result = cm_fn_00540a50(club, 0, 1);
    D((u32)work) = result;
    if (result == 0) {
        destination = output;
        source = club + 0xd7u;
        for (index = 0; index < 50; ++index) {
            value = D(source);
            D(destination + index * 4u) = value;
            source += 4u;
        }
        if (flag != 0) {
            for (index = 0; index < 50; ++index)
                D(destination + 200u + index * 4u) = 0;
        }
        return 1;
    }
    pointer = output;
    count = 0;
    source = club + 0xd7u;
    for (index = 50; index != 0; --index) {
        item = D(source);
        if (item != 0) {
            total = D(0x00ae2364u);
            if ((s32)D(item) < (s32)total) {
                destination = pointer + (u32)(s32)(s8)count * 4u;
                count = (u8)(count + 1u);
                D(destination) = item;
            }
        }
        source += 4u;
    }
    source = D((u32)work);
    cursor = 0;
    while ((s8)count < (s8)limit) {
        item = D(source + (u32)(s32)(s8)cursor * 4u + 0xd7u);
        if (item != 0) {
            total = D(0x00ae2364u);
            if ((s32)D(item) < (s32)total) {
                destination = pointer + (u32)(s32)(s8)count * 4u;
                count = (u8)(count + 1u);
                D(destination) = item;
            }
        }
        cursor = (u8)(cursor + 1u);
        if ((s8)cursor >= 50)
            break;
    }
    if ((s8)count < (s8)limit) {
        destination = pointer + (u32)(s32)(s8)count * 4u;
        total = (u32)((s32)(s8)limit - (s32)(s8)count);
        while (total != 0) {
            D(destination) = 0;
            destination += 4u;
            --total;
        }
    }
    return 1;
}
