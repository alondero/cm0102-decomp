#include "cmre.h"
#define B(a) (*(volatile u8 *)(a))
#define S(a) (*(volatile s8 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define D(a) (*(volatile u32 *)(a))
#define F(a) (*(volatile float *)(a))
extern u32 CM_THISCALL cm_fn_006bb950(u32,u32,u32);
extern u8 CM_THISCALL cm_fn_006cfc60(u32,u32,u32,u32,u32,u32,u32,u32);
extern void CM_THISCALL cm_fn_006d6f70(u32,u32,u32,u32,u32,u32,u32,u32);
extern void CM_THISCALL cm_fn_006f46b0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007164b0(u32,s8,u32);
extern unsigned long long CM_CDECL cm_fn_0090cfc0(u32,u32,u32);

u8 CM_THISCALL cm_fn_00716630(u32 self, s8 mode)
{
    u32 base, side, player=0, msg=0xffffffff, code=0, i, j, team, p, r;
    u8 scratch[4];
    s8 index=0;
    long double value;
    float floorval;
    scratch[2]=0;
    base=D(self+0x19e)+0x215e + (u32)((S(self+0x103)+S(self+0x102)*12)*90);
    side=B(self+0x27)==0;
    if (S(base+0x58+side)<=0) return 0;
    do {
        if ((u16)msg != 0xffff) break;
        p=D(base+(side*11+(s32)index)*4);
        if (p && D(p+0x69) && D(p+0x6d)) {
            if ((u8)cm_fn_007164b0(self,4,p)) {
                msg=0x20e3; code=0x31;
                cm_fn_006bb950(D(self+0x19e),B(p+0x27),0x40100000);
                goto major;
            }
            if ((u8)cm_fn_007164b0(self,3,p)) {
                team=D(D(self+0x69)+0x1a);
                r=B(team+0x77)==8?0:700;
                r=(u32)cm_fn_0090cfc0(r,team,1000-r);
                if ((s32)r<10) {
                    msg=0x2108; code=0x61;
                    cm_fn_006bb950(D(self+0x19e),B(p+0x27),0x40200000);
                } else {
                    r=(u32)cm_fn_0090cfc0(0,0,10);
                    if (r<=8) {
                        u8 m=B(0x716c78+r);
                        u32 target=D(0x716c70+(u32)m*4);
                        if (target==0x716860) {msg=0x20e4;code=0x32;}
                        else {msg=0x20e1;code=0x30;}
                    } else {msg=0x20e5;code=0x2f;}
                    cm_fn_006bb950(D(self+0x19e),B(p+0x27),0x40000000);
                }
major:
                team=D(p+0x69);
                value=((long double)CM_GLOBAL(double,0x968938)-(long double)S(team+0x5d)*CM_GLOBAL(double,0x96d330))*F(p+0x1a2);
                F(p+0x1a2)=(float)value;
                floorval=(float)((25-S(team+0x5d))*2);
                if (!(value>floorval)) value=floorval;
                F(p+0x1a2)=(float)value;
                if (value>CM_GLOBAL(double,0x9688a8)) D(p+0x1a2)=0x42c80000;
                player=p;
            } else if ((u8)(r=cm_fn_007164b0(self,2,p))) {
                r=(u32)cm_fn_0090cfc0(r,0,2);
                if (r || W(self+0x198)==0x5f) {msg=0x20e2;code=0x60;}
                else {msg=0x210b;code=0x5f;}
                player=p;
                cm_fn_006bb950(D(self+0x19e),B(p+0x27),0x3f933333);
                team=D(p+0x69);
                value=((long double)CM_GLOBAL(double,0x96d3e8)-(long double)S(team+0x5d)*CM_GLOBAL(double,0x967880))*F(p+0x1a2);
                F(p+0x1a2)=(float)value;
                floorval=(float)((25-S(team+0x5d))*2);
                if (!(value>floorval)) value=floorval;
                F(p+0x1a2)=(float)value;
                if (value>CM_GLOBAL(double,0x9688a8)) D(p+0x1a2)=0x42c80000;
            }
        }
        index=(s8)(index+1);
        side=B(self+0x27)==0;
    } while (index<S(base+0x58+side));
    if (!player) return 0;
    W(self+0x198)=(u16)(s16)(s8)code;
    if (mode>1 && (u32)cm_fn_0090cfc0(0,0,5)!=0) {
        team=D(self+0x19e);
        r=(u32)cm_fn_0090cfc0(0,0,3);
        if (S(team+0x1a3)>=(s32)(r+2)) {
            cm_fn_006d6f70(D(team+0x8f7a),0x210c,team,0xffffffff,B(self+0x27),0xffffffff,0xffffffff,0);
            mode=-50;
            goto after_message;
        }
    }
    if (mode>=0) {
        team=D(self+0x19e);
        cm_fn_006d6f70(D(team+0x8f7a),msg,team,B(self+0x2e),B(self+0x27),B(player+0x2e),B(player+0x27),0);
    }
after_message:
    for (i=0;i<2;i++) {
        p=D(self+0x19e)+0x4796+i*0x22d8;
        for (j=0;j<20;j++,p+=0x1be) {
            if (S(p+0x19)>=0 && S(p+0x19)<=10 && S(p+0x2b)<2 && p!=self && (u8)cm_fn_007164b0(p,1,0))
                cm_fn_006f46b0(p,B(self+0x102),B(self+0x103));
        }
    }
    if (cm_fn_006cfc60(D(self+0x19e),self,player,code,0xffffffff,(u32)&scratch[3],(u32)&scratch[1],(u32)&scratch[2])) {
        s8 found=-1, k=0;
        team=D(self+0x19e);
        while (k<S(team+0x1a3)) {
            p=team+(s32)k*11;
            if (D(p+0xb1)==self) {B(p+0xb9)+=scratch[1];found=k;}
            team=D(self+0x19e);
            k=(s8)(k+1);
        }
        if (found==-1) {
            p=team+(s32)S(team+0x1a3)*11;
            D(p+0xb1)=self;
            team=D(self+0x19e); p=team+(s32)S(team+0x1a3)*11;
            D(p+0xb5)=player;
            team=D(self+0x19e); p=team+(s32)S(team+0x1a3)*11;
            B(p+0xbb)=scratch[3];
            team=D(self+0x19e); p=team+(s32)S(team+0x1a3)*11;
            B(p+0xba)=(u8)code;
            team=D(self+0x19e); p=team+(s32)S(team+0x1a3)*11;
            B(p+0xb9)=scratch[1];
            team=D(self+0x19e); B(team+0x1a3)++;
        }
    }
    if (((mode>=0 && mode<10) || mode < -40) && S(D(self+0x19e)+0x1a3)<5)
        cm_fn_00716630(player,(s8)(mode+1));
    return 1;
}
