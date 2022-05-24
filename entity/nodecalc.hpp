/**************************************************************************
				事件运算

**************************************************************************/
struct tree_t;
namespace nodecalc
{
#define KEY_VAL(val) if (auto& it = tree->kv.find(val); it != tree->kv.end())
	bool abelian_sym = true;	// 阿贝尔对称 即运算的可交换性

	tree_t* _walk_tree_node(std::string& str, tree_t* tree, crstr a, const char* key)
	{
		if (tree->kv[key] == a)
		{
			return tree;
		}

		// children
		for (auto it : tree->children) {
			tree_t* t = _walk_tree_node(str, it.second, a, key);
			if (t)
				return t;
		}
		return 0;
	}
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
					PRINT("found ancestor!")
						return flag;
				}
			}
		}
		return flag;
	}
	int _calc_add(tree_t** out, tree_t* tree, crstr a, crstr b, const char* key)
	{
		{// kv
			crstr v = tree->kv[key];
			//PRINT("key:" << key << " val:" << v);
			if (v == a)
				return 1;
			if (v == b)
				return 2;
		}

		// children
		int flag = 0;
		tree_t* lstn = 0;
		for (auto& it : tree->children) {
			int ret = _calc_add(out, it.second, a, b, key);
			//PRINTV(ret);
			if (ret == 3)
				return ret;

			if (ret && tree != ROOT)
			{
				flag |= ret;
				if (flag == 3)
				{
					if (!abelian_sym)
					{// 顺序判断
						if (lstn != 0)
						{
							PRINTV(it.second->getindex());
							PRINTV(lstn->getindex());
							if (ret == 1 && it.second->getindex() > lstn->getindex() || ret == 2 && it.second->getindex() < lstn->getindex())
							{
								PRINT("order failed!")
									return 0;
							}
						}
					}
					*out = tree;
					//str = tree->kv[key];
					return flag;
				}
				lstn = it.second;
			}
		}
		return flag;
	}
	void _calc_addd(std::string& str, crstr a, const char* key)
	{
		NODE* node = _walk_tree_node(str, ROOT, a, key);
		if (node->parent)
			str = node->parent->kv[key];
	}
	void _calc_sub(std::string& str, crstr a, crstr b, const char* key)
	{
		if (a == b) {
			str = a;
			return;
		}
		NODE* node = _walk_tree_node(str, ROOT, a, key);

		if (node)
		{
			bool bina = false;
			for (auto& it : node->children)
			{
				if (auto& v = it.second->kv[key]; v == b)
				{
					bina = true;
					break;
				}
			}
			if (bina) {
				for (auto& it : node->children)
				{
					if (auto& v = it.second->kv[key]; v != b)
					{
						str = v;
						return;
					}
				}
			}
		}
	}
	void _calc_subb(std::string& str, crstr a, const char* key)
	{
		NODE* node = _walk_tree_node(str, ROOT, a, key);
		if (!node->children.empty())
		{
			for (auto& it : node->children)
			{
				str = it.second->kv[key];
				return;
			}
		}
	}
	void _wak_tree(tree_t* tree, crstr a, const char* k, const char* ok)
	{
		if (tree->kv[k] == a)
		{
			strlist.push_back(tree->kv[ok]);
		}

		// children
		for (auto it : tree->children) {
			_wak_tree(it.second, a, k, ok);
		}
	}
	int _calc_add(tree_t* tree, crstr a, crstr b, const char* key, const char* ok)
	{
		{// kv
			crstr v = tree->kv[key];
			//PRINT("key:" << key << " val:" << v);
			if (v == a)
				return 1;
			if (v == b)
				return 2;
		}

		// children
		int flag = 0;
		tree_t* lstn = 0;
		for (auto& it : tree->children) {
			int ret = _calc_add(it.second, a, b, key, ok);
			//PRINTV(ret);
			if (ret == 3)
				return ret;

			if (ret && tree != ROOT)
			{
				flag |= ret;
				if (flag == 3)
				{
					if (!abelian_sym)
					{// 顺序判断
						if (lstn != 0)
						{
							PRINTV(it.second->getindex());
							PRINTV(lstn->getindex());
							if (ret == 1 && it.second->getindex() > lstn->getindex() || ret == 2 && it.second->getindex() < lstn->getindex())
							{
								PRINT("order failed!")
									return 0;
							}
						}
					}
					strlist.push_back(tree->kv[ok]);
					return flag;
				}
				lstn = it.second;
			}
		}
		return flag;
	}
	struct res_t
	{
		// 携带的属性
		NODE* node;	
		string key;

		res_t() {}
		res_t(const res_t& v)
		{
			node = v.node;
			key = v.key;
		};
		~res_t() {}
	};
	vector<res_t*> reslist;		// 资源列表

	res_t& cres(const var& ent)
	{
		ASSERT(ent.resid >= 0 && ent.resid < reslist.size());
		return *reslist[ent.resid];
	}
	res_t& res(var& ent)
	{
		if (ent.resid == -1)
		{
			res_t* rs = new res_t();
			reslist.push_back(rs);
			ent.resid = reslist.size() - 1;

			ent.type = 0; // 自定义元素类型
			// 在资源上定义加法运算
			ent.fun_set = [&ent](const var& v) {
				if (v.type == 3)
				{
					res(ent).node->kv[res(ent).key] = cres(v).node->kv[cres(v).key];
				}
				else {
					//ent.node->kv[ent.key] = v.tostr();
				}
			};

		}
		//PRINTV(ent.resid);
		ASSERT(ent.resid < reslist.size());
		return *reslist[ent.resid];
	}
}

// ------------------------------------
// API
// ------------------------------------
API(calc_set_abelian)
{
	crstr b = GET_SPARAM(1);

	nodecalc::abelian_sym = atoi(b.c_str());
	PRINTV(nodecalc::abelian_sym)

		POP_SPARAM; return 0;
}
API(calc_addd)
{
	crstr a = GET_SPARAM(1);
	string c;
	nodecalc::_calc_addd(c, a, cur_property.c_str());

	PRINTV(c);
	strlist.push_back(c);

	POP_SPARAM; return 0;
}
API(calc_add)
{
	string a = GET_SPARAM(1);
	string b = GET_SPARAM(2);
	{
		NODE* an = GET_NODE(a, ROOT); ASSERT(an);
		NODE* bn = GET_NODE(b, ROOT); ASSERT(bn);
		a = an->kv[cur_property];
		b = bn->kv[cur_property];
	}

	NODE* n = 0;
	string c;
	if (a == b) {
		c = a;
	}
	else {
		nodecalc::_calc_add(&n, ROOT,
			a, b,
			cur_property.c_str());
		if (n) {
			c = n->kv[cur_property];

			PRINTV(n->name);
		}
	}
	strlist.push_back(c);

	if (ME) {
		if (n) *(work_stack.back()) += *n;
		ME->kv[cur_property] = c;
	}

	POP_SPARAM; return 0;
}
API(calc_subb)
{
	crstr a = GET_SPARAM(1);
	string c;
	nodecalc::_calc_subb(c, a, cur_property.c_str());

	PRINTV(c);
	strlist.push_back(c);

	POP_SPARAM; return 0;
}
API(calc_sub)
{
	crstr a = GET_SPARAM(1);
	crstr b = GET_SPARAM(2);
	string c;
	nodecalc::_calc_sub(c, a, b, cur_property.c_str());

	PRINTV(c);
	strlist.push_back(c);

	POP_SPARAM; return 0;
}
API(calc_wak)
{
	if (args == 2)
	{
		NODE* n = 0;
		crstr ok = GET_SPARAM(1);
		crstr expr = GET_SPARAM(2);
		code ccd(expr.c_str());
		string a = ccd.getname();
		ccd.next3();
		char op = ccd.cur();
		ccd.next();
		string b = ccd.getname();
		if (op == '+')
			nodecalc::_calc_add(n, a, b, cur_property.c_str(), ok.c_str());
		else if (op == '-')
		{
			string c;
			nodecalc::_calc_sub(c, a, b, cur_property.c_str());

			PRINTV(c);
			strlist.push_back(c);
		}
	}
	else
	{
		crstr nm = GET_SPARAM(1);
		NODE* n = nm == "me" ? ME : GET_NODE(nm, ROOT); ASSERT(n);
		if (args == 3) {
			crstr a = GET_SPARAM(2);
			crstr ok = GET_SPARAM(3);

			nodecalc::_wak_tree(n, a, cur_property.c_str(), ok.c_str());
		}
		else if (args == 4)
		{
			crstr a = GET_SPARAM(2);
			crstr b = GET_SPARAM(3);
			crstr ok = GET_SPARAM(4);

			nodecalc::_calc_add(n, a, b, cur_property.c_str(), ok.c_str());
		}
	}

	POP_SPARAM; return 0;
}

API(clearstrlist)
{
	strlist.clear();

	return 0;
}
API(calc_expr)
{
	ScePHG::node_walker(ROOT, [](ScePHG::tree_t* tree)->void
		{
			for (auto& it : tree->kv) {
				string result;
				const char* ps = it.second.c_str();
				const char* start  = ps;
				while (*ps != '\0') {
					
					if ((*ps) == '(')
					{
						int offset = ps - start + 1;
						int cnt = 0;
						while (*(++ps) != ')') cnt++;
						{
							// 内部变量
							gvarmapstack.addvar("_i", tree->index);
							gvarmapstack.addvar("_t", tree_t::getdepth(tree));
						}
						string str = it.second.substr(offset, cnt) + ";";
						var v = ScePHG::doexpr(str.c_str());

						result += VAR2STR(v);
					}
					else
					{
						result += *ps;
					}
					++ps;
				}
				it.second = result;
				//PRINTV(result);
			}
		});
	POP_SPARAM;

	return 0;
}
void NODECALC_REG_API()
{
	CALC([](code& cd, char o, int args)->var {

		PRINT("CALC: " << o << "(" << args << ")");
		//PRINTV(ME->name);
		
		if (o == '.')
		{
			crstr a = GET_SPARAM(1);
			crstr b = GET_SPARAM(2);
			
			NODE* n = a == "me" ? ME : GET_NODE(a, ROOT); ASSERT(n);
			string c = n->kv[b];
			PRINT(a << "." << b << "=" << c)
			var v;v.type = 3; nodecalc::res(v).node = n; nodecalc::res(v).key = b;
			strlist.push_back(c);
			POP_SPARAM;
			//PHG_VALPOP(2);
			cd.strstack.push_back(c);
			return v;
		}
		return 0;
		});

	PROP([](code& cd, const char* a, const char* b, var& v) {
			int args = 1;
			PRINT("PROP: " << a << "." << b);

			NODE* n = a == "me" ? ME : GET_NODE(a, ROOT); ASSERT(n);
			//PRINTV(cd.strstack.size());
			string sv = GET_SPARAM(1);
			n->kv[b] = sv;
			POP_SPARAM;
		});

	REG_API(abe, calc_set_abelian);

	REG_API(addd, calc_addd);
	REG_API(add,  calc_add);
	REG_API(subb, calc_subb);
	REG_API(sub,  calc_sub);
	REG_API(wak,  calc_wak);

	REG_API(cls, clearstrlist);

	REG_API(doexpr, calc_expr);
}
