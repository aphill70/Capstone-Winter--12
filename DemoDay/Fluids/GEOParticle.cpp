#include "GEOParticle.h"

GEOParticle::GEOParticle(int id, cVector3d& position, cVector3d& velocity) : 
	_id(id), _position(position), _velocity(velocity) 
{
}

	GEOParticle::GEOParticle(const GEOParticle& other) : _id(other._id), _position(other._position), _velocity(other._velocity)
{
}

GEOParticle::~GEOParticle(void) {
}

int GEOParticle::GetID(void){
	return _id;
}

void GEOParticle::GetPosition(cVector3d& position){
	position.copyfrom(_position);
}

double GEOParticle::GetPositionX(void) { return _position.x; }
double GEOParticle::GetPositionY(void) { return _position.y; }
double GEOParticle::GetPositionZ(void) { return _position.z; }

void GEOParticle::GetVelocity(cVector3d& velocity){
	velocity.copyfrom(_velocity);
}

double GEOParticle::GetVelocityX(void) { return _velocity.x; }
double GEOParticle::GetVelocityY(void) { return _velocity.y; }
double GEOParticle::GetVelocityZ(void) { return _velocity.z; }