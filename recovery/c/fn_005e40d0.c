/* fn_005e40d0 - preferred-image VA 0x005e40d0 (CM 01/02 3.9.68).
 *
 * Translation of the original 474-byte body. Structure recovered from the
 * listing:
 *   - reads the file header (0x30 bytes) into a local scratch record,
 *   - reuses the caller's record when its first two dwords match the header,
 *     otherwise allocates a 0x30-byte record and copies the header into it,
 *   - allocates the pixel block, reads the pixels, converts the 16-bit pixel
 *     format when the header width differs from the global mode, and returns
 *     the record.
 *
 * Original callees (explicit boundaries):
 *   0x00945b00 open  (path, mode text)
 *   0x00945bfe read  (buffer, count, element size, state)
 *   0x00945501 alloc(count, size)
 *   0x009452ca free(ptr)
 *   0x00945a09 close(stream)
 */
#include "cmre.h"

/* 0x0098f928: one-byte mode text passed to the open call. */
#define DAT_0098f928 CM_GLOBAL(u8, 0x0098f928)
/* 0x00ae316c: global pixel-format mode compared against the header. */
#define DAT_00ae316c CM_GLOBAL(int, 0x00ae316c)
/* 0x00aebe0c: when non-zero the whole routine returns 0 immediately. */
#define DAT_00aebe0c CM_GLOBAL(int, 0x00aebe0c)

extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);
extern void *CM_CDECL cm_fn_00945501(u32 arg1, u32 arg2);
extern u32 CM_CDECL cm_fn_00945a09(u32 stream);
extern volatile u32 *CM_CDECL cm_fn_00945b00(u32 path, volatile s8 *mode_text);
extern u32 CM_CDECL cm_fn_00945bfe(u32 buffer, u32 count, u32 element_size, u32 state_address);

int *cm_fn_005e40d0(int param_1, int *param_2)
{
    /* Scratch header read by 0x00945bfe; the original copies 0xc dwords
     * (0x30 bytes) out of it. */
    u32 local_30[12];
    /* Caller-owned home of the first stack argument: the original stores the
     * stream pointer there and reloads that slot later. */
    volatile u32 home_1;
    u32 stream;
    int *record;
    u32 i;

    if (DAT_00aebe0c != 0) {
        return (int *)0;
    }
    if (param_1 == 0) {
        return (int *)0;
    }

    stream = (u32)cm_fn_00945b00((u32)param_1, (volatile s8 *)&DAT_0098f928);
    home_1 = stream;                       /* 005e410c: store to the arg home */
    if (stream == 0) {
        return (int *)0;
    }

    if (cm_fn_00945bfe((u32)(void *)local_30, 0x30, 1, stream) != 1) {
        cm_fn_00945a09(stream);
        return (int *)0;
    }

    if (param_2 != (int *)0 &&
        param_2[0] == (int)local_30[0] &&
        param_2[1] == (int)local_30[1]) {
        record = param_2;
    } else {
        record = (int *)cm_fn_00945501(1, 0x30);
        if (record == (int *)0) {
            cm_fn_00945a09(stream);
            return (int *)0;
        }
        for (i = 0; i < 0xc; i++) {        /* 005e4196: rep movsd, 0xc dwords */
            record[i] = (int)local_30[i];
        }
        record[3] = (int)(u32)cm_fn_00945501(1, (u32)record[2]);
        if (record[3] == 0) {
            cm_fn_009452ca((u32)(void *)record);
            cm_fn_00945a09(home_1);        /* 005e41b3: reload of the arg home */
            return (int *)0;
        }
    }

    if (cm_fn_00945bfe((u32)record[3], (u32)record[2], 1, stream) != 1) {
        if (record != param_2 && record != (int *)0) {
            if (record[3] != 0) {
                cm_fn_009452ca((u32)record[3]);
                record[3] = 0;
            }
            cm_fn_009452ca((u32)(void *)record);
        }
        cm_fn_00945a09(stream);
        return (int *)0;
    }

    {
        s32 mode = DAT_00ae316c;           /* 005e4221: one read, reused */
        if ((s32)local_30[9] != mode) {
            /* 005e4235: imul of the two header dwords, 32-bit result. */
            u32 count = (u32)record[1] * (u32)record[0];
            if ((s32)count > 0) {
                u16 *pix = (u16 *)(void *)(u32)record[3];
                u32 n = count;
                if (mode == 0x7e0) {
                    do {                  /* 005e4246: 5-6-5 -> 5-5-5 */
                        u32 p = *pix;
                        *pix = (u16)(((p & 0xffe0u) << 1) | (p & 0x1fu));
                        pix++;
                    } while (--n != 0);
                } else {
                    do {                  /* 005e427b: 5-5-5 -> 5-6-5 */
                        u32 p = *pix;
                        *pix = (u16)(((p >> 1) & 0x7fe0u) | (p & 0x1fu));
                        pix++;
                    } while (--n != 0);
                }
            }
        }
    }

    cm_fn_00945a09(stream);
    return record;
}
