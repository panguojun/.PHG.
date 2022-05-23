/***************************************************************************
					{PHG}
					包含元素与节点树二元
					本文件是一个框架，内部引用了不同模块的技术
***************************************************************************/
#ifdef GROUP
#undef GROUP
#endif
#define GROUP	ScePHG	// 组定义

#define API(funname)		static var funname(ScePHG::code& cd, int args)
#define SPARAM(name)		string name = cd.strstack.back(); cd.strstack.pop_back();
#define PARAM(name)			var name = cd.valstack.back();cd.valstack.pop_back();
#define GET_SPARAM(index)	cd.strstack[cd.strstack.size() - 1 - (args - index)]
#define POP_SPARAM			for(int i = 0; i < args; i ++) cd.strstack.pop_back();
#define POP_SPARAMN(n)		for(int i = 0; i < n; i ++) cd.strstack.pop_back();

#define REG_API(funname,cppfunname)	ScePHG::register_api(#funname, cppfunname)
#define CALC	fun_calc = 
#define PROP	GROUP::add_var2 = 

//#define XML
#ifdef XML
#include "tinyxml.h"
#endif
using namespace std;
namespace ScePHG
{
	std::vector<string> strlist; 	// string values

// ---------------------------------------------------------------------
#include "base/phg_head.hpp"
// ---------------------------------------------------------------------
#include "base/element.hpp"
// ---------------------------------------------------------------------
#include "base/node.hpp"
// ---------------------------------------------------------------------
#include "entity/sprite.hpp"
#include "entity/nodecalc.hpp"
// ---------------------------------------------------------------------
	

	// clear all
	void clear()
	{
		sprite::clearres();
		tree_t::clear(ROOT);
		ROOT = 0;
		work_stack.clear();
		strlist.clear();
	}
	// =================================================================
	// API
	// =================================================================
	API(rendermod)
	{
		SPARAM(param1);
		renderstate = atoi(param1.c_str());
		return 0;
	}
	API(nform)
	{
		PRINT("nform");
		//tricombine::test();

		return 0;
	}
	API(rnd)
	{
		string param1 = GET_SPARAM(1);
		int maxrnd = atoi(param1.c_str());
		POP_SPARAM;
		return var(rand() % maxrnd);
	}
	API(inc)
	{
		string vname = GET_SPARAM(1);
		gvarmapstack.getvar(vname.c_str()) += var(1);
		POP_SPARAM;
		return 0;
	}
	API(dec)
	{
		string vname = GET_SPARAM(1);
		gvarmapstack.getvar(vname.c_str()) -= var(1);
		POP_SPARAM;
		return 0;
	}
	API(scat)
	{
		ASSERT(args == 2)
		string param1 = GET_SPARAM(1);
		string param2 = GET_SPARAM(2);
		POP_SPARAM;
		cd.strstack.push_back(param1 + param2);
		return 0;
	}
	API(scmp)
	{
		ASSERT(args == 2);
		string param1 = GET_SPARAM(1);
		string param2 = GET_SPARAM(2);
		POP_SPARAM; return int(param1 == param2);
	}
	API(tos)
	{
		ASSERT(args == 1);
		PARAM(v);
		cd.strstack.push_back(v.type == 1 ? to_string(v.ival) : to_string(v.fval));
		return 0;
	}
	API(strout)
	{
		ASSERT(args == 1);
		string param1 = GET_SPARAM(1);
		PRINT(param1);

		POP_SPARAM; return 0;
	}

	// setuptree
	API(setuptree)
	{
		string type; // 节点可以实现化为不同的实体类型
		if (args > 0)
		{
			type = GET_SPARAM(1);
			PRINTV(type);
		}
		NODE* node = ROOT;
		if (args > 1)
		{
			string nodename = GET_SPARAM(2);
			node = GET_NODE(nodename, ROOT);
			PRINTV(nodename);
		}
		if (type == "sprite")
			sprite::setup(node, { vec2::ZERO,0, 1 });
		else if (type == "sprite_add")
			sprite::setup_add(node);

		POP_SPARAM;
		return 0;
	}
	
#ifdef XML
// ----------------------------------------
#include "xmlparser.hpp"
// ----------------------------------------
	API(fromXML)
	{
		ASSERT(!work_stack.empty());

		SPARAM(filename);

		fromXML(filename, ME);

		return 0;
	}
#endif

#ifdef JSON
// ----------------------------------------
#include "jsonparser.hpp"
// ----------------------------------------
	API(tojson)
	{
		PRINT("------------- tojson ----------------");
		if (ROOT)
			JSON_PARSER::tojson(ROOT);
		return 0;
	}
	API(tojsonraw)
	{
		PRINT("------------- tojsonraw ----------------");
		if (ROOT)
			JSON_PARSER::tojson_raw(ROOT);
		return 0;
	}
#endif	
	// phgoper
	void phgoper(tree_t* tree)
	{
		work_stack.push_back(tree);
		{// bool
			KEY_VAL("phg")
			{
				ScePHG::dostring((it->second + ";").c_str());
			}
		}
		// children
		for (auto it : tree->children) {
			phgoper(it.second);
		}
	}
	API(phgoper)
	{
		phgoper(ROOT);

		return 0;
	}
	API(dostring)
	{
		SPARAM(str);
		
		ScePHG::dostring(str.c_str());

		return 0;
	}

	API(server)
	{
		SPARAM(ip);
		PRINT("======= start http-server: " << ip << ":8080\n");
		serverip = ip;
		std::thread serverthread = std::thread{ servermain, 800 };
		getchar();

		return 0;
	}
	
	// -----------------------------------
	// REG API
	// -----------------------------------
	void setup()
	{
		PRINT("setup ScePHG");

		tree = _tree;
		act = _act;

		REG_API(mod, rendermod);		// 渲染模式
		REG_API(rnd, rnd);			// 随机函数
		REG_API(inc, inc);			// inc
		REG_API(dec, dec);			// dec
		REG_API(scat, scat);			// 字符串拼接
		REG_API(scmp, scmp);			// 字符串比较
		REG_API(tos, tos);			// 转化字符串
		REG_API(so, strout);			// 字符串打印
		
		REG_API(do, dostring);			// dostring
		
		REG_API(dump, dump);

		REG_API(setup, setuptree);		// 生成节点树

		REG_API(server, server);

		// ELEMENT: 

		REG_API(phg, phgoper);			// 脚本
		
		NODE_REG_API();

		NODECALC_REG_API();			// node calc

		SPRITE_REG_API();			// sprite 注册
	}
};
// ====================================
// test
// ====================================
void realphg()
{
	ScePHG::dofile("main.r");
}

//------------------------------------------
// VB
//------------------------------------------
extern "C"
{
	int EXPORT_API _stdcall VB_doPHG(const char* script)
	{
		std::string str(script);
		//PRINTV(str);

		if (str.find(".r") != std::string::npos)
		{
			ScePHG::clear();
			ScePHG::setup();
			renderstate = 0;
			resetsm();

			ScePHG::dofile(str.c_str());
		}
		else
		{
			if (!ROOT) {
				ScePHG::setup();
			}
			ScePHG::dostring(str.c_str());
		}
		//MSGBOX("VB_doPHG Done!");
		return 0;
	}
	int EXPORT_API _stdcall getIntVar(const char* key)
	{
		return GET_VAR(key).ival;
	}
	int EXPORT_API _stdcall getStringSize()
	{
		return ScePHG::strlist.size();
	}

	int EXPORT_API _stdcall getStringDat(int index, char* buf)
	{
		if (ScePHG::strlist.empty() || index >= ScePHG::strlist.size())
		{
			//buf[0] = '\0';
			return 0;
		}

		const string& str = ScePHG::strlist[index % ScePHG::strlist.size()];
		if (str.length() > 16)
			return 0;
		memcpy(buf, str.c_str(), str.length()+1);
		return  str.length();
	}
}
