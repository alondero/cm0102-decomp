#include "cmre.h"
/* Preferred-image VAs, CM0102 3.9.68
 SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 76bb92-99 (and repeated sites) push both halves of the date:
 54cdb0 has ECX plus two DWORD stack arguments.
 76bb1e-35: 54cab0 receives ECX, output pointer, mode; its EAX
 is dereferenced at 76bb3a. 417c10 receives ECX and two pointers,
 e.g. 76bbaa-bbb7. 549ef0 receives ECX and two date words,
 76bafc-bb0a. */
extern u32 CM_CDECL cm_fn_005409b0(u32 *);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *,u32,u32);
extern u32 CM_THISCALL cm_fn_0054cab0(void *,void *,u32);
extern s32 CM_THISCALL cm_fn_0054cdb0(void *,u32,u32);
extern void CM_THISCALL cm_fn_00417c10(void *,void *,void *);
extern void CM_THISCALL cm_fn_0076c120(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0076d880(u32,u32);
extern u32 CM_THISCALL cm_fn_0076c060(u32,u32);
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define P(a) ((void *)(a))

void CM_THISCALL cm_fn_0076bab0(u32 self,u32 match)
{
    volatile struct {u32 prev,handler,state;} frame;
    u32 scratch[6];
    u32 team,nation,index,record,low,high,result,mode;
    u16 year,day,flags;
    u32 side;
    frame.prev=CM_FS(u32,0);
    frame.state=0xffffffffu;
    frame.handler=0x0095dd28;
    CM_FS(u32,0)=(u32)&frame;
    if (B(match+0x40)==0 && (B(match+0x4e)&1)) goto done;
    team=D(match+0x1c);
    result=cm_fn_005409b0((u32 *)team);
    if (result && (B(match+0x4e)&2)) goto done;
    day=W(match+0x28);
    year=W(match+0x2a);
    cm_fn_00549ef0(&scratch[4],year,day);
    low=D(0xae2c90);
    high=D(0xae2c94);
    frame.state=0;
    scratch[0]=low;
    scratch[1]=high;
    result=cm_fn_0054cab0(&scratch[4],&scratch[2],2);
    year=W(result+2);
    if ((u16)(scratch[0]>>16)<year) goto done;
    if ((u16)(scratch[0]>>16)==year && (s16)low<(s16)W(result)) goto done;

    /* The two straight-line halves at 76bb60 and 76bdd4 have the
       same record operations; only the first has the d880 calls. */
    for(side=0;side<2;side++) {
        team=D(match+0x1c+side*4);
        nation=D(team+0x53);
        index=D(nation);
        record=D(self+4)+index*110u;
        result=cm_fn_005409b0((u32 *)team);
        if (!result) {
            high=D(record+0x3e);
            low=D(record+0x3a);
            result=(u32)cm_fn_0054cdb0(P(0xae2c90),low,high);
            if ((s32)result<=14 && B(record+0x43)) {
                cm_fn_00417c10(P(record+0x32),&scratch[2],P(0xae2c90));
                day=W(match+0x28);
                year=W(match+0x2a);
                cm_fn_00549ef0(&scratch[0],year,day);
                low=D(record+0x3a);
                high=D(record+0x3e);
                scratch[2]=low;
                scratch[3]=high;
                if ((u16)(low>>16)<(u16)(scratch[0]>>16) ||
                    ((u16)(low>>16)==(u16)(scratch[0]>>16) &&
                     (s16)low<(s16)scratch[0]))
                    cm_fn_00417c10(P(record+0x3a),&scratch[2],&scratch[0]);
            } else {
                team=D(match+0x1c+side*4);
                nation=D(team+0x53);
                cm_fn_0076c120(self,nation,0,0xae2c90,match,0);
            }
            if (side==0 && !(B(match+0x4e)&2)) {
                team=D(match+0x1c);
                nation=D(team+0x53);
                cm_fn_0076d880(self,nation);
                team=D(match+0x20);
                nation=D(team+0x53);
                cm_fn_0076d880(self,nation);
            }
        }
        flags=W(match+0x4d);
        if (flags&0x100) mode=1;
        else {
            if (!(flags&0x200)) {
                team=D(match+0x1c+side*4);
                if (!cm_fn_005409b0((u32 *)team)) continue;
            }
            mode=2;
        }
        /* offsets 4e/62, versus 46/5a for the source dates */
        index=record+0x3a+mode*20;
        if(side==0) {
            high=D(index+4);
            low=D(index);
        } else if(mode==1) {
            high=D(index+4);
            low=D(index);
        } else {
            high=D(index+4);
            low=D(index);
        }
        result=(u32)cm_fn_0054cdb0(P(0xae2c90),low,high);
        if ((s32)result>14 || !B(index+9)) goto update;
        if(mode==2) {
            low=D(match);
            high=D(0x9cf7a0);
            if(low!=high) {
                team=D(match+0x1c+side*4);
                nation=D(team+0x53);
                if(side==0) {
                    high=D(self+0x36);
                    low=D(nation);
                } else {
                    high=D(self+0x36);
                    low=D(nation);
                }
                if(B(low+high)) goto update;
            }
        }
        cm_fn_00417c10(P(index-8),&scratch[2],P(0xae2c90));
        day=W(match+0x28);
        year=W(match+0x2a);
        cm_fn_00549ef0(&scratch[0],year,day);
        low=D(index);
        high=D(index+4);
        scratch[2]=low;
        scratch[3]=high;
        if ((u16)(low>>16)<(u16)(scratch[0]>>16) ||
            ((u16)(low>>16)==(u16)(scratch[0]>>16) &&
             (s16)low<(s16)scratch[0]))
            cm_fn_00417c10(P(index),&scratch[2],&scratch[0]);
        continue;
update:
        team=D(match+0x1c+side*4);
        nation=D(team+0x53);
        cm_fn_0076c120(self,nation,mode,0xae2c90,match,0);
    }
    team=D(match+0x1c);
    nation=D(team+0x53);
    cm_fn_0076c060(self,nation);
    team=D(match+0x20);
    nation=D(team+0x53);
    cm_fn_0076c060(self,nation);
done:
    CM_FS(u32,0)=frame.prev;
}
