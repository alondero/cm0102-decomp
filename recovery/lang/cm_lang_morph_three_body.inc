/* Native C recovered from primary language-three control flow.
 * generate-morph-three.py pins original instructions and table bytes.
 * Labels are preferred-image VAs; no original instruction executes. */
char *CM_LANG_CDECL cm_lang_morph_language_three(unsigned char *frame) {
    char *word = local(frame, 0x1018u), *destination = word;
    const char *qualifier;
    cm_lang_u32 literal = 0;
    int value = 0, property2 = 0, column = 0;
    goto L_00674bb2;
L_00674bb2:
    qualifier = argument(frame, 0x2820u);
    /* b'upper' */
    if (equal(qualifier, TEXT(0x009f15ecu))) goto L_006759fa;
    /* b'Upper' */
    if (equal(qualifier, TEXT(0x009f081cu))) goto L_006759fa;
    /* b'lower' */
    if (equal(qualifier, TEXT(0x009f15c0u))) goto L_00674c69;
    /* b'upper_pos1' */
    if (equal(qualifier, TEXT(0x009f0810u))) goto L_00674cd2;
    /* b'upper_pos2' */
    if (equal(qualifier, TEXT(0x009f0804u))) goto L_00674d0e;
    /* b'upper_pos3' */
    if (equal(qualifier, TEXT(0x009f07f8u))) goto L_00674d4a;
    /* b'pos1' */
    if (equal(qualifier, TEXT(0x009f07f0u))) goto L_00674da7;
    /* b'pos2' */
    if (equal(qualifier, TEXT(0x009f07e8u))) goto L_00674df6;
    /* b'pos3' */
    if (equal(qualifier, TEXT(0x009f07e0u))) goto L_00674e32;
    /* b'Der' */
    if (equal(qualifier, TEXT(0x009f07dcu))) goto L_00674e71;
    /* b'der' */
    if (equal(qualifier, TEXT(0x009f07c0u))) goto L_00674ef4;
    /* b'den' */
    if (equal(qualifier, TEXT(0x009f07a4u))) goto L_006775f2;
    /* b'die' */
    if (equal(qualifier, TEXT(0x009f07a0u))) goto L_006775f2;
    /* b'Den' */
    if (equal(qualifier, TEXT(0x009f079cu))) goto L_00674fef;
    /* b'in der' */
    if (equal(qualifier, TEXT(0x009f078cu))) goto L_00675050;
    /* b'bei der' */
    if (equal(qualifier, TEXT(0x009f0774u))) goto L_006750c2;
    /* b'dem' */
    if (equal(qualifier, TEXT(0x009f0750u))) goto L_00675145;
    /* b'des' */
    if (equal(qualifier, TEXT(0x009f0744u))) goto L_006751a6;
    /* b'im' */
    if (equal(qualifier, TEXT(0x009f0738u))) goto L_00675218;
    /* b'von' */
    if (equal(qualifier, TEXT(0x009f0730u))) goto L_0067759a;
    /* b'vom' */
    if (equal(qualifier, TEXT(0x009f072cu))) goto L_0067759a;
    /* b'Von' */
    if (equal(qualifier, TEXT(0x009f0728u))) goto L_006752f1;
    /* b'zu' */
    if (equal(qualifier, TEXT(0x009f06fcu))) goto L_00675385;
    /* b'in' */
    if (equal(qualifier, TEXT(0x009f06dcu))) goto L_00675419;
    /* b'bei' */
    if (equal(qualifier, TEXT(0x009f06c4u))) goto L_0067548b;
    /* b'mit' */
    if (equal(qualifier, TEXT(0x009f06b8u))) goto L_006754ec;
    /* b'aus' */
    if (equal(qualifier, TEXT(0x009f0688u))) goto L_00675580;
    /* b'des kommenden' */
    if (equal(qualifier, TEXT(0x009f064cu))) goto L_00675614;
    /* b'seine' */
    if (equal(qualifier, TEXT(0x009f0624u))) goto L_00675686;
    /* b'in2' */
    if (equal(qualifier, TEXT(0x009f0610u))) goto L_006756f8;
    /* b'Dem' */
    if (equal(qualifier, TEXT(0x009f060cu))) goto L_00675759;
    /* b'am' */
    if (equal(qualifier, TEXT(0x009f0600u))) goto L_006757ba;
    /* b'an' */
    if (equal(qualifier, TEXT(0x009f15dcu))) goto L_0067583d;
    /* b'nach' */
    if (equal(qualifier, TEXT(0x009f05d4u))) goto L_006758af;
    /* b'ein' */
    if (equal(qualifier, TEXT(0x009f05c0u))) goto L_00675921;
    /* b'Ein' */
    if (equal(qualifier, TEXT(0x009f05acu))) goto L_00675997;
    /* b'das' */
    if (equal(qualifier, TEXT(0x009f0588u))) goto L_00675a8b;
    /* b'Das' */
    if (equal(qualifier, TEXT(0x009f0574u))) goto L_00675afd;
    /* b'im2' */
    if (equal(qualifier, TEXT(0x009f0568u))) goto L_00675b5e;
    /* b'zum' */
    if (equal(qualifier, TEXT(0x009f0564u))) goto L_00675bae;
    /* b'In der' */
    if (equal(qualifier, TEXT(0x009f0554u))) goto L_00675c0f;
    /* b'am kommenden' */
    if (equal(qualifier, TEXT(0x009f052cu))) goto L_00675ca3;
    /* b'das kommende' */
    if (equal(qualifier, TEXT(0x009f04e4u))) goto L_00675d37;
    /* b'H\xe4lt' */
    if (equal(qualifier, TEXT(0x009f04acu))) goto L_00675dba;
    /* b'hat der' */
    if (equal(qualifier, TEXT(0x009f0494u))) goto L_00675e2c;
    /* b'versucht der' */
    if (equal(qualifier, TEXT(0x009f044cu))) goto L_00675ed1;
    /* b'erreicht' */
    if (equal(qualifier, TEXT(0x009f03f4u))) goto L_00675f76;
    /* b'erreichte' */
    if (equal(qualifier, TEXT(0x009f03d0u))) goto L_00675feb;
    /* b'versuchte der' */
    if (equal(qualifier, TEXT(0x009f03a8u))) goto L_00676060;
    /* b'hatte' */
    if (equal(qualifier, TEXT(0x009f0364u))) goto L_006760f4;
    /* b'in3' */
    if (equal(qualifier, TEXT(0x009f0350u))) goto L_00676169;
    /* b'In3' */
    if (equal(qualifier, TEXT(0x009f034cu))) goto L_006761b9;
    /* b'Ein2' */
    if (equal(qualifier, TEXT(0x009f0338u))) goto L_0067621a;
    /* b'ein2' */
    if (equal(qualifier, TEXT(0x009f0328u))) goto L_0067627b;
    /* b'ein3' */
    if (equal(qualifier, TEXT(0x009f0318u))) goto L_006762dc;
    /* b'einem' */
    if (equal(qualifier, TEXT(0x009f0300u))) goto L_0067634e;
    /* b'kann' */
    if (equal(qualifier, TEXT(0x009f02f8u))) goto L_0067639e;
    /* b'befindet' */
    if (equal(qualifier, TEXT(0x009f02dcu))) goto L_00676413;
    /* b'erringt' */
    if (equal(qualifier, TEXT(0x009f02bcu))) goto L_00676488;
    /* b'sollte' */
    if (equal(qualifier, TEXT(0x009f029cu))) goto L_006764fd;
    /* b'erk\xe4mpft' */
    if (equal(qualifier, TEXT(0x009f027cu))) goto L_00676572;
    /* b'erk\xe4mpfte' */
    if (equal(qualifier, TEXT(0x009f0258u))) goto L_006765e7;
    /* b'gleicht' */
    if (equal(qualifier, TEXT(0x009f0238u))) goto L_0067665c;
    /* b'glich' */
    if (equal(qualifier, TEXT(0x009f0218u))) goto L_006766d1;
    /* b'f\xfchrte' */
    if (equal(qualifier, TEXT(0x009f01fcu))) goto L_00676746;
    /* b'erzielt' */
    if (equal(qualifier, TEXT(0x009f01e0u))) goto L_006767bb;
    /* b'erzielte' */
    if (equal(qualifier, TEXT(0x009f01bcu))) goto L_00676830;
    /* b'nahm' */
    if (equal(qualifier, TEXT(0x009f019cu))) goto L_006768a5;
    /* b'wechselte' */
    if (equal(qualifier, TEXT(0x009f0180u))) goto L_0067691a;
    /* b'brachte' */
    if (equal(qualifier, TEXT(0x009f0160u))) goto L_0067698f;
    /* b'versucht' */
    if (equal(qualifier, TEXT(0x009f013cu))) goto L_00676a04;
    /* b'versuchte' */
    if (equal(qualifier, TEXT(0x009f039cu))) goto L_00676a79;
    /* b'baut' */
    if (equal(qualifier, TEXT(0x009f011cu))) goto L_00676ae1;
    /* b'baute' */
    if (equal(qualifier, TEXT(0x009f0104u))) goto L_00676b56;
    /* b'war' */
    if (equal(qualifier, TEXT(0x009f00f0u))) goto L_00676bcb;
    /* b'gewann' */
    if (equal(qualifier, TEXT(0x009f00d8u))) goto L_00676c40;
    /* b'trifft' */
    if (equal(qualifier, TEXT(0x009f00bcu))) goto L_00676cb5;
    /* b'traf' */
    if (equal(qualifier, TEXT(0x009f00a0u))) goto L_00676d2a;
    /* b'spielte' */
    if (equal(qualifier, TEXT(0x009f0088u))) goto L_00676d9f;
    /* b'greift' */
    if (equal(qualifier, TEXT(0x009f0068u))) goto L_00676e14;
    /* b'griff' */
    if (equal(qualifier, TEXT(0x009f004cu))) goto L_00676e89;
    /* b'\xe4ndert' */
    if (equal(qualifier, TEXT(0x009f0030u))) goto L_00676efe;
    /* b'\xe4nderte' */
    if (equal(qualifier, TEXT(0x009f0018u))) goto L_00676f73;
    /* b'zeigt' */
    if (equal(qualifier, TEXT(0x009efff8u))) goto L_00676fe8;
    /* b'zeigte' */
    if (equal(qualifier, TEXT(0x009effe0u))) goto L_0067705d;
    /* b'verpasste' */
    if (equal(qualifier, TEXT(0x009effc0u))) goto L_006770d2;
    /* b'beginnt' */
    if (equal(qualifier, TEXT(0x009effa0u))) goto L_00677147;
    /* b'unternimmt' */
    if (equal(qualifier, TEXT(0x009eff7cu))) goto L_006771bc;
    /* b'unternahm' */
    if (equal(qualifier, TEXT(0x009eff54u))) goto L_00677231;
    /* b'stand' */
    if (equal(qualifier, TEXT(0x009eff30u))) goto L_006772a6;
    /* b'bleibt' */
    if (equal(qualifier, TEXT(0x009eff14u))) goto L_0067731b;
    /* b'blieb' */
    if (equal(qualifier, TEXT(0x009efef8u))) goto L_00677390;
    /* b'widersteht' */
    if (equal(qualifier, TEXT(0x009efee4u))) goto L_00677405;
    /* b'widerstand' */
    if (equal(qualifier, TEXT(0x009efebcu))) goto L_0067747a;
    /* b'versuchte der' */
    if (equal(qualifier, TEXT(0x009f03a8u))) goto L_006774ef;
    if (*(const volatile unsigned char *)qualifier) copy_literal((char *)qualifier, 0x00ad9c64u);
    goto L_00675a12;
L_00674c69:
    lower_word_in_place(word);
    goto L_00675a12;
L_00674cd2:
    column = 1;
    goto L_00674d4c;
L_00674d0e:
    column = 2;
    goto L_00674d4c;
L_00674d4a:
    column = 3;
    goto L_00674d4c;
L_00674d4c:
    cm_lang_morph_table_b(word, (cm_lang_u32)column);
    *(volatile char *)word = (char)UPPER(signed_byte(*(const volatile unsigned char *)word));
    goto L_00675a12;
L_00674da7:
    column = 1;
    goto L_00674da9;
L_00674da9:
    cm_lang_morph_table_b(word, (cm_lang_u32)column);
    goto L_00675a12;
L_00674df6:
    column = 2;
    goto L_00674da9;
L_00674e32:
    column = 3;
    goto L_00674da9;
L_00674e71:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680874u + (cm_lang_u32)value * 4u)) {
    case 0x00674e89u: goto L_00674e89;
    case 0x00674e9au: goto L_00674e9a;
    case 0x00674eabu: goto L_00674eab;
    case 0x0067760au: goto L_0067760a;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00674e89:
    destination = local(frame, 0x818u);
    literal = 0x9f07d4u;
    goto L_00677616;
L_00674e9a:
    destination = local(frame, 0x818u);
    literal = 0x9f07ccu;
    goto L_00677616;
L_00674eab:
    destination = local(frame, 0x818u);
    literal = 0x9f07c4u;
    goto L_00677616;
L_00674ef4:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680888u + (cm_lang_u32)value * 4u)) {
    case 0x00674f0cu: goto L_00674f0c;
    case 0x00674f1du: goto L_00674f1d;
    case 0x00674f2eu: goto L_00674f2e;
    case 0x0067760au: goto L_0067760a;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00674f0c:
    destination = local(frame, 0x818u);
    literal = 0x9f07b8u;
    goto L_00677616;
L_00674f1d:
    destination = local(frame, 0x818u);
    literal = 0x9f07b0u;
    goto L_00677616;
L_00674f2e:
    destination = local(frame, 0x818u);
    literal = 0x9f07a8u;
    goto L_00677616;
L_00674fef:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x68089cu + (cm_lang_u32)value * 4u)) {
    case 0x00674e9au: goto L_00674e9a;
    case 0x00674eabu: goto L_00674eab;
    case 0x00675007u: goto L_00675007;
    case 0x0067760au: goto L_0067760a;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00675007:
    destination = local(frame, 0x818u);
    literal = 0x9f0794u;
    goto L_00677616;
L_00675050:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x6808b0u + (cm_lang_u32)value * 4u)) {
    case 0x00675068u: goto L_00675068;
    case 0x00675079u: goto L_00675079;
    case 0x00675230u: goto L_00675230;
    case 0x00675710u: goto L_00675710;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00675068:
    destination = local(frame, 0x818u);
    literal = 0x9f0784u;
    goto L_00677616;
L_00675079:
    destination = local(frame, 0x818u);
    literal = 0x9f077cu;
    goto L_00677616;
L_006750c2:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x6808c4u + (cm_lang_u32)value * 4u)) {
    case 0x006750dau: goto L_006750da;
    case 0x006750ebu: goto L_006750eb;
    case 0x006750fcu: goto L_006750fc;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_006750da:
    destination = local(frame, 0x818u);
    literal = 0x9f076cu;
    goto L_00677616;
L_006750eb:
    destination = local(frame, 0x818u);
    literal = 0x9f0760u;
    goto L_00677616;
L_006750fc:
    destination = local(frame, 0x818u);
    literal = 0x9f0754u;
    goto L_00677616;
L_00675145:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x6808d8u + (cm_lang_u32)value * 4u)) {
    case 0x00674f0cu: goto L_00674f0c;
    case 0x0067515du: goto L_0067515d;
    case 0x00675ab4u: goto L_00675ab4;
    case 0x0067760au: goto L_0067760a;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067515d:
    destination = local(frame, 0x818u);
    literal = 0x9f0748u;
    goto L_00677616;
L_006751a6:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x6808ecu + (cm_lang_u32)value * 4u)) {
    case 0x00674f0cu: goto L_00674f0c;
    case 0x006751beu: goto L_006751be;
    case 0x006751cfu: goto L_006751cf;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_006751be:
    destination = local(frame, 0x818u);
    literal = 0x9f073cu;
    goto L_00677616;
L_006751cf:
    destination = local(frame, 0x818u);
    literal = 0x9f1364u;
    goto L_00677616;
L_00675218:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680900u + (cm_lang_u32)value * 4u)) {
    case 0x00675068u: goto L_00675068;
    case 0x00675079u: goto L_00675079;
    case 0x00675230u: goto L_00675230;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00675230:
    destination = local(frame, 0x818u);
    literal = 0x9f0734u;
    goto L_00677616;
L_006752f1:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680914u + (cm_lang_u32)value * 4u)) {
    case 0x00675309u: goto L_00675309;
    case 0x0067531au: goto L_0067531a;
    case 0x0067532bu: goto L_0067532b;
    case 0x0067533cu: goto L_0067533c;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00675309:
    destination = local(frame, 0x818u);
    literal = 0x9f0720u;
    goto L_00677616;
L_0067531a:
    destination = local(frame, 0x818u);
    literal = 0x9f0714u;
    goto L_00677616;
L_0067532b:
    destination = local(frame, 0x818u);
    literal = 0x9f070cu;
    goto L_00677616;
L_0067533c:
    destination = local(frame, 0x818u);
    literal = 0x9f0700u;
    goto L_00677616;
L_00675385:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680928u + (cm_lang_u32)value * 4u)) {
    case 0x0067539du: goto L_0067539d;
    case 0x006753aeu: goto L_006753ae;
    case 0x006753bfu: goto L_006753bf;
    case 0x006753d0u: goto L_006753d0;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067539d:
    destination = local(frame, 0x818u);
    literal = 0x9f06f8u;
    goto L_00677616;
L_006753ae:
    destination = local(frame, 0x818u);
    literal = 0x9f06f0u;
    goto L_00677616;
L_006753bf:
    destination = local(frame, 0x818u);
    literal = 0x9f06e8u;
    goto L_00677616;
L_006753d0:
    destination = local(frame, 0x818u);
    literal = 0x9f06e0u;
    goto L_00677616;
L_00675419:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x68093cu + (cm_lang_u32)value * 4u)) {
    case 0x00675068u: goto L_00675068;
    case 0x00675431u: goto L_00675431;
    case 0x00675442u: goto L_00675442;
    case 0x00675710u: goto L_00675710;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00675431:
    destination = local(frame, 0x818u);
    literal = 0x9f06d4u;
    goto L_00677616;
L_00675442:
    destination = local(frame, 0x818u);
    literal = 0x9f06c8u;
    goto L_00677616;
L_0067548b:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680950u + (cm_lang_u32)value * 4u)) {
    case 0x006750dau: goto L_006750da;
    case 0x006750ebu: goto L_006750eb;
    case 0x006750fcu: goto L_006750fc;
    case 0x006754a3u: goto L_006754a3;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_006754a3:
    destination = local(frame, 0x818u);
    literal = 0x9f06bcu;
    goto L_00677616;
L_006754ec:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680964u + (cm_lang_u32)value * 4u)) {
    case 0x00675504u: goto L_00675504;
    case 0x00675515u: goto L_00675515;
    case 0x00675526u: goto L_00675526;
    case 0x00675537u: goto L_00675537;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00675504:
    destination = local(frame, 0x818u);
    literal = 0x9f06b0u;
    goto L_00677616;
L_00675515:
    destination = local(frame, 0x818u);
    literal = 0x9f06a4u;
    goto L_00677616;
L_00675526:
    destination = local(frame, 0x818u);
    literal = 0x9f0698u;
    goto L_00677616;
L_00675537:
    destination = local(frame, 0x818u);
    literal = 0x9f068cu;
    goto L_00677616;
L_00675580:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680978u + (cm_lang_u32)value * 4u)) {
    case 0x00675598u: goto L_00675598;
    case 0x006755a9u: goto L_006755a9;
    case 0x006755bau: goto L_006755ba;
    case 0x006755cbu: goto L_006755cb;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00675598:
    destination = local(frame, 0x818u);
    literal = 0x9f0680u;
    goto L_00677616;
L_006755a9:
    destination = local(frame, 0x818u);
    literal = 0x9f0674u;
    goto L_00677616;
L_006755ba:
    destination = local(frame, 0x818u);
    literal = 0x9f0668u;
    goto L_00677616;
L_006755cb:
    destination = local(frame, 0x818u);
    literal = 0x9f065cu;
    goto L_00677616;
L_00675614:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x68098cu + (cm_lang_u32)value * 4u)) {
    case 0x0067562cu: goto L_0067562c;
    case 0x0067563du: goto L_0067563d;
    case 0x00675710u: goto L_00675710;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067562c:
    destination = local(frame, 0x818u);
    literal = 0x9f063cu;
    goto L_00677616;
L_0067563d:
    destination = local(frame, 0x818u);
    literal = 0x9f062cu;
    goto L_00677616;
L_00675686:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x6809a0u + (cm_lang_u32)value * 4u)) {
    case 0x0067569eu: goto L_0067569e;
    case 0x006756afu: goto L_006756af;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067569e:
    destination = local(frame, 0x818u);
    literal = 0x9f061cu;
    goto L_00677616;
L_006756af:
    destination = local(frame, 0x818u);
    literal = 0x9f0614u;
    goto L_00677616;
L_006756f8:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x6809b4u + (cm_lang_u32)value * 4u)) {
    case 0x00675079u: goto L_00675079;
    case 0x006753aeu: goto L_006753ae;
    case 0x006753d0u: goto L_006753d0;
    case 0x00675710u: goto L_00675710;
    case 0x006758d8u: goto L_006758d8;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00675710:
    destination = local(frame, 0x818u);
    literal = 0x9f0e14u;
    goto L_00677616;
L_00675759:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x6809c8u + (cm_lang_u32)value * 4u)) {
    case 0x00674e89u: goto L_00674e89;
    case 0x00675007u: goto L_00675007;
    case 0x00675771u: goto L_00675771;
    case 0x0067760au: goto L_0067760a;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00675771:
    destination = local(frame, 0x818u);
    literal = 0x9f0604u;
    goto L_00677616;
L_006757ba:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x6809dcu + (cm_lang_u32)value * 4u)) {
    case 0x006757d2u: goto L_006757d2;
    case 0x006757e3u: goto L_006757e3;
    case 0x006757f4u: goto L_006757f4;
    case 0x00675cbbu: goto L_00675cbb;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_006757d2:
    destination = local(frame, 0x818u);
    literal = 0x9f05f8u;
    goto L_00677616;
L_006757e3:
    destination = local(frame, 0x818u);
    literal = 0x9f05f4u;
    goto L_00677616;
L_006757f4:
    destination = local(frame, 0x818u);
    literal = 0x9f05ecu;
    goto L_00677616;
L_0067583d:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x6809f0u + (cm_lang_u32)value * 4u)) {
    case 0x006757f4u: goto L_006757f4;
    case 0x00675855u: goto L_00675855;
    case 0x00675866u: goto L_00675866;
    case 0x00675cbbu: goto L_00675cbb;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00675855:
    destination = local(frame, 0x818u);
    literal = 0x9f05e4u;
    goto L_00677616;
L_00675866:
    destination = local(frame, 0x818u);
    literal = 0x9f05dcu;
    goto L_00677616;
L_006758af:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680a04u + (cm_lang_u32)value * 4u)) {
    case 0x00675079u: goto L_00675079;
    case 0x006753aeu: goto L_006753ae;
    case 0x006758c7u: goto L_006758c7;
    case 0x006758d8u: goto L_006758d8;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_006758c7:
    destination = local(frame, 0x818u);
    literal = 0x9f05ccu;
    goto L_00677616;
L_006758d8:
    destination = local(frame, 0x818u);
    literal = 0x9f05c4u;
    goto L_00677616;
L_00675921:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680a18u + (cm_lang_u32)value * 4u)) {
    case 0x00675939u: goto L_00675939;
    case 0x0067594au: goto L_0067594a;
    case 0x0067760au: goto L_0067760a;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00675939:
    destination = local(frame, 0x818u);
    literal = 0x9f05b8u;
    goto L_00677616;
L_0067594a:
    destination = local(frame, 0x818u);
    literal = 0x9f05b0u;
    goto L_00677616;
L_00675997:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680a2cu + (cm_lang_u32)value * 4u)) {
    case 0x006759afu: goto L_006759af;
    case 0x006759c0u: goto L_006759c0;
    case 0x006759d1u: goto L_006759d1;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_006759af:
    destination = local(frame, 0x818u);
    literal = 0x9f05a4u;
    goto L_00677616;
L_006759c0:
    destination = local(frame, 0x818u);
    literal = 0x9f059cu;
    goto L_00677616;
L_006759d1:
    literal = 0xad9c64u;
    destination = local(frame, 0x818u);
    copy_literal(destination, literal);
    goto L_006759fa;
L_006759fa:
    *(volatile char *)word = (char)UPPER(signed_byte(*(const volatile unsigned char *)word));
    goto L_00675a12;
L_00675a12:
    qualifier = argument(frame, 0x2824u);
    { int default_scope = equal(qualifier, TEXT(0x009f058cu));
      property2 = signed_byte(*(const volatile unsigned char *)(frame + 0x282cu));
      if (default_scope) goto L_006776d6;
    }
    /* b'e' */
    if (equal(qualifier, TEXT(0x0099c360u))) goto L_00677870;
    /* b'es' */
    if (equal(qualifier, TEXT(0x009efd1cu))) goto L_00677a0a;
    /* b'en' */
    if (equal(qualifier, TEXT(0x009f1558u))) goto L_00677ba4;
    /* b'en2' */
    if (equal(qualifier, TEXT(0x009efb58u))) goto L_00677d3e;
    /* b'erreicht' */
    if (equal(qualifier, TEXT(0x009f03f4u))) goto L_00677ed4;
    /* b'setzt' */
    if (equal(qualifier, TEXT(0x009efa60u))) goto L_00677f3a;
    /* b'ist' */
    if (equal(qualifier, TEXT(0x009efa4cu))) goto L_00677fa0;
    /* b'hat' */
    if (equal(qualifier, TEXT(0x009efa38u))) goto L_00678006;
    /* b'wird' */
    if (equal(qualifier, TEXT(0x009efa20u))) goto L_0067806c;
    /* b'wurde' */
    if (equal(qualifier, TEXT(0x009efa10u))) goto L_006780c8;
    /* b'gewinnt' */
    if (equal(qualifier, TEXT(0x009ef9f8u))) goto L_0067812e;
    /* b'steigt' */
    if (equal(qualifier, TEXT(0x009ef9d8u))) goto L_00678194;
    /* b'verliert' */
    if (equal(qualifier, TEXT(0x009ef9b8u))) goto L_0067a639;
    /* b'verlieren' */
    if (equal(qualifier, TEXT(0x009ef9acu))) goto L_0067a639;
    /* b'vermeidet' */
    if (equal(qualifier, TEXT(0x009ef9a0u))) goto L_0067826a;
    /* b'erh\xe4lt' */
    if (equal(qualifier, TEXT(0x009ef980u))) goto L_006782d0;
    /* b'nimmt' */
    if (equal(qualifier, TEXT(0x009ef964u))) goto L_00678336;
    /* b'verpasst' */
    if (equal(qualifier, TEXT(0x009ef948u))) goto L_0067839c;
    /* b's' */
    if (equal(qualifier, TEXT(0x0099c368u))) goto L_00678406;
    /* b'qualifiziert' */
    if (equal(qualifier, TEXT(0x009ef920u))) goto L_006784d8;
    /* b'steht' */
    if (equal(qualifier, TEXT(0x009ef8f8u))) goto L_0067853b;
    /* b'schlie\xdft' */
    if (equal(qualifier, TEXT(0x009ef8e4u))) goto L_00678590;
    /* b'stellt' */
    if (equal(qualifier, TEXT(0x009ef8c4u))) goto L_006785f3;
    /* b'hat seinen' */
    if (equal(qualifier, TEXT(0x009ef8a4u))) goto L_00678656;
    /* b'widerspricht' */
    if (equal(qualifier, TEXT(0x009ef878u))) goto L_006786b9;
    /* b'bestreitet' */
    if (equal(qualifier, TEXT(0x009ef84cu))) goto L_0067871c;
    /* b'informiert' */
    if (equal(qualifier, TEXT(0x009ef828u))) goto L_0067877f;
    /* b'gibt' */
    if (equal(qualifier, TEXT(0x009ef804u))) goto L_006787e2;
    /* b'darf' */
    if (equal(qualifier, TEXT(0x009ef7ecu))) goto L_00678845;
    /* b'mu\xdf' */
    if (equal(qualifier, TEXT(0x009ef7d8u))) goto L_006788a8;
    /* b'bietet' */
    if (equal(qualifier, TEXT(0x009ef7c0u))) goto L_0067890b;
    /* b'lehnt' */
    if (equal(qualifier, TEXT(0x009ef7a8u))) goto L_0067896e;
    /* b'fordert' */
    if (equal(qualifier, TEXT(0x009ef790u))) goto L_006789d1;
    /* b'l\xe4dt' */
    if (equal(qualifier, TEXT(0x009ef77cu))) goto L_00678a26;
    /* b'er' */
    if (equal(qualifier, TEXT(0x009ef768u))) goto L_00678a89;
    /* b'steckt' */
    if (equal(qualifier, TEXT(0x009ef760u))) goto L_00678ade;
    /* b'ben\xf6tigt' */
    if (equal(qualifier, TEXT(0x009ef740u))) goto L_00678b41;
    /* b'will' */
    if (equal(qualifier, TEXT(0x009ef720u))) goto L_00678ba4;
    /* b'wird der' */
    if (equal(qualifier, TEXT(0x009ef704u))) goto L_00678c07;
    /* b'spannt' */
    if (equal(qualifier, TEXT(0x009ef6c4u))) goto L_00678c94;
    /* b'stoppt' */
    if (equal(qualifier, TEXT(0x009ef6a8u))) goto L_00678cf7;
    /* b'bringt' */
    if (equal(qualifier, TEXT(0x009ef68cu))) goto L_00678d5a;
    /* b'h\xe4lt' */
    if (equal(qualifier, TEXT(0x009ef67cu))) goto L_00678daf;
    /* b'gilt' */
    if (equal(qualifier, TEXT(0x009ef664u))) goto L_00678e12;
    /* b'marschiert' */
    if (equal(qualifier, TEXT(0x009ef648u))) goto L_00678e75;
    /* b'f\xfchrt' */
    if (equal(qualifier, TEXT(0x009ef624u))) goto L_00678ed8;
    /* b'macht' */
    if (equal(qualifier, TEXT(0x009ef614u))) goto L_00678f2d;
    /* b'will seinem' */
    if (equal(qualifier, TEXT(0x009ef5f8u))) goto L_00678f90;
    /* b'w\xfcrde' */
    if (equal(qualifier, TEXT(0x009ef5d0u))) goto L_00678ff3;
    /* b'verlangt' */
    if (equal(qualifier, TEXT(0x009ef5b4u))) goto L_00679056;
    /* b'verpflichtet' */
    if (equal(qualifier, TEXT(0x009ef58cu))) goto L_006790b9;
    /* b'wechselt' */
    if (equal(qualifier, TEXT(0x009ef560u))) goto L_0067911c;
    /* b'besiegelt' */
    if (equal(qualifier, TEXT(0x009ef548u))) goto L_00679171;
    /* b'holt' */
    if (equal(qualifier, TEXT(0x009ef528u))) goto L_006791d4;
    /* b'sichert' */
    if (equal(qualifier, TEXT(0x009ef510u))) goto L_00679237;
    /* b'spielt' */
    if (equal(qualifier, TEXT(0x009ef4f0u))) goto L_0067929a;
    /* b'legt' */
    if (equal(qualifier, TEXT(0x009ef4e0u))) goto L_006792ef;
    /* b'hat sein' */
    if (equal(qualifier, TEXT(0x009ef4c4u))) goto L_00679352;
    /* b'geht' */
    if (equal(qualifier, TEXT(0x009ef4a4u))) goto L_006793b5;
    /* b'bittet' */
    if (equal(qualifier, TEXT(0x009ef48cu))) goto L_00679418;
    /* b'bricht' */
    if (equal(qualifier, TEXT(0x009ef474u))) goto L_0067947b;
    /* b'm\xf6chte' */
    if (equal(qualifier, TEXT(0x009ef458u))) goto L_006794de;
    /* b'zieht' */
    if (equal(qualifier, TEXT(0x009ef43cu))) goto L_00679541;
    /* b'siegt' */
    if (equal(qualifier, TEXT(0x009ef424u))) goto L_006795a4;
    /* b'dabei ist' */
    if (equal(qualifier, TEXT(0x009ef408u))) goto L_00679607;
    /* b'unmittelbar davor steht' */
    if (equal(qualifier, TEXT(0x009ef3d8u))) goto L_0067966a;
    /* b'das Zeug dazu hat' */
    if (equal(qualifier, TEXT(0x009ef38cu))) goto L_006796cd;
    /* b'nahm' */
    if (equal(qualifier, TEXT(0x009f019cu))) goto L_00679730;
    /* b'wechselt' */
    if (equal(qualifier, TEXT(0x009ef560u))) goto L_00679796;
    /* b'wechselte' */
    if (equal(qualifier, TEXT(0x009f0180u))) goto L_006797f2;
    /* b'bringt' */
    if (equal(qualifier, TEXT(0x009ef68cu))) goto L_00679858;
    /* b'brachte' */
    if (equal(qualifier, TEXT(0x009f0160u))) goto L_006798b4;
    /* b'versucht' */
    if (equal(qualifier, TEXT(0x009f013cu))) goto L_0067991a;
    /* b'versuchte' */
    if (equal(qualifier, TEXT(0x009f039cu))) goto L_00679980;
    /* b'baut' */
    if (equal(qualifier, TEXT(0x009f011cu))) goto L_006799e6;
    /* b'baute' */
    if (equal(qualifier, TEXT(0x009f0104u))) goto L_00679a4c;
    /* b'war' */
    if (equal(qualifier, TEXT(0x009f00f0u))) goto L_00679ab2;
    /* b'hatte' */
    if (equal(qualifier, TEXT(0x009f0364u))) goto L_00679b18;
    /* b'gewann' */
    if (equal(qualifier, TEXT(0x009f00d8u))) goto L_00679b74;
    /* b'gleicht' */
    if (equal(qualifier, TEXT(0x009f0238u))) goto L_00679bda;
    /* b'glich' */
    if (equal(qualifier, TEXT(0x009f0218u))) goto L_00679c40;
    /* b'trifft' */
    if (equal(qualifier, TEXT(0x009f00bcu))) goto L_00679ca6;
    /* b'traf' */
    if (equal(qualifier, TEXT(0x009f00a0u))) goto L_00679d0c;
    /* b'hatte' */
    if (equal(qualifier, TEXT(0x009f0364u))) goto L_00679d72;
    /* b'schlug' */
    if (equal(qualifier, TEXT(0x009ef238u))) goto L_00679dd8;
    /* b'erreichte' */
    if (equal(qualifier, TEXT(0x009f03d0u))) goto L_00679e3e;
    /* b'f\xfchrt' */
    if (equal(qualifier, TEXT(0x009ef624u))) goto L_00679ea4;
    /* b'f\xfchrte' */
    if (equal(qualifier, TEXT(0x009f01fcu))) goto L_00679f00;
    /* b'spielt' */
    if (equal(qualifier, TEXT(0x009ef4f0u))) goto L_00679f66;
    /* b'spielte' */
    if (equal(qualifier, TEXT(0x009f0088u))) goto L_00679fc2;
    /* b'zeigte' */
    if (equal(qualifier, TEXT(0x009effe0u))) goto L_0067a028;
    /* b'zeigt' */
    if (equal(qualifier, TEXT(0x009efff8u))) goto L_0067a08e;
    /* b'wollte' */
    if (equal(qualifier, TEXT(0x009ef1a0u))) goto L_0067a0f4;
    /* b'fordert' */
    if (equal(qualifier, TEXT(0x009ef790u))) goto L_0067a15a;
    /* b'forderte' */
    if (equal(qualifier, TEXT(0x009ef174u))) goto L_0067a1b6;
    /* b'beginnt' */
    if (equal(qualifier, TEXT(0x009effa0u))) goto L_0067a21c;
    /* b'steht' */
    if (equal(qualifier, TEXT(0x009ef8f8u))) goto L_0067a282;
    /* b'stand' */
    if (equal(qualifier, TEXT(0x009eff30u))) goto L_0067a2de;
    /* b'schlug zur\xfcck und glich' */
    if (equal(qualifier, TEXT(0x009ef110u))) goto L_0067a344;
    /* b'nahm sich ein wenig zur\xfcck und spielte' */
    if (equal(qualifier, TEXT(0x009ef0acu))) goto L_0067a3aa;
    /* b'unternimmt' */
    if (equal(qualifier, TEXT(0x009eff7cu))) goto L_0067a410;
    /* b'unternahm' */
    if (equal(qualifier, TEXT(0x009eff54u))) goto L_0067a476;
    /* b', welche' */
    if (equal(qualifier, TEXT(0x009ef024u))) goto L_0067a4ce;
    /* b'kann' */
    if (equal(qualifier, TEXT(0x009f02f8u))) goto L_0067a53f;
    /* b'befindet' */
    if (equal(qualifier, TEXT(0x009f02dcu))) goto L_0067a5a5;
    /* b'erringt' */
    if (equal(qualifier, TEXT(0x009f02bcu))) goto L_0067a60b;
    *(volatile cm_lang_u32 *)(frame + 0x14u) = 0;
    goto L_0067a683;
L_00675a8b:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680a40u + (cm_lang_u32)value * 4u)) {
    case 0x00674f2eu: goto L_00674f2e;
    case 0x00675aa3u: goto L_00675aa3;
    case 0x00675ab4u: goto L_00675ab4;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00675aa3:
    destination = local(frame, 0x818u);
    literal = 0x9f0580u;
    goto L_00677616;
L_00675ab4:
    destination = local(frame, 0x818u);
    literal = 0x9f0578u;
    goto L_00677616;
L_00675afd:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680a54u + (cm_lang_u32)value * 4u)) {
    case 0x00674eabu: goto L_00674eab;
    case 0x00675007u: goto L_00675007;
    case 0x00675b15u: goto L_00675b15;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00675b15:
    destination = local(frame, 0x818u);
    literal = 0x9f056cu;
    goto L_00677616;
L_00675b5e:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680a68u + (cm_lang_u32)value * 4u)) {
    case 0x00675068u: goto L_00675068;
    case 0x00675079u: goto L_00675079;
    case 0x00675230u: goto L_00675230;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00675bae:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680a7cu + (cm_lang_u32)value * 4u)) {
    case 0x0067539du: goto L_0067539d;
    case 0x006753bfu: goto L_006753bf;
    case 0x006753d0u: goto L_006753d0;
    case 0x00675bc6u: goto L_00675bc6;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00675bc6:
    destination = local(frame, 0x818u);
    literal = 0x9f055cu;
    goto L_00677616;
L_00675c0f:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680a90u + (cm_lang_u32)value * 4u)) {
    case 0x00675c27u: goto L_00675c27;
    case 0x00675c38u: goto L_00675c38;
    case 0x00675c49u: goto L_00675c49;
    case 0x00675c5au: goto L_00675c5a;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00675c27:
    destination = local(frame, 0x818u);
    literal = 0x9f0550u;
    goto L_00677616;
L_00675c38:
    destination = local(frame, 0x818u);
    literal = 0x9f0548u;
    goto L_00677616;
L_00675c49:
    destination = local(frame, 0x818u);
    literal = 0x9f0544u;
    goto L_00677616;
L_00675c5a:
    destination = local(frame, 0x818u);
    literal = 0x9f053cu;
    goto L_00677616;
L_00675ca3:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680aa4u + (cm_lang_u32)value * 4u)) {
    case 0x00675cbbu: goto L_00675cbb;
    case 0x00675cccu: goto L_00675ccc;
    case 0x00675cddu: goto L_00675cdd;
    case 0x00675ceeu: goto L_00675cee;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00675cbb:
    destination = local(frame, 0x818u);
    literal = 0x9f15d8u;
    goto L_00677616;
L_00675ccc:
    destination = local(frame, 0x818u);
    literal = 0x9f0518u;
    goto L_00677616;
L_00675cdd:
    destination = local(frame, 0x818u);
    literal = 0x9f0508u;
    goto L_00677616;
L_00675cee:
    destination = local(frame, 0x818u);
    literal = 0x9f04f4u;
    goto L_00677616;
L_00675d37:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680ab8u + (cm_lang_u32)value * 4u)) {
    case 0x00675d4fu: goto L_00675d4f;
    case 0x00675d60u: goto L_00675d60;
    case 0x00675d71u: goto L_00675d71;
    case 0x0067760au: goto L_0067760a;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00675d4f:
    destination = local(frame, 0x818u);
    literal = 0x9f04d4u;
    goto L_00677616;
L_00675d60:
    destination = local(frame, 0x818u);
    literal = 0x9f04c4u;
    goto L_00677616;
L_00675d71:
    destination = local(frame, 0x818u);
    literal = 0x9f04b4u;
    goto L_00677616;
L_00675dba:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680accu + (cm_lang_u32)value * 4u)) {
    case 0x00675dd2u: goto L_00675dd2;
    case 0x00675de3u: goto L_00675de3;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00675dd2:
    destination = local(frame, 0x818u);
    literal = 0x9f04a4u;
    goto L_00677616;
L_00675de3:
    destination = local(frame, 0x818u);
    literal = 0x9f049cu;
    goto L_00677616;
L_00675e2c:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680ae0u + (cm_lang_u32)value * 4u)) {
    case 0x00675e44u: goto L_00675e44;
    case 0x00675e55u: goto L_00675e55;
    case 0x00675e66u: goto L_00675e66;
    case 0x00675e77u: goto L_00675e77;
    case 0x00675e88u: goto L_00675e88;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00675e44:
    destination = local(frame, 0x818u);
    literal = 0x9f048cu;
    goto L_00677616;
L_00675e55:
    destination = local(frame, 0x818u);
    literal = 0x9f0480u;
    goto L_00677616;
L_00675e66:
    destination = local(frame, 0x818u);
    literal = 0x9f0474u;
    goto L_00677616;
L_00675e77:
    destination = local(frame, 0x818u);
    literal = 0x9f0468u;
    goto L_00677616;
L_00675e88:
    destination = local(frame, 0x818u);
    literal = 0x9f045cu;
    goto L_00677616;
L_00675ed1:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680af4u + (cm_lang_u32)value * 4u)) {
    case 0x00675ee9u: goto L_00675ee9;
    case 0x00675efau: goto L_00675efa;
    case 0x00675f0bu: goto L_00675f0b;
    case 0x00675f1cu: goto L_00675f1c;
    case 0x00675f2du: goto L_00675f2d;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00675ee9:
    destination = local(frame, 0x818u);
    literal = 0x9f0440u;
    goto L_00677616;
L_00675efa:
    destination = local(frame, 0x818u);
    literal = 0x9f0430u;
    goto L_00677616;
L_00675f0b:
    destination = local(frame, 0x818u);
    literal = 0x9f0420u;
    goto L_00677616;
L_00675f1c:
    destination = local(frame, 0x818u);
    literal = 0x9f0410u;
    goto L_00677616;
L_00675f2d:
    destination = local(frame, 0x818u);
    literal = 0x9f0400u;
    goto L_00677616;
L_00675f76:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00675fa2;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f03e8u;
    goto L_00677616;
L_00675fa2:
    destination = local(frame, 0x818u);
    literal = 0x9f03dcu;
    goto L_00677616;
L_00675feb:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00676017;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f03c4u;
    goto L_00677616;
L_00676017:
    destination = local(frame, 0x818u);
    literal = 0x9f03b8u;
    goto L_00677616;
L_00676060:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680b08u + (cm_lang_u32)value * 4u)) {
    case 0x00676078u: goto L_00676078;
    case 0x00676089u: goto L_00676089;
    case 0x0067609au: goto L_0067609a;
    case 0x006760abu: goto L_006760ab;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00676078:
    destination = local(frame, 0x818u);
    literal = 0x9f039cu;
    goto L_00677616;
L_00676089:
    destination = local(frame, 0x818u);
    literal = 0x9f038cu;
    goto L_00677616;
L_0067609a:
    destination = local(frame, 0x818u);
    literal = 0x9f037cu;
    goto L_00677616;
L_006760ab:
    destination = local(frame, 0x818u);
    literal = 0x9f036cu;
    goto L_00677616;
L_006760f4:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00676120;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f035cu;
    goto L_00677616;
L_00676120:
    destination = local(frame, 0x818u);
    literal = 0x9f0354u;
    goto L_00677616;
L_00676169:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680b1cu + (cm_lang_u32)value * 4u)) {
    case 0x00675068u: goto L_00675068;
    case 0x006750fcu: goto L_006750fc;
    case 0x00675230u: goto L_00675230;
    case 0x00675710u: goto L_00675710;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_006761b9:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680b30u + (cm_lang_u32)value * 4u)) {
    case 0x00675c27u: goto L_00675c27;
    case 0x00675c38u: goto L_00675c38;
    case 0x00675c49u: goto L_00675c49;
    case 0x006761d1u: goto L_006761d1;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_006761d1:
    destination = local(frame, 0x818u);
    literal = 0x9f0340u;
    goto L_00677616;
L_0067621a:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680b44u + (cm_lang_u32)value * 4u)) {
    case 0x006759afu: goto L_006759af;
    case 0x006759c0u: goto L_006759c0;
    case 0x00676232u: goto L_00676232;
    case 0x0067760au: goto L_0067760a;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00676232:
    destination = local(frame, 0x818u);
    literal = 0x9f0330u;
    goto L_00677616;
L_0067627b:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680b58u + (cm_lang_u32)value * 4u)) {
    case 0x00675939u: goto L_00675939;
    case 0x0067594au: goto L_0067594a;
    case 0x00676293u: goto L_00676293;
    case 0x0067760au: goto L_0067760a;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00676293:
    destination = local(frame, 0x818u);
    literal = 0x9f0320u;
    goto L_00677616;
L_006762dc:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680b6cu + (cm_lang_u32)value * 4u)) {
    case 0x006762f4u: goto L_006762f4;
    case 0x00676305u: goto L_00676305;
    case 0x0067760au: goto L_0067760a;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_006762f4:
    destination = local(frame, 0x818u);
    literal = 0x9f0310u;
    goto L_00677616;
L_00676305:
    destination = local(frame, 0x818u);
    literal = 0x9f0308u;
    goto L_00677616;
L_0067634e:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680b80u + (cm_lang_u32)value * 4u)) {
    case 0x006762f4u: goto L_006762f4;
    case 0x00676305u: goto L_00676305;
    case 0x0067760au: goto L_0067760a;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067639e:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_006763ca;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f02f0u;
    goto L_00677672;
L_006763ca:
    destination = local(frame, 0x818u);
    literal = 0x9f02e8u;
    goto L_00677672;
L_00676413:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_0067643f;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f02d0u;
    goto L_00677672;
L_0067643f:
    destination = local(frame, 0x818u);
    literal = 0x9f02c4u;
    goto L_00677672;
L_00676488:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_006764b4;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f02b0u;
    goto L_00677672;
L_006764b4:
    destination = local(frame, 0x818u);
    literal = 0x9f02a4u;
    goto L_00677672;
L_006764fd:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00676529;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f0290u;
    goto L_00677672;
L_00676529:
    destination = local(frame, 0x818u);
    literal = 0x9f0288u;
    goto L_00677672;
L_00676572:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_0067659e;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f0270u;
    goto L_00677672;
L_0067659e:
    destination = local(frame, 0x818u);
    literal = 0x9f0264u;
    goto L_00677672;
L_006765e7:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00676613;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f024cu;
    goto L_00677672;
L_00676613:
    destination = local(frame, 0x818u);
    literal = 0x9f0240u;
    goto L_00677672;
L_0067665c:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00676688;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f022cu;
    goto L_00677672;
L_00676688:
    destination = local(frame, 0x818u);
    literal = 0x9f0220u;
    goto L_00677672;
L_006766d1:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_006766fd;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f020cu;
    goto L_00677672;
L_006766fd:
    destination = local(frame, 0x818u);
    literal = 0x9f0204u;
    goto L_00677672;
L_00676746:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00676772;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f01f0u;
    goto L_00677672;
L_00676772:
    destination = local(frame, 0x818u);
    literal = 0x9f01e8u;
    goto L_00677672;
L_006767bb:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_006767e7;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f01d4u;
    goto L_00677672;
L_006767e7:
    destination = local(frame, 0x818u);
    literal = 0x9f01c8u;
    goto L_00677672;
L_00676830:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_0067685c;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f01b0u;
    goto L_00677672;
L_0067685c:
    destination = local(frame, 0x818u);
    literal = 0x9f01a4u;
    goto L_00677672;
L_006768a5:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_006768d1;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f0194u;
    goto L_00677672;
L_006768d1:
    destination = local(frame, 0x818u);
    literal = 0x9f018cu;
    goto L_00677672;
L_0067691a:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00676946;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f0174u;
    goto L_00677672;
L_00676946:
    destination = local(frame, 0x818u);
    literal = 0x9f0168u;
    goto L_00677672;
L_0067698f:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_006769bb;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f0154u;
    goto L_00677672;
L_006769bb:
    destination = local(frame, 0x818u);
    literal = 0x9f0148u;
    goto L_00677672;
L_00676a04:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00676a30;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f0130u;
    goto L_00677672;
L_00676a30:
    destination = local(frame, 0x818u);
    literal = 0x9f0440u;
    goto L_00677672;
L_00676a79:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00677503;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f0124u;
    goto L_00677672;
L_00676ae1:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00676b0d;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f0114u;
    goto L_00677672;
L_00676b0d:
    destination = local(frame, 0x818u);
    literal = 0x9f010cu;
    goto L_00677672;
L_00676b56:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00676b82;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f00fcu;
    goto L_00677672;
L_00676b82:
    destination = local(frame, 0x818u);
    literal = 0x9f00f4u;
    goto L_00677672;
L_00676bcb:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00676bf7;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f00e8u;
    goto L_00677672;
L_00676bf7:
    destination = local(frame, 0x818u);
    literal = 0x9f00e0u;
    goto L_00677672;
L_00676c40:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00676c6c;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f00ccu;
    goto L_00677672;
L_00676c6c:
    destination = local(frame, 0x818u);
    literal = 0x9f00c4u;
    goto L_00677672;
L_00676cb5:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00676ce1;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f00b0u;
    goto L_00677672;
L_00676ce1:
    destination = local(frame, 0x818u);
    literal = 0x9f00a8u;
    goto L_00677672;
L_00676d2a:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00676d56;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f0098u;
    goto L_00677672;
L_00676d56:
    destination = local(frame, 0x818u);
    literal = 0x9f0090u;
    goto L_00677672;
L_00676d9f:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00676dcb;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f007cu;
    goto L_00677672;
L_00676dcb:
    destination = local(frame, 0x818u);
    literal = 0x9f0070u;
    goto L_00677672;
L_00676e14:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00676e40;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f005cu;
    goto L_00677672;
L_00676e40:
    destination = local(frame, 0x818u);
    literal = 0x9f0054u;
    goto L_00677672;
L_00676e89:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00676eb5;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f0040u;
    goto L_00677672;
L_00676eb5:
    destination = local(frame, 0x818u);
    literal = 0x9f0038u;
    goto L_00677672;
L_00676efe:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00676f2a;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f0028u;
    goto L_00677672;
L_00676f2a:
    destination = local(frame, 0x818u);
    literal = 0x9f0020u;
    goto L_00677672;
L_00676f73:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00676f9f;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9f000cu;
    goto L_00677672;
L_00676f9f:
    destination = local(frame, 0x818u);
    literal = 0x9f0000u;
    goto L_00677672;
L_00676fe8:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00677014;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9efff0u;
    goto L_00677672;
L_00677014:
    destination = local(frame, 0x818u);
    literal = 0x9effe8u;
    goto L_00677672;
L_0067705d:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00677089;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9effd4u;
    goto L_00677672;
L_00677089:
    destination = local(frame, 0x818u);
    literal = 0x9effccu;
    goto L_00677672;
L_006770d2:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_006770fe;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9effb4u;
    goto L_00677672;
L_006770fe:
    destination = local(frame, 0x818u);
    literal = 0x9effa8u;
    goto L_00677672;
L_00677147:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00677173;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9eff94u;
    goto L_00677672;
L_00677173:
    destination = local(frame, 0x818u);
    literal = 0x9eff88u;
    goto L_00677672;
L_006771bc:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_006771e8;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9eff6cu;
    goto L_00677672;
L_006771e8:
    destination = local(frame, 0x818u);
    literal = 0x9eff60u;
    goto L_00677672;
L_00677231:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_0067725d;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9eff44u;
    goto L_00677672;
L_0067725d:
    destination = local(frame, 0x818u);
    literal = 0x9eff38u;
    goto L_00677672;
L_006772a6:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_006772d2;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9eff24u;
    goto L_00677672;
L_006772d2:
    destination = local(frame, 0x818u);
    literal = 0x9eff1cu;
    goto L_00677672;
L_0067731b:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00677347;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9eff08u;
    goto L_00677672;
L_00677347:
    destination = local(frame, 0x818u);
    literal = 0x9eff00u;
    goto L_00677672;
L_00677390:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_006773bc;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9eff08u;
    goto L_00677672;
L_006773bc:
    destination = local(frame, 0x818u);
    literal = 0x9efef0u;
    goto L_00677672;
L_00677405:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_00677431;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9efed4u;
    goto L_00677672;
L_00677431:
    destination = local(frame, 0x818u);
    literal = 0x9efec8u;
    goto L_00677672;
L_0067747a:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if (value < 0) goto L_00677558;
    if (value <= 3) goto L_006774a6;
    if (value != 4) goto L_00677558;
    destination = local(frame, 0x818u);
    literal = 0x9efeacu;
    goto L_00677672;
L_006774a6:
    destination = local(frame, 0x818u);
    literal = 0x9efea0u;
    goto L_00677672;
L_006774ef:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680b94u + (cm_lang_u32)value * 4u)) {
    case 0x00677503u: goto L_00677503;
    case 0x00677514u: goto L_00677514;
    case 0x00677525u: goto L_00677525;
    case 0x00677536u: goto L_00677536;
    case 0x00677547u: goto L_00677547;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00677503:
    destination = local(frame, 0x818u);
    literal = 0x9efe94u;
    goto L_00677672;
L_00677514:
    destination = local(frame, 0x818u);
    literal = 0x9f038cu;
    goto L_00677672;
L_00677525:
    destination = local(frame, 0x818u);
    literal = 0x9f036cu;
    goto L_00677672;
L_00677536:
    destination = local(frame, 0x818u);
    literal = 0x9f037cu;
    goto L_00677672;
L_00677547:
    destination = local(frame, 0x818u);
    literal = 0x9efe84u;
    goto L_00677672;
L_00677558:
    destination = local(frame, 0x818u);
    literal = 0x9f1308u;
    goto L_00677616;
L_0067759a:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680ba8u + (cm_lang_u32)value * 4u)) {
    case 0x006775aeu: goto L_006775ae;
    case 0x006775bfu: goto L_006775bf;
    case 0x006775d0u: goto L_006775d0;
    case 0x006775e1u: goto L_006775e1;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_006775ae:
    destination = local(frame, 0x818u);
    literal = 0x9f073cu;
    goto L_00677672;
L_006775bf:
    destination = local(frame, 0x818u);
    literal = 0x9efe78u;
    goto L_00677672;
L_006775d0:
    destination = local(frame, 0x818u);
    literal = 0x9efe70u;
    goto L_00677672;
L_006775e1:
    destination = local(frame, 0x818u);
    literal = 0x9f065cu;
    goto L_00677672;
L_006775f2:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 4) goto L_00677558;
    switch (*(const volatile cm_lang_u32 *)(0x680bbcu + (cm_lang_u32)value * 4u)) {
    case 0x0067760au: goto L_0067760a;
    case 0x00677638u: goto L_00677638;
    case 0x00677666u: goto L_00677666;
    case 0x00677694u: goto L_00677694;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067760a:
    destination = local(frame, 0x818u);
    literal = 0xad9c64u;
    goto L_00677616;
L_00677616:
    copy_literal(destination, literal);
    goto L_00675a12;
L_00677638:
    literal = 0x9f0578u;
    destination = local(frame, 0x818u);
    copy_literal(destination, literal);
    goto L_00675a12;
L_00677666:
    destination = local(frame, 0x818u);
    literal = 0x9f07a8u;
    goto L_00677672;
L_00677672:
    copy_literal(destination, literal);
    goto L_00675a12;
L_00677694:
    literal = 0x9f07b0u;
    destination = local(frame, 0x818u);
    copy_literal(destination, literal);
    goto L_00675a12;
L_006776d6:
    value = property2;
    if ((cm_lang_u32)value > 9) goto L_0067a683;
    switch (*(const volatile cm_lang_u32 *)(0x680bd0u + (cm_lang_u32)value * 4u)) {
    case 0x006776e9u: goto L_006776e9;
    case 0x00677707u: goto L_00677707;
    case 0x0067772cu: goto L_0067772c;
    case 0x0067774au: goto L_0067774a;
    case 0x0067776fu: goto L_0067776f;
    case 0x0067778du: goto L_0067778d;
    case 0x006777b2u: goto L_006777b2;
    case 0x006777d0u: goto L_006777d0;
    case 0x006777f5u: goto L_006777f5;
    case 0x00677813u: goto L_00677813;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_006776e9:
    literal = 0x9efe64u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677707:
    literal = 0x9efe54u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_0067772c:
    literal = 0x9efe48u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_0067774a:
    literal = 0x9efe3cu;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_0067776f:
    literal = 0x9efe2cu;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_0067778d:
    literal = 0x9efe18u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_006777b2:
    literal = 0x9efe04u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_006777d0:
    literal = 0x9efdf8u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_006777f5:
    literal = 0x9efdf0u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677813:
    literal = 0x9efde8u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677870:
    value = property2;
    if ((cm_lang_u32)value > 9) goto L_0067a683;
    switch (*(const volatile cm_lang_u32 *)(0x680bf8u + (cm_lang_u32)value * 4u)) {
    case 0x00677883u: goto L_00677883;
    case 0x006778a1u: goto L_006778a1;
    case 0x006778c6u: goto L_006778c6;
    case 0x006778e4u: goto L_006778e4;
    case 0x00677909u: goto L_00677909;
    case 0x00677927u: goto L_00677927;
    case 0x0067794cu: goto L_0067794c;
    case 0x0067796au: goto L_0067796a;
    case 0x0067798fu: goto L_0067798f;
    case 0x006779adu: goto L_006779ad;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00677883:
    literal = 0x9efdd4u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_006778a1:
    literal = 0x9efdbcu;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_006778c6:
    literal = 0x9efda8u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_006778e4:
    literal = 0x9efd94u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677909:
    literal = 0x9efd80u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677927:
    literal = 0x9efd64u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_0067794c:
    literal = 0x9efd4cu;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_0067796a:
    literal = 0x9efd38u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_0067798f:
    literal = 0x9efd2cu;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_006779ad:
    literal = 0x9efd20u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677a0a:
    value = property2;
    if ((cm_lang_u32)value > 9) goto L_0067a683;
    switch (*(const volatile cm_lang_u32 *)(0x680c20u + (cm_lang_u32)value * 4u)) {
    case 0x00677a1du: goto L_00677a1d;
    case 0x00677a3bu: goto L_00677a3b;
    case 0x00677a60u: goto L_00677a60;
    case 0x00677a7eu: goto L_00677a7e;
    case 0x00677aa3u: goto L_00677aa3;
    case 0x00677ac1u: goto L_00677ac1;
    case 0x00677ae6u: goto L_00677ae6;
    case 0x00677b04u: goto L_00677b04;
    case 0x00677b29u: goto L_00677b29;
    case 0x00677b47u: goto L_00677b47;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00677a1d:
    literal = 0x9efd08u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677a3b:
    literal = 0x9efcf0u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677a60:
    literal = 0x9efcdcu;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677a7e:
    literal = 0x9efcc8u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677aa3:
    literal = 0x9efcb4u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677ac1:
    literal = 0x9efc98u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677ae6:
    literal = 0x9efc80u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677b04:
    literal = 0x9efc6cu;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677b29:
    literal = 0x9efc60u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677b47:
    literal = 0x9efc54u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677ba4:
    value = property2;
    if ((cm_lang_u32)value > 9) goto L_0067a683;
    switch (*(const volatile cm_lang_u32 *)(0x680c48u + (cm_lang_u32)value * 4u)) {
    case 0x00677bb7u: goto L_00677bb7;
    case 0x00677bd5u: goto L_00677bd5;
    case 0x00677bfau: goto L_00677bfa;
    case 0x00677c18u: goto L_00677c18;
    case 0x00677c3du: goto L_00677c3d;
    case 0x00677c5bu: goto L_00677c5b;
    case 0x00677c80u: goto L_00677c80;
    case 0x00677c9eu: goto L_00677c9e;
    case 0x00677cc3u: goto L_00677cc3;
    case 0x00677ce1u: goto L_00677ce1;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00677bb7:
    literal = 0x9efc3cu;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677bd5:
    literal = 0x9efc20u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677bfa:
    literal = 0x9efc08u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677c18:
    literal = 0x9efbf0u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677c3d:
    literal = 0x9efbd4u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677c5b:
    literal = 0x9efbb4u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677c80:
    literal = 0x9efb94u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677c9e:
    literal = 0x9efb7cu;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677cc3:
    literal = 0x9efb6cu;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677ce1:
    literal = 0x9efb5cu;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677d3e:
    value = property2;
    if ((cm_lang_u32)value > 9) goto L_0067a683;
    switch (*(const volatile cm_lang_u32 *)(0x680c70u + (cm_lang_u32)value * 4u)) {
    case 0x00677d51u: goto L_00677d51;
    case 0x00677d6fu: goto L_00677d6f;
    case 0x00677d94u: goto L_00677d94;
    case 0x00677db2u: goto L_00677db2;
    case 0x00677dd7u: goto L_00677dd7;
    case 0x00677df5u: goto L_00677df5;
    case 0x00677e1au: goto L_00677e1a;
    case 0x00677e38u: goto L_00677e38;
    case 0x00677e5du: goto L_00677e5d;
    case 0x00677e7bu: goto L_00677e7b;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00677d51:
    literal = 0x9efb44u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677d6f:
    literal = 0x9efb2cu;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677d94:
    literal = 0x9efb14u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677db2:
    literal = 0x9efb00u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677dd7:
    literal = 0x9efae8u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677df5:
    literal = 0x9efaccu;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677e1a:
    literal = 0x9efab0u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677e38:
    literal = 0x9efa9cu;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677e5d:
    literal = 0x9efa90u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677e7b:
    literal = 0x9efa80u;
    destination = OUTPUT;
    copy_literal(destination, literal);
    goto L_0067a683;
L_00677ed4:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_00677ef8;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9efa74u;
    goto L_0067a666;
L_00677ef8:
    destination = local(frame, 0x18u);
    literal = 0x9efa68u;
    goto L_0067a666;
L_00677f3a:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_00677f5e;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9efa58u;
    goto L_0067a666;
L_00677f5e:
    destination = local(frame, 0x18u);
    literal = 0x9efa50u;
    goto L_0067a666;
L_00677fa0:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_00677fc4;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9efa44u;
    goto L_0067a666;
L_00677fc4:
    destination = local(frame, 0x18u);
    literal = 0x9efa3cu;
    goto L_0067a666;
L_00678006:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_0067802a;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9efa30u;
    goto L_0067a666;
L_0067802a:
    destination = local(frame, 0x18u);
    literal = 0x9efa28u;
    goto L_0067a666;
L_0067806c:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_00678c1a;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9efa18u;
    goto L_0067a666;
L_006780c8:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_006780ec;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9efa08u;
    goto L_0067a666;
L_006780ec:
    destination = local(frame, 0x18u);
    literal = 0x9efa00u;
    goto L_0067a666;
L_0067812e:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_00678152;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef9ecu;
    goto L_0067a666;
L_00678152:
    destination = local(frame, 0x18u);
    literal = 0x9ef9e0u;
    goto L_0067a666;
L_00678194:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_006781b8;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef9ccu;
    goto L_0067a666;
L_006781b8:
    destination = local(frame, 0x18u);
    literal = 0x9ef9c4u;
    goto L_0067a666;
L_0067826a:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_0067828e;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef994u;
    goto L_0067a666;
L_0067828e:
    destination = local(frame, 0x18u);
    literal = 0x9ef988u;
    goto L_0067a666;
L_006782d0:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_006782f4;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef974u;
    goto L_0067a666;
L_006782f4:
    destination = local(frame, 0x18u);
    literal = 0x9ef96cu;
    goto L_0067a666;
L_00678336:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_0067835a;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef95cu;
    goto L_0067a666;
L_0067835a:
    destination = local(frame, 0x18u);
    literal = 0x9ef954u;
    goto L_0067a666;
L_0067839c:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_006783c0;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef93cu;
    goto L_0067a666;
L_006783c0:
    destination = local(frame, 0x18u);
    literal = 0x9ef930u;
    goto L_0067a666;
L_00678406:
    if ((cm_lang_u32)property2 > 100u) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x00680c98u + 4u * *(const volatile unsigned char *)(0x00680ca8u + (cm_lang_u32)property2))) {
    case 0x00678421u: goto L_00678421;
    case 0x0067842fu: goto L_0067842f;
    case 0x0067843du: goto L_0067843d;
    case 0x0067a65du: goto L_0067a65d;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00678421:
    destination = local(frame, 0x18u);
    literal = 0x99c368u;
    goto L_0067a666;
L_0067842f:
    destination = local(frame, 0x18u);
    literal = 0xad9c64u;
    goto L_0067a666;
L_0067843d:
    { cm_lang_u32 length = word_length(word);
      if (!length) return 0;
      unsigned char last = *(const volatile unsigned char *)(word + length - 1u);
      PRINT(local(frame, 0x18u), TEXT(last == 0x53u || last == 0x73u ? 0x00ad9c64u : 0x0099c368u));
    }
    goto L_0067a683;
L_006784d8:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680d10u + (cm_lang_u32)value * 4u)) {
    case 0x006784ebu: goto L_006784eb;
    case 0x006784f9u: goto L_006784f9;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_006784eb:
    destination = local(frame, 0x18u);
    literal = 0x9ef910u;
    goto L_0067a666;
L_006784f9:
    destination = local(frame, 0x18u);
    literal = 0x9ef900u;
    goto L_0067a666;
L_0067853b:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680d24u + (cm_lang_u32)value * 4u)) {
    case 0x0067854eu: goto L_0067854e;
    case 0x0067a29cu: goto L_0067a29c;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067854e:
    destination = local(frame, 0x18u);
    literal = 0x9ef8f0u;
    goto L_0067a666;
L_00678590:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680d38u + (cm_lang_u32)value * 4u)) {
    case 0x006785a3u: goto L_006785a3;
    case 0x006785b1u: goto L_006785b1;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_006785a3:
    destination = local(frame, 0x18u);
    literal = 0x9ef8d8u;
    goto L_0067a666;
L_006785b1:
    destination = local(frame, 0x18u);
    literal = 0x9ef8ccu;
    goto L_0067a666;
L_006785f3:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680d4cu + (cm_lang_u32)value * 4u)) {
    case 0x00678606u: goto L_00678606;
    case 0x00678614u: goto L_00678614;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00678606:
    destination = local(frame, 0x18u);
    literal = 0x9ef8bcu;
    goto L_0067a666;
L_00678614:
    destination = local(frame, 0x18u);
    literal = 0x9ef8b0u;
    goto L_0067a666;
L_00678656:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680d60u + (cm_lang_u32)value * 4u)) {
    case 0x00678669u: goto L_00678669;
    case 0x00678677u: goto L_00678677;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00678669:
    destination = local(frame, 0x18u);
    literal = 0x9ef898u;
    goto L_0067a666;
L_00678677:
    destination = local(frame, 0x18u);
    literal = 0x9ef888u;
    goto L_0067a666;
L_006786b9:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680d74u + (cm_lang_u32)value * 4u)) {
    case 0x006786ccu: goto L_006786cc;
    case 0x006786dau: goto L_006786da;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_006786cc:
    destination = local(frame, 0x18u);
    literal = 0x9ef868u;
    goto L_0067a666;
L_006786da:
    destination = local(frame, 0x18u);
    literal = 0x9ef858u;
    goto L_0067a666;
L_0067871c:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680d88u + (cm_lang_u32)value * 4u)) {
    case 0x0067872fu: goto L_0067872f;
    case 0x0067873du: goto L_0067873d;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067872f:
    destination = local(frame, 0x18u);
    literal = 0x9ef840u;
    goto L_0067a666;
L_0067873d:
    destination = local(frame, 0x18u);
    literal = 0x9ef834u;
    goto L_0067a666;
L_0067877f:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680d9cu + (cm_lang_u32)value * 4u)) {
    case 0x00678792u: goto L_00678792;
    case 0x006787a0u: goto L_006787a0;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00678792:
    destination = local(frame, 0x18u);
    literal = 0x9ef81cu;
    goto L_0067a666;
L_006787a0:
    destination = local(frame, 0x18u);
    literal = 0x9ef80cu;
    goto L_0067a666;
L_006787e2:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680db0u + (cm_lang_u32)value * 4u)) {
    case 0x006787f5u: goto L_006787f5;
    case 0x00678803u: goto L_00678803;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_006787f5:
    destination = local(frame, 0x18u);
    literal = 0x9ef7fcu;
    goto L_0067a666;
L_00678803:
    destination = local(frame, 0x18u);
    literal = 0x9ef7f4u;
    goto L_0067a666;
L_00678845:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680dc4u + (cm_lang_u32)value * 4u)) {
    case 0x00678858u: goto L_00678858;
    case 0x00678866u: goto L_00678866;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00678858:
    destination = local(frame, 0x18u);
    literal = 0x9ef7e4u;
    goto L_0067a666;
L_00678866:
    destination = local(frame, 0x18u);
    literal = 0x9ef7dcu;
    goto L_0067a666;
L_006788a8:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680dd8u + (cm_lang_u32)value * 4u)) {
    case 0x006788bbu: goto L_006788bb;
    case 0x006788c9u: goto L_006788c9;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_006788bb:
    destination = local(frame, 0x18u);
    literal = 0x9ef7d0u;
    goto L_0067a666;
L_006788c9:
    destination = local(frame, 0x18u);
    literal = 0x9ef7c8u;
    goto L_0067a666;
L_0067890b:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680decu + (cm_lang_u32)value * 4u)) {
    case 0x0067891eu: goto L_0067891e;
    case 0x0067892cu: goto L_0067892c;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067891e:
    destination = local(frame, 0x18u);
    literal = 0x9ef7b8u;
    goto L_0067a666;
L_0067892c:
    destination = local(frame, 0x18u);
    literal = 0x9ef7b0u;
    goto L_0067a666;
L_0067896e:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680e00u + (cm_lang_u32)value * 4u)) {
    case 0x00678981u: goto L_00678981;
    case 0x0067898fu: goto L_0067898f;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00678981:
    destination = local(frame, 0x18u);
    literal = 0x9ef7a0u;
    goto L_0067a666;
L_0067898f:
    destination = local(frame, 0x18u);
    literal = 0x9ef798u;
    goto L_0067a666;
L_006789d1:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680e14u + (cm_lang_u32)value * 4u)) {
    case 0x006789e4u: goto L_006789e4;
    case 0x0067a174u: goto L_0067a174;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_006789e4:
    destination = local(frame, 0x18u);
    literal = 0x9ef784u;
    goto L_0067a666;
L_00678a26:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680e28u + (cm_lang_u32)value * 4u)) {
    case 0x00678a39u: goto L_00678a39;
    case 0x00678a47u: goto L_00678a47;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00678a39:
    destination = local(frame, 0x18u);
    literal = 0x9ef774u;
    goto L_0067a666;
L_00678a47:
    destination = local(frame, 0x18u);
    literal = 0x9ef76cu;
    goto L_0067a666;
L_00678a89:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680e3cu + (cm_lang_u32)value * 4u)) {
    case 0x00678a9cu: goto L_00678a9c;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00678a9c:
    destination = local(frame, 0x18u);
    literal = 0x9ef768u;
    goto L_0067a666;
L_00678ade:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680e50u + (cm_lang_u32)value * 4u)) {
    case 0x00678af1u: goto L_00678af1;
    case 0x00678affu: goto L_00678aff;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00678af1:
    destination = local(frame, 0x18u);
    literal = 0x9ef758u;
    goto L_0067a666;
L_00678aff:
    destination = local(frame, 0x18u);
    literal = 0x9ef74cu;
    goto L_0067a666;
L_00678b41:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680e64u + (cm_lang_u32)value * 4u)) {
    case 0x00678b54u: goto L_00678b54;
    case 0x00678b62u: goto L_00678b62;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00678b54:
    destination = local(frame, 0x18u);
    literal = 0x9ef734u;
    goto L_0067a666;
L_00678b62:
    destination = local(frame, 0x18u);
    literal = 0x9ef728u;
    goto L_0067a666;
L_00678ba4:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680e78u + (cm_lang_u32)value * 4u)) {
    case 0x00678bb7u: goto L_00678bb7;
    case 0x00678bc5u: goto L_00678bc5;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00678bb7:
    destination = local(frame, 0x18u);
    literal = 0x9ef718u;
    goto L_0067a666;
L_00678bc5:
    destination = local(frame, 0x18u);
    literal = 0x9ef710u;
    goto L_0067a666;
L_00678c07:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680e8cu + (cm_lang_u32)value * 4u)) {
    case 0x00678c1au: goto L_00678c1a;
    case 0x00678c28u: goto L_00678c28;
    case 0x00678c36u: goto L_00678c36;
    case 0x00678c44u: goto L_00678c44;
    case 0x00678c52u: goto L_00678c52;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00678c1a:
    destination = local(frame, 0x18u);
    literal = 0x9ef6fcu;
    goto L_0067a666;
L_00678c28:
    destination = local(frame, 0x18u);
    literal = 0x9ef6f0u;
    goto L_0067a666;
L_00678c36:
    destination = local(frame, 0x18u);
    literal = 0x9ef6e4u;
    goto L_0067a666;
L_00678c44:
    destination = local(frame, 0x18u);
    literal = 0x9ef6d8u;
    goto L_0067a666;
L_00678c52:
    destination = local(frame, 0x18u);
    literal = 0x9ef6ccu;
    goto L_0067a666;
L_00678c94:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680ea0u + (cm_lang_u32)value * 4u)) {
    case 0x00678ca7u: goto L_00678ca7;
    case 0x00678cb5u: goto L_00678cb5;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00678ca7:
    destination = local(frame, 0x18u);
    literal = 0x9ef6bcu;
    goto L_0067a666;
L_00678cb5:
    destination = local(frame, 0x18u);
    literal = 0x9ef6b0u;
    goto L_0067a666;
L_00678cf7:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680eb4u + (cm_lang_u32)value * 4u)) {
    case 0x00678d0au: goto L_00678d0a;
    case 0x00678d18u: goto L_00678d18;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00678d0a:
    destination = local(frame, 0x18u);
    literal = 0x9ef6a0u;
    goto L_0067a666;
L_00678d18:
    destination = local(frame, 0x18u);
    literal = 0x9ef694u;
    goto L_0067a666;
L_00678d5a:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680ec8u + (cm_lang_u32)value * 4u)) {
    case 0x00678d6du: goto L_00678d6d;
    case 0x00679872u: goto L_00679872;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00678d6d:
    destination = local(frame, 0x18u);
    literal = 0x9ef684u;
    goto L_0067a666;
L_00678daf:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680edcu + (cm_lang_u32)value * 4u)) {
    case 0x00678dc2u: goto L_00678dc2;
    case 0x00678dd0u: goto L_00678dd0;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00678dc2:
    destination = local(frame, 0x18u);
    literal = 0x9ef674u;
    goto L_0067a666;
L_00678dd0:
    destination = local(frame, 0x18u);
    literal = 0x9ef66cu;
    goto L_0067a666;
L_00678e12:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680ef0u + (cm_lang_u32)value * 4u)) {
    case 0x00678e25u: goto L_00678e25;
    case 0x00678e33u: goto L_00678e33;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00678e25:
    destination = local(frame, 0x18u);
    literal = 0x9ef65cu;
    goto L_0067a666;
L_00678e33:
    destination = local(frame, 0x18u);
    literal = 0x9ef654u;
    goto L_0067a666;
L_00678e75:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680f04u + (cm_lang_u32)value * 4u)) {
    case 0x00678e88u: goto L_00678e88;
    case 0x00678e96u: goto L_00678e96;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00678e88:
    destination = local(frame, 0x18u);
    literal = 0x9ef63cu;
    goto L_0067a666;
L_00678e96:
    destination = local(frame, 0x18u);
    literal = 0x9ef62cu;
    goto L_0067a666;
L_00678ed8:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680f18u + (cm_lang_u32)value * 4u)) {
    case 0x00678eebu: goto L_00678eeb;
    case 0x00679ebeu: goto L_00679ebe;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00678eeb:
    destination = local(frame, 0x18u);
    literal = 0x9ef61cu;
    goto L_0067a666;
L_00678f2d:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680f2cu + (cm_lang_u32)value * 4u)) {
    case 0x00678f40u: goto L_00678f40;
    case 0x00678f4eu: goto L_00678f4e;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00678f40:
    destination = local(frame, 0x18u);
    literal = 0x9ef60cu;
    goto L_0067a666;
L_00678f4e:
    destination = local(frame, 0x18u);
    literal = 0x9ef604u;
    goto L_0067a666;
L_00678f90:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680f40u + (cm_lang_u32)value * 4u)) {
    case 0x00678fa3u: goto L_00678fa3;
    case 0x00678fb1u: goto L_00678fb1;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00678fa3:
    destination = local(frame, 0x18u);
    literal = 0x9ef5e8u;
    goto L_0067a666;
L_00678fb1:
    destination = local(frame, 0x18u);
    literal = 0x9ef5d8u;
    goto L_0067a666;
L_00678ff3:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680f54u + (cm_lang_u32)value * 4u)) {
    case 0x00679006u: goto L_00679006;
    case 0x00679014u: goto L_00679014;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00679006:
    destination = local(frame, 0x18u);
    literal = 0x9ef5c8u;
    goto L_0067a666;
L_00679014:
    destination = local(frame, 0x18u);
    literal = 0x9ef5c0u;
    goto L_0067a666;
L_00679056:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680f68u + (cm_lang_u32)value * 4u)) {
    case 0x00679069u: goto L_00679069;
    case 0x00679077u: goto L_00679077;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00679069:
    destination = local(frame, 0x18u);
    literal = 0x9ef5a8u;
    goto L_0067a666;
L_00679077:
    destination = local(frame, 0x18u);
    literal = 0x9ef59cu;
    goto L_0067a666;
L_006790b9:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680f7cu + (cm_lang_u32)value * 4u)) {
    case 0x006790ccu: goto L_006790cc;
    case 0x006790dau: goto L_006790da;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_006790cc:
    destination = local(frame, 0x18u);
    literal = 0x9ef57cu;
    goto L_0067a666;
L_006790da:
    destination = local(frame, 0x18u);
    literal = 0x9ef56cu;
    goto L_0067a666;
L_0067911c:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680f90u + (cm_lang_u32)value * 4u)) {
    case 0x0067912fu: goto L_0067912f;
    case 0x006797b0u: goto L_006797b0;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067912f:
    destination = local(frame, 0x18u);
    literal = 0x9ef554u;
    goto L_0067a666;
L_00679171:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680fa4u + (cm_lang_u32)value * 4u)) {
    case 0x00679184u: goto L_00679184;
    case 0x00679192u: goto L_00679192;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00679184:
    destination = local(frame, 0x18u);
    literal = 0x9ef53cu;
    goto L_0067a666;
L_00679192:
    destination = local(frame, 0x18u);
    literal = 0x9ef530u;
    goto L_0067a666;
L_006791d4:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680fb8u + (cm_lang_u32)value * 4u)) {
    case 0x006791e7u: goto L_006791e7;
    case 0x006791f5u: goto L_006791f5;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_006791e7:
    destination = local(frame, 0x18u);
    literal = 0x9ef520u;
    goto L_0067a666;
L_006791f5:
    destination = local(frame, 0x18u);
    literal = 0x9ef518u;
    goto L_0067a666;
L_00679237:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680fccu + (cm_lang_u32)value * 4u)) {
    case 0x0067924au: goto L_0067924a;
    case 0x00679258u: goto L_00679258;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067924a:
    destination = local(frame, 0x18u);
    literal = 0x9ef504u;
    goto L_0067a666;
L_00679258:
    destination = local(frame, 0x18u);
    literal = 0x9ef4f8u;
    goto L_0067a666;
L_0067929a:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680fe0u + (cm_lang_u32)value * 4u)) {
    case 0x006792adu: goto L_006792ad;
    case 0x00679f80u: goto L_00679f80;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_006792ad:
    destination = local(frame, 0x18u);
    literal = 0x9ef4e8u;
    goto L_0067a666;
L_006792ef:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x680ff4u + (cm_lang_u32)value * 4u)) {
    case 0x00679302u: goto L_00679302;
    case 0x00679310u: goto L_00679310;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00679302:
    destination = local(frame, 0x18u);
    literal = 0x9ef4d8u;
    goto L_0067a666;
L_00679310:
    destination = local(frame, 0x18u);
    literal = 0x9ef4d0u;
    goto L_0067a666;
L_00679352:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x681008u + (cm_lang_u32)value * 4u)) {
    case 0x00679365u: goto L_00679365;
    case 0x00679373u: goto L_00679373;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00679365:
    destination = local(frame, 0x18u);
    literal = 0x9ef4b8u;
    goto L_0067a666;
L_00679373:
    destination = local(frame, 0x18u);
    literal = 0x9ef4acu;
    goto L_0067a666;
L_006793b5:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x68101cu + (cm_lang_u32)value * 4u)) {
    case 0x006793c8u: goto L_006793c8;
    case 0x006793d6u: goto L_006793d6;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_006793c8:
    destination = local(frame, 0x18u);
    literal = 0x9ef49cu;
    goto L_0067a666;
L_006793d6:
    destination = local(frame, 0x18u);
    literal = 0x9ef494u;
    goto L_0067a666;
L_00679418:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x681030u + (cm_lang_u32)value * 4u)) {
    case 0x0067942bu: goto L_0067942b;
    case 0x00679439u: goto L_00679439;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067942b:
    destination = local(frame, 0x18u);
    literal = 0x9ef484u;
    goto L_0067a666;
L_00679439:
    destination = local(frame, 0x18u);
    literal = 0x9ef47cu;
    goto L_0067a666;
L_0067947b:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x681044u + (cm_lang_u32)value * 4u)) {
    case 0x0067948eu: goto L_0067948e;
    case 0x0067949cu: goto L_0067949c;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067948e:
    destination = local(frame, 0x18u);
    literal = 0x9ef46cu;
    goto L_0067a666;
L_0067949c:
    destination = local(frame, 0x18u);
    literal = 0x9ef460u;
    goto L_0067a666;
L_006794de:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x681058u + (cm_lang_u32)value * 4u)) {
    case 0x006794f1u: goto L_006794f1;
    case 0x006794ffu: goto L_006794ff;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_006794f1:
    destination = local(frame, 0x18u);
    literal = 0x9ef450u;
    goto L_0067a666;
L_006794ff:
    destination = local(frame, 0x18u);
    literal = 0x9ef444u;
    goto L_0067a666;
L_00679541:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x68106cu + (cm_lang_u32)value * 4u)) {
    case 0x00679554u: goto L_00679554;
    case 0x00679562u: goto L_00679562;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_00679554:
    destination = local(frame, 0x18u);
    literal = 0x9ef434u;
    goto L_0067a666;
L_00679562:
    destination = local(frame, 0x18u);
    literal = 0x9ef42cu;
    goto L_0067a666;
L_006795a4:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x681080u + (cm_lang_u32)value * 4u)) {
    case 0x006795b7u: goto L_006795b7;
    case 0x006795c5u: goto L_006795c5;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_006795b7:
    destination = local(frame, 0x18u);
    literal = 0x9ef41cu;
    goto L_0067a666;
L_006795c5:
    destination = local(frame, 0x18u);
    literal = 0x9ef414u;
    goto L_0067a666;
L_00679607:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x681094u + (cm_lang_u32)value * 4u)) {
    case 0x0067961au: goto L_0067961a;
    case 0x00679628u: goto L_00679628;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067961a:
    destination = local(frame, 0x18u);
    literal = 0x9ef3fcu;
    goto L_0067a666;
L_00679628:
    destination = local(frame, 0x18u);
    literal = 0x9ef3f0u;
    goto L_0067a666;
L_0067966a:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x6810a8u + (cm_lang_u32)value * 4u)) {
    case 0x0067967du: goto L_0067967d;
    case 0x0067968bu: goto L_0067968b;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067967d:
    destination = local(frame, 0x18u);
    literal = 0x9ef3bcu;
    goto L_0067a666;
L_0067968b:
    destination = local(frame, 0x18u);
    literal = 0x9ef3a0u;
    goto L_0067a666;
L_006796cd:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x6810bcu + (cm_lang_u32)value * 4u)) {
    case 0x006796e0u: goto L_006796e0;
    case 0x006796eeu: goto L_006796ee;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_006796e0:
    destination = local(frame, 0x18u);
    literal = 0x9ef378u;
    goto L_0067a666;
L_006796ee:
    destination = local(frame, 0x18u);
    literal = 0x9ef360u;
    goto L_0067a666;
L_00679730:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_00679754;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef358u;
    goto L_0067a666;
L_00679754:
    destination = local(frame, 0x18u);
    literal = 0x9ef350u;
    goto L_0067a666;
L_00679796:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_0067912f;
    if (property2 != 4) goto L_0067a65d;
    goto L_006797b0;
L_006797b0:
    destination = local(frame, 0x18u);
    literal = 0x9ef344u;
    goto L_0067a666;
L_006797f2:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_00679816;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9f0174u;
    goto L_0067a666;
L_00679816:
    destination = local(frame, 0x18u);
    literal = 0x9ef338u;
    goto L_0067a666;
L_00679858:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_00678d6d;
    if (property2 != 4) goto L_0067a65d;
    goto L_00679872;
L_00679872:
    destination = local(frame, 0x18u);
    literal = 0x9ef32cu;
    goto L_0067a666;
L_006798b4:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_006798d8;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef320u;
    goto L_0067a666;
L_006798d8:
    destination = local(frame, 0x18u);
    literal = 0x9ef314u;
    goto L_0067a666;
L_0067991a:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_0067993e;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef308u;
    goto L_0067a666;
L_0067993e:
    destination = local(frame, 0x18u);
    literal = 0x9ef2fcu;
    goto L_0067a666;
L_00679980:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_006799a4;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef2f0u;
    goto L_0067a666;
L_006799a4:
    destination = local(frame, 0x18u);
    literal = 0x9ef2e4u;
    goto L_0067a666;
L_006799e6:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_00679a0a;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef2dcu;
    goto L_0067a666;
L_00679a0a:
    destination = local(frame, 0x18u);
    literal = 0x9ef2d4u;
    goto L_0067a666;
L_00679a4c:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_00679a70;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef2ccu;
    goto L_0067a666;
L_00679a70:
    destination = local(frame, 0x18u);
    literal = 0x9ef2c4u;
    goto L_0067a666;
L_00679ab2:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_00679ad6;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef2bcu;
    goto L_0067a666;
L_00679ad6:
    destination = local(frame, 0x18u);
    literal = 0x9ef2b4u;
    goto L_0067a666;
L_00679b18:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_00679d96;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef2acu;
    goto L_0067a666;
L_00679b74:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_00679b98;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef2a0u;
    goto L_0067a666;
L_00679b98:
    destination = local(frame, 0x18u);
    literal = 0x9ef298u;
    goto L_0067a666;
L_00679bda:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_00679bfe;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef28cu;
    goto L_0067a666;
L_00679bfe:
    destination = local(frame, 0x18u);
    literal = 0x9ef280u;
    goto L_0067a666;
L_00679c40:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_00679c64;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef274u;
    goto L_0067a666;
L_00679c64:
    destination = local(frame, 0x18u);
    literal = 0x9ef26cu;
    goto L_0067a666;
L_00679ca6:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_00679cca;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef260u;
    goto L_0067a666;
L_00679cca:
    destination = local(frame, 0x18u);
    literal = 0x9ef258u;
    goto L_0067a666;
L_00679d0c:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_00679d30;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef250u;
    goto L_0067a666;
L_00679d30:
    destination = local(frame, 0x18u);
    literal = 0x9ef248u;
    goto L_0067a666;
L_00679d72:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_00679d96;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef2acu;
    goto L_0067a666;
L_00679d96:
    destination = local(frame, 0x18u);
    literal = 0x9ef240u;
    goto L_0067a666;
L_00679dd8:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_00679dfc;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef22cu;
    goto L_0067a666;
L_00679dfc:
    destination = local(frame, 0x18u);
    literal = 0x9ef224u;
    goto L_0067a666;
L_00679e3e:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_00679e62;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef218u;
    goto L_0067a666;
L_00679e62:
    destination = local(frame, 0x18u);
    literal = 0x9ef20cu;
    goto L_0067a666;
L_00679ea4:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_00678eeb;
    if (property2 != 4) goto L_0067a65d;
    goto L_00679ebe;
L_00679ebe:
    destination = local(frame, 0x18u);
    literal = 0x9ef204u;
    goto L_0067a666;
L_00679f00:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_00679f24;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef1f8u;
    goto L_0067a666;
L_00679f24:
    destination = local(frame, 0x18u);
    literal = 0x9ef1f0u;
    goto L_0067a666;
L_00679f66:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_006792ad;
    if (property2 != 4) goto L_0067a65d;
    goto L_00679f80;
L_00679f80:
    destination = local(frame, 0x18u);
    literal = 0x9ef1e4u;
    goto L_0067a666;
L_00679fc2:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_00679fe6;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef1d8u;
    goto L_0067a666;
L_00679fe6:
    destination = local(frame, 0x18u);
    literal = 0x9ef1ccu;
    goto L_0067a666;
L_0067a028:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_0067a04c;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef1c0u;
    goto L_0067a666;
L_0067a04c:
    destination = local(frame, 0x18u);
    literal = 0x9ef1b8u;
    goto L_0067a666;
L_0067a08e:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_0067a0b2;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef1b0u;
    goto L_0067a666;
L_0067a0b2:
    destination = local(frame, 0x18u);
    literal = 0x9ef1a8u;
    goto L_0067a666;
L_0067a0f4:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_0067a118;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef194u;
    goto L_0067a666;
L_0067a118:
    destination = local(frame, 0x18u);
    literal = 0x9ef18cu;
    goto L_0067a666;
L_0067a15a:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_006789e4;
    if (property2 != 4) goto L_0067a65d;
    goto L_0067a174;
L_0067a174:
    destination = local(frame, 0x18u);
    literal = 0x9ef180u;
    goto L_0067a666;
L_0067a1b6:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_0067a1da;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef168u;
    goto L_0067a666;
L_0067a1da:
    destination = local(frame, 0x18u);
    literal = 0x9ef15cu;
    goto L_0067a666;
L_0067a21c:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_0067a240;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef150u;
    goto L_0067a666;
L_0067a240:
    destination = local(frame, 0x18u);
    literal = 0x9ef144u;
    goto L_0067a666;
L_0067a282:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_0067854e;
    if (property2 != 4) goto L_0067a65d;
    goto L_0067a29c;
L_0067a29c:
    destination = local(frame, 0x18u);
    literal = 0x9ef13cu;
    goto L_0067a666;
L_0067a2de:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_0067a302;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef130u;
    goto L_0067a666;
L_0067a302:
    destination = local(frame, 0x18u);
    literal = 0x9ef128u;
    goto L_0067a666;
L_0067a344:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_0067a368;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef0f0u;
    goto L_0067a666;
L_0067a368:
    destination = local(frame, 0x18u);
    literal = 0x9ef0d4u;
    goto L_0067a666;
L_0067a3aa:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_0067a3ce;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef080u;
    goto L_0067a666;
L_0067a3ce:
    destination = local(frame, 0x18u);
    literal = 0x9ef058u;
    goto L_0067a666;
L_0067a410:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_0067a434;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef048u;
    goto L_0067a666;
L_0067a434:
    destination = local(frame, 0x18u);
    literal = 0x9ef03cu;
    goto L_0067a666;
L_0067a476:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_0067a48c;
    if (property2 != 4) goto L_0067a65d;
    goto L_0067a48c;
L_0067a48c:
    destination = local(frame, 0x18u);
    literal = 0x9ef030u;
    goto L_0067a666;
L_0067a4ce:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067a65d;
    switch (*(const volatile cm_lang_u32 *)(0x6810d0u + (cm_lang_u32)value * 4u)) {
    case 0x0067a4e1u: goto L_0067a4e1;
    case 0x0067a4efu: goto L_0067a4ef;
    case 0x0067a4fdu: goto L_0067a4fd;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067a4e1:
    destination = local(frame, 0x18u);
    literal = 0x9ef018u;
    goto L_0067a666;
L_0067a4ef:
    destination = local(frame, 0x18u);
    literal = 0x9ef024u;
    goto L_0067a666;
L_0067a4fd:
    destination = local(frame, 0x18u);
    literal = 0x9ef00cu;
    goto L_0067a666;
L_0067a53f:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_0067a563;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9ef004u;
    goto L_0067a666;
L_0067a563:
    destination = local(frame, 0x18u);
    literal = 0x9eeffcu;
    goto L_0067a666;
L_0067a5a5:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_0067a5c9;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9eeff0u;
    goto L_0067a666;
L_0067a5c9:
    destination = local(frame, 0x18u);
    literal = 0x9eefe4u;
    goto L_0067a666;
L_0067a60b:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_0067a624;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9eefd8u;
    goto L_0067a666;
L_0067a624:
    destination = local(frame, 0x18u);
    literal = 0x9eefccu;
    goto L_0067a666;
L_0067a639:
    
    if (property2 < 0) goto L_0067a65d;
    if (property2 <= 3) goto L_0067a652;
    if (property2 != 4) goto L_0067a65d;
    destination = local(frame, 0x18u);
    literal = 0x9eefc0u;
    goto L_0067a666;
L_0067a652:
    destination = local(frame, 0x18u);
    literal = 0x9eefb4u;
    goto L_0067a666;
L_0067a65d:
    destination = local(frame, 0x18u);
    literal = 0x9f1308u;
    goto L_0067a666;
L_0067a666:
    copy_literal(destination, literal);
    goto L_0067a683;
L_0067a683:
    /* b'sollte' */
    if (equal(qualifier, TEXT(0x009f029cu))) goto L_0067a6b7;
    /* b'erk\xe4mpft' */
    if (equal(qualifier, TEXT(0x009f027cu))) goto L_0067a71d;
    /* b'erk\xe4mpfte' */
    if (equal(qualifier, TEXT(0x009f0258u))) goto L_0067a783;
    /* b'gleicht' */
    if (equal(qualifier, TEXT(0x009f0238u))) goto L_0067a7e9;
    /* b'glich' */
    if (equal(qualifier, TEXT(0x009f0218u))) goto L_0067a84f;
    /* b'f\xfchrte' */
    if (equal(qualifier, TEXT(0x009f01fcu))) goto L_0067a8b5;
    /* b'erzielt' */
    if (equal(qualifier, TEXT(0x009f01e0u))) goto L_0067a91b;
    /* b'erzielte' */
    if (equal(qualifier, TEXT(0x009f01bcu))) goto L_0067a981;
    /* b'nahm' */
    if (equal(qualifier, TEXT(0x009f019cu))) goto L_0067a9e7;
    /* b'wechselte' */
    if (equal(qualifier, TEXT(0x009f0180u))) goto L_0067aa4d;
    /* b'versucht' */
    if (equal(qualifier, TEXT(0x009f013cu))) goto L_0067aab3;
    /* b'brachte' */
    if (equal(qualifier, TEXT(0x009f0160u))) goto L_0067ab19;
    /* b'versuchte' */
    if (equal(qualifier, TEXT(0x009f039cu))) goto L_0067ab7f;
    /* b'baut' */
    if (equal(qualifier, TEXT(0x009f011cu))) goto L_0067abe5;
    /* b'baute' */
    if (equal(qualifier, TEXT(0x009f0104u))) goto L_0067ac4b;
    /* b'war' */
    if (equal(qualifier, TEXT(0x009f00f0u))) goto L_0067acb1;
    /* b'gewann' */
    if (equal(qualifier, TEXT(0x009f00d8u))) goto L_0067ad17;
    /* b'trifft' */
    if (equal(qualifier, TEXT(0x009f00bcu))) goto L_0067ad7d;
    /* b'traf' */
    if (equal(qualifier, TEXT(0x009f00a0u))) goto L_0067ade3;
    /* b'spielte' */
    if (equal(qualifier, TEXT(0x009f0088u))) goto L_0067ae49;
    /* b'greift' */
    if (equal(qualifier, TEXT(0x009f0068u))) goto L_0067aeaf;
    /* b'griff' */
    if (equal(qualifier, TEXT(0x009f004cu))) goto L_0067af15;
    /* b'\xe4ndert' */
    if (equal(qualifier, TEXT(0x009f0030u))) goto L_0067af7b;
    /* b'\xe4nderte' */
    if (equal(qualifier, TEXT(0x009f0018u))) goto L_0067afe1;
    /* b'zeigt' */
    if (equal(qualifier, TEXT(0x009efff8u))) goto L_0067b047;
    /* b'zeigte' */
    if (equal(qualifier, TEXT(0x009effe0u))) goto L_0067b0ad;
    /* b'verpasste' */
    if (equal(qualifier, TEXT(0x009effc0u))) goto L_0067b113;
    /* b'beginnt' */
    if (equal(qualifier, TEXT(0x009effa0u))) goto L_0067b179;
    /* b'unternimmt' */
    if (equal(qualifier, TEXT(0x009eff7cu))) goto L_0067b1df;
    /* b'unternahm' */
    if (equal(qualifier, TEXT(0x009eff54u))) goto L_0067b245;
    /* b'stand' */
    if (equal(qualifier, TEXT(0x009eff30u))) goto L_0067b2ab;
    /* b'bleibt' */
    if (equal(qualifier, TEXT(0x009eff14u))) goto L_0067b311;
    /* b'blieb' */
    if (equal(qualifier, TEXT(0x009efef8u))) goto L_0067b377;
    /* b'widersteht' */
    if (equal(qualifier, TEXT(0x009efee4u))) goto L_0067b3c6;
    /* b'widerstand' */
    if (equal(qualifier, TEXT(0x009efebcu))) goto L_0067b42c;
    /* b'versuchte der' */
    if (equal(qualifier, TEXT(0x009f03a8u))) goto L_0067b492;
    if (*(const volatile unsigned char *)qualifier && !*(const volatile cm_lang_u32 *)(frame + 0x14u))
        copy_literal((char *)qualifier, 0x00ad9c64u);
    return compose(frame);
L_0067a6b7:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067a6db;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9eefa8u;
    goto L_0067fd17;
L_0067a6db:
    destination = local(frame, 0x18u);
    literal = 0x9eefa0u;
    goto L_0067fcf4;
L_0067a71d:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067a741;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9eef94u;
    goto L_0067fd17;
L_0067a741:
    destination = local(frame, 0x18u);
    literal = 0x9eef88u;
    goto L_0067fcf4;
L_0067a783:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067a7a7;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9eef7cu;
    goto L_0067fd17;
L_0067a7a7:
    destination = local(frame, 0x18u);
    literal = 0x9eef70u;
    goto L_0067fcf4;
L_0067a7e9:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067a80d;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9ef28cu;
    goto L_0067fd17;
L_0067a80d:
    destination = local(frame, 0x18u);
    literal = 0x9ef280u;
    goto L_0067fcf4;
L_0067a84f:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067a873;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9ef274u;
    goto L_0067fd17;
L_0067a873:
    destination = local(frame, 0x18u);
    literal = 0x9ef26cu;
    goto L_0067fcf4;
L_0067a8b5:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067a8d9;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9ef1f8u;
    goto L_0067fd17;
L_0067a8d9:
    destination = local(frame, 0x18u);
    literal = 0x9eef68u;
    goto L_0067fcf4;
L_0067a91b:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067a93f;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9eef5cu;
    goto L_0067fd17;
L_0067a93f:
    destination = local(frame, 0x18u);
    literal = 0x9eef50u;
    goto L_0067fcf4;
L_0067a981:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067a9a5;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9eef44u;
    goto L_0067fd17;
L_0067a9a5:
    destination = local(frame, 0x18u);
    literal = 0x9eef38u;
    goto L_0067fcf4;
L_0067a9e7:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067aa0b;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9ef358u;
    goto L_0067fd17;
L_0067aa0b:
    destination = local(frame, 0x18u);
    literal = 0x9ef350u;
    goto L_0067fcf4;
L_0067aa4d:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067aa71;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9eef2cu;
    goto L_0067fd17;
L_0067aa71:
    destination = local(frame, 0x18u);
    literal = 0x9ef338u;
    goto L_0067fcf4;
L_0067aab3:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067aad7;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9ef308u;
    goto L_0067fd17;
L_0067aad7:
    destination = local(frame, 0x18u);
    literal = 0x9ef2fcu;
    goto L_0067fcf4;
L_0067ab19:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067ab3d;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9ef320u;
    goto L_0067fd17;
L_0067ab3d:
    destination = local(frame, 0x18u);
    literal = 0x9ef314u;
    goto L_0067fcf4;
L_0067ab7f:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067aba3;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9ef2f0u;
    goto L_0067fd17;
L_0067aba3:
    destination = local(frame, 0x18u);
    literal = 0x9ef2e4u;
    goto L_0067fcf4;
L_0067abe5:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067ac09;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9ef2dcu;
    goto L_0067fd17;
L_0067ac09:
    destination = local(frame, 0x18u);
    literal = 0x9ef2d4u;
    goto L_0067fcf4;
L_0067ac4b:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067ac6f;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9ef2ccu;
    goto L_0067fd17;
L_0067ac6f:
    destination = local(frame, 0x18u);
    literal = 0x9ef2c4u;
    goto L_0067fcf4;
L_0067acb1:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067acd5;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9ef2bcu;
    goto L_0067fd17;
L_0067acd5:
    destination = local(frame, 0x18u);
    literal = 0x9ef2b4u;
    goto L_0067fcf4;
L_0067ad17:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067ad3b;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9ef2a0u;
    goto L_0067fd17;
L_0067ad3b:
    destination = local(frame, 0x18u);
    literal = 0x9ef298u;
    goto L_0067fcf4;
L_0067ad7d:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067ada1;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9eef20u;
    goto L_0067fd17;
L_0067ada1:
    destination = local(frame, 0x18u);
    literal = 0x9ef258u;
    goto L_0067fcf4;
L_0067ade3:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067ae07;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9ef250u;
    goto L_0067fd17;
L_0067ae07:
    destination = local(frame, 0x18u);
    literal = 0x9ef248u;
    goto L_0067fcf4;
L_0067ae49:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067ae6d;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9ef1d8u;
    goto L_0067fd17;
L_0067ae6d:
    destination = local(frame, 0x18u);
    literal = 0x9ef1ccu;
    goto L_0067fcf4;
L_0067aeaf:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067aed3;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9eef14u;
    goto L_0067fd17;
L_0067aed3:
    destination = local(frame, 0x18u);
    literal = 0x9eef0cu;
    goto L_0067fcf4;
L_0067af15:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067af39;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9eef00u;
    goto L_0067fd17;
L_0067af39:
    destination = local(frame, 0x18u);
    literal = 0x9eeef8u;
    goto L_0067fcf4;
L_0067af7b:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067af9f;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9eeef0u;
    goto L_0067fd17;
L_0067af9f:
    destination = local(frame, 0x18u);
    literal = 0x9eeee8u;
    goto L_0067fcf4;
L_0067afe1:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067b005;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9eeedcu;
    goto L_0067fd17;
L_0067b005:
    destination = local(frame, 0x18u);
    literal = 0x9eeed0u;
    goto L_0067fcf4;
L_0067b047:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067b06b;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9ef1b0u;
    goto L_0067fd17;
L_0067b06b:
    destination = local(frame, 0x18u);
    literal = 0x9ef1a8u;
    goto L_0067fcf4;
L_0067b0ad:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067b0d1;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9ef1c0u;
    goto L_0067fd17;
L_0067b0d1:
    destination = local(frame, 0x18u);
    literal = 0x9ef1b8u;
    goto L_0067fcf4;
L_0067b113:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067b137;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9eeec4u;
    goto L_0067fd17;
L_0067b137:
    destination = local(frame, 0x18u);
    literal = 0x9eeeb8u;
    goto L_0067fcf4;
L_0067b179:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067b19d;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9ef150u;
    goto L_0067fd17;
L_0067b19d:
    destination = local(frame, 0x18u);
    literal = 0x9ef144u;
    goto L_0067fcf4;
L_0067b1df:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067b203;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9ef048u;
    goto L_0067fd17;
L_0067b203:
    destination = local(frame, 0x18u);
    literal = 0x9ef03cu;
    goto L_0067fcf4;
L_0067b245:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067b269;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9eeea8u;
    goto L_0067fd17;
L_0067b269:
    destination = local(frame, 0x18u);
    literal = 0x9ef030u;
    goto L_0067fcf4;
L_0067b2ab:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067b2cf;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9ef130u;
    goto L_0067fd17;
L_0067b2cf:
    destination = local(frame, 0x18u);
    literal = 0x9ef128u;
    goto L_0067fcf4;
L_0067b311:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067b335;
    goto L_0067b31e;
L_0067b31e:
    
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9eee9cu;
    goto L_0067fd17;
L_0067b335:
    destination = local(frame, 0x18u);
    literal = 0x9eee94u;
    goto L_0067fcf4;
L_0067b377:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 > 3) goto L_0067b31e;
    destination = local(frame, 0x18u);
    literal = 0x9eee8cu;
    goto L_0067fcf4;
L_0067b3c6:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067b3ea;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9eee7cu;
    goto L_0067fd17;
L_0067b3ea:
    destination = local(frame, 0x18u);
    literal = 0x9eee70u;
    goto L_0067fcf4;
L_0067b42c:
    
    if (property2 < 0) goto L_0067fd0e;
    if (property2 <= 3) goto L_0067b450;
    if (property2 != 4) goto L_0067fd0e;
    destination = local(frame, 0x18u);
    literal = 0x9eee60u;
    goto L_0067fd17;
L_0067b450:
    destination = local(frame, 0x18u);
    literal = 0x9eee54u;
    goto L_0067fcf4;
L_0067b492:
    value = property2;
    if ((cm_lang_u32)value > 4) goto L_0067fd0e;
    switch (*(const volatile cm_lang_u32 *)(0x6810e4u + (cm_lang_u32)value * 4u)) {
    case 0x0067b4a5u: goto L_0067b4a5;
    case 0x0067b4b3u: goto L_0067b4b3;
    case 0x0067b4c1u: goto L_0067b4c1;
    case 0x0067b4cfu: goto L_0067b4cf;
    case 0x0067b4ddu: goto L_0067b4dd;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067b4a5:
    destination = local(frame, 0x18u);
    literal = 0x9ef2e4u;
    goto L_0067fd17;
L_0067b4b3:
    destination = local(frame, 0x18u);
    literal = 0x9eee44u;
    goto L_0067fd17;
L_0067b4c1:
    destination = local(frame, 0x18u);
    literal = 0x9eee34u;
    goto L_0067fd17;
L_0067b4cf:
    destination = local(frame, 0x18u);
    literal = 0x9eee24u;
    goto L_0067fd17;
L_0067b4dd:
    destination = local(frame, 0x18u);
    literal = 0x9eee14u;
    goto L_0067fcf4;
L_0067fcf4:
    copy_literal(destination, literal);
    return compose(frame);
L_0067fd0e:
    copy_literal(local(frame, 0x18u), 0x009f1308u);
    return compose(frame);
L_0067fd17:
    copy_literal(destination, literal);
    return compose(frame);
}
