/*
 * HatController.h
 */

#ifndef INC_HATCONTROLLER_H_
#define INC_HATCONTROLLER_H_

#include "Logic/LightSensing.h"
#include "Logic/FollowLightStrategy.h"
#include "Interfaces/IMotor.h"

class HatController {
public:
	enum class State {
		INIT,
		FOLLOW,
	};

	enum class Event {
		INIT_COMPLETE,
	};

	HatController(LightSensing& lightSensing, FollowLightStrategy& followStrategy, IMotor& motor);

	void handleEvent(Event ev);
	void update();

private:
	LightSensing& lightSensing;
	FollowLightStrategy& followStrategy;
	IMotor& motor;
	volatile State state;        // for Live Expressions
};

#endif /* INC_HATCONTROLLER_H_ */
