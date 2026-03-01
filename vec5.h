#pragma once

#include <stdexcept>
#include <cstdint>
#include <cstdlib>
#include <glm/glm.hpp>

namespace m5
{
	template<typename T = float>
	struct Tvec5
	{
		T x{}, y{}, z{}, w{}, v{};

		Tvec5() {}
		Tvec5(T v)
			: x(v), y(v), z(v), w(v), v(v) {}
		Tvec5(T x, T y, T z, T w, T v)
			: x(x), y(y), z(z), w(w), v(v) {}
		Tvec5(const glm::vec<4, T>& xyzw, T v = {})
			: x(xyzw.x), y(xyzw.y), z(xyzw.z), w(xyzw.w), v(v) {}

		Tvec5(const Tvec5<T>& other)
			: x(other.x), y(other.y), z(other.z), w(other.w), v(other.v) {}
		Tvec5(Tvec5<T>&& other) noexcept
			: x(other.x), y(other.y), z(other.z), w(other.w), v(other.v)
		{
			other.x = {};
			other.y = {};
			other.z = {};
			other.w = {};
			other.v = {};
		}
		Tvec5<T>& operator=(const Tvec5<T>& other)
		{
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			this->w = other.w;
			this->v = other.v;

			return *this;
		}
		Tvec5<T>& operator=(Tvec5<T>&& other) noexcept
		{
			if (this != &other)
			{
				this->x = other.x;
				this->y = other.y;
				this->z = other.z;
				this->w = other.w;
				this->v = other.v;
				other.x = {};
				other.y = {};
				other.z = {};
				other.w = {};
				other.v = {};
			}

			return *this;
		}

		T& operator[](size_t i)
		{
			if (i >= 5) throw std::out_of_range("m5::vec5::operator[]: index out of range.");

			return (&x)[i];
		}
		const T& operator[](size_t i) const
		{
			if (i >= 5) throw std::out_of_range("m5::vec5::operator[]: index out of range.");

			return (&x)[i];
		}

		Tvec5<T> operator+(const Tvec5<T>& other) const
		{
			return Tvec5<T>
			{
				this->x + other.x,
				this->y + other.y,
				this->z + other.z,
				this->w + other.w,
				this->v + other.v
			};
		}
		Tvec5<T> operator+(T v) const
		{
			return Tvec5<T>
			{
				this->x + v,
				this->y + v,
				this->z + v,
				this->w + v,
				this->v + v
			};
		}
		Tvec5<T> operator-(const Tvec5<T>& other) const
		{
			return Tvec5<T>
			{
				this->x - other.x,
				this->y - other.y,
				this->z - other.z,
				this->w - other.w,
				this->v - other.v
			};
		}
		Tvec5<T> operator-(T v) const
		{
			return Tvec5<T>
			{
				this->x - v,
				this->y - v,
				this->z - v,
				this->w - v,
				this->v - v
			};
		}
		Tvec5<T> operator-() const
		{
			return Tvec5<T>
			{
				-this->x,
				-this->y,
				-this->z,
				-this->w,
				-this->v
			};
		}
		Tvec5<T> operator*(const Tvec5<T>& other) const
		{
			return Tvec5<T>
			{
				this->x * other.x,
				this->y * other.y,
				this->z * other.z,
				this->w * other.w,
				this->v * other.v
			};
		}
		Tvec5<T> operator*(T v) const
		{
			return Tvec5<T>
			{
				this->x * v,
				this->y * v,
				this->z * v,
				this->w * v,
				this->v * v
			};
		}
		Tvec5<T> operator/(const Tvec5<T>& other) const
		{
			return Tvec5<T>
			{
				this->x / other.x,
				this->y / other.y,
				this->z / other.z,
				this->w / other.w,
				this->v / other.v
			};
		}
		Tvec5<T> operator/(T v) const
		{
			float invV = 1.0f / v;
			return Tvec5<T>
			{
				this->x * invV,
				this->y * invV,
				this->z * invV,
				this->w * invV,
				this->v * invV
			};
		}

		Tvec5<T>& operator+=(const Tvec5<T>& other)
		{
			return *this = *this + other;
		}
		Tvec5<T>& operator+=(T v)
		{
			return *this = *this + v;
		}
		Tvec5<T>& operator-=(const Tvec5<T>& other)
		{
			return *this = *this - other;
		}
		Tvec5<T>& operator-=(T v)
		{
			return *this = *this - v;
		}
		Tvec5<T>& operator*=(const Tvec5<T>& other)
		{
			return *this = *this * other;
		}
		Tvec5<T>& operator*=(T v)
		{
			return *this = *this * v;
		}
		Tvec5<T>& operator/=(const Tvec5<T>& other)
		{
			return *this = *this / other;
		}
		Tvec5<T>& operator/=(T v)
		{
			return *this = *this / v;
		}

		bool operator==(const Tvec5<T>& other) const
		{
			return this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w && this->v == other.v;
		}

		T dot(const Tvec5<T>& other) const
		{
			return x * other.x + y * other.y + z * other.z + w * other.w + v * other.v;
		}
		T length2() const
		{
			return dot(*this);
		}
		T length() const
		{
			return glm::sqrt(length2());
		}
		Tvec5<T>& normalize()
		{
			float len = length2();
			if (len <= glm::epsilon<T>() * glm::epsilon<T>())
			{
				return *this;
			}
			return *this /= glm::sqrt(len);
		}
		Tvec5<T> normalized() const
		{
			Tvec5<T> v = *this;
			v.normalize();
			return v;
		}

		glm::vec<4, T>& xyzw()
		{
			return *(glm::vec<4, T>*)&x;
		}
		const glm::vec<4, T>& xyzw() const
		{
			return *(const glm::vec<4, T>*)&x;
		}
	};

	using vec5 = Tvec5<float>;
	using i8vec5 = Tvec5<int8_t>;
	using i16vec5 = Tvec5<int16_t>;
	using i32vec5 = Tvec5<int32_t>;
	using ivec5 = i32vec5;
	using i64vec5 = Tvec5<int64_t>;
	using u8vec5 = Tvec5<uint8_t>;
	using u16vec5 = Tvec5<uint16_t>;
	using u32vec5 = Tvec5<uint32_t>;
	using uvec5 = u32vec5;
	using u64vec5 = Tvec5<uint64_t>;

	inline float dot(const vec5& a, const vec5& b) { return a.dot(b); }
	inline vec5 normalize(const vec5& v) { return v.normalized(); }
	inline float length(const vec5& v) { return v.length(); }
	inline float length2(const vec5& v) { return v.length2(); }
	inline float distance(const vec5& a, const vec5& b) { return (b - a).length(); }
	inline float distance2(const vec5& a, const vec5& b) { return (b - a).length2(); }
}
