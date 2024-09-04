//
// Created by mateusz on 9/16/23.
//

#include "Base.hpp"

namespace mk::Math {
	float radiansToDegrees(float radians) {
		return makeInRange(radians, 2 * M_PIf) / M_PIf * 180.f;
	}

	float makeInRange(float value, float range) {
		float res = std::fmod(value, range);
		if (res > 0) return res;
		return res + value;
	}

	float degreesToRadians(float degrees) {
		return makeInRange(degrees, 360.f) / 180.f * M_PIf;
	}
}  // namespace mk::Math
