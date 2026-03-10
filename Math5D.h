#pragma once
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "vec5.h"
#include "vec6.h"
#include "json.hpp"

namespace m5
{
	//template<typename T = float>
	//Tvec5<T> cross(const Tvec5<T>& a, const Tvec5<T>& b, const Tvec5<T>& c, const Tvec5<T>& d)
	//{
	//}

	vec5 cross(const vec5& a, const vec5& b, const vec5& c, const vec5& d);

	//float dot(const vec5& a, const vec5& b);

	struct Bivector5
	{
		static constexpr size_t SIZE = 10;

		Bivector5(float ab = 0, float ac = 0, float ad = 0, float ae = 0, float bc = 0, float bd = 0, float be = 0, float cd = 0, float ce = 0, float de = 0)
			: ab{ ab }, ac{ ac }, ad{ ad }, ae{ ae }, bc{ bc }, bd{ bd }, be{ be }, cd{ cd }, ce{ ce }, de{ de } {}

		Bivector5(const nlohmann::json& j);
		nlohmann::json toJson() const;

		float ab;
		float ac;
		float ad;
		float ae;
		float bc;
		float bd;
		float be;
		float cd;
		float ce;
		float de;

		float& operator[](size_t i)
		{
			assert(i < SIZE);
			return (&ab)[i];
		}

		const float& operator[](size_t i) const
		{
			assert(i < SIZE);
			return (&ab)[i];
		}

		float lengthSquared() const;
		float length() const;

		void normalize();
		Bivector5 normalized() const;
	};

	Bivector5 wedge(const vec5& a, const vec5& b);

	struct Quadrivector5
	{
		static constexpr size_t SIZE = 5;

		Quadrivector5(float abcd = 0, float abce = 0, float abde = 0, float acde = 0, float bcde = 0)
			: abcd{ abcd }, abce{ abce }, abde{ abde }, acde{ acde }, bcde{ bcde } {}

		Quadrivector5(const nlohmann::json& j);
		nlohmann::json toJson() const;

		float abcd;
		float abce;
		float abde;
		float acde;
		float bcde;

		float operator[](size_t i)
		{
			assert(i < SIZE);
			return (&abcd)[i];
		}

		float lengthSquared() const;
		float length() const;
	};

	class Rotor5
	{
	public:

		// scalar
		float s;
		// bivector
		Bivector5 b;
		// quadrivector
		Quadrivector5 q;

		// default constructor
		Rotor5() : s{ 1 }, b{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, q{ 0, 0, 0, 0, 0 } {}
		Rotor5(float s, const Bivector5& b, Quadrivector5 q) : s{ s }, b{ b }, q{ q } {}
		// rotate one vector to another
		Rotor5(const vec5& from, const vec5& to);
		// rotate by angle on a given plane ("plane" must be normailzed)
		Rotor5(const Bivector5& plane, float radians);
		// interpolate between two rotors
		Rotor5(const Rotor5& from, const Rotor5& to, float a);

		Rotor5(const nlohmann::json& j);
		nlohmann::json toJson() const;

		// multiply
		Rotor5 operator*(const Rotor5& r) const;
		Rotor5 operator*=(const Rotor5& r);
		Rotor5 rotate(const Rotor5& r) const;

		// reverse
		Rotor5 reverse() const;
		Rotor5 operator~() const;

		// rotate a vector by the rotor
		vec5 rotate(const vec5& v) const;

		float lengthSquared() const;
		float length() const;
		void normalize();
		Rotor5 normalized() const;
	};

	class Mat5
	{
	public:

		struct Column
		{
			float value[5];
			constexpr float& operator[](std::size_t index) { return value[index]; }
			constexpr const float& operator[](std::size_t index) const { return value[index]; }
		};

		Column value[5]{};

		Mat5(float x);
		Mat5(const Rotor5& r);
		constexpr Mat5(
			const Column& c0 = {},
			const Column& c1 = {},
			const Column& c2 = {},
			const Column& c3 = {},
			const Column& c4 = {}
		) : value{ c0, c1, c2, c3, c4 } {}

		Mat5(const nlohmann::json& j);
		nlohmann::json toJson() const;

		Mat5 operator*(const Mat5& other) const;
		Mat5 operator*=(const Mat5& other);

		glm::vec4 multiply(const glm::vec4& v, float finalComp) const;
		glm::vec4 operator*(const glm::vec4& v) const;

		// implemented here for inlining
		Column& operator[](int index)
		{
			return value[index];
		}
		const Column& operator[](int index) const
		{
			return value[index];
		}

		void translate(const glm::vec4& translation);
		void scale(const glm::vec4& scale);
	};

	// json conversion functions
	template <typename T = float>
	glm::vec<2, T> vec2FromJson(const nlohmann::json& j)
	{
		return { j.at(0).get<T>(), j.at(1).get<T>() };
	}
	template <typename T = float>
	nlohmann::json vec2ToJson(const glm::vec<2, T>& vec)
	{
		return { vec.x, vec.y };
	}

	template <typename T = float>
	glm::vec<3, T> vec3FromJson(const nlohmann::json& j)
	{
		return { j.at(0).get<T>(), j.at(1).get<T>(), j.at(2).get<T>() };
	}
	template <typename T = float>
	nlohmann::json vec3ToJson(const glm::vec<3, T>& vec)
	{
		return { vec.x, vec.y, vec.z };
	}

	template <typename T = float>
	glm::vec<4, T> vec4FromJson(const nlohmann::json& j)
	{
		return { j.at(0).get<T>(), j.at(1).get<T>(), j.at(2).get<T>(), j.at(3).get<T>() };
	}
	template <typename T = float>
	nlohmann::json vec4ToJson(const glm::vec<4, T>& vec)
	{
		return { vec.x, vec.y, vec.z, vec.w };
	}

	template <typename T = float>
	glm::qua<T> quatFromJson(const nlohmann::json& j)
	{
		return { j.at(0).get<T>(), j.at(1).get<T>(), j.at(2).get<T>(), j.at(3).get<T>() };
	}
	template <typename T = float>
	nlohmann::json quatToJson(const glm::qua<T>& quat)
	{
		return { quat.w, quat.x, quat.y, quat.z };
	}
}