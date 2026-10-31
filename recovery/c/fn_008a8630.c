/* WIP: At entry the original executes MOV EAX,0x3a74; CALL 0x00945610
   (0x008a8635) before saving registers or allocating its locals.  This is a
   stack-probe/alloca machine-state boundary; an ordinary C call cannot be
   assumed to transport it faithfully.  No guessed signature is used. */
