"""Tooling names for the capture store (ADR 0009).

The store code is in `difftest.py`, a harness module, so the harness identity
covers every byte a test reads. This module only gives the recorder, the
expander and the CLI short names for it. Recording lives in
`capture_record.py` and offline expansion in `capture_expand.py`.
"""
from __future__ import annotations

from . import emu
from .difftest import CAPTURE_FORMAT as FORMAT
from .difftest import CAPTURE_REG_NAMES as REG_NAMES
from .difftest import CAPTURE_STACK_WORDS as STACK_WORDS
from .difftest import Capture
from .difftest import capture_names as names_for
from .difftest import capture_path as path_for
from .difftest import capture_store as store_dir
from .difftest import load_capture as load
from .difftest import save_capture as save
from .difftest import valid_capture_name as valid_name

PAGE = emu.PAGE

__all__ = ["FORMAT", "PAGE", "REG_NAMES", "STACK_WORDS", "Capture", "load", "names_for", "path_for",
           "save", "store_dir", "valid_name"]
