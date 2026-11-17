#include "cmre.h"
/*
 * Preferred-image VAs, CM0102 3.9.68 SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * Entry ECX saved at 0058e9c1, three stack args, ret 0xc at 0058eaed.
 * 0068d1b0 receivers: 0058e9c6,0058ea5e,0058ea93,0058ec11,
 * 0058ed04,0058ee3e. 0068d1f0: ECX at 0058ec88, target push
 * at 0058ec8a. 0090d130 takes no pushes: 0058ee82..0058ee9e
 * and 0058f09f..0058f0bb forward them to 00944c9f instead.
 * 0066f4e0 is variadic: two args at 0058eea6..0058eeb0,
 * three at 0058eeb8..0058eec7 (cleanup deferred to 0058eedd).
 */
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern u32 CM_THISCALL cm_fn_0068d1f0(u8 *self, const u8 *target);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern char *CM_CDECL cm_fn_0066f4e0(int,char *,...);
extern void CM_CDECL cm_fn_005e8290(const char *,const char *,int);
extern volatile u32 *CM_CDECL cm_fn_00945b00(u32,volatile s8 *);
extern u32 CM_CDECL cm_fn_00945a09(u32);
extern s32 CM_CDECL cm_fn_00946a22(volatile u8 *,u32,u32);
extern s32 CM_CDECL cm_fn_00946aae(volatile u8 *);
extern u32 CM_CDECL cm_fn_00945ce6(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00945bfe(u32,u32,u32,u32);
#define R(a) CM_GLOBAL(volatile u32,(u32)(a))
#define SEEK(p,o,m) cm_fn_00946a22((volatile u8 *)(p),(o),(m))
#define TELL(p) ((u32)cm_fn_00946aae((volatile u8 *)(p)))

void CM_THISCALL cm_fn_0058e9b0(u32 self,u32 first,u32 last,u32 list)
{
    /* L+0,L+4,L+8,L+0x108: one original 0x208-byte allocation. */
    u32 scratch[130];
    u8 *a=(u8 *)scratch+8, *b=(u8 *)scratch+264;
    u8 *ea=a,*eb=b;
    u32 p,node,x,y,line,msg,heading=0x009c7774,text=0x009c7680;
    int close_error=1;

    x=R(list+2);
    R(list+10)=x;
    if (!cm_fn_0068d1b0(list)) return;
    x=R(list+2);
    R(list+10)=x;
    x=R(first);
    if(x==0xffffffffu && R(last)==0xffffffffu) {
        p=(u32)cm_fn_00945b00(R(self),(volatile s8 *)0x009c79b0);
        if(!p) {
            line=0x132; msg=0x009c7980; close_error=0;
            ea=b; eb=a;
            goto error;
        }
        SEEK(p,0,2);
        x=TELL(p);
        R(first)=x;
        node=cm_fn_0068d1b0(list);
        if(!node) goto close;
        do {
            x=TELL(p);
            R(last)=x;
            if(cm_fn_00945ce6(node,R(self+4),1,p)!=1) {
                line=0x146; msg=0x009c7880; goto error;
            }
            node=cm_fn_0068d1b0(list);
            if(node) scratch[0]=TELL(p)+4;
            else scratch[0]=0xffffffffu;
            if(cm_fn_00945ce6((u32)scratch,4,1,p)!=1) {
                line=0x15d; msg=0x009c784c; goto error;
            }
        } while(node);
        goto close;
    }
    /* EAX retains the first value at 0058eb63; last is read anew. */
    y=R(last);
    if(x!=0xffffffffu && y==0xffffffffu) {
        p=(u32)cm_fn_00945b00(R(self),(volatile s8 *)0x009c7848);
        if(!p) {
            line=0x16f; msg=0x009c781c; close_error=0; goto error;
        }
        if(SEEK(p,R(first),0)) {
            line=0x176; msg=0x009c77e8; goto error;
        }
        node=cm_fn_0068d1b0(list);
        if(cm_fn_00945ce6(node,R(self+4),1,p)!=1) {
            line=0x181; msg=0x009c77bc; goto error;
        }
        x=R(first);
        R(last)=x;
        cm_fn_00945a09(p);
        cm_fn_0068d1f0((u8 *)list,(const u8 *)node);
        cm_fn_0058e9b0(self,first,last,list);
        return;
    }
    if(x==0xffffffffu && y==0xffffffffu) return;
    p=(u32)cm_fn_00945b00(R(self),(volatile s8 *)0x009c7848);
    if(!p) {
        line=0x199; msg=0x009c781c; close_error=0; goto error;
    }
    node=cm_fn_0068d1b0(list);
    while(node) {
        if(SEEK(p,R(last),0)) {
            line=0x1a3; msg=0x009c7788; close_error=0; goto error;
        }
        SEEK(p,R(self+4),1);
        if(cm_fn_00945bfe((u32)scratch,4,1,p)!=1) {
            line=0x1ac; msg=0x009c764c;
            heading=0x009c7970; text=0x009c78b0; goto error;
        }
        x=scratch[0];
        if(x!=0xffffffffu) {
            if(SEEK(p,x,0)) {
                line=0x1b7; msg=0x009c7614; goto error;
            }
            if(cm_fn_00945ce6(node,R(self+4),1,p)!=1) {
                line=0x1bf; msg=0x009c75e4; goto error;
            }
            x=scratch[0];
            R(last)=x;
            scratch[0]=0xffffffffu;
            if(cm_fn_00945ce6((u32)scratch,4,1,p)!=1) {
                line=0x1cb; msg=0x009c75ac; goto error;
            }
        } else {
            SEEK(p,0,2);
            x=TELL(p);
            y=R(self+4);
            scratch[1]=x;
            scratch[0]=0xffffffffu;
            if(cm_fn_00945ce6(node,y,1,p)!=1) {
                line=0x1dc; msg=0x009c75e4; goto error;
            }
            if(cm_fn_00945ce6((u32)scratch,4,1,p)!=1) {
                line=0x1e3; msg=0x009c75ac; goto error;
            }
            x=R(last);
            y=R(self+4);
            SEEK(p,x+y,0);
            if(cm_fn_00945ce6((u32)(scratch+1),4,1,p)!=1) {
                line=0x1ed; msg=0x009c756c; goto error;
            }
            x=scratch[1];
            R(last)=x;
        }
        node=cm_fn_0068d1b0(list);
    }
close:
    cm_fn_00945a09(p);
    return;
error:
    cm_fn_00944cff(0x009c7540,0,0,ea,eb);
    x=(u32)cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009a8120,x,ea,eb,line,msg);
    cm_fn_0066f4e0(0x00ae28bc,(char *)heading);
    cm_fn_0066f4e0(0x00ae26c4,(char *)text,0x00ae24d0);
    cm_fn_005e8290((const char *)0x00ae28bc,(const char *)0x00ae26c4,0);
    if(close_error) cm_fn_00945a09(p);
}
