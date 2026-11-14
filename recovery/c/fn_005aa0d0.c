/* Preferred-image VA 005aa0d0, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/RET0. Original callees and imports remain explicit boundaries. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern u32 CM_THISCALL cm_fn_0068d040(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *pointer);
extern u32 CM_THISCALL cm_fn_005ac060(u32 self, u32 argument);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

/* At 005aa2ae/2b4, EAX and the stack word both contain the buffer pointer.
   Original 009452ca leaves EAX unchanged on its null path; the caller returns it.
   The typed physical transport puts its first parameter in EAX and the second
   on the stack. Earlier free results are dead: a nonnull argument overwrites
   EAX before use, and a null argument only returns its incoming EAX. */
extern unsigned long long CM_CDECL __attribute__((regparm(1)))
    cm_fn_009452ca(u32 incoming_eax, u32 pointer);

u32 CM_THISCALL cm_fn_005aa0d0(u32 self)
{
    u8 work[0x200];
    u32 object, array, row, index, counter, rows_left, version, buffer, result;
    object = D(self + 0x1128u);
    if (object != 0) {
        cm_fn_0068d040(object);
        cm_fn_00944c94((void *)object);
    }
    if (cm_fn_005ac060(self, 0) == 0) {
        cm_fn_00944cff(0x009cc208u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0xe3u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
    }
    row = self;
    rows_left = 366;
    do {
        counter = 0;
        if ((s8)B(0x00b64738u) > 0) {
            do {
                index = (u32)(s32)(s16)counter * 4u;
                array = D(row);
                if (D(array + index) != 0) {
                    object = D(D(row) + index);
                    if (object != 0) {
                        cm_fn_0068d040(object);
                        cm_fn_00944c94((void *)object);
                    }
                    D(D(row) + index) = 0;
                }
                array = D(row + 4u);
                if (D(array + index) != 0) {
                    object = D(D(row + 4u) + index);
                    if (object != 0) {
                        cm_fn_0068d040(object);
                        cm_fn_00944c94((void *)object);
                    }
                    D(D(row + 4u) + index) = 0;
                }
                object = D(D(row + 8u) + index);
                if (object != 0) {
                    cm_fn_0068d040(object);
                    cm_fn_00944c94((void *)object);
                    D(D(row + 8u) + index) = 0;
                }
                ++counter;
            } while ((s16)counter < (s16)(s8)B(0x00b64738u));
        }
        array = D(row);
        cm_fn_009452ca(array, array);
        array = D(row + 4u);
        cm_fn_009452ca(array, array);
        array = D(row + 8u);
        cm_fn_009452ca(array, array);
        row += 12u;
        --rows_left;
    } while (rows_left != 0);
    if (B(self + 0x1183u) != 0) {
        object = D(self + 0x112cu);
        if (object != 0) {
            cm_fn_005aa0d0(object);
            cm_fn_00944c94((void *)object);
            D(self + 0x112cu) = 0;
        }
        buffer = D(0x00ae2a5cu);
        result = (u32)cm_fn_009452ca(buffer, buffer);
        D(0x00ae2a5cu) = 0;
        return result;
    }
    return 0;
}
