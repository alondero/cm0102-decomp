#include "cmre.h"
#include "cmre_x87.h"
/* Original preferred-image VAs below refer to CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   59e0d7/d9 and 59e324/329,59e4d8/4dd set the this receivers.
   59e3d0..3d9,59e3e9..3f0,59e541..54a,59e56c..573 forward
   the entry ECX and push four arguments to 59f300. */
extern u8 CM_THISCALL cm_fn_005986b0(u32);
extern long long CM_THISCALL cm_fn_00598800(u32,u32,u32,u32,u32,u32);
extern void CM_THISCALL cm_fn_0059f300(u32,u32,u32,u32,s8);
/* Each 90cfc0 call has exactly one live argument push, e.g.
   0059e1c4/1c7 and 0059e1d2/1d3, cleaned together at 0059e1d8. */
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
#define M(t,a) CM_GLOBAL(volatile t,(u32)(a))
#define D(a) M(u32,a)
#define F(a) ((long double)CM_GLOBAL(double,a))
#define RNG(b) do { ax=cm_fn_0090cfc0((u32)(b)); } while(0)

u32 CM_THISCALL cm_fn_0059e050(u32 self,u32 p,u32 q)
{
    u32 ax,dx,t,club,org,lo,hi,v,old1,old2,bx,kind;
    s32 rating,skill;
    u8 state;
    long double factor,z;
    long long cv;
    int linked;
    if(p && (t=D(p+0x53))!=0) {
        ax=D(t);
        if(ax==D(0x9cf510)) return 0;
        if(ax==D(0x9cf2dc)) return 0;
        if(ax==D(0x9cf3e4)) return 0;
        if(ax==D(0x9cf408)) return 0;
        if(ax==D(0x9cf2c0)) return 0;
        if(ax==D(0x9cf4a8)) return 0;
        if(ax==D(0x9cf244)) return 0;
        state=M(u8,p+0x64);
        if(state==2 || state==3) return 0;
    }
    state=cm_fn_005986b0(q);
    if(state==0xfe) {
        factor=M(s16,p+0x80)>0xea6 ? F(0x968fd0):F(0x96a730);
    } else if(state==0xff) {
        rating=M(s16,p+0x80);
        if(rating>0x1482) factor=F(0x967880);
        else if(rating>0xcb2) factor=F(0x96a730);
        else factor=F(0x967878);
    } else if(state) factor=F(0x96a350);
    else {
        rating=M(s16,p+0x80);
        if(rating>0x109a) {
            factor=F(0x967878);
            if(rating<0x186a) {
                club=D(p+0xbf);
                if(club && (org=D(club+0x69))!=0 &&
                   M(s8,org+0x20)>15 && M(s8,club+0x57)>15)
                    factor=F(0x967888);
            }
        } else {
            club=D(p+0xbf);
            if(club && (org=D(club+0x69))!=0 &&
               M(s8,club+0x57)>15 && M(s8,org+0x20)>15)
                factor=F(0x96a740);
            else factor=F(0x96a738);
        }
    }
    club=D(p+0xbf);
    linked=0;
    if(club) { org=D(club+0x69); linked=org!=0; }
    if(linked) {
        /* 0059e1bf __ftol */
        cv=(long long)(F(0x968dd8)/factor);
        ax=(u32)cv; dx=(u32)((unsigned long long)cv>>32);
        RNG(ax); bx=ax;
        ax=(u32)(s32)M(s8,org+0xf);
        RNG(ax);
        if((s32)ax<=(s32)bx) return 0;
        club=D(p+0xbf); lo=D(q); hi=D(q+4);
        dx=D(club+0x69);
        ax=(u32)(s32)M(s8,dx+0x20)*1000000u;
        RNG(ax);
        lo=0u-lo; hi=0u-(hi+(lo!=0));
        dx=(s32)ax<0 ? 0xffffffffu:0;
        if((s32)dx<(s32)hi || (dx==hi && ax<=lo)) return 0;
        club=D(p+0xbf);
        rating=M(s16,p+0x80);
        dx=D(club+0x69);
        skill=M(s8,dx+0x20);
        if(rating>=skill*750) return 0;
        /* 0059e26a _CIpow; 0059e285 __ftol */
        z=cm_x87_CIpow((long double)skill,F(0x968950));
        z=z*F(0x968930);
        z=z+F(0x96a5f0);
        z=z*(long double)rating;
        z=z+F(0x9688d8);
        cv=(long long)z;
        ax=(u32)cv; dx=(u32)((unsigned long long)cv>>32);
        lo=D(q); v=ax; ax=D(q+4);
        if((s32)ax<0) v-=lo; else v+=lo;
        if(M(s16,p+0x80)<2000) {
            dx=(u32)rating*(u32)rating;
            ax=dx+1500000u;
            if((s32)v>(s32)ax) v=ax;
        }
        ax=D(p+0x53);
        if(ax) {
            ax=D(ax); dx=D(0x9cf4a8);
            if(ax==dx || ax==D(0x9cf408) || ax==D(0x9cf2c0))
                v=(u32)((s32)v/3);
        }
        old1=D(q+0x154); old2=D(q+0xb4);
        old1+=v; hi=D(q+4); old2+=v;
        D(q+0x154)=old1; D(q+0xb4)=old2;
        ax=v+lo; dx=((s32)v<0?0xffffffffu:0)+hi+(ax<v);
        D(q)=ax; D(q+4)=dx;
    } else {
        /* 0059e426 __ftol */
        cv=(long long)(F(0x96a728)/factor);
        ax=(u32)cv; dx=(u32)((unsigned long long)cv>>32);
        RNG(ax);
        if(M(s16,p+0x80)<=(s32)ax) return 0;
        RNG(8);
        if(ax) return 0;
        ax=(u32)(s32)M(s16,p+0x80)+1000u;
        v=ax*1000u;
        ax=D(p+0x53);
        if(ax) {
            ax=D(ax); t=D(0x9cf4a8);
            if(ax==t || ax==D(0x9cf408) || ax==D(0x9cf2c0))
                v=(u32)((s32)v/3);
        }
        dx=D(q+0x154); bx=D(q+0xb4); lo=D(q);
        dx+=v; ax=v; D(q+0x154)=dx;
        dx=(s32)v<0?0xffffffffu:0;
        bx+=v; t=lo+ax;
        D(q+0xb4)=bx; D(q)=t; hi=D(q+4);
        D(q+4)=hi+dx+(t<lo);
    }
    cv=cm_fn_00598800(q,0,0,0,1,0);
    ax=(u32)cv; dx=(u32)((unsigned long long)cv>>32);
    bx=ax; state=M(u8,p+0x82); ax=(ax&0xffffff00u)|state;
    if((s32)bx<=0 && !state) { kind=3; goto emit; }
    if(state) {
        RNG(4);
        if(!ax) { kind=5; goto emit; }
    }
    RNG(4);
    if(!ax) {
        if(!linked) { kind=6; goto emit; }
        ax=(u32)(s32)M(s16,p+0x80);
        t=ax*ax;
        /* 0059e386 __ftol */
        cv=(long long)((long double)(s32)t*F(0x968e80));
        ax=(u32)cv; dx=(u32)((unsigned long long)cv>>32);
        if((s32)v<(s32)ax && (s32)v>50000) {kind=6; goto emit;}
    }
    state=M(u8,p+0x82); ax=(ax&0xffffff00u)|state;
    if(!state && M(s16,p+0x80)>0x128e) {
        RNG(4);
        if(!ax) {
            if(linked) {
                ax=D(p+0x53);
                if(!ax) goto normal;
                dx=D(ax); ax=D(0x9cf360);
                if(dx==ax) goto normal;
            }
            cm_fn_0059f300(self,p,v,bx,7);
            M(u8,p+0x82)=1;
            goto done;
        }
    }
normal:
    kind=1;
emit:
    cm_fn_0059f300(self,p,v,bx,(s8)kind);
done:
    if(linked) {
        ax=D(p+0xbf); t=D(ax+0x69);
        state=M(u8,t+0x20);
        if((s8)state>5) M(u8,t+0x20)=state-1;
    }
    M(u8,q+0x165)=0;
    return 1;
}
