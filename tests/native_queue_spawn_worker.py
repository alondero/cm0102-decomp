"""Importable spawn target: actual OS contention, Queue lock/store, no launch.

No executable top-level work: safe under Windows spawn's import of __mp_main__.
The selected Queue module path is explicit, never inferred from Analysis paths.
"""
import importlib.util
import os
from pathlib import Path
import traceback

from cmre.coordination import BusyError


def reserve_child(role, module_path, root, brief, barrier, contended, committed, reports):
    report = {'role': role, 'pid': os.getpid(), 'brief': dict(brief),
              'contentions': 0, 'reserve_calls': 0, 'native_actions': None,
              'error': None}
    try:
        spec = importlib.util.spec_from_file_location('spawn_selected_queue', module_path)
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        queue = module.Queue(Path(root), clock=lambda: brief['sampled_at'])
        if role == 'holder':
            # Real lock, held across the rendezvous solely to force an observed
            # nonblocking collision. No Queue policy or lock is mocked/replaced.
            with module.lock(Path(root) / 'queue.lock'):
                barrier.wait(timeout=12)
                if not contended.wait(timeout=12):
                    raise TimeoutError('Contender did not report real BusyError')
            report['reserve_calls'] += 1
            result = queue.reserve(**brief)
            # Set only after reserve has returned and its commit lock is released.
            committed.set()
        elif role == 'contender':
            barrier.wait(timeout=12)
            report['reserve_calls'] += 1
            try:
                queue.reserve(**brief)
            except BusyError:
                report['contentions'] += 1
                contended.set()
            else:
                raise AssertionError('Expected actual file-lock contention')
            if not committed.wait(timeout=12):
                raise TimeoutError('Holder did not commit before the one retry')
            report['reserve_calls'] += 1
            result = queue.reserve(**brief)
        else:
            raise ValueError('Unknown child role')
        report['native_actions'] = result['native_actions']
    except BaseException:
        report['error'] = traceback.format_exc()
        barrier.abort()
    finally:
        reports.put(report)
        reports.close()
        reports.join_thread()
