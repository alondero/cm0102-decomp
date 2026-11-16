#include "cmre.h"
extern void CM_THISCALL cm_fn_004a2540(u32, u32, u8);
extern u32 CM_CDECL cm_fn_004c08f0(u32, u32);
extern void cm_fn_005a8f60(void);
extern unsigned long long CM_CDECL cm_fn_005cc7d0(void);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);
extern char * CM_CDECL cm_fn_0066f4e0(int, char *, ...);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern void CM_CDECL cm_fn_00945934(u32);
#define M8(a) (*(volatile u8 *)(a))
#define M16(a) (*(volatile s16 *)(a))
#define M32(a) (*(volatile u32 *)(a))

void CM_THISCALL cm_fn_005223a0(u32 self)
{
    u8 scratch[0x210];
    u32 p, t, v;
    s32 i, j, round, off;
    u32 index = 0;
    u8 rank = 1, current = 1;
    char *name;
    if (M32(self + 8) == 0) {
        cm_fn_00944cff(0x9b55ac, 0, 0, scratch + 0x110, scratch + 0x10);
        name = cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, name, scratch + 0x110, scratch + 0x10, 0x1827);
        cm_fn_005e8290((char *)0x9870e0, (char *)0xae24d0, 0);
        CM_GLOBAL(u32, 0xb67a34) = 0;
        return;
    }
    p = (u32)cm_fn_00944e46((u32)(s32)M16(self + 0x3e) * 4);
    if (!p) {
        cm_fn_00944cff(0x9b55ac, 0, 0, scratch + 0x10, scratch + 0x110);
        name = cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9a8120, name, scratch + 0x10, scratch + 0x110, 0x182f, 0x9b5914);
        cm_fn_0066f4e0(0xae28bc, (char *)0x9a8108);
        cm_fn_0066f4e0(0xae26c4, (char *)0x9a7ff8, 0xae24d0);
        cm_fn_005e8290((char *)0xae28bc, (char *)0xae26c4, 0);
        CM_GLOBAL(u32, 0xb67a34) = 0;
        cm_fn_005cc7d0();
        cm_fn_005a8f60();
        /* 005224d6-005224dd: the returning call leaves its argument on
           the original stack; plain C cannot reproduce that continuation. */
        cm_fn_00945934(0xffffffff);
    }
    i = 0;
    if (M16(self + 0x3e) > 0) {
        do {
            t = M32(self + 0xa7);
            v = M32(t + (u32)i * 6);
            M32(p + (u32)i * 4) = v;
            ++i;
        } while (i < M16(self + 0x3e));
    }
    cm_fn_004c08f0(p, (u32)(s32)M16(self + 0x3e));
    round = (s16)(M16(self + 0x3c) - 2);
    if (round >= 0) {
        off = round * 104;
        do {
            if (!M32(p + (u32)(s32)(s16)index * 4)) break;
            if (round == (s32)M16(self + 0x3c) - 2) {
                t = M32(self + 0xa3);
                j = 0;
                if (M16(t + off + 0x1a) > 0) {
                    do {
                        v = M32(p + (u32)(s32)(s16)index * 4);
                        ++index;
                        cm_fn_004a2540(M32(self + 8), v, current);
                        ++rank;
                        ++j;
                        t = M32(self + 0xa3);
                    } while (j < M16(t + off + 0x1a));
                }
            }
            t = M32(self + 0xa3);
            current = rank;
            j = 0;
            if (M16(t + off + 0x1a) > 0) {
                do {
                    v = M32(p + (u32)(s32)(s16)index * 4);
                    if (v) {
                        ++index;
                        cm_fn_004a2540(M32(self + 8), v, current);
                    }
                    ++rank;
                    ++j;
                    t = M32(self + 0xa3);
                } while (j < M16(t + off + 0x1a));
            }
            --round;
            off -= 104;
        } while (round >= 0);
    }
    cm_fn_009452ca(p);
}
