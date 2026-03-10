#include "Math5D.h"
#include <glm/common.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>

namespace m5
{
	//float dot(const vec5& a, const vec5& b)
	//{
	//	return a.a * b.a + a.b * b.b + a.c * b.c + a.d * b.d + a.e * b.e;
	//}

	inline float det3(
		float a00, float a01, float a02,
		float a10, float a11, float a12,
		float a20, float a21, float a22)
	{
		return a00 * (a11 * a22 - a12 * a21)
			- a01 * (a10 * a22 - a12 * a20)
			+ a02 * (a10 * a21 - a11 * a20);
	}
	inline float det4(
		float a00, float a01, float a02, float a03,
		float a10, float a11, float a12, float a13,
		float a20, float a21, float a22, float a23,
		float a30, float a31, float a32, float a33)
	{
		return
			a00 * det3(a11, a12, a13, a21, a22, a23, a31, a32, a33)
			- a01 * det3(a10, a12, a13, a20, a22, a23, a30, a32, a33)
			+ a02 * det3(a10, a11, a13, a20, a21, a23, a30, a31, a33)
			- a03 * det3(a10, a11, a12, a20, a21, a22, a30, a31, a32);
	}
	inline float det4_cols(const vec5& r0, const vec5& r1, const vec5& r2, const vec5& r3, int c0, int c1, int c2, int c3)
	{
		return det4(
			r0[c0], r0[c1], r0[c2], r0[c3],
			r1[c0], r1[c1], r1[c2], r1[c3],
			r2[c0], r2[c1], r2[c2], r2[c3],
			r3[c0], r3[c1], r3[c2], r3[c3]
		);
	}

	vec5 cross(const vec5& a, const vec5& b, const vec5& c, const vec5& d)
	{
		float ca = det4_cols(a, b, c, d, 1, 2, 3, 4);
		float cb = -det4_cols(a, b, c, d, 0, 2, 3, 4);
		float cc = det4_cols(a, b, c, d, 0, 1, 3, 4);
		float cd = -det4_cols(a, b, c, d, 0, 1, 2, 4);
		float ce = det4_cols(a, b, c, d, 0, 1, 2, 3);
		return vec5{ ca, cb, cc, cd, ce };
	}

	Bivector5::Bivector5(const nlohmann::json& j)
	{
		ab = j[0].get<float>();
		ac = j[1].get<float>();
		ad = j[2].get<float>();
		ae = j[3].get<float>();
		bc = j[4].get<float>();
		bd = j[5].get<float>();
		be = j[6].get<float>();
		cd = j[7].get<float>();
		ce = j[8].get<float>();
		de = j[9].get<float>();
	}

	nlohmann::json Bivector5::toJson() const
	{
		return { ab, ac, ad, ae, bc, bd, be, cd, ce, de };
	}

	float Bivector5::lengthSquared() const
	{
		return ab * ab + ac * ac + ad * ad + ae * ae + bc * bc + bd * bd + be * be + cd * cd + ce * ce + de * de;
	}

	float Bivector5::length() const
	{
		return glm::sqrt(lengthSquared());
	}

	void Bivector5::normalize()
	{
		float len = length();
		ab /= len;
		ac /= len;
		ad /= len;
		ae /= len;
		bc /= len;
		bd /= len;
		be /= len;
		cd /= len;
		ce /= len;
		de /= len;
	}

	Bivector5 Bivector5::normalized() const
	{
		Bivector5 b = *this;
		b.normalize();
		return b;
	}

	Bivector5 wedge(const vec5& a, const vec5& b)
	{
		return Bivector5{
			a.a * b.b - a.b * b.a,
			a.a * b.c - a.c * b.a,
			a.a * b.d - a.d * b.a,
			a.a * b.e - a.e * b.a,
			a.b * b.c - a.c * b.b,
			a.b * b.d - a.d * b.b,
			a.b * b.e - a.e * b.b,
			a.c * b.d - a.d * b.c,
			a.c * b.e - a.e * b.c,
			a.d * b.e - a.e * b.d,
		}.normalized();
	}

	Quadrivector5::Quadrivector5(const nlohmann::json& j)
	{
		abcd = j[0].get<float>();
		abce = j[1].get<float>();
		abde = j[2].get<float>();
		acde = j[3].get<float>();
		bcde = j[4].get<float>();
	}

	nlohmann::json Quadrivector5::toJson() const
	{
		return { abcd, abce, abde, acde, bcde };
	}

	float Quadrivector5::lengthSquared() const
	{
		return abcd * abcd + abce * abce + abde * abde + acde * acde + bcde * bcde;
	}

	float Quadrivector5::length() const
	{
		return glm::sqrt(lengthSquared());
	}

	Rotor5::Rotor5(const vec5& from, const vec5& to)
	{
		q = {};

		float d = glm::clamp(dot(from, to), -1.0f, 1.0f);
		if (d == 1.0f)
		{
			s = 1.0f;
			b = {};
			return;
		}

		float radians = glm::acos(d);
		s = glm::cos(radians / 2.0f);
		float sina = glm::sin(radians / 2.0f);

		b = wedge(to, from);
		b.ab *= sina;
		b.ac *= sina;
		b.ad *= sina;
		b.ae *= sina;
		b.bc *= sina;
		b.bd *= sina;
		b.be *= sina;
		b.cd *= sina;
		b.ce *= sina;
		b.de *= sina;

		normalize();
	}

	Rotor5::Rotor5(const Bivector5& plane, float radians)
	{
		float sina = glm::sin(radians / 2.0f);
		s = glm::cos(radians / 2.0f);
		b.ab = -sina * plane.ab;
		b.ac = -sina * plane.ac;
		b.ad = -sina * plane.ad;
		b.ae = -sina * plane.ae;
		b.bc = -sina * plane.bc;
		b.bd = -sina * plane.bd;
		b.be = -sina * plane.be;
		b.cd = -sina * plane.cd;
		b.ce = -sina * plane.ce;
		b.de = -sina * plane.de;
		q = {};
	}

	Rotor5::Rotor5(const Rotor5& from, const Rotor5& to, float t)
	{
		// dot product
		float d =
			from.s * to.s +
			from.b.ab * to.b.ab +
			from.b.ac * to.b.ac +
			from.b.ad * to.b.ad +
			from.b.ae * to.b.ae +
			from.b.bc * to.b.bc +
			from.b.bd * to.b.bd +
			from.b.be * to.b.be +
			from.b.cd * to.b.cd +
			from.b.ce * to.b.ce +
			from.b.de * to.b.de +
			from.q.abcd * to.q.abcd +
			from.q.abce * to.q.abce +
			from.q.abde * to.q.abde +
			from.q.acde * to.q.acde +
			from.q.bcde * to.q.bcde;

		// flip dst if "dot" is negative
		Rotor5 dst = to;
		if (d < 0.0f)
		{
			dst.s *= -1.0f;
			dst.b.ab *= -1.0f;
			dst.b.ac *= -1.0f;
			dst.b.ad *= -1.0f;
			dst.b.ae *= -1.0f;
			dst.b.bc *= -1.0f;
			dst.b.bd *= -1.0f;
			dst.b.be *= -1.0f;
			dst.b.cd *= -1.0f;
			dst.b.ce *= -1.0f;
			dst.b.de *= -1.0f;
			dst.q.abcd *= -1.0f;
			dst.q.abce *= -1.0f;
			dst.q.abde *= -1.0f;
			dst.q.acde *= -1.0f;
			dst.q.bcde *= -1.0f;

			d *= -1.0f;
		}

		if (d > 0.99995f)
		{
			// nlerp if the rotors are too close
			s = glm::mix(from.s, dst.s, t);
			b.ab = glm::mix(from.b.ab, dst.b.ab, t);
			b.ac = glm::mix(from.b.ac, dst.b.ac, t);
			b.ad = glm::mix(from.b.ad, dst.b.ad, t);
			b.ae = glm::mix(from.b.ae, dst.b.ae, t);
			b.bc = glm::mix(from.b.bc, dst.b.bc, t);
			b.bd = glm::mix(from.b.bd, dst.b.bd, t);
			b.be = glm::mix(from.b.be, dst.b.be, t);
			b.cd = glm::mix(from.b.cd, dst.b.cd, t);
			b.ce = glm::mix(from.b.ce, dst.b.ce, t);
			b.de = glm::mix(from.b.de, dst.b.de, t);
			q.abcd = glm::mix(from.q.abcd, dst.q.abcd, t);
			q.abce = glm::mix(from.q.abce, dst.q.abce, t);
			q.abde = glm::mix(from.q.abde, dst.q.abde, t);
			q.acde = glm::mix(from.q.acde, dst.q.acde, t);
			q.bcde = glm::mix(from.q.bcde, dst.q.bcde, t);

			normalize();
			return;
		}

		// assume that "from" and "dst" both have a magnitude of 1,
		// i.e. they are the product of two unit vectors,
		// then cos(theta) = dot(from, dst)
		const float theta = glm::acos(d);
		const float fromFactor = glm::sin((1.0f - t) * theta) / glm::sin(theta);
		const float toFactor = glm::sin(t * theta) / glm::sin(theta);

		s = fromFactor * from.s + toFactor * dst.s;
		b.ab = fromFactor * from.b.ab + toFactor * dst.b.ab;
		b.ac = fromFactor * from.b.ac + toFactor * dst.b.ac;
		b.ad = fromFactor * from.b.ad + toFactor * dst.b.ad;
		b.ae = fromFactor * from.b.ae + toFactor * dst.b.ae;
		b.bc = fromFactor * from.b.bc + toFactor * dst.b.bc;
		b.bd = fromFactor * from.b.bd + toFactor * dst.b.bd;
		b.be = fromFactor * from.b.be + toFactor * dst.b.be;
		b.cd = fromFactor * from.b.cd + toFactor * dst.b.cd;
		b.ce = fromFactor * from.b.ce + toFactor * dst.b.ce;
		b.de = fromFactor * from.b.de + toFactor * dst.b.de;
		q.abcd = fromFactor * from.q.abcd + toFactor * dst.q.abcd;
		q.abce = fromFactor * from.q.abce + toFactor * dst.q.abce;
		q.abde = fromFactor * from.q.abde + toFactor * dst.q.abde;
		q.acde = fromFactor * from.q.acde + toFactor * dst.q.acde;
		q.bcde = fromFactor * from.q.bcde + toFactor * dst.q.bcde;
	}

	Rotor5::Rotor5(const nlohmann::json& j)
	{
		s = j[0].get<float>();
		b.ab = j[1].get<float>();
		b.ac = j[2].get<float>();
		b.ad = j[3].get<float>();
		b.ae = j[4].get<float>();
		b.bc = j[5].get<float>();
		b.bd = j[6].get<float>();
		b.be = j[7].get<float>();
		b.cd = j[8].get<float>();
		b.ce = j[9].get<float>();
		b.de = j[10].get<float>();
		q.abcd = j[11].get<float>();
		q.abce = j[12].get<float>();
		q.abde = j[13].get<float>();
		q.acde = j[14].get<float>();
		q.bcde = j[15].get<float>();
	}

	nlohmann::json Rotor5::toJson() const
	{
		return { s, b.ab, b.ac, b.ad, b.ae, b.bc, b.bd, b.be, b.cd, b.ce, b.de, q.abcd, q.abce, q.abde, q.acde, q.bcde };
	}

	Rotor5 Rotor5::operator*(const Rotor5& r) const
	{
		return
		{
			-b.ab * r.b.ab - b.ac * r.b.ac - b.ad * r.b.ad - b.ae * r.b.ae - b.bc * r.b.bc - b.bd * r.b.bd - b.be * r.b.be - b.cd * r.b.cd - b.ce * r.b.ce - b.de * r.b.de + q.abcd * r.q.abcd + q.abce * r.q.abce + q.abde * r.q.abde + q.acde * r.q.acde + q.bcde * r.q.bcde + s * r.s,
			Bivector5{
				b.ab * r.s - b.ac * r.b.bc - b.ad * r.b.bd - b.ae * r.b.be + b.bc * r.b.ac + b.bd * r.b.ad + b.be * r.b.ae - b.cd * r.q.abcd - b.ce * r.q.abce - b.de * r.q.abde - q.abcd * r.b.cd - q.abce * r.b.ce - q.abde * r.b.de + q.acde * r.q.bcde - q.bcde * r.q.acde + s * r.b.ab,
				b.ab * r.b.bc + b.ac * r.s - b.ad * r.b.cd - b.ae * r.b.ce - b.bc * r.b.ab + b.bd * r.q.abcd + b.be * r.q.abce + b.cd * r.b.ad + b.ce * r.b.ae - b.de * r.q.acde + q.abcd * r.b.bd + q.abce * r.b.be - q.abde * r.q.bcde - q.acde * r.b.de + q.bcde * r.q.abde + s * r.b.ac,
				b.ab * r.b.bd + b.ac * r.b.cd + b.ad * r.s - b.ae * r.b.de - b.bc * r.q.abcd - b.bd * r.b.ab + b.be * r.q.abde - b.cd * r.b.ac + b.ce * r.q.acde + b.de * r.b.ae - q.abcd * r.b.bc + q.abce * r.q.bcde + q.abde * r.b.be + q.acde * r.b.ce - q.bcde * r.q.abce + s * r.b.ad,
				b.ab * r.b.be + b.ac * r.b.ce + b.ad * r.b.de + b.ae * r.s - b.bc * r.q.abce - b.bd * r.q.abde - b.be * r.b.ab - b.cd * r.q.acde - b.ce * r.b.ac - b.de * r.b.ad - q.abcd * r.q.bcde - q.abce * r.b.bc - q.abde * r.b.bd - q.acde * r.b.cd + q.bcde * r.q.abcd + s * r.b.ae,
				-b.ab * r.b.ac + b.ac * r.b.ab - b.ad * r.q.abcd - b.ae * r.q.abce + b.bc * r.s - b.bd * r.b.cd - b.be * r.b.ce + b.cd * r.b.bd + b.ce * r.b.be - b.de * r.q.bcde - q.abcd * r.b.ad - q.abce * r.b.ae + q.abde * r.q.acde - q.acde * r.q.abde - q.bcde * r.b.de + s * r.b.bc,
				-b.ab * r.b.ad + b.ac * r.q.abcd + b.ad * r.b.ab - b.ae * r.q.abde + b.bc * r.b.cd + b.bd * r.s - b.be * r.b.de - b.cd * r.b.bc + b.ce * r.q.bcde + b.de * r.b.be + q.abcd * r.b.ac - q.abce * r.q.acde - q.abde * r.b.ae + q.acde * r.q.abce + q.bcde * r.b.ce + s * r.b.bd,
				-b.ab * r.b.ae + b.ac * r.q.abce + b.ad * r.q.abde + b.ae * r.b.ab + b.bc * r.b.ce + b.bd * r.b.de + b.be * r.s - b.cd * r.q.bcde - b.ce * r.b.bc - b.de * r.b.bd + q.abcd * r.q.acde + q.abce * r.b.ac + q.abde * r.b.ad - q.acde * r.q.abcd - q.bcde * r.b.cd + s * r.b.be,
				-b.ab * r.q.abcd - b.ac * r.b.ad + b.ad * r.b.ac - b.ae * r.q.acde - b.bc * r.b.bd + b.bd * r.b.bc - b.be * r.q.bcde + b.cd * r.s - b.ce * r.b.de + b.de * r.b.ce - q.abcd * r.b.ab + q.abce * r.q.abde - q.abde * r.q.abce - q.acde * r.b.ae - q.bcde * r.b.be + s * r.b.cd,
				-b.ab * r.q.abce - b.ac * r.b.ae + b.ad * r.q.acde + b.ae * r.b.ac - b.bc * r.b.be + b.bd * r.q.bcde + b.be * r.b.bc + b.cd * r.b.de + b.ce * r.s - b.de * r.b.cd - q.abcd * r.q.abde - q.abce * r.b.ab + q.abde * r.q.abcd + q.acde * r.b.ad + q.bcde * r.b.bd + s * r.b.ce,
				-b.ab * r.q.abde - b.ac * r.q.acde - b.ad * r.b.ae + b.ae * r.b.ad - b.bc * r.q.bcde - b.bd * r.b.be + b.be * r.b.bd - b.cd * r.b.ce + b.ce * r.b.cd + b.de * r.s + q.abcd * r.q.abce - q.abce * r.q.abcd - q.abde * r.b.ab - q.acde * r.b.ac - q.bcde * r.b.bc + s * r.b.de,
			},
			Quadrivector5{
				b.ab * r.b.cd - b.ac * r.b.bd + b.ad * r.b.bc - b.ae * r.q.bcde + b.bc * r.b.ad - b.bd * r.b.ac + b.be * r.q.acde + b.cd * r.b.ab - b.ce * r.q.abde + b.de * r.q.abce + q.abcd * r.s - q.abce * r.b.de + q.abde * r.b.ce - q.acde * r.b.be + q.bcde * r.b.ae + s * r.q.abcd,
				b.ab * r.b.ce - b.ac * r.b.be + b.ad * r.q.bcde + b.ae * r.b.bc + b.bc * r.b.ae - b.bd * r.q.acde - b.be * r.b.ac + b.cd * r.q.abde + b.ce * r.b.ab - b.de * r.q.abcd + q.abcd * r.b.de + q.abce * r.s - q.abde * r.b.cd + q.acde * r.b.bd - q.bcde * r.b.ad + s * r.q.abce,
				b.ab * r.b.de - b.ac * r.q.bcde - b.ad * r.b.be + b.ae * r.b.bd + b.bc * r.q.acde + b.bd * r.b.ae - b.be * r.b.ad - b.cd * r.q.abce + b.ce * r.q.abcd + b.de * r.b.ab - q.abcd * r.b.ce + q.abce * r.b.cd + q.abde * r.s - q.acde * r.b.bc + q.bcde * r.b.ac + s * r.q.abde,
				b.ab * r.q.bcde + b.ac * r.b.de - b.ad * r.b.ce + b.ae * r.b.cd - b.bc * r.q.abde + b.bd * r.q.abce - b.be * r.q.abcd + b.cd * r.b.ae - b.ce * r.b.ad + b.de * r.b.ac + q.abcd * r.b.be - q.abce * r.b.bd + q.abde * r.b.bc + q.acde * r.s - q.bcde * r.b.ab + s * r.q.acde,
				-b.ab * r.q.acde + b.ac * r.q.abde - b.ad * r.q.abce + b.ae * r.q.abcd + b.bc * r.b.de - b.bd * r.b.ce + b.be * r.b.cd + b.cd * r.b.be - b.ce * r.b.bd + b.de * r.b.bc - q.abcd * r.b.ae + q.abce * r.b.ad - q.abde * r.b.ac + q.acde * r.b.ab + q.bcde * r.s + s * r.q.bcde,
			},
		};
	}

	Rotor5 Rotor5::operator*=(const Rotor5& r)
	{
		*this = *this * r;
		return *this;
	}

	Rotor5 Rotor5::rotate(const Rotor5& r) const
	{
		Rotor5 l = *this;
		l *= r;
		return l;
	}

	vec5 Rotor5::rotate(const vec5& v) const
	{
		// TODO: optimize?
		return
		{
			-b.ab * b.ab * v.a + 2 * b.ab * b.bc * v.c + 2 * b.ab * b.bd * v.d + 2 * b.ab * b.be * v.e + 2 * b.ab * s * v.b - b.ac * b.ac * v.a - 2 * b.ac * b.bc * v.b + 2 * b.ac * b.cd * v.d + 2 * b.ac * b.ce * v.e + 2 * b.ac * s * v.c - b.ad * b.ad * v.a - 2 * b.ad * b.bd * v.b - 2 * b.ad * b.cd * v.c + 2 * b.ad * b.de * v.e + 2 * b.ad * s * v.d - b.ae * b.ae * v.a - 2 * b.ae * b.be * v.b - 2 * b.ae * b.ce * v.c - 2 * b.ae * b.de * v.d + 2 * b.ae * s * v.e + b.bc * b.bc * v.a + 2 * b.bc * q.abcd * v.d + 2 * b.bc * q.abce * v.e + b.bd * b.bd * v.a - 2 * b.bd * q.abcd * v.c + 2 * b.bd * q.abde * v.e + b.be * b.be * v.a - 2 * b.be * q.abce * v.c - 2 * b.be * q.abde * v.d + b.cd * b.cd * v.a + 2 * b.cd * q.abcd * v.b + 2 * b.cd * q.acde * v.e + b.ce * b.ce * v.a + 2 * b.ce * q.abce * v.b - 2 * b.ce * q.acde * v.d + b.de * b.de * v.a + 2 * b.de * q.abde * v.b + 2 * b.de * q.acde * v.c - q.abcd * q.abcd * v.a + 2 * q.abcd * q.bcde * v.e - q.abce * q.abce * v.a - 2 * q.abce * q.bcde * v.d - q.abde * q.abde * v.a + 2 * q.abde * q.bcde * v.c - q.acde * q.acde * v.a - 2 * q.acde * q.bcde * v.b + q.bcde * q.bcde * v.a + s * s * v.a,
			-b.ab * b.ab * v.b - 2 * b.ab * b.ac * v.c - 2 * b.ab * b.ad * v.d - 2 * b.ab * b.ae * v.e - 2 * b.ab * s * v.a + b.ac * b.ac * v.b - 2 * b.ac * b.bc * v.a - 2 * b.ac * q.abcd * v.d - 2 * b.ac * q.abce * v.e + b.ad * b.ad * v.b - 2 * b.ad * b.bd * v.a + 2 * b.ad * q.abcd * v.c - 2 * b.ad * q.abde * v.e + b.ae * b.ae * v.b - 2 * b.ae * b.be * v.a + 2 * b.ae * q.abce * v.c + 2 * b.ae * q.abde * v.d - b.bc * b.bc * v.b + 2 * b.bc * b.cd * v.d + 2 * b.bc * b.ce * v.e + 2 * b.bc * s * v.c - b.bd * b.bd * v.b - 2 * b.bd * b.cd * v.c + 2 * b.bd * b.de * v.e + 2 * b.bd * s * v.d - b.be * b.be * v.b - 2 * b.be * b.ce * v.c - 2 * b.be * b.de * v.d + 2 * b.be * s * v.e + b.cd * b.cd * v.b - 2 * b.cd * q.abcd * v.a + 2 * b.cd * q.bcde * v.e + b.ce * b.ce * v.b - 2 * b.ce * q.abce * v.a - 2 * b.ce * q.bcde * v.d + b.de * b.de * v.b - 2 * b.de * q.abde * v.a + 2 * b.de * q.bcde * v.c - q.abcd * q.abcd * v.b - 2 * q.abcd * q.acde * v.e - q.abce * q.abce * v.b + 2 * q.abce * q.acde * v.d - q.abde * q.abde * v.b - 2 * q.abde * q.acde * v.c + q.acde * q.acde * v.b - 2 * q.acde * q.bcde * v.a - q.bcde * q.bcde * v.b + s * s * v.b,
			b.ab * b.ab * v.c - 2 * b.ab * b.ac * v.b + 2 * b.ab * b.bc * v.a + 2 * b.ab * q.abcd * v.d + 2 * b.ab * q.abce * v.e - b.ac * b.ac * v.c - 2 * b.ac * b.ad * v.d - 2 * b.ac * b.ae * v.e - 2 * b.ac * s * v.a + b.ad * b.ad * v.c - 2 * b.ad * b.cd * v.a - 2 * b.ad * q.abcd * v.b - 2 * b.ad * q.acde * v.e + b.ae * b.ae * v.c - 2 * b.ae * b.ce * v.a - 2 * b.ae * q.abce * v.b + 2 * b.ae * q.acde * v.d - b.bc * b.bc * v.c - 2 * b.bc * b.bd * v.d - 2 * b.bc * b.be * v.e - 2 * b.bc * s * v.b + b.bd * b.bd * v.c - 2 * b.bd * b.cd * v.b + 2 * b.bd * q.abcd * v.a - 2 * b.bd * q.bcde * v.e + b.be * b.be * v.c - 2 * b.be * b.ce * v.b + 2 * b.be * q.abce * v.a + 2 * b.be * q.bcde * v.d - b.cd * b.cd * v.c + 2 * b.cd * b.de * v.e + 2 * b.cd * s * v.d - b.ce * b.ce * v.c - 2 * b.ce * b.de * v.d + 2 * b.ce * s * v.e + b.de * b.de * v.c - 2 * b.de * q.acde * v.a - 2 * b.de * q.bcde * v.b - q.abcd * q.abcd * v.c + 2 * q.abcd * q.abde * v.e - q.abce * q.abce * v.c - 2 * q.abce * q.abde * v.d + q.abde * q.abde * v.c - 2 * q.abde * q.acde * v.b + 2 * q.abde * q.bcde * v.a - q.acde * q.acde * v.c - q.bcde * q.bcde * v.c + s * s * v.c,
			b.ab * b.ab * v.d - 2 * b.ab * b.ad * v.b + 2 * b.ab * b.bd * v.a - 2 * b.ab * q.abcd * v.c + 2 * b.ab * q.abde * v.e + b.ac * b.ac * v.d - 2 * b.ac * b.ad * v.c + 2 * b.ac * b.cd * v.a + 2 * b.ac * q.abcd * v.b + 2 * b.ac * q.acde * v.e - b.ad * b.ad * v.d - 2 * b.ad * b.ae * v.e - 2 * b.ad * s * v.a + b.ae * b.ae * v.d - 2 * b.ae * b.de * v.a - 2 * b.ae * q.abde * v.b - 2 * b.ae * q.acde * v.c + b.bc * b.bc * v.d - 2 * b.bc * b.bd * v.c + 2 * b.bc * b.cd * v.b - 2 * b.bc * q.abcd * v.a + 2 * b.bc * q.bcde * v.e - b.bd * b.bd * v.d - 2 * b.bd * b.be * v.e - 2 * b.bd * s * v.b + b.be * b.be * v.d - 2 * b.be * b.de * v.b + 2 * b.be * q.abde * v.a - 2 * b.be * q.bcde * v.c - b.cd * b.cd * v.d - 2 * b.cd * b.ce * v.e - 2 * b.cd * s * v.c + b.ce * b.ce * v.d - 2 * b.ce * b.de * v.c + 2 * b.ce * q.acde * v.a + 2 * b.ce * q.bcde * v.b - b.de * b.de * v.d + 2 * b.de * s * v.e - q.abcd * q.abcd * v.d - 2 * q.abcd * q.abce * v.e + q.abce * q.abce * v.d - 2 * q.abce * q.abde * v.c + 2 * q.abce * q.acde * v.b - 2 * q.abce * q.bcde * v.a - q.abde * q.abde * v.d - q.acde * q.acde * v.d - q.bcde * q.bcde * v.d + s * s * v.d,
			b.ab * b.ab * v.e - 2 * b.ab * b.ae * v.b + 2 * b.ab * b.be * v.a - 2 * b.ab * q.abce * v.c - 2 * b.ab * q.abde * v.d + b.ac * b.ac * v.e - 2 * b.ac * b.ae * v.c + 2 * b.ac * b.ce * v.a + 2 * b.ac * q.abce * v.b - 2 * b.ac * q.acde * v.d + b.ad * b.ad * v.e - 2 * b.ad * b.ae * v.d + 2 * b.ad * b.de * v.a + 2 * b.ad * q.abde * v.b + 2 * b.ad * q.acde * v.c - b.ae * b.ae * v.e - 2 * b.ae * s * v.a + b.bc * b.bc * v.e - 2 * b.bc * b.be * v.c + 2 * b.bc * b.ce * v.b - 2 * b.bc * q.abce * v.a - 2 * b.bc * q.bcde * v.d + b.bd * b.bd * v.e - 2 * b.bd * b.be * v.d + 2 * b.bd * b.de * v.b - 2 * b.bd * q.abde * v.a + 2 * b.bd * q.bcde * v.c - b.be * b.be * v.e - 2 * b.be * s * v.b + b.cd * b.cd * v.e - 2 * b.cd * b.ce * v.d + 2 * b.cd * b.de * v.c - 2 * b.cd * q.acde * v.a - 2 * b.cd * q.bcde * v.b - b.ce * b.ce * v.e - 2 * b.ce * s * v.c - b.de * b.de * v.e - 2 * b.de * s * v.d + q.abcd * q.abcd * v.e - 2 * q.abcd * q.abce * v.d + 2 * q.abcd * q.abde * v.c - 2 * q.abcd * q.acde * v.b + 2 * q.abcd * q.bcde * v.a - q.abce * q.abce * v.e - q.abde * q.abde * v.e - q.acde * q.acde * v.e - q.bcde * q.bcde * v.e + s * s * v.e,
		};
		// grade-5 multivector should always be zero
		//-2 * b.ab * b.cd * v.e + 2 * b.ab * b.ce * v.d - 2 * b.ab * b.de * v.c + 2 * b.ab * q.acde * v.a + 2 * b.ab * q.bcde * v.b + 2 * b.ac * b.bd * v.e - 2 * b.ac * b.be * v.d + 2 * b.ac * b.de * v.b - 2 * b.ac * q.abde * v.a + 2 * b.ac * q.bcde * v.c - 2 * b.ad * b.bc * v.e + 2 * b.ad * b.be * v.c - 2 * b.ad * b.ce * v.b + 2 * b.ad * q.abce * v.a + 2 * b.ad * q.bcde * v.d + 2 * b.ae * b.bc * v.d - 2 * b.ae * b.bd * v.c + 2 * b.ae * b.cd * v.b - 2 * b.ae * q.abcd * v.a + 2 * b.ae * q.bcde * v.e - 2 * b.bc * b.de * v.a - 2 * b.bc * q.abde * v.b - 2 * b.bc * q.acde * v.c + 2 * b.bd * b.ce * v.a + 2 * b.bd * q.abce * v.b - 2 * b.bd * q.acde * v.d - 2 * b.be * b.cd * v.a - 2 * b.be * q.abcd * v.b - 2 * b.be * q.acde * v.e + 2 * b.cd * q.abce * v.c + 2 * b.cd * q.abde * v.d - 2 * b.ce * q.abcd * v.c + 2 * b.ce * q.abde * v.e - 2 * b.de * q.abcd * v.d - 2 * b.de * q.abce * v.e + 2 * q.abcd * s * v.e - 2 * q.abce * s * v.d + 2 * q.abde * s * v.c - 2 * q.acde * s * v.b + 2 * q.bcde * s * v.a;
	}

	float Rotor5::lengthSquared() const
	{
		return s * s + b.lengthSquared() + q.lengthSquared();
	}

	float Rotor5::length() const
	{
		return glm::sqrt(lengthSquared());
	}

	void Rotor5::normalize()
	{
		float len = length();
		s /= len;
		b.ab /= len;
		b.ac /= len;
		b.ad /= len;
		b.ae /= len;
		b.bc /= len;
		b.bd /= len;
		b.be /= len;
		b.cd /= len;
		b.ce /= len;
		b.de /= len;
		q.abcd /= len;
		q.abce /= len;
		q.abde /= len;
		q.acde /= len;
		q.bcde /= len;
	}

	Rotor5 Rotor5::normalized() const
	{
		Rotor5 r = *this;
		r.normalize();
		return r;
	}

	Mat5::Mat5(float x)
	{
		value[0][0] = x;
		value[1][1] = x;
		value[2][2] = x;
		value[3][3] = x;
		value[4][4] = x;
	}

	Mat5::Mat5(const Rotor5& r)
	{
		value[0] = {
			r.s * r.s - r.b.ab * r.b.ab - r.q.abcd * r.q.abcd - r.q.abce * r.q.abce - r.q.abde * r.q.abde - r.b.ac * r.b.ac - r.q.acde * r.q.acde - r.b.ad * r.b.ad - r.b.ae * r.b.ae + r.b.bc * r.b.bc + r.q.bcde * r.q.bcde + r.b.bd * r.b.bd + r.b.be * r.b.be + r.b.cd * r.b.cd + r.b.ce * r.b.ce + r.b.de * r.b.de,
			-2.0f * r.s * r.b.ab - 2.0f * r.q.abcd * r.b.cd - 2.0f * r.q.abce * r.b.ce - 2.0f * r.q.abde * r.b.de - 2.0f * r.b.ac * r.b.bc - 2.0f * r.q.acde * r.q.bcde - 2.0f * r.b.ad * r.b.bd - 2.0f * r.b.ae * r.b.be,
			-2.0f * r.s * r.b.ac + 2.0f * r.b.ab * r.b.bc + 2.0f * r.q.abcd * r.b.bd + 2.0f * r.q.abce * r.b.be + 2.0f * r.q.abde * r.q.bcde - 2.0f * r.q.acde * r.b.de - 2.0f * r.b.ad * r.b.cd - 2.0f * r.b.ae * r.b.ce,
			-2.0f * r.s * r.b.ad + 2.0f * r.b.ab * r.b.bd - 2.0f * r.q.abcd * r.b.bc - 2.0f * r.q.abce * r.q.bcde + 2.0f * r.q.abde * r.b.be + 2.0f * r.b.ac * r.b.cd + 2.0f * r.q.acde * r.b.ce - 2.0f * r.b.ae * r.b.de,
			-2.0f * r.s * r.b.ae + 2.0f * r.b.ab * r.b.be + 2.0f * r.q.abcd * r.q.bcde - 2.0f * r.q.abce * r.b.bc - 2.0f * r.q.abde * r.b.bd + 2.0f * r.b.ac * r.b.ce - 2.0f * r.q.acde * r.b.cd + 2.0f * r.b.ad * r.b.de,
		};

		value[1] = {
			2.0f * r.s * r.b.ab + 2.0f * r.q.abcd * r.b.cd + 2.0f * r.q.abce * r.b.ce + 2.0f * r.q.abde * r.b.de - 2.0f * r.b.ac * r.b.bc - 2.0f * r.q.acde * r.q.bcde - 2.0f * r.b.ad * r.b.bd - 2.0f * r.b.ae * r.b.be,
			r.s * r.s - r.b.ab * r.b.ab - r.q.abcd * r.q.abcd - r.q.abce * r.q.abce - r.q.abde * r.q.abde + r.b.ac * r.b.ac + r.q.acde * r.q.acde + r.b.ad * r.b.ad + r.b.ae * r.b.ae - r.b.bc * r.b.bc - r.q.bcde * r.q.bcde - r.b.bd * r.b.bd - r.b.be * r.b.be + r.b.cd * r.b.cd + r.b.ce * r.b.ce + r.b.de * r.b.de,
			-2.0f * r.s * r.b.bc - 2.0f * r.b.ab * r.b.ac - 2.0f * r.q.abcd * r.b.ad - 2.0f * r.q.abce * r.b.ae - 2.0f * r.q.abde * r.q.acde - 2.0f * r.q.bcde * r.b.de - 2.0f * r.b.bd * r.b.cd - 2.0f * r.b.be * r.b.ce,
			-2.0f * r.s * r.b.bd - 2.0f * r.b.ab * r.b.ad + 2.0f * r.q.abcd * r.b.ac + 2.0f * r.q.abce * r.q.acde - 2.0f * r.q.abde * r.b.ae + 2.0f * r.b.bc * r.b.cd + 2.0f * r.q.bcde * r.b.ce - 2.0f * r.b.be * r.b.de,
			-2.0f * r.s * r.b.be - 2.0f * r.b.ab * r.b.ae - 2.0f * r.q.abcd * r.q.acde + 2.0f * r.q.abce * r.b.ac + 2.0f * r.q.abde * r.b.ad + 2.0f * r.b.bc * r.b.ce - 2.0f * r.q.bcde * r.b.cd + 2.0f * r.b.bd * r.b.de,
		};

		value[2] = {
			2.0f * r.s * r.b.ac + 2.0f * r.b.ab * r.b.bc - 2.0f * r.q.abcd * r.b.bd - 2.0f * r.q.abce * r.b.be + 2.0f * r.q.abde * r.q.bcde + 2.0f * r.q.acde * r.b.de - 2.0f * r.b.ad * r.b.cd - 2.0f * r.b.ae * r.b.ce,
			2.0f * r.s * r.b.bc - 2.0f * r.b.ab * r.b.ac + 2.0f * r.q.abcd * r.b.ad + 2.0f * r.q.abce * r.b.ae - 2.0f * r.q.abde * r.q.acde + 2.0f * r.q.bcde * r.b.de - 2.0f * r.b.bd * r.b.cd - 2.0f * r.b.be * r.b.ce,
			r.s * r.s + r.b.ab * r.b.ab - r.q.abcd * r.q.abcd - r.q.abce * r.q.abce + r.q.abde * r.q.abde - r.b.ac * r.b.ac - r.q.acde * r.q.acde + r.b.ad * r.b.ad + r.b.ae * r.b.ae - r.b.bc * r.b.bc - r.q.bcde * r.q.bcde + r.b.bd * r.b.bd + r.b.be * r.b.be - r.b.cd * r.b.cd - r.b.ce * r.b.ce + r.b.de * r.b.de,
			-2.0f * r.s * r.b.cd - 2.0f * r.b.ab * r.q.abcd - 2.0f * r.q.abce * r.q.abde - 2.0f * r.b.ac * r.b.ad - 2.0f * r.q.acde * r.b.ae - 2.0f * r.b.bc * r.b.bd - 2.0f * r.q.bcde * r.b.be - 2.0f * r.b.ce * r.b.de,
			-2.0f * r.s * r.b.ce - 2.0f * r.b.ab * r.q.abce + 2.0f * r.q.abcd * r.q.abde - 2.0f * r.b.ac * r.b.ae + 2.0f * r.q.acde * r.b.ad - 2.0f * r.b.bc * r.b.be + 2.0f * r.q.bcde * r.b.bd + 2.0f * r.b.cd * r.b.de,
		};

		value[3] = {
			2.0f * r.s * r.b.ad + 2.0f * r.b.ab * r.b.bd + 2.0f * r.q.abcd * r.b.bc - 2.0f * r.q.abce * r.q.bcde - 2.0f * r.q.abde * r.b.be + 2.0f * r.b.ac * r.b.cd - 2.0f * r.q.acde * r.b.ce - 2.0f * r.b.ae * r.b.de,
			2.0f * r.s * r.b.bd - 2.0f * r.b.ab * r.b.ad - 2.0f * r.q.abcd * r.b.ac + 2.0f * r.q.abce * r.q.acde + 2.0f * r.q.abde * r.b.ae + 2.0f * r.b.bc * r.b.cd - 2.0f * r.q.bcde * r.b.ce - 2.0f * r.b.be * r.b.de,
			2.0f * r.s * r.b.cd + 2.0f * r.b.ab * r.q.abcd - 2.0f * r.q.abce * r.q.abde - 2.0f * r.b.ac * r.b.ad + 2.0f * r.q.acde * r.b.ae - 2.0f * r.b.bc * r.b.bd + 2.0f * r.q.bcde * r.b.be - 2.0f * r.b.ce * r.b.de,
			r.s * r.s + r.b.ab * r.b.ab - r.q.abcd * r.q.abcd + r.q.abce * r.q.abce - r.q.abde * r.q.abde + r.b.ac * r.b.ac - r.q.acde * r.q.acde - r.b.ad * r.b.ad + r.b.ae * r.b.ae + r.b.bc * r.b.bc - r.q.bcde * r.q.bcde - r.b.bd * r.b.bd + r.b.be * r.b.be - r.b.cd * r.b.cd + r.b.ce * r.b.ce - r.b.de * r.b.de,
			-2.0f * r.s * r.b.de - 2.0f * r.b.ab * r.q.abde - 2.0f * r.q.abcd * r.q.abce - 2.0f * r.b.ac * r.q.acde - 2.0f * r.b.ad * r.b.ae - 2.0f * r.b.bc * r.q.bcde - 2.0f * r.b.bd * r.b.be - 2.0f * r.b.cd * r.b.ce,
		};

		value[4] = {
			2.0f * r.s * r.b.ae + 2.0f * r.b.ab * r.b.be + 2.0f * r.q.abcd * r.q.bcde + 2.0f * r.q.abce * r.b.bc + 2.0f * r.q.abde * r.b.bd + 2.0f * r.b.ac * r.b.ce + 2.0f * r.q.acde * r.b.cd + 2.0f * r.b.ad * r.b.de,
			2.0f * r.s * r.b.be - 2.0f * r.b.ab * r.b.ae - 2.0f * r.q.abcd * r.q.acde - 2.0f * r.q.abce * r.b.ac - 2.0f * r.q.abde * r.b.ad + 2.0f * r.b.bc * r.b.ce + 2.0f * r.q.bcde * r.b.cd + 2.0f * r.b.bd * r.b.de,
			2.0f * r.s * r.b.ce + 2.0f * r.b.ab * r.q.abce + 2.0f * r.q.abcd * r.q.abde - 2.0f * r.b.ac * r.b.ae - 2.0f * r.q.acde * r.b.ad - 2.0f * r.b.bc * r.b.be - 2.0f * r.q.bcde * r.b.bd + 2.0f * r.b.cd * r.b.de,
			2.0f * r.s * r.b.de + 2.0f * r.b.ab * r.q.abde - 2.0f * r.q.abcd * r.q.abce + 2.0f * r.b.ac * r.q.acde - 2.0f * r.b.ad * r.b.ae + 2.0f * r.b.bc * r.q.bcde - 2.0f * r.b.bd * r.b.be - 2.0f * r.b.cd * r.b.ce,
			r.s * r.s + r.b.ab * r.b.ab + r.q.abcd * r.q.abcd - r.q.abce * r.q.abce - r.q.abde * r.q.abde + r.b.ac * r.b.ac - r.q.acde * r.q.acde + r.b.ad * r.b.ad - r.b.ae * r.b.ae + r.b.bc * r.b.bc - r.q.bcde * r.q.bcde + r.b.bd * r.b.bd - r.b.be * r.b.be + r.b.cd * r.b.cd - r.b.ce * r.b.ce - r.b.de * r.b.de,
		};
	}

	Mat5::Mat5(const nlohmann::json& j)
	{
		for (int col = 0; col < 5; ++col)
		{
			for (int row = 0; row < 5; ++row)
			{
				value[col][row] = j.at(col).at(row).get<float>();
			}
		}
	}

	nlohmann::json Mat5::toJson() const
	{
		return {
			{ value[0][0], value[0][1], value[0][2], value[0][3], value[0][4] },
			{ value[1][0], value[1][1], value[1][2], value[1][3], value[1][4] },
			{ value[2][0], value[2][1], value[2][2], value[2][3], value[2][4] },
			{ value[3][0], value[3][1], value[3][2], value[3][3], value[3][4] },
			{ value[4][0], value[4][1], value[4][2], value[4][3], value[4][4] },
		};
	}

	Mat5 Mat5::operator*(const Mat5& other) const
	{
		Mat5 result;

		for (int col = 0; col < 5; ++col)
		{
			for (int row = 0; row < 5; ++row)
			{
				for (int k = 0; k < 5; ++k)
				{
					result.value[col][row] += value[k][row] * other.value[col][k];
				}
			}
		}

		return result;
	}

	Mat5 Mat5::operator*=(const Mat5& other)
	{
		*this = *this * other;
		return *this;
	}

	glm::vec4 Mat5::multiply(const glm::vec4& v, float finalComp) const
	{
		return glm::vec4
		{
			value[0][0] * v.x + value[1][0] * v.y + value[2][0] * v.z + value[3][0] * v.w + value[4][0] * finalComp,
			value[0][1] * v.x + value[1][1] * v.y + value[2][1] * v.z + value[3][1] * v.w + value[4][1] * finalComp,
			value[0][2] * v.x + value[1][2] * v.y + value[2][2] * v.z + value[3][2] * v.w + value[4][2] * finalComp,
			value[0][3] * v.x + value[1][3] * v.y + value[2][3] * v.z + value[3][3] * v.w + value[4][3] * finalComp
		};
	}

	glm::vec4 Mat5::operator*(const glm::vec4& v) const
	{
		return multiply(v, 1.0f);
	}

	void Mat5::translate(const glm::vec4& v)
	{
		value[4][0] += v.x * value[0][0] + v.y * value[1][0] + v.z * value[2][0] + v.w * value[3][0];
		value[4][1] += v.x * value[0][1] + v.y * value[1][1] + v.z * value[2][1] + v.w * value[3][1];
		value[4][2] += v.x * value[0][2] + v.y * value[1][2] + v.z * value[2][2] + v.w * value[3][2];
		value[4][3] += v.x * value[0][3] + v.y * value[1][3] + v.z * value[2][3] + v.w * value[3][3];
	}

	void Mat5::scale(const glm::vec4& s)
	{
		for (int col = 0; col < 4; ++col)
		{
			for (int row = 0; row < 5; ++row)
			{
				value[col][row] *= s[col];
			}
		}
	}

	Mat5 createCamera(const glm::vec4& eye, const glm::vec4& forward, const glm::vec4& up, const glm::vec4& right, const glm::vec4& over)
	{
		Mat5 cameraD{ 1 };

		cameraD[0][0] = right.x;
		cameraD[1][0] = right.y;
		cameraD[2][0] = right.z;
		cameraD[3][0] = right.w;
		cameraD[0][1] = up.x;
		cameraD[1][1] = up.y;
		cameraD[2][1] = up.z;
		cameraD[3][1] = up.w;
		cameraD[0][2] = -forward.x;
		cameraD[1][2] = -forward.y;
		cameraD[2][2] = -forward.z;
		cameraD[3][2] = -forward.w;
		cameraD[0][3] = over.x;
		cameraD[1][3] = over.y;
		cameraD[2][3] = over.z;
		cameraD[3][3] = over.w;

		Mat5 cameraP{ 1 };
		cameraP[4][0] = -eye.x;
		cameraP[4][1] = -eye.y;
		cameraP[4][2] = -eye.z;
		cameraP[4][3] = -eye.w;

		return cameraD * cameraP;
	}

	// untested
	Mat5 perspective(float fovy, float aspect, float zNear, float zFar)
	{
		// https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/building-basic-perspective-projection-matrix.html
		Mat5 res{ 0.0f };

		float scale = 1.0f / glm::tan(glm::radians(fovy / 2.0f));
		res[0][0] = scale;
		res[1][1] = scale;
		res[2][2] = -zFar / (zFar - zNear);
		res[3][3] = scale;
		res[4][2] = -(zFar * zNear) / (zFar - zNear);
		res[2][4] = -1.0f;

		return res;
	}

	glm::vec4 adjustToMaxHorizSpeed(const glm::vec4& vel, const glm::vec4& deltaVel, float maxHorizSpeed)
	{
		glm::vec4 totalVel = vel + deltaVel;

		// the max horizontal speed can be thought of as the radius of a sphere centered at the origin.
		// a line segment between vel and totalVel will intersect the sphere's surface at the desired velocity.
		float radius = maxHorizSpeed;
		glm::vec4 intersection;

		glm::vec3 a{ vel.x, vel.z, vel.w };
		glm::vec3 b{ totalVel.x, totalVel.z, totalVel.w };

		// code adapted from https://stackoverflow.com/a/17499940/4885160
		double px = a.x;
		double py = a.y;
		double pz = a.z;

		double vx = b.x - px;
		double vy = b.y - py;
		double vz = b.z - pz;

		double A = vx * vx + vy * vy + vz * vz;
		double B = 2.0 * (px * vx + py * vy + pz * vz /*- vx * cx - vy * cy - vz * cz*/);
		double C = px * px /*- 2 * px * cx + cx * cx*/ + py * py /*- 2 * py * cy + cy * cy*/ +
			pz * pz /*- 2 * pz * cz + cz * cz*/ - radius * radius;

		if (A == 0.0)
		{
			// prevent division by zero
			return glm::vec4{ 0, totalVel.y, 0, 0 };
		}

		// discriminant
		double D = B * B - 4 * A * C;

		if (D < 0)
		{
			// prevent NaN
			return glm::vec4{ 0, totalVel.y, 0, 0 };
		}

		// there will always be two intersections for this line
		double t1 = (-B - glm::sqrt(D)) / (2.0 * A);
		glm::vec3 solution1 = {
			a.x * (1 - t1) + t1 * b.x,
			a.y * (1 - t1) + t1 * b.y,
			a.z * (1 - t1) + t1 * b.z
		};

		if (D == 0)
		{
			return { solution1.x, totalVel.y, solution1.y, solution1.z };
		}

		double t2 = (-B + glm::sqrt(D)) / (2.0 * A);
		glm::vec3 solution2 = {
			a.x * (1 - t2) + t2 * b.x,
			a.y * (1 - t2) + t2 * b.y,
			a.z * (1 - t2) + t2 * b.z
		};

		// prefer a solution that's on the line segment itself
		if (glm::abs(t1 - 0.5) < glm::abs(t2 - 0.5))
		{
			return intersection = glm::vec4{ solution1.x, totalVel.y, solution1.y, solution1.z };
		}

		return intersection = glm::vec4{ solution2.x, totalVel.y, solution2.y, solution2.z };
	}
}