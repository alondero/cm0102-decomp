/* Preferred-image VA 00945487, base 00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL4 selection helper. Preserve the caller-owned second-home write,
   cached upper pointer, unsigned pointer guards, signed comparator result
   and every original stride/first-home reload. The comparator and swap are
   explicit original boundaries. No size or pointer validity guard is added. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_009454d5(u32,u32,u32);
typedef u32 (CM_CDECL *compare_00945487)(u32,u32);
u32 CM_CDECL cm_fn_00945487(volatile u32 first_home,
                           volatile u32 second_home,
                           volatile u32 stride_home,
                           volatile u32 compare_home)
{
    u32 result=first_home;
    u32 upper=second_home;
    if (upper>result) {
        u32 stride=stride_home;
        second_home=result+stride;
        do {
            u32 selected=first_home;
            u32 cursor=second_home;
            while (cursor<=upper) {
                u32 target=compare_home;
                result=((compare_00945487)target)(cursor,selected);
                if ((s32)result>0)
                    selected=cursor;
                cursor+=stride_home;
            }
            stride=stride_home;
            result=cm_fn_009454d5(selected,upper,stride);
            upper-=stride_home;
        } while (upper>first_home);
    }
    return result;
}
