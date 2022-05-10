/**************************************************************************
*			树节点运算
*			人工智能的科学化？
**************************************************************************/
struct tree_t;
namespace nodecalc
{
	struct res_t
	{
		string md;				// 模型

		res_t() {}
		res_t(const res_t& v)
		{
			md = v.md;
		};
		~res_t() {}
	};
	vector<res_t*> reslist;				// 资源列表

	void clearres()
	{
		for (auto it : reslist)
			if (it) delete it;
		reslist.clear();
	}

	// *********************************************************************
	// Setup tree
	// *********************************************************************
#define KEY_VAL(val) if (auto& it = tree->kv.find(val); it != tree->kv.end())

	int _walk_tree_ancestor(tree_t** ancestor, tree_t* tree, crstr a, crstr b, const char* key)
	{
		if (tree->kv[key] == a)
			return 1;
		if (tree->kv[key] == b)
			return 2;

		// children
		int flag = 0;
		for (auto it : tree->children) {
			int ret = _walk_tree_ancestor(ancestor, it.second, a, b, key);
			
			if (ret == 3)
				return ret;

			if (ret)
			{
				flag |= ret;
				if (flag == 3)
				{
					*ancestor = tree;
					return flag;
				}
			}
		}
		return flag;
	}
	int _walk_tree_add(std::string& str, tree_t* tree, crstr a, crstr b, const char* key)
	{
		if (tree->kv[key] == a)
			return 1;
		if (tree->kv[key] == b)
			return 2;

		// children
		int flag = 0;
		for (auto it : tree->children) {
			int ret = _walk_tree_add(str, it.second, a, b, key);
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
    
		_walk_tree_add(str, ROOT, a, b, key);
	}
	void _calc_sub(std::string& str, crstr a, crstr b, const char* key)
	{
		if (a == b) {
			str = a;
			return;
		}
		
    		tree_t* ancestor = 0;
		_walk_tree_ancestor(&ancestor, ROOT, a, b, key);
		
		if(ancestor)
		{
			for(auto& it : tree->children)
			{
				if(auto& v = it->kv[key];v != b)
				{
					str = v;
					break;
				}
			}
		}
	}
	
	// 资源
	res_t& res(ENT& ent)
	{
		if (ent.resid == -1)
		{
			res_t* rs = new res_t();
			reslist.push_back(rs);
			ent.resid = reslist.size() - 1;

			ent.type = 0; // 自定义元素类型
			// 在资源上定义加法运算
			ent.fun_add = [](var& a, var& b)->var {
				var ret;
				ret.type = 0;
				_calc_add(
					res(ret).md,
					res(a).md,
					res(b).md,
					"pr1"
				);
				return ret;
			};
		}
		//PRINTV(ent.resid);
		ASSERT(ent.resid < reslist.size());
		return *reslist[ent.resid];
	}

	void setup(tree_t* tree)
	{
		work_stack.push_back(tree);

		ENT ent;
		{// 添加到变量列表
			KEY_VAL("pr1") {
				res(ent).md = it->second;
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
	nodecalc::_calc_add(c, a, b, "pr1");

	PRINTV(c);
	strlist.push_back(c);

	POP_SPARAM; return 0;
}
API(calc_sub)
{
	crstr a = GET_SPARAM(1);
	crstr b = GET_SPARAM(2);
	string c;
	nodecalc::_calc_sub(c, a, b, "pr1");

	PRINTV(c);
	strlist.push_back(c);

	POP_SPARAM; return 0;
}
void NODECALC_REG_API()
{
	REG_API(add, calc_add);
	REG_API(sub, calc_sub);
}
