/**************************************************************************
*			树运算
*			人工智能的科学化？
**************************************************************************/
struct tree_t;
namespace nodecalc
{
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
		for (auto it : add_reslist)
			if (it) delete it;
		add_reslist.clear();
	}

	// *********************************************************************
	// Setup tree
	// *********************************************************************
#define KEY_VAL(val) if (auto& it = tree->kv.find(val); it != tree->kv.end())

	int walk_addtreex(std::string& str, tree_t* tree, crstr a, crstr b, const char* key)
	{
		if (tree->kv[key] == a)
			return 1;
		if (tree->kv[key] == b)
			return 2;

		// children
		int flag = 0;
		for (auto it : tree->children) {
			int ret = walk_addtreex(str, it.second, a, b, key);
			//PRINTV(ret);
			if (ret == 3)
				return ret;

			if (ret)
			{
				flag |= ret;
				if (flag == 3)
				{
					str = tree->kv[key].c_str();
					return flag;
				}
			}
		}
		return flag;
	}
	
	void _calc_add(std::string& str, crstr a, crstr b, const char* key)
	{
		if (a == b) {
			str = a;
			return;
		}
    
		walk_addtreex(str, ROOT, a, b, key);
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
				_calc_add(
					addres(ret).md,
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
API(calc_add)
{
	crstr a = GET_SPARAM(1);
	crstr b = GET_SPARAM(2);
	string c;
	sprite::_calc_add(c, a, b, "pr1");

	PRINTV(c);
	strlist.push_back(c);

	POP_SPARAM; return 0;
}
void NODECALC_REG_API()
{
	REG_API(add, calc_add);
}
