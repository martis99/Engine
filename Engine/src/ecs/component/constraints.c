#include "pch.h"
#include "constraints.h"

Constraint constraint_create(Entity entity, float pos, int distance) {
	return (Constraint) { 1, entity, pos, distance };
}

Constraints constraints_create(int width, int height) {
	Constraints constraints = { 0 };
	constraints.r_h = 0;
	constraints.r_v = 0;
	constraints.r_d = 0;
	constraints.l.enabled = 0;
	constraints.r.enabled = 0;
	constraints.u.enabled = 0;
	constraints.d.enabled = 0;
	constraints.f.enabled = 0;
	constraints.b.enabled = 0;
	constraints.w = width;
	constraints.h = height;
	return constraints;
}

void constraints_add_l(Constraints* constraints, Constraint constraint) {
	constraints->l = constraint;
}

void constraints_add_r(Constraints* constraints, Constraint constraint) {
	constraints->r = constraint;
}

void constraints_add_u(Constraints* constraints, Constraint constraint) {
	constraints->u = constraint;
}

void constraints_add_d(Constraints* constraints, Constraint constraint) {
	constraints->d = constraint;
}

void constraints_add_f(Constraints* constraints, Constraint constraint) {
	constraints->f = constraint;
}

void constraints_add_b(Constraints* constraints, Constraint constraint) {
	constraints->b = constraint;
}