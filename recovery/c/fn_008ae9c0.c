#include "cmre.h"

/* fn_008ae9c0: thiscall (ECX = object, read before written) with three stack arguments
   (ret 0xc). Preferred-image VA 0x008ae9c0, image base 0x00400000.
   Object: [this] is a pointer to a table of 32-bit socket handles indexed by the
   sign-extended 16-bit first argument times 4; 0xffffffff marks an empty slot.
   Callees 0x00944c58 (recv), 0x00944c3a (closesocket) and 0x00944c4c (WSAGetLastError)
   are Winsock imports, stdcall per their import signatures. */

extern int CM_STDCALL cm_fn_00944c3a(u32 s);
extern int CM_STDCALL cm_fn_00944c4c(void);
extern int CM_STDCALL cm_fn_00944c58(u32 s, char *buf, int len, int flags);

int CM_THISCALL cm_fn_008ae9c0(int *param_1, s16 param_2, char *param_3, int param_4)
{
    u32 slot;
    int iVar2;
    s32 off;

    if (*(volatile u32 *)param_1 == 0u) {
        return -2;
    }
    off = (s32)param_2 * 4;
    slot = *(volatile u32 *)((u32)off + *(volatile u32 *)param_1);
    if (slot == 0xffffffffu || param_3 == 0 || param_4 < 1) {
        return -1;
    }

    iVar2 = cm_fn_00944c58(slot, param_3, param_4, 0);
    if (iVar2 == -1) {
        iVar2 = cm_fn_00944c4c();
        if (iVar2 == 0x2733) {
            return 0;
        }
        /* Close the slot if it still holds a handle; the table pointer is re-read. */
        if (*(volatile u32 *)param_1 != 0u) {
            slot = *(volatile u32 *)((u32)off + *(volatile u32 *)param_1);
            if (slot != 0xffffffffu) {
                cm_fn_00944c3a(slot);
                *(volatile u32 *)((u32)off + *(volatile u32 *)param_1) = 0xffffffffu;
            }
        }
        return -2;
    }
    if (iVar2 == 0) {
        if (*(volatile u32 *)param_1 != 0u) {
            slot = *(volatile u32 *)((u32)off + *(volatile u32 *)param_1);
            if (slot != 0xffffffffu) {
                cm_fn_00944c3a(slot);
                *(volatile u32 *)((u32)off + *(volatile u32 *)param_1) = 0xffffffffu;
            }
        }
        return -2;
    }
    return iVar2;
}
