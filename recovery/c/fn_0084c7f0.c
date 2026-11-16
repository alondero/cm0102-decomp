#include "cmre.h"

/* At preferred-image VAs 0084c7ff-0084c80a seven arguments are pushed;
   0084c80f-0084c828 performs no caller argument cleanup.  Pinned 3.9.68
   SHA-256: 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern int CM_STDCALL cm_fn_00944c10(u32, char *, u32, u8 *, u32, u32 *, u32 *);

u8 cm_fn_0084c7f0(void)
{
    struct {
        u32 returned;
        u32 needed;
        u8 buffer[20];
    } scratch;

    if (cm_fn_00944c10(2, (char *)0, 5, scratch.buffer, 20,
                       &scratch.needed, &scratch.returned))
        return scratch.returned != 0;
    return 0;
}
