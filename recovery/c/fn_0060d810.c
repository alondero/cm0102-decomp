/* Preferred-image VA 0x0060d810, base 0x00400000; primary executable SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Cdecl, two stack DWORDs. Callers at 00604081/00604226 overwrite EAX
   immediately afterwards (00604086/0060422b), supporting void.
   The first comparison chain continues at 0060e64c after a matching store;
   the second returns. Shared alias-store targets retain the original CFG. */
#include "cmre.h"

extern s32 CM_CDECL cm_fn_00951c50(const volatile u8 *first,
                                  const volatile u8 *second);

void CM_CDECL cm_fn_0060d810(u32 name, u32 value)
{
    s32 result;

    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9e30u); /* VA 0060d81c */
    if (result != 0) goto L_0060d837;
    CM_GLOBAL(volatile u32, 0x009cf200u) = value; /* VA 0060d82c */
    goto L_0060e64c;
L_0060d837:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9e28u); /* VA 0060d83d */
    if (result != 0) goto L_0060d854;
    CM_GLOBAL(volatile u32, 0x009cf204u) = value; /* VA 0060d849 */
    goto L_0060e64c;
L_0060d854:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9e20u); /* VA 0060d85a */
    if (result != 0) goto L_0060d871;
    CM_GLOBAL(volatile u32, 0x009cf208u) = value; /* VA 0060d866 */
    goto L_0060e64c;
L_0060d871:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9e18u); /* VA 0060d877 */
    if (result != 0) goto L_0060d88e;
    CM_GLOBAL(volatile u32, 0x009cf20cu) = value; /* VA 0060d883 */
    goto L_0060e64c;
L_0060d88e:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9e10u); /* VA 0060d894 */
    if (result != 0) goto L_0060d8ab;
    CM_GLOBAL(volatile u32, 0x009cf210u) = value; /* VA 0060d8a0 */
    goto L_0060e64c;
L_0060d8ab:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9dfcu); /* VA 0060d8b1 */
    if (result != 0) goto L_0060d8c8;
    CM_GLOBAL(volatile u32, 0x009cf214u) = value; /* VA 0060d8bd */
    goto L_0060e64c;
L_0060d8c8:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b1698u); /* VA 0060d8ce */
    if (result != 0) goto L_0060d8e5;
    CM_GLOBAL(volatile u32, 0x009cf218u) = value; /* VA 0060d8da */
    goto L_0060e64c;
L_0060d8e5:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9df4u); /* VA 0060d8eb */
    if (result != 0) goto L_0060d902;
    CM_GLOBAL(volatile u32, 0x009cf21cu) = value; /* VA 0060d8f7 */
    goto L_0060e64c;
L_0060d902:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9decu); /* VA 0060d908 */
    if (result != 0) goto L_0060d91f;
    CM_GLOBAL(volatile u32, 0x009cf220u) = value; /* VA 0060d914 */
    goto L_0060e64c;
L_0060d91f:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b167cu); /* VA 0060d925 */
    if (result != 0) goto L_0060d93c;
    CM_GLOBAL(volatile u32, 0x009cf224u) = value; /* VA 0060d931 */
    goto L_0060e64c;
L_0060d93c:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b1664u); /* VA 0060d942 */
    if (result != 0) goto L_0060d959;
    CM_GLOBAL(volatile u32, 0x009cf228u) = value; /* VA 0060d94e */
    goto L_0060e64c;
L_0060d959:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9de0u); /* VA 0060d95f */
    if (result != 0) goto L_0060d976;
    CM_GLOBAL(volatile u32, 0x009cf22cu) = value; /* VA 0060d96b */
    goto L_0060e64c;
L_0060d976:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9dd4u); /* VA 0060d97c */
    if (result == 0) goto L_0060e646;
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9dccu); /* VA 0060d992 */
    if (result == 0) goto L_0060e646;
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9dc4u); /* VA 0060d9a8 */
    if (result != 0) goto L_0060d9bf;
    CM_GLOBAL(volatile u32, 0x009cf234u) = value; /* VA 0060d9b4 */
    goto L_0060e64c;
L_0060d9bf:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9db8u); /* VA 0060d9c5 */
    if (result != 0) goto L_0060d9dc;
    CM_GLOBAL(volatile u32, 0x009cf238u) = value; /* VA 0060d9d1 */
    goto L_0060e64c;
L_0060d9dc:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9dacu); /* VA 0060d9e2 */
    if (result != 0) goto L_0060d9f9;
    CM_GLOBAL(volatile u32, 0x009cf23cu) = value; /* VA 0060d9ee */
    goto L_0060e64c;
L_0060d9f9:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9da4u); /* VA 0060d9ff */
    if (result != 0) goto L_0060da16;
    CM_GLOBAL(volatile u32, 0x009cf240u) = value; /* VA 0060da0b */
    goto L_0060e64c;
L_0060da16:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b164cu); /* VA 0060da1c */
    if (result != 0) goto L_0060da33;
    CM_GLOBAL(volatile u32, 0x009cf244u) = value; /* VA 0060da28 */
    goto L_0060e64c;
L_0060da33:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9d9cu); /* VA 0060da39 */
    if (result != 0) goto L_0060da50;
    CM_GLOBAL(volatile u32, 0x009cf248u) = value; /* VA 0060da45 */
    goto L_0060e64c;
L_0060da50:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9d94u); /* VA 0060da56 */
    if (result != 0) goto L_0060da6d;
    CM_GLOBAL(volatile u32, 0x009cf24cu) = value; /* VA 0060da62 */
    goto L_0060e64c;
L_0060da6d:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9d8cu); /* VA 0060da73 */
    if (result != 0) goto L_0060da8a;
    CM_GLOBAL(volatile u32, 0x009cf250u) = value; /* VA 0060da7f */
    goto L_0060e64c;
L_0060da8a:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9d84u); /* VA 0060da90 */
    if (result != 0) goto L_0060daa7;
    CM_GLOBAL(volatile u32, 0x009cf254u) = value; /* VA 0060da9c */
    goto L_0060e64c;
L_0060daa7:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9d70u); /* VA 0060daad */
    if (result != 0) goto L_0060dac4;
    CM_GLOBAL(volatile u32, 0x009cf258u) = value; /* VA 0060dab9 */
    goto L_0060e64c;
L_0060dac4:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9d64u); /* VA 0060daca */
    if (result != 0) goto L_0060dae1;
    CM_GLOBAL(volatile u32, 0x009cf25cu) = value; /* VA 0060dad6 */
    goto L_0060e64c;
L_0060dae1:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b1620u); /* VA 0060dae7 */
    if (result != 0) goto L_0060dafe;
    CM_GLOBAL(volatile u32, 0x009cf260u) = value; /* VA 0060daf3 */
    goto L_0060e64c;
L_0060dafe:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9d50u); /* VA 0060db04 */
    if (result != 0) goto L_0060db1b;
    CM_GLOBAL(volatile u32, 0x009cf264u) = value; /* VA 0060db10 */
    goto L_0060e64c;
L_0060db1b:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9d44u); /* VA 0060db21 */
    if (result != 0) goto L_0060db38;
    CM_GLOBAL(volatile u32, 0x009cf268u) = value; /* VA 0060db2d */
    goto L_0060e64c;
L_0060db38:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9d34u); /* VA 0060db3e */
    if (result == 0) goto L_0060e63e;
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9d24u); /* VA 0060db54 */
    if (result == 0) goto L_0060e63e;
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9d1cu); /* VA 0060db6a */
    if (result != 0) goto L_0060db81;
    CM_GLOBAL(volatile u32, 0x009cf274u) = value; /* VA 0060db76 */
    goto L_0060e64c;
L_0060db81:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9cfcu); /* VA 0060db87 */
    if (result != 0) goto L_0060db9e;
    CM_GLOBAL(volatile u32, 0x009cf278u) = value; /* VA 0060db93 */
    goto L_0060e64c;
L_0060db9e:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9cf0u); /* VA 0060dba4 */
    if (result != 0) goto L_0060dbbb;
    CM_GLOBAL(volatile u32, 0x009cf27cu) = value; /* VA 0060dbb0 */
    goto L_0060e64c;
L_0060dbbb:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9ce8u); /* VA 0060dbc1 */
    if (result != 0) goto L_0060dbd8;
    CM_GLOBAL(volatile u32, 0x009cf280u) = value; /* VA 0060dbcd */
    goto L_0060e64c;
L_0060dbd8:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9cd4u); /* VA 0060dbde */
    if (result != 0) goto L_0060dbf5;
    CM_GLOBAL(volatile u32, 0x009cf284u) = value; /* VA 0060dbea */
    goto L_0060e64c;
L_0060dbf5:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9cc4u); /* VA 0060dbfb */
    if (result != 0) goto L_0060dc12;
    CM_GLOBAL(volatile u32, 0x009cf288u) = value; /* VA 0060dc07 */
    goto L_0060e64c;
L_0060dc12:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9ca8u); /* VA 0060dc18 */
    if (result != 0) goto L_0060dc2f;
    CM_GLOBAL(volatile u32, 0x009cf28cu) = value; /* VA 0060dc24 */
    goto L_0060e64c;
L_0060dc2f:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9ca0u); /* VA 0060dc35 */
    if (result != 0) goto L_0060dc4c;
    CM_GLOBAL(volatile u32, 0x009cf290u) = value; /* VA 0060dc41 */
    goto L_0060e64c;
L_0060dc4c:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9c98u); /* VA 0060dc52 */
    if (result != 0) goto L_0060dc69;
    CM_GLOBAL(volatile u32, 0x009cf294u) = value; /* VA 0060dc5e */
    goto L_0060e64c;
L_0060dc69:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b1608u); /* VA 0060dc6f */
    if (result == 0) goto L_0060e636;
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9c8cu); /* VA 0060dc85 */
    if (result == 0) goto L_0060e636;
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9c80u); /* VA 0060dc9b */
    if (result != 0) goto L_0060dcb2;
    CM_GLOBAL(volatile u32, 0x009cf29cu) = value; /* VA 0060dca7 */
    goto L_0060e64c;
L_0060dcb2:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9c74u); /* VA 0060dcb8 */
    if (result != 0) goto L_0060dccf;
    CM_GLOBAL(volatile u32, 0x009cf2a0u) = value; /* VA 0060dcc4 */
    goto L_0060e64c;
L_0060dccf:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9c64u); /* VA 0060dcd5 */
    if (result != 0) goto L_0060dcec;
    CM_GLOBAL(volatile u32, 0x009cf2a4u) = value; /* VA 0060dce1 */
    goto L_0060e64c;
L_0060dcec:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9c58u); /* VA 0060dcf2 */
    if (result != 0) goto L_0060dd09;
    CM_GLOBAL(volatile u32, 0x009cf2a8u) = value; /* VA 0060dcfe */
    goto L_0060e64c;
L_0060dd09:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b15f4u); /* VA 0060dd0f */
    if (result != 0) goto L_0060dd26;
    CM_GLOBAL(volatile u32, 0x009cf2acu) = value; /* VA 0060dd1b */
    goto L_0060e64c;
L_0060dd26:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9c50u); /* VA 0060dd2c */
    if (result != 0) goto L_0060dd43;
    CM_GLOBAL(volatile u32, 0x009cf2b0u) = value; /* VA 0060dd38 */
    goto L_0060e64c;
L_0060dd43:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9c48u); /* VA 0060dd49 */
    if (result != 0) goto L_0060dd60;
    CM_GLOBAL(volatile u32, 0x009cf2b4u) = value; /* VA 0060dd55 */
    goto L_0060e64c;
L_0060dd60:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b15d0u); /* VA 0060dd66 */
    if (result != 0) goto L_0060dd7d;
    CM_GLOBAL(volatile u32, 0x009cf2b8u) = value; /* VA 0060dd72 */
    goto L_0060e64c;
L_0060dd7d:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9c38u); /* VA 0060dd83 */
    if (result != 0) goto L_0060dd9a;
    CM_GLOBAL(volatile u32, 0x009cf2bcu) = value; /* VA 0060dd8f */
    goto L_0060e64c;
L_0060dd9a:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b15b8u); /* VA 0060dda0 */
    if (result != 0) goto L_0060ddb7;
    CM_GLOBAL(volatile u32, 0x009cf2c0u) = value; /* VA 0060ddac */
    goto L_0060e64c;
L_0060ddb7:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9c2cu); /* VA 0060ddbd */
    if (result != 0) goto L_0060ddd4;
    CM_GLOBAL(volatile u32, 0x009cf2c4u) = value; /* VA 0060ddc9 */
    goto L_0060e64c;
L_0060ddd4:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9c20u); /* VA 0060ddda */
    if (result != 0) goto L_0060ddf1;
    CM_GLOBAL(volatile u32, 0x009cf2c8u) = value; /* VA 0060dde6 */
    goto L_0060e64c;
L_0060ddf1:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9c0cu); /* VA 0060ddf7 */
    if (result != 0) goto L_0060de0e;
    CM_GLOBAL(volatile u32, 0x009cf2ccu) = value; /* VA 0060de03 */
    goto L_0060e64c;
L_0060de0e:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9bfcu); /* VA 0060de14 */
    if (result != 0) goto L_0060de2b;
    CM_GLOBAL(volatile u32, 0x009cf2d0u) = value; /* VA 0060de20 */
    goto L_0060e64c;
L_0060de2b:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9bf4u); /* VA 0060de31 */
    if (result != 0) goto L_0060de48;
    CM_GLOBAL(volatile u32, 0x009cf2d4u) = value; /* VA 0060de3d */
    goto L_0060e64c;
L_0060de48:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9becu); /* VA 0060de4e */
    if (result != 0) goto L_0060de65;
    CM_GLOBAL(volatile u32, 0x009cf2d8u) = value; /* VA 0060de5a */
    goto L_0060e64c;
L_0060de65:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9bd8u); /* VA 0060de6b */
    if (result != 0) goto L_0060de82;
    CM_GLOBAL(volatile u32, 0x009cf2dcu) = value; /* VA 0060de77 */
    goto L_0060e64c;
L_0060de82:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9bccu); /* VA 0060de88 */
    if (result != 0) goto L_0060de9f;
    CM_GLOBAL(volatile u32, 0x009cf2e0u) = value; /* VA 0060de94 */
    goto L_0060e64c;
L_0060de9f:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9bc4u); /* VA 0060dea5 */
    if (result != 0) goto L_0060debc;
    CM_GLOBAL(volatile u32, 0x009cf2e4u) = value; /* VA 0060deb1 */
    goto L_0060e64c;
L_0060debc:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9bb0u); /* VA 0060dec2 */
    if (result != 0) goto L_0060ded9;
    CM_GLOBAL(volatile u32, 0x009cf2e8u) = value; /* VA 0060dece */
    goto L_0060e64c;
L_0060ded9:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9ba8u); /* VA 0060dedf */
    if (result != 0) goto L_0060def6;
    CM_GLOBAL(volatile u32, 0x009cf2ecu) = value; /* VA 0060deeb */
    goto L_0060e64c;
L_0060def6:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9b9cu); /* VA 0060defc */
    if (result != 0) goto L_0060df13;
    CM_GLOBAL(volatile u32, 0x009cf2f0u) = value; /* VA 0060df08 */
    goto L_0060e64c;
L_0060df13:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9b8cu); /* VA 0060df19 */
    if (result != 0) goto L_0060df30;
    CM_GLOBAL(volatile u32, 0x009cf2f4u) = value; /* VA 0060df25 */
    goto L_0060e64c;
L_0060df30:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9b84u); /* VA 0060df36 */
    if (result != 0) goto L_0060df4d;
    CM_GLOBAL(volatile u32, 0x009cf2f8u) = value; /* VA 0060df42 */
    goto L_0060e64c;
L_0060df4d:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b159cu); /* VA 0060df53 */
    if (result != 0) goto L_0060df6a;
    CM_GLOBAL(volatile u32, 0x009cf2fcu) = value; /* VA 0060df5f */
    goto L_0060e64c;
L_0060df6a:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b1584u); /* VA 0060df70 */
    if (result != 0) goto L_0060df87;
    CM_GLOBAL(volatile u32, 0x009cf300u) = value; /* VA 0060df7c */
    goto L_0060e64c;
L_0060df87:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9b70u); /* VA 0060df8d */
    if (result != 0) goto L_0060dfa4;
    CM_GLOBAL(volatile u32, 0x009cf304u) = value; /* VA 0060df99 */
    goto L_0060e64c;
L_0060dfa4:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9b68u); /* VA 0060dfaa */
    if (result != 0) goto L_0060dfc1;
    CM_GLOBAL(volatile u32, 0x009cf308u) = value; /* VA 0060dfb6 */
    goto L_0060e64c;
L_0060dfc1:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9b5cu); /* VA 0060dfc7 */
    if (result != 0) goto L_0060dfde;
    CM_GLOBAL(volatile u32, 0x009cf30cu) = value; /* VA 0060dfd3 */
    goto L_0060e64c;
L_0060dfde:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9b54u); /* VA 0060dfe4 */
    if (result != 0) goto L_0060dffb;
    CM_GLOBAL(volatile u32, 0x009cf310u) = value; /* VA 0060dff0 */
    goto L_0060e64c;
L_0060dffb:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b1568u); /* VA 0060e001 */
    if (result != 0) goto L_0060e018;
    CM_GLOBAL(volatile u32, 0x009cf314u) = value; /* VA 0060e00d */
    goto L_0060e64c;
L_0060e018:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9b4cu); /* VA 0060e01e */
    if (result != 0) goto L_0060e035;
    CM_GLOBAL(volatile u32, 0x009cf318u) = value; /* VA 0060e02a */
    goto L_0060e64c;
L_0060e035:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b1550u); /* VA 0060e03b */
    if (result != 0) goto L_0060e052;
    CM_GLOBAL(volatile u32, 0x009cf31cu) = value; /* VA 0060e047 */
    goto L_0060e64c;
L_0060e052:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9b44u); /* VA 0060e058 */
    if (result != 0) goto L_0060e06f;
    CM_GLOBAL(volatile u32, 0x009cf320u) = value; /* VA 0060e064 */
    goto L_0060e64c;
L_0060e06f:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9b38u); /* VA 0060e075 */
    if (result != 0) goto L_0060e08c;
    CM_GLOBAL(volatile u32, 0x009cf324u) = value; /* VA 0060e081 */
    goto L_0060e64c;
L_0060e08c:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9b30u); /* VA 0060e092 */
    if (result != 0) goto L_0060e0a9;
    CM_GLOBAL(volatile u32, 0x009cf328u) = value; /* VA 0060e09e */
    goto L_0060e64c;
L_0060e0a9:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9b20u); /* VA 0060e0af */
    if (result != 0) goto L_0060e0c6;
    CM_GLOBAL(volatile u32, 0x009cf32cu) = value; /* VA 0060e0bb */
    goto L_0060e64c;
L_0060e0c6:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9b18u); /* VA 0060e0cc */
    if (result != 0) goto L_0060e0e3;
    CM_GLOBAL(volatile u32, 0x009cf330u) = value; /* VA 0060e0d8 */
    goto L_0060e64c;
L_0060e0e3:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9b10u); /* VA 0060e0e9 */
    if (result != 0) goto L_0060e100;
    CM_GLOBAL(volatile u32, 0x009cf334u) = value; /* VA 0060e0f5 */
    goto L_0060e64c;
L_0060e100:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b1534u); /* VA 0060e106 */
    if (result != 0) goto L_0060e11d;
    CM_GLOBAL(volatile u32, 0x009cf338u) = value; /* VA 0060e112 */
    goto L_0060e64c;
L_0060e11d:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9b04u); /* VA 0060e123 */
    if (result != 0) goto L_0060e13a;
    CM_GLOBAL(volatile u32, 0x009cf33cu) = value; /* VA 0060e12f */
    goto L_0060e64c;
L_0060e13a:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9af8u); /* VA 0060e140 */
    if (result != 0) goto L_0060e157;
    CM_GLOBAL(volatile u32, 0x009cf340u) = value; /* VA 0060e14c */
    goto L_0060e64c;
L_0060e157:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9af0u); /* VA 0060e15d */
    if (result != 0) goto L_0060e174;
    CM_GLOBAL(volatile u32, 0x009cf344u) = value; /* VA 0060e169 */
    goto L_0060e64c;
L_0060e174:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9ae8u); /* VA 0060e17a */
    if (result != 0) goto L_0060e191;
    CM_GLOBAL(volatile u32, 0x009cf348u) = value; /* VA 0060e186 */
    goto L_0060e64c;
L_0060e191:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9ae0u); /* VA 0060e197 */
    if (result != 0) goto L_0060e1ae;
    CM_GLOBAL(volatile u32, 0x009cf34cu) = value; /* VA 0060e1a3 */
    goto L_0060e64c;
L_0060e1ae:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9ad4u); /* VA 0060e1b4 */
    if (result != 0) goto L_0060e1cb;
    CM_GLOBAL(volatile u32, 0x009cf350u) = value; /* VA 0060e1c0 */
    goto L_0060e64c;
L_0060e1cb:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9accu); /* VA 0060e1d1 */
    if (result != 0) goto L_0060e1e8;
    CM_GLOBAL(volatile u32, 0x009cf354u) = value; /* VA 0060e1dd */
    goto L_0060e64c;
L_0060e1e8:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9ac4u); /* VA 0060e1ee */
    if (result != 0) goto L_0060e205;
    CM_GLOBAL(volatile u32, 0x009cf358u) = value; /* VA 0060e1fa */
    goto L_0060e64c;
L_0060e205:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9abcu); /* VA 0060e20b */
    if (result != 0) goto L_0060e222;
    CM_GLOBAL(volatile u32, 0x009cf35cu) = value; /* VA 0060e217 */
    goto L_0060e64c;
L_0060e222:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b150cu); /* VA 0060e228 */
    if (result != 0) goto L_0060e23f;
    CM_GLOBAL(volatile u32, 0x009cf360u) = value; /* VA 0060e234 */
    goto L_0060e64c;
L_0060e23f:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9ab0u); /* VA 0060e245 */
    if (result != 0) goto L_0060e25c;
    CM_GLOBAL(volatile u32, 0x009cf364u) = value; /* VA 0060e251 */
    goto L_0060e64c;
L_0060e25c:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9aa8u); /* VA 0060e262 */
    if (result != 0) goto L_0060e279;
    CM_GLOBAL(volatile u32, 0x009cf368u) = value; /* VA 0060e26e */
    goto L_0060e64c;
L_0060e279:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b14ecu); /* VA 0060e27f */
    if (result != 0) goto L_0060e296;
    CM_GLOBAL(volatile u32, 0x009cf36cu) = value; /* VA 0060e28b */
    goto L_0060e64c;
L_0060e296:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9aa0u); /* VA 0060e29c */
    if (result != 0) goto L_0060e2b3;
    CM_GLOBAL(volatile u32, 0x009cf370u) = value; /* VA 0060e2a8 */
    goto L_0060e64c;
L_0060e2b3:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9a94u); /* VA 0060e2b9 */
    if (result == 0) goto L_0060e62e;
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9a88u); /* VA 0060e2cf */
    if (result == 0) goto L_0060e62e;
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9a7cu); /* VA 0060e2e5 */
    if (result != 0) goto L_0060e2fc;
    CM_GLOBAL(volatile u32, 0x009cf378u) = value; /* VA 0060e2f1 */
    goto L_0060e64c;
L_0060e2fc:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9a74u); /* VA 0060e302 */
    if (result != 0) goto L_0060e319;
    CM_GLOBAL(volatile u32, 0x009cf37cu) = value; /* VA 0060e30e */
    goto L_0060e64c;
L_0060e319:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9a6cu); /* VA 0060e31f */
    if (result != 0) goto L_0060e336;
    CM_GLOBAL(volatile u32, 0x009cf380u) = value; /* VA 0060e32b */
    goto L_0060e64c;
L_0060e336:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9a60u); /* VA 0060e33c */
    if (result != 0) goto L_0060e353;
    CM_GLOBAL(volatile u32, 0x009cf384u) = value; /* VA 0060e348 */
    goto L_0060e64c;
L_0060e353:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9a58u); /* VA 0060e359 */
    if (result != 0) goto L_0060e370;
    CM_GLOBAL(volatile u32, 0x009cf388u) = value; /* VA 0060e365 */
    goto L_0060e64c;
L_0060e370:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9a50u); /* VA 0060e376 */
    if (result != 0) goto L_0060e38d;
    CM_GLOBAL(volatile u32, 0x009cf38cu) = value; /* VA 0060e382 */
    goto L_0060e64c;
L_0060e38d:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9a48u); /* VA 0060e393 */
    if (result != 0) goto L_0060e3aa;
    CM_GLOBAL(volatile u32, 0x009cf390u) = value; /* VA 0060e39f */
    goto L_0060e64c;
L_0060e3aa:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9a40u); /* VA 0060e3b0 */
    if (result != 0) goto L_0060e3c7;
    CM_GLOBAL(volatile u32, 0x009cf394u) = value; /* VA 0060e3bc */
    goto L_0060e64c;
L_0060e3c7:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9a38u); /* VA 0060e3cd */
    if (result != 0) goto L_0060e3e4;
    CM_GLOBAL(volatile u32, 0x009cf398u) = value; /* VA 0060e3d9 */
    goto L_0060e64c;
L_0060e3e4:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9a30u); /* VA 0060e3ea */
    if (result != 0) goto L_0060e401;
    CM_GLOBAL(volatile u32, 0x009cf39cu) = value; /* VA 0060e3f6 */
    goto L_0060e64c;
L_0060e401:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9a20u); /* VA 0060e407 */
    if (result != 0) goto L_0060e41e;
    CM_GLOBAL(volatile u32, 0x009cf3a0u) = value; /* VA 0060e413 */
    goto L_0060e64c;
L_0060e41e:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9a14u); /* VA 0060e424 */
    if (result != 0) goto L_0060e43b;
    CM_GLOBAL(volatile u32, 0x009cf3a4u) = value; /* VA 0060e430 */
    goto L_0060e64c;
L_0060e43b:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9a08u); /* VA 0060e441 */
    if (result != 0) goto L_0060e458;
    CM_GLOBAL(volatile u32, 0x009cf3a8u) = value; /* VA 0060e44d */
    goto L_0060e64c;
L_0060e458:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9a00u); /* VA 0060e45e */
    if (result != 0) goto L_0060e475;
    CM_GLOBAL(volatile u32, 0x009cf3acu) = value; /* VA 0060e46a */
    goto L_0060e64c;
L_0060e475:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d99f4u); /* VA 0060e47b */
    if (result != 0) goto L_0060e492;
    CM_GLOBAL(volatile u32, 0x009cf3b0u) = value; /* VA 0060e487 */
    goto L_0060e64c;
L_0060e492:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d99ecu); /* VA 0060e498 */
    if (result != 0) goto L_0060e4af;
    CM_GLOBAL(volatile u32, 0x009cf3b4u) = value; /* VA 0060e4a4 */
    goto L_0060e64c;
L_0060e4af:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d99e0u); /* VA 0060e4b5 */
    if (result != 0) goto L_0060e4cc;
    CM_GLOBAL(volatile u32, 0x009cf3b8u) = value; /* VA 0060e4c1 */
    goto L_0060e64c;
L_0060e4cc:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d99d4u); /* VA 0060e4d2 */
    if (result != 0) goto L_0060e4e9;
    CM_GLOBAL(volatile u32, 0x009cf3bcu) = value; /* VA 0060e4de */
    goto L_0060e64c;
L_0060e4e9:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d99ccu); /* VA 0060e4ef */
    if (result != 0) goto L_0060e506;
    CM_GLOBAL(volatile u32, 0x009cf3c0u) = value; /* VA 0060e4fb */
    goto L_0060e64c;
L_0060e506:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d99c4u); /* VA 0060e50c */
    if (result != 0) goto L_0060e523;
    CM_GLOBAL(volatile u32, 0x009cf3c4u) = value; /* VA 0060e518 */
    goto L_0060e64c;
L_0060e523:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d99b8u); /* VA 0060e529 */
    if (result != 0) goto L_0060e540;
    CM_GLOBAL(volatile u32, 0x009cf3c8u) = value; /* VA 0060e535 */
    goto L_0060e64c;
L_0060e540:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d99acu); /* VA 0060e546 */
    if (result != 0) goto L_0060e55d;
    CM_GLOBAL(volatile u32, 0x009cf3ccu) = value; /* VA 0060e552 */
    goto L_0060e64c;
L_0060e55d:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b14bcu); /* VA 0060e563 */
    if (result != 0) goto L_0060e57a;
    CM_GLOBAL(volatile u32, 0x009cf3d0u) = value; /* VA 0060e56f */
    goto L_0060e64c;
L_0060e57a:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d99a4u); /* VA 0060e580 */
    if (result != 0) goto L_0060e597;
    CM_GLOBAL(volatile u32, 0x009cf3d4u) = value; /* VA 0060e58c */
    goto L_0060e64c;
L_0060e597:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d999cu); /* VA 0060e59d */
    if (result != 0) goto L_0060e5b4;
    CM_GLOBAL(volatile u32, 0x009cf3d8u) = value; /* VA 0060e5a9 */
    goto L_0060e64c;
L_0060e5b4:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9990u); /* VA 0060e5ba */
    if (result != 0) goto L_0060e5ce;
    CM_GLOBAL(volatile u32, 0x009cf3dcu) = value; /* VA 0060e5c6 */
    goto L_0060e64c;
L_0060e5ce:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9988u); /* VA 0060e5d4 */
    if (result == 0) goto L_0060e626;
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9978u); /* VA 0060e5e6 */
    if (result == 0) goto L_0060e626;
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b1494u); /* VA 0060e5f8 */
    if (result != 0) goto L_0060e60c;
    CM_GLOBAL(volatile u32, 0x009cf3e4u) = value; /* VA 0060e604 */
    goto L_0060e64c;
L_0060e60c:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9970u); /* VA 0060e612 */
    if (result != 0) goto L_0060e64c;
    CM_GLOBAL(volatile u32, 0x009cf3e8u) = value; /* VA 0060e61e */
    goto L_0060e64c;
L_0060e626:
    CM_GLOBAL(volatile u32, 0x009cf3e0u) = value; /* VA 0060e626 */
    goto L_0060e64c;
L_0060e62e:
    CM_GLOBAL(volatile u32, 0x009cf374u) = value; /* VA 0060e62e */
    goto L_0060e64c;
L_0060e636:
    CM_GLOBAL(volatile u32, 0x009cf298u) = value; /* VA 0060e636 */
    goto L_0060e64c;
L_0060e63e:
    CM_GLOBAL(volatile u32, 0x009cf26cu) = value; /* VA 0060e63e */
    goto L_0060e64c;
L_0060e646:
    CM_GLOBAL(volatile u32, 0x009cf230u) = value; /* VA 0060e646 */
L_0060e64c:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9968u); /* VA 0060e652 */
    if (result != 0) goto L_0060e667;
    CM_GLOBAL(volatile u32, 0x009cf3ecu) = value; /* VA 0060e65e */
    return;
L_0060e667:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9950u); /* VA 0060e66d */
    if (result != 0) goto L_0060e682;
    CM_GLOBAL(volatile u32, 0x009cf3f0u) = value; /* VA 0060e679 */
    return;
L_0060e682:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9944u); /* VA 0060e688 */
    if (result != 0) goto L_0060e69d;
    CM_GLOBAL(volatile u32, 0x009cf3f4u) = value; /* VA 0060e694 */
    return;
L_0060e69d:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9938u); /* VA 0060e6a3 */
    if (result != 0) goto L_0060e6b8;
    CM_GLOBAL(volatile u32, 0x009cf3f8u) = value; /* VA 0060e6af */
    return;
L_0060e6b8:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9930u); /* VA 0060e6be */
    if (result != 0) goto L_0060e6d3;
    CM_GLOBAL(volatile u32, 0x009cf3fcu) = value; /* VA 0060e6ca */
    return;
L_0060e6d3:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9928u); /* VA 0060e6d9 */
    if (result != 0) goto L_0060e6ee;
    CM_GLOBAL(volatile u32, 0x009cf400u) = value; /* VA 0060e6e5 */
    return;
L_0060e6ee:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d991cu); /* VA 0060e6f4 */
    if (result != 0) goto L_0060e709;
    CM_GLOBAL(volatile u32, 0x009cf404u) = value; /* VA 0060e700 */
    return;
L_0060e709:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b147cu); /* VA 0060e70f */
    if (result != 0) goto L_0060e724;
    CM_GLOBAL(volatile u32, 0x009cf408u) = value; /* VA 0060e71b */
    return;
L_0060e724:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9914u); /* VA 0060e72a */
    if (result != 0) goto L_0060e73f;
    CM_GLOBAL(volatile u32, 0x009cf40cu) = value; /* VA 0060e736 */
    return;
L_0060e73f:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9908u); /* VA 0060e745 */
    if (result != 0) goto L_0060e75a;
    CM_GLOBAL(volatile u32, 0x009cf410u) = value; /* VA 0060e751 */
    return;
L_0060e75a:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d98fcu); /* VA 0060e760 */
    if (result != 0) goto L_0060e775;
    CM_GLOBAL(volatile u32, 0x009cf414u) = value; /* VA 0060e76c */
    return;
L_0060e775:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d98f4u); /* VA 0060e77b */
    if (result != 0) goto L_0060e790;
    CM_GLOBAL(volatile u32, 0x009cf418u) = value; /* VA 0060e787 */
    return;
L_0060e790:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d98e0u); /* VA 0060e796 */
    if (result != 0) goto L_0060e7ab;
    CM_GLOBAL(volatile u32, 0x009cf41cu) = value; /* VA 0060e7a2 */
    return;
L_0060e7ab:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d98d4u); /* VA 0060e7b1 */
    if (result != 0) goto L_0060e7c6;
    CM_GLOBAL(volatile u32, 0x009cf420u) = value; /* VA 0060e7bd */
    return;
L_0060e7c6:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d98c8u); /* VA 0060e7cc */
    if (result != 0) goto L_0060e7e1;
    CM_GLOBAL(volatile u32, 0x009cf424u) = value; /* VA 0060e7d8 */
    return;
L_0060e7e1:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d98c0u); /* VA 0060e7e7 */
    if (result != 0) goto L_0060e7fc;
    CM_GLOBAL(volatile u32, 0x009cf428u) = value; /* VA 0060e7f3 */
    return;
L_0060e7fc:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d98b0u); /* VA 0060e802 */
    if (result != 0) goto L_0060e817;
    CM_GLOBAL(volatile u32, 0x009cf42cu) = value; /* VA 0060e80e */
    return;
L_0060e817:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b1450u); /* VA 0060e81d */
    if (result != 0) goto L_0060e832;
    CM_GLOBAL(volatile u32, 0x009cf430u) = value; /* VA 0060e829 */
    return;
L_0060e832:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b1430u); /* VA 0060e838 */
    if (result != 0) goto L_0060e84d;
    CM_GLOBAL(volatile u32, 0x009cf434u) = value; /* VA 0060e844 */
    return;
L_0060e84d:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d98a4u); /* VA 0060e853 */
    if (result != 0) goto L_0060e868;
    CM_GLOBAL(volatile u32, 0x009cf438u) = value; /* VA 0060e85f */
    return;
L_0060e868:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d989cu); /* VA 0060e86e */
    if (result != 0) goto L_0060e883;
    CM_GLOBAL(volatile u32, 0x009cf43cu) = value; /* VA 0060e87a */
    return;
L_0060e883:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d987cu); /* VA 0060e889 */
    if (result != 0) goto L_0060e89e;
    CM_GLOBAL(volatile u32, 0x009cf440u) = value; /* VA 0060e895 */
    return;
L_0060e89e:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9874u); /* VA 0060e8a4 */
    if (result != 0) goto L_0060e8b9;
    CM_GLOBAL(volatile u32, 0x009cf444u) = value; /* VA 0060e8b0 */
    return;
L_0060e8b9:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b13ecu); /* VA 0060e8bf */
    if (result != 0) goto L_0060e8d4;
    CM_GLOBAL(volatile u32, 0x009cf448u) = value; /* VA 0060e8cb */
    return;
L_0060e8d4:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d986cu); /* VA 0060e8da */
    if (result != 0) goto L_0060e8ef;
    CM_GLOBAL(volatile u32, 0x009cf44cu) = value; /* VA 0060e8e6 */
    return;
L_0060e8ef:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9860u); /* VA 0060e8f5 */
    if (result != 0) goto L_0060e90a;
    CM_GLOBAL(volatile u32, 0x009cf450u) = value; /* VA 0060e901 */
    return;
L_0060e90a:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d984cu); /* VA 0060e910 */
    if (result == 0) goto L_0060efbe;
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9838u); /* VA 0060e926 */
    if (result == 0) goto L_0060efbe;
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9828u); /* VA 0060e93c */
    if (result != 0) goto L_0060e951;
    CM_GLOBAL(volatile u32, 0x009cf458u) = value; /* VA 0060e948 */
    return;
L_0060e951:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b13bcu); /* VA 0060e957 */
    if (result != 0) goto L_0060e96c;
    CM_GLOBAL(volatile u32, 0x009cf45cu) = value; /* VA 0060e963 */
    return;
L_0060e96c:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9820u); /* VA 0060e972 */
    if (result != 0) goto L_0060e987;
    CM_GLOBAL(volatile u32, 0x009cf460u) = value; /* VA 0060e97e */
    return;
L_0060e987:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9814u); /* VA 0060e98d */
    if (result != 0) goto L_0060e9a2;
    CM_GLOBAL(volatile u32, 0x009cf464u) = value; /* VA 0060e999 */
    return;
L_0060e9a2:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9804u); /* VA 0060e9a8 */
    if (result != 0) goto L_0060e9bd;
    CM_GLOBAL(volatile u32, 0x009cf468u) = value; /* VA 0060e9b4 */
    return;
L_0060e9bd:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d97f8u); /* VA 0060e9c3 */
    if (result != 0) goto L_0060e9d8;
    CM_GLOBAL(volatile u32, 0x009cf46cu) = value; /* VA 0060e9cf */
    return;
L_0060e9d8:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d97ecu); /* VA 0060e9de */
    if (result != 0) goto L_0060e9f3;
    CM_GLOBAL(volatile u32, 0x009cf470u) = value; /* VA 0060e9ea */
    return;
L_0060e9f3:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d97e0u); /* VA 0060e9f9 */
    if (result != 0) goto L_0060ea0e;
    CM_GLOBAL(volatile u32, 0x009cf474u) = value; /* VA 0060ea05 */
    return;
L_0060ea0e:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d97d0u); /* VA 0060ea14 */
    if (result != 0) goto L_0060ea29;
    CM_GLOBAL(volatile u32, 0x009cf478u) = value; /* VA 0060ea20 */
    return;
L_0060ea29:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d97c8u); /* VA 0060ea2f */
    if (result != 0) goto L_0060ea44;
    CM_GLOBAL(volatile u32, 0x009cf47cu) = value; /* VA 0060ea3b */
    return;
L_0060ea44:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b1384u); /* VA 0060ea4a */
    if (result != 0) goto L_0060ea5f;
    CM_GLOBAL(volatile u32, 0x009cf480u) = value; /* VA 0060ea56 */
    return;
L_0060ea5f:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b14d0u); /* VA 0060ea65 */
    if (result != 0) goto L_0060ea7a;
    CM_GLOBAL(volatile u32, 0x009cf484u) = value; /* VA 0060ea71 */
    return;
L_0060ea7a:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b136cu); /* VA 0060ea80 */
    if (result != 0) goto L_0060ea95;
    CM_GLOBAL(volatile u32, 0x009cf488u) = value; /* VA 0060ea8c */
    return;
L_0060ea95:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d97bcu); /* VA 0060ea9b */
    if (result != 0) goto L_0060eab0;
    CM_GLOBAL(volatile u32, 0x009cf48cu) = value; /* VA 0060eaa7 */
    return;
L_0060eab0:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d97a8u); /* VA 0060eab6 */
    if (result != 0) goto L_0060eacb;
    CM_GLOBAL(volatile u32, 0x009cf490u) = value; /* VA 0060eac2 */
    return;
L_0060eacb:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d979cu); /* VA 0060ead1 */
    if (result != 0) goto L_0060eae6;
    CM_GLOBAL(volatile u32, 0x009cf494u) = value; /* VA 0060eadd */
    return;
L_0060eae6:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9780u); /* VA 0060eaec */
    if (result != 0) goto L_0060eb01;
    CM_GLOBAL(volatile u32, 0x009cf498u) = value; /* VA 0060eaf8 */
    return;
L_0060eb01:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9778u); /* VA 0060eb07 */
    if (result != 0) goto L_0060eb1c;
    CM_GLOBAL(volatile u32, 0x009cf49cu) = value; /* VA 0060eb13 */
    return;
L_0060eb1c:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9770u); /* VA 0060eb22 */
    if (result != 0) goto L_0060eb37;
    CM_GLOBAL(volatile u32, 0x009cf4a0u) = value; /* VA 0060eb2e */
    return;
L_0060eb37:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9764u); /* VA 0060eb3d */
    if (result != 0) goto L_0060eb52;
    CM_GLOBAL(volatile u32, 0x009cf4a4u) = value; /* VA 0060eb49 */
    return;
L_0060eb52:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b1334u); /* VA 0060eb58 */
    if (result != 0) goto L_0060eb6d;
    CM_GLOBAL(volatile u32, 0x009cf4a8u) = value; /* VA 0060eb64 */
    return;
L_0060eb6d:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b1304u); /* VA 0060eb73 */
    if (result != 0) goto L_0060eb88;
    CM_GLOBAL(volatile u32, 0x009cf4acu) = value; /* VA 0060eb7f */
    return;
L_0060eb88:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d975cu); /* VA 0060eb8e */
    if (result != 0) goto L_0060eba3;
    CM_GLOBAL(volatile u32, 0x009cf4b0u) = value; /* VA 0060eb9a */
    return;
L_0060eba3:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9754u); /* VA 0060eba9 */
    if (result != 0) goto L_0060ebbe;
    CM_GLOBAL(volatile u32, 0x009cf4b4u) = value; /* VA 0060ebb5 */
    return;
L_0060ebbe:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d974cu); /* VA 0060ebc4 */
    if (result == 0) goto L_0060efb5;
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d973cu); /* VA 0060ebda */
    if (result == 0) goto L_0060efb5;
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9730u); /* VA 0060ebf0 */
    if (result != 0) goto L_0060ec05;
    CM_GLOBAL(volatile u32, 0x009cf4bcu) = value; /* VA 0060ebfc */
    return;
L_0060ec05:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9724u); /* VA 0060ec0b */
    if (result != 0) goto L_0060ec20;
    CM_GLOBAL(volatile u32, 0x009cf4c0u) = value; /* VA 0060ec17 */
    return;
L_0060ec20:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9718u); /* VA 0060ec26 */
    if (result != 0) goto L_0060ec3b;
    CM_GLOBAL(volatile u32, 0x009cf4c4u) = value; /* VA 0060ec32 */
    return;
L_0060ec3b:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9710u); /* VA 0060ec41 */
    if (result != 0) goto L_0060ec56;
    CM_GLOBAL(volatile u32, 0x009cf4c8u) = value; /* VA 0060ec4d */
    return;
L_0060ec56:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9708u); /* VA 0060ec5c */
    if (result != 0) goto L_0060ec71;
    CM_GLOBAL(volatile u32, 0x009cf4ccu) = value; /* VA 0060ec68 */
    return;
L_0060ec71:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9700u); /* VA 0060ec77 */
    if (result != 0) goto L_0060ec8c;
    CM_GLOBAL(volatile u32, 0x009cf4d0u) = value; /* VA 0060ec83 */
    return;
L_0060ec8c:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d96ecu); /* VA 0060ec92 */
    if (result != 0) goto L_0060eca7;
    CM_GLOBAL(volatile u32, 0x009cf4d4u) = value; /* VA 0060ec9e */
    return;
L_0060eca7:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d96e4u); /* VA 0060ecad */
    if (result != 0) goto L_0060ecc2;
    CM_GLOBAL(volatile u32, 0x009cf4d8u) = value; /* VA 0060ecb9 */
    return;
L_0060ecc2:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b12ecu); /* VA 0060ecc8 */
    if (result != 0) goto L_0060ecdd;
    CM_GLOBAL(volatile u32, 0x009cf4dcu) = value; /* VA 0060ecd4 */
    return;
L_0060ecdd:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d96d4u); /* VA 0060ece3 */
    if (result != 0) goto L_0060ecf8;
    CM_GLOBAL(volatile u32, 0x009cf4e0u) = value; /* VA 0060ecef */
    return;
L_0060ecf8:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d96bcu); /* VA 0060ecfe */
    if (result != 0) goto L_0060ed13;
    CM_GLOBAL(volatile u32, 0x009cf4e4u) = value; /* VA 0060ed0a */
    return;
L_0060ed13:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d96acu); /* VA 0060ed19 */
    if (result != 0) goto L_0060ed2e;
    CM_GLOBAL(volatile u32, 0x009cf4e8u) = value; /* VA 0060ed25 */
    return;
L_0060ed2e:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d96a4u); /* VA 0060ed34 */
    if (result != 0) goto L_0060ed49;
    CM_GLOBAL(volatile u32, 0x009cf4ecu) = value; /* VA 0060ed40 */
    return;
L_0060ed49:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d969cu); /* VA 0060ed4f */
    if (result != 0) goto L_0060ed64;
    CM_GLOBAL(volatile u32, 0x009cf4f0u) = value; /* VA 0060ed5b */
    return;
L_0060ed64:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9694u); /* VA 0060ed6a */
    if (result != 0) goto L_0060ed7f;
    CM_GLOBAL(volatile u32, 0x009cf4f4u) = value; /* VA 0060ed76 */
    return;
L_0060ed7f:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9684u); /* VA 0060ed85 */
    if (result != 0) goto L_0060ed9a;
    CM_GLOBAL(volatile u32, 0x009cf4f8u) = value; /* VA 0060ed91 */
    return;
L_0060ed9a:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9678u); /* VA 0060eda0 */
    if (result != 0) goto L_0060edb5;
    CM_GLOBAL(volatile u32, 0x009cf4fcu) = value; /* VA 0060edac */
    return;
L_0060edb5:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9670u); /* VA 0060edbb */
    if (result != 0) goto L_0060edd0;
    CM_GLOBAL(volatile u32, 0x009cf500u) = value; /* VA 0060edc7 */
    return;
L_0060edd0:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9664u); /* VA 0060edd6 */
    if (result != 0) goto L_0060edeb;
    CM_GLOBAL(volatile u32, 0x009cf504u) = value; /* VA 0060ede2 */
    return;
L_0060edeb:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d965cu); /* VA 0060edf1 */
    if (result != 0) goto L_0060ee06;
    CM_GLOBAL(volatile u32, 0x009cf508u) = value; /* VA 0060edfd */
    return;
L_0060ee06:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d964cu); /* VA 0060ee0c */
    if (result != 0) goto L_0060ee21;
    CM_GLOBAL(volatile u32, 0x009cf50cu) = value; /* VA 0060ee18 */
    return;
L_0060ee21:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b12c0u); /* VA 0060ee27 */
    if (result != 0) goto L_0060ee3c;
    CM_GLOBAL(volatile u32, 0x009cf510u) = value; /* VA 0060ee33 */
    return;
L_0060ee3c:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d963cu); /* VA 0060ee42 */
    if (result == 0) goto L_0060efac;
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9634u); /* VA 0060ee58 */
    if (result == 0) goto L_0060efac;
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d962cu); /* VA 0060ee6e */
    if (result != 0) goto L_0060ee83;
    CM_GLOBAL(volatile u32, 0x009cf518u) = value; /* VA 0060ee7a */
    return;
L_0060ee83:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009b12a4u); /* VA 0060ee89 */
    if (result != 0) goto L_0060ee9e;
    CM_GLOBAL(volatile u32, 0x009cf51cu) = value; /* VA 0060ee95 */
    return;
L_0060ee9e:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9624u); /* VA 0060eea4 */
    if (result != 0) goto L_0060eeb9;
    CM_GLOBAL(volatile u32, 0x009cf520u) = value; /* VA 0060eeb0 */
    return;
L_0060eeb9:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d961cu); /* VA 0060eebf */
    if (result != 0) goto L_0060eed4;
    CM_GLOBAL(volatile u32, 0x009cf524u) = value; /* VA 0060eecb */
    return;
L_0060eed4:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9610u); /* VA 0060eeda */
    if (result != 0) goto L_0060eeef;
    CM_GLOBAL(volatile u32, 0x009cf528u) = value; /* VA 0060eee6 */
    return;
L_0060eeef:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9600u); /* VA 0060eef5 */
    if (result != 0) goto L_0060ef0a;
    CM_GLOBAL(volatile u32, 0x009cf52cu) = value; /* VA 0060ef01 */
    return;
L_0060ef0a:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d95f0u); /* VA 0060ef10 */
    if (result != 0) goto L_0060ef25;
    CM_GLOBAL(volatile u32, 0x009cf530u) = value; /* VA 0060ef1c */
    return;
L_0060ef25:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d95d8u); /* VA 0060ef2b */
    if (result != 0) goto L_0060ef40;
    CM_GLOBAL(volatile u32, 0x009cf534u) = value; /* VA 0060ef37 */
    return;
L_0060ef40:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d95bcu); /* VA 0060ef46 */
    if (result != 0) goto L_0060ef5b;
    CM_GLOBAL(volatile u32, 0x009cf538u) = value; /* VA 0060ef52 */
    return;
L_0060ef5b:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d95a8u); /* VA 0060ef61 */
    if (result != 0) goto L_0060ef76;
    CM_GLOBAL(volatile u32, 0x009cf53cu) = value; /* VA 0060ef6d */
    return;
L_0060ef76:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d959cu); /* VA 0060ef7c */
    if (result != 0) goto L_0060ef91;
    CM_GLOBAL(volatile u32, 0x009cf540u) = value; /* VA 0060ef88 */
    return;
L_0060ef91:
    result = cm_fn_00951c50((const volatile u8 *)name,
                              (const volatile u8 *)0x009d9590u); /* VA 0060ef97 */
    if (result != 0) goto L_0060efc4;
    CM_GLOBAL(volatile u32, 0x009cf544u) = value; /* VA 0060efa3 */
    return;
L_0060efac:
    CM_GLOBAL(volatile u32, 0x009cf514u) = value; /* VA 0060efac */
    return;
L_0060efb5:
    CM_GLOBAL(volatile u32, 0x009cf4b8u) = value; /* VA 0060efb5 */
    return;
L_0060efbe:
    CM_GLOBAL(volatile u32, 0x009cf454u) = value; /* VA 0060efbe */
L_0060efc4:
    return;
}
