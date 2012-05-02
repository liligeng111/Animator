
#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "vec.h"
#include <stdlib.h>

class Particle
{
private:
	Vec3f position;
	Vec3f velocity;
	float time;
public:
	Particle(float x, float y, float z, float t);
	void applyForce(const Vec3f& force, float t);
	void draw();
	bool too_far() {return position.length2() > 150;}
};

#endif	// __PARTICLE_H__