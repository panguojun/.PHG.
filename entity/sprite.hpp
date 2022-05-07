/**************************************************************************
*			精灵
*			可绘制的2D场景对象
**************************************************************************/
struct tree_t;
namespace sprite
{
	struct transform2_t { vec2 p; real ang; real s = 1; };
	struct spriteres_t
	{
		// 携带的属性
		bool vis = true;		// 可见性

		transform2_t trans;		// 空间变换
		float phang;			// 相位角
		string md;				// 模型

		spriteres_t() {}
		spriteres_t(const spriteres_t& v)
		{
			vis = v.vis;
			trans = v.trans;
			md = v.md;
		};
		~spriteres_t() {}
	};
	vector<spriteres_t*> reslist;			// 资源列表
	spriteres_t& res(ENT& ent)
	{
		if (ent.resid == -1)
		{
			reslist.push_back(new spriteres_t());
			ent.resid = reslist.size() - 1;
			//PRINT("create res id=" << resid);
		}
		ASSERT(ent.resid < reslist.size());
		return *reslist[ent.resid];
	}

	struct addres_t
	{
		// 携带的属性
		string md;				// 模型

		addres_t() {}
		addres_t(const addres_t& v)
		{
			md = v.md;
		};
		~addres_t() {}
	};
	vector<addres_t*> add_reslist;		// 资源列表

	void clearres()
	{
		for (auto it : reslist)
			if (it) delete it;
		reslist.clear();

		for (auto it : add_reslist)
			if (it) delete it;
		add_reslist.clear();
	}

	// *********************************************************************
	// Setup tree, generate entities
	// *********************************************************************
#define KEY_VAL(val) if (auto& it = tree->kv.find(val); it != tree->kv.end())

// data convert
	int stoint(crstr sval)
	{
		return atoi(sval.c_str());
	}
	real storeal(crstr sval)
	{
		return atof(sval.c_str());
	}
	vec2 stovec(crstr sval)
	{
		vec2 ret;
		sscanf(sval.c_str(), "%f,%f", &ret.x, &ret.y);
		return ret;
	}

	void setup(tree_t* tree, const transform2_t& parent, string str = "")
	{
		ASSERT(tree)
			work_stack.push_back(tree);

		ENT ent;
		transform2_t& trans = res(ent).trans;
		{// transform
			vec2 p;
			real ang = 0;
			real s = 1;
			{// transform desc
				KEY_VAL("p") // raw position
				{
					p = stovec(it->second);
				}
				KEY_VAL("x") // move x
				{
					p += vec2::UX * storeal(it->second);
				}
				KEY_VAL("y") // move y
				{
					p += vec2::UY * storeal(it->second);
				}
				KEY_VAL("a")
				{
					ang = PI / 180 * storeal(it->second);
				}
				KEY_VAL("s") // scale
				{
					s = storeal(it->second);
				}
			}
			trans = {
				parent.p + (vec2::UX.rotcopy(parent.ang)) * p.x + (vec2::UY.rotcopy(parent.ang)) * p.y,
				parent.ang + ang,
				parent.s * s
			};
			if (!str.empty()) str += ";";
			str += (to_string(trans.p.x) + "," + to_string(trans.p.y) + "," + to_string(trans.ang) + "," + to_string(trans.s));
		}
		{// 添加到变量列表

			//ent.sval = tree->name;
			KEY_VAL("vis") // vis
			{
				if (it->second == "false")
				{
					res(ent).vis = false;
				}
			}

			KEY_VAL("md") {
				res(ent).md = it->second;
			}

			KEY_VAL("pr1") {
				res(ent).md = it->second;
				PRINTV(it->second);
			}
			//if(CEHCK_CONSTR(res(ent).cstr))
			gvarmapstack.addvar(tree->name.c_str(), ent);
		}

		if (tree->children.empty())
		{
			strlist.push_back(str);
		}
		else {
			// children
			for (auto it : tree->children) {
				setup(it.second, res(ent).trans, str);
			}
		}
	}

	// 在节点树上搜索加法规则
	const char* walk_addtree(tree_t* tree, crstr a, crstr b, const char* key)
	{
		if (tree->children.size() >= 2)
		{
			int findcnt = 0;
			for (auto it : tree->children) {
				if (it.second->children.empty() && it.second->kv[key] == a)
				{
					findcnt++;
				}
				if (it.second->children.empty() && it.second->kv[key] == b)
				{
					findcnt++;
				}
			}

			if (findcnt == 2)
			{
				return tree->kv[key].c_str();
			}
		}

		// children
		for (auto it : tree->children) {
			if (const char* c = walk_addtree(it.second, a, b, key); c != 0)
			{
				return c;
			}
		}
		return 0;
	}
	const char* _calc_add(crstr a, crstr b, const char* key)
	{
		if (a == b)
			return a.c_str();

		MSGBOX("walk_addtree")
			const char* ret = walk_addtree(ROOT, a, b, key);

		if (ret == 0)
			return b.c_str();

		return ret;
	}

	// 加法资源
	addres_t& addres(ENT& ent)
	{
		if (ent.resid == -1)
		{
			addres_t* rs = new addres_t();
			add_reslist.push_back(rs);
			ent.resid = add_reslist.size() - 1;

			ent.type = 0; // 自定义元素类型
			// 在资源上定义加法运算
			ent.fun_add = [](var& a, var& b)->var {
				var ret;
				ret.type = 3;
				//MSGBOX("fun_add " << addres(ret).md)
				addres(ret).md = _calc_add(
					addres(a).md,
					addres(b).md,
					"pr1"
				);
				return ret;
			};

		}
		//PRINTV(ent.resid);
		ASSERT(ent.resid < add_reslist.size());
		return *add_reslist[ent.resid];
	}

	void setup_add(tree_t* tree)
	{
		work_stack.push_back(tree);

		ENT ent;
		{// 添加到变量列表

			KEY_VAL("md") {
				addres(ent).md = it->second;
			}
			KEY_VAL("pr1") {
				addres(ent).md = it->second;
			}
			gvarmapstack.addvar(addres(ent).md.c_str(), ent);
		}

		// children
		for (auto it : tree->children) {
			setup_add(it.second);
		}
	}
}

// ------------------------------------
// API
// ------------------------------------
API(getspriteloc)
{
	vec3list.clear();

	NODE* node = ROOT;
	if (args > 0) {
		string name = GET_SPARAM(1);
		node = GET_NODE(name, ROOT);
		if (!node)
			return 0;
	}

	ScePHG::node_walker(node, [](ScePHG::tree_t* tree)->void
		{
			if (var v; gvarmapstack.getvar(v, tree->name.c_str()))
			{
				sprite::transform2_t& t = sprite::res(v).trans;

				vec3list.emplace_back(t.p.x, t.p.y, t.ang);
			}
		});

	PRINTV(vec3list.size());
	POP_SPARAM;
	return 0;
}
API(calc_addmd)
{
	crstr a = GET_SPARAM(1);
	crstr b = GET_SPARAM(2);
	string c = sprite::_calc_add(a, b, "pr1");

	/*if (!strlist.empty() && c != a && c != b)
	{
		strlist.back() = c;
	}
	else*/
	strlist.push_back(c);

	POP_SPARAM; return 0;
}
void SPRITE_REG_API()
{
	REG_API(getsprloc, getspriteloc);	// 获得get sprite loc
	REG_API(addmd, calc_addmd);
}
