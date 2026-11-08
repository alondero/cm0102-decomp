/* Preferred-image VA 00538bc0, base 00400000; 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Cdecl, one argument. Indexed callers discard EAX: direct kills at 533984,
   538a78/538b2d,82cc74,888ab9,88a476; 5d5772 through 547f90/90cfc0;
   7acfa1 forwards through 7abac0 to kills 7a665e/7a6680. */
#include "cmre.h"
extern void CM_CDECL cm_fn_0053a610(const volatile s8 *,const volatile s8 *,volatile s16 *,volatile u8 *,volatile u8 *,volatile u8 *);
extern void CM_CDECL cm_fn_0053aa10(volatile u8 *,volatile u8 *,volatile u8 *);
extern u32 CM_THISCALL cm_fn_008b3640(u32,u32,u32);
#define CONTEXT CM_GLOBAL(volatile u32,0x00de1f44)
__attribute__((target("no-sse3")))
void CM_CDECL cm_fn_00538bc0(u32 player)
{
    /* L = ESP after SUB ESP,274h, before four saved registers.
       Shared native addresses: metadata L+2..23, triples L+24..e0,
       DWORD flags L+e4..273. No scratch initialization is added. */
    volatile u8 scratch[0x274];
#define COUNT (*(volatile s8 *)(scratch+2))
#define TOTAL (*(volatile s8 *)(scratch+3))
#define MAXIMUM (*(volatile s8 *)(scratch+4))
#define PASS (*(volatile s8 *)(scratch+5))
#define START (*(volatile s8 *)(scratch+6))
#define SUM (*(volatile u32 *)(scratch+8))
#define NUMBER (*(volatile u32 *)(scratch+12))
#define INCLUDED (*(volatile u32 *)(scratch+16))
#define CONVERT (*(volatile s32 *)(scratch+24))
#define THRESHOLD (*(volatile u32 *)(scratch+28))
#define RECORD (*(volatile u32 *)(scratch+32))
#define ROW(i) (*(volatile s16 *)(scratch+0x24+(s32)(i)*3))
#define FLAG(i) (*(volatile u32 *)(scratch+0xe4+(s32)(i)*4))
    u32 context,record,selected;
    s8 index,total,maximum;
    s16 value,shifted;
    s32 absolute,average;
    volatile s32 quotient;
    volatile long double live;
    volatile s32 converted;
    if(player==0)return;
    record=*(volatile u32 *)(player+0x61u);
    RECORD=record;
    if(record==0)return;
    context=CONTEXT;
    if(context)cm_fn_008b3640(context,player,0);
    cm_fn_0053a610((const volatile s8 *)record,(const volatile s8 *)player,
        (volatile s16 *)(scratch+0x24),scratch+7,scratch+0x17,scratch+3);
    PASS=0;
    START=(s8)(u8)(*(volatile u8 *)(scratch+7)+2u);
    do {
        NUMBER=0;SUM=0;THRESHOLD=30;
        do {
            index=START;total=TOTAL;
            INCLUDED=0;COUNT=0;MAXIMUM=-1;
            if(index<total) {
                do {
                    value=ROW(index);
                    absolute=value<0?-(s32)value:(s32)value;
                    if(absolute<=(s32)(s16)THRESHOLD) {
                        u32 sum=INCLUDED;
                        s8 count=COUNT;
                        sum+=(u32)(s32)value;
                        count=(s8)(u8)((u8)count+1u);
                        INCLUDED=sum;COUNT=count;FLAG(index)=1;
                    } else {
                        maximum=MAXIMUM;
                        FLAG(index)=0;
                        if(maximum==-1 || value>ROW(maximum))MAXIMUM=index;
                    }
                    total=TOTAL;
                    index=(s8)(u8)((u8)index+1u);
                } while(index<total);
                if(COUNT>=12)break;
            }
            THRESHOLD=THRESHOLD+5u;
        } while(COUNT<12);
        maximum=MAXIMUM;
        if(maximum>-1)FLAG(maximum)=1;
        selected=INCLUDED;
        index=START;
        quotient=(s32)selected/(s32)COUNT;
        average=(s32)(0xffffffecu-(u32)quotient);
        total=TOTAL;
        if(index<total) {
            do {
                if(index!=MAXIMUM) {
                    if(FLAG(index)==1) {
                        value=ROW(index);
                        shifted=(s16)(u16)((u16)value+(u16)average);
                        ROW(index)=shifted;
                        if(shifted<-125)ROW(index)=-125;
                        else if(shifted>125)ROW(index)=125;
                        else ROW(index)=shifted;
                    } else if(PASS==0 && (value=ROW(index),value>0) &&
                              *(volatile s8 *)(RECORD+0xfu)<15) {
                        if(index>=17 && index<=18) {
                            NUMBER=NUMBER+1u;
                            CONVERT=(s32)value;
                            live=(long double)CONVERT;
                            live*=CM_GLOBAL(volatile double,0x00968f98);
                            converted=(s32)(u32)(long long)live; /* __ftol 00538dad */
                            SUM=SUM+(u32)converted;
                        } else {
                            u32 number=NUMBER;
                            u32 sum=SUM;
                            number+=1u;sum+=(u32)(s32)value;
                            NUMBER=number;SUM=sum;
                        }
                    } else FLAG(index)=1;
                    if(ROW(0)<15 && *(volatile s8 *)(scratch+0x26+(s32)index*3)==1)
                        ROW(index)=-120;
                }
                total=TOTAL;
                index=(s8)(u8)((u8)index+1u);
            } while(index<total);
        }
        index=START;
        if(index<total) {
            u32 sum=SUM,number=NUMBER;
            do {
                if((s32)number<=1 || (s32)sum<=85)break;
                if(FLAG(index)!=1) {
                    quotient=(s32)(sum-85u)/(s32)number;
                    ROW(index)=(s16)(u16)((u16)ROW(index)-(u16)quotient);
                    if(ROW(index)<0)ROW(index)=0;
                }
                total=TOTAL;
                index=(s8)(u8)((u8)index+1u);
            } while(index<total);
        }
        PASS=(s8)(u8)((u8)PASS+1u);
    } while(PASS<2);
    record=RECORD;
    cm_fn_0053aa10((volatile u8 *)record,(volatile u8 *)player,scratch+0x24);
    if(*(volatile s8 *)(record+0xfu)>=15) {
        s8 right=*(volatile s8 *)(record+0x3au);
        s8 left=*(volatile s8 *)(record+0x2au);
        average=((s32)left+(s32)right)/2;
        *(volatile u8 *)(record+0x2au)=(u8)((u8)left-(u8)average);
        *(volatile u8 *)(record+0x3au)=(u8)((u8)right-(u8)average);
    }
    context=CONTEXT;
    if(context)cm_fn_008b3640(context,player,1);
}
