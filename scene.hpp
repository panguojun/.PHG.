/***************************************************************************
					{PHG}
					包含元素与节点树二元
					本文件是一个框架，内部引用了不同模块的技术
***************************************************************************/
#ifdef GROUP
#undef GROUP
#endif
#define GROUP	ScePHG	// 组定义

#define API(funname)			static var funname(ScePHG::code& cd, int args)
#define SPARAM(name)			string name = cd.strstack.back(); cd.strstack.pop_back();PRINT(name)
#define PARAM(name)			var& name = cd.valstack.back();
#define GET_SPARAM(index)		cd.strstack[cd.strstack.size() - 1 - (args - index)]
#define POP_SPARAM			for(int i = 0; i < args; i ++) cd.strstack.pop_back();

#define REG_API(funname,cppfunname)	ScePHG::register_api(#funname, cppfunname)

#define XML
#ifdef XML
#include "tinyxml.h"
#endif

namespace ScePHG
{
	std::vector<string> strlist; 	// string values

// ---------------------------------------------------------------------
#include "phg_head.hpp"
// ---------------------------------------------------------------------
#include "element.hpp"
// ---------------------------------------------------------------------
#include "node.hpp"
// ---------------------------------------------------------------------
#include "entity.hpp"
#include "sprite.hpp"
// ---------------------------------------------------------------------
	

	// clear all
	void clear()
	{
		entity::clearres();
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
		tricombine::test();

		return 0;
	}
	API(rnd)
	{
		string param1 = GET_SPARAM(1);
		int maxrnd = atoi(param1.c_str());
		POP_SPARAM;
		return var(rand() % maxrnd);
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
			sprite::setup(node, { vec2::ZERO,0, vec2(1,1) });
		else if (type == "sprite_add")
			sprite::setup_add(node);
		else if (type == "entity")
			entity::setup(node, { vec3::ZERO, quaternion(), vec3::ONE });

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

	// booloper
	API(booloper)
	{
		entity::booloper(ROOT);

		return 0;
	}

	API(draw)
	{
		{ // 方便之用
			renderstate = 0;
		}

		entity::draw(ROOT);

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
		REG_API(nform, nform);
		REG_API(rnd, rnd);

		REG_API(dump, dump);

		REG_API(setup, setuptree);		// 生成节点树
		
		REG_API(tojson, tojson);

		REG_API(tojsonraw, tojsonraw);

		// ELEMENT: 

		REG_API(bool, booloper);		// 布尔运算

		REG_API(phg, phgoper);			// 脚本

		REG_API(draw, draw);			// 绘制
		
		NODE_REG_API();
		
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
		init();

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
