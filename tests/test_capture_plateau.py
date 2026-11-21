"""Recorder plateau stop: end a capture round once no new target function is reached."""
from cmre import capture_record


def test_plateau_fires_only_after_the_quiet_period():
    watch = capture_record.Plateau(1800, start=0.0)
    assert not watch.update(10.0, reached=5)
    assert not watch.update(1700.0, reached=5)
    assert not watch.update(1809.0, reached=5)
    assert watch.update(1810.0, reached=5)


def test_any_new_function_restarts_the_quiet_period():
    watch = capture_record.Plateau(1800, start=0.0)
    watch.update(1000.0, reached=5)
    assert not watch.update(1500.0, reached=6)      # growth at 1500
    assert not watch.update(3200.0, reached=6)
    assert watch.update(3300.0, reached=6)


def test_zero_or_none_disables_the_stop():
    for seconds in (0, None):
        watch = capture_record.Plateau(seconds, start=0.0)
        assert not watch.update(10 ** 9, reached=1)


def test_stopped_reason_reaches_the_summary_status():
    watch = capture_record.Plateau(60, start=0.0)
    watch.update(61.0, reached=0)
    assert watch.fired and watch.reason() == "plateau: no new target function for 1 min"
