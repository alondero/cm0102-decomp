#include "cmre.h"

/* Preferred-image VA 00682b30, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch210: outer L0/inner-or-rank L4/self L8/searchbound Lc,
   diagnostic buffers L10/L110. Preserve original calloc contents and reloads.
   Qsort retains incoming EAX for short counts; its physical EDX supplies the
   upper bytes of the rank slot after the original MOV DX,count. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_CDECL cm_fn_00945501(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern unsigned long long CM_CDECL __attribute__((regparm(1)))
    cm_fn_00945333(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_006834f0(u32, u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);

u32 CM_THISCALL cm_fn_00682b30(u32 self, volatile u32 input_arg)
{
    u8 scratch[0x210];
    u32 l = (u32)scratch, buffer, input, eax, edx, outer, inner;
    u32 offset, row, vector, key, search, bound, item, source, destination;
    u32 count, next, rank;
    u16 count_word, word;
    u8 byte;
    unsigned long long result;
    count = (u32)(s32)(s16)W(self + 0x3e);
    D(l + 8) = self;
    eax = cm_fn_00945501(count, 0x3b);
    buffer = eax;
    if (buffer == 0) {
        cm_fn_00944cff(0x009f15f4, 0, 0, l + 0x110, l + 0x10);
        eax = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, eax,
                       l + 0x110, l + 0x10, 0x17d);
        eax = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return eax;
    }
    edx = 0;
    count_word = W(self + 0x3e);
    input = input_arg;
    D(l) = edx;
    if ((s16)count_word <= 0) goto sort;
    do {
        outer = (u32)(s32)(s16)(u16)edx;
        offset = outer * 0x3bu;
        eax = D(input + offset);
        D(buffer + offset) = eax;
        count_word = W(self + 0x3e);
        eax = 0;
        D(l + 4) = eax;
        if ((s16)count_word <= 0) goto next_outer;
        do {
            if ((u16)edx == (u16)eax) goto next_inner;
            inner = (u32)(s32)(s16)(u16)eax;
            row = input + inner * 0x3bu;
            word = W(input + offset + 0x19);
            if (word != W(row + 0x19)) goto next_inner;
            bound = (u32)(s32)(s16)count_word - 1u;
            search = 0;
            D(l + 0xc) = bound;
            if ((s32)bound <= 0) goto next_inner;
            vector = D(input + offset + 5);
            key = D(row);
            item = 0;
            for (;;) {
                if (D(vector + item * 14u) == key) break;
                bound = D(l + 0xc);
                ++search;
                item = (u32)(s32)(s16)(u16)search;
                if ((s32)item >= (s32)bound) {
                    input = input_arg;
                    goto next_inner;
                }
            }
            item = (u32)(s32)(s16)(u16)search;
            byte = B(buffer + offset + 0x11);
            input = input_arg;
            item *= 14;
            byte = (u8)(byte + B(vector + item + 4));
            B(buffer + offset + 0x11) = byte;
            source = D(input + offset + 5);
            byte = B(buffer + offset + 0x12);
            byte = (u8)(byte + B(source + item + 5));
            B(buffer + offset + 0x12) = byte;
            source = D(input + offset + 5);
            byte = B(buffer + offset + 0x13);
            byte = (u8)(byte + B(source + item + 6));
            B(buffer + offset + 0x13) = byte;
            source = D(input + offset + 5);
            byte = B(buffer + offset + 0x14);
            byte = (u8)(byte + B(source + item + 7));
            B(buffer + offset + 0x14) = byte;
            source = D(input + offset + 5);
            word = W(source + item + 8);
            W(buffer + offset + 0x15) = (u16)(W(buffer + offset + 0x15) + word);
            source = D(input + offset + 5);
            word = W(source + item + 0xa);
            W(buffer + offset + 0x17) = (u16)(W(buffer + offset + 0x17) + word);
            source = D(input + offset + 5);
            word = W(source + item + 0xc);
            W(buffer + offset + 0x19) = (u16)(W(buffer + offset + 0x19) + word);
next_inner:
            self = D(l + 8);
            eax = D(l + 4);
            edx = D(l);
            ++eax;
            count_word = W(self + 0x3e);
            D(l + 4) = eax;
        } while ((s16)(u16)eax < (s16)count_word);
next_outer:
        ++edx;
        count_word = W(self + 0x3e);
        D(l) = edx;
    } while ((s16)(u16)edx < (s16)count_word);
sort:
    count = (u32)(s32)(s16)W(self + 0x3e);
    result = cm_fn_00945333(eax, buffer, count, 0x3b, 0x006834f0);
    count_word = W(self + 0x3e);
    edx = ((u32)(result >> 32) & 0xffff0000u) | count_word;
    outer = 0;
    D(l + 4) = edx;
    D(l) = outer;
    if ((s16)count_word <= 0) goto release;
    do {
        inner = 0;
        if ((s16)count_word <= 0) goto next_rank;
        key = D(buffer + (u32)(s32)(s16)(u16)outer * 0x3bu);
        do {
            row = input + (u32)(s32)(s16)(u16)inner * 0x3bu;
            if (key == D(row)) {
                rank = B(l + 4);
                B(row + 4) = (u8)rank;
                break;
            }
            ++inner;
        } while ((s16)(u16)inner < (s16)count_word);
        outer = D(l);
next_rank:
        self = D(l + 8);
        outer = (u32)(s32)(s16)(u16)outer;
        count = (u32)(s32)(s16)W(self + 0x3e);
        next = outer + 1u;
        if ((s32)next < (s32)count) {
            destination = buffer + next * 0x3bu;
            source = buffer + outer * 0x3bu;
            eax = cm_fn_006834f0(source, destination);
            if (eax != 0) D(l + 4) = D(l + 4) - 1u;
        }
        count_word = W(self + 0x3e);
        ++outer;
        D(l) = outer;
    } while ((s16)(u16)outer < (s16)count_word);
release:
    return (u32)cm_fn_009452ca(buffer);
}
