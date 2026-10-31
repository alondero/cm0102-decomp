#include "cmre.h"

/* Original routine 0x0055ca00 (preferred-image VA, image base 0x00400000).
   Convention: thiscall (ECX = this, read before written), ret 8, two stack
   arguments: arg1 = pointer p (esi), arg2 = value argb ([esp+0x14] at the entry
   pushes). The routine returns the EAX left by its last step on each path, so
   the return value is modelled per path; the callee forwarding is explicit.

   Paths (listing labels):
     A  0x0055ca09  table lookup on *p (signed index >= 0) -> callee 0x007e7ff0 with
                    stack args (p, argb, 1, 1, 1); its EAX is returned.
     B  0x0055ca47  callee 0x005c6710 on this table entry ([0xae2c74]) with [p+0x14];
                    if its low dword is zero and *p != [0x9cf684], return [0x9cf684].
     C  0x0055ca65  table lookup through [[p+0x1c]+0x57]; call 0x007e7ff0 with
                    (p, argb, 1, 0, 0), then fall through to D.
     D  0x0055caa2  table lookup through [[p+0x20]+0x57]; call 0x007e7ff0 with
                    (p, argb, 0, 1, 0). Early exits return the EAX they hold.

   Byte loads `mov al, [..]` replace only the low byte of EAX; the returned
   value keeps the high 24 bits of the previous EAX. */

extern unsigned long long CM_THISCALL cm_fn_005c6710(u32 self, u32 arg);
extern u32 CM_THISCALL cm_fn_007e7ff0(u32 self, u32 p, u32 argb, u32 a3, u32 a4, u32 a5);

CM_THISCALL u32 cm_fn_0055ca00(u32 self, u32 p, u32 argb)
{
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 entry;
    u32 ebx;

    /* 0x0055ca09 */
    eax = *(volatile u32 *)p;
    if ((s32)eax <= -1) {
        goto path_b;
    }
    ecx = CM_GLOBAL(volatile u32, 0x00adadfc);
    entry = *(volatile u32 *)(ecx + eax * 4u);
    eax = entry;
    if (eax == 0u) {
        goto path_b;
    }
    eax = (eax & 0xffffff00u) | *(volatile u8 *)(entry + 0x50u);
    if ((eax & 0xffu) == 0xffu) {
        goto path_b;
    }
    edx = (u32)(s32)(s8)(u8)(eax & 0xffu);
    eax = *(volatile u32 *)self;
    ecx = *(volatile u32 *)(eax + edx * 4u);
    if (ecx == 0u) {
        goto path_b;
    }
    /* push 1; push 1; push 1; push argb; push p; call 0x007e7ff0 */
    return cm_fn_007e7ff0(ecx, p, argb, 1u, 1u, 1u);

path_b:
    /* 0x0055ca47 */
    eax = *(volatile u32 *)(p + 0x14u);
    ecx = CM_GLOBAL(volatile u32, 0x00ae2c74);
    eax = (u32)cm_fn_005c6710(ecx, eax);
    if (eax != 0u) {
        goto path_c;
    }
    ecx = *(volatile u32 *)p;
    eax = CM_GLOBAL(volatile u32, 0x009cf684);
    if (ecx != eax) {
        return eax;
    }
    goto path_c;

path_c:
    /* 0x0055ca65 */
    edx = *(volatile u32 *)(p + 0x1cu);
    ebx = argb;
    eax = *(volatile u32 *)(edx + 0x57u);
    if (eax == 0u) {
        goto path_d;
    }
    eax = *(volatile u32 *)eax;
    ecx = CM_GLOBAL(volatile u32, 0x00adadfc);
    entry = *(volatile u32 *)(ecx + eax * 4u);
    eax = entry;
    if (eax == 0u) {
        goto path_d;
    }
    eax = (eax & 0xffffff00u) | *(volatile u8 *)(entry + 0x50u);
    if ((eax & 0xffu) == 0xffu) {
        goto path_d;
    }
    edx = (u32)(s32)(s8)(u8)(eax & 0xffu);
    eax = *(volatile u32 *)self;
    ecx = *(volatile u32 *)(eax + edx * 4u);
    if (ecx == 0u) {
        goto path_d;
    }
    /* push 0; push 0; push 1; push ebx; push p; call 0x007e7ff0 (EAX discarded) */
    cm_fn_007e7ff0(ecx, p, ebx, 1u, 0u, 0u);

path_d:
    /* 0x0055caa2 */
    ecx = *(volatile u32 *)(p + 0x20u);
    eax = *(volatile u32 *)(ecx + 0x57u);
    if (eax == 0u) {
        return eax;
    }
    edx = *(volatile u32 *)eax;
    eax = CM_GLOBAL(volatile u32, 0x00adadfc);
    entry = *(volatile u32 *)(eax + edx * 4u);
    eax = entry;
    if (eax == 0u) {
        return eax;
    }
    eax = (eax & 0xffffff00u) | *(volatile u8 *)(entry + 0x50u);
    if ((eax & 0xffu) == 0xffu) {
        return eax;
    }
    edx = *(volatile u32 *)self;
    ecx = (u32)(s32)(s8)(u8)(eax & 0xffu);
    ecx = *(volatile u32 *)(edx + ecx * 4u);
    if (ecx == 0u) {
        return eax;
    }
    /* push 0; push 1; push 0; push ebx; push p; call 0x007e7ff0 */
    return cm_fn_007e7ff0(ecx, p, argb, 0u, 1u, 0u);
}
