/* Preferred-image VAs; base 00400000. CM0102 3.9.68 SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * Five consecutive comparison groups; shared stores and continuation order
 * follow the native branches. The original comparator remains a boundary.
 * All indexed callers discard EAX; retain the last full comparator result.
 */
#include "cmre.h"

extern s32 CM_CDECL cm_fn_00951c50(const volatile u8 *first, const volatile u8 *second);

s32 CM_CDECL cm_fn_006115e0(const volatile u8 *name, u32 identifier)
{
    s32 result;
    u32 value;
    /* 006115ec */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e05ecu);
    /* 006115f1 */ value = identifier;
    /* 006115fa */ if (result != 0) goto v_00611607;
    /* 006115fc */ CM_GLOBAL(volatile u32, 0x9cfa20u) = value;
    /* 00611602 */ goto v_006123f0;
v_00611607:
    /* 0061160d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e05ccu);
    /* 00611617 */ if (result != 0) goto v_00611624;
    /* 00611619 */ CM_GLOBAL(volatile u32, 0x9cfa24u) = value;
    /* 0061161f */ goto v_006123f0;
v_00611624:
    /* 0061162a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e05acu);
    /* 00611634 */ if (result != 0) goto v_00611641;
    /* 00611636 */ CM_GLOBAL(volatile u32, 0x9cfa28u) = value;
    /* 0061163c */ goto v_006123f0;
v_00611641:
    /* 00611647 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0584u);
    /* 00611651 */ if (result != 0) goto v_0061165e;
    /* 00611653 */ CM_GLOBAL(volatile u32, 0x9cfa2cu) = value;
    /* 00611659 */ goto v_006123f0;
v_0061165e:
    /* 00611664 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0564u);
    /* 0061166e */ if (result != 0) goto v_0061167b;
    /* 00611670 */ CM_GLOBAL(volatile u32, 0x9cfa30u) = value;
    /* 00611676 */ goto v_006123f0;
v_0061167b:
    /* 00611681 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0540u);
    /* 0061168b */ if (result != 0) goto v_00611698;
    /* 0061168d */ CM_GLOBAL(volatile u32, 0x9cfa34u) = value;
    /* 00611693 */ goto v_006123f0;
v_00611698:
    /* 0061169e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0514u);
    /* 006116a8 */ if (result != 0) goto v_006116b5;
    /* 006116aa */ CM_GLOBAL(volatile u32, 0x9cfa38u) = value;
    /* 006116b0 */ goto v_006123f0;
v_006116b5:
    /* 006116bb */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e04ecu);
    /* 006116c5 */ if (result != 0) goto v_006116d2;
    /* 006116c7 */ CM_GLOBAL(volatile u32, 0x9cfa3cu) = value;
    /* 006116cd */ goto v_006123f0;
v_006116d2:
    /* 006116d8 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e04c4u);
    /* 006116e2 */ if (result != 0) goto v_006116ef;
    /* 006116e4 */ CM_GLOBAL(volatile u32, 0x9cfa40u) = value;
    /* 006116ea */ goto v_006123f0;
v_006116ef:
    /* 006116f5 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e049cu);
    /* 006116ff */ if (result != 0) goto v_0061170c;
    /* 00611701 */ CM_GLOBAL(volatile u32, 0x9cfa44u) = value;
    /* 00611707 */ goto v_006123f0;
v_0061170c:
    /* 00611712 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0474u);
    /* 0061171c */ if (result != 0) goto v_00611729;
    /* 0061171e */ CM_GLOBAL(volatile u32, 0x9cfa48u) = value;
    /* 00611724 */ goto v_006123f0;
v_00611729:
    /* 0061172f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0444u);
    /* 00611739 */ if (result != 0) goto v_00611746;
    /* 0061173b */ CM_GLOBAL(volatile u32, 0x9cfa4cu) = value;
    /* 00611741 */ goto v_006123f0;
v_00611746:
    /* 0061174c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0418u);
    /* 00611756 */ if (result != 0) goto v_00611763;
    /* 00611758 */ CM_GLOBAL(volatile u32, 0x9cfa50u) = value;
    /* 0061175e */ goto v_006123f0;
v_00611763:
    /* 00611769 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e03ecu);
    /* 00611773 */ if (result != 0) goto v_00611780;
    /* 00611775 */ CM_GLOBAL(volatile u32, 0x9cfa54u) = value;
    /* 0061177b */ goto v_006123f0;
v_00611780:
    /* 00611786 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e03c0u);
    /* 00611790 */ if (result != 0) goto v_0061179d;
    /* 00611792 */ CM_GLOBAL(volatile u32, 0x9cfa58u) = value;
    /* 00611798 */ goto v_006123f0;
v_0061179d:
    /* 006117a3 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e038cu);
    /* 006117ad */ if (result != 0) goto v_006117ba;
    /* 006117af */ CM_GLOBAL(volatile u32, 0x9cfa5cu) = value;
    /* 006117b5 */ goto v_006123f0;
v_006117ba:
    /* 006117c0 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0358u);
    /* 006117ca */ if (result != 0) goto v_006117d7;
    /* 006117cc */ CM_GLOBAL(volatile u32, 0x9cfa60u) = value;
    /* 006117d2 */ goto v_006123f0;
v_006117d7:
    /* 006117dd */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0324u);
    /* 006117e7 */ if (result != 0) goto v_006117f4;
    /* 006117e9 */ CM_GLOBAL(volatile u32, 0x9cfa64u) = value;
    /* 006117ef */ goto v_006123f0;
v_006117f4:
    /* 006117fa */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e02f0u);
    /* 00611804 */ if (result != 0) goto v_00611811;
    /* 00611806 */ CM_GLOBAL(volatile u32, 0x9cfa68u) = value;
    /* 0061180c */ goto v_006123f0;
v_00611811:
    /* 00611817 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e02c4u);
    /* 00611821 */ if (result != 0) goto v_0061182e;
    /* 00611823 */ CM_GLOBAL(volatile u32, 0x9cfa6cu) = value;
    /* 00611829 */ goto v_006123f0;
v_0061182e:
    /* 00611834 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e029cu);
    /* 0061183e */ if (result != 0) goto v_0061184b;
    /* 00611840 */ CM_GLOBAL(volatile u32, 0x9cfa70u) = value;
    /* 00611846 */ goto v_006123f0;
v_0061184b:
    /* 00611851 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0270u);
    /* 0061185b */ if (result != 0) goto v_00611868;
    /* 0061185d */ CM_GLOBAL(volatile u32, 0x9cfa74u) = value;
    /* 00611863 */ goto v_006123f0;
v_00611868:
    /* 0061186e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0248u);
    /* 00611878 */ if (result != 0) goto v_00611885;
    /* 0061187a */ CM_GLOBAL(volatile u32, 0x9cfa78u) = value;
    /* 00611880 */ goto v_006123f0;
v_00611885:
    /* 0061188b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e022cu);
    /* 00611895 */ if (result != 0) goto v_006118a2;
    /* 00611897 */ CM_GLOBAL(volatile u32, 0x9cfa7cu) = value;
    /* 0061189d */ goto v_006123f0;
v_006118a2:
    /* 006118a8 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0208u);
    /* 006118b2 */ if (result != 0) goto v_006118bf;
    /* 006118b4 */ CM_GLOBAL(volatile u32, 0x9cfa80u) = value;
    /* 006118ba */ goto v_006123f0;
v_006118bf:
    /* 006118c5 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e01d8u);
    /* 006118cf */ if (result != 0) goto v_006118dc;
    /* 006118d1 */ CM_GLOBAL(volatile u32, 0x9cfa84u) = value;
    /* 006118d7 */ goto v_006123f0;
v_006118dc:
    /* 006118e2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e01acu);
    /* 006118ec */ if (result != 0) goto v_006118f9;
    /* 006118ee */ CM_GLOBAL(volatile u32, 0x9cfa88u) = value;
    /* 006118f4 */ goto v_006123f0;
v_006118f9:
    /* 006118ff */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e017cu);
    /* 00611909 */ if (result != 0) goto v_00611916;
    /* 0061190b */ CM_GLOBAL(volatile u32, 0x9cfa8cu) = value;
    /* 00611911 */ goto v_006123f0;
v_00611916:
    /* 0061191c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0150u);
    /* 00611926 */ if (result != 0) goto v_00611933;
    /* 00611928 */ CM_GLOBAL(volatile u32, 0x9cfa90u) = value;
    /* 0061192e */ goto v_006123f0;
v_00611933:
    /* 00611939 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0124u);
    /* 00611943 */ if (result != 0) goto v_00611950;
    /* 00611945 */ CM_GLOBAL(volatile u32, 0x9cfa94u) = value;
    /* 0061194b */ goto v_006123f0;
v_00611950:
    /* 00611956 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e00f8u);
    /* 00611960 */ if (result != 0) goto v_0061196d;
    /* 00611962 */ CM_GLOBAL(volatile u32, 0x9cfa98u) = value;
    /* 00611968 */ goto v_006123f0;
v_0061196d:
    /* 00611973 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e00ccu);
    /* 0061197d */ if (result != 0) goto v_0061198a;
    /* 0061197f */ CM_GLOBAL(volatile u32, 0x9cfa9cu) = value;
    /* 00611985 */ goto v_006123f0;
v_0061198a:
    /* 00611990 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e00a0u);
    /* 0061199a */ if (result != 0) goto v_006119a7;
    /* 0061199c */ CM_GLOBAL(volatile u32, 0x9cfaa0u) = value;
    /* 006119a2 */ goto v_006123f0;
v_006119a7:
    /* 006119ad */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e008cu);
    /* 006119b7 */ if (result != 0) goto v_006119c4;
    /* 006119b9 */ CM_GLOBAL(volatile u32, 0x9cfaa4u) = value;
    /* 006119bf */ goto v_006123f0;
v_006119c4:
    /* 006119ca */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0060u);
    /* 006119d4 */ if (result != 0) goto v_006119e1;
    /* 006119d6 */ CM_GLOBAL(volatile u32, 0x9cfaa8u) = value;
    /* 006119dc */ goto v_006123f0;
v_006119e1:
    /* 006119e7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0030u);
    /* 006119f1 */ if (result != 0) goto v_006119fe;
    /* 006119f3 */ CM_GLOBAL(volatile u32, 0x9cfaacu) = value;
    /* 006119f9 */ goto v_006123f0;
v_006119fe:
    /* 00611a04 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9e0004u);
    /* 00611a0e */ if (result != 0) goto v_00611a1b;
    /* 00611a10 */ CM_GLOBAL(volatile u32, 0x9cfab0u) = value;
    /* 00611a16 */ goto v_006123f0;
v_00611a1b:
    /* 00611a21 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dffe8u);
    /* 00611a2b */ if (result != 0) goto v_00611a38;
    /* 00611a2d */ CM_GLOBAL(volatile u32, 0x9cfab4u) = value;
    /* 00611a33 */ goto v_006123f0;
v_00611a38:
    /* 00611a3e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dffbcu);
    /* 00611a48 */ if (result != 0) goto v_00611a55;
    /* 00611a4a */ CM_GLOBAL(volatile u32, 0x9cfab8u) = value;
    /* 00611a50 */ goto v_006123f0;
v_00611a55:
    /* 00611a5b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dff88u);
    /* 00611a65 */ if (result != 0) goto v_00611a72;
    /* 00611a67 */ CM_GLOBAL(volatile u32, 0x9cfabcu) = value;
    /* 00611a6d */ goto v_006123f0;
v_00611a72:
    /* 00611a78 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dff5cu);
    /* 00611a82 */ if (result != 0) goto v_00611a8f;
    /* 00611a84 */ CM_GLOBAL(volatile u32, 0x9cfac0u) = value;
    /* 00611a8a */ goto v_006123f0;
v_00611a8f:
    /* 00611a95 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dff2cu);
    /* 00611a9f */ if (result != 0) goto v_00611aac;
    /* 00611aa1 */ CM_GLOBAL(volatile u32, 0x9cfac4u) = value;
    /* 00611aa7 */ goto v_006123f0;
v_00611aac:
    /* 00611ab2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfefcu);
    /* 00611abc */ if (result != 0) goto v_00611ac9;
    /* 00611abe */ CM_GLOBAL(volatile u32, 0x9cfac8u) = value;
    /* 00611ac4 */ goto v_006123f0;
v_00611ac9:
    /* 00611acf */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfed0u);
    /* 00611ad9 */ if (result != 0) goto v_00611ae6;
    /* 00611adb */ CM_GLOBAL(volatile u32, 0x9cfaccu) = value;
    /* 00611ae1 */ goto v_006123f0;
v_00611ae6:
    /* 00611aec */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfe9cu);
    /* 00611af6 */ if (result != 0) goto v_00611b03;
    /* 00611af8 */ CM_GLOBAL(volatile u32, 0x9cfad0u) = value;
    /* 00611afe */ goto v_006123f0;
v_00611b03:
    /* 00611b09 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfe70u);
    /* 00611b13 */ if (result != 0) goto v_00611b20;
    /* 00611b15 */ CM_GLOBAL(volatile u32, 0x9cfad4u) = value;
    /* 00611b1b */ goto v_006123f0;
v_00611b20:
    /* 00611b26 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfe54u);
    /* 00611b30 */ if (result != 0) goto v_00611b3d;
    /* 00611b32 */ CM_GLOBAL(volatile u32, 0x9cfad8u) = value;
    /* 00611b38 */ goto v_006123f0;
v_00611b3d:
    /* 00611b43 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfe30u);
    /* 00611b4d */ if (result != 0) goto v_00611b5a;
    /* 00611b4f */ CM_GLOBAL(volatile u32, 0x9cfadcu) = value;
    /* 00611b55 */ goto v_006123f0;
v_00611b5a:
    /* 00611b60 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfe0cu);
    /* 00611b6a */ if (result != 0) goto v_00611b77;
    /* 00611b6c */ CM_GLOBAL(volatile u32, 0x9cfae0u) = value;
    /* 00611b72 */ goto v_006123f0;
v_00611b77:
    /* 00611b7d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfde4u);
    /* 00611b87 */ if (result != 0) goto v_00611b94;
    /* 00611b89 */ CM_GLOBAL(volatile u32, 0x9cfae4u) = value;
    /* 00611b8f */ goto v_006123f0;
v_00611b94:
    /* 00611b9a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfdb8u);
    /* 00611ba4 */ if (result != 0) goto v_00611bb1;
    /* 00611ba6 */ CM_GLOBAL(volatile u32, 0x9cfae8u) = value;
    /* 00611bac */ goto v_006123f0;
v_00611bb1:
    /* 00611bb7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfd84u);
    /* 00611bc1 */ if (result != 0) goto v_00611bce;
    /* 00611bc3 */ CM_GLOBAL(volatile u32, 0x9cfaecu) = value;
    /* 00611bc9 */ goto v_006123f0;
v_00611bce:
    /* 00611bd4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfd58u);
    /* 00611bde */ if (result != 0) goto v_00611beb;
    /* 00611be0 */ CM_GLOBAL(volatile u32, 0x9cfaf0u) = value;
    /* 00611be6 */ goto v_006123f0;
v_00611beb:
    /* 00611bf1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfd28u);
    /* 00611bfb */ if (result != 0) goto v_00611c08;
    /* 00611bfd */ CM_GLOBAL(volatile u32, 0x9cfaf4u) = value;
    /* 00611c03 */ goto v_006123f0;
v_00611c08:
    /* 00611c0e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfcf4u);
    /* 00611c18 */ if (result != 0) goto v_00611c25;
    /* 00611c1a */ CM_GLOBAL(volatile u32, 0x9cfaf8u) = value;
    /* 00611c20 */ goto v_006123f0;
v_00611c25:
    /* 00611c2b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfcc8u);
    /* 00611c35 */ if (result != 0) goto v_00611c42;
    /* 00611c37 */ CM_GLOBAL(volatile u32, 0x9cfafcu) = value;
    /* 00611c3d */ goto v_006123f0;
v_00611c42:
    /* 00611c48 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfc98u);
    /* 00611c52 */ if (result != 0) goto v_00611c5f;
    /* 00611c54 */ CM_GLOBAL(volatile u32, 0x9cfb00u) = value;
    /* 00611c5a */ goto v_006123f0;
v_00611c5f:
    /* 00611c65 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfc7cu);
    /* 00611c6f */ if (result != 0) goto v_00611c7c;
    /* 00611c71 */ CM_GLOBAL(volatile u32, 0x9cfb04u) = value;
    /* 00611c77 */ goto v_006123f0;
v_00611c7c:
    /* 00611c82 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfc50u);
    /* 00611c8c */ if (result != 0) goto v_00611c99;
    /* 00611c8e */ CM_GLOBAL(volatile u32, 0x9cfb08u) = value;
    /* 00611c94 */ goto v_006123f0;
v_00611c99:
    /* 00611c9f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfc24u);
    /* 00611ca9 */ if (result != 0) goto v_00611cb6;
    /* 00611cab */ CM_GLOBAL(volatile u32, 0x9cfb0cu) = value;
    /* 00611cb1 */ goto v_006123f0;
v_00611cb6:
    /* 00611cbc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfc04u);
    /* 00611cc6 */ if (result != 0) goto v_00611cd3;
    /* 00611cc8 */ CM_GLOBAL(volatile u32, 0x9cfb10u) = value;
    /* 00611cce */ goto v_006123f0;
v_00611cd3:
    /* 00611cd9 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfbe4u);
    /* 00611ce3 */ if (result != 0) goto v_00611cf0;
    /* 00611ce5 */ CM_GLOBAL(volatile u32, 0x9cfb14u) = value;
    /* 00611ceb */ goto v_006123f0;
v_00611cf0:
    /* 00611cf6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfbb8u);
    /* 00611d00 */ if (result != 0) goto v_00611d0d;
    /* 00611d02 */ CM_GLOBAL(volatile u32, 0x9cfb18u) = value;
    /* 00611d08 */ goto v_006123f0;
v_00611d0d:
    /* 00611d13 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfb90u);
    /* 00611d1d */ if (result != 0) goto v_00611d2a;
    /* 00611d1f */ CM_GLOBAL(volatile u32, 0x9cfb1cu) = value;
    /* 00611d25 */ goto v_006123f0;
v_00611d2a:
    /* 00611d30 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfb6cu);
    /* 00611d3a */ if (result != 0) goto v_00611d47;
    /* 00611d3c */ CM_GLOBAL(volatile u32, 0x9cfb20u) = value;
    /* 00611d42 */ goto v_006123f0;
v_00611d47:
    /* 00611d4d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfb44u);
    /* 00611d57 */ if (result != 0) goto v_00611d64;
    /* 00611d59 */ CM_GLOBAL(volatile u32, 0x9cfb24u) = value;
    /* 00611d5f */ goto v_006123f0;
v_00611d64:
    /* 00611d6a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfb20u);
    /* 00611d74 */ if (result != 0) goto v_00611d81;
    /* 00611d76 */ CM_GLOBAL(volatile u32, 0x9cfb28u) = value;
    /* 00611d7c */ goto v_006123f0;
v_00611d81:
    /* 00611d87 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfafcu);
    /* 00611d91 */ if (result != 0) goto v_00611d9e;
    /* 00611d93 */ CM_GLOBAL(volatile u32, 0x9cfb2cu) = value;
    /* 00611d99 */ goto v_006123f0;
v_00611d9e:
    /* 00611da4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfad0u);
    /* 00611dae */ if (result != 0) goto v_00611dbb;
    /* 00611db0 */ CM_GLOBAL(volatile u32, 0x9cfb30u) = value;
    /* 00611db6 */ goto v_006123f0;
v_00611dbb:
    /* 00611dc1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfaacu);
    /* 00611dcb */ if (result != 0) goto v_00611dd8;
    /* 00611dcd */ CM_GLOBAL(volatile u32, 0x9cfb34u) = value;
    /* 00611dd3 */ goto v_006123f0;
v_00611dd8:
    /* 00611dde */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfa80u);
    /* 00611de8 */ if (result != 0) goto v_00611df5;
    /* 00611dea */ CM_GLOBAL(volatile u32, 0x9cfb38u) = value;
    /* 00611df0 */ goto v_006123f0;
v_00611df5:
    /* 00611dfb */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfa54u);
    /* 00611e05 */ if (result != 0) goto v_00611e12;
    /* 00611e07 */ CM_GLOBAL(volatile u32, 0x9cfb3cu) = value;
    /* 00611e0d */ goto v_006123f0;
v_00611e12:
    /* 00611e18 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfa2cu);
    /* 00611e22 */ if (result != 0) goto v_00611e2f;
    /* 00611e24 */ CM_GLOBAL(volatile u32, 0x9cfb40u) = value;
    /* 00611e2a */ goto v_006123f0;
v_00611e2f:
    /* 00611e35 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dfa04u);
    /* 00611e3f */ if (result != 0) goto v_00611e4c;
    /* 00611e41 */ CM_GLOBAL(volatile u32, 0x9cfb44u) = value;
    /* 00611e47 */ goto v_006123f0;
v_00611e4c:
    /* 00611e52 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df9e0u);
    /* 00611e5c */ if (result != 0) goto v_00611e69;
    /* 00611e5e */ CM_GLOBAL(volatile u32, 0x9cfb48u) = value;
    /* 00611e64 */ goto v_006123f0;
v_00611e69:
    /* 00611e6f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df9b8u);
    /* 00611e79 */ if (result != 0) goto v_00611e86;
    /* 00611e7b */ CM_GLOBAL(volatile u32, 0x9cfb4cu) = value;
    /* 00611e81 */ goto v_006123f0;
v_00611e86:
    /* 00611e8c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df990u);
    /* 00611e96 */ if (result != 0) goto v_00611ea3;
    /* 00611e98 */ CM_GLOBAL(volatile u32, 0x9cfb50u) = value;
    /* 00611e9e */ goto v_006123f0;
v_00611ea3:
    /* 00611ea9 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df970u);
    /* 00611eb3 */ if (result != 0) goto v_00611ec0;
    /* 00611eb5 */ CM_GLOBAL(volatile u32, 0x9cfb54u) = value;
    /* 00611ebb */ goto v_006123f0;
v_00611ec0:
    /* 00611ec6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df94cu);
    /* 00611ed0 */ if (result != 0) goto v_00611edd;
    /* 00611ed2 */ CM_GLOBAL(volatile u32, 0x9cfb58u) = value;
    /* 00611ed8 */ goto v_006123f0;
v_00611edd:
    /* 00611ee3 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df92cu);
    /* 00611eed */ if (result != 0) goto v_00611efa;
    /* 00611eef */ CM_GLOBAL(volatile u32, 0x9cfb5cu) = value;
    /* 00611ef5 */ goto v_006123f0;
v_00611efa:
    /* 00611f00 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df908u);
    /* 00611f0a */ if (result != 0) goto v_00611f17;
    /* 00611f0c */ CM_GLOBAL(volatile u32, 0x9cfb60u) = value;
    /* 00611f12 */ goto v_006123f0;
v_00611f17:
    /* 00611f1d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df8e8u);
    /* 00611f27 */ if (result != 0) goto v_00611f34;
    /* 00611f29 */ CM_GLOBAL(volatile u32, 0x9cfb64u) = value;
    /* 00611f2f */ goto v_006123f0;
v_00611f34:
    /* 00611f3a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df8bcu);
    /* 00611f44 */ if (result != 0) goto v_00611f51;
    /* 00611f46 */ CM_GLOBAL(volatile u32, 0x9cfb68u) = value;
    /* 00611f4c */ goto v_006123f0;
v_00611f51:
    /* 00611f57 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df890u);
    /* 00611f61 */ if (result != 0) goto v_00611f6e;
    /* 00611f63 */ CM_GLOBAL(volatile u32, 0x9cfb6cu) = value;
    /* 00611f69 */ goto v_006123f0;
v_00611f6e:
    /* 00611f74 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df85cu);
    /* 00611f7e */ if (result == 0) goto v_006123ea;
    /* 00611f8a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df82cu);
    /* 00611f94 */ if (result == 0) goto v_006123ea;
    /* 00611fa0 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df800u);
    /* 00611faa */ if (result == 0) goto v_006123e2;
    /* 00611fb6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df7d4u);
    /* 00611fc0 */ if (result == 0) goto v_006123e2;
    /* 00611fcc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df7a4u);
    /* 00611fd6 */ if (result == 0) goto v_006123da;
    /* 00611fe2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df778u);
    /* 00611fec */ if (result == 0) goto v_006123da;
    /* 00611ff8 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df748u);
    /* 00612002 */ if (result != 0) goto v_0061200f;
    /* 00612004 */ CM_GLOBAL(volatile u32, 0x9cfb7cu) = value;
    /* 0061200a */ goto v_006123f0;
v_0061200f:
    /* 00612015 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df72cu);
    /* 0061201f */ if (result != 0) goto v_0061202c;
    /* 00612021 */ CM_GLOBAL(volatile u32, 0x9cfb80u) = value;
    /* 00612027 */ goto v_006123f0;
v_0061202c:
    /* 00612032 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df704u);
    /* 0061203c */ if (result != 0) goto v_00612049;
    /* 0061203e */ CM_GLOBAL(volatile u32, 0x9cfb84u) = value;
    /* 00612044 */ goto v_006123f0;
v_00612049:
    /* 0061204f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df6e8u);
    /* 00612059 */ if (result != 0) goto v_00612066;
    /* 0061205b */ CM_GLOBAL(volatile u32, 0x9cfb88u) = value;
    /* 00612061 */ goto v_006123f0;
v_00612066:
    /* 0061206c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df6c0u);
    /* 00612076 */ if (result != 0) goto v_00612083;
    /* 00612078 */ CM_GLOBAL(volatile u32, 0x9cfb8cu) = value;
    /* 0061207e */ goto v_006123f0;
v_00612083:
    /* 00612089 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df6a0u);
    /* 00612093 */ if (result != 0) goto v_006120a0;
    /* 00612095 */ CM_GLOBAL(volatile u32, 0x9cfb90u) = value;
    /* 0061209b */ goto v_006123f0;
v_006120a0:
    /* 006120a6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df684u);
    /* 006120b0 */ if (result != 0) goto v_006120bd;
    /* 006120b2 */ CM_GLOBAL(volatile u32, 0x9cfb94u) = value;
    /* 006120b8 */ goto v_006123f0;
v_006120bd:
    /* 006120c3 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df668u);
    /* 006120cd */ if (result != 0) goto v_006120da;
    /* 006120cf */ CM_GLOBAL(volatile u32, 0x9cfb98u) = value;
    /* 006120d5 */ goto v_006123f0;
v_006120da:
    /* 006120e0 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df650u);
    /* 006120ea */ if (result != 0) goto v_006120f7;
    /* 006120ec */ CM_GLOBAL(volatile u32, 0x9cfb9cu) = value;
    /* 006120f2 */ goto v_006123f0;
v_006120f7:
    /* 006120fd */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df638u);
    /* 00612107 */ if (result != 0) goto v_00612114;
    /* 00612109 */ CM_GLOBAL(volatile u32, 0x9cfba0u) = value;
    /* 0061210f */ goto v_006123f0;
v_00612114:
    /* 0061211a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df624u);
    /* 00612124 */ if (result != 0) goto v_00612131;
    /* 00612126 */ CM_GLOBAL(volatile u32, 0x9cfba4u) = value;
    /* 0061212c */ goto v_006123f0;
v_00612131:
    /* 00612137 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df60cu);
    /* 00612141 */ if (result != 0) goto v_0061214e;
    /* 00612143 */ CM_GLOBAL(volatile u32, 0x9cfba8u) = value;
    /* 00612149 */ goto v_006123f0;
v_0061214e:
    /* 00612154 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df5e0u);
    /* 0061215e */ if (result != 0) goto v_0061216b;
    /* 00612160 */ CM_GLOBAL(volatile u32, 0x9cfbacu) = value;
    /* 00612166 */ goto v_006123f0;
v_0061216b:
    /* 00612171 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df5b4u);
    /* 0061217b */ if (result != 0) goto v_00612188;
    /* 0061217d */ CM_GLOBAL(volatile u32, 0x9cfbb0u) = value;
    /* 00612183 */ goto v_006123f0;
v_00612188:
    /* 0061218e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df588u);
    /* 00612198 */ if (result != 0) goto v_006121a5;
    /* 0061219a */ CM_GLOBAL(volatile u32, 0x9cfbb4u) = value;
    /* 006121a0 */ goto v_006123f0;
v_006121a5:
    /* 006121ab */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df55cu);
    /* 006121b5 */ if (result != 0) goto v_006121c2;
    /* 006121b7 */ CM_GLOBAL(volatile u32, 0x9cfbb8u) = value;
    /* 006121bd */ goto v_006123f0;
v_006121c2:
    /* 006121c8 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df544u);
    /* 006121d2 */ if (result != 0) goto v_006121df;
    /* 006121d4 */ CM_GLOBAL(volatile u32, 0x9cfbbcu) = value;
    /* 006121da */ goto v_006123f0;
v_006121df:
    /* 006121e5 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df514u);
    /* 006121ef */ if (result != 0) goto v_006121fc;
    /* 006121f1 */ CM_GLOBAL(volatile u32, 0x9cfbc0u) = value;
    /* 006121f7 */ goto v_006123f0;
v_006121fc:
    /* 00612202 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df4e4u);
    /* 0061220c */ if (result != 0) goto v_00612219;
    /* 0061220e */ CM_GLOBAL(volatile u32, 0x9cfbc4u) = value;
    /* 00612214 */ goto v_006123f0;
v_00612219:
    /* 0061221f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df4b4u);
    /* 00612229 */ if (result != 0) goto v_00612236;
    /* 0061222b */ CM_GLOBAL(volatile u32, 0x9cfbc8u) = value;
    /* 00612231 */ goto v_006123f0;
v_00612236:
    /* 0061223c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df484u);
    /* 00612246 */ if (result != 0) goto v_00612253;
    /* 00612248 */ CM_GLOBAL(volatile u32, 0x9cfbccu) = value;
    /* 0061224e */ goto v_006123f0;
v_00612253:
    /* 00612259 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df468u);
    /* 00612263 */ if (result != 0) goto v_00612270;
    /* 00612265 */ CM_GLOBAL(volatile u32, 0x9cfbd0u) = value;
    /* 0061226b */ goto v_006123f0;
v_00612270:
    /* 00612276 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df438u);
    /* 00612280 */ if (result != 0) goto v_0061228d;
    /* 00612282 */ CM_GLOBAL(volatile u32, 0x9cfbd4u) = value;
    /* 00612288 */ goto v_006123f0;
v_0061228d:
    /* 00612293 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df408u);
    /* 0061229d */ if (result != 0) goto v_006122aa;
    /* 0061229f */ CM_GLOBAL(volatile u32, 0x9cfbd8u) = value;
    /* 006122a5 */ goto v_006123f0;
v_006122aa:
    /* 006122b0 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df3dcu);
    /* 006122ba */ if (result != 0) goto v_006122c7;
    /* 006122bc */ CM_GLOBAL(volatile u32, 0x9cfbdcu) = value;
    /* 006122c2 */ goto v_006123f0;
v_006122c7:
    /* 006122cd */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df3b0u);
    /* 006122d7 */ if (result != 0) goto v_006122e4;
    /* 006122d9 */ CM_GLOBAL(volatile u32, 0x9cfbe0u) = value;
    /* 006122df */ goto v_006123f0;
v_006122e4:
    /* 006122ea */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df384u);
    /* 006122f4 */ if (result != 0) goto v_00612301;
    /* 006122f6 */ CM_GLOBAL(volatile u32, 0x9cfbe4u) = value;
    /* 006122fc */ goto v_006123f0;
v_00612301:
    /* 00612307 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df354u);
    /* 00612311 */ if (result != 0) goto v_0061231e;
    /* 00612313 */ CM_GLOBAL(volatile u32, 0x9cfbe8u) = value;
    /* 00612319 */ goto v_006123f0;
v_0061231e:
    /* 00612324 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df328u);
    /* 0061232e */ if (result != 0) goto v_0061233b;
    /* 00612330 */ CM_GLOBAL(volatile u32, 0x9cfbecu) = value;
    /* 00612336 */ goto v_006123f0;
v_0061233b:
    /* 00612341 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df2fcu);
    /* 0061234b */ if (result != 0) goto v_00612358;
    /* 0061234d */ CM_GLOBAL(volatile u32, 0x9cfbf0u) = value;
    /* 00612353 */ goto v_006123f0;
v_00612358:
    /* 0061235e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df2e0u);
    /* 00612368 */ if (result != 0) goto v_00612372;
    /* 0061236a */ CM_GLOBAL(volatile u32, 0x9cfbf4u) = value;
    /* 00612370 */ goto v_006123f0;
v_00612372:
    /* 00612378 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df2b4u);
    /* 00612382 */ if (result != 0) goto v_0061238c;
    /* 00612384 */ CM_GLOBAL(volatile u32, 0x9cfbf8u) = value;
    /* 0061238a */ goto v_006123f0;
v_0061238c:
    /* 00612392 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df288u);
    /* 0061239c */ if (result != 0) goto v_006123a6;
    /* 0061239e */ CM_GLOBAL(volatile u32, 0x9cfbfcu) = value;
    /* 006123a4 */ goto v_006123f0;
v_006123a6:
    /* 006123ac */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df26cu);
    /* 006123b6 */ if (result != 0) goto v_006123c0;
    /* 006123b8 */ CM_GLOBAL(volatile u32, 0x9cfc00u) = value;
    /* 006123be */ goto v_006123f0;
v_006123c0:
    /* 006123c6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df248u);
    /* 006123d0 */ if (result != 0) goto v_006123f0;
    /* 006123d2 */ CM_GLOBAL(volatile u32, 0x9cfc04u) = value;
    /* 006123d8 */ goto v_006123f0;
v_006123da:
    /* 006123da */ CM_GLOBAL(volatile u32, 0x9cfb78u) = value;
    /* 006123e0 */ goto v_006123f0;
v_006123e2:
    /* 006123e2 */ CM_GLOBAL(volatile u32, 0x9cfb74u) = value;
    /* 006123e8 */ goto v_006123f0;
v_006123ea:
    /* 006123ea */ CM_GLOBAL(volatile u32, 0x9cfb70u) = value;
v_006123f0:
    /* 006123f6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df224u);
    /* 00612400 */ if (result != 0) goto v_0061240d;
    /* 00612402 */ CM_GLOBAL(volatile u32, 0x9cfc08u) = value;
    /* 00612408 */ goto v_00612fbc;
v_0061240d:
    /* 00612413 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df1fcu);
    /* 0061241d */ if (result != 0) goto v_0061242a;
    /* 0061241f */ CM_GLOBAL(volatile u32, 0x9cfc0cu) = value;
    /* 00612425 */ goto v_00612fbc;
v_0061242a:
    /* 00612430 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df1d4u);
    /* 0061243a */ if (result != 0) goto v_00612447;
    /* 0061243c */ CM_GLOBAL(volatile u32, 0x9cfc10u) = value;
    /* 00612442 */ goto v_00612fbc;
v_00612447:
    /* 0061244d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df1a4u);
    /* 00612457 */ if (result != 0) goto v_00612464;
    /* 00612459 */ CM_GLOBAL(volatile u32, 0x9cfc14u) = value;
    /* 0061245f */ goto v_00612fbc;
v_00612464:
    /* 0061246a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df174u);
    /* 00612474 */ if (result != 0) goto v_00612481;
    /* 00612476 */ CM_GLOBAL(volatile u32, 0x9cfc18u) = value;
    /* 0061247c */ goto v_00612fbc;
v_00612481:
    /* 00612487 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df148u);
    /* 00612491 */ if (result != 0) goto v_0061249e;
    /* 00612493 */ CM_GLOBAL(volatile u32, 0x9cfc1cu) = value;
    /* 00612499 */ goto v_00612fbc;
v_0061249e:
    /* 006124a4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df118u);
    /* 006124ae */ if (result != 0) goto v_006124bb;
    /* 006124b0 */ CM_GLOBAL(volatile u32, 0x9cfc20u) = value;
    /* 006124b6 */ goto v_00612fbc;
v_006124bb:
    /* 006124c1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df0e8u);
    /* 006124cb */ if (result != 0) goto v_006124d8;
    /* 006124cd */ CM_GLOBAL(volatile u32, 0x9cfc24u) = value;
    /* 006124d3 */ goto v_00612fbc;
v_006124d8:
    /* 006124de */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df0b8u);
    /* 006124e8 */ if (result != 0) goto v_006124f5;
    /* 006124ea */ CM_GLOBAL(volatile u32, 0x9cfc28u) = value;
    /* 006124f0 */ goto v_00612fbc;
v_006124f5:
    /* 006124fb */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df08cu);
    /* 00612505 */ if (result != 0) goto v_00612512;
    /* 00612507 */ CM_GLOBAL(volatile u32, 0x9cfc2cu) = value;
    /* 0061250d */ goto v_00612fbc;
v_00612512:
    /* 00612518 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df05cu);
    /* 00612522 */ if (result != 0) goto v_0061252f;
    /* 00612524 */ CM_GLOBAL(volatile u32, 0x9cfc30u) = value;
    /* 0061252a */ goto v_00612fbc;
v_0061252f:
    /* 00612535 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df040u);
    /* 0061253f */ if (result != 0) goto v_0061254c;
    /* 00612541 */ CM_GLOBAL(volatile u32, 0x9cfc34u) = value;
    /* 00612547 */ goto v_00612fbc;
v_0061254c:
    /* 00612552 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df024u);
    /* 0061255c */ if (result != 0) goto v_00612569;
    /* 0061255e */ CM_GLOBAL(volatile u32, 0x9cfc38u) = value;
    /* 00612564 */ goto v_00612fbc;
v_00612569:
    /* 0061256f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9df00cu);
    /* 00612579 */ if (result != 0) goto v_00612586;
    /* 0061257b */ CM_GLOBAL(volatile u32, 0x9cfc3cu) = value;
    /* 00612581 */ goto v_00612fbc;
v_00612586:
    /* 0061258c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9defe8u);
    /* 00612596 */ if (result != 0) goto v_006125a3;
    /* 00612598 */ CM_GLOBAL(volatile u32, 0x9cfc40u) = value;
    /* 0061259e */ goto v_00612fbc;
v_006125a3:
    /* 006125a9 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9defb8u);
    /* 006125b3 */ if (result != 0) goto v_006125c0;
    /* 006125b5 */ CM_GLOBAL(volatile u32, 0x9cfc44u) = value;
    /* 006125bb */ goto v_00612fbc;
v_006125c0:
    /* 006125c6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9def88u);
    /* 006125d0 */ if (result != 0) goto v_006125dd;
    /* 006125d2 */ CM_GLOBAL(volatile u32, 0x9cfc48u) = value;
    /* 006125d8 */ goto v_00612fbc;
v_006125dd:
    /* 006125e3 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9def6cu);
    /* 006125ed */ if (result != 0) goto v_006125fa;
    /* 006125ef */ CM_GLOBAL(volatile u32, 0x9cfc4cu) = value;
    /* 006125f5 */ goto v_00612fbc;
v_006125fa:
    /* 00612600 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9def54u);
    /* 0061260a */ if (result != 0) goto v_00612617;
    /* 0061260c */ CM_GLOBAL(volatile u32, 0x9cfc50u) = value;
    /* 00612612 */ goto v_00612fbc;
v_00612617:
    /* 0061261d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9def30u);
    /* 00612627 */ if (result != 0) goto v_00612634;
    /* 00612629 */ CM_GLOBAL(volatile u32, 0x9cfc54u) = value;
    /* 0061262f */ goto v_00612fbc;
v_00612634:
    /* 0061263a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9def10u);
    /* 00612644 */ if (result != 0) goto v_00612651;
    /* 00612646 */ CM_GLOBAL(volatile u32, 0x9cfc58u) = value;
    /* 0061264c */ goto v_00612fbc;
v_00612651:
    /* 00612657 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9deee8u);
    /* 00612661 */ if (result == 0) goto v_00612fb6;
    /* 0061266d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9deec4u);
    /* 00612677 */ if (result == 0) goto v_00612fb6;
    /* 00612683 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9deeacu);
    /* 0061268d */ if (result != 0) goto v_0061269a;
    /* 0061268f */ CM_GLOBAL(volatile u32, 0x9cfc60u) = value;
    /* 00612695 */ goto v_00612fbc;
v_0061269a:
    /* 006126a0 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dee94u);
    /* 006126aa */ if (result != 0) goto v_006126b7;
    /* 006126ac */ CM_GLOBAL(volatile u32, 0x9cfc64u) = value;
    /* 006126b2 */ goto v_00612fbc;
v_006126b7:
    /* 006126bd */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dee74u);
    /* 006126c7 */ if (result != 0) goto v_006126d4;
    /* 006126c9 */ CM_GLOBAL(volatile u32, 0x9cfc68u) = value;
    /* 006126cf */ goto v_00612fbc;
v_006126d4:
    /* 006126da */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dee58u);
    /* 006126e4 */ if (result != 0) goto v_006126f1;
    /* 006126e6 */ CM_GLOBAL(volatile u32, 0x9cfc6cu) = value;
    /* 006126ec */ goto v_00612fbc;
v_006126f1:
    /* 006126f7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dee3cu);
    /* 00612701 */ if (result != 0) goto v_0061270e;
    /* 00612703 */ CM_GLOBAL(volatile u32, 0x9cfc70u) = value;
    /* 00612709 */ goto v_00612fbc;
v_0061270e:
    /* 00612714 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dee1cu);
    /* 0061271e */ if (result != 0) goto v_0061272b;
    /* 00612720 */ CM_GLOBAL(volatile u32, 0x9cfc74u) = value;
    /* 00612726 */ goto v_00612fbc;
v_0061272b:
    /* 00612731 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dee00u);
    /* 0061273b */ if (result != 0) goto v_00612748;
    /* 0061273d */ CM_GLOBAL(volatile u32, 0x9cfc78u) = value;
    /* 00612743 */ goto v_00612fbc;
v_00612748:
    /* 0061274e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dedd8u);
    /* 00612758 */ if (result != 0) goto v_00612765;
    /* 0061275a */ CM_GLOBAL(volatile u32, 0x9cfc7cu) = value;
    /* 00612760 */ goto v_00612fbc;
v_00612765:
    /* 0061276b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dedb8u);
    /* 00612775 */ if (result != 0) goto v_00612782;
    /* 00612777 */ CM_GLOBAL(volatile u32, 0x9cfc80u) = value;
    /* 0061277d */ goto v_00612fbc;
v_00612782:
    /* 00612788 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ded90u);
    /* 00612792 */ if (result != 0) goto v_0061279f;
    /* 00612794 */ CM_GLOBAL(volatile u32, 0x9cfc84u) = value;
    /* 0061279a */ goto v_00612fbc;
v_0061279f:
    /* 006127a5 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ded6cu);
    /* 006127af */ if (result != 0) goto v_006127bc;
    /* 006127b1 */ CM_GLOBAL(volatile u32, 0x9cfc88u) = value;
    /* 006127b7 */ goto v_00612fbc;
v_006127bc:
    /* 006127c2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ded44u);
    /* 006127cc */ if (result != 0) goto v_006127d9;
    /* 006127ce */ CM_GLOBAL(volatile u32, 0x9cfc8cu) = value;
    /* 006127d4 */ goto v_00612fbc;
v_006127d9:
    /* 006127df */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ded1cu);
    /* 006127e9 */ if (result != 0) goto v_006127f6;
    /* 006127eb */ CM_GLOBAL(volatile u32, 0x9cfc90u) = value;
    /* 006127f1 */ goto v_00612fbc;
v_006127f6:
    /* 006127fc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9decf8u);
    /* 00612806 */ if (result != 0) goto v_00612813;
    /* 00612808 */ CM_GLOBAL(volatile u32, 0x9cfc94u) = value;
    /* 0061280e */ goto v_00612fbc;
v_00612813:
    /* 00612819 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9decc8u);
    /* 00612823 */ if (result != 0) goto v_00612830;
    /* 00612825 */ CM_GLOBAL(volatile u32, 0x9cfc98u) = value;
    /* 0061282b */ goto v_00612fbc;
v_00612830:
    /* 00612836 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dec98u);
    /* 00612840 */ if (result == 0) goto v_00612fae;
    /* 0061284c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dec6cu);
    /* 00612856 */ if (result == 0) goto v_00612fae;
    /* 00612862 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dec40u);
    /* 0061286c */ if (result == 0) goto v_00612fa6;
    /* 00612878 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dec14u);
    /* 00612882 */ if (result == 0) goto v_00612fa6;
    /* 0061288e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9debe4u);
    /* 00612898 */ if (result == 0) goto v_00612f9e;
    /* 006128a4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9debb8u);
    /* 006128ae */ if (result == 0) goto v_00612f9e;
    /* 006128ba */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9deb8cu);
    /* 006128c4 */ if (result == 0) goto v_00612f96;
    /* 006128d0 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9deb60u);
    /* 006128da */ if (result == 0) goto v_00612f96;
    /* 006128e6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9deb38u);
    /* 006128f0 */ if (result == 0) goto v_00612f8e;
    /* 006128fc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9deb14u);
    /* 00612906 */ if (result == 0) goto v_00612f8e;
    /* 00612912 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9deaecu);
    /* 0061291c */ if (result == 0) goto v_00612f86;
    /* 00612928 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9deac8u);
    /* 00612932 */ if (result == 0) goto v_00612f86;
    /* 0061293e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dea9cu);
    /* 00612948 */ if (result != 0) goto v_00612955;
    /* 0061294a */ CM_GLOBAL(volatile u32, 0x9cfcb4u) = value;
    /* 00612950 */ goto v_00612fbc;
v_00612955:
    /* 0061295b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dea74u);
    /* 00612965 */ if (result == 0) goto v_00612f7e;
    /* 00612971 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dea44u);
    /* 0061297b */ if (result == 0) goto v_00612f7e;
    /* 00612987 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dea1cu);
    /* 00612991 */ if (result != 0) goto v_0061299e;
    /* 00612993 */ CM_GLOBAL(volatile u32, 0x9cfcbcu) = value;
    /* 00612999 */ goto v_00612fbc;
v_0061299e:
    /* 006129a4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de9f0u);
    /* 006129ae */ if (result != 0) goto v_006129bb;
    /* 006129b0 */ CM_GLOBAL(volatile u32, 0x9cfcc0u) = value;
    /* 006129b6 */ goto v_00612fbc;
v_006129bb:
    /* 006129c1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de9ccu);
    /* 006129cb */ if (result != 0) goto v_006129d8;
    /* 006129cd */ CM_GLOBAL(volatile u32, 0x9cfcc4u) = value;
    /* 006129d3 */ goto v_00612fbc;
v_006129d8:
    /* 006129de */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de9a8u);
    /* 006129e8 */ if (result != 0) goto v_006129f5;
    /* 006129ea */ CM_GLOBAL(volatile u32, 0x9cfcc8u) = value;
    /* 006129f0 */ goto v_00612fbc;
v_006129f5:
    /* 006129fb */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de984u);
    /* 00612a05 */ if (result != 0) goto v_00612a12;
    /* 00612a07 */ CM_GLOBAL(volatile u32, 0x9cfcccu) = value;
    /* 00612a0d */ goto v_00612fbc;
v_00612a12:
    /* 00612a18 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de960u);
    /* 00612a22 */ if (result != 0) goto v_00612a2f;
    /* 00612a24 */ CM_GLOBAL(volatile u32, 0x9cfcd0u) = value;
    /* 00612a2a */ goto v_00612fbc;
v_00612a2f:
    /* 00612a35 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de938u);
    /* 00612a3f */ if (result != 0) goto v_00612a4c;
    /* 00612a41 */ CM_GLOBAL(volatile u32, 0x9cfcd4u) = value;
    /* 00612a47 */ goto v_00612fbc;
v_00612a4c:
    /* 00612a52 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de910u);
    /* 00612a5c */ if (result != 0) goto v_00612a69;
    /* 00612a5e */ CM_GLOBAL(volatile u32, 0x9cfcd8u) = value;
    /* 00612a64 */ goto v_00612fbc;
v_00612a69:
    /* 00612a6f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de8e8u);
    /* 00612a79 */ if (result != 0) goto v_00612a86;
    /* 00612a7b */ CM_GLOBAL(volatile u32, 0x9cfcdcu) = value;
    /* 00612a81 */ goto v_00612fbc;
v_00612a86:
    /* 00612a8c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de8c0u);
    /* 00612a96 */ if (result != 0) goto v_00612aa3;
    /* 00612a98 */ CM_GLOBAL(volatile u32, 0x9cfce0u) = value;
    /* 00612a9e */ goto v_00612fbc;
v_00612aa3:
    /* 00612aa9 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de898u);
    /* 00612ab3 */ if (result != 0) goto v_00612ac0;
    /* 00612ab5 */ CM_GLOBAL(volatile u32, 0x9cfce4u) = value;
    /* 00612abb */ goto v_00612fbc;
v_00612ac0:
    /* 00612ac6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de878u);
    /* 00612ad0 */ if (result != 0) goto v_00612add;
    /* 00612ad2 */ CM_GLOBAL(volatile u32, 0x9cfce4u) = value;
    /* 00612ad8 */ goto v_00612fbc;
v_00612add:
    /* 00612ae3 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de850u);
    /* 00612aed */ if (result != 0) goto v_00612afa;
    /* 00612aef */ CM_GLOBAL(volatile u32, 0x9cfce8u) = value;
    /* 00612af5 */ goto v_00612fbc;
v_00612afa:
    /* 00612b00 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de824u);
    /* 00612b0a */ if (result != 0) goto v_00612b17;
    /* 00612b0c */ CM_GLOBAL(volatile u32, 0x9cfcecu) = value;
    /* 00612b12 */ goto v_00612fbc;
v_00612b17:
    /* 00612b1d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de7fcu);
    /* 00612b27 */ if (result != 0) goto v_00612b34;
    /* 00612b29 */ CM_GLOBAL(volatile u32, 0x9cfcf0u) = value;
    /* 00612b2f */ goto v_00612fbc;
v_00612b34:
    /* 00612b3a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de7d4u);
    /* 00612b44 */ if (result != 0) goto v_00612b51;
    /* 00612b46 */ CM_GLOBAL(volatile u32, 0x9cfcf4u) = value;
    /* 00612b4c */ goto v_00612fbc;
v_00612b51:
    /* 00612b57 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de7acu);
    /* 00612b61 */ if (result != 0) goto v_00612b6e;
    /* 00612b63 */ CM_GLOBAL(volatile u32, 0x9cfcf8u) = value;
    /* 00612b69 */ goto v_00612fbc;
v_00612b6e:
    /* 00612b74 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de780u);
    /* 00612b7e */ if (result != 0) goto v_00612b8b;
    /* 00612b80 */ CM_GLOBAL(volatile u32, 0x9cfcfcu) = value;
    /* 00612b86 */ goto v_00612fbc;
v_00612b8b:
    /* 00612b91 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de750u);
    /* 00612b9b */ if (result == 0) goto v_00612f76;
    /* 00612ba7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de720u);
    /* 00612bb1 */ if (result == 0) goto v_00612f76;
    /* 00612bbd */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de6ecu);
    /* 00612bc7 */ if (result == 0) goto v_00612f6e;
    /* 00612bd3 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de6bcu);
    /* 00612bdd */ if (result == 0) goto v_00612f6e;
    /* 00612be9 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de690u);
    /* 00612bf3 */ if (result == 0) goto v_00612f66;
    /* 00612bff */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de668u);
    /* 00612c09 */ if (result == 0) goto v_00612f66;
    /* 00612c15 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de640u);
    /* 00612c1f */ if (result != 0) goto v_00612c2c;
    /* 00612c21 */ CM_GLOBAL(volatile u32, 0x9cfd0cu) = value;
    /* 00612c27 */ goto v_00612fbc;
v_00612c2c:
    /* 00612c32 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de620u);
    /* 00612c3c */ if (result != 0) goto v_00612c49;
    /* 00612c3e */ CM_GLOBAL(volatile u32, 0x9cfd10u) = value;
    /* 00612c44 */ goto v_00612fbc;
v_00612c49:
    /* 00612c4f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de600u);
    /* 00612c59 */ if (result != 0) goto v_00612c66;
    /* 00612c5b */ CM_GLOBAL(volatile u32, 0x9cfd14u) = value;
    /* 00612c61 */ goto v_00612fbc;
v_00612c66:
    /* 00612c6c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de5e4u);
    /* 00612c76 */ if (result != 0) goto v_00612c83;
    /* 00612c78 */ CM_GLOBAL(volatile u32, 0x9cfd18u) = value;
    /* 00612c7e */ goto v_00612fbc;
v_00612c83:
    /* 00612c89 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de5c8u);
    /* 00612c93 */ if (result != 0) goto v_00612ca0;
    /* 00612c95 */ CM_GLOBAL(volatile u32, 0x9cfd1cu) = value;
    /* 00612c9b */ goto v_00612fbc;
v_00612ca0:
    /* 00612ca6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de5a8u);
    /* 00612cb0 */ if (result != 0) goto v_00612cbd;
    /* 00612cb2 */ CM_GLOBAL(volatile u32, 0x9cfd20u) = value;
    /* 00612cb8 */ goto v_00612fbc;
v_00612cbd:
    /* 00612cc3 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de580u);
    /* 00612ccd */ if (result != 0) goto v_00612cda;
    /* 00612ccf */ CM_GLOBAL(volatile u32, 0x9cfd24u) = value;
    /* 00612cd5 */ goto v_00612fbc;
v_00612cda:
    /* 00612ce0 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de558u);
    /* 00612cea */ if (result != 0) goto v_00612cf7;
    /* 00612cec */ CM_GLOBAL(volatile u32, 0x9cfd28u) = value;
    /* 00612cf2 */ goto v_00612fbc;
v_00612cf7:
    /* 00612cfd */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de534u);
    /* 00612d07 */ if (result != 0) goto v_00612d14;
    /* 00612d09 */ CM_GLOBAL(volatile u32, 0x9cfd2cu) = value;
    /* 00612d0f */ goto v_00612fbc;
v_00612d14:
    /* 00612d1a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de50cu);
    /* 00612d24 */ if (result != 0) goto v_00612d31;
    /* 00612d26 */ CM_GLOBAL(volatile u32, 0x9cfd30u) = value;
    /* 00612d2c */ goto v_00612fbc;
v_00612d31:
    /* 00612d37 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de4e4u);
    /* 00612d41 */ if (result != 0) goto v_00612d4e;
    /* 00612d43 */ CM_GLOBAL(volatile u32, 0x9cfd34u) = value;
    /* 00612d49 */ goto v_00612fbc;
v_00612d4e:
    /* 00612d54 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de4bcu);
    /* 00612d5e */ if (result == 0) goto v_00612f5e;
    /* 00612d6a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de498u);
    /* 00612d74 */ if (result == 0) goto v_00612f5e;
    /* 00612d80 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de468u);
    /* 00612d8a */ if (result != 0) goto v_00612d97;
    /* 00612d8c */ CM_GLOBAL(volatile u32, 0x9cfd3cu) = value;
    /* 00612d92 */ goto v_00612fbc;
v_00612d97:
    /* 00612d9d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de440u);
    /* 00612da7 */ if (result != 0) goto v_00612db4;
    /* 00612da9 */ CM_GLOBAL(volatile u32, 0x9cfd40u) = value;
    /* 00612daf */ goto v_00612fbc;
v_00612db4:
    /* 00612dba */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de41cu);
    /* 00612dc4 */ if (result != 0) goto v_00612dd1;
    /* 00612dc6 */ CM_GLOBAL(volatile u32, 0x9cfd44u) = value;
    /* 00612dcc */ goto v_00612fbc;
v_00612dd1:
    /* 00612dd7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de3f4u);
    /* 00612de1 */ if (result != 0) goto v_00612dee;
    /* 00612de3 */ CM_GLOBAL(volatile u32, 0x9cfd48u) = value;
    /* 00612de9 */ goto v_00612fbc;
v_00612dee:
    /* 00612df4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de3ccu);
    /* 00612dfe */ if (result != 0) goto v_00612e0b;
    /* 00612e00 */ CM_GLOBAL(volatile u32, 0x9cfd4cu) = value;
    /* 00612e06 */ goto v_00612fbc;
v_00612e0b:
    /* 00612e11 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de3a8u);
    /* 00612e1b */ if (result != 0) goto v_00612e28;
    /* 00612e1d */ CM_GLOBAL(volatile u32, 0x9cfd50u) = value;
    /* 00612e23 */ goto v_00612fbc;
v_00612e28:
    /* 00612e2e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de380u);
    /* 00612e38 */ if (result != 0) goto v_00612e45;
    /* 00612e3a */ CM_GLOBAL(volatile u32, 0x9cfd54u) = value;
    /* 00612e40 */ goto v_00612fbc;
v_00612e45:
    /* 00612e4b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de350u);
    /* 00612e55 */ if (result == 0) goto v_00612f56;
    /* 00612e61 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de320u);
    /* 00612e6b */ if (result == 0) goto v_00612f56;
    /* 00612e77 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de2f8u);
    /* 00612e81 */ if (result != 0) goto v_00612e8e;
    /* 00612e83 */ CM_GLOBAL(volatile u32, 0x9cfd5cu) = value;
    /* 00612e89 */ goto v_00612fbc;
v_00612e8e:
    /* 00612e94 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de2d0u);
    /* 00612e9e */ if (result != 0) goto v_00612eab;
    /* 00612ea0 */ CM_GLOBAL(volatile u32, 0x9cfd60u) = value;
    /* 00612ea6 */ goto v_00612fbc;
v_00612eab:
    /* 00612eb1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de2a4u);
    /* 00612ebb */ if (result != 0) goto v_00612ec8;
    /* 00612ebd */ CM_GLOBAL(volatile u32, 0x9cfd64u) = value;
    /* 00612ec3 */ goto v_00612fbc;
v_00612ec8:
    /* 00612ece */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de27cu);
    /* 00612ed8 */ if (result != 0) goto v_00612ee5;
    /* 00612eda */ CM_GLOBAL(volatile u32, 0x9cfd68u) = value;
    /* 00612ee0 */ goto v_00612fbc;
v_00612ee5:
    /* 00612eeb */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de25cu);
    /* 00612ef5 */ if (result != 0) goto v_00612f02;
    /* 00612ef7 */ CM_GLOBAL(volatile u32, 0x9cfd6cu) = value;
    /* 00612efd */ goto v_00612fbc;
v_00612f02:
    /* 00612f08 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de23cu);
    /* 00612f12 */ if (result != 0) goto v_00612f1f;
    /* 00612f14 */ CM_GLOBAL(volatile u32, 0x9cfd70u) = value;
    /* 00612f1a */ goto v_00612fbc;
v_00612f1f:
    /* 00612f25 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de21cu);
    /* 00612f2f */ if (result != 0) goto v_00612f3c;
    /* 00612f31 */ CM_GLOBAL(volatile u32, 0x9cfd74u) = value;
    /* 00612f37 */ goto v_00612fbc;
v_00612f3c:
    /* 00612f42 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de1fcu);
    /* 00612f4c */ if (result != 0) goto v_00612fbc;
    /* 00612f4e */ CM_GLOBAL(volatile u32, 0x9cfd78u) = value;
    /* 00612f54 */ goto v_00612fbc;
v_00612f56:
    /* 00612f56 */ CM_GLOBAL(volatile u32, 0x9cfd58u) = value;
    /* 00612f5c */ goto v_00612fbc;
v_00612f5e:
    /* 00612f5e */ CM_GLOBAL(volatile u32, 0x9cfd38u) = value;
    /* 00612f64 */ goto v_00612fbc;
v_00612f66:
    /* 00612f66 */ CM_GLOBAL(volatile u32, 0x9cfd08u) = value;
    /* 00612f6c */ goto v_00612fbc;
v_00612f6e:
    /* 00612f6e */ CM_GLOBAL(volatile u32, 0x9cfd04u) = value;
    /* 00612f74 */ goto v_00612fbc;
v_00612f76:
    /* 00612f76 */ CM_GLOBAL(volatile u32, 0x9cfd00u) = value;
    /* 00612f7c */ goto v_00612fbc;
v_00612f7e:
    /* 00612f7e */ CM_GLOBAL(volatile u32, 0x9cfcb8u) = value;
    /* 00612f84 */ goto v_00612fbc;
v_00612f86:
    /* 00612f86 */ CM_GLOBAL(volatile u32, 0x9cfcb0u) = value;
    /* 00612f8c */ goto v_00612fbc;
v_00612f8e:
    /* 00612f8e */ CM_GLOBAL(volatile u32, 0x9cfcacu) = value;
    /* 00612f94 */ goto v_00612fbc;
v_00612f96:
    /* 00612f96 */ CM_GLOBAL(volatile u32, 0x9cfca8u) = value;
    /* 00612f9c */ goto v_00612fbc;
v_00612f9e:
    /* 00612f9e */ CM_GLOBAL(volatile u32, 0x9cfca4u) = value;
    /* 00612fa4 */ goto v_00612fbc;
v_00612fa6:
    /* 00612fa6 */ CM_GLOBAL(volatile u32, 0x9cfca0u) = value;
    /* 00612fac */ goto v_00612fbc;
v_00612fae:
    /* 00612fae */ CM_GLOBAL(volatile u32, 0x9cfc9cu) = value;
    /* 00612fb4 */ goto v_00612fbc;
v_00612fb6:
    /* 00612fb6 */ CM_GLOBAL(volatile u32, 0x9cfc5cu) = value;
v_00612fbc:
    /* 00612fc2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de1d8u);
    /* 00612fcc */ if (result != 0) goto v_00612fd9;
    /* 00612fce */ CM_GLOBAL(volatile u32, 0x9cfd7cu) = value;
    /* 00612fd4 */ goto v_006130b0;
v_00612fd9:
    /* 00612fdf */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de1b4u);
    /* 00612fe9 */ if (result != 0) goto v_00612ff6;
    /* 00612feb */ CM_GLOBAL(volatile u32, 0x9cfd80u) = value;
    /* 00612ff1 */ goto v_006130b0;
v_00612ff6:
    /* 00612ffc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de190u);
    /* 00613006 */ if (result != 0) goto v_00613013;
    /* 00613008 */ CM_GLOBAL(volatile u32, 0x9cfd84u) = value;
    /* 0061300e */ goto v_006130b0;
v_00613013:
    /* 00613019 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de16cu);
    /* 00613023 */ if (result != 0) goto v_00613030;
    /* 00613025 */ CM_GLOBAL(volatile u32, 0x9cfd88u) = value;
    /* 0061302b */ goto v_006130b0;
v_00613030:
    /* 00613036 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de148u);
    /* 00613040 */ if (result != 0) goto v_0061304a;
    /* 00613042 */ CM_GLOBAL(volatile u32, 0x9cfd8cu) = value;
    /* 00613048 */ goto v_006130b0;
v_0061304a:
    /* 00613050 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de124u);
    /* 0061305a */ if (result != 0) goto v_00613064;
    /* 0061305c */ CM_GLOBAL(volatile u32, 0x9cfd90u) = value;
    /* 00613062 */ goto v_006130b0;
v_00613064:
    /* 0061306a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de100u);
    /* 00613074 */ if (result != 0) goto v_0061307e;
    /* 00613076 */ CM_GLOBAL(volatile u32, 0x9cfd94u) = value;
    /* 0061307c */ goto v_006130b0;
v_0061307e:
    /* 00613084 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de0e0u);
    /* 0061308e */ if (result != 0) goto v_00613098;
    /* 00613090 */ CM_GLOBAL(volatile u32, 0x9cfd98u) = value;
    /* 00613096 */ goto v_006130b0;
v_00613098:
    /* 0061309e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de0c0u);
    /* 006130a8 */ if (result != 0) goto v_006130b0;
    /* 006130aa */ CM_GLOBAL(volatile u32, 0x9cfd9cu) = value;
v_006130b0:
    /* 006130b6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de09cu);
    /* 006130c0 */ if (result != 0) goto v_006130cd;
    /* 006130c2 */ CM_GLOBAL(volatile u32, 0x9cfda0u) = value;
    /* 006130c8 */ goto v_00613f4c;
v_006130cd:
    /* 006130d3 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de078u);
    /* 006130dd */ if (result != 0) goto v_006130ea;
    /* 006130df */ CM_GLOBAL(volatile u32, 0x9cfda4u) = value;
    /* 006130e5 */ goto v_00613f4c;
v_006130ea:
    /* 006130f0 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de054u);
    /* 006130fa */ if (result != 0) goto v_00613107;
    /* 006130fc */ CM_GLOBAL(volatile u32, 0x9cfda8u) = value;
    /* 00613102 */ goto v_00613f4c;
v_00613107:
    /* 0061310d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de030u);
    /* 00613117 */ if (result != 0) goto v_00613124;
    /* 00613119 */ CM_GLOBAL(volatile u32, 0x9cfdacu) = value;
    /* 0061311f */ goto v_00613f4c;
v_00613124:
    /* 0061312a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9de00cu);
    /* 00613134 */ if (result != 0) goto v_00613141;
    /* 00613136 */ CM_GLOBAL(volatile u32, 0x9cfdb0u) = value;
    /* 0061313c */ goto v_00613f4c;
v_00613141:
    /* 00613147 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddfdcu);
    /* 00613151 */ if (result != 0) goto v_0061315e;
    /* 00613153 */ CM_GLOBAL(volatile u32, 0x9cfdb4u) = value;
    /* 00613159 */ goto v_00613f4c;
v_0061315e:
    /* 00613164 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddfacu);
    /* 0061316e */ if (result != 0) goto v_0061317b;
    /* 00613170 */ CM_GLOBAL(volatile u32, 0x9cfdb8u) = value;
    /* 00613176 */ goto v_00613f4c;
v_0061317b:
    /* 00613181 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddf7cu);
    /* 0061318b */ if (result != 0) goto v_00613198;
    /* 0061318d */ CM_GLOBAL(volatile u32, 0x9cfdbcu) = value;
    /* 00613193 */ goto v_00613f4c;
v_00613198:
    /* 0061319e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddf4cu);
    /* 006131a8 */ if (result != 0) goto v_006131b5;
    /* 006131aa */ CM_GLOBAL(volatile u32, 0x9cfdc0u) = value;
    /* 006131b0 */ goto v_00613f4c;
v_006131b5:
    /* 006131bb */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddf1cu);
    /* 006131c5 */ if (result != 0) goto v_006131d2;
    /* 006131c7 */ CM_GLOBAL(volatile u32, 0x9cfdc4u) = value;
    /* 006131cd */ goto v_00613f4c;
v_006131d2:
    /* 006131d8 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddeecu);
    /* 006131e2 */ if (result != 0) goto v_006131ef;
    /* 006131e4 */ CM_GLOBAL(volatile u32, 0x9cfdc8u) = value;
    /* 006131ea */ goto v_00613f4c;
v_006131ef:
    /* 006131f5 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dded0u);
    /* 006131ff */ if (result != 0) goto v_0061320c;
    /* 00613201 */ CM_GLOBAL(volatile u32, 0x9cfdccu) = value;
    /* 00613207 */ goto v_00613f4c;
v_0061320c:
    /* 00613212 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddeacu);
    /* 0061321c */ if (result != 0) goto v_00613229;
    /* 0061321e */ CM_GLOBAL(volatile u32, 0x9cfdd0u) = value;
    /* 00613224 */ goto v_00613f4c;
v_00613229:
    /* 0061322f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dde8cu);
    /* 00613239 */ if (result != 0) goto v_00613246;
    /* 0061323b */ CM_GLOBAL(volatile u32, 0x9cfdd4u) = value;
    /* 00613241 */ goto v_00613f4c;
v_00613246:
    /* 0061324c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dde70u);
    /* 00613256 */ if (result != 0) goto v_00613263;
    /* 00613258 */ CM_GLOBAL(volatile u32, 0x9cfdd8u) = value;
    /* 0061325e */ goto v_00613f4c;
v_00613263:
    /* 00613269 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dde48u);
    /* 00613273 */ if (result != 0) goto v_00613280;
    /* 00613275 */ CM_GLOBAL(volatile u32, 0x9cfddcu) = value;
    /* 0061327b */ goto v_00613f4c;
v_00613280:
    /* 00613286 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dde18u);
    /* 00613290 */ if (result != 0) goto v_0061329d;
    /* 00613292 */ CM_GLOBAL(volatile u32, 0x9cfde0u) = value;
    /* 00613298 */ goto v_00613f4c;
v_0061329d:
    /* 006132a3 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dddecu);
    /* 006132ad */ if (result != 0) goto v_006132ba;
    /* 006132af */ CM_GLOBAL(volatile u32, 0x9cfde4u) = value;
    /* 006132b5 */ goto v_00613f4c;
v_006132ba:
    /* 006132c0 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dddc0u);
    /* 006132ca */ if (result != 0) goto v_006132d7;
    /* 006132cc */ CM_GLOBAL(volatile u32, 0x9cfde8u) = value;
    /* 006132d2 */ goto v_00613f4c;
v_006132d7:
    /* 006132dd */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddd98u);
    /* 006132e7 */ if (result != 0) goto v_006132f4;
    /* 006132e9 */ CM_GLOBAL(volatile u32, 0x9cfdecu) = value;
    /* 006132ef */ goto v_00613f4c;
v_006132f4:
    /* 006132fa */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddd70u);
    /* 00613304 */ if (result != 0) goto v_00613311;
    /* 00613306 */ CM_GLOBAL(volatile u32, 0x9cfdf0u) = value;
    /* 0061330c */ goto v_00613f4c;
v_00613311:
    /* 00613317 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddd48u);
    /* 00613321 */ if (result != 0) goto v_0061332e;
    /* 00613323 */ CM_GLOBAL(volatile u32, 0x9cfdf4u) = value;
    /* 00613329 */ goto v_00613f4c;
v_0061332e:
    /* 00613334 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddd2cu);
    /* 0061333e */ if (result != 0) goto v_0061334b;
    /* 00613340 */ CM_GLOBAL(volatile u32, 0x9cfdf8u) = value;
    /* 00613346 */ goto v_00613f4c;
v_0061334b:
    /* 00613351 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddd0cu);
    /* 0061335b */ if (result != 0) goto v_00613368;
    /* 0061335d */ CM_GLOBAL(volatile u32, 0x9cfdfcu) = value;
    /* 00613363 */ goto v_00613f4c;
v_00613368:
    /* 0061336e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddce4u);
    /* 00613378 */ if (result != 0) goto v_00613385;
    /* 0061337a */ CM_GLOBAL(volatile u32, 0x9cfe00u) = value;
    /* 00613380 */ goto v_00613f4c;
v_00613385:
    /* 0061338b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddcb8u);
    /* 00613395 */ if (result != 0) goto v_006133a2;
    /* 00613397 */ CM_GLOBAL(volatile u32, 0x9cfe04u) = value;
    /* 0061339d */ goto v_00613f4c;
v_006133a2:
    /* 006133a8 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddc88u);
    /* 006133b2 */ if (result != 0) goto v_006133bf;
    /* 006133b4 */ CM_GLOBAL(volatile u32, 0x9cfe08u) = value;
    /* 006133ba */ goto v_00613f4c;
v_006133bf:
    /* 006133c5 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddc60u);
    /* 006133cf */ if (result != 0) goto v_006133dc;
    /* 006133d1 */ CM_GLOBAL(volatile u32, 0x9cfe0cu) = value;
    /* 006133d7 */ goto v_00613f4c;
v_006133dc:
    /* 006133e2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddc38u);
    /* 006133ec */ if (result != 0) goto v_006133f9;
    /* 006133ee */ CM_GLOBAL(volatile u32, 0x9cfe10u) = value;
    /* 006133f4 */ goto v_00613f4c;
v_006133f9:
    /* 006133ff */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddc10u);
    /* 00613409 */ if (result != 0) goto v_00613416;
    /* 0061340b */ CM_GLOBAL(volatile u32, 0x9cfe14u) = value;
    /* 00613411 */ goto v_00613f4c;
v_00613416:
    /* 0061341c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddbe0u);
    /* 00613426 */ if (result != 0) goto v_00613433;
    /* 00613428 */ CM_GLOBAL(volatile u32, 0x9cfe18u) = value;
    /* 0061342e */ goto v_00613f4c;
v_00613433:
    /* 00613439 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddbb8u);
    /* 00613443 */ if (result != 0) goto v_00613450;
    /* 00613445 */ CM_GLOBAL(volatile u32, 0x9cfe1cu) = value;
    /* 0061344b */ goto v_00613f4c;
v_00613450:
    /* 00613456 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddb8cu);
    /* 00613460 */ if (result != 0) goto v_0061346d;
    /* 00613462 */ CM_GLOBAL(volatile u32, 0x9cfe20u) = value;
    /* 00613468 */ goto v_00613f4c;
v_0061346d:
    /* 00613473 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddb60u);
    /* 0061347d */ if (result != 0) goto v_0061348a;
    /* 0061347f */ CM_GLOBAL(volatile u32, 0x9cfe24u) = value;
    /* 00613485 */ goto v_00613f4c;
v_0061348a:
    /* 00613490 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddb34u);
    /* 0061349a */ if (result != 0) goto v_006134a7;
    /* 0061349c */ CM_GLOBAL(volatile u32, 0x9cfe28u) = value;
    /* 006134a2 */ goto v_00613f4c;
v_006134a7:
    /* 006134ad */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddb08u);
    /* 006134b7 */ if (result != 0) goto v_006134c4;
    /* 006134b9 */ CM_GLOBAL(volatile u32, 0x9cfe2cu) = value;
    /* 006134bf */ goto v_00613f4c;
v_006134c4:
    /* 006134ca */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddae0u);
    /* 006134d4 */ if (result != 0) goto v_006134e1;
    /* 006134d6 */ CM_GLOBAL(volatile u32, 0x9cfe30u) = value;
    /* 006134dc */ goto v_00613f4c;
v_006134e1:
    /* 006134e7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ddab4u);
    /* 006134f1 */ if (result != 0) goto v_006134fe;
    /* 006134f3 */ CM_GLOBAL(volatile u32, 0x9cfe34u) = value;
    /* 006134f9 */ goto v_00613f4c;
v_006134fe:
    /* 00613504 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dda84u);
    /* 0061350e */ if (result != 0) goto v_0061351b;
    /* 00613510 */ CM_GLOBAL(volatile u32, 0x9cfe38u) = value;
    /* 00613516 */ goto v_00613f4c;
v_0061351b:
    /* 00613521 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dda58u);
    /* 0061352b */ if (result != 0) goto v_00613538;
    /* 0061352d */ CM_GLOBAL(volatile u32, 0x9cfe3cu) = value;
    /* 00613533 */ goto v_00613f4c;
v_00613538:
    /* 0061353e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dda2cu);
    /* 00613548 */ if (result != 0) goto v_00613555;
    /* 0061354a */ CM_GLOBAL(volatile u32, 0x9cfe40u) = value;
    /* 00613550 */ goto v_00613f4c;
v_00613555:
    /* 0061355b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dda04u);
    /* 00613565 */ if (result != 0) goto v_00613572;
    /* 00613567 */ CM_GLOBAL(volatile u32, 0x9cfe44u) = value;
    /* 0061356d */ goto v_00613f4c;
v_00613572:
    /* 00613578 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd9d8u);
    /* 00613582 */ if (result != 0) goto v_0061358f;
    /* 00613584 */ CM_GLOBAL(volatile u32, 0x9cfe48u) = value;
    /* 0061358a */ goto v_00613f4c;
v_0061358f:
    /* 00613595 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd9a8u);
    /* 0061359f */ if (result != 0) goto v_006135ac;
    /* 006135a1 */ CM_GLOBAL(volatile u32, 0x9cfe4cu) = value;
    /* 006135a7 */ goto v_00613f4c;
v_006135ac:
    /* 006135b2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd97cu);
    /* 006135bc */ if (result != 0) goto v_006135c9;
    /* 006135be */ CM_GLOBAL(volatile u32, 0x9cfe50u) = value;
    /* 006135c4 */ goto v_00613f4c;
v_006135c9:
    /* 006135cf */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd950u);
    /* 006135d9 */ if (result != 0) goto v_006135e6;
    /* 006135db */ CM_GLOBAL(volatile u32, 0x9cfe54u) = value;
    /* 006135e1 */ goto v_00613f4c;
v_006135e6:
    /* 006135ec */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd924u);
    /* 006135f6 */ if (result != 0) goto v_00613603;
    /* 006135f8 */ CM_GLOBAL(volatile u32, 0x9cfe58u) = value;
    /* 006135fe */ goto v_00613f4c;
v_00613603:
    /* 00613609 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd8f8u);
    /* 00613613 */ if (result != 0) goto v_00613620;
    /* 00613615 */ CM_GLOBAL(volatile u32, 0x9cfe5cu) = value;
    /* 0061361b */ goto v_00613f4c;
v_00613620:
    /* 00613626 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd8d0u);
    /* 00613630 */ if (result != 0) goto v_0061363d;
    /* 00613632 */ CM_GLOBAL(volatile u32, 0x9cfe60u) = value;
    /* 00613638 */ goto v_00613f4c;
v_0061363d:
    /* 00613643 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd8a0u);
    /* 0061364d */ if (result != 0) goto v_0061365a;
    /* 0061364f */ CM_GLOBAL(volatile u32, 0x9cfe64u) = value;
    /* 00613655 */ goto v_00613f4c;
v_0061365a:
    /* 00613660 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd874u);
    /* 0061366a */ if (result != 0) goto v_00613677;
    /* 0061366c */ CM_GLOBAL(volatile u32, 0x9cfe68u) = value;
    /* 00613672 */ goto v_00613f4c;
v_00613677:
    /* 0061367d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd848u);
    /* 00613687 */ if (result != 0) goto v_00613694;
    /* 00613689 */ CM_GLOBAL(volatile u32, 0x9cfe6cu) = value;
    /* 0061368f */ goto v_00613f4c;
v_00613694:
    /* 0061369a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd818u);
    /* 006136a4 */ if (result != 0) goto v_006136b1;
    /* 006136a6 */ CM_GLOBAL(volatile u32, 0x9cfe70u) = value;
    /* 006136ac */ goto v_00613f4c;
v_006136b1:
    /* 006136b7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd7ecu);
    /* 006136c1 */ if (result != 0) goto v_006136ce;
    /* 006136c3 */ CM_GLOBAL(volatile u32, 0x9cfe74u) = value;
    /* 006136c9 */ goto v_00613f4c;
v_006136ce:
    /* 006136d4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd7c0u);
    /* 006136de */ if (result != 0) goto v_006136eb;
    /* 006136e0 */ CM_GLOBAL(volatile u32, 0x9cfe78u) = value;
    /* 006136e6 */ goto v_00613f4c;
v_006136eb:
    /* 006136f1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd7a0u);
    /* 006136fb */ if (result != 0) goto v_00613708;
    /* 006136fd */ CM_GLOBAL(volatile u32, 0x9cfe7cu) = value;
    /* 00613703 */ goto v_00613f4c;
v_00613708:
    /* 0061370e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd780u);
    /* 00613718 */ if (result == 0) goto v_00613f46;
    /* 00613724 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd75cu);
    /* 0061372e */ if (result == 0) goto v_00613f46;
    /* 0061373a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd73cu);
    /* 00613744 */ if (result == 0) goto v_00613f3e;
    /* 00613750 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd718u);
    /* 0061375a */ if (result == 0) goto v_00613f3e;
    /* 00613766 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd6ecu);
    /* 00613770 */ if (result == 0) goto v_00613f36;
    /* 0061377c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd6c4u);
    /* 00613786 */ if (result == 0) goto v_00613f36;
    /* 00613792 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd6a4u);
    /* 0061379c */ if (result == 0) goto v_00613f2e;
    /* 006137a8 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd680u);
    /* 006137b2 */ if (result == 0) goto v_00613f2e;
    /* 006137be */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd658u);
    /* 006137c8 */ if (result == 0) goto v_00613f26;
    /* 006137d4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd630u);
    /* 006137de */ if (result == 0) goto v_00613f26;
    /* 006137ea */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd60cu);
    /* 006137f4 */ if (result == 0) goto v_00613f1e;
    /* 00613800 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd5e4u);
    /* 0061380a */ if (result == 0) goto v_00613f1e;
    /* 00613816 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd5c8u);
    /* 00613820 */ if (result == 0) goto v_00613f16;
    /* 0061382c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd5a4u);
    /* 00613836 */ if (result == 0) goto v_00613f16;
    /* 00613842 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd584u);
    /* 0061384c */ if (result == 0) goto v_00613f0e;
    /* 00613858 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd560u);
    /* 00613862 */ if (result == 0) goto v_00613f0e;
    /* 0061386e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd544u);
    /* 00613878 */ if (result != 0) goto v_00613885;
    /* 0061387a */ CM_GLOBAL(volatile u32, 0x9cfea0u) = value;
    /* 00613880 */ goto v_00613f4c;
v_00613885:
    /* 0061388b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd520u);
    /* 00613895 */ if (result != 0) goto v_006138a2;
    /* 00613897 */ CM_GLOBAL(volatile u32, 0x9cfea4u) = value;
    /* 0061389d */ goto v_00613f4c;
v_006138a2:
    /* 006138a8 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd4f8u);
    /* 006138b2 */ if (result != 0) goto v_006138bf;
    /* 006138b4 */ CM_GLOBAL(volatile u32, 0x9cfea8u) = value;
    /* 006138ba */ goto v_00613f4c;
v_006138bf:
    /* 006138c5 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd4ccu);
    /* 006138cf */ if (result != 0) goto v_006138dc;
    /* 006138d1 */ CM_GLOBAL(volatile u32, 0x9cfeacu) = value;
    /* 006138d7 */ goto v_00613f4c;
v_006138dc:
    /* 006138e2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd4a0u);
    /* 006138ec */ if (result != 0) goto v_006138f9;
    /* 006138ee */ CM_GLOBAL(volatile u32, 0x9cfeb0u) = value;
    /* 006138f4 */ goto v_00613f4c;
v_006138f9:
    /* 006138ff */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd478u);
    /* 00613909 */ if (result != 0) goto v_00613916;
    /* 0061390b */ CM_GLOBAL(volatile u32, 0x9cfeb4u) = value;
    /* 00613911 */ goto v_00613f4c;
v_00613916:
    /* 0061391c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd450u);
    /* 00613926 */ if (result != 0) goto v_00613933;
    /* 00613928 */ CM_GLOBAL(volatile u32, 0x9cfeb8u) = value;
    /* 0061392e */ goto v_00613f4c;
v_00613933:
    /* 00613939 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd424u);
    /* 00613943 */ if (result != 0) goto v_00613950;
    /* 00613945 */ CM_GLOBAL(volatile u32, 0x9cfebcu) = value;
    /* 0061394b */ goto v_00613f4c;
v_00613950:
    /* 00613956 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd3f8u);
    /* 00613960 */ if (result != 0) goto v_0061396d;
    /* 00613962 */ CM_GLOBAL(volatile u32, 0x9cfec0u) = value;
    /* 00613968 */ goto v_00613f4c;
v_0061396d:
    /* 00613973 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd3d0u);
    /* 0061397d */ if (result != 0) goto v_0061398a;
    /* 0061397f */ CM_GLOBAL(volatile u32, 0x9cfec4u) = value;
    /* 00613985 */ goto v_00613f4c;
v_0061398a:
    /* 00613990 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd3b4u);
    /* 0061399a */ if (result != 0) goto v_006139a7;
    /* 0061399c */ CM_GLOBAL(volatile u32, 0x9cfec8u) = value;
    /* 006139a2 */ goto v_00613f4c;
v_006139a7:
    /* 006139ad */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd398u);
    /* 006139b7 */ if (result != 0) goto v_006139c4;
    /* 006139b9 */ CM_GLOBAL(volatile u32, 0x9cfeccu) = value;
    /* 006139bf */ goto v_00613f4c;
v_006139c4:
    /* 006139ca */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd36cu);
    /* 006139d4 */ if (result != 0) goto v_006139e1;
    /* 006139d6 */ CM_GLOBAL(volatile u32, 0x9cfed0u) = value;
    /* 006139dc */ goto v_00613f4c;
v_006139e1:
    /* 006139e7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd340u);
    /* 006139f1 */ if (result != 0) goto v_006139fe;
    /* 006139f3 */ CM_GLOBAL(volatile u32, 0x9cfed4u) = value;
    /* 006139f9 */ goto v_00613f4c;
v_006139fe:
    /* 00613a04 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd318u);
    /* 00613a0e */ if (result != 0) goto v_00613a1b;
    /* 00613a10 */ CM_GLOBAL(volatile u32, 0x9cfed8u) = value;
    /* 00613a16 */ goto v_00613f4c;
v_00613a1b:
    /* 00613a21 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd2f0u);
    /* 00613a2b */ if (result != 0) goto v_00613a38;
    /* 00613a2d */ CM_GLOBAL(volatile u32, 0x9cfedcu) = value;
    /* 00613a33 */ goto v_00613f4c;
v_00613a38:
    /* 00613a3e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd2d4u);
    /* 00613a48 */ if (result != 0) goto v_00613a55;
    /* 00613a4a */ CM_GLOBAL(volatile u32, 0x9cfee0u) = value;
    /* 00613a50 */ goto v_00613f4c;
v_00613a55:
    /* 00613a5b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd2a8u);
    /* 00613a65 */ if (result != 0) goto v_00613a72;
    /* 00613a67 */ CM_GLOBAL(volatile u32, 0x9cfee4u) = value;
    /* 00613a6d */ goto v_00613f4c;
v_00613a72:
    /* 00613a78 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd27cu);
    /* 00613a82 */ if (result != 0) goto v_00613a8f;
    /* 00613a84 */ CM_GLOBAL(volatile u32, 0x9cfee8u) = value;
    /* 00613a8a */ goto v_00613f4c;
v_00613a8f:
    /* 00613a95 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd24cu);
    /* 00613a9f */ if (result != 0) goto v_00613aac;
    /* 00613aa1 */ CM_GLOBAL(volatile u32, 0x9cfeecu) = value;
    /* 00613aa7 */ goto v_00613f4c;
v_00613aac:
    /* 00613ab2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd224u);
    /* 00613abc */ if (result != 0) goto v_00613ac9;
    /* 00613abe */ CM_GLOBAL(volatile u32, 0x9cfef0u) = value;
    /* 00613ac4 */ goto v_00613f4c;
v_00613ac9:
    /* 00613acf */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd1fcu);
    /* 00613ad9 */ if (result != 0) goto v_00613ae6;
    /* 00613adb */ CM_GLOBAL(volatile u32, 0x9cfef4u) = value;
    /* 00613ae1 */ goto v_00613f4c;
v_00613ae6:
    /* 00613aec */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd1d0u);
    /* 00613af6 */ if (result != 0) goto v_00613b03;
    /* 00613af8 */ CM_GLOBAL(volatile u32, 0x9cfef8u) = value;
    /* 00613afe */ goto v_00613f4c;
v_00613b03:
    /* 00613b09 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd1b0u);
    /* 00613b13 */ if (result != 0) goto v_00613b20;
    /* 00613b15 */ CM_GLOBAL(volatile u32, 0x9cfefcu) = value;
    /* 00613b1b */ goto v_00613f4c;
v_00613b20:
    /* 00613b26 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd194u);
    /* 00613b30 */ if (result != 0) goto v_00613b3d;
    /* 00613b32 */ CM_GLOBAL(volatile u32, 0x9cff00u) = value;
    /* 00613b38 */ goto v_00613f4c;
v_00613b3d:
    /* 00613b43 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd168u);
    /* 00613b4d */ if (result != 0) goto v_00613b5a;
    /* 00613b4f */ CM_GLOBAL(volatile u32, 0x9cff04u) = value;
    /* 00613b55 */ goto v_00613f4c;
v_00613b5a:
    /* 00613b60 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd138u);
    /* 00613b6a */ if (result != 0) goto v_00613b77;
    /* 00613b6c */ CM_GLOBAL(volatile u32, 0x9cff08u) = value;
    /* 00613b72 */ goto v_00613f4c;
v_00613b77:
    /* 00613b7d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd104u);
    /* 00613b87 */ if (result != 0) goto v_00613b94;
    /* 00613b89 */ CM_GLOBAL(volatile u32, 0x9cff0cu) = value;
    /* 00613b8f */ goto v_00613f4c;
v_00613b94:
    /* 00613b9a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd0d8u);
    /* 00613ba4 */ if (result != 0) goto v_00613bb1;
    /* 00613ba6 */ CM_GLOBAL(volatile u32, 0x9cff10u) = value;
    /* 00613bac */ goto v_00613f4c;
v_00613bb1:
    /* 00613bb7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd0a8u);
    /* 00613bc1 */ if (result != 0) goto v_00613bce;
    /* 00613bc3 */ CM_GLOBAL(volatile u32, 0x9cff14u) = value;
    /* 00613bc9 */ goto v_00613f4c;
v_00613bce:
    /* 00613bd4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd078u);
    /* 00613bde */ if (result != 0) goto v_00613beb;
    /* 00613be0 */ CM_GLOBAL(volatile u32, 0x9cff18u) = value;
    /* 00613be6 */ goto v_00613f4c;
v_00613beb:
    /* 00613bf1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd04cu);
    /* 00613bfb */ if (result != 0) goto v_00613c08;
    /* 00613bfd */ CM_GLOBAL(volatile u32, 0x9cff1cu) = value;
    /* 00613c03 */ goto v_00613f4c;
v_00613c08:
    /* 00613c0e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dd020u);
    /* 00613c18 */ if (result != 0) goto v_00613c25;
    /* 00613c1a */ CM_GLOBAL(volatile u32, 0x9cff20u) = value;
    /* 00613c20 */ goto v_00613f4c;
v_00613c25:
    /* 00613c2b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcfecu);
    /* 00613c35 */ if (result != 0) goto v_00613c42;
    /* 00613c37 */ CM_GLOBAL(volatile u32, 0x9cff24u) = value;
    /* 00613c3d */ goto v_00613f4c;
v_00613c42:
    /* 00613c48 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcfc4u);
    /* 00613c52 */ if (result != 0) goto v_00613c5f;
    /* 00613c54 */ CM_GLOBAL(volatile u32, 0x9cff28u) = value;
    /* 00613c5a */ goto v_00613f4c;
v_00613c5f:
    /* 00613c65 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcf98u);
    /* 00613c6f */ if (result != 0) goto v_00613c7c;
    /* 00613c71 */ CM_GLOBAL(volatile u32, 0x9cff2cu) = value;
    /* 00613c77 */ goto v_00613f4c;
v_00613c7c:
    /* 00613c82 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcf68u);
    /* 00613c8c */ if (result != 0) goto v_00613c99;
    /* 00613c8e */ CM_GLOBAL(volatile u32, 0x9cff30u) = value;
    /* 00613c94 */ goto v_00613f4c;
v_00613c99:
    /* 00613c9f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcf4cu);
    /* 00613ca9 */ if (result != 0) goto v_00613cb6;
    /* 00613cab */ CM_GLOBAL(volatile u32, 0x9cff34u) = value;
    /* 00613cb1 */ goto v_00613f4c;
v_00613cb6:
    /* 00613cbc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcf30u);
    /* 00613cc6 */ if (result != 0) goto v_00613cd3;
    /* 00613cc8 */ CM_GLOBAL(volatile u32, 0x9cff38u) = value;
    /* 00613cce */ goto v_00613f4c;
v_00613cd3:
    /* 00613cd9 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcf00u);
    /* 00613ce3 */ if (result != 0) goto v_00613cf0;
    /* 00613ce5 */ CM_GLOBAL(volatile u32, 0x9cff3cu) = value;
    /* 00613ceb */ goto v_00613f4c;
v_00613cf0:
    /* 00613cf6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcee8u);
    /* 00613d00 */ if (result != 0) goto v_00613d0d;
    /* 00613d02 */ CM_GLOBAL(volatile u32, 0x9cff40u) = value;
    /* 00613d08 */ goto v_00613f4c;
v_00613d0d:
    /* 00613d13 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dceccu);
    /* 00613d1d */ if (result != 0) goto v_00613d2a;
    /* 00613d1f */ CM_GLOBAL(volatile u32, 0x9cff44u) = value;
    /* 00613d25 */ goto v_00613f4c;
v_00613d2a:
    /* 00613d30 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dceacu);
    /* 00613d3a */ if (result != 0) goto v_00613d47;
    /* 00613d3c */ CM_GLOBAL(volatile u32, 0x9cff48u) = value;
    /* 00613d42 */ goto v_00613f4c;
v_00613d47:
    /* 00613d4d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dce94u);
    /* 00613d57 */ if (result != 0) goto v_00613d64;
    /* 00613d59 */ CM_GLOBAL(volatile u32, 0x9cff4cu) = value;
    /* 00613d5f */ goto v_00613f4c;
v_00613d64:
    /* 00613d6a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dce78u);
    /* 00613d74 */ if (result != 0) goto v_00613d81;
    /* 00613d76 */ CM_GLOBAL(volatile u32, 0x9cff50u) = value;
    /* 00613d7c */ goto v_00613f4c;
v_00613d81:
    /* 00613d87 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dce5cu);
    /* 00613d91 */ if (result != 0) goto v_00613d9e;
    /* 00613d93 */ CM_GLOBAL(volatile u32, 0x9cff54u) = value;
    /* 00613d99 */ goto v_00613f4c;
v_00613d9e:
    /* 00613da4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dce34u);
    /* 00613dae */ if (result != 0) goto v_00613dbb;
    /* 00613db0 */ CM_GLOBAL(volatile u32, 0x9cff58u) = value;
    /* 00613db6 */ goto v_00613f4c;
v_00613dbb:
    /* 00613dc1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dce0cu);
    /* 00613dcb */ if (result != 0) goto v_00613dd8;
    /* 00613dcd */ CM_GLOBAL(volatile u32, 0x9cff5cu) = value;
    /* 00613dd3 */ goto v_00613f4c;
v_00613dd8:
    /* 00613dde */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcde8u);
    /* 00613de8 */ if (result != 0) goto v_00613df5;
    /* 00613dea */ CM_GLOBAL(volatile u32, 0x9cff60u) = value;
    /* 00613df0 */ goto v_00613f4c;
v_00613df5:
    /* 00613dfb */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcdc0u);
    /* 00613e05 */ if (result != 0) goto v_00613e12;
    /* 00613e07 */ CM_GLOBAL(volatile u32, 0x9cff64u) = value;
    /* 00613e0d */ goto v_00613f4c;
v_00613e12:
    /* 00613e18 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcd9cu);
    /* 00613e22 */ if (result != 0) goto v_00613e2f;
    /* 00613e24 */ CM_GLOBAL(volatile u32, 0x9cff68u) = value;
    /* 00613e2a */ goto v_00613f4c;
v_00613e2f:
    /* 00613e35 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcd78u);
    /* 00613e3f */ if (result != 0) goto v_00613e4c;
    /* 00613e41 */ CM_GLOBAL(volatile u32, 0x9cff6cu) = value;
    /* 00613e47 */ goto v_00613f4c;
v_00613e4c:
    /* 00613e52 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcd5cu);
    /* 00613e5c */ if (result != 0) goto v_00613e69;
    /* 00613e5e */ CM_GLOBAL(volatile u32, 0x9cff70u) = value;
    /* 00613e64 */ goto v_00613f4c;
v_00613e69:
    /* 00613e6f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcd30u);
    /* 00613e79 */ if (result != 0) goto v_00613e86;
    /* 00613e7b */ CM_GLOBAL(volatile u32, 0x9cff74u) = value;
    /* 00613e81 */ goto v_00613f4c;
v_00613e86:
    /* 00613e8c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcd14u);
    /* 00613e96 */ if (result != 0) goto v_00613ea3;
    /* 00613e98 */ CM_GLOBAL(volatile u32, 0x9cff78u) = value;
    /* 00613e9e */ goto v_00613f4c;
v_00613ea3:
    /* 00613ea9 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcce8u);
    /* 00613eb3 */ if (result != 0) goto v_00613ec0;
    /* 00613eb5 */ CM_GLOBAL(volatile u32, 0x9cff7cu) = value;
    /* 00613ebb */ goto v_00613f4c;
v_00613ec0:
    /* 00613ec6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dccb8u);
    /* 00613ed0 */ if (result != 0) goto v_00613eda;
    /* 00613ed2 */ CM_GLOBAL(volatile u32, 0x9cff80u) = value;
    /* 00613ed8 */ goto v_00613f4c;
v_00613eda:
    /* 00613ee0 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcc8cu);
    /* 00613eea */ if (result != 0) goto v_00613ef4;
    /* 00613eec */ CM_GLOBAL(volatile u32, 0x9cff84u) = value;
    /* 00613ef2 */ goto v_00613f4c;
v_00613ef4:
    /* 00613efa */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcc60u);
    /* 00613f04 */ if (result != 0) goto v_00613f4c;
    /* 00613f06 */ CM_GLOBAL(volatile u32, 0x9cff88u) = value;
    /* 00613f0c */ goto v_00613f4c;
v_00613f0e:
    /* 00613f0e */ CM_GLOBAL(volatile u32, 0x9cfe9cu) = value;
    /* 00613f14 */ goto v_00613f4c;
v_00613f16:
    /* 00613f16 */ CM_GLOBAL(volatile u32, 0x9cfe98u) = value;
    /* 00613f1c */ goto v_00613f4c;
v_00613f1e:
    /* 00613f1e */ CM_GLOBAL(volatile u32, 0x9cfe94u) = value;
    /* 00613f24 */ goto v_00613f4c;
v_00613f26:
    /* 00613f26 */ CM_GLOBAL(volatile u32, 0x9cfe90u) = value;
    /* 00613f2c */ goto v_00613f4c;
v_00613f2e:
    /* 00613f2e */ CM_GLOBAL(volatile u32, 0x9cfe8cu) = value;
    /* 00613f34 */ goto v_00613f4c;
v_00613f36:
    /* 00613f36 */ CM_GLOBAL(volatile u32, 0x9cfe88u) = value;
    /* 00613f3c */ goto v_00613f4c;
v_00613f3e:
    /* 00613f3e */ CM_GLOBAL(volatile u32, 0x9cfe84u) = value;
    /* 00613f44 */ goto v_00613f4c;
v_00613f46:
    /* 00613f46 */ CM_GLOBAL(volatile u32, 0x9cfe80u) = value;
v_00613f4c:
    /* 00613f52 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcc38u);
    /* 00613f5c */ if (result != 0) goto v_00613f67;
    /* 00613f5e */ CM_GLOBAL(volatile u32, 0x9cff8cu) = value;
    /* 00613f66 */ return result;
v_00613f67:
    /* 00613f6d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcc10u);
    /* 00613f77 */ if (result != 0) goto v_00613f82;
    /* 00613f79 */ CM_GLOBAL(volatile u32, 0x9cff90u) = value;
    /* 00613f81 */ return result;
v_00613f82:
    /* 00613f88 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcbe8u);
    /* 00613f92 */ if (result != 0) goto v_00613f9d;
    /* 00613f94 */ CM_GLOBAL(volatile u32, 0x9cff94u) = value;
    /* 00613f9c */ return result;
v_00613f9d:
    /* 00613fa3 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcbb8u);
    /* 00613fad */ if (result != 0) goto v_00613fb8;
    /* 00613faf */ CM_GLOBAL(volatile u32, 0x9cff98u) = value;
    /* 00613fb7 */ return result;
v_00613fb8:
    /* 00613fbe */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcb8cu);
    /* 00613fc8 */ if (result != 0) goto v_00613fd3;
    /* 00613fca */ CM_GLOBAL(volatile u32, 0x9cff9cu) = value;
    /* 00613fd2 */ return result;
v_00613fd3:
    /* 00613fd9 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcb64u);
    /* 00613fe3 */ if (result != 0) goto v_00613fee;
    /* 00613fe5 */ CM_GLOBAL(volatile u32, 0x9cffa0u) = value;
    /* 00613fed */ return result;
v_00613fee:
    /* 00613ff4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcb3cu);
    /* 00613ffe */ if (result != 0) goto v_00614009;
    /* 00614000 */ CM_GLOBAL(volatile u32, 0x9cffa4u) = value;
    /* 00614008 */ return result;
v_00614009:
    /* 0061400f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcb14u);
    /* 00614019 */ if (result != 0) goto v_00614024;
    /* 0061401b */ CM_GLOBAL(volatile u32, 0x9cffa8u) = value;
    /* 00614023 */ return result;
v_00614024:
    /* 0061402a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcae4u);
    /* 00614034 */ if (result != 0) goto v_0061403f;
    /* 00614036 */ CM_GLOBAL(volatile u32, 0x9cffacu) = value;
    /* 0061403e */ return result;
v_0061403f:
    /* 00614045 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dcab8u);
    /* 0061404f */ if (result != 0) goto v_0061405a;
    /* 00614051 */ CM_GLOBAL(volatile u32, 0x9cffb0u) = value;
    /* 00614059 */ return result;
v_0061405a:
    /* 00614060 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dca90u);
    /* 0061406a */ if (result != 0) goto v_00614075;
    /* 0061406c */ CM_GLOBAL(volatile u32, 0x9cffb4u) = value;
    /* 00614074 */ return result;
v_00614075:
    /* 0061407b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dca68u);
    /* 00614085 */ if (result != 0) goto v_00614090;
    /* 00614087 */ CM_GLOBAL(volatile u32, 0x9cffb8u) = value;
    /* 0061408f */ return result;
v_00614090:
    /* 00614096 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dca40u);
    /* 006140a0 */ if (result != 0) goto v_006140ab;
    /* 006140a2 */ CM_GLOBAL(volatile u32, 0x9cffbcu) = value;
    /* 006140aa */ return result;
v_006140ab:
    /* 006140b1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dca10u);
    /* 006140bb */ if (result != 0) goto v_006140c6;
    /* 006140bd */ CM_GLOBAL(volatile u32, 0x9cffc0u) = value;
    /* 006140c5 */ return result;
v_006140c6:
    /* 006140cc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc9f0u);
    /* 006140d6 */ if (result != 0) goto v_006140e1;
    /* 006140d8 */ CM_GLOBAL(volatile u32, 0x9cffc4u) = value;
    /* 006140e0 */ return result;
v_006140e1:
    /* 006140e7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc9d4u);
    /* 006140f1 */ if (result != 0) goto v_006140fc;
    /* 006140f3 */ CM_GLOBAL(volatile u32, 0x9cffc8u) = value;
    /* 006140fb */ return result;
v_006140fc:
    /* 00614102 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc9b0u);
    /* 0061410c */ if (result != 0) goto v_00614117;
    /* 0061410e */ CM_GLOBAL(volatile u32, 0x9cffccu) = value;
    /* 00614116 */ return result;
v_00614117:
    /* 0061411d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc990u);
    /* 00614127 */ if (result != 0) goto v_00614132;
    /* 00614129 */ CM_GLOBAL(volatile u32, 0x9cffd0u) = value;
    /* 00614131 */ return result;
v_00614132:
    /* 00614138 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc96cu);
    /* 00614142 */ if (result != 0) goto v_0061414d;
    /* 00614144 */ CM_GLOBAL(volatile u32, 0x9cffd4u) = value;
    /* 0061414c */ return result;
v_0061414d:
    /* 00614153 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc94cu);
    /* 0061415d */ if (result != 0) goto v_00614168;
    /* 0061415f */ CM_GLOBAL(volatile u32, 0x9cffd8u) = value;
    /* 00614167 */ return result;
v_00614168:
    /* 0061416e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc928u);
    /* 00614178 */ if (result != 0) goto v_00614183;
    /* 0061417a */ CM_GLOBAL(volatile u32, 0x9cffdcu) = value;
    /* 00614182 */ return result;
v_00614183:
    /* 00614189 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc904u);
    /* 00614193 */ if (result != 0) goto v_0061419e;
    /* 00614195 */ CM_GLOBAL(volatile u32, 0x9cffe0u) = value;
    /* 0061419d */ return result;
v_0061419e:
    /* 006141a4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc8e0u);
    /* 006141ae */ if (result != 0) goto v_006141b9;
    /* 006141b0 */ CM_GLOBAL(volatile u32, 0x9cffe4u) = value;
    /* 006141b8 */ return result;
v_006141b9:
    /* 006141bf */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc8c0u);
    /* 006141c9 */ if (result != 0) goto v_006141d4;
    /* 006141cb */ CM_GLOBAL(volatile u32, 0x9cffe8u) = value;
    /* 006141d3 */ return result;
v_006141d4:
    /* 006141da */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc89cu);
    /* 006141e4 */ if (result != 0) goto v_006141ef;
    /* 006141e6 */ CM_GLOBAL(volatile u32, 0x9cffecu) = value;
    /* 006141ee */ return result;
v_006141ef:
    /* 006141f5 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc87cu);
    /* 006141ff */ if (result != 0) goto v_0061420a;
    /* 00614201 */ CM_GLOBAL(volatile u32, 0x9cfff0u) = value;
    /* 00614209 */ return result;
v_0061420a:
    /* 00614210 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc85cu);
    /* 0061421a */ if (result != 0) goto v_00614225;
    /* 0061421c */ CM_GLOBAL(volatile u32, 0x9cfff4u) = value;
    /* 00614224 */ return result;
v_00614225:
    /* 0061422b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc840u);
    /* 00614235 */ if (result != 0) goto v_00614240;
    /* 00614237 */ CM_GLOBAL(volatile u32, 0x9cfff8u) = value;
    /* 0061423f */ return result;
v_00614240:
    /* 00614246 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc81cu);
    /* 00614250 */ if (result != 0) goto v_0061425b;
    /* 00614252 */ CM_GLOBAL(volatile u32, 0x9cfffcu) = value;
    /* 0061425a */ return result;
v_0061425b:
    /* 00614261 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc7fcu);
    /* 0061426b */ if (result != 0) goto v_00614276;
    /* 0061426d */ CM_GLOBAL(volatile u32, 0x9d0000u) = value;
    /* 00614275 */ return result;
v_00614276:
    /* 0061427c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc7d4u);
    /* 00614286 */ if (result != 0) goto v_00614291;
    /* 00614288 */ CM_GLOBAL(volatile u32, 0x9d0004u) = value;
    /* 00614290 */ return result;
v_00614291:
    /* 00614297 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc7b0u);
    /* 006142a1 */ if (result != 0) goto v_006142ac;
    /* 006142a3 */ CM_GLOBAL(volatile u32, 0x9d0008u) = value;
    /* 006142ab */ return result;
v_006142ac:
    /* 006142b2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc788u);
    /* 006142bc */ if (result != 0) goto v_006142c7;
    /* 006142be */ CM_GLOBAL(volatile u32, 0x9d000cu) = value;
    /* 006142c6 */ return result;
v_006142c7:
    /* 006142cd */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc760u);
    /* 006142d7 */ if (result != 0) goto v_006142e2;
    /* 006142d9 */ CM_GLOBAL(volatile u32, 0x9d0010u) = value;
    /* 006142e1 */ return result;
v_006142e2:
    /* 006142e8 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc740u);
    /* 006142f2 */ if (result != 0) goto v_006142fd;
    /* 006142f4 */ CM_GLOBAL(volatile u32, 0x9d0014u) = value;
    /* 006142fc */ return result;
v_006142fd:
    /* 00614303 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc724u);
    /* 0061430d */ if (result != 0) goto v_00614318;
    /* 0061430f */ CM_GLOBAL(volatile u32, 0x9d0018u) = value;
    /* 00614317 */ return result;
v_00614318:
    /* 0061431e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc700u);
    /* 00614328 */ if (result != 0) goto v_00614333;
    /* 0061432a */ CM_GLOBAL(volatile u32, 0x9d001cu) = value;
    /* 00614332 */ return result;
v_00614333:
    /* 00614339 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc6e0u);
    /* 00614343 */ if (result != 0) goto v_0061434e;
    /* 00614345 */ CM_GLOBAL(volatile u32, 0x9d0020u) = value;
    /* 0061434d */ return result;
v_0061434e:
    /* 00614354 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc6bcu);
    /* 0061435e */ if (result != 0) goto v_00614369;
    /* 00614360 */ CM_GLOBAL(volatile u32, 0x9d0024u) = value;
    /* 00614368 */ return result;
v_00614369:
    /* 0061436f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc69cu);
    /* 00614379 */ if (result != 0) goto v_00614384;
    /* 0061437b */ CM_GLOBAL(volatile u32, 0x9d0028u) = value;
    /* 00614383 */ return result;
v_00614384:
    /* 0061438a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc674u);
    /* 00614394 */ if (result != 0) goto v_0061439f;
    /* 00614396 */ CM_GLOBAL(volatile u32, 0x9d002cu) = value;
    /* 0061439e */ return result;
v_0061439f:
    /* 006143a5 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc650u);
    /* 006143af */ if (result != 0) goto v_006143ba;
    /* 006143b1 */ CM_GLOBAL(volatile u32, 0x9d0030u) = value;
    /* 006143b9 */ return result;
v_006143ba:
    /* 006143c0 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc62cu);
    /* 006143ca */ if (result != 0) goto v_006143d5;
    /* 006143cc */ CM_GLOBAL(volatile u32, 0x9d0034u) = value;
    /* 006143d4 */ return result;
v_006143d5:
    /* 006143db */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc60cu);
    /* 006143e5 */ if (result != 0) goto v_006143f0;
    /* 006143e7 */ CM_GLOBAL(volatile u32, 0x9d0038u) = value;
    /* 006143ef */ return result;
v_006143f0:
    /* 006143f6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc5e8u);
    /* 00614400 */ if (result != 0) goto v_0061440b;
    /* 00614402 */ CM_GLOBAL(volatile u32, 0x9d003cu) = value;
    /* 0061440a */ return result;
v_0061440b:
    /* 00614411 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc5c8u);
    /* 0061441b */ if (result != 0) goto v_00614426;
    /* 0061441d */ CM_GLOBAL(volatile u32, 0x9d0040u) = value;
    /* 00614425 */ return result;
v_00614426:
    /* 0061442c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc5a0u);
    /* 00614436 */ if (result != 0) goto v_00614441;
    /* 00614438 */ CM_GLOBAL(volatile u32, 0x9d0044u) = value;
    /* 00614440 */ return result;
v_00614441:
    /* 00614447 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc57cu);
    /* 00614451 */ if (result != 0) goto v_0061445c;
    /* 00614453 */ CM_GLOBAL(volatile u32, 0x9d0048u) = value;
    /* 0061445b */ return result;
v_0061445c:
    /* 00614462 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc554u);
    /* 0061446c */ if (result != 0) goto v_00614477;
    /* 0061446e */ CM_GLOBAL(volatile u32, 0x9d004cu) = value;
    /* 00614476 */ return result;
v_00614477:
    /* 0061447d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc530u);
    /* 00614487 */ if (result != 0) goto v_00614492;
    /* 00614489 */ CM_GLOBAL(volatile u32, 0x9d0050u) = value;
    /* 00614491 */ return result;
v_00614492:
    /* 00614498 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc504u);
    /* 006144a2 */ if (result != 0) goto v_006144ad;
    /* 006144a4 */ CM_GLOBAL(volatile u32, 0x9d0054u) = value;
    /* 006144ac */ return result;
v_006144ad:
    /* 006144b3 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc4dcu);
    /* 006144bd */ if (result != 0) goto v_006144c8;
    /* 006144bf */ CM_GLOBAL(volatile u32, 0x9d0058u) = value;
    /* 006144c7 */ return result;
v_006144c8:
    /* 006144ce */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc4b0u);
    /* 006144d8 */ if (result != 0) goto v_006144e3;
    /* 006144da */ CM_GLOBAL(volatile u32, 0x9d005cu) = value;
    /* 006144e2 */ return result;
v_006144e3:
    /* 006144e9 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc488u);
    /* 006144f3 */ if (result != 0) goto v_006144fe;
    /* 006144f5 */ CM_GLOBAL(volatile u32, 0x9d0060u) = value;
    /* 006144fd */ return result;
v_006144fe:
    /* 00614504 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc45cu);
    /* 0061450e */ if (result != 0) goto v_00614519;
    /* 00614510 */ CM_GLOBAL(volatile u32, 0x9d0064u) = value;
    /* 00614518 */ return result;
v_00614519:
    /* 0061451f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc434u);
    /* 00614529 */ if (result != 0) goto v_00614534;
    /* 0061452b */ CM_GLOBAL(volatile u32, 0x9d0068u) = value;
    /* 00614533 */ return result;
v_00614534:
    /* 0061453a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc408u);
    /* 00614544 */ if (result != 0) goto v_0061454f;
    /* 00614546 */ CM_GLOBAL(volatile u32, 0x9d006cu) = value;
    /* 0061454e */ return result;
v_0061454f:
    /* 00614555 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc3e0u);
    /* 0061455f */ if (result != 0) goto v_0061456a;
    /* 00614561 */ CM_GLOBAL(volatile u32, 0x9d0070u) = value;
    /* 00614569 */ return result;
v_0061456a:
    /* 00614570 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc3bcu);
    /* 0061457a */ if (result != 0) goto v_00614585;
    /* 0061457c */ CM_GLOBAL(volatile u32, 0x9d0074u) = value;
    /* 00614584 */ return result;
v_00614585:
    /* 0061458b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc39cu);
    /* 00614595 */ if (result != 0) goto v_006145a0;
    /* 00614597 */ CM_GLOBAL(volatile u32, 0x9d0078u) = value;
    /* 0061459f */ return result;
v_006145a0:
    /* 006145a6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc374u);
    /* 006145b0 */ if (result != 0) goto v_006145bb;
    /* 006145b2 */ CM_GLOBAL(volatile u32, 0x9d007cu) = value;
    /* 006145ba */ return result;
v_006145bb:
    /* 006145c1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc350u);
    /* 006145cb */ if (result != 0) goto v_006145d6;
    /* 006145cd */ CM_GLOBAL(volatile u32, 0x9d0080u) = value;
    /* 006145d5 */ return result;
v_006145d6:
    /* 006145dc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc32cu);
    /* 006145e6 */ if (result != 0) goto v_006145f1;
    /* 006145e8 */ CM_GLOBAL(volatile u32, 0x9d0084u) = value;
    /* 006145f0 */ return result;
v_006145f1:
    /* 006145f7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc304u);
    /* 00614601 */ if (result != 0) goto v_0061460c;
    /* 00614603 */ CM_GLOBAL(volatile u32, 0x9d0088u) = value;
    /* 0061460b */ return result;
v_0061460c:
    /* 00614612 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc2e0u);
    /* 0061461c */ if (result != 0) goto v_00614627;
    /* 0061461e */ CM_GLOBAL(volatile u32, 0x9d008cu) = value;
    /* 00614626 */ return result;
v_00614627:
    /* 0061462d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc2b0u);
    /* 00614637 */ if (result != 0) goto v_00614642;
    /* 00614639 */ CM_GLOBAL(volatile u32, 0x9d0090u) = value;
    /* 00614641 */ return result;
v_00614642:
    /* 00614648 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc28cu);
    /* 00614652 */ if (result != 0) goto v_0061465d;
    /* 00614654 */ CM_GLOBAL(volatile u32, 0x9d0094u) = value;
    /* 0061465c */ return result;
v_0061465d:
    /* 00614663 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc26cu);
    /* 0061466d */ if (result != 0) goto v_00614678;
    /* 0061466f */ CM_GLOBAL(volatile u32, 0x9d0098u) = value;
    /* 00614677 */ return result;
v_00614678:
    /* 0061467e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc240u);
    /* 00614688 */ if (result != 0) goto v_00614693;
    /* 0061468a */ CM_GLOBAL(volatile u32, 0x9d009cu) = value;
    /* 00614692 */ return result;
v_00614693:
    /* 00614699 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc218u);
    /* 006146a3 */ if (result != 0) goto v_006146ab;
    /* 006146a5 */ CM_GLOBAL(volatile u32, 0x9d00a0u) = value;
v_006146ab:
    /* 006146ad */ return result;
}
