/* fn_00917db0 - CM 01/02 3.9.68, preferred-image VA 0x00917db0.
   Original module: comp\worldcup\wc_concacaf_cup.cpp
   ECX is read before written (mov ebp, ecx) and the final `ret` pops no stack
   bytes: thiscall member function whose receiver arrives in ECX. The receiver
   fields read are +0x04, +0x08, +0xb2, +0xb6 and +0xba.

   Frame after `sub esp,0x208` (S) with ebx at S-4 and ebp at S-8:
     S+0x00 u32        malloc result (stored by 0x00917e05)
     S+0x04 u32        second-loop counter home ([esp+0x14] after esi/edi)
     S+0x08 u8[0x100]  buffer A ([esp+0x10] after the two register pushes)
     S+0x108 u8[0x100] buffer B ([esp+0x110])
   The two buffers stay in one allocation at those exact offsets. */

#include "cmre.h"

#define CMRE_STR_00AD4850 ((char *)0x00ad4850)
#define CMRE_STR_009870E0 ((char *)0x009870e0)
#define CMRE_STR_009870E8 ((char *)0x009870e8)

#define DAT_009CFA14 CM_GLOBAL(int, 0x009cfa14)
#define DAT_00AE23A4 CM_GLOBAL(int, 0x00ae23a4)
#define DAT_00AE24D0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00B67A34 CM_GLOBAL(u32, 0x00b67a34)
#define DAT_00B74318 CM_GLOBAL(u32, 0x00b74318)
#define LAB_004B07D0 CM_GLOBAL(u8, 0x004b07d0)

extern void CM_THISCALL cm_fn_004a2540(u32 self, u32 param_1, u8 param_2);
extern void cm_fn_005e8290(const char * param_1, const char * param_2, int param_3);
extern u32 CM_THISCALL cm_fn_00775070(u32 self, volatile u32 filter, volatile u32 mode);
extern u32 CM_THISCALL cm_fn_00775220(u32 self, volatile u32 first, volatile u32 second);
extern char * cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);
extern unsigned long long CM_CDECL __attribute__((regparm(2))) cm_fn_00945333(u32 eax_in, u32 edx_in, volatile u32 base_arg, volatile u32 count_arg, volatile u32 width_arg, volatile u32 compare_arg);

typedef struct {
    volatile u32 dest_ptr;
    volatile u32 counter_home;
    u8 buf_a[0x100];
    u8 buf_b[0x100];
} cmre_frame_00917db0;

void CM_THISCALL cm_fn_00917db0(u32 self)
{
    cmre_frame_00917db0 frame;
    u32 counter;          /* EBX */
    u32 dest;
    s32 f_ba;
    void *arg4;
    void *arg5;
    u32 arg6;

    counter = 0;

    if (*(volatile u32 *)(self + 8) == 0) {
        /* 0x00917dc1: pushes 0xad4850, 0, 0, buf_a, buf_b. */
        cm_fn_00944cff((u32)CMRE_STR_00AD4850, 0, 0,
                       (void *)frame.buf_b, (void *)frame.buf_a);
        /* 0x00917de8: pushes 0x7cf, buf_a, buf_b for the shared 0x944c9f call. */
        arg4 = (void *)frame.buf_b;
        arg5 = (void *)frame.buf_a;
        arg6 = 0x7cf;
    }
    else {
        u32 count = *(volatile u32 *)(self + 0xb6);
        u32 bytes = count << 2;
        unsigned long long alloc = cm_fn_00944e46(bytes);

        frame.dest_ptr = (u32)alloc;
        if ((u32)alloc == 0) {
            /* 0x00917e0b: pushes 0xad4850, 0, 0, buf_b, buf_a. */
            cm_fn_00944cff((u32)CMRE_STR_00AD4850, 0, 0,
                           (void *)frame.buf_a, (void *)frame.buf_b);
            /* 0x00917e32: pushes 0x7d7, buf_b, buf_a. */
            arg4 = (void *)frame.buf_a;
            arg5 = (void *)frame.buf_b;
            arg6 = 0x7d7;
        }
        else {
            dest = (u32)alloc;

            /* 0x00917e70: rep movsd (bytes>>2) then rep movsb (bytes&3) copy of
               the entry list at [self+0xb2]. */
            {
                const u8 *src = (const u8 *)(*(volatile u32 *)(self + 0xb2));
                u8 *dst = (u8 *)dest;
                u32 dwords = bytes >> 2;
                u32 remainder = bytes & 3;
                u32 i;

                for (i = 0; i < dwords; i++) {
                    *(volatile u32 *)dst = *(const volatile u32 *)src;
                    dst += 4;
                    src += 4;
                }
                for (i = 0; i < remainder; i++) {
                    *(volatile u8 *)dst = *(const volatile u8 *)src;
                    dst += 1;
                    src += 1;
                }
            }

            /* 0x00917ea0: EAX = dest, EDX = bytes; pushed 0x4b07d0, 4, count, dest. */
            cm_fn_00945333(dest, bytes, dest, count, 4, (u32)&LAB_004B07D0);

            /* 0x00917ea5: filter = 0xae23a4 + n*0x66, mode 0; ECX = 0xb74318. */
            {
                u32 n = (u32)DAT_009CFA14;
                u32 filter = (u32)DAT_00AE23A4 + (((n << 5) + n) * 3u) * 2u;

                cm_fn_00775070(DAT_00B74318, filter, 0);
            }

            /* 0x00917ece: entry test only; the field is re-read, not reused
               (0x00917ef6, 0x00917f0b, 0x00917f87, 0x00917fb5, 0x00917fcc,
               0x00918005 each load [self+0xba] again). */
            f_ba = (s32)*(volatile s8 *)(self + 0xba);

            /* 0x00917ee0: tag 1 while counter < 3 - *0xba; the exit test at
               0x00917ef6 reloads [self+0xba]. */
            if (3 - f_ba > 0) {
                for (;;) {
                    u32 index = (u32)(s16)counter;
                    u32 value;

                    counter = counter + 1;
                    value = *(volatile u32 *)(frame.dest_ptr + (s32)(s16)index * 4);
                    cm_fn_004a2540(*(volatile u32 *)(self + 8), value, 1);
                    if (!((s32)(s16)counter <
                          (3 - (s32)*(volatile s8 *)(self + 0xba)))) {
                        break;
                    }
                }
            }

            /* 0x00917f0b: tags (byte home)+1 up to 6, home stored in the frame.
               This bound reloads [self+0xba]; the loop then uses only the home. */
            {
                u32 home = (u32)(3 - (s32)*(volatile s8 *)(self + 0xba));

                frame.counter_home = home;
                if ((s16)home < 6) {
                    for (;;) {
                        u32 tag;
                        u32 index;
                        u32 value;

                        home = frame.counter_home;
                        tag = (home & 0xffffff00u) | (u32)(u8)(home + 1);
                        index = (u32)(s16)counter;
                        counter = counter + 1;
                        value = *(volatile u32 *)(frame.dest_ptr + (s32)(s16)index * 4);
                        cm_fn_004a2540(*(volatile u32 *)(self + 8), value, (u8)tag);
                        home = frame.counter_home;
                        home = home + 1;
                        frame.counter_home = home;
                        if (!((s16)home < 6)) {
                            break;
                        }
                    }
                }
            }

            /* 0x00917f50: six entries, tag 7 for 6..8 and 10 for 9..11. */
            {
                s16 si = 6;

                do {
                    u32 tag = (si < 9) ? 7u : 10u;
                    u32 index = (u32)(s16)counter;
                    u32 value;

                    counter = counter + 1;
                    value = *(volatile u32 *)(frame.dest_ptr + (s32)(s16)index * 4);
                    cm_fn_004a2540(*(volatile u32 *)(self + 8), value, (u8)tag);
                    si = (s16)(si + 1);
                } while (si < 12);
            }

            /* 0x00917f87: tag 13 while the 16-bit counter is under
               [self+0xb6] - [self+0xba]; both fields are re-read at the
               0x00917fb5 bottom of every iteration. */
            {
                s16 si = 12;
                s32 ba = (s32)*(volatile s8 *)(self + 0xba);
                s32 limit;

                limit = (s32)*(volatile u32 *)(self + 0xb6) - ba;
                if (limit > 12) {
                    for (;;) {
                        u32 index = (u32)(s16)counter;
                        u32 value;

                        counter = counter + 1;
                        value = *(volatile u32 *)(frame.dest_ptr + (s32)(s16)index * 4);
                        cm_fn_004a2540(*(volatile u32 *)(self + 8), value, 13);
                        ba = (s32)*(volatile s8 *)(self + 0xba);
                        limit = (s32)*(volatile u32 *)(self + 0xb6) - ba;
                        si = (s16)(si + 1);
                        if (!((s32)si < limit)) {
                            break;
                        }
                    }
                }
            }

            /* 0x00917fcc: link each record that has a +0x53 pointer; the bound
               is recomputed from both fields at the 0x00918005 loop bottom. */
            {
                s32 ba = (s32)*(volatile s8 *)(self + 0xba);
                s32 limit;

                limit = (s32)*(volatile u32 *)(self + 0xb6) - ba;
                if (limit > 0) {
                    s16 i = 0;

                    for (;;) {
                        u32 rec = *(volatile u32 *)(frame.dest_ptr + (s32)i * 4);

                        if (rec != 0) {
                            u32 link = *(volatile u32 *)(rec + 0x53);

                            if (link != 0) {
                                cm_fn_00775220(DAT_00B74318, link,
                                               *(volatile u32 *)(self + 4));
                            }
                        }
                        ba = (s32)*(volatile s8 *)(self + 0xba);
                        limit = (s32)*(volatile u32 *)(self + 0xb6) - ba;
                        i = (s16)(i + 1);
                        if (!((s32)i < limit)) {
                            break;
                        }
                    }
                }
            }

            /* 0x0091801c */
            cm_fn_009452ca(frame.dest_ptr);
            return;
        }
    }

    /* 0x00917e39: call 0x90d130 with no stack arguments, then the shared
       report call cm_fn_00944c9f(0xae24d0, 0x9870e8, EAX, arg4, arg5, arg6). */
    {
        char *msg = cm_fn_0090d130();

        cm_fn_00944c9f((u32)&DAT_00AE24D0, (u32)CMRE_STR_009870E8, (u32)msg,
                       arg4, arg5, arg6);
        cm_fn_005e8290(CMRE_STR_009870E0, (const char *)&DAT_00AE24D0, 0);
        DAT_00B67A34 = 0;
    }
}
