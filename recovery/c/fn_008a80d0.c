#include "cmre.h"
/* Original instructions: preferred-image VAs 008a80d0-008a832c,
 * CM0102 3.9.68 SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * ECX receivers and right-to-left pushes are recorded at each call site.
 * 008a82b7 and 008a82df replace only AL of the helper's EAX result. */
extern u32 CM_THISCALL cm_fn_005b7160(u8 *);
extern u32 CM_THISCALL cm_fn_005fc250(u32,u32);
extern u32 CM_THISCALL cm_fn_005fc370(u32,u32);
extern u32 CM_THISCALL cm_fn_005fc010(u32,u32);
extern u32 CM_THISCALL cm_fn_005fc130(u32,u32);
extern u32 CM_THISCALL cm_fn_005fc490(u32,u32);
extern u32 CM_THISCALL cm_fn_00897880(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00897bb0(u32,u32,s8,s8,u32,u32);
extern u32 CM_THISCALL cm_fn_00898150(u32,u32,s8,s8,u32);
extern u32 CM_THISCALL cm_fn_00897ee0(u32,u32,s8,u32,u32);
extern u32 CM_THISCALL cm_fn_006d6f70(u32,u16,u32,s8,u8,s8,s8,u32);
extern u32 CM_THISCALL cm_fn_008ac3d0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_006ce0f0(u32,u32);
extern u32 CM_THISCALL cm_fn_006ce110(volatile u8 *,s8);
extern void CM_THISCALL cm_fn_006bbb60(u32,u32);
extern void CM_THISCALL cm_fn_006bbf60(u32);
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
u32 CM_THISCALL cm_fn_008a80d0(u32 self)
{
    u32 p, q, v, receiver, a, b, mode, record, index;
    s8 row, col, count;
    if (D(self+0xe40)) return 0;
    cm_fn_005b7160((u8 *)(self+0xe44));
    p = D(self+0xdec);
    if (p) {
        q = self+0xe44;
        for (index=0; index<0x638; ++index) {
            D(p)=D(q); p+=4; q+=4;
        }
        W(p)=W(q);
        B(p+2)=B(q+2);
    }
    receiver=D(0xb63c98);
    p=cm_fn_005fc250(receiver,0);
    B(p)=B(self+0x2727);
    receiver=D(0xb63c98);
    p=cm_fn_005fc370(receiver,0);
    B(p)=B(self+0x2728);
    receiver=D(0xb63c98);
    v=D(self+0x2729);
    p=cm_fn_005fc010(receiver,0);
    D(p)=v;
    receiver=D(0xb63c98);
    v=D(self+0x272d);
    p=cm_fn_005fc130(receiver,0);
    D(p)=v;
    receiver=D(0xb63c98);
    p=cm_fn_005fc490(receiver,0);
    D(p)=D(self+0x2731);
    v=D(self+0x4a20);
    if (v!=0xffffffffu) {
        mode=D(self+8);
        p=D(0xae23bc)+v*110u;
        record=D(self);
        receiver=D(0xde1d08);
        cm_fn_00897880(receiver,record,p,mode);
    }
    q=self+0x4a2c;
    for (row=0;row<5;++row)
        for (col=0;col<2;++col) {
            mode=D(self+8); v=D(q); record=D(self);
            receiver=D(0xde1d08);
            cm_fn_00897bb0(receiver,record,row,col,v,mode);
            q+=4;
        }
    q=self+0x4a54;
    for (row=0;row<11;++row) {
        mode=D(self+8); v=B(q); record=D(self);
        receiver=D(0xde1d08);
        cm_fn_00898150(receiver,record,row,(s8)v,mode);
        ++q;
    }
    q=self+0x4a24;
    for (row=0;row<2;++row) {
        mode=D(self+8); v=D(q); record=D(self);
        receiver=D(0xde1d08);
        cm_fn_00897ee0(receiver,record,row,v,mode);
        q+=4;
    }
    a=D(self+4);
    if (a) {
        count=(s8)B(self+0x4a1f);
        if (count) {
            v=count>2 ? 0x1f59 : count==2 ? 0x1f5c : 0x1f5a;
            b=B(self+0xde9);
            receiver=D(a+0x8f7a);
            a=cm_fn_006d6f70(receiver,(u16)v,a,-1,(u8)b,-1,-1,0);
            count=(s8)B(self+0x4a1f);
            a=(a&0xffffff00u)|(u8)count;
            for (row=0;row<count;) {
                b=B(self+(u32)(s32)row+0x4a16);
                v=B(self+(u32)(s32)row+0x4a0d);
                a=cm_fn_008ac3d0(self,v,b);
                count=(s8)B(self+0x4a1f);
                a=(a&0xffffff00u)|(u8)count;
                ++row;
            }
        }
        a=(a&0xffffff00u)|B(self+0xde9);
        receiver=D(self+4);
        cm_fn_006ce0f0(receiver,a);
        b=B(self+0xde9);
        receiver=D(self+4);
        cm_fn_006ce110((volatile u8 *)receiver,(s8)b);
        b=B(self+0xde9);
        receiver=D(self+4);
        cm_fn_006bbb60(receiver,b);
        receiver=D(self+4);
        cm_fn_006bbf60(receiver);
    }
    return 1;
}
