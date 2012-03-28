#include "particle.h"

Particle::Particle()
{
}

Particle::Particle(glm::vec4 _pos, glm::vec4 _vel)
{
	this->mass = 1;
	this->position = _pos;
	this->velocity = _vel;
}

glm::vec4 Particle::getVelocity()
{
	return this->velocity;
}

void Particle::setVelocity(glm::vec4 _vel)
{
	this->velocity = _vel;
}

glm::vec4 Particle::getPosition()
{
	return this->position;
}

void Particle::setPosition(glm::vec4 _pos)
{
	this->position = _pos;
}

void Particle::updateForces(glm::vec4 _forceAvoid, glm::vec4 _forceCohesion, glm::vec4 _forceAlign)
{
	this->forceAvoid = _forceAvoid;
	this->forceCohesion = _forceCohesion;
	this->forceAlign = _forceAlign;
}

void Particle::updatePosition(float deltaT)
{
	this->position = this->position + (deltaT * this->velocity);
}

void Particle::updateVelocity()
{
	this->velocity = ALPHA_AVOIDANCE * this->forceAvoid + 
					BETA_COHESION * this->forceCohesion + 
					GAMMA_ALIGNMENT * this->forceAlign;
}

double Particle::distance(Particle* other)
{
	return sqrt( pow((this->position.x - other->position.x), 2) + 
		pow((this->position.y - other->position.y), 2) + 
		pow((this->position.z - other->position.z), 2));
}

glm::vec4 Particle::gradientSmoothingKernel(Particle* other, double h)
{
	double dist = this->distance(other);

	double q = dist/h;

	if(0 < q && q < 1)
	{
		return other->getPosition() * ((1/(4 * M_PI * pow(h, 5))) * (9*q - 12));
	}
	else if(1 <= q && q < 2)
	{
		return other->getPosition() * ((1/(4 * dist * M_PI * pow(h, 4))) * pow((2 - q), 2));
	}
	else
		return other->getPosition() * 0;
}

double Particle::smoothingKernel(Particle* other, double h)
{
	double dist = 0;

	dist = this->distance(other);

	return this->smoothingKernel(dist, h);
}

double Particle::smoothingKernel(double distance, double h)
{
	double front = (1 / (4 * pow(h, 3) * M_PI));
	double q = distance / h;

	if(0 <= q && q <= 1)
	{
		return front * (pow((2- q), 3) - 4 * pow((1 - q), 3));

	}
	else if(1 <= q && q <= 2)
	{
		return front * (4 * pow((1 - q), 3));
	}
	else
	{
		return 0;
	}
}

glm::vec4 Particle::changeInVelocity(Particle* other, double h)
{
	other->mass * (this->pressure + other->pressure) / (this->density * other->density) * this->gradientSmoothingKernel(other, h) - GRAVITY;

	return glm::vec4();
}


glm::vec4 Particle::changeInPressure(Particle* other, double h)
{
	return ((other->density - INIT_DENSITY) / pow(other->density, 2)) * this->gradientSmoothingKernel(other, h);
}

glm::vec4 Particle::changeInPressureSansDensity(Particle* other, double h)
{
	return other->mass * this->gradientSmoothingKernel(other, h);

}