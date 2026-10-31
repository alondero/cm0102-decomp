/* WIP: the two conversion paths call 0x00945610 with a size derived from the
   runtime MultiByteToWideChar result, then pass the resulting ESP-relative
   buffers to MultiByteToWideChar/CompareStringW (0x009518fa-0x009519a5).
   A fixed local buffer changes the addresses and allocation failure/stack
   probe behavior; plain C cannot express this exact dynamic stack boundary. */
#include "cmre.h"
