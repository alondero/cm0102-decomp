#include "cmre.h"
/* Original-instruction authority: CM0102 3.9.68 SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * Preferred-image VAs 0077af20..0077b61e: ECX receiver, RET 8.
 * Calls: 0077afa7/0077b0ad five stack args; 0077b088 six;
 * 0077b10d/0077b36b two; 0077b1be one; 0077b4a8 etc three.
 */
extern u16 CM_THISCALL cm_fn_008ae840(u32);
extern s32 CM_THISCALL cm_fn_008aed80(u32, void *, s32, void *, void *, void *);
extern s32 CM_THISCALL cm_fn_008aec90(u32, void *, u32, u32, void *, u32, u32);
extern u32 CM_STDCALL cm_fn_008ae140(void *, void *);
extern u32 CM_STDCALL cm_fn_008aee00(void *);
extern s32 CM_CDECL cm_fn_00946fa0(void *, void *, u32);
extern s32 CM_THISCALL cm_fn_008ae9c0(u32, s16, void *, s32);
#define W(a) CM_GLOBAL(volatile u16,(u32)(a))
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
static u32 sl(u32 a) { u32 n=0; while (CM_GLOBAL(volatile u8,a+n)) ++n; return n; }
static s32 sc(u32 a,u32 b) {
    u8 x,y;
    do { x=CM_GLOBAL(volatile u8,a++); y=CM_GLOBAL(volatile u8,b++);
         if(x!=y) return x<y?-1:1; } while(x);
    return 0;
}
static void cp(u32 d,u32 s,u32 n) {
    while(n>=4) { D(d)=D(s); d+=4;s+=4;n-=4; }
    while(n--) { CM_GLOBAL(volatile u8,d)=CM_GLOBAL(volatile u8,s);++d;++s; }
}
static void cs(u32 d,u32 s) { cp(d,s,sl(s)+1); }
u32 CM_THISCALL cm_fn_0077af20(u32 self, void *out, u16 *index) {
    u8 scratch[0x838];
    u32 t=(u32)scratch, buf=t+0x68, from=t+0x24;
    u32 i,n,a,b,cb,changed,j;
    s32 r,k,count;
    if(D(self+0xc3a5e)) {
        for(i=0;i<16;++i) if(W(self+0x4ba+i*2)==0xffff) break;
        if(i<16) {
            u16 h=cm_fn_008ae840(self);
            W(self+0x4ba+i*2)=h;
            if(h!=0xffff) {
                D(self+0x4da+i*4)=0xfffffffc;
                D(self+0x51a+i*4)=0xfffffffc;
                cb=D(self+0xc3a62);
                if(cb) ((void (CM_CDECL *)(u32))cb)(i+1);
            }
        }
        r=cm_fn_008aed80(self,(void *)buf,2000,(void *)from,(void *)(t+0x18),(void *)(t+0x1c));
        if(r>0 && sc(buf,0xa649d4)==0) {
            n=sl(0xa64994); cp(buf,0xa64994,n);
            a=D(t+0x18);
            n=sl(0xa64994); cp(buf+n,self+0xc3c60,50);
            n=sl(0xa64994); cp(buf+n+50,self+0xc3a6c,50);
            b=D(t+0x1c);
            n=sl(0xa64994);
            cm_fn_008aec90(self,(void *)buf,n+100,0x1a0b,(void *)from,a,b);
        }
    } else {
        r=cm_fn_008aed80(self,(void *)buf,2000,(void *)from,(void *)(t+0x18),(void *)(t+0x1c));
        if(r>0) {
            n=sl(0xa64994);
            r=cm_fn_00946fa0((void *)buf,(void *)0xa64994,n);
            if(r==0 && (s16)W(self+0xc3a6a)<9) {
                count=(s16)W(self+0xc3a6a);
                cm_fn_008ae140((void *)from,(void *)(self+0xc3c60+(u32)count*50));
                n=sl(0xa64994);
                count=(s16)W(self+0xc3a6a);
                cp(self+0xc3a6c+(u32)count*50,buf+n+50,50);
                count=(s16)W(self+0xc3a6a);
                k=0;
                while(k<count) {
                    if(sc(self+0xc3c60+(u32)count*50,self+0xc3c60+(u32)k*50)==0) break;
                    ++k;
                }
                if(k==count && cm_fn_008aee00((void *)from)) {
                    W(self+0xc3a6a)=W(self+0xc3a6a)+1;
                    do {
                        changed=0;
                        for(j=1;(s32)j<(s16)W(self+0xc3a6a);++j) {
                            a=self+0xc3c60+j*50;
                            if(sc(a-0x226,a-0x1f4)>0) {
                                cs(buf,a-50);cs(a-50,a);cs(a,buf);
                                cs(buf,a-0x226);cs(a-0x226,a-0x1f4);cs(a-0x1f4,buf);
                                changed=1;
                            }
                        }
                    } while(changed);
                }
            } else {
                n=sl(0xa64980);
                if(cm_fn_00946fa0((void *)buf,(void *)0xa64980,n)==0) {
                    cm_fn_008ae140((void *)from,(void *)(t+0x34));
                    count=(s16)W(self+0xc3a6a);
                    k=0;
                    while(k<count) {
                        if(sc(t+0x34,self+0xc3c60+(u32)k*50)==0) {
                            ++k;
                            while(k<count) {
                                a=self+0xc3c2e + (u32)k*50;
                                cs(a,a+50);
                                cs(a-0x1f4,a-0x1c2);
                                ++k;
                                count=(s16)W(self+0xc3a6a);
                            }
                            W(self+0xc3a6a)=W(self+0xc3a6a)-1;
                        }
                        count=(s16)W(self+0xc3a6a);
                        ++k;
                    }
                    count=(s16)W(self+0xc3a6a);
                    if(k==count) W(self+0xc3a6a)=W(self+0xc3a6a)+1;
                }
            }
        }
    }
    for(i=0;i<16;++i) {
        a=self+0x51a+i*4;
        b=self+0x4ba+i*2;
        if(W(b)==0xffff) continue;
        r=(s32)D(a);
        if(r<0) {
            u32 off=i*4;
            r=cm_fn_008ae9c0(self,(s16)W(b),(void *)(self+0x4de + off+(u32)r),1);
            while(r>0) {
                D(a)=D(a)+1;
                if(D(a)==0) {
                    n=D(a-0x40);
                    if(n) { D(a)=D(a-0x40); break; }
                    D(a)=0xfffffffc;
                }
                r=cm_fn_008ae9c0(self,(s16)W(b),(void *)(self+0x4de + off+D(a)),1);
            }
            if(r==-1 || r==-2) {
                W(b)=0xffff;
                cb=D(self+0xc3a66);
                if(cb) ((void (CM_CDECL *)(u32))cb)(i+1);
            }
        }
        if((s32)D(a)>0) {
            n=D(a-0x40);
            r=cm_fn_008ae9c0(self,(s16)W(b),(void *)(self+0x55a+i*50000-n+D(a)),(s32)n);
            while(r>0) {
                n=D(a-0x40)-(u32)r;
                D(a-0x40)=n;
                if((s32)n<=0) {
                    *index=(u16)(i+1);
                    n=D(self+0x51a+i*4);
                    cp((u32)out,self+0x55a+i*50000,n);
                    D(self+0x51a+i*4)=0xfffffffc;
                    return 1;
                }
                r=cm_fn_008ae9c0(self,(s16)W(b),(void *)(self+0x55a+i*50000-n+D(a)),(s32)n);
            }
            if(r==-1 || r==-2) {
                W(b)=0xffff;
                cb=D(self+0xc3a66);
                if(cb) ((void (CM_CDECL *)(u32))cb)(i+1);
            }
        }
    }
    return 0;
}
