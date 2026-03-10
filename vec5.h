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
		inline static constexpr size_t size = 5;

		T a{}, b{}, c{}, d{}, e{};

		Tvec5() {}
		Tvec5(T e)
			: a(e), b(e), c(e), d(e), e(e) {}
		Tvec5(T a, T b, T c, T d, T e)
			: a(a), b(b), c(c), d(d), e(e) {}
		Tvec5(const glm::vec<4, T>& abcd, T e = {})
			: a(abcd.x), b(abcd.y), c(abcd.z), d(abcd.w), e(e) {}

		Tvec5(const Tvec5<T>& other)
			: a(other.a), b(other.b), c(other.c), d(other.d), e(other.e) {}
		Tvec5(Tvec5<T>&& other) noexcept
			: a(other.a), b(other.b), c(other.c), d(other.d), e(other.e)
		{
			other.a = {};
			other.b = {};
			other.c = {};
			other.d = {};
			other.e = {};
		}
		Tvec5<T>& operator=(const Tvec5<T>& other)
		{
			this->a = other.a;
			this->b = other.b;
			this->c = other.c;
			this->d = other.d;
			this->e = other.e;

			return *this;
		}
		Tvec5<T>& operator=(Tvec5<T>&& other) noexcept
		{
			if (this != &other)
			{
				this->a = other.a;
				this->b = other.b;
				this->c = other.c;
				this->d = other.d;
				this->e = other.e;
				other.a = {};
				other.b = {};
				other.c = {};
				other.d = {};
				other.e = {};
			}

			return *this;
		}

		T& operator[](size_t i)
		{
			if (i >= size) throw std::out_of_range("m5::vec5::operator[]: index out of range.");

			return (&a)[i];
		}
		const T& operator[](size_t i) const
		{
			if (i >= size) throw std::out_of_range("m5::vec5::operator[]: index out of range.");

			return (&a)[i];
		}

		Tvec5<T> operator+(const Tvec5<T>& other) const
		{
			return Tvec5<T>
			{
				this->a + other.a,
				this->b + other.b,
				this->c + other.c,
				this->d + other.d,
				this->e + other.e,
			};
		}
		Tvec5<T> operator+(T v) const
		{
			return Tvec5<T>
			{
				this->a + v,
				this->b + v,
				this->c + v,
				this->d + v,
				this->e + v,
			};
		}
		Tvec5<T> operator-(const Tvec5<T>& other) const
		{
			return Tvec5<T>
			{
				this->a - other.a,
				this->b - other.b,
				this->c - other.c,
				this->d - other.d,
				this->e - other.e,
			};
		}
		Tvec5<T> operator-(T v) const
		{
			return Tvec5<T>
			{
				this->a - v,
				this->b - v,
				this->c - v,
				this->d - v,
				this->e - v,
			};
		}
		Tvec5<T> operator-() const
		{
			return Tvec5<T>
			{
				-this->a,
				-this->b,
				-this->c,
				-this->d,
				-this->e,
			};
		}
		Tvec5<T> operator*(const Tvec5<T>& other) const
		{
			return Tvec5<T>
			{
				this->a * other.a,
				this->b * other.b,
				this->c * other.c,
				this->d * other.d,
				this->e * other.e,
			};
		}
		Tvec5<T> operator*(T v) const
		{
			return Tvec5<T>
			{
				this->a * v,
				this->b * v,
				this->c * v,
				this->d * v,
				this->e * v,
			};
		}
		Tvec5<T> operator/(const Tvec5<T>& other) const
		{
			return Tvec5<T>
			{
				this->a / other.a,
				this->b / other.b,
				this->c / other.c,
				this->d / other.d,
				this->e / other.e,
			};
		}
		Tvec5<T> operator/(T v) const
		{
			float invV = 1.0f / v;
			return Tvec5<T>
			{
				this->a * invV,
				this->b * invV,
				this->c * invV,
				this->d * invV,
				this->e * invV,
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
			return this->a == other.a && this->b == other.b && this->c == other.c && this->d == other.d && this->e == other.e;
		}

		T dot(const Tvec5<T>& other) const
		{
			return a * other.a + b * other.b + c * other.c + d * other.d + e * other.e;
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
			return *(glm::vec<4, T>*)&a;
		}
		const glm::vec<4, T>& xyzw() const
		{
			return *(const glm::vec<4, T>*)&a;
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
	inline vec5 normalize(const vec5& e) { return e.normalized(); }
	inline float length(const vec5& e) { return e.length(); }
	inline float length2(const vec5& e) { return e.length2(); }
	inline float distance(const vec5& a, const vec5& b) { return (b - a).length(); }
	inline float distance2(const vec5& a, const vec5& b) { return (b - a).length2(); }
}