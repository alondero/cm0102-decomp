/* fn_008b50f0: switch on byte field [esi+0x15]; render one of nine format
   strings into a 0x64-byte stack scratch (base L, the post-`sub esp,0x64`
   esp), then clear the 12 bytes at [esi+9] and copy 0xb bytes of the scratch
   there.  code 8 returns before the clear/copy; codes > 8 reach the clear/copy
    with the scratch untouched.

    UNRESOLVED FIDELITY DEFECT (not repaired by a passing gate):
    CM0102 3.9.68 SHA-256
    0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86,
    preferred-image VAs: 0x008b50f0 reserves 0x64 bytes; 0x008b50f3
    saves ESI below them. For dispatch 9..254, 0x008b51f0..0x008b51f2
    reaches the copy without initializing that area. After PUSH 0xb at
    0x008b51fb, LEA ECX,[ESP+8] at 0x008b5205 therefore addresses
    entry ESP-0x64. The copy at 0x008b520b observes eleven existing bytes
    there, after the three destination DWORD clears.

    The automatic array below does NOT preserve those entry-stack bytes.
    A faithful repair needs an entry-stack preservation/binding contract
    outside this plain-C routine. Merely addressing entry ESP-0x64 through
    a compiler frame builtin cannot protect it against compiled prologue,
    spills or outgoing calls. Matching a particular compiler frame or
    initializing this array with guessed bytes would not repair fidelity.
    Keep this candidate unvalidated for that path pending such a contract. */
#include "cmre.h"

extern char *CM_CDECL cm_fn_0066f4e0(char *destination,
    const char *format, ...);
extern u32 CM_CDECL cm_fn_009466d0(u32 destination, u32 source, u32 count);

void CM_THISCALL cm_fn_008b50f0(volatile u8 *self)
{
    u8 scratch[0x64];
    u8 code = *(volatile u8 *)(self + 0x15);

    if (code == 0xff) {
        (void)cm_fn_0066f4e0((char *)scratch,
            (const char *)0x00a8ce94u, (void *)0x00ad9c64u);
    } else if (code == 0) {
        (void)cm_fn_0066f4e0((char *)scratch,
            (const char *)0x00a8ce64u, (void *)0x00ad9c64u);
    } else if (code == 1) {
        (void)cm_fn_0066f4e0((char *)scratch,
            (const char *)0x00a8ce1cu, (void *)0x00ad9c64u);
    } else if (code == 2) {
        (void)cm_fn_0066f4e0((char *)scratch,
            (const char *)0x00a8cdf0u, (void *)0x00ad9c64u);
    } else if (code == 3) {
        (void)cm_fn_0066f4e0((char *)scratch,
            (const char *)0x00a8cdc4u, (void *)0x00ad9c64u);
    } else if (code == 4) {
        (void)cm_fn_0066f4e0((char *)scratch,
            (const char *)0x00a8cd9cu, (void *)0x00ad9c64u);
    } else if (code == 5) {
        (void)cm_fn_0066f4e0((char *)scratch,
            (const char *)0x00a8cd70u, (void *)0x00ad9c64u);
    } else if (code == 6) {
        (void)cm_fn_0066f4e0((char *)scratch,
            (const char *)0x00a8cd48u, (void *)0x00ad9c64u);
    } else if (code == 7) {
        (void)cm_fn_0066f4e0((char *)scratch,
            (const char *)0x00a8cd04u, (void *)0x00ad9c64u);
    } else if (code == 8) {
        return;
    }

    /* 0x8b51f4: lea eax,[esi+9]; xor ecx,ecx; mov edx,eax;
       three dword clears; then copy 0xb bytes from L. */
    ((volatile u32 *)(self + 9))[0] = 0;
    ((volatile u32 *)(self + 9))[1] = 0;
    ((volatile u32 *)(self + 9))[2] = 0;
    (void)cm_fn_009466d0((u32)(unsigned long)(self + 9),
        (u32)(unsigned long)scratch, 0x0bu);
}
