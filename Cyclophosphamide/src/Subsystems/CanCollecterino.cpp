#include <Subsystems/CanCollecterino.h>
#include "../RobotMap.h"
#include <Math.h>
#include "../utilities/StallableMotor.h"

CanCollecterino::CanCollecterino() :
		Subsystem("CanCollecterino") {
	SAFE_INIT(CAN_MOTOR_LIFT_LEFT_PORT,
			liftMotorLeft = new CAN_MOTOR_TYPE(CAN_MOTOR_LIFT_LEFT_PORT););
	SAFE_INIT(CAN_MOTOR_LIFT_RIGHT_PORT,
			liftMotorRight = new CAN_MOTOR_TYPE(CAN_MOTOR_LIFT_RIGHT_PORT););

	SAFE_INIT(CAN_LIFT_POT_PORT, liftPot = new AnalogInput(CAN_LIFT_POT_PORT););

	SAFE_INIT(CAN_SENSOR_PORT, canSensor = new DigitalInput(CAN_SENSOR_PORT););

	armPID = new PIDController(CAN_ARM_P, CAN_ARM_I, CAN_ARM_D, liftPot,
			new StallableMotor(liftPot, 0.8, liftMotorRight, liftMotorLeft));
	armPID->SetOutputRange(-.7, .7);
	armPID->SetInputRange(CAN_POT_DOWN_POSITION, CAN_POT_UP_POSITION);
	armPID->SetPercentTolerance(5);

	toggleArms = true;
}

CanCollecterino::~CanCollecterino() {
	delete liftMotorLeft;
	delete liftMotorRight;
	delete canSensor;
	delete armPID;
}

void CanCollecterino::InitDefaultCommand() {

}

void CanCollecterino::setArms(float value) {
	setpoint = value;
	armPID->SetSetpoint(value);
	armPID->Enable();
}

AnalogInput *CanCollecterino::getLiftPot() {
	return liftPot;
}

void CanCollecterino::disableArms() {
	armPID->Disable();
}

bool CanCollecterino::armsWithinBounds() {
	return armPID->OnTarget();
}

PIDController *CanCollecterino::getArmPID() {
	return armPID;
}

void CanCollecterino::doTheToggleArms() {
	if (toggleArms) {
		toggleArms = false;
	} else {
		toggleArms = true;
	}
}

bool CanCollecterino::getToggleArms() {
	return toggleArms;
}

void CanCollecterino::getDatStatus() {

}

double CanCollecterino::getSetpoint() {
	return setpoint;
}
