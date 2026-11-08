/* Preferred-image VAs, base 00400000; primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original language formatter remains an explicit variadic cdecl boundary.
   The immutable original byte map and DWORD target table remain data inputs. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_0066f4e0(u32 output, u32 text, ...);

u32 CM_STDCALL cm_fn_00735160(u32 type, u32 choice, u32 output, u32 limit)
{
    u32 index, target, text;
    s32 selected;
    if (output == 0) return output;
    if ((s16)limit < 15) return output;
    index = (u32)(s32)(s16)type - 3u;
    target = 0x0073565e;
    if (index <= 127u) {
        u32 mapped = CM_GLOBAL(volatile u8, 0x007356f4u + index);
        target = CM_GLOBAL(volatile u32, 0x0073569cu + mapped * 4u);
    }
    selected = (s32)(s8)choice;
    switch (target) {
    case 0x00735214: /* Original target VA. */
        text = selected == 1 ? 0x00a27700u :
               selected == 2 ? 0x00a2771cu : 0x00a27808u;
        break;
    case 0x007352ce: /* Original target VA. */
        text = selected == 1 ? 0x00a27670u :
               selected == 2 ? 0x00a2767cu : 0x00a27808u;
        break;
    case 0x00735290: /* Original target VA. */
        text = selected == 1 ? 0x00a27688u :
               selected == 2 ? 0x00a276a4u : 0x00a27808u;
        break;
    case 0x00735252: /* Original target VA. */
        text = selected == 1 ? 0x00a276e4u :
               selected == 2 ? 0x00a276f0u : 0x00a27808u;
        break;
    case 0x007351d6: /* Original target VA. */
        text = selected == 1 ? 0x00a2773cu :
               selected == 2 ? 0x00a27770u : 0x00a27808u;
        break;
    case 0x0073530c: /* Original target VA. */
        text = selected == 1 ? 0x00a2764cu :
               selected == 2 ? 0x00a2765cu : 0x00a27808u;
        break;
    case 0x00735620: /* Original target VA. */
        text = selected == 1 ? 0x00a27430u :
               selected == 2 ? 0x00a2743cu : 0x00a27808u;
        break;
    case 0x0073534a: /* Original target VA. */
        text = selected == 1 ? 0x00a2762cu :
               selected == 2 ? 0x00a2763cu : 0x00a27808u;
        break;
    case 0x007355e2: /* Original target VA. */
        text = selected == 1 ? 0x00a2744cu :
               selected == 2 ? 0x00a27460u : 0x00a27808u;
        break;
    case 0x007353f7: /* Original target VA. */
        text = selected == 1 ? 0x00a275d0u :
               selected == 2 ? 0x00a27534u : 0x00a27808u;
        break;
    case 0x00735490: /* Original target VA. */
        text = selected == 1 ? 0x00a27554u :
               selected == 2 ? 0x00a27574u : 0x00a27808u;
        break;
    case 0x007354ce: /* Original target VA. */
        text = selected == 1 ? 0x00a27510u :
               selected == 2 ? 0x00a27534u : 0x00a27808u;
        break;
    case 0x00735414: /* Original target VA. */
        text = selected == 1 ? 0x00a275b4u :
               selected == 2 ? 0x00a275c0u : 0x00a27808u;
        break;
    case 0x0073550c: /* Original target VA. */
        text = selected == 1 ? 0x00a274e0u :
               selected == 2 ? 0x00a274f8u : 0x00a27808u;
        break;
    case 0x00735452: /* Original target VA. */
        text = selected == 1 ? 0x00a27588u :
               selected == 2 ? 0x00a275a0u : 0x00a27808u;
        break;
    case 0x0073554a: /* Original target VA. */
        text = selected == 1 ? 0x00a274b4u :
               selected == 2 ? 0x00a274c4u : 0x00a27808u;
        break;
    case 0x00735588: /* Original target VA. */
        text = selected == 1 ? 0x00a2744cu :
               selected == 2 ? 0x00a27494u : 0x00a27808u;
        break;
    case 0x00735388: /* Original target VA. */
        text = selected == 1 ? 0x00a2744cu :
               selected == 2 ? 0x00a27610u : 0x00a27808u;
        break;
    case 0x007353b9: /* Original target VA. */
        text = selected == 1 ? 0x00a275e8u :
               selected == 2 ? 0x00a275f8u : 0x00a27808u;
        break;
    case 0x007355b5: /* Original target VA. */
        text = selected == 1 ? 0x00a2744cu :
               selected == 2 ? 0x00a2747cu : 0x00a27808u;
        break;
    case 0x00735198: /* Original target VA. */
        text = selected == 1 ? 0x00a27798u :
               selected == 2 ? 0x00a277c8u : 0x00a27808u;
        break;
    case 0x0073565e: /* Original target VA. */
        text = selected == 1 ? 0x00a2740cu :
               selected == 2 ? 0x00a2741cu : 0x00a27808u;
        break;
    default:
        /* All 22 immutable original table slots were enumerated above. */
        __builtin_unreachable();
    }
    return cm_fn_0066f4e0(output, text);
}
