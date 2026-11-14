/* Translation of preferred-image VA 0x0095345b (image base 0x00400000).
   Listing:
     0095345b  push 0
     0095345d  push dword ptr [esp + 0x10]   ; a3
     00953461  push dword ptr [esp + 0x10]   ; a2
     00953465  push dword ptr [esp + 0x10]   ; a1
     00953469  call 0x94dbe1
     0095346e  add esp, 0x10
     00953471  ret
   The four pushes are the callee arguments in right-to-left order (0 last
   pushed is the fourth argument), the callee is cdecl (the caller removes the
   0x10 argument bytes), and the wrapper itself is cdecl (ret 0). */
#include "cmre.h"

extern int cm_fn_0094dbe1(u32 param_1, char *param_2, u32 param_3, u32 param_4);

void cm_fn_0095345b(u32 param_1, u32 param_2, u32 param_3)
{
    cm_fn_0094dbe1(param_1, (char *)param_2, param_3, 0);
}
