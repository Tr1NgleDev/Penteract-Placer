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
	BiVector4 b = wedge(v, w);

	// result vector
	glm::vec4 res;

	res.x = (u.y * b.zw) - (u.z * b.yw) + (u.w * b.yz);
	res.y = -(u.x * b.zw) + (u.z * b.xw) - (u.w * b.xz);
	res.z = (u.x * b.yw) - (u.y * b.xw) + (u.w * b.xy);
	res.w = -(u.x * b.yz) + (u.y * b.xz) - (u.z * b.xy);

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
	Mat6 result;

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
#define rotMat(mat, x, y, cos, sin) \
{ \
	Mat5 rot{ 1 }; \
	rot[x][x] = (cos); \
	rot[y][x] = -(sin); \
	rot[x][y] = (sin); \
	rot[y][y] = (cos); \
	mat *= rot; \
}

void Mat6::rotate(const BiVector5& planes, float radians)
{
	float cos = glm::cos(radians);
	float sin = glm::sin(radians);

	Mat5 tRot{ 1 };
	rotMat(tRot, 0, 1, planes[0] * cos, planes[0] * sin);
	rotMat(tRot, 0, 2, planes[1] * cos, planes[1] * sin);
	rotMat(tRot, 0, 3, planes[2] * cos, planes[2] * sin);
	rotMat(tRot, 0, 4, planes[3] * cos, planes[3] * sin);
	rotMat(tRot, 1, 2, planes[4] * cos, planes[4] * sin);
	rotMat(tRot, 1, 3, planes[5] * cos, planes[5] * sin);
	rotMat(tRot, 1, 4, planes[6] * cos, planes[6] * sin);
	rotMat(tRot, 2, 3, planes[7] * cos, planes[7] * sin);
	rotMat(tRot, 2, 4, planes[8] * cos, planes[8] * sin);
	rotMat(tRot, 3, 4, planes[9] * cos, planes[9] * sin);

	*this *= tRot;
}
void Mat6::rotateXY(float radians) { rotMat(*this, 0, 1, glm::cos(radians), glm::sin(radians)); }
void Mat6::rotateXZ(float radians) { rotMat(*this, 0, 2, glm::cos(radians), glm::sin(radians)); }
void Mat6::rotateXW(float radians) { rotMat(*this, 0, 3, glm::cos(radians), glm::sin(radians)); }
void Mat6::rotateXV(float radians) { rotMat(*this, 0, 4, glm::cos(radians), glm::sin(radians)); }
void Mat6::rotateYZ(float radians) { rotMat(*this, 1, 2, glm::cos(radians), glm::sin(radians)); }
void Mat6::rotateYW(float radians) { rotMat(*this, 1, 3, glm::cos(radians), glm::sin(radians)); }
void Mat6::rotateYV(float radians) { rotMat(*this, 1, 4, glm::cos(radians), glm::sin(radians)); }
void Mat6::rotateZW(float radians) { rotMat(*this, 2, 3, glm::cos(radians), glm::sin(radians)); }
void Mat6::rotateZV(float radians) { rotMat(*this, 2, 4, glm::cos(radians), glm::sin(radians)); }
void Mat6::rotateWV(float radians) { rotMat(*this, 3, 4, glm::cos(radians), glm::sin(radians)); }

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

BiVector5::BiVector5(float v)
	: xy(v), xz(v), xw(v), xv(v), yz(v), yw(v), yv(v), zw(v), zv(v), wv(v) {
}
BiVector5::BiVector5(float xy, float xz, float xw, float xv, float yz, float yw, float yv, float zw, float zv, float wv)
	: xy(xy), xz(xz), xw(xw), xv(xv), yz(yz), yw(yw), yv(yv), zw(zw), zv(zv), wv(wv) {
}
BiVector5::BiVector5(const BiVector5& other)
	: xy(other.xy), xz(other.xz), xw(other.xw), xv(other.xv), yz(other.yz), yw(other.yw), yv(other.yv), zw(other.zw), zv(other.zv), wv(other.wv) {
}

BiVector5::BiVector5(BiVector5&& other) noexcept
	: xy(other.xy), xz(other.xz), xw(other.xw), xv(other.xv), yz(other.yz), yw(other.yw), yv(other.yv), zw(other.zw), zv(other.zv), wv(other.wv)
{
	other.xy = 0;
	other.xz = 0;
	other.xw = 0;
	other.xv = 0;
	other.yz = 0;
	other.yw = 0;
	other.yv = 0;
	other.zw = 0;
	other.zv = 0;
	other.wv = 0;
}
BiVector5& BiVector5::operator=(const BiVector5& other)
{
	this->xy = other.xy;
	this->xz = other.xz;
	this->xw = other.xw;
	this->xv = other.xv;
	this->yz = other.yz;
	this->yw = other.yw;
	this->yv = other.yv;
	this->zw = other.zw;
	this->zv = other.zv;
	this->wv = other.wv;

	return *this;
}
BiVector5& BiVector5::operator=(BiVector5&& other) noexcept
{
	if (this != &other)
	{
		this->xy = other.xy;
		this->xz = other.xz;
		this->xw = other.xw;
		this->xv = other.xv;
		this->yz = other.yz;
		this->yw = other.yw;
		this->yv = other.yv;
		this->zw = other.zw;
		this->zv = other.zv;
		this->wv = other.wv;

		other.xy = 0;
		other.xz = 0;
		other.xw = 0;
		other.xv = 0;
		other.yz = 0;
		other.yw = 0;
		other.yv = 0;
		other.zw = 0;
		other.zv = 0;
		other.wv = 0;
	}

	return *this;
}

float& BiVector5::operator[](size_t i)
{
	if (i >= 10) throw std::out_of_range("m5::BiVector5::operator[]: index out of range.");
	return (&xy)[i];
}
const float& BiVector5::operator[](size_t i) const
{
	if (i >= 10) throw std::out_of_range("m5::BiVector5::operator[]: index out of range.");
	return (&xy)[i];
}

BiVector5 BiVector5::operator+(const BiVector5& other) const
{
	return BiVector5
	{
		this->xy + other.xy,
		this->xz + other.xz,
		this->xw + other.xw,
		this->xv + other.xv,
		this->yz + other.yz,
		this->yw + other.yw,
		this->yv + other.yv,
		this->zw + other.zw,
		this->zv + other.zv,
		this->wv + other.wv
	};
}
BiVector5 BiVector5::operator+(float v) const
{
	return BiVector5
	{
		this->xy + v,
		this->xz + v,
		this->xw + v,
		this->xv + v,
		this->yz + v,
		this->yw + v,
		this->yv + v,
		this->zw + v,
		this->zv + v,
		this->wv + v
	};
}
BiVector5 BiVector5::operator-(const BiVector5& other) const
{
	return BiVector5
	{
		this->xy - other.xy,
		this->xz - other.xz,
		this->xw - other.xw,
		this->xv - other.xv,
		this->yz - other.yz,
		this->yw - other.yw,
		this->yv - other.yv,
		this->zw - other.zw,
		this->zv - other.zv,
		this->wv - other.wv
	};
}
BiVector5 BiVector5::operator-(float v) const
{
	return BiVector5
	{
		this->xy - v,
		this->xz - v,
		this->xw - v,
		this->xv - v,
		this->yz - v,
		this->yw - v,
		this->yv - v,
		this->zw - v,
		this->zv - v,
		this->wv - v
	};
}
BiVector5 BiVector5::operator-() const
{
	return BiVector5
	{
		-this->xy,
		-this->xz,
		-this->xw,
		-this->xv,
		-this->yz,
		-this->yw,
		-this->yv,
		-this->zw,
		-this->zv,
		-this->wv
	};
}
BiVector5 BiVector5::operator*(const BiVector5& other) const
{
	return BiVector5
	{
		this->xy * other.xy,
		this->xz * other.xz,
		this->xw * other.xw,
		this->xv * other.xv,
		this->yz * other.yz,
		this->yw * other.yw,
		this->yv * other.yv,
		this->zw * other.zw,
		this->zv * other.zv,
		this->wv * other.wv
	};
}
BiVector5 BiVector5::operator*(float v) const
{
	return BiVector5
	{
		this->xy * v,
		this->xz * v,
		this->xw * v,
		this->xv * v,
		this->yz * v,
		this->yw * v,
		this->yv * v,
		this->zw * v,
		this->zv * v,
		this->wv * v
	};
}
BiVector5 BiVector5::operator/(const BiVector5& other) const
{
	return BiVector5
	{
		this->xy / other.xy,
		this->xz / other.xz,
		this->xw / other.xw,
		this->xv / other.xv,
		this->yz / other.yz,
		this->yw / other.yw,
		this->yv / other.yv,
		this->zw / other.zw,
		this->zv / other.zv,
		this->wv / other.wv
	};
}
BiVector5 BiVector5::operator/(float v) const
{
	float invV = 1.0f / v;
	return BiVector5
	{
		this->xy * invV,
		this->xz * invV,
		this->xw * invV,
		this->xv * invV,
		this->yz * invV,
		this->yw * invV,
		this->yv * invV,
		this->zw * invV,
		this->zv * invV,
		this->wv * invV
	};
}

BiVector5& BiVector5::operator+=(const BiVector5& other) { return *this = *this + other; }
BiVector5& BiVector5::operator+=(float v) { return *this = *this + v; }
BiVector5& BiVector5::operator-=(const BiVector5& other) { return *this = *this - other; }
BiVector5& BiVector5::operator-=(float v) { return *this = *this - v; }
BiVector5& BiVector5::operator*=(const BiVector5& other) { return *this = *this * other; }
BiVector5& BiVector5::operator*=(float v) { return *this = *this * v; }
BiVector5& BiVector5::operator/=(const BiVector5& other) { return *this = *this / other; }
BiVector5& BiVector5::operator/=(float v) { return *this = *this / v; }

bool BiVector5::operator==(const BiVector5& other) const
{
	return this->xy == other.xy && this->xz == other.xz && this->xw == other.xw && this->xv == other.xv &&
		this->yz == other.yz && this->yw == other.yw && this->yv == other.yv && this->zw == other.zw && this->zv == other.zv && this->wv == other.wv;
}

BiVector5 BiVector5::normalized() const
{
	BiVector5 v = *this;
	v.normalize();
	return v;
}
BiVector5& BiVector5::normalize()
{
	float len = length2(*this);
	if (len <= glm::epsilon<float>() * glm::epsilon<float>())
	{
		return *this;
	}
	return *this /= glm::sqrt(len);
}

float m5::dot(const BiVector5& a, const BiVector5& b)
{
	return
		a.xy * b.xy + a.xz * b.xz + a.xw * b.xw + a.xv * b.xv +
		a.yz * b.yz + a.yw * b.yw + a.yv * b.yv +
		a.zw * b.zw + a.zv * b.zv +
		a.wv * b.wv;
}

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

vec5 m5::cross(const vec5& u, const vec5& v, const vec5& w, const vec5& z)
{
	float cx = det4_cols(u, v, w, z, 1, 2, 3, 4);
	float cy = -det4_cols(u, v, w, z, 0, 2, 3, 4);
	float cz = det4_cols(u, v, w, z, 0, 1, 3, 4);
	float cw = -det4_cols(u, v, w, z, 0, 1, 2, 4);
	float cv = det4_cols(u, v, w, z, 0, 1, 2, 3);
	return vec5{ cx, cy, cz, cw, cv };
}

BiVector5 m5::wedge(const vec5& u, const vec5& v)
{
	BiVector5 result{};

	result.xy = u.x * v.y - u.y * v.x;
	result.xz = u.x * v.z - u.z * v.x;
	result.xw = u.x * v.w - u.w * v.x;
	result.xv = u.x * v.v - u.v * v.x;
	result.yz = u.y * v.z - u.z * v.y;
	result.yw = u.y * v.w - u.w * v.y;
	result.yv = u.y * v.v - u.v * v.y;
	result.zw = u.z * v.w - u.w * v.z;
	result.zv = u.z * v.v - u.v * v.z;
	result.wv = u.w * v.v - u.v * v.w;

	return result.normalized();
}

BiVector5 m5::normalize(const BiVector5& v)
{
	return v.normalized();
}
float m5::length(const BiVector5& v)
{
	return glm::sqrt(length2(v));
}
float m5::length2(const BiVector5& v)
{
	return dot(v, v);
}

Rotor5D::Rotor5D(float a, BiVector5 b, const vec5& q)
	: a(a), b(b), q(q) {
}

Rotor5D::Rotor5D(const BiVector5& plane, float radians)
{
	float cosHalf = glm::cos(radians * 0.5f);
	float sinHalf = glm::sin(radians * 0.5f);

	a = cosHalf;

	b = plane;
	b *= sinHalf;
}
Rotor5D::Rotor5D(const vec5& from, const vec5& to)
{
	float dot = glm::max(-1.0f, glm::min(1.0f, m5::dot(from, to)));
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

Rotor5D::Rotor5D(const Rotor5D& other)
	: a(other.a), b(other.b), q(other.q) {
}

Rotor5D::Rotor5D(Rotor5D&& other) noexcept
	: a(other.a), b(other.b), q(other.q)
{
	other.a = 1.0f;
	other.b = 0.0f;
	other.q = {};
}

Rotor5D& Rotor5D::operator=(const Rotor5D& other)
{
	this->a = other.a;
	this->b = other.b;
	this->q = other.q;
	return *this;
}

Rotor5D& Rotor5D::operator=(Rotor5D&& other) noexcept
{
	if (this != &other)
	{
		this->a = other.a;
		this->b = other.b;
		this->q = other.q;

		other.a = 1.0f;
		other.b = 0.0f;
		other.q = {};
	}
	return *this;
}

Rotor5D Rotor5D::operator*(const Rotor5D& r) const
{
	float a = this->a;
	float xy = this->b.xy, xz = this->b.xz, xw = this->b.xw, xv = this->b.xv;
	float yz = this->b.yz, yw = this->b.yw, yv = this->b.yv;
	float zw = this->b.zw, zv = this->b.zv, wv = this->b.wv;
	float qx = this->q.x, qy = this->q.y, qz = this->q.z, qw = this->q.w, qv = this->q.v;

	float ra = r.a;
	float rxy = r.b.xy, rxz = r.b.xz, rxw = r.b.xw, rxv = r.b.xv;
	float ryz = r.b.yz, ryw = r.b.yw, ryv = r.b.yv;
	float rzw = r.b.zw, rzv = r.b.zv, rwv = r.b.wv;
	float rqx = r.q.x, rqy = r.q.y, rqz = r.q.z, rqw = r.q.w, rqv = r.q.v;

	Rotor5D result{};
	result.a = +a * ra - wv * rwv - xv * rxv - xw * rxw - xy * rxy - xz * rxz - yv * ryv - yw * ryw - yz * ryz - zv * rzv - zw * rzw + qv * rqv + qw * rqw + qx * rqx + qy * rqy + qz * rqz;
	result.b.xy = +a * rxy - wv * rqz - xv * ryv - xw * ryw + xy * ra - xz * ryz + yv * rxv + yw * rxw + yz * rxz - zv * rqw - zw * rqv - qv * rzw - qw * rzv - qx * rqy + qy * rqx - qz * rwv;
	result.b.xz = +a * rxz - wv * rqy - xv * rzv - xw * rzw + xy * ryz + xz * ra + yv * rqw + yw * rqv - yz * rxy + zv * rxv + zw * rxw + qv * ryw + qw * ryv + qx * rqz - qy * rwv - qz * rqx;
	result.b.yz = +a * ryz - wv * rqx - xv * rqw - xw * rqv - xy * rxz + xz * rxy - yv * rzv - yw * rzw + yz * ra + zv * ryv + zw * ryw - qv * rxw - qw * rxv - qx * rwv - qy * rqz + qz * rqy;
	result.b.xw = +a * rxw + wv * rxv - xv * rwv + xw * ra + xy * ryw + xz * rzw + yv * rqz - yw * rxy - yz * rqv + zv * rqy - zw * rxz - qv * ryz + qw * rqx - qx * rqw + qy * rzv + qz * ryv;
	result.b.yw = +a * ryw + wv * ryv - xv * rqz + xw * rxy - xy * rxw + xz * rqv - yv * rwv + yw * ra + yz * rzw + zv * rqx - zw * ryz + qv * rxz - qw * rqy + qx * rzv + qy * rqw - qz * rxv;
	result.b.zw = +a * rzw + wv * rzv - xv * rqy + xw * rxz - xy * rqv - xz * rxw - yv * rqx + yw * ryz - yz * ryw - zv * rwv + zw * ra - qv * rxy + qw * rqz - qx * ryv - qy * rxv - qz * rqw;
	result.b.xv = +a * rxv - wv * rxw + xv * ra + xw * rwv + xy * ryv + xz * rzv - yv * rxy - yw * rqz - yz * rqw - zv * rxz - zw * rqy - qv * rqx - qw * ryz + qx * rqv - qy * rzw - qz * ryw;
	result.b.yv = +a * ryv - wv * ryw + xv * rxy + xw * rqz - xy * rxv + xz * rqw + yv * ra + yw * rwv + yz * rzv - zv * ryz - zw * rqx + qv * rqy + qw * rxz - qx * rzw - qy * rqv + qz * rxw;
	result.b.zv = +a * rzv - wv * rzw + xv * rxz + xw * rqy - xy * rqw - xz * rxv + yv * ryz + yw * rqx - yz * ryv + zv * ra + zw * rwv - qv * rqz - qw * rxy + qx * ryw + qy * rxw + qz * rqv;
	result.b.wv = +a * rwv + wv * ra + xv * rxw - xw * rxv - xy * rqz - xz * rqy + yv * ryw - yw * ryv - yz * rqx + zv * rzw - zw * rzv + qv * rqw - qw * rqv - qx * ryz - qy * rxz - qz * rxy;
	result.q.v = +a * rqv + wv * rqw - xv * rqx + xw * ryz + xy * rzw - xz * ryw + yv * rqy - yw * rxz + yz * rxw - zv * rqz + zw * rxy + qv * ra - qw * rwv + qx * rxv - qy * ryv + qz * rzv;
	result.q.w = +a * rqw - wv * rqv + xv * ryz + xw * rqx + xy * rzv - xz * ryv - yv * rxz - yw * rqy + yz * rxv + zv * rxy + zw * rqz + qv * rwv + qw * ra - qx * rxw + qy * ryw - qz * rzw;
	result.q.z = +a * rqz + wv * rxy + xv * ryw - xw * ryv + xy * rwv - xz * rqx - yv * rxw + yw * rxv + yz * rqy + zv * rqv - zw * rqw - qv * rzv + qw * rzw + qx * rxz - qy * ryz + qz * ra;
	result.q.y = +a * rqy + wv * rxz + xv * rzw - xw * rzv + xy * rqx + xz * rwv - yv * rqv + yw * rqw - yz * rqz - zv * rxw + zw * rxv + qv * ryv - qw * ryw - qx * rxy + qy * ra + qz * ryz;
	result.q.x = +a * rqx + wv * ryz + xv * rqv - xw * rqw - xy * rqy + xz * rqz + yv * rzw - yw * rzv + yz * rwv - zv * ryw + zw * ryv - qv * rxv + qw * rxw + qx * ra + qy * rxy - qz * rxz;
	return result;
}

Rotor5D& Rotor5D::operator*=(const Rotor5D& r)
{
	return *this = *this * r;
}

vec5 Rotor5D::rotate(const vec5& v) const
{
	return (Mat5)*this * v;
}

Rotor5D Rotor5D::operator-() const
{
	return Rotor5D{ a, -b, q };
}

bool Rotor5D::operator==(const Rotor5D& other) const
{
	return this->a == other.a && this->b == other.b &&
		this->q.x == other.q.x && this->q.y == other.q.y && this->q.z == other.q.z && this->q.w == other.q.w && this->q.v == other.q.v;
}

Rotor5D Rotor5D::normalized() const
{
	Rotor5D v = *this;
	v.normalize();
	return v;
}

Rotor5D& Rotor5D::normalize()
{
	float len = length2(*this);
	if (len <= glm::epsilon<float>() * glm::epsilon<float>())
	{
		return *this;
	}
	len = 1.0f / glm::sqrt(len);
	this->a *= len;
	this->b *= len;
	this->q = this->q * len;
	return *this;
}

Rotor5D::operator Mat5() const
{
	float a = this->a;
	float xy = this->b.xy, xz = this->b.xz, xw = this->b.xw, xv = this->b.xv;
	float yz = this->b.yz, yw = this->b.yw, yv = this->b.yv;
	float zw = this->b.zw, zv = this->b.zv, wv = this->b.wv;
	float qx = this->q.x, qy = this->q.y, qz = this->q.z, qw = this->q.w, qv = this->q.v;

	float m00 = +a * a + wv * wv - xv * xv - xw * xw - xy * xy - xz * xz + yv * yv + yw * yw + yz * yz + zv * zv + zw * zw - qv * qv - qw * qw + qx * qx - qy * qy - qz * qz;
	float m01 = +2.0f * a * xy + 2.0f * wv * qz - 2.0f * xv * yv - 2.0f * xw * yw - 2.0f * xz * yz + 2.0f * zv * qw + 2.0f * zw * qv - 2.0f * qx * qy;
	float m02 = +2.0f * a * xz + 2.0f * wv * qy - 2.0f * xv * zv - 2.0f * xw * zw + 2.0f * xy * yz - 2.0f * yv * qw - 2.0f * yw * qv + 2.0f * qx * qz;
	float m03 = +2.0f * a * xw - 2.0f * wv * xv + 2.0f * xy * yw + 2.0f * xz * zw - 2.0f * yv * qz + 2.0f * yz * qv - 2.0f * zv * qy - 2.0f * qw * qx;
	float m04 = +2.0f * a * xv + 2.0f * wv * xw + 2.0f * xy * yv + 2.0f * xz * zv + 2.0f * yw * qz + 2.0f * yz * qw + 2.0f * zw * qy + 2.0f * qv * qx;
	float m10 = -2.0f * a * xy - 2.0f * wv * qz - 2.0f * xv * yv - 2.0f * xw * yw - 2.0f * xz * yz - 2.0f * zv * qw - 2.0f * zw * qv - 2.0f * qx * qy;
	float m11 = +a * a + wv * wv + xv * xv + xw * xw - xy * xy + xz * xz - yv * yv - yw * yw - yz * yz + zv * zv + zw * zw - qv * qv - qw * qw - qx * qx + qy * qy - qz * qz;
	float m12 = +2.0f * a * yz + 2.0f * wv * qx + 2.0f * xv * qw + 2.0f * xw * qv - 2.0f * xy * xz - 2.0f * yv * zv - 2.0f * yw * zw - 2.0f * qy * qz;
	float m13 = +2.0f * a * yw - 2.0f * wv * yv + 2.0f * xv * qz - 2.0f * xw * xy - 2.0f * xz * qv + 2.0f * yz * zw - 2.0f * zv * qx + 2.0f * qw * qy;
	float m14 = +2.0f * a * yv + 2.0f * wv * yw - 2.0f * xv * xy - 2.0f * xw * qz - 2.0f * xz * qw + 2.0f * yz * zv + 2.0f * zw * qx - 2.0f * qv * qy;
	float m20 = -2.0f * a * xz - 2.0f * wv * qy - 2.0f * xv * zv - 2.0f * xw * zw + 2.0f * xy * yz + 2.0f * yv * qw + 2.0f * yw * qv + 2.0f * qx * qz;
	float m21 = -2.0f * a * yz - 2.0f * wv * qx - 2.0f * xv * qw - 2.0f * xw * qv - 2.0f * xy * xz - 2.0f * yv * zv - 2.0f * yw * zw - 2.0f * qy * qz;
	float m22 = +a * a + wv * wv + xv * xv + xw * xw + xy * xy - xz * xz + yv * yv + yw * yw - yz * yz - zv * zv - zw * zw - qv * qv - qw * qw - qx * qx - qy * qy + qz * qz;
	float m23 = +2.0f * a * zw - 2.0f * wv * zv + 2.0f * xv * qy - 2.0f * xw * xz + 2.0f * xy * qv + 2.0f * yv * qx - 2.0f * yw * yz - 2.0f * qw * qz;
	float m24 = +2.0f * a * zv + 2.0f * wv * zw - 2.0f * xv * xz - 2.0f * xw * qy + 2.0f * xy * qw - 2.0f * yv * yz - 2.0f * yw * qx + 2.0f * qv * qz;
	float m30 = -2.0f * a * xw - 2.0f * wv * xv + 2.0f * xy * yw + 2.0f * xz * zw + 2.0f * yv * qz - 2.0f * yz * qv + 2.0f * zv * qy - 2.0f * qw * qx;
	float m31 = -2.0f * a * yw - 2.0f * wv * yv - 2.0f * xv * qz - 2.0f * xw * xy + 2.0f * xz * qv + 2.0f * yz * zw + 2.0f * zv * qx + 2.0f * qw * qy;
	float m32 = -2.0f * a * zw - 2.0f * wv * zv - 2.0f * xv * qy - 2.0f * xw * xz - 2.0f * xy * qv - 2.0f * yv * qx - 2.0f * yw * yz - 2.0f * qw * qz;
	float m33 = +a * a - wv * wv + xv * xv - xw * xw + xy * xy + xz * xz + yv * yv - yw * yw + yz * yz + zv * zv - zw * zw - qv * qv + qw * qw - qx * qx - qy * qy - qz * qz;
	float m34 = +2.0f * a * wv - 2.0f * xv * xw + 2.0f * xy * qz + 2.0f * xz * qy - 2.0f * yv * yw + 2.0f * yz * qx - 2.0f * zv * zw - 2.0f * qv * qw;
	float m40 = -2.0f * a * xv + 2.0f * wv * xw + 2.0f * xy * yv + 2.0f * xz * zv - 2.0f * yw * qz - 2.0f * yz * qw - 2.0f * zw * qy + 2.0f * qv * qx;
	float m41 = -2.0f * a * yv + 2.0f * wv * yw - 2.0f * xv * xy + 2.0f * xw * qz + 2.0f * xz * qw + 2.0f * yz * zv - 2.0f * zw * qx - 2.0f * qv * qy;
	float m42 = -2.0f * a * zv + 2.0f * wv * zw - 2.0f * xv * xz + 2.0f * xw * qy - 2.0f * xy * qw - 2.0f * yv * yz + 2.0f * yw * qx + 2.0f * qv * qz;
	float m43 = -2.0f * a * wv - 2.0f * xv * xw - 2.0f * xy * qz - 2.0f * xz * qy - 2.0f * yv * yw - 2.0f * yz * qx - 2.0f * zv * zw - 2.0f * qv * qw;
	float m44 = +a * a - wv * wv - xv * xv + xw * xw + xy * xy + xz * xz - yv * yv + yw * yw + yz * yz - zv * zv + zw * zw + qv * qv - qw * qw - qx * qx - qy * qy - qz * qz;

	return Mat5
	{
		{ m00, m10, m20, m30, m40 },
		{ m01, m11, m21, m31, m41 },
		{ m02, m12, m22, m32, m42 },
		{ m03, m13, m23, m33, m43 },
		{ m04, m14, m24, m34, m44 },
	};
}

Rotor5D m5::normalize(const Rotor5D& v)
{
	return v.normalized();
}
float m5::length(const Rotor5D& v)
{
	return glm::sqrt(length2(v));
}
float m5::length2(const Rotor5D& v)
{
	return v.a * v.a + length2(v.b) + length2(v.q);
}
