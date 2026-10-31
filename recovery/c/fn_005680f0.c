/* Preferred-image VA005680f0/base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL2/RET0/full EAX. Original profile-v2 input clears DF.
   Preserve byte SCAS reads, the unbounded original 104h scratch copy,
   actual second-formal reload after original lowercase, and DWORD/tail
   string-copy widths. The original keeps the path/source/destination live
   in registers during the unsafe copy, then recomputes the scratch address
   with LEA and pushes an immediate key. Do not cache either across overflow.
   No original helper C source is composed or changed. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_0090c120(u32 path);
extern u32 CM_CDECL cm_fn_009532a6(u32 path);
extern u32 CM_CDECL cm_fn_00945a60(u32 haystack,u32 needle);
extern u32 CM_CDECL cm_fn_0090bde0(u32 key,u32 tail);

/* The three original branches have the same byte loop but distinct immediate
   keys after it. Keep that control flow: a stack-resident selected key can be
   overwritten by the original unbounded copy. scratch remains exactly 104h. */
#define COPY_TAIL(key_) do { \
    current=B(source+1u); \
    source+=2u; \
    B((u32)scratch)=current; \
    destination=(u32)scratch+1u; \
    while (current!=0) { \
        if (current==0x3a) B(destination-1u)=0x5c; \
        current=B(source); \
        B(destination)=current; \
        ++destination; \
        ++source; \
    } \
    source=cm_fn_0090bde0((key_),(u32)scratch); \
} while (0)

u32 CM_CDECL cm_fn_005680f0(u32 path,volatile u32 mode_arg)
{
    u8 scratch[0x104];
    register u32 saved_path=path;
    register u32 source=path;
    register u32 destination;
    u32 count=0xffffffffu,found;
    u8 current;
    do {
        current=B(source);
        ++source;
        --count;
    } while (current!=0 && count!=0);
    count=~count;
    if (count-1u==0) return 0;
    found=cm_fn_0090c120(saved_path);
    if (found!=0) return found;
    (void)cm_fn_009532a6(saved_path);
    if (mode_arg!=0) {
        found=cm_fn_00945a60(saved_path,0x009c3840u);
        if (found!=0) {
            do {
                source=found+8u;
                found=cm_fn_00945a60(source,0x009c3840u);
            } while (found!=0);
            if (source!=saved_path) {
                COPY_TAIL(0x009899e0u);
                goto copy_result;
            }
        }
    } else {
        found=cm_fn_00945a60(saved_path,0x009c3838u);
        if (found!=0) {
            do {
                source=found+6u;
                found=cm_fn_00945a60(source,0x009c3838u);
            } while (found!=0);
            if (source!=saved_path) {
                COPY_TAIL(0x009c382cu);
                goto copy_result;
            }
        }
    }
    found=cm_fn_00945a60(saved_path,0x009c3824u);
    if (found==0) return 0;
    do {
        source=found+4u;
        found=cm_fn_00945a60(source,0x009c3824u);
    } while (found!=0);
    if (source==saved_path) return 0;
    COPY_TAIL(0x009b5348u);
copy_result:
    count=0xffffffffu;
    do {
        current=B(source);
        ++source;
        --count;
    } while (current!=0 && count!=0);
    count=~count;
    source-=count;
    destination=saved_path;
    while (count>=4u) {
        found=D(source);
        D(destination)=found;
        source+=4u;
        destination+=4u;
        count-=4u;
    }
    while (count!=0) {
        current=B(source);
        B(destination)=current;
        ++source;
        ++destination;
        --count;
    }
    return 0;
}
