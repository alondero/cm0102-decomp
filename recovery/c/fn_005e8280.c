/* Preferred-image VA 0x005E8280, image base 0x00400000.
   Listing: mov eax, [esp+4]; mov [0x00AEBE14], eax; ret. Cdecl (ret 0, caller
   cleans up), one stack argument, no register inputs. Stores the argument to the
   32-bit global at 0x00AEBE14. Its only caller does not read EAX, so the return
   is void. */
#include "cmre.h"

void CM_CDECL cm_fn_005e8280(u32 arg1)
{
  CM_GLOBAL(u32, 0x00aebe14) = arg1;
}
