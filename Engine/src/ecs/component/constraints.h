#pragma once
#include "structs.h"

Constraint constraint_create(Entity entity, float pos, int distance);

Constraints constraints_create(int width, int height);
void constraints_add_l(Constraints* constraints, Constraint constraint);
void constraints_add_r(Constraints* constraints, Constraint constraint);
void constraints_add_u(Constraints* constraints, Constraint constraint);
void constraints_add_d(Constraints* constraints, Constraint constraint);
void constraints_add_f(Constraints* constraints, Constraint constraint);
void constraints_add_b(Constraints* constraints, Constraint constraint);