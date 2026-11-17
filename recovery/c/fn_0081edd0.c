#include "cmre.h"
/* Original-instruction evidence: preferred-image VAs 0081edd0..0081ef6b,
 * CM0102 3.9.68 SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * 0081ee3d/42, 0081ee65..71 and 0081ef1a..23 establish receivers.
 * Stack balances establish helper cleanup and the pending second argument
 * to 005edcc0 at 0081ef1f.
 */
extern void cm_fn_0041bfb0(void);
/* Register carrier, not a semantic 64-bit result. For the pinned binary above,
 * preferred-image VA 0081ee20 consumes EDX after this call (0081ee2c/3c).
 * Preserve EDX:EAX through the i386 integer return convention.
 * Original target epilogue 0066fb96 loads ECX from its saved FS predecessor,
 * 0066fba1 restores FS:[0], and 0066fbae returns without changing ECX.
 * Thus ECX here is the address of our linked frame; EDX remains the actual
 * target's post-call value, not a guessed zero or reconstructed callee result.
 * This bound-target review is not composition or runtime proof.
 */
extern unsigned long long cm_fn_0066f4e0(int, char *);
extern void CM_THISCALL cm_fn_00803f30(u32, u32, u32, u32);
extern void CM_THISCALL cm_fn_00803f80(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007fec60(u32, u32);
extern void cm_fn_0075db00(char, int);
extern u32 cm_fn_005ed300(int,u32,int,u32,u32,u32,u32,int,int,u32,int);
extern u32 cm_fn_005edcc0(int,int);
extern void CM_STDCALL cm_fn_009463af(u32,u32,u32,u32,u32);
extern u32 CM_STDCALL cm_fn_009462bb(u32,u32,u32,u32);

u32 cm_fn_0081edd0(void)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x350];
    u32 a, b, result;
    unsigned long long text_regs;
    frame.state = 0xffffffff;
    frame.handler = 0x961349;
    frame.prev = CM_FS(u32,0);
    CM_FS(u32,0) = (u32)&frame;
    cm_fn_009463af((u32)scratch,0xd4,4,0x4568f0,0x5a53c0);
    frame.state = 0;
    cm_fn_0041bfb0();
    text_regs = cm_fn_0066f4e0(0xdd70a8,(char *)0xa80288);
    /* 0081ee25/2c write only CX/DX; 0081ee3b/3c push full ECX/EDX. */
    a = ((u32)&frame & 0xffff0000u) | CM_GLOBAL(volatile u16,0xae3184);
    b = ((u32)(text_regs >> 32) & 0xffff0000u) | CM_GLOBAL(volatile u16,0xae31a8);
    cm_fn_00803f30(0xb74450,b,a,0xdd70a8);
    cm_fn_0075db00(4,0);
    cm_fn_0066f4e0(0xde1f64,(char *)0xa80220);
    cm_fn_00803f80(0xb74450,0xde1f64,0);
    cm_fn_0066f4e0((int)(scratch+4),(char *)0xa80508);
    *(u32 *)(scratch+0xcc) = 0x10;
    cm_fn_0066f4e0((int)(scratch+0xd8),(char *)0xa804ec);
    *(u32 *)(scratch+0x1a0) = 0x11;
    cm_fn_0066f4e0((int)(scratch+0x1ac),(char *)0xa804d4);
    *(u32 *)(scratch+0x274) = 0x12;
    cm_fn_0066f4e0((int)(scratch+0x280),(char *)0xa804c0);
    *(u32 *)(scratch+0x348) = 0x13;
    cm_fn_005ed300(0x6e,0x91,0x30c,0x217,2,6,3,4,(int)scratch,0,0);
    result = cm_fn_007fec60(0xb74450,0);
    cm_fn_005edcc0(result == 0,0);
    frame.state = 0xffffffff;
    result = cm_fn_009462bb((u32)scratch,0xd4,4,0x5a53c0);
    CM_FS(u32,0) = frame.prev;
    return result;
}
