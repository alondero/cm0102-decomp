/* Preferred-image VA 0x00715f30, base 0x00400000.
   Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The unaligned receiver field is read before any caller stack write. The
   nonnull branch passes the original self as a stack argument and uses the
   callee's actual cleanup; the null branch modifies AL only. Return value is
   physical EAX, not an inferred semantic result. GCC mixed naked C/asm and
   owned original-call VM continuations require emission and raw alias/fault
   review; native continuation is not established. */
#include "cmre.h"
extern u32 CM_THISCALL cm_fn_006c4130(void *, void *);
#define PLAYER_715F_STATE "+a"(eax), "+c"(ecx), "+d"(edx), "+b"(ebx), "+S"(esi), "+D"(edi), "+r"(ebp)
#define PLAYER_715F_STEP(t) __asm__ volatile(t : PLAYER_715F_STATE : : "memory", "cc")
u32 CM_THISCALL __attribute__((naked,noinline,used,optimize("O2","no-tree-ch","align-loops=1","align-jumps=1","align-labels=1")))
cm_fn_00715f30(void *self __attribute__((unused)))
{
 register u32 eax __asm__("eax"),ecx __asm__("ecx"),edx __asm__("edx");
 register u32 ebx __asm__("ebx"),esi __asm__("esi"),edi __asm__("edi"),ebp __asm__("ebp");
 u8 zero;
 __asm__ volatile("" : "=a"(eax),"=c"(ecx),"=d"(edx),"=b"(ebx),"=S"(esi),"=D"(edi),"=r"(ebp));
 __asm__ volatile("v_00715f30: movl %%ecx, %%eax\n"
                  "v_00715f32: movl 0x19e(%%eax), %%ecx\n"
                  "v_00715f38: testl %%ecx, %%ecx\n v_00715f3a:\n"
                  : PLAYER_715F_STATE, "=@ccz"(zero) : : "memory", "cc");
 if (!zero) {
  PLAYER_715F_STEP("v_00715f3c: pushl %%eax\n" CM_ORIGINAL_CALL(00715f3d,00715f42,006c4130) "ret\n");
  __builtin_unreachable();
 }
 PLAYER_715F_STEP("v_00715f43: orb $0xff, %%al\n v_00715f45: ret\n");
 __builtin_unreachable();
}
