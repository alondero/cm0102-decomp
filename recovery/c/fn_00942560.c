/* Preferred-image VA 0x00942560, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0093c590(u32 self, u32 count);
extern u32 CM_CDECL cm_fn_009415c0(u32 self);
extern void CM_CDECL cm_fn_009415e0(u32 input, u32 destination, s32 length);
extern u32 CM_CDECL cm_fn_00944250(u32 self, u32 size, u32 index);
extern u32 CM_CDECL cm_fn_009443a0(u32 arg1, u32 arg2);
extern u32 CM_CDECL cm_fn_00944480(u32 arg1, u32 arg2, u32 arg3);

u32 CM_CDECL cm_fn_00942560(u32 allocator, u32 input, u32 copy, u32 a4, u32 a5, u32 a6, u32 a7)
{
    u32 object, count, n = 0, words, used, value, flags, extra;
    u32 x, y, x_origin, y_origin, codes, i, offset, base, accumulator, delta, saved, repeat;
    u16 ax, cx;
    object = cm_fn_00944250(allocator, 0x73, 0);
    D(object) = allocator;
    D(object + 0x3c) = 0;
    D(object + 0x38) = 0;
    D(object + 0x54) = 0;
    D(object + 0x50) = 0;
    D(object + 0x4c) = 0;
    value = cm_fn_009415c0(input);
    count = (u32)(s32)(s16)value;
    W(object + 0x34) = (u16)count;
    value = cm_fn_009415c0(input); W(object + 0x68) = (u16)value;
    value = cm_fn_009415c0(input); W(object + 0x6a) = (u16)value;
    value = cm_fn_009415c0(input); W(object + 0x6c) = (u16)value;
    value = cm_fn_009415c0(input); W(object + 0x6e) = (u16)value;
    D(object + 0x58) = 0;
    D(object + 0x64) = 0;
    D(object + 0x60) = 0;
    D(object + 0x0a) = 0;
    D(object + 0x0e) = 0;
    W(object + 0x32) = 2;
    W(object + 4) = 0;
    D(object + 6) = 0;
    if ((s32)count < 0) {
        base = D(object);
        used = 0;
        D(object + 0x5c) = 0x400;
        words = cm_fn_009443a0(base, 0x800);
        do {
            value = D(object + 0x5c);
            if ((s32)used >= (s32)(value - 10u)) {
                /* CDQ; SUB EAX,EDX; SAR EAX,1 rounds signed division toward zero. */
                extra = (u32)((s32)(value - ((s32)value < 0 ? 0xffffffffu : 0u)) >> 1) + value;
                base = D(object);
                D(object + 0x5c) = extra;
                words = cm_fn_00944480(base, words, extra + extra);
            }
            value = cm_fn_009415c0(input);
            W(words + used * 2u) = (u16)value;
            flags = value;
            extra = (value & 0x100u) >> 8;
            ++used;
            value = cm_fn_009415c0(input);
            W(words + used * 2u) = (u16)value;
            ++used;
            value = cm_fn_009415c0(input);
            if (flags & 1u) {
                W(words + used * 2u) = (u16)value;
                ++used;
                value = cm_fn_009415c0(input);
            }
            W(words + used * 2u) = (u16)value;
            ++used;
            if (flags & 8u) {
                value = cm_fn_009415c0(input);
                W(words + used * 2u) = (u16)value; ++used;
            } else if (flags & 0x40u) {
                value = cm_fn_009415c0(input);
                W(words + used * 2u) = (u16)value; ++used;
                value = cm_fn_009415c0(input);
                W(words + used * 2u) = (u16)value; ++used;
            } else if (flags & 0x80u) {
                value = cm_fn_009415c0(input);
                W(words + used * 2u) = (u16)value; ++used;
                value = cm_fn_009415c0(input);
                W(words + used * 2u) = (u16)value; ++used;
                value = cm_fn_009415c0(input);
                W(words + used * 2u) = (u16)value; ++used;
                value = cm_fn_009415c0(input);
                W(words + used * 2u) = (u16)value; ++used;
            }
        } while (flags & 0x20u);
        D(object + 0x64) = 0;
        if (extra) {
            value = cm_fn_009415c0(input);
            value = (u32)(s32)(s16)value;
            D(object + 0x64) = value;
            if ((u8)copy) {
                if ((s32)value > 0) {
                    base = D(object);
                    value = cm_fn_00944250(base, value, 3);
                    extra = D(object + 0x64);
                    D(object + 0x60) = value;
                    cm_fn_009415e0(input, value, (s32)extra);
                }
            } else {
                offset = D(input + 4); offset += value; D(input + 4) = offset;
                D(object + 0x64) = 0;
            }
        }
        cm_fn_0093c590(object, 0);
        x = D(object + 0x40);
        y = D(object + 0x44);
        D(object + 0x54) = words;
        D(object + 0x58) = used;
    } else if ((s32)count > 0) {
        if ((s32)count <= 8) {
            D(object + 0x38) = object + 0x12;
            D(object + 0x3c) = object + 0x22;
        } else {
            base = D(object);
            value = cm_fn_009443a0(base, count * 4u);
            D(object + 0x38) = value;
            D(object + 0x3c) = value + count * 2u;
        }
        value = 0;
        for (i = 0; (s32)i < (s32)count; ++i) {
            base = D(object + 0x38);
            W(base + i * 2u) = (u16)value;
            value = cm_fn_009415c0(input);
            base = D(object + 0x3c);
            W(base + i * 2u) = (u16)value;
            ++value;
        }
        n = (u32)(s32)(s16)value;
        value = cm_fn_009415c0(input);
        value = (u32)(s32)(s16)value;
        D(object + 0x64) = value;
        if ((u8)copy) {
            if ((s32)value > 0) {
                base = D(object);
                value = cm_fn_00944250(base, value, 3);
                extra = D(object + 0x64);
                D(object + 0x60) = value;
                cm_fn_009415e0(input, value, (s32)extra);
            }
        } else {
            offset = D(input + 4); offset += value; D(input + 4) = offset;
            D(object + 0x64) = 0;
        }
        cm_fn_0093c590(object, n);
        x = D(object + 0x40); x_origin = x;
        y = D(object + 0x44); y_origin = y; codes = D(object + 0x48);
        W(object + 4) = (u16)count;
        D(object + 6) = (u32)(s32)(s16)n;
        i = 0;
        while ((s32)i < (s32)n) {
            offset = D(input + 4); base = D(input);
            ++i;
            flags = B(offset + base);
            B(codes + i - 1u) = (u8)flags;
            offset = D(input + 4); ++offset;
            D(input + 4) = offset;
            if (flags & 8u) {
                base = D(input);
                repeat = B(offset + base); ++offset;
                D(input + 4) = offset;
                while ((s32)repeat > 0) {
                    --repeat;
                    if ((s32)i >= (s32)n) goto flags_done;
                    ++i; B(codes + i - 1u) = (u8)flags;
                }
            }
        }
flags_done:
        accumulator = 0; i = 0;
        while ((s32)i < (s32)n) {
            flags = B(codes + i);
            if (flags & 2u) {
                base = D(input);
                offset = D(input + 4);
                /* MOVZX BP preserves the upper word loaded by MOV EBP,[ESI]. */
                delta = (base & 0xffff0000u) | B(base + offset);
                if (flags & 0x10u) accumulator += delta; else accumulator -= delta;
                ++offset; D(input + 4) = offset;
            } else if (!(flags & 0x10u)) {
                offset = D(input + 4); base = D(input);
                saved = B(base + offset); ++offset; D(input + 4) = offset;
                delta = B(base + offset); ++offset;
                delta |= saved << 8; D(input + 4) = offset;
                accumulator += delta;
            }
            ++i; W(x) = (u16)accumulator; x += 2;
        }
        accumulator = 0; i = 0; delta = i;
        while ((s32)i < (s32)n) {
            flags = B(codes + i);
            delta = (delta & 0xffffff00u) | (flags & 1u);
            B(codes + i) = (u8)delta;
            if (flags & 4u) {
                base = D(input); offset = D(input + 4);
                delta = (delta & 0xffff0000u) | B(base + offset);
                if (flags & 0x20u) accumulator += delta; else accumulator -= delta;
                ++offset; D(input + 4) = offset;
            } else if (!(flags & 0x20u)) {
                offset = D(input + 4); base = D(input);
                saved = B(base + offset); ++offset; D(input + 4) = offset;
                delta = B(base + offset); ++offset;
                delta |= saved << 8; D(input + 4) = offset;
                accumulator += delta;
            }
            ++i; W(y) = (u16)accumulator; y += 2;
        }
        x = x_origin; y = y_origin;
    } else goto done;
    W(y + n * 2u) = 0;
    ax = W(object + 0x68); ax = (u16)(ax - (u16)a4);
    W(x + n * 2u) = ax;
    W(y + n * 2u + 2u) = 0;
    cx = W(x + n * 2u); cx = (u16)(cx + (u16)a5);
    W(x + n * 2u + 2u) = cx;
    value = (u32)(s32)(s16)W(x + n * 2u);
    value += (u32)(s32)(s16)cx;
    cx = (u16)a6; cx = (u16)(cx + W(object + 0x6e));
    value = (u32)((s32)value >> 1);
    W(y + n * 2u + 4u) = cx;
    W(x + n * 2u + 4u) = (u16)value;
    cx = W(y + n * 2u + 4u); cx = (u16)(cx - (u16)a7);
    W(y + n * 2u + 6u) = cx;
    W(x + n * 2u + 6u) = (u16)value;
done:
    W(object + 0x36) = (u16)n;
    return object;
}
