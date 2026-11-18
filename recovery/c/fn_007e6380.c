#include "cmre.h"
/* Original instruction evidence: preferred-image VAs 007e6380--007e671a,
 * CM0102 3.9.68 SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * ECX receivers and stack arguments follow the MOV/LEA and PUSH instructions
 * immediately preceding each CALL; 007e671a pops the single entry argument.
 */
extern u32 CM_THISCALL cm_fn_0068d020(u32);
extern u32 CM_THISCALL cm_fn_0068d040(u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern void CM_FASTCALL cm_fn_0054a110(u16 *);
extern int CM_FASTCALL cm_fn_0054a3f0(short *);
/* 007e63fd--007e6427: receiver EDI, output buffer, mode 2.
 * 007e6411/007e642c consume the returned EAX as a pointer. */
extern u32 CM_THISCALL cm_fn_0054c5b0(short *, u32 *, int);
extern u32 CM_THISCALL cm_fn_0054c8e0(short *, u32 *, int);
/* Boundary carrier for EDX:EAX, not a semantic 64-bit result:
 * in the pinned image identified above, preferred-image VA 007e6441
 * is the last call before 007e6488 MOV DL and 007e6495 PUSH EDX.
 * 007e6446--007e6484 leave EDX unchanged, so its post-call upper
 * 24 bits must be captured rather than invented or zero-filled. */
extern unsigned long long CM_THISCALL cm_fn_00417c10(u32 *, u32 *, u16 *);
/* 007e648f--007e6498 loads ECX and pushes exactly three arguments. */
extern u32 CM_THISCALL cm_fn_005ae6a0(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_004b0700(u32);
extern u32 CM_STDCALL cm_fn_007e6ba0(u32,u32);
extern u32 CM_THISCALL cm_fn_005ae870(u8 *,u32,u32,u32,u32);
extern u8 CM_THISCALL cm_fn_005aa680(int,int,int);
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define W(a) CM_GLOBAL(volatile u16,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))

void CM_THISCALL cm_fn_007e6380(u32 self, u32 arg)
{
    volatile struct { u32 prev, handler, state; } seh;
    u8 scratch[0xb4];
    /* S denotes ESP after all four saved registers. The scratch allocation
     * starts at S+10h; SEH is separate and linked, not a scratch facsimile. */
    u32 s = (u32)scratch - 0x10;
    u32 a,b,c,d,e,esi,edi,ebp,n;
    seh.state = 0xffffffff;
    seh.handler = 0x96078c;
    seh.prev = CM_FS(u32,0);
    CM_FS(u32,0) = (u32)&seh;
    D(s+0x3c) = self;
    cm_fn_0068d020(s+0xa4);
    seh.state = 0;
    cm_fn_0068d020(s+0x10);
    *(volatile u8 *)&seh.state = 1;
    cm_fn_0068d020(s+0x40);
    *(volatile u8 *)&seh.state = 2;
    cm_fn_0054a110((u16 *)(s+0x24));
    edi = arg;
    *(volatile u8 *)&seh.state = 3;
    a = cm_fn_0054a3f0((short *)edi);
    if ((u16)a == 4) {
        a = cm_fn_0054c5b0((short *)edi,(u32 *)(s+0x34),2);
        b = s+0x2c;
        *(volatile u8 *)&seh.state = 4;
    } else {
        a = cm_fn_0054c8e0((short *)edi,(u32 *)(s+0xb4),2);
        b = s+0xbc;
        *(volatile u8 *)&seh.state = 5;
    }
    d = (u32)(cm_fn_00417c10((u32 *)(s+0x24),(u32 *)b,(u16 *)a) >> 32);
    esi = 0xb63d60;
    *(volatile u8 *)&seh.state = 3;
    D(s+0x2c) = esi;
    D(s+0x34) = 0x23;
    do {
        a = D(esi);
        if (a && B(a+0x11c)) {
            c = D(a);
            a = D(0x9cf300);
            if (c != a) {
                B(s+0x20) = 0;
                do {
                    /* 007e6488 changes DL only; 007e6495 pushes all EDX. */
                    d = (d & 0xffffff00u) | B(esi+0x14);
                    a = D(s+0x20);
                    c = D(0xae2a58);
                    a = cm_fn_005ae6a0(c,edi,a,d);
                    if (a) {
                        c = D(a); D(s+0x10) = c;
                        d = D(a+4); D(s+0x14) = d;
                        c = D(a+8); D(s+0x18) = c;
                        d = W(a+0xc); W(s+0x1c) = (u16)d;
                    }
                    a = D(s+0x12); D(s+0x1a) = a;
                    ebp = cm_fn_0068d1b0(s+0x10);
                    if (ebp) {
                        do {
                            c = D(ebp+0x14); d = D(c);
                            a = cm_fn_004b0700(d);
                            if (a) {
                                c = D(s+0x3c);
                                a = cm_fn_007e6ba0(ebp,1);
                                if (a) {
                                    esi = ebp; edi = s+0x50;
                                    for (n=0;n<0x13;n++) {
                                        a = D(esi); D(edi) = a;
                                        esi+=4; edi+=4;
                                    }
                                    W(edi) = W(esi);
                                    B(edi+2) = B(esi+2);
                                    a = D(ebp+0x14); c = D(a+0x5d);
                                    a = D(0x9cf4dc); d = D(c);
                                    if (d != a) {
                                        a = D(s+0x6c); d = D(s+0x70);
                                        D(s+0x6c) = d; D(s+0x70) = a;
                                        c = D(d); D(s+0x5c) = c;
                                        a = D(a); D(s+0x60) = a;
                                    } else d = D(s+0x6c);
                                    a = D(0x9cf684);
                                    esi = D(s+0x24); edi = D(s+0x26);
                                    W(s+0x9d) &= 0xfcff;
                                    c = a*3;
                                    D(s+0x50) = a;
                                    W(s+0x7a) = (u16)esi;
                                    W(s+0x78) = (u16)edi;
                                    c *= 9;
                                    B(s+0x90) = 0;
                                    c = c*4-a;
                                    a = D(0xae23d0); c += a;
                                    B(s+0x91) = 0; B(s+0x8b) = 0xff;
                                    B(s+0x8a) = 1; B(s+0x92) = 0xff;
                                    W(s+0x84) = 0; D(s+0x58) = 0xffffffff;
                                    D(s+0x64) = c; W(s+0x82) = 0;
                                    W(s+0x80) = 0;
                                    a = D(d+0x6e);
                                    if (a) {
                                        c = D(0xae2a58);
                                        a = cm_fn_005ae870((u8 *)c,a,esi,edi,0xffffffff);
                                        d = D(s+0x6c);
                                        if (!a) {
                                            a = D(d+0x6e);
                                            if (a) goto selected;
                                        }
                                    }
                                    d = D(d+0x69);
                                    if (!d) goto next;
                                    a = D(s+0x78); c = D(s+0x7a);
                                    e = D(0xae2a58);
                                    a = cm_fn_005ae870((u8 *)e,d,c,a,0xffffffff);
                                    if (a) goto next;
                                    d = D(s+0x6c); a = D(d+0x69);
                                    if (!a) goto next;
selected:
                                    D(s+0x68) = a;
                                    a = D(a); D(s+0x54) = a;
                                    c = D(ebp+0x14); a = D(c+0x5d);
                                    if (a) {
                                        d = D(a); a = D(0x9cf244);
                                        if (d == a) {
                                            a = D(s+0x9d);
                                            a = (a & 0xff44) | 0x44;
                                            W(s+0x9d) = (u16)a;
                                        }
                                    }
                                    c = D(0xae2a58);
                                    cm_fn_005aa680((int)c,(int)(s+0x50),1);
                                }
                            }
next:
                            ebp = cm_fn_0068d1b0(s+0x10);
                        } while (ebp);
                        edi = arg; esi = D(s+0x2c);
                    }
                    a = D(s+0x44); d = D(s+0x40); c = D(s+0x48);
                    D(s+0x14) = a; a = B(s+0x20);
                    /* 007e6684 loads EDX; 007e6698 replaces DX only.
                     * Preserve this value across 007e66ae's back edge. */
                    D(s+0x10) = d; d = (d & 0xffff0000u) | W(s+0x4c);
                    a = (u8)(a+1);
                    D(s+0x18) = c; W(s+0x1c) = (u16)d;
                    B(s+0x20) = (u8)a;
                } while ((s8)a <= 2);
            }
        }
        a = D(s+0x34); esi += 0x48; --a;
        D(s+0x2c) = esi; D(s+0x34) = a;
    } while (a);
    *(volatile u8 *)&seh.state = 1;
    cm_fn_0068d040(s+0x40);
    *(volatile u8 *)&seh.state = 0;
    cm_fn_0068d040(s+0x10);
    seh.state = 0xffffffff;
    cm_fn_0068d040(s+0xa4);
    c = seh.prev;
    CM_FS(u32,0) = c;
}
