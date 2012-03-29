#include "BasicFluidParticle.h"

BasicFluidParticle::BasicFluidParticle() : _position(0, 0, 0), _velocity(0, 0, 0) {
}

BasicFluidParticle::BasicFluidParticle(const cVector3d& position, const cVector3d& velocity) : 
	_position(position), _velocity(velocity)
{
}

BasicFluidParticle::~BasicFluidParticle() {}

void BasicFluidParticle::GetPosition(cVector3d& position) {
	position.copyfrom(_position);
}

void BasicFluidParticle::GetVelocity(cVector3d& velocity) {
	velocity.copyfrom(_velocity);
}