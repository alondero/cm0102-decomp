#include "cmre.h"
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
#define W(a) CM_GLOBAL(volatile u16,(u32)(a))
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
extern void CM_THISCALL cm_fn_006be120(u32 self,u32 selector);
extern u32 CM_THISCALL cm_fn_006ce460(u32 self,u32 mode);
extern void CM_THISCALL cm_fn_006d6f70(u32 self,u32 event,u32 object,u32 a,u32 b,u32 c,u32 d,u32 e);
extern u32 CM_THISCALL cm_fn_006f0970(u32 self);
extern void CM_THISCALL cm_fn_006f5810(u32 self,u32 mode);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination,u32 format,...);
/* Preferred-image VA006bd930/base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver, RET0, caller006b9eed observes signed AL. Explicit native
   callees retain the original implementation and all its state effects. */
u8 CM_THISCALL cm_fn_006bd930(u32 self)
{
    s32 state,x,y,z,t;
    u32 record,object,row,n,flag=0;
    u16 bits;
    u8 value,cl,bl;
    if(B(self+0x8eb2)==5) return 0;
    state=(s16)W(self+0x8ed4);
    if(state==0x1ef) {
        x=(s8)B(self+0x8eb7);y=(s16)W(self+0x8ed0);
        if(y<=x+0x1ef) return 0;
        record=D(self+0x4792);value=B(self+0xf5bd);B(record+0x45)=value;
        record=D(self+0x4792);value=B(self+0xf5f3);B(record+0x46)=value;
        object=D(self+0x8f7a);
        cm_fn_006d6f70(object,0x20f1,self,0xffffffffu,0,0xffffffffu,0xffffffffu,0);
        W(self+0x8ed0)=0x1ef;B(self+0x8eb7)=0;
        cm_fn_006ce460(self,0x3de);
        row=self+0x4796;n=20;do {cm_fn_006f5810(row,0);row+=0x1be;}while(--n);
        row=self+0x6a6e;n=20;do {cm_fn_006f5810(row,0);row+=0x1be;}while(--n);
        object=D(self+0xf582);W(self+0x8ed4)=0x3de;
        B(self+0x8eb2)=8;B(self+0x1ca)=0;
        if(object)cm_fn_006f0970(object);
        value=B(self+0x8eb6);B(self+0x8eae)=value;B(self+0x8eb6)=value==0;
        return 1;
    }
    if(state==0x3de) {
        x=(s8)B(self+0x8eb7);y=(s16)W(self+0x8ed0);
        if(y<=x+0x3de)return 0;
        record=D(self+0x4792);value=B(self+0xf5bd);B(record+0x43)=value;
        record=D(self+0x4792);value=B(self+0xf5f3);B(record+0x44)=value;
        record=D(self+0x4792);bits=W(record+0x38);cl=B(record+0x4b);
        if(!(bits&2)) {
            if(cl!=255 && !(bits&4)) {
                bl=B(record+0x44);
                if(bl!=cl) {
                    x=(s8)B(record+0x4c);y=(s8)bl;x+=y;
                    y=(s8)B(record+0x43);z=(s8)cl;y+=z;
                    if(y==x)goto pair_then_finish;
                }
            }
            if(bits&1) {
                x=(s8)B(record+0x4c);y=(s8)B(record+0x44);x+=y;
                y=(s8)B(record+0x43);z=(s8)cl;y+=z;
                if(y==x){B(self+0x8eb3)=4;goto finish_regular;}
            }
            if(bits&0x20) {
                x=(s8)B(record+0x4c);y=(s8)B(record+0x44);
                z=(s8)B(record+0x43);t=(s8)cl;x+=y;z+=t;
                if(z==x){B(self+0x8eb3)=1;goto finish_regular;}
            }
pair_then_finish:
            cm_fn_006be120(self,0);cm_fn_006be120(self,1);
finish_regular:
            B(self+0x8eb2)=8;B(self+0x1ca)=0;
            goto rows_then_zero;
        }
        if(cl!=255) {
            bl=B(record+0x44);x=(s8)B(record+0x4c);
            y=(s8)B(record+0x43);z=(s8)bl;x+=z;z=(s8)cl;y+=z;
            if(y!=x)goto finish_before_pair;
            if(!(bits&4) && bl!=cl && !(bits&0x4000))goto finish_before_pair;
            object=D(self+0x8f7a);
            cm_fn_006d6f70(object,0x20f2,self,0xffffffffu,0,0xffffffffu,0xffffffffu,0);
            W(self+0x8ed0)=0x3de;B(self+0x8eb7)=0;W(self+0x8ed4)=0x483;
            cm_fn_006ce460(self,0x3de);
            object=D(self+0xf582);if(object)cm_fn_006f0970(object);
            value=B(self+0x8eb6);B(self+0x8eb2)=8;cl=value==0;
            B(self+0x1ca)=0;B(self+0x8eae)=value;B(self+0x8eb6)=cl;
            return 2;
        }
        value=B(record+0x43);cl=B(record+0x44);
        if(value!=cl)goto finish_before_pair;
        object=D(self+0x8f7a);
        cm_fn_006d6f70(object,0x20f2,self,0xffffffffu,0,0xffffffffu,0xffffffffu,0);
        object=D(self+0xf582);W(self+0x8ed0)=0x3de;
        B(self+0x8eb7)=0;W(self+0x8ed4)=0x483;
        if(object)cm_fn_006f0970(object);
        cm_fn_006ce460(self,0x3de);
        value=B(self+0x8eb6);B(self+0x8eb2)=8;B(self+0x8eae)=value;
        B(self+0x1ca)=0;B(self+0x8eb6)=value==0;
        return 2;
finish_before_pair:
        B(self+0x8eb2)=8;B(self+0x1ca)=0;W(self+0x8ed4)=0;
        cm_fn_006be120(self,0);cm_fn_006be120(self,1);
        row=self+0x4796;n=20;do {cm_fn_006f5810(row,1);row+=0x1be;}while(--n);
        row=self+0x6a6e;n=20;do {cm_fn_006f5810(row,1);row+=0x1be;}while(--n);
        cm_fn_006ce460(self,0);return 4;
rows_then_zero:
        row=self+0x4796;n=20;do {cm_fn_006f5810(row,1);row+=0x1be;}while(--n);
        row=self+0x6a6e;n=20;do {cm_fn_006f5810(row,1);row+=0x1be;}while(--n);
        cm_fn_006ce460(self,0);W(self+0x8ed4)=0;return 4;
    }
    if(state==0x528) {
        value=B(self+0x1a8);
        if(!value) {
            x=(s8)B(self+0x8eb7);y=(s16)W(self+0x8ed0);
            if(y<=x+0x528)return 0;
        }
        record=D(self+0x4792);value=B(self+0xf5bd);B(record+0x47)=value;
        record=D(self+0x4792);value=B(self+0xf5f3);B(record+0x48)=value;
        record=D(self+0x4792);cl=B(record+0x4b);
        if(cl!=255) {
            bits=W(record+0x38);
            if(bits&0x4000) {
                bl=B(record+0x48);x=(s8)B(record+0x4c);y=(s8)bl;x+=y;
                y=(s8)B(record+0x47);z=(s8)cl;y+=z;
                if(y==x && bl==cl && (B(record+0x38)&0x21))flag=1;
            } else {
                x=(s8)B(record+0x4c);y=(s8)B(record+0x48);x+=y;
                y=(s8)B(record+0x47);z=(s8)cl;y+=z;
                if(y==x && (bits&0x21))flag=1;
            }
        } else if(B(record+0x38)&0x21)flag=1;
        bits=W(record+0x38);
        if(bits&1) {
            x=(s8)B(record+0x48);y=(s8)B(record+0x44);z=(s8)B(record+0x43);
            x-=y;y=(s8)B(record+0x47);y-=z;
            if(y==x && flag){B(self+0x8eb3)=4;goto finish_one;}
        }
        if(bits&0x20) {
            x=(s8)B(record+0x48);y=(s8)B(record+0x44);x-=y;
            y=(s8)B(record+0x47);z=(s8)B(record+0x43);y-=z;
            if(y==x && flag){B(self+0x8eb3)=1;goto finish_one;}
        }
        cm_fn_006be120(self,0);cm_fn_006be120(self,1);
        goto finish_one;
    }
    if(state==0x483) {
        if(B(self+0x1a8)) {
            record=D(self+0x4792);value=B(self+0xf5bd);B(record+0x47)=value;
            record=D(self+0x4792);value=B(self+0xf5f3);B(record+0x48)=value;
            B(self+0x8eb2)=8;B(self+0x1ca)=0;W(self+0x8ed4)=1;
            cm_fn_006be120(self,0);cm_fn_006be120(self,1);
            goto rows_one;
        }
        x=(s8)B(self+0x8eb7);y=(s16)W(self+0x8ed0);
        if(y<=x+0x483)return 0;
        object=D(self+0x8f7a);
        cm_fn_006d6f70(object,0x20f3,self,0xffffffffu,0,0xffffffffu,0xffffffffu,0);
        object=D(self+0xf582);W(self+0x8ed0)=0x483;
        B(self+0x8eb7)=0;W(self+0x8ed4)=0x528;B(self+0x8eb2)=8;B(self+0x1ca)=0;
        if(object)cm_fn_006f0970(object);
        value=B(self+0x8eb6);cl=value==0;B(self+0x8eae)=value;B(self+0x8eb6)=cl;
        return 3;
    }
    if(state==0 || state==1)return 4;
    cm_fn_00944c9f(0x00de1f64,0x00a158f8,state);return 0;
finish_one:
    B(self+0x8eb2)=8;B(self+0x1ca)=0;W(self+0x8ed4)=1;
rows_one:
    row=self+0x4796;n=20;do {cm_fn_006f5810(row,1);row+=0x1be;}while(--n);
    row=self+0x6a6e;n=20;do {cm_fn_006f5810(row,1);row+=0x1be;}while(--n);
    cm_fn_006ce460(self,1);return 4;
}
