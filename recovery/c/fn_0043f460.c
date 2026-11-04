#include "cmre.h"

/* Original x87 instructions and original CALL return words are VM-routed. */
unsigned long long CM_THISCALL __attribute__((naked))
cm_fn_0043f460(const int *self __attribute__((unused)), u32 flags __attribute__((unused)))
{
    __asm__(
        ".intel_syntax noprefix\n"
        "v_0043f460: cmre_x87_0043f460: fild dword ptr [ecx]\n"
        "cmre_x87_return_0043f460_0043f462:\n"
        "push ebx\n"
        "push ebp\n"
        "v_0043f464: cmre_x87_0043f464: fmul qword ptr [0xad9cb8]\n"
        "cmre_x87_return_0043f464_0043f46a:\n"
        CM_ORIGINAL_CALL(0043f46a,0043f46f,00945640)
        "mov ebx, eax\n"
        "mov eax, dword ptr [esp+0xc]\n"
        "test al, 0x20\n"
        "mov ebp, edx\n"
        "jnz 1f\n"
        "push esi\n"
        "push edi\n"
        "push eax\n"
        "push ebp\n"
        "push ebx\n"
        CM_ORIGINAL_CALL(0043f480,0043f485,004426e0)
        "add esp, 0xc\n"
        "mov edi, edx\n"
        "mov esi, eax\n"
        "push 0\n"
        "push 2\n"
        "push edi\n"
        "push esi\n"
        CM_ORIGINAL_CALL(0043f492,0043f497,00945e80)
        "add eax, ebx\n"
        "push edi\n"
        "adc edx, ebp\n"
        "push esi\n"
        "push edx\n"
        "push eax\n"
        CM_ORIGINAL_CALL(0043f49f,0043f4a4,00945e80)
        "push edi\n"
        "push esi\n"
        "push edx\n"
        "push eax\n"
        CM_ORIGINAL_CALL(0043f4a8,0043f4ad,00945e40)
        "pop edi\n"
        "pop esi\n"
        "pop ebp\n"
        "pop ebx\n"
        "ret 4\n"
        "1: mov edx, ebp\n"
        "mov eax, ebx\n"
        "pop ebp\n"
        "pop ebx\n"
        "ret 4\n"
        ".att_syntax prefix\n");
}
