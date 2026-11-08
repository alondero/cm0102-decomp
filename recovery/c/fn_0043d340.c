/* Preferred-image VA0043D340; base00400000. Target SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THISCALL/RET4, EAX dead in direct caller0043D31C. Original callees remain
   explicit boundaries. Retain all 58 SEH states and the final state 0x39. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define W(a) CM_GLOBAL(volatile u16,(u32)(a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_008af520(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0092fe80(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern void CM_CDECL cm_fn_005e8290(u32,u32,u32);

void CM_THISCALL cm_fn_0043d340(u32 self,u32 argument)
{
    u32 predecessor=CM_FS(u32,0);
    volatile struct {u32 prev,handler,state;} frame;
    volatile u8 scratch[0x204];
    u32 owner,allocation,result,output,sequence=0;
    frame.state=0xffffffffu;frame.handler=0x00954ebcu;frame.prev=predecessor;
    CM_FS(u32,0)=(u32)&frame;
    owner=D(self+0x13u);
    W(self+8u)=0x3au;
    allocation=(u32)cm_fn_00944e46(0xe8u);
    D(self+4u)=allocation;
    if(allocation==0){
        u32 first=(u32)scratch+4u,second=(u32)scratch+0x104u,message;
        cm_fn_00944cff(0x0098b0ccu,0,0,second,first);
        message=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u,0x009870e8u,message,second,first,0xa3u);
        cm_fn_005e8290(0x009870e0u,0x00ae24d0u,0);
        D(0x00b67a34u)=0;
    }else{
        /* Row 0, original constructor call 0x43d42c. */
        allocation=(u32)cm_fn_00944cf1(0x25du);
        frame.state=0x0u;
        if(allocation!=0){
            u32 value0=D(0x9cf9ecu); /* 0043d408 */
            u32 value1=D(0x9cff8cu); /* 0043d40e */
            sequence=1;
            result=cm_fn_008af520(allocation,owner,0,value1,0x7d0,0,value0,sequence,0,argument,sequence);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x0u)=result;
        /* Row 1, original constructor call 0x43d48a. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x1u;
        if(allocation!=0){
            u32 value0=D(0x9cf9ecu); /* 0043d462 */
            u32 value1=D(0x9cff88u); /* 0043d472 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1770,0xdac,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x4u)=result;
        /* Row 2, original constructor call 0x43d4e9. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x2u;
        if(allocation!=0){
            u32 value0=D(0x9cf9ecu); /* 0043d4c1 */
            u32 value1=D(0x9cff94u); /* 0043d4d1 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1b58,0xbb8,value0,0,0,1,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x8u)=result;
        /* Row 3, original constructor call 0x43d548. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x3u;
        if(allocation!=0){
            u32 value0=D(0x9cf9ecu); /* 0043d520 */
            u32 value1=D(0x9cff98u); /* 0043d530 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1388,0x9c4,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0xcu)=result;
        /* Row 4, original constructor call 0x43d5a8. */
        allocation=(u32)cm_fn_00944cf1(0x25du);
        frame.state=0x4u;
        if(allocation!=0){
            u32 value0=D(0x9cf9ecu); /* 0043d582 */
            u32 value1=D(0x9cff90u); /* 0043d590 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_008af520(allocation,owner,index,value1,0x1388,0x7d0,value0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x10u)=result;
        /* Row 5, original constructor call 0x43d605. */
        allocation=(u32)cm_fn_00944cf1(0x25du);
        frame.state=0x5u;
        if(allocation!=0){
            u32 value0=D(0x9cf9e0u); /* 0043d5e2 */
            u32 value1=D(0x9cffa0u); /* 0043d5f0 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_008af520(allocation,owner,index,value1,0x3e8,0,value0,1,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x14u)=result;
        /* Row 6, original constructor call 0x43d664. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x6u;
        if(allocation!=0){
            u32 value0=D(0x9cf9e0u); /* 0043d63c */
            u32 value1=D(0x9cff9cu); /* 0043d64c */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0xbb8,0x1f4,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x18u)=result;
        /* Row 7, original constructor call 0x43d6c3. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x7u;
        if(allocation!=0){
            u32 value0=D(0x9cf9e0u); /* 0043d69b */
            u32 value1=D(0x9cffa8u); /* 0043d6ab */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0xdac,0x1f4,value0,0,0,1,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x1cu)=result;
        /* Row 8, original constructor call 0x43d722. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x8u;
        if(allocation!=0){
            u32 value0=D(0x9cf9e0u); /* 0043d6fa */
            u32 value1=D(0x9cffacu); /* 0043d70a */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x9c4,0x3e8,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x20u)=result;
        /* Row 9, original constructor call 0x43d77f. */
        allocation=(u32)cm_fn_00944cf1(0x25du);
        frame.state=0x9u;
        if(allocation!=0){
            u32 value0=D(0x9cf9e0u); /* 0043d75c */
            u32 value1=D(0x9cffa4u); /* 0043d76a */
            u32 index=sequence;
            sequence++;
            result=cm_fn_008af520(allocation,owner,index,value1,0x9c4,0x64,value0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x24u)=result;
        /* Row 10, original constructor call 0x43d7dc. */
        allocation=(u32)cm_fn_00944cf1(0x25du);
        frame.state=0xau;
        if(allocation!=0){
            u32 value0=D(0x9cf9c0u); /* 0043d7b9 */
            u32 value1=D(0x9cffccu); /* 0043d7c7 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_008af520(allocation,owner,index,value1,0x1f4,0,value0,1,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x28u)=result;
        /* Row 11, original constructor call 0x43d83b. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0xbu;
        if(allocation!=0){
            u32 value0=D(0x9cf9c0u); /* 0043d813 */
            u32 value1=D(0x9cffc8u); /* 0043d823 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1388,0x5dc,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x2cu)=result;
        /* Row 12, original constructor call 0x43d89a. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0xcu;
        if(allocation!=0){
            u32 value0=D(0x9cf9c0u); /* 0043d872 */
            u32 value1=D(0x9cffd0u); /* 0043d882 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1194,0x3e8,value0,0,0,1,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x30u)=result;
        /* Row 13, original constructor call 0x43d8f6. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0xdu;
        if(allocation!=0){
            u32 value0=D(0x9cf9c0u); /* 0043d8d1 */
            u32 value1=D(0x9cffc4u); /* 0043d8e1 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0xbb8,0,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x34u)=result;
        /* Row 14, original constructor call 0x43d953. */
        allocation=(u32)cm_fn_00944cf1(0x25du);
        frame.state=0xeu;
        if(allocation!=0){
            u32 value0=D(0x9cf998u); /* 0043d930 */
            u32 value1=D(0x9cffdcu); /* 0043d93e */
            u32 index=sequence;
            sequence++;
            result=cm_fn_008af520(allocation,owner,index,value1,0x1f4,0,value0,1,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x38u)=result;
        /* Row 15, original constructor call 0x43d9b2. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0xfu;
        if(allocation!=0){
            u32 value0=D(0x9cf998u); /* 0043d98a */
            u32 value1=D(0x9cffd8u); /* 0043d99a */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1388,0x5dc,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x3cu)=result;
        /* Row 16, original constructor call 0x43da11. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x10u;
        if(allocation!=0){
            u32 value0=D(0x9cf998u); /* 0043d9e9 */
            u32 value1=D(0x9cffe0u); /* 0043d9f9 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1194,0x3e8,value0,0,0,1,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x40u)=result;
        /* Row 17, original constructor call 0x43da6d. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x11u;
        if(allocation!=0){
            u32 value0=D(0x9cf998u); /* 0043da48 */
            u32 value1=D(0x9cffd4u); /* 0043da58 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0xbb8,0,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x44u)=result;
        /* Row 18, original constructor call 0x43daca. */
        allocation=(u32)cm_fn_00944cf1(0x25du);
        frame.state=0x12u;
        if(allocation!=0){
            u32 value0=D(0x9cf9c8u); /* 0043daa7 */
            u32 value1=D(0x9cffecu); /* 0043dab5 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_008af520(allocation,owner,index,value1,0x1f4,0,value0,1,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x48u)=result;
        /* Row 19, original constructor call 0x43db29. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x13u;
        if(allocation!=0){
            u32 value0=D(0x9cf9c8u); /* 0043db01 */
            u32 value1=D(0x9cffe8u); /* 0043db11 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1388,0x5dc,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x4cu)=result;
        /* Row 20, original constructor call 0x43db88. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x14u;
        if(allocation!=0){
            u32 value0=D(0x9cf9c8u); /* 0043db60 */
            u32 value1=D(0x9cfff0u); /* 0043db70 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1194,0x3e8,value0,0,0,1,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x50u)=result;
        /* Row 21, original constructor call 0x43dbe4. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x15u;
        if(allocation!=0){
            u32 value0=D(0x9cf9c8u); /* 0043dbbf */
            u32 value1=D(0x9cffe4u); /* 0043dbcf */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0xbb8,0,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x54u)=result;
        /* Row 22, original constructor call 0x43dc41. */
        allocation=(u32)cm_fn_00944cf1(0x25du);
        frame.state=0x16u;
        if(allocation!=0){
            u32 value0=D(0x9cf9b4u); /* 0043dc1e */
            u32 value1=D(0x9cfffcu); /* 0043dc2c */
            u32 index=sequence;
            sequence++;
            result=cm_fn_008af520(allocation,owner,index,value1,0x1f4,0,value0,1,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x58u)=result;
        /* Row 23, original constructor call 0x43dca0. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x17u;
        if(allocation!=0){
            u32 value0=D(0x9cf9b4u); /* 0043dc78 */
            u32 value1=D(0x9cfff8u); /* 0043dc88 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1388,0x5dc,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x5cu)=result;
        /* Row 24, original constructor call 0x43dcff. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x18u;
        if(allocation!=0){
            u32 value0=D(0x9cf9b4u); /* 0043dcd7 */
            u32 value1=D(0x9d0000u); /* 0043dce7 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1194,0x3e8,value0,0,0,1,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x60u)=result;
        /* Row 25, original constructor call 0x43dd5b. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x19u;
        if(allocation!=0){
            u32 value0=D(0x9cf9b4u); /* 0043dd36 */
            u32 value1=D(0x9cfff4u); /* 0043dd46 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0xbb8,0,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x64u)=result;
        /* Row 26, original constructor call 0x43ddb8. */
        allocation=(u32)cm_fn_00944cf1(0x25du);
        frame.state=0x1au;
        if(allocation!=0){
            u32 value0=D(0x9cf9d4u); /* 0043dd95 */
            u32 value1=D(0x9d000cu); /* 0043dda3 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_008af520(allocation,owner,index,value1,0x1f4,0,value0,1,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x68u)=result;
        /* Row 27, original constructor call 0x43de17. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x1bu;
        if(allocation!=0){
            u32 value0=D(0x9cf9d4u); /* 0043ddef */
            u32 value1=D(0x9d0008u); /* 0043ddff */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1388,0x5dc,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x6cu)=result;
        /* Row 28, original constructor call 0x43de76. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x1cu;
        if(allocation!=0){
            u32 value0=D(0x9cf9d4u); /* 0043de4e */
            u32 value1=D(0x9d0010u); /* 0043de5e */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1194,0x3e8,value0,0,0,1,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x70u)=result;
        /* Row 29, original constructor call 0x43ded2. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x1du;
        if(allocation!=0){
            u32 value0=D(0x9cf9d4u); /* 0043dead */
            u32 value1=D(0x9d0004u); /* 0043debd */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0xbb8,0,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x74u)=result;
        /* Row 30, original constructor call 0x43df2f. */
        allocation=(u32)cm_fn_00944cf1(0x25du);
        frame.state=0x1eu;
        if(allocation!=0){
            u32 value0=D(0x9cf994u); /* 0043df0c */
            u32 value1=D(0x9d001cu); /* 0043df1a */
            u32 index=sequence;
            sequence++;
            result=cm_fn_008af520(allocation,owner,index,value1,0x1f4,0,value0,1,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x78u)=result;
        /* Row 31, original constructor call 0x43df8e. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x1fu;
        if(allocation!=0){
            u32 value0=D(0x9cf994u); /* 0043df66 */
            u32 value1=D(0x9d0018u); /* 0043df76 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1388,0x5dc,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x7cu)=result;
        /* Row 32, original constructor call 0x43dfed. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x20u;
        if(allocation!=0){
            u32 value0=D(0x9cf994u); /* 0043dfc5 */
            u32 value1=D(0x9d0020u); /* 0043dfd5 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1194,0x3e8,value0,0,0,1,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x80u)=result;
        /* Row 33, original constructor call 0x43e04c. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x21u;
        if(allocation!=0){
            u32 value0=D(0x9cf994u); /* 0043e027 */
            u32 value1=D(0x9d0014u); /* 0043e037 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0xbb8,0,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x84u)=result;
        /* Row 34, original constructor call 0x43e0ac. */
        allocation=(u32)cm_fn_00944cf1(0x25du);
        frame.state=0x22u;
        if(allocation!=0){
            u32 value0=D(0x9cf9a4u); /* 0043e089 */
            u32 value1=D(0x9d002cu); /* 0043e097 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_008af520(allocation,owner,index,value1,0x1f4,0,value0,1,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x88u)=result;
        /* Row 35, original constructor call 0x43e10e. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x23u;
        if(allocation!=0){
            u32 value0=D(0x9cf9a4u); /* 0043e0e6 */
            u32 value1=D(0x9d0028u); /* 0043e0f6 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1388,0x5dc,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x8cu)=result;
        /* Row 36, original constructor call 0x43e170. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x24u;
        if(allocation!=0){
            u32 value0=D(0x9cf9a4u); /* 0043e148 */
            u32 value1=D(0x9d0030u); /* 0043e158 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1194,0x3e8,value0,0,0,1,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x90u)=result;
        /* Row 37, original constructor call 0x43e1cf. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x25u;
        if(allocation!=0){
            u32 value0=D(0x9cf9a4u); /* 0043e1aa */
            u32 value1=D(0x9d0024u); /* 0043e1ba */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0xbb8,0,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x94u)=result;
        /* Row 38, original constructor call 0x43e22f. */
        allocation=(u32)cm_fn_00944cf1(0x25du);
        frame.state=0x26u;
        if(allocation!=0){
            u32 value0=D(0x9cf9d0u); /* 0043e20c */
            u32 value1=D(0x9d003cu); /* 0043e21a */
            u32 index=sequence;
            sequence++;
            result=cm_fn_008af520(allocation,owner,index,value1,0x1f4,0,value0,1,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x98u)=result;
        /* Row 39, original constructor call 0x43e291. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x27u;
        if(allocation!=0){
            u32 value0=D(0x9cf9d0u); /* 0043e269 */
            u32 value1=D(0x9d0038u); /* 0043e279 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1388,0x5dc,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0x9cu)=result;
        /* Row 40, original constructor call 0x43e2f3. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x28u;
        if(allocation!=0){
            u32 value0=D(0x9cf9d0u); /* 0043e2cb */
            u32 value1=D(0x9d0040u); /* 0043e2db */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1194,0x3e8,value0,0,0,1,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0xa0u)=result;
        /* Row 41, original constructor call 0x43e352. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x29u;
        if(allocation!=0){
            u32 value0=D(0x9cf9d0u); /* 0043e32d */
            u32 value1=D(0x9d0034u); /* 0043e33d */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0xbb8,0,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0xa4u)=result;
        /* Row 42, original constructor call 0x43e3b2. */
        allocation=(u32)cm_fn_00944cf1(0x25du);
        frame.state=0x2au;
        if(allocation!=0){
            u32 value0=D(0x9cf9bcu); /* 0043e38f */
            u32 value1=D(0x9d004cu); /* 0043e39d */
            u32 index=sequence;
            sequence++;
            result=cm_fn_008af520(allocation,owner,index,value1,0x1f4,0,value0,1,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0xa8u)=result;
        /* Row 43, original constructor call 0x43e414. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x2bu;
        if(allocation!=0){
            u32 value0=D(0x9cf9bcu); /* 0043e3ec */
            u32 value1=D(0x9d0048u); /* 0043e3fc */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1388,0x5dc,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0xacu)=result;
        /* Row 44, original constructor call 0x43e476. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x2cu;
        if(allocation!=0){
            u32 value0=D(0x9cf9bcu); /* 0043e44e */
            u32 value1=D(0x9d0050u); /* 0043e45e */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1194,0x3e8,value0,0,0,1,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0xb0u)=result;
        /* Row 45, original constructor call 0x43e4d5. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x2du;
        if(allocation!=0){
            u32 value0=D(0x9cf9bcu); /* 0043e4b0 */
            u32 value1=D(0x9d0044u); /* 0043e4c0 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0xbb8,0,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0xb4u)=result;
        /* Row 46, original constructor call 0x43e535. */
        allocation=(u32)cm_fn_00944cf1(0x25du);
        frame.state=0x2eu;
        if(allocation!=0){
            u32 value0=D(0x9cf9dcu); /* 0043e512 */
            u32 value1=D(0x9d005cu); /* 0043e520 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_008af520(allocation,owner,index,value1,0x1f4,0,value0,1,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0xb8u)=result;
        /* Row 47, original constructor call 0x43e597. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x2fu;
        if(allocation!=0){
            u32 value0=D(0x9cf9dcu); /* 0043e56f */
            u32 value1=D(0x9d0058u); /* 0043e57f */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1388,0x5dc,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0xbcu)=result;
        /* Row 48, original constructor call 0x43e5f9. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x30u;
        if(allocation!=0){
            u32 value0=D(0x9cf9dcu); /* 0043e5d1 */
            u32 value1=D(0x9d0060u); /* 0043e5e1 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1194,0x3e8,value0,0,0,1,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0xc0u)=result;
        /* Row 49, original constructor call 0x43e658. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x31u;
        if(allocation!=0){
            u32 value0=D(0x9cf9dcu); /* 0043e633 */
            u32 value1=D(0x9d0054u); /* 0043e643 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0xbb8,0,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0xc4u)=result;
        /* Row 50, original constructor call 0x43e6b8. */
        allocation=(u32)cm_fn_00944cf1(0x25du);
        frame.state=0x32u;
        if(allocation!=0){
            u32 value0=D(0x9cf9b0u); /* 0043e695 */
            u32 value1=D(0x9d006cu); /* 0043e6a3 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_008af520(allocation,owner,index,value1,0x1f4,0,value0,1,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0xc8u)=result;
        /* Row 51, original constructor call 0x43e71a. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x33u;
        if(allocation!=0){
            u32 value0=D(0x9cf9b0u); /* 0043e6f2 */
            u32 value1=D(0x9d0068u); /* 0043e702 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1388,0x5dc,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0xccu)=result;
        /* Row 52, original constructor call 0x43e77c. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x34u;
        if(allocation!=0){
            u32 value0=D(0x9cf9b0u); /* 0043e754 */
            u32 value1=D(0x9d0070u); /* 0043e764 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1194,0x3e8,value0,0,0,1,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0xd0u)=result;
        /* Row 53, original constructor call 0x43e7db. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x35u;
        if(allocation!=0){
            u32 value0=D(0x9cf9b0u); /* 0043e7b6 */
            u32 value1=D(0x9d0064u); /* 0043e7c6 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0xbb8,0,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0xd4u)=result;
        /* Row 54, original constructor call 0x43e83b. */
        allocation=(u32)cm_fn_00944cf1(0x25du);
        frame.state=0x36u;
        if(allocation!=0){
            u32 value0=D(0x9cf9e4u); /* 0043e818 */
            u32 value1=D(0x9d007cu); /* 0043e826 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_008af520(allocation,owner,index,value1,0x1f4,0,value0,1,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0xd8u)=result;
        /* Row 55, original constructor call 0x43e89d. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x37u;
        if(allocation!=0){
            u32 value0=D(0x9cf9e4u); /* 0043e875 */
            u32 value1=D(0x9d0078u); /* 0043e885 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1388,0x5dc,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0xdcu)=result;
        /* Row 56, original constructor call 0x43e8ff. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x38u;
        if(allocation!=0){
            u32 value0=D(0x9cf9e4u); /* 0043e8d7 */
            u32 value1=D(0x9d0080u); /* 0043e8e7 */
            u32 index=sequence;
            sequence++;
            result=cm_fn_0092fe80(allocation,owner,index,value1,0x1194,0x3e8,value0,0,0,1,argument,1);
        }else result=0;
        output=D(self+4u);
        frame.state=0xffffffffu;
        D(output+0xe0u)=result;
        /* Row 57, original constructor call 0x43e95b. */
        allocation=(u32)cm_fn_00944cf1(0x2bu);
        frame.state=0x39u;
        if(allocation!=0){
            u32 value0=D(0x9cf9e4u); /* 0043e939 */
            u32 value1=D(0x9d0074u); /* 0043e93f */
            result=cm_fn_0092fe80(allocation,owner,sequence,value1,0xbb8,0,value0,0,0,0,argument,1);
        }else result=0;
        output=D(self+4u);
        D(output+0xe4u)=result;
    }
    predecessor=frame.prev;
    CM_FS(u32,0)=predecessor;
}
