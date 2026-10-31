#include "cmre.h"

/* Preferred-image VA 0x004224e0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Keep the year comparison before the size store and reload mutable globals
   at each original loop back-edge. The result is a full zero-extended DWORD. */
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_THISCALL cm_fn_00687430(u32, u8, u32, u8, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))

u32 CM_THISCALL cm_fn_004224e0(u32 self)
{
    u8 scratch[512];
    u8 *filename, *extension;
    u32 year, total, excluded, clubs, limit, index, item, field, value, wanted;
    u32 allocation, line;
    u8 count = 0, selected;
    int overflow;
    char *version;

    year = W(self + 0x40u);
    W(self + 0x3eu) = 16;
    if (year == 2001) {
        excluded = D(0x00ae2350u);
        total = D(0x00ae235cu);
        clubs = D(0x00ae23b4u);
        limit = total - excluded * 2u;
        index = 0;
        if ((s32)limit > 0) {
            item = clubs + 0x57u;
            do {
                field = D(item);
                if (field != 0) {
                    value = D(field);
                    wanted = D(0x009cf5a4u);
                    if (value == wanted || value == D(0x009cf5a8u)) {
                        field = D(item);
                        D(item + 9u) = field;
                        field = D(self + 4u);
                        D(item) = field;
                    }
                }
                excluded = D(0x00ae2350u);
                total = D(0x00ae235cu);
                ++index;
                limit = total - excluded * 2u;
                item += 0x245u;
            } while ((s32)index < (s32)limit);
        }
    }
    value = (u32)(s32)(s16)W(self + 0x3eu);
    allocation = (u32)cm_fn_00944e46(value * 59u);
    D(self + 0xb1u) = allocation;
    if (allocation == 0) {
        filename = scratch + 256; extension = scratch;
        line = 0x22e; overflow = 0;
        goto error;
    }
    excluded = D(0x00ae2350u);
    total = D(0x00ae235cu);
    clubs = D(0x00ae23b4u);
    limit = total - excluded * 2u;
    index = 0;
    if ((s32)limit > 0) {
        do {
            field = D(clubs + 0x57u);
            if (field == 0) goto next;
            item = D(clubs + 0x60u);
            if (item == 0) goto next;
            value = D(item);
            wanted = D(0x009cf5a4u);
            if (value != wanted) goto next;
            value = D(field);
            wanted = D(0x009cf7c4u);
            if (value != wanted) goto next;
            if ((s16)(s8)count >= (s16)W(self + 0x3eu)) {
                filename = scratch; extension = scratch + 256;
                line = 0x23f; overflow = 1;
                goto error;
            }
            selected = count;
            count = (u8)(count + 1u);
            cm_fn_00687430(self, selected, clubs, 0, 0);
next:
            excluded = D(0x00ae2350u);
            total = D(0x00ae235cu);
            ++index;
            clubs += 0x245u;
            limit = total - excluded * 2u;
        } while ((s32)index < (s32)limit);
    }
    goto result;
error:
    cm_fn_00944cff(0x00989d04u, 0, 0, filename, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version,
                  filename, extension, line);
    cm_fn_005e8290((const char *)0x009870e0u,
                  (const char *)0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    if (!overflow) return 0;
    D(clubs + 0x57u) = 0;
result:
    return (s16)(s8)count == (s16)W(self + 0x3eu);
}
