#pragma once

#include <array>
#include <cstdint>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "vec5.h"
#include "vec6.h"

namespace m5
{
	struct BiVector4
	{
		float xy = 0, xz = 0, xw = 0, yz = 0, yw = 0, zw = 0;

		BiVector4() {}
		BiVector4(float v);
		BiVector4(float xy, float xz, float xw, float yz, float yw, float zw);

		BiVector4(const BiVector4& other);
		BiVector4(BiVector4&& other) noexcept;
		BiVector4& operator=(const BiVector4& other);
		BiVector4& operator=(BiVector4&& other) noexcept;

		float& operator[](size_t i);
		const float& operator[](size_t i) const;

		BiVector4 operator+(const BiVector4& other) const;
		BiVector4 operator+(float v) const;
		BiVector4 operator-(const BiVector4& other) const;
		BiVector4 operator-(float v) const;
		BiVector4 operator-() const;
		BiVector4 operator*(const BiVector4& other) const;
		BiVector4 operator*(float v) const;
		BiVector4 operator/(const BiVector4& other) const;
		BiVector4 operator/(float v) const;

		BiVector4& operator+=(const BiVector4& other);
		BiVector4& operator+=(float v);
		BiVector4& operator-=(const BiVector4& other);
		BiVector4& operator-=(float v);
		BiVector4& operator*=(const BiVector4& other);
		BiVector4& operator*=(float v);
		BiVector4& operator/=(const BiVector4& other);
		BiVector4& operator/=(float v);

		bool operator==(const BiVector4& other) const;

		BiVector4 normalized() const;
		BiVector4& normalize();

		inline static BiVector4 XY() { return BiVector4{ 1,0,0,0,0,0 }; }
		inline static BiVector4 XZ() { return BiVector4{ 0,1,0,0,0,0 }; }
		inline static BiVector4 XW() { return BiVector4{ 0,0,1,0,0,0 }; }
		inline static BiVector4 YZ() { return BiVector4{ 0,0,0,1,0,0 }; }
		inline static BiVector4 YW() { return BiVector4{ 0,0,0,0,1,0 }; }
		inline static BiVector4 ZW() { return BiVector4{ 0,0,0,0,0,1 }; }
	};

	float dot(const BiVector4& a, const BiVector4& b);
	glm::vec4 cross(const glm::vec4& u, const glm::vec4& v, const glm::vec4& w);
	BiVector4 wedge(const glm::vec4& u, const glm::vec4& v);
	BiVector4 normalize(const BiVector4& v);
	float length(const BiVector4& v);
	float length2(const BiVector4& v);

	struct Mat5;
	struct Rotor
	{
		float a = 1.0f;
		BiVector4 b = 0.0f;
		float xyzw = 0.0f;

		Rotor() {}
		Rotor(float a, BiVector4 b, float xyzw);
		Rotor(const BiVector4& plane, float radians);
		Rotor(const glm::vec4& from, const glm::vec4& to);
		Rotor(const BiVector4& u, const BiVector4& v);

		Rotor(const Rotor& other);
		Rotor(Rotor&& other) noexcept;
		Rotor& operator=(const Rotor& other);
		Rotor& operator=(Rotor&& other) noexcept;

		Rotor operator*(const Rotor& r) const;
		Rotor& operator*=(const Rotor& r);
		glm::vec4 rotate(const glm::vec4& v) const;

		Rotor operator-() const;
		bool operator==(const Rotor& other) const;

		Rotor normalized() const;
		Rotor& normalize();

		operator glm::mat4() const;
	};

	Rotor normalize(const Rotor& v);
	float length(const Rotor& v);
	float length2(const Rotor& v);

	struct Mat5
	{
		std::array<vec5, 5> value
		{
			vec5{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
			vec5{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
			vec5{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
			vec5{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
			vec5{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }
		};

		Mat5(float x = 0.0f);
		Mat5(const glm::mat4& m);
		Mat5(const m5::Rotor& r)
			: Mat5((glm::mat4)r) {
		}
		Mat5(const std::array<vec5, 5>& value);
		Mat5(const vec5& m0, const vec5& m1, const vec5& m2, const vec5& m3, const vec5& m4);

		Mat5(const Mat5& other);
		Mat5(Mat5&& other) noexcept;
		Mat5& operator=(const Mat5& other);
		Mat5& operator=(Mat5&& other) noexcept;

		Mat5 operator*(const Mat5& other) const;
		Mat5& operator*=(const Mat5& other);

		vec5 multiply(const vec5& v) const;
		glm::vec4 multiply(const glm::vec4& v, float finalComp = 1.0f) const;
		vec5 operator*(const vec5& v) const;
		glm::vec4 operator*(const glm::vec4& v) const;

		vec5& operator[](size_t i);
		const vec5& operator[](size_t i) const;

		void translate(const glm::vec4& v);
		void translate(float x, float y, float z, float w);
		void translate(float a);
		void scale(const glm::vec4& s);
		void scale(float x, float y, float z, float w);
		void scale(float s);

		static Mat5 transpose(const Mat5& m);
		static Mat5 inverse(const Mat5& m);

		operator glm::mat4() const;
	};

	template<glm::length_t L, typename T>
	inline glm::vec<L, T> safeNormalize(const glm::vec<L, T>& v)
	{
		T len = glm::dot(v, v);
		if (len <= glm::epsilon<T>() * glm::epsilon<T>())
		{
			return glm::vec<L, T>{};
		}
		return v / glm::sqrt(len);
	}

	m5::Mat5 createCamera4D(const glm::vec4& eye, const glm::vec4& left, const glm::vec4& up, const glm::vec4& forward, const glm::vec4& over);

	struct Mat6
	{
		std::array<vec6, 6> value
		{
			vec6{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
			vec6{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
			vec6{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
			vec6{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
			vec6{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
			vec6{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }
		};

		Mat6(float x = 0.0f);
		Mat6(const Mat5& m);
		Mat6(const std::array<vec6, 6>& value);
		Mat6(const vec6& m0, const vec6& m1, const vec6& m2, const vec6& m3, const vec6& m4, const vec6& m5);

		Mat6(const Mat6& other);
		Mat6(Mat6&& other) noexcept;
		Mat6& operator=(const Mat6& other);
		Mat6& operator=(Mat6&& other) noexcept;

		Mat6 operator*(const Mat6& other) const;
		Mat6& operator*=(const Mat6& other);

		vec6 multiply(const vec6& v) const;
		vec6 operator*(const vec6& v) const;

		vec6& operator[](size_t i);
		const vec6& operator[](size_t i) const;

		void translate(const vec5& v);
		void translate(float x, float y, float z, float w, float v);
		void translate(float a);
		void scale(const vec5& s);
		void scale(float x, float y, float z, float w, float v);
		void scale(float s);

		static Mat6 transpose(const Mat6& m);
		static Mat6 inverse(const Mat6& m);

		operator Mat5() const;
	};
	
	// yonder - at some distance in the direction indicated; over there
	m5::Mat6 createCamera5D(const vec5& eye, const vec5& left, const vec5& up, const vec5& forward, const vec5& over, const vec5& yonder);
}
