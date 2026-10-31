#include "cmre.h"

/* Preferred-image VA 0x00844300, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preserve EAX above AL, the signed attribute tests, and the target-table read. */
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, s8, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_STDCALL cm_fn_00844300(u32 person, s8 category)
{
    u8 scratch[0x200];
    u32 eax, index, target, other;
    s8 attribute;

    eax = D(person + 0x61);
    if (!eax && category != 15) goto no;
    index = (u32)(s32)category;
    if (index > 15) {
        cm_fn_00944cff(0x00a825a4, 0, 0, scratch + 256, scratch);
        eax = (u32)cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, eax,
                      (u32)scratch + 256, (u32)scratch, 0x1c3f);
        eax = cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        goto no;
    }
    target = D(0x008444f4 + index * 4u);
    switch (target) {
    case 0x0084433b:
        attribute = (s8)B(eax + 0xf);
        goto test_attribute;
    case 0x00844343:
        if ((s8)B(eax + 0x10) < 15) goto no;
        attribute = (s8)B(eax + 0x19);
        goto test_attribute;
    case 0x00844354:
        if ((s8)B(eax + 0x11) < 15) goto no;
        attribute = (s8)B(eax + 0x18);
        goto test_attribute;
    case 0x00844365:
        if ((s8)B(eax + 0x11) < 15) goto no;
        attribute = (s8)B(eax + 0x19);
        goto test_attribute;
    case 0x00844376:
        attribute = (s8)B(eax + 0x11);
        goto test_central;
    case 0x0084437e:
        if ((s8)B(eax + 0x16) < 15) goto no;
        attribute = (s8)B(eax + 0x18);
        goto test_attribute;
    case 0x0084438c:
        if ((s8)B(eax + 0x12) < 15) goto no;
        attribute = (s8)B(eax + 0x19);
        goto test_attribute;
    case 0x0084439a:
        attribute = (s8)B(eax + 0x16);
        goto test_central;
    case 0x0084439f:
        if ((s8)B(eax + 0x13) < 15) goto no;
        attribute = (s8)B(eax + 0x18);
        goto test_attribute;
    case 0x008443ad:
        if ((s8)B(eax + 0x13) < 15) goto no;
        attribute = (s8)B(eax + 0x19);
        goto test_attribute;
    case 0x008443bb:
        attribute = (s8)B(eax + 0x13);
        goto test_central;
    case 0x008443c0:
        if ((s8)B(eax + 0x14) < 15) goto no;
        if ((s8)B(eax + 0x18) < 15) goto no;
        goto yes;
    case 0x008443de:
        if ((s8)B(eax + 0x14) < 15) goto no;
        if ((s8)B(eax + 0x19) < 15) goto no;
        goto yes;
    case 0x008443fc:
        attribute = (s8)B(eax + 0x14);
        goto test_central;
    case 0x0084441e:
        if ((s8)B(eax + 0x15) < 15) goto no;
        goto yes;
    case 0x00844433:
        eax = D(person + 0x39);
        if (eax) {
            attribute = (s8)B(person + 0x3d);
            if (attribute == 11 || attribute == 15 || attribute == 13) {
                eax = cm_fn_00600a50(eax, 0, 1, 0, 0);
                if (eax) goto no;
            }
        }
        eax = D(person + 0x69);
        if (!eax) goto no;
        eax = D(0x00ae2364);
        other = D(person);
        eax -= 16;
        if ((s32)other >= (s32)eax) goto no;
        goto yes;
    default:
        /* The pinned read-only table has only the sixteen targets above. */
        return ((u32 (CM_FASTCALL *)(u32, u32))target)(15, index);
    }
test_central:
    if (attribute < 15) goto no;
    attribute = (s8)B(eax + 0x17);
test_attribute:
    if (attribute < 15) goto no;
yes:
    return (eax & 0xffffff00u) | 1u;
no:
    return eax & 0xffffff00u;
}
