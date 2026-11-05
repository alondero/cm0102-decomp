/* Preferred-image VA00533080/base00400000, primary SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL0. Solecaller532e04 overwrites fullEAX at532e0f before use. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))
extern void CM_THISCALL cm_fn_0054a110(u32);
extern void CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern void CM_THISCALL cm_fn_005335f0(u32,u32);
extern u32 CM_CDECL cm_fn_0053d740(u32,u32);
extern u32 CM_CDECL cm_fn_0053d980(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern void CM_CDECL cm_fn_005a53c0(void);

/* These two inline field-copy sequences have different native sret buffers.
   The packed record has110 initialized bytes, including its five dates. */
static void copy_record(u32 dest,u32 src,u32 o0,u32 o1,u32 o2,u32 o3,u32 o4)
{
    u32 i;
    for(i=0;i<0x10u;i+=4u)D(dest+i)=D(src+i);
    cm_fn_00417c10(dest+0x10u,o0,src+0x10u);
    W(dest+0x18u)=W(src+0x18u);
    D(dest+0x1au)=D(src+0x1au);
    D(dest+0x1eu)=D(src+0x1eu);
    B(dest+0x22u)=B(src+0x22u);
    B(dest+0x23u)=B(src+0x23u);
    D(dest+0x24u)=D(src+0x24u);
    B(dest+0x28u)=B(src+0x28u);
    cm_fn_00417c10(dest+0x29u,o1,src+0x29u);
    cm_fn_00417c10(dest+0x31u,o2,src+0x31u);
    D(dest+0x39u)=D(src+0x39u);
    B(dest+0x3du)=B(src+0x3du);
    cm_fn_00417c10(dest+0x3eu,o3,src+0x3eu);
    cm_fn_00417c10(dest+0x46u,o4,src+0x46u);
    D(dest+0x4eu)=D(src+0x4eu);
    D(dest+0x52u)=D(src+0x52u);
    for(i=0x56u;i<=0x60u;i++)B(dest+i)=B(src+i);
    D(dest+0x61u)=D(src+0x61u);
    D(dest+0x65u)=D(src+0x65u);
    D(dest+0x69u)=D(src+0x69u);
    B(dest+0x6du)=B(src+0x6du);
}

__attribute__((target("no-sse3")))
void CM_CDECL cm_fn_00533080(void)
{
    /* L=ESP after native SUBec, before four conditional saved GPRs.
       RecordLc..L79, temporarydateL7c and every sret output share this frame. */
    volatile struct {u8 local[0xec];u32 prev,handler,state;} frame;
    u32 l=(u32)frame.local,prior,bound,remaining;
    u32 base,first,second,off_first,off_second,person_first,person_second;
    u32 first_club,second_club,a,b,value;
    u16 saved5,saved7,word;
    long double x;
    frame.state=0xffffffffu;frame.handler=0x9578ffu;
    prior=CM_FS(u32,0);frame.prev=prior;CM_FS(u32,0)=(u32)&frame.prev;
    cm_fn_0054a110(l+0x1cu);frame.state=0;
    cm_fn_0054a110(l+0x35u);B((u32)&frame.state)=1;
    cm_fn_0054a110(l+0x3du);B((u32)&frame.state)=2;
    cm_fn_0054a110(l+0x4au);B((u32)&frame.state)=3;
    cm_fn_0054a110(l+0x52u);frame.state=4;
    cm_fn_0054a110(l+0x7cu);
    x=(long double)(s32)D(0xae2364u);
    B((u32)&frame.state)=5;
    x=x*(long double)Q(0x969508u);
    bound=(u32)(long long)x; /* __ftol CALL00533113. */
    if((s32)bound>0) {
        D(l)=bound;
        do {
            a=D(0xae2364u);first=cm_fn_0090cfc0(a);
            a=D(0xae2364u);second=cm_fn_0090cfc0(a);
            if(first==second)goto next_iteration;
            off_first=first*110u;base=D(0xae23bcu);
            if(D(base+off_first+0x61u)==0)goto next_iteration;
            off_second=second*110u;
            a=D(base+off_second+0x61u);person_second=base+off_second;
            if(a==0)goto next_iteration;
            a=D(base+off_first+0x69u);
            if(a==0) {
                b=D(person_second+0x69u);
                if(b!=0)goto next_iteration;
            } else if(D(person_second+0x69u)==0)goto next_iteration;
            first_club=D(base+off_first+0x39u);
            second_club=D(person_second+0x39u);D(l+4u)=second_club;
            if(first_club==0 || second_club==0)goto next_iteration;
            a=D(first_club+0x53u);b=D(second_club+0x53u);
            if(a==b) {a=D(first_club+0x57u);b=D(second_club+0x57u);}
            if(a!=b) {
                a=cm_fn_0053d740(base+off_first,0);
                b=cm_fn_0053d740(person_second,0);
                if(a!=b)goto next_iteration;
                base=D(0xae23bcu);a=D(first_club+0x53u);
                if(cm_fn_0053d980(base+off_first,a,1)==0)goto next_iteration;
                second_club=D(l+4u);base=D(0xae23bcu);a=D(second_club+0x53u);
                if(cm_fn_0053d980(base+off_second,a,1)==0)goto next_iteration;
                base=D(0xae23bcu);
            }
            person_first=base+off_first;
            copy_record(l+0xcu,person_first,l+0xdcu,l+0xe4u,l+0xbcu,l+0x9cu,l+0x84u);
            base=D(0xae23bcu);
            copy_record(base+off_first,base+off_second,l+0xacu,l+0xccu,l+0x8cu,l+0x94u,l+0xa4u);
            base=D(0xae23bcu);cm_fn_005335f0(base+off_second,l+0xcu);
            base=D(0xae23bcu);a=D(base+off_second);b=D(base+off_first);
            D(base+off_first)=a;
            base=D(0xae23bcu);D(base+off_second)=b;
            base=D(0xae23bcu);a=D(base+off_second+0x39u);b=D(base+off_first+0x39u);
            D(base+off_first+0x39u)=a;
            base=D(0xae23bcu);D(base+off_second+0x39u)=b;
            if(cm_fn_0090cfc0(2)!=0) {
                base=D(0xae23bcu);cm_fn_00417c10(l+0x7cu,l+0xb4u,base+off_first+0x10u);
                base=D(0xae23bcu);cm_fn_00417c10(base+off_first+0x10u,l+0xc4u,base+off_second+0x10u);
                base=D(0xae23bcu);cm_fn_00417c10(base+off_second+0x10u,l+0xd4u,l+0x7cu);
            }
            base=D(0xae23bcu);a=D(base+off_first+0x61u);b=D(base+off_second+0x61u);
            word=W(b+5u);saved5=W(a+5u);saved7=W(a+7u);W(a+5u)=word;
            base=D(0xae23bcu);b=D(base+off_second+0x61u);a=D(base+off_first+0x61u);
            word=W(b+7u);W(a+7u)=word;
            base=D(0xae23bcu);b=D(base+off_second+0x61u);W(b+5u)=saved5;
            base=D(0xae23bcu);a=D(base+off_second+0x61u);W(a+7u)=saved7;
next_iteration:
            value=D(l);remaining=value-1u;D(l)=remaining;
        } while(remaining!=0);
    }
    frame.state=0xffffffffu;
    /* Original5a53c0 is a single RET; ECX=Lc is not consumed. */
    cm_fn_005a53c0();
    prior=frame.prev;CM_FS(u32,0)=prior;
}
