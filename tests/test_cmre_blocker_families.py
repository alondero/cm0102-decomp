from cmre import work


def test_blocker_families_group_holds_and_keep_retry_conditions():
    records = {
        'fn_00401000': {'state': 'evidence-needed', 'blocker': 'x87', 'retry_when': 'proof'},
        'fn_00402000': {'state': 'harness-blocker', 'blocker': 'x87', 'retry_when': 'release'},
        'fn_00403000': {'state': 'needs-review', 'blocker': '', 'retry_when': ''},
    }
    families = work.blocker_families(records)
    assert families['x87']['functions'] == ['fn_00401000', 'fn_00402000']
    assert families['x87']['retry_conditions'] == ['proof', 'release']
    assert families['unclassified']['functions'] == ['fn_00403000']
