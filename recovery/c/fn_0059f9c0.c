/* fn_0059f9c0 - CM0102 3.9.68, preferred-image VA 0x0059f9c0.
   Calling convention: `ret 0xc` pops three 4-byte arguments, so the entry
   frame is recovered from the listing as
     entry ESP + 0x04 -> argument 1: receiver of 0x00785830
     entry ESP + 0x08 -> argument 2: branch selector (0 selects the "no
                         terracing" path)
     entry ESP + 0x0c -> argument 3: receiver of 0x004ae660
   The 0x1388-byte text buffer is the frame the original asked for with
   `mov eax,0x1388; call 0x00945610`: the legacy helper at 0x00945610 only
   reserves that frame (it takes the size in EAX - plain C cannot hand a
   value in EAX to an external callee, and its stack probe then runs off the
   end of the mapped stack and faults before the original's own fault at
   0x004ae6b2), so the frame is provided here by the local buffer and that
   helper call is not reproduced. */
#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern char *CM_CDECL cm_fn_0066f4e0(char *out, const char *format, ...);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
/* Legacy stack-frame helper of the original prologue; not called, see above. */
extern void CM_CDECL cm_fn_00945610(void);

u32 CM_STDCALL cm_fn_0059f9c0(u32 arg1, u32 arg2, u32 arg3)
{
    char buffer[0x1388];
    u32 first;
    u32 second;
    s32 selected;
    u32 base;
    u32 entry;
    u32 value;

    if (arg2 == 0u) {
        /* 0x0059f9d7: ecx = arg1; push 3; call 0x00785830 */
        first = (u32)cm_fn_00785830((void *)arg1, 3u);
        if (first == 0u) {
            (void)cm_fn_0066f4e0(buffer, (const char *)0x009c92d0u);
        } else {
            (void)cm_fn_0066f4e0(buffer, (const char *)0x009c9300u);
        }
    } else {
        /* 0x0059fa03: edi = arg1; three queries with index 1, 2 and 3 */
        first = (u32)cm_fn_00785830((void *)arg1, 1u);
        second = (u32)cm_fn_00785830((void *)arg1, 2u);
        selected = (s8)(u8)(u32)cm_fn_00785830((void *)arg1, 3u);
        if (first != 0xffffffffu) {
            /* 0x0059fa32: ecx = [0x00ae23b0]; edx = esi*39; ecx += edx*2 */
            base = CM_GLOBAL(volatile u32, 0x00ae23b0u);
            entry = base + (u32)first * 78u;
            if (selected == 0) {
                if (entry == 0u) {
                    (void)cm_fn_0066f4e0(buffer, (const char *)0x009c9158u);
                } else {
                    /* 0x0059fa5a: ecx = [ecx+0x3c]; ecx -= ebx */
                    value = *(volatile u32 *)(entry + 0x3cu) - second;
                    (void)cm_fn_0066f4e0(buffer, (const char *)0x009c91ccu, value);
                }
            } else {
                if (*(volatile u32 *)(entry + 0x40u) == 0u) {
                    (void)cm_fn_0066f4e0(buffer, (const char *)0x009c9158u);
                } else {
                    (void)cm_fn_0066f4e0(buffer, (const char *)0x009c9250u, second);
                }
            }
        } else {
            if (selected == 0) {
                (void)cm_fn_0066f4e0(buffer, (const char *)0x009c8ff0u);
            } else {
                base = CM_GLOBAL(volatile u32, 0x00ae23b0u);
                if (*(volatile u32 *)(base - 0x0eu) != 0u) {
                    (void)cm_fn_0066f4e0(buffer, (const char *)0x009c90d0u, (u32)selected);
                } else {
                    (void)cm_fn_0066f4e0(buffer, (const char *)0x009c9064u);
                }
            }
        }
    }
    /* 0x0059fab7: ecx = arg3; edx = buffer; call 0x004ae660 */
    return (u32)cm_fn_004ae660((void *)arg3, (u32)(unsigned long)buffer);
}
