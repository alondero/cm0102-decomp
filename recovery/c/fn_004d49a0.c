/* fn_004d49a0 - preferred-image VA 0x004d49a0 (module contract_manager.cpp)
 *
 * Recovered from the listing in the farm bundle for this job.
 *   entry      : __thiscall, ECX = self (read before written), ret 8 (2 stack args)
 *   stack args : [esp+4] = contract record pointer, [esp+8] = flag word
 *   locals     : one scratch area, base L (offsets are the original's
 *                [esp+disp] offsets with L == the address just after the
 *                sub esp / register pushes, i.e. L + disp)
 *
 * Scratch map (offsets from L):
 *   0x10 u32  result of cm_fn_004cf5f0
 *   0x14 u32  date word written by the comparison block
 *   0x18 u32  second word written by the comparison block
 *   0x1c      list object (cm_fn_0068d020 / 0x68d040 / 0x68d1b0)
 *   0x1e u32  list field copied to 0x26
 *   0x26 u32  list field copy destination
 *   0x2c u32  int slot written through cm_fn_00846920's last argument
 *   0x30      value container (cm_fn_0077bbe0 / 0x785790 / 0x7861e0)
 *   0x110     dword list filled by cm_fn_008d1da0
 *   0x188     buffer A
 *   0x288     buffer B
 */

#include "cmre.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

/* ------------------------------------------------------------------ */
/* original globals (preferred image)                                  */
/* the reads are volatile so that they keep the listing's order and    */
/* count (the original re-reads several of them).                      */
/* ------------------------------------------------------------------ */
#define G_ae1010 CM_GLOBAL(volatile u16, 0x00ae1010)
#define G_ae1012 CM_GLOBAL(volatile u16, 0x00ae1012)
#define G_ae1068 CM_GLOBAL(volatile u16, 0x00ae1068)
#define G_ae106a CM_GLOBAL(volatile u16, 0x00ae106a)
#define G_ae23b4 CM_GLOBAL(volatile u32, 0x00ae23b4)
#define G_ae23bc CM_GLOBAL(volatile u32, 0x00ae23bc)
#define G_ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define G_ae2d0c CM_GLOBAL(volatile u32, 0x00ae2d0c)
#define G_b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define G_de1f54 CM_GLOBAL(volatile u32, 0x00de1f54)
#define G_b7431c CM_GLOBAL(volatile u32, 0x00b7431c)

/* ------------------------------------------------------------------ */
/* original callees                                                     */
/* ------------------------------------------------------------------ */
extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self, volatile u32 person, u32 club);
extern u32 CM_CDECL cm_fn_0053ab90(u32 object, u32 date);
extern u32 CM_CDECL cm_fn_0053b620(u32 arg);
extern void CM_CDECL cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_0068d020(u32 self);
extern u32 CM_THISCALL cm_fn_0068d040(u32 self);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self_arg, u32 index_arg, u32 value_arg);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);
extern u32 CM_THISCALL cm_fn_00842180(u32 self, volatile u32 arg1, volatile u32 arg2, volatile u32 arg3, volatile u32 arg4, volatile u32 arg5);
/* the listing leaves no "add esp" after this call: the callee pops its six
   stack arguments (six pushes, 24 bytes) before the next stack access.
   0x004d4ba9 sets ecx to [0xde1f54]+0x856 for this call: the receiver. */
extern long double CM_THISCALL cm_fn_00846920(u32 self, int *param_1, int *param_2, int param_3, char param_4, u32 param_5, u32 *param_6);
extern s32 CM_THISCALL cm_fn_0085fa00(u32 self);
extern u32 CM_THISCALL cm_fn_008c61e0(void *self, u32 index);
/* same: no "add esp" after this call, the callee pops its four arguments.
   0x004d4f2f sets ecx to [0xde1f54] (null checked) for this call: receiver. */
extern char CM_THISCALL cm_fn_008d1da0(u32 self, int param_1, u32 param_2, int *param_3, char param_4);
/* one stack argument ("add esp, 4" after the call). */
extern unsigned long long CM_CDECL cm_fn_0090cfc0(u32 bound);
/* 6-byte cdecl at 0x0090d130: it ignores its three arguments and returns the
   version string at 0x00ad4394. The listing pushes exactly three words for it
   at 0x004d4b4a/0x004d4b4f/0x004d4b50 (buffer at L+0x288, buffer at L+0x188,
   0x20c3); the single "add esp, 0x24" at 0x004d4b77 also cleans the three
   pushes of each following call. */
extern const char *CM_CDECL cm_fn_0090d130(const char *file, const char *extension, u32 line);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

/* ------------------------------------------------------------------ */
/* helpers                                                              */
/* ------------------------------------------------------------------ */
typedef struct {
    void *prev;
    void *handler;
    u32   state;
} cm_seh_frame;

#define RD8(p)  (*(volatile u8 *)(u32)(p))
#define RD16(p) (*(volatile u16 *)(u32)(p))
#define RD32(p) (*(volatile u32 *)(u32)(p))

/* the container object at L+0x30 */
#define SET(IDX, VAL) cm_fn_00785790(cont, (u32)(IDX), (u32)(VAL))

void CM_THISCALL cm_fn_004d49a0(u32 self, int *param_1, u32 param_2)
{
    u8 scratch[0x388];
    cm_seh_frame volatile seh;
    volatile u8 *rec = (volatile u8 *)param_1;
    void *list = (void *)(scratch + 0x1c);
    void *cont = (void *)(scratch + 0x30);
    u32 club;
    u32 person;
    u32 res;
    u32 idx;
    u32 slot11;
    u32 flag14;
    u32 flag1a;
    u32 p61;
    s32  s35;
    s8   ar;

    /* 0x004d49a0 - 0x004d49b5: SEH frame, linked in listing order */
    seh.state   = 0xffffffffu;
    seh.handler = (void *)0x00956e66u;
    seh.prev    = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&seh;

    /* 0x004d49c1 */
    cm_fn_0068d020((u32)list);

    /* 0x004d49d1: the state word of the SEH frame */
    seh.state = 0u;

    /* 0x004d49dc: club record pointer (110 byte records) */
    idx  = RD32(rec + 0x00);
    club = G_ae23bc + 110u * idx;
    /* 0x004d49ef: person record pointer (581 byte records) */
    idx    = RD32(rec + 0x04);
    person = G_ae23b4 + 581u * idx;
    /* 0x004d4a05: push ebx (person) then push edi (club) -> arg1 = club */
    res = cm_fn_004cf5f0(self, club, person);
    /* 0x004d4a19 */
    RD32(scratch + 0x10) = res;

    /* 0x004d4a0e: container constructor (3000, 0) */
    cm_fn_0077bbe0(cont, 0xbb8u, 0u);

    /* 0x004d4a2b: mov byte ptr [esp + 0x398], 1, a BYTE store into the low
       byte of the linked frame's state dword (esp = L-8 after the two
       constructor-argument pushes, so [esp+0x398] = L+0x390 == the dword
       the entry frame zeroed at 0x004d49d1 and clears at 0x004d4f84). Thus
       the state seen by dispatch inside the guarded body is 1, not 0. */
    ((volatile u8 *)&seh.state)[0] = 1u;

    /* 0x004d4a22 - 0x004d4ad7: container slots 0..0xa */
    SET(0, RD32(club));
    SET(1, RD32(RD32(club + 0x04) + 0x33));
    SET(2, RD32(RD32(club + 0x08) + 0x33));
    SET(3, RD32(RD32(club + 0x0c) + 0x33));
    SET(4, RD32(rec + 0x04));
    SET(5, (u32)(s32)(s8)RD8(rec + 0x50));
    SET(6, (u32)(s32)(s16)RD16(rec + 0x2d));
    SET(7, (u32)RD16(rec + 0x2f));
    SET(8, RD32(rec + 0x59));

    /* 0x004d4ac4: (s8)((byte << 2) >> 2) */
    s35 = (s32)(s8)(u8)(RD8(rec + 0x35) << 2);
    s35 = s35 >> 2;
    SET(0xa, (u32)s35);

    /* 0x004d4adc: cm_fn_0053b620(byte) != 0 -> 0, == 0 -> 1 */
    SET(0xb, (cm_fn_0053b620((u32)RD8(rec + 0x61)) == 0u) ? 1u : 0u);

    /* 0x004d4afd: the person index slot */
    if (RD32(rec + 0x59) != 0xffffffffu) {
        u32 p = cm_fn_008c61e0((void *)G_de1f54, RD32(rec + 0x59));
        u32 v = RD32(p);
        if (v == 0u) {
            /* 0x004d4b1b */
            cm_fn_00944cff(0x009a8190u, 0u, 0u,
                           (void *)(scratch + 0x288), (void *)(scratch + 0x188));
            /* 0x004d4b3c - 0x004d4b51: the three pushes are the cdecl
               arguments of the 0x90d130 call: arg1 = L+0x288 (the file
               buffer, arg4 of the split call above), arg2 = L+0x188 (the
               extension buffer, its arg5), arg3 = 0x20c3 (the diagnostic
               line). The callee ignores them and returns the version
               string. They stay on the stack: the single add esp, 0x24 at
               0x004d4b77 cleans this call, 0x944c9f and 0x5e8290, so
               0x944c9f's varargs are version, file, extension and line. */
            const char *ver = cm_fn_0090d130((const char *)(scratch + 0x288),
                                             (const char *)(scratch + 0x188),
                                             0x20c3u);
            cm_fn_00944c9f((u32)&G_ae24d0, 0x009870e8u, (u32)ver,
                           (u32)(scratch + 0x288), (u32)(scratch + 0x188), 0x20c3u);
            /* 0x004d4b66 */
            cm_fn_005e8290((const char *)0x009870e0u, (const char *)&G_ae24d0, 0);
            /* 0x004d4b7a */
            G_b67a34 = 0u;
            goto L_end;
        }
        /* 0x004d4b89 */
        SET(9, RD32(v + 0x24));
    }

    /* 0x004d4b98: the long double helper writes an int through its last
       argument; the returned long double is discarded (fstp st(0)). */
    {
        long double ld = cm_fn_00846920(*(u32 *)(G_de1f54 + 0x856u),
                                         (int *)club, (int *)person, 0,
                                         (char)(s8)s35,
                                         (u32)(RD8(rec + 0x4f) >> 4),
                                         (u32 *)(scratch + 0x2c));
        (void)ld;
    }
    /* 0x004d4bc3 */
    SET(0xc, RD32(scratch + 0x2c));

    /* 0x004d4bd5 - 0x004d4c5b: slots 0xd..0x10 */
    {
        u32 pcf = RD32(person + 0xcf);
        if (pcf != 0u) {
            SET(0xd, RD32(pcf));
            SET(0xe, RD32(RD32(pcf + 0x04) + 0x33));
            SET(0xf, RD32(RD32(pcf + 0x08) + 0x33));
            SET(0x10, RD32(RD32(pcf + 0x0c) + 0x33));
        } else {
            SET(0xd, 0xffffffffu);
            SET(0xe, 0xffffffffu);
            SET(0xf, 0xffffffffu);
            SET(0x10, 0xffffffffu);
        }
    }

    /* 0x004d4c60 - 0x004d4cf5: slot 0x11, the two date thresholds */
    if (param_2 == 0u) {
        slot11 = 0u;
    } else {
        u32 date  = RD32(res + 0x2d);
        u32 other = RD32(res + 0x31);
        u16 dhi;
        u16 dlo;
        u16 a_hi = G_ae1012;
        u16 a_lo = G_ae1010;

        RD32(scratch + 0x14) = date;
        RD32(scratch + 0x18) = other;
        dhi = RD16(scratch + 0x16);
        dlo = RD16(scratch + 0x14);

        if (dhi > a_hi ||
            (dhi == a_hi && (s16)dlo > (s16)a_lo)) {
            /* 0x004d4cb0: second threshold */
            u16 b_hi = G_ae106a;
            u16 b_lo = G_ae1068;

            RD32(scratch + 0x14) = RD32(res + 0x2d);
            RD32(scratch + 0x18) = RD32(res + 0x31);
            dhi = RD16(scratch + 0x16);
            dlo = RD16(scratch + 0x14);

            if (dhi > b_hi ||
                (dhi == b_hi && (s16)dlo > (s16)b_lo)) {
                /* 0x004d4ce8 */
                param_2 = 0u;
                slot11 = 0u;
            } else {
                /* 0x004d4ce1 */
                slot11 = 1000u;
            }
        } else {
            /* 0x004d4ca9 */
            slot11 = 999u;
        }
    }
    SET(0x11, slot11);

    /* 0x004d4d00 */
    SET(0x12, 0x420e9u);

    /* 0x004d4d10 */
    {
        unsigned long long r = cm_fn_0090cfc0(3u);
        SET(0x13, (u32)r);
    }

    /* 0x004d4d26 */
    flag14 = 0u;
    if (person != 0u && res != 0u && RD8(rec + 0x50) == 2u) {
        flag14 = 1u;
    }
    SET(0x14, flag14);

    /* 0x004d4d4d - 0x004d4d9b */
    SET(0x15, (u32)(s32)(s8)RD8(rec + 0x61));
    SET(0x16, RD32(rec + 0x0c));
    SET(0x17, RD32(rec + 0x08));
    SET(0x18, (u32)(s32)(s16)RD16(rec + 0x25));
    SET(0x19, (u32)RD16(rec + 0x27));

    /* 0x004d4da0 - 0x004d4e1d: slot 0x1a */
    flag1a = 0u;
    p61 = RD32(club + 0x61);
    if (p61 != 0u && (u8)(RD8(rec + 0x35) & 0x3fu) == 2u) {
        int done = 0;
        if (RD8(p61 + 0x0f) == 0x14u) {
            ar = (s8)(u8)cm_fn_0053ab90(club, (u32)(rec + 0x2d));
            if (ar > 0x25) { flag1a = 1u; done = 1; }
        }
        if (!done) {
            if ((s8)RD8(p61 + 0x11) >= 0x0f) {
                ar = (s8)(u8)cm_fn_0053ab90(club, (u32)(rec + 0x2d));
                if (ar > 0x24) { flag1a = 1u; done = 1; }
            }
        }
        if (!done) {
            if ((s8)RD8(p61 + 0x13) >= 0x0f) {
                ar = (s8)(u8)cm_fn_0053ab90(club, (u32)(rec + 0x2d));
                if (ar > 0x23) { flag1a = 1u; done = 1; }
            }
        }
        if (!done) {
            if ((s8)RD8(p61 + 0x15) >= 0x0f) {
                ar = (s8)(u8)cm_fn_0053ab90(club, (u32)(rec + 0x2d));
                if (ar > 0x22) { flag1a = 1u; done = 1; }
            }
        }
    }
    SET(0x1a, flag1a);

    /* 0x004d4e28 */
    idx = RD32(club + 0x00);
    SET(0x1b, (u32)(RD8(G_ae2d0c + 79u * idx + 0x0bu) >> 7));

    /* 0x004d4e4c */
    if (param_2 != 0u) {
        goto L_club_list;
    }

    /* 0x004d4e5b: the person record path */
    if (person != 0u && res != 0u &&
        (s16)cm_fn_0085fa00(res) < 0xb4 && RD8(rec + 0x50) == 2u) {
        goto L_club_list;
    }

    /* 0x004d4e7c */
    {
        u32 pidx = RD32(rec + 0x04);
        u32 pptr = G_ae23b4 + 581u * pidx;
        cm_fn_007861e0((void *)G_b7431c, (u32)cont, pptr);
    }
    /* 0x004d4ea3: ecx = [edx + 0x856] with edx = [0xde1f54] */
    cm_fn_00842180(*(u32 *)(G_de1f54 + 0x856u), club, (u32)list, 1u, 1u, 0u);

    /* 0x004d4ec0 */
    if (RD8(rec + 0x50) == 2u && (u8)(RD8(rec + 0x35) & 0x3fu) == 2u) {
        /* 0x004d4ed9 */
        RD32(scratch + 0x26) = RD32(scratch + 0x1e);
        {
            u32 elem = cm_fn_0068d1b0((u32)list);
            while (elem != 0u) {
                u32 eidx = RD32(elem + 0x04);
                if (eidx != RD32(rec + 0x04)) {
                    u32 ep = G_ae23b4 + 581u * eidx;
                    cm_fn_007861e0((void *)G_b7431c, (u32)cont, ep);
                }
                elem = cm_fn_0068d1b0((u32)list);
            }
        }
    }
    goto L_end;

L_club_list:
    /* 0x004d4f2f */
    {
        u8 cnt = 0u;
        if (G_de1f54 != 0u && RD32(club + 0x61) != 0u) {
            cnt = (u8)cm_fn_008d1da0(G_de1f54, (int)club, 0x21au,
                                     (int *)(scratch + 0x110), 0x1e);
        }
        if ((s8)cnt > 0) {
            u8 *p = scratch + 0x110;
            s32 n = (s8)cnt;
            do {
                cm_fn_007861e0((void *)G_b7431c, (u32)cont, RD32(p));
                p += 4;
                n--;
            } while (n != 0);
        }
    }

L_end:
    /* 0x004d4f80 */
    seh.state = 0xffffffffu;
    cm_fn_0068d040((u32)list);
    CM_FS(void *, 0) = (void *)seh.prev;
}
