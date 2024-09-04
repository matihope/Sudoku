#include "Vector2.hpp"

namespace mk::Math {
	Vector2f normalizeVector(const Vector2f vector) {
		if (vector.x == 0 || vector.y == 0) return vector;
		float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
		return { vector.x / length, vector.y / length };
	}

	Vector2f rotateVector(Vector2f vector, float angleRads) {
		return {
			std::cos(angleRads) * vector.x - std::sin(angleRads) * vector.y,
			std::sin(angleRads) * vector.x + std::cos(angleRads) * vector.y
		};
	}
}  // namespace mk::Math
