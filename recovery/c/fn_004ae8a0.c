/* fn_004ae8a0 (comp_text.cpp), preferred image base 0x00400000, CM0102 3.9.68
   (sha256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86).
   Transcribed from the original instructions at the VAs quoted per statement.
   004ae8a0 is __thiscall, `ret 0xc` (three stack arguments) with ECX read first. */
#include "cmre.h"

#define CM_FN_FILE       ((u8 *)0x0099ebd8)   /* source file path string */
#define CM_FN_ERRFMT     ((u32)0x009870e8)    /* format string */
#define CM_FN_ERRBUF     ((u32)0x00ae24d0)    /* formatted message buffer */
#define CM_FN_ERRTITLE   ((char *)0x009870e0) /* title string */
#define CM_FN_SEHHANDLER ((void *)0x009568ee)

#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_0068d020(u32 self);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(volatile u8 *self, u32 edx_in,
                                                     const u8 *src, s16 count);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern char *cm_fn_0090d130(void); /* 004ae9c3 call: no stack arguments of its own */
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

typedef struct cm_seh_frame {
    void *volatile prev;
    void *volatile handler;
    u32 volatile state;
} cm_seh_frame;

u32 CM_THISCALL cm_fn_004ae8a0(u32 self, const char *name, u32 key_a, u32 key_b)
{
    cm_seh_frame frame;
    /* One scratch allocation for the original frame: [0x00] allocation temp
       ([esp+0x10]), [0x04] the 0xd0-byte record ([esp+0x14]),
       [0xd4]/[0x1d4]/[0x2d4]/[0x3d4] the four 256-byte buffers
       ([esp+0xe4], [esp+0x1e4], [esp+0x2e4], [esp+0x3e4]).
       Deliberately uninitialized: only the bytes the original writes are written. */
    u8 scratch[0x4d4];
    u32 fs_prev;
    u8 *err_a = 0;
    u8 *err_b = 0;
    u32 err_line = 0;
    u32 obj;
    u8 *rec;

    fs_prev = (u32)CM_FS(void *, 0);     /* 004ae8a0 mov eax, fs:[0] */
    frame.state = 0xffffffffu;           /* 004ae8a6 push -1 */
    frame.handler = CM_FN_SEHHANDLER;    /* 004ae8a8 push 0x9568ee */
    frame.prev = (void *)fs_prev;        /* 004ae8ad push eax */
    CM_FS(void *, 0) = (void *)&frame;   /* 004ae8ae mov fs:[0], esp */

    if (name == 0) {                     /* 004ae8c8 cmp esi, ebx */
        /* 004ae8ce..004ae8e5: 00944cff(file, 0, 0, scratch+0x1d4, scratch+0xd4) */
        cm_fn_00944cff((u32)CM_FN_FILE, 0, 0,
                       (void *)(scratch + 0x1d4), (void *)(scratch + 0xd4));
        /* 004ae8ed..004ae902: pushes 0xe3, scratch+0xd4, scratch+0x1d4 */
        err_a = scratch + 0x1d4;
        err_b = scratch + 0xd4;
        err_line = 0xe3;
        goto error_exit;
    }
    {
        const u8 *scan = (const u8 *)name;
        u32 cnt;

        while (*scan != 0) scan++;                        /* 004ae907..004ae90e */
        cnt = (u32)(scan - (const u8 *)name) + 1;         /* 004ae910 not ecx */
        if (cnt > 0xc8) {                                 /* 004ae912 cmp ecx, 0xc8 */
            /* 004ae91a..004ae931: 00944cff(file, 0, 0, scratch+0xd4, scratch+0x1d4) */
            cm_fn_00944cff((u32)CM_FN_FILE, 0, 0,
                           (void *)(scratch + 0xd4), (void *)(scratch + 0x1d4));
            /* 004ae939..004ae94d: pushes 0xeb, scratch+0x1d4, scratch+0xd4 */
            err_a = scratch + 0xd4;
            err_b = scratch + 0x1d4;
            err_line = 0xeb;
            goto error_exit;
        }
    }
    if (*(volatile u32 *)(self + 4) == 0) {              /* 004ae950 cmp [ebp+4], ebx */
        u32 alloc;

        alloc = (u32)cm_fn_00944cf1(0xe);                /* 004ae959..004ae960 */
        /* 004ae963 [esp+0x10] is four bytes before the record at
           004aea5e [esp+0x14]; both sites have the same saved-register depth. */
        *(u32 *)(void *)(scratch + 0x00) = alloc;
        frame.state = 0;                                 /* 004ae969 */
        if (alloc == 0) {                                /* 004ae967 je */
            obj = 0;                                     /* 004ae97b */
        }
        else {
            obj = cm_fn_0068d020(alloc);                 /* 004ae972..004ae974 */
        }
        frame.state = 0xffffffffu;                       /* 004ae97f */
        *(volatile u32 *)(self + 4) = obj;               /* 004ae98a */
        if (obj == 0) {                                  /* 004ae98d jne 004ae9f8 */
            /* 004ae98f..004ae9a6: 00944cff(file, 0, 0, scratch+0x3d4, scratch+0x2d4) */
            cm_fn_00944cff((u32)CM_FN_FILE, 0, 0,
                           (void *)(scratch + 0x3d4), (void *)(scratch + 0x2d4));
            /* 004ae9ae..004ae9c2: pushes 0xf7, scratch+0x2d4, scratch+0x3d4 */
            err_a = scratch + 0x3d4;
            err_b = scratch + 0x2d4;
            err_line = 0xf7;
            goto error_exit;
        }
    }
    obj = *(volatile u32 *)(self + 4);                   /* 004ae9f8 */
    /* 004ae9fb..004ae9fe: unaligned DWORD copy at record+0xa from record+0x2 */
    *(u32 *)(void *)((u8 *)obj + 0xa) = *(u32 *)(void *)((u8 *)obj + 2);
    rec = (u8 *)(u32)cm_fn_0068d1b0(*(volatile u32 *)(self + 4)); /* 004aea01..004aea04 */
    while (rec != 0) {                                   /* 004aea0b cmp edi, ebx */
        s32 order;
        const u8 *pa = rec;
        const u8 *pb = (const u8 *)name;

        /* 004aea0f..004aea3a: inlined byte comparison, unsigned */
        for (;;) {
            u8 c = pa[0];
            if (c != pb[0]) { order = (c < pb[0]) ? -1 : 1; break; }
            if (c == 0) { order = 0; break; }
            c = pa[1];
            if (c != pb[1]) { order = (c < pb[1]) ? -1 : 1; break; }
            pa += 2;
            pb += 2;
            if (c == 0) { order = 0; break; }
        }
        if (order == 0) {                                /* 004aea3a je 004aeac8 */
            u32 found = 0;

            if (*(u32 *)(void *)(rec + 0xc8) == key_a) { /* 004aeac8..004aead7 */
                if (*(u32 *)(void *)(rec + 0xcc) == key_b) { /* 004aead9..004aeae8 */
                    found = 1;                       /* 004aeaa8 mov eax, 1 */
                }
            }
            CM_FS(void *, 0) = frame.prev;               /* 004aeaad epilogue */
            return found;
        }
        rec = (u8 *)(u32)cm_fn_0068d1b0(*(volatile u32 *)(self + 4)); /* 004aea42..004aea51 */
    }
    {
        const u8 *src = (const u8 *)name;
        u32 cnt, i;

        while (*src != 0) src++;                         /* 004aea59..004aea62 */
        cnt = (u32)(src - (const u8 *)name) + 1;         /* 004aea64 not ecx */
        /* 004aea6f..004aea89: rep movsd/rep movsb copy strlen+1 bytes only. The
           rest of the 0xd0-byte record keeps the caller-frame bytes that were
           already there; nothing initializes or pads them. */
        for (i = 0; i != cnt; i++) {
            scratch[0x04 + i] = ((const u8 *)name)[i];
        }
        *(u32 *)(void *)(scratch + 0x04 + 0xc8) = key_a; /* 004aea99 [esp+0xe4] */
        *(u32 *)(void *)(scratch + 0x04 + 0xcc) = key_b; /* 004aea92 [esp+0xe8] */
        /* 004aeaa0..004aeaa3: ecx=self->[4], edx=key_b, push 0xd0, push record */
        cm_fn_0068d0d0((volatile u8 *)(u32)(*(volatile u32 *)(self + 4)), key_b,
                       (const u8 *)(scratch + 0x04), (s16)0xd0);
        CM_FS(void *, 0) = frame.prev;
        return 1;                                        /* 004aeaa8 mov eax, 1 */
    }
error_exit:
    {
        u32 msg;

        msg = (u32)cm_fn_0090d130();                     /* 004ae9c3 */
        /* 004ae9c8..004ae9d3: 00944c9f(0xae24d0, 0x9870e8, msg, err_a, err_b, line) */
        cm_fn_00944c9f(CM_FN_ERRBUF, CM_FN_ERRFMT, msg, (u32)err_a, (u32)err_b, err_line);
        cm_fn_005e8290((const char *)CM_FN_ERRTITLE, (const char *)CM_FN_ERRBUF, 0);
        DAT_00b67a34 = 0;                                /* 004ae9eb */
        CM_FS(void *, 0) = frame.prev;
        return 0;                                        /* 004ae9f1 */
    }
}
