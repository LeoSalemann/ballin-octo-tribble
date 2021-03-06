#include "StallableMotor.h"

StallableMotor::StallableMotor(PIDSource *input, float currentThreshold,
		CANTalon *motor, CANTalon *slaveMotor) {
	this->motor = motor;
	this->slaveMotor = slaveMotor;
	this->currentThreshold = currentThreshold;
	this->input = input;
	usingCANTalon = false;
	directionSwitch = false;
	directionMag = 0;
	pdirectionMag = 0;
	stalledStart = false;
	ThreadInit();
}

StallableMotor::~StallableMotor() {
	delete motor;
	delete slaveMotor;
}

StallableMotor::StallableMotor(CANTalon *motor, float currentThreshold) {
	this->motor = motor;
	this->currentThreshold = currentThreshold;
	usingCANTalon = true;
	ThreadInit();
}

void StallableMotor::ThreadInit() {
	pthread_create(&stallThread, NULL, &InitHelper, (void*) this);
}

void StallableMotor::ThreadKill() {
	pthread_exit(&stallThread);
}

void* StallableMotor::InitHelper(void *classref) {
	return ((StallableMotor*) classref)->StallCheck(NULL);
}

void* StallableMotor::StallCheck(void*) {
	float prevPosition = 0;
	float startTime = 0;
	while (420) {
		if (motor->GetOutputCurrent() > currentThreshold) {
			float currentPosition;
			if (usingCANTalon) {
				currentPosition = motor->GetEncPosition();
			} else {
				currentPosition = input->PIDGet();
			}
			if (abs(currentPosition - prevPosition) < STALLABLE_MOVE_THRESHOLD) {
				SmartDashboard::PutNumber("Position Difference",
						currentPosition - prevPosition);
				if (startTime == 0) {
					startTime = getTime();
				}
				double dtime = getTime() - startTime;
				SmartDashboard::PutNumber("Stall Time", dtime);
				if (dtime >= STALLABLE_TIME_STOP) {
					stalled = true;
//					stalledStart = true;
				}
			} else {
				if (startTime != 0) {
					startTime = 0;
					stalled = false;
				}
			}
			prevPosition = currentPosition;
		} else {
//			if (startTime != 0) {
//				if ((1 / pdirectionMag) * pdirectionMag
//						== (1 / directionMag) * directionMag) {
			startTime = 0;
			stalled = false;
//				}
//			}
		}
	}
}

void StallableMotor::PIDWrite(float output) {
	if (!stalled) {
		motor->Set(output);
		if (slaveMotor != NULL) {
			slaveMotor->Set(-output);
		}
	} else if (stalled) {
//		if (stalledStart == true) {
//			pdirectionMag = (1 / output) * output;
//			stalledStart = false;
//		}
//		directionMag = (1 / output) * output;

		motor->SetControlMode(CANSpeedController::kPercentVbus);
		motor->Set(0);
		motor->Disable();
		if (slaveMotor != NULL) {
			slaveMotor->SetControlMode(CANSpeedController::kPercentVbus);
			slaveMotor->Set(0);
			slaveMotor->Disable();
		}
	}
}

unsigned long StallableMotor::getTime() {
	using namespace std::chrono;
	unsigned long ms = duration_cast<milliseconds>(
			high_resolution_clock::now().time_since_epoch()).count();
	return ms;
}
