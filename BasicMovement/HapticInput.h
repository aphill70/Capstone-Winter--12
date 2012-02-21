#include <ctime>

#include "chai3d.h"
#include "IControlInput.h"
#include "WorldTransformation.h"

#define HAPTIC_MOVEMENT_SCALE 100.0

class HapticInput : public IControlInput {
private:
	// position of the cursor in the last frame and in this frame (new becomes last once ModifyTransformations is called)
	cVector3d lastPosition, newPosition;

	//
	clock_t lastPrintTime;
	//

	void MoveForward(WorldTransformation& transform, double amount);
	void MoveBack(WorldTransformation& transform, double amount);
	void MoveLeft(WorldTransformation& transform, double amount);
	void MoveSideways(WorldTransformation& transform, double amount);
	void MoveUp(WorldTransformation& transform, double amount);
	void MoveDown(WorldTransformation& transform, double amount);

	void RenderCenteringForce(void);
public:
	HapticInput(void);
	~HapticInput(void);

	void SetPosition(cVector3d&);
	virtual void ModifyTransformations(WorldTransformation& transform);
};