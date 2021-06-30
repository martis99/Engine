#include "pch.h"
#include "constraints_resolver.h"
#include "ecs/component/transform.h"
#include "ecs/component/constraints.h"

static void resolve_constraints_h(Ecs* ecs, Transform* transform, Constraints* constraints);
static void resolve_constraints_v(Ecs* ecs, Transform* transform, Constraints* constraints);
static void resolve_constraints_d(Ecs* ecs, Transform* transform, Constraints* constraints);

static Constraint* prepare_constraint_h(Ecs* ecs, Constraint* constraint, Transform** parent) {
	if (constraint->enabled == 0) {
		return constraint;
	}

	*parent = (Transform*)ecs_get(ecs, constraint->entity.id, C_TRANSFORM);
	Constraints* parent_constraints = ((Constraints*)ecs_get(ecs, constraint->entity.id, C_CONSTRAINTS));
	if (parent_constraints->r_h == 0) {
		resolve_constraints_h(ecs, *parent, parent_constraints);
	}

	return constraint;
}

static Constraint* prepare_constraint_v(Ecs* ecs, Constraint* constraint, Transform** parent) {
	if (constraint->enabled == 0) {
		return constraint;
	}

	*parent = (Transform*)ecs_get(ecs, constraint->entity.id, C_TRANSFORM);
	Constraints* parent_constraints = ((Constraints*)ecs_get(ecs, constraint->entity.id, C_CONSTRAINTS));
	if (parent_constraints->r_v == 0) {
		resolve_constraints_v(ecs, *parent, parent_constraints);
	}

	return constraint;
}

static Constraint* prepare_constraint_d(Ecs* ecs, Constraint* constraint, Transform** parent) {
	if (constraint->enabled == 0) {
		return constraint;
	}

	*parent = (Transform*)ecs_get(ecs, constraint->entity.id, C_TRANSFORM);
	Constraints* parent_constraints = ((Constraints*)ecs_get(ecs, constraint->entity.id, C_CONSTRAINTS));
	if (parent_constraints->r_d == 0) {
		resolve_constraints_d(ecs, *parent, parent_constraints);
	}

	return constraint;
}

static void resolve_constraints_h(Ecs* ecs, Transform* transform, Constraints* constraints) {
	Transform* lp, * rp;
	Constraint* l = prepare_constraint_h(ecs, &constraints->l, &lp);
	Constraint* r = prepare_constraint_h(ecs, &constraints->r, &rp);

	int w = constraints->w;
	if (l->enabled && r->enabled) {
		float min = lp->position.x + (l->pos * lp->scale.x) + l->distance;
		float max = rp->position.x + (r->pos * rp->scale.x) - r->distance;
		if (w == -1) {
			transform->position.x = min + (max - min - transform->scale_pref.x) / 2;
			transform->scale.x = transform->scale_pref.x;
		} else if (w == 0) {
			transform->position.x = min;
			transform->scale.x = max - min;
		} else {
			transform->position.x = min + (max - min - w) / 2;
			transform->scale.x = (float)w;
		}
	} else if (l->enabled) {
		if (w == -1) {
			float min = lp->position.x + (l->pos * lp->scale.x) + l->distance;
			transform->position.x = min;
			transform->scale.x = transform->scale_pref.x;
		} else {
			float min = lp->position.x + (l->pos * lp->scale.x) + l->distance;
			transform->position.x = min;
			transform->scale.x = (float)w;
		}
	} else if (r->enabled) {
		if (w == -1) {
			float max = rp->position.x + (r->pos * rp->scale.x) - r->distance;
			transform->position.x = max - transform->scale_pref.x;;
			transform->scale.x = transform->scale_pref.x;
		} else {
			float max = rp->position.x + (r->pos * rp->scale.x) - r->distance;
			transform->position.x = max - w;
			transform->scale.x = (float)w;
		}
	} else {
		if (w == -1) {
			transform->scale.x = transform->scale_pref.x;
		}
	}
	constraints->r_h = 1;
}

static void resolve_constraints_v(Ecs* ecs, Transform* transform, Constraints* constraints) {
	Transform* up, * dp;
	Constraint* u = prepare_constraint_v(ecs, &constraints->u, &up);
	Constraint* d = prepare_constraint_v(ecs, &constraints->d, &dp);
	int h = constraints->h;

	if (u->enabled && d->enabled) {
		float min = up->position.y + (u->pos * up->scale.y) + u->distance;
		float max = dp->position.y + (d->pos * dp->scale.y) - d->distance;
		if (h == -1) {
			transform->position.y = min + (max - min - transform->scale_pref.y) / 2;
			transform->scale.y = transform->scale_pref.y;
		} else if (h == 0) {
			transform->position.y = min;
			transform->scale.y = max - min;
		} else {
			transform->position.y = min + (max - min - h) / 2;
			transform->scale.y = (float)h;
		}
	} else if (u->enabled) {
		if (h == -1) {
			float min = up->position.y + (u->pos * up->scale.y) + u->distance;
			transform->position.y = min;
			transform->scale.y = transform->scale_pref.y;
		} else {
			float min = up->position.y + (u->pos * up->scale.y) + u->distance;
			transform->position.y = min;
			transform->scale.y = (float)h;
		}
	} else if (d->enabled) {
		if (h == -1) {
			float max = dp->position.y + (d->pos * dp->scale.y) - d->distance;
			transform->position.y = max - h;
			transform->scale.y = transform->scale_pref.y;
		} else {
			float max = dp->position.y + (d->pos * dp->scale.y) - d->distance;
			transform->position.y = max - transform->scale_pref.y;
			transform->scale.y = transform->scale_pref.y;
		}
	} else {
		if (h == -1) {
			transform->scale.y = transform->scale_pref.y;
		}
	}

	constraints->r_v = 1;
}

static void resolve_constraints_d(Ecs* ecs, Transform* transform, Constraints* constraints) {
	Transform* fp, * bp;
	Constraint* f = prepare_constraint_d(ecs, &constraints->f, &fp);
	Constraint* b = prepare_constraint_d(ecs, &constraints->b, &bp);

	if (b->enabled == 1) {
		transform->position.z = bp->position.z + 0.1f;
	}
	if (f->enabled == 1) {
		transform->position.z = bp->position.z - 0.1f;
	}
	constraints->r_d = 1;
}

static void resolve_constraints(Ecs* ecs, Transform* transform, Constraints* constraints) {
	resolve_constraints_h(ecs, transform, constraints);
	resolve_constraints_v(ecs, transform, constraints);
	resolve_constraints_d(ecs, transform, constraints);
}

void constraints_resolver_resolve(Ecs* ecs) {
	QueryResult* qr = ecs_query(ecs, 2, C_TRANSFORM, C_CONSTRAINTS);
	for (uint i = 0; i < qr->count; ++i) {
		Constraints* constraints = (Constraints*)ecs_get(ecs, qr->list[i], C_CONSTRAINTS);
		constraints->r_h = 0;
		constraints->r_v = 0;
		constraints->r_d = 0;
	}

	for (uint i = 0; i < qr->count; ++i) {
		Transform* transform = (Transform*)ecs_get(ecs, qr->list[i], C_TRANSFORM);
		Constraints* constraints = (Constraints*)ecs_get(ecs, qr->list[i], C_CONSTRAINTS);
		resolve_constraints(ecs, transform, constraints);
	}
}