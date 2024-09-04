//
// Created by mateusz on 10/2/23.
//

#pragma once

#include <Math/Vector2.hpp>

namespace mk {
	template<class T>
	class Rect {
	public:
		T left, top, width, height;

		[[nodiscard]] Math::Vector2<T> getPosition() const {
			return { left, top };
		}

		[[nodiscard]] Math::Vector2<T> getSize() const {
			return { width, height };
		}

		template<class X>
		bool overlaps(const Rect<X> &otherRect) {
			if (left + width < otherRect.left) return false;
			if (otherRect.left + otherRect.width < left) return false;
			if (top + height < otherRect.top) return false;
			if (otherRect.top + otherRect.height < top) return false;
			return true;
		}
	};

	using RectI = Rect<int>;
	using RectF = Rect<float>;
	using RectU = Rect<unsigned int>;
}  // namespace mk
