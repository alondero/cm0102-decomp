/* Preferred-image VA 00945161, base 00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL5 range search. Keep signed range tests, unsigned final bounds,
   caller-home writes/reloads, wrapped DWORD addressing and the read at
   index -1 before its later comparison. Original terminal error handling
   remains a boundary; no defensive initialization or bounds guard is added. */
#include "cmre.h"
extern void CM_CDECL __attribute__((noreturn)) cm_fn_0094a5e2(void);
u32 CM_CDECL cm_fn_00945161(volatile u32 record_home,
                           volatile u32 count_home,
                           volatile u32 position_home,
                           volatile u32 first_output_home,
                           volatile u32 last_output_home)
{
    u32 initial_count=count_home;
    u32 record=record_home;
    u32 index=CM_GLOBAL(volatile u32,record+0xcu);
    u32 table=CM_GLOBAL(volatile u32,record+0x10u);
    u32 last=index;
    u32 result=index;
    u32 output;
    record_home=index;
    if ((s32)initial_count>=0) {
        do {
            u32 position,entry,lower;
            u32 match=0u;
            if (index==0xffffffffu)
                cm_fn_0094a5e2();
            position=position_home;
            --index;
            entry=table+index*20u;
            lower=CM_GLOBAL(volatile u32,entry+4u);
            if ((s32)lower<(s32)position) {
                u32 upper=CM_GLOBAL(volatile u32,entry+8u);
                if ((s32)position<=(s32)upper)
                    match=1u;
            }
            if (match!=0u || index==0xffffffffu) {
                u32 previous=record_home;
                u32 count=count_home;
                count_home=count-1u;
                last=previous;
                record_home=index;
            }
        } while ((s32)count_home>=0);
        result=last;
    }
    output=first_output_home;
    ++index;
    CM_GLOBAL(volatile u32,output)=index;
    output=last_output_home;
    CM_GLOBAL(volatile u32,output)=result;
    if (result>CM_GLOBAL(volatile u32,record+0xcu) || index>result)
        cm_fn_0094a5e2();
    return table+index*20u;
}
