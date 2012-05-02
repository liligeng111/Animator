#include "particle.h"
#include <FL/gl.h>
#include "modelerdraw.h"

Particle::Particle(float x, float y, float z, float t) : position(x, y, z)
{
	velocity = Vec3f( 1.0f * (rand() % 100) / 50 - 1, 4.5, 1.0f * (rand() % 100) / 50 - 1);
	time = t;
}

void Particle::applyForce(const Vec3f& force, float t)
{
	float d = t - time;
	velocity += force * d;
	position += velocity * d;
	time = t;
}

void Particle::draw()
{
	glPushMatrix();
		glTranslated(position[0], position[1], position[2]);
		drawSphere(0.05);
	glPopMatrix();
}