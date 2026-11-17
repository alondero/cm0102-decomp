/* CM0102 3.9.68, preferred-image VA 0x007fec60 (scrman.cpp), body 199 bytes.
   Pinned binary SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Every VA below is a preferred-image VA of that image; the ABI statements are
   read from the original instructions at those VAs, not from callee records.

   Instruction evidence
     007fec60 movsx eax, word ptr [ecx + 0x382a]  ECX is the receiver.
     007fed16 / 007fed24 "ret 4"                  thiscall, one stack word.
     007fec67 sub esp, 0x200                     scratch base L = ESP-0x200,
                                                 second buffer L+0x100.
     007fec89..007fec99 five words pushed, "add esp, 0x14" after the call:
                                                 cm_fn_00944cff is cdecl with
                                                 five stack arguments.
     007feca7..007fecbe three words are pushed before 007fecae; 0090d130 is
       "mov eax, 0xad4394; ret" (0x0090d130, 0x0090d135) and reads no stack
       word, so those three words stay pending for the variadic tail of
       00944c9f, whose six words are 0x00ae24d0 (destination), 0x009870e8
       (format) and the four values 0xad4394, L+0x100, L, 0x5e1.
     007fecc3..007fecf4 "push 0 / push 0xae24d0 / push 0x9870e0 / call
       0x5e8290" then "add esp, 0x24": the three arguments of 005e8290 sit on
       top of the six words the 00944c9f call left on the stack, and the nine
       words are cleared together.  005e8290 ends with "add esp, 0x6c; ret"
       (0x005e8875), so it is cdecl and leaves all nine words to that single
       "add esp"; it reads only its own three arguments (0x005e82d0 reads
       [E+8], 0x005e8315 reads [E+0xc]), so the six extra words are stack
       state only.  The C call reproduces the same nine-word stack state.
     007fecd7 mov dword ptr [0xb67a34], 0        global store.
     007fece1 / 007fece6                        two calls, no pushed word.
     007feceb push -1 / 007feced call 0x945934  terminal call.
   Callees stay explicit original boundaries; their C is navigation only. */
#include "cmre.h"

extern void CM_CDECL cm_fn_005a8f60(void);
extern unsigned long long CM_CDECL cm_fn_005cc7d0(void);
/* 005e8290 takes three arguments (007fecc3..007feccf) and is cdecl; the six
   extra words model the residue of the 00944c9f call that the original leaves
   directly above them (007feccf, "add esp, 0x24"). */
extern void CM_CDECL cm_fn_005e8290(u32 string, u32 destination, u32 flag,
                                    u32 residue1, u32 residue2, u32 residue3,
                                    u32 residue4, u32 residue5, u32 residue6);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 dir,
                                   void *fname, void *ext);
extern void CM_CDECL cm_fn_00945934(u32 param);

u32 CM_THISCALL cm_fn_007fec60(u32 self, u32 optional)
{
    /* 007fec60..007fec73: signed WORD index, 3 * 256 byte stride, DWORD
       target slot at self + 0x7e2. */
    s32 index = *(volatile s16 *)(self + 0x382au);
    u32 offset = (u32)index * 3u;
    offset <<= 8;
    u32 target = *(volatile u32 *)(self + offset + 0x7e2u);

    if (target == 0) {
        /* 007fec7e..007fec99: L = ESP after "sub esp, 0x200" and the second
           buffer L + 0x100. 00944cff splits the path 0x00a7ddbc into file
           name (L + 0x100) and extension (L). */
        volatile u8 scratch[0x200];
        u32 scratch_base = (u32)scratch;
        (void)cm_fn_00944cff(0x00a7ddbcu, 0, 0,
                             (void *)(scratch_base + 0x100u),
                             (void *)scratch_base);

        /* 007fec9c..007fecbe: 0090d130 yields the constant 0xad4394; the
           pending words L + 0x100, L and 0x5e1 are the variadic tail of
           00944c9f, whose destination is 0x00ae24d0 and format 0x009870e8. */
        u32 diagnostic = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, diagnostic,
                             scratch_base + 0x100u, scratch_base, 0x5e1u);

        /* 007fecc3..007feccf: 005e8290(0x009870e0, 0x00ae24d0, 0) entered
           with the six 00944c9f words still on the stack above its three
           arguments (007fecd4 clears all nine). */
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0,
                       0x00ae24d0u, 0x009870e8u, diagnostic,
                       scratch_base + 0x100u, scratch_base, 0x5e1u);

        /* 007fecd7..007feced: global store, two calls, terminal call. */
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        cm_fn_005cc7d0();
        cm_fn_005a8f60();
        cm_fn_00945934(0xffffffffu);
    }

    /* 007fecf2..007fed24, including the approved 007fed0c branch. */
    u32 active = *(volatile u32 *)(target + 0x1f4u);
    if (active == 0)
        return 1;

    if (optional != 0)
        return 0;

    u32 nested = *(volatile u32 *)(target + 0x10u);
    if (nested != 0)
        return 1;
    return 0;
}
