/* WIP: The original calls 0x00945640 with an implicit x87 ST(0) input at
   0x0068f440, 0x0068f573, 0x0068f5fb, 0x0068f671, and 0x0068f6e7.  Their
   preceding FLD/FILD/FSUBR sequences are part of the boundary.  No plain-C
   transport is established, so no guessed conversion or call is used. */
