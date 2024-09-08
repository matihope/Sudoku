#pragma once

#include <cmath>
#include <ostream>

namespace mk::math {

	template<typename T>
	requires std::is_arithmetic_v<T> struct Vector2 {
		Vector2() = default;

		template<typename K1, typename K2>
		Vector2(K1 x, K2 y): x(x), y(y) {}

		/**
		 * @brief This is a weird constructor and it's useful if we want to cast from other types.
		 */
		template<typename K>
		explicit Vector2(K obj): x(obj.x), y(obj.y) {}

		T x = 0;
		T y = 0;

		bool operator==(const Vector2<T>& rhs) const { return x == rhs.x && y == rhs.y; }

		Vector2<T>& operator+=(const Vector2<T>& rhs) {
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		Vector2<T>& operator-=(const Vector2<T>& rhs) {
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}

		Vector2<T>& operator*=(const Vector2<T>& rhs) {
			x *= rhs.x;
			y *= rhs.y;
			return *this;
		}

		Vector2<T>& operator/=(const Vector2<T>& rhs) {
			x /= rhs.x;
			y /= rhs.y;
			return *this;
		}

		template<class X>
		Vector2<T>& operator*=(const X& rhs) {
			x *= rhs;
			y *= rhs;
			return *this;
		}

		template<class X>
		Vector2<T>& operator/=(const X& rhs) {
			x *= rhs;
			y *= rhs;
			return *this;
		}

		template<class X>
		Vector2<T>& operator+=(const X& rhs) {
			x += rhs;
			y += rhs;
			return *this;
		}

		template<class X>
		Vector2<T>& operator-=(const X& rhs) {
			x -= rhs;
			y -= rhs;
			return *this;
		}

		Vector2<T> operator+(const Vector2<T>& rhs) const { return { x + rhs.x, y + rhs.y }; }

		Vector2<T> operator-(const Vector2<T>& rhs) const { return { x - rhs.x, y - rhs.y }; }

		Vector2<T> operator*(const Vector2<T>& rhs) const { return { x * rhs.x, y * rhs.y }; }

		Vector2<T> operator/(const Vector2<T>& rhs) const { return { x / rhs.x, y / rhs.y }; }

		template<class X>
		Vector2<T> operator*(const X& rhs) const {
			return { x * rhs, y * rhs };
		}

		template<class X>
		Vector2<T> operator/(const X& rhs) const {
			return { x / rhs, y / rhs };
		}

		T length() const { return std::sqrt(length_squared()); }

		T length_squared() const { return x * x + y * y; }

		template<class X>
		Vector2<X> type() const {
			return { (X) x, (X) y };
		}

		template<class X>
		X as() const {
			return X(x, y);
		}

		friend std::ostream& operator<<(std::ostream& stream, const Vector2<T>& vector) {
			stream << vector.x << ", " << vector.y;
			return stream;
		}
	};

	using Vector2f = Vector2<float>;
	using Vector2i = Vector2<int>;
	using Vector2u = Vector2<unsigned int>;

	Vector2f normalizeVector(Vector2f vector);
	Vector2f rotateVector(Vector2f vector, float angleRads);
}  // namespace mk::Math
