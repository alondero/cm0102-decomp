/* Reconstructed from original instructions at preferred-image VAs 0x0088f3e0..0x0088f5f5
   (pinned CM0102 3.9.68 SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86).
   thiscall: ECX is read before it is written (0x0088f3fe mov esi,ecx) and the routine
   ends with `ret` (0 stack bytes), so `this` is the sole ECX input.

   The virtual dispatch at 0x0088f4e4/0x0088f4f6 is the original
   `mov edx,[esi]` / `call dword ptr [edx+0x3c]`: the slot at 0x00970120 (vtable
   0x009700e4 + 0x3c) points at 0x0088e8e0. This source keeps that exact slot read
   and calls through it; 0x0088e8e0 stays an explicit unregistered boundary rather
   than being inlined or guessed. */
#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0051c800(u32 self, volatile u32 round_arg);
extern u32 CM_THISCALL cm_fn_00522e00(u32 self, u32 owner, s16 count, u32 clubs, s16 rounds, u32 competition, u32 schedule, u16 year, u8 flag_a, u8 flag_b, u16 round_flag, u8 flag_c, u32 builder, u32 numbered, u32 value_38, u32 club_flags);
extern void CM_CDECL cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_006826d0(void *self, u32 arg);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

#define DAT_009cf5ec CM_GLOBAL(u32, 0x009cf5ec)
#define DAT_00adadfc CM_GLOBAL(u32, 0x00adadfc)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

#define S_FILE    ((const char *)0x00a8b53c)
#define S_ERROR   ((const char *)0x009870e0)
#define S_FORMAT  ((const char *)0x009870e8)

/* One volatile SEH record: 0x0088f3e6 push -1 (state), 0x0088f3e8 push handler,
   0x0088f3ed push fs:[0] (prev), 0x0088f3ee link esp into fs:[0]. */
typedef struct cm_seh_record {
    void *prev;
    void *handler;
    volatile u32 state;
} cm_seh_record;

void CM_THISCALL cm_fn_0088f3e0(void *self)
{
    volatile cm_seh_record frame;
    u8 scratch[0x40c];
    u8 *rec;
    u32 result;

    /* 0x0088f3e6..0x0088f3ee */
    frame.state = 0xffffffffu;
    frame.handler = (void *)0x0096313e;
    frame.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)(void *)&frame;

    /* 0x0088f400 push 8; call 0x00944e46; 0x0088f407 mov ebx,eax */
    rec = (u8 *)(u32)cm_fn_00944e46(8);
    if (rec == 0) {
        /* 0x0088f410..0x0088f46c: out-of-memory diagnostic.
           scratch+0x0c and scratch+0x20c are the two 0x200-byte separated
           buffers addressed at [esp+0x18] / [esp+0x218] before the
           three saved registers and edi are pushed. */
        cm_fn_00944cff((u32)0x00a8b53c, 0, 0,
                       (void *)(scratch + 0x20c), (void *)(scratch + 0x0c));
        {
            char *s = cm_fn_0090d130();
            cm_fn_00944c9f((u32)0x00ae24d0, (u32)0x009870e8, (u32)s,
                           (u32)(scratch + 0x20c), (u32)(scratch + 0x0c), 0x206);
        }
        cm_fn_005e8290(S_ERROR, (const char *)0x00ae24d0, 0);
        DAT_00b67a34 = 0;
    }
    else {
        s32 n;
        s16 count;

        /* 0x0088f471..0x0088f4a5: find the record whose byte +0x37 is 1 and
           copy its first dword into the fresh allocation. The stride is 0x3b
           (0x3b = 8*i - i then i + 28*i doubled). */
        n = 0;
        count = *(s16 *)((u8 *)self + 0x3e);
        if (count > 0) {
            u8 *base = (u8 *)*(u32 *)((u8 *)self + 0xb1);
            u8 *p = base + 0x37;
            do {
                if (*p == 1) {
                    *(u32 *)rec = *(u32 *)(base + (u32)n * 0x3b);
                    break;
                }
                n++;
                p += 0x3b;
            } while (n < (s32)count);
        }

        /* 0x0088f4a7..0x0088f4e1: find the record whose byte +0x37 is 2 in the
           array element indexed by DAT_009cf5ec and copy its first dword into
           the second half of the allocation. The search selector is reloaded
           from the object after each failed comparison, as in the listing. */
        {
            u32 list = *(u32 *)((u32)DAT_00adadfc + (u32)DAT_009cf5ec * 4);
            s32 j = 0;

            if (*(s16 *)((u8 *)list + 0x3e) > 0) {
                do {
                    u32 a = cm_fn_006826d0((void *)list, (u32)j);
                    if (*(u8 *)(a + 0x37) == 2) {
                        u32 b = cm_fn_006826d0((void *)list, (u32)j);
                        *(u32 *)(rec + 4) = *(u32 *)b;
                        break;
                    }
                    j++;
                } while (j < (s32)*(s16 *)((u8 *)list + 0x3e));
            }
        }

        /* 0x0088f4e4..0x0088f4f6: virtual slot call.
           edx = [esi]; arg order from the four pushes:
           0, (esp+0x14 after the push = scratch+0x00), (esp+0x14 before the
           push = scratch+0x04), 0; ECX = this. */
        {
            u32 vtable = *(u32 *)self;
            u32 (CM_THISCALL *slot)(void *, u32, void *, void *, u32);

            slot = (u32 (CM_THISCALL *)(void *, u32, void *, void *, u32))
                   *(u32 *)(vtable + 0x3c);
            result = slot(self, 0, (void *)(scratch + 0x00),
                          (void *)(scratch + 0x04), 0);
        }

        /* 0x0088f4f9 push 0xb2; call 0x00944cf1; 0x0088f508 store at
           [esp+0x18] (scratch+0x08 in the success path). */
        {
            u8 *alloc = (u8 *)(u32)cm_fn_00944cf1(0xb2);
            u32 made;

            frame.state = 0; /* 0x0088f510 */
            if (alloc == 0) {
                made = 0; /* 0x0088f543 */
            }
            else {
                /* P2 = scratch+0x00 (arg2 of the slot call, `rounds`),
                   P1 = scratch+0x04 (arg3, `round_flag`). */
                made = cm_fn_00522e00((u32)(u32)alloc, (u32)self, 2,
                                      (u32)(u32)alloc,
                                      (s16)*(u32 *)(scratch + 0x00),
                                      *(u32 *)((u8 *)self + 4),
                                      result,
                                      (u16)*(u16 *)((u8 *)self + 0x40),
                                      0, 1,
                                      (u16)*(u32 *)(scratch + 0x04),
                                      0x14, 0, 0, 0, 0);
            }

            {
                /* 0x0088f545: edx = [esi+0xc] loaded before the state reset. */
                u32 *home = (u32 *)(*(u32 *)((u8 *)self + 0xc));

                frame.state = 0xffffffffu; /* 0x0088f549 */
                *home = made;              /* 0x0088f554 */
            }

            /* 0x0088f556 frees ebx (the 8-byte record from 0x0088f407) and
               0x0088f55c frees edi (the slot-call result). Both are cdecl,
               cleaned by the single 0x0088f564 add esp,8. */
            cm_fn_009452ca((u32)(u32)rec);
            cm_fn_009452ca(result);

            /* 0x0088f561 reload, 0x0088f567 mov ecx,[eax], 0x0088f569 push 0,
               0x0088f56a call 0x0051c800. */
            {
                u32 o = *(u32 *)((u8 *)self + 0xc);
                cm_fn_0051c800(*(u32 *)o, 0);
            }

            /* 0x0088f56f reload, 0x0088f573 cmp [ecx],ebp. */
            {
                u32 o = *(u32 *)((u8 *)self + 0xc);

                if (*(u32 *)o == 0) {
                    /* 0x0088f577..0x0088f5d3: second diagnostic, buffers at
                       scratch+0x10c and scratch+0x30c (edi already popped). */
                    cm_fn_00944cff((u32)0x00a8b53c, 0, 0,
                                   (void *)(scratch + 0x30c),
                                   (void *)(scratch + 0x10c));
                    {
                        char *s = cm_fn_0090d130();
                        cm_fn_00944c9f((u32)0x00ae24d0, (u32)0x009870e8,
                                       (u32)s, (u32)(scratch + 0x30c),
                                       (u32)(scratch + 0x10c), 0x22e);
                    }
                    cm_fn_005e8290(S_ERROR, (const char *)0x00ae24d0, 0);
                    DAT_00b67a34 = 0;
                }
                else {
                    *(u32 *)((u8 *)self + 0x30) = 0; /* 0x0088f5db */
                }
            }
        }
    }

    /* 0x0088f5de..0x0088f5f5 */
    CM_FS(void *, 0) = frame.prev;
}
