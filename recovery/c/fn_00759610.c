/* Preferred-image VA 00759610, base 00400000; primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Native callees remain boundaries. L is ESP after SUB ESP,108h, before saves.
   Counter L+3, saved receiver L+4, event L+8, date/record L+e8.
   No initialization of unwritten scratch is implied. */
#include "cmre.h"
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_CDECL cm_fn_00600a50(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00735870(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0077bbe0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00785790(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007861e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007863f0(u32,u32,u32);
extern long double CM_THISCALL cm_fn_007c1e30(u32,u32,u32,u32,float);
extern u32 CM_CDECL cm_fn_007e4750(u32,u32,u32);
extern u32 CM_CDECL cm_fn_007e4de0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define NEXT() do { index=B(local+3); B(local+3)=(u8)(index+1u); } while(0)
#define PUT(v) cm_fn_00785790(local+8,index,(v))

u32 CM_THISCALL __attribute__((target("no-sse3")))
cm_fn_00759610(u32 self,u32 person,u32 other,u32 record)
{
    volatile struct { u32 prev,handler,state; } seh;
    u8 scratch[0x108];
    u32 local=(u32)scratch, eax, pointer, value, index, manager;
    long double extended;
    seh.state=0xffffffffu; seh.handler=0x0095d983;
    eax=CM_FS(u32,0); seh.prev=eax; CM_FS(u32,0)=(u32)&seh;
    D(local+4)=self; B(local+3)=0;
    if (!person) goto done;
    eax=D(person+0x1a); if (!eax) goto done;
    eax=D(person+0x39); if (!eax) goto done;
    pointer=D(eax+0xcf); if (!pointer) goto done;
    eax=D(person+0x61); if (!eax) goto done;
    if (!other) goto done;
    eax=D(other+0x39); if (!eax) goto done;
    pointer=D(eax+0xcf); if (!pointer) goto done;
    eax=D(other+0x61); if (!eax) goto done;
    cm_fn_0077bbe0(local+8,0xbe6,0);
    index=B(local+3); seh.state=0; B(local+3)=(u8)(index+1u);
    eax=D(person); PUT(eax);
    index=B(local+3); pointer=D(person+4); B(local+3)=(u8)(index+1u);
    eax=D(pointer+0x33); PUT(eax);
    index=B(local+3); pointer=D(person+8); B(local+3)=(u8)(index+1u);
    eax=D(pointer+0x33); PUT(eax);
    index=B(local+3); pointer=D(person+0xc); B(local+3)=(u8)(index+1u);
    eax=D(pointer+0x33); PUT(eax);
    index=B(local+3); value=D(other); B(local+3)=(u8)(index+1u); PUT(value);
    NEXT(); eax=D(other+4); value=D(eax+0x33); PUT(value);
    NEXT(); eax=D(other+8); value=D(eax+0x33); PUT(value);
    NEXT(); eax=D(other+0xc); value=D(eax+0x33); PUT(value);
    eax=cm_fn_0090cfc0(2); NEXT(); PUT(eax);
    eax=D(person+0x39); index=B(local+3); eax=D(eax);
    B(local+3)=(u8)(index+1u); PUT(eax);
    eax=D(other+0x39); value=eax ? D(eax) : 0xffffffffu;
    NEXT(); PUT(value);
    pointer=D(person+0x61); value=D(0x00b74350);
    extended=cm_fn_007c1e30(value,pointer,0xb,2,6.5f);
    /* Registered __ftol physical call VA 0075981d: preserve ST0's 80 bits,
       truncate to signed 64 bits, then use its low DWORD (EAX). */
    eax=(u32)(long long)extended;
    NEXT(); PUT(eax);
    pointer=D(person+0x39); index=B(local+3); manager=D(pointer+0xcf);
    B(local+3)=(u8)(index+1u); eax=D(manager); PUT(eax);
    NEXT(); pointer=D(manager+4); eax=D(pointer+0x33); PUT(eax);
    NEXT(); pointer=D(manager+8); eax=D(pointer+0x33); PUT(eax);
    NEXT(); pointer=D(manager+0xc); eax=D(pointer+0x33); PUT(eax);
    pointer=D(other+0x39); index=B(local+3); other=D(pointer+0xcf);
    B(local+3)=(u8)(index+1u); value=D(other); PUT(value);
    NEXT(); eax=D(other+4); value=D(eax+0x33); PUT(value);
    NEXT(); eax=D(other+8); value=D(eax+0x33); PUT(value);
    NEXT(); eax=D(other+0xc); value=D(eax+0x33); PUT(value);
    eax=D(person+0x39); other=0;
    eax=cm_fn_00600a50(eax,0,1,0,0);
    if (!eax) {
        pointer=D(person+0x39); value=D(pointer+0xcf);
        pointer=D(local+4);
        other=cm_fn_00735870(pointer,value,person,0x82);
    }
    NEXT(); PUT(other);
    cm_fn_0054a110(local+0xe8);
    pointer=D(person+0x39);
    *(volatile u8 *)&seh.state=1;
    cm_fn_007e4750(record,pointer,local+0xe8);
    cm_fn_007e4de0(local+0xe8,local+8,local+3);
    eax=D(person+0x61);
    if ((s16)W(eax+0xd)>0x128e) {
        pointer=D(person+0x1a); value=D(0x00b7431c);
        eax=cm_fn_007863f0(value,local+8,pointer);
    } else {
        eax=D(person+0x39); value=D(0x00b7431c);
        eax=cm_fn_007861e0(value,local+8,eax);
    }
done:
    CM_FS(u32,0)=seh.prev;
    return eax;
}
