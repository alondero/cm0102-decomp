/* Preferred-image VA 0x005c6740, image base 0x00400000.
   Target SHA-256: 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Thiscall with four stack arguments (ret 0x10). Offsets below are the listing's
   [esp+N] addresses after the prologue (esp = entry - 0x18 for the scratch block). */
#include "cmre.h"

/* 12 bytes of scratch at entry-0x18. The listing stores the receiver at +0 and
   passes entry-0x14 (+4) in ECX to cm_fn_00549ef0. That callee writes the low
   word of its first stack argument at its ECX+0, the low word of its second at
   ECX+2 and a u32 result at ECX+4 (see recovery/c/fn_00549ef0.c). */
struct cm_scratch {
    u32 saved_this;
    u16 buf_w0;
    u16 buf_w2;
    u32 buf_u4;
};

/* SEH registration record at entry-0x0c: prev (fs:[0] at entry), handler
   (0x0095a0f8), state (-1 on entry, 0 after the first call). */
struct cm_seh_frame {
    void *prev;
    u32 handler;
    u32 state;
};

extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *self, u32 first, u32 second);
extern u32 CM_FASTCALL cm_fn_0068d1b0(int param_1);

int CM_THISCALL
cm_fn_005c6740(int param_1, u32 param_2, u32 param_3, int param_4, int param_5)
{
    volatile struct cm_seh_frame frame;
    struct cm_scratch scratch;
    u32 self_reg;
    u32 rec;
    u32 result = 0;
    s8 count;
    s8 index;
    s32 i;

    frame.state = 0xffffffffu;
    frame.handler = 0x0095a0f8u;
    frame.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&frame;

    self_reg = (u32)param_1;
    scratch.saved_this = self_reg;
    cm_fn_00549ef0((void *)&scratch.buf_w0, param_2, param_3);

    rec = *(volatile u32 *)(self_reg + 0x34cc);
    frame.state = 0;
    if (rec == 0) {
        goto done;
    }
    *(volatile u32 *)(rec + 0xa) = *(volatile u32 *)(rec + 2);
    rec = cm_fn_0068d1b0((int)*(volatile u32 *)(self_reg + 0x34cc));
    if (rec == 0) {
        goto done;
    }

    while (1) {
        if (*(u32 *)rec == (u32)param_4) {
            count = *(s8 *)(rec + 8);
            if (count > 0) {
                index = 0;
                do {
                    i = (s32)index;
                    if (scratch.buf_w2 == *(u16 *)(rec + (u32)(i * 25) + 0xb)) {
                        if (scratch.buf_w0 == *(u16 *)(rec + (u32)(i * 25) + 9)) {
                            if (*(u32 *)(rec + (u32)(i * 25) + 25) == (u32)param_5) {
                                result = rec + (u32)(i * 25) + 9;
                                goto done;
                            }
                        }
                    }
                    index = (s8)(index + 1);
                } while (index < count);
                self_reg = scratch.saved_this;
            }
        }
        rec = cm_fn_0068d1b0((int)*(volatile u32 *)(self_reg + 0x34cc));
        if (rec == 0) {
            break;
        }
    }

done:
    CM_FS(void *, 0) = frame.prev;
    return (int)result;
}
