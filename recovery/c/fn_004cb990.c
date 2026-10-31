#include "cmre.h"

u32 CM_STDCALL cm_fn_004cb990(s8 selector, volatile u8 *object)
{
    s32 index = (s32)selector - 6;
    if ((u32)index > 9u) return 0;
    u32 target = CM_GLOBAL(volatile u32, 0x004cbaf4u + (u32)index * 4u);
    (void)target;

    switch (index) {
    case 0: /* selector 6 */
        if (object == 0) return 0;
        {
            s8 age = *(volatile s8 *)(object + 0x18);
            if (age < 0x1e) goto shared69;
            u32 active = *(volatile u32 *)(object + 0x69);
            if (active != 0) return 1;
            if (age > 0x24) return 1;
            if (*(volatile s8 *)(object + 0x57) >= 0x0a) goto shared69;
            u32 record = *(volatile u32 *)(object + 0x61);
            if (*(volatile s8 *)(record + 0x2f) >= 0x0c) return 1;
            goto shared69;
        }
    case 2: /* selector 8 */
        if (object == 0) return 0;
        {
            s8 age = *(volatile s8 *)(object + 0x18);
            if (age < 0x1e) goto shared69;
            u32 active = *(volatile u32 *)(object + 0x69);
            if (active == 0) {
                if (age > 0x24) goto code_check;
                u32 record = *(volatile u32 *)(object + 0x61);
                if (*(volatile s8 *)(record + 0x2f) < 0x0c) goto shared69;
                if (*(volatile s8 *)(object + 0x57) >= 0x0a) goto shared69;
            }
code_check:
            s8 code = *(volatile s8 *)(object + 0x3d);
            if (code == 5 || code == 0x0a) goto shared69;
            return 1;
        }
    case 3: /* selector 9 */
        if (object == 0) return 0;
        {
            u32 active = *(volatile u32 *)(object + 0x69);
            if (active == 0) return 0;
            s8 age = *(volatile s8 *)(object + 0x18);
            if (age < 0x1c) goto selector9_shared;
            if (*(volatile s8 *)(object + 0x3d) == 9) return 1;
selector9_shared:
            if (*(volatile s8 *)(object + 0x3d) == 9) return 1;
            return 0;
        }
    case 4: /* selector 10 */
        if (object == 0) return 0;
        {
            u32 active = *(volatile u32 *)(object + 0x69);
            if (active == 0) return 0;
            if (*(volatile s8 *)(object + 0x3d) == 0x0a) return 1;
            if (*(volatile s8 *)(object + 0x28) == 0x0a) return 1;
            if (*(volatile s8 *)(object + 0x3d) == 0x0a) return 1;
            return 0;
        }
    case 5: /* selector 11 */
        if (object == 0) return 0;
        return *(volatile u32 *)(object + 0x61) != 0;
    case 7: /* selector 13 */
    case 9: /* selector 15 */
        if (object == 0) return 0;
        {
            u32 record = *(volatile u32 *)(object + 0x61);
            if (record == 0) return 0;
            u32 active = *(volatile u32 *)(object + 0x69);
            if (active != 0) return 1;
            s8 age = *(volatile s8 *)(object + 0x18);
            if (age > 0x22) return 1;
            if (age < 0x1e) return 0;
            return *(volatile s8 *)(record + 0x2f) >= 0x0c;
        }
    default:
        return 0;
    }

shared69:
    {
        u32 active = *(volatile u32 *)(object + 0x69);
        if (active == 0) return 0;
        s8 code = *(volatile s8 *)(object + 0x3d);
        return code == 8 || code == 6 || code == 5;
    }
}
