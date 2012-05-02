#pragma warning(disable : 4786)

#include "particlesystem.h"


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>

const Vec3f ParticleSystem::g = Vec3f(0, -9.8, 0);

/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem() 
{
	time = 0;
	last_particle = 0;
}



/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{
	// TODO

}

void ParticleSystem::addParticle(Vec4f p)
{
	if (time - last_particle > 0.01)
	{
		last_particle = time;
		particles.push_back(Particle(p[0], p[1], p[2], time));
	}
}


/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{
    
	// TODO

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
    
	// TODO

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{
    
	// TODO

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{
	time = t;
	if (!simulate) return;

	for (int i = 0; i < particles.size(); i++)
	{
		if (!particles[i].too_far()) particles[i].applyForce(g, t);
	}
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{
	time = t;
	for (int i = 0; i < particles.size(); i++)
	{
		//printf("%d size: %d\n", i, particles.size());
		if (!particles[i].too_far()) particles[i].draw();
	}
}





/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) 
{

	// TODO
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{

	// TODO
}





