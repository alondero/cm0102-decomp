#include "cmre.h"
extern char *CM_CDECL cm_fn_0066f4e0(s32 value, char *text);
void CM_STDCALL cm_fn_005a73a0(s8 selector, volatile u8 *output, s16 threshold, s8 flag)
{
    char *text;
    if (output == (volatile u8 *)0) return;
    if (threshold < 0x32) { *output = 0; return; }
    if (flag == 0) {
        switch ((s32)selector - 1) {
        case 0: text = (char *)0x009c3480; break;
        case 1: text = (char *)0x009ca2a8; break;
        case 2: text = (char *)0x009ca294; break;
        case 3: text = (char *)0x009ca280; break;
        default: text = (char *)0x009ca274; break;
        }
    } else {
        switch ((s32)selector - 1) {
        case 0: text = (char *)0x009ca258; break;
        case 1: text = (char *)0x009ca23c; break;
        case 2: text = (char *)0x009ca228; break;
        case 3: text = (char *)0x009ca214; break;
        default: text = (char *)0x009ca208; break;
        }
    }
    (void)cm_fn_0066f4e0((s32)output, text);
}
