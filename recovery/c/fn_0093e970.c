#include "cmre.h"
/* Original instruction evidence: preferred-image VAs 0093e970-0093f992,
 * CM0102 3.9.68 SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * Stack calls and caller cleanup at 0093ea74-0093ea9a, 0093eacc-0093eaef
 * establish the two lookup paths below. Register residue in widened pushes is
 * not a new argument: consumers retain their declared narrow input widths.
 */
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define SW(a) ((s32)(s16)W(a))
extern u32 CM_CDECL cm_fn_00942350(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00942a40(u32,u32);
extern u32 CM_CDECL cm_fn_00942f50(u32,u16);
extern void CM_CDECL cm_fn_00944180(u32,u32);
extern void CM_CDECL cm_fn_009445d0(u32,u32);
extern void CM_CDECL cm_fn_0093e7d0(u32,s32);
extern void CM_CDECL cm_fn_0093e5b0(u32);
extern void CM_CDECL cm_fn_0093c200(u32,u32,u32,u32,u32);
extern void CM_CDECL cm_fn_0093c5e0(u32);
extern u32 CM_CDECL cm_fn_00944840(u32,u32);
extern u32 CM_CDECL cm_fn_009446e0(u32,u32);
extern u32 CM_CDECL cm_fn_0093f9a0(u32,u32,s32,u32);
extern u32 CM_CDECL cm_fn_00941f10(u32,u32);
extern u32 CM_CDECL cm_fn_00936510(u32,u32,u32,u32,u16,u32,u32);
extern u32 CM_CDECL cm_fn_0093fd10(u32,u16,u32,u32,u32,u32,u32,u32,u8,u8,u8,u32);
extern void CM_CDECL cm_fn_009409b0(u32,u8,u8);
extern u32 CM_CDECL cm_fn_00941480(u32);

void CM_CDECL cm_fn_0093e970(u32 s,u32 code,volatile u32 depth,
                            u32 dx,u32 dy,u32 mode,volatile u32 flags)
{
    /* Offsets are relative to ESP after four saves (0093e987).
       Related output words and the descriptor remain in one allocation. */
    u32 scratch[33];
    u32 l=(u32)scratch;
#define L(o) D(l+(o)-0x10)
#define LW(o) W(l+(o)-0x10)
    u32 f=flags, special=f&0x80, draw, kind;
    u32 p,t,a,b,c,d,x,y,save60,save64,stream,scan,acc=0,childflags;
    u32 saved[4],sx,sy,cf,id,ox,oy,changed,merged=0;
    s32 n,i;
    L(0x1c)=0;
    if(special) {
        t=D(D(s+0x139)+0xc);
        if(t && W(t+0x38)!=2000 && W(t+0x38)!=2001) {
            a=(u32)SW(t+0x32);
            b=D(s+0xe6);
            f &= 0xfffe; flags=f;
            if((s32)b >= (s32)a && (s32)D(s+0xea)>=(s32)a) {
                draw=0; goto flags_ready;
            }
            special=0; f &=0xff7f;
        } else { special=0; f|=1; }
        flags=f;
    }
    draw=(f&0x281)!=0;
flags_ready:
    kind=(f&0x200)?3:0;
    if((u8)mode>5) cm_fn_00944180(D(s+4),0x2713);
    LW(0x3c)=W(s+0x137);
    cm_fn_0093e7d0(s,1);
    if(f&8) {
        p=cm_fn_00942350(D(s+0x139),code,special,l+0x3c,l+0x44);
        L(0x1c)=cm_fn_00942f50(D(s+0x139),(u16)code);
        if(!depth) W(s+0xcc)=(u16)code;
    } else {
        t=cm_fn_00942a40(D(s+0x139),code);
        p=cm_fn_00942350(D(s+0x139),t&0xffff,special,l+0x3c,l+0x44);
        if(!depth) W(s+0xcc)=W(p+0x71);
        if((s32)code>=0x30 && (s32)code<=0x39) L(0x1c)=1;
    }
    if(!p) cm_fn_00944180(D(s+4),0x2715);
    D(s+0xc8)=p;
    if(SW(p+0x34)<0) {
        save60=D(p+0x60); stream=D(p+0x54); save64=D(p+0x64);
        D(p+0x54)=0; D(p+0x60)=0; D(p+0x64)=0;
        saved[0]=D(s+8); saved[1]=D(s+0xc);
        saved[2]=D(s+0x10); saved[3]=D(s+0x14);
        sx=D(s+0xe6); sy=D(s+0xea);
        childflags=((u8)f&0x9d)|0xc;
        L(0x50)=0; acc=0;
        if(D(s+0x12f)==2 || kind==3) childflags&=0xfdfe;
        scan=stream;
        do {
            cf=W(scan); acc|=cf; scan+=2;
            id=W(scan); scan+=2;
            a=(u32)SW(scan);
            if(cf&1) { ox=a; oy=(u32)SW(scan+2); scan+=4; }
            else {
                scan+=2;
                if(cf&2) { oy=(u32)(s32)(s8)a; ox=(u32)((s32)a>>8); }
                else { oy=a&255; ox=(a>>8)&255; }
            }
            changed=0;
            if(cf&2) {
                t=W(s+0x137);
                ox=(u32)((s32)((t>>1)+((ox*D(s+0xe6))<<6))/(s32)t);
                oy=(u32)((s32)((t>>1)+((oy*D(s+0xea))<<6))/(s32)t);
            }
            if(cf&8) {
                a=(u32)SW(scan)<<2; b=0; c=0; d=a; scan+=2; changed=1;
            } else if(cf&0x40) {
                a=(u32)SW(scan)<<2; scan+=2; b=0; c=0;
                d=(u32)SW(scan)<<2; scan+=2; changed=1;
            } else if(cf&0x80) {
                a=(u32)SW(scan)<<2; b=(u32)SW(scan+2)<<2; scan+=4;
                c=(u32)SW(scan)<<2; scan+=2;
                d=(u32)SW(scan)<<2; scan+=2; changed=1;
            }
            if(changed) {
                D(s+8)=a; D(s+0xc)=b; D(s+0x10)=c; D(s+0x14)=d;
                D(s+0x1c)=(a==0x10000 && !b && !c && d==a);
            }
            t=depth; depth=t+1;
            cm_fn_0093e970(s,id,t,dx,dy,0,childflags);
            if(changed) {
                D(s+8)=saved[0]; D(s+0xc)=saved[1];
                D(s+0x10)=saved[2]; D(s+0x14)=saved[3];
                D(s+0x1c)=(saved[0]==0x10000 && !saved[1] && !saved[2] && saved[3]==saved[0]);
                D(s+0xe6)=sx; D(s+0xea)=sy;
            }
            cm_fn_0093c200(l+0x40,D(s+0xc8),cf,ox,oy);
            a=D(s+0xc8); p=L(0x50);
            if(p!=a) { cm_fn_0093c5e0(a); p=L(0x50); }
            D(s+0xc8)=0;
        } while(cf&0x20);
        a=D(s+4); D(s+0xc8)=p; b=D(p+0x60); c=D(a+0x5c);
        if(b==c) D(a+0x8c)=1;
        else cm_fn_009445d0(a,b);
        D(p+0x60)=save60; D(p+0x64)=save64;
        cm_fn_009445d0(D(s+4),stream);
        y=D(p+0x50); x=D(p+0x4c); n=SW(p+0x36);
        if(!(acc&0x200)) {
            a=D(p+0x40); W(a+(u32)n*2)=0;
            b=D(s+0xc8); a=D(p+0x40); c=(u32)SW(b+0x36);
            d=W(a+c*2); c=(u32)SW(p+0x36);
            W(a+c*2+2)=(u16)(d+LW(0x4c));
            b=D(p+0x40); a=(u32)SW(p+0x36); c=W(s+0x137);
            a=(u32)SW(b+a*2+2)*D(s+0xe6);
            a=(u32)((s32)((a<<6)+(c>>1))/(s32)c);
            c=(u32)SW(p+0x36); b=D(p+0x4c); D(b+c*4+4)=a;
            if((u8)flags&0x81) {
                a=(u32)SW(p+0x36); b=D(p+0x4c); c=D(b+a*4+4); D(b+a*4+4)=c+32;
                b=D(p+0x4c); a=(u32)SW(p+0x36); c=D(b+a*4+4); D(b+a*4+4)=c&0xffffffc0;
            }
            c=(u32)SW(p+0x36); b=D(p+0x4c); D(b+c*4)=0;
        }
        if(((u8)flags&0x80) || kind==3 || (D(s+0x12f)==2 && draw)) {
            a=D(s+0xe6)<<6; t=LW(0x3c);
            sx=cm_fn_00944840(t,a);
            sy=cm_fn_00944840(t,D(s+0xea)<<6);
            n=SW(p+0x36);
            for(i=0;i<n+4;i++) {
                b=D(x+(u32)i*4); c=D(y+(u32)i*4);
                a=cm_fn_009446e0(b,sx); b=cm_fn_009446e0(c,sy);
                d=D(p+0x40); W(d+(u32)i*2)=(u16)a;
                d=D(p+0x44); W(d+(u32)i*2)=(u16)b;
            }
            if(special) cm_fn_00941f10(D(D(s+0x139)+4),p);
            if(draw) { merged=1; goto submit; }
        }
        goto common;
    }
    x=D(p+0x4c); y=D(p+0x50); n=SW(p+0x36);
    if((special||draw) && !((u8)flags&0x10) && n>0) {
        if(special) {
            cm_fn_0093f9a0(D(p+0x40),x,n+4,s+0xfe);
            cm_fn_0093f9a0(D(p+0x44),y,n+4,s+0x110);
            cm_fn_00941f10(D(D(s+0x139)+4),p);
        }
        if(!draw) goto common;
submit:
        cm_fn_0093e5b0(s);
        LW(0x6c)=W(p+0x34);
        if(n>32000) cm_fn_00944180(D(s+4),0x2716);
        if(merged) {
            L(0x70)=D(p+0x38); L(0x74)=D(p+0x3c);
            L(0x78)=D(p+0x40); L(0x7c)=D(p+0x44); L(0x80)=D(p+0x48);
            L(0x88)=y; LW(0x6e)=(u16)n; L(0x84)=x;
        } else {
            L(0x74)=D(p+0x3c); L(0x70)=D(p+0x38);
            L(0x7c)=D(p+0x44); L(0x80)=D(p+0x48);
            a=D(p+0x40); LW(0x6e)=(u16)n;
            L(0x78)=a; L(0x84)=x; L(0x88)=y;
            cm_fn_0093f9a0(a,x,n+4,s+0xfe);
            if(kind!=3) cm_fn_0093f9a0(D(p+0x44),y,n+4,s+0x110);
        }
        t=W(p+0x32);
        a=cm_fn_00936510(D(s+0x13d),l+0x5c,kind,L(0x1c),(u16)t,(u8)mode!=0,4);
        if(!merged && a==(u32)-100) cm_fn_0093f9a0(D(p+0x44),y,n+4,s+0x110);
        else if(a) cm_fn_00944180(D(s+4),a);
    } else {
        cm_fn_0093f9a0(D(p+0x40),x,n+4,s+0xfe);
        cm_fn_0093f9a0(D(p+0x44),y,n+4,s+0x110);
        if((u8)flags&0x10) {
            if((u8)dx) for(i=0;i<n;i++) D(x+(u32)i*4)+=(u32)(s32)(s8)dx;
            if((u8)dy) for(i=0;i<n;i++) D(y+(u32)i*4)-=(u32)(s32)(s8)dy;
        }
    }
common:
    D(s+0x9c)=(u32)LW(0x4c)<<16;
    a=(u32)LW(0x54)<<16;
    D(s+0xa0)=0; D(s+0xb8)=0; D(s+0xbc)=a;
    if(!depth) {
        c=D(x+(u32)n*4);
        if(c) for(i=0;i<n+4;i++) D(x+(u32)i*4)-=c;
        c=D(y+(u32)n*4);
        if(c) for(i=0;i<n+4;i++) D(y+(u32)i*4)-=c;
        if(!((u8)flags&0x10)) {
            a=D(x+(u32)n*4+4); b=D(x+(u32)n*4);
            c=(b+32)&0xffffffc0;
            d=(((a-b)+32)&0xffffffc0)+c;
            D(x+(u32)n*4)=c; D(x+(u32)n*4+4)=d;
            if((flags&0x140) && n>0) {
                t=(u32)((s32)(d-a-b+c)>>1);
                if(t) for(i=0;i<n;i++) D(x+(u32)i*4)+=t;
            }
        }
    }
    a=D(s+0xf6); b=D(s+0x9c);
    t=cm_fn_009446e0(b,a);
    a=D(s+0xfa); b=D(s+0xbc); D(s+0x9c)=t;
    t=cm_fn_009446e0(b,a); D(s+0xbc)=t;
    for(i=0;i<4;i++) {
        a=x+(u32)(n+i)*4; D(a)=D(a)<<10;
        a=y+(u32)(n+i)*4; D(a)=D(a)<<10;
    }
    if(!D(s+0x1c)) {
        a=D(s+8); c=D(s+0x10); d=D(s+0x14); b=D(s+0xc);
        if(!b && !c) {
            for(i=0;i<n+4;i++) {
                ox=D(x+(u32)i*4); oy=D(y+(u32)i*4);
                t=cm_fn_009446e0(a,ox); D(x+(u32)i*4)=t;
                t=cm_fn_009446e0(d,oy); D(y+(u32)i*4)=t;
            }
        } else {
            for(i=0;i<n+4;i++) {
                ox=D(x+(u32)i*4); oy=D(y+(u32)i*4);
                t=cm_fn_009446e0(b,oy);
                t+=cm_fn_009446e0(a,ox); D(x+(u32)i*4)=t;
                t=cm_fn_009446e0(d,oy);
                t+=cm_fn_009446e0(c,ox); D(y+(u32)i*4)=t;
            }
        }
        ox=D(s+0x9c); t=cm_fn_009446e0(a,ox); D(s+0x9c)=t;
        t=cm_fn_009446e0(c,ox); D(s+0xa0)=t;
        oy=D(s+0xbc); t=cm_fn_009446e0(b,oy); D(s+0xb8)=t;
        t=cm_fn_009446e0(d,oy); D(s+0xbc)=t;
    }
    D(s+0x94)=D(x+(u32)n*4+4)-D(x+(u32)n*4);
    D(s+0x98)=D(y+(u32)n*4+4)-D(y+(u32)n*4);
    D(s+0xb0)=D(x+(u32)n*4+8)-D(x+(u32)n*4+12);
    D(s+0xb4)=D(y+(u32)n*4+8)-D(y+(u32)n*4+12);
    for(i=0;i<4;i++) {
        a=x+(u32)(n+i)*4; D(a)=(u32)((s32)D(a)>>10);
        a=y+(u32)(n+i)*4; D(a)=(u32)((s32)D(a)>>10);
    }
    D(s+0xde)=0; D(s+0xda)=0; D(s+0xd6)=0;
    D(s+0xce)=0; D(s+0xd2)=0; D(s+0xa4)=0; D(s+0xa8)=0;
    D(s+0xc0)=0; D(s+0xc4)=0; B(p+0x70)=1;
    if(D(s+0x1c)) {
        t=255;
        if(special) { a=D(D(D(s+0x139)+4)+0x7c)&255; if(a!=255) t=a; }
        if((s32)D(s+0xe6)>(s32)t) B(p+0x70)=0;
    }
    if(((u8)flags&2) && SW(p+0x36)>0) {
        u32 result=0;
        u8 option=0, other, allow;
        u16 cc;
        if(B(s+0x12e) && ((u8)flags&1) && (s32)D(s+0x126)<=24) option=1;
        if((s32)D(p+0xa)<=0) {
            cc=W(s+0xcc); allow=B(p+0x70); other=allow;
            if(!(u8)mode && D(s+0x1c) && allow) {
                if(cc>=0x800 || (B(D(D(s+0x18)+0x10)+(cc>>3))&(1<<(cc&7)))) allow=0;
                if(cc>=0x800 || (B(D(D(s+0x18)+0x10)+(cc>>3))&(1<<(cc&7)))) other=0;
            }
            t=D(s+0x122);
            a=B(p+0x32); b=D(p+0x48); c=D(p+0x3c); d=D(p+0x38);
            id=W(p+0x34);
            result=cm_fn_0093fd10(D(s+4),(u16)id,d,c,x,y,b,mode,(u8)a,allow,other,t);
            cm_fn_009409b0(result,option,(u8)flags&0x20);
            D(s+0xce)=D(result+4)-D(result);
            D(s+0xd2)=D(result+12)-D(result+8);
            D(s+0xa4)=D(result+16); D(s+0xa8)=D(result+20);
            D(s+0xc0)=D(result+16); D(s+0xc4)=D(result+20); D(s+0xd6)=D(result+24);
            if(!(u8)mode && D(s+0x1c)) {
                if(!D(result+0x2204) && cc<0x800) {
                    a=D(D(s+0x18)+0x10)+(cc>>3); B(a)=B(a)|(1<<(cc&7));
                }
                if(!D(result+0x2208) && cc<0x800) {
                    a=D(D(s+0x18)+0x14)+(cc>>3); B(a)=B(a)|(1<<(cc&7));
                }
            }
        }
        D(s+0xc4)=D(s+0xc4)-D(x+(u32)n*4+8);
        D(s+0xc0)=D(s+0xc0)-D(y+(u32)n*4+8);
        if((s32)D(p+0xa)<=0) {
            D(s+0xda)=D(result+0x1c); D(result+0x1c)=0; D(s+0xde)=0;
        }
        t=D(x+(u32)n*4); if(t) D(s+0xa8)-=t;
        t=D(y+(u32)n*4); if(t) D(s+0xa4)-=t;
        D(s+0xa8)=D(s+0xa8)&0xffffffc0; D(s+0xa4)=D(s+0xa4)&0xffffffc0;
        cm_fn_00941480(result);
    }
    if(!((u8)flags&4)) cm_fn_0093e7d0(s,0);
}
