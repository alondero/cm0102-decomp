#include "cmre.h"

/* Cdecl with one stack argument at +4 (only its low byte is read, sign-extended
   like MOVSX). The routine has a 0x1c-byte local frame: 25 space bytes and a NUL
   at offsets 0 to 25 form the text buffer, offsets 26 and 27 stay uninitialized.
   Every call with ECX = 0x00b74450 is a thiscall on that global object. The
   0x007ff010 calls return a pointer that receives a zero byte before the next
   0x007ff270 call; 0x007ff270 takes (selector, buffer or value, flag/value). */

#define OBJ_B74450 ((int)0x00b74450)

extern u32 CM_THISCALL cm_fn_007fe600(u32 self,u32 param_2,u32 param_3,u32 param_4,u32 param_5,u32 param_6);
extern u32 CM_THISCALL cm_fn_007ff010(int param_1,int param_2);
extern void CM_THISCALL cm_fn_007ff270(int param_1,int param_2,char *param_3,int param_4);

void CM_CDECL cm_fn_005a2c30(s8 arg_04)
{
    struct {
        u32 words[6];
        u8 tail[4];
    } text;
    u32 found;
    u32 pointer;

    found = cm_fn_007fe600(0x00b74450u, 0x005a2e60u, 0x005a45c0u, 0u, 0x005a4eb0u, 0u);
    if (found == 0) {
        return;
    }

    cm_fn_007ff270(OBJ_B74450, 0, (char *)(u32)(s32)arg_04, 0);

    text.words[0] = 0x20202020u;
    text.words[1] = 0x20202020u;
    text.words[2] = 0x20202020u;
    text.words[3] = 0x20202020u;
    text.words[4] = 0x20202020u;
    text.words[5] = 0x20202020u;
    text.tail[0] = 0x20u;
    text.tail[1] = 0u;

    cm_fn_007ff270(OBJ_B74450, 1, (char *)&text, 1);
    pointer = cm_fn_007ff010(OBJ_B74450, 1);
    *(volatile u8 *)pointer = 0;
    cm_fn_007ff270(OBJ_B74450, 2, (char *)0, 0);
    cm_fn_007ff270(OBJ_B74450, 3, (char *)1, 0);
    cm_fn_007ff270(OBJ_B74450, 4, (char *)0, 0);
    cm_fn_007ff270(OBJ_B74450, 5, (char *)0, 0);
    cm_fn_007ff270(OBJ_B74450, 6, (char *)0, 0);
    cm_fn_007ff270(OBJ_B74450, 7, (char *)&text, 1);
    pointer = cm_fn_007ff010(OBJ_B74450, 7);
    *(volatile u8 *)pointer = 0;
    cm_fn_007ff270(OBJ_B74450, 8, (char *)0, 0);
    cm_fn_007ff270(OBJ_B74450, 9, (char *)1, 0);
    cm_fn_007ff270(OBJ_B74450, 0xa, (char *)0, 0);
    cm_fn_007ff270(OBJ_B74450, 0xb, (char *)0, 0);
    cm_fn_007ff270(OBJ_B74450, 0xc, (char *)0, 0);
    cm_fn_007ff270(OBJ_B74450, 0xd, (char *)&text, 1);
    pointer = cm_fn_007ff010(OBJ_B74450, 0xd);
    *(volatile u8 *)pointer = 0;
    cm_fn_007ff270(OBJ_B74450, 0xe, (char *)0, 0);
    cm_fn_007ff270(OBJ_B74450, 0xf, (char *)1, 0);
    cm_fn_007ff270(OBJ_B74450, 0x10, (char *)0, 0);
    cm_fn_007ff270(OBJ_B74450, 0x11, (char *)0, 0);
    cm_fn_007ff270(OBJ_B74450, 0x12, (char *)0, 0);
    cm_fn_007ff270(OBJ_B74450, 0x13, (char *)&text, 1);
    pointer = cm_fn_007ff010(OBJ_B74450, 0x13);
    *(volatile u8 *)pointer = 0;
    cm_fn_007ff270(OBJ_B74450, 0x14, (char *)0, 0);
    cm_fn_007ff270(OBJ_B74450, 0x15, (char *)1, 0);
    cm_fn_007ff270(OBJ_B74450, 0x16, (char *)0, 0);
    cm_fn_007ff270(OBJ_B74450, 0x17, (char *)0, 0);
    cm_fn_007ff270(OBJ_B74450, 0x18, (char *)0, 0);
    cm_fn_007ff270(OBJ_B74450, 0x19, (char *)1, 0);
}
