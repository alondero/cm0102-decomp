/* fn_007afa70 -- player_search.cpp, preferred-image VA 0x007afa70 (3.9.68).
   Translated statement-by-statement from the original listing.

   Frame: push -1 / 0x95f3c6 / fs:[0] (SEH frame), sub esp,0x25c, push
   ebx,ebp,esi,edi.  L = esp after `sub esp,0x25c`; 16 live saved bytes, so
   `lea [esp+d]` = L + d - 0x10 (plus any live argument pushes, noted below).
   One scratch allocation `sc` holds every original stack object at its own
   L-relative address; nothing is initialised that the listing does not write.

     L+0x00 dword   club iteration counter / 10-list counter
     L+0x04 dword   club record      (DAT_00ae23bc + id*0x6e)
     L+0x08 dword   club byte offset (i*0x6e) / list base from 7f6be0
     L+0x0c dword   comparison root taken from club+0x39 / +0x24 / +0x1a
     L+0x10 dword   root from club+0x39 (survives the loop)
     L+0x14 24 bytes written by cm_fn_007e4920 at 007afd11
     L+0x24 8 bytes  cm_fn_0054a110 object (007afa95); overwritten by the
                     24-byte block above before it is read (007afe16)
     L+0x2c 8 bytes  cm_fn_0054a110 object (007afaa7), rewritten by
                     cm_fn_00417c10 at 007afd27 and copied at 007afe28
     L+0x34 byte  template byte  -> candidate+0x20
     L+0x35 dword template       -> candidate+0x21
     L+0x39 dword template       -> candidate+0x25
     L+0x3d byte  template       -> candidate+0x29
     L+0x3e byte  template       -> candidate+0x2a
     L+0x3f byte  template       -> candidate+0x2b
     L+0x40 byte  template       -> candidate+0x2c
     L+0x44/0x4c/0x54 8-byte temporaries passed to cm_fn_00417c10
     L+0x5c  256-byte message buffer
     L+0x15c 256-byte message buffer

   L+0x3f is never written by the original: the stores at 007afd5b (dword
   -> L+0x39), 007afd5f (L+0x3d), 007afd67 (L+0x3e), 007afd90/007afd96
   (L+0x40) leave that one byte as whatever the stack held, and 007afe50
   later transports it into candidate+0x2b.  That original bug is preserved
   here by reading sc[0x3f] without initialising it; no synthetic value,
   padding or fixture narrowing is used. */

#include "cmre.h"

#define CM_00ae1050 CM_GLOBAL(u32, 0x00ae1050)
#define CM_00ae1080 CM_GLOBAL(u32, 0x00ae1080)
#define CM_00ae2364 CM_GLOBAL(s32, 0x00ae2364)
#define CM_00ae23b4 CM_GLOBAL(u32, 0x00ae23b4)
#define CM_00ae23bc CM_GLOBAL(u32, 0x00ae23bc)
#define CM_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define CM_00b74318 CM_GLOBAL(u32, 0x00b74318)
#define CM_00b7433c CM_GLOBAL(u32, 0x00b7433c)
#define CM_00b74448 CM_GLOBAL(u32, 0x00b74448)

#define CM_SOURCE_FILE ((u32)0x00a701b0)  /* player_search.cpp path string   */
#define CM_FMT_LOC     ((u32)0x009870e8)  /* format string at 009870e8       */
#define CM_STR_ERROR   ((u32)0x009870e0)  /* string at 009870e0              */
#define CM_ERROR_BUF   ((u32)0x00ae24d0)  /* message buffer 00ae24d0         */
#define CM_CMP_FN      ((u32)0x007bce50)  /* table pointer pushed to 945333  */

/* param_1 is the receiver (ecx); `ret 0`, no stack arguments. */
extern void CM_THISCALL cm_fn_00417c10(u32 *self, u32 *arg1, u16 *arg2);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern u32 CM_CDECL cm_fn_005407e0(u32 first, u32 second, u32 mode);
extern u32 CM_CDECL cm_fn_00544520(u32 person, u32 club);
extern u32 CM_CDECL cm_fn_00544590(u32 person, u32 club);
extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern void CM_CDECL cm_fn_005a8f60(void);
extern unsigned long long CM_CDECL cm_fn_005cc7d0(void);
extern void CM_CDECL cm_fn_005e8290(const char *param_1, const char *param_2,
                                    int param_3);
extern u32 CM_STDCALL cm_fn_0076d9b0(u32 record, u32 candidate);
extern u32 CM_THISCALL cm_fn_00795750(u32 self, u32 date, u32 key);
extern u32 CM_THISCALL cm_fn_007af600(u32 self, u32 input);
extern u32 CM_THISCALL cm_fn_007af870(int *self, int param_2);
extern u32 CM_CDECL cm_fn_007e4920(u32 source, u32 destination);
extern u32 CM_THISCALL cm_fn_007f6be0(u32 self, u32 arg);
extern u32 CM_THISCALL cm_fn_007f8d70(u32 self, u32 record, u32 supplied,
                                      u32 check_date);
/* Legacy record for 0090d130 declares no stack arguments; the three pushes
   made before the call belong to the variadic 00944c9f call that follows
   (MSVC pushes right-to-left and may call during argument evaluation). */
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                   void *arg5);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);
/* 4 stack arguments: base, count, width, table; caller cleans 0x10. */
extern unsigned long long CM_CDECL cm_fn_00945333(u32 base_arg, u32 count_arg,
                                                  u32 width_arg, u32 compare_arg);
extern unsigned long long cm_fn_00945667(u32 arg8, u32 argC);
extern void cm_fn_00945934(u32 param_1);

typedef struct cm_seh_frame_ {
    void *prev;
    void *handler;
    volatile u32 state;
} cm_seh_frame;

#define SCD(off) (*(u32 *)(void *)(sc + (off)))
#define SCB(off) (*(u8 *)(void *)(sc + (off)))

u32 CM_FASTCALL cm_fn_007afa70(int *param_1)
{
    u8 sc[0x25c];
    cm_seh_frame frame;
    u8 *self;
    u32 id;
    u32 clubRec;
    u32 root;
    u32 cmpObj;
    u32 esiClub;
    u32 ediClub;
    u32 ptr;
    u32 a;
    u32 b;
    u32 flag;
    u32 i;
    u32 off;
    u32 al;
    u32 counter;
    u32 listBase;
    u32 cand;
    u32 sret;
    char *pc;

    self = (u8 *)(void *)param_1;

    frame.state = 0xffffffffu;
    frame.handler = (void *)0x0095f3c6u;
    frame.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = &frame;

    /* 007afa91 / 007afa9c */
    cm_fn_0054a110((u16 *)(void *)(sc + 0x24));
    frame.state = 0;
    cm_fn_0054a110((u16 *)(void *)(sc + 0x2c));

    /* 007afaac */
    id = *(u32 *)(void *)self;
    frame.state = 1;
    clubRec = CM_00ae23bc + id * 0x6e;
    SCD(0x04) = clubRec;
    if (clubRec == 0) {
        /* 007afad3 -> 007afb5e, line 0x3eb */
        cm_fn_00944cff(CM_SOURCE_FILE, 0, 0, (void *)(sc + 0x15c),
                       (void *)(sc + 0x5c));
        pc = cm_fn_0090d130();
        cm_fn_00944c9f(CM_ERROR_BUF, CM_FMT_LOC, (u32)(void *)pc,
                       (u32)(void *)(sc + 0x15c), (u32)(void *)(sc + 0x5c),
                       0x3eb);
        cm_fn_005e8290((const char *)(void *)CM_STR_ERROR,
                       (const char *)(void *)CM_ERROR_BUF, 0);
        CM_00b67a34 = 0;
        CM_FS(void *, 0) = frame.prev;
        return 0;
    }

    /* 007afb02 */
    root = *(u32 *)(void *)(clubRec + 0x39);
    SCD(0x10) = root;
    if (root != 0) {
        a = *(u32 *)(void *)(root + 0x53);
        if (a != 0) {
            SCD(0x0c) = a;
            goto have_cmp;
        }
    }
    a = *(u32 *)(void *)(clubRec + 0x24);
    if (a != 0) {
        SCD(0x0c) = *(u32 *)(void *)(a + 0x53);
    } else {
        SCD(0x0c) = *(u32 *)(void *)(clubRec + 0x1a);
    }

have_cmp:
    cmpObj = SCD(0x0c);
    if (cmpObj == 0) {
        /* 007afb31 -> 007afb5e, line 0x3fd */
        cm_fn_00944cff(CM_SOURCE_FILE, 0, 0, (void *)(sc + 0x5c),
                       (void *)(sc + 0x15c));
        pc = cm_fn_0090d130();
        cm_fn_00944c9f(CM_ERROR_BUF, CM_FMT_LOC, (u32)(void *)pc,
                       (u32)(void *)(sc + 0x5c), (u32)(void *)(sc + 0x15c),
                       0x3fd);
        cm_fn_005e8290((const char *)(void *)CM_STR_ERROR,
                       (const char *)(void *)CM_ERROR_BUF, 0);
        CM_00b67a34 = 0;
        CM_FS(void *, 0) = frame.prev;
        return 0;
    }

    /* 007afb94 */
    a = *(u32 *)(void *)(self + 0x1ee);
    if (a != 0) {
        cm_fn_009452ca(a);
        *(u32 *)(void *)(self + 0x1ee) = 0;
    }
    *(u32 *)(void *)(self + 0x1f2) = 0;
    *(u32 *)(void *)(self + 0x1f6) = 0;
    *(u32 *)(void *)(self + 0x2a4) = 0;
    SCD(0x00) = 0;
    if (CM_00ae2364 <= 0) goto after_loop;
    SCD(0x08) = 0;

loop_clubs:
    /* 007afbd4 */
    b = CM_00ae23bc;
    a = SCD(0x08);
    esiClub = a + b;
    ptr = cm_fn_004cf5a0(CM_00ae1050, esiClub);
    CM_00ae1080 = ptr;
    if (ptr != 0) {
        ediClub = CM_00ae23b4 + *(u32 *)(void *)(ptr + 4) * 0x245;
    } else {
        ediClub = 0;
    }
    /* 007afc10 */
    a = SCD(0x10);
    if (a != 0 && ediClub == a) goto next_club;

    /* 007afc20 */
    flag = 0;
    if (*(u8 *)(void *)(self + 0xa) == 3) {
        if (*(u32 *)(void *)(esiClub + 0x69) == 0) goto next_club;
        flag = 1;
        goto record;
    }
    if (*(u32 *)(void *)(esiClub + 0x61) == 0) goto next_club;

    /* 007afc38 */
    sret = (u32)(s32)(s16)(u32)cm_fn_005407e0(esiClub, SCD(0x04), 0);
    if ((s32)sret >= 0xfa0) {
        flag = 1;
        goto record;
    }
    /* 007afc51 */
    if (cm_fn_007f8d70(CM_00b74448, esiClub, SCD(0x10), 0) != 0) {
        flag = 1;
        goto record;
    }
    /* 007afc6b */
    b = 0;
    if (ediClub != 0) {
        a = *(u32 *)(void *)(ediClub + 0x53);
        if (a != 0) b = 1;
    }
    if (b == 0) {
        a = *(u32 *)(void *)(esiClub + 0x24);
        if (a != 0 && *(u8 *)(void *)(esiClub + 0x28) != 0x0b) {
            a = *(u32 *)(void *)(a + 0x53);
        } else {
            a = *(u32 *)(void *)(esiClub + 0x1a);
        }
        if (a == 0) goto next_club;
    }
    /* 007afc93 */
    if (*(u8 *)(void *)(a + 0x76) != *(u8 *)(void *)(cmpObj + 0x76))
        goto next_club;

    /* 007afca3 */
    cm_fn_004cf5a0(CM_00ae1050, esiClub);
    if (cm_fn_00544520(esiClub, 0) != 0) {
        flag = 1;
        goto record;
    }
    if (cm_fn_00544590(esiClub, 0) != 0) {
        flag = 1;
        goto record;
    }
    if (cm_fn_0076d9b0(esiClub, SCD(0x04)) != 0) flag = 1;

record:
    /* 007afcf3 */
    if (cm_fn_007af600((u32)(void *)param_1, esiClub) != 0) goto next_club;
    if (flag == 0) goto next_club;

    /* 007afd0b */
    cm_fn_007e4920(esiClub, (u32)(void *)(sc + 0x14));
    cm_fn_00417c10((u32 *)(void *)(sc + 0x2c), (u32 *)(void *)(sc + 0x44),
                   (u16 *)(void *)0x00ae2c90u);
    SCB(0x34) = 0;
    a = *(u32 *)(void *)(esiClub + 0x69);
    i = 0;
    if (a != 0) {
        sret = (u32)(s32)(s16)(u32)cm_fn_005407e0(esiClub, SCD(0x04), 1);
        SCD(0x35) = sret;
    } else {
        SCD(0x35) = i;
    }
    /* 007afd57 */
    id = *(u32 *)(void *)self;
    SCD(0x39) = i;
    SCB(0x3d) = 0;
    SCB(0x3e) = 0;
    ptr = cm_fn_00795750(CM_00b7433c, CM_00ae23bc + id * 0x6e, esiClub);
    if (ptr != 0) {
        SCB(0x40) = *(u8 *)(void *)(ptr + 0x113);
    } else {
        SCB(0x40) = 0xff;
    }
    /* 007afd9b */
    a = *(u32 *)(void *)(self + 0x1f2);
    b = *(u32 *)(void *)(self + 0x1f6);
    if (b == a) {
        a = a + 0xfa;
        *(u32 *)(void *)(self + 0x1f2) = a;
        b = a * 0x2d;
        a = *(u32 *)(void *)(self + 0x1ee);
        ptr = (u32)cm_fn_00945667(a, b);
        *(u32 *)(void *)(self + 0x1ee) = ptr;
        if (ptr == 0) {
            /* 007afed1, line 0x470; the fatal call at 007aff39 does not
               return, the listing's next instruction is 007aff3e. */
            cm_fn_00944cff(CM_SOURCE_FILE, 0, 0, (void *)(sc + 0x5c),
                           (void *)(sc + 0x15c));
            pc = cm_fn_0090d130();
            cm_fn_00944c9f(CM_ERROR_BUF, CM_FMT_LOC, (u32)(void *)pc,
                           (u32)(void *)(sc + 0x5c), (u32)(void *)(sc + 0x15c),
                           0x470);
            cm_fn_005e8290((const char *)(void *)CM_STR_ERROR,
                           (const char *)(void *)CM_ERROR_BUF, 0);
            CM_00b67a34 = 0;
            cm_fn_005cc7d0();
            cm_fn_005a8f60();
            cm_fn_00945934(0xffffffffu);
            /* 007aff39 does not return; the listing's next instruction is
               007aff3e, the second-loop continuation, which is therefore
               unreachable.  Keep the epilogue here instead of jumping back
               into the loop the original never re-enters. */
            CM_FS(void *, 0) = frame.prev;
            return 0;
        }
    }
    /* 007afdda */
    b = *(u32 *)(void *)(self + 0x1f6);
    a = SCD(0x14);
    cand = b * 0x2d + *(u32 *)(void *)(self + 0x1ee);
    *(u32 *)(void *)(cand + 0x00) = a;
    *(u32 *)(void *)(cand + 0x04) = SCD(0x18);
    *(u32 *)(void *)(cand + 0x08) = SCD(0x1c);
    *(u32 *)(void *)(cand + 0x0c) = SCD(0x20);
    /* 007afe16 / 007afe28 */
    cm_fn_00417c10((u32 *)(void *)(cand + 0x10), (u32 *)(void *)(sc + 0x4c),
                   (u16 *)(void *)(sc + 0x24));
    cm_fn_00417c10((u32 *)(void *)(cand + 0x18), (u32 *)(void *)(sc + 0x54),
                   (u16 *)(void *)(sc + 0x2c));
    *(u8 *)(void *)(cand + 0x20) = SCB(0x34);
    *(u32 *)(void *)(cand + 0x21) = SCD(0x35);
    *(u32 *)(void *)(cand + 0x25) = SCD(0x39);
    *(u8 *)(void *)(cand + 0x29) = SCB(0x3d);
    *(u8 *)(void *)(cand + 0x2a) = SCB(0x3e);
    *(u8 *)(void *)(cand + 0x2b) = SCB(0x3f);   /* original uninitialised byte */
    *(u8 *)(void *)(cand + 0x2c) = SCB(0x40);
    *(u32 *)(void *)(self + 0x1f6) = *(u32 *)(void *)(self + 0x1f6) + 1;

next_club:
    /* 007afe6b */
    clubRec = SCD(0x04);
    off = SCD(0x08);
    i = SCD(0x00) + 1;
    off = off + 0x6e;
    b = (u32)((s32)i < CM_00ae2364);   /* cmp eax,ecx before the stores */
    SCD(0x00) = i;
    SCD(0x08) = off;
    if (b != 0) goto loop_clubs;

after_loop:
    /* 007afe93 */
    b = *(u32 *)(void *)(self + 0x1f6);
    a = *(u32 *)(void *)(self + 0x1ee);
    cm_fn_00945333(a, b, 0x2d, CM_CMP_FN);
    listBase = (u32)(void *)cm_fn_007f6be0(CM_00b74448, clubRec);
    counter = 0;
    SCD(0x08) = listBase;
    SCD(0x00) = counter;

second_loop:
    /* 007afec8 */
    if ((u8)counter == 0xff) {
        al = *(u8 *)(void *)listBase;
    } else {
        al = (u8)counter;
    }
    /* 007aff40 */
    b = (u32)((s32)(s8)al * 0x2b0);
    a = listBase + b;
    b = *(u8 *)(void *)(a + 0xb);
    esiClub = a + 1;
    if (b != 2 && b != 3) {
        b = *(u32 *)(void *)(esiClub + 0x1f6);
        i = 0;
        if ((s32)b > 0) {
            off = 0;
            do {
                a = *(u32 *)(void *)(esiClub + 0x1ee);
                a = *(u32 *)(void *)(off + a + 0xc);
                ptr = cm_fn_004cf5a0(CM_00ae1050, CM_00ae23bc + a * 0x6e);
                CM_00ae1080 = ptr;
                if (ptr != 0) {
                    ediClub = CM_00ae23b4 + *(u32 *)(void *)(ptr + 4) * 0x245;
                } else {
                    ediClub = 0;
                }
                a = SCD(0x10);
                if (a == 0 || ediClub != a) {
                    a = *(u32 *)(void *)(esiClub + 0x1ee);
                    cm_fn_007af870((int *)(void *)param_1,
                                   (int)(a + off));
                }
                i = i + 1;
                off = off + 0x2d;
                b = *(u32 *)(void *)(esiClub + 0x1f6);
            } while ((s32)i < (s32)b);
        }
    }
    /* 007afffd */
    counter = counter + 1;
    SCD(0x00) = counter;
    if ((s32)counter < 10) goto second_loop;

    /* 007b000b */
    a = *(u32 *)(void *)(self + 0x1f6);
    if (a != 0) {
        b = *(u32 *)(void *)(self + 0x1ee);
        cm_fn_00945333(b, a, 0x2d, CM_CMP_FN);
    }
    CM_FS(void *, 0) = frame.prev;
    return 1;
}
