/* WIP: canonical cases 361/525 fault at different stack data addresses.
 * Case 361 reaches original stack probe VA 00945627 through the retained
 * 00951C50 locale-helper boundary; data VA 01FFFF5C in original versus
 * 01FFFF40 (-O0) / 01FFFF50 (-O2). One failed canonical invocation.
 * Resolve physical call-boundary / fault-address obligations before retry.
 * Keep all inputs; do not imitate compiler frames or replace original helpers.
 */
/* Preferred-image VAs; base 00400000. CM0102 3.9.68 SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * Four consecutive comparison groups; shared stores and continuation order
 * follow the native branches. The original comparator remains a boundary.
 * Retain the last full comparator result in EAX.
 */
#include "cmre.h"

extern s32 CM_CDECL cm_fn_00951c50(const volatile u8 *first, const volatile u8 *second);

s32 CM_CDECL cm_fn_006146b0(const volatile u8 *name, u32 identifier)
{
    s32 result;
    u32 value;
    /* 006146bc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1f08u);
    /* 006146c1 */ value = identifier;
    /* 006146ca */ if (result != 0) goto v_006146d7;
    /* 006146cc */ CM_GLOBAL(volatile u32, 0x9d00a4u) = value;
    /* 006146d2 */ goto v_006152b7;
v_006146d7:
    /* 006146dd */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1ef4u);
    /* 006146e7 */ if (result != 0) goto v_006146f4;
    /* 006146e9 */ CM_GLOBAL(volatile u32, 0x9d00a8u) = value;
    /* 006146ef */ goto v_006152b7;
v_006146f4:
    /* 006146fa */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1ee8u);
    /* 00614704 */ if (result != 0) goto v_00614711;
    /* 00614706 */ CM_GLOBAL(volatile u32, 0x9d00acu) = value;
    /* 0061470c */ goto v_006152b7;
v_00614711:
    /* 00614717 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1ed8u);
    /* 00614721 */ if (result != 0) goto v_0061472e;
    /* 00614723 */ CM_GLOBAL(volatile u32, 0x9d00b0u) = value;
    /* 00614729 */ goto v_006152b7;
v_0061472e:
    /* 00614734 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1ec8u);
    /* 0061473e */ if (result != 0) goto v_0061474b;
    /* 00614740 */ CM_GLOBAL(volatile u32, 0x9d00b4u) = value;
    /* 00614746 */ goto v_006152b7;
v_0061474b:
    /* 00614751 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1ea4u);
    /* 0061475b */ if (result != 0) goto v_00614768;
    /* 0061475d */ CM_GLOBAL(volatile u32, 0x9d00b8u) = value;
    /* 00614763 */ goto v_006152b7;
v_00614768:
    /* 0061476e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1e90u);
    /* 00614778 */ if (result != 0) goto v_00614785;
    /* 0061477a */ CM_GLOBAL(volatile u32, 0x9d00bcu) = value;
    /* 00614780 */ goto v_006152b7;
v_00614785:
    /* 0061478b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1e74u);
    /* 00614795 */ if (result != 0) goto v_006147a2;
    /* 00614797 */ CM_GLOBAL(volatile u32, 0x9d00c0u) = value;
    /* 0061479d */ goto v_006152b7;
v_006147a2:
    /* 006147a8 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1e5cu);
    /* 006147b2 */ if (result != 0) goto v_006147bf;
    /* 006147b4 */ CM_GLOBAL(volatile u32, 0x9d00c4u) = value;
    /* 006147ba */ goto v_006152b7;
v_006147bf:
    /* 006147c5 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1e50u);
    /* 006147cf */ if (result != 0) goto v_006147dc;
    /* 006147d1 */ CM_GLOBAL(volatile u32, 0x9d00c8u) = value;
    /* 006147d7 */ goto v_006152b7;
v_006147dc:
    /* 006147e2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1e48u);
    /* 006147ec */ if (result != 0) goto v_006147f9;
    /* 006147ee */ CM_GLOBAL(volatile u32, 0x9d00ccu) = value;
    /* 006147f4 */ goto v_006152b7;
v_006147f9:
    /* 006147ff */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1e38u);
    /* 00614809 */ if (result != 0) goto v_00614816;
    /* 0061480b */ CM_GLOBAL(volatile u32, 0x9d00d0u) = value;
    /* 00614811 */ goto v_006152b7;
v_00614816:
    /* 0061481c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1e18u);
    /* 00614826 */ if (result != 0) goto v_00614833;
    /* 00614828 */ CM_GLOBAL(volatile u32, 0x9d00d4u) = value;
    /* 0061482e */ goto v_006152b7;
v_00614833:
    /* 00614839 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1e00u);
    /* 00614843 */ if (result != 0) goto v_00614850;
    /* 00614845 */ CM_GLOBAL(volatile u32, 0x9d00d8u) = value;
    /* 0061484b */ goto v_006152b7;
v_00614850:
    /* 00614856 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1decu);
    /* 00614860 */ if (result != 0) goto v_0061486d;
    /* 00614862 */ CM_GLOBAL(volatile u32, 0x9d00dcu) = value;
    /* 00614868 */ goto v_006152b7;
v_0061486d:
    /* 00614873 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1ddcu);
    /* 0061487d */ if (result != 0) goto v_0061488a;
    /* 0061487f */ CM_GLOBAL(volatile u32, 0x9d00e0u) = value;
    /* 00614885 */ goto v_006152b7;
v_0061488a:
    /* 00614890 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1dc8u);
    /* 0061489a */ if (result != 0) goto v_006148a7;
    /* 0061489c */ CM_GLOBAL(volatile u32, 0x9d00e4u) = value;
    /* 006148a2 */ goto v_006152b7;
v_006148a7:
    /* 006148ad */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1dbcu);
    /* 006148b7 */ if (result != 0) goto v_006148c4;
    /* 006148b9 */ CM_GLOBAL(volatile u32, 0x9d00e8u) = value;
    /* 006148bf */ goto v_006152b7;
v_006148c4:
    /* 006148ca */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1da8u);
    /* 006148d4 */ if (result != 0) goto v_006148e1;
    /* 006148d6 */ CM_GLOBAL(volatile u32, 0x9d00ecu) = value;
    /* 006148dc */ goto v_006152b7;
v_006148e1:
    /* 006148e7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1d9cu);
    /* 006148f1 */ if (result != 0) goto v_006148fe;
    /* 006148f3 */ CM_GLOBAL(volatile u32, 0x9d00f0u) = value;
    /* 006148f9 */ goto v_006152b7;
v_006148fe:
    /* 00614904 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1d88u);
    /* 0061490e */ if (result != 0) goto v_0061491b;
    /* 00614910 */ CM_GLOBAL(volatile u32, 0x9d00f4u) = value;
    /* 00614916 */ goto v_006152b7;
v_0061491b:
    /* 00614921 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1d70u);
    /* 0061492b */ if (result != 0) goto v_00614938;
    /* 0061492d */ CM_GLOBAL(volatile u32, 0x9d00f8u) = value;
    /* 00614933 */ goto v_006152b7;
v_00614938:
    /* 0061493e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1d54u);
    /* 00614948 */ if (result != 0) goto v_00614955;
    /* 0061494a */ CM_GLOBAL(volatile u32, 0x9d00fcu) = value;
    /* 00614950 */ goto v_006152b7;
v_00614955:
    /* 0061495b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1d40u);
    /* 00614965 */ if (result != 0) goto v_00614972;
    /* 00614967 */ CM_GLOBAL(volatile u32, 0x9d0100u) = value;
    /* 0061496d */ goto v_006152b7;
v_00614972:
    /* 00614978 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1d38u);
    /* 00614982 */ if (result != 0) goto v_0061498f;
    /* 00614984 */ CM_GLOBAL(volatile u32, 0x9d0104u) = value;
    /* 0061498a */ goto v_006152b7;
v_0061498f:
    /* 00614995 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1d24u);
    /* 0061499f */ if (result != 0) goto v_006149ac;
    /* 006149a1 */ CM_GLOBAL(volatile u32, 0x9d0108u) = value;
    /* 006149a7 */ goto v_006152b7;
v_006149ac:
    /* 006149b2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1d10u);
    /* 006149bc */ if (result != 0) goto v_006149c9;
    /* 006149be */ CM_GLOBAL(volatile u32, 0x9d010cu) = value;
    /* 006149c4 */ goto v_006152b7;
v_006149c9:
    /* 006149cf */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1d08u);
    /* 006149d9 */ if (result != 0) goto v_006149e6;
    /* 006149db */ CM_GLOBAL(volatile u32, 0x9d0110u) = value;
    /* 006149e1 */ goto v_006152b7;
v_006149e6:
    /* 006149ec */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1cf8u);
    /* 006149f6 */ if (result != 0) goto v_00614a03;
    /* 006149f8 */ CM_GLOBAL(volatile u32, 0x9d0114u) = value;
    /* 006149fe */ goto v_006152b7;
v_00614a03:
    /* 00614a09 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1ce8u);
    /* 00614a13 */ if (result != 0) goto v_00614a20;
    /* 00614a15 */ CM_GLOBAL(volatile u32, 0x9d0118u) = value;
    /* 00614a1b */ goto v_006152b7;
v_00614a20:
    /* 00614a26 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1cd8u);
    /* 00614a30 */ if (result != 0) goto v_00614a3d;
    /* 00614a32 */ CM_GLOBAL(volatile u32, 0x9d011cu) = value;
    /* 00614a38 */ goto v_006152b7;
v_00614a3d:
    /* 00614a43 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e568u);
    /* 00614a4d */ if (result != 0) goto v_00614a5a;
    /* 00614a4f */ CM_GLOBAL(volatile u32, 0x9d0120u) = value;
    /* 00614a55 */ goto v_006152b7;
v_00614a5a:
    /* 00614a60 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e58cu);
    /* 00614a6a */ if (result != 0) goto v_00614a77;
    /* 00614a6c */ CM_GLOBAL(volatile u32, 0x9d0124u) = value;
    /* 00614a72 */ goto v_006152b7;
v_00614a77:
    /* 00614a7d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e3c8u);
    /* 00614a87 */ if (result != 0) goto v_00614a94;
    /* 00614a89 */ CM_GLOBAL(volatile u32, 0x9d0128u) = value;
    /* 00614a8f */ goto v_006152b7;
v_00614a94:
    /* 00614a9a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e2c4u);
    /* 00614aa4 */ if (result != 0) goto v_00614ab1;
    /* 00614aa6 */ CM_GLOBAL(volatile u32, 0x9d012cu) = value;
    /* 00614aac */ goto v_006152b7;
v_00614ab1:
    /* 00614ab7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1cc0u);
    /* 00614ac1 */ if (result != 0) goto v_00614ace;
    /* 00614ac3 */ CM_GLOBAL(volatile u32, 0x9d0130u) = value;
    /* 00614ac9 */ goto v_006152b7;
v_00614ace:
    /* 00614ad4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1ca4u);
    /* 00614ade */ if (result != 0) goto v_00614aeb;
    /* 00614ae0 */ CM_GLOBAL(volatile u32, 0x9d0134u) = value;
    /* 00614ae6 */ goto v_006152b7;
v_00614aeb:
    /* 00614af1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e540u);
    /* 00614afb */ if (result != 0) goto v_00614b08;
    /* 00614afd */ CM_GLOBAL(volatile u32, 0x9d0138u) = value;
    /* 00614b03 */ goto v_006152b7;
v_00614b08:
    /* 00614b0e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1c90u);
    /* 00614b18 */ if (result != 0) goto v_00614b25;
    /* 00614b1a */ CM_GLOBAL(volatile u32, 0x9d013cu) = value;
    /* 00614b20 */ goto v_006152b7;
v_00614b25:
    /* 00614b2b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e34cu);
    /* 00614b35 */ if (result != 0) goto v_00614b42;
    /* 00614b37 */ CM_GLOBAL(volatile u32, 0x9d0140u) = value;
    /* 00614b3d */ goto v_006152b7;
v_00614b42:
    /* 00614b48 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1c7cu);
    /* 00614b52 */ if (result != 0) goto v_00614b5f;
    /* 00614b54 */ CM_GLOBAL(volatile u32, 0x9d0144u) = value;
    /* 00614b5a */ goto v_006152b7;
v_00614b5f:
    /* 00614b65 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e47cu);
    /* 00614b6f */ if (result != 0) goto v_00614b7c;
    /* 00614b71 */ CM_GLOBAL(volatile u32, 0x9d0148u) = value;
    /* 00614b77 */ goto v_006152b7;
v_00614b7c:
    /* 00614b82 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1c6cu);
    /* 00614b8c */ if (result != 0) goto v_00614b99;
    /* 00614b8e */ CM_GLOBAL(volatile u32, 0x9d014cu) = value;
    /* 00614b94 */ goto v_006152b7;
v_00614b99:
    /* 00614b9f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e520u);
    /* 00614ba9 */ if (result != 0) goto v_00614bb6;
    /* 00614bab */ CM_GLOBAL(volatile u32, 0x9d0150u) = value;
    /* 00614bb1 */ goto v_006152b7;
v_00614bb6:
    /* 00614bbc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1c54u);
    /* 00614bc6 */ if (result != 0) goto v_00614bd3;
    /* 00614bc8 */ CM_GLOBAL(volatile u32, 0x9d0154u) = value;
    /* 00614bce */ goto v_006152b7;
v_00614bd3:
    /* 00614bd9 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e4fcu);
    /* 00614be3 */ if (result != 0) goto v_00614bf0;
    /* 00614be5 */ CM_GLOBAL(volatile u32, 0x9d0158u) = value;
    /* 00614beb */ goto v_006152b7;
v_00614bf0:
    /* 00614bf6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1c38u);
    /* 00614c00 */ if (result != 0) goto v_00614c0d;
    /* 00614c02 */ CM_GLOBAL(volatile u32, 0x9d015cu) = value;
    /* 00614c08 */ goto v_006152b7;
v_00614c0d:
    /* 00614c13 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e4d8u);
    /* 00614c1d */ if (result != 0) goto v_00614c2a;
    /* 00614c1f */ CM_GLOBAL(volatile u32, 0x9d0160u) = value;
    /* 00614c25 */ goto v_006152b7;
v_00614c2a:
    /* 00614c30 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1c1cu);
    /* 00614c3a */ if (result != 0) goto v_00614c47;
    /* 00614c3c */ CM_GLOBAL(volatile u32, 0x9d0164u) = value;
    /* 00614c42 */ goto v_006152b7;
v_00614c47:
    /* 00614c4d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e4b8u);
    /* 00614c57 */ if (result != 0) goto v_00614c64;
    /* 00614c59 */ CM_GLOBAL(volatile u32, 0x9d0168u) = value;
    /* 00614c5f */ goto v_006152b7;
v_00614c64:
    /* 00614c6a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1c08u);
    /* 00614c74 */ if (result != 0) goto v_00614c81;
    /* 00614c76 */ CM_GLOBAL(volatile u32, 0x9d016cu) = value;
    /* 00614c7c */ goto v_006152b7;
v_00614c81:
    /* 00614c87 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1bf8u);
    /* 00614c91 */ if (result != 0) goto v_00614c9e;
    /* 00614c93 */ CM_GLOBAL(volatile u32, 0x9d0170u) = value;
    /* 00614c99 */ goto v_006152b7;
v_00614c9e:
    /* 00614ca4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1be8u);
    /* 00614cae */ if (result != 0) goto v_00614cbb;
    /* 00614cb0 */ CM_GLOBAL(volatile u32, 0x9d0174u) = value;
    /* 00614cb6 */ goto v_006152b7;
v_00614cbb:
    /* 00614cc1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e460u);
    /* 00614ccb */ if (result != 0) goto v_00614cd8;
    /* 00614ccd */ CM_GLOBAL(volatile u32, 0x9d0178u) = value;
    /* 00614cd3 */ goto v_006152b7;
v_00614cd8:
    /* 00614cde */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1bd4u);
    /* 00614ce8 */ if (result != 0) goto v_00614cf5;
    /* 00614cea */ CM_GLOBAL(volatile u32, 0x9d017cu) = value;
    /* 00614cf0 */ goto v_006152b7;
v_00614cf5:
    /* 00614cfb */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e438u);
    /* 00614d05 */ if (result != 0) goto v_00614d12;
    /* 00614d07 */ CM_GLOBAL(volatile u32, 0x9d0180u) = value;
    /* 00614d0d */ goto v_006152b7;
v_00614d12:
    /* 00614d18 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1bbcu);
    /* 00614d22 */ if (result != 0) goto v_00614d2f;
    /* 00614d24 */ CM_GLOBAL(volatile u32, 0x9d0184u) = value;
    /* 00614d2a */ goto v_006152b7;
v_00614d2f:
    /* 00614d35 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e0fcu);
    /* 00614d3f */ if (result != 0) goto v_00614d4c;
    /* 00614d41 */ CM_GLOBAL(volatile u32, 0x9d0188u) = value;
    /* 00614d47 */ goto v_006152b7;
v_00614d4c:
    /* 00614d52 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1ba8u);
    /* 00614d5c */ if (result != 0) goto v_00614d69;
    /* 00614d5e */ CM_GLOBAL(volatile u32, 0x9d018cu) = value;
    /* 00614d64 */ goto v_006152b7;
v_00614d69:
    /* 00614d6f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e3ecu);
    /* 00614d79 */ if (result != 0) goto v_00614d86;
    /* 00614d7b */ CM_GLOBAL(volatile u32, 0x9d0190u) = value;
    /* 00614d81 */ goto v_006152b7;
v_00614d86:
    /* 00614d8c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1b94u);
    /* 00614d96 */ if (result != 0) goto v_00614da3;
    /* 00614d98 */ CM_GLOBAL(volatile u32, 0x9d0194u) = value;
    /* 00614d9e */ goto v_006152b7;
v_00614da3:
    /* 00614da9 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e3a8u);
    /* 00614db3 */ if (result != 0) goto v_00614dc0;
    /* 00614db5 */ CM_GLOBAL(volatile u32, 0x9d0198u) = value;
    /* 00614dbb */ goto v_006152b7;
v_00614dc0:
    /* 00614dc6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1b84u);
    /* 00614dd0 */ if (result != 0) goto v_00614ddd;
    /* 00614dd2 */ CM_GLOBAL(volatile u32, 0x9d019cu) = value;
    /* 00614dd8 */ goto v_006152b7;
v_00614ddd:
    /* 00614de3 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1b6cu);
    /* 00614ded */ if (result != 0) goto v_00614dfa;
    /* 00614def */ CM_GLOBAL(volatile u32, 0x9d01a0u) = value;
    /* 00614df5 */ goto v_006152b7;
v_00614dfa:
    /* 00614e00 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1b54u);
    /* 00614e0a */ if (result != 0) goto v_00614e17;
    /* 00614e0c */ CM_GLOBAL(volatile u32, 0x9d01a4u) = value;
    /* 00614e12 */ goto v_006152b7;
v_00614e17:
    /* 00614e1d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1b3cu);
    /* 00614e27 */ if (result != 0) goto v_00614e34;
    /* 00614e29 */ CM_GLOBAL(volatile u32, 0x9d01a8u) = value;
    /* 00614e2f */ goto v_006152b7;
v_00614e34:
    /* 00614e3a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1b20u);
    /* 00614e44 */ if (result != 0) goto v_00614e51;
    /* 00614e46 */ CM_GLOBAL(volatile u32, 0x9d01acu) = value;
    /* 00614e4c */ goto v_006152b7;
v_00614e51:
    /* 00614e57 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1b0cu);
    /* 00614e61 */ if (result != 0) goto v_00614e6e;
    /* 00614e63 */ CM_GLOBAL(volatile u32, 0x9d01b0u) = value;
    /* 00614e69 */ goto v_006152b7;
v_00614e6e:
    /* 00614e74 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1af4u);
    /* 00614e7e */ if (result != 0) goto v_00614e8b;
    /* 00614e80 */ CM_GLOBAL(volatile u32, 0x9d01b4u) = value;
    /* 00614e86 */ goto v_006152b7;
v_00614e8b:
    /* 00614e91 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1ad4u);
    /* 00614e9b */ if (result != 0) goto v_00614ea8;
    /* 00614e9d */ CM_GLOBAL(volatile u32, 0x9d01b8u) = value;
    /* 00614ea3 */ goto v_006152b7;
v_00614ea8:
    /* 00614eae */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1ac0u);
    /* 00614eb8 */ if (result != 0) goto v_00614ec5;
    /* 00614eba */ CM_GLOBAL(volatile u32, 0x9d01bcu) = value;
    /* 00614ec0 */ goto v_006152b7;
v_00614ec5:
    /* 00614ecb */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1aa8u);
    /* 00614ed5 */ if (result != 0) goto v_00614ee2;
    /* 00614ed7 */ CM_GLOBAL(volatile u32, 0x9d01c0u) = value;
    /* 00614edd */ goto v_006152b7;
v_00614ee2:
    /* 00614ee8 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1a90u);
    /* 00614ef2 */ if (result != 0) goto v_00614eff;
    /* 00614ef4 */ CM_GLOBAL(volatile u32, 0x9d01c4u) = value;
    /* 00614efa */ goto v_006152b7;
v_00614eff:
    /* 00614f05 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1a84u);
    /* 00614f0f */ if (result != 0) goto v_00614f1c;
    /* 00614f11 */ CM_GLOBAL(volatile u32, 0x9d01c8u) = value;
    /* 00614f17 */ goto v_006152b7;
v_00614f1c:
    /* 00614f22 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1a6cu);
    /* 00614f2c */ if (result != 0) goto v_00614f39;
    /* 00614f2e */ CM_GLOBAL(volatile u32, 0x9d01ccu) = value;
    /* 00614f34 */ goto v_006152b7;
v_00614f39:
    /* 00614f3f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1a54u);
    /* 00614f49 */ if (result != 0) goto v_00614f56;
    /* 00614f4b */ CM_GLOBAL(volatile u32, 0x9d01d0u) = value;
    /* 00614f51 */ goto v_006152b7;
v_00614f56:
    /* 00614f5c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1a44u);
    /* 00614f66 */ if (result != 0) goto v_00614f73;
    /* 00614f68 */ CM_GLOBAL(volatile u32, 0x9d01d4u) = value;
    /* 00614f6e */ goto v_006152b7;
v_00614f73:
    /* 00614f79 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1a14u);
    /* 00614f83 */ if (result != 0) goto v_00614f90;
    /* 00614f85 */ CM_GLOBAL(volatile u32, 0x9d01d8u) = value;
    /* 00614f8b */ goto v_006152b7;
v_00614f90:
    /* 00614f96 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e19f8u);
    /* 00614fa0 */ if (result != 0) goto v_00614fad;
    /* 00614fa2 */ CM_GLOBAL(volatile u32, 0x9d01dcu) = value;
    /* 00614fa8 */ goto v_006152b7;
v_00614fad:
    /* 00614fb3 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e19e0u);
    /* 00614fbd */ if (result == 0) goto v_006152b1;
    /* 00614fc9 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e19c4u);
    /* 00614fd3 */ if (result == 0) goto v_006152b1;
    /* 00614fdf */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e19b8u);
    /* 00614fe9 */ if (result != 0) goto v_00614ff6;
    /* 00614feb */ CM_GLOBAL(volatile u32, 0x9d01e4u) = value;
    /* 00614ff1 */ goto v_006152b7;
v_00614ff6:
    /* 00614ffc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e19acu);
    /* 00615006 */ if (result != 0) goto v_00615013;
    /* 00615008 */ CM_GLOBAL(volatile u32, 0x9d01e8u) = value;
    /* 0061500e */ goto v_006152b7;
v_00615013:
    /* 00615019 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e19a4u);
    /* 00615023 */ if (result != 0) goto v_00615030;
    /* 00615025 */ CM_GLOBAL(volatile u32, 0x9d01ecu) = value;
    /* 0061502b */ goto v_006152b7;
v_00615030:
    /* 00615036 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1988u);
    /* 00615040 */ if (result != 0) goto v_0061504d;
    /* 00615042 */ CM_GLOBAL(volatile u32, 0x9d01f0u) = value;
    /* 00615048 */ goto v_006152b7;
v_0061504d:
    /* 00615053 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1974u);
    /* 0061505d */ if (result != 0) goto v_0061506a;
    /* 0061505f */ CM_GLOBAL(volatile u32, 0x9d01f4u) = value;
    /* 00615065 */ goto v_006152b7;
v_0061506a:
    /* 00615070 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1968u);
    /* 0061507a */ if (result != 0) goto v_00615087;
    /* 0061507c */ CM_GLOBAL(volatile u32, 0x9d01f8u) = value;
    /* 00615082 */ goto v_006152b7;
v_00615087:
    /* 0061508d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e195cu);
    /* 00615097 */ if (result != 0) goto v_006150a4;
    /* 00615099 */ CM_GLOBAL(volatile u32, 0x9d01fcu) = value;
    /* 0061509f */ goto v_006152b7;
v_006150a4:
    /* 006150aa */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e194cu);
    /* 006150b4 */ if (result != 0) goto v_006150c1;
    /* 006150b6 */ CM_GLOBAL(volatile u32, 0x9d0200u) = value;
    /* 006150bc */ goto v_006152b7;
v_006150c1:
    /* 006150c7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1938u);
    /* 006150d1 */ if (result != 0) goto v_006150de;
    /* 006150d3 */ CM_GLOBAL(volatile u32, 0x9d0204u) = value;
    /* 006150d9 */ goto v_006152b7;
v_006150de:
    /* 006150e4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1924u);
    /* 006150ee */ if (result != 0) goto v_006150fb;
    /* 006150f0 */ CM_GLOBAL(volatile u32, 0x9d0208u) = value;
    /* 006150f6 */ goto v_006152b7;
v_006150fb:
    /* 00615101 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e190cu);
    /* 0061510b */ if (result != 0) goto v_00615118;
    /* 0061510d */ CM_GLOBAL(volatile u32, 0x9d020cu) = value;
    /* 00615113 */ goto v_006152b7;
v_00615118:
    /* 0061511e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e18f4u);
    /* 00615128 */ if (result != 0) goto v_00615135;
    /* 0061512a */ CM_GLOBAL(volatile u32, 0x9d0210u) = value;
    /* 00615130 */ goto v_006152b7;
v_00615135:
    /* 0061513b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e18e4u);
    /* 00615145 */ if (result != 0) goto v_00615152;
    /* 00615147 */ CM_GLOBAL(volatile u32, 0x9d0214u) = value;
    /* 0061514d */ goto v_006152b7;
v_00615152:
    /* 00615158 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e18c8u);
    /* 00615162 */ if (result != 0) goto v_0061516f;
    /* 00615164 */ CM_GLOBAL(volatile u32, 0x9d0218u) = value;
    /* 0061516a */ goto v_006152b7;
v_0061516f:
    /* 00615175 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e18c0u);
    /* 0061517f */ if (result != 0) goto v_0061518c;
    /* 00615181 */ CM_GLOBAL(volatile u32, 0x9d021cu) = value;
    /* 00615187 */ goto v_006152b7;
v_0061518c:
    /* 00615192 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e18b8u);
    /* 0061519c */ if (result != 0) goto v_006151a9;
    /* 0061519e */ CM_GLOBAL(volatile u32, 0x9d0220u) = value;
    /* 006151a4 */ goto v_006152b7;
v_006151a9:
    /* 006151af */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e18a4u);
    /* 006151b9 */ if (result != 0) goto v_006151c6;
    /* 006151bb */ CM_GLOBAL(volatile u32, 0x9d0224u) = value;
    /* 006151c1 */ goto v_006152b7;
v_006151c6:
    /* 006151cc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1898u);
    /* 006151d6 */ if (result != 0) goto v_006151e3;
    /* 006151d8 */ CM_GLOBAL(volatile u32, 0x9d0228u) = value;
    /* 006151de */ goto v_006152b7;
v_006151e3:
    /* 006151e9 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1890u);
    /* 006151f3 */ if (result != 0) goto v_00615200;
    /* 006151f5 */ CM_GLOBAL(volatile u32, 0x9d022cu) = value;
    /* 006151fb */ goto v_006152b7;
v_00615200:
    /* 00615206 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1880u);
    /* 00615210 */ if (result != 0) goto v_0061521d;
    /* 00615212 */ CM_GLOBAL(volatile u32, 0x9d0230u) = value;
    /* 00615218 */ goto v_006152b7;
v_0061521d:
    /* 00615223 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e186cu);
    /* 0061522d */ if (result == 0) goto v_006152a9;
    /* 00615235 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1860u);
    /* 0061523f */ if (result == 0) goto v_006152a9;
    /* 00615247 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1854u);
    /* 00615251 */ if (result != 0) goto v_0061525b;
    /* 00615253 */ CM_GLOBAL(volatile u32, 0x9d0238u) = value;
    /* 00615259 */ goto v_006152b7;
v_0061525b:
    /* 00615261 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1848u);
    /* 0061526b */ if (result != 0) goto v_00615275;
    /* 0061526d */ CM_GLOBAL(volatile u32, 0x9d023cu) = value;
    /* 00615273 */ goto v_006152b7;
v_00615275:
    /* 0061527b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1834u);
    /* 00615285 */ if (result != 0) goto v_0061528f;
    /* 00615287 */ CM_GLOBAL(volatile u32, 0x9d0240u) = value;
    /* 0061528d */ goto v_006152b7;
v_0061528f:
    /* 00615295 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1828u);
    /* 0061529f */ if (result != 0) goto v_006152b7;
    /* 006152a1 */ CM_GLOBAL(volatile u32, 0x9d0244u) = value;
    /* 006152a7 */ goto v_006152b7;
v_006152a9:
    /* 006152a9 */ CM_GLOBAL(volatile u32, 0x9d0234u) = value;
    /* 006152af */ goto v_006152b7;
v_006152b1:
    /* 006152b1 */ CM_GLOBAL(volatile u32, 0x9d01e0u) = value;
v_006152b7:
    /* 006152bd */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1820u);
    /* 006152c7 */ if (result != 0) goto v_006152d4;
    /* 006152c9 */ CM_GLOBAL(volatile u32, 0x9d0248u) = value;
    /* 006152cf */ goto v_006157c0;
v_006152d4:
    /* 006152da */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e180cu);
    /* 006152e4 */ if (result != 0) goto v_006152f1;
    /* 006152e6 */ CM_GLOBAL(volatile u32, 0x9d024cu) = value;
    /* 006152ec */ goto v_006157c0;
v_006152f1:
    /* 006152f7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1804u);
    /* 00615301 */ if (result != 0) goto v_0061530e;
    /* 00615303 */ CM_GLOBAL(volatile u32, 0x9d0250u) = value;
    /* 00615309 */ goto v_006157c0;
v_0061530e:
    /* 00615314 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e36cu);
    /* 0061531e */ if (result != 0) goto v_0061532b;
    /* 00615320 */ CM_GLOBAL(volatile u32, 0x9d0254u) = value;
    /* 00615326 */ goto v_006157c0;
v_0061532b:
    /* 00615331 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e17f4u);
    /* 0061533b */ if (result != 0) goto v_00615348;
    /* 0061533d */ CM_GLOBAL(volatile u32, 0x9d0258u) = value;
    /* 00615343 */ goto v_006157c0;
v_00615348:
    /* 0061534e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e17e8u);
    /* 00615358 */ if (result != 0) goto v_00615365;
    /* 0061535a */ CM_GLOBAL(volatile u32, 0x9d0260u) = value;
    /* 00615360 */ goto v_006157c0;
v_00615365:
    /* 0061536b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e17ccu);
    /* 00615375 */ if (result != 0) goto v_00615382;
    /* 00615377 */ CM_GLOBAL(volatile u32, 0x9d025cu) = value;
    /* 0061537d */ goto v_006157c0;
v_00615382:
    /* 00615388 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e17c0u);
    /* 00615392 */ if (result != 0) goto v_0061539f;
    /* 00615394 */ CM_GLOBAL(volatile u32, 0x9d0264u) = value;
    /* 0061539a */ goto v_006157c0;
v_0061539f:
    /* 006153a5 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e17b8u);
    /* 006153af */ if (result != 0) goto v_006153bc;
    /* 006153b1 */ CM_GLOBAL(volatile u32, 0x9d0268u) = value;
    /* 006153b7 */ goto v_006157c0;
v_006153bc:
    /* 006153c2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e17a0u);
    /* 006153cc */ if (result != 0) goto v_006153d9;
    /* 006153ce */ CM_GLOBAL(volatile u32, 0x9d026cu) = value;
    /* 006153d4 */ goto v_006157c0;
v_006153d9:
    /* 006153df */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98a46cu);
    /* 006153e9 */ if (result != 0) goto v_006153f6;
    /* 006153eb */ CM_GLOBAL(volatile u32, 0x9d0270u) = value;
    /* 006153f1 */ goto v_006157c0;
v_006153f6:
    /* 006153fc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98a4f0u);
    /* 00615406 */ if (result != 0) goto v_00615413;
    /* 00615408 */ CM_GLOBAL(volatile u32, 0x9d0274u) = value;
    /* 0061540e */ goto v_006157c0;
v_00615413:
    /* 00615419 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98a2a8u);
    /* 00615423 */ if (result != 0) goto v_00615430;
    /* 00615425 */ CM_GLOBAL(volatile u32, 0x9d0278u) = value;
    /* 0061542b */ goto v_006157c0;
v_00615430:
    /* 00615436 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1780u);
    /* 00615440 */ if (result == 0) goto v_006157ba;
    /* 0061544c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e175cu);
    /* 00615456 */ if (result == 0) goto v_006157ba;
    /* 00615462 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98a304u);
    /* 0061546c */ if (result == 0) goto v_006157ba;
    /* 00615478 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e173cu);
    /* 00615482 */ if (result != 0) goto v_0061548f;
    /* 00615484 */ CM_GLOBAL(volatile u32, 0x9d0354u) = value;
    /* 0061548a */ goto v_006157c0;
v_0061548f:
    /* 00615495 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1718u);
    /* 0061549f */ if (result != 0) goto v_006154ac;
    /* 006154a1 */ CM_GLOBAL(volatile u32, 0x9d0280u) = value;
    /* 006154a7 */ goto v_006157c0;
v_006154ac:
    /* 006154b2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1708u);
    /* 006154bc */ if (result == 0) goto v_006157b2;
    /* 006154c8 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e16f8u);
    /* 006154d2 */ if (result == 0) goto v_006157b2;
    /* 006154de */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e16c4u);
    /* 006154e8 */ if (result == 0) goto v_006157b2;
    /* 006154f4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1698u);
    /* 006154fe */ if (result == 0) goto v_006157b2;
    /* 0061550a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1688u);
    /* 00615514 */ if (result == 0) goto v_006157aa;
    /* 00615520 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1670u);
    /* 0061552a */ if (result == 0) goto v_006157aa;
    /* 00615536 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e164cu);
    /* 00615540 */ if (result == 0) goto v_006157aa;
    /* 0061554c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1634u);
    /* 00615556 */ if (result == 0) goto v_006157a2;
    /* 00615562 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1620u);
    /* 0061556c */ if (result == 0) goto v_006157a2;
    /* 00615578 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1618u);
    /* 00615582 */ if (result == 0) goto v_0061579a;
    /* 0061558e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1608u);
    /* 00615598 */ if (result == 0) goto v_0061579a;
    /* 006155a4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e15f8u);
    /* 006155ae */ if (result == 0) goto v_00615792;
    /* 006155ba */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e15e4u);
    /* 006155c4 */ if (result == 0) goto v_00615792;
    /* 006155d0 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e15c8u);
    /* 006155da */ if (result == 0) goto v_0061578a;
    /* 006155e6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98a2e4u);
    /* 006155f0 */ if (result == 0) goto v_0061578a;
    /* 006155fc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e15b0u);
    /* 00615606 */ if (result == 0) goto v_00615782;
    /* 00615612 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e15a8u);
    /* 0061561c */ if (result == 0) goto v_00615782;
    /* 00615628 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e159cu);
    /* 00615632 */ if (result != 0) goto v_0061563f;
    /* 00615634 */ CM_GLOBAL(volatile u32, 0x9d029cu) = value;
    /* 0061563a */ goto v_006157c0;
v_0061563f:
    /* 00615645 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1590u);
    /* 0061564f */ if (result != 0) goto v_0061565c;
    /* 00615651 */ CM_GLOBAL(volatile u32, 0x9d02a0u) = value;
    /* 00615657 */ goto v_006157c0;
v_0061565c:
    /* 00615662 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e157cu);
    /* 0061566c */ if (result != 0) goto v_00615679;
    /* 0061566e */ CM_GLOBAL(volatile u32, 0xb63cdcu) = value;
    /* 00615674 */ goto v_006157c0;
v_00615679:
    /* 0061567f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e155cu);
    /* 00615689 */ if (result == 0) goto v_0061577a;
    /* 00615695 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1544u);
    /* 0061569f */ if (result == 0) goto v_0061577a;
    /* 006156ab */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1530u);
    /* 006156b5 */ if (result != 0) goto v_006156c2;
    /* 006156b7 */ CM_GLOBAL(volatile u32, 0xb63cbcu) = value;
    /* 006156bd */ goto v_006157c0;
v_006156c2:
    /* 006156c8 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1514u);
    /* 006156d2 */ if (result == 0) goto v_00615772;
    /* 006156de */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e14fcu);
    /* 006156e8 */ if (result == 0) goto v_00615772;
    /* 006156f4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e14e4u);
    /* 006156fe */ if (result == 0) goto v_00615772;
    /* 00615706 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e14d0u);
    /* 00615710 */ if (result == 0) goto v_0061576a;
    /* 00615718 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e14b8u);
    /* 00615722 */ if (result == 0) goto v_0061576a;
    /* 0061572a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e149cu);
    /* 00615734 */ if (result == 0) goto v_00615762;
    /* 0061573c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1484u);
    /* 00615746 */ if (result == 0) goto v_00615762;
    /* 0061574e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1470u);
    /* 00615758 */ if (result != 0) goto v_006157c0;
    /* 0061575a */ CM_GLOBAL(volatile u32, 0xb63cd8u) = value;
    /* 00615760 */ goto v_006157c0;
v_00615762:
    /* 00615762 */ CM_GLOBAL(volatile u32, 0xb63cacu) = value;
    /* 00615768 */ goto v_006157c0;
v_0061576a:
    /* 0061576a */ CM_GLOBAL(volatile u32, 0xb63ce8u) = value;
    /* 00615770 */ goto v_006157c0;
v_00615772:
    /* 00615772 */ CM_GLOBAL(volatile u32, 0xb63ca0u) = value;
    /* 00615778 */ goto v_006157c0;
v_0061577a:
    /* 0061577a */ CM_GLOBAL(volatile u32, 0xb63c9cu) = value;
    /* 00615780 */ goto v_006157c0;
v_00615782:
    /* 00615782 */ CM_GLOBAL(volatile u32, 0x9d0298u) = value;
    /* 00615788 */ goto v_006157c0;
v_0061578a:
    /* 0061578a */ CM_GLOBAL(volatile u32, 0x9d0358u) = value;
    /* 00615790 */ goto v_006157c0;
v_00615792:
    /* 00615792 */ CM_GLOBAL(volatile u32, 0x9d0294u) = value;
    /* 00615798 */ goto v_006157c0;
v_0061579a:
    /* 0061579a */ CM_GLOBAL(volatile u32, 0x9d0290u) = value;
    /* 006157a0 */ goto v_006157c0;
v_006157a2:
    /* 006157a2 */ CM_GLOBAL(volatile u32, 0x9d028cu) = value;
    /* 006157a8 */ goto v_006157c0;
v_006157aa:
    /* 006157aa */ CM_GLOBAL(volatile u32, 0x9d0288u) = value;
    /* 006157b0 */ goto v_006157c0;
v_006157b2:
    /* 006157b2 */ CM_GLOBAL(volatile u32, 0x9d0284u) = value;
    /* 006157b8 */ goto v_006157c0;
v_006157ba:
    /* 006157ba */ CM_GLOBAL(volatile u32, 0x9d027cu) = value;
v_006157c0:
    /* 006157c6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1458u);
    /* 006157d0 */ if (result == 0) goto v_0061683d;
    /* 006157dc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e143cu);
    /* 006157e6 */ if (result == 0) goto v_0061683d;
    /* 006157f2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e142cu);
    /* 006157fc */ if (result != 0) goto v_00615809;
    /* 006157fe */ CM_GLOBAL(volatile u32, 0xb63cb4u) = value;
    /* 00615804 */ goto v_00616843;
v_00615809:
    /* 0061580f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1414u);
    /* 00615819 */ if (result == 0) goto v_00616835;
    /* 00615825 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1400u);
    /* 0061582f */ if (result == 0) goto v_00616835;
    /* 0061583b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e13f0u);
    /* 00615845 */ if (result != 0) goto v_00615852;
    /* 00615847 */ CM_GLOBAL(volatile u32, 0xb63cd4u) = value;
    /* 0061584d */ goto v_00616843;
v_00615852:
    /* 00615858 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e13d8u);
    /* 00615862 */ if (result == 0) goto v_0061682d;
    /* 0061586e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e13c4u);
    /* 00615878 */ if (result == 0) goto v_0061682d;
    /* 00615884 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e13b0u);
    /* 0061588e */ if (result != 0) goto v_0061589b;
    /* 00615890 */ CM_GLOBAL(volatile u32, 0xb63ce4u) = value;
    /* 00615896 */ goto v_00616843;
v_0061589b:
    /* 006158a1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e139cu);
    /* 006158ab */ if (result == 0) goto v_00616825;
    /* 006158b7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e138cu);
    /* 006158c1 */ if (result == 0) goto v_00616825;
    /* 006158cd */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e137cu);
    /* 006158d7 */ if (result != 0) goto v_006158e4;
    /* 006158d9 */ CM_GLOBAL(volatile u32, 0xb63cf0u) = value;
    /* 006158df */ goto v_00616843;
v_006158e4:
    /* 006158ea */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1364u);
    /* 006158f4 */ if (result == 0) goto v_0061681d;
    /* 00615900 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1350u);
    /* 0061590a */ if (result == 0) goto v_0061681d;
    /* 00615916 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e133cu);
    /* 00615920 */ if (result == 0) goto v_0061681d;
    /* 0061592c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1328u);
    /* 00615936 */ if (result != 0) goto v_00615943;
    /* 00615938 */ CM_GLOBAL(volatile u32, 0xb63cc0u) = value;
    /* 0061593e */ goto v_00616843;
v_00615943:
    /* 00615949 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e130cu);
    /* 00615953 */ if (result == 0) goto v_00616815;
    /* 0061595f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e12f4u);
    /* 00615969 */ if (result == 0) goto v_00616815;
    /* 00615975 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e12e0u);
    /* 0061597f */ if (result == 0) goto v_0061680d;
    /* 0061598b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e12ccu);
    /* 00615995 */ if (result == 0) goto v_0061680d;
    /* 006159a1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e12b0u);
    /* 006159ab */ if (result == 0) goto v_00616805;
    /* 006159b7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1294u);
    /* 006159c1 */ if (result == 0) goto v_00616805;
    /* 006159cd */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e127cu);
    /* 006159d7 */ if (result != 0) goto v_006159e4;
    /* 006159d9 */ CM_GLOBAL(volatile u32, 0xb63ce0u) = value;
    /* 006159df */ goto v_00616843;
v_006159e4:
    /* 006159ea */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1260u);
    /* 006159f4 */ if (result == 0) goto v_006167fd;
    /* 00615a00 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e124cu);
    /* 00615a0a */ if (result == 0) goto v_006167fd;
    /* 00615a16 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1230u);
    /* 00615a20 */ if (result == 0) goto v_006167fd;
    /* 00615a2c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1220u);
    /* 00615a36 */ if (result != 0) goto v_00615a43;
    /* 00615a38 */ CM_GLOBAL(volatile u32, 0xb63cc4u) = value;
    /* 00615a3e */ goto v_00616843;
v_00615a43:
    /* 00615a49 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1208u);
    /* 00615a53 */ if (result == 0) goto v_006167f5;
    /* 00615a5f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e11f4u);
    /* 00615a69 */ if (result == 0) goto v_006167f5;
    /* 00615a75 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e11e8u);
    /* 00615a7f */ if (result != 0) goto v_00615a8c;
    /* 00615a81 */ CM_GLOBAL(volatile u32, 0xb63cfcu) = value;
    /* 00615a87 */ goto v_00616843;
v_00615a8c:
    /* 00615a92 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e11d4u);
    /* 00615a9c */ if (result == 0) goto v_006167ed;
    /* 00615aa8 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e11c4u);
    /* 00615ab2 */ if (result == 0) goto v_006167ed;
    /* 00615abe */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e11b0u);
    /* 00615ac8 */ if (result != 0) goto v_00615ad5;
    /* 00615aca */ CM_GLOBAL(volatile u32, 0x9d02a4u) = value;
    /* 00615ad0 */ goto v_00616843;
v_00615ad5:
    /* 00615adb */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1190u);
    /* 00615ae5 */ if (result == 0) goto v_006167e5;
    /* 00615af1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1178u);
    /* 00615afb */ if (result == 0) goto v_006167e5;
    /* 00615b07 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1164u);
    /* 00615b11 */ if (result != 0) goto v_00615b1e;
    /* 00615b13 */ CM_GLOBAL(volatile u32, 0x9d02acu) = value;
    /* 00615b19 */ goto v_00616843;
v_00615b1e:
    /* 00615b24 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1148u);
    /* 00615b2e */ if (result == 0) goto v_006167dd;
    /* 00615b3a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1130u);
    /* 00615b44 */ if (result == 0) goto v_006167dd;
    /* 00615b50 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1120u);
    /* 00615b5a */ if (result != 0) goto v_00615b67;
    /* 00615b5c */ CM_GLOBAL(volatile u32, 0x9d02b4u) = value;
    /* 00615b62 */ goto v_00616843;
v_00615b67:
    /* 00615b6d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1108u);
    /* 00615b77 */ if (result == 0) goto v_006167d5;
    /* 00615b83 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e10f4u);
    /* 00615b8d */ if (result == 0) goto v_006167d5;
    /* 00615b99 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e10e0u);
    /* 00615ba3 */ if (result != 0) goto v_00615bb0;
    /* 00615ba5 */ CM_GLOBAL(volatile u32, 0x9d02bcu) = value;
    /* 00615bab */ goto v_00616843;
v_00615bb0:
    /* 00615bb6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e10c4u);
    /* 00615bc0 */ if (result == 0) goto v_006167cd;
    /* 00615bcc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e10acu);
    /* 00615bd6 */ if (result == 0) goto v_006167cd;
    /* 00615be2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1094u);
    /* 00615bec */ if (result == 0) goto v_006167cd;
    /* 00615bf8 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e107cu);
    /* 00615c02 */ if (result != 0) goto v_00615c0f;
    /* 00615c04 */ CM_GLOBAL(volatile u32, 0x9d02c4u) = value;
    /* 00615c0a */ goto v_00616843;
v_00615c0f:
    /* 00615c15 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1060u);
    /* 00615c1f */ if (result != 0) goto v_00615c2c;
    /* 00615c21 */ CM_GLOBAL(volatile u32, 0x9d02c8u) = value;
    /* 00615c27 */ goto v_00616843;
v_00615c2c:
    /* 00615c32 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1048u);
    /* 00615c3c */ if (result != 0) goto v_00615c49;
    /* 00615c3e */ CM_GLOBAL(volatile u32, 0x9d02ccu) = value;
    /* 00615c44 */ goto v_00616843;
v_00615c49:
    /* 00615c4f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e102cu);
    /* 00615c59 */ if (result != 0) goto v_00615c66;
    /* 00615c5b */ CM_GLOBAL(volatile u32, 0x9d02d0u) = value;
    /* 00615c61 */ goto v_00616843;
v_00615c66:
    /* 00615c6c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1014u);
    /* 00615c76 */ if (result != 0) goto v_00615c83;
    /* 00615c78 */ CM_GLOBAL(volatile u32, 0x9d02d4u) = value;
    /* 00615c7e */ goto v_00616843;
v_00615c83:
    /* 00615c89 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0ffcu);
    /* 00615c93 */ if (result != 0) goto v_00615ca0;
    /* 00615c95 */ CM_GLOBAL(volatile u32, 0x9d02d8u) = value;
    /* 00615c9b */ goto v_00616843;
v_00615ca0:
    /* 00615ca6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0fe4u);
    /* 00615cb0 */ if (result != 0) goto v_00615cbd;
    /* 00615cb2 */ CM_GLOBAL(volatile u32, 0x9d02dcu) = value;
    /* 00615cb8 */ goto v_00616843;
v_00615cbd:
    /* 00615cc3 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0fc8u);
    /* 00615ccd */ if (result != 0) goto v_00615cda;
    /* 00615ccf */ CM_GLOBAL(volatile u32, 0x9d02e0u) = value;
    /* 00615cd5 */ goto v_00616843;
v_00615cda:
    /* 00615ce0 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e250u);
    /* 00615cea */ if (result != 0) goto v_00615cf7;
    /* 00615cec */ CM_GLOBAL(volatile u32, 0x9d02e4u) = value;
    /* 00615cf2 */ goto v_00616843;
v_00615cf7:
    /* 00615cfd */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0fb8u);
    /* 00615d07 */ if (result != 0) goto v_00615d14;
    /* 00615d09 */ CM_GLOBAL(volatile u32, 0x9d02e8u) = value;
    /* 00615d0f */ goto v_00616843;
v_00615d14:
    /* 00615d1a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0fa0u);
    /* 00615d24 */ if (result != 0) goto v_00615d31;
    /* 00615d26 */ CM_GLOBAL(volatile u32, 0x9d02ecu) = value;
    /* 00615d2c */ goto v_00616843;
v_00615d31:
    /* 00615d37 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0f90u);
    /* 00615d41 */ if (result != 0) goto v_00615d4e;
    /* 00615d43 */ CM_GLOBAL(volatile u32, 0x9d02f0u) = value;
    /* 00615d49 */ goto v_00616843;
v_00615d4e:
    /* 00615d54 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0f80u);
    /* 00615d5e */ if (result != 0) goto v_00615d6b;
    /* 00615d60 */ CM_GLOBAL(volatile u32, 0x9d02f4u) = value;
    /* 00615d66 */ goto v_00616843;
v_00615d6b:
    /* 00615d71 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0f74u);
    /* 00615d7b */ if (result != 0) goto v_00615d88;
    /* 00615d7d */ CM_GLOBAL(volatile u32, 0x9d02f8u) = value;
    /* 00615d83 */ goto v_00616843;
v_00615d88:
    /* 00615d8e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0f64u);
    /* 00615d98 */ if (result != 0) goto v_00615da5;
    /* 00615d9a */ CM_GLOBAL(volatile u32, 0x9d02fcu) = value;
    /* 00615da0 */ goto v_00616843;
v_00615da5:
    /* 00615dab */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0f58u);
    /* 00615db5 */ if (result != 0) goto v_00615dc2;
    /* 00615db7 */ CM_GLOBAL(volatile u32, 0x9d0304u) = value;
    /* 00615dbd */ goto v_00616843;
v_00615dc2:
    /* 00615dc8 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0f48u);
    /* 00615dd2 */ if (result != 0) goto v_00615ddf;
    /* 00615dd4 */ CM_GLOBAL(volatile u32, 0x9d0308u) = value;
    /* 00615dda */ goto v_00616843;
v_00615ddf:
    /* 00615de5 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0f38u);
    /* 00615def */ if (result == 0) goto v_006167c5;
    /* 00615dfb */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0f28u);
    /* 00615e05 */ if (result == 0) goto v_006167c5;
    /* 00615e11 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0f1cu);
    /* 00615e1b */ if (result != 0) goto v_00615e28;
    /* 00615e1d */ CM_GLOBAL(volatile u32, 0x9d0310u) = value;
    /* 00615e23 */ goto v_00616843;
v_00615e28:
    /* 00615e2e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0f08u);
    /* 00615e38 */ if (result != 0) goto v_00615e45;
    /* 00615e3a */ CM_GLOBAL(volatile u32, 0x9d0314u) = value;
    /* 00615e40 */ goto v_00616843;
v_00615e45:
    /* 00615e4b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0ef8u);
    /* 00615e55 */ if (result != 0) goto v_00615e62;
    /* 00615e57 */ CM_GLOBAL(volatile u32, 0x9d0318u) = value;
    /* 00615e5d */ goto v_00616843;
v_00615e62:
    /* 00615e68 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0ee4u);
    /* 00615e72 */ if (result != 0) goto v_00615e7f;
    /* 00615e74 */ CM_GLOBAL(volatile u32, 0x9d031cu) = value;
    /* 00615e7a */ goto v_00616843;
v_00615e7f:
    /* 00615e85 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0eccu);
    /* 00615e8f */ if (result != 0) goto v_00615e9c;
    /* 00615e91 */ CM_GLOBAL(volatile u32, 0x9d0320u) = value;
    /* 00615e97 */ goto v_00616843;
v_00615e9c:
    /* 00615ea2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0ebcu);
    /* 00615eac */ if (result != 0) goto v_00615eb9;
    /* 00615eae */ CM_GLOBAL(volatile u32, 0x9d0324u) = value;
    /* 00615eb4 */ goto v_00616843;
v_00615eb9:
    /* 00615ebf */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0eacu);
    /* 00615ec9 */ if (result != 0) goto v_00615ed6;
    /* 00615ecb */ CM_GLOBAL(volatile u32, 0x9d0328u) = value;
    /* 00615ed1 */ goto v_00616843;
v_00615ed6:
    /* 00615edc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0e9cu);
    /* 00615ee6 */ if (result == 0) goto v_006167bd;
    /* 00615ef2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0e8cu);
    /* 00615efc */ if (result == 0) goto v_006167bd;
    /* 00615f08 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0e7cu);
    /* 00615f12 */ if (result != 0) goto v_00615f1f;
    /* 00615f14 */ CM_GLOBAL(volatile u32, 0x9d0330u) = value;
    /* 00615f1a */ goto v_00616843;
v_00615f1f:
    /* 00615f25 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0e68u);
    /* 00615f2f */ if (result != 0) goto v_00615f3c;
    /* 00615f31 */ CM_GLOBAL(volatile u32, 0x9d0334u) = value;
    /* 00615f37 */ goto v_00616843;
v_00615f3c:
    /* 00615f42 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0e5cu);
    /* 00615f4c */ if (result != 0) goto v_00615f59;
    /* 00615f4e */ CM_GLOBAL(volatile u32, 0x9d0338u) = value;
    /* 00615f54 */ goto v_00616843;
v_00615f59:
    /* 00615f5f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98a664u);
    /* 00615f69 */ if (result != 0) goto v_00615f76;
    /* 00615f6b */ CM_GLOBAL(volatile u32, 0x9d033cu) = value;
    /* 00615f71 */ goto v_00616843;
v_00615f76:
    /* 00615f7c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98a4bcu);
    /* 00615f86 */ if (result != 0) goto v_00615f93;
    /* 00615f88 */ CM_GLOBAL(volatile u32, 0x9d0340u) = value;
    /* 00615f8e */ goto v_00616843;
v_00615f93:
    /* 00615f99 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98a4a4u);
    /* 00615fa3 */ if (result != 0) goto v_00615fb0;
    /* 00615fa5 */ CM_GLOBAL(volatile u32, 0x9d0344u) = value;
    /* 00615fab */ goto v_00616843;
v_00615fb0:
    /* 00615fb6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0e38u);
    /* 00615fc0 */ if (result == 0) goto v_006167b2;
    /* 00615fcc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0e14u);
    /* 00615fd6 */ if (result == 0) goto v_006167b2;
    /* 00615fe2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98a70cu);
    /* 00615fec */ if (result != 0) goto v_00615ff9;
    /* 00615fee */ CM_GLOBAL(volatile u32, 0x9d0350u) = value;
    /* 00615ff4 */ goto v_00616843;
v_00615ff9:
    /* 00615fff */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e173cu);
    /* 00616009 */ if (result != 0) goto v_00616016;
    /* 0061600b */ CM_GLOBAL(volatile u32, 0x9d0354u) = value;
    /* 00616011 */ goto v_00616843;
v_00616016:
    /* 0061601c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e15c8u);
    /* 00616026 */ if (result != 0) goto v_00616033;
    /* 00616028 */ CM_GLOBAL(volatile u32, 0x9d0358u) = value;
    /* 0061602e */ goto v_00616843;
v_00616033:
    /* 00616039 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98a7c0u);
    /* 00616043 */ if (result != 0) goto v_00616050;
    /* 00616045 */ CM_GLOBAL(volatile u32, 0x9d035cu) = value;
    /* 0061604b */ goto v_00616843;
v_00616050:
    /* 00616056 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98a800u);
    /* 00616060 */ if (result != 0) goto v_0061606d;
    /* 00616062 */ CM_GLOBAL(volatile u32, 0x9d0360u) = value;
    /* 00616068 */ goto v_00616843;
v_0061606d:
    /* 00616073 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0dfcu);
    /* 0061607d */ if (result == 0) goto v_006167a7;
    /* 00616089 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98a58cu);
    /* 00616093 */ if (result == 0) goto v_006167a7;
    /* 0061609f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98a2c8u);
    /* 006160a9 */ if (result != 0) goto v_006160b6;
    /* 006160ab */ CM_GLOBAL(volatile u32, 0x9d0368u) = value;
    /* 006160b1 */ goto v_00616843;
v_006160b6:
    /* 006160bc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98a484u);
    /* 006160c6 */ if (result != 0) goto v_006160d3;
    /* 006160c8 */ CM_GLOBAL(volatile u32, 0x9d036cu) = value;
    /* 006160ce */ goto v_00616843;
v_006160d3:
    /* 006160d9 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98a7a8u);
    /* 006160e3 */ if (result != 0) goto v_006160f0;
    /* 006160e5 */ CM_GLOBAL(volatile u32, 0x9d0370u) = value;
    /* 006160eb */ goto v_00616843;
v_006160f0:
    /* 006160f6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0de4u);
    /* 00616100 */ if (result != 0) goto v_0061610d;
    /* 00616102 */ CM_GLOBAL(volatile u32, 0x9d0374u) = value;
    /* 00616108 */ goto v_00616843;
v_0061610d:
    /* 00616113 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98a6a4u);
    /* 0061611d */ if (result != 0) goto v_0061612a;
    /* 0061611f */ CM_GLOBAL(volatile u32, 0x9d0378u) = value;
    /* 00616125 */ goto v_00616843;
v_0061612a:
    /* 00616130 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0dd4u);
    /* 0061613a */ if (result != 0) goto v_00616147;
    /* 0061613c */ CM_GLOBAL(volatile u32, 0x9d037cu) = value;
    /* 00616142 */ goto v_00616843;
v_00616147:
    /* 0061614d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0dc4u);
    /* 00616157 */ if (result != 0) goto v_00616164;
    /* 00616159 */ CM_GLOBAL(volatile u32, 0x9d0380u) = value;
    /* 0061615f */ goto v_00616843;
v_00616164:
    /* 0061616a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0dacu);
    /* 00616174 */ if (result != 0) goto v_00616181;
    /* 00616176 */ CM_GLOBAL(volatile u32, 0x9d0384u) = value;
    /* 0061617c */ goto v_00616843;
v_00616181:
    /* 00616187 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0d98u);
    /* 00616191 */ if (result != 0) goto v_0061619e;
    /* 00616193 */ CM_GLOBAL(volatile u32, 0x9d0388u) = value;
    /* 00616199 */ goto v_00616843;
v_0061619e:
    /* 006161a4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0d80u);
    /* 006161ae */ if (result == 0) goto v_0061679c;
    /* 006161ba */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0d70u);
    /* 006161c4 */ if (result == 0) goto v_0061679c;
    /* 006161d0 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0d58u);
    /* 006161da */ if (result != 0) goto v_006161e7;
    /* 006161dc */ CM_GLOBAL(volatile u32, 0x9d0390u) = value;
    /* 006161e2 */ goto v_00616843;
v_006161e7:
    /* 006161ed */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0d44u);
    /* 006161f7 */ if (result == 0) goto v_00616791;
    /* 00616203 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0d30u);
    /* 0061620d */ if (result == 0) goto v_00616791;
    /* 00616219 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0d20u);
    /* 00616223 */ if (result == 0) goto v_00616791;
    /* 0061622f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0d0cu);
    /* 00616239 */ if (result != 0) goto v_00616246;
    /* 0061623b */ CM_GLOBAL(volatile u32, 0x9d0398u) = value;
    /* 00616241 */ goto v_00616843;
v_00616246:
    /* 0061624c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0cf8u);
    /* 00616256 */ if (result != 0) goto v_00616263;
    /* 00616258 */ CM_GLOBAL(volatile u32, 0x9d039cu) = value;
    /* 0061625e */ goto v_00616843;
v_00616263:
    /* 00616269 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0cecu);
    /* 00616273 */ if (result != 0) goto v_00616280;
    /* 00616275 */ CM_GLOBAL(volatile u32, 0x9d03a0u) = value;
    /* 0061627b */ goto v_00616843;
v_00616280:
    /* 00616286 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0cdcu);
    /* 00616290 */ if (result != 0) goto v_0061629d;
    /* 00616292 */ CM_GLOBAL(volatile u32, 0x9d03a4u) = value;
    /* 00616298 */ goto v_00616843;
v_0061629d:
    /* 006162a3 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0cccu);
    /* 006162ad */ if (result != 0) goto v_006162ba;
    /* 006162af */ CM_GLOBAL(volatile u32, 0x9d03a8u) = value;
    /* 006162b5 */ goto v_00616843;
v_006162ba:
    /* 006162c0 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e2e8u);
    /* 006162ca */ if (result != 0) goto v_006162d7;
    /* 006162cc */ CM_GLOBAL(volatile u32, 0x9d03acu) = value;
    /* 006162d2 */ goto v_00616843;
v_006162d7:
    /* 006162dd */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0cb8u);
    /* 006162e7 */ if (result != 0) goto v_006162f4;
    /* 006162e9 */ CM_GLOBAL(volatile u32, 0x9d03b0u) = value;
    /* 006162ef */ goto v_00616843;
v_006162f4:
    /* 006162fa */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e29cu);
    /* 00616304 */ if (result != 0) goto v_00616311;
    /* 00616306 */ CM_GLOBAL(volatile u32, 0x9d03b4u) = value;
    /* 0061630c */ goto v_00616843;
v_00616311:
    /* 00616317 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0ca8u);
    /* 00616321 */ if (result != 0) goto v_0061632e;
    /* 00616323 */ CM_GLOBAL(volatile u32, 0x9d03b8u) = value;
    /* 00616329 */ goto v_00616843;
v_0061632e:
    /* 00616334 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e19cu);
    /* 0061633e */ if (result != 0) goto v_0061634b;
    /* 00616340 */ CM_GLOBAL(volatile u32, 0x9d03bcu) = value;
    /* 00616346 */ goto v_00616843;
v_0061634b:
    /* 00616351 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0c98u);
    /* 0061635b */ if (result != 0) goto v_00616368;
    /* 0061635d */ CM_GLOBAL(volatile u32, 0x9d03c0u) = value;
    /* 00616363 */ goto v_00616843;
v_00616368:
    /* 0061636e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e38cu);
    /* 00616378 */ if (result != 0) goto v_00616385;
    /* 0061637a */ CM_GLOBAL(volatile u32, 0x9d03c4u) = value;
    /* 00616380 */ goto v_00616843;
v_00616385:
    /* 0061638b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0c88u);
    /* 00616395 */ if (result != 0) goto v_006163a2;
    /* 00616397 */ CM_GLOBAL(volatile u32, 0x9d03c8u) = value;
    /* 0061639d */ goto v_00616843;
v_006163a2:
    /* 006163a8 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0c7cu);
    /* 006163b2 */ if (result != 0) goto v_006163bf;
    /* 006163b4 */ CM_GLOBAL(volatile u32, 0x9d03ccu) = value;
    /* 006163ba */ goto v_00616843;
v_006163bf:
    /* 006163c5 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0c6cu);
    /* 006163cf */ if (result == 0) goto v_00616786;
    /* 006163db */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0c54u);
    /* 006163e5 */ if (result == 0) goto v_00616786;
    /* 006163f1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0c40u);
    /* 006163fb */ if (result != 0) goto v_00616408;
    /* 006163fd */ CM_GLOBAL(volatile u32, 0x9d03d4u) = value;
    /* 00616403 */ goto v_00616843;
v_00616408:
    /* 0061640e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e1f8u);
    /* 00616418 */ if (result != 0) goto v_00616425;
    /* 0061641a */ CM_GLOBAL(volatile u32, 0x9d03d8u) = value;
    /* 00616420 */ goto v_00616843;
v_00616425:
    /* 0061642b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0c2cu);
    /* 00616435 */ if (result != 0) goto v_00616442;
    /* 00616437 */ CM_GLOBAL(volatile u32, 0x9d03dcu) = value;
    /* 0061643d */ goto v_00616843;
v_00616442:
    /* 00616448 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e0d4u);
    /* 00616452 */ if (result != 0) goto v_0061645f;
    /* 00616454 */ CM_GLOBAL(volatile u32, 0x9d03e0u) = value;
    /* 0061645a */ goto v_00616843;
v_0061645f:
    /* 00616465 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0c10u);
    /* 0061646f */ if (result != 0) goto v_0061647c;
    /* 00616471 */ CM_GLOBAL(volatile u32, 0x9d03e4u) = value;
    /* 00616477 */ goto v_00616843;
v_0061647c:
    /* 00616482 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e410u);
    /* 0061648c */ if (result != 0) goto v_00616499;
    /* 0061648e */ CM_GLOBAL(volatile u32, 0x9d03e8u) = value;
    /* 00616494 */ goto v_00616843;
v_00616499:
    /* 0061649f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0bf4u);
    /* 006164a9 */ if (result != 0) goto v_006164b6;
    /* 006164ab */ CM_GLOBAL(volatile u32, 0x9d03ecu) = value;
    /* 006164b1 */ goto v_00616843;
v_006164b6:
    /* 006164bc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e160u);
    /* 006164c6 */ if (result != 0) goto v_006164d3;
    /* 006164c8 */ CM_GLOBAL(volatile u32, 0x9d03f0u) = value;
    /* 006164ce */ goto v_00616843;
v_006164d3:
    /* 006164d9 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0be0u);
    /* 006164e3 */ if (result != 0) goto v_006164f0;
    /* 006164e5 */ CM_GLOBAL(volatile u32, 0x9d03f4u) = value;
    /* 006164eb */ goto v_00616843;
v_006164f0:
    /* 006164f6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e064u);
    /* 00616500 */ if (result != 0) goto v_0061650d;
    /* 00616502 */ CM_GLOBAL(volatile u32, 0x9d03f8u) = value;
    /* 00616508 */ goto v_00616843;
v_0061650d:
    /* 00616513 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0bc4u);
    /* 0061651d */ if (result != 0) goto v_0061652a;
    /* 0061651f */ CM_GLOBAL(volatile u32, 0x9d03fcu) = value;
    /* 00616525 */ goto v_00616843;
v_0061652a:
    /* 00616530 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e30cu);
    /* 0061653a */ if (result != 0) goto v_00616547;
    /* 0061653c */ CM_GLOBAL(volatile u32, 0x9d0400u) = value;
    /* 00616542 */ goto v_00616843;
v_00616547:
    /* 0061654d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0bb4u);
    /* 00616557 */ if (result != 0) goto v_00616564;
    /* 00616559 */ CM_GLOBAL(volatile u32, 0x9d0404u) = value;
    /* 0061655f */ goto v_00616843;
v_00616564:
    /* 0061656a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0ba4u);
    /* 00616574 */ if (result != 0) goto v_00616581;
    /* 00616576 */ CM_GLOBAL(volatile u32, 0x9d0408u) = value;
    /* 0061657c */ goto v_00616843;
v_00616581:
    /* 00616587 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0b90u);
    /* 00616591 */ if (result != 0) goto v_0061659e;
    /* 00616593 */ CM_GLOBAL(volatile u32, 0x9d040cu) = value;
    /* 00616599 */ goto v_00616843;
v_0061659e:
    /* 006165a4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e498u);
    /* 006165ae */ if (result != 0) goto v_006165bb;
    /* 006165b0 */ CM_GLOBAL(volatile u32, 0x9d0410u) = value;
    /* 006165b6 */ goto v_00616843;
v_006165bb:
    /* 006165c1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0b7cu);
    /* 006165cb */ if (result != 0) goto v_006165d8;
    /* 006165cd */ CM_GLOBAL(volatile u32, 0x9d0414u) = value;
    /* 006165d3 */ goto v_00616843;
v_006165d8:
    /* 006165de */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e08cu);
    /* 006165e8 */ if (result != 0) goto v_006165f5;
    /* 006165ea */ CM_GLOBAL(volatile u32, 0x9d0418u) = value;
    /* 006165f0 */ goto v_00616843;
v_006165f5:
    /* 006165fb */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0b68u);
    /* 00616605 */ if (result != 0) goto v_00616612;
    /* 00616607 */ CM_GLOBAL(volatile u32, 0x9d041cu) = value;
    /* 0061660d */ goto v_00616843;
v_00616612:
    /* 00616618 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0b5cu);
    /* 00616622 */ if (result == 0) goto v_0061677b;
    /* 0061662e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0b4cu);
    /* 00616638 */ if (result == 0) goto v_0061677b;
    /* 00616644 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0b3cu);
    /* 0061664e */ if (result == 0) goto v_00616770;
    /* 0061665a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0b30u);
    /* 00616664 */ if (result == 0) goto v_00616770;
    /* 00616670 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0b24u);
    /* 0061667a */ if (result == 0) goto v_00616765;
    /* 00616686 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0b14u);
    /* 00616690 */ if (result == 0) goto v_00616765;
    /* 0061669c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0af4u);
    /* 006166a6 */ if (result != 0) goto v_006166b3;
    /* 006166a8 */ CM_GLOBAL(volatile u32, 0x9d042cu) = value;
    /* 006166ae */ goto v_00616843;
v_006166b3:
    /* 006166b9 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0ae4u);
    /* 006166c3 */ if (result != 0) goto v_006166d0;
    /* 006166c5 */ CM_GLOBAL(volatile u32, 0x9d0430u) = value;
    /* 006166cb */ goto v_00616843;
v_006166d0:
    /* 006166d6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0ad4u);
    /* 006166e0 */ if (result != 0) goto v_006166ed;
    /* 006166e2 */ CM_GLOBAL(volatile u32, 0x9d0434u) = value;
    /* 006166e8 */ goto v_00616843;
v_006166ed:
    /* 006166f3 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0ac4u);
    /* 006166fd */ if (result != 0) goto v_0061670a;
    /* 006166ff */ CM_GLOBAL(volatile u32, 0x9d0438u) = value;
    /* 00616705 */ goto v_00616843;
v_0061670a:
    /* 00616710 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0abcu);
    /* 0061671a */ if (result != 0) goto v_00616727;
    /* 0061671c */ CM_GLOBAL(volatile u32, 0x9d043cu) = value;
    /* 00616722 */ goto v_00616843;
v_00616727:
    /* 0061672d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0aa0u);
    /* 00616737 */ if (result != 0) goto v_00616744;
    /* 00616739 */ CM_GLOBAL(volatile u32, 0x9d0440u) = value;
    /* 0061673f */ goto v_00616843;
v_00616744:
    /* 0061674a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0a80u);
    /* 00616754 */ if (result != 0) goto v_00616843;
    /* 0061675a */ CM_GLOBAL(volatile u32, 0x9d0444u) = value;
    /* 00616760 */ goto v_00616843;
v_00616765:
    /* 00616765 */ CM_GLOBAL(volatile u32, 0x9d0428u) = value;
    /* 0061676b */ goto v_00616843;
v_00616770:
    /* 00616770 */ CM_GLOBAL(volatile u32, 0x9d0424u) = value;
    /* 00616776 */ goto v_00616843;
v_0061677b:
    /* 0061677b */ CM_GLOBAL(volatile u32, 0x9d0420u) = value;
    /* 00616781 */ goto v_00616843;
v_00616786:
    /* 00616786 */ CM_GLOBAL(volatile u32, 0x9d03d0u) = value;
    /* 0061678c */ goto v_00616843;
v_00616791:
    /* 00616791 */ CM_GLOBAL(volatile u32, 0x9d0394u) = value;
    /* 00616797 */ goto v_00616843;
v_0061679c:
    /* 0061679c */ CM_GLOBAL(volatile u32, 0x9d038cu) = value;
    /* 006167a2 */ goto v_00616843;
v_006167a7:
    /* 006167a7 */ CM_GLOBAL(volatile u32, 0x9d0364u) = value;
    /* 006167ad */ goto v_00616843;
v_006167b2:
    /* 006167b2 */ CM_GLOBAL(volatile u32, 0x9d0348u) = value;
    /* 006167b8 */ goto v_00616843;
v_006167bd:
    /* 006167bd */ CM_GLOBAL(volatile u32, 0x9d032cu) = value;
    /* 006167c3 */ goto v_00616843;
v_006167c5:
    /* 006167c5 */ CM_GLOBAL(volatile u32, 0x9d030cu) = value;
    /* 006167cb */ goto v_00616843;
v_006167cd:
    /* 006167cd */ CM_GLOBAL(volatile u32, 0x9d02c0u) = value;
    /* 006167d3 */ goto v_00616843;
v_006167d5:
    /* 006167d5 */ CM_GLOBAL(volatile u32, 0x9d02b8u) = value;
    /* 006167db */ goto v_00616843;
v_006167dd:
    /* 006167dd */ CM_GLOBAL(volatile u32, 0x9d02b0u) = value;
    /* 006167e3 */ goto v_00616843;
v_006167e5:
    /* 006167e5 */ CM_GLOBAL(volatile u32, 0x9d02a8u) = value;
    /* 006167eb */ goto v_00616843;
v_006167ed:
    /* 006167ed */ CM_GLOBAL(volatile u32, 0xb63ca4u) = value;
    /* 006167f3 */ goto v_00616843;
v_006167f5:
    /* 006167f5 */ CM_GLOBAL(volatile u32, 0xb63cf4u) = value;
    /* 006167fb */ goto v_00616843;
v_006167fd:
    /* 006167fd */ CM_GLOBAL(volatile u32, 0xb63cb8u) = value;
    /* 00616803 */ goto v_00616843;
v_00616805:
    /* 00616805 */ CM_GLOBAL(volatile u32, 0xb63cccu) = value;
    /* 0061680b */ goto v_00616843;
v_0061680d:
    /* 0061680d */ CM_GLOBAL(volatile u32, 0xb63cc8u) = value;
    /* 00616813 */ goto v_00616843;
v_00616815:
    /* 00616815 */ CM_GLOBAL(volatile u32, 0xb63d00u) = value;
    /* 0061681b */ goto v_00616843;
v_0061681d:
    /* 0061681d */ CM_GLOBAL(volatile u32, 0xb63cf8u) = value;
    /* 00616823 */ goto v_00616843;
v_00616825:
    /* 00616825 */ CM_GLOBAL(volatile u32, 0xb63cecu) = value;
    /* 0061682b */ goto v_00616843;
v_0061682d:
    /* 0061682d */ CM_GLOBAL(volatile u32, 0xb63ca8u) = value;
    /* 00616833 */ goto v_00616843;
v_00616835:
    /* 00616835 */ CM_GLOBAL(volatile u32, 0xb63cb0u) = value;
    /* 0061683b */ goto v_00616843;
v_0061683d:
    /* 0061683d */ CM_GLOBAL(volatile u32, 0xb63cd0u) = value;
v_00616843:
    /* 00616849 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0a6cu);
    /* 00616853 */ if (result != 0) goto v_0061685e;
    /* 00616855 */ CM_GLOBAL(volatile u32, 0x9d0448u) = value;
    /* 0061685d */ return result;
v_0061685e:
    /* 00616864 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0a50u);
    /* 0061686e */ if (result == 0) goto v_00616f0c;
    /* 0061687a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0a38u);
    /* 00616884 */ if (result == 0) goto v_00616f0c;
    /* 00616890 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0a24u);
    /* 0061689a */ if (result != 0) goto v_006168a5;
    /* 0061689c */ CM_GLOBAL(volatile u32, 0x9d0450u) = value;
    /* 006168a4 */ return result;
v_006168a5:
    /* 006168ab */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0a08u);
    /* 006168b5 */ if (result == 0) goto v_00616f03;
    /* 006168c1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e09f0u);
    /* 006168cb */ if (result == 0) goto v_00616f03;
    /* 006168d7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e09e0u);
    /* 006168e1 */ if (result != 0) goto v_006168ec;
    /* 006168e3 */ CM_GLOBAL(volatile u32, 0x9d0458u) = value;
    /* 006168eb */ return result;
v_006168ec:
    /* 006168f2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e09c8u);
    /* 006168fc */ if (result != 0) goto v_00616907;
    /* 006168fe */ CM_GLOBAL(volatile u32, 0x9d045cu) = value;
    /* 00616906 */ return result;
v_00616907:
    /* 0061690d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e09bcu);
    /* 00616917 */ if (result != 0) goto v_00616922;
    /* 00616919 */ CM_GLOBAL(volatile u32, 0x9d0460u) = value;
    /* 00616921 */ return result;
v_00616922:
    /* 00616928 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e09a4u);
    /* 00616932 */ if (result != 0) goto v_0061693d;
    /* 00616934 */ CM_GLOBAL(volatile u32, 0x9d0464u) = value;
    /* 0061693c */ return result;
v_0061693d:
    /* 00616943 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0998u);
    /* 0061694d */ if (result != 0) goto v_00616958;
    /* 0061694f */ CM_GLOBAL(volatile u32, 0x9d0468u) = value;
    /* 00616957 */ return result;
v_00616958:
    /* 0061695e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0984u);
    /* 00616968 */ if (result == 0) goto v_00616efa;
    /* 00616974 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0974u);
    /* 0061697e */ if (result == 0) goto v_00616efa;
    /* 0061698a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0960u);
    /* 00616994 */ if (result == 0) goto v_00616ef1;
    /* 006169a0 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e094cu);
    /* 006169aa */ if (result == 0) goto v_00616ef1;
    /* 006169b6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0930u);
    /* 006169c0 */ if (result != 0) goto v_006169cb;
    /* 006169c2 */ CM_GLOBAL(volatile u32, 0x9d0474u) = value;
    /* 006169ca */ return result;
v_006169cb:
    /* 006169d1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0920u);
    /* 006169db */ if (result != 0) goto v_006169e6;
    /* 006169dd */ CM_GLOBAL(volatile u32, 0x9d0478u) = value;
    /* 006169e5 */ return result;
v_006169e6:
    /* 006169ec */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0908u);
    /* 006169f6 */ if (result != 0) goto v_00616a01;
    /* 006169f8 */ CM_GLOBAL(volatile u32, 0x9d047cu) = value;
    /* 00616a00 */ return result;
v_00616a01:
    /* 00616a07 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e08f0u);
    /* 00616a11 */ if (result == 0) goto v_00616ee8;
    /* 00616a1d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e08dcu);
    /* 00616a27 */ if (result == 0) goto v_00616ee8;
    /* 00616a33 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e08c4u);
    /* 00616a3d */ if (result != 0) goto v_00616a48;
    /* 00616a3f */ CM_GLOBAL(volatile u32, 0x9d0484u) = value;
    /* 00616a47 */ return result;
v_00616a48:
    /* 00616a4e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e08b8u);
    /* 00616a58 */ if (result != 0) goto v_00616a63;
    /* 00616a5a */ CM_GLOBAL(volatile u32, 0x9d0488u) = value;
    /* 00616a62 */ return result;
v_00616a63:
    /* 00616a69 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e08a0u);
    /* 00616a73 */ if (result != 0) goto v_00616a7e;
    /* 00616a75 */ CM_GLOBAL(volatile u32, 0x9d048cu) = value;
    /* 00616a7d */ return result;
v_00616a7e:
    /* 00616a84 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e088cu);
    /* 00616a8e */ if (result != 0) goto v_00616a99;
    /* 00616a90 */ CM_GLOBAL(volatile u32, 0x9d0490u) = value;
    /* 00616a98 */ return result;
v_00616a99:
    /* 00616a9f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0870u);
    /* 00616aa9 */ if (result == 0) goto v_00616edf;
    /* 00616ab5 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e085cu);
    /* 00616abf */ if (result == 0) goto v_00616edf;
    /* 00616acb */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e084cu);
    /* 00616ad5 */ if (result != 0) goto v_00616ae0;
    /* 00616ad7 */ CM_GLOBAL(volatile u32, 0x9d0498u) = value;
    /* 00616adf */ return result;
v_00616ae0:
    /* 00616ae6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0834u);
    /* 00616af0 */ if (result == 0) goto v_00616ed6;
    /* 00616afc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0824u);
    /* 00616b06 */ if (result == 0) goto v_00616ed6;
    /* 00616b12 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0810u);
    /* 00616b1c */ if (result != 0) goto v_00616b27;
    /* 00616b1e */ CM_GLOBAL(volatile u32, 0x9d04a0u) = value;
    /* 00616b26 */ return result;
v_00616b27:
    /* 00616b2d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e07f4u);
    /* 00616b37 */ if (result == 0) goto v_00616ecd;
    /* 00616b43 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e07dcu);
    /* 00616b4d */ if (result == 0) goto v_00616ecd;
    /* 00616b59 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e07d0u);
    /* 00616b63 */ if (result != 0) goto v_00616b6e;
    /* 00616b65 */ CM_GLOBAL(volatile u32, 0x9d04a8u) = value;
    /* 00616b6d */ return result;
v_00616b6e:
    /* 00616b74 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e07b8u);
    /* 00616b7e */ if (result == 0) goto v_00616ec4;
    /* 00616b8a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e07a8u);
    /* 00616b94 */ if (result == 0) goto v_00616ec4;
    /* 00616ba0 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0798u);
    /* 00616baa */ if (result != 0) goto v_00616bb5;
    /* 00616bac */ CM_GLOBAL(volatile u32, 0x9d04b0u) = value;
    /* 00616bb4 */ return result;
v_00616bb5:
    /* 00616bbb */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0780u);
    /* 00616bc5 */ if (result != 0) goto v_00616bd0;
    /* 00616bc7 */ CM_GLOBAL(volatile u32, 0x9d04b4u) = value;
    /* 00616bcf */ return result;
v_00616bd0:
    /* 00616bd6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0770u);
    /* 00616be0 */ if (result != 0) goto v_00616beb;
    /* 00616be2 */ CM_GLOBAL(volatile u32, 0x9d04b8u) = value;
    /* 00616bea */ return result;
v_00616beb:
    /* 00616bf1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e075cu);
    /* 00616bfb */ if (result != 0) goto v_00616c06;
    /* 00616bfd */ CM_GLOBAL(volatile u32, 0x9d04bcu) = value;
    /* 00616c05 */ return result;
v_00616c06:
    /* 00616c0c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e29cu);
    /* 00616c16 */ if (result != 0) goto v_00616c21;
    /* 00616c18 */ CM_GLOBAL(volatile u32, 0x9d04c0u) = value;
    /* 00616c20 */ return result;
v_00616c21:
    /* 00616c27 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0ca8u);
    /* 00616c31 */ if (result != 0) goto v_00616c3c;
    /* 00616c33 */ CM_GLOBAL(volatile u32, 0x9d04c4u) = value;
    /* 00616c3b */ return result;
v_00616c3c:
    /* 00616c42 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1bf8u);
    /* 00616c4c */ if (result != 0) goto v_00616c57;
    /* 00616c4e */ CM_GLOBAL(volatile u32, 0x9d04c8u) = value;
    /* 00616c56 */ return result;
v_00616c57:
    /* 00616c5d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e1be8u);
    /* 00616c67 */ if (result != 0) goto v_00616c72;
    /* 00616c69 */ CM_GLOBAL(volatile u32, 0x9d04ccu) = value;
    /* 00616c71 */ return result;
v_00616c72:
    /* 00616c78 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e074cu);
    /* 00616c82 */ if (result != 0) goto v_00616c8d;
    /* 00616c84 */ CM_GLOBAL(volatile u32, 0x9d04d0u) = value;
    /* 00616c8c */ return result;
v_00616c8d:
    /* 00616c93 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e073cu);
    /* 00616c9d */ if (result != 0) goto v_00616ca8;
    /* 00616c9f */ CM_GLOBAL(volatile u32, 0x9d04d4u) = value;
    /* 00616ca7 */ return result;
v_00616ca8:
    /* 00616cae */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e268u);
    /* 00616cb8 */ if (result != 0) goto v_00616cc3;
    /* 00616cba */ CM_GLOBAL(volatile u32, 0x9d04d8u) = value;
    /* 00616cc2 */ return result;
v_00616cc3:
    /* 00616cc9 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e072cu);
    /* 00616cd3 */ if (result != 0) goto v_00616cde;
    /* 00616cd5 */ CM_GLOBAL(volatile u32, 0x9d04dcu) = value;
    /* 00616cdd */ return result;
v_00616cde:
    /* 00616ce4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0718u);
    /* 00616cee */ if (result != 0) goto v_00616cf9;
    /* 00616cf0 */ CM_GLOBAL(volatile u32, 0x9d04e0u) = value;
    /* 00616cf8 */ return result;
v_00616cf9:
    /* 00616cff */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0704u);
    /* 00616d09 */ if (result != 0) goto v_00616d14;
    /* 00616d0b */ CM_GLOBAL(volatile u32, 0x9d04e4u) = value;
    /* 00616d13 */ return result;
v_00616d14:
    /* 00616d1a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e218u);
    /* 00616d24 */ if (result != 0) goto v_00616d2f;
    /* 00616d26 */ CM_GLOBAL(volatile u32, 0x9d04e8u) = value;
    /* 00616d2e */ return result;
v_00616d2f:
    /* 00616d35 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e06f4u);
    /* 00616d3f */ if (result != 0) goto v_00616d4a;
    /* 00616d41 */ CM_GLOBAL(volatile u32, 0x9d04ecu) = value;
    /* 00616d49 */ return result;
v_00616d4a:
    /* 00616d50 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e180u);
    /* 00616d5a */ if (result != 0) goto v_00616d65;
    /* 00616d5c */ CM_GLOBAL(volatile u32, 0x9d04f0u) = value;
    /* 00616d64 */ return result;
v_00616d65:
    /* 00616d6b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e06e4u);
    /* 00616d75 */ if (result != 0) goto v_00616d80;
    /* 00616d77 */ CM_GLOBAL(volatile u32, 0x9d04f4u) = value;
    /* 00616d7f */ return result;
v_00616d80:
    /* 00616d86 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e06d0u);
    /* 00616d90 */ if (result != 0) goto v_00616d9b;
    /* 00616d92 */ CM_GLOBAL(volatile u32, 0x9d04f8u) = value;
    /* 00616d9a */ return result;
v_00616d9b:
    /* 00616da1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e06b8u);
    /* 00616dab */ if (result != 0) goto v_00616db6;
    /* 00616dad */ CM_GLOBAL(volatile u32, 0x9d04fcu) = value;
    /* 00616db5 */ return result;
v_00616db6:
    /* 00616dbc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x98e0acu);
    /* 00616dc6 */ if (result != 0) goto v_00616dd1;
    /* 00616dc8 */ CM_GLOBAL(volatile u32, 0x9d0500u) = value;
    /* 00616dd0 */ return result;
v_00616dd1:
    /* 00616dd7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e06a0u);
    /* 00616de1 */ if (result != 0) goto v_00616dec;
    /* 00616de3 */ CM_GLOBAL(volatile u32, 0x9d0504u) = value;
    /* 00616deb */ return result;
v_00616dec:
    /* 00616df2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e068cu);
    /* 00616dfc */ if (result != 0) goto v_00616e07;
    /* 00616dfe */ CM_GLOBAL(volatile u32, 0x9d0508u) = value;
    /* 00616e06 */ return result;
v_00616e07:
    /* 00616e0d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0674u);
    /* 00616e17 */ if (result != 0) goto v_00616e22;
    /* 00616e19 */ CM_GLOBAL(volatile u32, 0x9d050cu) = value;
    /* 00616e21 */ return result;
v_00616e22:
    /* 00616e28 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0668u);
    /* 00616e32 */ if (result != 0) goto v_00616e3d;
    /* 00616e34 */ CM_GLOBAL(volatile u32, 0x9d0510u) = value;
    /* 00616e3c */ return result;
v_00616e3d:
    /* 00616e43 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0658u);
    /* 00616e4d */ if (result != 0) goto v_00616e58;
    /* 00616e4f */ CM_GLOBAL(volatile u32, 0x9d0514u) = value;
    /* 00616e57 */ return result;
v_00616e58:
    /* 00616e5e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0648u);
    /* 00616e68 */ if (result != 0) goto v_00616e73;
    /* 00616e6a */ CM_GLOBAL(volatile u32, 0x9d0518u) = value;
    /* 00616e72 */ return result;
v_00616e73:
    /* 00616e79 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0638u);
    /* 00616e83 */ if (result != 0) goto v_00616e8e;
    /* 00616e85 */ CM_GLOBAL(volatile u32, 0x9d051cu) = value;
    /* 00616e8d */ return result;
v_00616e8e:
    /* 00616e94 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0624u);
    /* 00616e9e */ if (result != 0) goto v_00616ea9;
    /* 00616ea0 */ CM_GLOBAL(volatile u32, 0x9d0520u) = value;
    /* 00616ea8 */ return result;
v_00616ea9:
    /* 00616eaf */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e060cu);
    /* 00616eb9 */ if (result != 0) goto v_00616f12;
    /* 00616ebb */ CM_GLOBAL(volatile u32, 0x9d0524u) = value;
    /* 00616ec3 */ return result;
v_00616ec4:
    /* 00616ec4 */ CM_GLOBAL(volatile u32, 0x9d04acu) = value;
    /* 00616ecc */ return result;
v_00616ecd:
    /* 00616ecd */ CM_GLOBAL(volatile u32, 0x9d04a4u) = value;
    /* 00616ed5 */ return result;
v_00616ed6:
    /* 00616ed6 */ CM_GLOBAL(volatile u32, 0x9d049cu) = value;
    /* 00616ede */ return result;
v_00616edf:
    /* 00616edf */ CM_GLOBAL(volatile u32, 0x9d0494u) = value;
    /* 00616ee7 */ return result;
v_00616ee8:
    /* 00616ee8 */ CM_GLOBAL(volatile u32, 0x9d0480u) = value;
    /* 00616ef0 */ return result;
v_00616ef1:
    /* 00616ef1 */ CM_GLOBAL(volatile u32, 0x9d0470u) = value;
    /* 00616ef9 */ return result;
v_00616efa:
    /* 00616efa */ CM_GLOBAL(volatile u32, 0x9d046cu) = value;
    /* 00616f02 */ return result;
v_00616f03:
    /* 00616f03 */ CM_GLOBAL(volatile u32, 0x9d0454u) = value;
    /* 00616f0b */ return result;
v_00616f0c:
    /* 00616f0c */ CM_GLOBAL(volatile u32, 0x9d044cu) = value;
v_00616f12:
    /* 00616f14 */ return result;
}
