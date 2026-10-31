/* WIP: unaccepted. First canonical failure case97: original callees emit
   differing decimal stack addresses into AE24D0 text at both optimization
   levels. Lifetime one failed invocation; resolve physical-call/text evidence
   before retrying. Full replay and source identity are in continuation139. */
/* Preferred-image VA006C54A0/base00400000, target SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/RET0. B=L-10 after SUB218/four saves; scratch begins B10.
   Original helpers remain explicit boundaries. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define W(a) CM_GLOBAL(volatile u16,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
#define S(a) ((s32)(s8)B(a))
#define H(a) ((s32)(s16)W(a))
#define F(a) CM_GLOBAL(volatile float,(u32)(a))
#define Q(a) CM_GLOBAL(volatile double,(u32)(a))
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_THISCALL cm_fn_006df460(u32);
extern u8 CM_THISCALL cm_fn_00714d30(u32);
extern u32 CM_THISCALL cm_fn_00716450(u32,float);
extern u32 CM_THISCALL cm_fn_005fd210(u32,u32);
extern u32 CM_THISCALL cm_fn_005fd480(u32,u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_THISCALL cm_fn_006e1090(u32,u8);
extern u32 CM_THISCALL cm_fn_006bbb60(u32,u32);
extern u32 CM_THISCALL cm_fn_006bbf60(u32);
extern u32 CM_THISCALL cm_fn_006d6f70(u32,u32,u32,u8,u8,u8,u8,u32);
extern u32 CM_THISCALL cm_fn_005b50d0(u32,u32);
extern u32 CM_THISCALL cm_fn_005b5290(u32,u8,u32,u32);
extern u32 CM_THISCALL cm_fn_006d10a0(u32,u8);
extern u32 CM_THISCALL cm_fn_006ce310(u32,u8,u32,u32,u32,u32,u32,u8);
extern u32 CM_THISCALL cm_fn_0069eb50(u32,u32);
extern u32 CM_CDECL cm_fn_0084b8b0(u32,u32);
extern u32 CM_THISCALL cm_fn_006c4290(u32);
extern u32 CM_THISCALL cm_fn_006c4470(u32);
extern u32 CM_THISCALL cm_fn_006c4ec0(u32);
extern u32 CM_THISCALL cm_fn_006c50d0(u32);
extern u32 CM_THISCALL cm_fn_006c4820(u32);
extern u32 CM_THISCALL cm_fn_006c51e0(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern u32 CM_CDECL cm_fn_005cc7d0(void);
extern u32 CM_CDECL cm_fn_005a8f60(void);
extern void CM_CDECL cm_fn_00945934(u32) __attribute__((noreturn));

static u32 message(u32 self,u32 id,u8 player,u8 side,u8 a,u8 b,u32 extra)
{
    u32 receiver=D(self+0x8f7a);
    return cm_fn_006d6f70(receiver,id,self,player,side,a,b,extra);
}
static u32 score(u32 self,u8 side,u32 pool)
{
    return cm_fn_006ce310(self,side,pool+2,pool+1,pool+3,pool+0xf,pool+0x11,0);
}
static u32 string_equal(u32 a,u32 b)
{
    u8 x,y;
    for(;;){
        x=B(a);y=B(b);if(x!=y)return 0;if(x==0)return 1;
        x=B(a+1);y=B(b+1);if(x!=y)return 0;
        a+=2;b+=2;if(x==0)return 1;
    }
}

u32 CM_THISCALL cm_fn_006c54a0(u32 self)
{
    volatile u8 scratch[0x218];
    u32 pool=(u32)scratch;
    u32 a,c,d,p,object,side,index,target,formation,flags,remaining,id,extra,other;
    u8 flag,mode,minute,seen,old,changed;
    s32 n,x,y;
    long double value;
    float amount;
    B(self+0x1c5)=0;D(self+0x4782)=0;D(self+0x4786)=0;
    D(self+0x477e)=0;B(self+0x8eaa)=0;B(self+0x8ea9)=0;
    D(self+0xf5aa)=0;D(self+0xf5a6)=0;D(self+0xf576)=0;
    B(pool+0x12)=0;B(pool+0x13)=0;
    D(self+0xf57a)=0;W(self+0x8ed6)=0;D(self+0x91de)=0;
    B(self+0x1cd)=0;D(self+0x4761)=0;D(self+0x4765)=0;
    B(self+0x4769)=0;B(self+0x475a)=0;flag=B(self+0x1a8);
    B(self+0x8ea6)=0xff;B(self+0x8eb9)=0xff;W(self+0x475f)=0xffff;
    B(self+0x475b)=0xff;B(self+0x475c)=0xff;B(self+0x475d)=0xff;B(self+0x475e)=0xff;
    if(flag!=0)return flag;
    if(B(self+0x8eb2)==8){
        B(pool+4)=1;D(self+0x476a)=0xc47a0000;D(self+0x476e)=0xc47a0000;B(pool+5)=1;
    }else{
        n=H(self+0x8ed0);x=H(self+0x1d2);d=(u32)n-(u32)x;
        y=H(self+0x1d4);B(pool+4)=(s32)d>0xa5;
        B(pool+5)=(s32)((u32)n-(u32)y)>0xa5;
    }
    n=H(self+0x8ed0);minute=(u8)(n/11);B(pool+0x10)=minute;
    if((s8)minute>0x3c && cm_fn_0090cfc0(6)==0){
        seen=0;B(pool+6)=0;
        do{
            if(seen!=0)break;
            side=(u32)S(pool+6);p=self+side*0x22d8u+0x4796;
            for(index=0;index<20;index++,p+=0x1be){
                if(seen!=0)break;
                if(S(p+0x19)>=0){
                    value=F(p+0x4d);
                    if(!(value>=(long double)F(0x96d30c)) && B(p+0x10d)==0){
                        B(pool+0x14)=B(p+0x2e);
                        a=cm_fn_00714d30(p);B(pool+8)=(u8)a;
                        d=B(pool+8);a=B(pool+0x14);
                        message(self,0x21cf,(u8)a,(u8)d,0xff,0xff,0);seen=1;
                    }
                }
            }
            B(pool+6)=(u8)(B(pool+6)+1);
        }while(S(pool+6)<2);
    }
    for(side=0;side<2;side++){
        if(B(self+0x8eb2)==5)break;
        if(B(self+0x1a8)!=0)break;
        a=cm_fn_006df460(self+side*0xd0u+0x9032);
        old=B(self+side+0x1ab);
        if((u8)a!=old || H(self+0x8ed0)<=1){
            B(pool+0x12+side)=1;B(pool+6+side)=old;B(self+side+0x1ab)=(u8)a;
        }
    }
    flag=B(self+0x8eae);
    if(flag!=0xff){
        mode=B(self+(u32)(s32)(s8)flag+0x1ab);
        if(mode==3 || mode==4){
            a=cm_fn_0090cfc0(10);n=(s8)(u8)a;D(pool+0x14)=(u32)n;
            W(self+0x8ed0)=(u16)(W(self+0x8ed0)+(u16)n);
            a=cm_fn_0090cfc0((u32)(n/2));minute=B(self+0x8eb7);
            minute=(u8)(minute+(u8)a);flag=B(self+0x8eae);B(self+0x8eb7)=minute;
            if(B(self+(flag==0)+0x1ab)==1){
                p=self+(flag==0)*0x22d8u+0x4796;
                for(remaining=20;remaining!=0;remaining--,p+=0x1be){
                    a=D(p+0x69);
                    if(a!=0){
                        value=(long double)(s32)D(pool+0x14);
                        value*=Q(0x96d3d8);value+=Q(0x967890);amount=(float)value;
                        cm_fn_00716450(p,amount);
                    }
                }
            }
        }
    }
    p=D(self+0x904d);
    if(p!=0){
        d=D(0xae2364);a=D(p);
        if((s32)a>=(s32)(d-16) && (s32)a<(s32)d && B(p+0x5f)!=0 &&
           cm_fn_005fd210(D(0xb63c98),p)==0){
            if(B(self+0x904b)==0)goto first_ready;
            d=D(self+0x4792);a=D(d+0x1c);
            if(cm_fn_0053b900(a)!=0)goto first_ready;
            a=D(self+0x904d);if(cm_fn_005fd480(D(0xb63c98),a)!=0)goto first_ready;
        }
    }
    a=cm_fn_0090cfc0(0x32);minute=B(pool+0x10);
    if((s32)(a+10)<(s8)minute){
        x=H(self+0x8ecc);y=H(self+0x8ed0);
        if((s32)((u32)y-(u32)x)>0x37){
            cm_fn_006e1090(self+0x9032,0);W(self+0x8ecc)=W(self+0x8ed0);
            cm_fn_006bbb60(self,0);cm_fn_006bbf60(self);
        }
    }
    goto first_complete;
first_ready:
    minute=B(pool+0x10);
first_complete:
    n=H(self+0x8ed4);
    if((n==0x3de && (s8)minute>0x54 && (s8)minute<0x5b && B(self+0x8d49)==0) ||
       (n==0x1ef && (s8)minute>0x27 && (s8)minute<0x2e && B(self+0x8d48)==0)){
        other=n==0x1ef?0x8d48:0x8d49;
        n=S(self+0x8eb7)/11;if(n<=2)n=(s32)(((u32)n&0xffffff00u)|2);
        extra=(u32)(s32)(s8)n;flag=B(self+0x8eae);
        message(self,0x21c1,flag,0,0,0,extra);
        B(self+other)=1;
    }
    p=D(self+0x911d);
    if(p!=0){
        d=D(0xae2364);a=D(p);
        if((s32)a>=(s32)(d-16) && (s32)a<(s32)d && B(p+0x5f)!=0 &&
           cm_fn_005fd210(D(0xb63c98),p)==0){
            if(B(self+0x911b)==0)goto second_complete;
            a=D(self+0x4792);c=D(a+0x20);
            if(cm_fn_0053b900(c)!=0)goto second_complete;
            a=D(self+0x911d);if(cm_fn_005fd480(D(0xb63c98),a)!=0)goto second_complete;
        }
    }
    a=cm_fn_0090cfc0(0x32);
    if((s32)(a+10)<(s8)minute){
        x=H(self+0x8ece);y=H(self+0x8ed0);
        if((s32)((u32)y-(u32)x)>0x37){
            cm_fn_006e1090(self+0x9102,0);W(self+0x8ece)=W(self+0x8ed0);
            cm_fn_006bbb60(self,1);cm_fn_006bbf60(self);
        }
    }
second_complete:
    B(pool+8)=0;
    do{
        side=(u32)S(pool+8);object=self+side*0xd0u;p=D(object+0x904d);D(pool+0x14)=object;
        if(p!=0){
            d=D(0xae2364);a=D(p);
            if((s32)a>=(s32)(d-16) && (s32)a<(s32)d && B(p+0x5f)!=0 &&
               cm_fn_005fd210(D(0xb63c98),p)==0){
                if(B(object+0x904b)==0)goto side_complete;
                a=D(self+0x4792);a=D(a+(B(pool+8)==0?0x1c:0x20));
                if(cm_fn_0053b900(a)!=0)goto side_complete;
                a=D(object+0x904d);if(cm_fn_005fd480(D(0xb63c98),a)!=0)goto side_complete;
            }
        }
        if(B(pool+0x12+side)==0)goto side_complete;
        a=D(object+0x905d);if(a==0)a=0xa15934;
        changed=(u8)!string_equal(a,0xa1594c);
        if(changed){a=D(object+0x905d);if(a==0)a=0xa15934;changed=(u8)!string_equal(a,0xa1593c);}
        mode=B(self+side+0x1ab);index=(u32)(s32)(s8)mode-1;
        if(index>4)goto side_complete;
        target=D(0x6c69bc+index*4);
        formation=self+side*0x18e3u+0x91e2;
        switch(target){
        case 0x6c5b71:
            c=D(object+0x9051);flags=0x10000;
            if(c!=0){value=F(self+side*4+0x476a);if(!(value>=(long double)Q(0x968e18)) && S(c+0xe)<=6)flags=0x20000;}
            if(!changed)flags=0x20000;
            cm_fn_005b50d0(formation,flags);cm_fn_005b50d0(formation,0x800);cm_fn_005b50d0(formation,0x2000);
            other=D(pool+8);
            if(H(self+0x8ed0)<=1){message(self,0x2137,0xff,(u8)other,0xff,0xff,0);cm_fn_006d10a0(self,(u8)other);}
            else{
                if(B(pool+4+side)){message(self,0x2139,0xff,(u8)other,0xff,0xff,0);W(self+side*2+0x1d2)=W(self+0x8ed0);}
                cm_fn_006d10a0(self,(u8)other);
            }
            goto side_complete;
        case 0x6c5c67:
            a=D(object+0x9051);
            if(a!=0){
                flag=B(self+side+0x1ad);old=B(a+0xe);n=flag==0 || (s8)flag<0?15:18;
                if((s8)old>n && changed)flags=0x10000;
                else if((s8)old<=5 && flag!=0){
                    value=F(self+side*4+0x476a);
                    if(!(value>(long double)Q(0x967890))){
                        cm_fn_005b50d0(formation,0x8000);flag=B(self+side+0x1ad);
                        c=D(pool+0x14);d=D(c+0x9051);n=S(d+0x13);
                        flags=n>=((s8)flag<=0?4:7)?0x800:0x1000;goto apply_second_flag;
                    }
                    flags=0x20000;
                }else flags=0x20000;
            }else flags=0x20000;
            cm_fn_005b50d0(formation,flags);flags=0x800;
apply_second_flag:
            cm_fn_005b50d0(formation,flags);cm_fn_005b50d0(formation,0x2000);
            if(H(self+0x8ed0)>1){
                old=B(pool+6+side);
                if(old==3 || old==4 || old==5)message(self,0x2138,0xff,B(pool+8),0xff,0xff,0);
                else if(H(self+0x8ed0)>1 && B(pool+6+side)==1 && B(pool+4+side)){
                    message(self,0x213a,0xff,B(pool+8),0xff,0xff,0);W(self+side*2+0x1d2)=W(self+0x8ed0);
                }
            }
            break;
        case 0x6c5e26:
        case 0x6c5fba:
            a=D(object+0x9051);
            if(a!=0){
                old=B(a+0xe);
                if(target==0x6c5fba && (s8)old>16 && changed)flags=0x10000;
                else flags=(s8)old>(target==0x6c5e26?16:12)?0x20000:0x8000;
                cm_fn_005b50d0(formation,flags);flag=B(self+side+0x1ad);
                a=D(object+0x9051);n=S(a+0x13);flags=n<((s8)flag<=0?10:13)?0x1000:0x800;
            }else{
                cm_fn_005b50d0(formation,target==0x6c5e26?0x8000:0x20000);flags=0x1000;
            }
            cm_fn_005b50d0(formation,flags);cm_fn_005b50d0(formation,0x2000);
            if(target==0x6c5fba){
                if(B(pool+4+side)){message(self,0x213c,0xff,B(pool+8),0xff,0xff,0);W(self+side*2+0x1d2)=W(self+0x8ed0);}
            }else{
                if(H(self+0x8ed0)<=1)message(self,0x2136,0xff,B(pool+8),0xff,0xff,0);
                else goto possible_change;
            }
            break;
        case 0x6c6114:
            a=D(object+0x9051);flags=0x8000;
            if(a!=0 && S(a+0xe)>18)flags=0x20000;
            cm_fn_005b50d0(formation,flags);cm_fn_005b50d0(formation,0x800);cm_fn_005b50d0(formation,0x4000);
            if(H(self+0x8ed0)<=1){message(self,0x2135,0xff,B(pool+8),0xff,0xff,0);break;}
possible_change:
            old=B(pool+6+side);
            if((old==2 || old==1) && B(pool+4+side)){
                other=D(pool+8);a=score(self,(u8)other,pool);
                message(self,(s8)(u8)a>0?0x213b:0x213d,0xff,(u8)other,0xff,0xff,0);
                W(self+side*2+0x1d2)=W(self+0x8ed0);
            }
            break;
        default:__builtin_unreachable();
        }
        p=self+side*0x22d8u+0x47af;
        for(remaining=20;remaining!=0;remaining--,p+=0x1be){flag=B(p);if((s8)flag>=0)cm_fn_005b5290(formation,flag,0x400000,0);}
side_complete:
        B(pool+8)=(u8)(B(pool+8)+1);
    }while(S(pool+8)<2);
    if(B(self+0x8ebb)==0)goto dispatch;
    mode=B(self+0x8eb2);if(mode==3 || mode==5)goto dispatch;
    if(mode==6){
        flag=B(self+0x8eae);
        if(flag==0 || flag==1){
            x=S(self+0x8ea7)-4;x=x<0?-x:x;y=S(self+0x8ea8)-(flag==0?11:0);y=y<0?-y:y;
            old=(u8)x;seen=(u8)y;
            if((s8)old>=0 && (s8)old<=8 && (s8)seen>=0 && (s8)seen<=11)value=F(0xa15e44+((u32)(s32)(s8)seen*9u+(u32)(s32)(s8)old)*4u);
            else value=F(0x96d2dc);
            if(!(value>=(long double)Q(0x9688a0)))goto dispatch;
        }
    }
    score(self,0,pool);flag=B(pool+2);old=B(pool+1);
    if((s8)flag<(s8)old || (flag==old && B(pool+3)==0)){
        if(cm_fn_0090cfc0(10)==0){
            a=D(self+0x904d);if(cm_fn_0084b8b0(a,0x9b63a8)!=0){id=0x212a;side=1;goto send;}
        }
        old=B(pool+1);flag=B(pool+2);
    }
    a=cm_fn_0090cfc0((u32)((s32)(s8)old-(s32)(s8)flag+1));flag=B(pool+2);old=B(pool+1);
    if(a!=0 && ((s8)flag<(s8)old || (flag==old && B(pool+3)==0)))goto select_first;
    a=cm_fn_0090cfc0((u32)((s32)(s8)flag-(s32)(s8)old+5));if(a==0)goto select_first;
    flag=B(pool+2);old=B(pool+1);
    a=cm_fn_0090cfc0((u32)((s32)(s8)flag-(s32)(s8)old+1));flag=B(pool+2);old=B(pool+1);
    if(a!=0 && ((s8)flag>(s8)old || (flag==old && B(pool+3)!=0)))goto select_second;
    a=cm_fn_0090cfc0((u32)((s32)(s8)old-(s32)(s8)flag+5));if(a!=0)goto dispatch;
select_second:
    side=0;other=0x1c;object=0x20;goto select_team;
select_first:
    side=1;other=0x20;object=0x1c;
select_team:
    a=D(self+0x4792);
    if(side!=0){c=D(0xb67a44);p=D(a+object);}
    else{p=D(a+object);c=D(0xb67a44);}
    if((u8)cm_fn_0069eb50(c,p)!=0){
        c=D(p+0x53);if(B(c+0x77)==8){id=0x2129;side^=1;goto send;}
    }
    for(index=0;index<7;index++){
        static const u32 globals[7]={0x9d0230,0x9d0234,0x9d0238,0x9d0268,0x9d023c,0x9d0240,0x9d0244};
        static const u32 messages[7]={0x2127,0x2128,0x2163,0x2166,0x212b,0x212c,0x212d};
        if(cm_fn_0090cfc0(2)==0){
            d=D(self+0x4792);a=D(d+other);
            if(side!=0){c=D(a);a=D(globals[index]);}
            else{p=D(globals[index]);c=D(a);a=p;}
            if(c==a){id=messages[index];goto send;}
        }
    }
    a=D(self+0x4792);c=D(a+other);d=D(c+0x53);index=(u32)S(d+0x76)-4;
    if(index>10)goto dispatch;
    if(side!=0){a=B(0x6c69e0+index);target=D(0x6c69d0+a*4);}
    else{a=B(0x6c69fc+index);target=D(0x6c69ec+a*4);}
    if(target==0x6c68d6)goto dispatch;
    if(target==0x6c6635 || target==0x6c68bb)id=0x2118;
    else{
        a=cm_fn_0090cfc0(2);
        if(target==0x6c6603 || target==0x6c686d)id=a==0?0x216b:0x216a;
        else id=a==0?0x2167:0x216a;
    }
send:
    message(self,id,0xff,(u8)side,0xff,0xff,0);
dispatch:
    index=(u32)S(self+0x8eb2)-1;
    if(index<=7){
        target=D(0x6c6a08+index*4);
        switch(target){
        case 0x6c68f3:return index;
        case 0x6c68ec:return cm_fn_006c4290(self);
        case 0x6c68fb:return cm_fn_006c4470(self);
        case 0x6c690a:return cm_fn_006c4ec0(self);
        case 0x6c6919:return cm_fn_006c50d0(self);
        case 0x6c6928:return cm_fn_006c4820(self);
        case 0x6c6937:return cm_fn_006c51e0(self);
        default:__builtin_unreachable();
        }
    }
    cm_fn_00944cff(0xa15884,0,0,pool+0x118,pool+0x18);
    a=cm_fn_0090d130();cm_fn_00944c9f(0xae24d0,0x9870e8,a,pool+0x118,pool+0x18,0x29ab);
    cm_fn_005e8290(0x9870e0,0xae24d0,0);D(0xb67a34)=0;
    cm_fn_005cc7d0();cm_fn_005a8f60();cm_fn_00945934(0xffffffffu);
}
