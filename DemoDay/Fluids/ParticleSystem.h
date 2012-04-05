#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "glm/glm.hpp"
#include <GL/glut.h>

#include "particle.h"

#define NEIGHTBOR_RADIUS 100

class ParticleSystem{
private:
	std::vector<Particle*> particles;

public:
	ParticleSystem();

	void initParticles(int _numParticles, glm::vec4 position);
	void updateSimulation();

	float* getParticlePosition(int _index);
	int getNumberOfParticles();

	friend class Particle;
private:
	std::vector<Particle*> getNeighbors(Particle* _curPart);

	glm::vec4 calculateAvoidanceForce(Particle* _curPart, std::vector<Particle*> _neighbors);
	glm::vec4 calculateCohesionForce(Particle* _curPart, std::vector<Particle*> _neighbors);
	glm::vec4 calculateAllignmentForce(Particle* _curPart, std::vector<Particle*> _neighbors);

	glm::vec4 calculatePressureForce(Particle* _curPart, std::vector<Particle*> _neighbors);
	glm::vec4 calculateVelocityChange(Particle* _curPart, std::vector<Particle*> _neighbors);
};

#endif