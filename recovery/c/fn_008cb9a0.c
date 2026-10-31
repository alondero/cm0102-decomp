#include "cmre.h"

/* Preferred-image VA 008cb9a0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original allocation, date, sort, lookup and iterator boundaries retained.
   Capacity/count arithmetic wraps in DWORDs, with late field reloads. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_00945667(u32,u32);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern void CM_CDECL cm_fn_00945333(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00842b00(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_008cb9a0(u32 self,volatile u32 record_arg,
                              volatile u32 date_arg)
{
    u8 scratch[0x208];
    u32 local=(u32)scratch,capacity,used,data,result,version,date;
    u32 record,value,key,base,receiver,found,list,node,address;
    u8 flags;
    capacity=D(self+0x924);
    used=D(self+0x928);
    if (used==capacity) {
        data=D(self+0x920);
        capacity+=10;
        D(self+0x924)=capacity;
        if (data!=0) result=cm_fn_00945667(data,capacity*12);
        else result=cm_fn_00944e46(capacity*12);
        D(self+0x920)=result;
        if (result==0) {
            cm_fn_00944cff(0x00a8d514,0,0,local+0x108,local+8);
            version=cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0,0x009a8120,version,
                local+0x108,local+8,0x27c3,0x00a8ec74);
            cm_fn_0066f4e0(0x00ae28bc,0x009a8108);
            cm_fn_0066f4e0(0x00ae26c4,0x009a7ff8,0x00ae24d0);
            return cm_fn_005e8290(0x00ae28bc,0x00ae26c4,0);
        }
    }
    date=date_arg;
    used=D(self+0x928);
    data=D(self+0x920);
    cm_fn_00417c10(data+used*12,local,date);
    used=D(self+0x928);
    record=record_arg;
    data=D(self+0x920);
    value=D(record+0x20);
    D(data+used*12+8)=value;
    used=D(self+0x928);
    data=D(self+0x920);
    ++used;
    D(self+0x928)=used;
    cm_fn_00945333(data,used,12,0x00778e90);
    key=D(record+0xc);
    B(record+0x2d)=1;
    B(record+0x2c)=0x13;
    base=D(0x00ae23bc);
    receiver=D(self+0x856);
    cm_fn_00842b00(receiver,base+key*110,0);
    key=D(record+0xc);
    data=D(self+0x86e);
    address=data+key*8;
    flags=B(address);
    B(address)=(u8)(flags|0x10);
    key=D(record+0xc);
    base=D(0x00ae23bc);
    receiver=D(0x00ae1050);
    found=cm_fn_004cf5a0(receiver,base+key*110);
    if (found!=0) {
        flags=B(found+0x49);
        if (flags!=0) {
            value=D(record+0x20);
            D(found+0x4a)=value;
        }
    }
    list=D(record+4);
    if (list==0) return 0;
    value=D(list+2);
    D(list+0xa)=value;
    list=D(record+4);
    node=cm_fn_0068d1b0(list);
    while (node!=0) {
        key=D(node);
        base=D(0x00ae23bc);
        receiver=D(self+0x856);
        cm_fn_00842b00(receiver,base+key*110,0);
        key=D(node);
        data=D(self+0x86e);
        address=data+key*8;
        flags=B(address);
        B(address)=(u8)(flags|0x10);
        list=D(record+4);
        node=cm_fn_0068d1b0(list);
    }
    return 0;
}
