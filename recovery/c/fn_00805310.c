/* Preferred-image VA00805310, base00400000; target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2/RET8, conservative full EAX. All engine/CRT calls remain original.
   Scratch L+8/L+108 are adjacent original filename/extension buffers.
   Contract filters advance the row only on the original non-null +39 path. */
#include "cmre.h"
#define B(p) CM_GLOBAL(volatile u8,(u32)(p))
#define W(p) CM_GLOBAL(volatile u16,(u32)(p))
#define D(p) CM_GLOBAL(volatile u32,(u32)(p))
extern u8 CM_THISCALL cm_fn_00418f60(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00418ff0(u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0062ece0(u32,u32);
extern u32 CM_CDECL cm_fn_00806080(u32,u32);
extern u32 CM_CDECL cm_fn_00806260(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);

/* Original two-byte comparison unroll, with left/right byte load order. */
static int compare_00805310(u32 left,u32 right)
{
    u8 x,y;
    for (;;) {
        x=B(left); y=B(right);
        if (x!=y) return x<y?-1:1;
        if (x==0) return 0;
        x=B(left+1); y=B(right+1);
        if (x!=y) return x<y?-1:1;
        left+=2; right+=2;
        if (x==0) return 0;
    }
}

u32 CM_THISCALL cm_fn_00805310(u32 self,u32 query,u32 flag)
{
    volatile u8 scratch[0x208];
    u32 row,out,a,n,index,other,receiver,line,p,q,t,offset;
    u8 first,status;
    int mode,keep;
    row=D(0xae23bc);
    if (row==0) {
        cm_fn_00944cff(0xa7de94,0,0,(u32)&scratch[0x108],(u32)&scratch[8]);
        p=(u32)&scratch[0x108]; q=(u32)&scratch[8]; line=0xac;
        goto error;
    }
    out=D(self+0x34);
    if (out==0) {
        cm_fn_00944cff(0xa7de94,0,0,(u32)&scratch[8],(u32)&scratch[0x108]);
        p=(u32)&scratch[8]; q=(u32)&scratch[0x108]; line=0xb6;
        goto error;
    }
    if (query==0) {
        cm_fn_00944cff(0xa7de94,0,0,(u32)&scratch[8],(u32)&scratch[0x108]);
        p=(u32)&scratch[8]; q=(u32)&scratch[0x108]; line=0xc0;
        goto error;
    }
    D(self+0x38)=0;
    first=B(query);
    mode=0;
    if (first!=0) {
        if (compare_00805310(query,0x9ca15c)==0) mode=1;
        else if (compare_00805310(query,0xa7df2c)==0) mode=2;
        else if (compare_00805310(query,0xa7df24)==0) mode=3;
        else if (compare_00805310(query,0xa7df1c)==0) mode=4;
        else if (compare_00805310(query,0xa7df10)==0) mode=5;
        else if (compare_00805310(query,0xa7df04)==0) mode=6;
        else if (compare_00805310(query,0xa7defc)==0) mode=7;
        else if (compare_00805310(query,0xa7def4)==0) mode=8;
        else if (compare_00805310(query,0xa7dee8)==0) mode=9;
        else if (compare_00805310(query,0xa7dedc)==0) mode=10;
        else if (compare_00805310(query,0xa7ded0)==0) mode=11;
        else if (compare_00805310(query,0xa7dec4)==0) mode=12;
    }
    n=D(0xae2364);
    if ((s32)n<=0) return n;
    index=0; offset=0;
    for (;;) {
        keep=0;
        switch (mode) {
        case 1:
            status=B(row+0x5f);
            if (status==3 || (flag!=0?status==1:status==2)) {
                a=D(row+0x39);
                if (a==0) keep=1;
            }
            break;
        case 2:
            a=D(row+0x61);
            if (a!=0) {
                receiver=D(0xb63d28);
                a=cm_fn_0062ece0(receiver,row);
                if ((u8)a!=0) keep=1;
            }
            break;
        case 3:
            status=B(row+0x5f);
            if (status==3 || (flag!=0?status==1:status==2)) {
                status=B(row+0x18);
                if ((s8)status<0x12) keep=1;
            }
            break;
        case 4: case 5:
            status=B(row+0x5f);
            if (status==3 || (flag!=0?status==1:status==2)) {
                a=D(0xae2d0c); status=B(a+offset+0xb);
                if ((status&0x30)==(mode==4?0x20:0x10)) keep=1;
            }
            break;
        case 6:
            status=B(row+0x5f);
            if (status==3 || (flag==0 && status==2)) {
                a=D(row+0x61);
                if (a!=0 && (s16)W(a+7)>0xa0 && (s16)W(a+5)>0x8c) {
                    a=D(row+0x39);
                    if (a==0) keep=1;
                }
            }
            break;
        case 7:
            receiver=D(0xad9c6c);
            a=cm_fn_00418f60(receiver,row,0);
            if ((u8)a!=0) keep=1;
            break;
        case 8:
            receiver=D(0xad9c6c);
            a=cm_fn_00418ff0(receiver,row);
            if ((u8)a!=0) keep=1;
            break;
        case 9: case 10: case 11: case 12:
            other=D(row+0x39);
            if (other!=0) {
                receiver=D(0xae1050);
                a=cm_fn_004cf5f0(receiver,row,other);
                if (a!=0) {
                    if (mode==9) { status=B(a+0x45); keep=(status&1)!=0; }
                    else if (mode==10) { status=B(a+0x45); keep=(status&2)!=0; }
                    else { a=D(a+0x45); keep=(a&(mode==11?0x3800u:0x20000u))!=0; }
                }
                if (keep) {
                    D(out)=row; out+=4;
                    a=D(self+0x38); D(self+0x38)=a+1;
                }
                row+=0x6e;
            }
            goto next;
        default:
            status=B(row+0x5f);
            if (!(status==3 || (flag!=0?status==1:status==2))) break;
            if (B(query)==0) { keep=1; break; }
            a=cm_fn_00806080(row,query);
            if (a!=0) { keep=1; break; }
            other=D(row+0x1a);
            if (other!=0) {
                a=other+4; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
                a=other+0x38; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
                a=other+0x53; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
                a=other+0x57; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
            }
            a=D(row+0x1a);
            if (a!=0) {
                other=D(a+0x71);
                if (other!=0) {
                    a=other+4; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
                    a=other+0x1f; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
                    a=other+0x23; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
                    a=other+0x3d; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
                    a=other+0xa3; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
                }
            }
            other=D(row+0x1e);
            if (other!=0) {
                a=other+4; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
                a=other+0x38; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
                a=other+0x53; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
                a=other+0x57; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
            }
            other=D(row+0x39);
            if (other!=0) {
                a=other+4; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
                a=other+0x38; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
            }
            a=D(row+0x39);
            if (a!=0) {
                other=D(a+0x57);
                if (other!=0) {
                    a=other+4; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
                    a=other+0x38; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
                    a=other+0x53; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
                }
            }
            a=D(row+0x39);
            if (a!=0) {
                other=D(a+0x53);
                if (other!=0) {
                    a=other+4; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
                    a=other+0x38; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
                    a=other+0x53; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
                    a=other+0x57; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
                }
            }
            a=D(row+0x39);
            if (a!=0) {
                a=D(a+0x53);
                if (a!=0) {
                    other=D(a+0x71);
                    if (other!=0) {
                        a=other+4; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
                        a=other+0x1f; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
                        a=other+0x23; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
                        a=other+0x3d; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
                        a=other+0xa3; if (a!=0 && cm_fn_00806260(a,query,0)!=0) { keep=1; break; }
                    }
                }
            }
            break;
        }
        if (keep) {
            D(out)=row; out+=4;
            a=D(self+0x38); D(self+0x38)=a+1;
        }
        row+=0x6e;
next:
        n=D(0xae2364);
        index++;
        offset+=0x4f;
        if ((s32)index>=(s32)n) return n;
    }
error:
    t=cm_fn_0090d130();
    cm_fn_00944c9f(0xae24d0,0x9870e8,t,p,q,line);
    a=cm_fn_005e8290(0x9870e0,0xae24d0,0);
    D(0xb67a34)=0;
    return a;
}
