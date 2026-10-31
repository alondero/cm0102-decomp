/* WIP: original constructor paths reach coverage 0.4763 with eight fixtures;
   128 seed variants reached 0.4116. Both builds matched observed outcomes,
   but neither met the 0.6 coverage gate. Needs a late-row original fixture. */
#include "cmre.h"

/* Preferred-image VA 0x005a90b0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The constructor caller discards EAX. Each row below is the argument set
   from its original allocation/constructor block, in original order. */
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 *CM_THISCALL cm_fn_0092fe80(u32 *, u32, u32, u32, u32, u32,
                                     u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00766e10(void *, u32, u32, u32, u32, u32,
                                    u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008af520(void *, u32, u32, u32, u32, u32,
                                    u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))

typedef struct { u32 prev, handler, state; } seh_record;
typedef struct {
    u32 size, helper, label, first, second, nation, extra, flag;
} award_row;

static const award_row rows[14] = {
    {0x2b, 0, 0x009cfdcc, 3000, 1500, 0,          0, 1},
    {0x2b, 0, 0x009cfdd0, 1500, 1000, 0,          0, 1},
    {0x2b, 0, 0x009cfdd4, 1000, 500,  0,          0, 1},
    {0x2b, 0, 0x009cfdd8, 2000, 1000, 0,          0, 0},
    {0x2b, 0, 0x009cfddc, 2500, 1250, 0x009cf7e8, 1, 1},
    {0xa2, 1, 0x009cfde0, 1000, 500,  0x009cf7e8, 0, 1},
    {0xa2, 1, 0x009cff80, 1000, 500,  0x009cf7e8, 0, 0},
    {0x25d,2, 0x009cfde4, 0,    0,    0x009cf7e8, 0, 1},
    {0x25d,2, 0x009cfde8, 0,    0,    0x009cf7e8, 0, 1},
    {0x2b, 0, 0x009cfdec, 1200, 600,  0x009cf7f8, 1, 1},
    {0xa2, 1, 0x009cff7c, 500,  200,  0x009cf7f8, 0, 1},
    {0xa2, 1, 0x009cff84, 500,  200,  0x009cf7f8, 0, 0},
    {0x25d,2, 0x009cfdf0, 0,    0,    0x009cf7f8, 0, 1},
    {0x25d,2, 0x009cfdf4, 0,    0,    0x009cf7f8, 0, 1}
};

void CM_THISCALL cm_fn_005a90b0(u32 self, u32 argument)
{
    volatile seh_record frame;
    u8 scratch[0x20c];
    u16 year_low;
    u32 owner, table, allocated, result, label, nation, year = 0;
    u32 count = 0, index, row, restore;
    char *version;
    const award_row *spec;

    frame.state = 0xffffffffu;
    frame.handler = 0x00959534u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    year_low = W(0x00ae2c92u);
    owner = D(self + 0x13u);
    D((u32)scratch + 4u) = self;
    W((u32)scratch + 8u) = year_low;
    W(self + 8u) = 14;
    table = (u32)cm_fn_00944e46(0x38);
    D(self + 4u) = table;
    if (table == 0) {
        cm_fn_00944cff(0x009cc0fcu, 0, 0, scratch + 0x10c, scratch + 12);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version,
                      scratch + 0x10c, scratch + 12, 0x79);
        cm_fn_005e8290((const char *)0x009870e0u,
                      (const char *)0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        goto done;
    }
    for (row = 0; row < 14; ++row) {
        spec = &rows[row];
        allocated = (u32)cm_fn_00944cf1(spec->size);
        D((u32)scratch) = allocated;
        /* VA 0x005a9336 loads a DWORD after only its low WORD was stored.
           Preserve the uninitialized upper half and cache it for later rows. */
        if (row == 5) year = D((u32)scratch + 8u);
        frame.state = row;
        result = 0;
        if (allocated != 0) {
            nation = spec->nation != 0 ? D(spec->nation) : 0xffffffffu;
            label = D(spec->label);
            index = count;
            if (row != 13) ++count;
            if (spec->helper == 0) {
                result = (u32)cm_fn_0092fe80((u32 *)allocated,
                    owner, index, label, spec->first, spec->second,
                    nation, 0, 0, spec->extra, argument, spec->flag);
            } else if (spec->helper == 1) {
                result = cm_fn_00766e10((void *)allocated,
                    owner, index, label, spec->first, spec->second,
                    nation, 4, year, 9, year, argument, spec->flag);
            } else {
                result = cm_fn_008af520((void *)allocated,
                    owner, index, label, 0, 0, nation, 0, 0, argument, spec->flag);
            }
        }
        if (row < 5 || row == 12) {
            table = D(self + 4u);
            frame.state = 0xffffffffu;
        } else if (row < 12) {
            self = D((u32)scratch + 4u);
            frame.state = 0xffffffffu;
            table = D(self + 4u);
        } else {
            table = D(self + 4u);
        }
        D(table + row * 4u) = result;
    }
done:
    restore = frame.prev;
    CM_FS(u32, 0) = restore;
}
