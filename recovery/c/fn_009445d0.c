/* Preferred-image VA 009445D0, base 00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL2. Physical EAX/EDX prefixes add no stack words. Preserve the full
   EDX:EAX pair: null allocation leaves incoming EDX, otherwise original
   009452CA supplies it. Every known EAX/EDX/ECX input is explicit at native
   boundaries. Original longjmp performs context transfer, never host C.
   The volatile address local retains the original erroneous 9Ch store
   without GCC treating that constant address as a bounded C array. */
#include "cmre.h"
extern unsigned long long CM_CDECL __attribute__((regparm(3)))
    cm_fn_009452ca(u32,u32,u32,u32);
extern void CM_CDECL __attribute__((regparm(3),noreturn))
    cm_fn_0094791c(u32,u32,u32,u32,u32);
unsigned long long CM_CDECL __attribute__((regparm(2)))
cm_fn_009445d0(u32 entry_eax,u32 entry_edx,
               volatile u32 owner_home,volatile u32 allocation_home)
{
    u32 owner=owner_home;
    u32 allocation,header,edx,index;
    if (owner==0u) {
        volatile u32 fault_address=0x9cu;
        CM_GLOBAL(volatile u32,fault_address)=0x5500aaffu;
        cm_fn_0094791c(entry_eax,entry_edx,0u,0x10u,0x271bu);
    }
    allocation=allocation_home;
    if (allocation==0u)
        return (unsigned long long)entry_edx<<32;
    header=allocation-0x10u;
    edx=CM_GLOBAL(volatile u32,header);
    if (edx!=0xaa53c5aau)
        goto bad_marker;
    edx=CM_GLOBAL(volatile u32,header+4u);
    if (CM_GLOBAL(volatile u8,edx+header+0x10u)!=0x5au)
        goto bad_marker;
    if (CM_GLOBAL(volatile u8,edx+header+0x11u)!=0xf0u)
        goto bad_marker;
    edx=CM_GLOBAL(volatile u32,owner+0xcu);
    index=CM_GLOBAL(volatile u32,header+8u);
    if (CM_GLOBAL(volatile u32,edx+index*4u)!=header) {
        CM_GLOBAL(volatile u32,owner+0x9cu)=0x5500aaffu;
        cm_fn_0094791c(header,edx,owner+0x10u,owner+0x10u,0x271fu);
    }
    CM_GLOBAL(volatile u32,edx+index*4u)=0u;
    edx=CM_GLOBAL(volatile u32,owner+4u);
    --edx;
    CM_GLOBAL(volatile u32,owner+4u)=edx;
    {
        volatile unsigned long long result=cm_fn_009452ca(header,edx,owner,header);
        return result;
    }
bad_marker:
    CM_GLOBAL(volatile u32,owner+0x9cu)=0x5500aaffu;
    cm_fn_0094791c(header,edx,owner+0x10u,owner+0x10u,0x2719u);
}
