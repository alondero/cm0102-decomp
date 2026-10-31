/* Preferred-image VA 0x0053A8B0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cdecl, four stack arguments (source, unread second argument,
   destination, optional byte pointer), plain ret. Each listed source byte is
   sign-extended to a word and stored at a destination offset, in this order:
   +0x0e->+0x00, +0x0f->+0x03, +0x12->+0x06, +0x13->+0x09, +0x14->+0x0c,
   +0x15->+0x0f, +0x16->+0x12, +0x1a->+0x15, +0x1c->+0x18, +0x1d->+0x1b,
   +0x1f->+0x1e, +0x20->+0x21, +0x22->+0x24; then, when the fourth argument is
   nonzero, the byte it points to = 0x0d; then +0x11->+0x27, +0x10->+0x2a,
   +0x17->+0x2d, +0x18->+0x30, +0x19->+0x33, +0x1b->+0x36, +0x1e->+0x39,
   +0x21->+0x3c. Direct callers do not observe EAX or EDX.
   Destination accesses are volatile so -O2 keeps the listing's order.
   Field meanings remain unknown. */
#include "cmre.h"

#define CM_FN_0053A8B0_COPY(src_off, dst_off) (*(volatile s16 *)(out + (dst_off)) = (s16)(s8)source[(src_off)])

void CM_CDECL cm_fn_0053a8b0(u8 *source, u32 unused, u8 *destination, u8 *marker)
{
    volatile u8 *out = (volatile u8 *)destination;

    (void)unused;
    CM_FN_0053A8B0_COPY(0x0e, 0x00);
    CM_FN_0053A8B0_COPY(0x0f, 0x03);
    CM_FN_0053A8B0_COPY(0x12, 0x06);
    CM_FN_0053A8B0_COPY(0x13, 0x09);
    CM_FN_0053A8B0_COPY(0x14, 0x0c);
    CM_FN_0053A8B0_COPY(0x15, 0x0f);
    CM_FN_0053A8B0_COPY(0x16, 0x12);
    CM_FN_0053A8B0_COPY(0x1a, 0x15);
    CM_FN_0053A8B0_COPY(0x1c, 0x18);
    CM_FN_0053A8B0_COPY(0x1d, 0x1b);
    CM_FN_0053A8B0_COPY(0x1f, 0x1e);
    CM_FN_0053A8B0_COPY(0x20, 0x21);
    CM_FN_0053A8B0_COPY(0x22, 0x24);
    if (marker != 0) {
        *(volatile u8 *)marker = 0x0d;
    }
    CM_FN_0053A8B0_COPY(0x11, 0x27);
    CM_FN_0053A8B0_COPY(0x10, 0x2a);
    CM_FN_0053A8B0_COPY(0x17, 0x2d);
    CM_FN_0053A8B0_COPY(0x18, 0x30);
    CM_FN_0053A8B0_COPY(0x19, 0x33);
    CM_FN_0053A8B0_COPY(0x1b, 0x36);
    CM_FN_0053A8B0_COPY(0x1e, 0x39);
    CM_FN_0053A8B0_COPY(0x21, 0x3c);
}
