#include "cmre.h"

/* Original instruction evidence: preferred-image VAs, CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   0051c020-0051c034 saves ECX and reads one stack argument; RET 4.
   0051c045-0051c089 and 0051c2bb-0051c316 show cdecl calls and
   cleanup, with the no-argument 0090d130 nested between format pushes. */
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern char * CM_CDECL cm_fn_0090d130(void);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);

#define B(a) CM_GLOBAL(volatile u8, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define D(a) CM_GLOBAL(volatile u32, (a))

u32 CM_THISCALL cm_fn_0051c020(u32 self, u32 p)
{
    u8 scratch[0x5e8];
    u16 flags;
    s8 first, second, a, b;
    u32 off, winner, stage, table, id1, id2;
    s16 count;
    char *name;
    if (!p) {
        cm_fn_00944cff(0x9b55ac, 0, 0, scratch, scratch + 0x100);
        name = cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, name,
                       scratch, scratch + 0x100, 0x8df);
        cm_fn_005e8290((const char *)0x9870e0, (const char *)0xae24d0, 0);
        D(0xb67a34) = 0;
        return 0xffffffffu;
    }
    flags = W(p + 0x38);
    if (flags & 0x10) {
        B(p + 0x3d) = 1;
        B(p + 0x3e) = 1;
    }
    first = (s8)B(p + 0x43);
    second = (s8)B(p + 0x44);
    if (first != second) {
        off = first > second ? 0x3d : 0x3e;
        winner = first > second ? 1 : 2;
        if (B(p + off) == 1)
            B(p + off) = 2;
        if (flags & 0x2000)
            B(p + off) = (u8)(B(p + off) - 3);
        return winner;
    }
    if (flags & 2) {
        a = (s8)B(p + 0x47);
        b = (s8)B(p + 0x48);
        if (a != b) {
            off = a > b ? 0x3d : 0x3e;
            winner = a > b ? 1 : 2;
            stage = B(p + off);
            if (stage == 1)
                B(p + off) = 2;
            else if (flags & 0x2000)
                B(p + off) = (u8)(stage - 3);
            else
                B(p + off) = 3;
            return winner;
        }
    }
    if (flags & 0x21) {
        a = (s8)B(p + 0x49);
        b = (s8)B(p + 0x4a);
        if (a != b) {
            off = a > b ? 0x3d : 0x3e;
            winner = a > b ? 1 : 2;
            stage = B(p + off);
            if (stage == 1)
                B(p + off) = 2;
            else if (flags & 0x2000)
                B(p + off) = (u8)(stage - 3);
            else
                B(p + off) = (u8)(((~flags) & 1) | 4);
            return winner;
        }
        /* 0051c293-0051c2bb: loads in instruction order. */
        {
            s32 v6 = b, v5 = a;
            s32 v4 = (s8)B(p + 0x48);
            s32 v3 = (s8)B(p + 0x47);
            s32 v2 = (s8)B(p + 0x44);
            s32 v1 = first;
            cm_fn_00944c9f((u32)(scratch + 0x200), 0x9b56b4,
                           v1, v2, v3, v4, v5, v6);
        }
        cm_fn_00944cff(0x9b55ac, 0, 0, scratch + 0x100, scratch);
        name = cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0, 0x9870e8, name,
                       scratch + 0x100, scratch, 0x97e);
        cm_fn_005e8290((const char *)0x9870e0, (const char *)0xae24d0, 0);
        D(0xb67a34) = 0;
    }
    flags = W(p + 0x38);
    if (flags & 0x800) {
        B(p + 0x3d) = 8;
        return 1;
    }
    if (flags & 0x1000) {
        B(p + 0x3e) = 8;
        return 2;
    }
    if (!(flags & 0x2000))
        return 0;
    a = (s8)B(p + 0x3d);
    b = (s8)B(p + 0x3e);
    a = (s8)((u8)a - 1);
    b = (s8)((u8)b - 1);
    B(p + 0x3d) = (u8)a;
    B(p + 0x3e) = (u8)b;
    if (a < b) return 1;
    if (a > b) return 2;
    count = (s16)W(self + 0x3e);
    if (count <= 0) return 2;
    table = D(self + 0xa7);
    id1 = D(p + 0x1c);
    id2 = D(p + 0x20);
    a = b = 0;
    do {
        u32 id = D(table);
        if (id == id1) a = (s8)B(table + 4);
        if (id == id2) b = (s8)B(table + 4);
        table += 6;
    } while (--count);
    return a < b ? 1 : 2;
}
