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
struct var_t : public varbase_t
{
	VAR_BASE(var_t);
	var_t() { }
	var_t(const varbase_t& v)
	{
		//PRINT("varbase_t copy " << v.type);
		(*(varbase_t*)this) = v;
	}
	// 资源加法函数
	std::function<var_t(var_t& a,var_t& b)> fun_add = 0;

	inline void operator = (const var_t& v)
	{
		varbase_t::operator=(v);
	}
	inline var_t operator + (var_t& v)
	{
		//PRINT("operator + " << v.resid << " " << resid)

		if (!fun_add) {
			return varbase_t::operator+(v);
		}
		else
		{
			return fun_add(*this, v);
		}
	}
	inline var_t operator - () const
	{
		//PRINT("-")
		return varbase_t::operator-();
	}
};

inline void _PHGPRINT(const std::string& pre, const var& v)
{
	PRINTV(v.type)
	if(v.type == 1)
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
		if(!fun_calc(cd, o, args).empty())
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
	default: {}
	}
	return INVALIDVAR;
}
