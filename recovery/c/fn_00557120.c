#include "cmre.h"

/* Stack argument (obj): 0x53 dword, read as a pointer and then dereferenced.
   ECX receiver (self): 0x0c dword array base, 0x12 signed word factor.
   Scratch frame (L = esp after `sub esp, 0x18`): frame[0x00..0x17] at L+k.
   frame[0x00] holds edi, frame[0x04] the dereferenced dword, frame[0x08..]
   is the `this` for 0x0068d0d0 and the fastcall argument for 0x0068d020,
   0x0068d1b0 and 0x0068d040. The SEH record sits at L+0x18 (prev, handler, state). */
#define OBJ_U32(off) (*(volatile u32 *)(obj + (off)))
#define THIS_U32(off) (*(volatile u32 *)(self + (off)))
#define THIS_S16(off) ((s32)(s16)*(volatile u16 *)(self + (off)))

extern void CM_FASTCALL cm_fn_0068d020(u32 p);
extern void CM_FASTCALL cm_fn_0068d040(u32 p);
extern u32 CM_FASTCALL cm_fn_0068d1b0(u32 p);
extern void CM_THISCALL cm_fn_00556e90(u32 self, u32 p2, u32 p3, u32 p4);
extern unsigned long long CM_THISCALL cm_fn_0068d0d0(volatile u8 *self, const u8 *src, s16 count);
extern u32 CM_CDECL cm_fn_00946464(u32 p1, u32 p2, u32 p3, s32 p4, u32 p5);

static u32 rd32(volatile u8 *p)
{
    return (u32)p[0] | ((u32)p[1] << 8) | ((u32)p[2] << 16) | ((u32)p[3] << 24);
}

static void wr32(volatile u8 *p, u32 v)
{
    p[0] = (u8)v;
    p[1] = (u8)(v >> 8);
    p[2] = (u8)(v >> 16);
    p[3] = (u8)(v >> 24);
}

void CM_THISCALL cm_fn_00557120(u32 self, u32 obj)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    volatile u8 frame[0x18];
    u32 ecx_v;
    u32 edi_v;
    u32 ebp_v;
    u32 esi_v;
    u32 eax_v;
    u32 edx_v;
    u32 hit;
    u32 ptr;
    u32 dv;
    u32 cv;
    s32 w;
    s32 copy;

    seh.state = -1;
    seh.handler = 0x00957cc8u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    cm_fn_0068d020((u32)&frame[0x08]);

    edi_v = 0xffffffffu;
    wr32(&frame[0x00], edi_v);

    ptr = OBJ_U32(0x53);
    w = THIS_S16(0x12);
    dv = *(volatile u32 *)ptr;
    cv = THIS_U32(0x0c);
    wr32(&frame[0x04], dv);
    seh.state = 0;
    esi_v = cm_fn_00946464((u32)&frame[0x00], cv, (u32)w, 8,
                           (u32)&CM_GLOBAL(u8, 0x0055e5f0));

    if (esi_v != 0u) {
        ecx_v = rd32(&frame[0x04]);
        edi_v = rd32(&frame[0x00]);
        ebp_v = THIS_U32(0x0c);

        if (esi_v > ebp_v) {
            for (;;) {
                edx_v = *(volatile u32 *)(esi_v - 4u);
                esi_v = esi_v - 8u;
                hit = 0u;
                if (ecx_v != edx_v) {
                    hit = 1u;
                } else if (edi_v != 0xffffffffu) {
                    edx_v = *(volatile u32 *)esi_v;
                    if (edx_v != 0xffffffffu) {
                        hit = ((edi_v - edx_v) != 0u) ? 1u : 0u;
                    }
                }
                if (hit != 0u) {
                    esi_v = esi_v + 8u;
                    break;
                }
                if (esi_v <= ebp_v) {
                    break;
                }
            }
        }

        eax_v = (u32)THIS_S16(0x12);
        edx_v = ebp_v + eax_v * 8u - 8u;
        if (esi_v <= edx_v) {
            for (;;) {
                eax_v = *(volatile u32 *)(esi_v + 4u);
                copy = 0;
                if (ecx_v == eax_v) {
                    if (edi_v == 0xffffffffu) {
                        copy = 1;
                    } else {
                        eax_v = *(volatile u32 *)esi_v;
                        if (eax_v == 0xffffffffu) {
                            copy = 1;
                        } else {
                            edi_v = edi_v - eax_v;
                            ecx_v = edi_v;
                            copy = (ecx_v == 0u) ? 1 : 0;
                        }
                    }
                } else {
                    ecx_v = ecx_v - eax_v;
                    copy = (ecx_v == 0u) ? 1 : 0;
                }
                if (copy == 0) {
                    break;
                }

                cm_fn_0068d0d0(&frame[0x08], (const u8 *)esi_v, 4);
                eax_v = (u32)THIS_S16(0x12);
                ecx_v = THIS_U32(0x0c);
                esi_v = esi_v + 8u;
                edx_v = ecx_v + eax_v * 8u - 8u;
                if (esi_v > edx_v) {
                    break;
                }
                ecx_v = rd32(&frame[0x04]);
                edi_v = rd32(&frame[0x00]);
            }
        }

        eax_v = rd32(&frame[0x0a]);
        wr32(&frame[0x12], eax_v);
        eax_v = cm_fn_0068d1b0((u32)&frame[0x08]);
        while (eax_v != 0u) {
            ecx_v = *(volatile u32 *)eax_v;
            cm_fn_00556e90(self, ecx_v, 0u, 1u);
            eax_v = cm_fn_0068d1b0((u32)&frame[0x08]);
        }
        seh.state = -1;
    } else {
        seh.state = (s32)edi_v;
    }

    cm_fn_0068d040((u32)&frame[0x08]);
    CM_FS(u32, 0) = seh.prev;
}
