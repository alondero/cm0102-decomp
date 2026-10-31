/* Preferred-image VA 00932e10, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern u32 CM_CDECL cm_fn_00945ce6(u32 buffer, u32 size, u32 count, u32 stream);

u32 CM_CDECL cm_fn_00932e10(u32 buffer, u32 size, u32 count, u32 stream)
{
    u32 record, total, accumulated, format, position, run, used, output, file;
    u8 previous, current, marker;

    if (stream == 0)
        return 0;
    if (D(stream + 4u) != 1u)
        return 0;

    record = stream + D(stream + 0xcu) * 0x10cu + 0x29u;
    total = size * count;
    accumulated = D(record);
    D(record) = accumulated + total;
    accumulated = D(stream + 8u);
    format = D(stream + 0x19u);
    D(stream + 8u) = accumulated + total;
    if (format != 4u) {
        file = D(stream);
        return cm_fn_00945ce6(buffer, size, count, file);
    }
    if ((s32)total <= 0)
        return count;

    position = 0;
    do {
        run = D(stream + 0x14u);
        if (run == 0) {
            D(stream + 0x14u) = 1;
            current = B(buffer + position);
            B(stream + 0x18u) = current;
            ++position;
            continue;
        }

        previous = B(stream + 0x18u);
        current = B(buffer + position);
        if (current == previous) {
            if ((s32)(run + 0x80u) < 0xff) {
                D(stream + 0x14u) = run + 1u;
                ++position;
                continue;
            }
        } else if ((s32)run <= 1 && previous <= 0x80u) {
            used = D(stream + 0xa3189u);
            if ((s32)used < 0x30d40) {
                B(stream + used + 0x72445u) = previous;
                used = D(stream + 0xa3189u);
                D(stream + 0xa3189u) = used + 1u;
            } else {
                file = D(stream);
                output = stream + 0x72445u;
                (void)cm_fn_00945ce6(output, 1, used, file);
                previous = B(stream + 0x18u);
                D(stream + 0xa3189u) = 1;
                B(output) = previous;
            }
            goto next_run;
        }

        /* Native 932EA8/932F8B reread the low byte of the run length. */
        marker = B(stream + 0x14u);
        used = D(stream + 0xa3189u);
        marker = (u8)(marker + 0x80u);
        if ((s32)used < 0x30d40) {
            B(stream + used + 0x72445u) = marker;
            used = D(stream + 0xa3189u);
            D(stream + 0xa3189u) = used + 1u;
        } else {
            file = D(stream);
            output = stream + 0x72445u;
            (void)cm_fn_00945ce6(output, 1, used, file);
            D(stream + 0xa3189u) = 1;
            B(output) = marker;
        }

        used = D(stream + 0xa3189u);
        if ((s32)used < 0x30d40) {
            previous = B(stream + 0x18u);
            B(stream + used + 0x72445u) = previous;
            used = D(stream + 0xa3189u);
            D(stream + 0xa3189u) = used + 1u;
        } else {
            file = D(stream);
            output = stream + 0x72445u;
            (void)cm_fn_00945ce6(output, 1, used, file);
            previous = B(stream + 0x18u);
            D(stream + 0xa3189u) = 1;
            B(output) = previous;
        }

    next_run:
        D(stream + 0x14u) = 1;
        current = B(buffer + position);
        ++position;
        B(stream + 0x18u) = current;
    } while ((s32)position < (s32)total);
    return count;
}
