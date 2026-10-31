/* WIP: memory mismatch on stack page, case 449 */
#include "cmre.h"

extern char *CM_CDECL cm_fn_0066f4e0(char *destination,
    const char *format, ...);
extern u32 CM_CDECL cm_fn_009466d0(u32 destination, u32 source, u32 count);

void CM_THISCALL cm_fn_008b50f0(volatile u8 *self)
{
    u8 code = *(volatile u8 *)(self + 0x15);
    char buffer[0x64];

    if (code == 0xff) {
        (void)cm_fn_0066f4e0(buffer, (const char *)0x00a8ce94u,
            (void *)0x00ad9c64u);
    } else if (code == 0) {
        (void)cm_fn_0066f4e0(buffer, (const char *)0x00a8ce64u,
            (void *)0x00ad9c64u);
    } else if (code == 1) {
        (void)cm_fn_0066f4e0(buffer, (const char *)0x00a8ce1cu,
            (void *)0x00ad9c64u);
    } else if (code == 2) {
        (void)cm_fn_0066f4e0(buffer, (const char *)0x00a8cdf0u,
            (void *)0x00ad9c64u);
    } else if (code == 3) {
        (void)cm_fn_0066f4e0(buffer, (const char *)0x00a8cdc4u,
            (void *)0x00ad9c64u);
    } else if (code == 4) {
        (void)cm_fn_0066f4e0(buffer, (const char *)0x00a8cd9cu,
            (void *)0x00ad9c64u);
    } else if (code == 5) {
        (void)cm_fn_0066f4e0(buffer, (const char *)0x00a8cd70u,
            (void *)0x00ad9c64u);
    } else if (code == 6) {
        (void)cm_fn_0066f4e0(buffer, (const char *)0x00a8cd48u,
            (void *)0x00ad9c64u);
    } else if (code == 7) {
        (void)cm_fn_0066f4e0(buffer, (const char *)0x00a8cd04u,
            (void *)0x00ad9c64u);
    } else if (code == 8) {
        return;
    }

    *(volatile u32 *)(self + 9) = 0;
    *(volatile u32 *)(self + 13) = 0;
    *(volatile u32 *)(self + 17) = 0;
    (void)cm_fn_009466d0((u32)(unsigned long)(self + 9),
        (u32)(unsigned long)buffer, 0x0b);
}
