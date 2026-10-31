/* Preferred-image VA 005b4a60, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define LD(o) (*(volatile u32 *)(work + (o)))
#define LW(o) (*(volatile u16 *)(work + (o)))
#define LB(o) (*(volatile u8 *)(work + (o)))
extern u32 CM_THISCALL cm_fn_005b4b70(u32 self, u32 index, u32 side, u32 packed);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_005b4a60(u32 self, u32 index)
{
    u8 work[0x20c];
    u32 side, inner, middle, packed, result, text;
    u8 outer;
    if ((s8)index < 0 || (s8)index > 10) {
        cm_fn_00944cff(0x009cc3dcu, 0, 0, work + 0x10c, work + 0xc);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, work + 0x10c, work + 0xc, 0x538u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    LB(4) = 0;
    do {
        /* Native EDI captures all four bytes after only a byte write. */
        side = LD(4);
        LD(0) = 0;
        do {
            LW(8) = LW(0);
            inner = 0;
            do {
                LW(0xa) = (u16)inner;
                packed = LD(8);
                result = cm_fn_005b4b70(self, index, side, packed);
                if (result != 0)
                    return 1;
                inner += 1u;
            } while ((s16)inner < 4);
            middle = LD(0) + 1u;
            LD(0) = middle;
        } while ((s16)middle < 3);
        outer = LB(4);
        outer = (u8)(outer + 1u);
        LB(4) = outer;
    } while ((s8)outer < 2);
    return 0;
}
