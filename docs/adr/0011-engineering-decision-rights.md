# ADR 0011: Engineering decision rights

Status: accepted (2026-11-19). Adam, the project owner, set the roles; this
record writes them into the rules.

## Context

Adam's instruction (2026-11-19): treat him as the product manager, not the
engineering manager. Agents make the engineering calls, and change the rules
that stop them working effectively.

Before this, the rules routed engineering decisions through him. Harness
releases waited for his review before merging, ADRs were "chosen by the
project owner", and full revalidations needed his request. On 19 Nov the
harness-blocker and evidence-needed lanes held about 280 KB of parked work.
Clearing most of it takes harness releases, so his review sat on the
critical path of the largest backlog.

## Decision

**Adam decides product questions:**

- Goals, scope and milestones.
- Spending money, such as costlier models or paid services.
- Use of his own computer and desktop, such as driving the game window.
- What may be published. The publication and privacy rules in `AGENTS.md`
  stay in force.

**Agents decide and ship engineering questions:**

- Harness releases and the gate policies they implement.
- Tooling, workflow and documentation rules.
- ADRs, and when to run revalidation and composition jobs.

An agent records each decision with its evidence. An ADR is needed only when
the decision changes what the gate accepts or how evidence is bound.

**Safeguards replace Adam's review:**

1. **Fidelity rules.** The fidelity rules in `AGENTS.md` are product
   requirements. No engineering decision may weaken them.
2. **Independent review.** A gate-policy change or harness release needs an
   independent review before it merges to `main`. Use a `cm-evidence-reviewer`
   agent, or a separate session that did not write the change. The reviewer
   checks the change against the fidelity rules and the ADR, and the release
   record keeps the review.
3. **Mechanical evidence.** The change needs:
   - focused tests that were written first and seen failing;
   - the [validation scope](../agent/HARNESS.md#validation-scope) receipt;
   - one full pytest and `cmre verify` before merge.

   A release that changes the verdict of an already accepted function names
   each such function in its record. It does not merge until each one is
   re-accepted or retracted through `cmre`.
4. **Tell Adam afterwards.** The session handoff reports each policy change in
   plain language: what changed and why. This is a report, not a request for
   approval.

ADR 0007's batching rules stay as they are: a worktree, at most one release
per working day, and a release applied at a batch boundary. Its review step is
the independent review above.

## Consequences

- Harness families get released at the pace evidence allows, with no wait
  for a person.
- If a reviewer and an author disagree, the stricter reading of the fidelity
  rules wins until new evidence settles it. Neither escalates to Adam.
- Earlier ADRs keep "chosen by the project owner" as historical fact. Later
  amendments to them follow this ADR.
