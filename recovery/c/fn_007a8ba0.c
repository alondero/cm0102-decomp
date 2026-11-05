#include "cmre.h"
/* Preferred-image VA007a8ba0/base00400000; target SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD2/RET8. Sole indexed caller overwrites EAX on both continuations
   before observing it (007acee5 or 007acf61). */
extern u32 CM_THISCALL cm_fn_0077bbe0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00785790(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00786090(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_CDECL cm_fn_006e86d0(u32,u32,u32,u32,u32);
#define B(a) CM_GLOBAL(volatile u8,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define D(a) CM_GLOBAL(volatile u32,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))

__attribute__((target("no-sse3")))
void CM_STDCALL cm_fn_007a8ba0(volatile u32 player_arg,volatile u32 team_arg)
{
    /* L is native ESP immediately after SUBf0, before saved registers.
       Native message L10..Lee is223bytes; adjacent flags L1/L2,
       selectedpointer L4 and secondscore L8 do not overlap it. */
    volatile struct {u8 scratch[0xf0];u32 prev,handler,state;} local;
    u32 l=(u32)local.scratch,message=l+0x10,player,team,selected=0;
    u32 candidate,pointer,a,score1,score2,sum,manager;
    s32 half,rating,high,low,signed_score;
    s8 index=-1,flagA=0,flagB=0,age;
    volatile long double live,age_part;
    volatile long long converted;
    local.state=0xffffffffu;local.handler=0x0095f20b;
    local.prev=CM_FS(u32,0);CM_FS(u32,0)=(u32)&local.prev;
    player=player_arg;
    if(!player)goto done;
    team=team_arg;
    if(!team)goto done;
    (void)cm_fn_0077bbe0(message,0xbce,0);
    a=D(player);local.state=0;
    (void)cm_fn_00785790(message,0,a);
    pointer=D(player+4);a=D(pointer+0x33);
    (void)cm_fn_00785790(message,1,a);
    pointer=D(player+8);a=D(pointer+0x33);
    (void)cm_fn_00785790(message,2,a);
    pointer=D(player+0xc);a=D(pointer+0x33);
    (void)cm_fn_00785790(message,3,a);
    a=D(team);(void)cm_fn_00785790(message,4,a);
    a=(u32)(s32)(s8)B(player+0x18);
    (void)cm_fn_00785790(message,5,a);
    age=(s8)B(team+0x7f);
    (void)cm_fn_00785790(message,6,age>15);
    do {
        if(selected)break;
        if(index==-1)candidate=D(team+0xd3);
        else candidate=D(team+(u32)((s32)index*4)+0x19f);
        if(candidate) {
            a=cm_fn_0090cfc0(2500);
            half=(s16)W(team+0x80);half=half/2;
            age=(s8)B(team+0x7f);
            sum=a+(u32)half;
            live=(long double)(s32)sum;
            age_part=(long double)age;
            age_part=age_part*(long double)Q(0x0096afc8);
            live=live+age_part;
            converted=(long long)live; /* Native FTOL007a8cdd; ADR0006. */
            score1=cm_fn_006e86d0(candidate,player,team,2,(u32)converted);
            /* Preserve actual native EAX when forwarding this score.
               006e86d0 observes its fifth WORD before its BYTE rewrite. */
            score2=cm_fn_006e86d0(candidate,player,team,1,score1);
            rating=(s16)W(team+0x80);
            high=rating/50+75;
            signed_score=(s16)score1;
            if(signed_score>high) {flagA=2;selected=candidate;}
            else {
                low=rating/100+50;
                if(signed_score>low) {flagA=1;selected=candidate;}
            }
            signed_score=(s16)score2;
            if(signed_score>high) {flagB=2;selected=candidate;}
            else {
                low=rating/100+50;
                if(signed_score>low) {flagB=1;selected=candidate;}
            }
            player=player_arg;
        }
        index=(s8)(index+1);
    }while(index<5);
    if(selected) {
        pointer=D(team+0xd3);
        (void)cm_fn_00785790(message,7,selected==pointer);
        a=D(selected);(void)cm_fn_00785790(message,8,a);
        pointer=D(selected+4);a=D(pointer+0x33);
        (void)cm_fn_00785790(message,9,a);
        pointer=D(selected+8);a=D(pointer+0x33);
        (void)cm_fn_00785790(message,10,a);
        pointer=D(selected+0xc);a=D(pointer+0x33);
        (void)cm_fn_00785790(message,11,a);
        (void)cm_fn_00785790(message,12,(u32)(s32)flagA);
        a=(u32)(s32)flagB;
    }else {
        (void)cm_fn_00785790(message,7,0xffffffffu);
        (void)cm_fn_00785790(message,8,0xffffffffu);
        (void)cm_fn_00785790(message,9,0xffffffffu);
        (void)cm_fn_00785790(message,10,0xffffffffu);
        (void)cm_fn_00785790(message,11,0xffffffffu);
        (void)cm_fn_00785790(message,12,0xffffffffu);
        a=0xffffffffu;
    }
    (void)cm_fn_00785790(message,13,a);
    manager=D(0x00b7431c);
    (void)cm_fn_00786090(manager,message,team,0,0,0,0);
done:
    CM_FS(u32,0)=local.prev;
}
