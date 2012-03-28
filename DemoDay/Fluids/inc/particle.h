#ifndef PARTICLE_H
#define PARTICLE_H

#define _USE_MATH_DEFINES 
#define INIT_DENSITY = 1

#include <glm/glm.hpp>
#include <math.h>

#define ALPHA_AVOIDANCE .33
#define BETA_COHESION .33
#define GAMMA_ALIGNMENT .33

#define GRAVITY 9.8

class Particle{
private: 
	glm::vec4 position;
	glm::vec4 velocity;

	glm::vec4 forceAvoid;
	glm::vec4 forceCohesion;
	glm::vec4 forceAlign;

	double distance(Particle* other);

	double pressure;
	double mass;
	double density;

public:

	Particle();

	Particle(glm::vec4 _pos, glm::vec4 _vel);

	glm::vec4 getVelocity();

	double getMass();

	double getDensity();

	void setVelocity(glm::vec4 _vel);

	glm::vec4 getPosition();

	void setPosition(glm::vec4 _pos);

	void updateForces(glm::vec4 _forceAvoid, glm::vec4 _forceCohesion, glm::vec4 _forceAlign);

	void updatePosition(float deltaT);

	void updateVelocity();

	glm::vec4 Particle::changeInVelocity(Particle* other, double h);
	glm::vec4 Particle::changeInPressure(Particle* other, double h);

	double smoothingKernel(Particle* vector, double h);
	double smoothingKernel(double distance, double h);

	glm::vec4 Particle::gradientSmoothingKernel(Particle* other, double h);
	glm::vec4 Particle::changeInPressureSansDensity(Particle* other, double h);


};
#endif