/* CM0102 3.9.68 preferred-image VA 006b6cc0; primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))
extern u32 CM_STDCALL cm_fn_0076d940(u32);
extern void CM_STDCALL cm_fn_005afe00(u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32,u32,u32);
extern long double CM_THISCALL cm_fn_004a31a0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00849ce0(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00600a50(u32,u32,u8,u32,u32);
/* Fixed nonzero small bounds use only the one stack argument; EAX result. */
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern void CM_THISCALL cm_fn_0072c1f0(u32,u32,u32,u32,u32,u32);
extern void CM_THISCALL cm_fn_00744240(u32,u32,u32,u8,u32);
extern long double CM_THISCALL cm_fn_007c1e30(u32,u32,u32,u32,float);
extern u32 CM_THISCALL cm_fn_008676f0(u32);
extern u32 CM_THISCALL cm_fn_00867560(u32,u32);

__attribute__((target("no-sse3")))
u32 CM_CDECL cm_fn_006b6cc0(u32 report,u32 packet)
{
    u32 checked, manager=0, side, club, row, team, player, x,y,z,k,opponent;
    u32 other, ptr, target, original_player, a,b,c;
    u16 delta_a=0,delta_b=0,delta_c=0;
    u8 sent=0,mask;
    s16 n;
    volatile double rating, quality, average;
    long double base,t,ratio,denom;
    volatile long long converted;
    /* Original reads this otherwise unused global before the argument read. */
    x=D(0x00b74318); (void)x;
    x=D(report+0x14u); checked=cm_fn_0076d940(x);
    if(B(0x009cc270)!=0 && D(0x00ae2a60)!=0) cm_fn_005afe00(report,packet);
    x=D(report);
    if((s32)x>-1) { y=D(0x00adadfc);z=D(y+x*4u);manager=D(z+8u); }
    for(side=0;side<2;side++) {
        x=D(packet+side*4u);y=D(0x00ae23b4);club=y+x*0x245u;
        team=packet+side*0x625u;row=team+0x1c5u;
        for(k=20;k;k--,row+=0x49u) {
            x=D(row-0x35u);
            if(x==0xffffffffu)continue;
            y=D(0x00ae23bc);player=y+x*0x6eu;
            x=D(player+0x61u);if(!x)continue;
            if(player) { x=D(player);y=D(0x00ae2364);if((s32)x>=(s32)y)continue; }
            if(manager) {
                x=D(report+0x14u);y=D(club+0x57u);
                if(x==y && B(packet+0x4cu)==0) {
                    x=D(0x00ae1050);other=cm_fn_004cf5f0(x,player,club);
                    x=D(player+0x61u);rating=(double)cm_fn_004a31a0(manager,x,1);
                    x=D(player+0x61u);quality=(double)cm_fn_004a31a0(manager,x,0x11);
                    if(other) {
                        mask=B(other+0x4fu)&0xf0u;
                        if(mask==0x30||mask==0x10||mask==0x20) {
                            x=D(0x00de1f54);x=D(x+0x856u);
                            other=cm_fn_00849ce0(x,club,player,0,0);
                            x=cm_fn_00600a50(club,0,1,0,0);
                            if(x || (other && D(other+0xfu)==D(player))) {
                                n=(s16)W(row+3u);
                                if(n>5000) {
                                    base=(long double)(s16)W(team+0x18eu);
                                    t=base*Q(0x009689c0);
                                    x=!(t>=(long double)n);
                                    if(!x) { n=(s16)W(row+7u);t=base*Q(0x009689d8);x=!(t>=(long double)n); }
                                    if(!x) { y=D(player+0x61u);n=(s16)W(y+5u);t=base*Q(0x0096a708);x=!(t>=(long double)n); }
                                    if(x && B(row)<=6) {
                                        opponent=packet+(side==0)*0x625u;
                                        y=B(opponent+0x120u);z=B(opponent+0x11fu);b=y+z;
                                        y=B(team+0x120u);z=B(team+0x11fu);a=y+z;
                                        if((s32)a<=(s32)b) {
                                            x=cm_fn_0090cfc0(10);x+=5u;
                                            if(!((long double)(s32)x>=rating)) {
                                                x=cm_fn_0090cfc0(10);y=D(player+0x61u);n=(s8)B(y+0x45u);
                                                if((s32)n<(s32)x) {
                                                    ptr=D(0x00b74350);denom=Q(ptr+0x18u);t=Q(0x009689a0);
                                                    if(!(denom<t)&&!(denom>t))ratio=Q(0x00968998);else {ratio=Q(ptr+0x10u);ratio/=Q(ptr+0x18u);}
                                                    ratio+=Q(0x00968918);
                                                    if(ratio>quality) {
                                                        denom=Q(ptr+0x18u);t=Q(0x009689a0);
                                                        if(!(denom<t)&&!(denom>t))ratio=Q(0x00968998);else {ratio=Q(ptr+0x10u);ratio/=Q(ptr+0x18u);}
                                                        ratio-=Q(0x00968918);
                                                        if(!(ratio>=quality))cm_fn_0072c1f0(D(0x00b6b7c8),player,club,0x11,report,0);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if(checked && manager && B(packet+0x4cu)==0) {
                x=D(player+0x61u);rating=(double)cm_fn_004a31a0(manager,x,1);
                if(B(row)>=9) {
                    opponent=packet+(side==0)*0x625u;
                    x=B(opponent+0x120u);y=B(opponent+0x11fu);b=x+y;
                    x=B(team+0x120u);y=B(team+0x11fu);a=x+y;
                    if((s32)a>=(s32)b) {
                        x=cm_fn_0090cfc0(5);x+=2u;
                        if(!((long double)(s32)x>=rating) && !sent) {
                            ptr=D(0x00b74350);denom=Q(ptr+0x18u);t=Q(0x009689a0);
                            if(!(denom<t)&&!(denom>t))average=6.8;else {ratio=Q(ptr+0x10u);ratio/=Q(ptr+0x18u);average=(double)ratio;}
                            x=D(player+0x61u);rating=(double)cm_fn_004a31a0(manager,x,0x11);
                            x=cm_fn_0090cfc0(9);t=(long double)(s32)x;t*=Q(0x00967880);t+=average;t+=Q(0x00968968);
                            if(!(t>=rating)) {cm_fn_00744240(D(0x00b6b7c8),player,club,0x34,report);sent=1;}
                        }
                    }
                }
            }
            x=D(player+0x61u);mask=B(row-4u);B(x+0x45u)=mask;
            x=D(player+0x61u);n=W(row+3u);W(x+0xbu)=n;
            x=D(player+0x61u);n=W(row+5u);W(x+9u)=n;
            x=D(player+0x61u);n=W(row+7u);W(x+0xdu)=n;
            x=D(packet+4u);y=D(0x00ae23b4);b=y+x*0x245u;
            x=D(packet);a=y+x*0x245u;x=D(a+0x53u);y=D(b+0x53u);
            if(x!=y||B(row-8u)==0)continue;
            x=D(player+0x61u);ptr=D(0x00b74350);t=cm_fn_007c1e30(ptr,x,6,0xb,6.5f);
            ratio=CM_GLOBAL(volatile float,0x00968970);
            if(!(t<ratio)&&!(t>ratio))continue;
            ptr=D(player+0x1au);if(!ptr)continue;
            x=D(club+0x53u);if(!x)continue;
            mask=B(ptr+0x76u);if(mask==B(x+0x76u))continue;
            x=D(player+0x61u);if(!x)continue;
            if((s16)W(x+0xbu)<=0x1676)continue;
            if((s8)B(player+0x57u)<=10)continue;
            if((s8)B(player+0x59u)>=15)continue;
            if((s8)B(player+0x5cu)>=10 && (s8)B(player+0x5du)>=10)continue;
            x=D(ptr);y=D(0x009cf300);
            if(x==y) {x=cm_fn_0090cfc0(3);if(!x)goto mark;}
            x=cm_fn_0090cfc0(10);if(x)continue;
mark:
            x=cm_fn_004cf5f0(D(0x00ae1050),player,club);if(!x)continue;
            y=cm_fn_008676f0(x);if(!(y&0x20))cm_fn_00867560(x,0x20);
        }
        delta_a=delta_b=delta_c=0;
        x=D(team+0x184u);
        if((s32)x>-1) {
            y=D(0x00ae23bc);original_player=y+x*0x6eu;
            if(original_player) {
                ptr=D(original_player+0x69u);
                if(ptr) {
                    n=W(ptr+0xau);delta_a=(u16)(W(team+0x188u)-n);W(ptr+0xau)=(u16)(n+delta_a);
                    ptr=D(original_player+0x69u);delta_b=W(team+0x18au);n=W(ptr+8u);delta_b=(u16)(delta_b-n);W(ptr+8u)=(u16)(n+delta_b);
                    ptr=D(original_player+0x69u);delta_c=W(team+0x18cu);n=W(ptr+0xcu);delta_c=(u16)(delta_c-n);W(ptr+0xcu)=(u16)(n+delta_c);
                }
            }
            for(c=0;c<6;c++) {
                if(!c)other=D(club+0xd3u);else other=D(club+0x19fu+(c-1u)*4u);
                if(!other||other==original_player)continue;
                ptr=D(other+0x69u);if(!ptr)continue;
                t=(long double)(s16)delta_a;t*=Q(0x009689e0);converted=(long long)t; /* __ftol VA006B738D */
                W(ptr+0xau)=(u16)(W(ptr+0xau)+(u16)converted);
                target=D(other+0x69u);n=W(target+0xau);a=n<1?1:n>10000?10000:(u32)(s32)n;
                W(target+0xau)=(u16)a;
                ptr=D(other+0x69u);t=(long double)(s16)delta_b;t*=Q(0x009689e0);converted=(long long)t; /* __ftol VA006B73D3 */
                W(ptr+8u)=(u16)(W(ptr+8u)+(u16)converted);
                target=D(other+0x69u);n=W(target+8u);a=n<1?1:n>10000?10000:(u32)(s32)n;W(target+8u)=(u16)a;
                ptr=D(other+0x69u);t=(long double)(s16)delta_c;t*=Q(0x009689e0);converted=(long long)t; /* __ftol VA006B741B */
                W(ptr+0xcu)=(u16)(W(ptr+0xcu)+(u16)converted);
                target=D(other+0x69u);n=W(target+0xcu);a=n<1?1:n>10000?10000:(u32)(s32)n;W(target+0xcu)=(u16)a;
            }
        }
        n=W(team+0x18eu);W(club+0x80u)=n;
    }
    return (club&0xffffff00u)|2u;
}
