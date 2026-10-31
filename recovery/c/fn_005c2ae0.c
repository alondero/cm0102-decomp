#include "cmre.h"

/* Preferred-image VA 005c2ae0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch20: outputs L0/4/8, cached club offset Lc, copied date L10,
   current date L18. Preserve unsigned year/signed day and early full EAX. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
typedef struct { u32 prev,handler,state; } frame_5c2ae0;
extern u32 CM_THISCALL cm_fn_0054c770(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_005c75a0(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_005c6f00(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_005c5570(u32,u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);

u32 CM_THISCALL cm_fn_005c2ae0(u32 self)
{
    volatile frame_5c2ae0 frame;
    u8 scratch[0x20];
    u32 local=(u32)scratch,result,index,count,table,record,year,day,other_year;
    u32 first,second,third,value,base,second_offset,third_offset,other,date_pointer;
    frame.state=0xffffffffu;
    frame.handler=0x00959e10;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    cm_fn_0054c770(0x00ae2c90,local+0x18,1);
    result=D(self+0x34d8);
    frame.state=0;
    if (result==0) goto done;
    index=D(self+0x34dc);
    result=D(self+0x34e0);
    if ((s32)index>=(s32)result) goto release;
    do {
        table=D(self+0x34d8);
        year=W(local+0x1a);
        day=D(local+0x18);
        record=table+index*20;
        result=record;
        if ((u16)year==W(record+2) && (u16)day==W(record)) {
            value=D(record);
            first=D(record+8);
            second=D(record+0xc);
            third=D(record+0x10);
            D(local+0x10)=value;
            value=D(record+4);
            D(local+0x14)=value;
            B((u32)&frame.state)=1;
            if (first==0xffffffffu || third==0xffffffffu || second==0xffffffffu) {
                ++index;
                B((u32)&frame.state)=0;
                D(self+0x34dc)=index;
                goto next;
            }
            second_offset=second*581;
            base=D(0x00ae23b4);
            D(local+0xc)=second_offset;
            result=cm_fn_005c75a0(self,base+second_offset,local+0x10,local+8,local+4,
                                  local,0,0,0,1);
            if ((u8)result==1) {
                third_offset=third*581;
                base=D(0x00ae23b4);
                result=cm_fn_005c75a0(self,base+third_offset,local+0x10,local+8,local+4,
                                      local,0,0,0,1);
                if ((u8)result==1) {
                    if (first==second) {
                        index=D(self+0x34dc);
                        base=D(0x00ae23b4);
                        other=base+third_offset;
                        table=D(self+0x34d8);
                        date_pointer=table+index*20;
                        result=cm_fn_005c6f00(self,base+first*581,date_pointer,other,1);
                    } else {
                        base=D(0x00ae23b4);
                        second_offset=D(local+0xc);
                        index=D(self+0x34dc);
                        other=base+second_offset;
                        table=D(self+0x34d8);
                        date_pointer=table+index*20;
                        result=cm_fn_005c6f00(self,base+first*581,date_pointer,other,0);
                    }
                    if (result!=0) cm_fn_005c5570(self,result);
                }
            }
            index=D(self+0x34dc);
            B((u32)&frame.state)=0;
            ++index;
            D(self+0x34dc)=index;
        } else {
            other_year=W(record+2);
            if ((u16)year<(u16)other_year) goto done;
            if ((u16)year==(u16)other_year) {
                value=W(record);
                if ((s16)day<(s16)value) goto done;
            }
            other_year=W(record+2);
            if ((u16)year>(u16)other_year) {
                ++index;
                D(self+0x34dc)=index;
            } else if ((u16)year==(u16)other_year) {
                value=W(record);
                result=(s16)day>(s16)value;
                if (result!=0) {
                    ++index;
                    D(self+0x34dc)=index;
                }
            }
        }
next:
        index=D(self+0x34dc);
        count=D(self+0x34e0);
        result=count;
    } while ((s32)index<(s32)count);
release:
    table=D(self+0x34d8);
    cm_fn_009452ca(table);
    result=0;
    D(self+0x34d8)=0;
    D(self+0x34e0)=0;
    D(self+0x34e4)=0;
done:
    CM_FS(volatile u32,0)=frame.prev;
    return result;
}
