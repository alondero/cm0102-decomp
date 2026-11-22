#include "cmre.h"
#define M8(a) (*(volatile u8 *)(a))
#define M16(a) (*(volatile u16 *)(a))
#define M32(a) (*(volatile u32 *)(a))
extern void CM_FASTCALL cm_fn_0054a110(u16 *);
extern void CM_THISCALL cm_fn_00417c10(u32 *,u32 *,u16 *);
extern void CM_THISCALL cm_fn_0058e340(u32 *,int *,int *,u32);
extern u32 CM_CDECL cm_fn_0053d980(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00544710(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32,u32,u32);
extern int CM_THISCALL cm_fn_0054cdb0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00849ce0(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004cda70(u32,u32,u32,u32,s32,double,u8);
extern u32 CM_THISCALL cm_fn_0086f5b0(u32,u32,u32);
extern long double CM_THISCALL cm_fn_007c1e30(u32,u32,u32,u32,float);
extern u32 CM_STDCALL cm_fn_0075c390(u32,u32,s8);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern char * CM_CDECL cm_fn_0090d130(void);
extern void CM_CDECL cm_fn_005e8290(const char *,const char *,int);

static void error_report(u8 *scratch, int reverse, u32 line)
{
    u8 *a=scratch+0x80, *b=scratch+0x180;
    if (reverse) {
        cm_fn_00944cff(0xa83b78,0,0,a,b);
        cm_fn_00944c9f(0xae24d0,0x9870e8,cm_fn_0090d130(),a,b,line);
    } else {
        cm_fn_00944cff(0xa83b78,0,0,b,a);
        cm_fn_00944c9f(0xae24d0,0x9870e8,cm_fn_0090d130(),b,a,line);
    }
    cm_fn_005e8290((const char *)0x9870e0,(const char *)0xae24d0,0);
    CM_GLOBAL(volatile u32,0xb67a34)=0;
}

void CM_THISCALL cm_fn_00868c50(u32 self,u32 p,u32 q)
{
    volatile struct { u32 prev,handler,state; } frame;
    u8 scratch[0x480];
    u32 table, obj, x, y, z, i, rec, target;
    s32 n, v, factor, f2;
    s16 threshold, low;
    frame.prev=CM_FS(u32,0); frame.handler=0x962b76; frame.state=~0u;
    CM_FS(u32,0)=(u32)&frame;
    if (!p || !q) { error_report(scratch,0,0x21c); goto done; }
    table=CM_GLOBAL(volatile u32,0xadadfc);
    obj=M32(table+M32(q)*4);
    if (!obj) { error_report(scratch,1,0x224); goto done; }
    n=(s16)M16(q+0x69)*500;
    v=(s16)M16(p+0x80);
    if (v<n) {
        n-=v; if(n>750)n=750;
        M16(p+0x80)+=(u16)n;
        v=(s16)M16(p+0x80);
        if(v<1)v=1; else if(v>10000)v=10000;
        M16(p+0x80)=(u16)v;
    }
    if (M32(p+0x57) && (x=M32(p+0x69))!=0) {
        v=(s32)M32(p+0x7b);
        if(v<150000 && v<(s32)(M32(p+0x73)+500))
            M32(p+0x7b)=(u32)v+(s16)M16(q+0x69)*20;
        v=(s32)M32(x+0x44);
        if(v<120000) M32(x+0x44)=(u32)v+((s16)M16(q+0x69)+20)*25;
    }
    n=((s16)M16(obj+0x3e)-1)*(s16)M16(obj+0x3c);
    /* Original __ftol call at 0x00868d9d. */
    threshold=(s16)(long long)((long double)n*CM_GLOBAL(double,0x968f78));
    cm_fn_0054a110((u16 *)(scratch+0x14));
    frame.state=0;
    cm_fn_00417c10((u32 *)(scratch+0x14),(u32 *)(scratch+0xc),(u16 *)0xae2c90);
    x=M32(p+0xcf); y=M32(p); z=M32(q);
    M8((u32)scratch+0x20)=5; M32((u32)scratch+0x1c)=y; M32((u32)scratch+0x21)=z;
    if(x) {
        if(M32(x+0x69)) {
            table=CM_GLOBAL(volatile u32,0xadadfc);
            y=M32(table+z*4);
            if((s8)M8(y+0x42)==1) {
                y=M32(p+0x53);
                factor=cm_fn_0053d980(x,y,1)?200:400; f2=200;
            } else factor=f2=400;
            for(i=0;i<3;i++) {
                x=M32(p+0xcf); rec=M32(x+0x69);
                n=(s16)M16(q+0x69)*(i==0?400:i==1?factor:f2);
                /* Original __ftol calls at 0x00868e5b, 0x00868ec6 and 0x00868f29. */
                M16(rec+(i==0?10:i==1?8:12))+=(u16)(long long)((long double)n*CM_GLOBAL(double,0x96ff88));
                x=M32(p+0xcf); rec=M32(x+0x69);
                low=(s16)((s16)M16(q+0x69)*(i==0?400:i==1?factor:f2));
                v=(s16)M16(rec+(i==0?10:i==1?8:12));
                if(low>v)v=low; else if(v>10000)v=10000;
                M16(rec+(i==0?10:i==1?8:12))=(u16)v;
            }
        }
        x=M32(p+0xcf);
        if(x) {
            y=M32(x); z=M32(self+0xc)+y*8;
            cm_fn_0058e340((u32 *)M32(self),(int *)z,(int *)(z+4),(u32)(scratch+0x14));
        } else error_report(scratch,0,0x1010);
        x=cm_fn_0086f5b0(self,M32(p+0xcf),1);
        if(x) {
            M16(x+0x8c)++;
            y=M32(x+0xb6); n=(s16)M16(q+0x69)*5000; z=M32(x+0xca);
            M32(x+0xb6)=y+n; y=M32(x+0xba); M32(x+0xca)=z+n; M32(x+0xba)=y+n;
        }
    }
    x=M32(p+0xbf);
    if(x) {
        y=M32(x); z=M32(self+0xc)+y*8;
        cm_fn_0058e340((u32 *)M32(self),(int *)z,(int *)(z+4),(u32)(scratch+0x14));
    }
    for(i=0;i<50;i++) {
        target=p+0xd7+i*4; x=M32(target);
        if(!x)continue;
        y=M32(x+0x61);
        if(!y || (s32)M32(x)>=(s32)CM_GLOBAL(volatile u32,0xae2364) || M32(x+0x39)!=p || x==M32(p+0xcf))continue;
        if(cm_fn_007c1e30(CM_GLOBAL(volatile u32,0xb74350),y,2,1,6.5f)>(long double)threshold) {
            x=M32(target);
            if(x) {
                y=M32(x); z=M32(self+0xc)+y*8;
                cm_fn_0058e340((u32 *)M32(self),(int *)z,(int *)(z+4),(u32)(scratch+0x14));
            } else error_report(scratch,0,0x1010);
            cm_fn_00544710(M32(target),20,1);
            x=cm_fn_004cf5f0(CM_GLOBAL(volatile u32,0xae1050),M32(target),p);
            if(!x)continue;
            v=(s8)M8(x+0x44)+100;
            M8(x+0x44)=(u8)(v>100?100:v< -100?-100:v);
            if(CM_GLOBAL(volatile u32,0xde1f54)) {
                y=M32(x+0x29); z=M32(x+0x25);
                if(cm_fn_0054cdb0(0xae2c90,z,y)>300) {
                    y=CM_GLOBAL(volatile u32,0xde1f54);
                    z=cm_fn_00849ce0(M32(y+0x856),p,M32(target),0,0);
                    if(z && M32(z+0xf)==M32(M32(target)))
                        cm_fn_004cda70(CM_GLOBAL(volatile u32,0xae1050),M32(target),0,0,-1,1.0,1);
                }
            }
        } else {
            cm_fn_00544710(M32(target),10,1);
            x=cm_fn_004cf5f0(CM_GLOBAL(volatile u32,0xae1050),M32(target),p);
            if(x) {
                v=(s8)M8(x+0x44)+50;
                M8(x+0x44)=(u8)(v>100?100:v< -100?-100:v);
            }
        }
    }
    cm_fn_0054a110((u16 *)(scratch+0x4c));
    *(volatile u8 *)&frame.state=1;
    cm_fn_00417c10((u32 *)(scratch+0x4c),(u32 *)(scratch+0xc),(u16 *)0xae2c90);
    z=M32(q); M8((u32)scratch+0x54)=5; M32((u32)scratch+0x55)=z;
    x=M32(p); y=M32(self+0x10)+x*8;
    cm_fn_0058e340((u32 *)M32(self+4),(int *)y,(int *)(y+4),(u32)(scratch+0x4c));
    if(CM_GLOBAL(volatile u32,0xb6b7c8) && (s16)M16(q+0x69)>3) {
        cm_fn_0075c390(p,q,1);
        if((s16)M16(q+0x69)>12) cm_fn_0075c390(p,q,3);
    }
done:
    CM_FS(u32,0)=frame.prev;
}
