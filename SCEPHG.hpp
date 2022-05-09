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
#define SPARAM(name)		string name = cd.strstack.back(); cd.strstack.pop_back();PRINT(name)
#define PARAM(name)			var& name = cd.valstack.back();
#define GET_SPARAM(index)	cd.strstack[cd.strstack.size() - 1 - (args - index)]
#define POP_SPARAM			for(int i = 0; i < args; i ++) cd.strstack.pop_back();

#define REG_API(funname,cppfunname)	ScePHG::register_api(#funname, cppfunname)

#define XML
#ifdef XML
#include "tinyxml.h"
#endif

namespace ScePHG
{
	std::vector<string> strlist; // string values

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

	inline string fixedname(crstr name)
	{
		const char* p = name.c_str();
		if (*p == '\'' || *p == '\"')
		{
			string ret = p + 1;
			ret.pop_back();
			return ret;
		}
		return name;
	}
	inline void fixedproperty(string& str)
	{
		str.erase(std::remove(str.begin(), str.end(), '\''), str.end());
		str.erase(std::remove(str.begin(), str.end(), '\"'), str.end());

	}
	inline rect_t torect(crstr str)
	{
		rect_t rect;
		sscanf_s(str.c_str(), "{x:%d,y:%d,width:%d,height:%d}",  &rect.x, &rect.y, &rect.width, &rect.height);
		return rect;
	}
	API(getrect)
	{
		rectlist.clear();
		NODE* node = ROOT;
		if (args > 0)
		{
			string param1 = GET_SPARAM(1);
			node = GET_NODE(param1, ROOT);
			if (!node)
				return 0;
		}
		string key = "rect";
		if (args > 1)
		{
			key = GET_SPARAM(2);
		}
		node_walker(node, [key](tree_t* tree)->void
			{
				auto& it = tree->kv.find(key);
				if (it != tree->kv.end())
				{
					string str= it->second;
					fixedproperty(str);
					rectlist.push_back(torect(str));
				}
			});
		POP_SPARAM;
		return 0;
	}
	inline vec3 tovec3(crstr str)
	{
		vec3 v;
		sscanf_s(str.c_str(), "%f,%f,%f", &v.x, &v.y, &v.z);
		return v;
	}
	API(getvec3)
	{
		vec3list.clear();
		NODE* node = ROOT;
		if (args > 0)
		{
			string param1 = GET_SPARAM(1);
			node = GET_NODE(param1, ROOT);
			if (!node)
				return 0;
		}
		string key = "pos";
		if (args > 1)
		{
			key = GET_SPARAM(2);
		}
		node_walker(node, [key](tree_t* tree)->void
			{
				auto& it = tree->kv.find(key);
				if (it != tree->kv.end())
				{
					string str = it->second;
					fixedproperty(str);
					vec3list.push_back(tovec3(str));
				}
			});
		POP_SPARAM;
		return 0;
	}
	API(getfval)
	{
		reallist.clear();
		NODE* node = ROOT;
		if (args > 0)
		{
			string param1 = GET_SPARAM(1);
			node = GET_NODE(param1, ROOT);
			if (!node)
				return 0;
		}
		string key = "x";
		if (args > 1)
		{
			key = GET_SPARAM(2);
		}
		node_walker(node, [key](tree_t* tree)->void
			{
				auto& it = tree->kv.find(key);
				if (it != tree->kv.end())
				{
					string str = it->second;
					fixedproperty(str);
					reallist.push_back(atof(str.c_str()));
				}
			});
		POP_SPARAM;
		return 0;
	}
	API(getival)
	{
		intlist.clear();
		NODE* node = ROOT;
		if (args > 0)
		{
			string param1 = GET_SPARAM(1);
			node = GET_NODE(param1, ROOT);
			if (!node)
				return 0;
		}
		string key = "x";
		if (args > 1)
		{
			key = GET_SPARAM(2);
		}
		node_walker(node, [key](tree_t* tree)->void
			{
				auto& it = tree->kv.find(key);
				if (it != tree->kv.end())
				{
					string str = it->second;
					fixedproperty(str);
					intlist.push_back(atoi(str.c_str()));
				}
			});
		POP_SPARAM;
		return 0;
	}
	
	API(getstr)
	{
		string param1 = GET_SPARAM(1);
		string param2 = GET_SPARAM(2);
		
		//strlist.clear();
		ScePHG::node_walker(ROOT, [param1, param2](ScePHG::tree_t* tree)->void
			{
				if (tree->name == param1) {
					auto& it = tree->kv.find(param2);
					if (it != tree->kv.end())
					{
						strlist.push_back(it->second.c_str());
					}
				}
			});
		POP_SPARAM;

		PRINTV(strlist.size());
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
#ifdef PYTHON
	API(formImgIDF)
	{
		SPARAM(img);

		ASSERT(ME);

		string json;
		python_interface::detect_rectangle(json, img);

		JSON_PARSER::fromJSON(ME,json);

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

	// constraint
	/*API(constraint)
	{
		return 0;
	}*/
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
		
		// NODE: 

		REG_API(cur, cur);				// ME
		REG_API(array, array);			// 节点阵列
		REG_API(sequ, sequ);			// 节点序列

		REG_API(addprop, addprop);		// 添加属性
		
		REG_API(getival, getival);		// 获得int value
		REG_API(getfval, getfval);		// 获得float value
		REG_API(getstr, getstr);		// 获得string
		REG_API(getvec3, getvec3);		// 获得RECT
		REG_API(getrect, getrect);		// 获得RECT

#ifdef XML
		REG_API(fromXML, fromXML);		// xml读入
#endif
#ifdef PYTHON
		REG_API(formImgIDF, formImgIDF);// 图像识别读入
#endif
		REG_API(tojson, tojson);

		REG_API(tojsonraw, tojsonraw);

		// ELEMENT: 

		REG_API(bool, booloper);		// 布尔运算

		REG_API(phg, phgoper);			// 脚本

		REG_API(draw, draw);			// 绘制

		// CONSTRAINT

		//REG_API(constraint, constraint);// 约束

		SPRITE_REG_API();				// sprite 注册
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
