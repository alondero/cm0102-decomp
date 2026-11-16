/* fn_009050d0  VA 0x009050d0 (preferred image base 0x00400000).
   Translated instruction-by-instruction from the original listing.

   Calling contract: STD6 / RET18 -- six dword stack arguments, the callee
   pops 0x18 itself (stdcall).

   Original frame L = esp after `sub esp,0xc`; the 12-byte search key handed
   to 0x00946464 is the contiguous scratch at L:
     L+0x00..L+0x03  key dword 0 (left uninitialized)
     L+0x04..L+0x07  key dword, written from arg3
     L+0x08          key byte, written 0xff
     L+0x09..L+0x0a  key s16, unaligned, left uninitialized
     L+0x0b          key s8, left uninitialized
   It is one allocation so those offsets stay adjacent, and the unaligned
   word/byte views stay unaligned.

   0x00946464 is the original 140-byte bsearch-style dispatcher; the original
   0x00905ce0 comparator is passed through to it and never reimplemented. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip_status, u8 flag, u32 person, u32 force);
extern u32 cm_fn_00946464(u32 param_1, u32 param_2, u32 param_3, int param_4, void *param_5);

typedef s16 cm_count_word __attribute__((may_alias));

u32 CM_STDCALL cm_fn_009050d0(u32 base, volatile u32 count, u32 key, u8 count4, u32 flag5, u32 flag6)
{
    /* L+0x00 .. L+0x0b: search key. Only the listing's initialized bytes are
       written; the rest stay uninitialized exactly as the original. */
    volatile u8 scratch[12];
    u32 end;
    u32 cur;
    u8 dl;
    u32 ecx;
    u32 signed_count;
    u8 bl;

    /* 0x9050d3 reads only the low word of the incoming DWORD home.
       Preserve that signed count for the dispatcher after overwriting home. */
    signed_count = (u32)(s32)*(volatile cm_count_word *)&count;
    ecx = signed_count;
    ecx = ecx + ecx * 2;
    end = base + ecx * 4 - 0xc;
    count = end;                                  /* 0x9050f9: DWORD arg2 home */

    /* 0x9050fd: mov [L+4], edx (arg3).  0x905101: mov byte [L+8], 0xff. */
    *(volatile u32 *)(scratch + 4) = key;
    *(volatile u8 *)(scratch + 8) = 0xff;

    /* 0x905106: call original bsearch dispatcher 0x00946464(key, base,
       movsx(arg2), 0xc, original comparator 0x905ce0). */
    cur = cm_fn_00946464((u32)scratch, base, signed_count, 0xc,
                         (void *)0x00905ce0u);
    if (cur == 0) return 0;                       /* 0x905110/0x905112 */

    dl = *(volatile u8 *)(scratch + 8);           /* 0x905118 */

    /* 0x90511c..0x905167: backward equality scan. */
    for (;;) {
        u32 old;
        u32 newp;
        s32 d;
        u8 cl;

        if (cur <= base) break;                   /* 0x90511c jbe 0x90516a */
        old = cur;                                /* 0x905120 */
        d = (s32)(*(volatile u32 *)(scratch + 4)
          - *(u32 *)(old - 8));                  /* 0x905124/0x90512a */
        newp = old - 0xc;                         /* 0x905127 */
        cur = newp;
        if (d != 0) { cur = newp + 0xc; break; }  /* 0x90515d -> 0x905167 */
        if (dl == 0xff) goto scan_161;            /* 0x90512e */
        cl = *(u8 *)(newp + 8);                   /* 0x905133 */
        if (cl == 0xff) goto scan_161;            /* 0x905136 */
        d = (s32)*(s16 *)(newp + 9)
          - (s32)*(volatile s16 *)(scratch + 9);  /* 0x90513b/0x90513f */
        if (d != 0) { cur = newp + 0xc; break; }  /* 0x905146 -> 0x90515d */
        d = (s32)*(volatile s8 *)(scratch + 0xb)
          - (s32)*(s8 *)(newp + 0xb);             /* 0x905148/0x90514c */
        if (d != 0) { cur = newp + 0xc; break; }  /* 0x905153 */
        d = (s32)(s8)cl - (s32)(s8)dl;            /* 0x905155/0x905158 */
        if (d != 0) { cur = newp + 0xc; break; }  /* 0x90515d -> 0x905167 */
    scan_161:
        if (cur > base) continue;                 /* 0x905161 ja 0x905120 */
        break;
    }

    /* 0x90516a: mov bl, byte arg4 ; dec bl. */
    bl = (u8)(count4 - 1);

    if (cur >= end) goto loop_done;               /* 0x905170 jae 0x9051cf */

    {
        u32 f6 = flag6;                           /* 0x905174 mov edi,[esp+0x34] */
        u32 f5 = flag5;                           /* 0x905178 mov ebp,[esp+0x30] */

        for (;;) {
            u32 p;

            /* 0x90517c */
            if (f5 != 0) {
                p = *(u32 *)cur;                  /* 0x905180 */
                if (p != 0 && *(u32 *)(p + 0x1db) != 0xffffffffu) {
                    cur += 0xc;                   /* 0x90518f */
                    goto loop_cont;               /* 0x905192 */
                }
            }

            /* 0x905194 */
            if (f6 != 0) {
                p = *(u32 *)cur;                  /* 0x905198 */
                if (p != 0) {                     /* 0x90519a */
                    u32 r = cm_fn_00600a50(p, 0, 1, 0, 0);  /* 0x9051a7 */
                    if (r != 0) {                 /* 0x9051af */
                        dl = *(volatile u8 *)(scratch + 8);  /* 0x9051b3 */
                        cur += 0xc;               /* 0x9051b7 */
                        goto loop_cont;           /* 0x9051ba */
                    }
                    dl = *(volatile u8 *)(scratch + 8);      /* 0x9051bc */
                }
            }

            /* 0x9051c0 */
            if (bl == 0) goto found;              /* 0x9051c2 je 0x9051d3 */
            cur += 0xc;                           /* 0x9051c4 */
            bl = (u8)(bl - 1);                    /* 0x9051c7 */

        loop_cont:
            /* 0x9051c9: cmp esi,[esp+0x24] (the overwritten arg2 home). */
            if (cur < count) continue;
            break;
        }
    }

loop_done:
    /* 0x9051cf */
    if (bl != 0) return 0;                        /* 0x9051d1 jne 0x90521d */

found:
    /* 0x9051d3 */
    {
        s32 d;
        u8 cl;

        d = (s32)(*(volatile u32 *)(scratch + 4)
          - *(u32 *)(cur + 4));                  /* 0x9051d3/0x9051da */
        if (d != 0) return 0;                     /* 0x90520d jne 0x90521d */
        if (dl == 0xff) goto ret_slot;            /* 0x9051de */
        cl = *(u8 *)(cur + 8);                    /* 0x9051e3 */
        if (cl == 0xff) goto ret_slot;            /* 0x9051e6 */
        d = (s32)*(s16 *)(cur + 9)
          - (s32)*(volatile s16 *)(scratch + 9);  /* 0x9051eb/0x9051ef */
        if (d != 0) return 0;                     /* 0x9051f6 */
        d = (s32)*(volatile s8 *)(scratch + 0xb)
          - (s32)*(s8 *)(cur + 0xb);              /* 0x9051f8/0x9051fc */
        if (d != 0) return 0;                     /* 0x905203 */
        d = (s32)(s8)cl - (s32)(s8)dl;            /* 0x905205/0x905208 */
        if (d != 0) return 0;                     /* 0x90520f */
    }

ret_slot:
    return *(u32 *)cur;                           /* 0x905211 */
}
