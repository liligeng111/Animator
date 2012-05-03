#pragma warning(disable : 4786)

#include "particlesystem.h"


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <FL/gl.h>
#include <math.h>
#include <limits.h>

const Vec3f ParticleSystem::g = Vec3f(0, -4.8f, 0);

const float K = 500; 
/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem() 
{
	particles = new Particle*[20];
	for (int i = 0; i < 20; i++)
		particles[i] = new Particle[20];
}



/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{
	for (int i = 0; i < 20; i++) delete[] particles[i];
	delete[] particles;

}

void ParticleSystem::addParticle(Vec4f p)
{
	Vec3f l = particles[0][0].getPosition();
	Vec3f r = particles[0][19].getPosition();
	particles[0][0].setPosition(l[0], p[1], l[2], time);
	particles[0][19].setPosition(r[0], p[1], r[2], time);
}


/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{
	time = t;
	bake_start_time = t;

	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
			particles[i][j].setPosition(0.2 * (j - 10), 5 - 0.01 * i * i, -0.8 - 0.1 * i, t);

	// These values are used by the UI ...
	// -ve bake_end_time indicates that simulation
	// is still progressing, and allows the
	// indicator window above the time slider
	// to correctly show the "baked" region
	// in grey.
	bake_end_time = -1;
	simulate = true;
	dirty = true;

}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t)
{
    
	bake_end_time = t;

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{
	time = t;

	// These values are used by the UI
	simulate = false;
	dirty = true;

}


Vec3f ParticleSystem::springForce(int i, int j, const Vec3f& position)
{
	if (i < 0 || i > 19 || j < 0 || j > 19) return Vec3f(0, 0, 0);
	
	Vec3f f = particles[i][j].getPosition() - position;
	float l = f.length();
	if (f.length() == 0) return Vec3f(0, 0.1, 0);
	f.normalize();
	return f * (K *(l - 0.2));
}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{
	time = t;
	if (!simulate) return;

	for (int i = 0; i < 20; i++)
	{
		for (int j = 19; j >= 0; j--)
		{
			if (i == 0 && (j == 0 || j == 19)) continue;
			Vec3f force = Vec3f(g);
			Vec3f position = particles[i][j].getPosition();
			
			force += springForce(i - 1, j, position);
			force += springForce(i + 1, j, position);
			force += springForce(i, j - 1, position);
			force += springForce(i, j + 1, position);

			if (force.length() > 10)
			{
				force.normalize();
				force *= 10;
			}

			particles[i][j].applyForce(force);
		}
	}
	
	for (int i = 0; i < 20; i++)
		for (int j = 19; j >= 0; j--)
			particles[i][j].move(t);

	bakeParticles(t);
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{
	time = t;
	if (bake_end_time == -1)
	{
		for (int i = 0; i < 19; i++)
		{
			for (int j = 0; j < 19; j++)
			{
				Vec3f p;
				glBegin(GL_QUADS);
				p = particles[i][j].getPosition();
				glVertex3f(p[0], p[1], p[2]);
				p = particles[i][j + 1].getPosition();
				glVertex3f(p[0], p[1], p[2]);
				p = particles[i + 1][j + 1].getPosition();
				glVertex3f(p[0], p[1], p[2]);
				p = particles[i + 1][j].getPosition();
				glVertex3f(p[0], p[1], p[2]);
				glEnd();
			}
		}
	}

	if (t <= bake_start_time || t >= bake_end_time) return;
	int C = bake.size() * (t - bake_start_time) / (bake_end_time - bake_start_time);
	//printf("%d : %d : %f : %f : %f\n", j, bake.size(), t, bake_start_time, bake_end_time);
	
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			Vec3f p;
			glBegin(GL_QUADS);
			p = bake[C][i][j].getPosition();
			glVertex3f(p[0], p[1], p[2]);
			p = bake[C][i][j + 1].getPosition();
			glVertex3f(p[0], p[1], p[2]);
			p = bake[C][i + 1][j + 1].getPosition();
			glVertex3f(p[0], p[1], p[2]);
			p = bake[C][i + 1][j].getPosition();
			glVertex3f(p[0], p[1], p[2]);
			glEnd();
		}
	}

}





/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) 
{
	Particle**  copy = new Particle*[20]; 
	for (int i = 0; i < 20; i++)
	{
		copy[i] = new Particle[20];
		for (int j = 0; j < 20; j++)
		{
			copy[i][j] = particles[i][j];
		}
	}
	bake.push_back(copy);
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{
	for (int i = 0; i < bake.size(); i++)
	{
		for (int j = 0; j < 20; j++) delete[] bake[i][j];
		delete[] bake[i];
	}

	bake.clear();
}





