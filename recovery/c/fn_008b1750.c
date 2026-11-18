#include "cmre.h"
/* Original instruction authority: preferred-image VAs 008b1750..008b222a,
   CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Receivers follow the ECX loads at each call; ret 8 at 008b222a.
   008b1852 calls the no-argument string helper with pending printf args.
   008b2069..206f pushes kind then player, retaining ECX=self, EDX.
 */
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32,volatile u32,u32);
extern u32 CM_CDECL cm_fn_005885c0(u32);
extern void CM_CDECL cm_fn_005e8290(const char *,const char *,int);
extern u8 CM_THISCALL cm_fn_0062c400(u32,u32);
extern u32 CM_THISCALL cm_fn_0062c660(u32,u32,u32);
extern void CM_THISCALL cm_fn_00867700(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008b2be0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008b39f0(u32,u32);
extern u32 CM_THISCALL cm_fn_008b41d0(u32,u32,volatile u32);
extern void CM_THISCALL cm_fn_008b4450(u32,u32,u32);
extern unsigned long long CM_FASTCALL cm_fn_008b4770(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008b4b20(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008b4f50(u32,u32);
extern int CM_THISCALL cm_fn_008b4f60(u8 *);
/* Each RNG call (e.g. 008b1ee0..1ee7) pushes one DWORD, caller pops 4. */
extern unsigned long long CM_CDECL cm_fn_0090cfc0(u32);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern unsigned long long CM_CDECL cm_fn_00945333(volatile u32,volatile u32,volatile u32,volatile u32);
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define W(a) CM_GLOBAL(volatile u16,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
#define SB(a) ((s8)B(a))

static void report(u8 *scratch,u32 a,u32 b,u32 line)
{
    cm_fn_00944cff(0xa8ccd0,0,0,scratch+a,scratch+b);
    cm_fn_00944c9f(0xae24d0,0x9870e8,(u32)cm_fn_0090d130(),
                      (u32)(scratch+a),(u32)(scratch+b),line);
    cm_fn_005e8290((const char *)0x9870e0,(const char *)0xae24d0,0);
    D(0xb67a34)=0;
}
static u32 club(u32 p)
{
    u32 a=cm_fn_004cf5a0(D(0xae1050),p);
    D(0xae1080)=a;
    if(a) { u32 id=D(a+4); return D(0xae23b4)+id*0x245u; }
    return 0;
}
static u32 row(u32 self,u32 date,s8 column,u8 *scratch,u32 a,u32 b)
{
    s32 d=(s32)(D(0xae2364)-D(date)-1);
    if(d<0 || d>=16) { report(scratch,a,b,0x68f); return 0; }
    if(column<0 || column>=10) column=1;
    return self+((u32)column+7u+(u32)d*10u)*155u;
}
void CM_THISCALL cm_fn_008b1750(u32 self,u32 p,u32 mode)
{
    u8 scratch[0x434];
    u32 entry,model,c,a,date,output,count,other=0;
    u16 days;
    s8 six=-1,one=-1;
    int removed=0;
    u32 eax=0,edx=0;
    unsigned long long rr;
    if(!p) { report(scratch,0x34,0x134,0x18a); return; }
    if((s32)D(p)>=(s32)D(0xae2364)) return;
    a=D(p+0x61);
    if(!a) { report(scratch,0x134,0x34,0x195); return; }
    D((u32)(scratch+0x20))=0;
    B((u32)(scratch+0x24))=0;
    c=D(a);
    entry=D(self+0x651d)+c*6u;
    model=cm_fn_008b39f0(self,p);
    if(!model) { report(scratch,0x134,0x34,0x1a3); return; }
    if(!(B(entry+5)&0x20)) {
        a=club(p);
        if(a) {
            a=club(p);
            if(D(a+0xcf)) {
                a=club(p);
                date=D(a+0xcf);
                if((s32)D(date)>=(s32)(D(0xae2364)-16u)) {
                    a=club(p); date=D(a+0xcf);
                    B(entry+5)=B(entry+5)|0x20;
                    for(s8 i=0;i<10;i++) {
                        a=row(self,date,i,scratch,0x134,0x34);
                        if(B(a+0x15)==6) six=i;
                        else {
                            a=row(self,date,i,scratch,0x334,0x234);
                            if(B(a+0x15)==1) one=i;
                        }
                    }
                    if(SB(D(p+0x61)+15)>=15 && six!=-1) B(entry)=(u8)six;
                    else if(one!=-1) B(entry)=(u8)one;
                    else B(entry)=1;
                    a=row(self,date,SB(entry),scratch,0x234,0x334);
                    eax=(u32)cm_fn_008b4f60((u8 *)a);
                    cm_fn_0062c660(D(0xb63d28),p,eax);
                }
            }
        }
    }
    days=(u16)(W(self+0x688d)-W(entry+1));
    if(!days && !(u16)mode) return;
    if(B(entry+5)&0x20) {
        int valid=0;
        a=club(p);
        if(a) {
            a=club(p);
            if(D(a+0xcf)) {
                a=club(p);
                a=D(a+0xcf);
                if((s32)D(a)>=(s32)(D(0xae2364)-16u)) valid=1;
            }
        }
        if(!valid) {
            B(entry+5)=B(entry+5)&0xdf;
            cm_fn_0062c660(D(0xb63d28),p,50);
            a=D(p+0x61);
            s8 age=SB(a+15);
            c=D(a);
            B(D(self+0x651d)+c*6u)=age>=15?6:1;
            count=cm_fn_008b4b20(self,p,(u32)(scratch+0x18));
            for(s8 i=0;(s32)i<(s32)count;i=(s8)(i+1))
                cm_fn_008b4450(self,D((u32)(scratch+0x18))+(u32)(s32)i*8u,0);
        }
    }
    if(days>30) days=30;
    W(entry+1)=W(self+0x688d);
    count=cm_fn_008b4b20(self,p,(u32)(scratch+0x18));
    output=D((u32)(scratch+0x18));
    u8 flags=B(entry+5);
    if((flags&0x10) && !count) B(entry+5)=flags&0xef;
    B((u32)(scratch+0x10))=0;
    for(s8 i=0;i<5;i++) {
        u32 selector=D((u32)(scratch+0x10));
        eax=cm_fn_008b4f50(model,selector);
        s16 strength=(s8)eax*2+60;
        a=club(p);
        eax=cm_fn_008b41d0(self,a,selector);
        strength=(s16)(strength+((s16)eax-100)/5);
        s16 bp=strength<0?0:strength>200?200:strength;
        s8 low=(s8)(SB(p+0x5b)/3-6), high=SB(p+0x58)/3;
        u8 v=B(entry+3+(u32)i/2);
        s8 initial=(i&1)?(s8)v>>4:(s8)(v<<4)>>4;
        u32 ch=(u32)(scratch+0x20+i);
        for(u32 d=days;d;d--) {
            s8 sum=(s8)(initial+SB(ch));
            edx=D(p+0x61);
            if(SB(edx+15)>18) {
                rr=cm_fn_0090cfc0(i==4?2:4);
                eax=(u32)rr; edx=(u32)(rr>>32);
                if(eax) continue;
            }
            if(bp>100) {
                eax=(u32)(s32)bp;
                rr=cm_fn_0090cfc0(210-eax);
                eax=(u32)rr; edx=(u32)(rr>>32);
                if((s32)eax<20 && sum<high) {
                    eax=sum<0?-(s32)sum:sum;
                    edx=eax*2+14;
                    rr=cm_fn_0090cfc0(edx);
                    eax=(u32)rr; edx=(u32)(rr>>32);
                    if((s32)eax<10) B(ch)=B(ch)+1;
                    continue;
                }
            }
            if(bp<110) {
                eax=(u32)((s32)bp+60);
                rr=cm_fn_0090cfc0(eax);
                eax=(u32)rr; edx=(u32)(rr>>32);
                if((s32)eax<20 && sum>-7 && sum>low) {
                    eax=sum<0?-(s32)sum:sum;
                    edx=sum<0?0xffffffffu:0;
                    rr=cm_fn_0090cfc0(eax*2+14);
                    eax=(u32)rr; edx=(u32)(rr>>32);
                    if((s32)eax<10) B(ch)=B(ch)-1;
                }
            }
        }
        B((u32)(scratch+0x10))=(u8)(i+1);
    }
    if((u16)mode) days=1;
    if(days) other=cm_fn_004cf5f0(D(0xae1050),p,D(p+0x39));
    /* 008b1ff1 initializes ESI; 008b218a advances it independently of
       the signed byte counter (008b2188, 008b2191..2196), even on wrap.
       Instruction authority and pinned binary SHA-256 are stated above. */
    u32 q=output+7u;
    for(s8 i=0;(s32)i<(s32)count;i=(s8)(i+1),q+=8u) {
        for(u32 d=days;d;d--) {
            s8 v=SB(q);
            if(((u16)mode && W(q-3)==(u16)mode) || v>0) {
                eax=(u32)(s32)v;
                rr=cm_fn_0090cfc0(eax);
                eax=(u32)rr; edx=(u32)(rr>>32);
                if((s32)eax<5) {
                    s8 next=(s8)(B(q-1)+2);
                    B(q-1)=(u8)next;
                    B(q-1)=next>20?20:(u8)next;
                }
                if(other && !(D(other+0x45)&0x8000)) {
                    u32 kind=W(q-3);
                    rr=cm_fn_008b4770(self,edx,p,kind);
                    s8 bl=(s8)rr;
                    v=SB(q);
                    int go=0;
                    if(v>50) {
                        edx=(u32)(s32)v; eax=100-edx;
                        rr=cm_fn_0090cfc0(eax);
                        eax=(u32)rr; edx=(u32)(rr>>32);
                        if((s32)eax<10) go=1;
                    }
                    if(!go && bl<12) {
                        rr=cm_fn_0090cfc0((u32)(s32)bl);
                        eax=(u32)rr; edx=(u32)(rr>>32);
                        if((s32)eax<2) go=1;
                    }
                    v=SB(p+0x5b);
                    if(go && v<15) {
                        edx=(u32)(s32)v;
                        rr=cm_fn_0090cfc0(edx);
                        eax=(u32)rr; edx=(u32)(rr>>32);
                        if((s32)eax<10 && D(0xb63d28) &&
                           !cm_fn_0062c400(D(0xb63d28),p)) {
                            cm_fn_00867700(other,0x8000,0);
                            s32 n=SB(other+0x44)-5;
                            B(other+0x44)=n>100?100:n< -100?(u8)-100:(u8)n;
                            cm_fn_005885c0(p);
                            other=0;
                        }
                    }
                }
            } else if(v<0) {
                eax=(u32)-(s32)v; edx=0xffffffffu;
                rr=cm_fn_0090cfc0(eax);
                eax=(u32)rr; edx=(u32)(rr>>32);
                if((s32)eax<5) {
                    s8 n=(s8)(B(q-1)-2);
                    B(q-1)=(u8)n;
                    if(n<1) { B(q)=0; B(q-1)=1; removed=1; }
                }
            }
        }
    }
    cm_fn_008b2be0(self,p,(u32)(scratch+0x20));
    if(removed) {
        u32 n=D(self+0x6525),old=n;
        for(u32 i=0;(s32)i<(s32)n;i++) {
            a=D(self+0x6521);
            u32 dest=a+i*8u;
            if(!B(dest+7)) {
                D(dest)=D(a+n*8u-8);
                D(dest+4)=D(a+n*8u-4);
                D(self+0x6525)=D(self+0x6525)-1;
            }
            n=D(self+0x6525);
        }
        n=D(self+0x6525);
        if(old!=n) {
            a=D(self+0x6521);
            cm_fn_00945333(a,n,8,0x8b4e00);
        }
    }
}
