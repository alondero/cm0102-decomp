/* Preferred-image VA 0x007771c0, image base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver plus THREE physical stack DWORDs at entry ESP S+4/+8/+0xc;
   RET 12 (callee cleanup) at 0x007774a3. The volatile stack formals represent
   caller-owned homes, not copies of those homes into the scratch extent.
   Direct callers discard EAX; void is that qualified observable contract,
   not a claim of historical void type or exhaustive indirect-call evidence.

   L is one contiguous, otherwise uninitialized 0x224-byte scratch extent:
     +2 duplicate byte; +3 counter byte; +4 eight-byte copy record;
     +0xc helper receiver/copy destination/loop-entry home;
     +0x14 saved bound; +0x18 saved ECX receiver; +0x1c copy destination;
     +0x24 diagnostic buffer A; +0x124 diagnostic buffer B (256 bytes each).
   Only original stores initialize these homes. In particular the receiver
   store at 007771dd precedes the first helper; no blanket initialization.
   The linked SEH record is separately one volatile prev/handler/state struct
   per ADR 0003. Native dispatch/unwind remains unproven.

   Preserve original caching as well as reloads: 00777351/53 load the scan
   base and WORD(L+6) ONCE; 00777376 jumps to 00777358, not 00777351.
   WORD(L+4) is reloaded only after each matching element+2 comparison.
   Entry-home restores at 0077737f/38e and chain-home reloads at 00777404/08/0f
   are intentional alias-sensitive accesses, as is the row-pointer reload
   at 007773eb between the duplicate store and the used-byte store.

   Bugs retained: first sort before null-record test; signed guards/counts;
   nine-byte rows; byte wrap; growth only on used==capacity, by three;
   failed allocation overwrites old pointer before null test; diagnostic-only
   zero store at 00b67a34. Field/global meanings remain unknown. All callees
   remain explicit original boundaries, not composed replacements. */
#include "cmre.h"
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

extern void CM_THISCALL cm_fn_0054a110(u32 self);
/* Only EAX of the incidental EDX:EAX adapter is pushed at 0x0077724f, and the
   helper observes only the low WORD of each stack word; no 64-bit semantic. */
extern u32 CM_THISCALL cm_fn_00549ef0(u32 self, u16 first, u16 second);
/* Receiver + destination first + source second, RET 8; return discarded. */
extern void CM_THISCALL cm_fn_00417c10(u32 self, u32 destination, u32 source);
extern u32 CM_CDECL cm_fn_005409b0(u32 input);
extern u32 CM_THISCALL cm_fn_0054c8e0(u32 self, u32 first, u32 second);
extern void CM_CDECL cm_fn_00945333(u32 base, u32 count, u32 stride,
                                    u32 comparator);
extern u32 CM_CDECL cm_fn_00945667(u32 pointer, u32 size);
extern void CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory,
                                    u32 name, u32 extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern void CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

void CM_THISCALL cm_fn_007771c0(u32 receiver, volatile u32 arg1,
                               volatile u32 arg2, volatile u32 arg3)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 scratch[0x224 / 4];
    u32 L = (u32)scratch;
    u32 channel, bound, source, value, pointer, table, base, slot, entry;
    u32 element, current, index, used, grown, version, table_input, record;
    u32 limit, selector, pointed_value, excluded;
    u16 input28, input2a, scan_key2, scan_key0, element_key;
    u8 counter, duplicate_value, count, capacity, used_byte, flags;
    s8 position;

    frame.state = 0xffffffffu;                 /* 0x007771c0 PUSH -1 */
    frame.handler = 0x0095df6bu;               /* 0x007771c2 PUSH 0095DF6B */
    frame.prev = CM_FS(u32, 0);                /* 0x007771c7/0x007771cd */
    CM_FS(u32, 0) = (u32)&frame;               /* 0x007771ce */
    D(L + 0x018u) = receiver;                 /* 0x007771dd, before helper */

    current = 0;                              /* 0x007771e7 XOR ESI,ESI */
    B(L + 0x002u) = 0;                        /* 0x007771e9 */
    cm_fn_0054a110(L + 0x004u);                /* 0x007771ee */

    channel = D(0x00ae2350u);                  /* 0x007771f3 */
    table_input = arg1;                       /* 0x007771f8: cached EBP */
    bound = D(0x00ae235cu);                    /* 0x007771ff */
    bound = bound - (channel + channel);
    frame.state = 0;                          /* 0x0077721a */
    D(L + 0x014u) = bound;                    /* 0x00777221 */
    cm_fn_00945333(table_input + 0x0d7u, 0x32u, 4u, 0x00778ef0u);

    record = arg3;                            /* 0x0077722a, after sort */
    if (record == 0)
        goto restore;

    input28 = W(record + 0x028u);             /* 0x0077723c */
    input2a = W(record + 0x02au);              /* 0x00777240 */
    source = cm_fn_00549ef0(L + 0x00cu, input2a, input28);
    cm_fn_00417c10(L + 0x004u, L + 0x01cu, source);

    /* arg2 is loaded only when the preceding helper returns zero. */
    if (cm_fn_005409b0(table_input) != 0 || arg2 != 0) {
        source = cm_fn_0054c8e0(L + 0x004u, L + 0x01cu, 1u);
        cm_fn_00417c10(L + 0x004u, L + 0x00cu, source);
    }

    counter = 0;                              /* 0x00777293: live AL */
    B(L + 0x003u) = counter;                  /* 0x00777295: physical home */
    for (;;) {
        entry = D(table_input + 0x0d7u + (u32)(s32)(s8)counter * 4u);
        D(L + 0x00cu) = entry;                /* 0x007772a5, before branch */
        if (entry == 0) goto advance;
        value = D(entry);                     /* 0x007772af */
        limit = D(0x00ae2364u);               /* 0x007772b1 */
        if ((s32)value >= (s32)limit) goto advance;
        selector = arg2;                      /* 0x007772bf */
        flags = B(entry + 0x05eu);            /* 0x007772c8 */
        if (selector == 0) {
            if ((flags & 8u) != 0) goto advance;
        } else {
            if ((flags & 4u) != 0) goto advance;
        }

        if (D(entry + 0x039u) == current) {
            if (current != 0) {
                B(L + 0x002u) = (u8)(B(L + 0x002u) + 1u); /* 007772ee */
                goto advance;
            }
        } else if (current != 0) {
            value = D(current);              /* 0x007772ff */
            if ((s32)value >= (s32)bound || (s32)value < 0) goto chain;
            pointer = D(current + 0x053u);
            if (pointer == 0) goto chain;
            if (B(pointer + 0x11cu) == 0) goto chain;
            pointed_value = D(pointer);      /* 0x0077732a */
            excluded = D(0x009cf260u);        /* 0x0077732c */
            if (pointed_value == excluded) goto chain;

            index = value + value * 2u;
            table = D(L + 0x018u);            /* 0x0077733d */
            base = D(table + 0x020u);         /* 0x00777341 */
            slot = base + index * 2u;
            count = B(slot + 5u);             /* 0x00777344, cached BL */
            position = 0;
            if ((s8)count > 0) {
                base = D(slot);              /* 0x00777351: ONCE */
                scan_key2 = W(L + 0x006u);    /* 0x00777353: ONCE */
                do {                         /* back edge targets 00777358 */
                    element = base + (u32)(s32)position * 9u;
                    element_key = W(element + 2u); /* 0x00777360 */
                    if (scan_key2 == element_key) {
                        scan_key0 = W(L + 0x004u); /* 0x00777366, conditional */
                        element_key = W(element); /* 0x0077736b */
                        if (scan_key0 == element_key) {
                            duplicate_value = B(L + 0x002u); /* 0077737b */
                            entry = D(L + 0x00cu); /* 0077737f, before ADD */
                            B(element + 8u) = (u8)(B(element + 8u) + duplicate_value);
                            goto chain;
                        }
                    }
                    position = (s8)(u8)((u8)position + 1u);
                } while (position < (s8)count);
                entry = D(L + 0x00cu);        /* 0x0077738e, scan exhausted */
            }
            capacity = B(slot + 4u);          /* 0x00777392 */
            if (count == capacity) {
                capacity = (u8)(capacity + 3u);
                B(slot + 4u) = capacity;
                base = D(slot);              /* 0x007773a4 */
                grown = cm_fn_00945667(base, (u32)(s32)(s8)capacity * 9u);
                D(slot) = grown;             /* 0x007773b0: old pointer lost */
                if (grown == 0) goto diagnostic;
            }
            used = (u32)(s32)(s8)B(slot + 5u); /* 0x007773b6 */
            base = D(slot);                  /* 0x007773ba */
            cm_fn_00417c10(base + used * 9u, L + 0x01cu, L + 0x004u);
            used = (u32)(s32)(s8)B(slot + 5u); /* 0x007773d0 */
            base = D(slot);                  /* 0x007773d4 */
            duplicate_value = B(L + 0x002u);  /* 0x007773d6 */
            B(base + used * 9u + 8u) = duplicate_value; /* 0x007773e4 */
            used_byte = B(slot + 5u);         /* 0x007773e8 */
            base = D(slot);                  /* 0x007773eb, alias-sensitive */
            used_byte = (u8)(used_byte + 1u);
            B(slot + 5u) = used_byte;         /* 0x007773f1 */
            used = (u32)(s32)(s8)used_byte;
            cm_fn_00945333(base, used, 9u, 0x00778e90u);
        }
chain:
        current = D(entry + 0x039u);          /* 0x00777401 */
        counter = B(L + 0x003u);              /* 0x00777404 */
        table_input = arg1;                   /* 0x00777408: physical S+4 */
        bound = D(L + 0x014u);                /* 0x0077740f */
        B(L + 0x002u) = 1;                    /* 0x00777413 */
advance:
        counter = (u8)(counter + 1u);         /* 0x00777418 */
        B(L + 0x003u) = counter;              /* 0x0077741c, even on exit */
        if ((s8)counter >= 0x32) break;
    }
    goto restore;

diagnostic:
    /* Original 00777451/56/57 leaves three words live across argument-free
       0090d130 (MOV EAX,00ad4394; RET). Formatter receives all SIX words;
       log receives THREE. No changed F6 semantics or invented defaults. */
    cm_fn_00944cff(0x00a5b2a4u, 0u, 0u, L + 0x124u, L + 0x024u);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, L + 0x124u, L + 0x024u,
                   0x21c8u);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0u);
    D(0x00b67a34u) = 0;                        /* 0x00777481 */

restore:
    CM_FS(u32, 0) = frame.prev;                /* 0x0077748b/0x00777496 */
}
