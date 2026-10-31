#include "cmre.h"

/* Preferred-image VA 007cfd30, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Keep BYTE relegation count, signed WORD selected count and late scratch
   reloads. Direction is written by the original partner lookup; no default
   initialization is supplied. Original helper stack/register boundaries stay. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_004afbd0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00540a50(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_006826d0(u32,u32);
extern u32 CM_THISCALL cm_fn_0048ece0(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_006831a0(u32,u32,u32,u32);
extern u32 CM_STDCALL cm_fn_006830b0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_007cfd30(u32 self)
{
    u8 scratch[0x214];
    u32 local=(u32)scratch,base,count,key,target,array,selected=0,scan;
    u32 club,country,parent,code,record,partner,direction,competition,value;
    u32 receiver,result,raw_count,version;
    u8 relegated;
    base=D(0x00adadfc);
    count=D(0x00ae235c);
    D(local+4)=self;
    key=D(0x009cf610);
    target=D(base+key*4);
    D(local+8)=0;
    B(local+3)=0;
    array=cm_fn_00944e46(count*4);
    D(local+0x10)=array;
    if (array==0) {
        cm_fn_00944cff(0x00a78724,0,0,local+0x114,local+0x14);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,
                       local+0x114,local+0x14,0x28d);
        result=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        return result;
    }
    count=D(0x00ae235c);
    club=D(0x00ae23b4);
    scan=0;
    if ((s32)count>0) {
        do {
            country=D(club+0x57);
            if (country==0) goto next_club;
            parent=D(club+0x53);
            if (parent==0) goto next_club;
            code=D(country);
            if (code!=D(0x009cf614) && code!=D(0x009cf618) && code!=D(0x009cf61c))
                goto next_club;
            value=D(parent);
            key=D(0x009cf434);
            if (value==key) {
                D(array+(u32)(s32)(s16)selected*4)=club;
                ++selected;
            }
next_club:
            count=D(0x00ae235c);
            ++scan;
            club+=0x245;
        } while ((s32)scan<(s32)count);
        D(local+8)=selected;
    }
    result=club;
    scan=0;
    if ((s16)W(target+0x3e)>0) {
        do {
            record=cm_fn_006826d0(target,scan);
            club=D(record);
            partner=cm_fn_00540a50(club,local+0xc,1);
            if (partner!=0) {
                direction=D(local+0xc);
                if (direction==0) {
                    country=D(partner+0x57);
                    if (country!=0) {
                        code=D(country);
                        key=D(0x009cf610);
                        if (code==key) {
                            country=D(club+0x57);
                            key=D(0x009cf60c);
                            code=D(country);
                            if (code!=key) {
                                record=cm_fn_006826d0(target,scan);
                                B(record+0x37)=3;
                                parent=D(local+4);
                                competition=D(parent+4);
                                cm_fn_0048ece0(target,competition,club,partner,1);
                            }
                        }
                    }
                }
            }
            record=cm_fn_006826d0(target,scan);
            result=record;
            if (B(record+0x37)==3) {
                relegated=B(local+3);
                base=D(0x00ae23d0);
                relegated=(u8)(relegated+1);
                B(local+3)=relegated;
                key=D(0x009cf614);
                competition=base+key*107;
                record=cm_fn_006826d0(target,scan);
                club=D(record);
                receiver=D(local+4);
                result=cm_fn_006831a0(receiver,club,competition,1);
            }
            count=(u32)(s32)(s16)W(target+0x3e);
            ++scan;
        } while ((s32)scan<(s32)count);
        selected=D(local+8);
        array=D(local+0x10);
    }
    raw_count=(u32)(s32)(s8)B(local+3);
    raw_count=(result&0xffff0000u)|(u16)raw_count;
    if ((s16)selected>=(s16)raw_count) {
        cm_fn_004afbd0(array,selected,raw_count);
        count=(u32)(s32)(s8)B(local+3);
        if ((s32)count>0) {
            parent=array;
            do {
                key=D(0x009cf610);
                receiver=D(local+4);
                base=D(0x00ae23d0);
                competition=base+key*107;
                club=D(parent);
                (void)receiver;
                cm_fn_006830b0(club,competition,1);
                parent+=4;
                --count;
            } while (count!=0);
        }
    }
    return cm_fn_009452ca(array);
}
