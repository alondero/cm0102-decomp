#include "cmre.h"

/* Object fields by byte offset from the object pointer (ECX on entry):
   0x04 pointer to a pointer, 0x0c array base, 0x2c dword, 0x30 signed dword,
   0x3c and 0x3e words, 0x44 byte, 0x45 dword, 0xa7 and 0xa9 words, 0xb1 dword,
   0xb5 byte, 0xdd dword, 0xe6 dword. Element vtable slot 0x10 takes the
   element as ECX and two stack arguments; the vtable slot 0x94 takes only ECX. */
#define OBJ_U8(off) (*(volatile u8 *)(obj + (off)))
#define OBJ_U16(off) (*(volatile u16 *)(obj + (off)))
#define OBJ_U32(off) (*(volatile u32 *)(obj + (off)))

typedef u8 (CM_THISCALL *cm_slot10_fn)(u32 self, u32 arg_a, u32 arg_b);
typedef void (CM_THISCALL *cm_slot94_fn)(u32 self);

extern u32 CM_CDECL cm_fn_004b0700(u32 v);
extern void CM_FASTCALL cm_fn_00682e10(int param_1);
extern void CM_THISCALL cm_fn_0086a070(int param_1, int param_2);

u8 CM_THISCALL cm_fn_00689ad0(u32 self, u32 arg1, u32 arg2)
{
    u32 obj;
    u32 ecx;
    u32 eax;
    u32 edx;
    u32 ebx;
    u32 edi;
    u32 dd;
    u32 v;
    u32 cnt;
    u8 al;
    s16 di;
    s16 bx;

    obj = self;
    ecx = OBJ_U32(0xb1);
    if (ecx == 0) {
        return 0;
    }
    if (OBJ_U8(0xb5) != 0) {
        return 0;
    }
    if (OBJ_U32(0x45) == 0) {
        return 0;
    }
    al = OBJ_U8(0x44);
    dd = OBJ_U32(0xdd);
    if (al == 0xff) {
        if (dd != 0) {
            goto L_ba9;
        }
    } else {
        if (dd != 0) {
            return 1;
        }
    }

    if (OBJ_U16(0xa7) == 0xffffu) {
        s16 ax = (s16)OBJ_U16(0x3e);
        if (ax > 0) {
            OBJ_U16(0xa7) = (u16)((s32)(s16)(ax - 1) * (s32)(s16)OBJ_U16(0x3c));
        }
    }

    edx = (u32)(s32)(s16)OBJ_U16(0x3e);
    cnt = 0;
    if ((s32)edx > 0) {
        di = (s16)OBJ_U16(0xa7);
        ecx = ecx + 0x11u;
        do {
            bx = (s16)(s8)*(volatile u8 *)ecx;
            if (bx < di) {
                return 0;
            }
            cnt = cnt + 1;
            ecx = ecx + 0x3bu;
        } while ((s32)cnt < (s32)edx);
    }

    eax = OBJ_U32(0x04);
    ecx = *(volatile u32 *)eax;
    v = cm_fn_004b0700(ecx);
    if (v != 0 && (s16)OBJ_U16(0xa9) > 8) {
        if (OBJ_U32(0xe6) != 0) {
            goto L_ba9;
        }
        cm_fn_0086a070((int)CM_GLOBAL(volatile u32, 0x00dd7f00), (int)obj);
    }
    /* 0x00689b98 */
    if (OBJ_U32(0xe6) != 0) {
        goto L_ba9;
    }
    cm_fn_00682e10((int)obj);

L_ba9:
    eax = arg1;
    ebx = 1;
    if (eax != ebx) {
        goto L_bea;
    }
    if (OBJ_U8(0x44) != 0xff) {
        goto L_bea;
    }
    eax = OBJ_U32(0x30);
    edi = 0;
    if ((s32)eax >= 0) {
        do {
            edx = OBJ_U32(0x0c);
            ecx = *(volatile u32 *)(edx + edi * 4u);
            eax = *(volatile u32 *)ecx;
            {
                cm_slot10_fn slot = (cm_slot10_fn)(*(volatile u32 *)(eax + 0x10u));
                u8 r = slot(ecx, 0u, ebx);
                if (r == 0) {
                    return 0;
                }
            }
            eax = OBJ_U32(0x30);
            edi = edi + 1;
        } while ((s32)edi <= (s32)eax);
    }

    ecx = OBJ_U32(0x2c) - 1u;
    eax = OBJ_U32(0x30);
    if ((s32)eax < (s32)ecx) {
        return 0;
    }

L_bea:
    eax = OBJ_U32(0xdd);
    if (eax != 0) {
        goto L_c0c;
    }
    if (arg2 != 0) {
        edx = OBJ_U32(0x00);
        {
            cm_slot94_fn slot = (cm_slot94_fn)(*(volatile u32 *)(edx + 0x94u));
            slot(obj);
        }
    }
    OBJ_U32(0xdd) = ebx;

L_c0c:
    return (u8)ebx;
}
