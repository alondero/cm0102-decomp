/* Preferred-image VAs; base 00400000. CM0102 3.9.68 SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * Three consecutive comparison groups; shared stores and continuation order
 * follow the native branches. The original comparator remains a boundary.
 * All indexed callers discard EAX; retain the last full comparator result.
 */
#include "cmre.h"

extern s32 CM_CDECL cm_fn_00951c50(const volatile u8 *first, const volatile u8 *second);

s32 CM_CDECL cm_fn_0060efd0(const volatile u8 *name, u32 identifier)
{
    s32 result;
    u32 value;
    /* 0060efdc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc200u);
    /* 0060efe1 */ value = identifier;
    /* 0060efea */ if (result != 0) goto v_0060eff7;
    /* 0060efec */ CM_GLOBAL(volatile u32, 0x9cf548u) = value;
    /* 0060eff2 */ goto v_0060fd31;
v_0060eff7:
    /* 0060effd */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc1e8u);
    /* 0060f007 */ if (result != 0) goto v_0060f014;
    /* 0060f009 */ CM_GLOBAL(volatile u32, 0x9cf54cu) = value;
    /* 0060f00f */ goto v_0060fd31;
v_0060f014:
    /* 0060f01a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc1d0u);
    /* 0060f024 */ if (result != 0) goto v_0060f031;
    /* 0060f026 */ CM_GLOBAL(volatile u32, 0x9cf550u) = value;
    /* 0060f02c */ goto v_0060fd31;
v_0060f031:
    /* 0060f037 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc1a8u);
    /* 0060f041 */ if (result != 0) goto v_0060f04e;
    /* 0060f043 */ CM_GLOBAL(volatile u32, 0x9cf554u) = value;
    /* 0060f049 */ goto v_0060fd31;
v_0060f04e:
    /* 0060f054 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc188u);
    /* 0060f05e */ if (result != 0) goto v_0060f06b;
    /* 0060f060 */ CM_GLOBAL(volatile u32, 0x9cf558u) = value;
    /* 0060f066 */ goto v_0060fd31;
v_0060f06b:
    /* 0060f071 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc168u);
    /* 0060f07b */ if (result != 0) goto v_0060f088;
    /* 0060f07d */ CM_GLOBAL(volatile u32, 0x9cf55cu) = value;
    /* 0060f083 */ goto v_0060fd31;
v_0060f088:
    /* 0060f08e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc148u);
    /* 0060f098 */ if (result != 0) goto v_0060f0a5;
    /* 0060f09a */ CM_GLOBAL(volatile u32, 0x9cf560u) = value;
    /* 0060f0a0 */ goto v_0060fd31;
v_0060f0a5:
    /* 0060f0ab */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc138u);
    /* 0060f0b5 */ if (result != 0) goto v_0060f0c2;
    /* 0060f0b7 */ CM_GLOBAL(volatile u32, 0x9cf564u) = value;
    /* 0060f0bd */ goto v_0060fd31;
v_0060f0c2:
    /* 0060f0c8 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc11cu);
    /* 0060f0d2 */ if (result == 0) goto v_0060fd2b;
    /* 0060f0de */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc104u);
    /* 0060f0e8 */ if (result == 0) goto v_0060fd2b;
    /* 0060f0f4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc0ecu);
    /* 0060f0fe */ if (result == 0) goto v_0060fd2b;
    /* 0060f10a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc0d8u);
    /* 0060f114 */ if (result == 0) goto v_0060fd23;
    /* 0060f120 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc0c0u);
    /* 0060f12a */ if (result == 0) goto v_0060fd23;
    /* 0060f136 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc0b0u);
    /* 0060f140 */ if (result != 0) goto v_0060f14d;
    /* 0060f142 */ CM_GLOBAL(volatile u32, 0x9cf570u) = value;
    /* 0060f148 */ goto v_0060fd31;
v_0060f14d:
    /* 0060f153 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc0a0u);
    /* 0060f15d */ if (result != 0) goto v_0060f16a;
    /* 0060f15f */ CM_GLOBAL(volatile u32, 0x9cf574u) = value;
    /* 0060f165 */ goto v_0060fd31;
v_0060f16a:
    /* 0060f170 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc08cu);
    /* 0060f17a */ if (result != 0) goto v_0060f187;
    /* 0060f17c */ CM_GLOBAL(volatile u32, 0x9cf578u) = value;
    /* 0060f182 */ goto v_0060fd31;
v_0060f187:
    /* 0060f18d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc078u);
    /* 0060f197 */ if (result != 0) goto v_0060f1a4;
    /* 0060f199 */ CM_GLOBAL(volatile u32, 0x9cf57cu) = value;
    /* 0060f19f */ goto v_0060fd31;
v_0060f1a4:
    /* 0060f1aa */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc064u);
    /* 0060f1b4 */ if (result != 0) goto v_0060f1c1;
    /* 0060f1b6 */ CM_GLOBAL(volatile u32, 0x9cf580u) = value;
    /* 0060f1bc */ goto v_0060fd31;
v_0060f1c1:
    /* 0060f1c7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc050u);
    /* 0060f1d1 */ if (result != 0) goto v_0060f1de;
    /* 0060f1d3 */ CM_GLOBAL(volatile u32, 0x9cf584u) = value;
    /* 0060f1d9 */ goto v_0060fd31;
v_0060f1de:
    /* 0060f1e4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc03cu);
    /* 0060f1ee */ if (result != 0) goto v_0060f1fb;
    /* 0060f1f0 */ CM_GLOBAL(volatile u32, 0x9cf588u) = value;
    /* 0060f1f6 */ goto v_0060fd31;
v_0060f1fb:
    /* 0060f201 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc02cu);
    /* 0060f20b */ if (result != 0) goto v_0060f218;
    /* 0060f20d */ CM_GLOBAL(volatile u32, 0x9cf58cu) = value;
    /* 0060f213 */ goto v_0060fd31;
v_0060f218:
    /* 0060f21e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc014u);
    /* 0060f228 */ if (result != 0) goto v_0060f235;
    /* 0060f22a */ CM_GLOBAL(volatile u32, 0x9cf590u) = value;
    /* 0060f230 */ goto v_0060fd31;
v_0060f235:
    /* 0060f23b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dc000u);
    /* 0060f245 */ if (result != 0) goto v_0060f252;
    /* 0060f247 */ CM_GLOBAL(volatile u32, 0x9cf594u) = value;
    /* 0060f24d */ goto v_0060fd31;
v_0060f252:
    /* 0060f258 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbff0u);
    /* 0060f262 */ if (result == 0) goto v_0060fd1b;
    /* 0060f26e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbfd8u);
    /* 0060f278 */ if (result == 0) goto v_0060fd1b;
    /* 0060f284 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbfc0u);
    /* 0060f28e */ if (result != 0) goto v_0060f29b;
    /* 0060f290 */ CM_GLOBAL(volatile u32, 0x9cf59cu) = value;
    /* 0060f296 */ goto v_0060fd31;
v_0060f29b:
    /* 0060f2a1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbfa8u);
    /* 0060f2ab */ if (result != 0) goto v_0060f2b8;
    /* 0060f2ad */ CM_GLOBAL(volatile u32, 0x9cf5a0u) = value;
    /* 0060f2b3 */ goto v_0060fd31;
v_0060f2b8:
    /* 0060f2be */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbf8cu);
    /* 0060f2c8 */ if (result != 0) goto v_0060f2d5;
    /* 0060f2ca */ CM_GLOBAL(volatile u32, 0x9cf5a4u) = value;
    /* 0060f2d0 */ goto v_0060fd31;
v_0060f2d5:
    /* 0060f2db */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbf70u);
    /* 0060f2e5 */ if (result != 0) goto v_0060f2f2;
    /* 0060f2e7 */ CM_GLOBAL(volatile u32, 0x9cf5a8u) = value;
    /* 0060f2ed */ goto v_0060fd31;
v_0060f2f2:
    /* 0060f2f8 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbf58u);
    /* 0060f302 */ if (result != 0) goto v_0060f30f;
    /* 0060f304 */ CM_GLOBAL(volatile u32, 0x9cf5acu) = value;
    /* 0060f30a */ goto v_0060fd31;
v_0060f30f:
    /* 0060f315 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbf40u);
    /* 0060f31f */ if (result != 0) goto v_0060f32c;
    /* 0060f321 */ CM_GLOBAL(volatile u32, 0x9cf5b0u) = value;
    /* 0060f327 */ goto v_0060fd31;
v_0060f32c:
    /* 0060f332 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbf28u);
    /* 0060f33c */ if (result != 0) goto v_0060f349;
    /* 0060f33e */ CM_GLOBAL(volatile u32, 0x9cf5b4u) = value;
    /* 0060f344 */ goto v_0060fd31;
v_0060f349:
    /* 0060f34f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbf0cu);
    /* 0060f359 */ if (result != 0) goto v_0060f366;
    /* 0060f35b */ CM_GLOBAL(volatile u32, 0x9cf5b8u) = value;
    /* 0060f361 */ goto v_0060fd31;
v_0060f366:
    /* 0060f36c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbef0u);
    /* 0060f376 */ if (result != 0) goto v_0060f383;
    /* 0060f378 */ CM_GLOBAL(volatile u32, 0x9cf5bcu) = value;
    /* 0060f37e */ goto v_0060fd31;
v_0060f383:
    /* 0060f389 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbed8u);
    /* 0060f393 */ if (result != 0) goto v_0060f3a0;
    /* 0060f395 */ CM_GLOBAL(volatile u32, 0x9cf5c0u) = value;
    /* 0060f39b */ goto v_0060fd31;
v_0060f3a0:
    /* 0060f3a6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbec0u);
    /* 0060f3b0 */ if (result != 0) goto v_0060f3bd;
    /* 0060f3b2 */ CM_GLOBAL(volatile u32, 0x9cf5c4u) = value;
    /* 0060f3b8 */ goto v_0060fd31;
v_0060f3bd:
    /* 0060f3c3 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbea8u);
    /* 0060f3cd */ if (result != 0) goto v_0060f3da;
    /* 0060f3cf */ CM_GLOBAL(volatile u32, 0x9cf5c8u) = value;
    /* 0060f3d5 */ goto v_0060fd31;
v_0060f3da:
    /* 0060f3e0 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbe90u);
    /* 0060f3ea */ if (result != 0) goto v_0060f3f7;
    /* 0060f3ec */ CM_GLOBAL(volatile u32, 0x9cf5ccu) = value;
    /* 0060f3f2 */ goto v_0060fd31;
v_0060f3f7:
    /* 0060f3fd */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbe78u);
    /* 0060f407 */ if (result != 0) goto v_0060f414;
    /* 0060f409 */ CM_GLOBAL(volatile u32, 0x9cf5d0u) = value;
    /* 0060f40f */ goto v_0060fd31;
v_0060f414:
    /* 0060f41a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbe68u);
    /* 0060f424 */ if (result != 0) goto v_0060f431;
    /* 0060f426 */ CM_GLOBAL(volatile u32, 0x9cf5d4u) = value;
    /* 0060f42c */ goto v_0060fd31;
v_0060f431:
    /* 0060f437 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbe5cu);
    /* 0060f441 */ if (result == 0) goto v_0060fd13;
    /* 0060f44d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbe4cu);
    /* 0060f457 */ if (result == 0) goto v_0060fd13;
    /* 0060f463 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbe30u);
    /* 0060f46d */ if (result != 0) goto v_0060f47a;
    /* 0060f46f */ CM_GLOBAL(volatile u32, 0x9cf5dcu) = value;
    /* 0060f475 */ goto v_0060fd31;
v_0060f47a:
    /* 0060f480 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbe18u);
    /* 0060f48a */ if (result != 0) goto v_0060f497;
    /* 0060f48c */ CM_GLOBAL(volatile u32, 0x9cf5e0u) = value;
    /* 0060f492 */ goto v_0060fd31;
v_0060f497:
    /* 0060f49d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbdfcu);
    /* 0060f4a7 */ if (result != 0) goto v_0060f4b4;
    /* 0060f4a9 */ CM_GLOBAL(volatile u32, 0x9cf5e4u) = value;
    /* 0060f4af */ goto v_0060fd31;
v_0060f4b4:
    /* 0060f4ba */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbde4u);
    /* 0060f4c4 */ if (result != 0) goto v_0060f4d1;
    /* 0060f4c6 */ CM_GLOBAL(volatile u32, 0x9cf5e8u) = value;
    /* 0060f4cc */ goto v_0060fd31;
v_0060f4d1:
    /* 0060f4d7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbdc8u);
    /* 0060f4e1 */ if (result != 0) goto v_0060f4ee;
    /* 0060f4e3 */ CM_GLOBAL(volatile u32, 0x9cf5ecu) = value;
    /* 0060f4e9 */ goto v_0060fd31;
v_0060f4ee:
    /* 0060f4f4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbdb0u);
    /* 0060f4fe */ if (result == 0) goto v_0060fd0b;
    /* 0060f50a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbd90u);
    /* 0060f514 */ if (result == 0) goto v_0060fd0b;
    /* 0060f520 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbd68u);
    /* 0060f52a */ if (result != 0) goto v_0060f537;
    /* 0060f52c */ CM_GLOBAL(volatile u32, 0x9cf5f4u) = value;
    /* 0060f532 */ goto v_0060fd31;
v_0060f537:
    /* 0060f53d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbd48u);
    /* 0060f547 */ if (result != 0) goto v_0060f554;
    /* 0060f549 */ CM_GLOBAL(volatile u32, 0x9cf5f8u) = value;
    /* 0060f54f */ goto v_0060fd31;
v_0060f554:
    /* 0060f55a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbd20u);
    /* 0060f564 */ if (result != 0) goto v_0060f571;
    /* 0060f566 */ CM_GLOBAL(volatile u32, 0x9cf5fcu) = value;
    /* 0060f56c */ goto v_0060fd31;
v_0060f571:
    /* 0060f577 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbcf8u);
    /* 0060f581 */ if (result != 0) goto v_0060f58e;
    /* 0060f583 */ CM_GLOBAL(volatile u32, 0x9cf600u) = value;
    /* 0060f589 */ goto v_0060fd31;
v_0060f58e:
    /* 0060f594 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbcd0u);
    /* 0060f59e */ if (result != 0) goto v_0060f5ab;
    /* 0060f5a0 */ CM_GLOBAL(volatile u32, 0x9cf604u) = value;
    /* 0060f5a6 */ goto v_0060fd31;
v_0060f5ab:
    /* 0060f5b1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbca8u);
    /* 0060f5bb */ if (result != 0) goto v_0060f5c8;
    /* 0060f5bd */ CM_GLOBAL(volatile u32, 0x9cf608u) = value;
    /* 0060f5c3 */ goto v_0060fd31;
v_0060f5c8:
    /* 0060f5ce */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbc8cu);
    /* 0060f5d8 */ if (result == 0) goto v_0060fd03;
    /* 0060f5e4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbc70u);
    /* 0060f5ee */ if (result == 0) goto v_0060fd03;
    /* 0060f5fa */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbc54u);
    /* 0060f604 */ if (result == 0) goto v_0060fcfb;
    /* 0060f610 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbc38u);
    /* 0060f61a */ if (result == 0) goto v_0060fcfb;
    /* 0060f626 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbc14u);
    /* 0060f630 */ if (result != 0) goto v_0060f63d;
    /* 0060f632 */ CM_GLOBAL(volatile u32, 0x9cf614u) = value;
    /* 0060f638 */ goto v_0060fd31;
v_0060f63d:
    /* 0060f643 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbbecu);
    /* 0060f64d */ if (result != 0) goto v_0060f65a;
    /* 0060f64f */ CM_GLOBAL(volatile u32, 0x9cf618u) = value;
    /* 0060f655 */ goto v_0060fd31;
v_0060f65a:
    /* 0060f660 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbbc8u);
    /* 0060f66a */ if (result != 0) goto v_0060f677;
    /* 0060f66c */ CM_GLOBAL(volatile u32, 0x9cf61cu) = value;
    /* 0060f672 */ goto v_0060fd31;
v_0060f677:
    /* 0060f67d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbbacu);
    /* 0060f687 */ if (result != 0) goto v_0060f694;
    /* 0060f689 */ CM_GLOBAL(volatile u32, 0x9cf620u) = value;
    /* 0060f68f */ goto v_0060fd31;
v_0060f694:
    /* 0060f69a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbb94u);
    /* 0060f6a4 */ if (result != 0) goto v_0060f6b1;
    /* 0060f6a6 */ CM_GLOBAL(volatile u32, 0x9cf624u) = value;
    /* 0060f6ac */ goto v_0060fd31;
v_0060f6b1:
    /* 0060f6b7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbb7cu);
    /* 0060f6c1 */ if (result != 0) goto v_0060f6ce;
    /* 0060f6c3 */ CM_GLOBAL(volatile u32, 0x9cf628u) = value;
    /* 0060f6c9 */ goto v_0060fd31;
v_0060f6ce:
    /* 0060f6d4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbb60u);
    /* 0060f6de */ if (result != 0) goto v_0060f6eb;
    /* 0060f6e0 */ CM_GLOBAL(volatile u32, 0x9cf62cu) = value;
    /* 0060f6e6 */ goto v_0060fd31;
v_0060f6eb:
    /* 0060f6f1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbb44u);
    /* 0060f6fb */ if (result != 0) goto v_0060f708;
    /* 0060f6fd */ CM_GLOBAL(volatile u32, 0x9cf630u) = value;
    /* 0060f703 */ goto v_0060fd31;
v_0060f708:
    /* 0060f70e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbb28u);
    /* 0060f718 */ if (result != 0) goto v_0060f725;
    /* 0060f71a */ CM_GLOBAL(volatile u32, 0x9cf634u) = value;
    /* 0060f720 */ goto v_0060fd31;
v_0060f725:
    /* 0060f72b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbb0cu);
    /* 0060f735 */ if (result != 0) goto v_0060f742;
    /* 0060f737 */ CM_GLOBAL(volatile u32, 0x9cf638u) = value;
    /* 0060f73d */ goto v_0060fd31;
v_0060f742:
    /* 0060f748 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbae8u);
    /* 0060f752 */ if (result != 0) goto v_0060f75f;
    /* 0060f754 */ CM_GLOBAL(volatile u32, 0x9cf63cu) = value;
    /* 0060f75a */ goto v_0060fd31;
v_0060f75f:
    /* 0060f765 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbaccu);
    /* 0060f76f */ if (result != 0) goto v_0060f77c;
    /* 0060f771 */ CM_GLOBAL(volatile u32, 0x9cf640u) = value;
    /* 0060f777 */ goto v_0060fd31;
v_0060f77c:
    /* 0060f782 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbab0u);
    /* 0060f78c */ if (result != 0) goto v_0060f799;
    /* 0060f78e */ CM_GLOBAL(volatile u32, 0x9cf644u) = value;
    /* 0060f794 */ goto v_0060fd31;
v_0060f799:
    /* 0060f79f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dba98u);
    /* 0060f7a9 */ if (result != 0) goto v_0060f7b6;
    /* 0060f7ab */ CM_GLOBAL(volatile u32, 0x9cf648u) = value;
    /* 0060f7b1 */ goto v_0060fd31;
v_0060f7b6:
    /* 0060f7bc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dba84u);
    /* 0060f7c6 */ if (result == 0) goto v_0060fcf3;
    /* 0060f7d2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dba6cu);
    /* 0060f7dc */ if (result == 0) goto v_0060fcf3;
    /* 0060f7e8 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dba54u);
    /* 0060f7f2 */ if (result == 0) goto v_0060fceb;
    /* 0060f7fe */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dba3cu);
    /* 0060f808 */ if (result == 0) goto v_0060fceb;
    /* 0060f814 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dba20u);
    /* 0060f81e */ if (result != 0) goto v_0060f82b;
    /* 0060f820 */ CM_GLOBAL(volatile u32, 0x9cf654u) = value;
    /* 0060f826 */ goto v_0060fd31;
v_0060f82b:
    /* 0060f831 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dba04u);
    /* 0060f83b */ if (result != 0) goto v_0060f848;
    /* 0060f83d */ CM_GLOBAL(volatile u32, 0x9cf658u) = value;
    /* 0060f843 */ goto v_0060fd31;
v_0060f848:
    /* 0060f84e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db9f0u);
    /* 0060f858 */ if (result == 0) goto v_0060fce3;
    /* 0060f864 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db9dcu);
    /* 0060f86e */ if (result == 0) goto v_0060fce3;
    /* 0060f87a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db9c4u);
    /* 0060f884 */ if (result == 0) goto v_0060fcdb;
    /* 0060f890 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db9acu);
    /* 0060f89a */ if (result == 0) goto v_0060fcdb;
    /* 0060f8a6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db98cu);
    /* 0060f8b0 */ if (result == 0) goto v_0060fcd3;
    /* 0060f8bc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db97cu);
    /* 0060f8c6 */ if (result == 0) goto v_0060fcd3;
    /* 0060f8d2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db96cu);
    /* 0060f8dc */ if (result == 0) goto v_0060fcd3;
    /* 0060f8e8 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db964u);
    /* 0060f8f2 */ if (result == 0) goto v_0060fcd3;
    /* 0060f8fe */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db958u);
    /* 0060f908 */ if (result != 0) goto v_0060f915;
    /* 0060f90a */ CM_GLOBAL(volatile u32, 0x9cf668u) = value;
    /* 0060f910 */ goto v_0060fd31;
v_0060f915:
    /* 0060f91b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db93cu);
    /* 0060f925 */ if (result == 0) goto v_0060fccb;
    /* 0060f931 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db928u);
    /* 0060f93b */ if (result == 0) goto v_0060fccb;
    /* 0060f947 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db910u);
    /* 0060f951 */ if (result == 0) goto v_0060fccb;
    /* 0060f95d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db8f4u);
    /* 0060f967 */ if (result == 0) goto v_0060fcc3;
    /* 0060f973 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db8e0u);
    /* 0060f97d */ if (result == 0) goto v_0060fcc3;
    /* 0060f989 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db8c4u);
    /* 0060f993 */ if (result != 0) goto v_0060f9a0;
    /* 0060f995 */ CM_GLOBAL(volatile u32, 0x9cf674u) = value;
    /* 0060f99b */ goto v_0060fd31;
v_0060f9a0:
    /* 0060f9a6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db8a0u);
    /* 0060f9b0 */ if (result != 0) goto v_0060f9bd;
    /* 0060f9b2 */ CM_GLOBAL(volatile u32, 0x9cf678u) = value;
    /* 0060f9b8 */ goto v_0060fd31;
v_0060f9bd:
    /* 0060f9c3 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db884u);
    /* 0060f9cd */ if (result != 0) goto v_0060f9da;
    /* 0060f9cf */ CM_GLOBAL(volatile u32, 0x9cf67cu) = value;
    /* 0060f9d5 */ goto v_0060fd31;
v_0060f9da:
    /* 0060f9e0 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db86cu);
    /* 0060f9ea */ if (result != 0) goto v_0060f9f7;
    /* 0060f9ec */ CM_GLOBAL(volatile u32, 0x9cf680u) = value;
    /* 0060f9f2 */ goto v_0060fd31;
v_0060f9f7:
    /* 0060f9fd */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9b5b40u);
    /* 0060fa07 */ if (result != 0) goto v_0060fa14;
    /* 0060fa09 */ CM_GLOBAL(volatile u32, 0x9cf684u) = value;
    /* 0060fa0f */ goto v_0060fd31;
v_0060fa14:
    /* 0060fa1a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db850u);
    /* 0060fa24 */ if (result != 0) goto v_0060fa31;
    /* 0060fa26 */ CM_GLOBAL(volatile u32, 0x9cf688u) = value;
    /* 0060fa2c */ goto v_0060fd31;
v_0060fa31:
    /* 0060fa37 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db838u);
    /* 0060fa41 */ if (result == 0) goto v_0060fcbb;
    /* 0060fa4d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db828u);
    /* 0060fa57 */ if (result == 0) goto v_0060fcbb;
    /* 0060fa63 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db810u);
    /* 0060fa6d */ if (result != 0) goto v_0060fa7a;
    /* 0060fa6f */ CM_GLOBAL(volatile u32, 0x9cf690u) = value;
    /* 0060fa75 */ goto v_0060fd31;
v_0060fa7a:
    /* 0060fa80 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db7fcu);
    /* 0060fa8a */ if (result != 0) goto v_0060fa97;
    /* 0060fa8c */ CM_GLOBAL(volatile u32, 0x9cf694u) = value;
    /* 0060fa92 */ goto v_0060fd31;
v_0060fa97:
    /* 0060fa9d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db7dcu);
    /* 0060faa7 */ if (result != 0) goto v_0060fab4;
    /* 0060faa9 */ CM_GLOBAL(volatile u32, 0x9cf698u) = value;
    /* 0060faaf */ goto v_0060fd31;
v_0060fab4:
    /* 0060faba */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db7c8u);
    /* 0060fac4 */ if (result != 0) goto v_0060fad1;
    /* 0060fac6 */ CM_GLOBAL(volatile u32, 0x9cf69cu) = value;
    /* 0060facc */ goto v_0060fd31;
v_0060fad1:
    /* 0060fad7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db7b4u);
    /* 0060fae1 */ if (result != 0) goto v_0060faee;
    /* 0060fae3 */ CM_GLOBAL(volatile u32, 0x9cf6a0u) = value;
    /* 0060fae9 */ goto v_0060fd31;
v_0060faee:
    /* 0060faf4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db79cu);
    /* 0060fafe */ if (result != 0) goto v_0060fb0b;
    /* 0060fb00 */ CM_GLOBAL(volatile u32, 0x9cf6a4u) = value;
    /* 0060fb06 */ goto v_0060fd31;
v_0060fb0b:
    /* 0060fb11 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db784u);
    /* 0060fb1b */ if (result != 0) goto v_0060fb28;
    /* 0060fb1d */ CM_GLOBAL(volatile u32, 0x9cf6a8u) = value;
    /* 0060fb23 */ goto v_0060fd31;
v_0060fb28:
    /* 0060fb2e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db76cu);
    /* 0060fb38 */ if (result != 0) goto v_0060fb45;
    /* 0060fb3a */ CM_GLOBAL(volatile u32, 0x9cf6acu) = value;
    /* 0060fb40 */ goto v_0060fd31;
v_0060fb45:
    /* 0060fb4b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db754u);
    /* 0060fb55 */ if (result != 0) goto v_0060fb62;
    /* 0060fb57 */ CM_GLOBAL(volatile u32, 0x9cf6b0u) = value;
    /* 0060fb5d */ goto v_0060fd31;
v_0060fb62:
    /* 0060fb68 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db73cu);
    /* 0060fb72 */ if (result != 0) goto v_0060fb7f;
    /* 0060fb74 */ CM_GLOBAL(volatile u32, 0x9cf6b4u) = value;
    /* 0060fb7a */ goto v_0060fd31;
v_0060fb7f:
    /* 0060fb85 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db728u);
    /* 0060fb8f */ if (result != 0) goto v_0060fb9c;
    /* 0060fb91 */ CM_GLOBAL(volatile u32, 0x9cf6b8u) = value;
    /* 0060fb97 */ goto v_0060fd31;
v_0060fb9c:
    /* 0060fba2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db70cu);
    /* 0060fbac */ if (result != 0) goto v_0060fbb9;
    /* 0060fbae */ CM_GLOBAL(volatile u32, 0x9cf6bcu) = value;
    /* 0060fbb4 */ goto v_0060fd31;
v_0060fbb9:
    /* 0060fbbf */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db6f0u);
    /* 0060fbc9 */ if (result != 0) goto v_0060fbd6;
    /* 0060fbcb */ CM_GLOBAL(volatile u32, 0x9cf6c0u) = value;
    /* 0060fbd1 */ goto v_0060fd31;
v_0060fbd6:
    /* 0060fbdc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db6ccu);
    /* 0060fbe6 */ if (result != 0) goto v_0060fbf3;
    /* 0060fbe8 */ CM_GLOBAL(volatile u32, 0x9cf6c4u) = value;
    /* 0060fbee */ goto v_0060fd31;
v_0060fbf3:
    /* 0060fbf9 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db6a8u);
    /* 0060fc03 */ if (result != 0) goto v_0060fc10;
    /* 0060fc05 */ CM_GLOBAL(volatile u32, 0x9cf6c8u) = value;
    /* 0060fc0b */ goto v_0060fd31;
v_0060fc10:
    /* 0060fc16 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db684u);
    /* 0060fc20 */ if (result != 0) goto v_0060fc2d;
    /* 0060fc22 */ CM_GLOBAL(volatile u32, 0x9cf6ccu) = value;
    /* 0060fc28 */ goto v_0060fd31;
v_0060fc2d:
    /* 0060fc33 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db660u);
    /* 0060fc3d */ if (result != 0) goto v_0060fc4a;
    /* 0060fc3f */ CM_GLOBAL(volatile u32, 0x9cf6d0u) = value;
    /* 0060fc45 */ goto v_0060fd31;
v_0060fc4a:
    /* 0060fc50 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db63cu);
    /* 0060fc5a */ if (result != 0) goto v_0060fc67;
    /* 0060fc5c */ CM_GLOBAL(volatile u32, 0x9cf6d4u) = value;
    /* 0060fc62 */ goto v_0060fd31;
v_0060fc67:
    /* 0060fc6d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db618u);
    /* 0060fc77 */ if (result != 0) goto v_0060fc84;
    /* 0060fc79 */ CM_GLOBAL(volatile u32, 0x9cf6d8u) = value;
    /* 0060fc7f */ goto v_0060fd31;
v_0060fc84:
    /* 0060fc8a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db5f4u);
    /* 0060fc94 */ if (result != 0) goto v_0060fca1;
    /* 0060fc96 */ CM_GLOBAL(volatile u32, 0x9cf6dcu) = value;
    /* 0060fc9c */ goto v_0060fd31;
v_0060fca1:
    /* 0060fca7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db5d0u);
    /* 0060fcb1 */ if (result != 0) goto v_0060fd31;
    /* 0060fcb3 */ CM_GLOBAL(volatile u32, 0x9cf6e0u) = value;
    /* 0060fcb9 */ goto v_0060fd31;
v_0060fcbb:
    /* 0060fcbb */ CM_GLOBAL(volatile u32, 0x9cf68cu) = value;
    /* 0060fcc1 */ goto v_0060fd31;
v_0060fcc3:
    /* 0060fcc3 */ CM_GLOBAL(volatile u32, 0x9cf670u) = value;
    /* 0060fcc9 */ goto v_0060fd31;
v_0060fccb:
    /* 0060fccb */ CM_GLOBAL(volatile u32, 0x9cf66cu) = value;
    /* 0060fcd1 */ goto v_0060fd31;
v_0060fcd3:
    /* 0060fcd3 */ CM_GLOBAL(volatile u32, 0x9cf664u) = value;
    /* 0060fcd9 */ goto v_0060fd31;
v_0060fcdb:
    /* 0060fcdb */ CM_GLOBAL(volatile u32, 0x9cf660u) = value;
    /* 0060fce1 */ goto v_0060fd31;
v_0060fce3:
    /* 0060fce3 */ CM_GLOBAL(volatile u32, 0x9cf65cu) = value;
    /* 0060fce9 */ goto v_0060fd31;
v_0060fceb:
    /* 0060fceb */ CM_GLOBAL(volatile u32, 0x9cf650u) = value;
    /* 0060fcf1 */ goto v_0060fd31;
v_0060fcf3:
    /* 0060fcf3 */ CM_GLOBAL(volatile u32, 0x9cf64cu) = value;
    /* 0060fcf9 */ goto v_0060fd31;
v_0060fcfb:
    /* 0060fcfb */ CM_GLOBAL(volatile u32, 0x9cf610u) = value;
    /* 0060fd01 */ goto v_0060fd31;
v_0060fd03:
    /* 0060fd03 */ CM_GLOBAL(volatile u32, 0x9cf60cu) = value;
    /* 0060fd09 */ goto v_0060fd31;
v_0060fd0b:
    /* 0060fd0b */ CM_GLOBAL(volatile u32, 0x9cf5f0u) = value;
    /* 0060fd11 */ goto v_0060fd31;
v_0060fd13:
    /* 0060fd13 */ CM_GLOBAL(volatile u32, 0x9cf5d8u) = value;
    /* 0060fd19 */ goto v_0060fd31;
v_0060fd1b:
    /* 0060fd1b */ CM_GLOBAL(volatile u32, 0x9cf598u) = value;
    /* 0060fd21 */ goto v_0060fd31;
v_0060fd23:
    /* 0060fd23 */ CM_GLOBAL(volatile u32, 0x9cf56cu) = value;
    /* 0060fd29 */ goto v_0060fd31;
v_0060fd2b:
    /* 0060fd2b */ CM_GLOBAL(volatile u32, 0x9cf568u) = value;
v_0060fd31:
    /* 0060fd37 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db5c0u);
    /* 0060fd41 */ if (result == 0) goto v_00610d9a;
    /* 0060fd4d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db5a8u);
    /* 0060fd57 */ if (result == 0) goto v_00610d9a;
    /* 0060fd63 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db588u);
    /* 0060fd6d */ if (result == 0) goto v_00610d92;
    /* 0060fd79 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db570u);
    /* 0060fd83 */ if (result == 0) goto v_00610d92;
    /* 0060fd8f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db558u);
    /* 0060fd99 */ if (result == 0) goto v_00610d92;
    /* 0060fda5 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db53cu);
    /* 0060fdaf */ if (result != 0) goto v_0060fdbc;
    /* 0060fdb1 */ CM_GLOBAL(volatile u32, 0x9cf6ecu) = value;
    /* 0060fdb7 */ goto v_00610da0;
v_0060fdbc:
    /* 0060fdc2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db530u);
    /* 0060fdcc */ if (result != 0) goto v_0060fdd9;
    /* 0060fdce */ CM_GLOBAL(volatile u32, 0x9cf6f0u) = value;
    /* 0060fdd4 */ goto v_00610da0;
v_0060fdd9:
    /* 0060fddf */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db51cu);
    /* 0060fde9 */ if (result != 0) goto v_0060fdf6;
    /* 0060fdeb */ CM_GLOBAL(volatile u32, 0x9cf6f4u) = value;
    /* 0060fdf1 */ goto v_00610da0;
v_0060fdf6:
    /* 0060fdfc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db50cu);
    /* 0060fe06 */ if (result != 0) goto v_0060fe13;
    /* 0060fe08 */ CM_GLOBAL(volatile u32, 0x9cf6f8u) = value;
    /* 0060fe0e */ goto v_00610da0;
v_0060fe13:
    /* 0060fe19 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db4f8u);
    /* 0060fe23 */ if (result != 0) goto v_0060fe30;
    /* 0060fe25 */ CM_GLOBAL(volatile u32, 0x9cf6fcu) = value;
    /* 0060fe2b */ goto v_00610da0;
v_0060fe30:
    /* 0060fe36 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db4ecu);
    /* 0060fe40 */ if (result != 0) goto v_0060fe4d;
    /* 0060fe42 */ CM_GLOBAL(volatile u32, 0x9cf700u) = value;
    /* 0060fe48 */ goto v_00610da0;
v_0060fe4d:
    /* 0060fe53 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db4d8u);
    /* 0060fe5d */ if (result != 0) goto v_0060fe6a;
    /* 0060fe5f */ CM_GLOBAL(volatile u32, 0x9cf704u) = value;
    /* 0060fe65 */ goto v_00610da0;
v_0060fe6a:
    /* 0060fe70 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db4ccu);
    /* 0060fe7a */ if (result == 0) goto v_00610d8a;
    /* 0060fe86 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db4c0u);
    /* 0060fe90 */ if (result == 0) goto v_00610d8a;
    /* 0060fe9c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db4b4u);
    /* 0060fea6 */ if (result != 0) goto v_0060feb3;
    /* 0060fea8 */ CM_GLOBAL(volatile u32, 0x9cf70cu) = value;
    /* 0060feae */ goto v_00610da0;
v_0060feb3:
    /* 0060feb9 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db4a0u);
    /* 0060fec3 */ if (result != 0) goto v_0060fed0;
    /* 0060fec5 */ CM_GLOBAL(volatile u32, 0x9cf710u) = value;
    /* 0060fecb */ goto v_00610da0;
v_0060fed0:
    /* 0060fed6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db494u);
    /* 0060fee0 */ if (result != 0) goto v_0060feed;
    /* 0060fee2 */ CM_GLOBAL(volatile u32, 0x9cf714u) = value;
    /* 0060fee8 */ goto v_00610da0;
v_0060feed:
    /* 0060fef3 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db480u);
    /* 0060fefd */ if (result == 0) goto v_00610d82;
    /* 0060ff09 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db474u);
    /* 0060ff13 */ if (result == 0) goto v_00610d82;
    /* 0060ff1f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db468u);
    /* 0060ff29 */ if (result == 0) goto v_00610d7a;
    /* 0060ff35 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db454u);
    /* 0060ff3f */ if (result == 0) goto v_00610d7a;
    /* 0060ff4b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db440u);
    /* 0060ff55 */ if (result != 0) goto v_0060ff62;
    /* 0060ff57 */ CM_GLOBAL(volatile u32, 0x9cf720u) = value;
    /* 0060ff5d */ goto v_00610da0;
v_0060ff62:
    /* 0060ff68 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db42cu);
    /* 0060ff72 */ if (result != 0) goto v_0060ff7f;
    /* 0060ff74 */ CM_GLOBAL(volatile u32, 0x9cf724u) = value;
    /* 0060ff7a */ goto v_00610da0;
v_0060ff7f:
    /* 0060ff85 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db420u);
    /* 0060ff8f */ if (result != 0) goto v_0060ff9c;
    /* 0060ff91 */ CM_GLOBAL(volatile u32, 0x9cf728u) = value;
    /* 0060ff97 */ goto v_00610da0;
v_0060ff9c:
    /* 0060ffa2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db408u);
    /* 0060ffac */ if (result != 0) goto v_0060ffb9;
    /* 0060ffae */ CM_GLOBAL(volatile u32, 0x9cf72cu) = value;
    /* 0060ffb4 */ goto v_00610da0;
v_0060ffb9:
    /* 0060ffbf */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db3ecu);
    /* 0060ffc9 */ if (result != 0) goto v_0060ffd6;
    /* 0060ffcb */ CM_GLOBAL(volatile u32, 0x9cf730u) = value;
    /* 0060ffd1 */ goto v_00610da0;
v_0060ffd6:
    /* 0060ffdc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db3dcu);
    /* 0060ffe6 */ if (result != 0) goto v_0060fff3;
    /* 0060ffe8 */ CM_GLOBAL(volatile u32, 0x9cf734u) = value;
    /* 0060ffee */ goto v_00610da0;
v_0060fff3:
    /* 0060fff9 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db3c0u);
    /* 00610003 */ if (result != 0) goto v_00610010;
    /* 00610005 */ CM_GLOBAL(volatile u32, 0x9cf738u) = value;
    /* 0061000b */ goto v_00610da0;
v_00610010:
    /* 00610016 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db3b0u);
    /* 00610020 */ if (result != 0) goto v_0061002d;
    /* 00610022 */ CM_GLOBAL(volatile u32, 0x9cf73cu) = value;
    /* 00610028 */ goto v_00610da0;
v_0061002d:
    /* 00610033 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db3a4u);
    /* 0061003d */ if (result != 0) goto v_0061004a;
    /* 0061003f */ CM_GLOBAL(volatile u32, 0x9cf740u) = value;
    /* 00610045 */ goto v_00610da0;
v_0061004a:
    /* 00610050 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db390u);
    /* 0061005a */ if (result != 0) goto v_00610067;
    /* 0061005c */ CM_GLOBAL(volatile u32, 0x9cf744u) = value;
    /* 00610062 */ goto v_00610da0;
v_00610067:
    /* 0061006d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db384u);
    /* 00610077 */ if (result != 0) goto v_00610084;
    /* 00610079 */ CM_GLOBAL(volatile u32, 0x9cf748u) = value;
    /* 0061007f */ goto v_00610da0;
v_00610084:
    /* 0061008a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db374u);
    /* 00610094 */ if (result != 0) goto v_006100a1;
    /* 00610096 */ CM_GLOBAL(volatile u32, 0x9cf74cu) = value;
    /* 0061009c */ goto v_00610da0;
v_006100a1:
    /* 006100a7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db360u);
    /* 006100b1 */ if (result != 0) goto v_006100be;
    /* 006100b3 */ CM_GLOBAL(volatile u32, 0x9cf750u) = value;
    /* 006100b9 */ goto v_00610da0;
v_006100be:
    /* 006100c4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db348u);
    /* 006100ce */ if (result != 0) goto v_006100db;
    /* 006100d0 */ CM_GLOBAL(volatile u32, 0x9cf754u) = value;
    /* 006100d6 */ goto v_00610da0;
v_006100db:
    /* 006100e1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db32cu);
    /* 006100eb */ if (result == 0) goto v_00610d72;
    /* 006100f7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db318u);
    /* 00610101 */ if (result == 0) goto v_00610d72;
    /* 0061010d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db308u);
    /* 00610117 */ if (result != 0) goto v_00610124;
    /* 00610119 */ CM_GLOBAL(volatile u32, 0x9cf75cu) = value;
    /* 0061011f */ goto v_00610da0;
v_00610124:
    /* 0061012a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db2f4u);
    /* 00610134 */ if (result != 0) goto v_00610141;
    /* 00610136 */ CM_GLOBAL(volatile u32, 0x9cf760u) = value;
    /* 0061013c */ goto v_00610da0;
v_00610141:
    /* 00610147 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db2e0u);
    /* 00610151 */ if (result != 0) goto v_0061015e;
    /* 00610153 */ CM_GLOBAL(volatile u32, 0x9cf764u) = value;
    /* 00610159 */ goto v_00610da0;
v_0061015e:
    /* 00610164 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db2ccu);
    /* 0061016e */ if (result == 0) goto v_00610d6a;
    /* 0061017a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db2b8u);
    /* 00610184 */ if (result == 0) goto v_00610d6a;
    /* 00610190 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db290u);
    /* 0061019a */ if (result != 0) goto v_006101a7;
    /* 0061019c */ CM_GLOBAL(volatile u32, 0x9cf76cu) = value;
    /* 006101a2 */ goto v_00610da0;
v_006101a7:
    /* 006101ad */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db268u);
    /* 006101b7 */ if (result != 0) goto v_006101c4;
    /* 006101b9 */ CM_GLOBAL(volatile u32, 0x9cf770u) = value;
    /* 006101bf */ goto v_00610da0;
v_006101c4:
    /* 006101ca */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db244u);
    /* 006101d4 */ if (result != 0) goto v_006101e1;
    /* 006101d6 */ CM_GLOBAL(volatile u32, 0x9cf774u) = value;
    /* 006101dc */ goto v_00610da0;
v_006101e1:
    /* 006101e7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db218u);
    /* 006101f1 */ if (result != 0) goto v_006101fe;
    /* 006101f3 */ CM_GLOBAL(volatile u32, 0x9cf778u) = value;
    /* 006101f9 */ goto v_00610da0;
v_006101fe:
    /* 00610204 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db1f0u);
    /* 0061020e */ if (result != 0) goto v_0061021b;
    /* 00610210 */ CM_GLOBAL(volatile u32, 0x9cf77cu) = value;
    /* 00610216 */ goto v_00610da0;
v_0061021b:
    /* 00610221 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db1c8u);
    /* 0061022b */ if (result != 0) goto v_00610238;
    /* 0061022d */ CM_GLOBAL(volatile u32, 0x9cf780u) = value;
    /* 00610233 */ goto v_00610da0;
v_00610238:
    /* 0061023e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db1a4u);
    /* 00610248 */ if (result != 0) goto v_00610255;
    /* 0061024a */ CM_GLOBAL(volatile u32, 0x9cf784u) = value;
    /* 00610250 */ goto v_00610da0;
v_00610255:
    /* 0061025b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db18cu);
    /* 00610265 */ if (result != 0) goto v_00610272;
    /* 00610267 */ CM_GLOBAL(volatile u32, 0x9cf788u) = value;
    /* 0061026d */ goto v_00610da0;
v_00610272:
    /* 00610278 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db164u);
    /* 00610282 */ if (result == 0) goto v_00610d62;
    /* 0061028e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db144u);
    /* 00610298 */ if (result == 0) goto v_00610d62;
    /* 006102a4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db138u);
    /* 006102ae */ if (result != 0) goto v_006102bb;
    /* 006102b0 */ CM_GLOBAL(volatile u32, 0x9cf790u) = value;
    /* 006102b6 */ goto v_00610da0;
v_006102bb:
    /* 006102c1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db124u);
    /* 006102cb */ if (result != 0) goto v_006102d8;
    /* 006102cd */ CM_GLOBAL(volatile u32, 0x9cf794u) = value;
    /* 006102d3 */ goto v_00610da0;
v_006102d8:
    /* 006102de */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db10cu);
    /* 006102e8 */ if (result != 0) goto v_006102f5;
    /* 006102ea */ CM_GLOBAL(volatile u32, 0x9cf798u) = value;
    /* 006102f0 */ goto v_00610da0;
v_006102f5:
    /* 006102fb */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db0fcu);
    /* 00610305 */ if (result != 0) goto v_00610312;
    /* 00610307 */ CM_GLOBAL(volatile u32, 0x9cf79cu) = value;
    /* 0061030d */ goto v_00610da0;
v_00610312:
    /* 00610318 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db0d0u);
    /* 00610322 */ if (result == 0) goto v_00610d5a;
    /* 0061032e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db0c0u);
    /* 00610338 */ if (result == 0) goto v_00610d5a;
    /* 00610344 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db0a0u);
    /* 0061034e */ if (result != 0) goto v_0061035b;
    /* 00610350 */ CM_GLOBAL(volatile u32, 0x9cf7a4u) = value;
    /* 00610356 */ goto v_00610da0;
v_0061035b:
    /* 00610361 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db090u);
    /* 0061036b */ if (result != 0) goto v_00610378;
    /* 0061036d */ CM_GLOBAL(volatile u32, 0x9cf7a8u) = value;
    /* 00610373 */ goto v_00610da0;
v_00610378:
    /* 0061037e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db078u);
    /* 00610388 */ if (result != 0) goto v_00610395;
    /* 0061038a */ CM_GLOBAL(volatile u32, 0x9cf7acu) = value;
    /* 00610390 */ goto v_00610da0;
v_00610395:
    /* 0061039b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db068u);
    /* 006103a5 */ if (result == 0) goto v_00610d52;
    /* 006103b1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db04cu);
    /* 006103bb */ if (result == 0) goto v_00610d52;
    /* 006103c7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db030u);
    /* 006103d1 */ if (result == 0) goto v_00610d52;
    /* 006103dd */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9db010u);
    /* 006103e7 */ if (result != 0) goto v_006103f4;
    /* 006103e9 */ CM_GLOBAL(volatile u32, 0x9cf7b4u) = value;
    /* 006103ef */ goto v_00610da0;
v_006103f4:
    /* 006103fa */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9daff4u);
    /* 00610404 */ if (result != 0) goto v_00610411;
    /* 00610406 */ CM_GLOBAL(volatile u32, 0x9cf7b8u) = value;
    /* 0061040c */ goto v_00610da0;
v_00610411:
    /* 00610417 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dafd4u);
    /* 00610421 */ if (result != 0) goto v_0061042e;
    /* 00610423 */ CM_GLOBAL(volatile u32, 0x9cf7bcu) = value;
    /* 00610429 */ goto v_00610da0;
v_0061042e:
    /* 00610434 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dafb4u);
    /* 0061043e */ if (result == 0) goto v_00610d4a;
    /* 0061044a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbc14u);
    /* 00610454 */ if (result == 0) goto v_00610d4a;
    /* 00610460 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9daf9cu);
    /* 0061046a */ if (result == 0) goto v_00610d42;
    /* 00610476 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbf8cu);
    /* 00610480 */ if (result == 0) goto v_00610d42;
    /* 0061048c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9daf84u);
    /* 00610496 */ if (result == 0) goto v_00610d3a;
    /* 006104a2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dbd48u);
    /* 006104ac */ if (result == 0) goto v_00610d3a;
    /* 006104b8 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9daf68u);
    /* 006104c2 */ if (result == 0) goto v_00610d32;
    /* 006104ce */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9daf50u);
    /* 006104d8 */ if (result == 0) goto v_00610d32;
    /* 006104e4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9daf40u);
    /* 006104ee */ if (result != 0) goto v_006104fb;
    /* 006104f0 */ CM_GLOBAL(volatile u32, 0x9cf7d0u) = value;
    /* 006104f6 */ goto v_00610da0;
v_006104fb:
    /* 00610501 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9daf24u);
    /* 0061050b */ if (result != 0) goto v_00610518;
    /* 0061050d */ CM_GLOBAL(volatile u32, 0x9cf7d4u) = value;
    /* 00610513 */ goto v_00610da0;
v_00610518:
    /* 0061051e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9daf08u);
    /* 00610528 */ if (result != 0) goto v_00610535;
    /* 0061052a */ CM_GLOBAL(volatile u32, 0x9cf7d8u) = value;
    /* 00610530 */ goto v_00610da0;
v_00610535:
    /* 0061053b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9daeecu);
    /* 00610545 */ if (result != 0) goto v_00610552;
    /* 00610547 */ CM_GLOBAL(volatile u32, 0x9cf7dcu) = value;
    /* 0061054d */ goto v_00610da0;
v_00610552:
    /* 00610558 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9daed0u);
    /* 00610562 */ if (result != 0) goto v_0061056f;
    /* 00610564 */ CM_GLOBAL(volatile u32, 0x9cf7e0u) = value;
    /* 0061056a */ goto v_00610da0;
v_0061056f:
    /* 00610575 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9daec4u);
    /* 0061057f */ if (result != 0) goto v_0061058c;
    /* 00610581 */ CM_GLOBAL(volatile u32, 0x9cf7e4u) = value;
    /* 00610587 */ goto v_00610da0;
v_0061058c:
    /* 00610592 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9daea8u);
    /* 0061059c */ if (result != 0) goto v_006105a9;
    /* 0061059e */ CM_GLOBAL(volatile u32, 0x9cf7e8u) = value;
    /* 006105a4 */ goto v_00610da0;
v_006105a9:
    /* 006105af */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dae88u);
    /* 006105b9 */ if (result != 0) goto v_006105c6;
    /* 006105bb */ CM_GLOBAL(volatile u32, 0x9cf7ecu) = value;
    /* 006105c1 */ goto v_00610da0;
v_006105c6:
    /* 006105cc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dae68u);
    /* 006105d6 */ if (result != 0) goto v_006105e3;
    /* 006105d8 */ CM_GLOBAL(volatile u32, 0x9cf7f0u) = value;
    /* 006105de */ goto v_00610da0;
v_006105e3:
    /* 006105e9 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dae50u);
    /* 006105f3 */ if (result != 0) goto v_00610600;
    /* 006105f5 */ CM_GLOBAL(volatile u32, 0x9cf7f4u) = value;
    /* 006105fb */ goto v_00610da0;
v_00610600:
    /* 00610606 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dae38u);
    /* 00610610 */ if (result != 0) goto v_0061061d;
    /* 00610612 */ CM_GLOBAL(volatile u32, 0x9cf7f8u) = value;
    /* 00610618 */ goto v_00610da0;
v_0061061d:
    /* 00610623 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dae18u);
    /* 0061062d */ if (result != 0) goto v_0061063a;
    /* 0061062f */ CM_GLOBAL(volatile u32, 0x9cf7fcu) = value;
    /* 00610635 */ goto v_00610da0;
v_0061063a:
    /* 00610640 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dadf8u);
    /* 0061064a */ if (result != 0) goto v_00610657;
    /* 0061064c */ CM_GLOBAL(volatile u32, 0x9cf800u) = value;
    /* 00610652 */ goto v_00610da0;
v_00610657:
    /* 0061065d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dadd8u);
    /* 00610667 */ if (result != 0) goto v_00610674;
    /* 00610669 */ CM_GLOBAL(volatile u32, 0x9cf804u) = value;
    /* 0061066f */ goto v_00610da0;
v_00610674:
    /* 0061067a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dadb8u);
    /* 00610684 */ if (result != 0) goto v_00610691;
    /* 00610686 */ CM_GLOBAL(volatile u32, 0x9cf808u) = value;
    /* 0061068c */ goto v_00610da0;
v_00610691:
    /* 00610697 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dada0u);
    /* 006106a1 */ if (result != 0) goto v_006106ae;
    /* 006106a3 */ CM_GLOBAL(volatile u32, 0x9cf80cu) = value;
    /* 006106a9 */ goto v_00610da0;
v_006106ae:
    /* 006106b4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dad88u);
    /* 006106be */ if (result != 0) goto v_006106cb;
    /* 006106c0 */ CM_GLOBAL(volatile u32, 0x9cf810u) = value;
    /* 006106c6 */ goto v_00610da0;
v_006106cb:
    /* 006106d1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dad78u);
    /* 006106db */ if (result != 0) goto v_006106e8;
    /* 006106dd */ CM_GLOBAL(volatile u32, 0x9cf814u) = value;
    /* 006106e3 */ goto v_00610da0;
v_006106e8:
    /* 006106ee */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dad68u);
    /* 006106f8 */ if (result != 0) goto v_00610705;
    /* 006106fa */ CM_GLOBAL(volatile u32, 0x9cf818u) = value;
    /* 00610700 */ goto v_00610da0;
v_00610705:
    /* 0061070b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dad58u);
    /* 00610715 */ if (result != 0) goto v_00610722;
    /* 00610717 */ CM_GLOBAL(volatile u32, 0x9cf81cu) = value;
    /* 0061071d */ goto v_00610da0;
v_00610722:
    /* 00610728 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dad48u);
    /* 00610732 */ if (result != 0) goto v_0061073f;
    /* 00610734 */ CM_GLOBAL(volatile u32, 0x9cf820u) = value;
    /* 0061073a */ goto v_00610da0;
v_0061073f:
    /* 00610745 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dad38u);
    /* 0061074f */ if (result != 0) goto v_0061075c;
    /* 00610751 */ CM_GLOBAL(volatile u32, 0x9cf824u) = value;
    /* 00610757 */ goto v_00610da0;
v_0061075c:
    /* 00610762 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dad20u);
    /* 0061076c */ if (result != 0) goto v_00610779;
    /* 0061076e */ CM_GLOBAL(volatile u32, 0x9cf82cu) = value;
    /* 00610774 */ goto v_00610da0;
v_00610779:
    /* 0061077f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dad08u);
    /* 00610789 */ if (result != 0) goto v_00610796;
    /* 0061078b */ CM_GLOBAL(volatile u32, 0x9cf830u) = value;
    /* 00610791 */ goto v_00610da0;
v_00610796:
    /* 0061079c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dacf0u);
    /* 006107a6 */ if (result != 0) goto v_006107b3;
    /* 006107a8 */ CM_GLOBAL(volatile u32, 0x9cf834u) = value;
    /* 006107ae */ goto v_00610da0;
v_006107b3:
    /* 006107b9 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dacd8u);
    /* 006107c3 */ if (result != 0) goto v_006107d0;
    /* 006107c5 */ CM_GLOBAL(volatile u32, 0x9cf838u) = value;
    /* 006107cb */ goto v_00610da0;
v_006107d0:
    /* 006107d6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dacc4u);
    /* 006107e0 */ if (result != 0) goto v_006107ed;
    /* 006107e2 */ CM_GLOBAL(volatile u32, 0x9cf83cu) = value;
    /* 006107e8 */ goto v_00610da0;
v_006107ed:
    /* 006107f3 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9daca8u);
    /* 006107fd */ if (result != 0) goto v_0061080a;
    /* 006107ff */ CM_GLOBAL(volatile u32, 0x9cf840u) = value;
    /* 00610805 */ goto v_00610da0;
v_0061080a:
    /* 00610810 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dac98u);
    /* 0061081a */ if (result != 0) goto v_00610827;
    /* 0061081c */ CM_GLOBAL(volatile u32, 0x9cf844u) = value;
    /* 00610822 */ goto v_00610da0;
v_00610827:
    /* 0061082d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dac78u);
    /* 00610837 */ if (result != 0) goto v_00610844;
    /* 00610839 */ CM_GLOBAL(volatile u32, 0x9cf848u) = value;
    /* 0061083f */ goto v_00610da0;
v_00610844:
    /* 0061084a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dac5cu);
    /* 00610854 */ if (result != 0) goto v_00610861;
    /* 00610856 */ CM_GLOBAL(volatile u32, 0x9cf84cu) = value;
    /* 0061085c */ goto v_00610da0;
v_00610861:
    /* 00610867 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dac40u);
    /* 00610871 */ if (result != 0) goto v_0061087e;
    /* 00610873 */ CM_GLOBAL(volatile u32, 0x9cf850u) = value;
    /* 00610879 */ goto v_00610da0;
v_0061087e:
    /* 00610884 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dac20u);
    /* 0061088e */ if (result == 0) goto v_00610d2a;
    /* 0061089a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dabf8u);
    /* 006108a4 */ if (result == 0) goto v_00610d2a;
    /* 006108b0 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dabe0u);
    /* 006108ba */ if (result != 0) goto v_006108c7;
    /* 006108bc */ CM_GLOBAL(volatile u32, 0x9cf858u) = value;
    /* 006108c2 */ goto v_00610da0;
v_006108c7:
    /* 006108cd */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dabc8u);
    /* 006108d7 */ if (result != 0) goto v_006108e4;
    /* 006108d9 */ CM_GLOBAL(volatile u32, 0x9cf85cu) = value;
    /* 006108df */ goto v_00610da0;
v_006108e4:
    /* 006108ea */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dabb4u);
    /* 006108f4 */ if (result != 0) goto v_00610901;
    /* 006108f6 */ CM_GLOBAL(volatile u32, 0x9cf860u) = value;
    /* 006108fc */ goto v_00610da0;
v_00610901:
    /* 00610907 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9daba4u);
    /* 00610911 */ if (result != 0) goto v_0061091e;
    /* 00610913 */ CM_GLOBAL(volatile u32, 0x9cf864u) = value;
    /* 00610919 */ goto v_00610da0;
v_0061091e:
    /* 00610924 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dab8cu);
    /* 0061092e */ if (result != 0) goto v_0061093b;
    /* 00610930 */ CM_GLOBAL(volatile u32, 0x9cf868u) = value;
    /* 00610936 */ goto v_00610da0;
v_0061093b:
    /* 00610941 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dab74u);
    /* 0061094b */ if (result == 0) goto v_00610d22;
    /* 00610957 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dab58u);
    /* 00610961 */ if (result == 0) goto v_00610d22;
    /* 0061096d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dab40u);
    /* 00610977 */ if (result == 0) goto v_00610d1a;
    /* 00610983 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dab24u);
    /* 0061098d */ if (result == 0) goto v_00610d1a;
    /* 00610999 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dab0cu);
    /* 006109a3 */ if (result != 0) goto v_006109b0;
    /* 006109a5 */ CM_GLOBAL(volatile u32, 0x9cf874u) = value;
    /* 006109ab */ goto v_00610da0;
v_006109b0:
    /* 006109b6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9dab00u);
    /* 006109c0 */ if (result != 0) goto v_006109cd;
    /* 006109c2 */ CM_GLOBAL(volatile u32, 0x9cf878u) = value;
    /* 006109c8 */ goto v_00610da0;
v_006109cd:
    /* 006109d3 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9daae8u);
    /* 006109dd */ if (result == 0) goto v_00610d09;
    /* 006109e9 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9daad0u);
    /* 006109f3 */ if (result == 0) goto v_00610d09;
    /* 006109ff */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9daab4u);
    /* 00610a09 */ if (result != 0) goto v_00610a16;
    /* 00610a0b */ CM_GLOBAL(volatile u32, 0x9cf880u) = value;
    /* 00610a11 */ goto v_00610da0;
v_00610a16:
    /* 00610a1c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9daa94u);
    /* 00610a26 */ if (result != 0) goto v_00610a33;
    /* 00610a28 */ CM_GLOBAL(volatile u32, 0x9cf884u) = value;
    /* 00610a2e */ goto v_00610da0;
v_00610a33:
    /* 00610a39 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9daa74u);
    /* 00610a43 */ if (result != 0) goto v_00610a50;
    /* 00610a45 */ CM_GLOBAL(volatile u32, 0x9cf888u) = value;
    /* 00610a4b */ goto v_00610da0;
v_00610a50:
    /* 00610a56 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9daa54u);
    /* 00610a60 */ if (result != 0) goto v_00610a6d;
    /* 00610a62 */ CM_GLOBAL(volatile u32, 0x9cf88cu) = value;
    /* 00610a68 */ goto v_00610da0;
v_00610a6d:
    /* 00610a73 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9daa34u);
    /* 00610a7d */ if (result != 0) goto v_00610a8a;
    /* 00610a7f */ CM_GLOBAL(volatile u32, 0x9cf890u) = value;
    /* 00610a85 */ goto v_00610da0;
v_00610a8a:
    /* 00610a90 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9daa14u);
    /* 00610a9a */ if (result != 0) goto v_00610aa7;
    /* 00610a9c */ CM_GLOBAL(volatile u32, 0x9cf894u) = value;
    /* 00610aa2 */ goto v_00610da0;
v_00610aa7:
    /* 00610aad */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9daa04u);
    /* 00610ab7 */ if (result != 0) goto v_00610ac4;
    /* 00610ab9 */ CM_GLOBAL(volatile u32, 0x9cf898u) = value;
    /* 00610abf */ goto v_00610da0;
v_00610ac4:
    /* 00610aca */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da9e0u);
    /* 00610ad4 */ if (result != 0) goto v_00610ae1;
    /* 00610ad6 */ CM_GLOBAL(volatile u32, 0x9cf89cu) = value;
    /* 00610adc */ goto v_00610da0;
v_00610ae1:
    /* 00610ae7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da9ccu);
    /* 00610af1 */ if (result != 0) goto v_00610afe;
    /* 00610af3 */ CM_GLOBAL(volatile u32, 0x9cf8a0u) = value;
    /* 00610af9 */ goto v_00610da0;
v_00610afe:
    /* 00610b04 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da9acu);
    /* 00610b0e */ if (result == 0) goto v_00610cfe;
    /* 00610b1a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da990u);
    /* 00610b24 */ if (result == 0) goto v_00610cfe;
    /* 00610b30 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da978u);
    /* 00610b3a */ if (result == 0) goto v_00610cfe;
    /* 00610b46 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da958u);
    /* 00610b50 */ if (result == 0) goto v_00610cf3;
    /* 00610b5c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da93cu);
    /* 00610b66 */ if (result == 0) goto v_00610cf3;
    /* 00610b72 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da924u);
    /* 00610b7c */ if (result == 0) goto v_00610cf3;
    /* 00610b88 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da900u);
    /* 00610b92 */ if (result == 0) goto v_00610ce8;
    /* 00610b9e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da8e0u);
    /* 00610ba8 */ if (result == 0) goto v_00610ce8;
    /* 00610bb4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da8b8u);
    /* 00610bbe */ if (result == 0) goto v_00610ce8;
    /* 00610bca */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da898u);
    /* 00610bd4 */ if (result == 0) goto v_00610cdd;
    /* 00610be0 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da87cu);
    /* 00610bea */ if (result == 0) goto v_00610cdd;
    /* 00610bf6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da854u);
    /* 00610c00 */ if (result == 0) goto v_00610cdd;
    /* 00610c0c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da834u);
    /* 00610c16 */ if (result == 0) goto v_00610cd2;
    /* 00610c22 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da818u);
    /* 00610c2c */ if (result == 0) goto v_00610cd2;
    /* 00610c38 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da7f0u);
    /* 00610c42 */ if (result == 0) goto v_00610cd2;
    /* 00610c4e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da7d0u);
    /* 00610c58 */ if (result != 0) goto v_00610c65;
    /* 00610c5a */ CM_GLOBAL(volatile u32, 0x9cf8bcu) = value;
    /* 00610c60 */ goto v_00610da0;
v_00610c65:
    /* 00610c6b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da7bcu);
    /* 00610c75 */ if (result == 0) goto v_00610cc7;
    /* 00610c7d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da7b0u);
    /* 00610c87 */ if (result == 0) goto v_00610cc7;
    /* 00610c8f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da798u);
    /* 00610c99 */ if (result != 0) goto v_00610ca6;
    /* 00610c9b */ CM_GLOBAL(volatile u32, 0x9cf8c4u) = value;
    /* 00610ca1 */ goto v_00610da0;
v_00610ca6:
    /* 00610cac */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da77cu);
    /* 00610cb6 */ if (result != 0) goto v_00610da0;
    /* 00610cbc */ CM_GLOBAL(volatile u32, 0x9cf8c8u) = value;
    /* 00610cc2 */ goto v_00610da0;
v_00610cc7:
    /* 00610cc7 */ CM_GLOBAL(volatile u32, 0x9cf8c0u) = value;
    /* 00610ccd */ goto v_00610da0;
v_00610cd2:
    /* 00610cd2 */ CM_GLOBAL(volatile u32, 0x9cf8b8u) = value;
    /* 00610cd8 */ goto v_00610da0;
v_00610cdd:
    /* 00610cdd */ CM_GLOBAL(volatile u32, 0x9cf8b4u) = value;
    /* 00610ce3 */ goto v_00610da0;
v_00610ce8:
    /* 00610ce8 */ CM_GLOBAL(volatile u32, 0x9cf8b0u) = value;
    /* 00610cee */ goto v_00610da0;
v_00610cf3:
    /* 00610cf3 */ CM_GLOBAL(volatile u32, 0x9cf8acu) = value;
    /* 00610cf9 */ goto v_00610da0;
v_00610cfe:
    /* 00610cfe */ CM_GLOBAL(volatile u32, 0x9cf8a8u) = value;
    /* 00610d04 */ goto v_00610da0;
v_00610d09:
    /* 00610d09 */ CM_GLOBAL(volatile u32, 0x9cf87cu) = value;
    /* 00610d0f */ CM_GLOBAL(volatile u32, 0x9cf8a4u) = value;
    /* 00610d15 */ goto v_00610da0;
v_00610d1a:
    /* 00610d1a */ CM_GLOBAL(volatile u32, 0x9cf870u) = value;
    /* 00610d20 */ goto v_00610da0;
v_00610d22:
    /* 00610d22 */ CM_GLOBAL(volatile u32, 0x9cf86cu) = value;
    /* 00610d28 */ goto v_00610da0;
v_00610d2a:
    /* 00610d2a */ CM_GLOBAL(volatile u32, 0x9cf854u) = value;
    /* 00610d30 */ goto v_00610da0;
v_00610d32:
    /* 00610d32 */ CM_GLOBAL(volatile u32, 0x9cf7ccu) = value;
    /* 00610d38 */ goto v_00610da0;
v_00610d3a:
    /* 00610d3a */ CM_GLOBAL(volatile u32, 0x9cf7c8u) = value;
    /* 00610d40 */ goto v_00610da0;
v_00610d42:
    /* 00610d42 */ CM_GLOBAL(volatile u32, 0x9cf7c4u) = value;
    /* 00610d48 */ goto v_00610da0;
v_00610d4a:
    /* 00610d4a */ CM_GLOBAL(volatile u32, 0x9cf7c0u) = value;
    /* 00610d50 */ goto v_00610da0;
v_00610d52:
    /* 00610d52 */ CM_GLOBAL(volatile u32, 0x9cf7b0u) = value;
    /* 00610d58 */ goto v_00610da0;
v_00610d5a:
    /* 00610d5a */ CM_GLOBAL(volatile u32, 0x9cf7a0u) = value;
    /* 00610d60 */ goto v_00610da0;
v_00610d62:
    /* 00610d62 */ CM_GLOBAL(volatile u32, 0x9cf78cu) = value;
    /* 00610d68 */ goto v_00610da0;
v_00610d6a:
    /* 00610d6a */ CM_GLOBAL(volatile u32, 0x9cf768u) = value;
    /* 00610d70 */ goto v_00610da0;
v_00610d72:
    /* 00610d72 */ CM_GLOBAL(volatile u32, 0x9cf758u) = value;
    /* 00610d78 */ goto v_00610da0;
v_00610d7a:
    /* 00610d7a */ CM_GLOBAL(volatile u32, 0x9cf71cu) = value;
    /* 00610d80 */ goto v_00610da0;
v_00610d82:
    /* 00610d82 */ CM_GLOBAL(volatile u32, 0x9cf718u) = value;
    /* 00610d88 */ goto v_00610da0;
v_00610d8a:
    /* 00610d8a */ CM_GLOBAL(volatile u32, 0x9cf708u) = value;
    /* 00610d90 */ goto v_00610da0;
v_00610d92:
    /* 00610d92 */ CM_GLOBAL(volatile u32, 0x9cf6e8u) = value;
    /* 00610d98 */ goto v_00610da0;
v_00610d9a:
    /* 00610d9a */ CM_GLOBAL(volatile u32, 0x9cf6e4u) = value;
v_00610da0:
    /* 00610da6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da764u);
    /* 00610db0 */ if (result != 0) goto v_00610dbb;
    /* 00610db2 */ CM_GLOBAL(volatile u32, 0x9cf8ccu) = value;
    /* 00610dba */ return result;
v_00610dbb:
    /* 00610dc1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da748u);
    /* 00610dcb */ if (result != 0) goto v_00610dd6;
    /* 00610dcd */ CM_GLOBAL(volatile u32, 0x9cf8d8u) = value;
    /* 00610dd5 */ return result;
v_00610dd6:
    /* 00610ddc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da728u);
    /* 00610de6 */ if (result != 0) goto v_00610df1;
    /* 00610de8 */ CM_GLOBAL(volatile u32, 0x9cf8dcu) = value;
    /* 00610df0 */ return result;
v_00610df1:
    /* 00610df7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da708u);
    /* 00610e01 */ if (result != 0) goto v_00610e0c;
    /* 00610e03 */ CM_GLOBAL(volatile u32, 0x9cf8e0u) = value;
    /* 00610e0b */ return result;
v_00610e0c:
    /* 00610e12 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da6e4u);
    /* 00610e1c */ if (result != 0) goto v_00610e27;
    /* 00610e1e */ CM_GLOBAL(volatile u32, 0x9cf8e4u) = value;
    /* 00610e26 */ return result;
v_00610e27:
    /* 00610e2d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da6c0u);
    /* 00610e37 */ if (result != 0) goto v_00610e42;
    /* 00610e39 */ CM_GLOBAL(volatile u32, 0x9cf8e8u) = value;
    /* 00610e41 */ return result;
v_00610e42:
    /* 00610e48 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da69cu);
    /* 00610e52 */ if (result != 0) goto v_00610e5d;
    /* 00610e54 */ CM_GLOBAL(volatile u32, 0x9cf8ecu) = value;
    /* 00610e5c */ return result;
v_00610e5d:
    /* 00610e63 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da678u);
    /* 00610e6d */ if (result != 0) goto v_00610e78;
    /* 00610e6f */ CM_GLOBAL(volatile u32, 0x9cf8f0u) = value;
    /* 00610e77 */ return result;
v_00610e78:
    /* 00610e7e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da654u);
    /* 00610e88 */ if (result != 0) goto v_00610e93;
    /* 00610e8a */ CM_GLOBAL(volatile u32, 0x9cf8f4u) = value;
    /* 00610e92 */ return result;
v_00610e93:
    /* 00610e99 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da644u);
    /* 00610ea3 */ if (result != 0) goto v_00610eae;
    /* 00610ea5 */ CM_GLOBAL(volatile u32, 0x9cf8f8u) = value;
    /* 00610ead */ return result;
v_00610eae:
    /* 00610eb4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da618u);
    /* 00610ebe */ if (result == 0) goto v_006115d0;
    /* 00610eca */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da5f0u);
    /* 00610ed4 */ if (result == 0) goto v_006115d0;
    /* 00610ee0 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da5d4u);
    /* 00610eea */ if (result != 0) goto v_00610ef5;
    /* 00610eec */ CM_GLOBAL(volatile u32, 0x9cf900u) = value;
    /* 00610ef4 */ return result;
v_00610ef5:
    /* 00610efb */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da5b8u);
    /* 00610f05 */ if (result != 0) goto v_00610f10;
    /* 00610f07 */ CM_GLOBAL(volatile u32, 0x9cf904u) = value;
    /* 00610f0f */ return result;
v_00610f10:
    /* 00610f16 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da59cu);
    /* 00610f20 */ if (result != 0) goto v_00610f2b;
    /* 00610f22 */ CM_GLOBAL(volatile u32, 0x9cf908u) = value;
    /* 00610f2a */ return result;
v_00610f2b:
    /* 00610f31 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da580u);
    /* 00610f3b */ if (result != 0) goto v_00610f46;
    /* 00610f3d */ CM_GLOBAL(volatile u32, 0x9cf90cu) = value;
    /* 00610f45 */ return result;
v_00610f46:
    /* 00610f4c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da564u);
    /* 00610f56 */ if (result != 0) goto v_00610f61;
    /* 00610f58 */ CM_GLOBAL(volatile u32, 0x9cf910u) = value;
    /* 00610f60 */ return result;
v_00610f61:
    /* 00610f67 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da544u);
    /* 00610f71 */ if (result != 0) goto v_00610f7c;
    /* 00610f73 */ CM_GLOBAL(volatile u32, 0x9cf914u) = value;
    /* 00610f7b */ return result;
v_00610f7c:
    /* 00610f82 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da520u);
    /* 00610f8c */ if (result != 0) goto v_00610f97;
    /* 00610f8e */ CM_GLOBAL(volatile u32, 0x9cf918u) = value;
    /* 00610f96 */ return result;
v_00610f97:
    /* 00610f9d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da500u);
    /* 00610fa7 */ if (result != 0) goto v_00610fb2;
    /* 00610fa9 */ CM_GLOBAL(volatile u32, 0x9cf91cu) = value;
    /* 00610fb1 */ return result;
v_00610fb2:
    /* 00610fb8 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da4e0u);
    /* 00610fc2 */ if (result != 0) goto v_00610fcd;
    /* 00610fc4 */ CM_GLOBAL(volatile u32, 0x9cf920u) = value;
    /* 00610fcc */ return result;
v_00610fcd:
    /* 00610fd3 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da4c0u);
    /* 00610fdd */ if (result != 0) goto v_00610fe8;
    /* 00610fdf */ CM_GLOBAL(volatile u32, 0x9cf924u) = value;
    /* 00610fe7 */ return result;
v_00610fe8:
    /* 00610fee */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da4a0u);
    /* 00610ff8 */ if (result != 0) goto v_00611003;
    /* 00610ffa */ CM_GLOBAL(volatile u32, 0x9cf928u) = value;
    /* 00611002 */ return result;
v_00611003:
    /* 00611009 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da488u);
    /* 00611013 */ if (result != 0) goto v_0061101e;
    /* 00611015 */ CM_GLOBAL(volatile u32, 0x9cf92cu) = value;
    /* 0061101d */ return result;
v_0061101e:
    /* 00611024 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da46cu);
    /* 0061102e */ if (result != 0) goto v_00611039;
    /* 00611030 */ CM_GLOBAL(volatile u32, 0x9cf930u) = value;
    /* 00611038 */ return result;
v_00611039:
    /* 0061103f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da460u);
    /* 00611049 */ if (result != 0) goto v_00611054;
    /* 0061104b */ CM_GLOBAL(volatile u32, 0x9cf934u) = value;
    /* 00611053 */ return result;
v_00611054:
    /* 0061105a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da448u);
    /* 00611064 */ if (result != 0) goto v_0061106f;
    /* 00611066 */ CM_GLOBAL(volatile u32, 0x9cf938u) = value;
    /* 0061106e */ return result;
v_0061106f:
    /* 00611075 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da438u);
    /* 0061107f */ if (result != 0) goto v_0061108a;
    /* 00611081 */ CM_GLOBAL(volatile u32, 0x9cf93cu) = value;
    /* 00611089 */ return result;
v_0061108a:
    /* 00611090 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da424u);
    /* 0061109a */ if (result != 0) goto v_006110a5;
    /* 0061109c */ CM_GLOBAL(volatile u32, 0x9cf940u) = value;
    /* 006110a4 */ return result;
v_006110a5:
    /* 006110ab */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da410u);
    /* 006110b5 */ if (result != 0) goto v_006110c0;
    /* 006110b7 */ CM_GLOBAL(volatile u32, 0x9cf944u) = value;
    /* 006110bf */ return result;
v_006110c0:
    /* 006110c6 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da3f8u);
    /* 006110d0 */ if (result != 0) goto v_006110db;
    /* 006110d2 */ CM_GLOBAL(volatile u32, 0x9cf948u) = value;
    /* 006110da */ return result;
v_006110db:
    /* 006110e1 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da3e0u);
    /* 006110eb */ if (result != 0) goto v_006110f6;
    /* 006110ed */ CM_GLOBAL(volatile u32, 0x9cf94cu) = value;
    /* 006110f5 */ return result;
v_006110f6:
    /* 006110fc */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da3ccu);
    /* 00611106 */ if (result != 0) goto v_00611111;
    /* 00611108 */ CM_GLOBAL(volatile u32, 0x9cf950u) = value;
    /* 00611110 */ return result;
v_00611111:
    /* 00611117 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da3c0u);
    /* 00611121 */ if (result != 0) goto v_0061112c;
    /* 00611123 */ CM_GLOBAL(volatile u32, 0x9cf954u) = value;
    /* 0061112b */ return result;
v_0061112c:
    /* 00611132 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da3acu);
    /* 0061113c */ if (result != 0) goto v_00611147;
    /* 0061113e */ CM_GLOBAL(volatile u32, 0x9cf958u) = value;
    /* 00611146 */ return result;
v_00611147:
    /* 0061114d */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da398u);
    /* 00611157 */ if (result != 0) goto v_00611162;
    /* 00611159 */ CM_GLOBAL(volatile u32, 0x9cf95cu) = value;
    /* 00611161 */ return result;
v_00611162:
    /* 00611168 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da380u);
    /* 00611172 */ if (result != 0) goto v_0061117d;
    /* 00611174 */ CM_GLOBAL(volatile u32, 0x9cf960u) = value;
    /* 0061117c */ return result;
v_0061117d:
    /* 00611183 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da36cu);
    /* 0061118d */ if (result != 0) goto v_00611198;
    /* 0061118f */ CM_GLOBAL(volatile u32, 0x9cf964u) = value;
    /* 00611197 */ return result;
v_00611198:
    /* 0061119e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da34cu);
    /* 006111a8 */ if (result != 0) goto v_006111b3;
    /* 006111aa */ CM_GLOBAL(volatile u32, 0x9cf968u) = value;
    /* 006111b2 */ return result;
v_006111b3:
    /* 006111b9 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da328u);
    /* 006111c3 */ if (result != 0) goto v_006111ce;
    /* 006111c5 */ CM_GLOBAL(volatile u32, 0x9cf96cu) = value;
    /* 006111cd */ return result;
v_006111ce:
    /* 006111d4 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da300u);
    /* 006111de */ if (result != 0) goto v_006111e9;
    /* 006111e0 */ CM_GLOBAL(volatile u32, 0x9cf970u) = value;
    /* 006111e8 */ return result;
v_006111e9:
    /* 006111ef */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da2dcu);
    /* 006111f9 */ if (result != 0) goto v_00611204;
    /* 006111fb */ CM_GLOBAL(volatile u32, 0x9cf974u) = value;
    /* 00611203 */ return result;
v_00611204:
    /* 0061120a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da2bcu);
    /* 00611214 */ if (result != 0) goto v_0061121f;
    /* 00611216 */ CM_GLOBAL(volatile u32, 0x9cf978u) = value;
    /* 0061121e */ return result;
v_0061121f:
    /* 00611225 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da294u);
    /* 0061122f */ if (result != 0) goto v_0061123a;
    /* 00611231 */ CM_GLOBAL(volatile u32, 0x9cf97cu) = value;
    /* 00611239 */ return result;
v_0061123a:
    /* 00611240 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da270u);
    /* 0061124a */ if (result != 0) goto v_00611255;
    /* 0061124c */ CM_GLOBAL(volatile u32, 0x9cf980u) = value;
    /* 00611254 */ return result;
v_00611255:
    /* 0061125b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da250u);
    /* 00611265 */ if (result != 0) goto v_00611270;
    /* 00611267 */ CM_GLOBAL(volatile u32, 0x9cf984u) = value;
    /* 0061126f */ return result;
v_00611270:
    /* 00611276 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da228u);
    /* 00611280 */ if (result != 0) goto v_0061128b;
    /* 00611282 */ CM_GLOBAL(volatile u32, 0x9cf988u) = value;
    /* 0061128a */ return result;
v_0061128b:
    /* 00611291 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da208u);
    /* 0061129b */ if (result != 0) goto v_006112a6;
    /* 0061129d */ CM_GLOBAL(volatile u32, 0x9cf98cu) = value;
    /* 006112a5 */ return result;
v_006112a6:
    /* 006112ac */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da1e8u);
    /* 006112b6 */ if (result != 0) goto v_006112c1;
    /* 006112b8 */ CM_GLOBAL(volatile u32, 0x9cf990u) = value;
    /* 006112c0 */ return result;
v_006112c1:
    /* 006112c7 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da1c4u);
    /* 006112d1 */ if (result != 0) goto v_006112dc;
    /* 006112d3 */ CM_GLOBAL(volatile u32, 0x9cf994u) = value;
    /* 006112db */ return result;
v_006112dc:
    /* 006112e2 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da19cu);
    /* 006112ec */ if (result != 0) goto v_006112f7;
    /* 006112ee */ CM_GLOBAL(volatile u32, 0x9cf998u) = value;
    /* 006112f6 */ return result;
v_006112f7:
    /* 006112fd */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da178u);
    /* 00611307 */ if (result != 0) goto v_00611312;
    /* 00611309 */ CM_GLOBAL(volatile u32, 0x9cf99cu) = value;
    /* 00611311 */ return result;
v_00611312:
    /* 00611318 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da158u);
    /* 00611322 */ if (result != 0) goto v_0061132d;
    /* 00611324 */ CM_GLOBAL(volatile u32, 0x9cf9a0u) = value;
    /* 0061132c */ return result;
v_0061132d:
    /* 00611333 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da130u);
    /* 0061133d */ if (result != 0) goto v_00611348;
    /* 0061133f */ CM_GLOBAL(volatile u32, 0x9cf9a4u) = value;
    /* 00611347 */ return result;
v_00611348:
    /* 0061134e */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da110u);
    /* 00611358 */ if (result != 0) goto v_00611363;
    /* 0061135a */ CM_GLOBAL(volatile u32, 0x9cf9a8u) = value;
    /* 00611362 */ return result;
v_00611363:
    /* 00611369 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da0ecu);
    /* 00611373 */ if (result != 0) goto v_0061137e;
    /* 00611375 */ CM_GLOBAL(volatile u32, 0x9cf9acu) = value;
    /* 0061137d */ return result;
v_0061137e:
    /* 00611384 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da0c0u);
    /* 0061138e */ if (result != 0) goto v_00611399;
    /* 00611390 */ CM_GLOBAL(volatile u32, 0x9cf9b0u) = value;
    /* 00611398 */ return result;
v_00611399:
    /* 0061139f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da09cu);
    /* 006113a9 */ if (result != 0) goto v_006113b4;
    /* 006113ab */ CM_GLOBAL(volatile u32, 0x9cf9b4u) = value;
    /* 006113b3 */ return result;
v_006113b4:
    /* 006113ba */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da084u);
    /* 006113c4 */ if (result != 0) goto v_006113cf;
    /* 006113c6 */ CM_GLOBAL(volatile u32, 0x9cf9b8u) = value;
    /* 006113ce */ return result;
v_006113cf:
    /* 006113d5 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da05cu);
    /* 006113df */ if (result != 0) goto v_006113ea;
    /* 006113e1 */ CM_GLOBAL(volatile u32, 0x9cf9bcu) = value;
    /* 006113e9 */ return result;
v_006113ea:
    /* 006113f0 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da038u);
    /* 006113fa */ if (result != 0) goto v_00611405;
    /* 006113fc */ CM_GLOBAL(volatile u32, 0x9cf9c0u) = value;
    /* 00611404 */ return result;
v_00611405:
    /* 0061140b */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9da014u);
    /* 00611415 */ if (result != 0) goto v_00611420;
    /* 00611417 */ CM_GLOBAL(volatile u32, 0x9cf9c4u) = value;
    /* 0061141f */ return result;
v_00611420:
    /* 00611426 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9d9ff0u);
    /* 00611430 */ if (result != 0) goto v_0061143b;
    /* 00611432 */ CM_GLOBAL(volatile u32, 0x9cf9c8u) = value;
    /* 0061143a */ return result;
v_0061143b:
    /* 00611441 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9d9fccu);
    /* 0061144b */ if (result != 0) goto v_00611456;
    /* 0061144d */ CM_GLOBAL(volatile u32, 0x9cf9ccu) = value;
    /* 00611455 */ return result;
v_00611456:
    /* 0061145c */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9d9fa8u);
    /* 00611466 */ if (result != 0) goto v_00611471;
    /* 00611468 */ CM_GLOBAL(volatile u32, 0x9cf9d0u) = value;
    /* 00611470 */ return result;
v_00611471:
    /* 00611477 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9d9f7cu);
    /* 00611481 */ if (result != 0) goto v_0061148c;
    /* 00611483 */ CM_GLOBAL(volatile u32, 0x9cf9d4u) = value;
    /* 0061148b */ return result;
v_0061148c:
    /* 00611492 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9d9f58u);
    /* 0061149c */ if (result != 0) goto v_006114a7;
    /* 0061149e */ CM_GLOBAL(volatile u32, 0x9cf9d8u) = value;
    /* 006114a6 */ return result;
v_006114a7:
    /* 006114ad */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9d9f2cu);
    /* 006114b7 */ if (result != 0) goto v_006114c2;
    /* 006114b9 */ CM_GLOBAL(volatile u32, 0x9cf9dcu) = value;
    /* 006114c1 */ return result;
v_006114c2:
    /* 006114c8 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9d9f08u);
    /* 006114d2 */ if (result != 0) goto v_006114dd;
    /* 006114d4 */ CM_GLOBAL(volatile u32, 0x9cf9e0u) = value;
    /* 006114dc */ return result;
v_006114dd:
    /* 006114e3 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9d9ee0u);
    /* 006114ed */ if (result != 0) goto v_006114f8;
    /* 006114ef */ CM_GLOBAL(volatile u32, 0x9cf9e4u) = value;
    /* 006114f7 */ return result;
v_006114f8:
    /* 006114fe */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9d9ed0u);
    /* 00611508 */ if (result != 0) goto v_00611513;
    /* 0061150a */ CM_GLOBAL(volatile u32, 0x9cf9e8u) = value;
    /* 00611512 */ return result;
v_00611513:
    /* 00611519 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9d9eacu);
    /* 00611523 */ if (result != 0) goto v_0061152e;
    /* 00611525 */ CM_GLOBAL(volatile u32, 0x9cf9ecu) = value;
    /* 0061152d */ return result;
v_0061152e:
    /* 00611534 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9d9e90u);
    /* 0061153e */ if (result != 0) goto v_00611549;
    /* 00611540 */ CM_GLOBAL(volatile u32, 0x9cf9f0u) = value;
    /* 00611548 */ return result;
v_00611549:
    /* 0061154f */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9d9e74u);
    /* 00611559 */ if (result != 0) goto v_00611564;
    /* 0061155b */ CM_GLOBAL(volatile u32, 0x9cf9f4u) = value;
    /* 00611563 */ return result;
v_00611564:
    /* 0061156a */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9d9e60u);
    /* 00611574 */ if (result != 0) goto v_0061157f;
    /* 00611576 */ CM_GLOBAL(volatile u32, 0x9cf9f8u) = value;
    /* 0061157e */ return result;
v_0061157f:
    /* 00611585 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9d9e4cu);
    /* 0061158f */ if (result != 0) goto v_0061159a;
    /* 00611591 */ CM_GLOBAL(volatile u32, 0x9cf9fcu) = value;
    /* 00611599 */ return result;
v_0061159a:
    /* 006115a0 */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9ce524u);
    /* 006115aa */ if (result != 0) goto v_006115b5;
    /* 006115ac */ CM_GLOBAL(volatile u32, 0x9cfa00u) = value;
    /* 006115b4 */ return result;
v_006115b5:
    /* 006115bb */ result = cm_fn_00951c50(name, (const volatile u8 *)0x9d9e3cu);
    /* 006115c5 */ if (result != 0) goto v_006115d6;
    /* 006115c7 */ CM_GLOBAL(volatile u32, 0x9cfa04u) = value;
    /* 006115cf */ return result;
v_006115d0:
    /* 006115d0 */ CM_GLOBAL(volatile u32, 0x9cf8fcu) = value;
v_006115d6:
    /* 006115d8 */ return result;
}
