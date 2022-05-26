/**************************************************************************
*				元素
*				用于运算
**************************************************************************/
#ifdef ELEMENT
#undef ELEMENT
#endif
#define ELEMENT		ScePHG::var_t
#define OBJ			ELEMENT
#define ENT			ELEMENT

#ifdef STRING2VAR
#undef STRING2VAR
#endif
#define STRING2VAR(str)	ScePHG::var_t(str)
// ------------------------------------------
//#include "entity.hpp"
// ------------------------------------------
// var
struct var_t
{
	std::function<var_t(var_t& a, var_t& b)> fun_add = 0;	// 资源加法函数
	std::function<void(const var_t& v)> fun_set = 0;		// 资源set函数
	union {
		int ival = 0;
		real fval;
	};
	string sval;
	int resid = -1;
	int type = 1; // 1 -int, 2 -real, 3 -string

	var_t() { }
	var_t(int _val) {
		type = 1; ival = _val;
	}
	var_t(real _val) {
		type = 2; fval = _val;
	}
	var_t(bool _val) {
		type = 1; ival = (int)_val;
	}
	var_t(const char* _val) {
		type = 3; sval = _val;
		//PRINTV(sval);
	}
	var_t(const var_t& v)
	{
		//PRINT("var_t copy " << v.type);
		(*this) = v;
	}
	void operator = (int v)
	{
		type = 1; ival = v; resid = -1;
	}
	void operator = (real v)
	{
		type = 2; fval = v; resid = -1;
	}
	void operator = (const var_t& v)
	{
		//PRINT("var_t ="  << v.type << "," << v.fval);
		type = v.type;
		if (type == 3)
			sval = v.sval;
		else if(type == 2)
			fval = v.fval;
		else if(type == 1)
			ival = v.ival;
		else if(fun_set)
		{
			fun_set(v);
		}
		resid = v.resid;
	}
	bool operator == (int v) const
	{
		return type == 1 && ival == v;
	}
	bool operator != (int v) const
	{
		return type != 1 || ival != v;
	}
	bool operator == (const var_t& v) const
	{
		return  (type == v.type && ((type == 1 && ival == v.ival) || (type == 2 && fval == v.fval))) ||
			(type != v.type && float(*this) == float(v));
	}
	bool operator != (const var_t& v) const
	{
		return !(*this == v);
	}
	operator int() const
	{
		//PRINT("var_t::int " << ival)
		return ival;
	}
	operator float() const
	{
		//PRINT("var_t::int " << ival)
		if (type == 1)
			float(ival);
		if (type == 2)
			return fval;
		if (type == 3)
			return atof(sval.c_str());
		return 0.0f;
	}
	inline string tostr() const
	{
		//PRINT(type << ":" << sval);
		if (type == 1)
			return to_string(ival);
		if (type == 2)
			return to_string(fval);
		if (type == 3)
			return sval;
		return "";
	}
	var_t operator + (var_t& v) const
	{
		var_t ret;
		
		if (type == 1 && v.type == 1) 
			ret.ival = ival + v.ival; 
		else if (type == 3 || v.type == 3)
		{
			ret.type = 3;
			ret.sval = tostr() + v.tostr();
			//PRINTV(ret.sval)
		}
		else{
			ret.type = 2;
			ret.fval = float(*this) + float(v);
		}
		return ret;
	}
	void operator += (var_t& v)
	{
		ASSERT(type != 3 && v.type != 3);
		if (type == 1 && v.type == 1)
			ival += v.ival;
		else if(type == 2){
			fval += float(v);
		}
	}
	var_t operator - (var_t& v) const
	{
		ASSERT(type != 3 && v.type != 3);
		var_t ret;
		
		if (type == 1 && v.type == 1)
			ret.ival = ival - v.ival;
		else{
			ret.type = 2;
			ret.fval = float(*this) - float(v);
		}
		return ret;
	}
	void operator -= (var_t& v)
	{
		ASSERT(type != 3 && v.type != 3);
		if (type == 1 && v.type == 1) {
			ival -= v.ival;
		}
		else if (type == 2) {
			fval -= v.fval;
		}
	}
	var_t operator - () const
	{
		ASSERT(type != 3);
		var_t ret;
		ret.type = type;
		if (type == 1) {
			ret.ival = -ival;
		}
		else if (type == 2) {
			ret.fval = -fval;
		}
		return ret;
	}
	var_t operator * (var_t& v) const
	{
		ASSERT(type != 3 && v.type != 3);
		var_t ret;
		if (type == 1 && v.type == 1){
			ret.ival = ival * v.ival;
		}
		else{
			ret.type = 2;
			ret.fval = float(*this) * float(v);
		}
		return ret;
	}
	var_t operator / (var_t& v) const
	{
		ASSERT(type != 3 && v.type != 3);
		var_t ret;
		if (type == 2 || v.type == 2) {
			ret.type = 2;
			ret.fval = float(*this) / float(v);
		}
		else
		{
			ret.ival = ival / v.ival;
		}
		return ret;
	}
	bool operator > (const var_t& v) const
	{
		ASSERT(type != 3 && v.type != 3);
		if (type == 2 || v.type == 2) {
			return float(*this) > float(v);
		}
		else
		{
			return ival > v.ival;
		}
	}
	bool operator < (const var_t& v) const
	{
		ASSERT(type != 3 && v.type != 3);
		if (type == 2 || v.type == 2) {
			return float(*this) < float(v);
		}
		else
		{
			return ival < v.ival;
		}
	}
};

inline void _PHGPRINT(const std::string& pre, const var& v)
{
	if (v.type == 1)
		PRINT(pre << v.ival)
	else if (v.type == 2)
		PRINT(pre << v.fval)
	else if (v.type == 3)
		PRINT(pre << v.sval)
	else
		PRINT(pre << "unkown type")
}

// ------------------------------------------
#define USE_STRING
#include "phg.hpp"
#undef USE_STRING
// ------------------------------------------
// 运算
using fun_calc_t = std::function<var(code& cd, char o, int args)>;
fun_calc_t fun_calc = 0;
static var _act(code& cd, int args)
{
	opr o = cd.oprstack.pop();

	if (fun_calc)
	{
		if (var ret = fun_calc(cd, o, args); ret != 0)
			return ret;
	}

	PRINT("calc:" << o << "(" << args << ")")

		switch (o) {
		case '+': {
			if (args > 1) {
				var& b = PHG_VALSTACK(1);
				var& a = PHG_VALSTACK(2);
				var ret = a + b;
				PHG_VALPOP(2);
				return ret;
			}
			else {
				return cd.valstack.pop();
			}
		}
		case '-': {
			if (args > 1) {
				var& b = PHG_VALSTACK(1);
				var& a = PHG_VALSTACK(2);
				var ret = a - b;
				PHG_VALPOP(2);
				return ret;
			}
			else {
				return -cd.valstack.pop();
			}
		}
		case '*': {
			if (args > 1) {
				var& b = PHG_VALSTACK(1);
				var& a = PHG_VALSTACK(2);
				var ret = a * b;
				PHG_VALPOP(2);
				return ret;
			}
			else {
				return cd.valstack.pop();
			}
		}
		case '/': {
			if (args > 1) {
				var& b = PHG_VALSTACK(1);
				var& a = PHG_VALSTACK(2);
				var ret = a / b;
				PHG_VALPOP(2);
				return ret;
			}
			else {
				return cd.valstack.pop();
			}
		}
		case '=': {
			var& b = PHG_VALSTACK(1);
			var& a = PHG_VALSTACK(2);
			var ret = var(int(a == b));
			PHG_VALPOP(2);
			return ret;
		}
		case '>': {
			var& b = PHG_VALSTACK(1);
			var& a = PHG_VALSTACK(2);
			var ret = a > b;
			PHG_VALPOP(2);
			return ret;
		}
		case '<': {
			var& b = PHG_VALSTACK(1);
			var& a = PHG_VALSTACK(2);
			var ret = a < b;
			PHG_VALPOP(2);
			return ret;
		}
		case '&': {
			var& b = PHG_VALSTACK(1);
			var& a = PHG_VALSTACK(2);
			var ret = int(a) && int(b);
			PHG_VALPOP(2);
			return ret;
		}
		case '|': {
			var& b = PHG_VALSTACK(1);
			var& a = PHG_VALSTACK(2);
			var ret = int(a) || int(b);
			PHG_VALPOP(2);
			return ret;
		}
		case '!': {
			if (args > 1) {
				var& b = PHG_VALSTACK(1);
				var& a = PHG_VALSTACK(2);
				var ret = !(a == b);
				PHG_VALPOP(2);
				return ret;
			}
			else {
				var a = cd.valstack.pop();
				return !int(a);
			}
		}
		default: {}
		}
	return INVALIDVAR;
}
