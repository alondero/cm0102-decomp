#include "cmre.h"

/* Argument record fields by byte offset from the record pointer (first stack
   argument): 0x00 pointer P, 0x2c and 0x2d count-like bytes, 0x48 flag byte.
   Results of 0x007e82e0 point at objects whose byte 0x04 is updated here and
   whose word 0x08 is incremented. The 0x0c-byte record handed to 0x00556940 and
   0x00557280 lives at scratch offset 0x0c. */
#define REC_U8(off) (*(volatile u8 *)(rec + (off)))
#define REC_U32(off) (*(volatile u32 *)(rec + (off)))
#define OBJ_U8(p, off) (*(volatile u8 *)((p) + (off)))

/* Scratch bytes in the 0x20 bytes that `sub esp, 0x20` reserves (offsets from
   that base B). 0x03 flag byte, 0x04 dword flag (0 on entry), 0x0c..0x0f record
   header, 0x10..0x17 buffer filled by 0x0054a110 and 0x0054a110's callee, 0x18..0x1b
   record tail, 0x1c, 0x1d..0x1e, 0x1f record bytes. Byte 0x1f is read before any
   write in this routine (uninitialised scratch, see report). */
#define SCR_U8(off) (*(volatile u8 *)(scr + (off)))
#define SCR_U16(off) (*(volatile u16 *)(scr + (off)))
#define SCR_U32(off) (*(volatile u32 *)(scr + (off)))

extern int CM_THISCALL cm_fn_007e82e0(int param_1, u32 param_2, int param_3);
extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);
extern void CM_THISCALL cm_fn_00556940(int param_1, int param_2, char *param_3);
/* The callee pops its three arguments (ret 0xc); the printed prototype did not
   show a convention. */
extern void CM_STDCALL cm_fn_00557280(int param_1, char *param_2, int param_3);

/* Virtual slot at vtable offset 0x0c, called with the object as ECX and two
   stack arguments that the callee pops. */
typedef u32 (CM_THISCALL *cm_slot_0c_fn)(u32 self, u32 arg1, u32 arg2);

/* ECX is not read by this routine; it is forwarded to 0x007e82e0 as that
   callee's this pointer, so it is an explicit parameter here. */
void CM_THISCALL cm_fn_007e21f0(u32 fwd_ecx, u32 rec, u32 arg2)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u8 scr[0x20];
    u8 bl;
    u8 al_v;
    u8 cl_v;
    u8 dl_v;
    u32 ebp_v;
    u32 r1;
    u32 obj;
    u32 p;
    u32 eax_v;
    u32 r2;
    u32 vt;
    u32 base;
    u32 g;

    seh.state = -1;
    seh.handler = 0x00960488u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    bl = 0;
    SCR_U8(0x03) = 0xff;
    SCR_U32(0x04) = 0;
    if (REC_U8(0x2c) == 0 && REC_U8(0x2d) == 0) {
        goto exit;
    }

    p = REC_U32(0x00);
    r1 = (u32)cm_fn_007e82e0((int)fwd_ecx, p, 1);

    if (REC_U8(0x2d) != 0) {
        bl = 1;
        SCR_U8(0x03) = 2;
        ebp_v = 1;
    } else {
        ebp_v = arg2;
    }

    cl_v = REC_U8(0x2c);
    dl_v = (u8)(OBJ_U8(r1, 0x04) + cl_v);
    cl_v = dl_v;
    OBJ_U8(r1, 0x04) = dl_v;
    if (cl_v > 1) {
        dl_v = (u8)(dl_v >> 1);
        bl = (u8)(bl + dl_v);
        SCR_U8(0x03) = 1;
        dl_v = (u8)(dl_v << 1);
        ebp_v = (ebp_v & 0xffff0000u) | (u32)cl_v;
        cl_v = (u8)(cl_v - dl_v);
        OBJ_U8(r1, 0x04) = cl_v;
    }

    if (REC_U8(0x2d) == 1 && REC_U8(0x2c) < 2) {
        g = CM_GLOBAL(volatile u32, 0x00ae28f0);
        p = REC_U32(0x00);
        vt = *(volatile u32 *)g;
        obj = *(volatile u32 *)(vt + 0x60u);
        r2 = (u32)cm_fn_007e82e0((int)obj, p, 1);
        *(volatile u16 *)(r2 + 0x08u) = (u16)(*(volatile u16 *)(r2 + 0x08u) + 0x0cu);

        eax_v = arg2;
        p = REC_U32(0x00);
        vt = *(volatile u32 *)obj;
        eax_v = ((cm_slot_0c_fn)(*(volatile u32 *)(vt + 0x0cu)))(obj, p, eax_v);
        if (eax_v != 0) {
            eax_v = 1;
        } else {
            eax_v = SCR_U32(0x04);
        }
    } else {
        eax_v = SCR_U32(0x04);
    }

    if (bl == 0) {
        goto exit;
    }
    if (eax_v != 0) {
        goto exit;
    }

    cm_fn_0054a110((u16 *)(scr + 0x10));
    seh.state = 0;
    SCR_U8(0x0c) = bl;
    SCR_U16(0x0e) = 0xffffu;
    SCR_U8(0x0d) = 0;
    cm_fn_00417c10((u32 *)(scr + 0x10), (u32 *)(scr + 0x04), (u16 *)0x00ae2c90u);

    al_v = SCR_U8(0x03);
    cl_v = REC_U8(0x48);
    SCR_U8(0x1c) = al_v;
    al_v = SCR_U8(0x1f);
    al_v = (u8)(al_v & 0xfdu);
    cl_v = (u8)(cl_v == 0 ? 1u : 0u);
    al_v = (u8)(al_v & 0xfbu);
    cl_v = (u8)(cl_v << 2);
    cl_v = (u8)(cl_v | al_v);
    SCR_U8(0x1f) = cl_v;

    p = REC_U32(0x00);
    g = CM_GLOBAL(volatile u32, 0x00ae28f0);
    SCR_U16(0x1d) = (u16)ebp_v;
    SCR_U8(0x18) = 0xff;
    SCR_U8(0x19) = 0x19;
    SCR_U8(0x1a) = 0xff;
    SCR_U8(0x1b) = 0xff;
    cm_fn_00556940((int)g, (int)p, (char *)(scr + 0x0c));

    p = REC_U32(0x00);
    base = CM_GLOBAL(volatile u32, 0x00ae23bc);
    g = CM_GLOBAL(volatile u32, 0x00ae28f0);
    cm_fn_00557280((int)(base + p * 110u), (char *)(scr + 0x0c), (int)arg2);

exit:
    CM_FS(u32, 0) = seh.prev;
}
