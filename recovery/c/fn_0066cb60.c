/* fn_0066cb60 -- preferred-image VA 0x0066cb60, image base 0x00400000, pinned
   CM0102 3.9.68 sha256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   Convention: the object register is copied from ECX before this body writes
   that register, and the return pops one stack dword that no instruction here
   reads. The entry is therefore thiscall with one unused stack argument.

   Frame: the entry reserves a 0x410-byte local block L, saves four registers
   below it, and links a volatile three-word exception record above it (previous
   frame, handler, state). The saved predecessor is restored just before the
   return. Each local displacement below is resolved at its own instruction site
   against L, accounting for saves and for pushed arguments still outstanding;
   the four message buffers stay inside that single allocation at their original
   0x200-byte spacing and are left uninitialized.

   ABI evidence, from original instructions at preferred-image VAs in the pinned
   binary named above:
   - The allocation and free helpers each take one caller-removed stack word,
     and the report helper takes five caller-removed stack words: cdecl.
   - The indirect call through the object's offset-0x3c slot takes four stack
     words with no caller cleanup, with ECX = object: thiscall.
   - The large twenty-two-word helper has no caller cleanup and gets ECX from
     the allocation result left in EAX: thiscall.
   - The final no-argument helper gets ECX from the pointer read out of the
     object's offset-0xc field: thiscall.
   - The trailing cleanup removes exactly nine dwords: three pushed for the
     report call, three pushed for the formatter, and three pushed for the error
     hook. The text value returned by the helper before the formatter is simply
     forwarded as one of the formatter's variadic arguments, in original order.

   Dispatch: the object's vtable pointer is read, and the real function pointer
   at offset 0x3c of that table is loaded and called with the same ECX and the
   same four stack arguments. In the pinned image that slot routes to the real
   virtual function at VA 0x0066c240. Keeping the original table reads and the
   indirect call means no substitute callback, direct binding, invented return
   value or fake slot target is introduced. */
#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern u32 CM_CDECL cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_00684230(u32 self);
extern u32 * CM_THISCALL cm_fn_0068b100(u32 *param_1,int param_2,u16 param_3,int param_4,u16 param_5, u32 param_6,u32 *param_7,u16 param_8,u8 param_9, u8 param_10,u8 param_11,int param_12,int param_13,u16 param_14, u8 param_15,u16 param_16,u8 param_17,u8 param_18, u16 param_19,u16 param_20,u16 param_21,u32 param_22, u8 param_23);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

/* Indirect slot at object offset 0x3c; the receiver in ECX is the object. */
typedef u32 (CM_THISCALL *cm_slot3c_t)(u32 self, u32 arg1, u32 arg2, u32 arg3, u32 arg4);

u32 CM_THISCALL cm_fn_0066cb60(u32 self, u32 unused_arg __attribute__((unused)))
{
    volatile struct { u32 prev; u32 handler; u32 state; } frame;
    u8 L[0x410];
    u32 vtable;
    u32 sched;
    u32 clubs;
    u32 table;
    u32 child;
    u32 holder;
    u32 i;
    u32 ptr;
    u32 v;
    u32 name;
    u32 ext;
    u32 line;
    u32 result;

    frame.state = 0xffffffffu;              /* exception state starts at -1 */
    frame.handler = 0x0095c76e;             /* handler installed on entry */
    frame.prev = CM_FS(u32, 0);             /* save previous frame head */
    CM_FS(u32, 0) = (u32)&frame;            /* link this frame */

    vtable = D(self);                       /* first object word is the vtable */
    B((u32)L) = 2;                          /* scratch mode byte */
    B((u32)L + 1) = 0;
    B((u32)L + 2) = 0;
    B((u32)L + 3) = 0;
    /* four stack words are (0, L+8, L+4, 0); ECX is the object. */
    sched = ((cm_slot3c_t)D(vtable + 0x3c))(self, 0, (u32)(L + 8), (u32)(L + 4), 0);

    clubs = (u32)cm_fn_00944e46(0x14);      /* allocate the club table */
    if (clubs == 0) {                       /* allocation failed */
        name = (u32)(L + 0x10);
        ext = (u32)(L + 0x210);
        line = 0x245;
        goto report;
    }
    i = 0;
    ptr = clubs;
    do {
        table = D(self + 0xee);
        i = i + 8;
        ptr = ptr + 4;
        v = D(table + i - 4);
        D(ptr - 4) = v;
        table = D(self + 0xee);             /* original reload */
        D(table + i - 4) = 0;
    } while ((s32)i < 0x28);

    v = (u32)cm_fn_00944cf1(0xee);
    D((u32)(L + 0xc)) = v;
    frame.state = 0;
    child = 0;
    if (v != 0) {
        /* Twenty-two stack words, ECX = allocation result in EAX. The source
           object fields yield only the low 16/8 bits for the narrow arguments;
           the upper halves of those registers were never defined. */
        child = (u32)cm_fn_0068b100((u32 *)v, self, 5, clubs, 2, D(self + 4),
                    (u32 *)sched, (u16)D((u32)(L + 8)), B(self + 0xc2), B(self + 0xc3),
                    B(self + 0xc4), (int)(self + 0xc5), (int)(u32)L,
                    W(self + 0x40), 0, (u16)D((u32)(L + 4)), B(self + 0x51), 1,
                    0x82, W(self + 0xd9), 0xffff, 0, 2);
    }
    holder = D(self + 0xc);
    frame.state = 0xffffffffu;
    D(holder) = child;
    (void)cm_fn_009452ca(clubs);
    (void)cm_fn_009452ca(sched);
    holder = D(self + 0xc);                 /* original reload */
    child = D(holder);
    if (child == 0) {
        name = (u32)(L + 0x310);
        ext = (u32)(L + 0x110);
        line = 0x25b;
        goto report;
    }
    (void)cm_fn_00684230(child);            /* ECX = pointer read from holder */
    D(self + 0x30) = 0;
    result = 1;
    goto done;

report:
    /* (file, 0, 0, name, ext) */
    (void)cm_fn_00944cff(0x009eb15c, 0, 0, (void *)name, (void *)ext);
    /* The three words pushed for the report path are still on the stack here;
       the next helper consumes none of them and the formatter reads them back
       as its last three arguments. */
    (void)cm_fn_00944c9f(0x00ae24d0, 0x009870e8, (u32)cm_fn_0090d130(),
                         name, ext, line);
    (void)cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    result = 0;
done:
    CM_FS(u32, 0) = frame.prev;             /* unlink the frame */
    return result;
}
