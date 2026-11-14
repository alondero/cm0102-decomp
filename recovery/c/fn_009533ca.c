/* Translation of preferred-image VA 0x009533ca (image base 0x00400000).
   Listing:
     009533ca  lea eax, [esp + 0xc]
     009533ce  push eax
     009533cf  push dword ptr [esp + 0xc]
     009533d3  push dword ptr [esp + 0xc]
     009533d7  call 0x95345b
     009533dc  add esp, 0xc
     009533df  ret
   At entry ESP+0x4/0x8/0xc are the three stack argument slots (the frame is
   cdecl: `ret` pops only the return address, and the caller removes the
   callee's 0xc argument bytes).  `lea eax, [esp + 0xc]` takes the address of
   the third slot before any push, so the third argument handed to the callee
   is the address of that slot.  The two `push dword ptr [esp + 0xc]` read the
   stack relative to the current ESP, which after the first push is the second
   argument slot and after the second push the first slot; pushed right to
   left they become the callee's first and second arguments.  The callee
   cm_fn_0095345b is cdecl with three u32 arguments. */
#include "cmre.h"

extern void cm_fn_0095345b(u32 param_1, u32 param_2, u32 param_3);

void cm_fn_009533ca(u32 param_1, u32 param_2, u32 param_3)
{
    cm_fn_0095345b(param_1, param_2, (u32)(unsigned long)&param_3);
}
