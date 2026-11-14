/* fn_004849f0 -- preferred-image VA 0x004849f0, image base 0x00400000.
   Module: e:\dev\cm3\cm3 00-01\cm3\code\club_screens.cpp (2683 bytes).
   Translated from the original listing; callees stay explicit boundaries. */

#include "cmre.h"

/* ---------------------------------------------------------------- */
/* original globals                                                 */
/* ---------------------------------------------------------------- */
#define DAT_00ae2364 CM_GLOBAL(int, 0x00ae2364)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b63d28 CM_GLOBAL(int, 0x00b63d28)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define PTR_00ad9c6c CM_GLOBAL(u32, 0x00ad9c6c)
#define PTR_00ae1050 CM_GLOBAL(u32, 0x00ae1050)
#define PTR_00ae28f0 CM_GLOBAL(u32, 0x00ae28f0)
#define PTR_00b74318 CM_GLOBAL(u32, 0x00b74318)
#define PTR_00b7434c CM_GLOBAL(u32, 0x00b7434c)
#define PTR_00b74448 CM_GLOBAL(u32, 0x00b74448)
#define PTR_00de1f54 CM_GLOBAL(u32, 0x00de1f54)

/* string constants: only their original addresses are used */
#define STR_009870e0 ((const char *)0x009870e0)
#define STR_009870e8 ((const char *)0x009870e8)
#define STR_0098e598 ((const char *)0x0098e598)

/* ---------------------------------------------------------------- */
/* callees (explicit boundaries)                                     */
/* ---------------------------------------------------------------- */
extern u8 CM_THISCALL cm_fn_00418f60(u8 *self, u32 *index_pointer, u32 second_arg);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self, volatile u32 person, u32 club);
extern u32 CM_THISCALL cm_fn_004cf6d0(u32 self, u32 argument);
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern u32 CM_CDECL cm_fn_0053d740(u8 *arg1, u32 *arg2);
extern u32 CM_CDECL cm_fn_0053d980(u32 arg_1, u32 arg_2, u32 arg_3);
extern u32 CM_CDECL cm_fn_005444a0(u32 person, u32 input_club);
extern u32 CM_CDECL cm_fn_00544590(u32 person, u32 club);
extern u32 CM_CDECL cm_fn_00544600(u32 person, u32 club);
extern u32 CM_THISCALL cm_fn_0055d270(u32 self, volatile u32 person, u32 club_key, u32 amount_arg, u32 match_arg);
extern u32 CM_THISCALL cm_fn_0055d360(u32 self, u32 person, u32 key);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u8 CM_THISCALL cm_fn_0062c400(u32 self, u32 person);
extern unsigned long long CM_STDCALL cm_fn_0062c590(u32 in_EAX, u32 in_EDX, u32 self, u32 person, u32 optional);
extern u32 CM_STDCALL cm_fn_00631550(u32 arg);
extern u32 CM_THISCALL cm_fn_0068d020(u32 self);
extern u32 CM_THISCALL cm_fn_0068d040(u32 self);
extern u32 CM_THISCALL cm_fn_007757d0(u32 self, volatile u32 person_argument, volatile u32 date_argument, volatile u32 club_filter_argument, volatile u32 text_argument, volatile u32 match_argument);
extern u8 CM_STDCALL cm_fn_007a7eb0(s32 *param_1);
extern u32 CM_THISCALL cm_fn_007f8d70(u32 self, volatile u32 record, volatile u32 supplied, volatile u32 check_date);
extern u32 CM_THISCALL cm_fn_00842180(u32 self, volatile u32 arg1, volatile u32 arg2, volatile u32 arg3, volatile u32 arg4, volatile u32 arg5);
extern u8 CM_THISCALL cm_fn_0085fa20(u32 self);
extern u32 CM_THISCALL cm_fn_008676f0(volatile u8 *self);
extern u32 CM_THISCALL cm_fn_008c7560(u32 self, u32 person, u32 club, u32 flag);
extern u32 CM_THISCALL cm_fn_008cca10(u32 self, u32 record, u32 output);
extern u32 CM_THISCALL cm_fn_008cde70(u32 self, u32 key, u32 mode);
extern u32 CM_THISCALL cm_fn_008cf5e0(int param_1, u32 param_2, int param_3);
extern u32 CM_THISCALL cm_fn_008cf6b0(u32 param_1, u32 param_2, u32 param_3, u32 param_4);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

/* The recorded prototype of the legacy callee 0x0090d130 is (void); the
   original call site pushes three cdecl arguments (buf10c, buf20c, 0x517f)
   that the caller removes with the shared "add esp, 0x24".  The call is made
   through a matching prototype so the three stack arguments are pushed. */
typedef char *(*cm_90d130_void_fn)(void);
typedef char *(*cm_90d130_args_fn)(char *, char *, u32);

union cm_90d130_ptr {
    cm_90d130_void_fn v;
    cm_90d130_args_fn a;
};

/* 0x0062c590 is called with the original's own shape: ECX holds the
   receiver, the stack holds (person, optional) and the callee removes
   those eight bytes (the single "add esp" after the call site proves it).
   The recorded prototype lists the two register inputs in front of it, so
   the call is made through the shape the original used. */
typedef unsigned long long (CM_THISCALL *cm_62c590_fn)(u32 self, u32 person, u32 optional);

union cm_62c590_ptr {
    unsigned long long (*std5)(u32, u32, u32, u32, u32) CM_STDCALL;
    cm_62c590_fn tc3;
};

/* ---------------------------------------------------------------- */
/* local scratch: one allocation, original offsets kept             */
/*   +0x03 cvar1   (u8)                                            */
/*   +0x04 object built by cm_fn_0068d020, word read back at +0x04 */
/*   +0x14 club pointer                                               */
/*   +0x18 low dword of the cm_fn_0062c590 result                   */
/*   +0x1c six bytes written by cm_fn_008cf6b0                      */
/*   +0x48 buf20c (256 bytes)                                       */
/*   +0x148 buf10c (256 bytes)                                      */
/* ---------------------------------------------------------------- */
typedef struct {
    u8          pad03[3];
    u8          cvar1;         /* +0x03 */
    volatile u32 w04;          /* +0x04 */
    u8          pad08[0x0c];
    u32         club;          /* +0x14 */
    u32         res;           /* +0x18 */
    u8          b1c[6];        /* +0x1c */
    u8          pad22[0x26];
    u8          buf20c[0x100]; /* +0x48 */
    u8          buf10c[0x100]; /* +0x148 */
} cm_loc_t;

typedef struct {
    void *prev;
    void *handler;
    u32   state;
} cm_seh_t;

u32 cm_fn_004849f0(int *param_1, int *param_2, int param_3)
{
    volatile cm_seh_t frame;
    cm_loc_t loc;
    u32 arg1 = (u32)param_1;
    u32 arg2 = (u32)param_2;
    u32 arg3 = (u32)param_3;
    u32 club = 0;
    u32 res = 0;
    u8  cvar1 = 0;
    u32 t0 = 0;
    u32 t1 = 0;
    u32 t2 = 0;
    union cm_90d130_ptr fn90d130;
    union cm_62c590_ptr fn62c590;

#define FINISH(v)                                                          \
    do {                                                                   \
        frame.state = 0xffffffffu;                                         \
        cm_fn_0068d040((u32)&loc.w04);                                    \
        CM_FS(void *, 0) = frame.prev;                                     \
        return (u32)(v);                                                  \
    } while (0)

    /* 0x004849f0: SEH frame: state, handler, prev, then link */
    frame.state = 0xffffffffu;
    frame.handler = (void *)0x00955f7b;
    frame.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&frame;

    /* 0x00484a0f: object at local +0x04 */
    cm_fn_0068d020((u32)&loc.w04);

    /* 0x00484a18: esi = arg1 ; ebx = 0 */
    /* 0x00484a23: state = 0 */
    frame.state = 0;

    if (arg1 == 0) {
        /* 0x00484a2c */
        cm_fn_00944cff((u32)STR_0098e598, 0, 0,
                       (void *)&loc.buf10c[0], (void *)&loc.buf20c[0]);
        fn90d130.v = cm_fn_0090d130;
        t0 = (u32)fn90d130.a((char *)&loc.buf10c[0],
                              (char *)&loc.buf20c[0],
                              0x517fu);
        /* The original removes the three arguments of cm_fn_0090d130 only
           with the shared "add esp, 0x24", so they are still on the stack
           when the variadic formatter runs: its %s arguments are taken
           from that residue (buf10c, buf20c and the id).  The same values
           are handed to the formatter so its stack view is unchanged. */
        cm_fn_00944c9f((u32)&DAT_00ae24d0, (u32)STR_009870e8, t0,
                       (u32)&loc.buf10c[0], (u32)&loc.buf20c[0], 0x517fu);
        cm_fn_005e8290(STR_009870e0, (const char *)&DAT_00ae24d0, 0);
        DAT_00b67a34 = 0;
        frame.state = 0xffffffffu;
        cm_fn_0068d040((u32)&loc.w04);
        CM_FS(void *, 0) = frame.prev;
        return 0;
    }

    /* 0x00484aa3 */
    cvar1 = cm_fn_0062c400((u32)CM_GLOBAL(int, 0x00b63d28), arg1);
    /* 0x00484abc */
    fn62c590.std5 = cm_fn_0062c590;
    res = (u32)fn62c590.tc3((u32)CM_GLOBAL(int, 0x00b63d28), arg1, 1);
    loc.res = res;

    /* 0x00484ac1: edi = arg3 */
    /* 0x00484ad0 */
    if (arg3 != 0) {
        cm_fn_008cf6b0((u32)PTR_00de1f54, arg3, (u32)&loc.b1c[0], 1);
    }

    /* 0x00484ae3 */
    cm_fn_00842180((u32)(*(u32 *)((u32)PTR_00de1f54 + 0x856)),
                   arg1, (u32)&loc.w04, 1, 2, 0);

    /* 0x00484aff */
    if (*(s32 *)arg1 < DAT_00ae2364) {
        /* 0x00484b11 */
        club = cm_fn_004cf5f0((u32)PTR_00ae1050, arg1, arg2);
        loc.club = club;
        if (arg2 != 0) {
            /* 0x00484b2c */
            if (club != 0 && *(u32 *)arg2 != *(u32 *)(club + 4)) {
                t1 = cm_fn_004cf6d0((u32)PTR_00ae1050, arg1);
                if (t1 != 0) {
                    t2 = cm_fn_004cf6d0((u32)PTR_00ae1050, arg1);
                    if (*(u32 *)(t2 + 4) == *(u32 *)arg2) {
                        club = cm_fn_004cf6d0((u32)PTR_00ae1050, arg1);
                        loc.club = club;
                    }
                }
            }
        }
        if (arg2 == 0) {
            goto second_half;
        }
    } else {
        /* 0x00484b6e */
        loc.club = club;
        if (arg2 == 0) {
            goto second_half;
        }
    }

    /* 0x00484b7a */
    if (cm_fn_0053b900((u32 *)arg2) == 0) {
        /* 0x00484b87 */
        if (cm_fn_00418f60((u8 *)PTR_00ad9c6c, (u32 *)arg1, 0) != 0) {
            FINISH(0x15);
        }
    }
    /* 0x00484bb3 */
    if (CM_GLOBAL(int, 0x00b63d28) != 0) {
        /* 0x00484bbd: original tests AL only (0x00484bc3 test al, al) */
        if ((u8)cm_fn_00631550(arg1) != 0) {
            FINISH(0x15);
        }
    }
    /* 0x00484be2 */
    if (cm_fn_008c7560((u32)PTR_00de1f54, arg1, arg2, 0) != 0) {
        FINISH(1);
    }
    /* 0x00484c10 */
    if (arg3 != 0) {
        /* 0x00484c20 */
        if ((u8)cm_fn_008cf5e0((int)PTR_00de1f54, arg1, (int)arg3) == 1) {
            FINISH(2);
        }
        if (arg3 != 0) {
            /* 0x00484c54 */
            if ((u8)cm_fn_008cf5e0((int)PTR_00de1f54, arg1, (int)arg3) == 0x0a) {
                FINISH(2);
            }
            if (arg3 != 0) {
                /* 0x00484c84 */
                if ((u8)cm_fn_008cf5e0((int)PTR_00de1f54, arg1, (int)arg3) == 0x0b) {
                    FINISH(3);
                }
                if (arg3 != 0) {
                    /* 0x00484cb4 */
                    if ((u8)cm_fn_008cf5e0((int)PTR_00de1f54, arg1, (int)arg3) != 0) {
                        FINISH(3);
                    }
                }
            }
        }
    }
    /* 0x00484cd8 */
    if (cm_fn_007757d0((u32)PTR_00b74318, arg1, 0x00ae2c90u, arg2,
                       0x00de1f64u, 0) != 0) {
        FINISH(4);
    }
    /* 0x00484d10 */
    if (arg3 != 0) {
        /* 0x00484d23 */
        if (cm_fn_0055d270((u32)PTR_00ae28f0, arg1, *(u32 *)(arg3 + 0x14),
                           1, 0) != 0) {
            FINISH(5);
        }
    }
    /* 0x00484d47 */
    if (cvar1 == 2) {
        FINISH(6);
    }
    /* 0x00484d69: ebx = 0xffffffff */
    /* 0x00484d6c */
    if (arg3 != 0) {
        /* 0x00484d70 */
        if (cm_fn_0053b900((u32 *)arg2) == 0) {
            /* 0x00484d7d */
            if (*(s32 *)arg1 < DAT_00ae2364) {
                /* 0x00484d88 */
                if (!(loc.b1c[0] == 0xff && loc.b1c[1] == 0xff &&
                      loc.b1c[2] == 0xff)) {
                    /* 0x00484d9a */
                    if (cm_fn_0053d980(arg1, *(u32 *)(arg2 + 0x53), 1) != 0) {
                        FINISH(7);
                    }
                }
            }
        }
    }
    /* 0x00484dc4 */
    if (cvar1 == 1) {
        FINISH(8);
    }
    /* 0x00484de2 */
    if (arg3 != 0) {
        /* 0x00484de6 */
        if (cm_fn_0053b900((u32 *)arg2) == 0) {
            /* 0x00484df3 */
            if (*(s32 *)arg1 < DAT_00ae2364) {
                /* 0x00484dfe */
                if (!(loc.b1c[3] == 0xff && loc.b1c[4] == 0xff &&
                      loc.b1c[5] == 0xff)) {
                    /* 0x00484e10 */
                    if (cm_fn_0053d980(arg1, *(u32 *)(arg2 + 0x53), 1) != 0) {
                        /* 0x00484e23 */
                        if (cm_fn_0053d740((u8 *)arg1,
                                           (u32 *)(*(u32 *)(arg2 + 0x53))) == 0) {
                            FINISH(9);
                        }
                    }
                }
            }
        }
    }
    /* 0x00484e4b */
    if ((s16)res < 0x41) {
        FINISH(0x16);
    }
    /* 0x00484e6a */
    if (*(s32 *)arg1 < DAT_00ae2364) {
        /* 0x00484e81 */
        if (cm_fn_007f8d70((u32)PTR_00b74448, arg1, 0, 0) != 0) {
            FINISH(0x17);
        }
    }
    /* 0x00484ea1 */
    if (arg3 != 0) {
        /* 0x00484eb0 */
        if (cm_fn_0055d360((u32)PTR_00ae28f0, arg1, *(u32 *)(arg3 + 0x14)) != 0) {
            FINISH(0x0a);
        }
    }
    /* 0x00484ed0 */
    if (cm_fn_0053b900((u32 *)arg2) == 0) {
        /* 0x00484ee9 */
        if (*(s32 *)arg1 < DAT_00ae2364) {
            /* 0x00484ef2 */
            if (cm_fn_008cca10((u32)PTR_00de1f54, arg1, 0) != 0) {
                FINISH(0x18);
            }
        }
    }
    /* 0x00484f12 */
    if (cm_fn_0053b900((u32 *)arg2) == 0) {
        /* 0x00484f1f */
        if (*(s32 *)arg1 < DAT_00ae2364) {
            /* 0x00484f33 */
            if (cm_fn_008cde70((u32)PTR_00de1f54, arg1, 1) != 0) {
                FINISH(0x0b);
            }
        }
    }
    /* 0x00484f53 */
    if (cm_fn_0053b900((u32 *)arg2) == 0) {
        /* 0x00484f60 */
        if (*(s32 *)arg1 < DAT_00ae2364) {
            /* 0x00484f6b */
            if ((u16)loc.w04 != 0) {
                FINISH(0x0c);
            }
        }
    }
    /* 0x00484f8a */
    if (cm_fn_0053b900((u32 *)arg2) == 0) {
        /* 0x00484f97 */
        if (*(s32 *)arg1 < DAT_00ae2364) {
            /* 0x00484fa6 */
            if (cm_fn_00544600(arg1, 0) != 0) {
                FINISH(0x0d);
            }
        }
    }
    /* 0x00484fc9 */
    if (cm_fn_0053b900((u32 *)arg2) == 0) {
        /* 0x00484fd6 */
        if (*(s32 *)arg1 < DAT_00ae2364) {
            /* 0x00484fe4 */
            if (cm_fn_005444a0(arg1, 0) != 0) {
                FINISH(0x0e);
            }
        }
    }
    /* 0x00485007 */
    if (cm_fn_0053b900((u32 *)arg2) == 0) {
        /* 0x00485014 */
        if (*(s32 *)arg1 < DAT_00ae2364) {
            /* 0x00485022 */
            if (cm_fn_00544590(arg1, 0) != 0) {
                FINISH(0x0f);
            }
        }
    }
    /* 0x00485045 */
    if (cm_fn_0053b900((u32 *)arg2) == 0) {
        /* 0x00485052 */
        if (*(s32 *)arg1 < DAT_00ae2364) {
            /* 0x00485062 */
            if (loc.club != 0) {
                /* 0x00485068 */
                if (cm_fn_0085fa20(loc.club) != 0) {
                    FINISH(0x10);
                }
            }
        }
    }
    /* 0x00485088 */
    if (*(s32 *)arg1 < DAT_00ae2364) {
        /* 0x0048509f */
        if (cm_fn_007a7eb0((s32 *)arg1) != 0) {
            /* 0x004850a8 */
            if ((s8) *((u8 *)arg1 + 0x18) > 0x1e) {
                FINISH(0x11);
            }
        }
    }
    /* 0x004850c5 */
    if (loc.club != 0) {
        /* 0x004850c9 */
        if (*((u8 *)loc.club + 0x49) != 0) {
            FINISH(0x12);
        }
        /* 0x004850e4 */
        if (*(u32 *)((u8 *)loc.club + 0x45) != 0) {
            FINISH(0x13);
        }
        /* 0x004850eb */
        if (cm_fn_008676f0((volatile u8 *)loc.club) != 0) {
            FINISH(0x13);
        }
        /* 0x0048510a */
        if ((*((u8 *)loc.club + 0x35) & 0x3f) == 2 &&
            (*((u8 *)loc.club + 0x4f) & 0xf0) == 0) {
            /* 0x0048511b */
            if (*(u32 *)((u8 *)arg1 + 0x61) != 0) {
                FINISH(0x14);
            }
        }
    }
    FINISH(0x19);

second_half:
    /* 0x00485161 */
    if (*(s32 *)arg1 < DAT_00ae2364) {
        /* 0x00485175 */
        if (cm_fn_008cca10((u32)PTR_00de1f54, arg1, 0) != 0) {
            FINISH(0x18);
        }
    }
    /* 0x00485196 */
    if (arg2 == 0 || cm_fn_0053b900((u32 *)arg2) == 0) {
        /* 0x004851a7 */
        if (*(s32 *)arg1 < DAT_00ae2364) {
            /* 0x004851bc */
            if (cm_fn_008cde70((u32)PTR_00de1f54, arg1, 1) != 0) {
                FINISH(0x0b);
            }
        }
    }
    /* 0x004851e0 */
    if (arg2 == 0 || cm_fn_0053b900((u32 *)arg2) == 0) {
        /* 0x004851f1 */
        if (*(s32 *)arg1 < DAT_00ae2364) {
            /* 0x004851fd */
            if ((u16)loc.w04 != 0) {
                FINISH(0x0c);
            }
        }
    }
    /* 0x00485220 */
    if (*(s32 *)arg1 < DAT_00ae2364) {
        /* 0x00485236 */
        if (cm_fn_007f8d70((u32)PTR_00b74448, arg1, 0, 0) != 0) {
            FINISH(0x17);
        }
    }
    /* 0x0048525a */
    if (arg2 == 0 || cm_fn_0053b900((u32 *)arg2) == 0) {
        /* 0x0048526b */
        if (*(s32 *)arg1 < DAT_00ae2364) {
            /* 0x0048527a */
            if (cm_fn_00544600(arg1, 0) != 0) {
                FINISH(0x0d);
            }
        }
    }
    /* 0x004852a1 */
    if (arg2 == 0 || cm_fn_0053b900((u32 *)arg2) == 0) {
        /* 0x004852b2 */
        if (*(s32 *)arg1 < DAT_00ae2364) {
            /* 0x004852c1 */
            if (cm_fn_005444a0(arg1, 0) != 0) {
                FINISH(0x0e);
            }
        }
    }
    /* 0x004852e8 */
    if (arg2 == 0 || cm_fn_0053b900((u32 *)arg2) == 0) {
        /* 0x004852f9 */
        if (*(s32 *)arg1 < DAT_00ae2364) {
            /* 0x00485307 */
            if (cm_fn_00544590(arg1, 0) != 0) {
                FINISH(0x0f);
            }
        }
    }
    /* 0x0048532e */
    if (arg2 == 0 || cm_fn_0053b900((u32 *)arg2) == 0) {
        /* 0x0048533f */
        if (*(s32 *)arg1 < DAT_00ae2364) {
            /* 0x0048534b */
            if (loc.club != 0) {
                /* 0x00485351 */
                if (cm_fn_0085fa20(loc.club) != 0) {
                    FINISH(0x10);
                }
            }
        }
    }
    /* 0x00485375 */
    if (*(s32 *)arg1 < DAT_00ae2364) {
        /* 0x00485388 */
        if (cm_fn_007a7eb0((s32 *)arg1) != 0) {
            FINISH(0x11);
        }
    }
    /* 0x004853ac */
    if (loc.club != 0) {
        /* 0x004853b0 */
        if (*((u8 *)loc.club + 0x49) != 0) {
            FINISH(0x12);
        }
        /* 0x004853d2 */
        if (*(u32 *)((u8 *)loc.club + 0x45) != 0) {
            FINISH(0x13);
        }
        /* 0x004853f4 */
        if ((*((u8 *)loc.club + 0x35) & 0x3f) == 2 &&
            (*((u8 *)loc.club + 0x4f) & 0xf0) == 0) {
            /* 0x00485403 */
            if (*(u32 *)((u8 *)arg1 + 0x61) != 0) {
                FINISH(0x14);
            }
        }
    }
    /* 0x00485425 */
    if (cvar1 == 2) {
        FINISH(6);
    }
    /* 0x00485448 */
    if (cvar1 == 1) {
        FINISH(8);
    }
    FINISH(0x19);

#undef FINISH
}
