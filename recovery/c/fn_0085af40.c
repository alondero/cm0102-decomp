/* Preferred-image VA 0x0085af40, image base 0x00400000.
   Stdcall with three stack inputs (ret 0xc). ECX is written before every read, so
   there is no incoming receiver. arg1 is the receiver for two calls to
   cm_fn_00785830 (index 0 then 1); the global at 0x00ae23b4 selects a 581-byte
   record (0x245 stride) indexed by the first result. arg2 selects between two
   branches; arg3 is reloaded from the stack after the formatting call and is the
   receiver for the thiscall boundaries. Return is EAX, which is the EAX left by
   the last callee on each path (cm_fn_004ae8a0 or cm_fn_004ae770). Format strings
   and buffers are the original immediates. */
#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern char * CM_CDECL cm_fn_0066f4e0(int param_1, char *param_2, ...);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(void *self, u8 *param_2, int param_3, int param_4);
extern u32 CM_THISCALL cm_fn_004ae770(u32 *self, char *param_2);

u32 CM_STDCALL cm_fn_0085af40(u32 arg1, int arg2, u32 arg3)
{
    u32 eax;
    u32 ebp;
    u32 esi;
    u32 ptr38;
    int iVar3;

    eax = (u32)cm_fn_00785830((void *)arg1, 0u);
    esi = (u32)CM_GLOBAL(int, 0x00ae23b4) + eax + eax * 580u;
    ebp = (u32)cm_fn_00785830((void *)arg1, 1u);
    iVar3 = (int)*(s8 *)(esi + 0x52u);
    ptr38 = esi + 0x38u;

    if (arg2 == 0) {
        cm_fn_0066f4e0((int)0x00de1f64, (char *)0x00a82bd4, iVar3, iVar3, ptr38);
        cm_fn_004ae660((void *)arg3, 0x00de1f64u);
        return cm_fn_004ae8a0((void *)arg3, (u8 *)ptr38, 0x7d5, (int)esi);
    }

    cm_fn_0066f4e0((int)0x00de1f64, (char *)0x00a82c34, iVar3, iVar3, ptr38);
    cm_fn_004ae660((void *)arg3, 0x00de1f64u);
    eax = cm_fn_004ae8a0((void *)arg3, (u8 *)ptr38, 0x7d5, (int)esi);
    if (ebp == 0) {
        return eax;
    }

    cm_fn_0066f4e0((int)0x00de1f64, (char *)0x00a82c14);
    return cm_fn_004ae770((u32 *)arg3, (char *)0x00de1f64);
}
