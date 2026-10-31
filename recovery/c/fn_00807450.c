/* Preferred-image VA00807450, base00400000; target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4, full EAX. Original helpers remain explicit boundaries.
   L is the native allocation after SUB270; saved registers are below L.
   Packed L4 keeps its upper bytes through the later byte-counter loops. */
#include "cmre.h"
#define B(p) CM_GLOBAL(volatile u8,(u32)(p))
#define W(p) CM_GLOBAL(volatile u16,(u32)(p))
#define D(p) CM_GLOBAL(volatile u32,(u32)(p))
extern u32 CM_THISCALL cm_fn_0043f620(u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u8 CM_CDECL cm_fn_005431e0(u32,u32);
extern u8 CM_CDECL cm_fn_00543280(u32,u32,u32);
extern u8 CM_CDECL cm_fn_00543f30(u32,u32);
extern u32 CM_CDECL cm_fn_00544520(u32,u32);
extern u32 CM_CDECL cm_fn_00544590(u32,u32);
extern u32 CM_CDECL cm_fn_00544670(u32);
/* The helper writes only AX on its normal return; native callers retain EAX.
   Physical incoming EAX here equals the one original stack argument. */
extern u32 CM_CDECL __attribute__((regparm(1))) cm_fn_005b7d20(u32,u32);
extern void CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern u8 CM_THISCALL cm_fn_0062c400(u32,u32);
extern u32 CM_STDCALL cm_fn_0076d9b0(u32,u32);
extern u8 CM_THISCALL cm_fn_007c6830(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007f8d70(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00805f10(u32,u32);
extern u32 CM_CDECL cm_fn_00805fb0(u32,u32);
extern u32 CM_CDECL cm_fn_00806030(u32,u32);
extern u32 CM_CDECL cm_fn_00806080(u32,u32);
extern u32 CM_CDECL cm_fn_008061f0(u32,u32);
extern long double CM_THISCALL cm_fn_00844780(u32,u32,u32,u32,u32,u32);
extern long double CM_THISCALL cm_fn_00846920(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0085e410(u32);
extern u16 CM_THISCALL cm_fn_0085fa00(u32);
extern u8 CM_THISCALL cm_fn_0085fa20(u32);
extern u32 CM_THISCALL cm_fn_00868210(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);

u32 CM_THISCALL cm_fn_00807450(u32 self,u32 candidate)
{
    volatile struct {u32 previous,handler,state;} frame;
    volatile u8 scratch[0x270];
    u32 a,b,c,flags,high,context,club,person,contract,receiver,packed;
    u32 result=0,output1,output2,line,text,index,base,parameter;
    u8 value,kind,age,nation,matched,visited,old;
    long double rating;
    frame.state=0xffffffffu;
    frame.handler=0x96118e;
    frame.previous=CM_FS(u32,0);
    CM_FS(u32,0)=(u32)&frame;
    D((u32)scratch+0x1c)=0;
    cm_fn_0085e410((u32)scratch+0x220);
    frame.state=0;
    packed=0xffffffffu;
    if (candidate==0) {
        output1=(u32)scratch+0x20; output2=(u32)scratch+0x120;
        line=0x1f2;
        goto error;
    }
    flags=D(self); a=D(self+4);
    if (flags==0x14140081 && a==0x44aaa884) goto success;
    a=(u32)(s32)(s16)W(0xb77c7a);
    context=D(0xb74c26+a*768u);
    if (context==0) {
        output1=(u32)scratch+0x120; output2=(u32)scratch+0x20;
        line=0x1ff;
        goto error;
    }
    a=D(context+0x39);
    if (a!=0) a=D(a+0x53);
    if (a==0) {
        a=D(context+0x24);
        if (a!=0) a=D(a+0x53);
        else a=D(context+0x1a);
    }
    club=a;
    if (club==0) {
        output1=(u32)scratch+0x120; output2=(u32)scratch+0x20;
        line=0x20e;
        goto error;
    }
    a=D(candidate+0xc);
    b=D(0xae23bc);
    person=b+a*110u;
    if ((flags&1)==0) {
        c=flags&2;
        if (c!=0) {
            a=D(person+0x1a);
            if (a!=club && D(person+0x1e)!=club) goto finish;
        }
        if ((flags&4)!=0) {
            a=D(person+0x1a);
            if (a==0 || B(a+0x7f)!=2) {
                a=D(person+0x1e);
                if (a==0 || B(a+0x7f)!=2) goto finish;
            }
        }
        if ((flags&8)!=0) {
            a=D(person+0x1e);
            if (a==0) goto finish;
            a=D(a); b=D(0x9cf424);
            if (a!=b) goto finish;
        }
        if ((flags&0x10)!=0) {
            a=D(person+0x1a);
            if (a!=0) {a=D(a); b=D(self+0x18); if(a==b) goto club_id_ok;}
            a=D(person+0x1e);
            if (a==0) goto finish;
            a=D(a); b=D(self+0x18);
            if (a!=b) goto finish;
        }
club_id_ok:
        if ((flags&0x20)!=0) {
            a=D(person+0x1a);
            if(a!=0) {value=B(a+0x76); kind=B(self+0x1c); if(value==kind) goto club_byte_ok;}
            a=D(person+0x1e);
            if(a==0) goto finish;
            value=B(a+0x76); kind=B(self+0x1c);
            if(value!=kind) goto finish;
        }
club_byte_ok:
        a=D(context+0x24);
        if(a!=0 && D(person+0x61)!=0) {
            if(c!=0 && club==D(a+0x53)) {
                receiver=D(0xb74318);
                if(cm_fn_0076d9b0(person,context)==0) goto finish;
            }
            a=D(self);
            if((a&0x10)!=0) {
                a=D(context+0x24); b=D(self+0x18); a=D(a+0x53);
                if(b==D(a)) {
                    receiver=D(0xb74318);
                    if(cm_fn_0076d9b0(person,context)==0) goto finish;
                }
            }
        }
    }
    flags=D(self); high=D(self+4);
    if((flags&0x80)==0) {
        age=B(person+0x18);
        if((flags&0x100)!=0 && (s8)age>0x15) goto finish;
        if((flags&0x200)!=0 && ((s8)age<0x13 || (s8)age>0x19)) goto finish;
        if((flags&0x400)!=0 && ((s8)age<0x14 || (s8)age>0x1e)) goto finish;
        if((flags&0x800)!=0 && ((s8)age<0x19 || (s8)age>0x20)) goto finish;
        if((flags&0x1000)!=0 && (s8)age<0x1e) goto finish;
        if((flags&0x2000)!=0 && (s8)age>0x1e) goto finish;
        if((flags&0x4000)!=0 && ((s8)age<0x1e || (s8)age>0x28)) goto finish;
        if((flags&0x8000)!=0 && ((s8)age<0x28 || (s8)age>0x32)) goto finish;
        if((flags&0x10000)!=0 && ((s8)age<0x32 || (s8)age>0x3c)) goto finish;
        if((flags&0x20000)!=0 && (s8)age<0x3c) goto finish;
    }
    if((flags&0x40000)==0) {
        if(B(self+0x1d)==1) {
            receiver=D(0xb74350); nation=B(club+0x77);
            if((s8)cm_fn_007c6830(receiver,person,0,nation,1)<4) goto finish;
        }
        if(B(self+0x1d)==2) {
            nation=B(club+0x78);
            if((s8)nation>=1 && (s8)nation<=0x40) {
                receiver=D(0xb74350);
                if((s8)cm_fn_007c6830(receiver,person,0,nation,1)<4) goto finish;
            }
        }
        if(B(self+0x1d)==3) {
            nation=B(club+0x79);
            if((s8)nation>=1 && (s8)nation<=0x40) {
                receiver=D(0xb74350);
                if((s8)cm_fn_007c6830(receiver,person,0,nation,1)<4) goto finish;
            }
        }
    }
    flags=D(self); high=D(self+4);
    if((flags&0x100000)==0) {
        a=D(person+0x39);
        if(a!=0) a=D(a+0x53);
        if(a==0) {
            a=D(person+0x24);
            if(a!=0 && B(person+0x28)!=0xb) a=D(a+0x53);
            else a=D(person+0x1a);
        }
        if((flags&0x200000)!=0 && a!=club) goto finish;
        if((flags&0x400000)!=0 && a==club) goto finish;
        if((flags&0x800000)!=0) {
            if(a==0) goto finish;
            b=D(a); c=D(self+0x1e);
            if(b!=c) goto finish;
        }
        if((flags&0x1000000)!=0) {
            if(a==0) goto finish;
            value=B(a+0x76); kind=B(self+0x22);
            if(value!=kind) goto finish;
        }
    }
    if((flags&0x4000000)==0) {
        receiver=D(0xae1050); contract=cm_fn_004cf5a0(receiver,person);
        a=D(self);
        if((a&0x8000000)!=0 && B(self+0x23)==0xff && contract!=0) {
            if(cm_fn_0085fa20(contract)==0) goto finish;
            a=D(contract+4); b=D(0xae23b4); a=D(b+a*581u+0x53);
            if(cm_fn_00544670(a)==0) goto finish;
        }
        a=D(self);
        if((a&0x8000000)!=0) {
            value=B(self+0x23);
            if((s8)value>=0 && (s8)value<7) {
                D((u32)scratch+8)=0;
                frame.state=(frame.state&0xffffff00u)|1;
                a=cm_fn_0043f620((u32)scratch+8,0x8000000u|value);
                b=D(person+0x52);
                frame.state&=0xffffff00u;
                if((s32)b>(s32)a) goto finish;
            }
        }
        a=D(self);
        if((a&0x8000000)!=0 && B(self+0x23)==7) {
            D((u32)scratch+8)=0;
            frame.state=(frame.state&0xffffff00u)|2;
            a=cm_fn_0043f620((u32)scratch+8,6);
            b=D(person+0x52);
            frame.state&=0xffffff00u;
            if((s32)b<(s32)a) goto finish;
        }
    } else contract=D((u32)scratch+0x1c);
    flags=D(self); high=D(self+4);
    if((flags&0x10000000)==0) {
        if((flags&0x20000000)!=0 && cm_fn_00544520(person,0)==0) goto finish;
        a=D(self);
        if((a&0x40000000)!=0 && cm_fn_00544590(person,0)==0) goto finish;
    }
    high=D(self+4);
    if((high&0x40000000)==0) {
        receiver=D(0xb74448);
        if(cm_fn_007f8d70(receiver,person,0,0)==0) goto finish;
    }
    flags=D(self); high=D(self+4);
    if((flags&0x80000000)!=0 || (high&3)!=0) {
        if(contract==0) {receiver=D(0xae1050); contract=cm_fn_004cf5a0(receiver,person);}
        high=D(self+4);
        if((high&1)!=0 && contract!=0 && cm_fn_0085fa20(contract)==0) goto finish;
        high=D(self+4);
        if((high&2)!=0) {
            if(D(person+0x39)==0 || contract==0) goto finish;
            if(cm_fn_0085fa20(contract)!=0) goto finish;
            if((s16)cm_fn_0085fa00(contract)>=0x16d) goto finish;
        }
        a=D(self);
        if((a&0x80000000)!=0) {
            if(contract==0) goto finish;
            a=D(context+0x39);
            if(cm_fn_00868210(contract,a,0,0)!=0) goto finish;
        }
    }
    flags=D(self); high=D(self+4);
    if((high&4)==0) {
        a=D(person+0x61); packed=cm_fn_005b7d20(a,a);
        flags=D(self); high=D(self+4);
        if((high&8)!=0 && (packed&1)==0) goto finish;
        if((high&0x10)!=0 && (packed&6)==0) goto finish;
        if((high&0x20)!=0 && (packed&0x38)==0) goto finish;
        if((high&0x40)!=0 && (packed&0x40)==0) goto finish;
    }
    if((high&0x80)==0) {
        if((u16)packed==0xffff) {a=D(person+0x61); packed=cm_fn_005b7d20(a,a);}
        high=D(self+4); flags=D(self);
        if((high&0x100)!=0 && (packed&0x80)==0) goto finish;
        if((high&0x200)!=0 && (packed&0x700)==0) goto finish;
        if((high&0x400)!=0 && (packed&0x800)==0) goto finish;
    }
    a=D(person+0x39);
    if(a!=0) kind=B(person+0x3d);
    else {
        a=D(person+0x24);
        if(a!=0) {kind=B(person+0x28); if(kind!=0xb) goto kind_ready;}
        if(B(self+0x24)==9) kind=0;
        else kind=B(person+0x3d);
    }
kind_ready:
    packed=(packed&0xffffff00u)|kind;
    if((high&0x800)==0) {
        value=B(self+0x24);
        if(value==1 && kind!=1) goto finish;
        if(value==2 && kind!=2 && kind!=3 && kind!=4) goto finish;
        if(value==3 && kind!=5 && kind!=0xc) goto finish;
        if(value==4 && kind!=6 && kind!=7 && kind!=0xd) goto finish;
        if(value==5 && kind!=8 && kind!=0xf) goto finish;
        if(value==6 && kind!=9) goto finish;
        if(value==7 && kind!=0xa) goto finish;
        if(value==8 && kind!=0xb && kind!=0xc && kind!=0xd && kind!=0xf) goto finish;
        if(value==9 && kind!=0) goto finish;
    }
    if((high&0x2000)==0) {
        receiver=D(0xb63d28); value=cm_fn_0062c400(receiver,person);
        flags=D(self); high=D(self+4);
        if((high&0x4000)!=0 && (value==2 || value==1)) goto finish;
    }
    if((high&0x8000)==0 && (high&0x10000)!=0) {
        a=D(context+0x39);
        if(a==0) goto finish;
        c=D(person+0x61);
        if(c!=0) {
            b=(u32)(s32)(s16)W(a+0x80); c=(u32)(s32)(s16)W(c+0xd);
            if((s32)c>(s32)(b+5000u)) goto finish;
            receiver=D(0xde1f54); receiver=D(receiver+0x856);
            rating=cm_fn_00846920(receiver,person,a,1,0,1,0);
        } else {
            c=D(person+0x69);
            if(c==0) goto text_filters;
            b=(u32)(s32)(s16)W(a+0x80); c=(u32)(s32)(s16)W(c+0xc);
            if((s32)c>(s32)(b+5000u)) goto finish;
            receiver=D(0xde1f54); receiver=D(receiver+0x856);
            rating=cm_fn_00844780(receiver,person,a,1,packed,0);
        }
        /* Native FCOMP sets C0 on both less-than and unordered. */
        if(!(rating>=(long double)CM_GLOBAL(volatile double,0x968e58))) goto finish;
    }
text_filters:
    high=D(self+4);
    if((high&0x200000)==0 && B(self+0x7f)!=0) {
        parameter=self+0x7f;
        if(cm_fn_00806080(person,parameter)!=0) goto text_done;
        a=D(person+0x1a);
        if(a!=0 && cm_fn_00805fb0(a,parameter)!=0) goto text_done;
        a=D(person+0x1a);
        if(a!=0) {a=D(a+0x71); if(a!=0 && cm_fn_00805f10(a,parameter)!=0) goto text_done;}
        a=D(person+0x39);
        if(a!=0 && cm_fn_00806030(a,parameter)!=0) goto text_done;
        a=D(person+0x39);
        if(a!=0) {a=D(a+0x57); if(a!=0 && cm_fn_008061f0(a,parameter)!=0) goto text_done;}
        a=D(person+0x39);
        if(a!=0) {a=D(a+0x53); if(a!=0 && cm_fn_00805fb0(a,parameter)!=0) goto text_done;}
        a=D(person+0x39);
        if(a!=0) {a=D(a+0x53); if(a!=0) {a=D(a+0x71); if(a!=0 && cm_fn_00805f10(a,parameter)!=0) goto text_done;}}
        goto finish;
    }
text_done:
    flags=D(self); high=D(self+4);
    if((high&0x800000)==0) {
        if((high&0x1000000)!=0 && B(candidate+0x29)==0) goto finish;
        if((high&0x2000000)!=0 && B(candidate+0x2a)==0) goto finish;
    }
    if((high&0x20000)==0) {
        index=1; visited=0; matched=0; packed=(packed&0xffffff00u)|1;
        do {
            base=self+index;
            value=B(base+0x24);
            if((s8)value>1 || (s8)B(base+0x43)<0x14) {
                parameter=packed;
                if(cm_fn_00543280(context,person,parameter)==0) goto finish;
                value=cm_fn_005431e0(person,parameter); kind=B(base+0x24);
                if((s8)value>=(s8)kind && (s8)value<=(s8)B(base+0x43)) {
                    kind=B(self+0x64); old=matched; matched++;
                    if((s32)(s8)old==(s32)(s8)kind-1) break;
                }
                kind=B(self+0x63); old=visited; visited++;
                if((s32)(s8)old==(s32)(s8)kind-1) break;
            }
            index++; packed=(packed&0xffffff00u)|(u8)index;
        } while((s8)index<=0x1f);
        if((s8)matched<(s8)B(self+0x64)) goto finish;
    }
    high=D(self+4);
    if((high&0x80000)==0) {
        index=1; visited=0; matched=0; packed=(packed&0xffffff00u)|1;
        do {
            base=self+index;
            value=B(base+0x64);
            if((s8)value>1 || (s8)B(base+0x70)<0x14) {
                value=cm_fn_00543f30(person,packed); kind=B(base+0x64);
                if((s8)value>=(s8)kind && (s8)value<=(s8)B(base+0x70)) {
                    kind=B(self+0x7e); old=matched; matched++;
                    if((s32)(s8)old==(s32)(s8)kind-1) break;
                }
                kind=B(self+0x7d); old=visited; visited++;
                if((s32)(s8)old==(s32)(s8)kind-1) break;
            }
            index++; packed=(packed&0xffffff00u)|(u8)index;
        } while((s8)index<=0xc);
        if((s8)matched<(s8)B(self+0x7e)) goto finish;
    }
success:
    result=1;
    goto finish;
error:
    cm_fn_00944cff(0xa7df38,0,0,output1,output2);
    text=cm_fn_0090d130();
    cm_fn_00944c9f(0xae24d0,0x9870e8,text,output1,output2,line);
    cm_fn_005e8290(0x9870e0,0xae24d0,0);
    D(0xb67a34)=0;
finish:
    a=frame.previous;
    CM_FS(u32,0)=a;
    return result;
}
