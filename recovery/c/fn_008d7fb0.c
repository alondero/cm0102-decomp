/* Plain-C recovery at preferred-image VA 008d7fb0 (image base 00400000).
   Stdcall: three stack arguments (a1 object, a2 selector, a3 second object),
   callee pops 0x0c. ECX is never read before it is written.

   Frame: after the prologue the stack pointer is B (`sub esp,0x144` plus four
   pushes). Stack locals are named by their original offset from B, for example
   slot14 is [B+0x14]. The 0x12c-byte buffer at [B+0x28] is the only stack
   object whose address escapes (to thiscall helpers and the varargs formatter).
   The formatter at 0066f4e0 is cdecl with variadic arguments: every push
   before its `add esp,0x24` or `add esp,0x14` or `add esp,0x0c` is passed. */
#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae770(u32 self, u32 arg);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, u32 buffer, u32 count, u32 value);
extern u8 CM_THISCALL cm_fn_008e7f10(u32 self, u32 arg);
extern u32 CM_THISCALL cm_fn_00786d30(u32 self, u32 p2, u32 p3, u32 p4, u32 p5, u32 p6);
extern u32 cm_fn_0053e140(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5, u32 p6, u32 p7);
extern u32 cm_fn_0053e990(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 dst, u32 fmt, ...);

void CM_STDCALL cm_fn_008d7fb0(u32 a1, u32 a2, u32 a3)
{
    u8 buffer[0x12c];   /* [B+0x28] .. [B+0x153] */
    u32 obj;
    u32 ebp_v;          /* ebp: result of the first call (index 0) */
    u32 edi_v;
    u32 ebx_v;
    u32 slot14;
    u32 slot18;
    u32 slot1c;
    u32 slot20;
    u32 slot24;
    u8 slot13;
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 k;
    u32 elem;
    u32 table;
    u32 count;
    u32 buf_addr;
    u32 sx_r4;
    u32 sx_r5;
    u32 v5;
    u32 v6;
    u32 ret;

    ebp_v = (u32)cm_fn_00785830((void *)a1, 0u);                       /* 007fc1-007fce */
    eax = (u32)cm_fn_00785830((void *)a1, 9u);                         /* 007fca-007fd0 */
    slot13 = (u8)eax;                                     /* 007fd7 */
    if (slot13 == 0) {
        /* 007fdd-00800f: block taken when the index-9 byte is zero */
        edi_v = (u32)cm_fn_00785830((void *)a1, 7u);                   /* 007fdd-007fe1 */
        eax = (u32)cm_fn_00785830((void *)a1, 8u);                     /* 007fe6-007fec */

        /* 007ff1-008003: table at [0xde1f54] = { data, count }. When the index
           is not below the signed count, the element is read from data[0]. */
        table = CM_GLOBAL(u32, 0x00de1f54);
        count = *(volatile u32 *)(table + 4u);
        elem = *(volatile u32 *)table;
        if ((s32)edi_v < (s32)count) {
            elem = elem + edi_v * 4u;
        }
        elem = *(volatile u32 *)elem;

        if (elem != 0 && *(volatile u32 *)(elem + 0x24u) == eax) {
            slot13 = cm_fn_008e7f10(elem, ebp_v);         /* 00800e-008016 */
            if (slot13 != 0) {
                /* 00801c-008045 */
                u32 d6;
                u32 sx_s;
                s32 w;
                u32 t;
                u32 g3;

                sx_s = (u32)(s32)(s8)slot13;              /* movsx edx, al */
                d6 = *(volatile u32 *)(a1 + 0xd6u);       /* 00801f */
                w = CM_GLOBAL(volatile s16, 0x00b77c7a);  /* movsx eax, word */
                t = *(volatile u32 *)((((u32)w * 3u) << 8) + 0x00b74c26u);
                g3 = CM_GLOBAL(u32, 0x00b7431c);
                cm_fn_00786d30(g3, t, d6, 9u, sx_s, 0u);
            }
        }
    }

    /* 00804a: */
    if (a2 != 0) {
        /* 00805d-008098: calls with indices 1, 2, 3, 4, 5 and 0x0a */
        eax = (u32)cm_fn_00785830((void *)a1, 1u);
        slot1c = eax;                                     /* [B+0x1c] */
        eax = (u32)cm_fn_00785830((void *)a1, 2u);
        slot18 = eax;                                     /* [B+0x18] */
        eax = (u32)cm_fn_00785830((void *)a1, 3u);
        slot20 = eax;                                     /* [B+0x20] */
        eax = (u32)cm_fn_00785830((void *)a1, 4u);
        ebx_v = eax;
        eax = (u32)cm_fn_00785830((void *)a1, 5u);
        edi_v = eax;
        eax = (u32)cm_fn_00785830((void *)a1, 0x0au);
        slot14 = eax;                                     /* 0080a7 [B+0x14] */

        /* 0080ab-0080d7: 53e140(r1, r2, r3, buffer, 0x12c, 1, r0) */
        ecx = slot20;
        edx = slot18;
        cm_fn_0053e140(slot1c, edx, ecx, (u32)buffer, 0x12cu, 1u, ebp_v);

        /* 0080da-0080f0: derived indices from edi (r5) and ebx (r4) */
        eax = CM_GLOBAL(u32, 0x00ae23b4);                 /* K */
        ecx = edi_v * 145u;
        edx = ebx_v * 145u;
        edi_v = edi_v + ecx * 4u;                         /* 581 * r5 */
        edx = ebx_v + edx * 4u;                           /* 581 * r4 */
        buf_addr = (u32)buffer;                           /* 0080df */
        sx_r5 = (u32)(s32)*(volatile s8 *)(edi_v + eax + 0x52u);   /* 0080e7 */
        v6 = edi_v + eax + 0x38u;                         /* 0080ec */
        slot24 = edx;                                     /* 0080f0 [B+0x24] */
        sx_r4 = (u32)(s32)*(volatile s8 *)(edx + eax + 0x52u);     /* 0080f4 */

        /* 0080fa-008111: formatter 0xde1f64 with fmt 0xa94c24 and seven
           varargs: edx, edx, ecx, ecx, eax, esi, buffer address. */
        obj = slot24;
        v5 = obj + eax + 0x38u;                           /* 0080fe */
        cm_fn_0066f4e0(0x00de1f64u, 0x00a94c24u, sx_r4, sx_r4, sx_r5, sx_r5,
                       v5, v6, buf_addr);

        /* 008116: esi becomes the third argument. */
        obj = a3;
        cm_fn_004ae660((void *)obj, 0x00de1f64u);                 /* 008120-008127 */

        if (slot13 == 1) {
            /* 008134-008158 */
            k = CM_GLOBAL(u32, 0x00ae23b4);
            eax = (u32)(s32)*(volatile s8 *)(edi_v + k + 0x52u);
            ecx = edi_v + k + 0x38u;
            cm_fn_0066f4e0(0x00de1f64u, 0x00a94be4u, eax, eax, ecx);
        } else if (slot13 == 4) {
            /* 00815e-0081a5 */
            k = CM_GLOBAL(u32, 0x00ae23b4);
            ret = slot14;
            eax = (u32)(s32)*(volatile s8 *)(edi_v + k + 0x52u);
            if (ret != 0) {
                edx = edi_v + k + 0x38u;
                cm_fn_0066f4e0(0x00de1f64u, 0x00a94b98u, eax, eax, edx);
            } else {
                ecx = edi_v + k + 0x38u;
                cm_fn_0066f4e0(0x00de1f64u, 0x00a94b48u, eax, eax, ecx);
            }
        } else {
            /* 0081a7 */
            CM_GLOBAL(volatile u8, 0x00de1f64) = 0u;
        }

        /* 0081ae-0081d7 */
        cm_fn_004ae770(obj, 0x00de1f64u);
        if ((s32)ebx_v > -1) {
            edx = CM_GLOBAL(u32, 0x00ae23b4);
            eax = slot24;                                 /* [B+0x24] */
            eax = eax + edx;
            cm_fn_004ae8a0(obj, eax + 0x38u, 0x7d5u, eax);
        }

        /* 0081dc-0081f1 */
        k = CM_GLOBAL(u32, 0x00ae23b4);
        eax = edi_v + k;
        cm_fn_004ae8a0(obj, eax + 0x38u, 0x7d5u, eax);

        /* 0081f6-008223: 53e990(110*r0 + G2, r1, r2, r3, 0) with r0 scaled to
           11 * r0 in ebp before the call. */
        eax = slot20;                                     /* [B+0x20] */
        edx = 5u * ebp_v;                                 /* 0081fa */
        ecx = slot18;                                     /* [B+0x18] */
        ebp_v = ebp_v + edx * 2u;                         /* 008204: 11 * r0 */
        edx = slot1c;                                     /* [B+0x1c] */
        eax = CM_GLOBAL(u32, 0x00ae23bc);                 /* G2 */
        edi_v = ebp_v * 5u;
        edi_v = edi_v << 1;                               /* 110 * r0 */
        ecx = edi_v + eax;
        ret = cm_fn_0053e990(ecx, edx, slot18, slot20, 0u);
        if (ret == 0) {
            goto done;                                    /* 008228 */
        }

        /* 00822e-008248 */
        edx = CM_GLOBAL(u32, 0x00ae23bc);
        eax = (u32)buffer;
        edi_v = edi_v + edx;
        ecx = obj;
        cm_fn_004ae8a0(obj, eax, 0x7d7u, edi_v);
    } else {
        /* 008255-008289: indices 1, 2, 3; 53e140(r1, r2, r3, buffer, 0x12c, 5, r0) */
        eax = (u32)cm_fn_00785830((void *)a1, 1u);
        edi_v = eax;
        eax = (u32)cm_fn_00785830((void *)a1, 2u);
        ebx_v = eax;
        eax = (u32)cm_fn_00785830((void *)a1, 3u);
        slot14 = eax;                                     /* 008280 [B+0x14] */
        cm_fn_0053e140(edi_v, ebx_v, eax, (u32)buffer, 0x12cu, 5u, ebp_v);

        /* 00828c-00829b: formatter 0xde1f64 with fmt 0xa94b0c and buffer */
        cm_fn_0066f4e0(0x00de1f64u, 0x00a94b0cu, (u32)buffer);

        /* 0082a0-0082af */
        cm_fn_004ae660((void *)a3, 0x00de1f64u);

        /* 0082b4-0082d5 */
        eax = 5u * ebp_v;
        ecx = slot14;
        edx = CM_GLOBAL(u32, 0x00ae23bc);
        ebp_v = ebp_v + eax * 2u;                         /* 11 * r0 */
        v5 = ebp_v * 5u;
        v5 = v5 << 1;                                     /* 110 * r0 */
        eax = v5 + edx;
        ret = cm_fn_0053e990(eax, edi_v, ebx_v, ecx, 0u);
        if (ret == 0) {
            goto done;                                    /* 0082df */
        }

        /* 0082e1-0082fb */
        ecx = CM_GLOBAL(u32, 0x00ae23bc);
        edx = (u32)buffer;
        v5 = v5 + ecx;
        cm_fn_004ae8a0(a3, edx, 0x7d7u, v5);
    }

done:
    return;
}
