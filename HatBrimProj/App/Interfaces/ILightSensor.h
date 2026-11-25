/*
 * ILightSensor.h
 */

#ifndef INC_ILIGHTSENSOR_H_
#define INC_ILIGHTSENSOR_H_

class ILightSensor {
public:
	virtual int read() = 0;  // light strength of one sensor
};

#endif /* INC_ILIGHTSENSOR_H_ */
