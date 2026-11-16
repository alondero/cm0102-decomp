/* Preferred-image VAs, CM0102 3.9.68, SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 *
 * HELD: body translation only, NOT a faithful stack-probe boundary.
 * 0059fcf0 MOV EAX,1388; 0059fcf5 CALL 00945610 allocates the
 * original frame through a callee-driven ESP change and relocated RET.
 * The local below describes subsequent buffer accesses but does NOT
 * preserve that helper's execution, probing, or return-slot observations.
 * An ordinary C call to 00945610 is not ABI-compatible. No such call
 * is fabricated here. A passing body test cannot resolve this blocker;
 * an evidenced plain-C boundary is still required before acceptance.
 */
#include "cmre.h"

/* 0059fd16/5b/66: ECX receiver, one pushed index.
 * 0059fdc1: cdecl destination, format, and three optional words;
 * caller removes 8 or 14h bytes at 0059fd2f/46/c6.
 * 0059fde7: ECX=EBX, pushes record, 7d5h, record+4.
 * 0059fdf5: ECX=EBX, one pushed buffer; final EAX is retained.
 * All ABI evidence is from the pinned image identified above.
 */
extern unsigned long long CM_THISCALL cm_fn_00785830(void *, u32);
extern char *CM_CDECL cm_fn_0066f4e0(char *, const char *, ...);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, const char *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *, u32);

/* After the original allocation, incoming stack slots are:
 * 0059fcfa [ESP+1390] = argument 2;
 * 0059fd02 [ESP+1398] after PUSH EBX = argument 3;
 * 0059fd0d [ESP+1390] after PUSH EBX = argument 1.
 * 0059fe01 RET 0c supplies the stdcall cleanup.
 */
u32 CM_STDCALL cm_fn_0059fcf0(u32 arg1, u32 arg2, u32 arg3)
{
    char scratch[0x1388];
    u32 index, record, base;
    s16 mode;
    s32 value;

    if (arg2 == 0) {
        mode = (s16)cm_fn_00785830((void *)arg1, 1);
        cm_fn_0066f4e0(scratch, (const char *)
                      (mode != 0 ? 0x009c96a4u : 0x009c9688u));
    } else {
        index = (u32)cm_fn_00785830((void *)arg1, 0);
        mode = (s16)cm_fn_00785830((void *)arg1, 1);
        if (mode == 0 || mode == 1) {
            base = CM_GLOBAL(volatile u32, 0x00ae23b4);
            record = base + index + ((index * 9u << 4) + index) * 4u;
            value = CM_GLOBAL(volatile s8, record + 0x37u);
            cm_fn_0066f4e0(scratch,
                          (const char *)(mode == 0 ? 0x009c94ecu : 0x009c95c4u),
                          value, value, record + 4u);
        }
        base = CM_GLOBAL(volatile u32, 0x00ae23b4);
        record = base + index + ((index * 9u << 4) + index) * 4u;
        cm_fn_004ae8a0(arg3, (const char *)(record + 4u), 0x7d5u, record);
    }
    return (u32)cm_fn_004ae660((void *)arg3, (u32)scratch);
}
