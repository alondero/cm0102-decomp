/* Preferred-image VA 0x00646bb0, image base 0x00400000, CM0102 3.9.68
   (SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1c627a22c445d14d489bd9bff86).

   Out-of-frame weight observations (the held concern for this routine).

   The listing addresses every local from one scratch base A = esp after
   `sub esp,0x270` (0x00646bc5).  The SEH record is pushed just above it, so
   prev/handler/state live at A+0x270/A+0x274/A+0x278 (0x00646bb0..0x00646bc5,
   restored from [esp+0x280] at 0x00646e53).  The weight bytes are read as
   `[esp+ecx+0x14]`/`[esp+ecx+0x15]` with ecx = sign-extended low word of the
   loop index (0x00646c40, 0x00646c6d, 0x00646c71), i.e. A+4+i and A+5+i, with
   no bound on i: only A+4..A+0x13 are initialised (0x00646bd0..0x00646c33,
   value 1 at A+4, 2 at A+5..A+7, 3 at A+0x14..A+0x17, 0 at A+8..A+0x13) while
   the loop runs to [esi+0x3e]-1 entries.  For i above 0x13 the routine therefore
   reads its own uninitialised scratch, then the linked SEH record, then the
   caller's stack.

   A plain-C `u8 scratch[0x270]` leaves the record (A+0x270..A+0x27B) to the
   compiler's own frame neighbours, so the weight reads at A+0x26C..A+0x277 and
   every read above A+0x27C observe spill slots whose contents change with the
   optimisation level.  One object carrying the scratch and the record at the
   listing's offsets keeps the bytes the routine itself owns in memory we own
   and never spill into, so both builds observe the same initialised scratch
   bytes plus the same record bytes the original has there at loop time.  The
   unbounded pair table (A+0x20 + 4n, stores at 0x00646c7f/0x00646ca5) and
   weight reads share the address mapping below: offsets at or above A+0x27c
   refer to the caller's return slot and following bytes, not memory beyond
   the allocated frame object.  In particular, table index 151 and weight
   index 632 both address A+0x27c.  Nothing in the region is initialised beyond
   the listing's own stores.  This address mapping does not establish callee
   composition or runtime proof for a table spanning the frame boundary.

   Indices use wrapping 32-bit address arithmetic; no signed-overflow wrapping.
   No direct callers establish a narrower return; EAX is kept in full. */
#include "cmre.h"

typedef struct {
    u32 prev;
    u32 handler;
    u32 state;
} cm_00646bb0_seh;

#define CM_00646bb0_SCRATCH 0x270u

/* A = esp after `sub esp,0x270`; the record ends exactly at the call's return
   address (A = return address - 0x27C), so byte A+0x27C is the caller's stack
   and is deliberately not part of this object. */
struct cm_00646bb0_frame {
    u8 scratch[CM_00646bb0_SCRATCH];
    /* listing 0x00646bb0..0x00646bc5: the record is pushed immediately above
       the scratch area and is read back at 0x00646e53 */
    volatile cm_00646bb0_seh seh;
};

extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_THISCALL cm_fn_00522e00(u32, u32, s16, u32, s16, u32,
                                     u32, u16, u8, u8, u16, u8,
                                     u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0051c800(u32, volatile u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

/* Preferred-image VAs 0x00646bc5/0x00646bfd establish ESP=A-0x10.
   0x00646c6d/0x00646c71 and 0x00646c7f/0x00646ca5 use that same base.
   Both access families must cross the return-slot boundary together.
   Binary identity is the pinned 3.9.68 SHA-256 stated above. */
#define STACK_ADDRESS(off) \
    ((s32)(u32)(off) < 0x27c ? work + (u32)(off) : \
     (u32)__builtin_frame_address(0) + 4u + (u32)(off) - 0x27cu)

u32 CM_THISCALL cm_fn_00646bb0(u32 self)
{
    struct cm_00646bb0_frame mem;
    u32 work = (u32)mem.scratch;
    u32 index = 0, count = 0, teams, row, base, offset, list_index;
    u32 value, other, first, second, owner, table, target, round_data;
    u32 allocation, eax;
    u16 score, year;
    char *version;

    /* 0x00646bb0: push -1 / push 0x0095ba4e / link fs:[0] */
    mem.seh.state = 0xffffffffu;
    mem.seh.handler = 0x0095ba4eu;
    mem.seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&mem.seh;

    /* 0x00646bd0: [esp+0x11..0x13] with three saved pushes live -> A+5..A+7 */
    B(work + 5u) = 2;
    B(work + 6u) = 2;
    B(work + 7u) = 2;
    /* 0x00646be2: [esp+0x20..0x23] with three saved pushes live -> A+0x14..0x17 */
    B(work + 0x14u) = 3;
    B(work + 0x15u) = 3;
    B(work + 0x16u) = 3;
    B(work + 0x17u) = 3;
    /* 0x00646bf2 */
    teams = W(self + 0x3eu);
    /* 0x00646bfe: edi is pushed now, so [esp+0x10] is A+0 */
    D(work) = 0;
    B(work + 4u) = 1;
    for (row = 8; row < 20; ++row) B(work + row) = 0;

    if ((s16)teams <= 0) goto pairs_ready;
next_row:
    /* 0x00646c3d */
    row = (u32)(s32)(s16)index;
    if ((s32)row < (s32)(s16)teams - 1) {
        /* 0x00646c4c */
        base = D(self + 0xb1u);
        offset = row * 59u;
        score = W(base + offset + 0x19u);
        if (score == W(base + offset + 0x54u)) {
            /* The bytes beyond the SEH record belong to the caller, not
               to the compiler's frame adjoining our local scratch.  At
               0x00646bc5 A is entry ESP-0x27c; entry ESP is the return slot. */
            value = B(STACK_ADDRESS(row + 4u));
            if (value != B(STACK_ADDRESS(row + 5u))) {
                /* 0x00646c77 */
                list_index = (u32)(s32)(s16)(u16)D(work);
                D(STACK_ADDRESS(32u + list_index * 4u)) = D(base + offset);
                value = D(work) + 1u;
                row = row + 1u;
                D(work) = value;
                list_index = (u32)(s32)(s16)(u16)value;
                other = D(base + row * 59u);
                count = value + 1u;
                D(STACK_ADDRESS(32u + list_index * 4u)) = other;
                B(base + offset + 0x3au) = (u8)(index + 2u);
                base = D(self + 0xb1u);
                D(work) = count;
                index = index + 1u;
                B(base + offset + 0x74u) = (u8)index;
                base = D(self + 0xb1u);
                B(base + offset + 0x37u) = 1;
                base = D(self + 0xb1u);
                B(base + offset + 0x72u) = 1;
                goto advance;
            }
        }
    }
    /* 0x00646ce2 */
    if ((s16)index > 0) {
        offset = row * 59u;
        base = D(self + 0xb1u);
        score = W(base + offset + 0x19u);
        if (score == W((base + offset) - 0x22u)) {
            B(base + offset + 0x3au) = (u8)(index + 1u);
            base = D(self + 0xb1u);
            B(base + offset + 0x39u) = B(base + offset + 0x3au);
        }
    }
advance:
    /* 0x00646d21 */
    teams = W(self + 0x3eu);
    index = index + 1u;
    if ((s16)index < (s16)teams) goto next_row;
    count = D(work);
pairs_ready:
    /* 0x00646d33 */
    eax = (u32)((s32)(s16)(u16)count / 2);
    B(self + 0xeeu) = (u8)eax;
    if ((u8)eax == 0) {
        D(self + 0x2cu) = 0;
        D(self + 0x30u) = 0xffffffffu;
        D(self + 0xe6u) = 0;
        goto done;
    }
    /* 0x00646d5a: indirect virtual call, this = self */
    table = D(self);
    target = D(table + 0x3cu);
    round_data = ((u32 (CM_THISCALL *)(u32, u32, u32, u32, u32))target)
        (self, 0, work + 24u, work + 28u, 0);
    allocation = (u32)cm_fn_00944cf1(0xb2);
    D(work) = allocation;
    mem.seh.state = 0;
    if (allocation != 0) {
        second = D(work + 28u);
        year = W(self + 0x40u);
        owner = D(self + 4u);
        first = D(work + 24u);
        eax = cm_fn_00522e00(allocation, self, (s16)(u16)count, work + 32u,
                             (s16)(u16)first, owner, round_data, year, 0, 1,
                             (u16)second, 20, 0, 0, 0, 0);
    } else {
        eax = 0;
    }
    /* 0x00646dba */
    table = D(self + 12u);
    mem.seh.state = 0xffffffffu;
    D(table) = eax;
    table = D(self + 12u);
    if (D(table) == 0) {
        /* 0x00646dd1 */
        cm_fn_00944cff(0x009ea384u, 0, 0, (void *)(work + 0x170u),
                       (void *)(work + 0x70u));
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)version,
                       work + 0x170u, work + 0x70u, 0x28eu);
        eax = cm_fn_005e8290((const char *)0x009870e0u,
                             (const char *)0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
    } else {
        /* 0x00646e35 */
        cm_fn_009452ca(round_data);
        table = D(self + 12u);
        eax = cm_fn_0051c800(D(table), 0);
        D(self + 0x30u) = 0;
        B(self + 0xc5u) = 6;
    }
done:
    /* 0x00646e53: saved predecessor, which the unbounded table may have
       overwritten in the original too */
    CM_FS(u32, 0) = mem.seh.prev;
    return eax;
}
