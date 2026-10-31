/* Primary 3.9.68. Original incoming stack storage, not a month default.
 * Named slots: numeric byte ESP-209; weekday ordinal dword ESP-208;
 * month-only dword ESP-204. Original month scans change only the low byte.
 * Other private frame contents are outside the normal-return contract.
 */
.text
.macro DATE_ENTRY name, variant, frame, seed
.globl _cm_date_\name
_cm_date_\name:
    subl $\frame, %esp
    pushl %esi
    movl %ecx, %esi
    .if \variant == 0
        movzbl (\frame + 4 - \seed)(%esp), %eax
    .else
        movl (\frame + 4 - \seed)(%esp), %eax
    .endif
    pushl %eax
    pushl (\frame + 20)(%esp)
    pushl (\frame + 20)(%esp)
    pushl (\frame + 20)(%esp)
    pushl %esi
    pushl $\variant
    call _cm_date_text_worker
    addl $24, %esp
    popl %esi
    addl $\frame, %esp
    ret $12
.endm

DATE_ENTRY weekday_numeric,      0, 0x20c, 0x209
DATE_ENTRY short_weekday_year,   1, 0x208, 0x208
DATE_ENTRY short_month_year,     2, 0x204, 0x204
DATE_ENTRY short_weekday,        3, 0x208, 0x208
DATE_ENTRY full_weekday_year,    4, 0x208, 0x208
DATE_ENTRY full_weekday,         5, 0x208, 0x208
DATE_ENTRY full_month_year,      6, 0x204, 0x204
