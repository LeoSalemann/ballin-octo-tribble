#ifndef Autonomous_H
#define Autonomous_H

#include <Commands/Automatic/BestDrive.h>
#include "Commands/CommandGroup.h"
#include "WPILib.h"

class Autonomous: public CommandGroup {
public:
	static Autonomous *createDriveDistance(float distance,
			BestDrive::Direction direction);
	static Autonomous *createDriveDuration(float duration, float heading);
	static Autonomous *createTurnTo(double targetAngle);
	static Autonomous *createTripleTote();
	static Autonomous *createTurningTripleTote();
	static Autonomous *createStartWithCan();
	static Autonomous *createStartWithCanThenDrive();
	static Autonomous *createSimpleDriveForward();
	static Autonomous *createGetCenterCan();

	Autonomous();
	Autonomous(char *style);
	Autonomous(int argc, char **argv);
	virtual void Initialize();
	~Autonomous();
};

#endif
