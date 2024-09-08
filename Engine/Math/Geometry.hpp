#pragma once

#include "Vector2.hpp"

#include <vector>

namespace mk::math {
	bool doLinesIntersect(
		Vector2f lineAStart,
		Vector2f lineAEnd,
		Vector2f LineBStart,
		Vector2f LineBEnd
	);
	Vector2f findLineIntersection(
		Vector2f lineAStart,
		Vector2f lineAEnd,
		Vector2f LineBStart,
		Vector2f LineBEnd
	);

	std::vector<Vector2i> drawLine(Vector2i start, Vector2i end);
}
