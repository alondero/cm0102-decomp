/* Translation of preferred-image VA 0x006aab70 from CM0102 3.9.68
   (CM0102/Game/cm0102.exe, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86).

   Entry ABI: 0x006aad26 and 0x006aada0 execute "ret 0xc", so the callee pops
   three dword arguments; ECX is never read before it is written, so there is
   no thiscall receiver.  The entry is therefore CM_STDCALL with three u32
   arguments: [esp+4] (receiver for 0x00785830), [esp+8] (branch selector),
   [esp+0xc] (receiver for 0x004ae660 / 0x004ae8a0).

   Semantic gate: `cmre test fn_006aab70 --c` passes at -O0 and -O2. */
#include "cmre.h"

#define DAT_00ae23a8 CM_GLOBAL(int, 0x00ae23a8)
#define DAT_00ae23b4 CM_GLOBAL(int, 0x00ae23b4)
#define DAT_00de1f64 CM_GLOBAL(u32, 0x00de1f64)

/* 0x006aac43, 0x006aacee and 0x006aad58 push the address 0x00de1f64, i.e. the
   address of the 0x00de1f64 global. */
#define DEST_00de1f64 ((u32)(unsigned long)&DAT_00de1f64)

/* Format strings in the original image (globals at preferred-image VAs). */
#define s_FA_reaction_to_request_for_match_00a11680 ((char *)0x00a11680)
#define s_FA_reaction_to_complaint_about_r_00a116b0 ((char *)0x00a116b0)
#define s_The__s_Football_Association_indi_00a11c64 ((char *)0x00a11c64)
#define eg__Eng_00a116d8 ((char *)0x00a116d8)
#define eg__Eng_00a117b0 ((char *)0x00a117b0)
#define eg__Eng_00a11888 ((char *)0x00a11888)
#define eg__Eng_00a11998 ((char *)0x00a11998)
#define eg__Eng_00a11a64 ((char *)0x00a11a64)
#define eg__Eng_00a11b18 ((char *)0x00a11b18)
#define eg__Eng_00a11bc4 ((char *)0x00a11bc4)
#define eg__Eng_00a11cb0 ((char *)0x00a11cb0)
#define eg__Eng_00a11dcc ((char *)0x00a11dcc)
#define eg__Eng_00a11ea4 ((char *)0x00a11ea4)

/* Callee ABI, from original instructions at preferred-image VAs of the pinned
   3.9.68 binary (SHA-256 0c78e7da...bff86):
   - 0x00785830: one dword index pushed and ECX loaded from the receiver
     (0x006aab8c, 0x006aab95, 0x006aaba0, 0x006aabad, 0x006aabb8, 0x006aad32,
     0x006aad3b); the caller never cleans the pushed index -> thiscall.
   - 0x004ae660: one dword pushed, ECX = receiver (0x006aacf3, 0x006aad6c);
     caller never cleans -> thiscall.
   - 0x004ae8a0: three dwords pushed, ECX = receiver (0x006aad0e, 0x006aad87);
     caller never cleans -> thiscall.
   - 0x006181d0: four dwords pushed (0x006aabcf, 0x006aabd7, 0x006aabd9,
     0x006aabdd) and the caller cleans 0x10 at 0x006aabe3 -> cdecl.
   - 0x00944c9f: three dwords pushed (0x006aac3d, 0x006aac3e, 0x006aac43) and
     the caller cleans 0xc at 0x006aac4d -> cdecl, variadic tail.
   - 0x0066f4e0: five dwords are pushed at 0x006aabef/0x006aabf0/0x006aabf2/
     0x006aabf4/0x006aacda (destination, format, 0x64, 0x64, buffer) with the
     caller cleaning 0x14 at 0x006aace4, and two dwords are pushed at
     0x006aad4c/0x006aad58 (destination, string) with the caller cleaning 8 at
     0x006aad69 -> cdecl with a variadic tail; the variadic prototype keeps
     both original stack layouts. */
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, const char *name, u32 key_a, u32 key_b);
extern unsigned long long CM_CDECL cm_fn_006181d0(u32 arg1, u32 arg2, u32 arg3, u32 arg4);
extern char * CM_CDECL cm_fn_0066f4e0(u32 dest, const char *format, ...);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);

void CM_STDCALL cm_fn_006aab70(u32 arg1, u32 arg2, u32 arg3)
{
    /* The original reserves 0x7c local bytes below the saved registers.
       The 0x006181d0 buffer starts at local offset 4 (0x006aabd1:
       "lea eax, [esp+0x18]") and the byte written at 0x006aaba2
       ("mov byte ptr [esp+0x17], al") sits one byte below it, at offset 3.
       Both stay in one allocation so the original adjacency is preserved. */
    union {
        u32 align;
        u8 b[0x7c];
    } scratch;
    u8 *buf = &scratch.b[4];
    /* The byte is stored before the 0x006181d0 call and loaded after it
       (0x006aac2e: "movsx eax, byte ptr [esp+0x13]"), so it must stay in
       memory across that call. */
    volatile u8 *b1_store = &scratch.b[3];
    u32 r0;
    u32 r1;
    u32 r2;
    u32 r3;
    u32 r4;
    u8 b2;
    s8 b1;
    const char *format;

    if (arg2 == 0) {
        /* 0x006aad29 */
        r0 = (u32)cm_fn_00785830((void *)(unsigned long)arg1, 0);
        r1 = (u32)cm_fn_00785830((void *)(unsigned long)arg1, 1);
        if ((s8)r1 == 2) {
            format = s_FA_reaction_to_request_for_match_00a11680;
        } else {
            format = s_FA_reaction_to_complaint_about_r_00a116b0;
        }
        cm_fn_0066f4e0(DEST_00de1f64, format);
        cm_fn_004ae660((void *)(unsigned long)arg3, DEST_00de1f64);
        {
            u32 base = (u32)((u32)DAT_00ae23b4 + (u32)(r0 * 581u));
            cm_fn_004ae8a0(arg3, (const char *)(unsigned long)(base + 4u), 0x7d5u, base);
        }
        return;
    }

    /* 0x006aab81 */
    r0 = (u32)cm_fn_00785830((void *)(unsigned long)arg1, 0);
    r1 = (u32)cm_fn_00785830((void *)(unsigned long)arg1, 1);
    *b1_store = (u8)r1;
    r2 = (u32)cm_fn_00785830((void *)(unsigned long)arg1, 2);
    r3 = (u32)cm_fn_00785830((void *)(unsigned long)arg1, 3);
    r4 = (u32)cm_fn_00785830((void *)(unsigned long)arg1, 4);

    /* 0x006aabc1..0x006aabe3: destination = 0x00ae23a8 + r3 * 0x122. */
    cm_fn_006181d0((u32)((u32)DAT_00ae23a8 + (u32)(r3 * 290u)),
                   (u32)(unsigned long)buf, 0x78u, 9u);

    b2 = (u8)r2;
    b1 = (s8)*b1_store;

    if (b2 == 0x0c) {
        format = eg__Eng_00a11ea4;
    } else if (b2 == 4) {
        format = eg__Eng_00a11dcc;
    } else if (b2 == 0x0a) {
        format = eg__Eng_00a11cb0;
    } else if (b1 == 1) {
        if (b2 == 0) {
            format = eg__Eng_00a116d8;
        } else if ((s32)r4 < 5) {
            format = eg__Eng_00a11998;
        } else if ((s32)r4 < 6) {
            format = eg__Eng_00a11888;
        } else {
            format = eg__Eng_00a117b0;
        }
    } else if (b1 == 2) {
        if (b2 == 0) {
            format = eg__Eng_00a11a64;
        } else if (b2 == 6) {
            format = eg__Eng_00a11b18;
        } else {
            format = eg__Eng_00a11bc4;
        }
    } else {
        /* 0x006aac39: the 0x0066f4e0 call is skipped. */
        cm_fn_00944c9f(DEST_00de1f64,
                       (u32)(unsigned long)s_The__s_Football_Association_indi_00a11c64,
                       (u32)(unsigned long)buf);
        format = 0;
    }

    if (format != 0) {
        cm_fn_0066f4e0(DEST_00de1f64, format, 100, 100, (u32)(unsigned long)buf);
    }

    /* 0x006aace7 */
    cm_fn_004ae660((void *)(unsigned long)arg3, DEST_00de1f64);
    {
        u32 base = (u32)((u32)DAT_00ae23b4 + (u32)(r0 * 581u));
        cm_fn_004ae8a0(arg3, (const char *)(unsigned long)(base + 4u), 0x7d5u, base);
    }
}
