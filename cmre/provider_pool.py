"""Lease isolated OpenCode clients to concurrent stages of one durable queue."""
from pathlib import Path
from queue import Empty, Queue

ROLES = ('deepseek', 'minimax')
XDG = ('XDG_DATA_HOME', 'XDG_STATE_HOME', 'XDG_CONFIG_HOME', 'XDG_CACHE_HOME')


def validate(profiles):
    if not isinstance(profiles, list) or not 1 <= len(profiles) <= 8:
        raise ValueError('Pool needs 1..8 isolated profile pairs')
    models, paths = {}, set()
    for pair in profiles:
        if not isinstance(pair, dict) or set(pair) != set(ROLES):
            raise ValueError('Each slot needs deepseek and minimax profiles')
        for role in ROLES:
            profile = pair[role]
            model = profile.get('model')
            if not isinstance(model, str) or not model:
                raise ValueError('Every slot needs an exact model')
            if model != models.setdefault(role, model):
                raise ValueError('Pool slots must use the same model for each role')
            environment = profile.get('environment', {})
            for key in XDG:
                value = environment.get(key)
                if not isinstance(value, str) or not value:
                    raise ValueError(f'Each isolated slot must set {key}')
                path = Path(value).resolve()
                # Ancestors overlap too: a shared parent state tree is unsafe.
                if any(path == old or path.is_relative_to(old) or old.is_relative_to(path)
                       for old in paths):
                    raise ValueError('Provider slots must have disjoint XDG trees')
                paths.add(path)
    if len(set(models.values())) != 2:
        raise ValueError('Independent providers must use distinct models')
    return profiles


class ActorPool:
    def __init__(self, profiles, parent, *, factory=None):
        from .provider_opencode import OpenCodeActor
        factory = factory or OpenCodeActor
        self.profiles = validate(profiles)
        self.available = {role: Queue() for role in ROLES}
        for pair in self.profiles:
            actor = factory(pair, parent)
            for role in ROLES:
                state_path = Path(pair[role]['environment']['XDG_DATA_HOME'])
                self.available[role].put((actor, state_path))

    def __call__(self, job, role, context, stage_path):
        try:
            actor, state_path = self.available[role].get_nowait()
        except (KeyError, Empty) as error:
            raise ValueError('Scheduler exceeded isolated provider capacity') from error
        try:
            from .coordination import lock
            # Protect the actual client state even if another controller is
            # accidentally launched against the same provisioned slot report.
            state_path.mkdir(parents=True, exist_ok=True)
            with lock(state_path / 'cmre-provider-slot.lock'):
                return actor(job, role, context, stage_path)
        finally:
            self.available[role].put((actor, state_path))
