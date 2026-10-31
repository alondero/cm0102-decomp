/* WIP: The original executes REP MOVSD and REP MOVSB without CLD. Their
   direction depends on incoming EFLAGS.DF, which the current plain-C contract
   does not expose. A forward C copy would not preserve that behavior. The
   function has not been tested. */
