/* fn_005a0050 -- plain C reconstruction of the original 32-bit routine at
 * preferred-image VA 0x005a0050 (image base 0x00400000, cm0102.exe 3.9.68).
 *
 * Frame reconstruction (ESP == P immediately after the 0x945610 probe):
 *   0x005a0050  mov eax,0x1388
 *   0x005a0055  call 0x945610          <- CRT stack probe: reserves the 0x1388
 *                                         byte frame (net ESP -= 0x1388) and
 *                                         migrates the caller's return slot.
 *   0x005a005a  mov eax,dword ptr [esp+0x1390]   -> arg2 (flag)
 *   0x005a0061  push esi
 *   0x005a0062  test eax,eax / jne 0x5a00d6
 *   0x005a0066  mov ecx,dword ptr [esp+0x1390]   -> arg1 (object)
 *   ...
 *   0x005a01dd  mov ecx,dword ptr [esp+0x1398]   -> arg3 (object)
 *   0x005a01e9  call 0x4ae660                    -> (arg3, message buffer)
 *   0x005a01ef  add esp,0x1388 / ret 0xc         -> frame release, three args
 * so, with P = ESP after the probe:
 *   [P+0x138c] = arg1, [P+0x1390] = arg2, [P+0x1394] = arg3 and the 0x1388
 *   byte message buffer lives at [P] (the bottom of the probed frame).
 *
 * Stack-probe obligation (the item this function was HELD for) -- resolved:
 *   0x945610 is the CRT stack-probe helper (__chkstk/__alloca_probe).  Its
 *   entire effect for this routine is to reserve and commit the 0x1388-byte
 *   frame that the following code indexes: the probe returns with ESP moved
 *   down by exactly 0x1388 (net, seen by the caller), which is why the final
 *   "add esp,0x1388" restores ESP to the return slot and why the slots
 *   [P+0x138c], [P+0x1390], [P+0x1394] land on arg1, arg2 and arg3 and the
 *   message buffer lands at [P], the bottom of the probed frame.  It takes no
 *   argument in memory (the size travels in EAX) and writes nothing outside the
 *   frame it creates, so plain C does not need an EAX/stack-probe transport:
 *   the same 0x1388 bytes are reserved as the local message buffer below, and
 *   every original stack slot is addressed through that frame instead of
 *   through a raw ESP displacement.  The observable boundary calls (0x785830,
 *   0x66f4e0, 0x4ae660), their arguments, the message buffer contents and the
 *   final register state (including the "ret 0xc" argument cleanup) are
 *   unchanged: cmre test passes at -O0 and -O2.
 */
#include "cmre.h"

/* Original globals: 0x00ae23b0 is a DWORD pointer to the stadium table. */
#define DAT_00ae23b0 CM_GLOBAL(volatile u32, 0x00ae23b0)

/* Original DWORD target tables (read exactly as the original dispatches). */
#define JT1_BASE 0x005a01f8
#define JT2_BASE 0x005a020c

/* Original message strings (addresses pushed as immediates in the original). */
#define S_009c9740 ((char *)0x009c9740)
#define S_009c97ac ((char *)0x009c97ac)
#define S_009c9800 ((char *)0x009c9800)
#define S_009c987c ((char *)0x009c987c)
#define S_009c98cc ((char *)0x009c98cc)
#define S_009c9958 ((char *)0x009c9958)
#define S_009c9a00 ((char *)0x009c9a00)
#define S_009c9ac8 ((char *)0x009c9ac8)
#define S_009c9b40 ((char *)0x009c9b40)
#define S_009c9b64 ((char *)0x009c9b64)
#define S_009c9b80 ((char *)0x009c9b80)
#define S_009c9b9c ((char *)0x009c9b9c)
#define S_009c9bb8 ((char *)0x009c9bb8)
#define S_009c9bd8 ((char *)0x009c9bd8)

extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern char *cm_fn_0066f4e0(int param_1, char *param_2, ...);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);

/* 0x005a01f5: ret 0xc -- the callee pops the three stack arguments, so the
 * entry keeps the original's stack-cleaning convention. */
u32 CM_STDCALL cm_fn_005a0050(void *arg1, u32 arg2, void *arg3)
{
    /* The original message buffer: 0x1388 bytes at the bottom of the probed
     * frame (0x005a0086/0x005a009f/.../0x005a017d all address it). */
    char buffer[0x1388];

    u32 r1;             /* EAX of call 0x785830 with index 1 (kept in ESI) */
    u32 r2;             /* EAX of call 0x785830 with index 2 */
    u32 idx;            /* jump table index */
    u32 tgt;            /* DWORD read from the original target table */
    s32 sv;             /* sign extended AL of the 0x785830 result */
    u32 base;           /* [0x00ae23b0] */
    u32 off;            /* esi * 78 */
    s32 ch;             /* sign extended byte at base + esi*78 + 0x37 */
    char *ptr;          /* base + esi*78 + 4 */

    if (arg2 == 0) {
        /* 0x005a0066: mov ecx,[esp+0x1390] ; 0x005a006d: push 2 ; call */
        r2 = (u32)cm_fn_00785830(arg1, 2);
        /* 0x005a0074: movsx eax,al ; 0x005a0077: add eax,-4 */
        sv = (s32)(s8)(u8)r2;
        idx = (u32)(sv - 4);
        if (idx > 4u) {
            /* 0x005a00c7: default target of the first table */
            cm_fn_0066f4e0((int)buffer, S_009c9b40);
        } else {
            /* 0x005a007f: jmp dword ptr [eax*4 + 0x5a01f8] */
            tgt = CM_GLOBAL(volatile u32, JT1_BASE + idx * 4);
            switch (tgt) {
            case 0x005a009f: /* 0x005a009f: 0x009c9b9c */
                cm_fn_0066f4e0((int)buffer, S_009c9b9c);
                break;
            case 0x005a0095: /* 0x005a0095: 0x009c9bb8 */
                cm_fn_0066f4e0((int)buffer, S_009c9bb8);
                break;
            case 0x005a00ae: /* 0x005a00ae: 0x009c9b80 */
                cm_fn_0066f4e0((int)buffer, S_009c9b80);
                break;
            case 0x005a0086: /* 0x005a0086: 0x009c9bd8 */
                cm_fn_0066f4e0((int)buffer, S_009c9bd8);
                break;
            case 0x005a00bd: /* 0x005a00bd: 0x009c9b64 */
                cm_fn_0066f4e0((int)buffer, S_009c9b64);
                break;
            default:
                break;
            }
        }
    } else {
        /* 0x005a00d7: mov edi,[esp+0x1394] (arg1) ; 0x005a00de: push 1 ; call */
        r1 = (u32)cm_fn_00785830(arg1, 1);
        /* 0x005a00e7: push 2 ; 0x005a00eb: mov esi,eax ; call */
        r2 = (u32)cm_fn_00785830(arg1, 2);
        /* 0x005a00f2: movsx eax,al ; 0x005a00f5: dec eax */
        sv = (s32)(s8)(u8)r2;
        idx = (u32)(sv - 1);
        if (idx <= 7u) {
            /* 0x005a0100: jmp dword ptr [eax*4 + 0x5a020c] */
            tgt = CM_GLOBAL(volatile u32, JT2_BASE + idx * 4);
            switch (tgt) {
            case 0x005a018c: /* 0x005a018c: 0x009c987c + esi */
                cm_fn_0066f4e0((int)buffer, S_009c987c, r1);
                break;
            case 0x005a01a1: /* 0x005a01a1: 0x009c9800 + esi */
                cm_fn_0066f4e0((int)buffer, S_009c9800, r1);
                break;
            case 0x005a01b6: /* 0x005a01b6: 0x009c97ac + esi */
                cm_fn_0066f4e0((int)buffer, S_009c97ac, r1);
                break;
            case 0x005a015c: /* 0x005a015c: 0x009c98cc + char + char + ptr */
                base = DAT_00ae23b0;
                off = r1 * 78u;
                ch = (s8)*(volatile s8 *)(base + off + 0x37u);
                ptr = (char *)(base + off + 4u);
                cm_fn_0066f4e0((int)buffer, S_009c98cc, ch, ch, ptr);
                break;
            case 0x005a0139: /* 0x005a0139: 0x009c9958 + char + char + ptr */
                base = DAT_00ae23b0;
                off = r1 * 78u;
                ch = (s8)*(volatile s8 *)(base + off + 0x37u);
                ptr = (char *)(base + off + 4u);
                cm_fn_0066f4e0((int)buffer, S_009c9958, ch, ch, ptr);
                break;
            case 0x005a0116: /* 0x005a0116: 0x009c9a00 + char + char + ptr */
                base = DAT_00ae23b0;
                off = r1 * 78u;
                ch = (s8)*(volatile s8 *)(base + off + 0x37u);
                ptr = (char *)(base + off + 4u);
                cm_fn_0066f4e0((int)buffer, S_009c9a00, ch, ch, ptr);
                break;
            case 0x005a0107: /* 0x005a0107: 0x009c9ac8 */
                cm_fn_0066f4e0((int)buffer, S_009c9ac8);
                break;
            case 0x005a01cb: /* 0x005a01cb: 0x009c9740 */
                cm_fn_0066f4e0((int)buffer, S_009c9740);
                break;
            default:
                break;
            }
        }
    }

    /* 0x005a01dd: mov ecx,[esp+0x1398] (arg3) ; 0x005a01e4: lea edx,[esp+4]
     * 0x005a01e8: push edx ; 0x005a01e9: call 0x4ae660 */
    return (u32)cm_fn_004ae660(arg3, (u32)(unsigned long)buffer);
}
