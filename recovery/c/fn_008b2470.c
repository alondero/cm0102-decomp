/* Preferred-image VA 0x008b2470, image base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Region [0x008b2470,0x008b2880) sha256
   94e1bfc7a6f1d6c736d759824d109c769ea4a2b458d4241d545b0a00b17d469a;
   executable end 0x008b2873 exclusive (1027 bytes), then padding to 0x008b2880.
   THISCALL: ECX receiver (ebp), three physical DWORD stack arguments at S+4
   (arg1), S+8 (arg2) and S+0xc (arg3); callee cleanup RET 0xc at 0x008b2870.
   arg2 is a NULLABLE club POINTER (0x008b2476 load of S+8, EBX zeroed 0x247e,
   CMP/JE 0x2482/0x2491 skips the first traversal when null; non-null forwarded
   unchanged to 0x00541f90, which adds 0xd7 and reads club storage), never a
   small selector. arg3 is nullable and is tested only after the first traversal
   (0x008b257d/0x2584/0x2586) and forwarded as the first physical stack word of
   0x008b2880. arg1 is dereferenced only after the arg3-null exit guard.
   Return interface (exact independent verdict required_C_return_interface):
   the only pre-final normal exit, 0x008b2586 -> 0x008b2866 when arg3 == 0,
   leaves the residual full EAX = 0 (the first traversal's counter reaches 0,
   or arg2 was 0 and EAX came from the 0x008b2476 load); it returns that
   evidenced zero. The final edge 0x008b285d/285e/285f/2861 pushes arg1 then
   arg3, sets ECX = receiver and CALLs u32 0x008b2880(receiver,arg3,arg1),
   whose full EAX flows through the shared epilogue 0x008b2866..0x008b2870.
   No unconditional void, no blanket zero on the final edge, no low-byte
   narrowing. 0x00690890 keeps its u32 declaration and returned call.
   Frame: L = S-0x4d0 (0x008b2470). After the EBX/EBP/ESI/EDI pushes B = L-0x10.
   ONE contiguous uninitialized 0x4d0-byte scratch extent, raw offsets exactly
   as addressed:
     L+0x002 B+0x12 byte flag, set 0xff at 0x008b2487, rewritten 0x008b26e7
     L+0x003 B+0x13 byte flag, set 0xff at 0x008b248c, rewritten 0x008b2636
     L+0x004 B+0x14 DWORD traversal counter (0x008b24aa/256b, 0x008b270b/283d)
     L+0x008 L+8     0x32 DWORD club rows written by 0x00541f90 (arg2 = L+8)
     L+0x0d0 B+0xe0  256-byte splitpath/formatter buffer
     L+0x1d0 B+0x1e0 256-byte splitpath/formatter buffer
     L+0x2d0 B+0x2e0 256-byte splitpath/formatter buffer
     L+0x3d0 B+0x3e0 256-byte splitpath/formatter buffer
   The four buffers are adjacent (each 0x100 apart) and nothing is initialized
   by this translation; uninitialized bytes stay uninitialized.
   Diagnostic call order is identical at all four sites: 0x00944cff, then the
   argument-free 0x0090d130, then the six-word 0x00944c9f, then the three-word
   0x005e8290. The three early pushed words (line constant, two buffer
   addresses) belong to 0x00944c9f; 0x0090d130 owns no argument word and pops
   none (MOV EAX,0x00ad4394; RET). Each site keeps the verdict's exact buffer
   order in both the 0x00944cff words and the 0x00944c9f variadic words.
   Preserved original bugs: the diagnostic joins fall through a byte read at
   [EAX+0x15] with EAX = 0 (0x008b2630 and 0x008b26e1) and the 0x008b2829
   dest+0x8b4f60 call runs with a null receiver on the diagnostic path; no safe
   fallback is added. Receiver field +0x651d is a separately loaded DWORD
   pointer and every original reload of it is retained. Row address arithmetic
   is the original wrapped DWORD sequence. All callees remain explicit
   original-code boundaries; no composed substitute is claimed. */
#include "cmre.h"
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

/* Five caller-cleaned words, bare RET at 0x00944e45. */
extern void CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory,
                                    u32 name, u32 extension);
/* Argument-free: MOV EAX,0x00ad4394; RET at 0x0090d130. */
extern u32 CM_CDECL cm_fn_0090d130(void);
/* destination=[EBP+8], format=[EBP+12], vararg cursor [EBP+16], bare RET. */
extern void CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
/* Three words, bare RET epilogues at 0x005e8878/0x005e889d. */
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);
/* Caller-cleaned (club, output, mode); null club returns EAX=0, else EAX=1;
   writes at most `mode == 0 ? 50 : 100` DWORD rows to output. */
extern u32 CM_CDECL cm_fn_00541f90(u32 club, u32 output, u32 mode);
/* ECX receiver from [0x00b63d28], RET 8, reads entry DWORD and BYTE. */
extern u32 CM_THISCALL cm_fn_0062c660(u32 self, u32 entry, u32 value);
/* ECX address receiver, no stack words, bare RET, EAX sign-extended/0x100. */
extern u32 CM_THISCALL cm_fn_008b4f60(u32 self);
/* Two physical stack words (arg3 first, then arg1), ECX receiver, RET 8. */
extern u32 CM_THISCALL cm_fn_008b2880(u32 self, u32 first, u32 second);

/* Original row-address sequence (0x008b25b1, 0x008b265d, 0x008b27ac):
   lea edx,[eax+eax*4]; movsx eax,cl; lea ecx,[eax+edx*2+7];
   mov eax,ecx; shl eax,5; sub eax,ecx; lea ecx,[ebp+eax*4]; add eax,ecx
   = receiver + (sel + 10*j + 7)*31*5, all 32-bit wrapping. */
#define ROW_ADDR(jval, selv) \
    (receiver + ((((selv) + (jval) * 10u) + 7u) * 0x1fu * 5u))

u32 CM_THISCALL cm_fn_008b2470(u32 receiver, u32 arg1, u32 arg2, u32 arg3)
{
    u32 scratch[0x4d0 / 4];
    u32 l = (u32)scratch;
    u32 rowp, row, obj, cur, base, index, addr, value, a1, version;
    u8 sel, cl;

    B(l + 2) = 0xff;
    B(l + 3) = 0xff;
    if (arg2 != 0) {
        cm_fn_00541f90(arg2, l + 8, 0);
        rowp = l + 8;
        D(l + 4) = 0x32;
        for (;;) {
            row = D(rowp);
            if (row != 0) {
                cur = D(0x00ae2364);
                value = D(row);
                if ((s32)value < (s32)cur) {
                    base = D(0x00b63d28);
                    cm_fn_0062c660(base, row, 0x32);
                    obj = D(row + 0x61);
                    if (obj == 0) {
                        cm_fn_00944cff(0x00a8ccd0, 0, 0, l + 0xd0, l + 0x2d0);
                        version = cm_fn_0090d130();
                        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version,
                                       l + 0xd0, l + 0x2d0, 0x34c);
                        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
                        D(0x00b67a34) = 0;
                    } else {
                        cl = B(obj + 0xf);
                        index = D(obj);
                        if ((s32)(s8)cl < 0xf) {
                            base = D(receiver + 0x651d);
                            *(volatile u8 *)(base + index * 3u * 2u) = 1;
                        } else {
                            base = D(receiver + 0x651d);
                            *(volatile u8 *)(base + index * 3u * 2u) = 6;
                        }
                    }
                }
            }
            value = D(l + 4);
            rowp += 4;
            value -= 1;
            D(l + 4) = value;
            if (value == 0) break;
        }
    }
    if (arg3 == 0) return 0;

    sel = 0;
    for (;;) {
        cur = D(0x00ae2364);
        cl = sel;
        cur -= D(arg1);
        cur -= 1;
        if ((s32)cur >= 0 && (s32)cur < 0x10) {
            if ((s8)sel < 0 || (s8)sel >= 0xa) cl = 1;
            addr = ROW_ADDR(cur, (u32)(s32)(s8)cl);
        } else {
            cm_fn_00944cff(0x00a8ccd0, 0, 0, l + 0x2d0, l + 0xd0);
            version = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version,
                           l + 0x2d0, l + 0xd0, 0x68f);
            cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
            addr = 0;
            D(0x00b67a34) = addr;
        }
        if (B(addr + 0x15) == 6) {
            B(l + 3) = sel;
        } else {
            cur = D(0x00ae2364);
            value = D(arg1);
            cl = sel;
            cur -= value;
            cur -= 1;
            if ((s32)cur >= 0 && (s32)cur < 0x10) {
                if ((s8)sel < 0 || (s8)sel >= 0xa) cl = 1;
                addr = ROW_ADDR(cur, (u32)(s32)(s8)cl);
            } else {
                cm_fn_00944cff(0x00a8ccd0, 0, 0, l + 0x3d0, l + 0x1d0);
                version = cm_fn_0090d130();
                cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version,
                               l + 0x3d0, l + 0x1d0, 0x68f);
                cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
                D(0x00b67a34) = 0;
                addr = 0;
            }
            if (B(addr + 0x15) == 1) B(l + 2) = sel;
        }
        sel = (u8)(sel + 1u);
        if ((s8)sel >= 0xa) break;
    }

    cm_fn_00541f90(arg3, l + 8, 0);
    rowp = l + 8;
    D(l + 4) = 0x32;
    for (;;) {
        row = D(rowp);
        if (row == 0) goto row_join;
        obj = D(row + 0x61);
        if (obj == 0) goto row_join;
        value = D(row);
        cur = D(0x00ae2364);
        if ((s32)value >= (s32)cur) goto row_join;
        if ((s32)(s8)B(obj + 0xf) < 0xf) goto flag_from_12;
        cl = B(l + 3);
        if (cl == 0xff) goto flag_from_12;
        index = D(obj);
        base = D(receiver + 0x651d);
        *(volatile u8 *)(base + index * 3u * 2u) = cl;
        goto row_body;
    flag_from_12:
        cl = B(l + 2);
        index = D(obj);
        if (cl == 0xff) {
            base = D(receiver + 0x651d);
            *(volatile u8 *)(base + index * 3u * 2u) = 1;
        } else {
            base = D(receiver + 0x651d);
            *(volatile u8 *)(base + index * 3u * 2u) = cl;
        }
    row_body:
        obj = D(row + 0x61);
        base = D(receiver + 0x651d);
        index = D(obj);
        a1 = arg1;
        cl = B(base + index * 3u * 2u);
        cur = D(0x00ae2364);
        cur -= D(a1);
        cur -= 1;
        if ((s32)cur >= 0 && (s32)cur < 0x10) {
            if ((s8)cl < 0 || (s8)cl >= 0xa) cl = 1;
            addr = ROW_ADDR(cur, (u32)(s32)(s8)cl);
        } else {
            cm_fn_00944cff(0x00a8ccd0, 0, 0, l + 0x1d0, l + 0x3d0);
            version = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version,
                           l + 0x1d0, l + 0x3d0, 0x68f);
            cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
            D(0x00b67a34) = 0;
            addr = 0;
        }
        value = cm_fn_008b4f60(addr);
        base = D(0x00b63d28);
        cm_fn_0062c660(base, row, value);
    row_join:
        value = D(l + 4);
        rowp += 4;
        value -= 1;
        D(l + 4) = value;
        if (value == 0) break;
    }
    return cm_fn_008b2880(receiver, arg3, arg1);
}
