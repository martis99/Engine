#include "pch.h"
#include "constraints_resolver.h"
#include "ecs/component/transform.h"
#include "ecs/component/constraints.h"

static void resolve_constraints_x(Ecs* ecs, Transform* transform, Constraints* constraints);
static void resolve_constraints_y(Ecs* ecs, Transform* transform, Constraints* constraints);
static void resolve_constraints_z(Ecs* ecs, Transform* transform, Constraints* constraints);

static Constraint* prepare_constraint_x(Ecs* ecs, Constraint* constraint, Transform** parent) {
	if (constraint->enabled == 0) {
		return constraint;
	}

	*parent = (Transform*)ecs_get(ecs, constraint->entity.id, C_TRANSFORM);
	Constraints* parent_constraints = ((Constraints*)ecs_get(ecs, constraint->entity.id, C_CONSTRAINTS));
	if (parent_constraints->resolved.x == 0) {
		resolve_constraints_x(ecs, *parent, parent_constraints);
	}

	return constraint;
}

static Constraint* prepare_constraint_y(Ecs* ecs, Constraint* constraint, Transform** parent) {
	if (constraint->enabled == 0) {
		return constraint;
	}

	*parent = (Transform*)ecs_get(ecs, constraint->entity.id, C_TRANSFORM);
	Constraints* parent_constraints = ((Constraints*)ecs_get(ecs, constraint->entity.id, C_CONSTRAINTS));
	if (parent_constraints->resolved.y == 0) {
		resolve_constraints_y(ecs, *parent, parent_constraints);
	}

	return constraint;
}

static Constraint* prepare_constraint_z(Ecs* ecs, Constraint* constraint, Transform** parent) {
	if (constraint->enabled == 0) {
		return constraint;
	}

	*parent = (Transform*)ecs_get(ecs, constraint->entity.id, C_TRANSFORM);
	Constraints* parent_constraints = ((Constraints*)ecs_get(ecs, constraint->entity.id, C_CONSTRAINTS));
	if (parent_constraints->resolved.z == 0) {
		resolve_constraints_z(ecs, *parent, parent_constraints);
	}

	return constraint;
}

static void calculate(float min, float max, Constraint* minc, Constraint* maxc, float size, float pref_scale, float* pos, float* scale) {
	if (minc->enabled && maxc->enabled) {
		if (size == -1) {
			*pos = min + (max - min - pref_scale) / 2;
			*scale = pref_scale;
		} else if (size == 0) {
			*pos = min;
			*scale = max - min;
		} else {
			*pos = min + (max - min - size) / 2;
			*scale = size;
		}
	} else if (minc->enabled) {
		if (size == -1) {
			*pos = min;
			*scale = pref_scale;
		} else {
			*pos = min;
			*scale = size;
		}
	} else if (maxc->enabled) {
		if (size == -1) {
			*pos = max - pref_scale;
			*scale = pref_scale;
		} else {
			*pos = max - size;
			*scale = size;
		}
	} else {
		if (size == -1) {
			*scale = pref_scale;
		}
	}
}

static void resolve_constraints_x(Ecs* ecs, Transform* transform, Constraints* constraints) {
	Transform* mint, * maxt;
	Constraint* minc = prepare_constraint_x(ecs, &constraints->l, &mint);
	Constraint* maxc = prepare_constraint_x(ecs, &constraints->r, &maxt);

	float min = minc->enabled ? mint->position.x + (minc->pos * mint->scale.x) + minc->distance : 0.0f;
	float max = maxc->enabled ? maxt->position.x + (maxc->pos * maxt->scale.x) - maxc->distance : 0.0f;
	calculate(min, max, minc, maxc, constraints->size.x, transform->scale_pref.x, &transform->position.x, &transform->scale.x);
	constraints->resolved.x = 1;
}

static void resolve_constraints_y(Ecs* ecs, Transform* transform, Constraints* constraints) {
	Transform* mint, * maxt;
	Constraint* minc = prepare_constraint_y(ecs, &constraints->u, &mint);
	Constraint* maxc = prepare_constraint_y(ecs, &constraints->d, &maxt);

	float min = minc->enabled ? mint->position.y + (minc->pos * mint->scale.y) + minc->distance : 0.0f;
	float max = maxc->enabled ? maxt->position.y + (maxc->pos * maxt->scale.y) - maxc->distance : 0.0f;
	calculate(min, max, minc, maxc, constraints->size.y, transform->scale_pref.y, &transform->position.y, &transform->scale.y);
	constraints->resolved.y = 1;
}

static void resolve_constraints_z(Ecs* ecs, Transform* transform, Constraints* constraints) {
	Transform* mint, * maxt;
	Constraint* minc = prepare_constraint_z(ecs, &constraints->b, &mint);
	Constraint* maxc = prepare_constraint_z(ecs, &constraints->f, &maxt);

	float min = minc->enabled ? mint->position.z + (minc->pos * mint->scale.z) + minc->distance : 0.0f;
	float max = maxc->enabled ? maxt->position.z + (maxc->pos * maxt->scale.z) - maxc->distance : 0.0f;
	calculate(min, max, minc, maxc, constraints->size.z, transform->scale_pref.z, &transform->position.z, &transform->scale.z);
	constraints->resolved.z = 1;
}

static void resolve_constraints(Ecs* ecs, Transform* transform, Constraints* constraints) {
	resolve_constraints_x(ecs, transform, constraints);
	resolve_constraints_y(ecs, transform, constraints);
	resolve_constraints_z(ecs, transform, constraints);
}

void constraints_resolver_resolve(Ecs* ecs) {
	QueryResult* qr = ecs_query(ecs, 2, C_TRANSFORM, C_CONSTRAINTS);
	for (uint i = 0; i < qr->count; ++i) {
		Constraints* constraints = (Constraints*)ecs_get(ecs, qr->list[i], C_CONSTRAINTS);
		constraints->resolved = (vec3){ 0, 0, 0 };
	}

	for (uint i = 0; i < qr->count; ++i) {
		Transform* transform = (Transform*)ecs_get(ecs, qr->list[i], C_TRANSFORM);
		Constraints* constraints = (Constraints*)ecs_get(ecs, qr->list[i], C_CONSTRAINTS);
		resolve_constraints(ecs, transform, constraints);
	}
}