/* Reconstruction of FUN_009035a0, preferred-image VA 0x009035a0.
   Pinned binary: CM0102 3.9.68 SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Frame: `sub esp,0x2e4` at 0x009035b9 after three SEH pushes.  The scratch
   region is F..F+0x2e4 and the linked SEH record sits at F+0x2e4..F+0x2f0. */
#include "cmre.h"

#define EXCEPTION_LIST CM_FS(void *, 0)
#define DAT_009cf5bc CM_GLOBAL(u32, 0x009cf5bc)
#define DAT_009cf6f8 CM_GLOBAL(u32, 0x009cf6f8)
#define DAT_00adadfc CM_GLOBAL(u32, 0x00adadfc)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
/* 0x009035ce reads a word at 0x00ae2c90; 0x009036b6 reads a word at 0x00ae2c92.
   Ghidra's `DAT_00ae2c90._2_2_` is the u16 offcut at 0x00ae2c92. */
#define DAT_00ae2c90 CM_GLOBAL(u16, 0x00ae2c90)
#define DAT_00ae2c92 CM_GLOBAL(u16, 0x00ae2c92)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define DAT_00b7431c CM_GLOBAL(u32, 0x00b7431c)

#define STR_009870E0 ((u32)0x009870e0)
#define STR_009870E8 ((u32)0x009870e8)
#define STR_00AA1A1C ((u32)0x00aa1a1c)
#define SEH_HANDLER  ((void *)0x0096521b)

/* Callee ABIs from the original calls (cleanup verified against the 0x0090363b
   loop balance 0x7875e0=4, 0x785790=8, 0x7861e0=8) and the explicit
   `add esp,0x14` / `add esp,0x24` / `add esp,4` caller cleanups. */
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index_arg, u32 value_arg);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);
extern u32 CM_STDCALL cm_fn_007875e0(u32 club);
extern void CM_THISCALL cm_fn_00903b50(u32 self);
extern u32 CM_THISCALL cm_fn_00903cc0(u32 self);
extern u32 CM_THISCALL cm_fn_00904040(u32 self, u32 output);
extern u32 CM_THISCALL cm_fn_00904210(u32 self, volatile u32 first_arg, volatile u32 second_arg);
extern void CM_THISCALL cm_fn_00904580(int *param_1, u32 param_2, u32 param_3);
extern void CM_THISCALL cm_fn_00904e30(u32 self, volatile u32 records, volatile u32 nrecords);
/* 0x009036aa / 0x00903789 set ECX=ebx immediately before these calls:
   the original ABI passes the receiver in ECX (thiscall, no stack args). */
extern void CM_THISCALL cm_fn_009058e0(u32 self);
extern u32 CM_THISCALL cm_fn_00905f00(u32 self);
extern char *CM_CDECL cm_fn_0090d130(u32 arg1, u32 arg2, u32 arg3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg);

typedef struct {
    volatile void *prev;     /* F+0x2e4: [esp+0x2f0] at 0x00903790 */
    volatile void *handler;  /* F+0x2e8: 0x96521b at 0x009035a8 */
    volatile u32 state;      /* F+0x2ec: [esp+0x300]/[esp+0x2f8] */
} CmSehFrame;

void CM_THISCALL cm_fn_009035a0(u8 *self, u32 mode)
{
    CmSehFrame frame;
    u8 scratch[0x2e4];
    void *prev;
    u32 esi;
    s16 counter;
    u32 home;
    u32 result;

#define LOCAL_2F0 (*(volatile u32 *)(scratch + 0x000))
#define LOCAL_2EC (scratch + 0x004)
#define LOCAL_20C (scratch + 0x0e4)
#define LOCAL_10C (scratch + 0x1e4)

    /* 0x009035a0-0x009035b2: save fs:[0], link the SEH record. */
    prev = (void *)EXCEPTION_LIST;
    frame.state = 0xffffffff;      /* push -1 */
    frame.handler = SEH_HANDLER;   /* push 0x96521b */
    frame.prev = prev;             /* push eax */
    EXCEPTION_LIST = &frame;

    if (mode == 0) {
        u16 season = DAT_00ae2c92;                       /* 0x009036b6 */
        if (season != *(u16 *)(self + 0x439)) {           /* 0x009036bc */
            *(u16 *)(self + 0x439) = season;              /* 0x009036cb */
            cm_fn_00903b50((u32)self);                    /* 0x009036d2 */
            cm_fn_00903cc0((u32)self);                    /* 0x009036d9 */
            result = cm_fn_00904040((u32)self, (u32)&LOCAL_2F0); /* 0x009036e5 */
            esi = result;
            if ((u16)result == 0) {                       /* 0x009036ec */
                cm_fn_00944cff(STR_00AA1A1C, 0, 0, (void *)LOCAL_10C, (void *)LOCAL_20C);
                result = (u32)cm_fn_0090d130((u32)LOCAL_10C, (u32)LOCAL_20C, 0x2b7);
                cm_fn_00944c9f((u32)&DAT_00ae24d0, STR_009870E8, result);
                cm_fn_005e8290((const char *)STR_009870E0, (const char *)&DAT_00ae24d0, 0);
                DAT_00b67a34 = 0;                         /* 0x0090374d */
            } else {
                home = LOCAL_2F0;                         /* 0x00903755 */
                cm_fn_00904210((u32)self, home, esi);
                home = LOCAL_2F0;                         /* 0x00903762 */
                cm_fn_00904580((int *)self, home, esi);
                home = LOCAL_2F0;                         /* 0x0090376f */
                cm_fn_00904e30((u32)self, home, esi);
                home = LOCAL_2F0;                         /* 0x0090377c */
                cm_fn_009452ca(home);
            }
            cm_fn_00905f00((u32)self);                    /* 0x0090378b */
        }
    } else {
        if (DAT_00ae2c90 == 100) {                        /* 0x009035ce */
            u32 base = DAT_00adadfc;                      /* 0x009035dc */
            esi = *(u32 *)(base + DAT_009cf5bc * 4);      /* 0x009035e7 */
            if (esi == 0)                                 /* 0x009035ea */
                goto done;
            if (*(u32 *)(base + DAT_009cf6f8 * 4) != 0) { /* 0x009035f8 */
                cm_fn_0077bbe0((void *)LOCAL_2EC, 0x16, 0);
                frame.state = 0;                          /* 0x00903613 */
                cm_fn_00785790((void *)LOCAL_2EC, 0, 0);
                cm_fn_00785790((void *)LOCAL_2EC, 2, *(u16 *)(self + 0x439));
                counter = 0;                              /* xor ebp,ebp */
                while (counter < *(s16 *)(esi + 0x3e)) {  /* 0x00903698 */
                    /* movsx eax,bp; eax*8-eax -> *7; +eax*4 -> *29; +eax*2 -> *59 */
                    u32 idx = (u32)((s32)counter * 59);
                    u32 club = *(volatile u32 *)(*(volatile u32 *)(esi + 0xb1) + idx);
                    if (cm_fn_007875e0(club) != 0) {       /* 0x0090365d */
                        club = *(volatile u32 *)(*(volatile u32 *)(esi + 0xb1) + idx);
                        cm_fn_00785790((void *)LOCAL_2EC, 1, *(u32 *)club);
                        club = *(volatile u32 *)(*(volatile u32 *)(esi + 0xb1) + idx);
                        cm_fn_007861e0((void *)DAT_00b7431c, (u32)LOCAL_2EC, club);
                    }
                    counter = (s16)(counter + 1);         /* inc ebp */
                }
                frame.state = 0xffffffff;                 /* 0x0090369f */
            }
        }
        /* 0x009035d6 `jne 0x9036aa` and 0x009036b1 both reach this call. */
        cm_fn_009058e0((u32)self);                        /* 0x009036ac */
    }

done:
    EXCEPTION_LIST = (void *)frame.prev;                  /* 0x0090379a */
}
