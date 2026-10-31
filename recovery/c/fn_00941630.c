#include "cmre.h"

/* WIP: the non-null error-output branch calls __setjmp3 at 0x00947998.
 * That original boundary saves incoming EBP/EBX/EDI/ESI, live ESP, return
 * address, and FS state into the caller's buffer. Plain C cannot provide those
 * exact machine-context inputs; do not replace them with guessed values. */
