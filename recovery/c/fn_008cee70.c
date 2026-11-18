#include "cmre.h"

/* Original instruction evidence: preferred-image VAs below, CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   008cee79/80 and RET 8 establish the entry receiver and two stack args.
   008cefce/008cf031 pass ECX plus two stack args to 0054c5b0;
   008cefd3/008cf03d consume its EAX as the following source pointer.
   008ceee8 and the later 00417c10 calls pass receiver plus two args.
   008cef65..008cef7c: the three pushes preceding 0090d130 belong to
   the subsequent variadic formatter, not to the no-argument call.
   AX alone is set on zero returns and forwarded at 008cf0ca. */
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern void CM_THISCALL cm_fn_00417c10(u32 self, void *out, const void *source);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32 self, void *out, u32 value);
/* Bound call-site ABI, same pinned image/hash as above: 008cee80 saves
   entry ECX in EBX; 008cef00 loads date; 008cef07/09 push zero/date;
   008cef0a restores ECX from EBX immediately before 008cef0c CALL.
   Preserve that receiver explicitly; this is not callee composition proof. */
extern void CM_THISCALL cm_fn_008d0bb0(u32 self, void *a, void *b);
extern u16 CM_THISCALL cm_fn_008cf0e0(u32 self, u32 event, u32 date);
extern u32 CM_CDECL cm_fn_00669bd0(void *object, void *a, void *b);
extern u32 CM_CDECL cm_fn_00944cff(u32 a, u32 b, u32 c, void *d, void *e);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(const char *a, const char *b, int c);

#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))

u16 CM_THISCALL cm_fn_008cee70(u32 self, u32 event, u32 date)
{
    u8 scratch[0x210];
    u32 index = D(event + 0xc);
    u32 manager = D(0xae1050);
    u32 target = D(0xae23bc) + index * 110u;
    u32 found = cm_fn_004cf5a0(manager, target);
    u32 object = 0;
    u8 mode;
    D(0xae1080) = found;
    if (found && (B(found + 0x35) & 0x3f) != 5) {
        u32 n = D(found + 4);
        object = D(0xae23b4) + n * 581u;
    }
    mode = B(event + 0x30) & 15;
    if (mode == 0) {
        cm_fn_00417c10(date, scratch, (void *)0xae2c90);
        return 0;
    }
    if (mode == 5) {
        cm_fn_008d0bb0(self, (void *)date, (void *)0);
        return 0;
    }
    if (mode == 1 || mode == 4)
        return cm_fn_008cf0e0(self, event, date);
    if (mode == 2) {
        if (!object) {
            char *text;
            cm_fn_00944cff(0xa8d514, 0, 0, scratch + 0x110, scratch + 0x10);
            text = cm_fn_0090d130();
            cm_fn_00944c9f(0xae24d0, 0x9870e8, text,
                          scratch + 0x110, scratch + 0x10, 0x31da);
            cm_fn_005e8290((const char *)0x9870e0, (const char *)0xae24d0, 0);
            D(0xb67a34) = 0;
            return 0;
        }
        if (cm_fn_00669bd0((void *)object, (void *)0, (void *)date)) {
            u32 source = cm_fn_0054c5b0(date, scratch, 7);
            cm_fn_00417c10(date, scratch + 8, (void *)source);
        } else {
            cm_fn_00417c10(date, scratch + 8, (void *)0xae2c90);
        }
        return 0;
    }
    if (mode == 3) {
        found = cm_fn_004cf5a0(D(0xae1050), target);
        if (found) {
            u32 source = cm_fn_0054c5b0(found + 0x2d, scratch + 8, 1);
            u32 low, high;
            u16 year;
            cm_fn_00417c10(date, scratch, (void *)source);
            low = D(0xae2c90);
            high = D(0xae2c94);
            year = W(date + 2);
            *(u32 *)(scratch) = low;
            *(u32 *)(scratch + 4) = high;
            if ((u16)(low >> 16) > year ||
                ((u16)(low >> 16) == year && (s16)low > (s16)W(date)))
                cm_fn_00417c10(date, scratch + 8, (void *)0xae2c90);
        } else {
            cm_fn_00417c10(date, scratch + 8, (void *)0xae2c90);
        }
    }
    return 0;
}
