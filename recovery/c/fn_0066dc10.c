/* Preferred-image VA 0x0066dc10, base 0x00400000; primary executable SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Both direct callers test full EAX (0066d8af, 0066df45). */
#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_004afd80(u32 record, u8 selection);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory,
                                  void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(const char *title, const char *message,
                                   s32 flags);

u32 CM_THISCALL cm_fn_0066dc10(u32 self)
{
    u8 scratch[0x200];
    u32 allocation, selector, base, result, records, first, second;
    u32 count, index, data, needle, cursor;

    CM_GLOBAL(volatile u16, self + 0x3eu) = 2;
    allocation = (u32)cm_fn_00944e46(0xcu);
    CM_GLOBAL(volatile u32, self + 0xa7u) = allocation;
    if (allocation == 0) {
        u32 text;
        cm_fn_00944cff(0x009eb198u, 0, 0, scratch + 0x100, scratch);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                       scratch + 0x100, scratch, 0xc1u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        return 0xffffffffu;
    }

    selector = CM_GLOBAL(volatile u32, 0x009cfa00u);
    base = CM_GLOBAL(volatile u32, 0x00ae23d0u);
    result = cm_fn_004afd80(base + selector * 0x6bu, 0);
    records = CM_GLOBAL(volatile u32, self + 0xa7u);
    CM_GLOBAL(volatile u32, records) = result;
    records = CM_GLOBAL(volatile u32, self + 0xa7u);
    if (CM_GLOBAL(volatile u32, records) == 0) {
        count = CM_GLOBAL(volatile u32, 0x00ae235cu);
        index = 0;
        if ((s32)count > 0) {
            data = CM_GLOBAL(volatile u32, self + 4u);
            base = CM_GLOBAL(volatile u32, 0x00ae23b4u);
            needle = CM_GLOBAL(volatile u32, data + 0x5du);
            cursor = base + 0x53u;
            do {
                if (CM_GLOBAL(volatile u32, cursor) == needle) {
                    CM_GLOBAL(volatile u32, records) = base + index * 0x245u;
                    break;
                }
                ++index;
                cursor += 0x245u;
            } while ((s32)index < (s32)count);
        }
    }

    selector = CM_GLOBAL(volatile u32, 0x009cfa04u);
    base = CM_GLOBAL(volatile u32, 0x00ae23d0u);
    result = cm_fn_004afd80(base + selector * 0x6bu, 0);
    records = CM_GLOBAL(volatile u32, self + 0xa7u);
    CM_GLOBAL(volatile u32, records + 6u) = result;
    records = CM_GLOBAL(volatile u32, self + 0xa7u);
    if (CM_GLOBAL(volatile u32, records + 6u) == 0) {
        count = CM_GLOBAL(volatile u32, 0x00ae235cu);
        index = 0;
        if ((s32)count > 0) {
            data = CM_GLOBAL(volatile u32, self + 4u);
            needle = CM_GLOBAL(volatile u32, data + 0x5du);
            cursor = CM_GLOBAL(volatile u32, 0x00ae23b4u) + 0x53u;
            do {
                if (CM_GLOBAL(volatile u32, cursor) == needle) {
                    data = CM_GLOBAL(volatile u32, records);
                    if (index != CM_GLOBAL(volatile u32, data)) {
                        base = CM_GLOBAL(volatile u32, 0x00ae23b4u);
                        CM_GLOBAL(volatile u32, records + 6u) = base + index * 0x245u;
                        break;
                    }
                }
                ++index;
                cursor += 0x245u;
            } while ((s32)index < (s32)count);
        }
    }

    records = CM_GLOBAL(volatile u32, self + 0xa7u);
    first = CM_GLOBAL(volatile u32, records);
    second = CM_GLOBAL(volatile u32, records + 6u);
    if (first == second) {
        selector = CM_GLOBAL(volatile u32, 0x009cfa04u);
        base = CM_GLOBAL(volatile u32, 0x00ae23d0u);
        result = cm_fn_004afd80(base + selector * 0x6bu, 1);
        records = CM_GLOBAL(volatile u32, self + 0xa7u);
        CM_GLOBAL(volatile u32, records + 6u) = result;
        records = CM_GLOBAL(volatile u32, self + 0xa7u);
        if (CM_GLOBAL(volatile u32, records + 6u) == 0) {
            u32 text;
            /* The original swaps the two buffer roles on this error path. */
            cm_fn_00944cff(0x009eb198u, 0, 0, scratch, scratch + 0x100);
            text = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                           scratch, scratch + 0x100, 0xefu);
            cm_fn_005e8290((const char *)0x009870e0u,
                           (const char *)0x00ae24d0u, 0);
            CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
            return 0;
        }
    }

    CM_GLOBAL(volatile u8, records + 4u) = 0;
    records = CM_GLOBAL(volatile u32, self + 0xa7u);
    CM_GLOBAL(volatile u8, records + 5u) = 0;
    records = CM_GLOBAL(volatile u32, self + 0xa7u);
    CM_GLOBAL(volatile u8, records + 0xau) = 0;
    records = CM_GLOBAL(volatile u32, self + 0xa7u);
    CM_GLOBAL(volatile u8, records + 0xbu) = 0;
    return 1;
}
