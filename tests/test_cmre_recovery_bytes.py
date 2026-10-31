"""Accepted recovered C is stored byte-for-byte, so published bytes equal the accepted hash."""
import subprocess

import pytest

from cmre import env


def _attributes(path):
    out = subprocess.run(
        ["git", "check-attr", "text", "eol", "--", path],
        cwd=env.ROOT, capture_output=True, text=True, check=True).stdout
    return {line.split(": ", 2)[1]: line.split(": ", 2)[2] for line in out.splitlines()}


def test_recovered_c_is_not_line_ending_normalised():
    attrs = _attributes("recovery/c/fn_00401000.c")
    assert attrs["text"] == "unset", attrs
    assert attrs["eol"] != "lf", attrs


def test_crlf_accepted_source_keeps_its_bytes():
    attrs = _attributes("recovery/c/fn_00945956.c")
    assert attrs["text"] == "unset", attrs


@pytest.mark.parametrize("path", [
    "recovery/lang/cm_lang_morph.c",
    "recovery/crt/cm_crt_search.h",
    "recovery/date/cm_date.c",
    "recovery/rng/cm_rng.c",
])
def test_legacy_sources_keep_their_bytes(path):
    attrs = _attributes(path)
    assert attrs["text"] == "unset", attrs
