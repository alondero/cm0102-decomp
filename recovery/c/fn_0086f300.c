#include "cmre.h"

/* Argument record fields by byte offset from param_1:
   0x08 kind byte (0x96 selects the first branch), 0x09 signed index dword,
   0x0d / 0x11 / 0x15 dwords, 0x19 byte.
   Table rows are 0x6b bytes apart from the word-sized base at VA 0x00ae23d0,
   with a signed byte at row + 0x37. Output buffer VA 0x00de1f64.
   param_2 is the object passed as ECX to the thiscall helpers. */
#define REC_U32(addr, off) (*(volatile u32 *)((u32)(addr) + (u32)(off)))
#define REC_U8(addr, off) (*(volatile u8 *)((u32)(addr) + (u32)(off)))
#define REC_S8(addr, off) (*(volatile s8 *)((u32)(addr) + (u32)(off)))
#define OUT_BUF 0x00de1f64u

extern unsigned long long CM_THISCALL cm_fn_004ae660(u32 self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, u32 p2, u32 p3, u32 p4);
extern u32 cm_fn_0053e140(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5, u32 p6, u32 p7);
extern u32 cm_fn_0053e990(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5);
extern char *cm_fn_0066f4e0(u32 dst, u32 fmt, ...);

void CM_STDCALL cm_fn_0086f300(u32 param_1, u32 param_2)
{
    u8 local_c8[200];
    u8 cVar1;
    u8 cl;
    u32 pcVar8;
    u32 iVar5;
    u32 iVar6;
    u32 iVar7;
    u32 uVar2;
    u32 uVar3;
    u32 uVar4;
    u32 esi;
    u32 edi;
    u32 v;

    cl = REC_U8(param_1, 0x08);
    if (cl == 0x96u) {
        iVar7 = REC_U32(param_1, 0x09);
        uVar2 = REC_U32(param_1, 0x15);
        uVar3 = REC_U32(param_1, 0x0d);
        uVar4 = REC_U32(param_1, 0x11);
        cVar1 = REC_U8(param_1, 0x19);
        cm_fn_0053e140(uVar3, uVar4, uVar2, (u32)local_c8, 200u, 1u, iVar7);
        if (cVar1 == 1u) {
            pcVar8 = 0x00a84a34u;
        } else if (cVar1 == 0u) {
            pcVar8 = 0x00a84a04u;
        } else {
            pcVar8 = 0x00a849d4u;
        }
        cm_fn_0066f4e0(OUT_BUF, pcVar8, (u32)local_c8);
        cm_fn_004ae660(param_2, OUT_BUF);
        iVar7 = iVar7 * 0x6eu;
        iVar5 = cm_fn_0053e990(iVar7 + CM_GLOBAL(u32, 0x00ae23bc), uVar3, uVar4, uVar2, 0u);
        if (iVar5 == 0u) {
            return;
        }
        cm_fn_004ae8a0(param_2, (u32)local_c8, 0x7d7u, iVar7 + CM_GLOBAL(u32, 0x00ae23bc));
        return;
    }

    if (cl == 0u || cl == 1u || cl == 2u || cl == 5u) {
        esi = REC_U32(param_1, 0x09) * 0x6bu;
        iVar6 = (u32)(s32)REC_S8(esi + CM_GLOBAL(u32, 0x00ae23d0), 0x37);
        iVar7 = esi + CM_GLOBAL(u32, 0x00ae23d0) + 4u;
        if (cl == 0u) {
            pcVar8 = 0x00a84848u;
        } else if (cl == 1u) {
            pcVar8 = 0x00a84818u;
        } else if (cl == 2u) {
            pcVar8 = 0x00a847e8u;
        } else {
            pcVar8 = 0x00a847b0u;
        }
        cm_fn_0066f4e0(OUT_BUF, pcVar8, iVar6, iVar6, iVar7);
        cm_fn_004ae660(param_2, OUT_BUF);
        cm_fn_004ae8a0(param_2, CM_GLOBAL(u32, 0x00ae23d0) + esi + 4u, 0x7d0u,
                       CM_GLOBAL(u32, 0x00ae23d0) + esi);
        return;
    }

    if (cl == 8u || cl == 9u) {
        esi = 0u;
        v = REC_U32(param_1, 0x09);
        if (v != 0xffffffffu) {
            esi = CM_GLOBAL(u32, 0x00ae23d0) + v * 0x6bu;
        }
        iVar6 = (u32)(s32)REC_S8(esi, 0x37);
        edi = esi + 4u;
        if (cl == 8u) {
            pcVar8 = 0x00a843acu;
        } else {
            pcVar8 = 0x00a84368u;
        }
        cm_fn_0066f4e0(OUT_BUF, pcVar8, iVar6, iVar6, edi);
        cm_fn_004ae660(param_2, OUT_BUF);
        if (esi == 0u) {
            return;
        }
        cm_fn_004ae8a0(param_2, edi, 0x7d0u, esi);
        return;
    }
}
