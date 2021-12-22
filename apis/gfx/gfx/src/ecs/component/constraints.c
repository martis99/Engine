#include "pch.h"
#include "constraints.h"

Constraint constraint_create(Entity entity, float pos, int distance) {
	return (Constraint) { 1, entity, pos, distance };
}

Constraints constraints_create(vec3 size) {
	Constraints constraints = { 0 };
	constraints.resolved = (vec3){ 0, 0, 0 };
	constraints.size = size;
	constraints.l.enabled = 0;
	constraints.r.enabled = 0;
	constraints.u.enabled = 0;
	constraints.d.enabled = 0;
	constraints.f.enabled = 0;
	constraints.b.enabled = 0;
	return constraints;
}