/* WIP: The entry links a four-DWORD registration record with scope-table
   metadata (prev, handler 0x0094bf5c, table 0x00971448, state), saves ESP at
   EBP-0x18, and enters the non-returning 0x00945934 path. The plain-C SEH
   boundary in cmre.h models only {prev, handler, state}; using it here would
   change the record layout and unwind contract. No test attempted. */
