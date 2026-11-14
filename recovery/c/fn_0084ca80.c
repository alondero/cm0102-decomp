/* fn_0084ca80 -- preferred-image VA 0x0084ca80 (image base 0x00400000).
   Reconstructed from the original listing only.

   Listing shape (thiscall: ECX read before written, `ret` pops 0 stack bytes):
     push esi / mov esi,ecx / push edi
     mov ecx,0xa / xor eax,eax / mov edi,esi
     mov dword ptr [esi+0x2c],0
     push eax ; rep stosd (10 dwords of 0 at esi) ; lea edi,[esi+0x28]
     push edi ; push eax ; call 0x944c22
     ... vtable call at [vtable+0x18] with (this, [0x00b67a2c], 1) ...
     ... vtable call at [vtable+0x08] with (this) ...
     EAX = esi (the receiver) on every return path. */
#include "cmre.h"

#define DAT_00b67a2c CM_GLOBAL(volatile u32, 0x00b67a2c)

/* Original callee: the listing balances its three pushed words without an
   `add esp`, so the callee removes them (stdcall-like). */
extern s32 CM_STDCALL cm_fn_00944c22(u32 a1, u32 *a2, u32 a3);

/* Interface slots reached through the object's vtable pointer (first dword).
   The receiver is the first stack argument (last push), callee cleans. */
typedef s32 (CM_STDCALL *cm_iface_slot3_t)(u32 *self, u32 a2, u32 a3);
typedef void (CM_STDCALL *cm_iface_slot1_t)(u32 *self);

u32 * CM_THISCALL cm_fn_0084ca80(u32 *self)
{
  u32 *base = self;
  u32 *slot = base + 10;                      /* [esi + 0x28] */
  volatile u32 *vslot = (volatile u32 *)slot; /* DWORD field: reload every read */
  u32 *iface;
  u32 vtbl;
  s32 res;
  int i;

  base[0xb] = 0;                              /* mov dword ptr [esi + 0x2c], 0 */

  for (i = 0; i < 10; i++) {                  /* rep stosd: 10 dwords of 0 at esi */
    base[i] = 0;
  }

  res = cm_fn_00944c22(0, slot, 0);          /* push eax / push edi / push eax */
  if (res != 0) {
    *vslot = 0;                               /* mov dword ptr [edi], 0 */
    return base;
  }

  iface = (u32 *)*vslot;                       /* mov eax, dword ptr [edi] */
  vtbl = *(u32 *)iface;                       /* mov ecx, dword ptr [eax] */
  res = ((cm_iface_slot3_t)(*(volatile u32 *)(vtbl + 0x18)))
        (iface, DAT_00b67a2c, 1);             /* push 1 / push edx / push eax */
  if (res == 0) {
    base[0xb] = 1;                            /* mov dword ptr [esi + 0x2c], 1 */
    return base;
  }

  iface = (u32 *)*vslot;                       /* mov eax, dword ptr [edi] (reload) */
  if (iface != 0) {
    vtbl = *(u32 *)iface;                     /* mov ecx, dword ptr [eax] */
    ((cm_iface_slot1_t)(*(volatile u32 *)(vtbl + 0x08)))(iface); /* push eax */
  }
  *vslot = 0;                                 /* mov dword ptr [edi], 0 */
  return base;
}
