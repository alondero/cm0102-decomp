/* Preferred-image VA 0055d420, base 00400000; target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Allocate the 43-slot owner table and conditionally construct its entries.
   Original allocators/constructors/error helpers remain explicit boundaries.
   Return preserves the original final EAX, including the final table-base load.
   Scratch starts at original post-SUB ESP: allocation word + two 256-byte buffers.
   State and owner-slot accesses follow the listing; arithmetic wraps as u32. */
#include "cmre.h"
extern void *CM_CDECL cm_fn_00945501(u32,u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_0090d130(void *,void *,u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(const char *,const char *,u32);
extern u32 CM_CDECL cm_fn_006699d0(u32);
extern u32 CM_THISCALL cm_fn_007db100(u32,u32);
extern u32 CM_THISCALL cm_fn_007db360(u32,u32);
extern u32 CM_THISCALL cm_fn_007db5b0(u32,u32);
extern u32 CM_THISCALL cm_fn_007db8f0(u32,u32);
extern u32 CM_THISCALL cm_fn_007dbca0(u32,u32);
extern u32 CM_THISCALL cm_fn_007dc080(u32,u32);
extern u32 CM_THISCALL cm_fn_007dc3c0(u32,u32);
extern u32 CM_THISCALL cm_fn_007dc700(u32,u32);
extern u32 CM_THISCALL cm_fn_007dcae0(u32,u32);
extern u32 CM_THISCALL cm_fn_007dcf90(u32,u32);
extern u32 CM_THISCALL cm_fn_007dd400(u32,u32);
extern u32 CM_THISCALL cm_fn_007dd660(u32,u32);
extern u32 CM_THISCALL cm_fn_007dd990(u32,u32);
extern u32 CM_THISCALL cm_fn_007ddcc0(u32,u32);
extern u32 CM_THISCALL cm_fn_007de2e0(u32,u32);
extern u32 CM_THISCALL cm_fn_007de620(u32,u32);
extern u32 CM_THISCALL cm_fn_007de960(u32,u32);
extern u32 CM_THISCALL cm_fn_007ded90(u32,u32);
extern u32 CM_THISCALL cm_fn_007df180(u32,u32);
extern u32 CM_THISCALL cm_fn_007df480(u32,u32);
extern u32 CM_THISCALL cm_fn_007df960(u32,u32);
extern u32 CM_THISCALL cm_fn_007dfd70(u32,u32);
extern u32 CM_THISCALL cm_fn_007e01b0(u32,u32);
extern u32 CM_THISCALL cm_fn_007e0490(u32,u32);
extern u32 CM_THISCALL cm_fn_007e0730(u32,u32);
extern u32 CM_THISCALL cm_fn_007e0af0(u32,u32);
extern u32 CM_THISCALL cm_fn_007e0ef0(u32,u32);
extern u32 CM_THISCALL cm_fn_007e1150(u32,u32);
extern u32 CM_THISCALL cm_fn_007e13d0(u32,u32);
extern u32 CM_THISCALL cm_fn_007e1610(u32,u32);
extern u32 CM_THISCALL cm_fn_007e1a60(u32,u32);
extern u32 CM_THISCALL cm_fn_007e1e00(u32,u32);
extern u32 CM_THISCALL cm_fn_007e21a0(u32,u32);
extern u32 CM_THISCALL cm_fn_007e2470(u32,u32);
extern u32 CM_THISCALL cm_fn_007e29e0(u32,u32);
extern u32 CM_THISCALL cm_fn_007e2c30(u32,u32);
extern u32 CM_THISCALL cm_fn_007e2f80(u32,u32);
extern u32 CM_THISCALL cm_fn_007e32d0(u32,u32);
extern u32 CM_THISCALL cm_fn_007e3650(u32,u32);
extern u32 CM_THISCALL cm_fn_007e3a10(u32,u32);
extern u32 CM_THISCALL cm_fn_007e3d60(u32,u32);
extern u32 CM_THISCALL cm_fn_007e40f0(u32,u32);
extern u32 CM_THISCALL cm_fn_007e43c0(u32,u32);

#define WORD(address) (*(volatile u32 *)(u32)(address))
#define BYTE(address) (*(volatile u8 *)(u32)(address))
u32 CM_THISCALL cm_fn_0055d420(u32 owner, u32 input)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x204];
    u32 eax, ecx, edx;
    frame.prev = CM_FS(u32,0);
    frame.handler = 0x00957fd6u;
    frame.state = 0xffffffffu;
    CM_FS(u32,0) = (u32)&frame;
    eax = (u32)cm_fn_00945501(0x2bu,4u);
    WORD(owner) = eax;
    if (!eax) {
        cm_fn_00944cff(0x009b8f18u,0,0,scratch+0x104,scratch+4);
        eax = cm_fn_0090d130(scratch+0x104,scratch+4,0x161e);
        cm_fn_00944c9f(0x00ae24d0u,0x009870e8u,eax,
                      (u32)(scratch+0x104),(u32)(scratch+4),0x161eu);
        eax = cm_fn_005e8290((const char *)0x009870e0u,
                            (const char *)0x00ae24d0u,0);
        WORD(0x00b67a34u) = 0;
        goto finished;
    }
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055d4bb */
    *(volatile u32 *)scratch = eax; /* 0055d4c3 */
    frame.state = 0u; /* 0055d4ce */
    if (eax == 0) goto L_0055d4e7; /* 0055d4db */
    eax = cm_fn_007df180(eax,input); /* 0055d4e0 */
    goto L_0055d4e9; /* 0055d4e5 */
L_0055d4e7:;
    eax = 0; /* 0055d4e7 */
L_0055d4e9:;
    ecx = WORD(owner); /* 0055d4e9 */
    frame.state = 0xffffffffu; /* 0055d4f1 */
    WORD(ecx) = eax; /* 0055d4f8 */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055d4fa */
    *(volatile u32 *)scratch = eax; /* 0055d502 */
    frame.state = 1u; /* 0055d508 */
    if (eax == 0) goto L_0055d51f; /* 0055d513 */
    eax = cm_fn_007dd400(eax,input); /* 0055d518 */
    goto L_0055d521; /* 0055d51d */
L_0055d51f:;
    eax = 0; /* 0055d51f */
L_0055d521:;
    edx = WORD(owner); /* 0055d521 */
    frame.state = 0xffffffffu; /* 0055d525 */
    WORD(edx + 4) = eax; /* 0055d52c */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055d52f */
    *(volatile u32 *)scratch = eax; /* 0055d537 */
    frame.state = 2u; /* 0055d53d */
    if (eax == 0) goto L_0055d554; /* 0055d548 */
    eax = cm_fn_007e29e0(eax,input); /* 0055d54d */
    goto L_0055d556; /* 0055d552 */
L_0055d554:;
    eax = 0; /* 0055d554 */
L_0055d556:;
    ecx = WORD(owner); /* 0055d556 */
    frame.state = 0xffffffffu; /* 0055d558 */
    WORD(ecx + 8) = eax; /* 0055d55f */
    eax = WORD(0x9cf218u); /* 0055d562 */
    edx = eax + eax*8; /* 0055d567 */
    edx <<= 4; /* 0055d56a */
    edx += eax; /* 0055d56d */
    eax = WORD(0xae23a8u); /* 0055d56f */
    ecx = (ecx & 0xffffff00u) | BYTE(eax + edx*2 + 0x11c); /* 0055d574 */
    if ((u8)ecx == 0) goto L_0055d5b4; /* 0055d57d */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055d581 */
    *(volatile u32 *)scratch = eax; /* 0055d589 */
    frame.state = 3u; /* 0055d58f */
    if (eax == 0) goto L_0055d5a6; /* 0055d59a */
    eax = cm_fn_007db100(eax,input); /* 0055d59f */
    goto L_0055d5a8; /* 0055d5a4 */
L_0055d5a6:;
    eax = 0; /* 0055d5a6 */
L_0055d5a8:;
    ecx = WORD(owner); /* 0055d5a8 */
    frame.state = 0xffffffffu; /* 0055d5aa */
    WORD(ecx + 0xc) = eax; /* 0055d5b1 */
L_0055d5b4:;
    eax = WORD(0x9cf244u); /* 0055d5b4 */
    edx = eax + eax*8; /* 0055d5b9 */
    edx <<= 4; /* 0055d5bc */
    edx += eax; /* 0055d5bf */
    eax = WORD(0xae23a8u); /* 0055d5c1 */
    ecx = (ecx & 0xffffff00u) | BYTE(eax + edx*2 + 0x11c); /* 0055d5c6 */
    if ((u8)ecx == 0) goto L_0055d63b; /* 0055d5cf */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055d5d3 */
    *(volatile u32 *)scratch = eax; /* 0055d5db */
    frame.state = 4u; /* 0055d5e1 */
    if (eax == 0) goto L_0055d5f8; /* 0055d5ec */
    eax = cm_fn_007dbca0(eax,input); /* 0055d5f1 */
    goto L_0055d5fa; /* 0055d5f6 */
L_0055d5f8:;
    eax = 0; /* 0055d5f8 */
L_0055d5fa:;
    ecx = WORD(owner); /* 0055d5fa */
    frame.state = 0xffffffffu; /* 0055d5fe */
    WORD(ecx + 0x10) = eax; /* 0055d605 */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055d608 */
    *(volatile u32 *)scratch = eax; /* 0055d610 */
    frame.state = 5u; /* 0055d616 */
    if (eax == 0) goto L_0055d62d; /* 0055d621 */
    eax = cm_fn_007db8f0(eax,input); /* 0055d626 */
    goto L_0055d62f; /* 0055d62b */
L_0055d62d:;
    eax = 0; /* 0055d62d */
L_0055d62f:;
    edx = WORD(owner); /* 0055d62f */
    frame.state = 0xffffffffu; /* 0055d631 */
    WORD(edx + 0x14) = eax; /* 0055d638 */
L_0055d63b:;
    eax = WORD(0x9cf260u); /* 0055d63b */
    edx = WORD(0xae23a8u); /* 0055d640 */
    ecx = eax + eax*8; /* 0055d646 */
    ecx <<= 4; /* 0055d649 */
    ecx += eax; /* 0055d64c */
    eax = (eax & 0xffffff00u) | BYTE(edx + ecx*2 + 0x11c); /* 0055d64e */
    if ((u8)eax == 0) goto L_0055d6c3; /* 0055d657 */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055d65b */
    *(volatile u32 *)scratch = eax; /* 0055d663 */
    frame.state = 6u; /* 0055d669 */
    if (eax == 0) goto L_0055d680; /* 0055d674 */
    eax = cm_fn_007dc080(eax,input); /* 0055d679 */
    goto L_0055d682; /* 0055d67e */
L_0055d680:;
    eax = 0; /* 0055d680 */
L_0055d682:;
    ecx = WORD(owner); /* 0055d682 */
    frame.state = 0xffffffffu; /* 0055d686 */
    WORD(ecx + 0x18) = eax; /* 0055d68d */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055d690 */
    *(volatile u32 *)scratch = eax; /* 0055d698 */
    frame.state = 7u; /* 0055d69e */
    if (eax == 0) goto L_0055d6b5; /* 0055d6a9 */
    eax = cm_fn_007dc3c0(eax,input); /* 0055d6ae */
    goto L_0055d6b7; /* 0055d6b3 */
L_0055d6b5:;
    eax = 0; /* 0055d6b5 */
L_0055d6b7:;
    edx = WORD(owner); /* 0055d6b7 */
    frame.state = 0xffffffffu; /* 0055d6b9 */
    WORD(edx + 0x1c) = eax; /* 0055d6c0 */
L_0055d6c3:;
    eax = WORD(0x9cf2c0u); /* 0055d6c3 */
    edx = WORD(0xae23a8u); /* 0055d6c8 */
    ecx = eax + eax*8; /* 0055d6ce */
    ecx <<= 4; /* 0055d6d1 */
    ecx += eax; /* 0055d6d4 */
    eax = (eax & 0xffffff00u) | BYTE(edx + ecx*2 + 0x11c); /* 0055d6d6 */
    if ((u8)eax == 0) goto L_0055d716; /* 0055d6df */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055d6e3 */
    *(volatile u32 *)scratch = eax; /* 0055d6eb */
    frame.state = 8u; /* 0055d6f1 */
    if (eax == 0) goto L_0055d708; /* 0055d6fc */
    eax = cm_fn_007dcae0(eax,input); /* 0055d701 */
    goto L_0055d70a; /* 0055d706 */
L_0055d708:;
    eax = 0; /* 0055d708 */
L_0055d70a:;
    ecx = WORD(owner); /* 0055d70a */
    frame.state = 0xffffffffu; /* 0055d70c */
    WORD(ecx + 0x20) = eax; /* 0055d713 */
L_0055d716:;
    eax = WORD(0x9cf2e4u); /* 0055d716 */
    edx = eax + eax*8; /* 0055d71b */
    edx <<= 4; /* 0055d71e */
    edx += eax; /* 0055d721 */
    eax = WORD(0xae23a8u); /* 0055d723 */
    ecx = (ecx & 0xffffff00u) | BYTE(eax + edx*2 + 0x11c); /* 0055d728 */
    if ((u8)ecx == 0) goto L_0055d768; /* 0055d731 */
    eax = (u32)cm_fn_00944cf1(0x2bu); /* 0055d735 */
    *(volatile u32 *)scratch = eax; /* 0055d73d */
    frame.state = 9u; /* 0055d743 */
    if (eax == 0) goto L_0055d75a; /* 0055d74e */
    eax = cm_fn_007dcf90(eax,input); /* 0055d753 */
    goto L_0055d75c; /* 0055d758 */
L_0055d75a:;
    eax = 0; /* 0055d75a */
L_0055d75c:;
    ecx = WORD(owner); /* 0055d75c */
    frame.state = 0xffffffffu; /* 0055d75e */
    WORD(ecx + 0x24) = eax; /* 0055d765 */
L_0055d768:;
    eax = WORD(0x9cf2fcu); /* 0055d768 */
    edx = eax + eax*8; /* 0055d76d */
    edx <<= 4; /* 0055d770 */
    edx += eax; /* 0055d773 */
    eax = WORD(0xae23a8u); /* 0055d775 */
    ecx = (ecx & 0xffffff00u) | BYTE(eax + edx*2 + 0x11c); /* 0055d77a */
    if ((u8)ecx == 0) goto L_0055d7ef; /* 0055d783 */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055d787 */
    *(volatile u32 *)scratch = eax; /* 0055d78f */
    frame.state = 0xau; /* 0055d795 */
    if (eax == 0) goto L_0055d7ac; /* 0055d7a0 */
    eax = cm_fn_007dd990(eax,input); /* 0055d7a5 */
    goto L_0055d7ae; /* 0055d7aa */
L_0055d7ac:;
    eax = 0; /* 0055d7ac */
L_0055d7ae:;
    ecx = WORD(owner); /* 0055d7ae */
    frame.state = 0xffffffffu; /* 0055d7b2 */
    WORD(ecx + 0x28) = eax; /* 0055d7b9 */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055d7bc */
    *(volatile u32 *)scratch = eax; /* 0055d7c4 */
    frame.state = 0xbu; /* 0055d7ca */
    if (eax == 0) goto L_0055d7e1; /* 0055d7d5 */
    eax = cm_fn_007dd660(eax,input); /* 0055d7da */
    goto L_0055d7e3; /* 0055d7df */
L_0055d7e1:;
    eax = 0; /* 0055d7e1 */
L_0055d7e3:;
    edx = WORD(owner); /* 0055d7e3 */
    frame.state = 0xffffffffu; /* 0055d7e5 */
    WORD(edx + 0x2c) = eax; /* 0055d7ec */
L_0055d7ef:;
    eax = WORD(0x9cf300u); /* 0055d7ef */
    edx = WORD(0xae23a8u); /* 0055d7f4 */
    ecx = eax + eax*8; /* 0055d7fa */
    ecx <<= 4; /* 0055d7fd */
    ecx += eax; /* 0055d800 */
    eax = (eax & 0xffffff00u) | BYTE(edx + ecx*2 + 0x11c); /* 0055d802 */
    if ((u8)eax == 0) goto L_0055d842; /* 0055d80b */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055d80f */
    *(volatile u32 *)scratch = eax; /* 0055d817 */
    frame.state = 0xcu; /* 0055d81d */
    if (eax == 0) goto L_0055d834; /* 0055d828 */
    eax = cm_fn_007ddcc0(eax,input); /* 0055d82d */
    goto L_0055d836; /* 0055d832 */
L_0055d834:;
    eax = 0; /* 0055d834 */
L_0055d836:;
    ecx = WORD(owner); /* 0055d836 */
    frame.state = 0xffffffffu; /* 0055d838 */
    WORD(ecx + 0x30) = eax; /* 0055d83f */
L_0055d842:;
    eax = WORD(0x9cf314u); /* 0055d842 */
    edx = eax + eax*8; /* 0055d847 */
    edx <<= 4; /* 0055d84a */
    edx += eax; /* 0055d84d */
    eax = WORD(0xae23a8u); /* 0055d84f */
    ecx = (ecx & 0xffffff00u) | BYTE(eax + edx*2 + 0x11c); /* 0055d854 */
    if ((u8)ecx == 0) goto L_0055d8c9; /* 0055d85d */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055d861 */
    *(volatile u32 *)scratch = eax; /* 0055d869 */
    frame.state = 0xdu; /* 0055d86f */
    if (eax == 0) goto L_0055d886; /* 0055d87a */
    eax = cm_fn_007de620(eax,input); /* 0055d87f */
    goto L_0055d888; /* 0055d884 */
L_0055d886:;
    eax = 0; /* 0055d886 */
L_0055d888:;
    ecx = WORD(owner); /* 0055d888 */
    frame.state = 0xffffffffu; /* 0055d88c */
    WORD(ecx + 0x34) = eax; /* 0055d893 */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055d896 */
    *(volatile u32 *)scratch = eax; /* 0055d89e */
    frame.state = 0xeu; /* 0055d8a4 */
    if (eax == 0) goto L_0055d8bb; /* 0055d8af */
    eax = cm_fn_007de2e0(eax,input); /* 0055d8b4 */
    goto L_0055d8bd; /* 0055d8b9 */
L_0055d8bb:;
    eax = 0; /* 0055d8bb */
L_0055d8bd:;
    edx = WORD(owner); /* 0055d8bd */
    frame.state = 0xffffffffu; /* 0055d8bf */
    WORD(edx + 0x38) = eax; /* 0055d8c6 */
L_0055d8c9:;
    eax = WORD(0x9cf338u); /* 0055d8c9 */
    edx = WORD(0xae23a8u); /* 0055d8ce */
    ecx = eax + eax*8; /* 0055d8d4 */
    ecx <<= 4; /* 0055d8d7 */
    ecx += eax; /* 0055d8da */
    eax = (eax & 0xffffff00u) | BYTE(edx + ecx*2 + 0x11c); /* 0055d8dc */
    if ((u8)eax == 0) goto L_0055d91c; /* 0055d8e5 */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055d8e9 */
    *(volatile u32 *)scratch = eax; /* 0055d8f1 */
    frame.state = 0xfu; /* 0055d8f7 */
    if (eax == 0) goto L_0055d90e; /* 0055d902 */
    eax = cm_fn_007ded90(eax,input); /* 0055d907 */
    goto L_0055d910; /* 0055d90c */
L_0055d90e:;
    eax = 0; /* 0055d90e */
L_0055d910:;
    ecx = WORD(owner); /* 0055d910 */
    frame.state = 0xffffffffu; /* 0055d912 */
    WORD(ecx + 0x3c) = eax; /* 0055d919 */
L_0055d91c:;
    eax = WORD(0x9cf2dcu); /* 0055d91c */
    edx = eax + eax*8; /* 0055d921 */
    edx <<= 4; /* 0055d924 */
    edx += eax; /* 0055d927 */
    eax = WORD(0xae23a8u); /* 0055d929 */
    ecx = (ecx & 0xffffff00u) | BYTE(eax + edx*2 + 0x11c); /* 0055d92e */
    if ((u8)ecx == 0) goto L_0055d96e; /* 0055d937 */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055d93b */
    *(volatile u32 *)scratch = eax; /* 0055d943 */
    frame.state = 0x10u; /* 0055d949 */
    if (eax == 0) goto L_0055d960; /* 0055d954 */
    eax = cm_fn_007df480(eax,input); /* 0055d959 */
    goto L_0055d962; /* 0055d95e */
L_0055d960:;
    eax = 0; /* 0055d960 */
L_0055d962:;
    ecx = WORD(owner); /* 0055d962 */
    frame.state = 0xffffffffu; /* 0055d964 */
    WORD(ecx + 0x40) = eax; /* 0055d96b */
L_0055d96e:;
    eax = WORD(0x9cf360u); /* 0055d96e */
    edx = eax + eax*8; /* 0055d973 */
    edx <<= 4; /* 0055d976 */
    edx += eax; /* 0055d979 */
    eax = WORD(0xae23a8u); /* 0055d97b */
    ecx = (ecx & 0xffffff00u) | BYTE(eax + edx*2 + 0x11c); /* 0055d980 */
    if ((u8)ecx == 0) goto L_0055d9f5; /* 0055d989 */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055d98d */
    *(volatile u32 *)scratch = eax; /* 0055d995 */
    frame.state = 0x11u; /* 0055d99b */
    if (eax == 0) goto L_0055d9b2; /* 0055d9a6 */
    eax = cm_fn_007dfd70(eax,input); /* 0055d9ab */
    goto L_0055d9b4; /* 0055d9b0 */
L_0055d9b2:;
    eax = 0; /* 0055d9b2 */
L_0055d9b4:;
    ecx = WORD(owner); /* 0055d9b4 */
    frame.state = 0xffffffffu; /* 0055d9b8 */
    WORD(ecx + 0x44) = eax; /* 0055d9bf */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055d9c2 */
    *(volatile u32 *)scratch = eax; /* 0055d9ca */
    frame.state = 0x12u; /* 0055d9d0 */
    if (eax == 0) goto L_0055d9e7; /* 0055d9db */
    eax = cm_fn_007df960(eax,input); /* 0055d9e0 */
    goto L_0055d9e9; /* 0055d9e5 */
L_0055d9e7:;
    eax = 0; /* 0055d9e7 */
L_0055d9e9:;
    edx = WORD(owner); /* 0055d9e9 */
    frame.state = 0xffffffffu; /* 0055d9eb */
    WORD(edx + 0x48) = eax; /* 0055d9f2 */
L_0055d9f5:;
    eax = WORD(0x9cf36cu); /* 0055d9f5 */
    edx = WORD(0xae23a8u); /* 0055d9fa */
    ecx = eax + eax*8; /* 0055da00 */
    ecx <<= 4; /* 0055da03 */
    ecx += eax; /* 0055da06 */
    eax = (eax & 0xffffff00u) | BYTE(edx + ecx*2 + 0x11c); /* 0055da08 */
    if ((u8)eax == 0) goto L_0055da7d; /* 0055da11 */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055da15 */
    *(volatile u32 *)scratch = eax; /* 0055da1d */
    frame.state = 0x13u; /* 0055da23 */
    if (eax == 0) goto L_0055da3a; /* 0055da2e */
    eax = cm_fn_007e0490(eax,input); /* 0055da33 */
    goto L_0055da3c; /* 0055da38 */
L_0055da3a:;
    eax = 0; /* 0055da3a */
L_0055da3c:;
    ecx = WORD(owner); /* 0055da3c */
    frame.state = 0xffffffffu; /* 0055da40 */
    WORD(ecx + 0x4c) = eax; /* 0055da47 */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055da4a */
    *(volatile u32 *)scratch = eax; /* 0055da52 */
    frame.state = 0x14u; /* 0055da58 */
    if (eax == 0) goto L_0055da6f; /* 0055da63 */
    eax = cm_fn_007e01b0(eax,input); /* 0055da68 */
    goto L_0055da71; /* 0055da6d */
L_0055da6f:;
    eax = 0; /* 0055da6f */
L_0055da71:;
    edx = WORD(owner); /* 0055da71 */
    frame.state = 0xffffffffu; /* 0055da73 */
    WORD(edx + 0x50) = eax; /* 0055da7a */
L_0055da7d:;
    eax = WORD(0x9cf408u); /* 0055da7d */
    edx = WORD(0xae23a8u); /* 0055da82 */
    ecx = eax + eax*8; /* 0055da88 */
    ecx <<= 4; /* 0055da8b */
    ecx += eax; /* 0055da8e */
    eax = (eax & 0xffffff00u) | BYTE(edx + ecx*2 + 0x11c); /* 0055da90 */
    if ((u8)eax == 0) goto L_0055db05; /* 0055da99 */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055da9d */
    *(volatile u32 *)scratch = eax; /* 0055daa5 */
    frame.state = 0x15u; /* 0055daab */
    if (eax == 0) goto L_0055dac2; /* 0055dab6 */
    eax = cm_fn_007e1150(eax,input); /* 0055dabb */
    goto L_0055dac4; /* 0055dac0 */
L_0055dac2:;
    eax = 0; /* 0055dac2 */
L_0055dac4:;
    ecx = WORD(owner); /* 0055dac4 */
    frame.state = 0xffffffffu; /* 0055dac8 */
    WORD(ecx + 0x54) = eax; /* 0055dacf */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055dad2 */
    *(volatile u32 *)scratch = eax; /* 0055dada */
    frame.state = 0x16u; /* 0055dae0 */
    if (eax == 0) goto L_0055daf7; /* 0055daeb */
    eax = cm_fn_007e0ef0(eax,input); /* 0055daf0 */
    goto L_0055daf9; /* 0055daf5 */
L_0055daf7:;
    eax = 0; /* 0055daf7 */
L_0055daf9:;
    edx = WORD(owner); /* 0055daf9 */
    frame.state = 0xffffffffu; /* 0055dafb */
    WORD(edx + 0x58) = eax; /* 0055db02 */
L_0055db05:;
    eax = WORD(0x9cf434u); /* 0055db05 */
    edx = WORD(0xae23a8u); /* 0055db0a */
    ecx = eax + eax*8; /* 0055db10 */
    ecx <<= 4; /* 0055db13 */
    ecx += eax; /* 0055db16 */
    eax = (eax & 0xffffff00u) | BYTE(edx + ecx*2 + 0x11c); /* 0055db18 */
    if ((u8)eax == 0) goto L_0055db58; /* 0055db21 */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055db25 */
    *(volatile u32 *)scratch = eax; /* 0055db2d */
    frame.state = 0x17u; /* 0055db33 */
    if (eax == 0) goto L_0055db4a; /* 0055db3e */
    eax = cm_fn_007e1a60(eax,input); /* 0055db43 */
    goto L_0055db4c; /* 0055db48 */
L_0055db4a:;
    eax = 0; /* 0055db4a */
L_0055db4c:;
    ecx = WORD(owner); /* 0055db4c */
    frame.state = 0xffffffffu; /* 0055db4e */
    WORD(ecx + 0x5c) = eax; /* 0055db55 */
L_0055db58:;
    eax = WORD(0x9cf45cu); /* 0055db58 */
    edx = eax + eax*8; /* 0055db5d */
    edx <<= 4; /* 0055db60 */
    edx += eax; /* 0055db63 */
    eax = WORD(0xae23a8u); /* 0055db65 */
    ecx = (ecx & 0xffffff00u) | BYTE(eax + edx*2 + 0x11c); /* 0055db6a */
    if ((u8)ecx == 0) goto L_0055dbdf; /* 0055db73 */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055db77 */
    *(volatile u32 *)scratch = eax; /* 0055db7f */
    frame.state = 0x18u; /* 0055db85 */
    if (eax == 0) goto L_0055db9c; /* 0055db90 */
    eax = cm_fn_007e2470(eax,input); /* 0055db95 */
    goto L_0055db9e; /* 0055db9a */
L_0055db9c:;
    eax = 0; /* 0055db9c */
L_0055db9e:;
    ecx = WORD(owner); /* 0055db9e */
    frame.state = 0xffffffffu; /* 0055dba2 */
    WORD(ecx + 0x60) = eax; /* 0055dba9 */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055dbac */
    *(volatile u32 *)scratch = eax; /* 0055dbb4 */
    frame.state = 0x19u; /* 0055dbba */
    if (eax == 0) goto L_0055dbd1; /* 0055dbc5 */
    eax = cm_fn_007e21a0(eax,input); /* 0055dbca */
    goto L_0055dbd3; /* 0055dbcf */
L_0055dbd1:;
    eax = 0; /* 0055dbd1 */
L_0055dbd3:;
    edx = WORD(owner); /* 0055dbd3 */
    frame.state = 0xffffffffu; /* 0055dbd5 */
    WORD(edx + 0x64) = eax; /* 0055dbdc */
L_0055dbdf:;
    eax = WORD(0x9cf488u); /* 0055dbdf */
    edx = WORD(0xae23a8u); /* 0055dbe4 */
    ecx = eax + eax*8; /* 0055dbea */
    ecx <<= 4; /* 0055dbed */
    ecx += eax; /* 0055dbf0 */
    eax = (eax & 0xffffff00u) | BYTE(edx + ecx*2 + 0x11c); /* 0055dbf2 */
    if ((u8)eax == 0) goto L_0055dc67; /* 0055dbfb */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055dbff */
    *(volatile u32 *)scratch = eax; /* 0055dc07 */
    frame.state = 0x1au; /* 0055dc0d */
    if (eax == 0) goto L_0055dc24; /* 0055dc18 */
    eax = cm_fn_007e2f80(eax,input); /* 0055dc1d */
    goto L_0055dc26; /* 0055dc22 */
L_0055dc24:;
    eax = 0; /* 0055dc24 */
L_0055dc26:;
    ecx = WORD(owner); /* 0055dc26 */
    frame.state = 0xffffffffu; /* 0055dc2a */
    WORD(ecx + 0x68) = eax; /* 0055dc31 */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055dc34 */
    *(volatile u32 *)scratch = eax; /* 0055dc3c */
    frame.state = 0x1bu; /* 0055dc42 */
    if (eax == 0) goto L_0055dc59; /* 0055dc4d */
    eax = cm_fn_007e2c30(eax,input); /* 0055dc52 */
    goto L_0055dc5b; /* 0055dc57 */
L_0055dc59:;
    eax = 0; /* 0055dc59 */
L_0055dc5b:;
    edx = WORD(owner); /* 0055dc5b */
    frame.state = 0xffffffffu; /* 0055dc5d */
    WORD(edx + 0x6c) = eax; /* 0055dc64 */
L_0055dc67:;
    eax = WORD(0x9cf4a8u); /* 0055dc67 */
    edx = WORD(0xae23a8u); /* 0055dc6c */
    ecx = eax + eax*8; /* 0055dc72 */
    ecx <<= 4; /* 0055dc75 */
    ecx += eax; /* 0055dc78 */
    eax = (eax & 0xffffff00u) | BYTE(edx + ecx*2 + 0x11c); /* 0055dc7a */
    if ((u8)eax == 0) goto L_0055dcef; /* 0055dc83 */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055dc87 */
    *(volatile u32 *)scratch = eax; /* 0055dc8f */
    frame.state = 0x1cu; /* 0055dc95 */
    if (eax == 0) goto L_0055dcac; /* 0055dca0 */
    eax = cm_fn_007e3650(eax,input); /* 0055dca5 */
    goto L_0055dcae; /* 0055dcaa */
L_0055dcac:;
    eax = 0; /* 0055dcac */
L_0055dcae:;
    ecx = WORD(owner); /* 0055dcae */
    frame.state = 0xffffffffu; /* 0055dcb2 */
    WORD(ecx + 0x70) = eax; /* 0055dcb9 */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055dcbc */
    *(volatile u32 *)scratch = eax; /* 0055dcc4 */
    frame.state = 0x1du; /* 0055dcca */
    if (eax == 0) goto L_0055dce1; /* 0055dcd5 */
    eax = cm_fn_007e32d0(eax,input); /* 0055dcda */
    goto L_0055dce3; /* 0055dcdf */
L_0055dce1:;
    eax = 0; /* 0055dce1 */
L_0055dce3:;
    edx = WORD(owner); /* 0055dce3 */
    frame.state = 0xffffffffu; /* 0055dce5 */
    WORD(edx + 0x74) = eax; /* 0055dcec */
L_0055dcef:;
    eax = WORD(0x9cf4f8u); /* 0055dcef */
    edx = WORD(0xae23a8u); /* 0055dcf4 */
    ecx = eax + eax*8; /* 0055dcfa */
    ecx <<= 4; /* 0055dcfd */
    ecx += eax; /* 0055dd00 */
    eax = (eax & 0xffffff00u) | BYTE(edx + ecx*2 + 0x11c); /* 0055dd02 */
    if ((u8)eax == 0) goto L_0055dd42; /* 0055dd0b */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055dd0f */
    *(volatile u32 *)scratch = eax; /* 0055dd17 */
    frame.state = 0x1eu; /* 0055dd1d */
    if (eax == 0) goto L_0055dd34; /* 0055dd28 */
    eax = cm_fn_007e40f0(eax,input); /* 0055dd2d */
    goto L_0055dd36; /* 0055dd32 */
L_0055dd34:;
    eax = 0; /* 0055dd34 */
L_0055dd36:;
    ecx = WORD(owner); /* 0055dd36 */
    frame.state = 0xffffffffu; /* 0055dd38 */
    WORD(ecx + 0x78) = eax; /* 0055dd3f */
L_0055dd42:;
    eax = WORD(0x9cf430u); /* 0055dd42 */
    edx = eax + eax*8; /* 0055dd47 */
    edx <<= 4; /* 0055dd4a */
    edx += eax; /* 0055dd4d */
    eax = WORD(0xae23a8u); /* 0055dd4f */
    ecx = (ecx & 0xffffff00u) | BYTE(eax + edx*2 + 0x11c); /* 0055dd54 */
    if ((u8)ecx == 0) goto L_0055dd94; /* 0055dd5d */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055dd61 */
    *(volatile u32 *)scratch = eax; /* 0055dd69 */
    frame.state = 0x1fu; /* 0055dd6f */
    if (eax == 0) goto L_0055dd86; /* 0055dd7a */
    eax = cm_fn_007e1610(eax,input); /* 0055dd7f */
    goto L_0055dd88; /* 0055dd84 */
L_0055dd86:;
    eax = 0; /* 0055dd86 */
L_0055dd88:;
    ecx = WORD(owner); /* 0055dd88 */
    frame.state = 0xffffffffu; /* 0055dd8a */
    WORD(ecx + 0x7c) = eax; /* 0055dd91 */
L_0055dd94:;
    eax = WORD(0x9cf31cu); /* 0055dd94 */
    edx = eax + eax*8; /* 0055dd99 */
    edx <<= 4; /* 0055dd9c */
    edx += eax; /* 0055dd9f */
    eax = WORD(0xae23a8u); /* 0055dda1 */
    ecx = (ecx & 0xffffff00u) | BYTE(eax + edx*2 + 0x11c); /* 0055dda6 */
    if ((u8)ecx == 0) goto L_0055dde9; /* 0055ddaf */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055ddb3 */
    *(volatile u32 *)scratch = eax; /* 0055ddbb */
    frame.state = 0x20u; /* 0055ddc1 */
    if (eax == 0) goto L_0055ddd8; /* 0055ddcc */
    eax = cm_fn_007de960(eax,input); /* 0055ddd1 */
    goto L_0055ddda; /* 0055ddd6 */
L_0055ddd8:;
    eax = 0; /* 0055ddd8 */
L_0055ddda:;
    ecx = WORD(owner); /* 0055ddda */
    frame.state = 0xffffffffu; /* 0055dddc */
    WORD(ecx + 0x80) = eax; /* 0055dde3 */
L_0055dde9:;
    eax = WORD(0x9cf2acu); /* 0055dde9 */
    edx = eax + eax*8; /* 0055ddee */
    edx <<= 4; /* 0055ddf1 */
    edx += eax; /* 0055ddf4 */
    eax = WORD(0xae23a8u); /* 0055ddf6 */
    ecx = (ecx & 0xffffff00u) | BYTE(eax + edx*2 + 0x11c); /* 0055ddfb */
    if ((u8)ecx == 0) goto L_0055de3e; /* 0055de04 */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055de08 */
    *(volatile u32 *)scratch = eax; /* 0055de10 */
    frame.state = 0x21u; /* 0055de16 */
    if (eax == 0) goto L_0055de2d; /* 0055de21 */
    eax = cm_fn_007dc700(eax,input); /* 0055de26 */
    goto L_0055de2f; /* 0055de2b */
L_0055de2d:;
    eax = 0; /* 0055de2d */
L_0055de2f:;
    ecx = WORD(owner); /* 0055de2f */
    frame.state = 0xffffffffu; /* 0055de31 */
    WORD(ecx + 0x84) = eax; /* 0055de38 */
L_0055de3e:;
    eax = WORD(0x9cf224u); /* 0055de3e */
    edx = eax + eax*8; /* 0055de43 */
    edx <<= 4; /* 0055de46 */
    edx += eax; /* 0055de49 */
    eax = WORD(0xae23a8u); /* 0055de4b */
    ecx = (ecx & 0xffffff00u) | BYTE(eax + edx*2 + 0x11c); /* 0055de50 */
    if ((u8)ecx == 0) goto L_0055de93; /* 0055de59 */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055de5d */
    *(volatile u32 *)scratch = eax; /* 0055de65 */
    frame.state = 0x22u; /* 0055de6b */
    if (eax == 0) goto L_0055de82; /* 0055de76 */
    eax = cm_fn_007db5b0(eax,input); /* 0055de7b */
    goto L_0055de84; /* 0055de80 */
L_0055de82:;
    eax = 0; /* 0055de82 */
L_0055de84:;
    ecx = WORD(owner); /* 0055de84 */
    frame.state = 0xffffffffu; /* 0055de86 */
    WORD(ecx + 0x88) = eax; /* 0055de8d */
L_0055de93:;
    ecx = WORD(0x9cfa18u); /* 0055de93 */
    eax = ecx; /* 0055de99 */
    eax <<= 5; /* 0055de9b */
    eax += ecx; /* 0055de9e */
    edx = eax + eax*2; /* 0055dea0 */
    eax = WORD(0xae23a4u); /* 0055dea3 */
    ecx = eax + edx*2; /* 0055dea8 */
    eax = cm_fn_006699d0(ecx); /* 0055deac */
    if (eax == 0) goto L_0055def0; /* 0055deb6 */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055deba */
    *(volatile u32 *)scratch = eax; /* 0055dec2 */
    frame.state = 0x23u; /* 0055dec8 */
    if (eax == 0) goto L_0055dedf; /* 0055ded3 */
    eax = cm_fn_007e13d0(eax,input); /* 0055ded8 */
    goto L_0055dee1; /* 0055dedd */
L_0055dedf:;
    eax = 0; /* 0055dedf */
L_0055dee1:;
    edx = WORD(owner); /* 0055dee1 */
    frame.state = 0xffffffffu; /* 0055dee3 */
    WORD(edx + 0x8c) = eax; /* 0055deea */
L_0055def0:;
    eax = WORD(0x9cf3e4u); /* 0055def0 */
    edx = WORD(0xae23a8u); /* 0055def5 */
    ecx = eax + eax*8; /* 0055defb */
    ecx <<= 4; /* 0055defe */
    ecx += eax; /* 0055df01 */
    eax = (eax & 0xffffff00u) | BYTE(edx + ecx*2 + 0x11c); /* 0055df03 */
    if ((u8)eax == 0) goto L_0055df46; /* 0055df0c */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055df10 */
    *(volatile u32 *)scratch = eax; /* 0055df18 */
    frame.state = 0x24u; /* 0055df1e */
    if (eax == 0) goto L_0055df35; /* 0055df29 */
    eax = cm_fn_007e0af0(eax,input); /* 0055df2e */
    goto L_0055df37; /* 0055df33 */
L_0055df35:;
    eax = 0; /* 0055df35 */
L_0055df37:;
    ecx = WORD(owner); /* 0055df37 */
    frame.state = 0xffffffffu; /* 0055df39 */
    WORD(ecx + 0x90) = eax; /* 0055df40 */
L_0055df46:;
    eax = WORD(0x9cf4dcu); /* 0055df46 */
    edx = eax + eax*8; /* 0055df4b */
    edx <<= 4; /* 0055df4e */
    edx += eax; /* 0055df51 */
    eax = WORD(0xae23a8u); /* 0055df53 */
    ecx = (ecx & 0xffffff00u) | BYTE(eax + edx*2 + 0x11c); /* 0055df58 */
    if ((u8)ecx == 0) goto L_0055dfd3; /* 0055df61 */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055df65 */
    *(volatile u32 *)scratch = eax; /* 0055df6d */
    frame.state = 0x25u; /* 0055df73 */
    if (eax == 0) goto L_0055df8a; /* 0055df7e */
    eax = cm_fn_007e3d60(eax,input); /* 0055df83 */
    goto L_0055df8c; /* 0055df88 */
L_0055df8a:;
    eax = 0; /* 0055df8a */
L_0055df8c:;
    ecx = WORD(owner); /* 0055df8c */
    frame.state = 0xffffffffu; /* 0055df90 */
    WORD(ecx + 0x94) = eax; /* 0055df97 */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055df9d */
    *(volatile u32 *)scratch = eax; /* 0055dfa5 */
    frame.state = 0x26u; /* 0055dfab */
    if (eax == 0) goto L_0055dfc2; /* 0055dfb6 */
    eax = cm_fn_007e3a10(eax,input); /* 0055dfbb */
    goto L_0055dfc4; /* 0055dfc0 */
L_0055dfc2:;
    eax = 0; /* 0055dfc2 */
L_0055dfc4:;
    edx = WORD(owner); /* 0055dfc4 */
    frame.state = 0xffffffffu; /* 0055dfc6 */
    WORD(edx + 0x98) = eax; /* 0055dfcd */
L_0055dfd3:;
    eax = WORD(0x9cf448u); /* 0055dfd3 */
    edx = WORD(0xae23a8u); /* 0055dfd8 */
    ecx = eax + eax*8; /* 0055dfde */
    ecx <<= 4; /* 0055dfe1 */
    ecx += eax; /* 0055dfe4 */
    eax = (eax & 0xffffff00u) | BYTE(edx + ecx*2 + 0x11c); /* 0055dfe6 */
    if ((u8)eax == 0) goto L_0055e029; /* 0055dfef */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055dff3 */
    *(volatile u32 *)scratch = eax; /* 0055dffb */
    frame.state = 0x27u; /* 0055e001 */
    if (eax == 0) goto L_0055e018; /* 0055e00c */
    eax = cm_fn_007e1e00(eax,input); /* 0055e011 */
    goto L_0055e01a; /* 0055e016 */
L_0055e018:;
    eax = 0; /* 0055e018 */
L_0055e01a:;
    ecx = WORD(owner); /* 0055e01a */
    frame.state = 0xffffffffu; /* 0055e01c */
    WORD(ecx + 0x9c) = eax; /* 0055e023 */
L_0055e029:;
    eax = WORD(0x9cf510u); /* 0055e029 */
    edx = eax + eax*8; /* 0055e02e */
    edx <<= 4; /* 0055e031 */
    edx += eax; /* 0055e034 */
    eax = WORD(0xae23a8u); /* 0055e036 */
    ecx = (ecx & 0xffffff00u) | BYTE(eax + edx*2 + 0x11c); /* 0055e03b */
    if ((u8)ecx == 0) goto L_0055e07e; /* 0055e044 */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055e048 */
    *(volatile u32 *)scratch = eax; /* 0055e050 */
    frame.state = 0x28u; /* 0055e056 */
    if (eax == 0) goto L_0055e06d; /* 0055e061 */
    eax = cm_fn_007e43c0(eax,input); /* 0055e066 */
    goto L_0055e06f; /* 0055e06b */
L_0055e06d:;
    eax = 0; /* 0055e06d */
L_0055e06f:;
    ecx = WORD(owner); /* 0055e06f */
    frame.state = 0xffffffffu; /* 0055e071 */
    WORD(ecx + 0xa0) = eax; /* 0055e078 */
L_0055e07e:;
    eax = WORD(0xdd7110u); /* 0055e07e */
    edx = (eax & 0x80000000u) ? 0xffffffffu : 0; /* 0055e085 */
    eax ^= edx; /* 0055e086 */
    eax -= edx; /* 0055e088 */
    if ((s32)eax >= 0x3e044) goto L_0055e0d9; /* 0055e08f */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055e091 */
    *(volatile u32 *)scratch = eax; /* 0055e099 */
    frame.state = 0x29u; /* 0055e09f */
    if (eax == 0) goto L_0055e0c6; /* 0055e0aa */
    eax = cm_fn_007db360(eax,0); /* 0055e0b0 */
    edx = WORD(owner); /* 0055e0b5 */
    frame.state = 0xffffffffu; /* 0055e0b7 */
    WORD(edx + 0xa4) = eax; /* 0055e0be */
    goto L_0055e10f; /* 0055e0c4 */
L_0055e0c6:;
    edx = WORD(owner); /* 0055e0c6 */
    eax = 0; /* 0055e0c8 */
    frame.state = 0xffffffffu; /* 0055e0ca */
    WORD(edx + 0xa4) = eax; /* 0055e0d1 */
    goto L_0055e10f; /* 0055e0d7 */
L_0055e0d9:;
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055e0d9 */
    *(volatile u32 *)scratch = eax; /* 0055e0e1 */
    frame.state = 0x2au; /* 0055e0e7 */
    if (eax == 0) goto L_0055e0fe; /* 0055e0f2 */
    eax = cm_fn_007db360(eax,input); /* 0055e0f7 */
    goto L_0055e100; /* 0055e0fc */
L_0055e0fe:;
    eax = 0; /* 0055e0fe */
L_0055e100:;
    ecx = WORD(owner); /* 0055e100 */
    frame.state = 0xffffffffu; /* 0055e102 */
    WORD(ecx + 0xa4) = eax; /* 0055e109 */
L_0055e10f:;
    eax = WORD(0x9cf484u); /* 0055e10f */
    edx = eax + eax*8; /* 0055e115 */
    edx <<= 4; /* 0055e118 */
    edx += eax; /* 0055e11b */
    eax = WORD(0xae23a8u); /* 0055e11d */
    ecx = (ecx & 0xffffff00u) | BYTE(eax + edx*2 + 0x11c); /* 0055e122 */
    if ((u8)ecx == 0) goto L_0055e1a9; /* 0055e12b */
    eax = WORD(0xdd7110u); /* 0055e12d */
    edx = (eax & 0x80000000u) ? 0xffffffffu : 0; /* 0055e134 */
    eax ^= edx; /* 0055e135 */
    eax -= edx; /* 0055e137 */
    if ((s32)eax >= 0x3e047) goto L_0055e17a; /* 0055e13e */
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055e140 */
    *(volatile u32 *)scratch = eax; /* 0055e148 */
    frame.state = 0x2bu; /* 0055e14e */
    if (eax == 0) goto L_0055e16e; /* 0055e159 */
    eax = cm_fn_007e0730(eax,0); /* 0055e15f */
    ecx = WORD(owner); /* 0055e164 */
    WORD(ecx + 0xa8) = eax; /* 0055e166 */
    goto L_0055e1a9; /* 0055e16c */
L_0055e16e:;
    ecx = WORD(owner); /* 0055e16e */
    eax = 0; /* 0055e170 */
    WORD(ecx + 0xa8) = eax; /* 0055e172 */
    goto L_0055e1a9; /* 0055e178 */
L_0055e17a:;
    eax = (u32)cm_fn_00944cf1(0x23u); /* 0055e17a */
    *(volatile u32 *)scratch = eax; /* 0055e182 */
    frame.state = 0x2cu; /* 0055e188 */
    if (eax == 0) goto L_0055e19f; /* 0055e193 */
    eax = cm_fn_007e0730(eax,input); /* 0055e198 */
    goto L_0055e1a1; /* 0055e19d */
L_0055e19f:;
    eax = 0; /* 0055e19f */
L_0055e1a1:;
    edx = WORD(owner); /* 0055e1a1 */
    WORD(edx + 0xa8) = eax; /* 0055e1a3 */
L_0055e1a9:
finished:
    CM_FS(u32,0) = frame.prev;
    return eax;
}
