#pragma once

#include <stdexcept>
#include <cstdint>
#include <cstdlib>
#include <glm/glm.hpp>
#include "vec5.h"

namespace m5
{
	template<typename T = float>
	struct Tvec6
	{
		T x{}, y{}, z{}, w{}, v{}, u{};

		Tvec6() {}
		Tvec6(T v)
			: x(v), y(v), z(v), w(v), v(v), u(v) {}
		Tvec6(T x, T y, T z, T w, T v, T u)
			: x(x), y(y), z(z), w(w), v(v), u(u) {}
		Tvec6(const glm::vec<4, T>& xyzw, T v = {}, T u = {})
			: x(xyzw.x), y(xyzw.y), z(xyzw.z), w(xyzw.w), v(v), u(u) {}
		Tvec6(const Tvec5<T>& xyzwv, T u = {})
			: x(xyzwv.x), y(xyzwv.y), z(xyzwv.z), w(xyzwv.w), v(xyzwv.v), u(u) {}

		Tvec6(const Tvec6<T>& other)
			: x(other.x), y(other.y), z(other.z), w(other.w), v(other.v), u(other.u) {}
		Tvec6(Tvec6<T>&& other) noexcept
			: x(other.x), y(other.y), z(other.z), w(other.w), v(other.v), u(other.u)
		{
			other.x = {};
			other.y = {};
			other.z = {};
			other.w = {};
			other.v = {};
			other.u = {};
		}
		Tvec6<T>& operator=(const Tvec6<T>& other)
		{
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			this->w = other.w;
			this->v = other.v;
			this->u = other.u;

			return *this;
		}
		Tvec6<T>& operator=(Tvec6<T>&& other) noexcept
		{
			if (this != &other)
			{
				this->x = other.x;
				this->y = other.y;
				this->z = other.z;
				this->w = other.w;
				this->v = other.v;
				this->u = other.u;
				other.x = {};
				other.y = {};
				other.z = {};
				other.w = {};
				other.v = {};
				other.u = {};
			}

			return *this;
		}

		T& operator[](size_t i)
		{
			if (i >= 6) throw std::out_of_range("m5::vec6::operator[]: index out of range.");

			return (&x)[i];
		}
		const T& operator[](size_t i) const
		{
			if (i >= 6) throw std::out_of_range("m5::vec6::operator[]: index out of range.");

			return (&x)[i];
		}

		Tvec6<T> operator+(const Tvec6<T>& other) const
		{
			return Tvec6<T>
			{
				this->x + other.x,
				this->y + other.y,
				this->z + other.z,
				this->w + other.w,
				this->v + other.v,
				this->v + other.u
			};
		}
		Tvec6<T> operator+(T v) const
		{
			return Tvec6<T>
			{
				this->x + v,
				this->y + v,
				this->z + v,
				this->w + v,
				this->v + v,
				this->u + v
			};
		}
		Tvec6<T> operator-(const Tvec6<T>& other) const
		{
			return Tvec6<T>
			{
				this->x - other.x,
				this->y - other.y,
				this->z - other.z,
				this->w - other.w,
				this->v - other.v,
				this->u - other.u
			};
		}
		Tvec6<T> operator-(T v) const
		{
			return Tvec6<T>
			{
				this->x - v,
				this->y - v,
				this->z - v,
				this->w - v,
				this->v - v,
				this->u - v
			};
		}
		Tvec6<T> operator-() const
		{
			return Tvec6<T>
			{
				-this->x,
				-this->y,
				-this->z,
				-this->w,
				-this->v,
				-this->u
			};
		}
		Tvec6<T> operator*(const Tvec6<T>& other) const
		{
			return Tvec6<T>
			{
				this->x * other.x,
				this->y * other.y,
				this->z * other.z,
				this->w * other.w,
				this->v * other.v,
				this->u * other.u
			};
		}
		Tvec6<T> operator*(T v) const
		{
			return Tvec6<T>
			{
				this->x * v,
				this->y * v,
				this->z * v,
				this->w * v,
				this->v * v,
				this->u * v
			};
		}
		Tvec6<T> operator/(const Tvec6<T>& other) const
		{
			return Tvec6<T>
			{
				this->x / other.x,
				this->y / other.y,
				this->z / other.z,
				this->w / other.w,
				this->v / other.v,
				this->u / other.u
			};
		}
		Tvec6<T> operator/(T v) const
		{
			float invV = 1.0f / v;
			return Tvec6<T>
			{
				this->x * invV,
				this->y * invV,
				this->z * invV,
				this->w * invV,
				this->v * invV,
				this->u * invV
			};
		}

		Tvec6<T>& operator+=(const Tvec6<T>& other)
		{
			return *this = *this + other;
		}
		Tvec6<T>& operator+=(T v)
		{
			return *this = *this + v;
		}
		Tvec6<T>& operator-=(const Tvec6<T>& other)
		{
			return *this = *this - other;
		}
		Tvec6<T>& operator-=(T v)
		{
			return *this = *this - v;
		}
		Tvec6<T>& operator*=(const Tvec6<T>& other)
		{
			return *this = *this * other;
		}
		Tvec6<T>& operator*=(T v)
		{
			return *this = *this * v;
		}
		Tvec6<T>& operator/=(const Tvec6<T>& other)
		{
			return *this = *this / other;
		}
		Tvec6<T>& operator/=(T v)
		{
			return *this = *this / v;
		}

		bool operator==(const Tvec6<T>& other) const
		{
			return this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w && this->v == other.v && this->u == other.u;
		}

		T dot(const Tvec6<T>& other) const
		{
			return x * other.x + y * other.y + z * other.z + w * other.w + v * other.v + u * other.u;
		}
		T length2() const
		{
			return dot(*this);
		}
		T length() const
		{
			return glm::sqrt(length2());
		}
		Tvec6<T>& normalize()
		{
			float len = length2();
			if (len <= glm::epsilon<T>() * glm::epsilon<T>())
			{
				return *this;
			}
			return *this /= glm::sqrt(len);
		}
		Tvec6<T> normalized() const
		{
			Tvec6<T> v = *this;
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

		Tvec5<T>& xyzwv()
		{
			return *(Tvec5<T>*)&x;
		}
		const Tvec5<T>& xyzwv() const
		{
			return *(const Tvec5<T>*)&x;
		}
	};

	using vec6 = Tvec6<float>;
	using i8vec6 = Tvec6<int8_t>;
	using i16vec6 = Tvec6<int16_t>;
	using i32vec6 = Tvec6<int32_t>;
	using ivec6 = i32vec6;
	using i64vec6 = Tvec6<int64_t>;
	using u8vec6 = Tvec6<uint8_t>;
	using u16vec6 = Tvec6<uint16_t>;
	using u32vec6 = Tvec6<uint32_t>;
	using uvec6 = u32vec6;
	using u64vec6 = Tvec6<uint64_t>;

	inline float dot(const vec6& a, const vec6& b) { return a.dot(b); }
	inline vec6 normalize(const vec6& v) { return v.normalized(); }
	inline float length(const vec6& v) { return v.length(); }
	inline float length2(const vec6& v) { return v.length2(); }
	inline float distance(const vec6& a, const vec6& b) { return (b - a).length(); }
	inline float distance2(const vec6& a, const vec6& b) { return (b - a).length2(); }
}
