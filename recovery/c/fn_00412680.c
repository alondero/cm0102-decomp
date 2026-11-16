/* fn_00412680 @ preferred-image VA 0x00412680 (image base 0x00400000,
   module award\australia_awards.cpp). thiscall (ECX), `ret 4` = one stack arg.
   Reconstructed from the original instruction listing only.

   HELD concern (year DWORD): the listing writes only a WORD at frame L+4
   (0x004126ab, from word [0x00ae2c92]) and later loads the DWORD at L+4 at
   0x004127a9 / 0x004127b5, 0x00412814 / 0x00412820 and 0x0041287f / 0x0041288b
   and forwards it to the original constructors as arg 8 and arg 10
   (arg 10 = arg 8 + 1). The reconstruction keeps one scratch block with the
   original offsets and performs those DWORD reloads from the same block, so
   the actual bits (including the never-initialized upper 16) are transported
   to the original callee exactly as the original does: they are neither
   zeroed nor narrowed, and no initialization is invented for them.

   Receiver of every constructor call is ECX = EAX = the object just returned
   by the allocator (`mov ecx, eax` at 0x0041276e/0x004127d8/0x00412843/
   0x004128ae/0x0041290c/0x0041296b/0x004129ca/0x00412a29/0x00412a86);
   the `push edi` in each group is the first stack argument (this+0x13). */

#include "cmre.h"

/* Original SEH frame record: prev at L+0x208, handler at L+0x20c,
   state (try level) at L+0x210, immediately after the 0x208-byte scratch. */
struct cm_412680_seh {
    void *prev;
    void *handler;
    u32 state;
};

struct cm_412680_frame {
    u8 body[0x208];
    volatile struct cm_412680_seh seh;
};

extern void cm_fn_005e8290(const char *param_1,const char *param_2,int param_3);
extern u32 CM_THISCALL cm_fn_00766e10(void *self, u32 arg_04, u32 arg_08, u32 arg_0c, u32 arg_10, u32 arg_14, u32 arg_18, u32 arg_1c, u32 arg_20, u32 arg_24, u32 arg_28, u32 arg_2c, u32 arg_30);
extern u32 CM_THISCALL cm_fn_008af520(void *self, u32 arg_04, u32 arg_08, u32 arg_0c, u32 arg_10, u32 arg_14, u32 arg_18, u32 arg_1c, u32 arg_20, u32 arg_24, u32 arg_28);
/* The three pushes before `call 0x90d130` (0x004126ee/0x004126f0/0x004126f1)
   are its arguments: the enclosing `add esp, 0x24` at 0x00412717 accounts for
   exactly those three plus the two following three-push groups, so the callee
   does not pop them. Its EAX result is forwarded to the next call. */
extern char * CM_CDECL cm_fn_0090d130(u32 arg1, u32 arg2, u32 arg3);
extern u32 * CM_THISCALL cm_fn_0092fe80(u32 *self, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6, u32 arg7, u32 arg8, u32 arg9, u32 arg10, u32 arg11);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);

#define G_ae2c92 CM_GLOBAL(u16, 0x00ae2c92)
#define G_9cf89c CM_GLOBAL(u32, 0x009cf89c)
#define G_9cfe7c CM_GLOBAL(u32, 0x009cfe7c)
#define G_9cfe80 CM_GLOBAL(u32, 0x009cfe80)
#define G_9cfe84 CM_GLOBAL(u32, 0x009cfe84)
#define G_9cfe88 CM_GLOBAL(u32, 0x009cfe88)
#define G_9cfe8c CM_GLOBAL(u32, 0x009cfe8c)
#define G_9cfe90 CM_GLOBAL(u32, 0x009cfe90)
#define G_9cfe94 CM_GLOBAL(u32, 0x009cfe94)
#define G_9cfe98 CM_GLOBAL(u32, 0x009cfe98)
#define G_9cfe9c CM_GLOBAL(u32, 0x009cfe9c)
#define G_b67a34 CM_GLOBAL(u32, 0x00b67a34)

void CM_THISCALL cm_fn_00412680(void *self, u32 stack_arg)

{
    struct cm_412680_frame K;
    u8 *const B = K.body;
    void *prev;
    u32 edi;
    u32 idx;
    u32 r;
    u32 pv;
    u32 y_a;
    u32 y_b;
    u32 ia;
    volatile u32 *slots;
    char *s;

    /* 0x00412680/0x00412682/0x00412687/0x0041268e: frame build, then link. */
    K.seh.state = 0xffffffffu;
    K.seh.handler = (void *)0x00953afeu;
    prev = CM_FS(void *, 0);
    K.seh.prev = prev;
    CM_FS(void *, 0) = (void *)&K.seh;

    /* 0x004126a8 */
    edi = *(volatile u32 *)((u8 *)self + 0x13);
    /* 0x0041269b/0x004126ab: WORD store into the L+4 slot. */
    *(u16 *)(B + 4) = G_ae2c92;
    /* 0x004126b0 */
    idx = 0;
    /* 0x004126b2 */
    *(u16 *)((u8 *)self + 8) = 9;
    /* 0x004126a6/0x004126b8/0x004126bd */
    pv = (u32)cm_fn_00944e46(0x24);
    /* 0x004126c2 */
    *(volatile u32 *)((u8 *)self + 4) = pv;
    /* 0x004126c0: cmp eax, esi (only the DWORD return's EAX is tested). */
    if (pv == 0) {
        /* 0x004126c7..0x004126e0 */
        cm_fn_00944cff(0x00987cc0u, 0, 0, (void *)(B + 0x108), (void *)(B + 8));
        /* 0x004126e3..0x004126f8. The 90d130 argument pushes at
           0x004126ee/0x004126f0/0x004126f1 are not popped before this call, so
           the callee's varargs 2..4 are those three slots in place
           ([esp+12] = L+0x108, [esp+16] = L+8, [esp+20] = 0x74); the format at
           0x009870e8 ("v%s_%s_%s_%d") reads exactly them. */
        s = cm_fn_0090d130((u32)(B + 0x108), (u32)(B + 8), 0x74);
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)s, (u32)(B + 0x108),
                       (u32)(B + 8), 0x74);
        /* 0x00412707..0x00412712 */
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        /* 0x0041271a */
        G_b67a34 = 0;
    }
    else {
        /* 0x00412726: 0x25d-byte constructor. */
        pv = (u32)cm_fn_00944cf1(0x25d);
        /* 0x00412733 */
        *(volatile u32 *)(B + 0) = pv;
        /* 0x00412740: state = esi (0) */
        K.seh.state = 0;
        /* 0x0041273e/0x00412747 */
        if (pv == 0) {
            r = 0;
        }
        else {
            /* 0x00412755 */
            idx = 1;
            /* 0x0041275a..0x00412770: ECX = eax = pv */
            r = cm_fn_008af520((void *)pv, edi, 0, G_9cfe7c, 1000, 500,
                               G_9cf89c, 1u, 0, stack_arg, 1u);
        }
        /* 0x00412779: read the base while try level 0 is still active. */
        slots = (volatile u32 *)*(volatile u32 *)((u8 *)self + 4);
        /* 0x00412781 */
        K.seh.state = 0xffffffffu;
        /* 0x0041278c: slot 0 */
        slots[0] = r;

        /* 0x0041277c/0x0041278e: 0xa2-byte constructor, slot 1 */
        pv = (u32)cm_fn_00944cf1(0xa2);
        /* 0x00412796 */
        *(volatile u32 *)(B + 0) = pv;
        /* 0x0041279c */
        K.seh.state = 1;
        /* 0x0041279a/0x004127a7 */
        if (pv == 0) {
            r = 0;
        }
        else {
            /* 0x004127a9: DWORD reload of the L+4 slot (first view). */
            y_a = *(volatile u32 *)(B + 4);
            /* 0x004127ad/0x004127af */
            ia = idx;
            idx = idx + 1;
            /* 0x004127b5: DWORD reload of the L+4 slot (second view). */
            y_b = *(volatile u32 *)(B + 4);
            /* 0x004127b0..0x004127da: ECX = eax = pv */
            r = cm_fn_00766e10((void *)pv, edi, ia, G_9cfe80, 0x5dc, 0x2bc,
                               G_9cf89c, 9, y_b, 4, y_a + 1, stack_arg, 0);
        }
        /* 0x004127e3 */
        slots = (volatile u32 *)*(volatile u32 *)((u8 *)self + 4);
        /* 0x004127eb */
        K.seh.state = 0xffffffffu;
        /* 0x004127f6: slot 1 */
        slots[1] = r;

        /* 0x004127e6/0x004127f9: 0xa2-byte constructor, slot 2 */
        pv = (u32)cm_fn_00944cf1(0xa2);
        /* 0x00412801 */
        *(volatile u32 *)(B + 0) = pv;
        /* 0x00412807 */
        K.seh.state = 2;
        /* 0x00412805/0x00412812 */
        if (pv == 0) {
            r = 0;
        }
        else {
            /* 0x00412814 */
            y_a = *(volatile u32 *)(B + 4);
            /* 0x00412818/0x0041281a */
            ia = idx;
            idx = idx + 1;
            /* 0x00412820 */
            y_b = *(volatile u32 *)(B + 4);
            /* 0x0041281b..0x00412845: ECX = eax = pv */
            r = cm_fn_00766e10((void *)pv, edi, ia, G_9cfe84, 0x3e8, 0x1f4,
                               G_9cf89c, 9, y_b, 4, y_a + 1, stack_arg, 1);
        }
        /* 0x0041284e */
        slots = (volatile u32 *)*(volatile u32 *)((u8 *)self + 4);
        /* 0x00412856 */
        K.seh.state = 0xffffffffu;
        /* 0x00412861: slot 2 */
        slots[2] = r;

        /* 0x00412851/0x00412864: 0xa2-byte constructor, slot 3 */
        pv = (u32)cm_fn_00944cf1(0xa2);
        /* 0x0041286c */
        *(volatile u32 *)(B + 0) = pv;
        /* 0x00412872 */
        K.seh.state = 3;
        /* 0x00412870/0x0041287d */
        if (pv == 0) {
            r = 0;
        }
        else {
            /* 0x0041287f */
            y_a = *(volatile u32 *)(B + 4);
            /* 0x00412883/0x00412885 */
            ia = idx;
            idx = idx + 1;
            /* 0x0041288b */
            y_b = *(volatile u32 *)(B + 4);
            /* 0x00412886..0x004128b0: ECX = eax = pv */
            r = cm_fn_00766e10((void *)pv, edi, ia, G_9cfe88, 0x320, 0x190,
                               G_9cf89c, 9, y_b, 4, y_a + 1, stack_arg, 1);
        }
        /* 0x004128b9 */
        slots = (volatile u32 *)*(volatile u32 *)((u8 *)self + 4);
        /* 0x004128be */
        K.seh.state = 0xffffffffu;
        /* 0x004128c9: slot 3 */
        slots[3] = r;

        /* 0x004128bc/0x004128cc: 0x2b-byte constructor, slot 4 */
        pv = (u32)cm_fn_00944cf1(0x2b);
        /* 0x004128d4 */
        *(volatile u32 *)(B + 0) = pv;
        /* 0x004128da */
        K.seh.state = 4;
        /* 0x004128d8/0x004128e5 */
        if (pv == 0) {
            r = 0;
        }
        else {
            /* 0x00412902/0x0041290e */
            ia = idx;
            idx = idx + 1;
            /* 0x004128e7..0x0041290f: ECX = eax = pv */
            r = (u32)cm_fn_0092fe80((u32 *)pv, edi, ia, G_9cfe8c, 0x7d0, 0x3e8,
                                    G_9cf89c, 1u, 0, 0, stack_arg, 1u);
        }
        /* 0x00412918 */
        slots = (volatile u32 *)*(volatile u32 *)((u8 *)self + 4);
        /* 0x0041291d */
        K.seh.state = 0xffffffffu;
        /* 0x00412928: slot 4 */
        slots[4] = r;

        /* 0x0041291b/0x0041292b: 0x2b-byte constructor, slot 5 */
        pv = (u32)cm_fn_00944cf1(0x2b);
        /* 0x00412933 */
        *(volatile u32 *)(B + 0) = pv;
        /* 0x00412939 */
        K.seh.state = 5;
        /* 0x00412937/0x00412944 */
        if (pv == 0) {
            r = 0;
        }
        else {
            /* 0x00412961/0x0041296d */
            ia = idx;
            idx = idx + 1;
            /* 0x00412946..0x0041296e: ECX = eax = pv */
            r = (u32)cm_fn_0092fe80((u32 *)pv, edi, ia, G_9cfe90, 0x5dc, 0x2ee,
                                    G_9cf89c, 1u, 0, 0, stack_arg, 1u);
        }
        /* 0x00412977 */
        slots = (volatile u32 *)*(volatile u32 *)((u8 *)self + 4);
        /* 0x0041297c */
        K.seh.state = 0xffffffffu;
        /* 0x00412987: slot 5 */
        slots[5] = r;

        /* 0x0041297a/0x0041298a: 0x2b-byte constructor, slot 6 */
        pv = (u32)cm_fn_00944cf1(0x2b);
        /* 0x00412992 */
        *(volatile u32 *)(B + 0) = pv;
        /* 0x00412998 */
        K.seh.state = 6;
        /* 0x00412996/0x004129a3 */
        if (pv == 0) {
            r = 0;
        }
        else {
            /* 0x004129c0/0x004129cc */
            ia = idx;
            idx = idx + 1;
            /* 0x004129a5..0x004129cd: ECX = eax = pv */
            r = (u32)cm_fn_0092fe80((u32 *)pv, edi, ia, G_9cfe94, 0x5dc, 0x2ee,
                                    G_9cf89c, 1u, 0, 0, stack_arg, 1u);
        }
        /* 0x004129d6 */
        slots = (volatile u32 *)*(volatile u32 *)((u8 *)self + 4);
        /* 0x004129db */
        K.seh.state = 0xffffffffu;
        /* 0x004129e6: slot 6 */
        slots[6] = r;

        /* 0x004129d9/0x004129e9: 0x2b-byte constructor, slot 7 */
        pv = (u32)cm_fn_00944cf1(0x2b);
        /* 0x004129f1 */
        *(volatile u32 *)(B + 0) = pv;
        /* 0x004129f7 */
        K.seh.state = 7;
        /* 0x004129f5/0x00412a02 */
        if (pv == 0) {
            r = 0;
        }
        else {
            /* 0x00412a1f/0x00412a2b */
            ia = idx;
            idx = idx + 1;
            /* 0x00412a04..0x00412a2c: ECX = eax = pv */
            r = (u32)cm_fn_0092fe80((u32 *)pv, edi, ia, G_9cfe98, 0x5dc, 0x2ee,
                                    G_9cf89c, 1u, 0, 1u, stack_arg, 1u);
        }
        /* 0x00412a35 */
        slots = (volatile u32 *)*(volatile u32 *)((u8 *)self + 4);
        /* 0x00412a3a */
        K.seh.state = 0xffffffffu;
        /* 0x00412a45: slot 7 */
        slots[7] = r;

        /* 0x00412a38/0x00412a48: 0x2b-byte constructor, slot 8 */
        pv = (u32)cm_fn_00944cf1(0x2b);
        /* 0x00412a50 */
        *(volatile u32 *)(B + 0) = pv;
        /* 0x00412a56 */
        K.seh.state = 8;
        /* 0x00412a54/0x00412a61 */
        if (pv == 0) {
            r = 0;
        }
        else {
            /* 0x00412a84: no increment for the last constructor. */
            r = (u32)cm_fn_0092fe80((u32 *)pv, edi, idx, G_9cfe9c, 0x5dc, 0x2ee,
                                    G_9cf89c, 1u, 0, 0, stack_arg, 0);
        }
        /* 0x00412a91/0x00412a95: slot 8 */
        ((u32 *)(*(volatile u32 *)((u8 *)self + 4)))[8] = r;
    }

    /* 0x00412a98..0x00412aa2: restore fs:[0] from the linked frame. */
    CM_FS(void *, 0) = K.seh.prev;
    return;
}
