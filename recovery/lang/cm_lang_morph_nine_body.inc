/* Primary VA 0067B50F language-nine body. Original table values select
 * verified literal actions; property100 selector clears one prefix byte. */
typedef struct { cm_lang_u32 key, table; } nine_rule;
static const nine_rule nine_prefix_rules[] = {
    { 0x009eee10u, 0x006810f8u },
    { 0x0098e8ccu, 0x0068110cu },
    { 0x009eedd4u, 0x00681244u },
    { 0x0099c35cu, 0x00681244u },
    { 0x009eedd0u, 0x00681120u },
    { 0x009eedc0u, 0x00000000u },
    { 0x009eedbcu, 0x00000000u },
    { 0x009eedb8u, 0x00681134u },
    { 0x009eeda0u, 0x00681148u },
    { 0x009eed80u, 0x0068115cu },
    { 0x009eed68u, 0x00681170u },
    { 0x009eed3cu, 0x00681184u },
    { 0x009eed1cu, 0x006811acu },
    { 0x009eed18u, 0x006811acu },
    { 0x009eed10u, 0x00681198u },
};
static const nine_rule nine_suffix_rules[] = {
    { 0x009eecb0u, 0x00681258u },
    { 0x009eec74u, 0x0068126cu },
    { 0x009eec34u, 0x00681280u },
    { 0x009eebdcu, 0x00681294u },
    { 0x009eeb90u, 0x006812a8u },
    { 0x009eeb4cu, 0x006812bcu },
    { 0x009eeae4u, 0x006812d0u },
    { 0x009eea88u, 0x006812e4u },
};
static cm_lang_u32 nine_literal(cm_lang_u32 target) {
    switch (target) {
    case 0x0067b624u: return 0x009eee0cu;
    case 0x0067b635u: return 0x009eee08u;
    case 0x0067b646u: return 0x009eee04u;
    case 0x0067b657u: return 0x009eedfcu;
    case 0x0067b668u: return 0x009eedf4u;
    case 0x0067b6c9u: return 0x009eedf0u;
    case 0x0067b6dau: return 0x009eedecu;
    case 0x0067b6ebu: return 0x009eede8u;
    case 0x0067b6fcu: return 0x009eede0u;
    case 0x0067b70du: return 0x009eedd8u;
    case 0x0067b7e6u: return 0x009eedccu;
    case 0x0067b7f7u: return 0x009f15c8u;
    case 0x0067b808u: return 0x009eedc8u;
    case 0x0067b819u: return 0x009eedc4u;
    case 0x0067b8f2u: return 0x009eedb4u;
    case 0x0067b903u: return 0x009eedb0u;
    case 0x0067b914u: return 0x009eedacu;
    case 0x0067b925u: return 0x009eeda4u;
    case 0x0067b936u: return 0x009f07ccu;
    case 0x0067b997u: return 0x009eed9cu;
    case 0x0067b9a8u: return 0x009eed98u;
    case 0x0067b9b9u: return 0x009eed94u;
    case 0x0067b9cau: return 0x009eed8cu;
    case 0x0067b9dbu: return 0x009eed84u;
    case 0x0067ba3cu: return 0x009f15d4u;
    case 0x0067ba4du: return 0x009eed7cu;
    case 0x0067ba5eu: return 0x009f1360u;
    case 0x0067ba6fu: return 0x009eed74u;
    case 0x0067ba80u: return 0x009eed70u;
    case 0x0067bae1u: return 0x009eed60u;
    case 0x0067baf2u: return 0x009eed58u;
    case 0x0067bb03u: return 0x009eed50u;
    case 0x0067bb14u: return 0x009eed48u;
    case 0x0067bb25u: return 0x009eed40u;
    case 0x0067bb86u: return 0x009eed38u;
    case 0x0067bb97u: return 0x009eed30u;
    case 0x0067bba8u: return 0x009eed28u;
    case 0x0067bbb9u: return 0x009eed20u;
    case 0x0067bc92u: return 0x009eed08u;
    case 0x0067bca3u: return 0x009eed00u;
    case 0x0067bcb4u: return 0x009eecf8u;
    case 0x0067bcc5u: return 0x009eecf0u;
    case 0x0067bd1au: return 0x009eececu;
    case 0x0067bd2bu: return 0x009eece4u;
    case 0x0067bd3cu: return 0x009eecdcu;
    case 0x0067bd4du: return 0x009eecd4u;
    case 0x0067bd7eu: return 0x009f14c0u;
    case 0x0067bd8fu: return 0x009eecd0u;
    case 0x0067bda0u: return 0x009f0e58u;
    case 0x0067bdaeu: return 0x009eecc8u;
    case 0x0067bdbcu: return 0x009f07b0u;
    case 0x0067bde8u: return 0x00ad9c64u;
    case 0x0067bdf6u: return 0x009eecc4u;
    case 0x0067be04u: return 0x009eecc0u;
    case 0x0067be12u: return 0x009eecbcu;
    case 0x0067be20u: return 0x009f1308u;
    case 0x0067bee6u: return 0x009eeca4u;
    case 0x0067bef4u: return 0x009eec98u;
    case 0x0067bf02u: return 0x009eec8cu;
    case 0x0067bf10u: return 0x009eec80u;
    case 0x0067bf6eu: return 0x009eec68u;
    case 0x0067bf7cu: return 0x009eec5cu;
    case 0x0067bf8au: return 0x009eec50u;
    case 0x0067bf98u: return 0x009eec44u;
    case 0x0067bff6u: return 0x009eec24u;
    case 0x0067c004u: return 0x009eec14u;
    case 0x0067c012u: return 0x009eec00u;
    case 0x0067c020u: return 0x009eebecu;
    case 0x0067c07eu: return 0x009eebccu;
    case 0x0067c08cu: return 0x009eebbcu;
    case 0x0067c09au: return 0x009eebacu;
    case 0x0067c0a8u: return 0x009eeb9cu;
    case 0x0067c106u: return 0x009eeb84u;
    case 0x0067c114u: return 0x009eeb78u;
    case 0x0067c122u: return 0x009eeb6cu;
    case 0x0067c130u: return 0x009eeb60u;
    case 0x0067c18eu: return 0x009eeb38u;
    case 0x0067c19cu: return 0x009eeb24u;
    case 0x0067c1aau: return 0x009eeb0cu;
    case 0x0067c1b8u: return 0x009eeaf4u;
    case 0x0067c216u: return 0x009eead4u;
    case 0x0067c224u: return 0x009eeac4u;
    case 0x0067c232u: return 0x009eeab0u;
    case 0x0067c240u: return 0x009eea9cu;
    case 0x0067c29eu: return 0x009eea74u;
    case 0x0067c2acu: return 0x009eea60u;
    case 0x0067c2bau: return 0x009eea4cu;
    case 0x0067c2c8u: return 0x009eea38u;
    default: __builtin_trap(); /* Outside pinned original-table target contract. */
    }
}
static cm_lang_u32 nine_property(unsigned char *frame, cm_lang_u32 offset, cm_lang_u32 table) {
    cm_lang_u32 value = (cm_lang_u32)signed_byte(*(const volatile unsigned char *)(frame + offset));
    return value > 4u ? 0x009f1308u : nine_literal(*(const volatile cm_lang_u32 *)(table + 4u * value));
}
char *CM_LANG_CDECL cm_lang_morph_language_nine(unsigned char *frame) {
    const char *prefix = argument(frame, 0x2820u);
    char *word = local(frame, 0x1018u);
    if (equal(prefix, TEXT(0x009f15ecu)))
        *(volatile char *)word = (char)UPPER(signed_byte(*(const volatile unsigned char *)word));
    else if (equal(prefix, TEXT(0x009f15c0u))) lower_word_in_place(word);
    else {
        cm_lang_u32 matched = 0;
        for (cm_lang_u32 n = 0; n < sizeof(nine_prefix_rules) / sizeof(nine_prefix_rules[0]); ++n) {
            const nine_rule *rule = &nine_prefix_rules[n];
            if (!equal(prefix, TEXT(rule->key))) continue;
            if (rule->table) copy_literal(local(frame, 0x818u), nine_property(frame, 0x2828u, rule->table));
            else {
                cm_lang_u32 value = (cm_lang_u32)signed_byte(*(const volatile unsigned char *)(frame + 0x2828u));
                if (value > 100u) copy_literal(local(frame, 0x818u), 0x009f1308u);
                else {
                    cm_lang_u32 target = *(const volatile cm_lang_u32 *)(0x006811c0u +
                        4u * *(const volatile unsigned char *)(0x006811dcu + value));
                    if (target == 0x0067bdcau) *(volatile char *)local(frame, 0x818u) = 0;
                    else copy_literal(local(frame, 0x818u), nine_literal(target));
                }
            }
            matched = 1; break;
        }
        if (!matched && *(const volatile unsigned char *)prefix) copy_literal((char *)prefix, 0x00ad9c64u);
    }
    const char *suffix = argument(frame, 0x2824u);
    if (equal(suffix, TEXT(0x0099c368u))) copy_literal(local(frame, 0x18u), 0x00ad9c64u);
    else {
        cm_lang_u32 matched = 0;
        for (cm_lang_u32 n = 0; n < sizeof(nine_suffix_rules) / sizeof(nine_suffix_rules[0]); ++n) {
            const nine_rule *rule = &nine_suffix_rules[n];
            if (!equal(suffix, TEXT(rule->key))) continue;
            copy_literal(local(frame, 0x18u), nine_property(frame, 0x282cu, rule->table));
            matched = 1; break;
        }
        if (!matched && *(const volatile unsigned char *)suffix) copy_literal((char *)suffix, 0x00ad9c64u);
    }
    return compose(frame);
}
