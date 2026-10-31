/* WIP: The original calls 0x00945640 with the x87 ST(0) value at
   0x007c362b, 0x007c366a, 0x007c37a4, and 0x007c37e3.  The immediately
   preceding FILD/FMUL/FIDIV sequences establish the implicit input.  Plain C
   has no proven way to transport this call boundary; no guessed cast used. */
