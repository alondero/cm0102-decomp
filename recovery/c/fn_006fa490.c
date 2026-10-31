/* WIP: The original calls 0x00945640 with implicit x87 ST(0) at 0x006fac0c,
   after two float table loads, FMUL ST(1), and FADD ST(0),ST(0).  Plain C has
   no proven transport for this call boundary; no guessed cast is used. */
