/* Preferred-image VA00427F00, base00400000; target SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/RET0. Sole direct caller immediately calls 00427C20, which overwrites
   EAX before use. B=L-10 after SUB540 and four saves. One uninitialized
   scratch allocation keeps byte counters, two error buffers and 200 pointers. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define W(a) CM_GLOBAL(volatile u16,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
extern u32 CM_CDECL cm_fn_00945501(u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern void CM_CDECL cm_fn_00945333(u32,u32,u32,u32);

/* Each native comparison reads alternating bytes in pairs and stops at the
   first mismatch or equal NUL. Only equality is used by this routine. */
static int name_equal(u32 a,u32 b)
{
    u8 x,y;
    for(;;){
        x=B(a);y=B(b);if(x!=y)return 0;if(x==0)return 1;
        x=B(a+1);y=B(b+1);if(x!=y)return 0;
        a+=2;b+=2;if(x==0)return 1;
    }
}

void CM_THISCALL cm_fn_00427f00(u32 self)
{
    volatile u8 scratch[0x540];
    u32 pool=(u32)scratch, first=pool+0x20, second=pool+0x120;
    u32 vector=pool+0x220,p,n,v,t,a,c,idx,limit,id0,id1,id2,line,version;
    u8 x,y;
    /* B+18,+1B,+22,+21,+1F,+1A,+20,+1C,+17,+1D,+19,+1E,+23,
       +13,+16,+14,+15, in the original initialization order. */
    D(pool+0x18)=self;D(pool+0x14)=0;
    B(pool+8)=0;B(pool+0xb)=0;B(pool+0x12)=0;B(pool+0x11)=0;
    B(pool+0xf)=0;B(pool+0xa)=0;B(pool+0x10)=0;B(pool+0xc)=0;
    B(pool+7)=0;B(pool+0xd)=0;B(pool+9)=0;B(pool+0xe)=0;
    B(pool+0x13)=0;B(pool+3)=0;B(pool+6)=0;B(pool+4)=0;B(pool+5)=0;
    B(self+0x4b)=0xff;W(self+0x38)=0x47;W(self+0x36)=0x47;
    p=cm_fn_00945501(6,0x47);D(self+0x14)=p;
    if(p==0){
        cm_fn_00944cff(0x98a3bc,0,0,first,second);
        a=first;c=second;line=0x1e0;goto error;
    }
    n=D(0xae235c);p=D(0xae23b4);D(pool+0x1c)=0;
    if((s32)n>0){
        do{
            v=D(p+0x53);
            if(v!=0){
                t=D(v);v=D(0x9cf260);
                if(t==v){
                    a=p+4;
#define NAME(address,offset) if(name_equal(a,address)){v=D(self+0x14);D(v+offset)=p;}
                    NAME(0x98a398,0x000)
                    NAME(0x98aa20,0x006)
                    NAME(0x98aa0c,0x00c)
                    NAME(0x98a9f4,0x012)
                    NAME(0x98a9d4,0x018)
                    NAME(0x98a9b8,0x01e)
                    NAME(0x98a9a4,0x024)
                    NAME(0x98a988,0x02a)
                    NAME(0x98a974,0x030)
                    NAME(0x98a954,0x036)
                    NAME(0x98a93c,0x03c)
                    NAME(0x98a924,0x042)
                    NAME(0x98a8f8,0x048)
                    NAME(0x98a8e0,0x04e)
                    NAME(0x98a8b4,0x054)
                    NAME(0x98a8a0,0x05a)
                    NAME(0x98a880,0x060)
                    NAME(0x98a86c,0x066)
                    NAME(0x98a854,0x06c)
                    NAME(0x98a830,0x072)
                    NAME(0x98a818,0x078)
                    NAME(0x98a800,0x07e)
                    NAME(0x98a7dc,0x084)
                    NAME(0x98a7c0,0x08a)
                    NAME(0x98a7a8,0x090)
                    NAME(0x98a790,0x096)
                    NAME(0x98a770,0x09c)
                    NAME(0x98a758,0x0a2)
                    NAME(0x98a73c,0x0a8)
                    NAME(0x98a724,0x0ae)
                    NAME(0x98a70c,0x0b4)
                    NAME(0x98a6ec,0x0ba)
                    NAME(0x98a6d0,0x0c0)
                    NAME(0x98a6b4,0x0c6)
                    NAME(0x98a6a4,0x0cc)
                    NAME(0x98a68c,0x0d2)
                    NAME(0x98a67c,0x0d8)
                    NAME(0x98a664,0x0de)
                    NAME(0x98a650,0x0e4)
                    NAME(0x98a634,0x0ea)
                    NAME(0x98a61c,0x0f0)
                    NAME(0x98a600,0x0f6)
                    NAME(0x98a5e4,0x0fc)
                    NAME(0x98a5c4,0x102)
                    NAME(0x98a5ac,0x108)
                    NAME(0x98a58c,0x10e)
                    NAME(0x98a578,0x114)
                    NAME(0x98a560,0x11a)
                    NAME(0x98a544,0x120)
                    NAME(0x98a52c,0x126)
                    NAME(0x98a510,0x12c)
                    NAME(0x98a4f0,0x132)
                    NAME(0x98a4d4,0x138)
                    NAME(0x98a4bc,0x13e)
                    NAME(0x98a4a4,0x144)
                    NAME(0x98a484,0x14a)
                    NAME(0x98a46c,0x150)
                    NAME(0x98a450,0x156)
                    NAME(0x98a344,0x15c)
                    NAME(0x98a434,0x162)
                    NAME(0x98a420,0x168)
                    NAME(0x98a380,0x16e)
                    NAME(0x98a364,0x174)
                    NAME(0x98a294,0x17a)
                    NAME(0x98a408,0x180)
                    NAME(0x98a3f0,0x186)
                    NAME(0x98a328,0x18c)
                    NAME(0x98a304,0x192)
                    NAME(0x98a2e4,0x198)
                    NAME(0x98a2c8,0x19e)
                    NAME(0x98a2a8,0x1a4)
#undef NAME
                }
            }
            t=D(pool+0x1c);p+=0x245;++t;idx=(u32)(s32)(s16)t;
            D(pool+0x1c)=t;n=D(0xae235c);
        }while((s32)idx<(s32)n);
    }
    p=D(self+0x14);c=0;
    do{
        a=(u32)(s32)(s16)c*6u;
        if(D(p+a)==0)break;
        ++c;
    }while((s16)c<0x47);
    if((u16)c==0x47)return;
    n=D(0xae235c);p=D(0xae23b4);idx=0;
    if((s32)n>0){
        id0=D(0x9cf9c0);id1=D(0x9cf998);id2=D(0x9cf9c8);
        do{
            if((s16)W(pool+0x14)>=0xc8)break;
            v=D(p+0x60);
            if(v!=0){
                v=D(v);
                if(v==id0 || v==id1 || v==id2 || v==D(0x9cf9b4) ||
                   v==D(0x9cf9d4) || v==D(0x9cf994) || v==D(0x9cf9a4) ||
                   v==D(0x9cf9d0) || v==D(0x9cf9bc) || v==D(0x9cf9dc) ||
                   v==D(0x9cf9b0) || v==D(0x9cf9e4)){
                    a=(u32)(s32)(s16)W(pool+0x14);
                    D(vector+a*4)=p;t=D(pool+0x14);++t;D(pool+0x14)=t;
                }
            }
            ++idx;p+=0x245;t=(u32)(s32)(s16)idx;n=D(0xae235c);
        }while((s32)t<(s32)n);
    }
    t=D(pool+0x14);n=(u32)(s32)(s16)t;
    cm_fn_00945333(vector,n,4,0x4b07d0);
    if((s16)t<=0)goto incomplete;
    limit=n;a=vector;
    do{
        c=D(a);id0=D(0x9cf9c0);v=D(c+0x60);v=D(v);
        /* Each branch loads the destination before updating its flag. The
           destination can overlap records; retain every DWORD reload. */
#define FIRST(flag,counter,base) do{ \
        x=B(pool+flag);if(x==0){ \
            idx=(u32)(s32)(s8)B(pool+counter);p=D(self+0x14);idx+=base; \
            B(pool+flag)=1;D(p+idx*6u)=c; \
            x=B(pool+counter);++x;B(pool+counter)=x;D(a)=0; \
        } \
    }while(0)
        if(v==id0){FIRST(0xf,4,0x3d);}
        else if(v==D(0x9cf998)){FIRST(0xe,6,0x36);}
        else if(v==D(0x9cf9c8)){FIRST(0xa,5,0x42);}
        else if(v==D(0x9cf9b4)){FIRST(0x10,4,0x3d);}
        else if(v==D(0x9cf9d4)){FIRST(0xc,5,0x42);}
        else if(v==D(0x9cf994)){FIRST(0x12,6,0x36);}
        else if(v==D(0x9cf9a4)){FIRST(0x11,4,0x3d);}
        else if(v==D(0x9cf9d0)){FIRST(7,5,0x42);}
        else if(v==D(0x9cf9bc)){FIRST(0xd,4,0x3d);}
        else if(v==D(0x9cf9dc)){FIRST(8,5,0x42);}
        else if(v==D(0x9cf9b0)){FIRST(9,4,0x3d);}
        else if(v==D(0x9cf9e4)){FIRST(0xb,5,0x42);}
#undef FIRST
        p=D(a);
        if(p!=0){
            c=D(p+0x57);
            if(c!=0){
                c=D(c);id0=D(0x9cf9ec);
                if(c==id0 && (s8)B(pool+0x13)<5){
                    idx=(u32)(s32)(s8)B(pool+6);v=D(self+0x14);idx+=0x36;
                    D(v+idx*6u)=p;x=B(pool+6);y=B(pool+0x13);++x;++y;
                    B(pool+6)=x;B(pool+0x13)=y;D(a)=0;
                }
            }
        }
        a+=4;--limit;
    }while(limit!=0);
    t=D(pool+0x14);if((s16)t<=0)goto incomplete;
    a=vector;
    do{
        p=D(a);
        if(p!=0){
            v=D(p+0x60);id0=D(0x9cf9c0);v=D(v);
#define REST(flag,max) do{ \
        if((s8)B(pool+flag)<max){ \
            idx=(u32)(s32)(s8)B(pool+3);c=D(self+0x14);D(c+idx*6u)=p; \
            x=B(pool+3);y=B(pool+flag);++x;++y;B(pool+3)=x;B(pool+flag)=y; \
        } \
    }while(0)
            if(v==id0){REST(0xf,5);}
            else if(v==D(0x9cf998)){REST(0xe,7);}
            else if(v==D(0x9cf9c8)){REST(0xa,5);}
            else if(v==D(0x9cf9b4)){REST(0x10,5);}
            else if(v==D(0x9cf9d4)){REST(0xc,5);}
            else if(v==D(0x9cf994)){REST(0x12,7);}
            else if(v==D(0x9cf9a4)){REST(0x11,7);}
            else if(v==D(0x9cf9d0)){REST(7,5);}
            else if(v==D(0x9cf9bc)){REST(0xd,5);}
            else if(v==D(0x9cf9dc)){REST(8,5);}
            else if(v==D(0x9cf9b0)){REST(9,5);}
            else if(v==D(0x9cf9e4)){REST(0xb,5);}
#undef REST
        }
        a+=4;--n;
    }while(n!=0);
    if(B(pool+3)==0x36 && B(pool+6)==7 && B(pool+4)==5 && B(pool+5)==5)return;
incomplete:
    cm_fn_00944cff(0x98a3bc,0,0,second,first);
    a=second;c=first;line=0x3ce;
error:
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0xae24d0,0x9870e8,version,a,c,line);
    cm_fn_005e8290(0x9870e0,0xae24d0,0);
    D(0xb67a34)=0;
}
