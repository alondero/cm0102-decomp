/* Preferred-image VA 0x00785830, image base 0x00400000.
   Target SHA-256: 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original object fields and diagnostic callee signatures remain unknown. */
#include "cmre.h"

/* Original callees are explicit hybrid boundaries. The declarations name
   linker targets only; argument/register layouts are retained by the adapter. */
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void *, void *, u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* Entry tail-transfers the original partial EAX, after reserving 512 bytes.
   A signed low byte in [0,50) selects the unaligned dword access. Preserve all
   other incoming registers and EAX's upper bytes: invalid diagnostics can
   observe them. Naked mixed C/asm is specific to the pinned GCC toolchain;
   inspect both emitted builds for spills, saves and unwanted register writes. */
static void __attribute__((used, noinline, naked, regparm(1), optimize("O2")))
news_getter_control(u32 raw)
{
    register u32 saved_cx __asm__("ecx");
    register u32 saved_dx __asm__("edx");
    register u32 saved_bx __asm__("ebx");
    register u32 saved_bp __asm__("ebp");
    register u32 saved_si __asm__("esi");
    register u32 saved_di __asm__("edi");
    __asm__ volatile ("" : "=c" (saved_cx), "=d" (saved_dx), "=b" (saved_bx),
                      "=r" (saved_bp), "=S" (saved_si), "=D" (saved_di));
    s8 index = (s8)(u8)raw;
    if (index >= 0 && index < 50) {
        __asm__ volatile ("jmp _news_getter_access" : : "a" (raw),
                          "c" (saved_cx), "d" (saved_dx), "b" (saved_bx),
                          "r" (saved_bp), "S" (saved_si), "D" (saved_di) : "memory");
        __builtin_unreachable();
    }
    __asm__ volatile ("jmp _news_getter_diagnostic" : : "a" (raw),
                      "c" (saved_cx), "d" (saved_dx), "b" (saved_bx),
                      "r" (saved_bp), "S" (saved_si), "D" (saved_di) : "memory");
    __builtin_unreachable();
}

/* Entry/access/diagnostic ABI adapter. The original instructions reserve two
   uninitialized 256-byte buffers, preserve partial EAX before invalid calls,
   and leave arguments on the stack until their original cleanup points.
   Getter reads are deliberately unaligned. Entry EDX survives the valid path;
   invalid return retains the last callee's EDX. EDX:EAX is a validation adapter,
   not evidence of an original semantic 64-bit return. */
unsigned long long CM_THISCALL __attribute__((naked))
cm_fn_00785830(void *self __attribute__((unused)), u32 index __attribute__((unused)))
{
    __asm__(
        "v_00785830:\n"
        "movb 4(%esp), %al\n"
        "v_00785834:\n"
        "subl $512, %esp\n"
        "jmp _news_getter_control\n"
        "_news_getter_access:\n"
        /* Recreate original CMP flags after the C routing decision. */
        "cmpb $50, %al\n"
        "v_00785842:\n"
        "movsbl %al, %eax\n"
        "v_00785845:\n"
        "movl 5(%ecx,%eax,4), %eax\n"
        "addl $512, %esp\n"
        "ret $4\n"
        "_news_getter_diagnostic:\n"
        /* Original callee sees TEST flags for negatives, CMP flags otherwise. */
        "testb %al, %al\n"
        "js 1f\n"
        "cmpb $50, %al\n"
        "1:\n"
        "v_00785852:\n"
        "leal (%esp), %ecx\n"
        "v_00785856:\n"
        "leal 256(%esp), %edx\n"
        "v_0078585d:\n"
        "pushl %ecx\n"
        "v_0078585e:\n"
        "pushl %edx\n"
        "v_0078585f:\n"
        "pushl $0\n"
        "v_00785861:\n"
        "pushl $0\n"
        "v_00785863:\n"
        "pushl $0x00a693ec\n"
        CM_ORIGINAL_CALL(00785868, 0078586d, 00944cff)
        "addl $20, %esp\n"
        "v_00785870:\n"
        "leal (%esp), %eax\n"
        "v_00785874:\n"
        "leal 256(%esp), %ecx\n"
        "v_0078587b:\n"
        "pushl $0x1e75\n"
        "v_00785880:\n"
        "pushl %eax\n"
        "v_00785881:\n"
        "pushl %ecx\n"
        CM_ORIGINAL_CALL(00785882, 00785887, 0090d130)
        "pushl %eax\n"
        "v_00785888:\n"
        "pushl $0x009870e8\n"
        "v_0078588d:\n"
        "pushl $0x00ae24d0\n"
        CM_ORIGINAL_CALL(00785892, 00785897, 00944c9f)
        "pushl $0\n"
        "v_00785899:\n"
        "pushl $0x00ae24d0\n"
        "v_0078589e:\n"
        "pushl $0x009870e0\n"
        CM_ORIGINAL_CALL(007858a3, 007858a8, 005e8290)
        "addl $36, %esp\n"
        "v_007858ab:\n"
        "movl $0, 0x00b67a34\n"
        "xorl %eax, %eax\n"
        "addl $512, %esp\n"
        "ret $4\n"
    );
}