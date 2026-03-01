#include "Math5D.h"

using namespace m5;

BiVector4::BiVector4(float v)
	: xy(v), xz(v), xw(v), yz(v), yw(v), zw(v) {}
BiVector4::BiVector4(float xy, float xz, float xw, float yz, float yw, float zw)
	: xy(xy), xz(xz), xw(xw), yz(yz), yw(yw), zw(zw) {}
BiVector4::BiVector4(const BiVector4& other)
	: xy(other.xy), xz(other.xz), xw(other.xw), yz(other.yz), yw(other.yw), zw(other.zw) {}

BiVector4::BiVector4(BiVector4&& other) noexcept
	: xy(other.xy), xz(other.xz), xw(other.xw), yz(other.yz), yw(other.yw), zw(other.zw)
{
	other.xy = 0;
	other.xz = 0;
	other.xw = 0;
	other.yz = 0;
	other.yw = 0;
	other.zw = 0;
}
BiVector4& BiVector4::operator=(const BiVector4& other)
{
	this->xy = other.xy;
	this->xz = other.xz;
	this->xw = other.xw;
	this->yz = other.yz;
	this->yw = other.yw;
	this->zw = other.zw;

	return *this;
}
BiVector4& BiVector4::operator=(BiVector4&& other) noexcept
{
	if (this != &other)
	{
		this->xy = other.xy;
		this->xz = other.xz;
		this->xw = other.xw;
		this->yz = other.yz;
		this->yw = other.yw;
		this->zw = other.zw;
		other.xy = 0;
		other.xz = 0;
		other.xw = 0;
		other.yz = 0;
		other.yw = 0;
		other.zw = 0;
	}

	return *this;
}

float& BiVector4::operator[](size_t i)
{
	if (i >= 6) throw std::out_of_range("m5::BiVector4::operator[]: index out of range.");

	return (&xy)[i];
}
const float& BiVector4::operator[](size_t i) const
{
	if (i >= 6) throw std::out_of_range("m5::BiVector4::operator[]: index out of range.");

	return (&xy)[i];
}

BiVector4 BiVector4::operator+(const BiVector4& other) const
{
	return BiVector4
	{
		this->xy + other.xy,
		this->xz + other.xz,
		this->xw + other.xw,
		this->yz + other.yz,
		this->yw + other.yw,
		this->zw + other.zw
	};
}
BiVector4 BiVector4::operator+(float v) const
{
	return BiVector4
	{
		this->xy + v,
		this->xz + v,
		this->xw + v,
		this->yz + v,
		this->yw + v,
		this->zw + v
	};
}
BiVector4 BiVector4::operator-(const BiVector4& other) const
{
	return BiVector4
	{
		this->xy - other.xy,
		this->xz - other.xz,
		this->xw - other.xw,
		this->yz - other.yz,
		this->yw - other.yw,
		this->zw - other.zw
	};
}
BiVector4 BiVector4::operator-(float v) const
{
	return BiVector4
	{
		this->xy - v,
		this->xz - v,
		this->xw - v,
		this->yz - v,
		this->yw - v,
		this->zw - v
	};
}
BiVector4 BiVector4::operator-() const
{
	return BiVector4
	{
		-this->xy,
		-this->xz,
		-this->xw,
		-this->yz,
		-this->yw,
		-this->zw
	};
}
BiVector4 BiVector4::operator*(const BiVector4& other) const
{
	return BiVector4
	{
		this->xy * other.xy,
		this->xz * other.xz,
		this->xw * other.xw,
		this->yz * other.yz,
		this->yw * other.yw,
		this->zw * other.zw
	};
}
BiVector4 BiVector4::operator*(float v) const
{
	return BiVector4
	{
		this->xy * v,
		this->xz * v,
		this->xw * v,
		this->yz * v,
		this->yw * v,
		this->zw * v
	};
}
BiVector4 BiVector4::operator/(const BiVector4& other) const
{
	return BiVector4
	{
		this->xy / other.xy,
		this->xz / other.xz,
		this->xw / other.xw,
		this->yz / other.yz,
		this->yw / other.yw,
		this->zw / other.zw
	};
}
BiVector4 BiVector4::operator/(float v) const
{
	float invV = 1.0f / v;
	return BiVector4
	{
		this->xy * invV,
		this->xz * invV,
		this->xw * invV,
		this->yz * invV,
		this->yw * invV,
		this->zw * invV
	};
}

BiVector4& BiVector4::operator+=(const BiVector4& other)
{
	return *this = *this + other;
}
BiVector4& BiVector4::operator+=(float v)
{
	return *this = *this + v;
}
BiVector4& BiVector4::operator-=(const BiVector4& other)
{
	return *this = *this - other;
}
BiVector4& BiVector4::operator-=(float v)
{
	return *this = *this - v;
}
BiVector4& BiVector4::operator*=(const BiVector4& other)
{
	return *this = *this * other;
}
BiVector4& BiVector4::operator*=(float v)
{
	return *this = *this * v;
}
BiVector4& BiVector4::operator/=(const BiVector4& other)
{
	return *this = *this / other;
}
BiVector4& BiVector4::operator/=(float v)
{
	return *this = *this / v;
}

bool BiVector4::operator==(const BiVector4& other) const
{
	return this->xy == other.xy && this->xz == other.xz && this->xw == other.xw && this->yz == other.yz && this->yw == other.yw && this->zw == other.zw;
}

BiVector4 BiVector4::normalized() const
{
	BiVector4 v = *this;
	v.normalize();
	return v;
}
BiVector4& BiVector4::normalize()
{
	float len = length2(*this);
	if (len <= glm::epsilon<float>() * glm::epsilon<float>())
	{
		return *this;
	}
	return *this /= glm::sqrt(len);
}

float m5::dot(const BiVector4& a, const BiVector4& b)
{
	return a.xy * b.xy + a.xz * b.xz + a.xw * b.xw + a.yz * b.yz + a.yw * b.yw + a.zw * b.zw;
}
glm::vec4 m5::cross(const glm::vec4& u, const glm::vec4& v, const glm::vec4& w)
{
	// intermediate values
	float a = (v.x * w.y) - (v.y * w.x);
	float b = (v.x * w.z) - (v.z * w.x);
	float c = (v.x * w.w) - (v.w * w.x);
	float d = (v.y * w.z) - (v.z * w.y);
	float e = (v.y * w.w) - (v.w * w.y);
	float f = (v.z * w.w) - (v.w * w.z);

	// result vector
	glm::vec4 res;

	res.x = (u.y * f) - (u.z * e) + (u.w * d);
	res.y = -(u.x * f) + (u.z * c) - (u.w * b);
	res.z = (u.x * e) - (u.y * c) + (u.w * a);
	res.w = -(u.x * d) + (u.y * b) - (u.z * a);

	return res;
}
BiVector4 m5::wedge(const glm::vec4& u, const glm::vec4& v)
{
	BiVector4 result{};

	result.xy = u.x * v.y - u.y * v.x;
	result.xz = u.x * v.z - u.z * v.x;
	result.xw = u.x * v.w - u.w * v.x;
	result.yz = u.y * v.z - u.z * v.y;
	result.yw = u.y * v.w - u.w * v.y;
	result.zw = u.z * v.w - u.w * v.z;

	return result.normalized();
}
BiVector4 m5::normalize(const BiVector4& v)
{
	return v.normalized();
}
float m5::length(const BiVector4& v)
{
	return glm::sqrt(length2(v));
}
float m5::length2(const BiVector4& v)
{
	return dot(v, v);
}

Rotor4D::Rotor4D(float a, BiVector4 b, float xyzw)
	: a(a), b(b), xyzw(xyzw) {}
Rotor4D::Rotor4D(const BiVector4& plane, float radians)
{
	float cosHalf = glm::cos(radians * 0.5f);
	float sinHalf = glm::sin(radians * 0.5f);

	a = cosHalf;

	b = plane;
	b *= sinHalf;
}
Rotor4D::Rotor4D(const glm::vec4& from, const glm::vec4& to)
{
	float dot = glm::max(-1.0f, glm::min(1.0f, glm::dot(from, to)));
	if (dot >= 1.0f - glm::epsilon<float>())
	{
		return;
	}

	// calculate the half angle
	float radians = glm::acos(dot);
	a = glm::cos(radians / 2.f);
	float sina = glm::sin(radians / 2.f);
	// the left side of the products have b a, not a b, so flip
	b = wedge(to, from);
	b *= sina;

	normalize();
}

Rotor4D::Rotor4D(const BiVector4& u, const BiVector4& v)
{
	a =
		- u.xy * v.xy
		- u.xz * v.xz
		- u.xw * v.xw
		- u.yz * v.yz
		- u.yw * v.yw
		- u.zw * v.zw;

	b = BiVector4
	{
		-u.xw * v.yw - u.xz * v.yz + u.yw * v.xw + u.yz * v.xz,
		-u.xw * v.zw + u.xy * v.yz - u.yz * v.xy + u.zw * v.xw,
		+u.xy * v.yw + u.xz * v.zw - u.yw * v.xy - u.zw * v.xz,
		-u.xy * v.xz + u.xz * v.xy - u.yw * v.zw + u.zw * v.yw,
		+u.xw * v.xy - u.xy * v.xw + u.yz * v.zw - u.zw * v.yz,
		+u.xw * v.xz - u.xz * v.xw + u.yw * v.yz - u.yz * v.yw
	};

	xyzw =
		+ u.xw * v.yz
		+ u.xy * v.zw
		- u.xz * v.yw
		- u.yw * v.xz
		+ u.yz * v.xw
		+ u.zw * v.xy;
}

Rotor4D::Rotor4D(const Rotor4D& other)
	: a(other.a), b(other.b), xyzw(other.xyzw) {}
Rotor4D::Rotor4D(Rotor4D&& other) noexcept
	: a(other.a), b(other.b), xyzw(other.xyzw)
{
	other.a = 1.0f;
	other.b = 0.0f;
	other.xyzw = 0.0f;
}
Rotor4D& Rotor4D::operator=(const Rotor4D& other)
{
	this->a = other.a;
	this->b = other.b;
	this->xyzw = other.xyzw;

	return *this;
}
Rotor4D& Rotor4D::operator=(Rotor4D&& other) noexcept
{
	if (this != &other)
	{
		this->a = other.a;
		this->b = other.b;
		this->xyzw = other.xyzw;
		other.a = 1.0f;
		other.b = 0.0f;
		other.xyzw = 0.0f;
	}

	return *this;
}

Rotor4D Rotor4D::operator*(const Rotor4D& r) const
{
	Rotor4D result{ this->b, r.b };

	result.a +=
		r.a * this->a +
		r.xyzw * this->xyzw;
	result.b +=
		r.b * this->a +
		this->b * r.a +
		BiVector4 // r.b * this->xyzw
		{
			-r.b.zw * this->xyzw,
			+r.b.yw * this->xyzw,
			-r.b.yz * this->xyzw,
			-r.b.xw * this->xyzw,
			+r.b.xz * this->xyzw,
			-r.b.xy * this->xyzw,
		} +
		BiVector4 // this->b * r.xyzw
		{
			-this->b.zw * r.xyzw,
			+this->b.yw * r.xyzw,
			-this->b.yz * r.xyzw,
			-this->b.xw * r.xyzw,
			+this->b.xz * r.xyzw,
			-this->b.xy * r.xyzw,
		};
	result.xyzw +=
		this->a * r.xyzw +
		r.a * this->xyzw;

	return result;
}
Rotor4D& Rotor4D::operator*=(const Rotor4D& r)
{
	return *this = *this * r;
}
glm::vec4 Rotor4D::rotate(const glm::vec4& v) const
{
	float xyz = b.xy * v.z - b.xz * v.y + b.yz * v.x + xyzw * v.w;
	float xyw = -b.xw * v.y + b.xy * v.w + b.yw * v.x + -xyzw * v.z;
	float xzw = -b.xw * v.z + b.xz * v.w + b.zw * v.x + xyzw * v.y;
	float yzw = -b.yw * v.z + b.yz * v.w + b.zw * v.y + -xyzw * v.x;

	float sX = a * v.x + b.xw * v.w + b.xy * v.y + b.xz * v.z;
	float sY = a * v.y + -b.xy * v.x + b.yw * v.w + b.yz * v.z;
	float sZ = a * v.z + -b.xz * v.x - b.yz * v.y + b.zw * v.w;
	float sW = a * v.w + -b.xw * v.x - b.yw * v.y - b.zw * v.z;

	return glm::vec4
	{
		a * sX + (-sY * -b.xy - sZ * -b.xz - sW * -b.xw) + (b.yw * xyw + b.yz * xyz + b.zw * xzw) + (xyzw * yzw),
		a * sY + (sX * -b.xy - sZ * -b.yz - sW * -b.yw) + (-b.xw * xyw - b.xz * xyz + b.zw * yzw) + (-xyzw * xzw),
		a * sZ + (sX * -b.xz + sY * -b.yz - sW * -b.zw) + (-b.xw * xzw + b.xy * xyz - b.yw * yzw) + (xyzw * xyw),
		a * sW + (sX * -b.xw + sY * -b.yw + sZ * -b.zw) + (b.xy * xyw + b.xz * xzw + b.yz * yzw) + (-xyzw * xyz)
	};
}

Rotor4D Rotor4D::operator-() const
{
	return Rotor4D{ a, -b, xyzw };
}
bool Rotor4D::operator==(const Rotor4D& other) const
{
	return this->a == other.a && this->b == other.b && this->xyzw == other.xyzw;
}

Rotor4D Rotor4D::normalized() const
{
	Rotor4D v = *this;
	v.normalize();
	return v;
}
Rotor4D& Rotor4D::normalize()
{
	float len = length2(*this);
	if (len <= glm::epsilon<float>() * glm::epsilon<float>())
	{
		return *this;
	}
	len = 1.0f / glm::sqrt(len);
	this->a *= len;
	this->b *= len;
	this->xyzw *= len;

	return *this;
}

Rotor4D::operator glm::mat4() const
{
	return glm::mat4
	{
		rotate({ 1, 0, 0, 0 }),
		rotate({ 0, 1, 0, 0 }),
		rotate({ 0, 0, 1, 0 }),
		rotate({ 0, 0, 0, 1 })
	};
}

Rotor4D m5::normalize(const Rotor4D& v)
{
	return v.normalized();
}
float m5::length(const Rotor4D& v)
{
	return glm::sqrt(length2(v));
}
float m5::length2(const Rotor4D& v)
{
	return v.a * v.a + length2(v.b) + v.xyzw * v.xyzw;
}

Mat5::Mat5(float x)
{
	value[0][0] =
	value[1][1] =
	value[2][2] =
	value[3][3] =
	value[4][4] = x;
}
Mat5::Mat5(const glm::mat4& m)
{
	value[0] = m[0];
	value[1] = m[1];
	value[2] = m[2];
	value[3] = m[3];
	value[4][4] = 1.0f;
}

Mat5::Mat5(const std::array<vec5, 5>& value)
	: value(value)
{
}
Mat5::Mat5(const vec5& m0, const vec5& m1, const vec5& m2, const vec5& m3, const vec5& m4)
{
	value[0] = m0;
	value[1] = m1;
	value[2] = m2;
	value[3] = m3;
	value[4] = m4;
}

Mat5::Mat5(const Mat5& other)
{
	this->value = other.value;
}
Mat5::Mat5(Mat5&& other) noexcept
{
	this->value = std::move(other.value);
}
Mat5& Mat5::operator=(const Mat5& other)
{
	this->value = other.value;

	return *this;
}
Mat5& Mat5::operator=(Mat5&& other) noexcept
{
	this->value = std::move(other.value);

	return *this;
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
Mat5& Mat5::operator*=(const Mat5& other)
{
	return *this = *this * other;
}

vec5 Mat5::multiply(const vec5& v) const
{
	return vec5
	{
		value[0][0] * v[0] + value[1][0] * v[1] + value[2][0] * v[2] + value[3][0] * v[3] + value[4][0] * v[4],
		value[0][1] * v[0] + value[1][1] * v[1] + value[2][1] * v[2] + value[3][1] * v[3] + value[4][1] * v[4],
		value[0][2] * v[0] + value[1][2] * v[1] + value[2][2] * v[2] + value[3][2] * v[3] + value[4][2] * v[4],
		value[0][3] * v[0] + value[1][3] * v[1] + value[2][3] * v[2] + value[3][3] * v[3] + value[4][3] * v[4],
		value[0][4] * v[0] + value[1][4] * v[1] + value[2][4] * v[2] + value[3][4] * v[3] + value[4][4] * v[4]
	};
}
glm::vec4 Mat5::multiply(const glm::vec4& v, float finalComp) const
{
	return multiply(vec5{ v, finalComp }).xyzw();
}
vec5 Mat5::operator*(const vec5& v) const
{
	return multiply(v);
}
glm::vec4 Mat5::operator*(const glm::vec4& v) const
{
	return multiply(v, 1.0f);
}

vec5& Mat5::operator[](size_t i)
{
	if (i >= 5) throw std::out_of_range("m5::Mat5::operator[]: index out of range.");
	return value[i];
}
const vec5& Mat5::operator[](size_t i) const
{
	if (i >= 5) throw std::out_of_range("m5::Mat5::operator[]: index out of range.");
	return value[i];
}

void Mat5::translate(const glm::vec4& v)
{
	value[4][0] += (value[0][0] * v.x) + (value[1][0] * v.y) + (value[2][0] * v.z) + (value[3][0] * v.w);
	value[4][1] += (value[0][1] * v.x) + (value[1][1] * v.y) + (value[2][1] * v.z) + (value[3][1] * v.w);
	value[4][2] += (value[0][2] * v.x) + (value[1][2] * v.y) + (value[2][2] * v.z) + (value[3][2] * v.w);
	value[4][3] += (value[0][3] * v.x) + (value[1][3] * v.y) + (value[2][3] * v.z) + (value[3][3] * v.w);
}
void Mat5::translate(float x, float y, float z, float w)
{
	translate({ x,y,z,w });
}
void Mat5::translate(float a)
{
	translate(a, a, a, a);
}
void Mat5::scale(const glm::vec4& s)
{
	for (int row = 0; row < 5; ++row)
		for (int col = 0; col < 4; ++col)
			value[col][row] *= s[col];
}
void Mat5::scale(float x, float y, float z, float w)
{
	scale({ x,y,z,w });
}
void Mat5::scale(float s)
{
	scale(s, s, s, s);
}

Mat5 Mat5::transpose(const Mat5& m)
{
	return Mat5
	{
		{ m[0][0],m[1][0],m[2][0],m[3][0],m[4][0] },
		{ m[0][1],m[1][1],m[2][1],m[3][1],m[4][1] },
		{ m[0][2],m[1][2],m[2][2],m[3][2],m[4][2] },
		{ m[0][3],m[1][3],m[2][3],m[3][3],m[4][3] },
		{ m[0][4],m[1][4],m[2][4],m[3][4],m[4][4] }
	};
}
Mat5 Mat5::inverse(const Mat5& m)
{
	Mat5 s{ 1 };
	Mat5 t{ m };

	// Forward elimination
	for (int i = 0; i < 5 - 1; ++i)
	{
		int pivot = i;
		float pivotsize = t[i][i];

		for (int j = i + 1; j < 5; ++j)
		{
			float tmp = t[j][i];
			if (fabs(tmp) > fabs(pivotsize))
			{
				pivotsize = tmp;
				pivot = j;
			}
		}

		if (pivotsize == 0.0f) return s;

		if (pivot != i)
		{
			for (int j = 0; j < 5; ++j)
			{
				float tmp;
				tmp = t[i][j];
				t[i][j] = t[pivot][j];
				t[pivot][j] = tmp;
				tmp = s[i][j];
				s[i][j] = s[pivot][j];
				s[pivot][j] = tmp;
			}
		}

		for (int j = i + 1; j < 5; ++j)
		{
			float f = t[j][i] / t[i][i];
			for (int k = i + 1; k < 5; ++k)
			{
				t[j][k] -= f * t[i][k];
			}
			for (int k = 0; k < 5; ++k)
			{
				s[j][k] -= f * s[i][k];
			}
			t[j][i] = 0.0f;
		}
	}

	// Backward substitution
	for (int i = 0; i < 5; ++i)
	{
		float f = t[i][i];
		if (f == 0.0f) return Mat5{ 1 };

		for (int j = 0; j < 5; ++j)
		{
			t[i][j] /= f;
			s[i][j] /= f;
		}

		for (int j = 0; j < i; ++j)
		{
			f = t[j][i];
			for (int k = 0; k < 5; ++k)
			{
				t[j][k] -= f * t[i][k];
				s[j][k] -= f * s[i][k];
			}
		}
	}

	return s;
}
Mat5::operator glm::mat4() const
{
	return glm::mat4
	{
		value[0].xyzw(),
		value[1].xyzw(),
		value[2].xyzw(),
		value[3].xyzw()
	};
}

Mat5 m5::createCamera4D(const glm::vec4& eye, const glm::vec4& left, const glm::vec4& up, const glm::vec4& forward, const glm::vec4& over)
{
	Mat5 cameraD{ -left, up, forward, over, {} };
	cameraD = Mat5::transpose(cameraD);

	Mat5 cameraP{ {}, {}, {}, {}, -eye };

	return cameraD * cameraP;
}

Mat6::Mat6(float x)
{
	value[0][0] =
		value[1][1] =
		value[2][2] =
		value[3][3] =
		value[4][4] =
		value[5][5] = x;
}
Mat6::Mat6(const Mat5& m)
{
	value[0] = m[0];
	value[1] = m[1];
	value[2] = m[2];
	value[3] = m[3];
	value[4] = m[4];
	value[5][5] = 1.0f;
}

Mat6::Mat6(const std::array<vec6, 6>& value)
	: value(value)
{
}
Mat6::Mat6(const vec6& m0, const vec6& m1, const vec6& m2, const vec6& m3, const vec6& m4, const vec6& m5)
{
	value[0] = m0;
	value[1] = m1;
	value[2] = m2;
	value[3] = m3;
	value[4] = m4;
	value[5] = m5;
}

Mat6::Mat6(const Mat6& other)
{
	this->value = other.value;
}
Mat6::Mat6(Mat6&& other) noexcept
{
	this->value = std::move(other.value);
}
Mat6& Mat6::operator=(const Mat6& other)
{
	this->value = other.value;

	return *this;
}
Mat6& Mat6::operator=(Mat6&& other) noexcept
{
	this->value = std::move(other.value);

	return *this;
}

Mat6 Mat6::operator*(const Mat6& other) const
{
	Mat5 result;

	for (int col = 0; col < 6; ++col)
	{
		for (int row = 0; row < 6; ++row)
		{
			for (int k = 0; k < 6; ++k)
			{
				result.value[col][row] += value[k][row] * other.value[col][k];
			}
		}
	}

	return result;
}
Mat6& Mat6::operator*=(const Mat6& other)
{
	return *this = *this * other;
}

vec6 Mat6::multiply(const vec6& v) const
{
	return vec6
	{
		value[0][0] * v[0] + value[1][0] * v[1] + value[2][0] * v[2] + value[3][0] * v[3] + value[4][0] * v[4] + value[5][0] * v[5],
		value[0][1] * v[0] + value[1][1] * v[1] + value[2][1] * v[2] + value[3][1] * v[3] + value[4][1] * v[4] + value[5][1] * v[5],
		value[0][2] * v[0] + value[1][2] * v[1] + value[2][2] * v[2] + value[3][2] * v[3] + value[4][2] * v[4] + value[5][2] * v[5],
		value[0][3] * v[0] + value[1][3] * v[1] + value[2][3] * v[2] + value[3][3] * v[3] + value[4][3] * v[4] + value[5][3] * v[5],
		value[0][4] * v[0] + value[1][4] * v[1] + value[2][4] * v[2] + value[3][4] * v[3] + value[4][4] * v[4] + value[5][4] * v[5],
		value[0][5] * v[0] + value[1][5] * v[1] + value[2][5] * v[2] + value[3][5] * v[3] + value[4][5] * v[4] + value[5][5] * v[5]
	};
}
vec6 Mat6::operator*(const vec6& v) const
{
	return multiply(v);
}

vec6& Mat6::operator[](size_t i)
{
	if (i >= 6) throw std::out_of_range("m5::Mat6::operator[]: index out of range.");
	return value[i];
}
const vec6& Mat6::operator[](size_t i) const
{
	if (i >= 6) throw std::out_of_range("m5::Mat6::operator[]: index out of range.");
	return value[i];
}

void Mat6::translate(const vec5& v)
{
	value[5][0] += (value[0][0] * v.x) + (value[1][0] * v.y) + (value[2][0] * v.z) + (value[3][0] * v.w) + (value[4][0] * v.v);
	value[5][1] += (value[0][1] * v.x) + (value[1][1] * v.y) + (value[2][1] * v.z) + (value[3][1] * v.w) + (value[4][1] * v.v);
	value[5][2] += (value[0][2] * v.x) + (value[1][2] * v.y) + (value[2][2] * v.z) + (value[3][2] * v.w) + (value[4][2] * v.v);
	value[5][3] += (value[0][3] * v.x) + (value[1][3] * v.y) + (value[2][3] * v.z) + (value[3][3] * v.w) + (value[4][3] * v.v);
	value[5][4] += (value[0][4] * v.x) + (value[1][4] * v.y) + (value[2][4] * v.z) + (value[3][4] * v.w) + (value[4][4] * v.v);
}
void Mat6::translate(float x, float y, float z, float w, float v)
{
	translate({ x,y,z,w,v });
}
void Mat6::translate(float a)
{
	translate(a, a, a, a, a);
}
void Mat6::scale(const vec5& s)
{
	for (int row = 0; row < 6; ++row)
		for (int col = 0; col < 5; ++col)
			value[col][row] *= s[col];
}
void Mat6::scale(float x, float y, float z, float w, float v)
{
	scale({ x,y,z,w,v });
}
void Mat6::scale(float s)
{
	scale(s, s, s, s, s);
}

Mat6 Mat6::transpose(const Mat6& m)
{
	return Mat6
	{
		{ m[0][0],m[1][0],m[2][0],m[3][0],m[4][0],m[5][0] },
		{ m[0][1],m[1][1],m[2][1],m[3][1],m[4][1],m[5][1] },
		{ m[0][2],m[1][2],m[2][2],m[3][2],m[4][2],m[5][2] },
		{ m[0][3],m[1][3],m[2][3],m[3][3],m[4][3],m[5][3] },
		{ m[0][4],m[1][4],m[2][4],m[3][4],m[4][4],m[5][4] },
		{ m[0][5],m[1][5],m[2][5],m[3][5],m[4][5],m[5][5] }
	};
}
Mat6 Mat6::inverse(const Mat6& m)
{
	Mat6 s{ 1 };
	Mat6 t{ m };

	// Forward elimination
	for (int i = 0; i < 6 - 1; ++i)
	{
		int pivot = i;
		float pivotsize = t[i][i];

		for (int j = i + 1; j < 6; ++j)
		{
			float tmp = t[j][i];
			if (fabs(tmp) > fabs(pivotsize))
			{
				pivotsize = tmp;
				pivot = j;
			}
		}

		if (pivotsize == 0.0f) return s;

		if (pivot != i)
		{
			for (int j = 0; j < 6; ++j)
			{
				float tmp;
				tmp = t[i][j];
				t[i][j] = t[pivot][j];
				t[pivot][j] = tmp;
				tmp = s[i][j];
				s[i][j] = s[pivot][j];
				s[pivot][j] = tmp;
			}
		}

		for (int j = i + 1; j < 6; ++j)
		{
			float f = t[j][i] / t[i][i];
			for (int k = i + 1; k < 6; ++k)
			{
				t[j][k] -= f * t[i][k];
			}
			for (int k = 0; k < 6; ++k)
			{
				s[j][k] -= f * s[i][k];
			}
			t[j][i] = 0.0f;
		}
	}

	// Backward substitution
	for (int i = 0; i < 6; ++i)
	{
		float f = t[i][i];
		if (f == 0.0f) return Mat6{ 1 };

		for (int j = 0; j < 6; ++j)
		{
			t[i][j] /= f;
			s[i][j] /= f;
		}

		for (int j = 0; j < i; ++j)
		{
			f = t[j][i];
			for (int k = 0; k < 6; ++k)
			{
				t[j][k] -= f * t[i][k];
				s[j][k] -= f * s[i][k];
			}
		}
	}

	return s;
}
Mat6::operator Mat5() const
{
	return Mat5
	{
		value[0].xyzwv(),
		value[1].xyzwv(),
		value[2].xyzwv(),
		value[3].xyzwv(),
		value[4].xyzwv()
	};
}

// yonder - at some distance in the direction indicated; over there
Mat6 m5::createCamera5D(const vec5& eye, const vec5& left, const vec5& up, const vec5& forward, const vec5& over, const vec5& yonder)
{
	Mat6 cameraD{ -left, up, forward, over, yonder, {} };
	cameraD = Mat6::transpose(cameraD);

	Mat6 cameraP{ {}, {}, {}, {}, {}, -eye };

	return cameraD * cameraP;
}
