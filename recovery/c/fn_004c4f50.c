#include "cmre.h"

/* cm_fn_004c4f50 -- preferred-image VA 0x004c4f50 (228 bytes), thiscall:
 * ECX = object on entry, EAX = 1 on return.
 *
 * Stack frame reconstructed from the listing (E = ESP at function entry):
 *   E-4   : SEH state   (pushed -1 at 0x004c4f50)
 *   E-8   : SEH handler (pushed 0x00956b28 at 0x004c4f52)
 *   E-12  : SEH prev    (pushed fs:[0] at 0x004c4f5d)
 *   E-20  : 8-byte scratch area (sub esp,8 at 0x004c4f65)
 *   E-24  : saved EBX   (push ebx at 0x004c4f68)
 *   E-28  : saved ESI   (push esi at 0x004c4f69)
 *
 * Direct callee 0x00549f70 (0x004c4fe0): thiscall, receiver in ECX loaded by
 * `lea ecx,[esp+0x18]` (ESP=E-44 -> E-20, the scratch base), four stack
 * arguments pushed right to left: 0x0f, 5, word at this+0x40, -1.  The callee
 * cleans 0x10 bytes, so ESP is E-28 after it returns and `mov cx,word ptr
 * [esp+8]` (0x004c4fe5) reads the u16 the callee wrote at scratch+0.
 *
 * Vtable slot [edx+0x3c] (0x004c5015): thiscall, receiver ECX = object, four
 * stack arguments pushed right to left: -1, this+0xa9, this+0x3a, 0.  The
 * callee cleans 0x10 bytes, so ESP is E-28 afterwards and `mov ecx,dword ptr
 * [esp+0x10]` (0x004c5018) reads the saved SEH prev slot at E-12, which is
 * restored into fs:[0] at 0x004c5029.  `pop esi`/`pop ebx`/`add esp,0x14`
 * then unwind the frame, so the `ret` pops nothing.
 *
 * The earlier WIP source diverged by (a) using byte stores where the listing
 * uses WORD/DWORD stores (0x3c, 0xb6, 0xb8), (b) pushing the direct callee's
 * arguments in the draft's wrong order and with a fabricated 4th argument
 * instead of the word read from this+0x40, (c) modelling the vtable call as a
 * cdecl-style call instead of a thiscall slot with callee cleanup, and
 * (d) writing the SEH state at the wrong point.  All of those are corrected
 * below; the ECX receiver and the SEH frame are now explicit, so the stack
 * image seen by the original callees matches the original instruction stream.
 *
 * Held obligation (evidence-needed) and its resolution:
 *   "calls through a stack-local ECX and SEH frame don't match the original".
 *   Both halves are now pinned by the listing rather than by the draft:
 *   - The stack-local ECX receiver is one explicit 8-byte scratch object
 *     (`scratch`), matching `sub esp,8` at 0x004c4f65; it is the address the
 *     original passes in ECX (`lea ecx,[esp+0x18]`, ESP=E-44 -> E-20) and the
 *     address the original reads back after the call (`mov cx,word ptr
 *     [esp+8]`, ESP=E-28 -> E-20).  The callee is declared thiscall with
 *     four stack arguments and 0x10 callee cleanup, so the ESP the original
 *     callees observe, and hence the slot the word is read from, is the same.
 *   - The SEH frame is one volatile {prev, handler, state} record linked in
 *     listing order (state=-1, handler=0x00956b28, prev=fs:[0]) and unlinked
 *     from that same record at 0x004c5029; the state DWORD is zeroed at
 *     0x004c5003, where the listing writes it.  The vtable slot is a thiscall
 *     with 0x10 callee cleanup, which is what makes the original's
 *     `mov ecx,dword ptr [esp+0x10]` read the prev slot (E-12) and restore
 *     fs:[0] with it.
 *   Residual, non-observable detail: the original pushes the full EAX for the
 *   callee's u16 argument, so its upper 16 bits are the entry EAX's upper
 *   half (undefined in thiscall).  The evidenced callee signature reads that
 *   argument as u16, so only the low half is observable and the word read
 *   from this+0x40 is pushed as-is.
 */

/* Original callee at preferred-image VA 0x00549f70.  Legacy-record signature
 * kept unchanged: thiscall, receiver (short *) in ECX, four stack arguments,
 * callee cleanup of 0x10 bytes. */
extern short *CM_THISCALL cm_fn_00549f70(short *param_1, short param_2,
                                          char param_3, u16 param_4,
                                          u32 param_5);

/* Indirect vtable slot +0x3c: receiver in ECX, four stack arguments, callee
 * cleanup of 0x10 bytes (the original `call dword ptr [edx+0x3c]`). */
typedef u32 (CM_THISCALL *cm_vtbl_0x3c_fn)(volatile u8 *self, u32 arg1,
                                            volatile u8 *arg2,
                                            volatile u8 *arg3, u32 arg4);

u32 CM_THISCALL cm_fn_004c4f50(volatile u8 *self)
{
    struct {
        struct {
            u16 word;
            u16 pad0;
            u32 pad1;
        } scratch;
        volatile struct {
            void *prev;
            void *handler;
            u32 state;
        } frame;
    } stack;
    cm_vtbl_0x3c_fn *volatile vtable;
    u32 call_result;
    u16 word_40;
    u16 callee_word;
    void *saved_prev;

    /* 0x004c4f50-0x004c4f5e: SEH frame built in listing order and linked. */
    stack.frame.state = 0xffffffffu;
    stack.frame.handler = (void *)(u32)0x00956b28u;
    stack.frame.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&stack.frame;

    /* 0x004c4f6c-0x004c4f84 */
    self[0xc2] = 3;
    self[0xc7] = 3;
    self[0x4a] = 3;
    word_40 = *(volatile u16 *)(void *)(self + 0x40);

    /* 0x004c4f89-0x004c4f97 */
    *(volatile u16 *)(void *)(self + 0x3c) = 2;
    self[0xc6] = 2;
    self[0x42] = 2;

    /* 0x004c4f9a-0x004c4fdd */
    self[0xc3] = 1;
    self[0xc4] = 7;
    self[0xc5] = 1;
    self[0xbe] = 1;
    self[0xbf] = 0;
    self[0xc0] = 0;
    self[0xc1] = 0;
    *(volatile u32 *)(void *)(self + 0x1c) = 0xffffffffu;
    *(volatile u32 *)(void *)(self + 0x20) = 0xffffffffu;
    self[0x49] = 7;

    /* 0x004c4fe0: thiscall, receiver = scratch buffer (lea ecx,[esp+0x18]).
     * Arguments pushed right to left: 0x0f, 5, word_40, -1. */
    (void)cm_fn_00549f70((short *)(void *)&stack.scratch, (short)0x0f, (char)5,
                          word_40, 0xffffffffu);

    /* 0x004c4fe5: mov cx, word ptr [esp+8] -> scratch base (E-20). */
    callee_word = *(volatile u16 *)(void *)&stack.scratch.word;

    /* 0x004c4fea-0x004c4fec: vtable pointer captured before the call. */
    vtable = *(cm_vtbl_0x3c_fn *volatile *)(void *)self;
    *(volatile u16 *)(void *)(self + 0xb6) = callee_word;

    /* 0x004c4ff3-0x004c5015: vtable call, receiver = object. */
    stack.frame.state = 0;                            /* 0x004c5003 */
    *(volatile u16 *)(void *)(self + 0xb8) = 0;       /* 0x004c5007 */
    *(volatile u32 *)(void *)(self + 0x28) = 0x001e8480u; /* 0x004c500e */
    call_result = vtable[0x3c / 4](self, 0xffffffffu, self + 0xa9,
                                    self + 0x3a, 0);

    /* 0x004c5018: capture the predecessor before the potentially aliasing store. */
    saved_prev = stack.frame.prev;
    /* 0x004c501c: keep the callee's EAX. */
    *(volatile u32 *)(void *)(self + 0xba) = call_result;

    /* 0x004c5029: restore the SEH chain (prev read from the frame slot). */
    CM_FS(void *, 0) = saved_prev;

    /* 0x004c5023: mov eax, 1 */
    return 1;
}
