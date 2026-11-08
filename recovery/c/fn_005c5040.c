/* Preferred-image VA 005c5040, base 00400000, binary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original callees only. In particular the native 0090cfc0 declaration below
   MUST NOT link the accepted regparm(2) C definition. Its physical transport
   interface is different. All three native bounds here are fixed positive
   small integers (2,3,2): original 0090d01f CDQ kills incoming EDX and
   0090cfd5/0090d008 kill incoming EAX before their first use. Only EAX is
   consumed; ECX output is not carried across these boundaries. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_CDECL cm_fn_00946464(u32 key,u32 base,u32 count,u32 size,u32 cmp);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern u32 CM_CDECL cm_fn_0090cfc0(u32 bound);

/* Reconstructed callback: preferred-image VA 005c5290..005c52c2.
   The native 00946464 boundary passes two stack DWORDs and consumes full EAX.
   Preserve its ordered pointer/year/byte reads, wrapping year subtraction,
   either-FF equality and signed right-byte minus left-byte result. */
static s32 CM_CDECL selection_compare(u32 left_slot,u32 right_slot)
{
    u32 left=D(left_slot);
    u32 right=D(right_slot);
    u32 left_year=D(left+6u);
    u32 right_year=D(right+6u);
    u32 difference=left_year-right_year;
    u8 left_byte,right_byte;
    if(difference)return (s32)difference;
    left_byte=B(left+5u);
    if(left_byte==0xff)return 0;
    right_byte=B(right+5u);
    if(right_byte==0xff)return 0;
    return (s32)(s8)right_byte-(s32)(s8)left_byte;
}

u32 CM_THISCALL cm_fn_005c5040(u32 self,u32 idptr,u32 base,u32 count,u32 rank,u32 keyptr)
{
    volatile u8 scratch[32];
    u32 first,idx,end,item,candidate,root,id,key,list,value,owner,own_owner;
    u32 root_base=D(0x00ae23b4);
    id=D(idptr);
    first=root_base+id*0x245u;
    key=D(keyptr);
    *(volatile u32 *)(scratch+8)=0;
    scratch[12]=0;
    *(volatile u32 *)(scratch+18)=0;
    *(volatile u16 *)(scratch+22)=0;
    *(volatile u16 *)(scratch+24)=0;
    *(volatile u32 *)(scratch+26)=0;
    scratch[13]=0xff;
    *(volatile u32 *)(scratch+14)=key;
    *(volatile u32 *)(scratch+0)=(u32)(scratch+8);
    idx=cm_fn_00946464((u32)scratch,base,count,4,(u32)selection_compare);
    if(!idx)return 0;
    if(idx>base) {
        key=D(keyptr);
        do {
            item=D(idx-4u);
            idx-=4u;
            if(D(item+6u)!=key){idx+=4u;break;}
        }while(idx>base);
    }
    end=base+count*4u-4u;
    if(idx>=end)return 0;
    do {
        item=D(idx);
        root=D(0x00ae23b4);
        id=D(item);
        candidate=root+id*0x245u;
        if(D(candidate+0x53u)!=keyptr)return 0;
        id=D(item);
        candidate=root+id*0x245u;
        if((s8)B(item+5u)>(s8)(u8)rank)goto next;
        value=(u32)(s32)(s16)W(0x00ae2c90);
        if(value==(u32)W(self+0x34e8u)) {
            value=D(candidate+0x1dbu);
            root=D(0x009cf6f8);
            if(value==root)goto next;
        }
        if(first==candidate)goto next;
        list=D(item+0xau);
        if(list) {
            value=D(list+2u);
            D(list+0xau)=value;
            list=D(item+0xau);
            value=cm_fn_0068d1b0(list);
            while(value) {
                root=D(value);
                if(root==D(idptr))goto next;
                list=D(item+0xau);
                value=cm_fn_0068d1b0(list);
            }
        }
        if(first==D(candidate+0xb3u))goto next;
        if(candidate==D(first+0xb3u))goto next;
        if(first==D(candidate+0xb7u))goto next;
        if(candidate==D(first+0xb7u))goto next;
        if(first==D(candidate+0xbbu))goto next;
        if(candidate==D(first+0xbbu))goto next;
        owner=D(candidate+0x53u);
        own_owner=D(first+0x53u);
        if(own_owner==D(owner+0x110u))goto related;
        if(owner==D(own_owner+0x110u))goto related;
        if(own_owner==D(owner+0x114u))goto related;
        if(owner==D(own_owner+0x114u))goto related;
        if(own_owner==D(owner+0x118u))goto related;
        if(owner!=D(own_owner+0x118u))goto unrelated;
related:
        if(!cm_fn_0090cfc0(2))goto next;
unrelated:
        root=D(first+0x57u);
        value=D(candidate+0x57u);
        if(root==value)goto next;
        value=cm_fn_0090cfc0(3);
        root=B(item+4u);
        if((s8)(u8)root>(s8)(u8)(value+3u))goto next;
        if(cm_fn_0090cfc0(2)==1)return item;
next:
        idx+=4u;
    }while(idx<end);
    return 0;
}
