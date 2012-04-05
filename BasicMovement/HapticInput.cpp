#include <stdio.h>
#include "HapticInput.h"
#include "Globals.h"

HapticInput::HapticInput(void) : lastPosition(0, 0, 0), newPosition(0, 0, 0) {

}

HapticInput::~HapticInput(void) {}

void HapticInput::SetPosition(cVector3d& newPos) {
	newPosition.copyfrom(newPos);
}

void HapticInput::ModifyTransformations(WorldTransformation& transform) {
	clock_t curTime = clock();
	double millis = curTime - lastPrintTime;
	if (millis < 0.06) {
		return;
	}

	// The Falcon's axes are mapped very differently from normal 3D space:
	//		X movement corresponds to the Y component
	//		Y movement corresponds to the Z component
	//		Z movement corresponds to the X component
	double movementX = (newPosition.x - lastPosition.x) * HAPTIC_MOVEMENT_SCALE;
	double movementY = (newPosition.y - lastPosition.y) * HAPTIC_MOVEMENT_SCALE;
	double movementZ = (newPosition.z - lastPosition.z) * HAPTIC_MOVEMENT_SCALE;
	
	if (movementX > 0.0) {
		MoveForward(transform, movementX);
	} else {
		MoveBack(transform, movementX);
	}
	
	MoveUp(transform, movementZ);
	MoveSideways(transform, movementY);
	

	lastPosition.copyfrom(newPosition);

	RenderCenteringForce();
}

void HapticInput::MoveForward(WorldTransformation& transform, double amount) {
	double yComponent = sin(transform.rotateX * PI / 180.0) * amount;
	double xzComponent = sqrt( amount * amount - yComponent * yComponent);
	double xComponent = sin(transform.rotateY * PI / 180.0) * xzComponent;
	double zComponent = cos(transform.rotateY * PI / 180.0) * xzComponent;

	transform.xPos -= xComponent;
	transform.yPos += yComponent;
	transform.zPos += zComponent;
}

void HapticInput::MoveBack(WorldTransformation& transform, double amount) {
	double yComponent = sin(transform.rotateX * PI / 180.0) * amount;
	double xzComponent = sqrt( amount * amount - yComponent * yComponent);
	double xComponent = sin(transform.rotateY * PI / 180.0) * xzComponent;
	double zComponent = cos(transform.rotateY * PI / 180.0) * xzComponent;

	transform.xPos += xComponent;
	transform.yPos -= yComponent;
	transform.zPos -= zComponent;
}

void HapticInput::MoveLeft(WorldTransformation& transform, double amount) {
	double angle = transform.rotateY + 90;
	double xComponent = sin(angle * PI / 180.0) * amount;
	double zComponent = cos(angle * PI / 180.0) * amount;

	transform.xPos += xComponent;
	transform.zPos -= zComponent;
}

void HapticInput::MoveSideways(WorldTransformation& transform, double amount) {
	double angle = transform.rotateY - 90;
	double xComponent = sin(angle * PI / 180.0) * amount;
	double zComponent = cos(angle * PI / 180.0) * amount;

	transform.xPos += xComponent;
	transform.zPos -= zComponent;
}

void HapticInput::MoveUp(WorldTransformation& transform, double amount) {
	double angle = transform.rotateX - 90;
	double yComponent = sin(angle * PI / 180.0) * amount;
	double xzComponent = sqrt( amount * amount - yComponent * yComponent);
	double xComponent = sin(transform.rotateY * PI / 180.0) * xzComponent;
	double zComponent = cos(transform.rotateY * PI / 180.0) * xzComponent;

	transform.xPos -= xComponent;
	transform.yPos += yComponent;
	transform.zPos += zComponent;
}

void HapticInput::MoveDown(WorldTransformation& transform, double amount) {
	double angle = transform.rotateX - 90;
	double yComponent = sin(angle * PI / 180.0) * amount;
	double xzComponent = sqrt( amount * amount - yComponent * yComponent);
	double xComponent = sin(transform.rotateY * PI / 180.0) * xzComponent;
	double zComponent = cos(transform.rotateY * PI / 180.0) * xzComponent;

	transform.xPos += xComponent;
	transform.yPos -= yComponent;
	transform.zPos -= zComponent;
}

void HapticInput::RenderCenteringForce(void) {
	//Linear increase of force
	cVector3d centeringForce;
	centeringForce.copyfrom(lastPosition);
	centeringForce.negate();

	double magnitude = centeringForce.length();
	double desiredMagnitude = (magnitude / HAPTIC_RADIUS) * MAX_HAPTIC_FORCE;
	centeringForce.mul(desiredMagnitude / magnitude);
	
	cVector3d linearVelocity;
	hapticDevice->getLinearVelocity(linearVelocity);
	double dotProduct = linearVelocity.dot(centeringForce);
	//printf("Linear Velocity (magnitude): %f\n", linearVelocity.length());
	printf("dot product of force to render with current velocity: %f\n", dotProduct);
	if (linearVelocity.length() > 0.005) {	// Make sure it isn't going fast already
		//centeringForce.mul(0.5);
		//centeringForce.negate();
	}
	
	hapticDevice->setForce(centeringForce);
}