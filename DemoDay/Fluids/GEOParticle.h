#pragma once

#include "chai3d.h"

#include "IFluidParticle.h"

class GEOParticle : public IFluidParticle {
protected:
	int _id;
	cVector3d _position, _velocity;
public:
	GEOParticle(int id, cVector3d& position, cVector3d& velocity);
	virtual ~GEOParticle(void);

	int GetID(void);

	// Copies this particle's position into the vector 'position'
	virtual void GetPosition(cVector3d& position);
	virtual double GetPositionX(void);
	virtual double GetPositionY(void);
	virtual double GetPositionZ(void);
	// Copies this particle's velocity into the vector 'velocity'
	virtual void GetVelocity(cVector3d& velocity);
	virtual double GetVelocityX(void);
	virtual double GetVelocityY(void);
	virtual double GetVelocityZ(void);
};