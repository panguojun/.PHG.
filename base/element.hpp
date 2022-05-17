/**************************************************************************
*							元素
*						  用于运算
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
	// 资源加法函数
	std::function<var_t(var_t& a, var_t& b)> fun_add = 0;

	union {
		int ival = 0;
		real fval;
	};
	int resid = -1;
	int type = 1; // 1 -int, 2 -real, others

	var_t() { }
	var_t(int _val) {
		type = 1; ival = _val; resid = -1;
	}
	var_t(real _val) {
		type = 2; fval = _val; resid = -1;
	}
	var_t(const var_t& v)
	{
		//PRINT("var_t copy " << v.type);
		(*this) = v;
	}

	void operator = (const var_t& v)
	{
		//PRINT("var_t ="  << v.type << "," << v.fval);
		type = v.type;
		if (type == 2)
			fval = v.fval;
		else
			ival = v.ival;
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
		return type == v.type &&
			((type == 1 && ival == v) || (type == 2 && fval == v));
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

	var_t operator + (var_t& v) const
	{
		var_t ret;
		if (type == 2 || v.type == 2) {
			ret.type = 2;
			ret.fval = fval + v.fval;
		}
		else
		{
			ret.ival = ival + v.ival;
		}
		return ret;
	}
	var_t operator - (var_t& v) const
	{
		var_t ret;
		if (type == 2 || v.type == 2) {
			ret.type = 2;
			ret.fval = fval - v.fval;
		}
		else
		{
			ret.ival = ival - v.ival;
		}
		return ret;
	}
	var_t operator - () const
	{
		var_t ret;
		if (type == 2) {
			ret.type = 2;
			ret.fval = -fval;
		}
		else
		{
			ret.ival = -ival;
		}
		return ret;
	}
	var_t operator * (var_t& v) const
	{
		var_t ret;
		if (type == 2 || v.type == 2) {
			ret.type = 2;
			ret.fval = fval * v.fval;
		}
		else
		{
			ret.ival = ival * v.ival;
		}
		return ret;
	}
	var_t operator / (var_t& v) const
	{
		var_t ret;
		if (type == 2 || v.type == 2) {
			ret.type = 2;
			ret.fval = fval / v.fval;
		}
		else
		{
			ret.ival = ival / v.ival;
		}
		return ret;
	}
};

inline void _PHGPRINT(const std::string& pre, const var& v)
{
	if (v.type == 1)
		PRINT(pre << v.ival)
	else if (v.type == 2)
		PRINT(pre << v.fval)
	else
		PRINT(pre << "unkown type")
}

// ------------------------------------------
#include "phg.hpp"
// ------------------------------------------
// 运算
using fun_calc_t = std::function<string(code& cd, char o, int args)>;
fun_calc_t fun_calc = 0;
static var _act(code& cd, int args)
{
	opr o = cd.oprstack.pop();

	if (fun_calc)
	{
		if (!fun_calc(cd, o, args).empty())
			return 0;
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
				return !a;
			}
		}
		default: {}
		}
	return INVALIDVAR;
}