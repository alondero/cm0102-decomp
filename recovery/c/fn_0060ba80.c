#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define DAT_009d00a4 CM_GLOBAL(volatile u32, 0x009d00a4)
#define DAT_009d00a8 CM_GLOBAL(volatile u32, 0x009d00a8)
#define DAT_009d00ac CM_GLOBAL(volatile u32, 0x009d00ac)
#define DAT_009d00b0 CM_GLOBAL(volatile u32, 0x009d00b0)
#define DAT_009d00b4 CM_GLOBAL(volatile u32, 0x009d00b4)
#define DAT_009d00b8 CM_GLOBAL(volatile u32, 0x009d00b8)
#define DAT_009d00bc CM_GLOBAL(volatile u32, 0x009d00bc)
#define DAT_009d00c0 CM_GLOBAL(volatile u32, 0x009d00c0)
#define DAT_009d00c4 CM_GLOBAL(volatile u32, 0x009d00c4)
#define DAT_009d00c8 CM_GLOBAL(volatile u32, 0x009d00c8)
#define DAT_009d00cc CM_GLOBAL(volatile u32, 0x009d00cc)
#define DAT_009d00d0 CM_GLOBAL(volatile u32, 0x009d00d0)
#define DAT_009d00d4 CM_GLOBAL(volatile u32, 0x009d00d4)
#define DAT_009d00d8 CM_GLOBAL(volatile u32, 0x009d00d8)
#define DAT_009d00dc CM_GLOBAL(volatile u32, 0x009d00dc)
#define DAT_009d00e0 CM_GLOBAL(volatile u32, 0x009d00e0)
#define DAT_009d00e4 CM_GLOBAL(volatile u32, 0x009d00e4)
#define DAT_009d00e8 CM_GLOBAL(volatile u32, 0x009d00e8)
#define DAT_009d00ec CM_GLOBAL(volatile u32, 0x009d00ec)
#define DAT_009d00f0 CM_GLOBAL(volatile u32, 0x009d00f0)
#define DAT_009d00f4 CM_GLOBAL(volatile u32, 0x009d00f4)
#define DAT_009d00f8 CM_GLOBAL(volatile u32, 0x009d00f8)
#define DAT_009d00fc CM_GLOBAL(volatile u32, 0x009d00fc)
#define DAT_009d0100 CM_GLOBAL(volatile u32, 0x009d0100)
#define DAT_009d0104 CM_GLOBAL(volatile u32, 0x009d0104)
#define DAT_009d0108 CM_GLOBAL(volatile u32, 0x009d0108)
#define DAT_009d010c CM_GLOBAL(volatile u32, 0x009d010c)
#define DAT_009d0110 CM_GLOBAL(volatile u32, 0x009d0110)
#define DAT_009d0114 CM_GLOBAL(volatile u32, 0x009d0114)
#define DAT_009d0118 CM_GLOBAL(volatile u32, 0x009d0118)
#define DAT_009d011c CM_GLOBAL(volatile u32, 0x009d011c)
#define DAT_009d0120 CM_GLOBAL(volatile u32, 0x009d0120)
#define DAT_009d0124 CM_GLOBAL(volatile u32, 0x009d0124)
#define DAT_009d0128 CM_GLOBAL(volatile u32, 0x009d0128)
#define DAT_009d012c CM_GLOBAL(volatile u32, 0x009d012c)
#define DAT_009d0130 CM_GLOBAL(volatile u32, 0x009d0130)
#define DAT_009d0134 CM_GLOBAL(volatile u32, 0x009d0134)
#define DAT_009d0138 CM_GLOBAL(volatile u32, 0x009d0138)
#define DAT_009d013c CM_GLOBAL(volatile u32, 0x009d013c)
#define DAT_009d0140 CM_GLOBAL(volatile u32, 0x009d0140)
#define DAT_009d0144 CM_GLOBAL(volatile u32, 0x009d0144)
#define DAT_009d0148 CM_GLOBAL(volatile u32, 0x009d0148)
#define DAT_009d014c CM_GLOBAL(volatile u32, 0x009d014c)
#define DAT_009d0150 CM_GLOBAL(volatile u32, 0x009d0150)
#define DAT_009d0154 CM_GLOBAL(volatile u32, 0x009d0154)
#define DAT_009d0158 CM_GLOBAL(volatile u32, 0x009d0158)
#define DAT_009d015c CM_GLOBAL(volatile u32, 0x009d015c)
#define DAT_009d0160 CM_GLOBAL(volatile u32, 0x009d0160)
#define DAT_009d0164 CM_GLOBAL(volatile u32, 0x009d0164)
#define DAT_009d0168 CM_GLOBAL(volatile u32, 0x009d0168)
#define DAT_009d016c CM_GLOBAL(volatile u32, 0x009d016c)
#define DAT_009d0170 CM_GLOBAL(volatile u32, 0x009d0170)
#define DAT_009d0174 CM_GLOBAL(volatile u32, 0x009d0174)
#define DAT_009d0178 CM_GLOBAL(volatile u32, 0x009d0178)
#define DAT_009d017c CM_GLOBAL(volatile u32, 0x009d017c)
#define DAT_009d0180 CM_GLOBAL(volatile u32, 0x009d0180)
#define DAT_009d0184 CM_GLOBAL(volatile u32, 0x009d0184)
#define DAT_009d0188 CM_GLOBAL(volatile u32, 0x009d0188)
#define DAT_009d018c CM_GLOBAL(volatile u32, 0x009d018c)
#define DAT_009d0190 CM_GLOBAL(volatile u32, 0x009d0190)
#define DAT_009d0194 CM_GLOBAL(volatile u32, 0x009d0194)
#define DAT_009d0198 CM_GLOBAL(volatile u32, 0x009d0198)
#define DAT_009d019c CM_GLOBAL(volatile u32, 0x009d019c)
#define DAT_009d01a0 CM_GLOBAL(volatile u32, 0x009d01a0)
#define DAT_009d01a4 CM_GLOBAL(volatile u32, 0x009d01a4)
#define DAT_009d01a8 CM_GLOBAL(volatile u32, 0x009d01a8)
#define DAT_009d01ac CM_GLOBAL(volatile u32, 0x009d01ac)
#define DAT_009d01b0 CM_GLOBAL(volatile u32, 0x009d01b0)
#define DAT_009d01b4 CM_GLOBAL(volatile u32, 0x009d01b4)
#define DAT_009d01b8 CM_GLOBAL(volatile u32, 0x009d01b8)
#define DAT_009d01bc CM_GLOBAL(volatile u32, 0x009d01bc)
#define DAT_009d01c0 CM_GLOBAL(volatile u32, 0x009d01c0)
#define DAT_009d01c4 CM_GLOBAL(volatile u32, 0x009d01c4)
#define DAT_009d01c8 CM_GLOBAL(volatile u32, 0x009d01c8)
#define DAT_009d01cc CM_GLOBAL(volatile u32, 0x009d01cc)
#define DAT_009d01d0 CM_GLOBAL(volatile u32, 0x009d01d0)
#define DAT_009d01d4 CM_GLOBAL(volatile u32, 0x009d01d4)
#define DAT_009d01d8 CM_GLOBAL(volatile u32, 0x009d01d8)
#define DAT_009d01dc CM_GLOBAL(volatile u32, 0x009d01dc)
#define DAT_009d01e0 CM_GLOBAL(volatile u32, 0x009d01e0)
#define DAT_009d01e4 CM_GLOBAL(volatile u32, 0x009d01e4)
#define DAT_009d01e8 CM_GLOBAL(volatile u32, 0x009d01e8)
#define DAT_009d01ec CM_GLOBAL(volatile u32, 0x009d01ec)
#define DAT_009d01f0 CM_GLOBAL(volatile u32, 0x009d01f0)
#define DAT_009d01f4 CM_GLOBAL(volatile u32, 0x009d01f4)
#define DAT_009d01f8 CM_GLOBAL(volatile u32, 0x009d01f8)
#define DAT_009d01fc CM_GLOBAL(volatile u32, 0x009d01fc)
#define DAT_009d0200 CM_GLOBAL(volatile u32, 0x009d0200)
#define DAT_009d0204 CM_GLOBAL(volatile u32, 0x009d0204)
#define DAT_009d0208 CM_GLOBAL(volatile u32, 0x009d0208)
#define DAT_009d020c CM_GLOBAL(volatile u32, 0x009d020c)
#define DAT_009d0210 CM_GLOBAL(volatile u32, 0x009d0210)
#define DAT_009d0214 CM_GLOBAL(volatile u32, 0x009d0214)
#define DAT_009d0218 CM_GLOBAL(volatile u32, 0x009d0218)
#define DAT_009d021c CM_GLOBAL(volatile u32, 0x009d021c)
#define DAT_009d0220 CM_GLOBAL(volatile u32, 0x009d0220)
#define DAT_009d0224 CM_GLOBAL(volatile u32, 0x009d0224)
#define DAT_009d0228 CM_GLOBAL(volatile u32, 0x009d0228)
#define DAT_009d022c CM_GLOBAL(volatile u32, 0x009d022c)
#define DAT_009d0230 CM_GLOBAL(volatile u32, 0x009d0230)
#define DAT_009d0234 CM_GLOBAL(volatile u32, 0x009d0234)
#define DAT_009d0238 CM_GLOBAL(volatile u32, 0x009d0238)
#define DAT_009d023c CM_GLOBAL(volatile u32, 0x009d023c)
#define DAT_009d0240 CM_GLOBAL(volatile u32, 0x009d0240)
#define DAT_009d0244 CM_GLOBAL(volatile u32, 0x009d0244)
#define DAT_009d0248 CM_GLOBAL(volatile u32, 0x009d0248)
#define DAT_009d024c CM_GLOBAL(volatile u32, 0x009d024c)
#define DAT_009d0250 CM_GLOBAL(volatile u32, 0x009d0250)
#define DAT_009d0254 CM_GLOBAL(volatile u32, 0x009d0254)
#define DAT_009d0258 CM_GLOBAL(volatile u32, 0x009d0258)
#define DAT_009d025c CM_GLOBAL(volatile u32, 0x009d025c)
#define DAT_009d0260 CM_GLOBAL(volatile u32, 0x009d0260)
#define DAT_009d0264 CM_GLOBAL(volatile u32, 0x009d0264)
#define DAT_009d0268 CM_GLOBAL(volatile u32, 0x009d0268)
#define DAT_009d026c CM_GLOBAL(volatile u32, 0x009d026c)
#define DAT_009d0270 CM_GLOBAL(volatile u32, 0x009d0270)
#define DAT_009d0274 CM_GLOBAL(volatile u32, 0x009d0274)
#define DAT_009d0278 CM_GLOBAL(volatile u32, 0x009d0278)
#define DAT_009d027c CM_GLOBAL(volatile u32, 0x009d027c)
#define DAT_009d0280 CM_GLOBAL(volatile u32, 0x009d0280)
#define DAT_009d0284 CM_GLOBAL(volatile u32, 0x009d0284)
#define DAT_009d0288 CM_GLOBAL(volatile u32, 0x009d0288)
#define DAT_009d028c CM_GLOBAL(volatile u32, 0x009d028c)
#define DAT_009d0290 CM_GLOBAL(volatile u32, 0x009d0290)
#define DAT_009d0294 CM_GLOBAL(volatile u32, 0x009d0294)
#define DAT_009d0298 CM_GLOBAL(volatile u32, 0x009d0298)
#define DAT_009d029c CM_GLOBAL(volatile u32, 0x009d029c)
#define DAT_009d02a0 CM_GLOBAL(volatile u32, 0x009d02a0)
#define DAT_009d02a4 CM_GLOBAL(volatile u32, 0x009d02a4)
#define DAT_009d02a8 CM_GLOBAL(volatile u32, 0x009d02a8)
#define DAT_009d02ac CM_GLOBAL(volatile u32, 0x009d02ac)
#define DAT_009d02b0 CM_GLOBAL(volatile u32, 0x009d02b0)
#define DAT_009d02b4 CM_GLOBAL(volatile u32, 0x009d02b4)
#define DAT_009d02b8 CM_GLOBAL(volatile u32, 0x009d02b8)
#define DAT_009d02bc CM_GLOBAL(volatile u32, 0x009d02bc)
#define DAT_009d02c0 CM_GLOBAL(volatile u32, 0x009d02c0)
#define DAT_009d02c4 CM_GLOBAL(volatile u32, 0x009d02c4)
#define DAT_009d02c8 CM_GLOBAL(volatile u32, 0x009d02c8)
#define DAT_009d02cc CM_GLOBAL(volatile u32, 0x009d02cc)
#define DAT_009d02d0 CM_GLOBAL(volatile u32, 0x009d02d0)
#define DAT_009d02d4 CM_GLOBAL(volatile u32, 0x009d02d4)
#define DAT_009d02d8 CM_GLOBAL(volatile u32, 0x009d02d8)
#define DAT_009d02dc CM_GLOBAL(volatile u32, 0x009d02dc)
#define DAT_009d02e0 CM_GLOBAL(volatile u32, 0x009d02e0)
#define DAT_009d02e4 CM_GLOBAL(volatile u32, 0x009d02e4)
#define DAT_009d02e8 CM_GLOBAL(volatile u32, 0x009d02e8)
#define DAT_009d02ec CM_GLOBAL(volatile u32, 0x009d02ec)
#define DAT_009d02f0 CM_GLOBAL(volatile u32, 0x009d02f0)
#define DAT_009d02f4 CM_GLOBAL(volatile u32, 0x009d02f4)
#define DAT_009d02f8 CM_GLOBAL(volatile u32, 0x009d02f8)
#define DAT_009d02fc CM_GLOBAL(volatile u32, 0x009d02fc)
#define DAT_009d0304 CM_GLOBAL(volatile u32, 0x009d0304)
#define DAT_009d0308 CM_GLOBAL(volatile u32, 0x009d0308)
#define DAT_009d030c CM_GLOBAL(volatile u32, 0x009d030c)
#define DAT_009d0310 CM_GLOBAL(volatile u32, 0x009d0310)
#define DAT_009d0314 CM_GLOBAL(volatile u32, 0x009d0314)
#define DAT_009d0318 CM_GLOBAL(volatile u32, 0x009d0318)
#define DAT_009d031c CM_GLOBAL(volatile u32, 0x009d031c)
#define DAT_009d0320 CM_GLOBAL(volatile u32, 0x009d0320)
#define DAT_009d0324 CM_GLOBAL(volatile u32, 0x009d0324)
#define DAT_009d0328 CM_GLOBAL(volatile u32, 0x009d0328)
#define DAT_009d032c CM_GLOBAL(volatile u32, 0x009d032c)
#define DAT_009d0330 CM_GLOBAL(volatile u32, 0x009d0330)
#define DAT_009d0334 CM_GLOBAL(volatile u32, 0x009d0334)
#define DAT_009d0338 CM_GLOBAL(volatile u32, 0x009d0338)
#define DAT_009d033c CM_GLOBAL(volatile u32, 0x009d033c)
#define DAT_009d0340 CM_GLOBAL(volatile u32, 0x009d0340)
#define DAT_009d0344 CM_GLOBAL(volatile u32, 0x009d0344)
#define DAT_009d0348 CM_GLOBAL(volatile u32, 0x009d0348)
#define DAT_009d0350 CM_GLOBAL(volatile u32, 0x009d0350)
#define DAT_009d0354 CM_GLOBAL(volatile u32, 0x009d0354)
#define DAT_009d0358 CM_GLOBAL(volatile u32, 0x009d0358)
#define DAT_009d035c CM_GLOBAL(volatile u32, 0x009d035c)
#define DAT_009d0360 CM_GLOBAL(volatile u32, 0x009d0360)
#define DAT_009d0364 CM_GLOBAL(volatile u32, 0x009d0364)
#define DAT_009d0368 CM_GLOBAL(volatile u32, 0x009d0368)
#define DAT_009d036c CM_GLOBAL(volatile u32, 0x009d036c)
#define DAT_009d0370 CM_GLOBAL(volatile u32, 0x009d0370)
#define DAT_009d0374 CM_GLOBAL(volatile u32, 0x009d0374)
#define DAT_009d0378 CM_GLOBAL(volatile u32, 0x009d0378)
#define DAT_009d037c CM_GLOBAL(volatile u32, 0x009d037c)
#define DAT_009d0380 CM_GLOBAL(volatile u32, 0x009d0380)
#define DAT_009d0384 CM_GLOBAL(volatile u32, 0x009d0384)
#define DAT_009d0388 CM_GLOBAL(volatile u32, 0x009d0388)
#define DAT_009d038c CM_GLOBAL(volatile u32, 0x009d038c)
#define DAT_009d0390 CM_GLOBAL(volatile u32, 0x009d0390)
#define DAT_009d0394 CM_GLOBAL(volatile u32, 0x009d0394)
#define DAT_009d0398 CM_GLOBAL(volatile u32, 0x009d0398)
#define DAT_009d039c CM_GLOBAL(volatile u32, 0x009d039c)
#define DAT_009d03a0 CM_GLOBAL(volatile u32, 0x009d03a0)
#define DAT_009d03a4 CM_GLOBAL(volatile u32, 0x009d03a4)
#define DAT_009d03a8 CM_GLOBAL(volatile u32, 0x009d03a8)
#define DAT_009d03ac CM_GLOBAL(volatile u32, 0x009d03ac)
#define DAT_009d03b0 CM_GLOBAL(volatile u32, 0x009d03b0)
#define DAT_009d03b4 CM_GLOBAL(volatile u32, 0x009d03b4)
#define DAT_009d03b8 CM_GLOBAL(volatile u32, 0x009d03b8)
#define DAT_009d03bc CM_GLOBAL(volatile u32, 0x009d03bc)
#define DAT_009d03c0 CM_GLOBAL(volatile u32, 0x009d03c0)
#define DAT_009d03c4 CM_GLOBAL(volatile u32, 0x009d03c4)
#define DAT_009d03c8 CM_GLOBAL(volatile u32, 0x009d03c8)
#define DAT_009d03cc CM_GLOBAL(volatile u32, 0x009d03cc)
#define DAT_009d03d0 CM_GLOBAL(volatile u32, 0x009d03d0)
#define DAT_009d03d4 CM_GLOBAL(volatile u32, 0x009d03d4)
#define DAT_009d03d8 CM_GLOBAL(volatile u32, 0x009d03d8)
#define DAT_009d03dc CM_GLOBAL(volatile u32, 0x009d03dc)
#define DAT_009d03e0 CM_GLOBAL(volatile u32, 0x009d03e0)
#define DAT_009d03e4 CM_GLOBAL(volatile u32, 0x009d03e4)
#define DAT_009d03e8 CM_GLOBAL(volatile u32, 0x009d03e8)
#define DAT_009d03ec CM_GLOBAL(volatile u32, 0x009d03ec)
#define DAT_009d03f0 CM_GLOBAL(volatile u32, 0x009d03f0)
#define DAT_009d03f4 CM_GLOBAL(volatile u32, 0x009d03f4)
#define DAT_009d03f8 CM_GLOBAL(volatile u32, 0x009d03f8)
#define DAT_009d03fc CM_GLOBAL(volatile u32, 0x009d03fc)
#define DAT_009d0400 CM_GLOBAL(volatile u32, 0x009d0400)
#define DAT_009d0404 CM_GLOBAL(volatile u32, 0x009d0404)
#define DAT_009d0408 CM_GLOBAL(volatile u32, 0x009d0408)
#define DAT_009d040c CM_GLOBAL(volatile u32, 0x009d040c)
#define DAT_009d0410 CM_GLOBAL(volatile u32, 0x009d0410)
#define DAT_009d0414 CM_GLOBAL(volatile u32, 0x009d0414)
#define DAT_009d0418 CM_GLOBAL(volatile u32, 0x009d0418)
#define DAT_009d041c CM_GLOBAL(volatile u32, 0x009d041c)
#define DAT_009d0420 CM_GLOBAL(volatile u32, 0x009d0420)
#define DAT_009d0424 CM_GLOBAL(volatile u32, 0x009d0424)
#define DAT_009d0428 CM_GLOBAL(volatile u32, 0x009d0428)
#define DAT_009d042c CM_GLOBAL(volatile u32, 0x009d042c)
#define DAT_009d0430 CM_GLOBAL(volatile u32, 0x009d0430)
#define DAT_009d0434 CM_GLOBAL(volatile u32, 0x009d0434)
#define DAT_009d0438 CM_GLOBAL(volatile u32, 0x009d0438)
#define DAT_009d043c CM_GLOBAL(volatile u32, 0x009d043c)
#define DAT_009d0440 CM_GLOBAL(volatile u32, 0x009d0440)
#define DAT_009d0444 CM_GLOBAL(volatile u32, 0x009d0444)
#define DAT_009d0448 CM_GLOBAL(volatile u32, 0x009d0448)
#define DAT_009d044c CM_GLOBAL(volatile u32, 0x009d044c)
#define DAT_009d0450 CM_GLOBAL(volatile u32, 0x009d0450)
#define DAT_009d0454 CM_GLOBAL(volatile u32, 0x009d0454)
#define DAT_009d0458 CM_GLOBAL(volatile u32, 0x009d0458)
#define DAT_009d045c CM_GLOBAL(volatile u32, 0x009d045c)
#define DAT_009d0460 CM_GLOBAL(volatile u32, 0x009d0460)
#define DAT_009d0464 CM_GLOBAL(volatile u32, 0x009d0464)
#define DAT_009d0468 CM_GLOBAL(volatile u32, 0x009d0468)
#define DAT_009d046c CM_GLOBAL(volatile u32, 0x009d046c)
#define DAT_009d0470 CM_GLOBAL(volatile u32, 0x009d0470)
#define DAT_009d0474 CM_GLOBAL(volatile u32, 0x009d0474)
#define DAT_009d0478 CM_GLOBAL(volatile u32, 0x009d0478)
#define DAT_009d047c CM_GLOBAL(volatile u32, 0x009d047c)
#define DAT_009d0480 CM_GLOBAL(volatile u32, 0x009d0480)
#define DAT_009d0484 CM_GLOBAL(volatile u32, 0x009d0484)
#define DAT_009d0488 CM_GLOBAL(volatile u32, 0x009d0488)
#define DAT_009d048c CM_GLOBAL(volatile u32, 0x009d048c)
#define DAT_009d0490 CM_GLOBAL(volatile u32, 0x009d0490)
#define DAT_009d0494 CM_GLOBAL(volatile u32, 0x009d0494)
#define DAT_009d0498 CM_GLOBAL(volatile u32, 0x009d0498)
#define DAT_009d049c CM_GLOBAL(volatile u32, 0x009d049c)
#define DAT_009d04a0 CM_GLOBAL(volatile u32, 0x009d04a0)
#define DAT_009d04a4 CM_GLOBAL(volatile u32, 0x009d04a4)
#define DAT_009d04a8 CM_GLOBAL(volatile u32, 0x009d04a8)
#define DAT_009d04ac CM_GLOBAL(volatile u32, 0x009d04ac)
#define DAT_009d04b0 CM_GLOBAL(volatile u32, 0x009d04b0)
#define DAT_009d04b4 CM_GLOBAL(volatile u32, 0x009d04b4)
#define DAT_009d04b8 CM_GLOBAL(volatile u32, 0x009d04b8)
#define DAT_009d04bc CM_GLOBAL(volatile u32, 0x009d04bc)
#define DAT_009d04c0 CM_GLOBAL(volatile u32, 0x009d04c0)
#define DAT_009d04c4 CM_GLOBAL(volatile u32, 0x009d04c4)
#define DAT_009d04c8 CM_GLOBAL(volatile u32, 0x009d04c8)
#define DAT_009d04cc CM_GLOBAL(volatile u32, 0x009d04cc)
#define DAT_009d04d0 CM_GLOBAL(volatile u32, 0x009d04d0)
#define DAT_009d04d4 CM_GLOBAL(volatile u32, 0x009d04d4)
#define DAT_009d04d8 CM_GLOBAL(volatile u32, 0x009d04d8)
#define DAT_009d04dc CM_GLOBAL(volatile u32, 0x009d04dc)
#define DAT_009d04e0 CM_GLOBAL(volatile u32, 0x009d04e0)
#define DAT_009d04e4 CM_GLOBAL(volatile u32, 0x009d04e4)
#define DAT_009d04e8 CM_GLOBAL(volatile u32, 0x009d04e8)
#define DAT_009d04ec CM_GLOBAL(volatile u32, 0x009d04ec)
#define DAT_009d04f0 CM_GLOBAL(volatile u32, 0x009d04f0)
#define DAT_009d04f4 CM_GLOBAL(volatile u32, 0x009d04f4)
#define DAT_009d04f8 CM_GLOBAL(volatile u32, 0x009d04f8)
#define DAT_009d04fc CM_GLOBAL(volatile u32, 0x009d04fc)
#define DAT_009d0500 CM_GLOBAL(volatile u32, 0x009d0500)
#define DAT_009d0504 CM_GLOBAL(volatile u32, 0x009d0504)
#define DAT_009d0508 CM_GLOBAL(volatile u32, 0x009d0508)
#define DAT_009d050c CM_GLOBAL(volatile u32, 0x009d050c)
#define DAT_009d0510 CM_GLOBAL(volatile u32, 0x009d0510)
#define DAT_009d0514 CM_GLOBAL(volatile u32, 0x009d0514)
#define DAT_009d0518 CM_GLOBAL(volatile u32, 0x009d0518)
#define DAT_009d051c CM_GLOBAL(volatile u32, 0x009d051c)
#define DAT_009d0520 CM_GLOBAL(volatile u32, 0x009d0520)
#define DAT_009d0524 CM_GLOBAL(volatile u32, 0x009d0524)
#define DAT_00b63c9c CM_GLOBAL(volatile u32, 0x00b63c9c)
#define DAT_00b63ca0 CM_GLOBAL(volatile u32, 0x00b63ca0)
#define DAT_00b63ca4 CM_GLOBAL(volatile u32, 0x00b63ca4)
#define DAT_00b63ca8 CM_GLOBAL(volatile u32, 0x00b63ca8)
#define DAT_00b63cac CM_GLOBAL(volatile u32, 0x00b63cac)
#define DAT_00b63cb0 CM_GLOBAL(volatile u32, 0x00b63cb0)
#define DAT_00b63cb4 CM_GLOBAL(volatile u32, 0x00b63cb4)
#define DAT_00b63cb8 CM_GLOBAL(volatile u32, 0x00b63cb8)
#define DAT_00b63cbc CM_GLOBAL(volatile u32, 0x00b63cbc)
#define DAT_00b63cc0 CM_GLOBAL(volatile u32, 0x00b63cc0)
#define DAT_00b63cc4 CM_GLOBAL(volatile u32, 0x00b63cc4)
#define DAT_00b63cc8 CM_GLOBAL(volatile u32, 0x00b63cc8)
#define DAT_00b63ccc CM_GLOBAL(volatile u32, 0x00b63ccc)
#define DAT_00b63cd0 CM_GLOBAL(volatile u32, 0x00b63cd0)
#define DAT_00b63cd4 CM_GLOBAL(volatile u32, 0x00b63cd4)
#define DAT_00b63cd8 CM_GLOBAL(volatile u32, 0x00b63cd8)
#define DAT_00b63cdc CM_GLOBAL(volatile u32, 0x00b63cdc)
#define DAT_00b63ce0 CM_GLOBAL(volatile u32, 0x00b63ce0)
#define DAT_00b63ce4 CM_GLOBAL(volatile u32, 0x00b63ce4)
#define DAT_00b63ce8 CM_GLOBAL(volatile u32, 0x00b63ce8)
#define DAT_00b63cec CM_GLOBAL(volatile u32, 0x00b63cec)
#define DAT_00b63cf0 CM_GLOBAL(volatile u32, 0x00b63cf0)
#define DAT_00b63cf4 CM_GLOBAL(volatile u32, 0x00b63cf4)
#define DAT_00b63cf8 CM_GLOBAL(volatile u32, 0x00b63cf8)
#define DAT_00b63cfc CM_GLOBAL(volatile u32, 0x00b63cfc)
#define DAT_00b63d00 CM_GLOBAL(volatile u32, 0x00b63d00)

u32 cm_fn_0060ba80(u32 param_1,u32 param_2)

{
  if (param_1 == DAT_009d00a4) {
    DAT_009d00a4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d00a8) {
    DAT_009d00a8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d00ac) {
    DAT_009d00ac = param_2;
    return 1;
  }
  if (param_1 == DAT_009d00b0) {
    DAT_009d00b0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d00b4) {
    DAT_009d00b4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d00b8) {
    DAT_009d00b8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d00bc) {
    DAT_009d00bc = param_2;
    return 1;
  }
  if (param_1 == DAT_009d00c0) {
    DAT_009d00c0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d00c4) {
    DAT_009d00c4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d00c8) {
    DAT_009d00c8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d00cc) {
    DAT_009d00cc = param_2;
    return 1;
  }
  if (param_1 == DAT_009d00d0) {
    DAT_009d00d0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d00d4) {
    DAT_009d00d4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d00d8) {
    DAT_009d00d8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d00dc) {
    DAT_009d00dc = param_2;
    return 1;
  }
  if (param_1 == DAT_009d00e0) {
    DAT_009d00e0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d00e4) {
    DAT_009d00e4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d00e8) {
    DAT_009d00e8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d00ec) {
    DAT_009d00ec = param_2;
    return 1;
  }
  if (param_1 == DAT_009d00f0) {
    DAT_009d00f0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d00f4) {
    DAT_009d00f4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d00f8) {
    DAT_009d00f8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d00fc) {
    DAT_009d00fc = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0100) {
    DAT_009d0100 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0104) {
    DAT_009d0104 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0108) {
    DAT_009d0108 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d010c) {
    DAT_009d010c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0110) {
    DAT_009d0110 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0114) {
    DAT_009d0114 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0118) {
    DAT_009d0118 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d011c) {
    DAT_009d011c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0120) {
    DAT_009d0120 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0124) {
    DAT_009d0124 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0128) {
    DAT_009d0128 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d012c) {
    DAT_009d012c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0130) {
    DAT_009d0130 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0134) {
    DAT_009d0134 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0138) {
    DAT_009d0138 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d013c) {
    DAT_009d013c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0140) {
    DAT_009d0140 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0144) {
    DAT_009d0144 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0148) {
    DAT_009d0148 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d014c) {
    DAT_009d014c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0150) {
    DAT_009d0150 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0154) {
    DAT_009d0154 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0158) {
    DAT_009d0158 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d015c) {
    DAT_009d015c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0160) {
    DAT_009d0160 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0164) {
    DAT_009d0164 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0168) {
    DAT_009d0168 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d016c) {
    DAT_009d016c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0170) {
    DAT_009d0170 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0174) {
    DAT_009d0174 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0178) {
    DAT_009d0178 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d017c) {
    DAT_009d017c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0180) {
    DAT_009d0180 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0184) {
    DAT_009d0184 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0188) {
    DAT_009d0188 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d018c) {
    DAT_009d018c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0190) {
    DAT_009d0190 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0194) {
    DAT_009d0194 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0198) {
    DAT_009d0198 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d019c) {
    DAT_009d019c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d01a0) {
    DAT_009d01a0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d01a4) {
    DAT_009d01a4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d01a8) {
    DAT_009d01a8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d01ac) {
    DAT_009d01ac = param_2;
    return 1;
  }
  if (param_1 == DAT_009d01b0) {
    DAT_009d01b0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d01b4) {
    DAT_009d01b4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d01b8) {
    DAT_009d01b8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d01bc) {
    DAT_009d01bc = param_2;
    return 1;
  }
  if (param_1 == DAT_009d01c0) {
    DAT_009d01c0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d01c4) {
    DAT_009d01c4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d01c8) {
    DAT_009d01c8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d01cc) {
    DAT_009d01cc = param_2;
    return 1;
  }
  if (param_1 == DAT_009d01d0) {
    DAT_009d01d0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d01d4) {
    DAT_009d01d4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d01d8) {
    DAT_009d01d8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d01dc) {
    DAT_009d01dc = param_2;
    return 1;
  }
  if (param_1 == DAT_009d01e0) {
    DAT_009d01e0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d01e4) {
    DAT_009d01e4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d01e8) {
    DAT_009d01e8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d01ec) {
    DAT_009d01ec = param_2;
    return 1;
  }
  if (param_1 == DAT_009d01f0) {
    DAT_009d01f0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d01f4) {
    DAT_009d01f4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d01f8) {
    DAT_009d01f8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d01fc) {
    DAT_009d01fc = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0200) {
    DAT_009d0200 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0204) {
    DAT_009d0204 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0208) {
    DAT_009d0208 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d020c) {
    DAT_009d020c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0210) {
    DAT_009d0210 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0214) {
    DAT_009d0214 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0218) {
    DAT_009d0218 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d021c) {
    DAT_009d021c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0220) {
    DAT_009d0220 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0224) {
    DAT_009d0224 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0228) {
    DAT_009d0228 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d022c) {
    DAT_009d022c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0230) {
    DAT_009d0230 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0234) {
    DAT_009d0234 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0238) {
    DAT_009d0238 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d023c) {
    DAT_009d023c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0240) {
    DAT_009d0240 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0244) {
    DAT_009d0244 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0248) {
    DAT_009d0248 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d024c) {
    DAT_009d024c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0250) {
    DAT_009d0250 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0254) {
    DAT_009d0254 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0258) {
    DAT_009d0258 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d025c) {
    DAT_009d025c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0260) {
    DAT_009d0260 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0264) {
    DAT_009d0264 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0268) {
    DAT_009d0268 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d026c) {
    DAT_009d026c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0270) {
    DAT_009d0270 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0274) {
    DAT_009d0274 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0278) {
    DAT_009d0278 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d027c) {
    DAT_009d027c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0354) {
    DAT_009d0354 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0280) {
    DAT_009d0280 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0284) {
    DAT_009d0284 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0288) {
    DAT_009d0288 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d028c) {
    DAT_009d028c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0290) {
    DAT_009d0290 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0294) {
    DAT_009d0294 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0298) {
    DAT_009d0298 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d029c) {
    DAT_009d029c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d02a0) {
    DAT_009d02a0 = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63cdc) {
    DAT_00b63cdc = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63c9c) {
    DAT_00b63c9c = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63cbc) {
    DAT_00b63cbc = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63ca0) {
    DAT_00b63ca0 = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63ce8) {
    DAT_00b63ce8 = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63cac) {
    DAT_00b63cac = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63cd8) {
    DAT_00b63cd8 = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63cd0) {
    DAT_00b63cd0 = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63cb4) {
    DAT_00b63cb4 = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63cb0) {
    DAT_00b63cb0 = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63cd4) {
    DAT_00b63cd4 = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63ca8) {
    DAT_00b63ca8 = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63ce4) {
    DAT_00b63ce4 = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63cec) {
    DAT_00b63cec = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63cf0) {
    DAT_00b63cf0 = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63cf8) {
    DAT_00b63cf8 = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63cc0) {
    DAT_00b63cc0 = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63d00) {
    DAT_00b63d00 = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63cc8) {
    DAT_00b63cc8 = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63ccc) {
    DAT_00b63ccc = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63ce0) {
    DAT_00b63ce0 = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63cb8) {
    DAT_00b63cb8 = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63cc4) {
    DAT_00b63cc4 = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63cf4) {
    DAT_00b63cf4 = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63cfc) {
    DAT_00b63cfc = param_2;
    return 1;
  }
  if (param_1 == DAT_00b63ca4) {
    DAT_00b63ca4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d02a4) {
    DAT_009d02a4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d02a8) {
    DAT_009d02a8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d02ac) {
    DAT_009d02ac = param_2;
    return 1;
  }
  if (param_1 == DAT_009d02b0) {
    DAT_009d02b0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d02b4) {
    DAT_009d02b4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d02b8) {
    DAT_009d02b8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d02bc) {
    DAT_009d02bc = param_2;
    return 1;
  }
  if (param_1 == DAT_009d02c0) {
    DAT_009d02c0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d02c4) {
    DAT_009d02c4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d02c8) {
    DAT_009d02c8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d02cc) {
    DAT_009d02cc = param_2;
    return 1;
  }
  if (param_1 == DAT_009d02d0) {
    DAT_009d02d0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d02d4) {
    DAT_009d02d4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d02d8) {
    DAT_009d02d8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d02dc) {
    DAT_009d02dc = param_2;
    return 1;
  }
  if (param_1 == DAT_009d02e0) {
    DAT_009d02e0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d02e4) {
    DAT_009d02e4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d02e8) {
    DAT_009d02e8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d02ec) {
    DAT_009d02ec = param_2;
    return 1;
  }
  if (param_1 == DAT_009d02f0) {
    DAT_009d02f0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d02f4) {
    DAT_009d02f4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d02f8) {
    DAT_009d02f8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d02fc) {
    DAT_009d02fc = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0304) {
    DAT_009d0304 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0308) {
    DAT_009d0308 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d030c) {
    DAT_009d030c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0310) {
    DAT_009d0310 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0314) {
    DAT_009d0314 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0318) {
    DAT_009d0318 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d031c) {
    DAT_009d031c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0320) {
    DAT_009d0320 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0324) {
    DAT_009d0324 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0328) {
    DAT_009d0328 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d032c) {
    DAT_009d032c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0330) {
    DAT_009d0330 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0334) {
    DAT_009d0334 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0338) {
    DAT_009d0338 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d033c) {
    DAT_009d033c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0340) {
    DAT_009d0340 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0344) {
    DAT_009d0344 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0348) {
    DAT_009d0348 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0350) {
    DAT_009d0350 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0358) {
    DAT_009d0358 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d035c) {
    DAT_009d035c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0360) {
    DAT_009d0360 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0364) {
    DAT_009d0364 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0368) {
    DAT_009d0368 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d036c) {
    DAT_009d036c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0370) {
    DAT_009d0370 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0374) {
    DAT_009d0374 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0378) {
    DAT_009d0378 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d037c) {
    DAT_009d037c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0380) {
    DAT_009d0380 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0384) {
    DAT_009d0384 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0388) {
    DAT_009d0388 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d038c) {
    DAT_009d038c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0390) {
    DAT_009d0390 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0394) {
    DAT_009d0394 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0398) {
    DAT_009d0398 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d039c) {
    DAT_009d039c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d03a0) {
    DAT_009d03a0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d03a4) {
    DAT_009d03a4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d03a8) {
    DAT_009d03a8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d03ac) {
    DAT_009d03ac = param_2;
    return 1;
  }
  if (param_1 == DAT_009d03b0) {
    DAT_009d03b0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d03b4) {
    DAT_009d03b4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d03b8) {
    DAT_009d03b8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d03bc) {
    DAT_009d03bc = param_2;
    return 1;
  }
  if (param_1 == DAT_009d03c0) {
    DAT_009d03c0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d03c4) {
    DAT_009d03c4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d03c8) {
    DAT_009d03c8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d03cc) {
    DAT_009d03cc = param_2;
    return 1;
  }
  if (param_1 == DAT_009d03d0) {
    DAT_009d03d0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d03d4) {
    DAT_009d03d4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d03d8) {
    DAT_009d03d8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d03dc) {
    DAT_009d03dc = param_2;
    return 1;
  }
  if (param_1 == DAT_009d03e0) {
    DAT_009d03e0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d03e4) {
    DAT_009d03e4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d03e8) {
    DAT_009d03e8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d03ec) {
    DAT_009d03ec = param_2;
    return 1;
  }
  if (param_1 == DAT_009d03f0) {
    DAT_009d03f0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d03f4) {
    DAT_009d03f4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d03f8) {
    DAT_009d03f8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d03fc) {
    DAT_009d03fc = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0400) {
    DAT_009d0400 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0404) {
    DAT_009d0404 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0408) {
    DAT_009d0408 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d040c) {
    DAT_009d040c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0410) {
    DAT_009d0410 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0414) {
    DAT_009d0414 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0418) {
    DAT_009d0418 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d041c) {
    DAT_009d041c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0420) {
    DAT_009d0420 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0424) {
    DAT_009d0424 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0428) {
    DAT_009d0428 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d042c) {
    DAT_009d042c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0430) {
    DAT_009d0430 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0434) {
    DAT_009d0434 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0438) {
    DAT_009d0438 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d043c) {
    DAT_009d043c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0440) {
    DAT_009d0440 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0444) {
    DAT_009d0444 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0448) {
    DAT_009d0448 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d044c) {
    DAT_009d044c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0450) {
    DAT_009d0450 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0454) {
    DAT_009d0454 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0458) {
    DAT_009d0458 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d045c) {
    DAT_009d045c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0460) {
    DAT_009d0460 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0464) {
    DAT_009d0464 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0468) {
    DAT_009d0468 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d046c) {
    DAT_009d046c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0470) {
    DAT_009d0470 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0474) {
    DAT_009d0474 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0478) {
    DAT_009d0478 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d047c) {
    DAT_009d047c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0480) {
    DAT_009d0480 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0484) {
    DAT_009d0484 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0488) {
    DAT_009d0488 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d048c) {
    DAT_009d048c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0490) {
    DAT_009d0490 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0494) {
    DAT_009d0494 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0498) {
    DAT_009d0498 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d049c) {
    DAT_009d049c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d04a0) {
    DAT_009d04a0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d04a4) {
    DAT_009d04a4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d04a8) {
    DAT_009d04a8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d04ac) {
    DAT_009d04ac = param_2;
    return 1;
  }
  if (param_1 == DAT_009d04b0) {
    DAT_009d04b0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d04b4) {
    DAT_009d04b4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d04b8) {
    DAT_009d04b8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d04bc) {
    DAT_009d04bc = param_2;
    return 1;
  }
  if (param_1 == DAT_009d04c0) {
    DAT_009d04c0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d04c4) {
    DAT_009d04c4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d04c8) {
    DAT_009d04c8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d04cc) {
    DAT_009d04cc = param_2;
    return 1;
  }
  if (param_1 == DAT_009d04d0) {
    DAT_009d04d0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d04d4) {
    DAT_009d04d4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d04d8) {
    DAT_009d04d8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d04dc) {
    DAT_009d04dc = param_2;
    return 1;
  }
  if (param_1 == DAT_009d04e0) {
    DAT_009d04e0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d04e4) {
    DAT_009d04e4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d04e8) {
    DAT_009d04e8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d04ec) {
    DAT_009d04ec = param_2;
    return 1;
  }
  if (param_1 == DAT_009d04f0) {
    DAT_009d04f0 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d04f4) {
    DAT_009d04f4 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d04f8) {
    DAT_009d04f8 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d04fc) {
    DAT_009d04fc = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0500) {
    DAT_009d0500 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0504) {
    DAT_009d0504 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0508) {
    DAT_009d0508 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d050c) {
    DAT_009d050c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0510) {
    DAT_009d0510 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0514) {
    DAT_009d0514 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0518) {
    DAT_009d0518 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d051c) {
    DAT_009d051c = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0520) {
    DAT_009d0520 = param_2;
    return 1;
  }
  if (param_1 == DAT_009d0524) {
    DAT_009d0524 = param_2;
    return 1;
  }
  return 0;
}


