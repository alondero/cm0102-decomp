"""Replenish finite provider queues from actual work state, never from band totals.

This prepares private jobs and an adoption inbox. It does not clear holds,
adopt fixtures, or infer source approval from a passing differential test.
"""
from collections import Counter, defaultdict, deque
from pathlib import Path
import hashlib
import json
import time
import random
import struct

STAGES = ('intake', 'author', 'reconcile', 'repair', 'capability', 'review', 'owned')


def classify(*, lifted, source, record):
    state = record.get('state')
    if state == 'harness-blocker':
        return 'capability'
    if not lifted or state == 'evidence-needed':
        return 'intake'
    if source:
        if state == 'ready':
            return 'author'
        return 'repair' if state == 'translation-defect' else 'reconcile'
    return 'author'


def needs_helper_intake(record, current_identity):
    """An explicitly reconciled, unchanged input obligation can feed authors."""
    return not (record.get('state') == 'ready' and record.get('reason', '').strip()
                and record.get('identity') == current_identity)


def task_queues(manifests, now):
    """Observe task ownership without exposing tokens or stealing a submission."""
    queues = {}
    for manifest in manifests:
        stage = None
        if manifest['status'] == 'submitted':
            stage = 'review'
        elif (manifest.get('lease') or {}).get('expires_at', 0) > now or any(
                attempt.get('status') == 'testing' for attempt in manifest.get('attempts', [])):
            stage = 'owned'
        if stage:
            queues[int(manifest['va'], 16)] = {'stage': stage, 'task_id': manifest['task_id']}
    return queues


def attempt_counts(manifests, states):
    """Same durable attempts/nonduplicated failures as the task API, in one scan."""
    counts = Counter({int(fid[3:], 16): record.get('failures', 0)
                      for fid, record in states.items()})
    for manifest in manifests:
        va = int(manifest['va'], 16)
        events = states.get(f'fn_{va:08x}', {}).get('report_events', {})
        for attempt in manifest.get('attempts', []):
            event = f"{manifest['task_id']}:{attempt['number']}"
            counts[va] += 1 - (event in events and events[event] != 'pass')
    return counts


def snapshot():
    from . import env, inventory, ledger, lift, queue, tasks, work
    records = {e.va: e.record for e in ledger.cmre_entries()}
    legacy = {e.va for e in ledger.legacy_entries()}
    states = work.read()['functions']
    manifests = tasks.status()
    owned = task_queues(manifests, time.time())
    attempts = attempt_counts(manifests, states)
    shared_claims = work.claims()
    rows = []
    for va, function in inventory.load().functions.items():
        record = records.get(va, {})
        if va in legacy or record.get('c') or queue.compiler_generated(va):
            continue
        fid = f'fn_{va:08x}'
        source = env.C_DIR / (fid + '.c')
        hint = env.HINTS_DIR / (fid + '.json')
        state = states.get(fid, {})
        stage = classify(lifted=bool(record.get('lifted')), source=source.is_file(), record=state)
        if va in owned:
            stage = owned[va]['stage']
        if fid in shared_claims:
            stage = 'owned'
        reason, retry = state.get('reason', ''), state.get('retry_when', '')
        blocker = state.get('blocker') or 'unclassified'
        if stage != 'owned' and stage != 'review' and attempts[va] >= tasks.MAX_ATTEMPTS:
            stage, blocker = 'capability', 'attempt-budget-exhausted'
            reason = 'The durable C attempt allowance is exhausted; automatic input/source retries are unavailable.'
            retry = 'Retain all attempts; explicit coordinator investigation is required without resetting counters.'
        if stage == 'author' and not source.is_file():
            if (0x00945640 in lift.discover(va).callees
                    and needs_helper_intake(state, work.identity(va))):
                stage, blocker = 'intake', 'registered-helper-controls'
                reason += ' Registered __ftol sites need evidenced computation sensitivity before acceptance.'
                retry = 'Establish original site/value witnesses and a named source-site control obligation under ADR0006.'
        rows.append({'function': fid, 'va': f'{va:#010x}',
                     'body_bytes': function.body_bytes,
                     'stage': stage,
                     'state': state.get('state', 'unrecorded'),
                     'blocker': blocker,
                     'reason': reason,
                     'retry_when': retry,
                     'source': str(source) if source.is_file() else None,
                     'hints': str(hint) if hint.is_file() else None,
                     'task_id': owned.get(va, {}).get('task_id'),
                     'dispatch_hold': work.dispatch_hold(va, states)})
        rows[-1]['work_record'] = states.get(fid)
        rows[-1]['input_identity'] = work.identity(va)
    return rows


def select(rows, *, limit=48, buffer_bytes=100000):
    """Spread intake across blocker families, keep downstream candidates supplied."""
    if limit < 1 or buffer_bytes < 1:
        raise ValueError('Queue limit and byte buffer must be positive')
    groups = defaultdict(list)
    for row in rows:
        groups[(row['stage'], row['blocker'])].append(row)
    for values in groups.values():
        # Bounded medium bodies avoid both tiny-leaf starvation and dispatching
        # every enormous root before a representative can establish progress.
        values.sort(key=lambda r: (0 if 1024 <= r['body_bytes'] <= 8000 else 1,
                                   -r['body_bytes'], r['function']))
    selected, totals = [], Counter()
    keys = sorted(groups, key=lambda k: (STAGES.index(k[0]),
                                        -sum(r['body_bytes'] for r in groups[k])))
    rotation = {stage: deque(k for k in keys if k[0] == stage) for stage in STAGES}
    # Give every nonempty stage a first turn, then refill until its byte watermark.
    while len(selected) < limit:
        changed = False
        for stage in ('author', 'reconcile', 'repair', 'intake'):
            for _ in range(len(rotation[stage])):
                key = rotation[stage].popleft()
                rotation[stage].append(key)
                if not groups[key] or totals[stage] >= buffer_bytes:
                    continue
                row = groups[key].pop(0)
                selected.append(row)
                totals[stage] += row['body_bytes']
                changed = True
                if len(selected) >= limit:
                    break
                break
            if len(selected) >= limit:
                break
        if not changed:
            break
    return selected


def _write(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2, allow_nan=False) + '\n', encoding='utf-8')


def pin(path):
    path = Path(path).resolve(strict=True)
    return {'path': str(path), 'sha256': hashlib.sha256(path.read_bytes()).hexdigest()}


def native_packet(va, path):
    """Root, every indexed caller window and explicit callee contract evidence."""
    from . import env, inventory, lift
    iv = inventory.load()
    root = lift.discover(va)
    callers = sorted(caller for caller, callees in iv.callees.items() if va in callees)
    lines = [f'CM0102 3.9.68 SHA256 {env.TARGET_SHA256}',
             'Addresses: preferred-image VA, image base 0x00400000.',
             f'Decoded root {va:#010x}; discovery problems: {root.problems!r}',
             'Native bytes and instructions are evidence; source signatures remain unproved.']

    def render(insn):
        return f'{insn.va:08x} {insn.raw.hex()} {insn.mnemonic} {insn.op_str}'

    lines.extend(render(root.insns[pc]) for pc in sorted(root.insns))
    lines.append('Original callees: ' + ', '.join(f'{c:#010x}' for c in sorted(root.callees)))
    lines.append('Indexed direct callers: ' + ', '.join(f'{c:#010x}' for c in callers))
    lines.append('Caller windows below cover every indexed caller, 8 instructions before and '
                 '12 after each direct call; indirect callers and remaining bodies are not reviewed.')
    for caller in callers:
        decoded = lift.discover(caller)
        insns = [decoded.insns[pc] for pc in sorted(decoded.insns)]
        indices = set()
        for i, insn in enumerate(insns):
            if insn.target == va:
                indices.update(range(max(0, i-8), min(len(insns), i+13)))
        lines.append(f'Indexed caller {caller:#010x}:')
        lines.extend(render(insns[i]) for i in sorted(indices))
    for callee in sorted(root.callees):
        if callee not in iv.functions:
            lines.append(f'Callee {callee:#010x}: outside function inventory; contract unknown.')
            continue
        decoded = lift.discover(callee)
        insns = [decoded.insns[pc] for pc in sorted(decoded.insns)]
        complete = len(insns) <= 160
        lines.append(f'Callee {callee:#010x}: ' + ('complete decode' if complete else
                     'first 32 instructions and return sites only; remaining body unreviewed') +
                     f'; discovery problems {decoded.problems!r}')
        lines.extend(render(i) for n, i in enumerate(insns)
                     if complete or n < 32 or i.kind == 'ret')
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text('\n'.join(lines) + '\n', encoding='utf-8')
    return pin(path)


def diagnostic_seed(va):
    """Serialize one core-generator sample, not assumed game initialization.

    This is a diagnostic sample with an empty discovered-global set, not a
    replay claim for case zero of the full differential corpus.
    """
    from . import difftest
    from .recover import C_INPUTS
    opts = difftest.Options.for_function(va, tier='c')
    opts.inputs = C_INPUTS
    templates = list(difftest.arena_templates())
    case = difftest.make_case(random.Random(opts.seed), opts, [], templates)
    padded = [t + bytes(len(case.arena) - len(t)) for t in templates]
    actual = memoryview(case.arena).cast('I')
    index = min(range(len(padded)), key=lambda i: sum(
        a != b for a, b in zip(actual, memoryview(padded[i]).cast('I'))))
    reference = memoryview(padded[index]).cast('I')
    fixture = {'regs': case.regs, 'stack': case.stack, 'arena_template': index,
               'arena_writes': {f'{i*4:#x}': struct.unpack_from('<I', case.arena, i*4)[0]
                                for i, (a, b) in enumerate(zip(actual, reference)) if a != b},
               'globals': {f'{pc:#x}': value for pc, value in case.globals.items()},
               'import_seed': case.import_seed}
    if case.fpcw is not None:
        fixture['fpcw'] = case.fpcw
    if case.import_transcript is not None:
        fixture['import_transcript'] = case.import_transcript
    return fixture


def broker_seed(fixture):
    """Preserve core fixture addresses while making their radix unambiguous."""
    # Core fixtures use hexadecimal string keys even without a prefix. Broker
    # patches use int(text,0), so decimal-looking legacy keys need a 0x prefix.
    return {**fixture, **{field: {f'{int(key,16):#x}': value for key, value in
            fixture.get(field, {}).items()} for field in ('arena_writes', 'globals')}}


def context_budget(config):
    """Check initial owner payload before creating a paid dispatch queue.

    The actor repeats its exact check with live stage metadata. Review payloads
    are qualified separately after their full source/emissions are frozen.
    """
    import math
    from .provider_opencode import (OpenCodeActor, PART_LINES, MAX_CONTEXT_PARTS,
                                    MAX_CONTEXT_BYTES, encode_context, _dumps)
    actor = OpenCodeActor({}, Path('unused'), runner=lambda: None, collector=lambda: None)
    document = actor._context_document(config, {'phase': 'owner', 'results': [], 'existing': {}},
                                       config['owner'], 'owner', config['mode'])
    text = _dumps(encode_context(document))
    parts, size = math.ceil(len(text.splitlines()) / PART_LINES), len(text.encode('utf-8'))
    return {'ready': parts <= MAX_CONTEXT_PARTS and size <= MAX_CONTEXT_BYTES,
            'parts': parts, 'bytes': size,
            'limits': {'parts': MAX_CONTEXT_PARTS, 'bytes': MAX_CONTEXT_BYTES}}


def prepare(output, rows, profiles, *, slots=4, duration_seconds=10800,
            limit=48, buffer_bytes=100000, evidence=native_packet, scalable=False,
            owner_override=None):
    from . import env
    from .provider_campaign import _validate_manifest
    if type(slots) is not int or not 1 <= slots <= 8:
        raise ValueError('slots must be 1..8')
    if owner_override is not None and owner_override not in ('deepseek', 'minimax'):
        raise ValueError('Owner override must name a configured provider role')
    output = Path(output).resolve()
    if not output.is_relative_to(env.ANALYSIS.resolve()):
        raise ValueError('Prepared queues must stay in local analysis storage')
    if output.exists():
        raise ValueError('Use a fresh preparation directory; retain prior queues')
    chosen = select(rows, limit=limit, buffer_bytes=buffer_bytes)
    if not chosen:
        raise ValueError('No replenishment candidates')
    jobs, packet_holds = [], []
    for i, row in enumerate(chosen):
        fid = row['function']
        hints = json.loads(Path(row['hints']).read_text(encoding='utf-8')) if row['hints'] else {}
        effective_hints = {**hints, **hints.get('c_hints', {})}
        fixtures = effective_hints.get('fixtures') or []
        fixture = broker_seed(fixtures[0]) if fixtures else diagnostic_seed(int(row['va'], 16))
        seed_path = output / 'seeds' / (fid + '.json')
        _write(seed_path, fixture)
        author = row['stage'] == 'author' and not row['dispatch_hold']
        # Both providers build upstream supply. Authoring remains independently
        # reviewed; no held source is silently requeued as a C task.
        owner = owner_override or ('minimax' if author or i % 2 else 'deepseek')
        goal = (f"Recover {fid} from original instructions, complete O0/O2 builds and full C gate; "
                'return the exact frozen bundle for independent review.' if author else
                f"Investigate {fid}: {row['reason']} Required next evidence: {row['retry_when']}. "
                'Read original evidence first. Establish an original returning fixture and '
                'the exact remaining blocker. Existing __ftol holds must be checked against ADR0006; '
                'other x87 boundaries remain explicit. Do not repeat a known failed probe unchanged. '
                'Identify exact registered-helper producer operations/widths and unresolved sites; '
                'an existing return by itself does not resolve the named evidence gap. '
                'Do not clear holds, edit canonical inputs, invent initialization or API outputs. '
                'A small reviewed input bundle or precise evidence-bound hold is the result.')
        context = []
        context.append(evidence(int(row['va'], 16), output / 'native' / (fid + '.txt')))
        if row['source']:
            context.append(pin(row['source']))
        if row['hints']:
            # Reuse the reviewed unchanged corpus by hash instead of copying
            # every fixture into every inference turn. The selected seed is
            # complete; full original hints remain retained and gate inputs.
            hint_summary = output / 'hint-index' / (fid + '.json')
            _write(hint_summary, {'original': pin(row['hints']),
                'fixture_count': len(fixtures),
                'settings': {k: v for k, v in hints.items() if k not in ('fixtures', 'c_hints')},
                'note': 'Selected seed is complete; remaining original fixtures are unchanged, '
                        'retained at the pinned path, and not reviewed by this input proposal.'})
            context.append(pin(hint_summary))
        adr = env.ROOT / 'docs/adr/0006-compiler-inserted-helpers-in-plain-c.md'
        if adr.is_file():
            context.append(pin(adr))
        config = {'function': row['va'], 'mode': 'c' if author else 'input',
                  'owner': owner, 'reviewer': 'deepseek' if owner == 'minimax' else 'minimax',
                  'goal': goal, 'allowed_functions': [row['va']],
                  'seed_fixture': pin(seed_path), 'context_files': context,
                  'max_body_bytes': row['body_bytes'], 'max_rounds': 12,
                  'provider_retries': 0, 'smoke_timeout_seconds': 180,
                  'provider_timeout_seconds': 900,
                  'auto_bundle': True,
                  'require_no_imports': False,
                  'min_coverage': 0.6 if not author else 0}
        from .pipeline_policy import supply_stage
        config['pipeline_stage'] = supply_stage(row) if scalable else row['stage']
        if scalable:
            config.update(defer_publication=True, defer_execution=True,
                          reserve_task=True, self_service_controls=True, auto_test_after_build=True)
            if not author:
                from . import work
                root_va = int(row['va'], 16)
                original_work = row.get('work_record', work.read()['functions'].get(fid))
                config['adoption_guard'] = {
                    'work': original_work, 'identity': row.get('input_identity') or work.identity(root_va),
                    'hints': pin(row['hints']) if row['hints'] else None,
                    'source': pin(row['source']) if row['source'] else None}
                config['goal'] += (
                    ' Automatic downstream promotion requires propose.resolution with reason, '
                    'obligations (the exact primary/additional hold reason strings, or an empty list), '
                    'evidence [{va, bytes, supports}] from the pinned original, and unknowns []. '
                    'Resolve every named obligation explicitly; a returning probe alone is insufficient. '
                    'For retained source, independently reconcile that exact source for a new C attempt; '
                    'this is not source acceptance. Otherwise hold with the remaining gap.')
        if not fixtures:
            config['goal'] += (' The supplied seed is a core-generator diagnostic sample '
                'with empty discovered-global inputs, not observed game initialization '
                'or case zero of the full differential corpus.')
        if evidence is native_packet:
            from . import inventory, lift
            root_va = int(row['va'], 16)
            iv = inventory.load()
            scope = {root_va, *lift.discover(root_va).callees}
            scope.update(caller for caller, callees in iv.callees.items() if root_va in callees)
            config['allowed_functions'] = [f'{va:#010x}' for va in sorted(scope)]
            if author and 0x00945640 in scope:
                config['require_controls'] = True
                config['goal'] += (' Registered helper source sites still require a pinned '
                    'named control plan and passing per-site sensitivity evidence. '
                    'Input readiness alone does not discharge source sensitivity.')
        if author and row['source']:
            config['candidate_seed'] = pin(row['source'])
        if not author and evidence is native_packet and '__ftol' in (row['reason'] + row['retry_when']):
            from . import lift
            sites = sorted(pc for pc, insn in lift.discover(int(row['va'], 16)).insns.items()
                           if insn.target == 0x00945640)
            if sites:
                config['required_pcs'] = [f'{sites[0]:#010x}']
                config['goal'] += f' The first bounded witness must reach helper site {sites[0]:#010x}.'
        job = {'id': fid.replace('_', '-') + '-' + row['stage'], 'config': config}
        qualification = context_budget(config)
        if qualification['ready']:
            jobs.append(job)
        else:
            packet_holds.append({'job': job, 'context': qualification,
                'retry_when': 'Prepare a bounded proof projection while retaining complete source/emissions and original evidence'})
    _write(output / 'packet-holds.json', packet_holds)
    if not jobs:
        raise ValueError('All selected packets exceed context budgets; retained packet-holds.json')
    manifest = _validate_manifest({'schema': 'cmre-provider-campaign/1',
        'duration_seconds': duration_seconds, 'concurrency': slots * 2,
        'role_capacity': {'deepseek': slots, 'minimax': slots},
        'roles': {role: {'model': profile['model']} for role, profile in profiles.items()},
        'jobs': jobs})
    counts = {stage: {'functions': sum(r['stage'] == stage for r in rows),
                      'body_bytes': sum(r['body_bytes'] for r in rows if r['stage'] == stage)}
              for stage in STAGES}
    report = {'schema': 'cmre-pipeline-supply/1', 'inventory': counts,
              'buffer_bytes_per_stage': buffer_bytes, 'selection': chosen,
              'dispatch_jobs': len(jobs), 'packet_held_jobs': len(packet_holds),
              'selected_by_stage': {stage: sum(r['stage'] == stage for r in chosen) for stage in STAGES},
              'selected_bytes_by_stage': {stage: sum(r['body_bytes'] for r in chosen
                                                    if r['stage'] == stage) for stage in STAGES},
              'manifest': str(output / 'manifest.json'),
              'warning': 'Investigation candidates are not ready C; input readiness awaits canonical adoption.'}
    _write(output / 'manifest.json', manifest)
    _write(output / 'supply.json', report)
    return report


def inbox(directory):
    """Durable stage queues, including completed inputs awaiting canonical adoption."""
    from . import provider_jobs
    state = provider_jobs.load(directory)
    queues = defaultdict(list)
    for job in state['jobs'].values():
        mode, status = job['config']['mode'], job['state']
        stage = ('adoption-held' if job.get('pipeline_hold') else
                 'execution' if status == 'awaiting-execution' else
                 'publication' if status == 'reviewed' else
                 'controls' if job.get('next_stage') == 'controls' or status == 'held' and
                 (job.get('escalation') or {}).get('adapter') == 'controls-plan' else
                 'adopted' if status == 'ready' and job.get('adoption_receipt') else
                 'review' if status == 'needs-review' or status == 'running' and job.get('phase') == 'review' else
                 'adoption' if status == 'ready' and mode == 'input' else
                 status if status in ('accepted', 'cancelled', 'exhausted') else
                 'held' if status == 'held' else 'author' if mode == 'c' else 'intake')
        bundle = job.get('bundle') or {}
        queues[stage].append({'job': job['id'], 'function': job['config']['function'],
                              'state': status, 'bundle': {k: bundle[k] for k in
                                  ('sha256', 'pin', 'required_checks') if k in bundle},
                              'reason': job.get('hold_reason') or job.get('reason')})
    return {'schema': 'cmre-pipeline-inbox/1',
            'counts': {k: len(v) for k, v in queues.items()}, 'queues': dict(queues)}


def acknowledge(directory, job_id, receipt):
    """Record coordinator adoption after normal input gates and hold resolution.

    This records scheduling provenance. It neither adopts data nor accepts C.
    The exact retained adoption receipt is required and every dispatch hold must
    already be resolved before upstream backpressure can be released.
    """
    from . import env, ledger, provider_jobs, work
    state = provider_jobs.load(directory)
    job = state['jobs'][job_id]
    if job['state'] != 'ready' or job['config']['mode'] != 'input':
        raise ValueError('Only reviewed ready input evidence can be acknowledged')
    va = int(job['config']['function'], 16)
    if work.dispatch_hold(va, work.read()['functions']):
        raise ValueError('Resolve the evidenced dispatch hold before acknowledging adoption')
    record = json.loads(ledger.record_path(va).read_text(encoding='utf-8'))
    if not record.get('lifted'):
        raise ValueError('Accepted native lift is required before downstream C dispatch')
    receipt = Path(receipt).resolve(strict=True)
    if not receipt.is_relative_to(env.ANALYSIS.resolve()):
        raise ValueError('Retain adoption evidence in local analysis storage')
    payload = json.loads(receipt.read_text(encoding='utf-8')) if receipt.stat().st_size <= 65536 else None
    if not isinstance(payload, dict):
        raise ValueError('Adoption receipt must be a bounded JSON object linking the gates')
    retained = pin(receipt)
    if job.get('adoption_receipt'):
        if job['adoption_receipt'] != retained:
            raise ValueError('Prior adoption receipt differs; retain its provenance')
        return {'job': job_id, 'adoption_receipt': retained}
    bundle_hash = (job.get('bundle') or {}).get('sha256')
    if (payload.get('schema') != 'cmre-pipeline-adoption/1'
            or payload.get('function') != job['config']['function']
            or not bundle_hash or payload.get('bundle_sha256') != bundle_hash):
        raise ValueError('Adoption receipt must identify this function and reviewed bundle')
    hint_path = env.HINTS_DIR / f'fn_{va:08x}.json'
    if (payload.get('canonical_hints') != pin(hint_path)
            or payload.get('native_gate') != pin(ledger.record_path(va))):
        raise ValueError('Adoption receipt must pin current canonical hints and accepted native gate')
    provider_jobs.update(directory, job_id, {'adoption_receipt': retained},
                         expected_revision=job['revision'])
    return {'job': job_id, 'adoption_receipt': retained}
