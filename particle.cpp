#include "particle.h"
#include <FL/gl.h>
#include "modelerdraw.h"

Particle::Particle(float x, float y, float z, float t) : position(x, y, z)
{
	velocity = Vec3f( 1.0f * (rand() % 100) / 50 - 1, 4.5, 1.0f * (rand() % 100) / 50 - 1);
	time = t;
}

Particle::Particle()
{
	time = 0;
}

void Particle::setPosition(float x, float y, float z, float t)
{
	time = t;
	position = Vec3f(x, y, z);
	velocity = Vec3f(0, 0, 0);
	force = Vec3f(0, 0, 0);
}

void Particle::applyForce(const Vec3f& f)
{
	force = f;
}

void Particle::move(float t)
{	
	float d = t - time;
	velocity += force * d * 2;
	position += velocity * d;
	time = t;
	velocity *= 0.01f;
}

void Particle::draw()
{
	glPushMatrix();
		glTranslated(position[0], position[1], position[2]);
		drawSphere(0.05);
	glPopMatrix();
}