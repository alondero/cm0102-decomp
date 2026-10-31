#include "cmre.h"

/* Preferred-image VA 005750a0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preserve the three scratch group slots, signed WORD selected count, and
   the call made even when a new group finds all three slots occupied.
   Nested 6826d0 calls consume only their own stack argument; the earlier
   group and flag pushes belong to the subsequent 6831a0 call. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_004afbd0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_006826d0(u32,u32);
extern u32 CM_STDCALL cm_fn_006830b0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_006831a0(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_005750a0(u32 self)
{
    u8 scratch[0x21c];
    u32 local=(u32)scratch,base,key,target,count,array,result,club,country;
    u32 parent,code,selected=0,seen=0,scan,value,limit,position,record;
    u32 group_slot,competition,receiver,group,version;
    u8 index,group_index;
    base=D(0x00adadfc);
    D(local)=self;
    key=D(0x009cf69c);
    target=D(base+key*4);
    count=D(0x00ae235c);
    D(local+0x18)=target;
    D(local+0xc)=0;
    D(local+0x10)=0;
    D(local+0x14)=0;
    array=cm_fn_00944e46(count*4);
    D(local+4)=array;
    if (array==0) {
        cm_fn_00944cff(0x009c43b0,0,0,local+0x11c,local+0x1c);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,
                       local+0x11c,local+0x1c,0x2f9);
        result=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        return result;
    }
    count=D(0x00ae235c);
    club=D(0x00ae23b4);
    D(local+8)=0;
    if ((s32)count>0) {
        do {
            country=D(club+0x57);
            if (country==0) goto next_club;
            parent=D(club+0x53);
            if (parent==0) goto next_club;
            code=D(country);
            if (code==D(0x009cf764)) goto next_club;
            if (code==D(0x009cf5bc)) goto next_club;
            if (code==D(0x009cf5c0)) goto next_club;
            if (code==D(0x009cf5c4)) goto next_club;
            if (code==D(0x009cf5c8)) goto next_club;
            if (code==D(0x009cf69c)) goto next_club;
            value=D(parent);
            key=D(0x009cf2e4);
            if (value==key) {
                position=(u32)(s32)(s16)selected;
                ++selected;
                D(array+position*4)=club;
                if (seen==0) seen=D(club+0x57);
            }
next_club:
            scan=D(local+8);
            count=D(0x00ae235c);
            ++scan;
            club+=0x245;
            D(local+8)=scan;
        } while ((s32)scan<(s32)count);
    }
    if ((s16)selected==0) return cm_fn_009452ca(array);
    cm_fn_004afbd0(array,selected,3);
    limit=(u32)(s32)(s16)selected;
    scan=0;
    if ((s32)limit>0) {
        position=D(local+4);
        do {
            record=D(position);
            country=D(record+0x57);
            for (index=0;index<3;++index) {
                if (country!=0 && country==D(local+0xc+(u32)index*4)) break;
            }
            if (index==3) {
                for (group_index=0;group_index<3;++group_index) {
                    group_slot=local+0xc+(u32)group_index*4;
                    if (D(group_slot)==0) {
                        D(group_slot)=country;
                        break;
                    }
                }
                key=D(0x009cf69c);
                base=D(0x00ae23d0);
                competition=base+key*107;
                receiver=D(local);
                (void)receiver;
                cm_fn_006830b0(record,competition,1);
                if (group_index==2) break;
            }
            ++scan;
            position+=4;
        } while ((s32)scan<(s32)limit);
    }
    target=D(local+0x18);
    scan=0;
    group_index=0;
    if ((s16)W(target+0x3e)>0) {
        do {
            if ((s8)group_index>=3) break;
            record=cm_fn_006826d0(target,scan);
            if (B(record+0x37)==3) {
                group=D(local+0xc+(u32)(s32)(s8)group_index*4);
                group_index=(u8)(group_index+1);
                record=cm_fn_006826d0(target,scan);
                club=D(record);
                receiver=D(local);
                cm_fn_006831a0(receiver,club,group,1);
            }
            count=(u32)(s32)(s16)W(target+0x3e);
            ++scan;
        } while ((s32)scan<(s32)count);
    }
    array=D(local+4);
    return cm_fn_009452ca(array);
}
