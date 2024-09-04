//
// Created by mateusz on 9/16/23.
//

#include "Geometry.hpp"

#include "Base.hpp"
#include "Vector2.hpp"

#include <vector>

namespace mk::Math {

	float _dotProduct(const Vector2f &vec1, const Vector2f &vec2) {
		return vec1.x * vec2.x + vec1.y * vec2.y;
	}

	// here vectors are points
	float _determinant(
		const Vector2f &tail, const Vector2f &head1, const Vector2f &head2
	) {
		return (head1.x - tail.x) * (head2.y - tail.y)
		     - (head2.x - tail.x) * (head1.y - tail.y);
	}

	bool isPointInsideConvex(
		const std::vector<Vector2f> &convex, const Vector2f &point
	) {
		if (convex.size() < 3) return false;

		bool has_neg = false;
		bool has_pos = false;
		for (std::size_t i = 0; i < convex.size(); i++) {
			int now_sign = sign(
				_determinant(point, convex[i], convex[(i + 1) % convex.size()])
			);
			if (now_sign == -1) has_neg = true;
			if (now_sign == 1) has_pos = true;
			if (has_neg && has_pos) return false;
		}
		return true;
	}

	Vector2f _getPerpendicular(const Vector2f &vec) {
		return { -vec.y, vec.x };
	}

	bool doShapesIntersect(
		const std::vector<Vector2f> &shape1, const std::vector<Vector2f> &shape2
	) {
		const std::vector<Vector2f> *s1 = &shape1;
		const std::vector<Vector2f> *s2 = &shape2;
		for (size_t t = 0; t < 2; ++t) {
			if (t == 1) {
				s1 = &shape2;
				s2 = &shape1;
			}
			for (size_t i = 0; i < s1->size(); i++) {
				Vector2f perpendicular = _getPerpendicular(Vector2f(
					(*s1)[(i + 1) % s1->size()].x - (*s1)[i].x,
					(*s1)[(i + 1) % s1->size()].y - (*s1)[i].y
				));
				float    min1          = FLOAT_INFINITY;
				float    max1          = -FLOAT_INFINITY;
				for (auto j : *s1) {
					float dp = _dotProduct(perpendicular, j);
					min1     = std::min(min1, dp);
					max1     = std::max(max1, dp);
				}
				float min2 = FLOAT_INFINITY;
				float max2 = -FLOAT_INFINITY;
				for (auto j : *s2) {
					float dp = _dotProduct(perpendicular, j);
					min2     = std::min(min2, dp);
					max2     = std::max(max2, dp);
				}
				if (max2 < min1 || min2 > max1) return false;
			}
		}
		return true;
	}

	Vector2f findLineIntersection(
		Vector2f p1, Vector2f p2, Vector2f p3, Vector2f p4
	) {
		float px
			= ((p1.x * p2.y - p1.y * p2.x) * (p3.x - p4.x)
		       - (p1.x - p2.x) * (p3.x * p4.y - p3.y * p4.x))
		    / ((p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x));

		float py
			= ((p1.x * p2.y - p1.y * p2.x) * (p3.y - p4.y)
		       - (p1.y - p2.y) * (p3.x * p4.y - p3.y * p4.x))
		    / ((p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x));

		return { px, py };
	}

	std::vector<Vector2i> drawLine(Vector2i start, Vector2i end) {
		if (start == end) return { start };

		Math::Vector2i currentPosition = start;
		Math::Vector2f dirVec = normalizeVector((end - start).type<float>());
		Math::Vector2f step   = { dirVec.y / dirVec.x, dirVec.x / dirVec.y };
		Math::Vector2f stepLength  = { std::sqrt(step.x * step.x + 1),
			                           std::sqrt(step.y * step.y + 1) };
		Math::Vector2f rayProgress = { 0, 0 };

		float distance    = 0;
		float maxDistance = (end - start).length();

		std::vector<Math::Vector2i> points;

		while (distance < maxDistance) {
			points.push_back(currentPosition);
			if (rayProgress.x < rayProgress.y) {
				currentPosition.x += Math::sign(dirVec.x);
				distance = rayProgress.x;
				rayProgress.x += stepLength.x;
			} else {
				currentPosition.y += Math::sign(dirVec.y);
				distance = rayProgress.y;
				rayProgress.y += stepLength.y;
			}
		}

		return points;
	}

}  // namespace mk::Math
