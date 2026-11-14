/* Preferred-image VA 0x0062c590, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Physical EAX/EDX/ECX input prefix and EDX:EAX output projection; this makes
   no original high-level 64-bit type claim. Preserve incoming high bits on
   NULL/range exits and the original signed low-DX guards. Ordinary callees
   remain original boundaries; physical caller-stack alias/native-exception
   parity and composed execution remain separate obligations. */
#include "cmre.h"

extern u8 CM_THISCALL cm_fn_0062c400(u32 self, u32 person);

unsigned long long CM_STDCALL __attribute__((regparm(3), target("no-sse3")))
cm_fn_0062c590(u32 in_EAX, u32 in_EDX, u32 self, u32 person, u32 optional)
{
    u32 eax;
    s32 edx;

    if (person == 0) {
        eax = in_EAX & 0xffff0000u;
        edx = (s32)in_EDX;
        return ((unsigned long long)(u32)edx << 32) | eax;
    }

    {
        s32 id = *(volatile s32 *)(unsigned long)person;
        s32 limit = CM_GLOBAL(volatile s32, 0xae2364);

        if (id >= limit) {
            u32 t = optional;
            eax = (t != 0u) ? 0xffffffffu : 0u;
            eax = (eax & 0xffffd954u) + 0x2710u;
            edx = (s32)in_EDX;
            return ((unsigned long long)(u32)edx << 32) | eax;
        }

        {
            u8 al = cm_fn_0062c400(self, person);
            u32 table = *(volatile u32 *)(unsigned long)self;
            u8 bl = al;
            u32 id2 = *(volatile u32 *)(unsigned long)person;
            u32 index = (id2 << 5) - id2;
            u32 base = table + index;
            s32 v10 = *(volatile s16 *)(unsigned long)(base + 0xau);
            s32 v8 = *(volatile s16 *)(unsigned long)(base + 0x8u);
            s32 prod = v10 * v8;
            long long wide = (long long)prod * 0x68db8badLL;
            s32 q = (s32)(u32)((unsigned long long)wide >> 32);
            s16 dx;
            u32 res;

            q = q >> 12;
            q = q + (s32)((u32)q >> 31);

            dx = (s16)(u16)(u32)q;
            if (bl == 1 && dx >= 0x2328) {
                q = 0x2328;
                res = (u32)q;
            } else if (dx > 0x64) {
                res = (u32)q;
            } else {
                res = 0x64u;
            }

            edx = q;
            if (optional != 0) {
                u32 slot;
                long double x;
                long long ft;

                edx = (s32)(s16)(u16)res;   /* movsx edx, ax */
                slot = (u32)edx;            /* mov [esp+0x10], edx */
                x = (long double)(s32)slot; /* fild dword [esp+0x10] */
                x = x * (long double)CM_GLOBAL(volatile float, 0x96bac8);
                x = x + (long double)CM_GLOBAL(volatile double, 0x968968);
                /* call 0x0062c641 -> 0x00945640: registered FTOL */
                ft = (long long)x;
                res = (u32)(unsigned long long)ft;
                edx = (s32)(u32)((unsigned long long)ft >> 32);
                if ((s16)(u16)res > 0x64)
                    res = 0x64u;
            }

            eax = res;
            return ((unsigned long long)(u32)edx << 32) | eax;
        }
    }
}
