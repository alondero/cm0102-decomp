#include "cmre.h"

/* Preferred-image VA 0x00718aa0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Slot zero and slot fourteen are read at their original points in the ordered
   setter sequence. The report callback receives the original explicit ECX. */
extern u32 CM_THISCALL cm_fn_007fe4a0(u32, u32, u32, u32, u32, u32, u32);
extern void CM_THISCALL cm_fn_007ff130(u32, u32, u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007ff270(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_006b5260(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_CDECL cm_fn_00718aa0(u32 token, u32 report)
{
    static const struct { u32 slot, value; } setters[42] = {
        {0,0}, {1,1}, {2,1}, {3,0xffffffffu}, {4,0xffffffffu},
        {5,0xffffffffu}, {6,0xffffffffu}, {7,0}, {8,0xffffffffu},
        {9,0xffffffffu}, {10,3}, {11,0xffffffffu}, {12,0}, {13,0xffffffffu},
        {14,0}, {15,0}, {16,0xffffffffu}, {17,1}, {18,0}, {19,1},
        {20,0}, {21,0}, {22,0}, {23,0}, {24,0}, {25,0}, {26,0xffffffffu},
        {27,0}, {28,1}, {29,0}, {33,0xffffffffu}, {34,0}, {35,0xffffffffu},
        {36,0}, {37,0}, {38,0}, {39,0}, {40,0}, {41,0}, {45,0}, {47,0}, {48,0}
    };
    u8 scratch[512];
    u32 eax, i, value, index, object, slot, receiver;
    u8 count;
    char *version;
    if (!token || !report) {
        cm_fn_00944cff(0x00a161d4, 0, 0, scratch + 256, scratch);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, scratch + 256, scratch, 0x24b);
        eax = cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return eax;
    }
    eax = cm_fn_007fe4a0(0x00b74450, token, 0x00718e90, 0x0071a240, 1, 0x0071b120, 0);
    if (!eax) return eax;
    for (i = 0; i < 42; ++i) {
        slot = setters[i].slot;
        if (!slot) value = report;
        else if (slot == 14) value = D(report + 0x63);
        else value = setters[i].value;
        cm_fn_007ff130(0x00b74450, token, slot, value, 0);
    }
    index = (u32)(s32)(s16)W(0x00b77c7a);
    object = D(report + 0x5b);
    slot = object + index * 4u + 0xc5;
    value = D(slot);
    if (!value) {
        D(slot) = 1;
        count = B(object + 0x108);
        ++count;
        B(object + 0x108) = count;
    }
    eax = D(report + 0x57);
    if (eax) return eax;
    receiver = D(0x00b67a50);
    cm_fn_006b5260(receiver, report);
    eax = D(report + 0x57);
    if (!eax) return eax;
    return (u32)cm_fn_007ff270(0x00b74450, 1, 2, 0);
}
