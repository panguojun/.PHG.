/*********************************************************************
*						公共头文件
**********************************************************************/
#pragma once
#define _HAS_STD_BYTE 1

#include <stdio.h>  
#include <stdlib.h> 
#include <stdarg.h>
#include <math.h>
#include <tgmath.h>
#include <vector>
#include <algorithm>
#include <map>
#include <list>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <windows.h>
#include <thread>
#include <functional>

// **********************************************************************
// macro
// **********************************************************************
#define	IMAGESCALE		1024

#define WIN

#if defined(_MSC_VER) && defined(PMDLL)
#define EXPORT_API _declspec(dllexport)
#else
#define EXPORT_API
#endif

//#ifdef _DEBUG
#define PRINT(msg)			MYTRACE(msg)//{std::stringstream ss; ss << "\n> " << msg; printf(ss.str().c_str());}
#define PRINTV(v)			PRINT(#v << " = " << v)
#define PRINTVEC(vm, v)		PRINT(vm << " = " << v.x << "," << v.y << "," << v.z)
#define PRINTVEC2(v)		PRINT(#v << " = " << v.x << "," << v.y)
#define PRINTVEC3(v)		PRINT(#v << " = " << (v).x << "," << (v).y << "," << (v).z)
#define PRINTVEC4(v)		PRINT(#v << " = " << v.x << "," << v.y << "," << v.z << "," << v.w)
#define PRINTTRI(p1,p2,p3)	PRINT("tri" << "(" << p1.x << "," << p1.y << "," << p1.z << "," << p2.x << "," << p2.y << "," << p2.z << "," << p3.x << "," << p3.y << "," << p3.z << ")")
#define PRINTCVP(p)			PRINT("cvp(" << 50 * p.x + 500 << "," << 50 * p.y + 500 << "," << 50 * p.z + 500 << ")")

//#endif // DEBUG

extern std::stringstream logmessage;
inline void savelog(std::stringstream& log = logmessage, const std::string& logfile = "log.txt")
{
	std::ofstream	out(logfile, std::ofstream::app);
	out << log.str();
	out << std::endl;

	log.str("");
}
#define MSGBOX(msg)		{std::stringstream ss; ss << "\n" << msg; ::MessageBoxA(0,ss.str().c_str(),"MSG", 0);}
#define MYTRACE(msg)	{std::stringstream ss; ss << "\n" << msg; printf(ss.str().c_str()); ::OutputDebugStringA(ss.str().c_str());LOGMSG(msg)}
#define LOGMSG(msg)		{if(gblog) {logmessage << msg; savelog();}}
#define event(obj, msg)	{if(gblog) logmessage << "\n" << obj << " : " << msg;}
#define ASSERT(x)		{if(!(x)){std::stringstream ss; ss << "ASSERT FAILED! " << __FILE__ << "(" << __LINE__ << ")"; ::MessageBoxA(0, ss.str().c_str(), "ASSERT", 0); throw;} }
#define ERROR(x)		{if(!(x)){std::stringstream ss; ss << "\n" << msg; ::MessageBoxA(0, ss.str().c_str(), "ERROR", 0);} }
#define ERRORMSG(msg)	{std::stringstream ss; ss << "\n" << msg; ::MessageBoxA(0, ss.str().c_str(), "ERROR", 0);}

#define COND_THROW(cond, msg)  if(cond){std::stringstream ss; ss << msg; throw(std::runtime_error(ss.str()));}

#define real			float
#define crstr			const std::string&
#define uint			unsigned int
#define uchar			unsigned char
#define	EPSINON			1e-3
#define PI				3.14159265358979323846264338327950288

#define	FIMAGESCALE		((real)IMAGESCALE)
#define	UNIT			(1.0f / FIMAGESCALE)
#define DRAWSCALE		1

#define RGB(r, g, b)	(0xFF000000 | ((unsigned int)((((unsigned char)(r))) | (((unsigned int)(unsigned char)(g)) << 8) | (((unsigned int)(unsigned char)(b)) << 16))))
inline unsigned int _RGB(unsigned char r, unsigned char g, unsigned char b){return RGB(r, g, b);}

#define RGBA(r,g,b,a)   ((unsigned int)(((unsigned char)(r)) | (((unsigned int)(unsigned char)(g)) << 8) | (((unsigned int)(unsigned char)(b)) << 16) |(((unsigned int)(unsigned char)(a)) << 24)))
#define GetRValue(rgb)	((unsigned char)(0x000000ff & (rgb)))
#define GetGValue(rgb)	((unsigned char)((0x0000ff00 & (rgb))>>8))
#define GetBValue(rgb)	((unsigned char)((0x00ff0000 & (rgb))>>16))
#define GetAValue(rgb)	((unsigned char)((0xff000000 & (rgb))>>24))

#define MIN(a, b)		((a) < (b) ? (a) : (b))
inline real _MIN(real a, real b) {return ((a) < (b) ? (a) : (b));}
inline int _MINi(int a, int b) {return ((a) < (b) ? (a) : (b));}

#define MAX(a, b)		((a) > (b) ? (a) : (b))
inline real _MAX(real a, real b) {	return ((a) > (b) ? (a) : (b));}

#define vec				vector3
#define vec3			vector3
#define rvec			vector3&
#define crvec			const vector3&
#define vec2			vector2
#define rvec2			vector2&
#define crvec2			const vector2&
#define pnt				point_t

#define ivec2(ix, iy)	vector2((ix / FIMAGESCALE - 0.5f) * 2.0f, (iy / FIMAGESCALE - 0.5f) * 2.0f)
#define pixel2d(p, cor)	pixel(int((p.x / 2 + 0.5) * FIMAGESCALE), int((p.y / 2 + 0.5) * FIMAGESCALE), cor)
#define vec2i(p)		int((p.x / 2 + 0.5) * FIMAGESCALE), int((p.y / 2 + 0.5) * FIMAGESCALE)

#define mix				lerp

#define LOOP0(i, szvar, szval) const int szvar = szval; for(int i = 0; i <= szvar; i ++){real a##i = i / real(szvar);	
#define LOOP(i, szvar, szval) const int szvar = szval; for(int i = 0; i < szvar; i ++){real a##i = i / real(szvar);	
#define FOR(i, szvar, szval) const int szvar = szval; for(int i = 0; i < szvar; i ++){real a##i = i / real(szvar);	
#define END	}
#define RNDBW	blendcor(1, 0xFF808080, rrnd(0.25, 1))
#define RNDCOR	RGB(rrnd(0, 255), rrnd(0, 255), rrnd(0, 255))
#define VECLIST	std::vector<vertex>
#define ADDE(__e, __v)	__e.push_back(__v)

#define E_	0.61828183
#define E1	(E_)
#define E2	(E1 * E_)
#define E3	(E2 * E_)
#define E4	(E3 * E_)
#define E5	(E4 * E_)
#define E6	(E5 * E_)
#define E7	(E6 * E_)
#define E8	(E7 * E_)
#define E9	(E8 * E_)

#define PUSH	push_back
#define	CIRCLE(d_i, r, n, ang, ang0)	LOOP(d_i, len##d_i, n)real ang##d_i = a##d_i * ang + ang0;	vec2 dp##d_i((r) * cos(ang##d_i), (r) * sin(ang##d_i));
#define	CIRCLE2(d_i, r, n, ang0)	LOOP(d_i, len##d_i, n)real ang##d_i = a##d_i * PI * 2 + ang0;	vec2 dp##d_i((r) * cos(ang##d_i), (r) * sin(ang##d_i));
#define	CIRCLE3(d_i, r, n, ang0)	LOOP0(d_i, len##d_i, n)real ang##d_i = a##d_i * PI * 2 + ang0;	vec2 dp##d_i((r) * cos(ang##d_i), (r) * sin(ang##d_i));
#define	CIRCLE4(d_i, r, n, ang, ang0)	LOOP0(d_i, len##d_i, n)real ang##d_i = a##d_i * ang + ang0;	vec2 dp##d_i((r) * cos(ang##d_i), (r) * sin(ang##d_i));

#define BEGINDRAW renderstate = 1;
#define ENDDRAW renderstate = 2;
#define DEF_SCALES(s) 	real s1 = s  * E_ * E_;real s2 = s1 * E_ * E_;real s3 = s2 * E_ * E_;real s4 = s3 * E_ * E_;real s5 = s4 * E_ * E_;real s6 = s5 * E_ * E_;real s7 = s6 * E_ * E_;real s8 = s7 * E_ * E_;
#define DEF_SCALES2(s) 	real s1 = s  * E_;real s2 = s1 * E_;real s3 = s2 * E_;real s4 = s3 * E_;real s5 = s4 * E_;real s6 = s5 * E_;real s7 = s6 * E_;real s8 = s7 * E_;
#define START_MESH	resetsm();color = 0xFFFFFFFF;
#define END_MESH //exportsm();
#define	VEC2STR(v)	(v).x << ", " << (v).y << ", " << (v).z

// ----------------------------------------------------------------------
#define d100				(rand() % 100 + 1)
#define d6					(rand() % 6 + 1)
#define dx(x)				(rand() % x + 1)					

// ----------------------------------------------------------------------
#define ENT(ind)			entlist[ind % entlist.size()]
#define HITTEST(p1, p2, r)	(((p1) - (p2)).sqrlen() < (r) * (r))

// ----------------------------------------------------------------------
#define LUA_FUN_BEGIN(fun)	static int fun(lua_State* L){ PRINT("call:" << #fun);int n = lua_gettop(L);for (int i = 0; i < n; i++) {luaparam[i] = lua_tonumber(L, 1 + i);PRINT("luaparam" << i << ":" << luaparam[i])}
#define LUA_FUN_BEGIN_NOPPRAM(fun)	static int fun(lua_State* L){ PRINT(#fun);
#define LUA_FUN_END	return 0;}
#define LUA_FUN_END_RET(ret)	lua_pushnumber(L, ret); return 1;}
#define LUA_REGIST(fun)	lua_register(L, #fun, fun);

// ----------------------------------------------------------------------
inline real rrnd(real min = 0.0, real max = 1.0) { return (max) == (min) ? (min) : (min)+((real)(rand()) / (RAND_MAX + 1.0)) * ((max)-(min)); }
inline int rndi(int min, int max) { return min == max ? min : rand() % (max - min + 1) + min; }
inline int RGB2(int r, int g, int b) { return RGB(r, g, b); }
inline int RGBA2(int r, int g, int b, int a) { return RGBA(r, g, b, a); }

// ----------------------------------------------------------------------
//#define JSONMAP_BEGIN	void loadjson(const std::string& filename){PRINT("loadjson:" << filename);std::string str;Json json(&std::ifstream(filename));json.writeToStr(str);PRINT(str);JsonDict* jd = (JsonDict*)json.getRoot();
//#define READ_JSON_NUM(key)	{auto_t au;au.floatdat = ((JsonList*)jd->getValue(#key))->toNumber();keyvalmap[#key]=au;}
//#define JS_FLOAT(key)	(keyvalmap[#key].floatdat)
//#define JS_INT(key)	(keyvalmap[#key].intdat)
//namespace JSONAUTO
//{
//	struct auto_t {
//		union {
//			int intdat;
//			float floatdat;
//		};
//		std::string strdat;
//	};
//	std::map<std::string, auto_t> keyvalmap;
//	JSONMAP_BEGIN
//		READ_JSON_NUM(key1);
//	READ_JSON_NUM(key2);
//	END
//		void test()
//	{
//		PRINT("JSVAL=" << JS_FLOAT(key2));
//	}
//}

// ----------------------------------------------------------------------
real SCREENWID, SCREENHGT;
real MAXZ = 100.0;
real EXPORT_SCALE = 1.0;
real USIZE0 = 0.01f;
real USIZE = USIZE0 / EXPORT_SCALE;
#define MINSTEP  USIZE / rrnd(20.0f, 50.0f)
bool gblog = true;
std::stringstream logmessage;
real frametime = 0;

int renderstate = 0;		// 渲染模式: 0 -3D构型， 1 -正常渲染 2 -网格渲染, >=3 -2d绘图，-1 not rendering 

// **********************************************************************
// 2D
// **********************************************************************
struct vector2 {
	union {
		real val[2];
		struct {
			real x;
			real y;
		};
	};

	static const vector2 ZERO;
	static const vector2 UX;
	static const vector2 UY;
	static const vector2 CENTER;

	real& operator [](int ind) {
		return val[ind];
	}

	vector2() {
		x = 0;
		y = 0;
	}
	vector2(real v)
	{
		x = v;
		y = v;
	}
	vector2(real _x, real _y) {
		x = _x;
		y = _y;
	}
	vector2 operator + (const vector2& _p) const
	{
		vector2 fp;
		fp.x = x + _p.x;
		fp.y = y + _p.y;

		return fp;
	}
	void operator += (const vector2& _p)
	{
		x += _p.x;
		y += _p.y;
	}
	vector2 operator - (const vector2& _p) const
	{
		vector2 fp;
		fp.x = x - _p.x;
		fp.y = y - _p.y;
		return fp;
	}
	vector2 operator - () const
	{
		vector2 fp;
		fp.x = -x;
		fp.y = -y;
		return fp;
	}

	vector2 operator * (real s) const
	{
		vector2 fp;
		fp.x = s * x;
		fp.y = s * y;
		return fp;
	}
	vector2 operator * (const vector2& b) const
	{
		return vector2(x * b.x - y * b.y, x * b.y + y * b.x);
	}
	vector2 operator / (real s) const
	{
		vector2 fp;
		fp.x = x / s;
		fp.y = y / s;
		return fp;
	}
	bool operator == (const vector2& rv) const
	{
		return (fabs(x - rv.x) <= 1e-5 && fabs(y - rv.y) <= 1e-5);
	}
	bool operator != (const vector2& rv) const
	{
		return (fabs(x - rv.x) > 1e-5 || fabs(y - rv.y) > 1e-5);
	}
	real len() const
	{
		return sqrt(x * x + y * y);
	}
	real sqrlen() const
	{
		return (x * x + y * y);
	}
	real angle() const
	{
		return atan2(y, x);
	}
	void norm()
	{
		real r = len();
		if (r > 0)
		{
			x /= r;
			y /= r;
		}
	}
	vector2 normcopy() const
	{
		real r = len();
		if (r > 0)
		{
			return vector2(x / r, y / r);
		}
		return vector2::ZERO;
	}
	void rot(real angle)
	{
		(*this) = (*this) * vector2::fromanglelength(angle, 1);
	}
	vector2 rotcopy(real angle) const
	{
		return (*this) * vector2::fromanglelength(angle, 1);
	}
	void rot(real angle, const vector2& o)
	{
		vector2 v = (*this) - o;
		v = v * vector2::fromanglelength(angle, 1);
		(*this) = v + o;
	}
	vector2 rotcopy(real angle, const vector2& o) const
	{
		vector2 v = (*this) - o;
		v = v * vector2::fromanglelength(angle, 1);
		return v + o;
	}
	vector2 rotcpy(real angle, const vector2& o) const
	{
		vector2 v = (*this) - o;
		v = v * vector2::fromanglelength(angle, 1);
		return v + o;
	}
	real dot(const vector2& v) const
	{
		return x * v.x + y * v.y;
	}
	real cross(const vector2& v) const
	{
		return x * v.y - y * v.x;
	}
	static vector2 fromanglelength(real _angle, real _r);
};

const vector2 vector2::ZERO = vector2(0, 0);
const vector2 vector2::UX = vector2(1, 0);
const vector2 vector2::UY = vector2(0, 1);
const vector2 vector2::CENTER = vector2(0.5, 0.5);

vector2 vector2::fromanglelength(real _angle, real _r)
{
	return vector2(_r * cos(_angle), _r * sin(_angle));
}

// **********************************************************************
// 3D
// **********************************************************************
struct vector3
{
	static const vector3 ZERO;
	static const vector3 ONE;
	static const vector3 UX;
	static const vector3 UY;
	static const vector3 UZ;
	static const vector3 CENTER;
	static real sEPSINON;
	union {
		real val[3];
		struct
		{
			real x;
			real y;
			real z;
		};
		struct
		{
			real r;
			real g;
			real b;
		};
	};
	vector3()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	vector3(real v)
	{
		x = v;
		y = v;
		z = v;
	}
	vector3(real _x, real _y, real _z = 0.0f)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	vector3(const vector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
	real& operator [](int ind) {
		return val[ind];
	}
	vector2 xy() const
	{
		vector2 fp;
		fp.x = x;
		fp.y = y;
		return fp;
	}
	void xy(const vec2& _xy)
	{
		x = _xy.x;
		y = _xy.y;
	}
	vector2 xz() const
	{
		vector2 fp;
		fp.x = x;
		fp.y = z;
		return fp;
	}
	void xz(const vec2& _xz)
	{
		x = _xz.x;
		z = _xz.y;
	}
	vector2 yz() const
	{
		vector2 fp;
		fp.x = y;
		fp.y = z;
		return fp;
	}
	vector3 zxy() const
	{
		vector3 fp;
		fp.x = z;
		fp.y = x;
		fp.z = y;
		return fp;
	}
	vector3 operator + (const vector3& _p) const
	{
		vector3 fp;
		fp.x = x + _p.x;
		fp.y = y + _p.y;
		fp.z = z + _p.z;
		return fp;
	}
	void operator += (const vector3& _p)
	{
		x += _p.x;
		y += _p.y;
		z += _p.z;
	}
	vector3 operator - (const vector3& _p) const
	{
		vector3 fp;
		fp.x = x - _p.x;
		fp.y = y - _p.y;
		fp.z = z - _p.z;
		return fp;
	}
	void operator -= (const vector3& _p)
	{
		x -= _p.x;
		y -= _p.y;
		z -= _p.z;
	}
	vector3 operator - () const
	{
		vector3 fp;
		fp.x = -x;
		fp.y = -y;
		fp.z = -z;
		return fp;
	}
	vector3 operator * (real s) const
	{
		vector3 fp;
		fp.x = s * x;
		fp.y = s * y;
		fp.z = s * z;
		return fp;
	}
	vector3 operator * (const vector3& v) const
	{
		vector3 fp;
		fp.x = v.x * x;
		fp.y = v.y * y;
		fp.z = v.z * z;
		return fp;
	}
	void operator *= (real s)
	{
		x = s * x;
		y = s * y;
		z = s * z;
	}
	vector3 operator / (real s) const
	{
		vector3 fp;
		fp.x = x / s;
		fp.y = y / s;
		fp.z = z / s;
		return fp;
	}
	void operator /= (real s)
	{
		x = x / s;
		y = y / s;
		z = z / s;
	}
	bool operator == (const vector3& rv) const
	{
		return (fabs(x - rv.x) <= sEPSINON && fabs(y - rv.y) <= sEPSINON && fabs(z - rv.z) <= sEPSINON);
	}
	bool operator != (const vector3& rv) const
	{
		return (fabs(x - rv.x) > sEPSINON || fabs(y - rv.y) > sEPSINON || fabs(z - rv.z) > sEPSINON);
	}
	bool operator < (const vector3& rv) const
	{
		if (x < rv.x && y < rv.y && z < rv.z)
			return true;
		return false;
	}
	bool operator > (const vector3& rv) const
	{
		if (x > rv.x && y > rv.y && z > rv.z)
			return true;
		return false;
	}
	real len() const
	{
		return sqrt(x * x + y * y + z * z);
	}
	real lenxy() const
	{
		return sqrt(x * x + y * y);
	}
	real sqrlenxy() const
	{
		return (x * x + y * y);
	}
	real lenxz() const
	{
		return sqrt(x * x + z * z);
	}
	real sqrlen() const
	{
		return (x * x + y * y + z * z);
	}
	void norm()
	{
		real r = len();
		if (r > 0)
		{
			x /= r;
			y /= r;
			z /= r;
		}
	}
	vector3 normcopy() const
	{
		real r = len();
		if (r > 0)
		{
			return vector3(this->x / r,
				this->y / r,
				this->z / r);
		}
		return vector3::ZERO;
	}
	void fromanglelength(real _angle, real _r)
	{
		x = _r * cos(_angle);
		y = _r * sin(_angle);
	}
	real angle() const
	{
		return atan2(y, x);
	}
	real dot(const vector3& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	vector3 cross(const vector3& v) const
	{
		vector3 n;
		n.x = -(y * v.z - z * v.y);
		n.y = -(z * v.x - x * v.z);
		n.z = -(x * v.y - y * v.x);
		return n;
	}
	void rot(real angle, const vector3& ax);
	vector3 rotcopy(real angle, const vector3& ax) const;
	vector3 rotcpy(real angle, const vector3& ax) const;
	static vector3 rnd(real min = 0, real max = 1);

	static vector3 rndrad(real r = 1);
};

const vector3 vector3::ZERO = vector3(0, 0, 0);
const vector3 vector3::ONE = vector3(1, 1, 1);
const vector3 vector3::UX = vector3(1, 0, 0);
const vector3 vector3::UY = vector3(0, 1, 0);
const vector3 vector3::UZ = vector3(0, 0, 1);
const vector3 vector3::CENTER = vector3(0.0, 0.0, MAXZ / 2);
real vector3::sEPSINON = EPSINON;
// ----------------------------------------
vector3 vector3::rnd(real min, real max)
{
	return vector3(rrnd(min, max), rrnd(min, max), rrnd(min, max));
}
// ----------------------------------------
vector3 vector3::rndrad(real r)
{
	return rnd(-1, 1).normcopy() * r;
}
// **********************************************************************
// 4D
// **********************************************************************
struct vector4
{
	static const vector4 ZERO;
	static const vector4 UX;
	static const vector4 UY;
	static const vector4 UZ;
	static const vector4 UW;
	static const vector4 CENTER;
	union {
		real val[4];
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
	};
	vector4()
	{
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}
	vector4(float _x, float _y, float _z, float _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
	real& operator [](int ind) {
		return val[ind];
	}
	vector4 operator + (const vector4& _p) const
	{
		vector4 fp;
		fp.x = x + _p.x;
		fp.y = y + _p.y;
		fp.z = z + _p.z;
		fp.w = w + _p.w;
		return fp;
	}
	void operator += (const vector4& _p)
	{
		x += _p.x;
		y += _p.y;
		z += _p.z;
		w += _p.w;
	}
	vector4 operator - (const vector4& _p) const
	{
		vector4 fp;
		fp.x = x - _p.x;
		fp.y = y - _p.y;
		fp.z = z - _p.z;
		fp.w = w - _p.w;
		return fp;
	}
	void operator -= (const vector4& _p)
	{
		x -= _p.x;
		y -= _p.y;
		z -= _p.z;
		w -= _p.w;
	}
	vector4 operator - () const
	{
		vector4 fp;
		fp.x = -x;
		fp.y = -y;
		fp.z = -z;
		fp.w = -w;
		return fp;
	}
	vector4 operator * (float s) const
	{
		vector4 fp;
		fp.x = s * x;
		fp.y = s * y;
		fp.z = s * z;
		fp.w = s * w;
		return fp;
	}
	void operator *= (float s)
	{
		x = s * x;
		y = s * y;
		z = s * z;
		w = s * w;
	}
	vector4 operator / (float s) const
	{
		vector4 fp;
		fp.x = x / s;
		fp.y = y / s;
		fp.z = z / s;
		fp.w = w / s;
		return fp;
	}
	void operator /= (float s)
	{
		x = x / s;
		y = y / s;
		z = z / s;
		w = w / s;
	}
	bool operator == (const vector4& rv) const
	{
		return (fabs(x - rv.x) <= EPSINON && fabs(y - rv.y) <= EPSINON && fabs(z - rv.z) <= EPSINON && fabs(w - rv.w) <= EPSINON);
	}
	bool operator != (const vector4& rv) const
	{
		return (fabs(x - rv.x) > EPSINON || fabs(y - rv.y) > EPSINON || fabs(z - rv.z) > EPSINON || fabs(w - rv.w) > EPSINON);
	}
	float len() const
	{
		return sqrt(x * x + y * y + z * z + w * w);
	}
	float sqrlen() const
	{
		return (x * x + y * y + z * z + w * w);
	}
	real norm()
	{
		float r = len();
		if (r > 1e-5)
		{
			x /= r;
			y /= r;
			z /= r;
			w /= r;
		}
		else
		{
			ERRORMSG("r == 0");
		}
		return r;
	}
	vector4 normcopy()
	{
		float r = len();
		if (r > 0)
		{
			return vector4(
				this->x / r,
				this->y / r,
				this->z / r,
				this->w / r
			);
		}
		else
		{
			ERRORMSG("r == 0");
		}
		return vector4(0, 0, 0, 0);
	}
	float dot(const vector4& v) const
	{
		return x * v.x + y * v.y + z * v.z + w * v.w;
	}

	// Cross4 computes the four-dimensional cross product of the three vectors
	// U, V and W, in that order. It returns the resulting four-vector.

	vector4 cross(const vector4& V, const vector4& W) const
	{
		vector4 result;
		double A, B, C, D, E, F;       // Intermediate Values

		// Calculate intermediate values.

		A = (V.x * W.y) - (V.y * W.x);
		B = (V.x * W.z) - (V.z * W.x);
		C = (V.x * W.w) - (V.w * W.x);
		D = (V.y * W.z) - (V.z * W.y);
		E = (V.y * W.w) - (V.w * W.y);
		F = (V.z * W.w) - (V.w * W.z);

		// Calculate the result-vector components.

		result.x = (y * F) - (z * E) + (w * D);
		result.y = -(x * F) + (z * C) - (w * B);
		result.z = (x * E) - (y * C) + (w * A);
		result.w = -(x * D) + (y * B) - (z * A);

		return result;
	}

};
const vector4 vector4::ZERO = vector4(0, 0, 0, 0);
const vector4 vector4::UX = vector4(1, 0, 0, 0);
const vector4 vector4::UY = vector4(0, 1, 0, 0);
const vector4 vector4::UZ = vector4(0, 0, 1, 0);
const vector4 vector4::UW = vector4(0, 0, 0, 1);
const vector4 vector4::CENTER = vector4(0.5, 0.5, 0.5, 0.5);
// **********************************************************************
// 四元数 
// **********************************************************************
struct  quaternion
{
	real w, x, y, z;

	quaternion(
		real fW = 1.0,
		real fX = 0.0, real fY = 0.0, real fZ = 0.0)
	{
		w = fW;
		x = fX;
		y = fY;
		z = fZ;
	}
	quaternion(const quaternion& rkQ)
	{
		w = rkQ.w;
		x = rkQ.x;
		y = rkQ.y;
		z = rkQ.z;
	}
	quaternion(const real& rfAngle, const vector3& rkAxis)
	{
		this->fromangleaxis(rfAngle, rkAxis);
	}
	//-----------------------------------------------------------------------
	quaternion operator+ (const quaternion& rkQ) const
	{
		return quaternion(w + rkQ.w, x + rkQ.x, y + rkQ.y, z + rkQ.z);
	}
	//-----------------------------------------------------------------------
	quaternion operator- (const quaternion& rkQ) const
	{
		return quaternion(w - rkQ.w, x - rkQ.x, y - rkQ.y, z - rkQ.z);
	}
	//-----------------------------------------------------------------------
	vector3 operator* (const vector3& v) const
	{
		// nVidia SDK implementation
		vector3 uv, uuv;
		vector3 qvec(x, y, z);
		uv = qvec.cross(v);
		uuv = qvec.cross(uv);
		uv = uv * (2.0f * w);
		uuv = uuv * 2.0f;

		return v + uv + uuv;
	}
	//-----------------------------------------------------------------------
	quaternion operator* (const quaternion& rkQ) const
	{
		// NOTE:  Multiplication is not generally commutative, so in most
		// cases p*q != q*p.

		return quaternion
		(
			w * rkQ.w - x * rkQ.x - y * rkQ.y - z * rkQ.z,
			w * rkQ.x + x * rkQ.w + y * rkQ.z - z * rkQ.y,
			w * rkQ.y + y * rkQ.w + z * rkQ.x - x * rkQ.z,
			w * rkQ.z + z * rkQ.w + x * rkQ.y - y * rkQ.x
		);
	}
	//-----------------------------------------------------------------------
	quaternion operator* (real fScalar) const
	{
		return quaternion(fScalar * w, fScalar * x, fScalar * y, fScalar * z);
	}
	//-----------------------------------------------------------------------
	quaternion friend operator* (real fScalar, const quaternion& rkQ)
	{
		return quaternion(fScalar * rkQ.w, fScalar * rkQ.x, fScalar * rkQ.y,
			fScalar * rkQ.z);
	}
	//-----------------------------------------------------------------------
	bool operator ==(const quaternion& rkQ) const
	{
		return w == rkQ.w && x == rkQ.x && y == rkQ.y && z == rkQ.z;
	}
	//-----------------------------------------------------------------------
	quaternion operator - () const
	{
		quaternion q;
		q.x = -x;
		q.y = -y;
		q.z = -z;
		q.w = -w;
		return q;
	}
	//-----------------------------------------------------------------------
	real dot(const quaternion& rkQ) const
	{
		return w * rkQ.w + x * rkQ.x + y * rkQ.y + z * rkQ.z;
	}
	//-----------------------------------------------------------------------
	real norm() const
	{
		return w * w + x * x + y * y + z * z;
	}
	//-----------------------------------------------------------------------
	real normalise(void)
	{
		real len = norm();
		real factor = 1.0f / sqrt(len);
		*this = *this * factor;
		return len;
	}
	//-----------------------------------------------------------------------
	void fromangleaxis(real rfAngle,
		const vector3& rkAxis)
	{
		// assert:  axis[] is unit length
		//
		// The quaternion representing the rotation is
		//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

		real fHalfAngle(0.5 * rfAngle);
		real fSin = sin(fHalfAngle);
		w = cos(fHalfAngle);
		x = fSin * rkAxis.x;
		y = fSin * rkAxis.y;
		z = fSin * rkAxis.z;
	}
	//-----------------------------------------------------------------------
	void fromeuler(real pitch, real yaw, real roll)
	{
		real t0 = cos(yaw * 0.5);
		real t1 = sin(yaw * 0.5);
		real t2 = cos(roll * 0.5);
		real t3 = sin(roll * 0.5);
		real t4 = cos(pitch * 0.5);
		real t5 = sin(pitch * 0.5);

		w = t2 * t4 * t0 + t3 * t5 * t1;
		x = t3 * t4 * t0 - t2 * t5 * t1;
		y = t2 * t5 * t0 + t3 * t4 * t1;
		z = t2 * t4 * t1 - t3 * t5 * t0;
	}
	//-----------------------------------------------------------------------
	vec3 xyz() const
	{
		return vec3(x, y, z);
	}
	//-----------------------------------------------------------------------
	vec3 toeuler()
	{
		vec3 v;

		real epsilon = 0.00001;
		real halfpi = 0.5 * PI;

		real temp = 2 * (y * z - x * w);
		if (temp >= 1 - epsilon)
		{
			v.x = halfpi;
			v.y = -atan2(y, w);
			v.z = -atan2(z, w);
		}
		else if (-temp >= 1 - epsilon)
		{
			v.x = -halfpi;
			v.y = -atan2(y, w);
			v.z = -atan2(z, w);
		}
		else
		{
			v.x = asin(temp);
			v.y = -atan2(x * z + y * w, 0.5 - x * x - y * y);
			v.z = -atan2(x * y + z * w, 0.5 - x * x - z * z);
		}

		return v;
	}
	quaternion slerp(const quaternion& qa, const quaternion& qb, double t) {
		// quaternion to return
		quaternion qm;
		// Calculate angle between them.
		double cosHalfTheta = qa.w * qb.w + qa.x * qb.x + qa.y * qb.y + qa.z * qb.z;
		// if qa=qb or qa=-qb then theta = 0 and we can return qa
		if (abs(cosHalfTheta) >= 1.0) {
			qm.w = qa.w; qm.x = qa.x; qm.y = qa.y; qm.z = qa.z;
			return qm;
		}
		// Calculate temporary values.
		double halfTheta = acos(cosHalfTheta);
		double sinHalfTheta = sqrt(1.0 - cosHalfTheta * cosHalfTheta);
		// if theta = 180 degrees then result is not fully defined
		// we could rotate around any axis normal to qa or qb
		if (fabs(sinHalfTheta) < 0.001) { // fabs is floating point absolute
			qm.w = (qa.w * 0.5 + qb.w * 0.5);
			qm.x = (qa.x * 0.5 + qb.x * 0.5);
			qm.y = (qa.y * 0.5 + qb.y * 0.5);
			qm.z = (qa.z * 0.5 + qb.z * 0.5);
			return qm;
		}
		double ratioA = sin((1 - t) * halfTheta) / sinHalfTheta;
		double ratioB = sin(t * halfTheta) / sinHalfTheta;
		//calculate Quaternion.
		qm.w = (qa.w * ratioA + qb.w * ratioB);
		qm.x = (qa.x * ratioA + qb.x * ratioB);
		qm.y = (qa.y * ratioA + qb.y * ratioB);
		qm.z = (qa.z * ratioA + qb.z * ratioB);
		return qm;
	}
	quaternion slerp(real fT, const quaternion& rkP,
		const quaternion& rkQ, bool shortestPath)
	{
		const real msEpsilon = 1e-03;
		real fCos = rkP.dot(rkQ);
		quaternion rkT;

		// Do we need to invert rotation?
		if (fCos < 0.0f && shortestPath)
		{
			fCos = -fCos;
			rkT = -rkQ;
		}
		else
		{
			rkT = rkQ;
		}

		if (fabs(fCos) < 1 - msEpsilon)
		{
			// Standard case (slerp)
			real fSin = sqrt(1 - (fCos * fCos));
			real fAngle = atan2(fSin, fCos);
			real fInvSin = 1.0f / fSin;
			real fCoeff0 = sin((1.0f - fT) * fAngle) * fInvSin;
			real fCoeff1 = sin(fT * fAngle) * fInvSin;
			return fCoeff0 * rkP + fCoeff1 * rkT;
		}
		else
		{
			// There are two situations:
			// 1. "rkP" and "rkQ" are very close (fCos ~= +1), so we can do a linear
			//    interpolation safely.
			// 2. "rkP" and "rkQ" are almost inverse of each other (fCos ~= -1), there
			//    are an infinite number of possibilities interpolation. but we haven't
			//    have method to fix this case, so just use linear interpolation here.
			quaternion t = (1.0f - fT) * rkP + fT * rkT;
			// taking the complement requires renormalisation
			t.normalise();
			return t;
		}
	}
	//-----------------------------------------------------------------------
	quaternion nlerp(real fT, const quaternion& rkP,
		const quaternion& rkQ, bool shortestPath)
	{
		quaternion result;
		real fCos = rkP.dot(rkQ);
		if (fCos < 0.0f && shortestPath)
		{
			result = rkP + fT * ((-rkQ) - rkP);
		}
		else
		{
			result = rkP + fT * (rkQ - rkP);
		}
		result.normalise();
		return result;
	}
};
// ----------------------------------------
void vector3::rot(real angle, const vector3& ax = vec::UZ)
{
	quaternion q(angle, ax);
	*this = q * (*this);
}
// ----------------------------------------
vector3 vector3::rotcopy(real angle, const vector3& ax = vec::UZ) const
{
	quaternion q(angle, ax);
	return q * (*this);
}

vector3 vector3::rotcpy(real angle, const vector3& ax = vec::UZ) const
{
	quaternion q(angle, ax);
	return q * (*this);
}

// **********************************************************************
// ray3d
// **********************************************************************
struct ray3
{
	vector3 o;
	vector3 dir;
	ray3() {}
	ray3(const vector3& p1, const vector3& p2)
	{
		o = p1;
		dir = (p2 - p1).normcopy();
	}

};
// **********************************************************************
// segment
// **********************************************************************
struct segment
{
	vector3 s;
	vector3 e;
	segment(const vector3& _s, const vector3& _e)
	{
		s = _s;
		e = _e;
	}
};
// **********************************************************************
// plane
// **********************************************************************
struct plane_t
{
	vector3 o;
	vector3 n;
	plane_t(const vector3& _o, const vector3& _n)
	{
		o = _o;
		n = _n;
		n.norm();
	}
	plane_t(const vector3& p1, const vector3& p2, const vector3& p3)
	{
		n = (p2 - p1).cross(p3 - p1);
		n.norm();
		o = p1;
	}
};
// **********************************************************************
// 2D point
// **********************************************************************
struct point_t
{
	int x = 0, y = 0;
	point_t()
	{
		x = 0;
		y = 0;
	}
	point_t(int ix, int iy)
	{
		x = ix; y = iy;
	}
	int len()
	{
		return sqrt((x) * (x)+(y) * (y));
	}
	vec2 tovec() const
	{
		return vec2(x, y);
	}
	bool operator == (const point_t& rp) const
	{
		return (x == rp.x && y == rp.y);
	}
	bool operator != (const point_t& rp) const
	{
		return (x != rp.x || y != rp.y);
	}
	point_t operator + (const point_t& rp) const
	{
		point_t p;
		p.x = x + rp.x;
		p.y = y + rp.y;
		return  p;
	}
	point_t operator - (const point_t& rp) const
	{
		point_t p;
		p.x = x - rp.x;
		p.y = y - rp.y;
		return  p;
	}
	point_t operator / (real factor) const
	{
		point_t p;
		p.x = x / factor;
		p.y = y / factor;
		return  p;
	}
	static real dis(const point_t& p1, const point_t& p2)
	{
		return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
	}
	static real Mdis(const point_t& p1, const point_t& p2)
	{
		return abs(p1.x - p2.x) + abs(p1.y - p2.y);
	}
	static real dis(int x1, int y1, int x2, int y2)
	{
		return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	}
	static point_t fromvec(const vec2& v)
	{
		point_t p;
		p.x = v.x;
		p.y = v.y;
		return p;
	}
};
// **********************************************************************
// 2D rect
// **********************************************************************
struct rect_t
{
	int x = 0, y = 0, width = 0, height = 0;
};
// **********************************************************************
// 3D point
// **********************************************************************
struct point3_t
{
	int x = 0, y = 0, z = 0;
	point3_t()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	point3_t(int ix, int iy, int iz)
	{
		x = ix; y = iy; z = iz;
	}
	int llen() const
	{
		return ((x)*(x) + (y)*(y) + (z)*(z));
	}
	
	bool operator == (const point3_t& rp) const
	{
		return (x == rp.x && y == rp.y && z == rp.z);
	}
	point3_t operator + (const point3_t& rp) const
	{
		point3_t p;
		p.x = x + rp.x;
		p.y = y + rp.y;
		p.z = z + rp.z;
		return  p;
	}
	point3_t operator - (const point3_t& rp) const
	{
		point3_t p;
		p.x = x - rp.x;
		p.y = y - rp.y;
		p.z = z - rp.z;
		return  p;
	}
	point3_t operator / (real factor) const
	{
		point3_t p;
		p.x = x / factor;
		p.y = y / factor;
		p.z = z / factor;
		return  p;
	}
	vec3 tovec() const
	{
		return vec3(x, y, z);
	}
};

// **********************************************************************
// boundingbox
// **********************************************************************
struct boundingbox
{
	vector3	a;
	vector3 b;
	boundingbox() {}
	boundingbox(const vector3& pt)
	{
		a = pt;
		b = pt;
	}
	boundingbox(const vector3& _min, const vector3& _max)
	{
		a = _min;
		b = _max;
	}
	inline bool intersection(const vector3& pt, real tolerance = 0) const
	{
		return
			pt.x >= a.x - tolerance && pt.x < b.x + tolerance &&
			pt.y >= a.y - tolerance && pt.y < b.y + tolerance &&
			pt.z >= a.z - tolerance && pt.z < b.z + tolerance;
	}
	std::pair<bool, real> intersection(const ray3& ray) const
	{
		const boundingbox& box = *this;
		real lowt = 0.0f;
		real t;
		bool hit = false;
		vector3 hitpoint;
		const vector3& min = box.a;
		const vector3& max = box.b;
		const vector3& rayorig = ray.o;
		const vector3& raydir = ray.dir;

		// Check origin inside first
		if (rayorig > min && rayorig < max)
		{
			return std::pair<bool, real>(true, 0.);
		}

		// Check each face in turn, only check closest 3
		// Min x
		if (rayorig.x < min.x && raydir.x > 0)
		{
			t = (min.x - rayorig.x) / raydir.x;
			if (t > 0)
			{
				// Substitute t back into ray and check bounds and dist
				hitpoint = rayorig + raydir * t;
				if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
					hitpoint.z >= min.z && hitpoint.z <= max.z &&
					(!hit || t < lowt))
				{
					hit = true;
					lowt = t;
				}
			}
		}
		// Max x
		if (rayorig.x > max.x && raydir.x < 0)
		{
			t = (max.x - rayorig.x) / raydir.x;
			if (t > 0)
			{
				// Substitute t back into ray and check bounds and dist
				hitpoint = rayorig + raydir * t;
				if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
					hitpoint.z >= min.z && hitpoint.z <= max.z &&
					(!hit || t < lowt))
				{
					hit = true;
					lowt = t;
				}
			}
		}
		// Min y
		if (rayorig.y < min.y && raydir.y > 0)
		{
			t = (min.y - rayorig.y) / raydir.y;
			if (t > 0)
			{
				// Substitute t back into ray and check bounds and dist
				hitpoint = rayorig + raydir * t;
				if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
					hitpoint.z >= min.z && hitpoint.z <= max.z &&
					(!hit || t < lowt))
				{
					hit = true;
					lowt = t;
				}
			}
		}
		// Max y
		if (rayorig.y > max.y && raydir.y < 0)
		{
			t = (max.y - rayorig.y) / raydir.y;
			if (t > 0)
			{
				// Substitute t back into ray and check bounds and dist
				hitpoint = rayorig + raydir * t;
				if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
					hitpoint.z >= min.z && hitpoint.z <= max.z &&
					(!hit || t < lowt))
				{
					hit = true;
					lowt = t;
				}
			}
		}
		// Min z
		if (rayorig.z < min.z && raydir.z > 0)
		{
			t = (min.z - rayorig.z) / raydir.z;
			if (t > 0)
			{
				// Substitute t back into ray and check bounds and dist
				hitpoint = rayorig + raydir * t;
				if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
					hitpoint.y >= min.y && hitpoint.y <= max.y &&
					(!hit || t < lowt))
				{
					hit = true;
					lowt = t;
				}
			}
		}
		// Max z
		if (rayorig.z > max.z && raydir.z < 0)
		{
			t = (max.z - rayorig.z) / raydir.z;
			if (t > 0)
			{
				// Substitute t back into ray and check bounds and dist
				hitpoint = rayorig + raydir * t;
				if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
					hitpoint.y >= min.y && hitpoint.y <= max.y &&
					(!hit || t < lowt))
				{
					hit = true;
					lowt = t;
				}
			}
		}

		return std::pair<bool, real>(hit, lowt);
	}
	/** Returns whether or not this box intersects another. */
	inline bool intersection(const boundingbox& b2) const
	{
		// Use up to 6 separating planes
		if (b.x < b2.a.x)
			return false;
		if (b.y < b2.a.y)
			return false;
		if (b.z < b2.a.z)
			return false;

		if (a.x > b2.b.x)
			return false;
		if (a.y > b2.b.y)
			return false;
		if (a.z > b2.b.z)
			return false;

		// otherwise, must be intersecting
		return true;

	}
	void centerzero(const vector3& pt)
	{
		a = pt;
		b = pt;
	}
	void resize(const vector3& pt)
	{
		pt.x < a.x ? a.x = pt.x : 0;
		pt.x > b.x ? b.x = pt.x : 0;
		pt.y < a.y ? a.y = pt.y : 0;
		pt.y > b.y ? b.y = pt.y : 0;
		pt.z < a.z ? a.z = pt.z : 0;
		pt.z > b.z ? b.z = pt.z : 0;
	}
	vector3 getcenter()
	{
		return (a + b) * 0.5;
	}
};

// 整数boundingbox
struct boundingboxi
{
	point3_t a = point3_t(100000, 100000, 100000);
	point3_t b = point3_t(-100000, -100000, -100000);

	boundingboxi() {}

	boundingboxi(const point3_t& _min, const point3_t& _max)
	{
		a = _min;
		b = _max;
	}
	inline bool intersection(const point3_t& pt, int tolerance = 0)
	{
		return 	
			pt.x >= a.x - tolerance && pt.x < b.x + tolerance &&
			pt.y >= a.y - tolerance && pt.y < b.y + tolerance &&
			pt.z >= a.z - tolerance && pt.z < b.z + tolerance;
	}
	inline bool intersection(const boundingboxi& b2) const
	{
		// Use up to 6 separating planes
		if (b.x < b2.a.x)
			return false;
		if (b.y < b2.a.y)
			return false;
		if (b.z < b2.a.z)
			return false;

		if (a.x > b2.b.x)
			return false;
		if (a.y > b2.b.y)
			return false;
		if (a.z > b2.b.z)
			return false;

		// otherwise, must be intersecting
		return true;
	}
};
// **********************************************************************
// CTexture
// **********************************************************************
#define TEXSIZEX		1024
#define TEXSIZEY		1024
inline unsigned int widthByte32(unsigned int width, unsigned int depth)
{
	// 32 bits alignment (4 bytes)
	int rest = (width * depth / 8) % 4;
	if (rest != 0)
		return (width * depth / 8 + 4 - rest);
	else
		return (width * depth / 8);
}

struct CTexture
{
	unsigned char data[TEXSIZEX * TEXSIZEY * 3]; // 24位数据
	unsigned int at(int x, int y)
	{
		if (x >= TEXSIZEX || x < 0 ||
			y >= TEXSIZEY || y < 0)
			return 0;

		int BytePerPixel = 24 / 8;
		int i = x, j = y;
		int m_WidthByte32 = widthByte32(TEXSIZEX, 24);
		return
			(data[m_WidthByte32 * j + i * BytePerPixel + 0] << 16) |
			(data[m_WidthByte32 * j + i * BytePerPixel + 1] << 8) |
			data[m_WidthByte32 * j + i * BytePerPixel + 2]
			;
	}
};
// **********************************************************************
// HSL
// **********************************************************************
typedef struct
{
	int hue;              // [0,255]
	int saturation;       // [0,255]
	int luminance;        // [0,255]
}COLOR_HSL;

// Converts HSL to RGB
int HSLtoRGB(const COLOR_HSL* hsl)
{
	real h = hsl->hue / 255.f * 360.f;     // h must be [0, 360]
	real s = hsl->saturation / 255.f;		// s must be [0, 1]
	real l = hsl->luminance / 255.f;        // l must be [0, 1]
	real R, G, B;
	if (hsl->saturation == 0)
	{
		// achromatic color (gray scale)
		R = G = B = l * 255.f;
	}
	else
	{
		real q = (l < 0.5f) ? (l * (1.0f + s)) : (l + s - (l * s));
		real p = (2.0f * l) - q;
		real Hk = h / 360.0f;
		real T[3];
		T[0] = Hk + 0.3333333f; // Tr   0.3333333f=1.0/3.0
		T[1] = Hk;              // Tb
		T[2] = Hk - 0.3333333f; // Tg
		for (int i = 0; i < 3; i++)
		{
			if (T[i] < 0) T[i] += 1.0f;
			if (T[i] > 1) T[i] -= 1.0f;
			if ((T[i] * 6) < 1)
			{
				T[i] = p + ((q - p) * 6.0f * T[i]);
			}
			else if ((T[i] * 2.0f) < 1) //(1.0/6.0)<=T[i] && T[i]<0.5
			{
				T[i] = q;
			}
			else if ((T[i] * 3.0f) < 2) // 0.5<=T[i] && T[i]<(2.0/3.0)
			{
				T[i] = p + (q - p) * ((2.0f / 3.0f) - T[i]) * 6.0f;
			}
			else T[i] = p;
		}
		R = T[0] * 255.0f;
		G = T[1] * 255.0f;
		B = T[2] * 255.0f;
	}
	unsigned char red = (unsigned char)((R > 255) ? 255 : ((R < 0) ? 0 : R));
	unsigned char green = (unsigned char)((G > 255) ? 255 : ((G < 0) ? 0 : G));
	unsigned char blue = (unsigned char)((B > 255) ? 255 : ((B < 0) ? 0 : B));
	return RGB(red, green, blue);
}
// **********************************************************************
// vector math
// **********************************************************************

inline real fract(real x)
{
	return x - floor(x);
}
inline vec3 floor(vec3 v)
{
	return vec3(floor(v.x), floor(v.y), floor(v.z));
}
inline vec3 fract(vec3 v)
{
	return vec3(fract(v.x), fract(v.y), fract(v.z));
}
real lerp(real h1, real h2, real alpha, real power = 1.0)
{
	if (power != 1.0)
		alpha = pow(alpha, power);

	return h1 * (1.0f - alpha) + h2 * alpha;
}
vector2 lerp(vector2 v1, vector2 v2, real alpha, real power = 1.0)
{
	if (power != 1.0)
		alpha = pow(alpha, power);

	return v1 * (1 - alpha) + v2 * alpha;
}
vector3 lerp(vector3 v1, vector3 v2, real alpha, real power = 1.0)
{
	if (power != 1.0)
		alpha = pow(alpha, power);

	return v1 * (1 - alpha) + v2 * alpha;
}
// **********************************************************************
// globles
// **********************************************************************
real 				rndmap[1024][1024];
real				fmin1 = 0;
real				fmax1 = 0;

real				fheightmap0[IMAGESCALE][IMAGESCALE] = { 0 };
real(*fheightmap)[IMAGESCALE] = fheightmap0;

real				depthmap0[IMAGESCALE][IMAGESCALE] = { 0 };
real(*depthmap)[IMAGESCALE] = depthmap0;

int					drawmap0[IMAGESCALE][IMAGESCALE] = { 0 };
int(*drawmap)[IMAGESCALE] = drawmap0;

// **********************************************************************
// Perlin 2d random
// **********************************************************************
// Ken Perlin's Map Gennor
// ------------------------------------------------
int myfloor(real value) { return (value >= 0 ? (int)value : (int)value - 1); } // 取整
inline real fade(real x) { return (x * x * x * (x * (6 * x - 15) + 10)); } // this equates to 6x^5 - 15x^4 + 10x^3  // 插值算法

// ------------------------------------------------
inline real dot(real x1, real y1, real x2, real y2)
{
	return x1 * x2 + y1 * y2;
}
// ------------------------------------------------
extern real blend(real h1, real h2, real alpha, real power);
void perlinmap(real map[IMAGESCALE][IMAGESCALE], int size, real* min1, real* max1, int octaves)
{
	//set up some variables
	int i, j, k, x, y, grad11, grad12, grad21, grad22;

	real pixel_value,
		fracX, fracY,
		noise11, noise12, noise21, noise22,
		interpolatedx1, interpolatedx2, interpolatedxy,
		amplitude, frequency,
		gain = 0.50f, lacunarity = 2.0f;
	real gradients[8][2];
	int permutations[256];

	*min1 = 1000000;
	*max1 = -1000000;

	//梯度
	for (i = 0; i < 8; ++i)
	{
		gradients[i][0] = cos(0.785398163f * (real)i);
		gradients[i][1] = sin(0.785398163f * (real)i);
	}

	// 置换表
	{
		for (i = 0; i < 256; ++i)
			permutations[i] = i;
		for (i = 0; i < 256; ++i)
		{
			j = rand() % 256;
			k = permutations[i];
			permutations[i] = permutations[j];
			permutations[j] = k;
		}
	}

	// 生成MAP
	for (i = 0; i < size; ++i)
	{
		for (j = 0; j < size; ++j)
		{
			//get the value for this pixel by adding successive layers
			amplitude = 1.0f;
			frequency = 1.0f / (real)size;
			pixel_value = 0.0f;
			map[j][i] = 0;
			for (k = 0; k < octaves; ++k)
			{
				// 整数
				x = (int)((real)j * frequency);
				y = (int)((real)i * frequency);

				// 小数
				fracX = (real)j * frequency - (real)x;
				fracY = (real)i * frequency - (real)y;

				// 得到梯度索引
				grad11 = permutations[(x + permutations[y % 255]) % 255] % 8;
				grad12 = permutations[(x + 1 + permutations[y % 255]) % 255] % 8;
				grad21 = permutations[(x + permutations[(y + 1) % 255]) % 255] % 8;
				grad22 = permutations[(x + 1 + permutations[(y + 1) % 255]) % 255] % 8;

				// 四个角的梯度投影
				noise11 = dot(gradients[grad11][0], gradients[grad11][1], fracX, fracY);
				noise12 = dot(gradients[grad12][0], gradients[grad12][1], fracX - 1.0f, fracY);
				noise21 = dot(gradients[grad21][0], gradients[grad21][1], fracX, fracY - 1.0f);
				noise22 = dot(gradients[grad22][0], gradients[grad22][1], fracX - 1.0f, fracY - 1.0f);

				// 插值算法
				fracX = fade(fracX);
				fracY = fade(fracY);

				interpolatedx1 = blend(noise11, noise12, fracX, 1);
				interpolatedx2 = blend(noise21, noise22, fracX, 1);
				interpolatedxy = blend(interpolatedx1, interpolatedx2, fracY, 1);

				//	叠加
				pixel_value += interpolatedxy * amplitude;
				amplitude *= gain;

				// 缩小区域
				frequency *= lacunarity;
			}

			//put it in the map
			map[j][i] = pixel_value;

			//do some quick checks
			if (pixel_value < *min1)
				*min1 = pixel_value;
			else if (pixel_value > * max1)
				*max1 = pixel_value;
		}
	}
}

// ------------------------------------------------
// perlinnosie 3d
// ------------------------------------------------
struct ImprovedNoise
{
	static double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
	static double lerp(double t, double a, double b) { return a + t * (b - a); }
	static double grad(int hash, double x, double y, double z) {
		int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE
		double u = h < 8 ? x : y,                 // INTO 12 GRADIENT DIRECTIONS.
			v = h < 4 ? y : h == 12 || h == 14 ? x : z;
		return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
	}

	static double noise(double x, double y, double z)
	{
		static int p[512] = { -1 };
		if (p[0] == -1)
		{
			static int permutation[512] = { 151,160,137,91,90,15,
				131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
				190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
				88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
				77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
				102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
				135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
				5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
				223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
				129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
				251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
				49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
				138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
			};

			for (int i = 0; i < 256; i++) p[256 + i] = p[i] = permutation[i];
		}


		int X = (int)floor(x) & 255,                  	// FIND UNIT CUBE THAT
			Y = (int)floor(y) & 255,                  		// CONTAINS POINT.
			Z = (int)floor(z) & 255;
		x -= floor(x);                                	// FIND RELATIVE X,Y,Z
		y -= floor(y);                                	// OF POINT IN CUBE.
		z -= floor(z);
		double u = fade(x),                              // COMPUTE FADE CURVES
			v = fade(y),                                // FOR EACH OF X,Y,Z.
			w = fade(z);
		int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z,      // HASH COORDINATES OF
			B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;      // THE 8 CUBE CORNERS,

		return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),  // AND ADD
			grad(p[BA], x - 1, y, z)), // BLENDED
			lerp(u, grad(p[AB], x, y - 1, z),  // RESULTS
				grad(p[BB], x - 1, y - 1, z))),// FROM  8
			lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),  // CORNERS
				grad(p[BA + 1], x - 1, y, z - 1)), // OF CUBE
				lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
					grad(p[BB + 1], x - 1, y - 1, z - 1))));
	}
};
real perlin(real x, real y, real z)
{
	return ImprovedNoise::noise(x, y, z);
}

double fBm(double x, double y, double z, int octaves)
{
	double frequency = 1.0f;
	double amplitude = 0.5f;
	double sum = 0.0f;
	double gain = 0.50f;
	double lacunarity = 2.0f;

	for (int i = 0; i < octaves; i++)
	{
		sum += perlin(x * frequency, y * frequency, z * frequency) * amplitude;
		//sum += amplitude * (1-fabs(perlin(x * frequency, y * frequency, z * frequency)));
		frequency *= lacunarity;
		amplitude *= gain;
	}
	return sum;
}
inline double ridge(double h, real offset)
{
	h = fabs(h);
	h = offset - h;
	h = h * h;
	return h;
}
double RidgedMF(double x, double y, double z, int octaves, real offset)
{
	double sum = 0;
	real amplitude = 0.5f;
	real frequency = 1.0f;
	double prev = 1.0f;

	double gain = 0.50f;
	double lacunarity = 2.5f;

	for (int i = 0; i < octaves; i++)
	{
		double n = ridge(perlin(x * frequency, y * frequency, z * frequency), offset);
		sum += n * amplitude * prev;
		prev = n;
		frequency *= lacunarity;
		amplitude *= gain;
	}

	return sum;
}

// ××××××××××××××××××××××××××××××××××××××××××××××××××××××
// Polygonising a scalar field
// 代码来自： http://paulbourke.net/geometry/polygonise/
// ××××××××××××××××××××××××××××××××××××××××××××××××××××××
#define XYZ vec
typedef struct {
	XYZ p[3];
} TRIANGLE;

typedef struct {
	XYZ p[8];
	double val[8];
} GRIDCELL;

/*
   Linearly interpolate the position where an isosurface cuts
   an edge between two vertices, each with their own scalar value
*/

XYZ VertexInterp(double isolevel, XYZ p1, XYZ p2, double valp1, double valp2)
{
	double mu;
	XYZ p;

	if (fabs(isolevel - valp1) < 0.00001)
		return(p1);
	if (fabs(isolevel - valp2) < 0.00001)
		return(p2);
	if (fabs(valp1 - valp2) < 0.00001)
		return(p1);
	mu = (isolevel - valp1) / (valp2 - valp1);
	p.x = p1.x + mu * (p2.x - p1.x);
	p.y = p1.y + mu * (p2.y - p1.y);
	p.z = p1.z + mu * (p2.z - p1.z);

	return(p);
}
/*
   Given a grid cell and an isolevel, calculate the triangular
   facets required to represent the isosurface through the cell.
   Return the number of triangular facets, the array "triangles"
   will be loaded up with the vertices at most 5 triangular facets.
	0 will be returned if the grid cell is either totally above
   of totally below the isolevel.
*/
int Polygonise(GRIDCELL grid, double isolevel, TRIANGLE* triangles)
{
	int i, ntriang;
	int cubeindex;
	XYZ vertlist[12];

	static const int edgeTable[256] = {
	0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
	0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
	0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
	0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
	0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
	0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
	0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
	0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
	0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
	0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
	0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
	0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
	0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
	0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
	0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
	0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
	0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
	0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
	0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
	0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
	0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
	0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
	0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
	0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
	0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
	0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
	0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
	0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
	0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
	0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
	0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
	0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0 };
	static const int triTable[256][16] =
	{ {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
	{3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
	{3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
	{3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
	{9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
	{9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
	{2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
	{8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
	{9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
	{4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
	{3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
	{1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
	{4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
	{4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
	{5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
	{2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
	{9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
	{0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
	{2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
	{10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
	{5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
	{5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
	{9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
	{0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
	{1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
	{10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
	{8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
	{2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
	{7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
	{2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
	{11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
	{5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
	{11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
	{11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
	{1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
	{9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
	{5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
	{2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
	{5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
	{6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
	{3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
	{6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
	{5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
	{1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
	{10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
	{6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
	{8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
	{7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
	{3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
	{5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
	{0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
	{9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
	{8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
	{5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
	{0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
	{6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
	{10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
	{10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
	{8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
	{1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
	{0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
	{10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
	{3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
	{6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
	{9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
	{8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
	{3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
	{6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
	{0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
	{10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
	{10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
	{2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
	{7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
	{7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
	{2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
	{1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
	{11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
	{8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
	{0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
	{7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
	{10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
	{2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
	{6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
	{7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
	{2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
	{1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
	{10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
	{10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
	{0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
	{7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
	{6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
	{8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
	{9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
	{6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
	{4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
	{10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
	{8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
	{0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
	{1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
	{8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
	{10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
	{4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
	{10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
	{5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
	{11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
	{9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
	{6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
	{7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
	{3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
	{7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
	{3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
	{6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
	{9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
	{1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
	{4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
	{7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
	{6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
	{3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
	{0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
	{6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
	{0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
	{11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
	{6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
	{5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
	{9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
	{1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
	{1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
	{10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
	{0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
	{5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
	{10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
	{11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
	{9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
	{7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
	{2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
	{8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
	{9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
	{9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
	{1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
	{9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
	{9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
	{5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
	{0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
	{10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
	{2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
	{0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
	{0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
	{9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
	{5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
	{3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
	{5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
	{8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
	{0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
	{9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
	{1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
	{3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
	{4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
	{9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
	{11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
	{11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
	{2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
	{9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
	{3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
	{1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
	{4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
	{3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
	{0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
	{9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
	{1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1} };

	/*
	   Determine the index into the edge table which
	   tells us which vertices are inside of the surface
	*/
	cubeindex = 0;
	if (grid.val[0] < isolevel) cubeindex |= 1;
	if (grid.val[1] < isolevel) cubeindex |= 2;
	if (grid.val[2] < isolevel) cubeindex |= 4;
	if (grid.val[3] < isolevel) cubeindex |= 8;
	if (grid.val[4] < isolevel) cubeindex |= 16;
	if (grid.val[5] < isolevel) cubeindex |= 32;
	if (grid.val[6] < isolevel) cubeindex |= 64;
	if (grid.val[7] < isolevel) cubeindex |= 128;

	/* Cube is entirely in/out of the surface */
	if (edgeTable[cubeindex] == 0)
		return(0);

	/* Find the vertices where the surface intersects the cube */
	if (edgeTable[cubeindex] & 1)
		vertlist[0] =
		VertexInterp(isolevel, grid.p[0], grid.p[1], grid.val[0], grid.val[1]);
	if (edgeTable[cubeindex] & 2)
		vertlist[1] =
		VertexInterp(isolevel, grid.p[1], grid.p[2], grid.val[1], grid.val[2]);
	if (edgeTable[cubeindex] & 4)
		vertlist[2] =
		VertexInterp(isolevel, grid.p[2], grid.p[3], grid.val[2], grid.val[3]);
	if (edgeTable[cubeindex] & 8)
		vertlist[3] =
		VertexInterp(isolevel, grid.p[3], grid.p[0], grid.val[3], grid.val[0]);
	if (edgeTable[cubeindex] & 16)
		vertlist[4] =
		VertexInterp(isolevel, grid.p[4], grid.p[5], grid.val[4], grid.val[5]);
	if (edgeTable[cubeindex] & 32)
		vertlist[5] =
		VertexInterp(isolevel, grid.p[5], grid.p[6], grid.val[5], grid.val[6]);
	if (edgeTable[cubeindex] & 64)
		vertlist[6] =
		VertexInterp(isolevel, grid.p[6], grid.p[7], grid.val[6], grid.val[7]);
	if (edgeTable[cubeindex] & 128)
		vertlist[7] =
		VertexInterp(isolevel, grid.p[7], grid.p[4], grid.val[7], grid.val[4]);
	if (edgeTable[cubeindex] & 256)
		vertlist[8] =
		VertexInterp(isolevel, grid.p[0], grid.p[4], grid.val[0], grid.val[4]);
	if (edgeTable[cubeindex] & 512)
		vertlist[9] =
		VertexInterp(isolevel, grid.p[1], grid.p[5], grid.val[1], grid.val[5]);
	if (edgeTable[cubeindex] & 1024)
		vertlist[10] =
		VertexInterp(isolevel, grid.p[2], grid.p[6], grid.val[2], grid.val[6]);
	if (edgeTable[cubeindex] & 2048)
		vertlist[11] =
		VertexInterp(isolevel, grid.p[3], grid.p[7], grid.val[3], grid.val[7]);

	/* Create the triangle */
	ntriang = 0;
	for (i = 0; triTable[cubeindex][i] != -1; i += 3) {
		triangles[ntriang].p[0] = vertlist[triTable[cubeindex][i]];
		triangles[ntriang].p[1] = vertlist[triTable[cubeindex][i + 1]];
		triangles[ntriang].p[2] = vertlist[triTable[cubeindex][i + 2]];
		ntriang++;
	}

	return(ntriang);
}

//××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××
// otaviogood's noise from https://www.shadertoy.com/view/ld2SzK
//××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××
// This spiral noise works by successively adding and rotating sin waves while increasing frequency.
// It should work the same on all computers since it's not based on a hash function like some other noises.
// It can be much faster than other noise functions if you're ok with some repetition.
const float nudge = 0.739513;	// size of perpendicular vector
float normalizer = 1.0 / sqrt(1.0 + nudge * nudge);	// pythagorean theorem on that perpendicular to maintain scale
float SpiralNoiseC(vec3 p)
{
	float n = 0.0;	// noise amount
	float iter = 1.0;
	for (int i = 0; i < 8; i++)
	{
		// add sin and cos scaled inverse with the frequency
		n += -abs(sin(p.y * iter) + cos(p.x * iter)) / iter;	// abs for a ridged look
		// rotate by adding perpendicular and scaling down
		p.xy(p.xy() + vec2(p.y, -p.x) * nudge);
		p.xy(p.xy() * normalizer);
		// rotate on other axis
		p.xz(p.xz() + vec2(p.z, -p.x) * nudge);
		p.xz(p.xz() * normalizer);
		// increase the frequency
		iter *= 1.733733;
	}
	return n;
}

float SpiralNoise3D(vec3 p)
{
	float n = 0.0;
	float iter = 1.0;
	for (int i = 0; i < 5; i++)
	{
		n += (sin(p.y * iter) + cos(p.x * iter)) / iter;
		p.xz(p.xz() + vec2(p.z, -p.x) * nudge);
		p.xz(p.xz() * normalizer);
		iter *= 1.33733;
	}
	return n;
}

float NebulaNoise(vec3 p)
{
	float final = p.y + 4.5;
	final -= SpiralNoiseC(p);   // mid-range noise
	final += SpiralNoiseC(p.zxy() * 0.5123 + vec3(100.0, 100.0, 100.0)) * 4.0;   // large scale features
	final -= SpiralNoise3D(p);   // more large scale features, but 3d

	return final;
}

float map(vec3 p)
{
#ifdef ROTATION
	R(p.xz, iMouse.x * 0.008 * pi + iTime * 0.1);
#endif

	float NebNoise = abs(NebulaNoise(p / 0.5) * 0.5);

	return NebNoise + 0.03;
}

//--------------------------------------------------------------
// iq's noise
//--------------------------------------------------------------
float hash0(float n) { return fract(sin(n) * 753.5453123); }
float noise(vec3 x)
{
	vec3 p = floor(x);
	vec3 f = fract(x);
	f = f * f * (vec3(3.0, 3.0, 3.0) - f * 2.0);

	float n = p.x + p.y * 157.0 + 113.0 * p.z;
	return mix(mix(mix(hash0(n + 0.0), hash0(n + 1.0), f.x),
		mix(hash0(n + 157.0), hash0(n + 158.0), f.x), f.y),
		mix(mix(hash0(n + 113.0), hash0(n + 114.0), f.x),
			mix(hash0(n + 270.0), hash0(n + 271.0), f.x), f.y), f.z);
}

// **********************************************************************
// shape blend
// **********************************************************************
int blendi(int h1, int h2, real alpha, real power = 1.0)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;
	if (power != 1.0)
		alpha = pow(alpha, power);

	return int(h1 * (1.0f - alpha) + h2 * alpha);
}
real blend(real h1, real h2, real alpha, real power = 1.0)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;
	if (power != 1.0)
		alpha = pow(alpha, power);

	return h1 * (1.0f - alpha) + h2 * alpha;
}
vector2 blend(const vector2& v1, const vector2& v2, real alpha, real power = 1.0)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;
	if (power != 1.0)
		alpha = pow(alpha, power);

	return v1 * (1 - alpha) + v2 * alpha;
}
vector3 blend(const vector3& v1, const vector3& v2, real alpha, real power = 1.0)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;
	if (power != 1.0)
		alpha = pow(alpha, power);

	return v1 * (1 - alpha) + v2 * alpha;
}

// ------------------------------------------------
real blend2(real h1, real h2, real alpha, real power = 1.0)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	if (alpha < 0.5f)
	{
		alpha = (0.5f - alpha) * 2.0f;
		if (power != 1.0)
			alpha = pow(alpha, power);
		return h2 * (1 - alpha) + h1 * alpha;
	}
	else
	{
		alpha = (alpha - 0.5f) * 2.0f;
		if (power != 1.0)
			alpha = pow(alpha, power);
		return h2 * (1 - alpha) + h1 * alpha;
	}
}
// ------------------------------------------------
real blendn(real h1, real h2, real alpha, int n, real power)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	/*
	alpha = alpha * n - int(alpha * n);
	if (alpha < 0.5)
	{
		alpha = (0.5 - alpha) / 0.5;

	}
	else
	{
		alpha = (alpha - 0.5) / 0.5;
	}*/
	alpha = pow(fabs(cos(alpha * n * PI)), power);

	return h2 * (1 - alpha) + h1 * alpha;
}
// ------------------------------------------------
vector2 blend2(vector2 h1, vector2 h2, real alpha, real power = 1.0)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	if (alpha < 0.5f)
	{
		alpha = (0.5f - alpha) * 2.0f;
		if (power != 1.0)
			alpha = pow(alpha, power);
		return h2 * (1 - alpha) + h1 * alpha;
	}
	else
	{
		alpha = (alpha - 0.5f) * 2.0f;
		if (power != 1.0)
			alpha = pow(alpha, power);
		return h2 * (1 - alpha) + h1 * alpha;
	}
}
// ------------------------------------------------
vector3 blend2(vector3 h1, vector3 h2, real alpha, real power = 1.0)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	if (alpha < 0.5f)
	{
		alpha = (0.5f - alpha) * 2.0f;
		if (power != 1.0)
			alpha = pow(alpha, power);
		return h2 * (1 - alpha) + h1 * alpha;
	}
	else
	{
		alpha = (alpha - 0.5f) * 2.0f;
		if (power != 1.0)
			alpha = pow(alpha, power);
		return h2 * (1 - alpha) + h1 * alpha;
	}
}

real blend3(real h1, real h2, real alpha, real mid = 0.5, real power = 1)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	if (alpha < mid)
	{
		alpha = (mid - alpha) / mid;
		alpha = pow(alpha, power);
		return h2 * (1 - alpha) + h1 * alpha;
	}
	else
	{
		alpha = (alpha - mid) / (1 - mid);
		alpha = pow(alpha, power);
		return h2 * (1 - alpha) + h1 * alpha;
	}
}

real blend4(real h1, real h2, real alpha, real power = 1)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	real mid = 0.5;
	if (alpha < mid)
	{
		alpha = alpha / mid;
		alpha = pow(alpha, 1.0f / power);
		return h1 * (1 - alpha) + h2 * alpha;
	}
	else
	{
		alpha = (1 - alpha) / (1 - mid);
		alpha = pow(alpha, 1.0f / power);
		return h1 * (1 - alpha) + h2 * alpha;
	}
}

real blend5(real h1, real h2, real alpha, real mid = 0.5, real power = 1)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	if (alpha < mid)
	{
		alpha = alpha / mid;
		alpha = pow(alpha, 1.0f / power);
		return h1 * (1 - alpha) + h2 * alpha;
	}
	else
	{
		alpha = (1 - alpha) / (1 - mid);
		alpha = pow(alpha, 1.0f / power);
		return h1 * (1 - alpha) + h2 * alpha;
	}
}
inline real clamp(real v, real lo, real hi)
{
	ASSERT(!(hi < lo));
	return (v < lo) ? lo : (hi < v) ? hi : v;
}
inline int clampi(int v, int lo, int hi)
{
	ASSERT(!(hi < lo));
	return (v < lo) ? lo : (hi < v) ? hi : v;
}
// ------------------------------------------------------------------------------------------------
vec3 bezier2(vec3 cp[3], real t)
{
	return cp[0] * ((1 - t) * (1 - t)) + cp[1] * (2 * t * (1 - t)) + cp[2] * (t * t);
}
// ------------------------------------------------------------------------------------------------
vec3 bezier3(vec3 cp[4], real t)
{
	real s = 1 - t;
	return  cp[0] * (s * s * s) +
		cp[1] * (3 * t * s * s) +
		cp[2] * (3 * t * t * s) +
		cp[3] * (t * t * t);
}
// ------------------------------------------------------------------------------------------------
vec2 bezier2(vec2 cp[3], real t)
{
	return cp[0] * ((1 - t) * (1 - t)) + cp[1] * (2 * t * (1 - t)) + cp[2] * (t * t);
}
// ------------------------------------------------------------------------------------------------
vec2 bezier2(vec2 p1, vec2 p2, vec2 p3, real t)
{
	return p1 * ((1 - t) * (1 - t)) + p2 * (2 * t * (1 - t)) + p3 * (t * t);
}
// ------------------------------------------------------------------------------------------------
vec2 bezier3(vec2 cp[4], real t)
{
	real s = 1 - t;
	return  cp[0] * (s * s * s) +
		cp[1] * (3 * t * s * s) +
		cp[2] * (3 * t * t * s) +
		cp[3] * (t * t * t);
}
// ------------------------------------------------------------------------------------------------
vec bezier2(vec p1, vec p2, vec p3, real t)
{
	return p1 * ((1 - t) * (1 - t)) + p2 * (2 * t * (1 - t)) + p3 * (t * t);
}
// ------------------------------------------------------------------------------------------------
real bezier2(real cp[3], real t)
{
	return cp[0] * ((1 - t) * (1 - t)) + cp[1] * (2 * t * (1 - t)) + cp[2] * (t * t);
}
// ------------------------------------------------------------------------------------------------
real bezier3(real cp[4], real t)
{
	real s = 1 - t;
	return  cp[0] * (s * s * s) +
		cp[1] * (3 * t * s * s) +
		cp[2] * (3 * t * t * s) +
		cp[3] * (t * t * t);
}
// -----------------------------------------------------------------------
inline real roundblend(real h1, real h2, real alpha)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;
	//alpha = sqrt(1 - (1-alpha)*(1-alpha));
	alpha = sqrt(1 - (alpha) * (alpha));
	return h2 * (1.0f - alpha) + h1 * alpha;

}
// -----------------------------------------------------------------------
inline real roundblend2(real h1, real h2, real alpha, real power = 1)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	if (power != 1)
		alpha = pow(alpha, power);

	if (alpha < 0.5)
	{
		alpha = alpha * 2;
		alpha = sqrt(1 - (1 - alpha) * (1 - alpha));

		return h1 * (1 - alpha) + h2 * alpha;
	}
	else
	{
		alpha = (alpha - 0.5) * 2;
		alpha = sqrt(1 - (alpha) * (alpha));

		return h1 * (1 - alpha) + h2 * alpha;
	}

}

// -----------------------------------------------------------------------
// 三角函数插值
inline real BlendSin(real h1, real h2, real alpha)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	alpha = sin(alpha * PI / 2);

	return h1 * (1 - alpha) + h2 * alpha;
}
// -----------------------------------------------------------------------
// 傅立叶级数
inline real FT(real angle, real t[] = 0, real dt = 0)
{
	if (t == 0)
	{
		static real s_t0[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
		t = s_t0;
	}

	real yy = 0;
	yy += 1 * sin(1 * angle + (t[0] + dt) * PI);
	yy += 0.5 * sin(2 * angle + (t[1] + dt) * PI);
	yy += 0.25 * sin(4 * angle + (t[2] + dt) * PI);
	yy += 0.125 * sin(8 * angle + (t[3] + dt) * PI);

	return yy;
}
// -----------------------------------------------------------------------
inline real FTU(real ang, real t[] = 0, real dt = 0)
{
	real ft = FT(ang, t, dt);
	real max1 = (1 + 0.5 + 0.25 + 0.125);
	real min1 = -max1;
	return (ft - min1) / (max1 - min1);
}
// -----------------------------------------------------------------------
inline real BlendFT(real h1, real h2, real alpha, real t[] = 0, real dt = 0)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	alpha = FTU(alpha, t, dt);

	return h1 * (1 - alpha) + h2 * alpha;
}
// -----------------------------------------------------------------------
inline real FT2D1(real anglex, real angley, real tx[] = 0, real ty[] = 0, real dtx = 0, real dty = 0)
{
	if (tx == 0)
	{
		static real s_t0x[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
		tx = s_t0x;
	}
	if (ty == 0)
	{
		static real s_t0y[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
		ty = s_t0y;
	}

	real yy = 0;
	yy += 1 * sin(1 * anglex + (tx[0] + dtx) * PI) + 1 * sin(1 * angley + (ty[0] + dty) * PI);
	yy += 0.5 * sin(2 * anglex + (tx[1] + dtx) * PI) + 0.5 * sin(2 * angley + (ty[1] + dty) * PI);
	yy += 0.25 * sin(4 * anglex + (tx[2] + dtx) * PI) + 0.25 * sin(4 * angley + (ty[2] + dty) * PI);
	yy += 0.125 * sin(8 * anglex + (tx[3] + dtx) * PI) + 0.125 * sin(8 * angley + (ty[3] + dty) * PI);

	return yy;
}
// ------------------------------------------------------------------------------------------------
real FFT(real ax, real ay, real rndmap[1024][1024])
{
	real ft = 0;
	for (int i = 0; i < 8; i++)
	{
		real dz = blend(ax, ay, rndmap[100][i], 2);
		{
			real A = 2 * rndmap[0][i];
			real F = blend(20, 50, rndmap[1][i]);
			real T0 = 2 * PI * rndmap[2][i];
			ft += A * (0.5 + 0.5 * sin(T0 + F * dz * PI * 2));
		}
	}
	return ft / (8);
}

// -----------------------------------------------------------------------
inline real FT2D(real anglex, real angley, real tx[] = 0, real ty[] = 0, real dtx = 0, real dty = 0)
{
	static real s_fx[] = { rrnd(0, 8), rrnd(0, 4), rrnd(0, 2), rrnd(0, 1) };
	static real s_fy[] = { rrnd(0, 8), rrnd(0, 4), rrnd(0, 2), rrnd(0, 1) };
	if (tx == 0)
	{
		static real s_t0x[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
		tx = s_t0x;
	}
	if (ty == 0)
	{
		static real s_t0y[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
		ty = s_t0y;
	}

	real yy = 0;
	yy += 1 * sin(s_fx[0] * anglex + (tx[0] + dtx) * PI + s_fy[0] * angley + (ty[0] + dty) * PI);
	yy += .5 * sin(s_fx[1] * anglex + (tx[1] + dtx) * PI + s_fy[1] * angley + (ty[1] + dty) * PI);
	yy += .25 * sin(s_fx[2] * anglex + (tx[2] + dtx) * PI + s_fy[2] * angley + (ty[2] + dty) * PI);
	yy += .125 * sin(s_fx[3] * anglex + (tx[3] + dtx) * PI + s_fy[3] * angley + (ty[3] + dty) * PI);

	return yy;
}

// ------------------------------------------------
// 2d
real blend2d(real h1, real h2, real alphaX, real alphaY)
{
	int size;
	real alpha;
	alphaX < 0 ? alphaX = 0 : 0;
	alphaX > 1 ? alphaX = 1 : 0;
	alphaY < 0 ? alphaY = 0 : 0;
	alphaY > 1 ? alphaY = 1 : 0;
	size = IMAGESCALE;
	alpha = fheightmap[(int)(alphaX * size + 0.5) % size][(int)(alphaY * size + 0.5) % size];
	alpha = (alpha - fmin1) / (fmax1 - fmin1);

	return h1 * (1 - alpha) + h2 * alpha;
}
// **********************************************************************
// color blend
// **********************************************************************
inline int blendcor(int c1, int c2, real alpha, real power = 1.0)
{
	//alpha = alpha != 1 ? abs(alpha) - (int)(alpha) : 1;	
	alpha = alpha > 1 ? 1 : alpha;
	alpha = alpha < 0 ? 0 : alpha;

	if (power != 1.0)
		alpha = pow(alpha, power);
	return _RGB(GetRValue(c2) * alpha + GetRValue(c1) * (1 - alpha),
		GetGValue(c2) * alpha + GetGValue(c1) * (1 - alpha),
		GetBValue(c2) * alpha + GetBValue(c1) * (1 - alpha)
	);

}
inline int blendcorRGBA(int c1, int c2, real alpha, real power = 1.0)
{
	//alpha = alpha != 1 ? abs(alpha) - (int)(alpha) : 1;
	alpha = alpha > 1 ? 1 : alpha;
	alpha = alpha < 0 ? 0 : alpha;

	if (power != 1.0)
		alpha = pow(alpha, power);
	return RGBA(GetRValue(c2) * alpha + GetRValue(c1) * (1 - alpha),
		GetGValue(c2) * alpha + GetGValue(c1) * (1 - alpha),
		GetBValue(c2) * alpha + GetBValue(c1) * (1 - alpha),
		GetAValue(c2) * alpha + GetAValue(c1) * (1 - alpha)
	);

}

// ------------------------------------------------
// 2d
inline int blendcor2d(int c1, int c2, real alphaX, real alphaY, real power = 1.0)
{
	real alpha;
	int size = IMAGESCALE;
	alphaX < 0 ? alphaX = 0 : 0;
	alphaX > 1 ? alphaX = 1 : 0;
	alphaY < 0 ? alphaY = 0 : 0;
	alphaY > 1 ? alphaY = 1 : 0;

	alpha = fheightmap[(int)(alphaX * size) % size][(int)(alphaY * size) % size];
	alpha = (alpha - fmin1) / (fmax1 - fmin1);

	return blendcor(c1, c2, alpha, power);
}

// color add

inline int addcor(int c1, int c2)
{
	int r = GetRValue(c1) + GetRValue(c2);
	int g = GetGValue(c1) + GetGValue(c2);
	int b = GetBValue(c1) + GetBValue(c2);
	return RGB(r > 255 ? 255 : r,
		g > 255 ? 255 : g,
		b > 255 ? 255 : b);

}
// mixorder

void mixorder(char ar[4])
{
	for (int i = 0; i < 4; i++)
	{
		char a = rand() % 4;
		char b = rand() % 4;

		char tm = ar[b];
		ar[b] = ar[a];
		ar[a] = tm;
	}
}

inline int rgb(const vec3& v)
{
	return _RGB(int(v.x * 255), int(v.y * 255), int(v.z * 255));
}
// **********************************************************************
// 3d数学
// **********************************************************************
inline real length(crvec2 p)
{
	return p.len();
}
inline real length(crvec p)
{
	return p.len();
}
inline vec3 normalize(crvec p)
{
	return p.normcopy();
}
inline real dot(crvec v1, crvec v2)
{
	return v1.dot(v2);
}
inline vec3 cross(crvec v1, crvec v2)
{
	return v1.cross(v2);
}
inline real smoothstep(real a, real b, real s)
{
	real t = blend(0.0f, 1.0f, (s - a) / (b - a));
	return t * t * (3.0 - 2.0 * t);
}
// cross
inline void cross(crvec v1, crvec v2, rvec dest)
{
	dest.x = v1.y * v2.z - v1.z * v2.y;
	dest.y = v1.z * v2.x - v1.x * v2.z;
	dest.z = v1.x * v2.y - v1.y * v2.x;
}
/**
	计算一条3d空间直线和一个3d空间平面的交点
	@param            pnv        空间平面的法线的单位矢量
	@param            pd         空间平面的参数距离,平面的方程为: pnv.dot( pv ) = pd
	@param            lvt        空间直线的方向
	@param            lv         空间直线上的一点
	@param            outV    计算后得到的位于平面上的交点坐标

*/
void planeAndSLIntersectionV(crvec pnv, real pd, crvec lv, crvec lvt, rvec outV)
{
	real dis = pnv.dot(lv) - pd;

	outV.x = lv.x + lvt.x * 1000;
	outV.y = lv.y + lvt.y * 1000;
	outV.z = lv.z + lvt.z * 1000;

	real td = pnv.dot(outV) - pd;

	real k = dis / (dis - td);

	outV.x -= lv.x;
	outV.y -= lv.y;
	outV.z -= lv.z;

	outV.x *= k;
	outV.y *= k;
	outV.z *= k;

	outV.x += lv.x;
	outV.y += lv.y;
	outV.z += lv.z;
}
/**
* 计算3d空间两个平面相交直线,得到的直线数据为: 直线的切向ltv和直线上的一点lpv
* @param  panv  空间平面a的法线的单位矢量
  @param  pad          空间平面a的参数距离,平面的方程为: pnv.dot( pv ) = pd
  @param  pbnv  空间平面b的法线的单位矢量
  @param  pbd          空间平面b的参数距离,平面的方程为: pnv.dot( pv ) = pd
  @param  ltv          两个平面交线的切向
  @param  lv          两个平面交线上的一点
*
**/

void pAndPIntersectionLine(crvec panv, real pad, crvec pbnv, real pbd, rvec ltv, rvec lv)
{
	// 计算出两条法线的叉积,即为交线的切向
	cross(panv, pbnv, ltv);
	ltv.norm();
	// 计算平面a上的一点
	vec pav;
	pav.x = pad * panv.x;
	pav.y = pad * panv.y;
	pav.z = pad * panv.z;
	vec tnv;
	// 计算出在平面a上的,垂直于交线ltv的直线的切向
	cross(ltv, panv, tnv);
	// 计算由pav和tnv决定的直线和平面b的交点,
	// 这个交点就是两个平面交线上的一点
	planeAndSLIntersectionV(pbnv, pbd, tnv, pav, lv);

}
// ------------------------------------------------
void getplanepm(crvec p1, crvec p2, crvec p3, rvec n, real& d)
{
	n = (p2 - p1).cross(p3 - p1); n.norm();
	d = p1.dot(n);
}

// ------------------------------------------------
// A = y2 - y1,B = x1- x2,C = x2 * y1 - x1 * y2
//distance(P,l)=|aX+bY+c|/   
inline real distance(const vector3& p1, const vector3& p2)
{
	return (p1 - p2).len();
}
//点到线段的最短距离,x0,y0是圆心
real distance(vector3 p, vector3 p1, vector3 p2)
{
	double ans = 0;
	double a, b, c;
	a = distance(p1, p2);
	b = distance(p1, p);
	c = distance(p2, p);
	if (c + b == a) {//点在线段上
		ans = 0;
		return ans;
	}
	if (a <= 0.00001) {//不是线段，是一个点
		ans = b;
		return ans;
	}
	if (c * c >= a * a + b * b) { //组成直角三角形或钝角三角形，p1为直角或钝角
		ans = b;
		return ans;
	}
	if (b * b >= a * a + c * c) {// 组成直角三角形或钝角三角形，p2为直角或钝角
		ans = c;
		return ans;
	}
	// 组成锐角三角形，则求三角形的高
	double p0 = (a + b + c) / 2;// 半周长
	double s = sqrt(p0 * (p0 - a) * (p0 - b) * (p0 - c));// 海伦公式求面积
	ans = 2 * s / a;// 返回点到线的距离（利用三角形面积公式求高）
	return ans;
}

real distance(const vector3& p, const segment& l)
{
	return distance(p, l.s, l.e);
}
real distance(const vector3& p1s, const vector3& p1e, const vector3& p2s, const vector3& p2e)
{
	segment l1(p1s, p1e);
	segment l2(p2s, p2e);

	return MIN(
		MIN(distance(p1s, l2), distance(p1e, l2)),
		MIN(distance(p2s, l1), distance(p2e, l1))
	);
}
// -----------------------------------------------------------------
// 三角形碰撞 
bool  checkPointInTriangle(
	const vector3& point,
	const vector3& a, const vector3& b, const vector3& c,
	real tolerance = 0.01f)
{
	real total_angles = 0.0f;

	// make the 3 vectors

	vector3 v1(point.x - a.x, point.y - a.y, point.z - a.z);
	v1.norm();
	vector3 v2(point.x - b.x, point.y - b.y, point.z - b.z);
	v2.norm();
	vector3 v3(point.x - c.x, point.y - c.y, point.z - c.z);
	v3.norm();

	real Dot1 = v2.dot(v1);
	Dot1 < -1.0f ? Dot1 = -1.0f : NULL;
	Dot1 > 1.0f ? Dot1 = 1.0f : NULL;
	total_angles += acos(Dot1);

	real Dot2 = v3.dot(v2);
	Dot2 < -1.0f ? Dot2 = -1.0f : NULL;
	Dot2 > 1.0f ? Dot2 = 1.0f : NULL;
	total_angles += acos(Dot2);

	real Dot3 = v1.dot(v3);
	Dot3 < -1.0f ? Dot3 = -1.0f : NULL;
	Dot3 > 1.0f ? Dot3 = 1.0f : NULL;
	total_angles += acos(Dot3);

	real deta = fabs(total_angles - 2.0f * PI);
	//PRINT("checkPointInTriangle " << deta);
	if (deta <= tolerance)
		return true;

	return false;
}
// -----------------------------------------------------------------
bool checkPointInTriangle0(const vec& s, const vec& a, const vec& b, const vec& c)
{
	int as_x = s.x - a.x;
	int as_y = s.y - a.y;

	bool s_ab = (b.x - a.x) * as_y - (b.y - a.y) * as_x > 0;

	if ((c.x - a.x) * as_y - (c.y - a.y) * as_x > 0 == s_ab) return false;

	if ((c.x - b.x) * (s.y - b.y) - (c.y - b.y) * (s.x - b.x) > 0 != s_ab) return false;

	return true;
}

bool PointinTriangle(const vector3& P, const vector3& A, const vector3& B, const vector3& C)
{
	vector3 v0 = C - A;
	vector3 v1 = B - A;
	vector3 v2 = P - A;

	float dot00 = v0.dot(v0);
	float dot01 = v0.dot(v1);
	float dot02 = v0.dot(v2);
	float dot11 = v1.dot(v1);
	float dot12 = v1.dot(v2);

	float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01);

	float u = (dot11 * dot02 - dot01 * dot12) * inverDeno;
	if (u < 0 || u > 1) // if u out of range, return directly
	{
		return false;
	}

	float v = (dot00 * dot12 - dot01 * dot02) * inverDeno;
	if (v < 0 || v > 1) // if v out of range, return directly
	{
		return false;
	}

	return u + v <= 1;
}
// 两线相交
inline bool LLIntersection(vec3& intersection, const vec3& p1, const vec3& v1, const vec3& p2, const vec3& v2)
{
	intersection = vec3::ZERO;
	if (v1.dot(v2) == 1)
	{
		// 两线平行
		return false;
	}

	vec3 startPointSeg = p2 - p1;
	vec3 vecS1 = v1.cross(v2);            // 有向面积1
	vec3 vecS2 = startPointSeg.cross(v2); // 有向面积2
	float num = startPointSeg.dot(vecS1);

	// 判断两这直线是否共面
	if (num >= 1E-05f || num <= -1E-05f)
	{
		return false;
	}

	// 有向面积比值，利用点乘是因为结果可能是正数或者负数
	float num2 = vecS2.dot(vecS1) / vecS1.sqrlen();

	intersection = p1 + v1 * num2;
	return true;
}
real dot(const vec3& O, const vec3& A, const vec3& B) {//点乘 
	real oa_x = A.x - O.x;
	real oa_y = A.y - O.y;
	real ob_x = B.x - O.x;
	real ob_y = B.y - O.y;
	return oa_x * ob_x + oa_y * ob_y;
}
double dis(const vec3& p1, const vec3& p2) {//求两点之间距离
	double ans = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
	return sqrt(ans);
}
real disOfPointToLine(const vec3& O, const vec3& lstart, const vec3& lend)
{
	real cos0 = dot(lstart, O, lend) / (dis(O, lstart) * dis(lstart, lend));
	return dis(O, lstart) * sin(acos(cos0));
}

// **********************************************************************
// projection
// **********************************************************************
inline vector2 viewprjold(const vector3& p)
{
	real vpf = blend(1, 0.1, p.z / 16.0);
	return vector2(
		0.5 + (p.x - 0.5) * vpf,
		0.95 + (p.y - 0.95) * vpf
	);
}
extern inline void viewprj(const vector3& rp, vector2& p2d, real& depth);
// **********************************************************************
// common draw
// **********************************************************************
// pixel
inline bool pixel(int ix, int iy, int color, real alpha = 1, real depth = 0)
{
	if (ix < 0 || iy < 0 || ix >= IMAGESCALE || iy >= IMAGESCALE)
		return false;
	if (//depth >= 0 && 
		(depthmap[ix][iy] == 0 || depth <= depthmap[ix][iy])
		)
	{
		//if (alpha >= 1)
		//{
		drawmap[ix][iy] = color;
		depthmap[ix][iy] = depth;// < 0 ? 0 : depth;
	//}
	//else
	//{
		//drawmap[ix][iy] = blendcor(drawmap[ix][iy], color, alpha, 1);
		//depthmap[ix][iy] = depth;// < 0 ? 0 : depth;
	//}
		return true;
	}

	return false;
}
inline bool pixeladd(int ix, int iy, int color)
{
	if (ix < 0 || iy < 0 || ix >= IMAGESCALE || iy >= IMAGESCALE)
		return false;

	drawmap[ix][iy] |= color;

	return true;
}
inline bool pixel(real x, real y, int color = 0xFFFFFFFF, real alpha = 1, real depth = 0)
{
	return pixel(int((x + 0.5) * IMAGESCALE), int((y + 0.5) * IMAGESCALE), color, alpha, depth);
}
inline bool pixel(const vector2& p, int color = 0xFFFFFFFF, real alpha = 1, real depth = 0)
{
	return pixel(int((p.x + 0.5) * IMAGESCALE + 0.5), int((p.y + 0.5) * IMAGESCALE + 0.5), color, alpha, depth);
}
inline bool pixel(const vector3& p, int color = 0xFFFFFFFF, real alpha = 1.0)
{
	vec2 p2d;
	real depth;
	viewprj(p, p2d, depth);
	return pixel(p2d, color, alpha, depth);
}
// 抗锯齿算法
inline bool pixelAA(real x, real y, int color = 0xFFFFFFFF, real depth = 0)
{
	x = x * IMAGESCALE;
	y = y * IMAGESCALE;
	int ix = int(x);
	int iy = int(y);
	real fx = x - ix;
	real fy = y - iy;

	if (ix < 0 || iy < 0 || ix >= IMAGESCALE - 1 || iy >= IMAGESCALE - 1)
		return false;
	real& rdepthval = depthmap[int(x + 0.5)][int(y + 0.5)];
	if (depth >= 0 &&
		(rdepthval == 0 || depth <= rdepthval)
		)
	{
		drawmap[ix][iy] = blendcor(drawmap[ix][iy], color, (1 - fx) * (1 - fy));
		drawmap[ix + 1][iy] = blendcor(drawmap[ix + 1][iy], color, (fx) * (1 - fy));
		drawmap[ix][iy + 1] = blendcor(drawmap[ix][iy + 1], color, (1 - fx) * (fy));
		drawmap[ix + 1][iy + 1] = blendcor(drawmap[ix + 1][iy + 1], color, (fx) * (fy));

		drawmap[int(x + 0.5)][int(y + 0.5)] = color;
		rdepthval = depth < 0 ? 0 : depth;
		return true;
	}
	return false;
}
inline bool pixelAA(const vector2& p, int color, real depth = 0)
{
	return pixelAA(p.x, p.y, color, depth);
}
inline bool pixelAA(const vector3& p, int color)
{
	vec2 p2d;
	real depth;
	viewprj(p, p2d, depth);
	return pixelAA(p2d, color, depth);
}

// point
inline void pointi(int x0, int y0, int size, int color, real alpha = 1, real depth = 0)
{
	if (size == 0)
		return;

	if (!pixel(x0, y0, color, 1, depth))
		return;

	if (size == 1)
		return;

	int halfsize = size / 2;
	int x1 = (x0 - halfsize); x1 < 0 ? x1 = 0 : 0;
	int y1 = (y0 - halfsize); y1 < 0 ? y1 = 0 : 0;
	int x2 = (x0 + halfsize); x2 >= IMAGESCALE ? x2 = IMAGESCALE - 1 : 0;
	int y2 = (y0 + halfsize); y2 >= IMAGESCALE ? y2 = IMAGESCALE - 1 : 0;

	for (int i = x1; i < x2; i++)
		for (int j = y1; j < y2; j++)
		{
			real alpha1 = 1;// blend2d(0.5, 1, real(i) / IMAGESCALE, real(j) / IMAGESCALE);

			real sqrdis = (i - x0) * (i - x0) + (j - y0) * (j - y0);
			real dhalfsize = halfsize * halfsize;// blend(0, halfsize * halfsize, alpha1, alpha1 * 4);
			real ds = (1 - sqrdis / dhalfsize);
			if (ds > 0)
			{
				if (depthmap[i][j] == 0 || depth <= depthmap[i][j])
					drawmap[i][j] = blendcor(drawmap[i][j], color, alpha);
			}
		}
}
inline void point(real x0, real y0, int size, int color, real alpha = 1, real depth = 0)
{
	pointi(((x0 + 0.5) * IMAGESCALE), ((y0 + 0.5) * IMAGESCALE), (size), color, alpha, depth);
}
inline void point(const pnt& p, int size = 4, int color = 0xFFFFFFFF, real alpha = 1, real depth = 0)
{
	pointi(p.x, p.y, size, color, alpha, depth);
}
inline void pointi(const pnt& p, int size = 4, int color = 0xFFFFFFFF, real alpha = 1, real depth = 0)
{
	pointi(p.x, p.y, size, color, alpha, depth);
}
inline void pointi(const vector2& p, int size, int color, real alpha = 1, real depth = 0)
{
	pointi(p.x, p.y, size, color, alpha, depth);
}
inline void point(const vector2& p, int size, int color, real alpha = 1, real depth = 0)
{
	point(p.x, p.y, size, color, alpha, depth);
}
inline void point(const vector3& p, int size, int color, real alpha = 1)
{
	vec2 p2d;
	real depth;
	viewprj(p, p2d, depth);
	point(p2d, size, color, alpha, depth);
}

inline void pointi_sharp(int x0, int y0, int size, int color, real depth = 0)
{
	if (size == 0)
		return;

	if (!pixel(x0, y0, color, 1, depth))
		return;

	if (size == 1)
		return;

	int halfsize = size / 2;
	int x1 = (x0 - halfsize); x1 < 0 ? x1 = 0 : 0;
	int y1 = (y0 - halfsize); y1 < 0 ? y1 = 0 : 0;
	int x2 = (x0 + halfsize); x2 >= IMAGESCALE ? x2 = IMAGESCALE - 1 : 0;
	int y2 = (y0 + halfsize); y2 >= IMAGESCALE ? y2 = IMAGESCALE - 1 : 0;

	for (int i = x1; i < x2; i++)
		for (int j = y1; j < y2; j++)
		{
			real sqrdis = (i - x0) * (i - x0) + (j - y0) * (j - y0);
			real ds = (1 - sqrdis / (halfsize * halfsize));
			if (ds > 0)
			{
				if (depthmap[i][j] == 0 || depth <= depthmap[i][j])
				{
					drawmap[i][j] = color;// blendcor(drawmap[i][j], color, ds, 0.25 + (1 - ds) * 2);
					depthmap[i][j] = depth;
				}
			}
		}
}
inline void point_sharp(real x0, real y0, int size, int color, real depth = 0)
{
	pointi_sharp(int(x0 * IMAGESCALE + 0.5), int(y0 * IMAGESCALE + 0.5), size, color, depth);
}
inline void point_sharp(const vector2& p, int size, int color, real depth = 0)
{
	point_sharp(p.x, p.y, size, color, depth);
}
inline void point_sharp(const vector3& p, int size, int color)
{
	vec2 p2d;
	real depth;
	viewprj(p, p2d, depth);
	point_sharp(p2d, size, color, depth);
}
// -------------------------------------------------
// viewprj
// -------------------------------------------------
inline void viewprj(const vector3& rp, vector2& p2d, real& depth)
{
	//vec p = rp;
	//p -= CAM_POS;
	//p.rot(PI * 0.25, vec::UY);
	//p.rot(PI * 0.15, vec::UX);
	//p += CAM_POS;
	//p.z += MAXZ / 3;

	real z = rp.z + 10;
	depth = z;
	real vpf = blend(1, 0.01, z / 100.0f);
	p2d.x = rp.x * vpf;
	p2d.y = rp.y * vpf;
}
inline real getviewdis(const vec& p1, const vec& p2)
{
	real depth;
	vec2 pp1, pp2;
	viewprj(p1, pp1, depth);
	viewprj(p2, pp2, depth);
	return (pp1 - pp2).len() * (IMAGESCALE);
}
// line
void line3d(vec p1, vec p2, int cor = 0xFFFFFFFF)
{
	int len = getviewdis(p1, p2);
	for (int i = 0; i <= len; i++)
	{
		real ai = i / real(len);
		pixel(blend(p1, p2, ai), cor);
	}
}
void line2d(vec2 p1, vec2 p2, int cor = 0xFFFFFFFF)
{
	int len = (p2 - p1).len() * IMAGESCALE;
	for (int i = 0; i <= len; i++)
	{
		real ai = i / real(len);
		pixel(blend(p1, p2, ai), cor);
	}
}
void line2di(const pnt& p1, const pnt& p2, int size, int cor = 0xFFFFFFFF)
{
	int len = (p2 - p1).len();
	for (int i = 0; i <= len; i++)
	{
		real ai = i / real(len);
		pointi(blend(p1.x, p2.x, ai), blend(p1.y, p2.y, ai), size, cor);
	}
}
void circle(vec2 o, real r, int cor = 0xFFFFFFFF)
{
	int len = (r * 2 * PI) * IMAGESCALE;
	for (int i = 0; i <= len; i++)
	{
		real ai = i / real(len);
		real ang = ai * PI * 2;
		pixel(o + vec2(r * cos(ang), r * sin(ang)), cor);
	}
}
void drawrect(const pnt& p1, const pnt& p2, int cor = 0xFFFFFFFF)
{
	for (int i = p1.x; i <= p2.x; i++)
	for (int j = p1.y; j <= p2.y; j++)
	{
		pixel(i, j, cor);
	}
}
void drawrect(int x1, int y1, int x2, int y2, int cor = 0xFFFFFFFF)
{
	for (int i = x1; i <= x2; i++)
	{
		pixel(i, y1, cor);
		pixel(i, y2, cor);
	}
	for (int j = y1; j <= y2; j++)
	{
		pixel(x1, j, cor);
		pixel(x2, j, cor);
	}
}
// **********************************************************************
// 几何体造型基础
// **********************************************************************
struct vertex
{
	vec3 p;
	vec3 n;
	vec2 uv;

	int cor = 0;
	int ind = -1;

	int tag = 0;

	bool bread = false;

	// connect to next
	int next = -1; // -1 没有链接 -2 下一点 >= 0 连接位置

	// deform
	real deformwgt = 0;
	vec3 bone;

	vertex()
	{
		cor = 0;
		ind = -1;
	}
	vertex(const vec& _p)
	{
		p = _p;
		ind = -1;
	}
	vertex(real x, real y, real z)
	{
		p.x = x, p.y = y, p.z = z;
		ind = -1;
	}
	vertex(const vec& _p, const vec& _n)
	{
		p = _p;
		n = _n;
		ind = -1;
	}
	vertex(const vec& _p, int _ind)
	{
		p = _p;
		ind = _ind;
	}
	vertex(const vec& _p, const vec2& _uv)
	{
		p = _p;
		uv = _uv;
		ind = -1;
	}
	operator const vec& ()
	{
		return p;
	}
	bool operator==(const vertex& v) const
	{
		return p == v.p;
	}
	vec operator + (const vertex& v) const
	{
		return p + v.p;
	}
	vec operator + (const vec3& _p) const
	{
		return p + _p;
	}
	vec operator - (const vertex& v) const
	{
		return p - v.p;
	}
	vec operator - (const vec3& _p) const
	{
		return p - _p;
	}
	vertex operator * (real s) const
	{
		return vertex(p * s, ind);
	}
};
struct triangle
{
	int	vertexIndex[3];
	int	normalIndex[3]; // 读入数据时使用

	bool bread = false;
};
struct submesh
{
	std::vector<vertex> vertices;
	std::vector<int> cors;
	std::vector<triangle> tris;

	// 后边两项在读入模型时才会使用
	std::vector<vector3> normals;
	std::vector<vector2> uvs;

	submesh() {}
	submesh(const submesh& sm)
	{
		PRINT("submesh copy:" << sm.vertices.size())
	}
	void operator = (const submesh& sm)
	{
		PRINT("[submesh=" << sm.vertices.size() << "]")
		vertices = sm.vertices;
		cors = sm.cors;
		tris = sm.tris;
		normals = sm.normals;
		uvs = sm.uvs;
	}
};

// -------------------------------------------------
inline void drawline(vec3 p1, vec3 p2, int  cor = 0xFFFFFFFF)
{
	for (int i = 0; i < 1000; i++)
	{
		vec3 p = blend(p1, p2, i / 1000.0);
		vec2 p2d;
		real depth;
		viewprj(p, p2d, depth);
		pixel(p2d, cor, depth);
	}
}
inline void drawline(crvec2 p1, crvec2 p2, int cor = 0xFFFFFFFF)
{
	for (int i = 0; i < 1000; i++)
	{
		vec2 p = blend(p1, p2, i / 1000.0);
		pixel(p, cor);
	}
}

inline void drawlinei(crvec2 p1, crvec2 p2, int cor = 0xFFFFFFFF)
{
	int len = (p1 - p2).len();
	for (int i = 0; i < len - 8; i++)
	{
		vec2 p = blend(p1, p2, i / real(len));
		pointi(p, 2, cor);
	}
}
// -------------------------------------------------
void drawtriangle(const vec& p1, const vec& p2, const vec& p3, int cor = 1)
{
	if (renderstate == 1)
	{
		static vec light(0.25, 1, -1);
		vec norm = (p2 - p1).cross(p3 - p1);
		norm = norm.normcopy();
		if (norm.sqrlen() == 0)
			return; // 无效三角形	

		int cor0 = rrnd(50, 80);//0x000000FF & cor;
		{
			int lum = blend(cor0, 255, norm.dot(light));
			cor = RGB(lum, lum, lum);
		}

		LOOP(i, len, getviewdis(p1, p2))
			vec p12 = blend(p1, p2, ai);
		LOOP(j, wid, getviewdis(p12, p3))
			vec p = blend(p12, p3, aj);
		pixelAA(p, cor);
		END
			END
	}
	//else if(renderstate == 2)
	{
		line3d(p1, p2, 0xFFFFFFFF);
		line3d(p2, p3, 0xFFFFFFFF);
		line3d(p3, p1, 0xFFFFFFFF);
	}
}
// -------------------------------------------------
void rendersubmesh(const submesh& sm)
{
	const std::vector<vertex>& vertices = sm.vertices;
	const std::vector<triangle>& tris = sm.tris;

	for (int i = 0; i < tris.size(); i++)
	{
		const triangle& tri = tris[i];
		//if(!tri.enable)
		//	continue;
		vec pos[3];
		for (int ii = 0; ii < 3; ii++)
		{
			int ind = tri.vertexIndex[ii];
			pos[ii] = vertices[tri.vertexIndex[ii]].p;
		}
		drawtriangle(pos[0], pos[2], pos[1]);
	}
}

// -------------------------------------------------
bool loadsubmesh3DS(const char* fileName, submesh& sm)
{
	FILE* file;
	if (0 != (fopen_s(&file, fileName, "r")))
	{
		printf("File can't be opened\n");
		return false;
	}

	while (1) {
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf_s(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		if (strcmp(lineHeader, "v") == 0)
		{
			vector3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			sm.vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			vector2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			sm.uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			vector3 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			sm.normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			triangle tri;
			int matches = fscanf_s(file, "%d//%d %d//%d %d//%d\n", &tri.vertexIndex[0], &tri.normalIndex[0], &tri.vertexIndex[1], &tri.normalIndex[1], &tri.vertexIndex[2], &tri.normalIndex[2]);
			if (matches != 6)
				//int matches = fscanf(file, "%d//%d//%d %d//%d//%d %d//%d//%d\n", &tri.vertexIndex[0], &tri.normalIndex[0], &tri.uvIndex[0], &tri.vertexIndex[1], &tri.normalIndex[1], &tri.uvIndex[1], &tri.vertexIndex[2], &tri.normalIndex[2], &tri.uvIndex[2]);
				//if (matches != 9)			
			{
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			//PRINT(tri.vertexIndex[2])
			sm.tris.push_back(tri);
		}
		else
		{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}

	fclose(file);

	return true;
}

// -------------------------------------------------
bool savesubmesh3DS(const char* fileName, submesh& sm)
{
	FILE* file;
	if (0 != (fopen_s(&file, fileName, "wb")))
	{
		printf("File can't be opened\n");
		return false;
	}
	std::stringstream ss;
	ss << "o Submesh\n";
	for (auto v : sm.vertices)
	{
		ss << "v " << v.p.x << " " << v.p.y << " " << v.p.z << "\n";
	}
	for (auto v : sm.vertices)
	{
		ss << "vn " << v.n.x << " " << v.n.y << " " << v.n.z << "\n";
	}
	ss << "s off\n";
	
	for (auto f : sm.tris)
	{
		ss << "f " 
			<< f.vertexIndex[0] + 1 << "//" << f.vertexIndex[0] + 1 << " "
			<< f.vertexIndex[1] + 1 << "//" << f.vertexIndex[1] + 1 << " "
			<< f.vertexIndex[2] + 1 << "//" << f.vertexIndex[2] + 1
			 << "\n";
	}

	fprintf(file, ss.str().c_str());

	fclose(file);

	return true;
}

// ----------------------------------------------------------------------
#define SMCATCH_START(wid)	submesh sm;	triangle tri1, tri2; int verIndex = 0;int lowerind[wid + 1];	
#define SMCATCH_ING(wid, s, i, j, pp) 	{const int step = s;	if ((i % step == 0 && j % step == 0)){if(i > 0 && j > 0){tri1.vertexIndex[2] = lowerind[j - step];tri1.vertexIndex[1] = lowerind[j];tri1.vertexIndex[0] = verIndex - 1;tri2.vertexIndex[2] = verIndex;tri2.vertexIndex[1] = verIndex - 1;	tri2.vertexIndex[0] = lowerind[j];sm.tris.push_back(tri1);sm.tris.push_back(tri2);}	sm.vertices.push_back(pp);lowerind[j - step] = verIndex - 1;if(j == wid) lowerind[j] = verIndex;verIndex ++;}}
#define SMCATCH_ING2(wid, s, i, j, pp) 	{const int step = s;	if ((i % step == 0 && j % step == 0)){if(i > 0 && j > 0){tri1.vertexIndex[0] = lowerind[j - step];tri1.vertexIndex[1] = lowerind[j];tri1.vertexIndex[2] = verIndex - 1;tri2.vertexIndex[0] = verIndex;tri2.vertexIndex[1] = verIndex - 1;	tri2.vertexIndex[2] = lowerind[j];sm.tris.push_back(tri1);sm.tris.push_back(tri2);}	sm.vertices.push_back(pp);lowerind[j - step] = verIndex - 1;if(j == wid) lowerind[j] = verIndex;verIndex ++;}}
#define SWITCHSM(sm)	gsubmesh = &sm
#define RESTORESM		gsubmesh = &gsubmesh0
#define SUBMESH			(*gsubmesh)

// **********************************************************************
// 三角形导出
// **********************************************************************
submesh gsubmesh0;	// 默认模型
submesh gimportsm;	// unity导入的模型
submesh* gsubmesh = &gsubmesh0;	

bool gcommonvertex = false;
bool gsearchcomvertex = false;
int gverindfindstart = 0;
bool gcalcnorm = false;

struct targetpos_t
{
	targetpos_t(crvec p)
	{
		pos = p;
	}
	targetpos_t(crvec p, crvec v)
	{
		pos = p;
		dir = v;
	}
	vec pos;
	vec dir;
};
std::vector<targetpos_t>	gtargets;

extern uint color;

void resetsm(submesh*sm = gsubmesh)
{
	sm->vertices.clear();
	sm->cors.clear();
	sm->tris.clear();

	// 后边两项在读入模型时才会使用
	sm->normals.clear();
	sm->uvs.clear();

	gtargets.clear();
}
inline void clearedgeind(VECLIST& e)
{
	for (int i = 0; i < e.size(); i++)
	{
		e[i].ind = -1;
	}
}
inline int getverind(vertex& v)
{
	if (gcommonvertex && v.ind != -1)
	{
		gsubmesh->cors[v.ind] = color;
		return v.ind;
	}

	if (gcommonvertex && gsearchcomvertex)
	{
		// 寻找同样的顶点
		for (int i = gsubmesh->vertices.size()-1; i >= gverindfindstart; i--)
		{
			if (gsubmesh->vertices[i] == v.p)
			{
				gsubmesh->cors[i] = color;
				v.ind = i;
				return i;
			}
		}
	}

	int ind = gsubmesh->vertices.size();
	v.ind = ind;
	gsubmesh->vertices.push_back(v);
	gsubmesh->cors.push_back(color);
	return ind;
}

inline void addtri2sm(vertex& p1, vertex& p2, vertex& p3)
{
	//if (*gsubmesh->vertices.size() >= 60000)
	//{
		//LOGMSG("添加失败 三角形数量超过60000!")
		//
	//	return;
	//}

	//vec norm = (p2 - p1).cross(p3 - p1);
	//real llen = norm.sqrlen();
	//if (llen > 1e-18 && llen < 1e8)
	{
		triangle tri;
		tri.vertexIndex[0] = getverind(p1);
		tri.vertexIndex[1] = getverind(p2);
		tri.vertexIndex[2] = getverind(p3);

		gsubmesh->tris.push_back(tri);
	}
}
inline int getverind0(const vertex& v)
{
	gsubmesh->vertices.push_back(v.p);
	gsubmesh->cors.push_back(color);
	return gsubmesh->vertices.size() - 1;
}
inline void addtri2sm0(const vertex& p1, const vertex& p2, const vertex& p3)
{
	//if (gsubmesh->vertices.size() >= 60000)
	//{
	//	//LOGMSG("添加失败 三角形数量超过60000!")
	//	//
	//	return;
	//}

	vec norm = (p2 - p1).cross(p3 - p1);
	real llen = norm.sqrlen();
	if (llen > 1e-18 && llen < 1e8)
	{
		triangle tri;
		tri.vertexIndex[0] = getverind0(p1);
		tri.vertexIndex[1] = getverind0(p2);
		tri.vertexIndex[2] = getverind0(p3);

		gsubmesh->tris.push_back(tri);
	}
}
inline void addtri2smi(int t1, int t2, int t3)
{
	if (t1 >= gsubmesh->vertices.size() || t2 >= gsubmesh->vertices.size() || t3 >= gsubmesh->vertices.size())
		return;

	if (gsubmesh->vertices.size() >= 60000)
	{
		//LOGMSG("添加失败 三角形数量超过60000!")
		//
		return;
	}

	triangle tri;
	tri.vertexIndex[0] = t1;
	tri.vertexIndex[1] = t2;
	tri.vertexIndex[2] = t3;

	gsubmesh->tris.push_back(tri);
}
inline void exportsm()
{
	//exportdat(*gsubmesh);
}
inline void rendersubmesh()
{
	rendersubmesh(*gsubmesh);
}
// ------------------------------------------------
// READ TEMPLATE
inline std::string readTemplate()
{
	std::stringstream templateString;
	std::ifstream f("output/model.cs");
	if (f)
	{
		std::copy(std::istreambuf_iterator<char>(f),
			std::istreambuf_iterator<char>(),
			std::ostreambuf_iterator<char>(templateString));
		f.close();
	}
	return templateString.str();
}

// -------------------------------------------------
// 法线逻辑
// -------------------------------------------------
inline void uniformnorm(const vec& p, const vec& n)
{
	for (int i = 0; i < gsubmesh->vertices.size(); i++)
	{
		if (gsubmesh->vertices[i] == p)
		{
			gsubmesh->vertices[i].n = n;
		}
	}
}
inline void closeedgenorm(VECLIST& e)
{
	e[e.size() - 1].n = e[0].n;
}
void calcroundnorm(VECLIST& e, crvec o)
{
	for (int i = 0; i < e.size(); i++)
	{
		e[i].n = (e[i].p - o).normcopy();
	}
}

#define PMDLL
#ifdef PMDLL
// **********************************************************************
// Export
// **********************************************************************
extern submesh* gsubmesh;
extern void resetsm(submesh* sm);
extern void exportsm();

extern void drawScene(const char* script);

int drawround = 0;

vec gettrinorm(const vec& p1, const vec& p2, const vec& p3)
{
	return (p2 - p1).cross((p3 - p1)).normcopy();
}

// ---------------------------------------------------------
// 三角形雕塑
// ---------------------------------------------------------
vec getpointnorm(vec* v, int* t, int tcnt, vec p1)
{
	VECLIST normlist;
	for (int i = 0; i < tcnt; i += 3)
	{// 每一个三角形
		vec& _p1 = v[t[i]];
		vec& _p2 = v[t[i] + 1];
		vec& _p3 = v[t[i] + 2];
		{// 寻找位置相同的点并移动它
			if ((_p1 == p1) || (_p2 == p1) || (_p3 == p1))
			{
				normlist.push_back((_p2 - _p1).cross(_p3 - _p1).normcopy());
			}
		}
	}
	vec norm = vec::ZERO;
	for (int i = 0; i < normlist.size(); i++)
	{
		norm += normlist[i];
	}
	norm /= normlist.size();
	return norm.normcopy();
}

extern bool dolua(const std::string& luascript);
extern "C"
{
	// ---------------------------------------------------------
	// 独立绘制场景
	// ---------------------------------------------------------
	void EXPORT_API drawscene(const char* script)
	{
		renderstate = 1;
		drawScene(script);

	}
	// ---------------------------------------------------------
	// VB
	// ---------------------------------------------------------
	int EXPORT_API _stdcall vbmain()
	{
		renderstate = 1;
		char imgname[512];
		srand(18858146);

	//	init();

		{
			if (renderstate > 0)
			{
				// clear
				memset(drawmap0, 0, sizeof(drawmap0));
				memset(depthmap0, 0, sizeof(depthmap0));

				//printf("generating image ... ");
			}

		}
		return 0;
	}


	// ---------------------------------------------------------
	// 点列表
	// ---------------------------------------------------------
	std::vector<VECLIST> estack;
	int verliststackpos = 0;
	int EXPORT_API getStackSize()
	{
		return estack.size();
	}
	bool EXPORT_API getStackDat(int* dat)
	{
		for (int i = 0; i < estack.size(); i++)
		{
			dat[i] = estack[i].size();
		}
		return true;
	}
	int EXPORT_API getStackPos()
	{
		return verliststackpos;
	}
	bool EXPORT_API setStackPos(int pos)
	{
		verliststackpos = pos;
		return true;
	}
	int EXPORT_API getVerListSize()
	{
		if (estack.empty())
			return 0;

		if (verliststackpos >= estack.size())
			return 0;

		return estack[verliststackpos].size();
	}
	bool EXPORT_API getVerList(vec* v)
	{
		if (estack.empty())
			return false;

		if (verliststackpos >= estack.size())
			return false;

		for (int i = 0; i < estack[verliststackpos].size(); i++)
		{
			v[i] = estack[verliststackpos][i].p;
		}
		return true;
	}
	bool EXPORT_API getStack(void** pestack)
	{	
		*pestack = &estack;
		return true;
	}

	// ---------------------------------------------------------
	// 图片
	// ---------------------------------------------------------
	bool EXPORT_API getImage(char* buf)
	{
		memcpy(buf, drawmap, sizeof(drawmap0));
		return true;
	}
	// ---------------------------------------------------------
	// rect
	// ---------------------------------------------------------
	std::vector<rect_t> rectlist;
	int EXPORT_API getRectSize()
	{
		return rectlist.size();
	}
	bool EXPORT_API getRectDat(int* dat)
	{
		for (int i = 0; i < rectlist.size(); i++)
		{
			dat[4 * i]	 = rectlist[i].x;
			dat[4 * i + 1] = rectlist[i].y;
			dat[4 * i + 2] = rectlist[i].width;
			dat[4 * i + 3] = rectlist[i].height;
		}
		return true;
	}
	// ---------------------------------------------------------
	// vec3
	// ---------------------------------------------------------
	std::vector<vec3> vec3list;
	int EXPORT_API getVec3Size()
	{
		return vec3list.size();
	}
	bool EXPORT_API getVec3Dat(float* dat)
	{
		for (int i = 0; i < vec3list.size(); i ++)
		{
			dat[3 * i] = vec3list[i].x;
			dat[3 * i + 1] = vec3list[i].y;
			dat[3 * i + 2] = vec3list[i].z;
		}
		return true;
	}
	// ---------------------------------------------------------
	// int
	// ---------------------------------------------------------
	std::vector<int> intlist;
	int EXPORT_API getIntSize()
	{
		return intlist.size();
	}
	bool EXPORT_API getIntDat(int* dat)
	{
		for (int i = 0; i < intlist.size(); i ++)
		{
			dat[i] = intlist[i];
		}
		return true;
	}
	// ---------------------------------------------------------
	// real
	// ---------------------------------------------------------
	std::vector<real> reallist;
	int EXPORT_API getRealSize()
	{
		return reallist.size();
	}
	bool EXPORT_API getRealDat(float* dat)
	{
		for (int i = 0; i < reallist.size(); i++)
		{
			dat[i] = reallist[i];
		}
		return true;
	}
	// ---------------------------------------------------------
	// 反向法线
	// ---------------------------------------------------------
	void EXPORT_API invnorm(int* t, int tcnt)
	{
		for (int i = 0; i < tcnt; i += 3)
		{
			int& p1 = t[i];
			int& p2 = t[i + 1];
			int& p3 = t[i + 2];
			int t = p2;
			p2 = p3;
			p3 = t;
		}
	}

	// ---------------------------------------------------------
	// 设置当前方位
	// ---------------------------------------------------------
	bool gIsSetDxDy = 0;
	vec gDX, gDY;
	void EXPORT_API setdxdy(vec dx, vec dy)
	{
		gIsSetDxDy = 1;
		gDX = dx;
		gDY = dy;
	}

	// ---------------------------------------------------------
	// 导出几何数据
	// ---------------------------------------------------------
	int EXPORT_API getVcnt()
	{
		return gsubmesh->vertices.size();
	}
	bool EXPORT_API getVdata(vec* v)
	{
		for (int i = 0; i < gsubmesh->vertices.size(); i++)
		{
			v[i] = gsubmesh->vertices[i];
		}
		return true;
	}
	bool EXPORT_API setVdata(vec* v, int cnt)
	{
		for (int i = 0; i < cnt; i++)
		{
			gimportsm.vertices.push_back(v[i]);
		}
		return true;
	}
	bool EXPORT_API getNorm(vec* n)
	{
		for (int i = 0; i < gsubmesh->tris.size(); i++)
		{
			int t1 = gsubmesh->tris[i].vertexIndex[0];
			int t2 = gsubmesh->tris[i].vertexIndex[1];
			int t3 = gsubmesh->tris[i].vertexIndex[2];

			vertex& p1 = gsubmesh->vertices[t1];
			vertex& p2 = gsubmesh->vertices[t2];
			vertex& p3 = gsubmesh->vertices[t3];

			vec nm = (p2.p - p1.p).cross(p3.p - p1.p);
			nm.norm();

			n[t1] += nm;
			n[t2] += nm;
			n[t3] += nm;
		}
		for (int i = 0; i < gsubmesh->vertices.size(); i++)
		{
			if (gsubmesh->vertices[i].n != vec::ZERO)
				n[i] = gsubmesh->vertices[i].n;
			else
				n[i].norm();
		}
		return true;
	}
	bool EXPORT_API getCdata(int* c)
	{
		for (int i = 0; i < gsubmesh->vertices.size(); i++)
		{
			c[i] = gsubmesh->cors[i];
		}
		return true;
	}
	bool EXPORT_API getUVdata(vec2* uv)
	{
		for (int i = 0; i < gsubmesh->vertices.size(); i++)
		{
			uv[i] = gsubmesh->vertices[i].uv;
		}
		return true;
	}
	int EXPORT_API getTcnt()
	{
		return gsubmesh->tris.size() * 3;
	}
	bool EXPORT_API getTdata(int* t)
	{
		int cnt = 0;
		for (int i = 0; i < gsubmesh->tris.size(); i++)
		{
			t[cnt++] = gsubmesh->tris[i].vertexIndex[0];
			t[cnt++] = gsubmesh->tris[i].vertexIndex[1];
			t[cnt++] = gsubmesh->tris[i].vertexIndex[2];
		}
		return true;
	}
	bool EXPORT_API setTdata(int* t, int cnt)
	{
		for (int i = 0; i < cnt; i+=3)
		{
			triangle tri;
			tri.vertexIndex[0] = t[i];
			tri.vertexIndex[1] = t[i+1];
			tri.vertexIndex[2] = t[i+2];
			gimportsm.tris.push_back(tri);
		}
		return true;
	}

	// ---------------------------------------------------------
	// 导出挂点信息
	// ---------------------------------------------------------
	int EXPORT_API gettargetcnt()
	{
		return gtargets.size();
	}
	bool EXPORT_API gettargetpos(vec* p)
	{
		for (int i = 0; i < gtargets.size(); i++)
		{
			p[i] = gtargets[i].pos * EXPORT_SCALE;
		}
		return true;
	}
	bool EXPORT_API gettargetdir(vec* p)
	{
		for (int i = 0; i < gtargets.size(); i++)
		{
			p[i] = gtargets[i].dir;
		}
		return true;
	}
	// ---------------------------------------------------------
	// 内存操作
	// ---------------------------------------------------------
	void EXPORT_API converVecs2Bytes(vec* src, unsigned char* des, int sz)
	{
		memcpy(des, src, sz);
	}
	void EXPORT_API converInts2Bytes(int* src, unsigned char* des, int sz)
	{
		memcpy(des, src, sz);
	}
	void EXPORT_API converCors2Bytes(unsigned int* src, unsigned char* des, int sz)
	{
		memcpy(des, src, sz);
	}

	// ---------------------------------------------------------
	// 色彩
	// ---------------------------------------------------------
	unsigned int EXPORT_API HSL2Color32(real h, real l, real s)
	{
		COLOR_HSL hsl;
		hsl.hue = h * 255;
		hsl.saturation = s * 255;
		hsl.luminance = l * 255;

		return  HSLtoRGB(&hsl);
	}

	// ---------------------------------------------------------
	// 屏幕尺寸
	// ---------------------------------------------------------
	void EXPORT_API screensize(real w, real h)
	{
		SCREENWID = w;
		SCREENHGT = h;
	}
}
// **********************************************************************
// read/write BMP
// **********************************************************************
int bmp_read(unsigned char* image, const char* filename)
{
	FILE* fp;
	unsigned char header[54];

	if (0 != (fopen_s(&fp, filename, "rb")))
	{
		PRINT("file not exist!\n");
		return -1;
	}

	fread(header, sizeof(unsigned char), 54, fp);
	fread(image, sizeof(unsigned char), (size_t)(long)2048 * 2048 * 3, fp);

	fclose(fp);
	return 0;
}
int bmp_write(unsigned char* image, int xSize, int ySize, const char* fileName)
{
	unsigned char header[54] = {
		0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0,
		54, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0
	};

	// sizeof(long) = 4  
	long fileSize = (long)xSize * (long)ySize * 3 + 54;     // image array size + 54  
	long temp = 0;                      // temp for byte convertion  
	long width = xSize;                 // image width  
	long height = ySize;                // image height 
	int i = 0;                          // loop variable  

	FILE* fp;

	for (i = 0; i != 4; ++i)                // write fileSize from byte2 ~ byte 6  
	{
		header[2 + i] = (unsigned char)(fileSize >> (8 * i)) & 0x000000ff;
	}
	for (i = 0; i != 4; ++i)                // write width from byte18 ~ byte 22  
	{
		header[18 + i] = (unsigned char)(width >> (8 * i)) & 0x000000ff;
	}
	for (i = 0; i != 4; ++i)                // write height from byte22 ~ byte 26  
	{
		header[22 + i] = (unsigned char)(height >> (8 * i)) & 0x000000ff;
	}


	if (fopen_s(&fp, fileName, "wb") != 0)
	{
		PRINT("bmp_write openfile " << fileName << " failed!\n")
			return -1;
	}

	fwrite(header, sizeof(unsigned char), 54, fp);                              // write header  
	fwrite(image, sizeof(unsigned char), xSize * ySize * 3, fp);       // write image array  

	fclose(fp);

	return 0;
}

// **********************************************************************
// DRAW
// **********************************************************************
extern int servermain(int num);

inline std::string getExePath(void)
{
	char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	(strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串  
	return std::string(szFilePath);
}

void drawScene(const char* script)
{
	PRINT("======= start http-server: localhost:8080");
	std::thread serverthread = std::thread{ servermain, 100 };

	char imgname[512];
	srand(18858146);

	SetCurrentDirectoryA(getExePath().c_str());

	for (int i = 1; i <= 1; i++)
	{
		printf("\n@@@@@@@@@@@@@@@@@@@@@@ ROUND(%d/10) @@@@@@@@@@@@@@@@@@@@@@\n", i);
		drawround = i;
		if (renderstate > 0)
		{
			// clear
			memset(drawmap0, 0, sizeof(drawmap0));
			memset(depthmap0, 0, sizeof(depthmap0));

			//printf("generating image ... ");
		}

		//PRINT("\n");

		if (renderstate > 0)
		{
			//printf("\ndone!\n");

			printf("\nsaving image ... ");
			sprintf_s(imgname, "image_%d.bmp", i);
			//sprintf_s(imgname, "image.bmp");

			static unsigned char buf[IMAGESCALE * IMAGESCALE * 3];
			for (int ii = 0; ii < IMAGESCALE; ii++)
				for (int jj = 0; jj < IMAGESCALE; jj++)
				{
					buf[3 * (jj * IMAGESCALE + ii) + 0] = GetBValue(drawmap[ii][jj]);
					buf[3 * (jj * IMAGESCALE + ii) + 1] = GetGValue(drawmap[ii][jj]);
					buf[3 * (jj * IMAGESCALE + ii) + 2] = GetRValue(drawmap[ii][jj]);
				}
			bmp_write(buf, IMAGESCALE, IMAGESCALE, imgname);

			printf("\ndone!\n");

			system(imgname);
		}
		printf("\nany key to continue ...\n");
		getchar();
	}
}
#endif