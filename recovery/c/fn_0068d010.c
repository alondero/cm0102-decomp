/* Preferred-image VA 0x0068D010, image base 0x00400000.
   Listing: mov eax, [ecx]; push eax; call 0x00944C94; pop ecx; ret. Thiscall
   (ECX is read first), no stack arguments. Pushes the dword at receiver offset 0
   as the single argument to the cdecl callee 0x00944C94 and discards its result.
   Callers do not read EAX, so the return is void. */
#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);

void CM_THISCALL cm_fn_0068d010(void *self)
{
  (void)cm_fn_00944c94((void *)*(u32 *)self);
}
