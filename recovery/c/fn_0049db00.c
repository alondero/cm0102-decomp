#include "cmre.h"
/* Preferred-image VAs below refer to CM0102 3.9.68, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   49dbb2/49e616: receiver for 48d200; 49dc05/49e684: 6826d0.
   49e293: receiver for 729500; 49e4fa: receiver for 5fe290.
   49deef and subsequent renderer sites load ECX=B74450.
   49ea8c..49eaa6 selects the receiver of 55f990 after 18 pushes.
   49db3f..49db56: the three pre-90d130 pushes belong to 944c9f. */
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define W(a) CM_GLOBAL(volatile u16,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
#define SB(a) ((s8)B(a))
#define SW(a) ((s16)W(a))
#define P(a) ((u32)(a))
extern u32 CM_THISCALL cm_fn_0048d200(u32,s32);
extern u32 cm_fn_0053b3d0(u32);
extern u32 cm_fn_0053b900(u32);
extern u32 CM_THISCALL cm_fn_005fe290(u32,u32,u32);
extern u32 cm_fn_00600a50(u32,u32,u8,u32,u32);
extern u32 cm_fn_0066f4e0(u32,u32,...);
extern u32 CM_THISCALL cm_fn_006826d0(u32,u32);
extern u32 CM_THISCALL cm_fn_00729500(u32,u32,u32,u32,u16,u16);
extern u32 CM_STDCALL cm_fn_007e6f20(u32);
extern u32 CM_THISCALL cm_fn_008040a0(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00804100(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 cm_fn_00909ec0(u32);
extern u32 cm_fn_0090c310(u32,u32);
extern u32 cm_fn_0090d130(void);
extern u32 cm_fn_00944c9f(u32,u32,...);
extern u32 cm_fn_00944cff(u32,u32,u32,u32,u32);
extern void cm_fn_005e8290(u32,u32,u32);
extern u32 cm_fn_005e5ad0(u32,u32);
extern u32 CM_THISCALL cm_fn_0055f990(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);

static void copyz(u32 dst,u32 src)
{
    u32 n=0,i;
    do { ++n; } while(B(src+n-1));
    for(i=0;i<(n>>2);++i) D(dst+i*4)=D(src+i*4);
    for(i=n&~3u;i<n;++i) B(dst+i)=B(src+i);
}
static void text(u32 out,u32 col,u32 row,u32 a,u32 b,u32 c,u32 d,u32 color,u32 str)
{
    cm_fn_00804100(0xb74450,out,col,row,a,b,c,d,color,str);
}
static void linktext(u32 out,u32 col,u32 row,u32 a,u32 b,u32 c,u32 d,u32 color,u32 str,u32 kind,u32 value)
{
    cm_fn_008040a0(0xb74450,out,col,row,a,b,c,d,color,str,kind,value);
}
static void competition(u32 p,u32 out,u32 rowptr,u32 kind,u32 club,u32 side,u32 buf,u32 comp)
{
    u32 obj,part,t,i,nation,x,fmt,val;
    s32 index;
    if(!comp) goto empty;
    t=D(comp); obj=D(D(0xadadfc)+t*4);
    index=SB(p+0x42);
    if(index<=(s32)D(obj+0x30)) {
        part=cm_fn_0048d200(obj,index);
        if(part && B(part+0x43)==2) {
            comp=D(p+0x14);
            t=D(comp);
            if(t!=D(0x9cf590) && !cm_fn_007e6f20(comp)) {
                B(buf)=0;
                if(SW(part+0x3e)>0) {
                    i=0;
                    do {
                        x=cm_fn_006826d0(part,i);
                        if(D(x)==club) {
                            x=cm_fn_0090c310((u32)((s32)SB(x+4)+1),3);
                            copyz(buf,x);
                            break;
                        }
                        ++i;
                    } while((s32)i<SW(part+0x3e));
                }
                if(B(buf)) linktext(out,side,D(rowptr),0x30,W(0xaebe04),12,1,W(0xae3184),buf,kind,p);
                else text(out,side,D(rowptr),0x30,W(0xaebe04),12,1,W(0xae3184),buf);
                return;
            }
        }
    }
    comp=D(p+0x14); t=D(comp);
    if(t==D(0x9cf590)) {
        x=cm_fn_00909ec0(D(p+(side?0x20:0x1c)));
        if((u16)x==0x413) { fmt=0x99ca64; val=0x209800; }
        else {
            x=cm_fn_00909ec0(D(p+(side?0x20:0x1c)));
            if((u16)x==0x47b) { fmt=0x99ca34; val=0x23d800; }
            else { fmt=0x99ca04; val=0x20a000; }
        }
        cm_fn_0066f4e0(buf,fmt,0xad9c64);
        val=(D(0x9cf590)<<22)|val;
        linktext(out,side,D(rowptr),0x30,W(0xaebe04),12,1,W(0xae3184),buf,0x7e5,val);
        return;
    }
    if(t==D(0x9cf668)) {
        x=D(p+0x1c); t=D(p+0x20);
        nation=D(x+0x53); t=D(t+0x53);
        if(nation==t) {
            x=D(club+0x57);
            if(x) goto related;
        }
        if(cm_fn_0053b900(club)) goto empty;
        nation=D(club+0x53);
        val=cm_fn_0053b3d0(nation);
        x=D(club+0x53)+0x53;
        goto national;
    }
    t=B(obj+0x42);
    if(t==1) {
        x=D(club+0x57);
        if(x && B(p+(side?0x41:0x40))) {
related:
            t=D(x);
            if(D(D(0xadadfc)+t*4)) {
                linktext(out,side,D(rowptr),0x30,W(0xaebe04),12,1,W(0xae3184),x+0x53,2000,x);
            } else text(out,side,D(rowptr),0x30,W(0xaebe04),12,1,W(0xae3184),0xad9c64);
            return;
        }
    }
    if(t==2) {
        nation=D(club+0x53);
        if(nation) {
            val=cm_fn_0053b3d0(nation);
            x=D(club+0x53)+0x53;
national:
            linktext(out,side,D(rowptr),0x30,W(0xaebe04),12,1,W(0xae3184),x,0x7d5,val);
            return;
        }
    }
empty:
    text(out,side,D(rowptr),0x30,W(0xaebe04),12,1,W(0xae3184),0);
}
static void statusword(u32 dst,s32 v)
{
    u32 f=0;
    if(v==5) f=0x99c368;
    else if(v==4) f=0x99c364;
    else if(v==3) f=0x99c360;
    else if(v==6) f=0x99c35c;
    else if(v==8) f=0x99c370;
    if(f) cm_fn_0066f4e0(dst,f);
}
u32 CM_CDECL cm_fn_0049db00(u32 p,u32 out,u32 rowptr,u32 kind)
{
    /* L is immediately after SUB ESP,A08. The four saved registers
       place [ESP+18] at L+8, [ESP+48] at L+38. */
    u8 scratch[0xa08];
    u32 base=P(scratch),buf=base+0x38;
    u32 a=base+8,b=base+0x10,c=base+0x18,d=base+0x20;
    u32 e=base+0x28,f=base+0x30;
    u32 club,flag,r,x,y,z,style,valid,v,stadium,city,width,self,offset;
    s32 s;
    if(!p) {
        cm_fn_00944cff(0x99c15c,0,0,base+0x908,base+0x808);
        x=cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0,0x9870e8,x,base+0x908,base+0x808,0x203e);
        cm_fn_005e8290(0x9870e0,0xae24d0,0);
        D(0xb67a34)=0;
        return 0;
    }
    x=D(p+0x14); flag=B(p+0x40); club=D(p+0x1c);
    competition(p,out,rowptr,kind,club,0,buf,x);
    r=cm_fn_00600a50(club,0,1,0,0);
    linktext(out,2,D(rowptr),1,0,1,3,W(r?0xaebdec:0xae3184),club+0x38,flag?0x7d5:0x7d6,club);
    B(c)=0; B(d)=0;
    s=SB(p+0x4b);
    if(s!=-1) {
        cm_fn_00944c9f(d,0x98806c,s);
        cm_fn_00944c9f(c,0x98806c,(s32)SB(p+0x4c));
        style=3;
    } else {
        s=SB(p+0x3d);
        if(s==1) cm_fn_0066f4e0(d,0x99c370);
        else if(s==2) cm_fn_0066f4e0(d,0x99c36c);
        else if(s<0) cm_fn_00944c9f(d,0x98806c,-s);
        s=SB(p+0x3e);
        if(s==1) cm_fn_0066f4e0(c,0x99c370);
        else if(s==2) cm_fn_0066f4e0(c,0x99c36c);
        else if(s<0) cm_fn_00944c9f(c,0x98806c,-s);
        style=1;
    }
    if(B(d)) text(out,1,D(rowptr),1,0,12,style,W(0xaebdec),d);
    if(B(c)) text(out,7,D(rowptr),1,0,12,style,W(0xaebdec),c);
    if(B(p+0x43)!=255) {
        s=SB(p+0x3d); B(b)=0; B(a)=0;
        statusword(a,s);
        statusword(b,SB(p+0x3e));
        if(B(p+0x47)!=255) {
            cm_fn_00944c9f(f,0x98806c,(s32)SB(p+0x47));
            s=SB(p+0x48);
        } else {
            cm_fn_00944c9f(f,0x98806c,(s32)SB(p+0x43));
            s=SB(p+0x44);
        }
        cm_fn_00944c9f(e,0x98806c,s);
        if(B(a)) text(out,3,D(rowptr),1,0,0x40,1,W(0xaebdec),a);
        valid=0;
        x=B(p+0x43);
        if(x!=254 && x!=253) {
            v=D(p+8);
            if((s32)v>-1) {
                y=W(p+0x28); z=W(p+0x2a);
                x=D(p+0x10); r=D(p+0xc); self=D(0xb6b7c0);
                valid=cm_fn_00729500(self,v,r,x,(u16)z,(u16)y);
            }
        }
        if(valid) {
            cm_fn_00944c9f(0xde1f64,0x99c354,f);
            text(out,4,D(rowptr),1,0,12,3,W(0xaebde8),0xde1f64);
            v=D(p+8);
            linktext(out,4,D(rowptr),1,0,12,3,W(0xaebde8),0x989adc,0x7d3,v);
            cm_fn_00944c9f(0xde1f64,0x99c34c,e);
            style=3;
        } else {
            x=B(p+0x43);
            if(x==254) {
                cm_fn_0066f4e0(0xde1f64,0x99c9e4,0xad9c64);
                text(out,4,D(rowptr),1,0,12,2,W(0xaebde8),0xde1f64);
            } else if(x==253) {
                cm_fn_0066f4e0(0xde1f64,0x99c9c4,0xad9c64);
                v=D(p+8);
                linktext(out,4,D(rowptr),1,0,12,2,W(0xaebde8),0xde1f64,0x7d3,v);
            } else {
                cm_fn_00944c9f(0xde1f64,0x99c354,f);
                text(out,4,D(rowptr),1,0,12,3,W(0xaebde8),0xde1f64);
            }
            text(out,4,D(rowptr),1,0,12,3,W(0xaebde8),0x989adc);
            x=B(p+0x43);
            if(x==254) {
                cm_fn_0066f4e0(0xde1f64,0x99c9a4,0xad9c64);
                style=2;
            } else if(x==253) {
                cm_fn_0066f4e0(0xde1f64,0x99c984,0xad9c64);
                style=2;
            } else {
                cm_fn_00944c9f(0xde1f64,0x99c34c,e);
                style=3;
            }
        }
        text(out,4,D(rowptr),1,0,12,style,W(0xaebde8),0xde1f64);
        if(B(b)) text(out,5,D(rowptr),1,0,1,1,W(0xaebdec),b);
    } else {
        x=cm_fn_005fe290(D(0xb63c98),p,0);
        if(x) {
            cm_fn_0066f4e0(0xde1f64,0x9918d4);
            r=W(0xaebdec); style=1;
        } else {
            cm_fn_0066f4e0(0xde1f64,0x99c1b4);
            r=W(0xaebde8); style=3;
        }
        linktext(out,4,D(rowptr),1,0,12,style,r,0xde1f64,0x7d2,p);
    }
    club=D(p+0x20); flag=B(p+0x41);
    r=cm_fn_00600a50(club,0,1,0,0);
    linktext(out,6,D(rowptr),1,0,1,3,W(r?0xaebdec:0xae3184),club+0x38,flag?0x7d5:0x7d6,club);
    x=D(p+0x14);
    competition(p,out,rowptr,kind,club,8,buf,x);
    D(rowptr)=D(rowptr)+1;
    if(B(p+0x3c)) {
        stadium=D(p+0x18);
        if(stadium) {
            copyz(buf,stadium+4);
            city=D(stadium+0x38);
            if(city) {
                x=(u32)(s32)SB(city+0x1e); y=(u32)(s32)SB(stadium+0x37);
                cm_fn_0066f4e0(0xde1f64,0x9902a4,y,y,x,x,buf,city+4);
            } else copyz(0xde1f64,buf);
            width=cm_fn_005e5ad0(2,0xde1f64);
            r=W(0xaebdd4); y=D(rowptr);
            offset=(u32)(s32)SW(0xb77c7a); offset=(offset*3)<<8;
            self=D(offset+0xb74c4c);
            cm_fn_0055f990(self,1,0xb4,0,width+0xb4,0,0,y,1,0,0,1,2,r,0xde1f64,0,0,0,out);
            D(rowptr)=D(rowptr)+1;
        }
    }
    return 1;
}
