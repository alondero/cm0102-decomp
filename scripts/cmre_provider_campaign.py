"""Run a provider-owned recovery campaign from an ordinary terminal."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

# More than two stages require explicitly provisioned isolated profile pairs.
MAX_CLI_WORKERS = 2


def load(path):
    from scripts.cmre_provider_artifacts import load as strict_load
    return strict_load(Path(path))


def _profile_entries(profiles):
    if not isinstance(profiles, dict) or set(profiles) != {'deepseek', 'minimax'}:
        raise ValueError('Profiles must contain exactly the deepseek and minimax roles')
    entries = {}
    for role, profile in profiles.items():
        if isinstance(profile, (str, Path)):
            profile = load(profile)
        if not isinstance(profile, dict):
            raise ValueError(f'Profile {role} must be a mapping')
        entries[role] = profile
    return entries


def _check_profile_models(profiles, manifest):
    entries = _profile_entries(profiles)
    roles = manifest.get('roles') if isinstance(manifest, dict) else None
    if not isinstance(roles, dict) or set(roles) != {'deepseek', 'minimax'}:
        raise ValueError('Manifest roles must contain exactly deepseek and minimax')
    for role, profile in entries.items():
        if profile.get('model') != roles[role].get('model'):
            raise ValueError(f'Profile model for {role} differs from the manifest role model')
    return entries


def _check_worker_cap(manifest, slot_profiles=None):
    """Refuse more than two workers before anything is written."""
    if not isinstance(manifest, dict):
        return
    concurrency = manifest.get('concurrency')
    if isinstance(concurrency, bool) or not isinstance(concurrency, int):
        return
    maximum = 2 * len(slot_profiles) if slot_profiles else MAX_CLI_WORKERS
    capacities = manifest.get('role_capacity', {'deepseek': 1, 'minimax': 1})
    if any(n > (len(slot_profiles) if slot_profiles else 1) for n in capacities.values()):
        raise ValueError('role_capacity exceeds the isolated profiles supplied')
    if concurrency > maximum:
        raise ValueError(f'At most {maximum} workers are allowed; '
                         f'concurrency {concurrency} is refused')


def identity(profiles, parent, slot_profiles=None):
    from cmre import env, recover
    receipt = load(parent)
    if (receipt.get('state') != 'finished' or not receipt.get('steps')
            or any(step.get('state') != 'finished' or step.get('exit_code') != 0
                   for step in receipt['steps'])):
        raise ValueError('A finished green parent preflight receipt is required')
    modules = ['provider_jobs', 'provider_probe', 'provider_broker',
               'provider_campaign', 'provider_opencode', 'provider_runtime',
               'provider_retry', 'provider_pool', 'pipeline_flow',
               'pipeline_policy', 'pipeline_adoption', 'pipeline_controller', 'source_policy']
    paths = [env.ROOT / 'cmre' / (name + '.py') for name in modules]
    paths += [Path(__file__), env.ROOT / 'scripts/cmre_provider_worker.py',
              env.ROOT / 'scripts/cmre_provider_artifacts.py']
    profile_ids = {}
    for role, profile in profiles.items():
        if isinstance(profile, str):
            profile = load(profile)
        executable = Path(profile['executable']).resolve(strict=True)
        profile_ids[role] = {'model': profile['model'], 'client_version': profile['client_version'],
                             'executable_sha256': env.digest(executable),
                             'profile_sha256': hashlib.sha256(json.dumps(
                                 profile, sort_keys=True, allow_nan=False).encode()).hexdigest()}
    result = {'execution': recover.execution_identity(),
            'controller': {env.rel(path): env.digest(path) for path in paths},
            'profiles': profile_ids, 'parent_sha256': env.digest(Path(parent))}
    if slot_profiles:
        result['pool_sha256'] = hashlib.sha256(json.dumps(
            slot_profiles, sort_keys=True, allow_nan=False).encode()).hexdigest()
        result['pool_executables'] = [
            {role: env.digest(Path(profile['executable'])) for role, profile in pair.items()}
            for pair in slot_profiles]
    return result


def pool_profiles(path):
    from cmre.provider_pool import validate
    report = load(path)
    pairs = [_profile_entries(load(row['path'])) for row in report['profiles']]
    return validate(pairs)


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest='command', required=True)
    init = sub.add_parser('init', help='Validate and freeze a campaign; does not start its clock')
    init.add_argument('directory', type=Path)
    init.add_argument('--manifest', type=Path, required=True)
    init.add_argument('--profiles', type=Path, required=True)
    init.add_argument('--parent', type=Path, required=True)
    init.add_argument('--slot-profiles', type=Path, help='provisioned slots.json for pooled capacity')
    init.add_argument('--pipeline', action='store_true', help='enable continuous replenishment and reviewed adoption')
    init.add_argument('--policy', type=Path, help='initial pipeline stage limits and buffer watermarks')
    run = sub.add_parser('run', help='Run or resume the same persisted deadline')
    run.add_argument('directory', type=Path)
    run.add_argument('--profiles', type=Path, required=True)
    run.add_argument('--parent', type=Path, required=True)
    run.add_argument('--once', action='store_true')
    run.add_argument('--steps', type=int, help='bound paid/broker/publication dispatch stages; drain started work')
    run.add_argument('--slot-profiles', type=Path)
    retry = sub.add_parser('retry',
                            help='Prepare one safe held job for a fresh attempt; '
                                 'runs no inference')
    retry.add_argument('directory', type=Path)
    retry.add_argument('--job', required=True)
    retry.add_argument('--reason', required=True)
    retry.add_argument('--profiles', type=Path, required=True)
    retry.add_argument('--parent', type=Path, required=True)
    retry.add_argument('--slot-profiles', type=Path)
    status = sub.add_parser('status', help='Inspect durable progress without inference')
    status.add_argument('directory', type=Path)
    scale = sub.add_parser('scale', help='change one stage without restarting or renewing the run')
    scale.add_argument('directory', type=Path)
    from cmre.pipeline_policy import STAGES
    scale.add_argument('--stage', choices=STAGES, required=True)
    scale.add_argument('--workers', type=int, required=True)
    scale.add_argument('--owners', help='comma-separated roles for newly prepared jobs in this supply stage')
    scale.add_argument('--execution-workers', type=int, help='shared broker execution pool limit, 1..16')
    for option in ('low-functions', 'high-functions', 'low-bytes', 'high-bytes'):
        scale.add_argument('--' + option, type=int)
    args = parser.parse_args(argv)
    from cmre import provider_campaign as campaign
    if args.command == 'scale':
        from cmre.pipeline_controller import scale as scale_pipeline
        result = scale_pipeline(args.directory, args.stage, args.workers,
            owners=args.owners.split(',') if args.owners else None,
            execution_workers=args.execution_workers, **{
            name: getattr(args, name) for name in ('low_functions', 'high_functions', 'low_bytes', 'high_bytes')
            if getattr(args, name) is not None})
    elif args.command == 'status':
        result = campaign.status(args.directory.resolve())
        from cmre.pipeline_controller import status as pipeline_status
        result['pipeline'] = pipeline_status(args.directory.resolve())
    elif args.command == 'retry':
        profiles = load(args.profiles)
        slots = pool_profiles(args.slot_profiles) if args.slot_profiles else None
        current_identity = identity(profiles, args.parent, slots) if slots else identity(profiles, args.parent)
        from cmre import provider_jobs, provider_retry
        state = provider_jobs.load(args.directory.resolve())
        _check_profile_models(profiles, state['manifest'])
        result = provider_retry.retry_held(args.directory.resolve(), args.job,
                                           args.reason, current_identity)
    else:
        profiles = load(args.profiles)
        slots = pool_profiles(args.slot_profiles) if args.slot_profiles else None
        raw_manifest = load(args.manifest) if args.command == 'init' else None
        if args.command == 'init':
            if args.policy and not args.pipeline:
                raise ValueError('--policy requires --pipeline')
            if raw_manifest.get('pipeline') and not args.pipeline:
                raise ValueError('Use --pipeline to initialize the continuous service and its policy')
            if args.pipeline:
                from cmre.pipeline_controller import enable
                raw_manifest = enable(raw_manifest)
            _check_profile_models(profiles, raw_manifest)
            _check_worker_cap(raw_manifest, slots)
        if slots:
            from cmre import provider_jobs
            for pair in slots:
                _check_profile_models(pair, raw_manifest if raw_manifest else
                                      provider_jobs.load(args.directory.resolve())['manifest'])
        current_identity = identity(profiles, args.parent, slots) if slots else identity(profiles, args.parent)
        if args.command == 'init':
            from cmre.provider_runtime import prepare_manifest, seed_candidates
            manifest = prepare_manifest(args.directory.resolve(), raw_manifest)
            _check_worker_cap(manifest, slots)
            if args.pipeline:
                from cmre import pipeline_policy
                policy = load(args.policy) if args.policy else pipeline_policy.defaults(manifest)
                pipeline_policy.validate(policy, manifest)
            result = campaign.initialize(args.directory.resolve(), manifest, current_identity)
            seed_candidates(args.directory.resolve(), manifest)
            if args.pipeline:
                result['pipeline'] = pipeline_policy.write(args.directory.resolve(), policy, manifest)
        else:
            from cmre import provider_jobs
            from cmre.provider_opencode import OpenCodeActor
            from cmre.provider_runtime import Executor, verify_seeded_candidates
            state = provider_jobs.load(args.directory.resolve())
            _check_profile_models(profiles, state['manifest'])
            _check_worker_cap(state['manifest'], slots)
            if state.get('identity') != current_identity:
                raise ValueError('Controller identity changed before provider dispatch')
            verify_seeded_candidates(args.directory.resolve(), state['manifest'])
            from cmre.provider_pool import ActorPool
            actor = (ActorPool(slots, args.parent.resolve()) if slots else
                     OpenCodeActor(profiles, args.parent.resolve()))
            executor = Executor(args.directory.resolve())
            options = dict(actor=actor, execute=executor, identity=current_identity,
                           once=args.once, max_steps=args.steps)
            if state['manifest'].get('pipeline'):
                from cmre.pipeline_controller import Service
                from cmre import coordination, pipeline_policy
                with coordination.writer(), Service(args.directory.resolve(), _profile_entries(profiles), executor) as service:
                    result = campaign.run(args.directory.resolve(), service=service,
                        limits=lambda state: pipeline_policy.read(args.directory.resolve(), state['manifest']),
                        **options)
            else:
                result = campaign.run(args.directory.resolve(), **options)
    print(json.dumps(result, indent=2, allow_nan=False))
    return 0


if __name__ == '__main__':
    try:
        raise SystemExit(main())
    except (OSError, ValueError, RuntimeError) as error:
        print(f'{type(error).__name__}: {error}', file=sys.stderr)
        raise SystemExit(1)
