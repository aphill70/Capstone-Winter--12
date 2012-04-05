#pragma once

#include "chai3d.h"

#include "IFluidParticle.h"

class BasicFluidParticle : public IFluidParticle {
protected:
	cVector3d _position, _velocity;
public:
	BasicFluidParticle();
	BasicFluidParticle(const cVector3d& position, const cVector3d& velocity);
	virtual ~BasicFluidParticle();

	// Copies this particle's position into the vector 'position'
	virtual void GetPosition(cVector3d& position);
	// Copies this particle's velocity into the vector 'velocity'
	virtual void GetVelocity(cVector3d& velocity);
};