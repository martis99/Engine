#include "constraints.h"

Constraint constraint_create(Entity entity, float pos, int distance)
{
	return (Constraint){
		.enabled  = 1,
		.entity	  = entity,
		.pos	  = pos,
		.distance = distance,
	};
}

Constraints constraints_create(vec3 size)
{
	return (Constraints){
		.resolved  = (vec3){ 0, 0, 0 },
		.size	   = size,
		.l.enabled = 0,
		.r.enabled = 0,
		.u.enabled = 0,
		.d.enabled = 0,
		.f.enabled = 0,
		.b.enabled = 0,
	};
}
