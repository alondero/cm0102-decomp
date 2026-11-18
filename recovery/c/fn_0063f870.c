/* fn_0063f870 (comp\cups\ita_cup.cpp) preferred-image VA 0x0063f870,
   image base 0x00400000, pinned CM0102 3.9.68 binary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Reconstructed from the original instructions only.

   ABI findings from original instructions at preferred-image VAs:
   - 0x0063f8b1 `mov esi, ecx` is the first read of ECX (ECX read before
     written per the brief evidence), and the routine ends in a plain `ret`
     (0x0063fac7 / 0x0063fae5 pop no stack bytes): thiscall receiver, no
     stack arguments.
   - 0x0063f9be `mov ecx, esi` then 0x0063f9c0 `call dword ptr [edx + 0x3c]`
     with the four stack arguments pushed at 0x0063f9b4 (0), 0x0063f9ba
     (&local_418), 0x0063f9bb (&local_414) and 0x0063f9bc (7): thiscall
     indirect call with receiver `this` and stack arguments (7, &0x1c,
     &0x18, 0).  The callee cleans those 0x10 bytes (no later `add esp`).
   - 0x0063fac8 `push 0` / `call 0x0051c410` has no `mov ecx` of its own;
     ECX was loaded at 0x0063fa3c `mov ecx, dword ptr [edx + 0x1c]` and is
     forwarded unchanged, so that loaded object is the explicit receiver.
   - 0x0063f9b3..0x0063fa10 pushes 15 stack arguments for the constructor
     0x00522e00 with ECX from 0x0063fa0e `mov ecx, eax` (the new object).
   - Stack locals keep their original adjacency: one 0x424-byte scratch
     region (L = ESP after `sub esp, 0x424` at 0x0063f885) holding the
     24 byte flags at L+0x00, local_418 at L+0x18, local_414 at L+0x1c,
     the new-object home at L+0x20 and the four 256-byte buffers at
     L+0x24, L+0x124, L+0x224 and L+0x324.  The SEH record occupies
     L+0x424..L+0x42f (prev/handler/state).
   - Return width: every exit is a plain `ret` (0x0063fac7, 0x0063fae5) and
     the routine is entered for its side effects; the EAX left differs per
     exit (the 0x00522e00/0x0051c410 store result on the success exit, the
     0x00944c9f/0x005e8290 residue on the 0x00b67a34-clearing exits).  The
     brief's profile reports no EDX read by callers, so no 64-bit result is
     claimed; no return value is declared.  A caller that consumed EAX would
     require widening this declaration. */
#include "cmre.h"

#define CM_SEH_HANDLER_VA 0x0095b92eu

typedef struct cm_seh_frame {
    u32 prev;
    u32 handler;
    u32 state;
} cm_seh_frame;

extern u32 CM_THISCALL cm_fn_0051c410(u32 self, u32 index);
extern u32 CM_THISCALL cm_fn_00522e00(u32 self, u32 owner, s16 count, u32 clubs,
                                      s16 rounds, u32 competition, u32 schedule,
                                      u16 year, u8 flag_a, u8 flag_b,
                                      u16 round_flag, u8 flag_c, u32 builder,
                                      u32 numbered, u32 value_38, u32 club_flags);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern char *CM_CDECL cm_fn_0090d130(u32 param_1, u32 param_2, u32 param_3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                   void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

/* DWORD view of an object field: the original reloads these fields between
   calls, and DWORD stores can overlap any pointer-typed view of the same
   bytes, so the access stays a volatile 32-bit load/store. */
#define FIELD32(base, off) (*(volatile u32 *)((u32)(base) + (u32)(off)))

#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

void CM_THISCALL cm_fn_0063f870(u32 self)
{
    u32 scratch_words[0x109]; /* L+0x000 .. L+0x423 */
    volatile cm_seh_frame frame; /* L+0x424 prev, L+0x428 handler, L+0x42c state */
    u8 *flags;
    u8 *hi_buf;
    u8 *lo_buf;
    u32 line;
    u32 *block;
    u32 i;
    s32 limit;
    u32 counted;
    u32 *cursor;
    u32 element;
    u32 sched;
    u32 slot;
    u32 *pv;
    u32 object;
    u32 arr;
    u32 holder;

    flags = (u8 *)scratch_words; /* L+0x00, 24 flag bytes */

    /* 0x0063f870 push -1 / 0x0063f872 push 0x0095b92e / 0x0063f877 mov eax,
       fs:[0] / push eax / 0x0063f87e mov fs:[0], esp  */
    frame.state = 0xffffffffu;
    frame.handler = CM_SEH_HANDLER_VA;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    /* 0x0063f88e..0x0063f8aa: bytes L+0x00..L+0x07 = 3 */
    flags[0x00] = 3;
    flags[0x01] = 3;
    flags[0x02] = 3;
    flags[0x03] = 3;
    flags[0x04] = 3;
    flags[0x05] = 3;
    flags[0x06] = 3;
    flags[0x07] = 3;
    /* 0x0063f8b5..0x0063f8d1: bytes L+0x08..L+0x0f = 2 */
    flags[0x08] = 2;
    flags[0x09] = 2;
    flags[0x0a] = 2;
    flags[0x0b] = 2;
    flags[0x0c] = 2;
    flags[0x0d] = 2;
    flags[0x0e] = 2;
    flags[0x0f] = 2;
    /* 0x0063f8d5..0x0063f8f8: bytes L+0x10..L+0x17 = 1 */
    flags[0x10] = 1;
    flags[0x11] = 1;
    flags[0x12] = 1;
    flags[0x13] = 1;
    flags[0x14] = 1;
    flags[0x15] = 1;
    flags[0x16] = 1;
    flags[0x17] = 1;

    /* 0x0063f8fd push 0x60 / call 0x00944e46 */
    block = (u32 *)(u32)cm_fn_00944e46(0x60);

    if (block == (u32 *)0) {
        /* 0x0063f90b..0x0063f91f */
        cm_fn_00944cff(0x009ea23cu, 0, 0,
                       (void *)((u8 *)scratch_words + 0x224),
                       (void *)((u8 *)scratch_words + 0x24));
        /* 0x0063f927..0x0063f938 */
        hi_buf = (u8 *)scratch_words + 0x224;
        lo_buf = (u8 *)scratch_words + 0x24;
        line = 0x29cu;
    }
    else {
        /* 0x0063f93d..0x0063f967: block[0..7] = *(u32*)of the field pointer */
        i = 0;
        do {
            if (i == 0) {
                block[0] = FIELD32(FIELD32(self, 0xb1), 0);
            }
            else {
                block[i] = FIELD32(
                    FIELD32(FIELD32(FIELD32(self, 0xc), (i - 1) * 4), 0xb1), 0);
            }
            i = i + 1;
        } while ((s32)i < 8);

        /* 0x0063f969..0x0063f9ac: move the non-null entries of the
           [self+0xee] array to block[8..], clearing the source slots.
           counted (edi) starts at 8 and the loop stops at 24. */
        limit = (s32)FIELD32(self, 0xf2);
        i = 0;
        counted = 8;
        if (limit > 0) {
            cursor = block + 8;
            do {
                if ((s32)counted >= 0x18) {
                    break;
                }
                element = FIELD32(FIELD32(self, 0xee), i * 4);
                if (element != 0) {
                    *cursor = element;
                    counted = counted + 1;
                    cursor = cursor + 1;
                    FIELD32(FIELD32(self, 0xee), i * 4) = 0;
                }
                limit = (s32)FIELD32(self, 0xf2);
                i = i + 1;
            } while ((s32)i < limit);
        }

        /* 0x0063f9ae..0x0063f9c0: thiscall virtual slot 0x3c, selector 7 */
        slot = FIELD32(FIELD32(self, 0), 0x3c);
        sched = ((u32 (CM_THISCALL *)(u32, u32, u32 *, u32 *, u32))slot)(
            self, 7, (u32 *)((u8 *)scratch_words + 0x1c),
            (u32 *)((u8 *)scratch_words + 0x18), 0);

        /* 0x0063f9c3 push 0xb2 / call 0x00944cf1 */
        pv = (u32 *)(u32)cm_fn_00944cf1(0xb2);
        *(volatile u32 *)((u8 *)scratch_words + 0x20) = (u32)pv; /* 0x0063f9d2 */
        frame.state = 0; /* 0x0063f9d8 mov dword ptr [esp+0x438], 0 */
        if (pv == (u32 *)0) {
            object = 0;
        }
        else {
            /* Pinned CM0102 3.9.68 SHA-256
               0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86,
               preferred-image VAs: 0x0063f9e5 loads the separate round flag;
               0x0063f9ee loads the year WORD from [esi+0x40], pushed at
               0x0063fa02 as stack argument seven to 0x00522e00.
               Keep the loads ordered before the constructor call. */
            u32 round_flag = *(volatile u32 *)((u8 *)scratch_words + 0x18);
            u16 year = *(volatile u16 *)(self + 0x40u);
            u32 competition = FIELD32(self, 4); /* 0x0063f9fb */
            u32 rounds = *(volatile u32 *)((u8 *)scratch_words + 0x1c);
            object = cm_fn_00522e00(
                (u32)pv, self, 0x18, (u32)block,
                (s16)rounds,
                competition, sched,
                year, 7, 1,
                (u16)round_flag, 0x14, 0, 0,
                0, (u32)flags);
        }
        arr = FIELD32(self, 0xc); /* 0x0063fa19 mov ecx, [esi+0xc] */
        /* 0x0063fa1d mov dword ptr [esp+0x43c], 0xffffffff */
        frame.state = 0xffffffffu;
        FIELD32(arr, 0x1c) = object; /* 0x0063fa28 mov [ecx+0x1c], eax */
        cm_fn_009452ca((u32)block); /* 0x0063fa2b */
        cm_fn_009452ca(sched); /* 0x0063fa31 */
        arr = FIELD32(self, 0xc); /* 0x0063fa36 mov edx, [esi+0xc] */
        holder = FIELD32(arr, 0x1c); /* 0x0063fa3c mov ecx, [edx+0x1c] */
        if (holder != 0) {
            /* 0x0063fac8: ECX forwarded from 0x0063fa3c */
            cm_fn_0051c410(holder, 0);
            CM_FS(u32, 0) = frame.prev;
            return;
        }
        /* 0x0063fa48..0x0063fa77 */
        cm_fn_00944cff(0x009ea23cu, 0, 0,
                       (void *)((u8 *)scratch_words + 0x324),
                       (void *)((u8 *)scratch_words + 0x124));
        hi_buf = (u8 *)scratch_words + 0x324;
        lo_buf = (u8 *)scratch_words + 0x124;
        line = 0x2c0u;
    }

    /* 0x0063fa7d push eax / call 0x0090d130 */
    slot = (u32)cm_fn_0090d130((u32)hi_buf, (u32)lo_buf, line);
    /* 0x0063fa83 push eax / 0x0063fa84 push 0x009870e8 / 0x0063fa89 push
       0x00ae24d0 / 0x0063fa8e call 0x00944c9f.  Only one variadic value is
       pushed here, but the format at 0x009870e8 consumes four: the message
       built by 0x0090d130 plus three values the callee reads from the slots
       the caller never rewrote.  Those slots are the stale 0x0090d130
       argument pushes (0x0063fa7d `push eax`, 0x0063f937 `push edx` and
       0x0063f932 `push 0x29c` / 0x0063fa77 `push 0x2c0`), so the callee
       reads &hi_buf, &lo_buf and the line number there.  The reconstruction
       supplies those three values explicitly. */
    cm_fn_00944c9f((u32)&DAT_00ae24d0, 0x009870e8u, slot, (u32)hi_buf,
                   (u32)lo_buf, line);
    /* 0x0063fa93..0x0063fa9f */
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)&DAT_00ae24d0, 0);
    /* 0x0063faa7 */
    DAT_00b67a34 = 0;
    /* 0x0063fab3 mov ecx, [esp+0x424] / mov fs:[0], ecx */
    CM_FS(u32, 0) = frame.prev;
}
