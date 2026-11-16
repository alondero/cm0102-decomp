/* fn_00908fc0 (532 bytes) -- plain C recovered from the original 3.9.68
   instructions at preferred-image VA 0x00908fc0, image base 0x00400000,
   binary SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   No original instruction is executed by this translation; every callee stays an
   explicit boundary.  The vtable slot called at 0x00909091 is read from the
   receiver's own vtable DWORD at [vtable+0x3c], exactly as the original does; no
    slot target is assumed or invented here.
    HELD dependency: original VA 0x00906fc7 installs vtable 0x00970844;
    slot VA 0x00970880 contains 0x009072d0 (same pinned hash above).
    This translation preserves that actual indirect target, not a substitute.
    Registration and original-derived fixtures for 0x009072d0 remain required;
    preserving dispatch alone does not resolve that evidence gap. */

#include "cmre.h"

/* SEH record: fs:[0] chain head, { prev, handler, state } (ADR 0003).
   0x00908fc6 pushes state 0xffffffff, 0x00908fc8 handler 0x009653de,
   0x00908fc0 reads the predecessor from fs:[0]; 0x00908fce links esp. */
typedef struct {
    void *prev;
    void *handler;
    volatile u32 state;
} seh_frame_t;

/* call dword ptr [edx+0x3c] at 0x00909091 with ECX = ESI and four stack dwords
   (-3, L+0x0c, L+0x04, 0).  At 0x009090a3 ([esp+0x10] with esp = L-0x10) the
   L+0x00 local is read again, so the callee popped the 0x10 argument bytes:
   MS __thiscall.  Return is EAX (stored as the [object+0xc] home dword and as
   param_7 of the next call). */
typedef void * (CM_THISCALL *cm_slot_3c_fn)(void *self, s32 arg1, void *arg2,
                                            void *arg3, u32 arg4);

extern u32 CM_CDECL cm_fn_00909ec0(u32 club);
extern void CM_CDECL cm_fn_005e8290(const char *param_1, const char *param_2,
                                    int param_3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                   void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);
/* VA 0x00909181 calls the no-argument helper with three already-pushed
   formatter arguments live. VA 0x00909191 consumes those arguments along
   with the helper result, format and destination; cleanup is at 0x009091a6.
   All instruction citations refer to the pinned hash above. */
extern char * CM_CDECL cm_fn_0090d130(void);
/* 0x00909119 mov ecx,eax (the 0xee-byte allocation) supplies the receiver;
   0x0090911b call; no `add esp` after it in this routine, so the callee owns
   its 23 stack dwords. */
extern u32 * CM_THISCALL cm_fn_0068b100(u32 *param_1, int param_2, u16 param_3,
                                        int param_4, u16 param_5, u32 param_6,
                                        u32 *param_7, u16 param_8, u8 param_9,
                                        u8 param_10, u8 param_11, int param_12,
                                        int param_13, u16 param_14, u8 param_15,
                                        u16 param_16, u8 param_17, u8 param_18,
                                        u16 param_19, u16 param_20, u16 param_21,
                                        u32 param_22, u8 param_23);

/* One 0x40c-byte scratch object below the SEH record, with original offsets. */
#define L_SCRATCH_SIZE 0x40cu
#define OFF_I      0x000u  /* 0x00909037 counter, then 0x009090a3 allocation  */
#define OFF_OUT1   0x004u  /* 0x0090908b output #1 (param_16 at 0x009090d2)  */
#define OFF_OUT2   0x008u  /* 0x00909087 output #2 (param_8  at 0x00909102)  */
#define OFF_BUF_A  0x00cu  /* esp=L-12, [esp+0x18] */
#define OFF_BUF_B  0x10cu  /* esp=L-12, [esp+0x118] */
#define OFF_BUF_C  0x20cu  /* esp=L-12, [esp+0x218] */
#define OFF_BUF_D  0x30cu  /* esp=L-12, [esp+0x318] */

#define CM_STR_FILE   0x00aa1fb4u
#define CM_STR_TITLE  0x009870e0u
#define CM_STR_FORMAT 0x009870e8u
#define CM_STR_DEST   0x00ae24d0u
#define CM_LINE_CAPACITY_FAIL 0x4e6u
#define CM_LINE_BUILD_FAIL    0x506u

void CM_THISCALL cm_fn_00908fc0(void *param_1)
{
    u8 *const obj = (u8 *)param_1;
    volatile seh_frame_t frame;
    u8 scratch[L_SCRATCH_SIZE];
    u8 *buf;
    void *msg_from;   /* arg4 of 0x00944cff; arg1 of 0x0090d130 */
    void *msg_text;   /* arg5 of 0x00944cff; arg2 of 0x0090d130 */
    char *msg_text_ptr;
    u32 line_no;
    u32 *res;
    s32 cap;
    s16 count;
    u32 i;

    void *saved_prev = CM_FS(void *, 0);          /* 0x00908fc0 */
    frame.state = 0xffffffffu;                     /* 0x00908fc6 bb push -1 */
    frame.handler = (void *)0x009653deu;           /* 0x00908fc8 */
    frame.prev = saved_prev;                       /* 0x00908fcd */
    CM_FS(void *, 0) = (void *)(u32)&frame;        /* 0x00908fce */

    count = 0;                                     /* 0x00908fe0 xor ebx,ebx */
    cap = (s32)*(volatile s16 *)(obj + 0x3e);      /* 0x00908fe2 movsx */
    buf = (u8 *)(u32)cm_fn_00944e46((u32)cap << 2);/* 0x00908fe6 shl eax,2 */
    if (buf == (u8 *)0) {                          /* 0x00908ff4 cmp/jne */
        cm_fn_00944cff(CM_STR_FILE, 0, 0,
                       (void *)(scratch + OFF_BUF_C),
                       (void *)(scratch + OFF_BUF_A));
        msg_from = scratch + OFF_BUF_C;
        msg_text = scratch + OFF_BUF_A;
        line_no = CM_LINE_CAPACITY_FAIL;
        goto report;
    }

    i = 0;                                         /* 0x00909037 */
    {
        s32 n = CM_GLOBAL(volatile s32, 0x00ae235c);
        u32 rec = CM_GLOBAL(volatile u32, 0x00ae23b4);

        *(u32 *)(scratch + OFF_I) = i;
        if (n > 0) {                               /* 0x0090903b test/jle */
            for (;;) {
                if ((s32)count >= (s32)*(volatile s16 *)(obj + 0x3e))
                    break;                         /* 0x0090903f jge */
                u32 club_league = *(volatile u32 *)(rec + 0x57u);
                u32 league = *(volatile u32 *)(obj + 4);
                if (club_league == league) {
                    if ((u16)cm_fn_00909ec0((u32)(u32)rec) == 0x47b) {
                        u32 slot = (u32)(s32)count;    /* 0x0090905e movsx */
                        count = (s16)(count + 1);      /* 0x00909061 inc ebx */
                        *(u32 *)(buf + slot * 4) = (u32)(u32)rec;
                    }
                }
                n = CM_GLOBAL(volatile s32, 0x00ae235c); /* 0x0090906a */
                i = i + 1;                         /* 0x00909070 */
                rec = rec + 0x245u;                /* 0x00909071 */
                *(u32 *)(scratch + OFF_I) = i;
                if ((s32)i >= n)
                    break;                         /* 0x0090907d jl */
            }
        }
    }

    /* 0x0090907f: dispatch through the receiver's vtable slot +0x3c. */
    {
        u32 vt = *(volatile u32 *)obj;                /* 0x0090907f */
        cm_slot_3c_fn slot = (cm_slot_3c_fn)CM_GLOBAL(volatile u32, vt + 0x3cu);
        void *vtres = slot(param_1, (s32)-3, (void *)(scratch + OFF_OUT2),
                           (void *)(scratch + OFF_OUT1), 0);
        u8 *alloc = (u8 *)(u32)cm_fn_00944cf1(0xee);   /* 0x0090909b */

        *(u32 *)(scratch + OFF_I) = (u32)(u32)alloc;   /* 0x009090a3 */
        count = 0;                                     /* 0x009090a7 */
        frame.state = 0;                               /* 0x009090ab = ebx */
        if (alloc == (u8 *)0) {
            res = (u32 *)0;                            /* 0x00909122 */
        } else {
            /* Capture reads in original PUSH-construction order. Narrow formal
               types consume only the low byte/word of the partial registers. */
            u16 a7 = *(volatile u16 *)(obj + 0xa7);
            u16 d9 = *(volatile u16 *)(obj + 0xd9);
            u16 ab = *(volatile u16 *)(obj + 0xab);
            u8 b51 = *(volatile u8 *)(obj + 0x51);
            u32 out1 = *(volatile u32 *)(scratch + OFF_OUT1);
            u16 w40 = *(volatile u16 *)(obj + 0x40);
            u8 c4 = *(volatile u8 *)(obj + 0xc4);
            u8 c3 = *(volatile u8 *)(obj + 0xc3);
            u8 c2 = *(volatile u8 *)(obj + 0xc2);
            u32 out2 = *(volatile u32 *)(scratch + OFF_OUT2);
            u32 field4 = *(volatile u32 *)(obj + 4);
            u16 w3c = *(volatile u16 *)(obj + 0x3c);
            u16 w3e = *(volatile u16 *)(obj + 0x3e);
            /* 23 stack dwords, pushed by the original in reverse order; the
               [esp+..] loads at 0x009090d2 / 0x00909102 read L+0x04 and L+0x08. */
            res = cm_fn_0068b100((u32 *)(void *)alloc,
                                 (int)(u32)obj,
                                  w3e,
                                 (int)(u32)buf,
                                  w3c,
                                  field4,
                                 (u32 *)vtres,
                                  (u16)out2,
                                  c2,
                                  c3,
                                  c4,
                                 (int)(u32)(obj + 0xc5),
                                 (int)(u32)(obj + 0xbe),
                                  w40,
                                 0,
                                  (u16)out1,
                                  b51,
                                 1,
                                  ab,
                                  d9,
                                  a7,
                                 0,
                                 2);
        }

        {
            u32 home = *(volatile u32 *)(obj + 0xc);    /* 0x00909124 */
            frame.state = 0xffffffffu;                  /* 0x00909128 */
            *(volatile u32 *)(home) = (u32)(u32)res;    /* 0x00909133 */
        }
        cm_fn_009452ca((u32)(u32)buf);                  /* 0x00909135 */
        cm_fn_009452ca((u32)(u32)vtres);                /* 0x0090913b */

        {
            u32 home = *(volatile u32 *)(obj + 0xc);    /* 0x00909140 */
            u32 built = *(volatile u32 *)(home);        /* 0x00909146 */

            if (built != 0) {                           /* 0x00909149 */
                u32 built2;
                *(volatile u32 *)(obj + 0x30) = 0;      /* 0x009091b1 */
                built2 = *(volatile u32 *)(home);       /* 0x009091b4 */
                *(volatile u16 *)(u32)(built2 + 0x3a) = 0x47b;  /* +0x3a */
                goto done;
            }
        }

        cm_fn_00944cff(CM_STR_FILE, 0, 0,
                       (void *)(scratch + OFF_BUF_D),
                       (void *)(scratch + OFF_BUF_B));
        msg_from = scratch + OFF_BUF_D;
        msg_text = scratch + OFF_BUF_B;
        line_no = CM_LINE_BUILD_FAIL;
    }

report:
    msg_text_ptr = cm_fn_0090d130();
    cm_fn_00944c9f(CM_STR_DEST, CM_STR_FORMAT, (u32)(u32)msg_text_ptr,
                   msg_from, msg_text, line_no);
    cm_fn_005e8290((const char *)(void *)CM_STR_TITLE,
                   (const char *)(void *)CM_STR_DEST, 0);
    CM_GLOBAL(u32, 0x00b67a34) = 0;                     /* 0x009091a9 */

done:
    CM_FS(void *, 0) = frame.prev;                      /* 0x009091bc */
}
