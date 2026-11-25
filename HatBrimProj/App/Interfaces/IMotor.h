/*
 * IMotor.h
 */

#ifndef INC_IMOTOR_H_
#define INC_IMOTOR_H_

class IMotor {
public:
	virtual void drive(float speed) = 0;  // -1.0 = left, 0 = stop, +1.0 = right
	virtual void stop() = 0;
};

#endif /* INC_IMOTOR_H_ */
