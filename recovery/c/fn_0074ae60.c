#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self_arg, u32 index_arg, u32 value_arg);
extern unsigned long long CM_THISCALL cm_fn_007863f0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00786780(void *self, u32 arg1);
extern int cm_fn_0090cfc0(int param_1);

/* Stdcall with one stack argument (ret 4), no result. The argument is a record pointer P.
   The routine links an SEH frame {prev, handler, state} at fs:[0] (state -1, then 0 before
   the three 0x00785790 calls). Its 0xe0-byte scratch area sits directly below the frame and
   is passed as the receiver of the callees. Fields used by offset: +0x53, +0x57 (pointer),
   +0x69 (pointer to a record with a word at +0x69 and a dword at +0x38), +0x80 (signed word).
   The global 0x00b7431c is the receiver of the last callee. Memory accesses are volatile to
   keep the listing order. */
void CM_STDCALL cm_fn_0074ae60(u32 param_1)
{
    volatile struct {
        u32 prev;
        u32 handler;
        s32 state;
    } frame;
    u8 scratch[0xe0];
    u32 p;
    u32 q;
    u32 v0;
    s32 s;
    s32 r;
    u32 n1;
    u32 g;
    u32 v53;
    int first_path;

    frame.state = -1;
    frame.handler = 0x0095d85bU;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    p = param_1;
    if (p != 0 && CM_GLOBAL(volatile u32, p + 0x53) != 0) {
        q = CM_GLOBAL(volatile u32, p + 0x69);
        if (q != 0 && CM_GLOBAL(volatile u32, q + 0x38) != 0 &&
            CM_GLOBAL(volatile u32, p + 0x57) != 0) {
            cm_fn_0077bbe0((void *)scratch, 0x1790U, 0U);
            v0 = CM_GLOBAL(volatile u32, p);
            frame.state = 0;
            cm_fn_00785790((void *)scratch, 0U, v0);
            s = (s16)CM_GLOBAL(volatile u16, p + 0x80);
            cm_fn_00785790((void *)scratch, 1U, (u32)s);
            r = cm_fn_0090cfc0(2);
            cm_fn_00785790((void *)scratch, 2U, (u32)r);

            first_path = 0;
            if ((s16)CM_GLOBAL(volatile u16, p + 0x80) >= 0x2328) {
                n1 = CM_GLOBAL(volatile u32, p + 0x57);
                if (n1 != 0 && (s16)CM_GLOBAL(volatile u16, n1 + 0x69) > 0x0c) {
                    first_path = 1;
                }
            }
            if (first_path) {
                g = CM_GLOBAL(volatile u32, 0x00b7431c);
                cm_fn_00786780((void *)g, (u32)scratch);
            } else {
                v53 = CM_GLOBAL(volatile u32, p + 0x53);
                g = CM_GLOBAL(volatile u32, 0x00b7431c);
                cm_fn_007863f0((void *)g, (u32)scratch, v53);
            }
        }
    }
    CM_FS(u32, 0) = frame.prev;
}
