/**************************************************************************
*				事件运算

**************************************************************************/
struct tree_t;
namespace calc
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
	int _walk_tree_add(tree_t** out, tree_t* tree, crstr a, crstr b, const char* key)
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
			int ret = _walk_tree_add(out, it.second, a, b, key);
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

	void _calc_add(tree_t** out, crstr a, crstr b, const char* key)
	{
		_walk_tree_add(out, ROOT, a, b, key);
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
	int _wak_tree_add(tree_t* tree, crstr a, crstr b, const char* key, const char* ok)
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
			int ret = _wak_tree_add(it.second, a, b, key, ok);
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
}

// ------------------------------------
// API
// ------------------------------------
API(calc_set_abelian)
{
	crstr b = GET_SPARAM(1);

	calc::abelian_sym = atoi(b.c_str());
	PRINTV(calc::abelian_sym)

	POP_SPARAM; return 0;
}
API(calc_addd)
{
	crstr a = GET_SPARAM(1);
	string c;
	calc::_calc_addd(c, a, cur_property.c_str());

	PRINTV(c);
	strlist.push_back(c);

	POP_SPARAM; return 0;
}
API(calc_add)
{
	string a = GET_SPARAM(1);
	string b = GET_SPARAM(2);
	/*{
		NODE* an = GET_NODE(a, ROOT); ASSERT(an);
		NODE* bn = GET_NODE(b, ROOT); ASSERT(bn);
		a = an->kv[cur_property];
		b = bn->kv[cur_property];
	}*/

	NODE* n = 0;
	string c;
	if (a == b) {
		c = a;
	}
	else {
		calc::_calc_add(&n,
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
	calc::_calc_subb(c, a, cur_property.c_str());

	PRINTV(c);
	strlist.push_back(c);

	POP_SPARAM; return 0;
}
API(calc_sub)
{
	crstr a = GET_SPARAM(1);
	crstr b = GET_SPARAM(2);
	string c;
	calc::_calc_sub(c, a, b, cur_property.c_str());

	PRINTV(c);
	strlist.push_back(c);

	POP_SPARAM; return 0;
}
API(clearstrlist)
{
	strlist.clear();

	return 0;
}
API(wak)
{
	crstr nm = GET_SPARAM(1);
	NODE* n = nm == "me" ? ME : GET_NODE(nm, ROOT); ASSERT(n);
	if (args == 3) {
		crstr a = GET_SPARAM(2);
		crstr ok = GET_SPARAM(3);

		calc::_wak_tree(n, a, cur_property.c_str(), ok.c_str());
	}
	else if (args == 4)
	{
		crstr a = GET_SPARAM(2);
		crstr b = GET_SPARAM(3);
		crstr ok = GET_SPARAM(4);

		calc::_wak_tree_add(n, a, b, cur_property.c_str(), ok.c_str());

	}

	POP_SPARAM; return 0;
}

void NODECALC_REG_API()
{
	CALC([](code& cd, char o, int args)->string {

		PRINT("CALC: " << o << "(" << args << ")");
		//PRINTV(ME->name);
		/*if (o == '+')
		{
			crstr a = GET_SPARAM(1);
			crstr b = GET_SPARAM(2);
			string c;
			if (a == b) {
				c = a;
			}
			else
			{
				NODE* n = 0;
				calc::_calc_add(&n,
					a, b,
					cur_property.c_str());
				if (n) {
					c = n->kv[cur_property];

					PRINTV(n->name);
					work_stack.push_back(n);
				}
			}
			strlist.push_back(c);
			return c;
		}
		else */
		if (o == '.')
		{
			crstr a = GET_SPARAM(1);
			crstr b = GET_SPARAM(2);
			NODE* n = a == "me" ? ME : GET_NODE(a, ROOT); ASSERT(n);
			string c = n->kv[b];

			strlist.push_back(c);
			return c;
		}
		return "";
	});

	REG_API(abe, calc_set_abelian);

	REG_API(addd, calc_addd);
	REG_API(add, calc_add);
	REG_API(subb, calc_subb);
	REG_API(sub, calc_sub);

	REG_API(cls, clearstrlist);
	REG_API(wak, wak);
}
