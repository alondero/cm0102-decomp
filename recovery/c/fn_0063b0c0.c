#include "cmre.h"

/* Preferred-image VA 0063b0c0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Each division scan restarts even when the previous scan filled the target;
   its equality stop and cached global count remain in their original places.
   Raw index high bits are retained at the original helper boundary. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_THISCALL cm_fn_00687430(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_0063b0c0(u32 self)
{
    u8 scratch[0x200];
    u32 local=(u32)scratch,result,selected=0,count,club,scan,group;
    u32 country,key,division,wanted,limit,base,raw_index,version;
    W(self+0x3e)=4;
    result=cm_fn_00944e46(0xec);
    D(self+0xb1)=result;
    if (result==0) {
        cm_fn_00944cff(0x009ea0b8,0,0,local+0x100,local);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,
                       local+0x100,local,0xb5);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        return 0xffffffffu;
    }
    count=D(0x00ae235c);
    for (group=0;group<3;++group) {
        club=D(0x00ae23b4);
        scan=0;
        if ((s32)count>0) {
            do {
                country=D(club+0x53);
                if (country!=0) {
                    key=D(0x009cf2dc);
                    if (D(country)==key) {
                        division=D(club+0x1db);
                        wanted=D(0x009cf6e8+group*8);
                        if (division==wanted) {
                            limit=(u32)(s32)(s16)W(self+0x3e);
                            if ((s32)selected<(s32)limit) {
                                raw_index=(division&0xffffff00u)|(u8)selected;
                                ++selected;
                                cm_fn_00687430(self,raw_index,club,0,0);
                                count=D(0x00ae235c);
                            }
                        }
                    }
                }
                limit=(u32)(s32)(s16)W(self+0x3e);
                if (selected==limit) break;
                ++scan;
                club+=0x245;
            } while ((s32)(s16)scan<(s32)count);
        }
    }
    base=D(0x00ae2350);
    club=D(0x00ae23b4);
    scan=0;
    limit=count-base*2;
    if ((s32)limit>0) {
        do {
            limit=(u32)(s32)(s16)W(self+0x3e);
            if ((s32)selected>=(s32)limit) break;
            country=D(club+0x57);
            if (country!=0) {
                key=D(country);
                wanted=D(0x009cf80c);
                if (key==wanted) {
                    raw_index=(key&0xffffff00u)|(u8)selected;
                    ++selected;
                    cm_fn_00687430(self,raw_index,club,0,0);
                    count=D(0x00ae235c);
                }
            }
            base=D(0x00ae2350);
            ++scan;
            club+=0x245;
            limit=count-base*2;
        } while ((s32)(s16)scan<(s32)limit);
    }
    limit=(u32)(s32)(s16)W(self+0x3e);
    return selected==limit;
}
