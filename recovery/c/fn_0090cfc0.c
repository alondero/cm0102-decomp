/* WIP: The high-range branch leaves a computed x87 ST(0) value live for the
   call to 0x00945640 (__ftol). The plain-C boundary cannot transport that
   incoming FPU value exactly; no numeric approximation or omitted branch is
   valid. No test attempted. */
