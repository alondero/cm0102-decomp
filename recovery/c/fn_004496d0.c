#include "cmre.h"

/* Evidence: CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   All addresses below are preferred-image VAs.
   004496d7/0044972c/004497a2/004497c1/004497eb identify four
   stack arguments; 004496e5 saves the incoming ECX receiver.
   004497ad..004497ba pass receiver, index, zero.
   004497e3..004497e6 pass receiver and index.
   004497f2..004497f8 pass receiver, index, record, mode, object.
   00449765 calls the no-stack-argument string helper between pushes
   belonging to the subsequent variadic formatter. */
extern u32 CM_THISCALL cm_fn_004afa40(volatile u32 *self);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg);
extern u32 CM_THISCALL cm_fn_00452710(u32 self, u32 *index, u32 create);
extern u32 CM_THISCALL cm_fn_004528d0(u32 self, u32 *index, u32 create);
extern u32 CM_THISCALL cm_fn_00452a90(u32 self, u32 *index);
extern u32 CM_THISCALL cm_fn_00449810(u32 self, u32 *index,
                                     u8 *record, u32 mode, u32 object);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);

u32 CM_THISCALL cm_fn_004496d0(u32 self, u32 *index, u32 which,
                              u32 mode, u32 object)
{
    u8 scratch[512];
    u8 *first;
    u8 *second;
    u32 line;
    u32 record;
    char *text;
    if (object == 0) {
        cm_fn_00944cff(0x0098b6a0, 0, 0, scratch + 256, scratch);
        first = scratch + 256;
        second = scratch;
        line = 0x95f;
    } else {
        cm_fn_004afa40((volatile u32 *)object);
        cm_fn_004ae660((void *)object, 0x0098ba0c);
        if (index != 0) {
            if ((u8)which == 1) {
                record = cm_fn_00452710(self, index, 0);
            } else {
                record = cm_fn_004528d0(self, index, 0);
                if ((u8)mode == 0x14 && record != 0) {
                    /* 004497d0..004497e1 test C3: equal OR unordered.
                       Long double retains the FLD m32 widening. This
                       predicate deliberately includes NaNs, unlike ==.
                       Native FP-status equivalence requires gate evidence. */
                    long double value =
                        *(volatile float *)(record + 0x1f1u);
                    long double limit =
                        CM_GLOBAL(volatile double, 0x00968948);
                    if (!(value < limit || value > limit))
                        cm_fn_00452a90(self, index);
                }
            }
            return cm_fn_00449810(self, index, (u8 *)record, mode, object);
        }
        cm_fn_00944cff(0x0098b6a0, 0, 0, scratch, scratch + 256);
        first = scratch;
        second = scratch + 256;
        line = 0x96b;
    }
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, first, second, line);
    cm_fn_005e8290((const char *)0x009870e0,
                   (const char *)0x00ae24d0, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    return 0;
}
