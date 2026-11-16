#include "cmre.h"
#define M8(a) (*(volatile u8 *)(a))
#define M16(a) (*(volatile u16 *)(a))
#define M32(a) (*(volatile u32 *)(a))
extern void CM_FASTCALL cm_fn_0054a110(u16 *);
extern void CM_THISCALL cm_fn_00417c10(u32 *,u32 *,u16 *);
extern u32 CM_CDECL cm_fn_0053d980(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0058e340(u32 *,int *,int *,u32);
extern u32 CM_THISCALL cm_fn_0086f5b0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00544710(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32,u32,u32);
extern int CM_THISCALL cm_fn_0054cdb0(short *,u32,u32);
extern u32 CM_THISCALL cm_fn_00849ce0(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004cda70(u32,u32,u32,u32,s32,double,u8);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(const char *,const char *,int);

u32 CM_THISCALL cm_fn_00869480(u32 self,u32 staff,u32 club,s8 mode)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x47c];
    u32 a,b,c,d,p,q,i,idx;
    s32 v,w;
    s16 n,k,j;
    long double t;
    frame.prev=CM_FS(u32,0); frame.handler=0x962b96; frame.state=~0u;
    CM_FS(u32,0)=(u32)&frame;
    if (!staff || !club) {
        cm_fn_00944cff(0xa83b78,0,0,scratch+0x37c,scratch+0x27c);
        a=(u32)cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0,0x9870e8,a,scratch+0x37c,scratch+0x27c,0x301);
        a=cm_fn_005e8290((char *)0x9870e0,(char *)0xae24d0,0);
        CM_GLOBAL(u32,0xb67a34)=0;
        CM_FS(u32,0)=frame.prev; return a;
    }
    cm_fn_0054a110((u16 *)(scratch+0x10)); frame.state=0;
    cm_fn_00417c10((u32 *)(scratch+0x10),(u32 *)(scratch+8),(u16 *)0xae2c90);
    a=M32(club);
    M16(staff+0x80)+=500; b=M16(staff+0x80); c=M32(staff);
    M8((u32)scratch+0x1c)=8; M32((u32)scratch+0x18)=c; M32((u32)scratch+0x1d)=a;
    if ((s16)b>10000) M16(staff+0x80)=10000;
    v=(s32)M32(staff+0x7b);
    if (v<150000 && v<(s32)(M32(staff+0x73)+500))
        M32(staff+0x7b)=(u32)v+(s32)(s16)M16(club+0x69)*10;
    a=M32(staff+0xcf);
    if (a) {
        if (M32(a+0x69)) {
            b=M32(M32(0xadadfc)+M32(club)*4); p=club;
            if (b && (s32)M32(b+0x1c)>-1) {
                p=M32(b+0x1c)*107+M32(0xae23d0);
                if (!p) p=club;
            }
            if ((s8)M8(b+0x42)==1) {
                j=200; k=cm_fn_0053d980(a,M32(staff+0x53),1)?200:400;
            } else j=k=400;
            n=(s16)M16(p+0x69); b=M32(M32(staff+0xcf)+0x69);
            /* Original __ftol calls at VAs 008695e1, 0086963e, 0086969e:
               plain C long-double cast models the x87 input and truncation. */
            t=(long double)(n*400)*CM_GLOBAL(double,0x96ff88);
            M16(b+10)+=(u16)(long long)t;
            n=(s16)(M16(p+0x69)*400); b=M32(M32(staff+0xcf)+0x69); w=(s16)M16(b+10);
            M16(b+10)=n>w?n:w>10000?10000:w;
            n=(s16)M16(p+0x69); b=M32(M32(staff+0xcf)+0x69);
            t=(long double)(n*k)*CM_GLOBAL(double,0x96ff88);
            M16(b+8)+=(u16)(long long)t;
            n=(s16)(M16(p+0x69)*k); b=M32(M32(staff+0xcf)+0x69); w=(s16)M16(b+8);
            M16(b+8)=n>w?n:w>10000?10000:w;
            n=(s16)M16(p+0x69); b=M32(M32(staff+0xcf)+0x69);
            t=(long double)(n*j)*CM_GLOBAL(double,0x96ff88);
            M16(b+12)+=(u16)(long long)t;
            n=(s16)(M16(p+0x69)*j); b=M32(M32(staff+0xcf)+0x69); w=(s16)M16(b+12);
            M16(b+12)=n>w?n:w>10000?10000:w;
        }
        a=M32(staff+0xcf);
        if (a) {
            b=M32(self+12)+M32(a)*8;
            cm_fn_0058e340((u32 *)M32(self),(int *)b,(int *)(b+4),(u32)(scratch+0x10));
        }
        a=cm_fn_0086f5b0(self,M32(staff+0xcf),1);
        if (a) {
            v=(s32)((u32)((s32)(s16)M16(club+0x69)*(s32)mode)*2500)/100;
            M32(a+0xb6)+=v; M32(a+0xba)+=v; M32(a+0xca)+=v;
        }
    }
    a=M32(staff+0xbf);
    if (a) {
        b=M32(self+12)+M32(a)*8;
        cm_fn_0058e340((u32 *)M32(self),(int *)b,(int *)(b+4),(u32)(scratch+0x10));
    }
    for (i=0;i<50;i++) {
        p=M32(staff+0xd7+i*4);
        if (!p || !M32(p+0x61) || (s32)M32(p)>=(s32)M32(0xae2364) || p==M32(staff+0xcf)) continue;
        cm_fn_00544710(p,15,1);
        a=cm_fn_004cf5f0(M32(0xae1050),p,staff);
        if (!a) continue;
        v=(s8)M8(a+0x44)+75;
        M8(a+0x44)=v>100?100:v< -100?-100:v;
        b=M8(a+0x4f)>>4;
        if (!b || (b>3 && b!=5) || !M32(0xde1f54)) continue;
        c=M32(a+0x29); d=M32(a+0x25);
        if (cm_fn_0054cdb0((short *)0xae2c90,d,c)<=300) continue;
        q=cm_fn_00849ce0(M32(M32(0xde1f54)+0x856),staff,p,0,0);
        if (q && M32(q+15)==M32(p))
            cm_fn_004cda70(M32(0xae1050),p,0,0,-1,1.0,1);
    }
    cm_fn_0054a110((u16 *)(scratch+0x48));
    *(volatile u8 *)&frame.state=1;
    cm_fn_00417c10((u32 *)(scratch+0x48),(u32 *)(scratch+8),(u16 *)0xae2c90);
    a=M32(club); M8((u32)scratch+0x50)=8; M32((u32)scratch+0x51)=a;
    idx=M32(staff); b=M32(self+0x10)+idx*8;
    a=cm_fn_0058e340((u32 *)M32(self+4),(int *)b,(int *)(b+4),(u32)(scratch+0x48));
    CM_FS(u32,0)=frame.prev;
    return a;
}
