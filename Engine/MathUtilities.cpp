#include "MathUtilities.h"

float MathUtilities::ToRadians(float degree) {
	return degree * (float)PI / 180;
}

float MathUtilities::WrapAngleRad(float radians) {
	const float tau = 2 * PI;
	while (radians > tau) {
		radians -= tau;
	}
	while (radians < 0) {
		radians += tau;
	}
	return radians;
}
