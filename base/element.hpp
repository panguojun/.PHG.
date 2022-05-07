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
struct var_t : public varbase_t
{
	VAR_BASE(var_t);
	var_t() { }

	// 资源加法函数
	std::function<var_t(var_t& a,var_t& b)> fun_add = 0;

	var_t operator + (var_t& v)
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
};

inline void _PHGPRINT(const std::string& pre, const var& v)
{
	//if (v.sval == "")
	PRINT(pre << v.ival)
		//else
		//	PRINT(pre << v.sval)
}

// ------------------------------------------
#include "phg.hpp"
// ------------------------------------------
// 运算
static var _act(code& cd, int args)
{
	opr o = cd.oprstack.pop();

	//PRINT("act:" << o << "(" << args << ")")

		switch (o) {
		case '+': {
			if (args > 1) {
				var& b = PHG_VALSTACK(1);
				PRINTV(b.resid);
				var& a = PHG_VALSTACK(2);
				PRINTV(a.resid);
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
				return cd.valstack.pop();
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