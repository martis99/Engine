#pragma once

typedef struct Constraint {
	bool enabled;
	Entity entity;
	float pos;
	int distance;
} Constraint;

typedef struct Constraints {
	bool r_h, r_v, r_d;
	Constraint l, r, u, d, f, b;
	int w, h;
} Constraints;

Constraint constraint_create(Entity entity, float pos, int distance);

Constraints constraints_create(int width, int height);
void constraints_add_l(Constraints* constraints, Constraint constraint);
void constraints_add_r(Constraints* constraints, Constraint constraint);
void constraints_add_u(Constraints* constraints, Constraint constraint);
void constraints_add_d(Constraints* constraints, Constraint constraint);
void constraints_add_f(Constraints* constraints, Constraint constraint);
void constraints_add_b(Constraints* constraints, Constraint constraint);