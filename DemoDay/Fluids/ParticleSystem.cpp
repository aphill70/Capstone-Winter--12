#include "ParticleSystem.h"
#include <stdio.h>
#include <iostream>
ParticleSystem::ParticleSystem()
{
}


void ParticleSystem::initParticles(int _numParticles, glm::vec4 _initialPos)
{
	srand(time(NULL));
	for(int i = 0; i < _numParticles; i++)
	{
		int velx = (rand() % 10) - rand() % 10;
		int vely = rand() % 10 - rand() % 10;
		int velz = rand() % 10 - rand() % 10;

		this->particles.push_back(new Particle(_initialPos, glm::vec4(glm::vec3(velx, vely, velz), 0)));
	}
}

void ParticleSystem::updateSimulation()
{
	for(int i = 0; i < particles.size(); i++)
	{
		std::vector<Particle*> neighbors = getNeighbors(particles[i]);

		glm::vec4 avoidanceForce = this->calculateAvoidanceForce(particles[i], neighbors);
		glm::vec4 cohesionForce = this->calculateCohesionForce(particles[i], neighbors);
		glm::vec4 alignmentForce = this->calculateAllignmentForce(particles[i], neighbors);

		particles[i]->updateForces(avoidanceForce, cohesionForce, alignmentForce);
	}

	for(int i = 0; i < particles.size(); i++)
	{
		particles[i]->updateVelocity();
		particles[i]->updatePosition(0.1);
	}
}

float* ParticleSystem::getParticlePosition(int _index)
{
	float* pointer = new float[3];
	float result[3] = {this->particles[_index]->getPosition().x, 
					this->particles[_index]->getPosition().y, 
					this->particles[_index]->getPosition().z};

	pointer = result;
	return pointer;
}

int ParticleSystem::getNumberOfParticles()
{
	return this->particles.size();
}

std::vector<Particle*> ParticleSystem::getNeighbors(Particle* _curPart)
{
	std::vector<Particle*> result;
	for(int i = 0; i < particles.size(); i++)
	{
		if(_curPart != particles[i]	&& glm::distance(_curPart->getPosition(), particles[i]->getPosition()) < NEIGHTBOR_RADIUS)
		{
			result.push_back(particles[i]);
		}
	}

	return result;
}

glm::vec4 ParticleSystem::calculateAvoidanceForce(Particle* _curPart, std::vector<Particle*> _neighbors)
{
	glm::vec4 result;

	for(int i = 0; i < _neighbors.size(); i++)
	{
		result += _curPart->getPosition() - _neighbors[i]->getPosition();
	}

	return result;
}

glm::vec4 ParticleSystem::calculateCohesionForce(Particle* _curPart, std::vector<Particle*> _neighbors)
{
	glm::vec4 result;

	for(int i = 0; i < _neighbors.size(); i++)
	{
		result += _neighbors[i]->getPosition() / _neighbors.size();
	}

	result = result - _curPart->getPosition();

	return result;
}

glm::vec4 ParticleSystem::calculateAllignmentForce(Particle* _curPart, std::vector<Particle*> _neighbors)
{
	glm::vec4 result;

	for(int i = 0; i < _neighbors.size(); i++)
	{
		result += _neighbors[i]->getVelocity() / _neighbors.size();
	}

	return result;
}

glm::vec4 ParticleSystem::calculatePressureForce(Particle* _curPart, std::vector<Particle*> _neighbors)
{
	glm::vec4 result1;
	glm::vec4 result2;

	for(int i = 0; i < _neighbors.size(); i++)
	{
		result1 += _curPart->changeInPressure(_neighbors[i], NEIGHTBOR_RADIUS);
		result2 += _curPart->changeInPressureSansDensity(_neighbors[i], NEIGHTBOR_RADIUS);
	}

	return /*K */ 1 * _curPart->mass * ((_curPart->density - INIT_DENSITY)/ pow(_curPart->density, 2)) * (result1 + result2);
}

glm::vec4 ParticleSystem::calculateVelocityChange(Particle* _curPart, std::vector<Particle*> _neighbors)
{
	glm::vec4 result;

	for(int i = 0; i < _neighbors.size(); i++)
	{
		result += _curPart->changeInVelocity(_neighbors[i], NEIGHTBOR_RADIUS);
	}

	return result;
}