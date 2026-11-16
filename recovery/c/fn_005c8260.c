/* Reconstruction of fn_005c8260 (comp/friendly.cpp), preferred-image VA 0x005c8260,
   image base 0x00400000, pinned CM0102 3.9.68 binary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1c627a22c445d14d489bd9bff86.
   Every statement below follows the original instructions at that VA
   (0x005c8260..0x005c85c9, `ret 8`, ECX read before written -> thiscall).

   HELD concern resolution (VA 0x005c856c..0x005c8579):
     005c856c  mov ecx, dword ptr [edi + 0x65]   ; ECX := object stored in record+0x65
     005c856f  test ecx, ecx
     005c8571  je 0x005c8579
     005c8573  mov edx, dword ptr [ecx]          ; vtable pointer read from object+0
     005c8575  push 1                            ; exactly one stack argument
     005c8577  call dword ptr [edx]              ; virtual slot 0
     005c8579  mov dword ptr [edi + 0x65], 0
   THIS is the object at [record+0x65] (ECX loaded from it), not `this` and not the
   stack list at L+0x10 that 0x0068d020 initialises. One stack argument (1) is pushed
   and no `add esp` follows the call, so the callee pops those 4 bytes (thiscall with
   one stack parameter). EAX is dead: the next instruction stores to [edi+0x65] and
   never reads EAX, so the slot is declared void. The slot target is resolved at run
   time from the object's own vtable, so no static target is assumed here. */

#include "cmre.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

/* Callees stay explicit boundaries to the original code (preferred-image VAs). */
extern void CM_THISCALL cm_fn_0054c5b0(void *self, void *out, u32 arg);
extern u32 *CM_THISCALL cm_fn_0054c8e0(void *self, void *out, u32 arg);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *self, u32 first, u32 second);
extern u32 CM_THISCALL cm_fn_005ab230(u32 self, u32 record);
extern u32 CM_THISCALL cm_fn_005abc30(u32 self, u32 record);
extern u32 CM_THISCALL cm_fn_005ac7a0(u32 self, u32 record);
extern u32 CM_THISCALL cm_fn_005acbe0(u32 self, u32 first_day, u32 first_year,
                                      u32 last_day, u32 last_year, u32 club_arg,
                                      u32 output_arg, u32 key_arg, u32 filter_arg,
                                      u32 side_arg);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_0068d020(u32 self);
extern u32 CM_THISCALL cm_fn_0068d040(u32 self);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern u32 CM_THISCALL cm_fn_0068d1f0(u8 *self, const u8 *target);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);
extern char *cm_fn_0090d130(void *a, void *b, u32 c);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 a1, u32 a2, u32 a3, void *a4, void *a5);

void CM_THISCALL cm_fn_005c8260(void *this_arg, u8 *rec, u32 flag)
{
    /* One scratch allocation for the whole original frame (L+0 .. L+0x237) with the
       SEH registration record (L+0x238 prev, L+0x23c handler, L+0x240 state) directly
       after it, matching the original stack layout. */
    struct {
        u8 scratch[0x238];
        volatile void *seh_prev;
        volatile u32 seh_handler;
        volatile u32 seh_state;
    } fr;

    u8 *L = fr.scratch;
    u8 *ctx = (u8 *)this_arg;

    /* SEH prologue: push -1; push 0x95a277; push fs:[0]; fs:[0] = esp  */
    fr.seh_state = 0xffffffffu;
    fr.seh_handler = 0x0095a277u;
    fr.seh_prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&fr.seh_prev;

    /* 005c828b: mov dword ptr [esp + 0x14], ebp   (saved receiver at L+8) */
    *(u32 *)(L + 0x08) = (u32)this_arg;

    if (rec == NULL) {
        /* 005c8291..005c82aa: 944cff(0x009cd0a0, 0, 0, &L+0x138, &L+0x38) */
        cm_fn_00944cff(0x009cd0a0u, 0u, 0u, (void *)(L + 0x138), (void *)(L + 0x38));
        {
            /* 005c82b8..005c82bf: push 0x10ad; push &L+0x38; push &L+0x138;
               call 90d130. Those three slots are not consumed by the nested call
               for the variadic 944c9f below: after 005c82c4..005c82ca pushes the
               90d130 result, the format and the destination, 944c9f is entered
               with esp at the first of the three, so they are its arguments 4..6
               (the 005c82e4 `add esp,0x24` accounts for exactly those 9 pushes,
               36 bytes, over the three calls). */
            char *ver = cm_fn_0090d130((void *)(L + 0x138), (void *)(L + 0x38),
                                       0x10adu);
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)ver,
                           (u32)(L + 0x138), (u32)(L + 0x38), 0x10adu);
        }
        /* 005c82d4..005c82df: 5e8290(0x009870e0, 0x00ae24d0, 0) */
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        CM_GLOBAL(u32, 0x00b67a34) = 0;
        /* 005c82ed: jmp 0x5c85b2 (skips push esi / pop esi) */
    }
    else {
        u32 *ret;
        u32 d0, d1, cnt, v, node, list, l2;
        u16 hi, recy;

        /* 005c82f3: 54c8e0(ecx = 0x00ae2c90, &L+0x30, 7) */
        ret = cm_fn_0054c8e0((void *)0x00ae2c90u, (void *)(L + 0x30), 7u);
        d0 = ret[0];                                     /* 005c8304 */
        d1 = ret[1];                                     /* 005c8306 */
        *(u32 *)(L + 0x00) = d0;                         /* 005c8310 */
        *(u32 *)(L + 0x04) = d1;                         /* 005c8314 */
        hi = *(u16 *)(L + 0x02);                         /* 005c8318 */
        recy = *(u16 *)(rec + 6);                        /* 005c8309 */
        if (hi < recy ||                                 /* 005c831d jb / ja */
            (hi == recy &&
             (s16)(*(u16 *)(L + 0x00)) < (s16)(*(u16 *)(rec + 4)))) {  /* 005c8328.. */
            u32 e;

            /* 005c8338: 77bbe0(ecx = &L+0x38, 0xb, 0) */
            cm_fn_0077bbe0((void *)(L + 0x38), 0x0bu, 0u);
            v = *(u32 *)rec;                             /* 005c8344 */
            fr.seh_state = 0;                            /* 005c8346 (dword) */
            v = *(u32 *)(u32)v;                          /* 005c834d */
            cm_fn_00785790((void *)(L + 0x38), 0u, v);   /* 005c834f..005c8355 */
            cm_fn_00785790((void *)(L + 0x38), 1u,
                           (u32)(s32)(s16)(*(u16 *)(rec + 4)));         /* 005c835a.. */
            cm_fn_00785790((void *)(L + 0x38), 2u,
                           (u32)(*(u16 *)(rec + 6)));                   /* 005c8369.. */
            /* 005c837e..005c83cb: four (pointer, byte) pairs at rec+0x15 / rec+0x25 */
            *(u32 *)(L + 0x00) = 4;
            for (;;) {
                u32 k = 4u - *(u32 *)(L + 0x00);
                u8 *cp = rec + 0x25 + k;
                u8 *pp = rec + 0x15 + k * 4u;
                u32 seen;

                e = *(u32 *)pp;                          /* 005c838c */
                if (e != 0) {
                    seen = 0;
                    if (e == *(u32 *)rec) {              /* 005c8393..005c8397 */
                        if (flag != 0) seen = 1;         /* 005c8399..005c83a2 */
                    }
                    if (seen == 0) {
                        if (*(u8 *)cp == 1 && e != *(u32 *)rec) /* 005c83a4..005c83ab */
                            seen = 1;
                    }
                    if (seen == 0) {
                        /* 005c83ad: 7861e0(ecx = [0xb7431c], &L+0x38, value) */
                        cm_fn_007861e0((void *)(u32)(CM_GLOBAL(u32, 0x00b7431c)),
                                       (u32)(L + 0x38), e);
                    }
                }
                cnt = *(u32 *)(L + 0x00);                /* 005c83be */
                cnt = cnt - 1u;
                *(u32 *)(L + 0x00) = cnt;
                if (cnt == 0) break;
            }
            ctx = (u8 *)(u32)(*(u32 *)(L + 0x08));       /* 005c83cd */
            fr.seh_state = 0xffffffffu;                  /* 005c83d1 (dword) */
        }

        /* 005c83dc..005c8424: drop `rec` from the list at ctx+0x34d0 */
        list = *(u32 *)(ctx + 0x34d0);
        if (list != 0) {
            *(u32 *)(u32)(list + 0x0a) = *(u32 *)(u32)(list + 0x02);
            node = cm_fn_0068d1b0(*(u32 *)(ctx + 0x34d0));   /* 005c83ec..005c83f2 */
            while (node != 0) {
                if ((u32)rec == *(u32 *)(u32)node) {              /* 005c83fb */
                    l2 = *(u32 *)(ctx + 0x34d0);                  /* 005c83ff */
                    cm_fn_0068d1f0((u8 *)(u32)l2, (const u8 *)(u32)node); /* 005c8405 */
                    l2 = *(u32 *)(ctx + 0x34d0);                  /* 005c840b */
                    *(u32 *)(u32)(l2 + 0x0a) = *(u32 *)(u32)(l2 + 0x02);
                }
                node = cm_fn_0068d1b0(*(u32 *)(ctx + 0x34d0));    /* 005c8417 */
            }
        }

        /* 005c8426: 54c8e0(ecx = rec+4, &L+0x20, 7) */
        cm_fn_0054c8e0((void *)(rec + 4), (void *)(L + 0x20), 7u);

        /* 005c8434..005c8448: 54c5b0(ecx = rec+4, &L+0x28, 0xa) */
        fr.seh_state = 1;                            /* 005c843d (dword) */
        cm_fn_0054c5b0((void *)(rec + 4), (void *)(L + 0x28), 0x0au);

        /* 005c844d..005c8459: 68d020(ecx = &L+0x10) */
        *(volatile u8 *)&fr.seh_state = 2;           /* 005c8451 (byte) */
        cm_fn_0068d020((u32)(L + 0x10));

        v = *(u32 *)(rec + 0x65);                    /* 005c845e */
        *(volatile u8 *)&fr.seh_state = 3;           /* 005c8465 (byte, bl = 3) */
        if (v != 0) {
            u32 f4 = *(u32 *)(u32)(v + 4);           /* 005c8472 */
            u32 key = *(u32 *)(u32)f4;               /* 005c847d */
            u32 a4 = *(u32 *)(L + 0x2a);             /* 005c847f */
            u32 a3 = *(u32 *)(L + 0x28);             /* 005c8484 */
            u32 a2 = *(u32 *)(L + 0x22);             /* 005c8489 */
            u32 a1 = *(u32 *)(L + 0x20);             /* 005c8490 */
            u32 obj;

            /* 005c849d: 5acbe0(ecx = [0x00ae2a58], 9 stack arguments) */
            cm_fn_005acbe0(CM_GLOBAL(u32, 0x00ae2a58), a1, a2, a3, a4,
                           0xffffffffu, (u32)(L + 0x10), key,
                           0xfffffffeu, 0xffffffffu);

            /* 005c84a2..005c84ae */
            *(u32 *)(L + 0x1a) = *(u32 *)(L + 0x12);
            node = cm_fn_0068d1b0((u32)(L + 0x10));
            while (node != 0) {
                u8 *np = (u8 *)(u32)node;
                u32 a_today, a_today2, q;
                u16 hi_l, lo_l;

                /* 005c84bd: mov cx,[esi+0x28]; mov dx,[esi+0x2a]; push both.
                   The original pushes 32-bit registers whose upper halves are
                   whatever the previous code left there; the declared callee
                   arguments are 32-bit, so the words are zero extended here. */
                {
                    u32 arg2 = (u32)(*(u16 *)(np + 0x28));
                    u32 arg1 = (u32)(*(u16 *)(np + 0x2a));
                    cm_fn_00549ef0((void *)(L + 0x00), arg1, arg2); /* ecx = &L+0 */
                }

                a_today = CM_GLOBAL(u32, 0x00ae2c90);        /* 005c84d0 */
                a_today2 = CM_GLOBAL(u32, 0x00ae2c94);       /* 005c84d5 */
                *(u32 *)(L + 0x08) = a_today;                /* 005c84db */
                hi_l = *(u16 *)(L + 0x0a);                   /* 005c84df */
                q = (hi_l == *(u16 *)(L + 0x02)) ? 1u : 0u;  /* 005c84e4..005c84f5 */
                *(volatile u8 *)&fr.seh_state = 4;           /* 005c84e9 (byte) */
                *(u32 *)(L + 0x0c) = a_today2;               /* 005c84f1 */
                lo_l = *(u16 *)(L + 0x00);                   /* 005c84f7 */
                if (q != 0 && (u16)a_today == lo_l) {
                    u32 r = cm_fn_005ac7a0(CM_GLOBAL(u32, 0x00ae2a58), node);
                    if (r != 0) {                            /* 005c850a */
                        u32 nf4 = *(u32 *)(ctx + 4);         /* 005c850e */
                        *(u8 *)(u32)(r + 0x42) = 0xff;       /* 005c8511 */
                        *(u32 *)(u32)(r + 0x14) = nf4;       /* 005c8515 */
                        *(u32 *)(u32)r = CM_GLOBAL(u32, 0x009cf668); /* 005c851e */
                    }
                }
                else {
                    cm_fn_005abc30(CM_GLOBAL(u32, 0x00ae2a58), node);  /* 005c8529 */
                    if (*(u8 *)(np + 0x43) != 0xff) {        /* 005c852e */
                        *(u8 *)(np + 0x42) = 0xff;           /* 005c8534 */
                        *(u32 *)(np + 0x14) = *(u32 *)(ctx + 4); /* 005c8538 */
                        *(u32 *)np = CM_GLOBAL(u32, 0x009cf668); /* 005c8544 */
                        cm_fn_005ab230(CM_GLOBAL(u32, 0x00ae2a58), node); /* 005c854d */
                    }
                }
                *(volatile u8 *)&fr.seh_state = 3;           /* 005c8556 (byte, bl) */
                node = cm_fn_0068d1b0((u32)(L + 0x10));      /* 005c855d */
            }

            /* 005c856c..005c8579: object at rec+0x65, virtual slot 0, one stack
               argument (1); ECX = that object; result unused; no caller cleanup. */
            obj = *(u32 *)(rec + 0x65);
            if (obj != 0) {
                u32 vt = *(u32 *)(u32)obj;               /* 005c8573 */
                ((void (CM_THISCALL *)(void *, u32))(u32)vt)((void *)(u32)obj, 1u);
            }
            *(u32 *)(rec + 0x65) = 0;                    /* 005c8579 */
        }

        /* 005c8580..005c85ac */
        {
            s32 sw = (s32)(*(s16 *)(ctx + 0xaf));
            s32 sb = (s32)(*(s8 *)(rec + 0x64));
            u32 slot = (u32)sw + (u32)sb;
            u32 base = CM_GLOBAL(u32, 0x00adadfc);

            *(volatile u8 *)&fr.seh_state = 2;           /* 005c8597 (byte) */
            *(u32 *)(u32)(base + slot * 4u) = 0;         /* 005c859f */
            *(u32 *)rec = 0;                             /* 005c85a6 */
            cm_fn_0068d040((u32)(L + 0x10));             /* 005c85ac */
        }
    }

    /* 005c85b2..005c85c9: restore fs:[0] and return */
    CM_FS(void *, 0) = (void *)fr.seh_prev;
}
