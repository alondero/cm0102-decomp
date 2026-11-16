/* fn_0094bc6e - shared tail of fn_00946012, preferred-image VA 0x0094bc6e,
   image base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   Listing (VA, instruction):
     0094bc6e  cmp word ptr [esp], 0x27f
     0094bc74  je 0x94bc79
     0094bc76  fldcw word ptr [esp]
     0094bc79  pop edx
     0094bc7a  ret

   The chunk is entered with the caller's saved x87 control word in the stack
   slot at [esp] (fn_00946012 pushes it with `push eax; fnstcw word ptr [esp]`
   and cm_fn_0094bbe5 rewrites its high word) and the caller's return address
   in the next slot, [esp+4].  It restores the saved control word when that word
   differs from 0x27f (the value cm_fn_0094bbe5 leaves behind), drops the slot
   into EDX and returns to the word at [esp+4].

   The harness enters this chunk with the slot at the top of the stack, so the
   chunk returns to the caller's stack word rather than to its caller; the C
   below reproduces that transfer by returning through the same slot.

   Plain C cannot spell fldcw, so the control word is loaded from an FXSAVE
   image whose control-word field is replaced before FXRSTOR restores the FPU
   state; the image is the untouched save of the same state, so only the
   control word changes.  target("sse") only enables the FXSAVE/FXRSTOR
   builtins on a -m32 build; it does not change the emitted behaviour. */
#include "cmre.h"

struct cmre_fx_area {
    u16 control_word;   /* 0x00 */
    u16 status_word;    /* 0x02 */
    u16 tag_word;       /* 0x04 */
    u16 reserved0;      /* 0x06 */
    u8  ip_block[8];    /* 0x08 */
    u8  dp_block[8];    /* 0x10 */
    u32 mxcsr;          /* 0x18 */
    u32 mxcsr_mask;     /* 0x1c */
    u32 st_space[16];   /* 0x20 */
    u8  xmm_space[128]; /* 0x60 */
    u8  reserved1[288]; /* 0xe0 */
} __attribute__((aligned(16)));

__attribute__((target("sse")))
u32 cm_fn_0094bc6e(void)
{
  /* the slot the caller pushed, immediately below this frame's saved frame
     pointer, and the caller's return address in the slot above it */
  u32 *slot = (u32 *)((unsigned char *)__builtin_frame_address(0) + 4);
  u32 continuation;

  /* 0094bc6e cmp word ptr [esp], 0x27f / 0094bc76 fldcw word ptr [esp]:
     the caller's saved control word is loaded into the x87 control register
     unless it already is 0x27f.  The caller reads that control word after the
     chunk returns, so the load is observable even though this chunk ends in
     a transfer. */
  if (*(volatile u16 *)slot != 0x27f) {
    struct cmre_fx_area area;
    u16 saved_control_word = *(volatile u16 *)slot;
    __builtin_ia32_fxsave(&area);
    area.control_word = saved_control_word;
    __builtin_ia32_fxrstor(&area);
  }

  continuation = slot[1];

  /* 0094bc79 pop edx / 0094bc7a ret: the slot is dropped and the return
     address is taken from the word above it. */
  *(volatile u32 *)slot = continuation;
  return 0;
}
