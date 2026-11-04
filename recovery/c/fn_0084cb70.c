/* WIP: preferred-image VA 0x0084cb70, base 0x00400000, CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   VA 0x0084cb9e inherits ECX from the indirect call at 0x0084cb94.
   No original-instruction proof of that value exists in this batch.
   An inexact diagnostic with a guessed null receiver passed the finite gate;
   it is retained locally under diagnostics/day1/l6 and was never accepted.
   VA 0x0084cba6 leaves its callee's EAX as the result. Returning the saved
   pre-call item pointer is also unsupported. Recover both actual boundaries. */
#error Recover physical ECX and returned EAX across the original indirect calls
#include "cmre.h"
