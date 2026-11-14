"""Same-provider stages must not run concurrently.

Two OpenCode clients for one provider share that provider's client state
database; the 2026-11-10 campaign observed ``database is locked`` when two
MiniMax author stages were dispatched together.
"""
from __future__ import annotations

from cmre import provider_campaign as pc


def job(job_id, owner, state="prepared", reviewer="deepseek"):
    return {"id": job_id, "state": state,
            "config": {"owner": owner, "reviewer": reviewer}}


def test_one_stage_per_provider_role_even_with_higher_concurrency():
    candidates = [job("a", "minimax"), job("b", "minimax"), job("c", "minimax")]
    batch = pc._dispatch_batch(candidates, 2)
    assert [item["id"] for item in batch] == ["a"]


def test_author_and_independent_reviewer_may_run_together():
    candidates = [job("a", "minimax"), job("b", "minimax", state="needs-review",
                                           reviewer="deepseek")]
    batch = pc._dispatch_batch(candidates, 2)
    assert [item["id"] for item in batch] == ["b", "a"]  # review drains first


def test_review_stage_uses_the_reviewer_role():
    candidates = [job("a", "minimax", state="needs-review", reviewer="deepseek"),
                  job("b", "minimax")]
    batch = pc._dispatch_batch(candidates, 2)
    assert [item["id"] for item in batch] == ["a", "b"]


def test_concurrency_one_keeps_first_candidate():
    batch = pc._dispatch_batch([job("a", "minimax"), job("b", "deepseek")], 1)
    assert [item["id"] for item in batch] == ["a"]
