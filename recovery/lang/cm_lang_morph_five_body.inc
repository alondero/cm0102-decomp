/* Native C recovered from primary language-five control flow.
 * generate-morph-five.py pins original instructions and table bytes.
 * Labels are preferred-image VAs; no original instruction executes. */
char *CM_LANG_CDECL cm_lang_morph_language_five(unsigned char *frame) {
    char *word = local(frame, 0x1018u), *destination = word;
    const char *qualifier;
    cm_lang_u32 literal = 0;
    int value = 0, property2 = 0;
    goto L_0067c2ee;
L_0067c2ee:
    qualifier = argument(frame, 0x2820u);
    /* b'upper' */
    if (equal(qualifier, TEXT(0x009f15ecu))) goto L_0067c32d;
    /* b'lower' */
    if (equal(qualifier, TEXT(0x009f15c0u))) goto L_0067c382;
    /* b'El' */
    if (equal(qualifier, TEXT(0x009eea34u))) goto L_0067c884;
    /* b'{El' */
    if (equal(qualifier, TEXT(0x009eea30u))) goto L_0067c884;
    /* b'el' */
    if (equal(qualifier, TEXT(0x009eea2cu))) goto L_0067c82e;
    /* b'la' */
    if (equal(qualifier, TEXT(0x009f14bcu))) goto L_0067c82e;
    /* b'del' */
    if (equal(qualifier, TEXT(0x009eea28u))) goto L_0067c7b9;
    /* b'de la' */
    if (equal(qualifier, TEXT(0x009f145cu))) goto L_0067c7b9;
    /* b'DEL' */
    if (equal(qualifier, TEXT(0x009eea24u))) goto L_0067c553;
    /* b'al' */
    if (equal(qualifier, TEXT(0x009eea00u))) goto L_0067c5f8;
    /* b'un' */
    if (equal(qualifier, TEXT(0x009ee9e4u))) goto L_0067c69d;
    /* b'Un' */
    if (equal(qualifier, TEXT(0x009ee9d0u))) goto L_0067c731;
    if (*(const volatile unsigned char *)qualifier) copy_literal((char *)qualifier, 0x00ad9c64u);
    goto L_0067c907;
L_0067c32d:
    *(volatile char *)word = (char)UPPER(signed_byte(*(const volatile unsigned char *)word));
    goto L_0067c907;
L_0067c382:
    lower_word_in_place(word);
    goto L_0067c907;
L_0067c553:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 5) goto L_0067c8de;
    switch (*(const volatile cm_lang_u32 *)(0x6812f8u + (cm_lang_u32)value * 4u)) {
    case 0x0067c56bu: goto L_0067c56b;
    case 0x0067c57cu: goto L_0067c57c;
    case 0x0067c58du: goto L_0067c58d;
    case 0x0067c59eu: goto L_0067c59e;
    case 0x0067c5afu: goto L_0067c5af;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067c56b:
    destination = local(frame, 0x818u);
    literal = 0x9eed9cu;
    goto L_0067c8ea;
L_0067c57c:
    destination = local(frame, 0x818u);
    literal = 0x9eea1cu;
    goto L_0067c8ea;
L_0067c58d:
    destination = local(frame, 0x818u);
    literal = 0x9eea14u;
    goto L_0067c8ea;
L_0067c59e:
    destination = local(frame, 0x818u);
    literal = 0x9eea0cu;
    goto L_0067c8ea;
L_0067c5af:
    destination = local(frame, 0x818u);
    literal = 0x9eea04u;
    goto L_0067c8ea;
L_0067c5f8:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 5) goto L_0067c8de;
    switch (*(const volatile cm_lang_u32 *)(0x681310u + (cm_lang_u32)value * 4u)) {
    case 0x0067c610u: goto L_0067c610;
    case 0x0067c621u: goto L_0067c621;
    case 0x0067c632u: goto L_0067c632;
    case 0x0067c643u: goto L_0067c643;
    case 0x0067c654u: goto L_0067c654;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067c610:
    destination = local(frame, 0x818u);
    literal = 0x9f15d4u;
    goto L_0067c8ea;
L_0067c621:
    destination = local(frame, 0x818u);
    literal = 0x9f0cecu;
    goto L_0067c8ea;
L_0067c632:
    destination = local(frame, 0x818u);
    literal = 0x9ee9f8u;
    goto L_0067c8ea;
L_0067c643:
    destination = local(frame, 0x818u);
    literal = 0x9ee9f0u;
    goto L_0067c8ea;
L_0067c654:
    destination = local(frame, 0x818u);
    literal = 0x9ee9e8u;
    goto L_0067c8ea;
L_0067c69d:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 5) goto L_0067c8de;
    switch (*(const volatile cm_lang_u32 *)(0x681328u + (cm_lang_u32)value * 4u)) {
    case 0x0067c6b5u: goto L_0067c6b5;
    case 0x0067c6c6u: goto L_0067c6c6;
    case 0x0067c6d7u: goto L_0067c6d7;
    case 0x0067c6e8u: goto L_0067c6e8;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067c6b5:
    destination = local(frame, 0x818u);
    literal = 0x9f0d20u;
    goto L_0067c8ea;
L_0067c6c6:
    destination = local(frame, 0x818u);
    literal = 0x9f0d10u;
    goto L_0067c8ea;
L_0067c6d7:
    destination = local(frame, 0x818u);
    literal = 0x9ee9dcu;
    goto L_0067c8ea;
L_0067c6e8:
    destination = local(frame, 0x818u);
    literal = 0x9ee9d4u;
    goto L_0067c8ea;
L_0067c731:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 5) goto L_0067c8de;
    switch (*(const volatile cm_lang_u32 *)(0x681340u + (cm_lang_u32)value * 4u)) {
    case 0x0067c749u: goto L_0067c749;
    case 0x0067c75au: goto L_0067c75a;
    case 0x0067c76bu: goto L_0067c76b;
    case 0x0067c77cu: goto L_0067c77c;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067c749:
    destination = local(frame, 0x818u);
    literal = 0x9f0d04u;
    goto L_0067c8ea;
L_0067c75a:
    destination = local(frame, 0x818u);
    literal = 0x9f0cf4u;
    goto L_0067c8ea;
L_0067c76b:
    destination = local(frame, 0x818u);
    literal = 0x9ee9c8u;
    goto L_0067c8ea;
L_0067c77c:
    destination = local(frame, 0x818u);
    literal = 0x9ee9c0u;
    goto L_0067c8ea;
L_0067c7b9:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 100u) goto L_0067c8de;
    switch (*(const volatile cm_lang_u32 *)(0x00681358u + 4u * *(const volatile unsigned char *)(0x00681374u + (cm_lang_u32)value))) {
    case 0x0067c7d9u: goto L_0067c7d9;
    case 0x0067c7eau: goto L_0067c7ea;
    case 0x0067c7fbu: goto L_0067c7fb;
    case 0x0067c80cu: goto L_0067c80c;
    case 0x0067c81du: goto L_0067c81d;
    case 0x0067c898u: goto L_0067c898;
    case 0x0067c8deu: goto L_0067c8de;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067c7d9:
    destination = local(frame, 0x818u);
    literal = 0x9f14c0u;
    goto L_0067c8ea;
L_0067c7ea:
    destination = local(frame, 0x818u);
    literal = 0x9f0cb4u;
    goto L_0067c8ea;
L_0067c7fb:
    destination = local(frame, 0x818u);
    literal = 0x9f1454u;
    goto L_0067c8ea;
L_0067c80c:
    destination = local(frame, 0x818u);
    literal = 0x9ee9b8u;
    goto L_0067c8ea;
L_0067c81d:
    destination = local(frame, 0x818u);
    literal = 0x9ee9b0u;
    goto L_0067c8ea;
L_0067c82e:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 5) goto L_0067c8de;
    switch (*(const volatile cm_lang_u32 *)(0x6813dcu + (cm_lang_u32)value * 4u)) {
    case 0x0067c846u: goto L_0067c846;
    case 0x0067c857u: goto L_0067c857;
    case 0x0067c868u: goto L_0067c868;
    case 0x0067c876u: goto L_0067c876;
    case 0x0067c898u: goto L_0067c898;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067c846:
    destination = local(frame, 0x818u);
    literal = 0x9ee9acu;
    goto L_0067c8ea;
L_0067c857:
    destination = local(frame, 0x818u);
    literal = 0x9f130cu;
    goto L_0067c8ea;
L_0067c868:
    destination = local(frame, 0x818u);
    literal = 0x9ee9a4u;
    goto L_0067c8ea;
L_0067c876:
    destination = local(frame, 0x818u);
    literal = 0x9ee99cu;
    goto L_0067c8ea;
L_0067c884:
    value = signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
    if ((cm_lang_u32)value > 5) goto L_0067c8de;
    switch (*(const volatile cm_lang_u32 *)(0x6813f4u + (cm_lang_u32)value * 4u)) {
    case 0x0067c898u: goto L_0067c898;
    case 0x0067c8a6u: goto L_0067c8a6;
    case 0x0067c8b4u: goto L_0067c8b4;
    case 0x0067c8c2u: goto L_0067c8c2;
    case 0x0067c8d0u: goto L_0067c8d0;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067c898:
    destination = local(frame, 0x818u);
    literal = 0xad9c64u;
    goto L_0067c8ea;
L_0067c8a6:
    destination = local(frame, 0x818u);
    literal = 0x9ee998u;
    goto L_0067c8ea;
L_0067c8b4:
    destination = local(frame, 0x818u);
    literal = 0x9f14b0u;
    goto L_0067c8ea;
L_0067c8c2:
    destination = local(frame, 0x818u);
    literal = 0x9ee990u;
    goto L_0067c8ea;
L_0067c8d0:
    destination = local(frame, 0x818u);
    literal = 0x9ee988u;
    goto L_0067c8ea;
L_0067c8de:
    destination = local(frame, 0x818u);
    literal = 0x9f1308u;
    goto L_0067c8ea;
L_0067c8ea:
    copy_literal(destination, literal);
    goto L_0067c907;
L_0067c907:
    qualifier = argument(frame, 0x2824u);
    { int suffix_s = equal(qualifier, TEXT(0x0099c368u));
      property2 = signed_byte(*(const volatile unsigned char *)(frame + 0x282cu));
      if (suffix_s) goto L_0067c94d;
    }
    /* b'se ha clasificado' */
    if (equal(qualifier, TEXT(0x009ee974u))) goto L_0067c98f;
    /* b'se clasifica' */
    if (equal(qualifier, TEXT(0x009ee93cu))) goto L_0067c9f2;
    /* b'deja' */
    if (equal(qualifier, TEXT(0x009ee914u))) goto L_0067ca55;
    /* b'dejaba' */
    if (equal(qualifier, TEXT(0x009ee8fcu))) goto L_0067cab8;
    /* b'entra' */
    if (equal(qualifier, TEXT(0x009ee8e0u))) goto L_0067cb1b;
    /* b'acaba' */
    if (equal(qualifier, TEXT(0x009ee8c8u))) goto L_0067cb7e;
    /* b'es' */
    if (equal(qualifier, TEXT(0x009efd1cu))) goto L_0067cbc5;
    /* b'no se ha' */
    if (equal(qualifier, TEXT(0x009ee8b0u))) goto L_0067cc28;
    /* b'no se le' */
    if (equal(qualifier, TEXT(0x009ee88cu))) goto L_0067cc8b;
    /* b'no logra' */
    if (equal(qualifier, TEXT(0x009ee868u))) goto L_0067ccee;
    /* b'desmiente' */
    if (equal(qualifier, TEXT(0x009ee844u))) goto L_0067cd51;
    /* b'recibir\xe1' */
    if (equal(qualifier, TEXT(0x009ee820u))) goto L_0067cdb4;
    /* b'perdi\xf3' */
    if (equal(qualifier, TEXT(0x009ee800u))) goto L_0067ce17;
    /* b'podr\xeda' */
    if (equal(qualifier, TEXT(0x009ee7e4u))) goto L_0067ce7a;
    /* b'evita' */
    if (equal(qualifier, TEXT(0x009ee7c8u))) goto L_0067cedd;
    /* b'no ha' */
    if (equal(qualifier, TEXT(0x009ee7b0u))) goto L_0067cf40;
    /* b'queda' */
    if (equal(qualifier, TEXT(0x009ee798u))) goto L_0067cfa3;
    /* b'acaba' */
    if (equal(qualifier, TEXT(0x009ee8c8u))) goto L_0067d006;
    /* b'no asciende' */
    if (equal(qualifier, TEXT(0x009ee76cu))) goto L_0067d069;
    /* b'no estaba' */
    if (equal(qualifier, TEXT(0x009ee740u))) goto L_0067d0cc;
    /* b'no est\xe1' */
    if (equal(qualifier, TEXT(0x009ee720u))) goto L_0067d12f;
    /* b'ofrece' */
    if (equal(qualifier, TEXT(0x009ee700u))) goto L_0067d192;
    /* b'te ofrece' */
    if (equal(qualifier, TEXT(0x009ee6e0u))) goto L_0067d1f5;
    /* b'consigui\xf3' */
    if (equal(qualifier, TEXT(0x009ee6bcu))) goto L_0067d258;
    /* b'intenta' */
    if (equal(qualifier, TEXT(0x009ee698u))) goto L_0067d2bb;
    /* b'empieza' */
    if (equal(qualifier, TEXT(0x009ee678u))) goto L_0067d31e;
    /* b'pasa' */
    if (equal(qualifier, TEXT(0x009ee658u))) goto L_0067d381;
    /* b'consigue' */
    if (equal(qualifier, TEXT(0x009ee63cu))) goto L_0067d3e4;
    /* b'logra' */
    if (equal(qualifier, TEXT(0x009ee634u))) goto L_0067d42b;
    /* b'avanza' */
    if (equal(qualifier, TEXT(0x009ee61cu))) goto L_0067d48e;
    /* b'ha vencido' */
    if (equal(qualifier, TEXT(0x009ee5fcu))) goto L_0067d4f1;
    /* b'ha ganado' */
    if (equal(qualifier, TEXT(0x009ee5d4u))) goto L_0067d554;
    /* b'vence' */
    if (equal(qualifier, TEXT(0x009ee5b4u))) goto L_0067d5b7;
    /* b'ha perdido' */
    if (equal(qualifier, TEXT(0x009ee598u))) goto L_0067d61a;
    /* b'ha sido derrotado' */
    if (equal(qualifier, TEXT(0x009ee568u))) goto L_0067d67d;
    /* b'ha ca\xeddo' */
    if (equal(qualifier, TEXT(0x009ee504u))) goto L_0067d6fc;
    /* b'ha' */
    if (equal(qualifier, TEXT(0x009ee4e8u))) goto L_0067d75f;
    /* b'no quiere' */
    if (equal(qualifier, TEXT(0x009ee4d0u))) goto L_0067d7c2;
    /* b'rechaza' */
    if (equal(qualifier, TEXT(0x009ee4b0u))) goto L_0067d825;
    /* b'tratar\xe1' */
    if (equal(qualifier, TEXT(0x009ee490u))) goto L_0067d888;
    /* b'quiere' */
    if (equal(qualifier, TEXT(0x009ee470u))) goto L_0067d8eb;
    /* b'est\xe1' */
    if (equal(qualifier, TEXT(0x009ee454u))) goto L_0067f462;
    /* b'esta' */
    if (equal(qualifier, TEXT(0x009ee44cu))) goto L_0067f462;
    /* b'cobra' */
    if (equal(qualifier, TEXT(0x009ee444u))) goto L_0067d9be;
    /* b'contrata' */
    if (equal(qualifier, TEXT(0x009ee428u))) goto L_0067da21;
    /* b'ficha' */
    if (equal(qualifier, TEXT(0x009ee408u))) goto L_0067da84;
    /* b'no puede' */
    if (equal(qualifier, TEXT(0x009ee3ecu))) goto L_0067dae7;
    /* b'tiene' */
    if (equal(qualifier, TEXT(0x009ee3ccu))) goto L_0067db4a;
    /* b'acepta' */
    if (equal(qualifier, TEXT(0x009ee3b4u))) goto L_0067dbad;
    /* b'se ha' */
    if (equal(qualifier, TEXT(0x009ee3acu))) goto L_0067dbf4;
    /* b'clasificado' */
    if (equal(qualifier, TEXT(0x009ee390u))) goto L_0067dc57;
    /* b'desciende' */
    if (equal(qualifier, TEXT(0x009ee344u))) goto L_0067dcd6;
    /* b'asciende' */
    if (equal(qualifier, TEXT(0x009ee320u))) goto L_0067dd39;
    /* b'ha sido eliminado' */
    if (equal(qualifier, TEXT(0x009ee2f4u))) goto L_0067dd9c;
    /* b'eliminado' */
    if (equal(qualifier, TEXT(0x009eeb90u))) goto L_0067de1b;
    /* b'gana' */
    if (equal(qualifier, TEXT(0x009ee294u))) goto L_0067de9a;
    /* b'lograr\xe1' */
    if (equal(qualifier, TEXT(0x009ee27cu))) goto L_0067defd;
    /* b'intentar\xe1' */
    if (equal(qualifier, TEXT(0x009ee258u))) goto L_0067df60;
    /* b'descendido' */
    if (equal(qualifier, TEXT(0x009ee234u))) goto L_0067dfc3;
    /* b'premiado' */
    if (equal(qualifier, TEXT(0x009ee1f0u))) goto L_0067e042;
    /* b'debe' */
    if (equal(qualifier, TEXT(0x009ee1e8u))) goto L_0067e089;
    /* b'puede' */
    if (equal(qualifier, TEXT(0x009ee1d0u))) goto L_0067e0ec;
    /* b'no tiene' */
    if (equal(qualifier, TEXT(0x009ee1b4u))) goto L_0067e14f;
    /* b'pierde' */
    if (equal(qualifier, TEXT(0x009ee194u))) goto L_0067e1b2;
    /* b'recibe' */
    if (equal(qualifier, TEXT(0x009ee178u))) goto L_0067e215;
    /* b'jugar\xe1' */
    if (equal(qualifier, TEXT(0x009ee15cu))) goto L_0067e278;
    /* b'acepta' */
    if (equal(qualifier, TEXT(0x009ee3b4u))) goto L_0067e2db;
    /* b'no permite' */
    if (equal(qualifier, TEXT(0x009ee128u))) goto L_0067e33e;
    /* b'premiado' */
    if (equal(qualifier, TEXT(0x009ee1f0u))) goto L_0067e3a1;
    /* b'empezar\xe1' */
    if (equal(qualifier, TEXT(0x009ee0d0u))) goto L_0067e420;
    /* b'vuelve' */
    if (equal(qualifier, TEXT(0x009ee0b0u))) goto L_0067e483;
    /* b'sac\xf3' */
    if (equal(qualifier, TEXT(0x009ee094u))) goto L_0067e4e6;
    /* b'saca' */
    if (equal(qualifier, TEXT(0x009ee078u))) goto L_0067e549;
    /* b'comienza' */
    if (equal(qualifier, TEXT(0x009ee05cu))) goto L_0067e5ac;
    /* b'puso' */
    if (equal(qualifier, TEXT(0x009ee03cu))) goto L_0067e60f;
    /* b'pone' */
    if (equal(qualifier, TEXT(0x009ee020u))) goto L_0067e672;
    /* b'empez\xf3' */
    if (equal(qualifier, TEXT(0x009ee008u))) goto L_0067e6d5;
    /* b'parece' */
    if (equal(qualifier, TEXT(0x009edfecu))) goto L_0067e738;
    /* b'juega' */
    if (equal(qualifier, TEXT(0x009edfd0u))) goto L_0067e79b;
    /* b'dej\xf3' */
    if (equal(qualifier, TEXT(0x009edfb8u))) goto L_0067e7fe;
    /* b'intentaba' */
    if (equal(qualifier, TEXT(0x009edf98u))) goto L_0067e861;
    /* b'parec\xeda' */
    if (equal(qualifier, TEXT(0x009edf78u))) goto L_0067e8c4;
    /* b'se ve obligado' */
    if (equal(qualifier, TEXT(0x009edf50u))) goto L_0067e927;
    /* b'hace' */
    if (equal(qualifier, TEXT(0x009edf00u))) goto L_0067e9a6;
    /* b'hizo' */
    if (equal(qualifier, TEXT(0x009edee8u))) goto L_0067ea09;
    /* b'va' */
    if (equal(qualifier, TEXT(0x009f11fcu))) goto L_0067ea6c;
    /* b'iba' */
    if (equal(qualifier, TEXT(0x009edec8u))) goto L_0067eacf;
    /* b'introdujo' */
    if (equal(qualifier, TEXT(0x009edeacu))) goto L_0067eb32;
    /* b'realiz\xf3' */
    if (equal(qualifier, TEXT(0x009ede88u))) goto L_0067eb95;
    /* b'intent\xf3' */
    if (equal(qualifier, TEXT(0x009ede68u))) goto L_0067ebf8;
    /* b'estaba' */
    if (equal(qualifier, TEXT(0x009ede48u))) goto L_0067ec5b;
    /* b'quer\xeda' */
    if (equal(qualifier, TEXT(0x009ede2cu))) goto L_0067ecbe;
    /* b'marc\xf3' */
    if (equal(qualifier, TEXT(0x009ede10u))) goto L_0067ed21;
    /* b'no sel le' */
    if (equal(qualifier, TEXT(0x009eddf0u))) goto L_0067ed84;
    /* b'gan\xf3' */
    if (equal(qualifier, TEXT(0x009eddb8u))) goto L_0067ee03;
    /* b'logr\xf3' */
    if (equal(qualifier, TEXT(0x009edd9cu))) goto L_0067ee66;
    /* b'lo ha' */
    if (equal(qualifier, TEXT(0x009edd80u))) goto L_0067eec9;
    /* b'pudo' */
    if (equal(qualifier, TEXT(0x009edd68u))) goto L_0067ef2c;
    /* b'empat\xf3' */
    if (equal(qualifier, TEXT(0x009edd4cu))) goto L_0067ef8f;
    /* b'marca' */
    if (equal(qualifier, TEXT(0x009edd30u))) goto L_0067eff2;
    /* b'ten\xeda' */
    if (equal(qualifier, TEXT(0x009edd18u))) goto L_0067f055;
    /* b'consigue' */
    if (equal(qualifier, TEXT(0x009ee63cu))) goto L_0067f0b8;
    /* b'ganaba' */
    if (equal(qualifier, TEXT(0x009edce8u))) goto L_0067f11b;
    /* b'adopta' */
    if (equal(qualifier, TEXT(0x009edcccu))) goto L_0067f17e;
    /* b'adopt\xf3' */
    if (equal(qualifier, TEXT(0x009edcb0u))) goto L_0067f1e1;
    /* b'cambia' */
    if (equal(qualifier, TEXT(0x009edc94u))) goto L_0067f244;
    /* b'cambi\xf3' */
    if (equal(qualifier, TEXT(0x009edc78u))) goto L_0067f2a7;
    /* b'se mete' */
    if (equal(qualifier, TEXT(0x009edc5cu))) goto L_0067f30a;
    /* b'se met\xeda' */
    if (equal(qualifier, TEXT(0x009edc38u))) goto L_0067f36d;
    /* b'volvi\xf3' */
    if (equal(qualifier, TEXT(0x009edc18u))) goto L_0067f3d0;
    /* b'tenga' */
    if (equal(qualifier, TEXT(0x009edbfcu))) goto L_0067f433;
    *(volatile cm_lang_u32 *)(frame + 0x14u) = 0;
    goto L_0067f4ad;
L_0067c94d:
    destination = local(frame, 0x18u);
    literal = 0xad9c64u;
    goto L_0067f490;
L_0067c98f:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x68140cu + (cm_lang_u32)value * 4u)) {
    case 0x0067c9a2u: goto L_0067c9a2;
    case 0x0067c9b0u: goto L_0067c9b0;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067c9a2:
    destination = local(frame, 0x18u);
    literal = 0x9ee960u;
    goto L_0067f490;
L_0067c9b0:
    destination = local(frame, 0x18u);
    literal = 0x9ee94cu;
    goto L_0067f490;
L_0067c9f2:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681424u + (cm_lang_u32)value * 4u)) {
    case 0x0067ca05u: goto L_0067ca05;
    case 0x0067ca13u: goto L_0067ca13;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067ca05:
    destination = local(frame, 0x18u);
    literal = 0x9ee92cu;
    goto L_0067f490;
L_0067ca13:
    destination = local(frame, 0x18u);
    literal = 0x9ee91cu;
    goto L_0067f490;
L_0067ca55:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x68143cu + (cm_lang_u32)value * 4u)) {
    case 0x0067ca68u: goto L_0067ca68;
    case 0x0067ca76u: goto L_0067ca76;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067ca68:
    destination = local(frame, 0x18u);
    literal = 0x9ee90cu;
    goto L_0067f490;
L_0067ca76:
    destination = local(frame, 0x18u);
    literal = 0x9ee904u;
    goto L_0067f490;
L_0067cab8:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681454u + (cm_lang_u32)value * 4u)) {
    case 0x0067cacbu: goto L_0067cacb;
    case 0x0067cad9u: goto L_0067cad9;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067cacb:
    destination = local(frame, 0x18u);
    literal = 0x9ee8f4u;
    goto L_0067f490;
L_0067cad9:
    destination = local(frame, 0x18u);
    literal = 0x9ee8e8u;
    goto L_0067f490;
L_0067cb1b:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x68146cu + (cm_lang_u32)value * 4u)) {
    case 0x0067cb2eu: goto L_0067cb2e;
    case 0x0067cb3cu: goto L_0067cb3c;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067cb2e:
    destination = local(frame, 0x18u);
    literal = 0x9ee8d8u;
    goto L_0067f490;
L_0067cb3c:
    destination = local(frame, 0x18u);
    literal = 0x9ee8d0u;
    goto L_0067f490;
L_0067cb7e:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681484u + (cm_lang_u32)value * 4u)) {
    case 0x0067d019u: goto L_0067d019;
    case 0x0067d027u: goto L_0067d027;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067cbc5:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x68149cu + (cm_lang_u32)value * 4u)) {
    case 0x0067cbd8u: goto L_0067cbd8;
    case 0x0067cbe6u: goto L_0067cbe6;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067cbd8:
    destination = local(frame, 0x18u);
    literal = 0x9ee8c4u;
    goto L_0067f490;
L_0067cbe6:
    destination = local(frame, 0x18u);
    literal = 0x9ee8bcu;
    goto L_0067f490;
L_0067cc28:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x6814b4u + (cm_lang_u32)value * 4u)) {
    case 0x0067cc3bu: goto L_0067cc3b;
    case 0x0067cc49u: goto L_0067cc49;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067cc3b:
    destination = local(frame, 0x18u);
    literal = 0x9ee8a4u;
    goto L_0067f490;
L_0067cc49:
    destination = local(frame, 0x18u);
    literal = 0x9ee898u;
    goto L_0067f490;
L_0067cc8b:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x6814ccu + (cm_lang_u32)value * 4u)) {
    case 0x0067cc9eu: goto L_0067cc9e;
    case 0x0067ccacu: goto L_0067ccac;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067cc9e:
    destination = local(frame, 0x18u);
    literal = 0x9ee880u;
    goto L_0067f490;
L_0067ccac:
    destination = local(frame, 0x18u);
    literal = 0x9ee874u;
    goto L_0067f490;
L_0067ccee:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x6814e4u + (cm_lang_u32)value * 4u)) {
    case 0x0067cd01u: goto L_0067cd01;
    case 0x0067cd0fu: goto L_0067cd0f;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067cd01:
    destination = local(frame, 0x18u);
    literal = 0x9ee85cu;
    goto L_0067f490;
L_0067cd0f:
    destination = local(frame, 0x18u);
    literal = 0x9ee850u;
    goto L_0067f490;
L_0067cd51:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x6814fcu + (cm_lang_u32)value * 4u)) {
    case 0x0067cd64u: goto L_0067cd64;
    case 0x0067cd72u: goto L_0067cd72;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067cd64:
    destination = local(frame, 0x18u);
    literal = 0x9ee838u;
    goto L_0067f490;
L_0067cd72:
    destination = local(frame, 0x18u);
    literal = 0x9ee82cu;
    goto L_0067f490;
L_0067cdb4:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681514u + (cm_lang_u32)value * 4u)) {
    case 0x0067cdc7u: goto L_0067cdc7;
    case 0x0067cdd5u: goto L_0067cdd5;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067cdc7:
    destination = local(frame, 0x18u);
    literal = 0x9ee814u;
    goto L_0067f490;
L_0067cdd5:
    destination = local(frame, 0x18u);
    literal = 0x9ee808u;
    goto L_0067f490;
L_0067ce17:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x68152cu + (cm_lang_u32)value * 4u)) {
    case 0x0067ce2au: goto L_0067ce2a;
    case 0x0067ce38u: goto L_0067ce38;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067ce2a:
    destination = local(frame, 0x18u);
    literal = 0x9ee7f8u;
    goto L_0067f490;
L_0067ce38:
    destination = local(frame, 0x18u);
    literal = 0x9ee7ecu;
    goto L_0067f490;
L_0067ce7a:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681544u + (cm_lang_u32)value * 4u)) {
    case 0x0067ce8du: goto L_0067ce8d;
    case 0x0067ce9bu: goto L_0067ce9b;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067ce8d:
    destination = local(frame, 0x18u);
    literal = 0x9ee7dcu;
    goto L_0067f490;
L_0067ce9b:
    destination = local(frame, 0x18u);
    literal = 0x9ee7d0u;
    goto L_0067f490;
L_0067cedd:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x68155cu + (cm_lang_u32)value * 4u)) {
    case 0x0067cef0u: goto L_0067cef0;
    case 0x0067cefeu: goto L_0067cefe;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067cef0:
    destination = local(frame, 0x18u);
    literal = 0x9ee7c0u;
    goto L_0067f490;
L_0067cefe:
    destination = local(frame, 0x18u);
    literal = 0x9ee7b8u;
    goto L_0067f490;
L_0067cf40:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681574u + (cm_lang_u32)value * 4u)) {
    case 0x0067cf53u: goto L_0067cf53;
    case 0x0067cf61u: goto L_0067cf61;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067cf53:
    destination = local(frame, 0x18u);
    literal = 0x9ee7a8u;
    goto L_0067f490;
L_0067cf61:
    destination = local(frame, 0x18u);
    literal = 0x9ee7a0u;
    goto L_0067f490;
L_0067cfa3:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x68158cu + (cm_lang_u32)value * 4u)) {
    case 0x0067cfb6u: goto L_0067cfb6;
    case 0x0067cfc4u: goto L_0067cfc4;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067cfb6:
    destination = local(frame, 0x18u);
    literal = 0x9ee790u;
    goto L_0067f490;
L_0067cfc4:
    destination = local(frame, 0x18u);
    literal = 0x9ee788u;
    goto L_0067f490;
L_0067d006:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x6815a4u + (cm_lang_u32)value * 4u)) {
    case 0x0067d019u: goto L_0067d019;
    case 0x0067d027u: goto L_0067d027;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d019:
    destination = local(frame, 0x18u);
    literal = 0x9ee780u;
    goto L_0067f490;
L_0067d027:
    destination = local(frame, 0x18u);
    literal = 0x9ee778u;
    goto L_0067f490;
L_0067d069:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x6815bcu + (cm_lang_u32)value * 4u)) {
    case 0x0067d07cu: goto L_0067d07c;
    case 0x0067d08au: goto L_0067d08a;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d07c:
    destination = local(frame, 0x18u);
    literal = 0x9ee75cu;
    goto L_0067f490;
L_0067d08a:
    destination = local(frame, 0x18u);
    literal = 0x9ee74cu;
    goto L_0067f490;
L_0067d0cc:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x6815d4u + (cm_lang_u32)value * 4u)) {
    case 0x0067d0dfu: goto L_0067d0df;
    case 0x0067d0edu: goto L_0067d0ed;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d0df:
    destination = local(frame, 0x18u);
    literal = 0x9ee734u;
    goto L_0067f490;
L_0067d0ed:
    destination = local(frame, 0x18u);
    literal = 0x9ee728u;
    goto L_0067f490;
L_0067d12f:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x6815ecu + (cm_lang_u32)value * 4u)) {
    case 0x0067d142u: goto L_0067d142;
    case 0x0067d150u: goto L_0067d150;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d142:
    destination = local(frame, 0x18u);
    literal = 0x9ee714u;
    goto L_0067f490;
L_0067d150:
    destination = local(frame, 0x18u);
    literal = 0x9ee708u;
    goto L_0067f490;
L_0067d192:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681604u + (cm_lang_u32)value * 4u)) {
    case 0x0067d1a5u: goto L_0067d1a5;
    case 0x0067d1b3u: goto L_0067d1b3;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d1a5:
    destination = local(frame, 0x18u);
    literal = 0x9ee6f8u;
    goto L_0067f490;
L_0067d1b3:
    destination = local(frame, 0x18u);
    literal = 0x9ee6ecu;
    goto L_0067f490;
L_0067d1f5:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x68161cu + (cm_lang_u32)value * 4u)) {
    case 0x0067d208u: goto L_0067d208;
    case 0x0067d216u: goto L_0067d216;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d208:
    destination = local(frame, 0x18u);
    literal = 0x9ee6d4u;
    goto L_0067f490;
L_0067d216:
    destination = local(frame, 0x18u);
    literal = 0x9ee6c8u;
    goto L_0067f490;
L_0067d258:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681634u + (cm_lang_u32)value * 4u)) {
    case 0x0067d26bu: goto L_0067d26b;
    case 0x0067d279u: goto L_0067d279;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d26b:
    destination = local(frame, 0x18u);
    literal = 0x9ee6b0u;
    goto L_0067f490;
L_0067d279:
    destination = local(frame, 0x18u);
    literal = 0x9ee6a0u;
    goto L_0067f490;
L_0067d2bb:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x68164cu + (cm_lang_u32)value * 4u)) {
    case 0x0067d2ceu: goto L_0067d2ce;
    case 0x0067d2dcu: goto L_0067d2dc;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d2ce:
    destination = local(frame, 0x18u);
    literal = 0x9ee68cu;
    goto L_0067f490;
L_0067d2dc:
    destination = local(frame, 0x18u);
    literal = 0x9ee680u;
    goto L_0067f490;
L_0067d31e:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681664u + (cm_lang_u32)value * 4u)) {
    case 0x0067d331u: goto L_0067d331;
    case 0x0067d33fu: goto L_0067d33f;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d331:
    destination = local(frame, 0x18u);
    literal = 0x9ee66cu;
    goto L_0067f490;
L_0067d33f:
    destination = local(frame, 0x18u);
    literal = 0x9ee660u;
    goto L_0067f490;
L_0067d381:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x68167cu + (cm_lang_u32)value * 4u)) {
    case 0x0067d394u: goto L_0067d394;
    case 0x0067d3a2u: goto L_0067d3a2;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d394:
    destination = local(frame, 0x18u);
    literal = 0x9ee650u;
    goto L_0067f490;
L_0067d3a2:
    destination = local(frame, 0x18u);
    literal = 0x9ee648u;
    goto L_0067f490;
L_0067d3e4:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681694u + (cm_lang_u32)value * 4u)) {
    case 0x0067f0cbu: goto L_0067f0cb;
    case 0x0067f0d9u: goto L_0067f0d9;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d42b:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x6816acu + (cm_lang_u32)value * 4u)) {
    case 0x0067d43eu: goto L_0067d43e;
    case 0x0067d44cu: goto L_0067d44c;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d43e:
    destination = local(frame, 0x18u);
    literal = 0x9ee62cu;
    goto L_0067f490;
L_0067d44c:
    destination = local(frame, 0x18u);
    literal = 0x9ee624u;
    goto L_0067f490;
L_0067d48e:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x6816c4u + (cm_lang_u32)value * 4u)) {
    case 0x0067d4a1u: goto L_0067d4a1;
    case 0x0067d4afu: goto L_0067d4af;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d4a1:
    destination = local(frame, 0x18u);
    literal = 0x9ee614u;
    goto L_0067f490;
L_0067d4af:
    destination = local(frame, 0x18u);
    literal = 0x9ee608u;
    goto L_0067f490;
L_0067d4f1:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x6816dcu + (cm_lang_u32)value * 4u)) {
    case 0x0067d504u: goto L_0067d504;
    case 0x0067d512u: goto L_0067d512;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d504:
    destination = local(frame, 0x18u);
    literal = 0x9ee5f0u;
    goto L_0067f490;
L_0067d512:
    destination = local(frame, 0x18u);
    literal = 0x9ee5e0u;
    goto L_0067f490;
L_0067d554:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x6816f4u + (cm_lang_u32)value * 4u)) {
    case 0x0067d567u: goto L_0067d567;
    case 0x0067d575u: goto L_0067d575;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d567:
    destination = local(frame, 0x18u);
    literal = 0x9ee5c8u;
    goto L_0067f490;
L_0067d575:
    destination = local(frame, 0x18u);
    literal = 0x9ee5bcu;
    goto L_0067f490;
L_0067d5b7:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x68170cu + (cm_lang_u32)value * 4u)) {
    case 0x0067d5cau: goto L_0067d5ca;
    case 0x0067d5d8u: goto L_0067d5d8;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d5ca:
    destination = local(frame, 0x18u);
    literal = 0x9ee5acu;
    goto L_0067f490;
L_0067d5d8:
    destination = local(frame, 0x18u);
    literal = 0x9ee5a4u;
    goto L_0067f490;
L_0067d61a:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681724u + (cm_lang_u32)value * 4u)) {
    case 0x0067d62du: goto L_0067d62d;
    case 0x0067d63bu: goto L_0067d63b;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d62d:
    destination = local(frame, 0x18u);
    literal = 0x9ee58cu;
    goto L_0067f490;
L_0067d63b:
    destination = local(frame, 0x18u);
    literal = 0x9ee57cu;
    goto L_0067f490;
L_0067d67d:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x68173cu + (cm_lang_u32)value * 4u)) {
    case 0x0067d690u: goto L_0067d690;
    case 0x0067d69eu: goto L_0067d69e;
    case 0x0067d6acu: goto L_0067d6ac;
    case 0x0067d6bau: goto L_0067d6ba;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d690:
    destination = local(frame, 0x18u);
    literal = 0x9ee554u;
    goto L_0067f490;
L_0067d69e:
    destination = local(frame, 0x18u);
    literal = 0x9ee540u;
    goto L_0067f490;
L_0067d6ac:
    destination = local(frame, 0x18u);
    literal = 0x9ee528u;
    goto L_0067f490;
L_0067d6ba:
    destination = local(frame, 0x18u);
    literal = 0x9ee510u;
    goto L_0067f490;
L_0067d6fc:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681754u + (cm_lang_u32)value * 4u)) {
    case 0x0067d70fu: goto L_0067d70f;
    case 0x0067d71du: goto L_0067d71d;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d70f:
    destination = local(frame, 0x18u);
    literal = 0x9ee4f8u;
    goto L_0067f490;
L_0067d71d:
    destination = local(frame, 0x18u);
    literal = 0x9ee4ecu;
    goto L_0067f490;
L_0067d75f:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x68176cu + (cm_lang_u32)value * 4u)) {
    case 0x0067d772u: goto L_0067d772;
    case 0x0067d780u: goto L_0067d780;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d772:
    destination = local(frame, 0x18u);
    literal = 0x9ee4e4u;
    goto L_0067f490;
L_0067d780:
    destination = local(frame, 0x18u);
    literal = 0x9ee4dcu;
    goto L_0067f490;
L_0067d7c2:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681784u + (cm_lang_u32)value * 4u)) {
    case 0x0067d7d5u: goto L_0067d7d5;
    case 0x0067d7e3u: goto L_0067d7e3;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d7d5:
    destination = local(frame, 0x18u);
    literal = 0x9ee4c4u;
    goto L_0067f490;
L_0067d7e3:
    destination = local(frame, 0x18u);
    literal = 0x9ee4b8u;
    goto L_0067f490;
L_0067d825:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x68179cu + (cm_lang_u32)value * 4u)) {
    case 0x0067d838u: goto L_0067d838;
    case 0x0067d846u: goto L_0067d846;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d838:
    destination = local(frame, 0x18u);
    literal = 0x9ee4a4u;
    goto L_0067f490;
L_0067d846:
    destination = local(frame, 0x18u);
    literal = 0x9ee498u;
    goto L_0067f490;
L_0067d888:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x6817b4u + (cm_lang_u32)value * 4u)) {
    case 0x0067d89bu: goto L_0067d89b;
    case 0x0067d8a9u: goto L_0067d8a9;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d89b:
    destination = local(frame, 0x18u);
    literal = 0x9ee484u;
    goto L_0067f490;
L_0067d8a9:
    destination = local(frame, 0x18u);
    literal = 0x9ee478u;
    goto L_0067f490;
L_0067d8eb:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x6817ccu + (cm_lang_u32)value * 4u)) {
    case 0x0067d8feu: goto L_0067d8fe;
    case 0x0067d90cu: goto L_0067d90c;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d8fe:
    destination = local(frame, 0x18u);
    literal = 0x9ee468u;
    goto L_0067f490;
L_0067d90c:
    destination = local(frame, 0x18u);
    literal = 0x9ee45cu;
    goto L_0067f490;
L_0067d9be:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x6817e4u + (cm_lang_u32)value * 4u)) {
    case 0x0067d9d1u: goto L_0067d9d1;
    case 0x0067d9dfu: goto L_0067d9df;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067d9d1:
    destination = local(frame, 0x18u);
    literal = 0x9ee43cu;
    goto L_0067f490;
L_0067d9df:
    destination = local(frame, 0x18u);
    literal = 0x9ee434u;
    goto L_0067f490;
L_0067da21:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x6817fcu + (cm_lang_u32)value * 4u)) {
    case 0x0067da34u: goto L_0067da34;
    case 0x0067da42u: goto L_0067da42;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067da34:
    destination = local(frame, 0x18u);
    literal = 0x9ee41cu;
    goto L_0067f490;
L_0067da42:
    destination = local(frame, 0x18u);
    literal = 0x9ee410u;
    goto L_0067f490;
L_0067da84:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681814u + (cm_lang_u32)value * 4u)) {
    case 0x0067da97u: goto L_0067da97;
    case 0x0067daa5u: goto L_0067daa5;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067da97:
    destination = local(frame, 0x18u);
    literal = 0x9ee400u;
    goto L_0067f490;
L_0067daa5:
    destination = local(frame, 0x18u);
    literal = 0x9ee3f8u;
    goto L_0067f490;
L_0067dae7:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x68182cu + (cm_lang_u32)value * 4u)) {
    case 0x0067dafau: goto L_0067dafa;
    case 0x0067db08u: goto L_0067db08;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067dafa:
    destination = local(frame, 0x18u);
    literal = 0x9ee3e0u;
    goto L_0067f490;
L_0067db08:
    destination = local(frame, 0x18u);
    literal = 0x9ee3d4u;
    goto L_0067f490;
L_0067db4a:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681844u + (cm_lang_u32)value * 4u)) {
    case 0x0067db5du: goto L_0067db5d;
    case 0x0067db6bu: goto L_0067db6b;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067db5d:
    destination = local(frame, 0x18u);
    literal = 0x9ee3c4u;
    goto L_0067f490;
L_0067db6b:
    destination = local(frame, 0x18u);
    literal = 0x9ee3bcu;
    goto L_0067f490;
L_0067dbad:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x68185cu + (cm_lang_u32)value * 4u)) {
    case 0x0067e2eeu: goto L_0067e2ee;
    case 0x0067e2fcu: goto L_0067e2fc;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067dbf4:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681874u + (cm_lang_u32)value * 4u)) {
    case 0x0067dc07u: goto L_0067dc07;
    case 0x0067dc15u: goto L_0067dc15;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067dc07:
    destination = local(frame, 0x18u);
    literal = 0x9ee3a4u;
    goto L_0067f490;
L_0067dc15:
    destination = local(frame, 0x18u);
    literal = 0x9ee39cu;
    goto L_0067f490;
L_0067dc57:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x68188cu + (cm_lang_u32)value * 4u)) {
    case 0x0067dc6au: goto L_0067dc6a;
    case 0x0067dc78u: goto L_0067dc78;
    case 0x0067dc86u: goto L_0067dc86;
    case 0x0067dc94u: goto L_0067dc94;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067dc6a:
    destination = local(frame, 0x18u);
    literal = 0x9ee380u;
    goto L_0067f490;
L_0067dc78:
    destination = local(frame, 0x18u);
    literal = 0x9ee370u;
    goto L_0067f490;
L_0067dc86:
    destination = local(frame, 0x18u);
    literal = 0x9ee360u;
    goto L_0067f490;
L_0067dc94:
    destination = local(frame, 0x18u);
    literal = 0x9ee350u;
    goto L_0067f490;
L_0067dcd6:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x6818a4u + (cm_lang_u32)value * 4u)) {
    case 0x0067dce9u: goto L_0067dce9;
    case 0x0067dcf7u: goto L_0067dcf7;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067dce9:
    destination = local(frame, 0x18u);
    literal = 0x9ee338u;
    goto L_0067f490;
L_0067dcf7:
    destination = local(frame, 0x18u);
    literal = 0x9ee32cu;
    goto L_0067f490;
L_0067dd39:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x6818bcu + (cm_lang_u32)value * 4u)) {
    case 0x0067dd4cu: goto L_0067dd4c;
    case 0x0067dd5au: goto L_0067dd5a;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067dd4c:
    destination = local(frame, 0x18u);
    literal = 0x9ee314u;
    goto L_0067f490;
L_0067dd5a:
    destination = local(frame, 0x18u);
    literal = 0x9ee308u;
    goto L_0067f490;
L_0067dd9c:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x6818d4u + (cm_lang_u32)value * 4u)) {
    case 0x0067ddafu: goto L_0067ddaf;
    case 0x0067ddbdu: goto L_0067ddbd;
    case 0x0067ddcbu: goto L_0067ddcb;
    case 0x0067ddd9u: goto L_0067ddd9;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067ddaf:
    destination = local(frame, 0x18u);
    literal = 0x9ee2e0u;
    goto L_0067f490;
L_0067ddbd:
    destination = local(frame, 0x18u);
    literal = 0x9ee2ccu;
    goto L_0067f490;
L_0067ddcb:
    destination = local(frame, 0x18u);
    literal = 0x9ee2b4u;
    goto L_0067f490;
L_0067ddd9:
    destination = local(frame, 0x18u);
    literal = 0x9ee29cu;
    goto L_0067f490;
L_0067de1b:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x6818ecu + (cm_lang_u32)value * 4u)) {
    case 0x0067de2eu: goto L_0067de2e;
    case 0x0067de3cu: goto L_0067de3c;
    case 0x0067de4au: goto L_0067de4a;
    case 0x0067de58u: goto L_0067de58;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067de2e:
    destination = local(frame, 0x18u);
    literal = 0x9eeb84u;
    goto L_0067f490;
L_0067de3c:
    destination = local(frame, 0x18u);
    literal = 0x9eeb78u;
    goto L_0067f490;
L_0067de4a:
    destination = local(frame, 0x18u);
    literal = 0x9eeb6cu;
    goto L_0067f490;
L_0067de58:
    destination = local(frame, 0x18u);
    literal = 0x9eeb60u;
    goto L_0067f490;
L_0067de9a:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681904u + (cm_lang_u32)value * 4u)) {
    case 0x0067deadu: goto L_0067dead;
    case 0x0067debbu: goto L_0067debb;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067dead:
    destination = local(frame, 0x18u);
    literal = 0x9ee28cu;
    goto L_0067f490;
L_0067debb:
    destination = local(frame, 0x18u);
    literal = 0x9ee284u;
    goto L_0067f490;
L_0067defd:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x68191cu + (cm_lang_u32)value * 4u)) {
    case 0x0067df10u: goto L_0067df10;
    case 0x0067df1eu: goto L_0067df1e;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067df10:
    destination = local(frame, 0x18u);
    literal = 0x9ee270u;
    goto L_0067f490;
L_0067df1e:
    destination = local(frame, 0x18u);
    literal = 0x9ee264u;
    goto L_0067f490;
L_0067df60:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681934u + (cm_lang_u32)value * 4u)) {
    case 0x0067df73u: goto L_0067df73;
    case 0x0067df81u: goto L_0067df81;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067df73:
    destination = local(frame, 0x18u);
    literal = 0x9ee24cu;
    goto L_0067f490;
L_0067df81:
    destination = local(frame, 0x18u);
    literal = 0x9ee240u;
    goto L_0067f490;
L_0067dfc3:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x68194cu + (cm_lang_u32)value * 4u)) {
    case 0x0067dfd6u: goto L_0067dfd6;
    case 0x0067dfe4u: goto L_0067dfe4;
    case 0x0067dff2u: goto L_0067dff2;
    case 0x0067e000u: goto L_0067e000;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067dfd6:
    destination = local(frame, 0x18u);
    literal = 0x9ee228u;
    goto L_0067f490;
L_0067dfe4:
    destination = local(frame, 0x18u);
    literal = 0x9ee21cu;
    goto L_0067f490;
L_0067dff2:
    destination = local(frame, 0x18u);
    literal = 0x9ee20cu;
    goto L_0067f490;
L_0067e000:
    destination = local(frame, 0x18u);
    literal = 0x9ee1fcu;
    goto L_0067f490;
L_0067e042:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681964u + (cm_lang_u32)value * 4u)) {
    case 0x0067e3b4u: goto L_0067e3b4;
    case 0x0067e3c2u: goto L_0067e3c2;
    case 0x0067e3d0u: goto L_0067e3d0;
    case 0x0067e3deu: goto L_0067e3de;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e089:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x68197cu + (cm_lang_u32)value * 4u)) {
    case 0x0067e09cu: goto L_0067e09c;
    case 0x0067e0aau: goto L_0067e0aa;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e09c:
    destination = local(frame, 0x18u);
    literal = 0x9ee1e0u;
    goto L_0067f490;
L_0067e0aa:
    destination = local(frame, 0x18u);
    literal = 0x9ee1d8u;
    goto L_0067f490;
L_0067e0ec:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681994u + (cm_lang_u32)value * 4u)) {
    case 0x0067e0ffu: goto L_0067e0ff;
    case 0x0067e10du: goto L_0067e10d;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e0ff:
    destination = local(frame, 0x18u);
    literal = 0x9ee1c8u;
    goto L_0067f490;
L_0067e10d:
    destination = local(frame, 0x18u);
    literal = 0x9ee1c0u;
    goto L_0067f490;
L_0067e14f:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x6819acu + (cm_lang_u32)value * 4u)) {
    case 0x0067e162u: goto L_0067e162;
    case 0x0067e170u: goto L_0067e170;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e162:
    destination = local(frame, 0x18u);
    literal = 0x9ee1a8u;
    goto L_0067f490;
L_0067e170:
    destination = local(frame, 0x18u);
    literal = 0x9ee19cu;
    goto L_0067f490;
L_0067e1b2:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x6819c4u + (cm_lang_u32)value * 4u)) {
    case 0x0067e1c5u: goto L_0067e1c5;
    case 0x0067e1d3u: goto L_0067e1d3;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e1c5:
    destination = local(frame, 0x18u);
    literal = 0x9ee18cu;
    goto L_0067f490;
L_0067e1d3:
    destination = local(frame, 0x18u);
    literal = 0x9ee180u;
    goto L_0067f490;
L_0067e215:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x6819dcu + (cm_lang_u32)value * 4u)) {
    case 0x0067e228u: goto L_0067e228;
    case 0x0067e236u: goto L_0067e236;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e228:
    destination = local(frame, 0x18u);
    literal = 0x9ee170u;
    goto L_0067f490;
L_0067e236:
    destination = local(frame, 0x18u);
    literal = 0x9ee164u;
    goto L_0067f490;
L_0067e278:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x6819f4u + (cm_lang_u32)value * 4u)) {
    case 0x0067e28bu: goto L_0067e28b;
    case 0x0067e299u: goto L_0067e299;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e28b:
    destination = local(frame, 0x18u);
    literal = 0x9ee154u;
    goto L_0067f490;
L_0067e299:
    destination = local(frame, 0x18u);
    literal = 0x9ee148u;
    goto L_0067f490;
L_0067e2db:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681a0cu + (cm_lang_u32)value * 4u)) {
    case 0x0067e2eeu: goto L_0067e2ee;
    case 0x0067e2fcu: goto L_0067e2fc;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e2ee:
    destination = local(frame, 0x18u);
    literal = 0x9ee140u;
    goto L_0067f490;
L_0067e2fc:
    destination = local(frame, 0x18u);
    literal = 0x9ee134u;
    goto L_0067f490;
L_0067e33e:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681a24u + (cm_lang_u32)value * 4u)) {
    case 0x0067e351u: goto L_0067e351;
    case 0x0067e35fu: goto L_0067e35f;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e351:
    destination = local(frame, 0x18u);
    literal = 0x9ee11cu;
    goto L_0067f490;
L_0067e35f:
    destination = local(frame, 0x18u);
    literal = 0x9ee10cu;
    goto L_0067f490;
L_0067e3a1:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681a3cu + (cm_lang_u32)value * 4u)) {
    case 0x0067e3b4u: goto L_0067e3b4;
    case 0x0067e3c2u: goto L_0067e3c2;
    case 0x0067e3d0u: goto L_0067e3d0;
    case 0x0067e3deu: goto L_0067e3de;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e3b4:
    destination = local(frame, 0x18u);
    literal = 0x9ee100u;
    goto L_0067f490;
L_0067e3c2:
    destination = local(frame, 0x18u);
    literal = 0x9ee0f4u;
    goto L_0067f490;
L_0067e3d0:
    destination = local(frame, 0x18u);
    literal = 0x9ee0e8u;
    goto L_0067f490;
L_0067e3de:
    destination = local(frame, 0x18u);
    literal = 0x9ee0dcu;
    goto L_0067f490;
L_0067e420:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681a54u + (cm_lang_u32)value * 4u)) {
    case 0x0067e433u: goto L_0067e433;
    case 0x0067e441u: goto L_0067e441;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e433:
    destination = local(frame, 0x18u);
    literal = 0x9ee0c4u;
    goto L_0067f490;
L_0067e441:
    destination = local(frame, 0x18u);
    literal = 0x9ee0b8u;
    goto L_0067f490;
L_0067e483:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681a6cu + (cm_lang_u32)value * 4u)) {
    case 0x0067e496u: goto L_0067e496;
    case 0x0067e4a4u: goto L_0067e4a4;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e496:
    destination = local(frame, 0x18u);
    literal = 0x9ee0a8u;
    goto L_0067f490;
L_0067e4a4:
    destination = local(frame, 0x18u);
    literal = 0x9ee09cu;
    goto L_0067f490;
L_0067e4e6:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681a84u + (cm_lang_u32)value * 4u)) {
    case 0x0067e4f9u: goto L_0067e4f9;
    case 0x0067e507u: goto L_0067e507;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e4f9:
    destination = local(frame, 0x18u);
    literal = 0x9ee08cu;
    goto L_0067f490;
L_0067e507:
    destination = local(frame, 0x18u);
    literal = 0x9ee080u;
    goto L_0067f490;
L_0067e549:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681a9cu + (cm_lang_u32)value * 4u)) {
    case 0x0067e55cu: goto L_0067e55c;
    case 0x0067e56au: goto L_0067e56a;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e55c:
    destination = local(frame, 0x18u);
    literal = 0x9ee070u;
    goto L_0067f490;
L_0067e56a:
    destination = local(frame, 0x18u);
    literal = 0x9ee068u;
    goto L_0067f490;
L_0067e5ac:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681ab4u + (cm_lang_u32)value * 4u)) {
    case 0x0067e5bfu: goto L_0067e5bf;
    case 0x0067e5cdu: goto L_0067e5cd;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e5bf:
    destination = local(frame, 0x18u);
    literal = 0x9ee050u;
    goto L_0067f490;
L_0067e5cd:
    destination = local(frame, 0x18u);
    literal = 0x9ee044u;
    goto L_0067f490;
L_0067e60f:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681accu + (cm_lang_u32)value * 4u)) {
    case 0x0067e622u: goto L_0067e622;
    case 0x0067e630u: goto L_0067e630;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e622:
    destination = local(frame, 0x18u);
    literal = 0x9ee034u;
    goto L_0067f490;
L_0067e630:
    destination = local(frame, 0x18u);
    literal = 0x9ee028u;
    goto L_0067f490;
L_0067e672:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681ae4u + (cm_lang_u32)value * 4u)) {
    case 0x0067e685u: goto L_0067e685;
    case 0x0067e693u: goto L_0067e693;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e685:
    destination = local(frame, 0x18u);
    literal = 0x9ee018u;
    goto L_0067f490;
L_0067e693:
    destination = local(frame, 0x18u);
    literal = 0x9ee010u;
    goto L_0067f490;
L_0067e6d5:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681afcu + (cm_lang_u32)value * 4u)) {
    case 0x0067e6e8u: goto L_0067e6e8;
    case 0x0067e6f6u: goto L_0067e6f6;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e6e8:
    destination = local(frame, 0x18u);
    literal = 0x9ee000u;
    goto L_0067f490;
L_0067e6f6:
    destination = local(frame, 0x18u);
    literal = 0x9edff4u;
    goto L_0067f490;
L_0067e738:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681b14u + (cm_lang_u32)value * 4u)) {
    case 0x0067e74bu: goto L_0067e74b;
    case 0x0067e759u: goto L_0067e759;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e74b:
    destination = local(frame, 0x18u);
    literal = 0x9edfe4u;
    goto L_0067f490;
L_0067e759:
    destination = local(frame, 0x18u);
    literal = 0x9edfd8u;
    goto L_0067f490;
L_0067e79b:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681b2cu + (cm_lang_u32)value * 4u)) {
    case 0x0067e7aeu: goto L_0067e7ae;
    case 0x0067e7bcu: goto L_0067e7bc;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e7ae:
    destination = local(frame, 0x18u);
    literal = 0x9edfc8u;
    goto L_0067f490;
L_0067e7bc:
    destination = local(frame, 0x18u);
    literal = 0x9edfc0u;
    goto L_0067f490;
L_0067e7fe:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681b44u + (cm_lang_u32)value * 4u)) {
    case 0x0067e811u: goto L_0067e811;
    case 0x0067e81fu: goto L_0067e81f;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e811:
    destination = local(frame, 0x18u);
    literal = 0x9edfb0u;
    goto L_0067f490;
L_0067e81f:
    destination = local(frame, 0x18u);
    literal = 0x9edfa4u;
    goto L_0067f490;
L_0067e861:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681b5cu + (cm_lang_u32)value * 4u)) {
    case 0x0067e874u: goto L_0067e874;
    case 0x0067e882u: goto L_0067e882;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e874:
    destination = local(frame, 0x18u);
    literal = 0x9edf8cu;
    goto L_0067f490;
L_0067e882:
    destination = local(frame, 0x18u);
    literal = 0x9edf80u;
    goto L_0067f490;
L_0067e8c4:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681b74u + (cm_lang_u32)value * 4u)) {
    case 0x0067e8d7u: goto L_0067e8d7;
    case 0x0067e8e5u: goto L_0067e8e5;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e8d7:
    destination = local(frame, 0x18u);
    literal = 0x9edf6cu;
    goto L_0067f490;
L_0067e8e5:
    destination = local(frame, 0x18u);
    literal = 0x9edf60u;
    goto L_0067f490;
L_0067e927:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681b8cu + (cm_lang_u32)value * 4u)) {
    case 0x0067e93au: goto L_0067e93a;
    case 0x0067e948u: goto L_0067e948;
    case 0x0067e956u: goto L_0067e956;
    case 0x0067e964u: goto L_0067e964;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e93a:
    destination = local(frame, 0x18u);
    literal = 0x9edf40u;
    goto L_0067f490;
L_0067e948:
    destination = local(frame, 0x18u);
    literal = 0x9edf30u;
    goto L_0067f490;
L_0067e956:
    destination = local(frame, 0x18u);
    literal = 0x9edf1cu;
    goto L_0067f490;
L_0067e964:
    destination = local(frame, 0x18u);
    literal = 0x9edf08u;
    goto L_0067f490;
L_0067e9a6:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681ba4u + (cm_lang_u32)value * 4u)) {
    case 0x0067e9b9u: goto L_0067e9b9;
    case 0x0067e9c7u: goto L_0067e9c7;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067e9b9:
    destination = local(frame, 0x18u);
    literal = 0x9edef8u;
    goto L_0067f490;
L_0067e9c7:
    destination = local(frame, 0x18u);
    literal = 0x9edef0u;
    goto L_0067f490;
L_0067ea09:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681bbcu + (cm_lang_u32)value * 4u)) {
    case 0x0067ea1cu: goto L_0067ea1c;
    case 0x0067ea2au: goto L_0067ea2a;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067ea1c:
    destination = local(frame, 0x18u);
    literal = 0x9edee0u;
    goto L_0067f490;
L_0067ea2a:
    destination = local(frame, 0x18u);
    literal = 0x9eded4u;
    goto L_0067f490;
L_0067ea6c:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681bd4u + (cm_lang_u32)value * 4u)) {
    case 0x0067ea7fu: goto L_0067ea7f;
    case 0x0067ea8du: goto L_0067ea8d;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067ea7f:
    destination = local(frame, 0x18u);
    literal = 0x9f11f8u;
    goto L_0067f490;
L_0067ea8d:
    destination = local(frame, 0x18u);
    literal = 0x9edeccu;
    goto L_0067f490;
L_0067eacf:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681becu + (cm_lang_u32)value * 4u)) {
    case 0x0067eae2u: goto L_0067eae2;
    case 0x0067eaf0u: goto L_0067eaf0;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067eae2:
    destination = local(frame, 0x18u);
    literal = 0x9edec0u;
    goto L_0067f490;
L_0067eaf0:
    destination = local(frame, 0x18u);
    literal = 0x9edeb8u;
    goto L_0067f490;
L_0067eb32:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681c04u + (cm_lang_u32)value * 4u)) {
    case 0x0067eb45u: goto L_0067eb45;
    case 0x0067eb53u: goto L_0067eb53;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067eb45:
    destination = local(frame, 0x18u);
    literal = 0x9edea0u;
    goto L_0067f490;
L_0067eb53:
    destination = local(frame, 0x18u);
    literal = 0x9ede90u;
    goto L_0067f490;
L_0067eb95:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681c1cu + (cm_lang_u32)value * 4u)) {
    case 0x0067eba8u: goto L_0067eba8;
    case 0x0067ebb6u: goto L_0067ebb6;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067eba8:
    destination = local(frame, 0x18u);
    literal = 0x9ede7cu;
    goto L_0067f490;
L_0067ebb6:
    destination = local(frame, 0x18u);
    literal = 0x9ede70u;
    goto L_0067f490;
L_0067ebf8:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681c34u + (cm_lang_u32)value * 4u)) {
    case 0x0067ec0bu: goto L_0067ec0b;
    case 0x0067ec19u: goto L_0067ec19;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067ec0b:
    destination = local(frame, 0x18u);
    literal = 0x9ede5cu;
    goto L_0067f490;
L_0067ec19:
    destination = local(frame, 0x18u);
    literal = 0x9ede50u;
    goto L_0067f490;
L_0067ec5b:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681c4cu + (cm_lang_u32)value * 4u)) {
    case 0x0067ec6eu: goto L_0067ec6e;
    case 0x0067ec7cu: goto L_0067ec7c;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067ec6e:
    destination = local(frame, 0x18u);
    literal = 0x9ede40u;
    goto L_0067f490;
L_0067ec7c:
    destination = local(frame, 0x18u);
    literal = 0x9ede34u;
    goto L_0067f490;
L_0067ecbe:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681c64u + (cm_lang_u32)value * 4u)) {
    case 0x0067ecd1u: goto L_0067ecd1;
    case 0x0067ecdfu: goto L_0067ecdf;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067ecd1:
    destination = local(frame, 0x18u);
    literal = 0x9ede24u;
    goto L_0067f490;
L_0067ecdf:
    destination = local(frame, 0x18u);
    literal = 0x9ede18u;
    goto L_0067f490;
L_0067ed21:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681c7cu + (cm_lang_u32)value * 4u)) {
    case 0x0067ed34u: goto L_0067ed34;
    case 0x0067ed42u: goto L_0067ed42;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067ed34:
    destination = local(frame, 0x18u);
    literal = 0x9ede08u;
    goto L_0067f490;
L_0067ed42:
    destination = local(frame, 0x18u);
    literal = 0x9eddfcu;
    goto L_0067f490;
L_0067ed84:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681c94u + (cm_lang_u32)value * 4u)) {
    case 0x0067ed97u: goto L_0067ed97;
    case 0x0067eda5u: goto L_0067eda5;
    case 0x0067edb3u: goto L_0067edb3;
    case 0x0067edc1u: goto L_0067edc1;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067ed97:
    destination = local(frame, 0x18u);
    literal = 0x9edde4u;
    goto L_0067f490;
L_0067eda5:
    destination = local(frame, 0x18u);
    literal = 0x9eddd8u;
    goto L_0067f490;
L_0067edb3:
    destination = local(frame, 0x18u);
    literal = 0x9eddccu;
    goto L_0067f490;
L_0067edc1:
    destination = local(frame, 0x18u);
    literal = 0x9eddc0u;
    goto L_0067f490;
L_0067ee03:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681cacu + (cm_lang_u32)value * 4u)) {
    case 0x0067ee16u: goto L_0067ee16;
    case 0x0067ee24u: goto L_0067ee24;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067ee16:
    destination = local(frame, 0x18u);
    literal = 0x9eddb0u;
    goto L_0067f490;
L_0067ee24:
    destination = local(frame, 0x18u);
    literal = 0x9edda4u;
    goto L_0067f490;
L_0067ee66:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681cc4u + (cm_lang_u32)value * 4u)) {
    case 0x0067ee79u: goto L_0067ee79;
    case 0x0067ee87u: goto L_0067ee87;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067ee79:
    destination = local(frame, 0x18u);
    literal = 0x9edd94u;
    goto L_0067f490;
L_0067ee87:
    destination = local(frame, 0x18u);
    literal = 0x9edd88u;
    goto L_0067f490;
L_0067eec9:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681cdcu + (cm_lang_u32)value * 4u)) {
    case 0x0067eedcu: goto L_0067eedc;
    case 0x0067eeeau: goto L_0067eeea;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067eedc:
    destination = local(frame, 0x18u);
    literal = 0x9edd78u;
    goto L_0067f490;
L_0067eeea:
    destination = local(frame, 0x18u);
    literal = 0x9edd70u;
    goto L_0067f490;
L_0067ef2c:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681cf4u + (cm_lang_u32)value * 4u)) {
    case 0x0067ef3fu: goto L_0067ef3f;
    case 0x0067ef4du: goto L_0067ef4d;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067ef3f:
    destination = local(frame, 0x18u);
    literal = 0x9edd60u;
    goto L_0067f490;
L_0067ef4d:
    destination = local(frame, 0x18u);
    literal = 0x9edd54u;
    goto L_0067f490;
L_0067ef8f:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681d0cu + (cm_lang_u32)value * 4u)) {
    case 0x0067efa2u: goto L_0067efa2;
    case 0x0067efb0u: goto L_0067efb0;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067efa2:
    destination = local(frame, 0x18u);
    literal = 0x9edd44u;
    goto L_0067f490;
L_0067efb0:
    destination = local(frame, 0x18u);
    literal = 0x9edd38u;
    goto L_0067f490;
L_0067eff2:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681d24u + (cm_lang_u32)value * 4u)) {
    case 0x0067f005u: goto L_0067f005;
    case 0x0067f013u: goto L_0067f013;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f005:
    destination = local(frame, 0x18u);
    literal = 0x9edd28u;
    goto L_0067f490;
L_0067f013:
    destination = local(frame, 0x18u);
    literal = 0x9edd20u;
    goto L_0067f490;
L_0067f055:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681d3cu + (cm_lang_u32)value * 4u)) {
    case 0x0067f068u: goto L_0067f068;
    case 0x0067f076u: goto L_0067f076;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f068:
    destination = local(frame, 0x18u);
    literal = 0x9edd10u;
    goto L_0067f490;
L_0067f076:
    destination = local(frame, 0x18u);
    literal = 0x9edd08u;
    goto L_0067f490;
L_0067f0b8:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681d54u + (cm_lang_u32)value * 4u)) {
    case 0x0067f0cbu: goto L_0067f0cb;
    case 0x0067f0d9u: goto L_0067f0d9;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f0cb:
    destination = local(frame, 0x18u);
    literal = 0x9edcfcu;
    goto L_0067f490;
L_0067f0d9:
    destination = local(frame, 0x18u);
    literal = 0x9edcf0u;
    goto L_0067f490;
L_0067f11b:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681d6cu + (cm_lang_u32)value * 4u)) {
    case 0x0067f12eu: goto L_0067f12e;
    case 0x0067f13cu: goto L_0067f13c;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f12e:
    destination = local(frame, 0x18u);
    literal = 0x9edce0u;
    goto L_0067f490;
L_0067f13c:
    destination = local(frame, 0x18u);
    literal = 0x9edcd4u;
    goto L_0067f490;
L_0067f17e:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681d84u + (cm_lang_u32)value * 4u)) {
    case 0x0067f191u: goto L_0067f191;
    case 0x0067f19fu: goto L_0067f19f;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f191:
    destination = local(frame, 0x18u);
    literal = 0x9edcc4u;
    goto L_0067f490;
L_0067f19f:
    destination = local(frame, 0x18u);
    literal = 0x9edcb8u;
    goto L_0067f490;
L_0067f1e1:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681d9cu + (cm_lang_u32)value * 4u)) {
    case 0x0067f1f4u: goto L_0067f1f4;
    case 0x0067f202u: goto L_0067f202;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f1f4:
    destination = local(frame, 0x18u);
    literal = 0x9edca8u;
    goto L_0067f490;
L_0067f202:
    destination = local(frame, 0x18u);
    literal = 0x9edc9cu;
    goto L_0067f490;
L_0067f244:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681db4u + (cm_lang_u32)value * 4u)) {
    case 0x0067f257u: goto L_0067f257;
    case 0x0067f265u: goto L_0067f265;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f257:
    destination = local(frame, 0x18u);
    literal = 0x9edc8cu;
    goto L_0067f490;
L_0067f265:
    destination = local(frame, 0x18u);
    literal = 0x9edc80u;
    goto L_0067f490;
L_0067f2a7:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681dccu + (cm_lang_u32)value * 4u)) {
    case 0x0067f2bau: goto L_0067f2ba;
    case 0x0067f2c8u: goto L_0067f2c8;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f2ba:
    destination = local(frame, 0x18u);
    literal = 0x9edc70u;
    goto L_0067f490;
L_0067f2c8:
    destination = local(frame, 0x18u);
    literal = 0x9edc64u;
    goto L_0067f490;
L_0067f30a:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681de4u + (cm_lang_u32)value * 4u)) {
    case 0x0067f31du: goto L_0067f31d;
    case 0x0067f32bu: goto L_0067f32b;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f31d:
    destination = local(frame, 0x18u);
    literal = 0x9edc50u;
    goto L_0067f490;
L_0067f32b:
    destination = local(frame, 0x18u);
    literal = 0x9edc44u;
    goto L_0067f490;
L_0067f36d:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681dfcu + (cm_lang_u32)value * 4u)) {
    case 0x0067f380u: goto L_0067f380;
    case 0x0067f38eu: goto L_0067f38e;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f380:
    destination = local(frame, 0x18u);
    literal = 0x9edc2cu;
    goto L_0067f490;
L_0067f38e:
    destination = local(frame, 0x18u);
    literal = 0x9edc20u;
    goto L_0067f490;
L_0067f3d0:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681e14u + (cm_lang_u32)value * 4u)) {
    case 0x0067f3e3u: goto L_0067f3e3;
    case 0x0067f3f1u: goto L_0067f3f1;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f3e3:
    destination = local(frame, 0x18u);
    literal = 0x9edc10u;
    goto L_0067f490;
L_0067f3f1:
    destination = local(frame, 0x18u);
    literal = 0x9edc04u;
    goto L_0067f490;
L_0067f433:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681e2cu + (cm_lang_u32)value * 4u)) {
    case 0x0067f442u: goto L_0067f442;
    case 0x0067f44du: goto L_0067f44d;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f442:
    destination = local(frame, 0x18u);
    literal = 0x9edbf4u;
    goto L_0067f490;
L_0067f44d:
    destination = local(frame, 0x18u);
    literal = 0x9edbecu;
    goto L_0067f490;
L_0067f462:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067f487;
    switch (*(const volatile cm_lang_u32 *)(0x681e44u + (cm_lang_u32)value * 4u)) {
    case 0x0067f471u: goto L_0067f471;
    case 0x0067f47cu: goto L_0067f47c;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f471:
    destination = local(frame, 0x18u);
    literal = 0x9edbe4u;
    goto L_0067f490;
L_0067f47c:
    destination = local(frame, 0x18u);
    literal = 0x9edbdcu;
    goto L_0067f490;
L_0067f487:
    destination = local(frame, 0x18u);
    literal = 0x9f1308u;
    goto L_0067f490;
L_0067f490:
    copy_literal(destination, literal);
    goto L_0067f4ad;
L_0067f4ad:
    /* b'no perdiera' */
    if (equal(qualifier, TEXT(0x009edbd0u))) goto L_0067f4e1;
    /* b'comenz\xf3' */
    if (equal(qualifier, TEXT(0x009edba8u))) goto L_0067f544;
    /* b'ganar\xe1' */
    if (equal(qualifier, TEXT(0x009edb88u))) goto L_0067f5a7;
    /* b'reclama' */
    if (equal(qualifier, TEXT(0x009edb6cu))) goto L_0067f60a;
    /* b'pide' */
    if (equal(qualifier, TEXT(0x009edb4cu))) goto L_0067f66d;
    /* b'empata' */
    if (equal(qualifier, TEXT(0x009edb34u))) goto L_0067f6d0;
    /* b'realiza' */
    if (equal(qualifier, TEXT(0x009edb18u))) goto L_0067f733;
    /* b'empiece' */
    if (equal(qualifier, TEXT(0x009edaf8u))) goto L_0067f796;
    /* b'derrotase' */
    if (equal(qualifier, TEXT(0x009edad4u))) goto L_0067f7f9;
    /* b'pueda' */
    if (equal(qualifier, TEXT(0x009edab4u))) goto L_0067f85c;
    /* b'seguir\xe1' */
    if (equal(qualifier, TEXT(0x009eda9cu))) goto L_0067f8bf;
    /* b'sea' */
    if (equal(qualifier, TEXT(0x009eda80u))) goto L_0067f922;
    /* b'sigue' */
    if (equal(qualifier, TEXT(0x009eda68u))) goto L_0067f985;
    /* b'derrotado' */
    if (equal(qualifier, TEXT(0x009eda4cu))) goto L_0067f9e8;
    /* b'frena' */
    if (equal(qualifier, TEXT(0x009eda14u))) goto L_0067fa67;
    /* b'lidera' */
    if (equal(qualifier, TEXT(0x009ed9fcu))) goto L_0067faca;
    /* b'segu\xeda' */
    if (equal(qualifier, TEXT(0x009ed9e0u))) goto L_0067fb2d;
    /* b'aguantaba' */
    if (equal(qualifier, TEXT(0x009ed9c0u))) goto L_0067fb90;
    /* b'suele' */
    if (equal(qualifier, TEXT(0x009ed9a0u))) goto L_0067fbf3;
    /* b'no le permitiese' */
    if (equal(qualifier, TEXT(0x009ed97cu))) goto L_0067fc56;
    /* b'no le aumentase' */
    if (equal(qualifier, TEXT(0x009ed944u))) goto L_0067fcb9;
    if (*(const volatile unsigned char *)qualifier && !*(const volatile cm_lang_u32 *)(frame + 0x14u))
        copy_literal((char *)qualifier, 0x00ad9c64u);
    return compose(frame);
L_0067f4e1:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067fd0e;
    switch (*(const volatile cm_lang_u32 *)(0x681e5cu + (cm_lang_u32)value * 4u)) {
    case 0x0067f4f4u: goto L_0067f4f4;
    case 0x0067f502u: goto L_0067f502;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f4f4:
    destination = local(frame, 0x18u);
    literal = 0x9edbc0u;
    goto L_0067fd17;
L_0067f502:
    destination = local(frame, 0x18u);
    literal = 0x9edbb0u;
    goto L_0067fcf4;
L_0067f544:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067fd0e;
    switch (*(const volatile cm_lang_u32 *)(0x681e74u + (cm_lang_u32)value * 4u)) {
    case 0x0067f557u: goto L_0067f557;
    case 0x0067f565u: goto L_0067f565;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f557:
    destination = local(frame, 0x18u);
    literal = 0x9edb9cu;
    goto L_0067fd17;
L_0067f565:
    destination = local(frame, 0x18u);
    literal = 0x9edb90u;
    goto L_0067fcf4;
L_0067f5a7:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067fd0e;
    switch (*(const volatile cm_lang_u32 *)(0x681e8cu + (cm_lang_u32)value * 4u)) {
    case 0x0067f5bau: goto L_0067f5ba;
    case 0x0067f5c8u: goto L_0067f5c8;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f5ba:
    destination = local(frame, 0x18u);
    literal = 0x9edb80u;
    goto L_0067fd17;
L_0067f5c8:
    destination = local(frame, 0x18u);
    literal = 0x9edb74u;
    goto L_0067fcf4;
L_0067f60a:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067fd0e;
    switch (*(const volatile cm_lang_u32 *)(0x681ea4u + (cm_lang_u32)value * 4u)) {
    case 0x0067f61du: goto L_0067f61d;
    case 0x0067f62bu: goto L_0067f62b;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f61d:
    destination = local(frame, 0x18u);
    literal = 0x9edb60u;
    goto L_0067fd17;
L_0067f62b:
    destination = local(frame, 0x18u);
    literal = 0x9edb54u;
    goto L_0067fcf4;
L_0067f66d:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067fd0e;
    switch (*(const volatile cm_lang_u32 *)(0x681ebcu + (cm_lang_u32)value * 4u)) {
    case 0x0067f680u: goto L_0067f680;
    case 0x0067f68eu: goto L_0067f68e;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f680:
    destination = local(frame, 0x18u);
    literal = 0x9edb44u;
    goto L_0067fd17;
L_0067f68e:
    destination = local(frame, 0x18u);
    literal = 0x9edb3cu;
    goto L_0067fcf4;
L_0067f6d0:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067fd0e;
    switch (*(const volatile cm_lang_u32 *)(0x681ed4u + (cm_lang_u32)value * 4u)) {
    case 0x0067f6e3u: goto L_0067f6e3;
    case 0x0067f6f1u: goto L_0067f6f1;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f6e3:
    destination = local(frame, 0x18u);
    literal = 0x9edb2cu;
    goto L_0067fd17;
L_0067f6f1:
    destination = local(frame, 0x18u);
    literal = 0x9edb20u;
    goto L_0067fcf4;
L_0067f733:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067fd0e;
    switch (*(const volatile cm_lang_u32 *)(0x681eecu + (cm_lang_u32)value * 4u)) {
    case 0x0067f746u: goto L_0067f746;
    case 0x0067f754u: goto L_0067f754;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f746:
    destination = local(frame, 0x18u);
    literal = 0x9edb0cu;
    goto L_0067fd17;
L_0067f754:
    destination = local(frame, 0x18u);
    literal = 0x9edb00u;
    goto L_0067fcf4;
L_0067f796:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067fd0e;
    switch (*(const volatile cm_lang_u32 *)(0x681f04u + (cm_lang_u32)value * 4u)) {
    case 0x0067f7a9u: goto L_0067f7a9;
    case 0x0067f7b7u: goto L_0067f7b7;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f7a9:
    destination = local(frame, 0x18u);
    literal = 0x9edaecu;
    goto L_0067fd17;
L_0067f7b7:
    destination = local(frame, 0x18u);
    literal = 0x9edae0u;
    goto L_0067fcf4;
L_0067f7f9:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067fd0e;
    switch (*(const volatile cm_lang_u32 *)(0x681f1cu + (cm_lang_u32)value * 4u)) {
    case 0x0067f80cu: goto L_0067f80c;
    case 0x0067f81au: goto L_0067f81a;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f80c:
    destination = local(frame, 0x18u);
    literal = 0x9edac8u;
    goto L_0067fd17;
L_0067f81a:
    destination = local(frame, 0x18u);
    literal = 0x9edabcu;
    goto L_0067fcf4;
L_0067f85c:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067fd0e;
    switch (*(const volatile cm_lang_u32 *)(0x681f34u + (cm_lang_u32)value * 4u)) {
    case 0x0067f86fu: goto L_0067f86f;
    case 0x0067f87du: goto L_0067f87d;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f86f:
    destination = local(frame, 0x18u);
    literal = 0x9edaacu;
    goto L_0067fd17;
L_0067f87d:
    destination = local(frame, 0x18u);
    literal = 0x9edaa4u;
    goto L_0067fcf4;
L_0067f8bf:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067fd0e;
    switch (*(const volatile cm_lang_u32 *)(0x681f4cu + (cm_lang_u32)value * 4u)) {
    case 0x0067f8d2u: goto L_0067f8d2;
    case 0x0067f8e0u: goto L_0067f8e0;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f8d2:
    destination = local(frame, 0x18u);
    literal = 0x9eda90u;
    goto L_0067fd17;
L_0067f8e0:
    destination = local(frame, 0x18u);
    literal = 0x9eda84u;
    goto L_0067fcf4;
L_0067f922:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067fd0e;
    switch (*(const volatile cm_lang_u32 *)(0x681f64u + (cm_lang_u32)value * 4u)) {
    case 0x0067f935u: goto L_0067f935;
    case 0x0067f943u: goto L_0067f943;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f935:
    destination = local(frame, 0x18u);
    literal = 0x9eda78u;
    goto L_0067fd17;
L_0067f943:
    destination = local(frame, 0x18u);
    literal = 0x9eda70u;
    goto L_0067fcf4;
L_0067f985:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067fd0e;
    switch (*(const volatile cm_lang_u32 *)(0x681f7cu + (cm_lang_u32)value * 4u)) {
    case 0x0067f998u: goto L_0067f998;
    case 0x0067f9a6u: goto L_0067f9a6;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f998:
    destination = local(frame, 0x18u);
    literal = 0x9eda60u;
    goto L_0067fd17;
L_0067f9a6:
    destination = local(frame, 0x18u);
    literal = 0x9eda58u;
    goto L_0067fcf4;
L_0067f9e8:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067fd0e;
    switch (*(const volatile cm_lang_u32 *)(0x681f94u + (cm_lang_u32)value * 4u)) {
    case 0x0067f9fbu: goto L_0067f9fb;
    case 0x0067fa09u: goto L_0067fa09;
    case 0x0067fa17u: goto L_0067fa17;
    case 0x0067fa25u: goto L_0067fa25;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067f9fb:
    destination = local(frame, 0x18u);
    literal = 0x9eda40u;
    goto L_0067fd17;
L_0067fa09:
    destination = local(frame, 0x18u);
    literal = 0x9eda34u;
    goto L_0067fd17;
L_0067fa17:
    destination = local(frame, 0x18u);
    literal = 0x9eda28u;
    goto L_0067fd17;
L_0067fa25:
    destination = local(frame, 0x18u);
    literal = 0x9eda1cu;
    goto L_0067fcf4;
L_0067fa67:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067fd0e;
    switch (*(const volatile cm_lang_u32 *)(0x681facu + (cm_lang_u32)value * 4u)) {
    case 0x0067fa7au: goto L_0067fa7a;
    case 0x0067fa88u: goto L_0067fa88;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067fa7a:
    destination = local(frame, 0x18u);
    literal = 0x9eda0cu;
    goto L_0067fd17;
L_0067fa88:
    destination = local(frame, 0x18u);
    literal = 0x9eda04u;
    goto L_0067fcf4;
L_0067faca:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067fd0e;
    switch (*(const volatile cm_lang_u32 *)(0x681fc4u + (cm_lang_u32)value * 4u)) {
    case 0x0067faddu: goto L_0067fadd;
    case 0x0067faebu: goto L_0067faeb;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067fadd:
    destination = local(frame, 0x18u);
    literal = 0x9ed9f4u;
    goto L_0067fd17;
L_0067faeb:
    destination = local(frame, 0x18u);
    literal = 0x9ed9e8u;
    goto L_0067fcf4;
L_0067fb2d:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067fd0e;
    switch (*(const volatile cm_lang_u32 *)(0x681fdcu + (cm_lang_u32)value * 4u)) {
    case 0x0067fb40u: goto L_0067fb40;
    case 0x0067fb4eu: goto L_0067fb4e;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067fb40:
    destination = local(frame, 0x18u);
    literal = 0x9ed9d8u;
    goto L_0067fd17;
L_0067fb4e:
    destination = local(frame, 0x18u);
    literal = 0x9ed9ccu;
    goto L_0067fcf4;
L_0067fb90:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067fd0e;
    switch (*(const volatile cm_lang_u32 *)(0x681ff4u + (cm_lang_u32)value * 4u)) {
    case 0x0067fba3u: goto L_0067fba3;
    case 0x0067fbb1u: goto L_0067fbb1;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067fba3:
    destination = local(frame, 0x18u);
    literal = 0x9ed9b4u;
    goto L_0067fd17;
L_0067fbb1:
    destination = local(frame, 0x18u);
    literal = 0x9ed9a8u;
    goto L_0067fcf4;
L_0067fbf3:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067fd0e;
    switch (*(const volatile cm_lang_u32 *)(0x68200cu + (cm_lang_u32)value * 4u)) {
    case 0x0067fc06u: goto L_0067fc06;
    case 0x0067fc14u: goto L_0067fc14;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067fc06:
    destination = local(frame, 0x18u);
    literal = 0x9ed998u;
    goto L_0067fd17;
L_0067fc14:
    destination = local(frame, 0x18u);
    literal = 0x9ed990u;
    goto L_0067fcf4;
L_0067fc56:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067fd0e;
    switch (*(const volatile cm_lang_u32 *)(0x682024u + (cm_lang_u32)value * 4u)) {
    case 0x0067fc69u: goto L_0067fc69;
    case 0x0067fc77u: goto L_0067fc77;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067fc69:
    destination = local(frame, 0x18u);
    literal = 0x9ed968u;
    goto L_0067fd17;
L_0067fc77:
    destination = local(frame, 0x18u);
    literal = 0x9ed954u;
    goto L_0067fd17;
L_0067fcb9:
    value = property2;
    if ((cm_lang_u32)value > 5) goto L_0067fd0e;
    switch (*(const volatile cm_lang_u32 *)(0x68203cu + (cm_lang_u32)value * 4u)) {
    case 0x0067fcc8u: goto L_0067fcc8;
    case 0x0067fcebu: goto L_0067fceb;
    default: __builtin_trap(); /* Outside immutable pinned-table contract. */
    }
L_0067fcc8:
    literal = 0x9ed930u;
    destination = local(frame, 0x18u);
    copy_literal(destination, literal);
    return compose(frame);
L_0067fceb:
    destination = local(frame, 0x18u);
    literal = 0x9ed91cu;
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
