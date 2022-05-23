/**************************************************************************
*				节点树的解析
*			节点树就是把个体组成家族
*			包括阵列，序列, 选择子等重要概念
**************************************************************************/
#define NODE		tree_t
#define ROOT		ScePHG::gtree
#define ME			work_stack.empty() ? 0 : work_stack.back()
#define GET_NODE(name, node)	name == "me" ? ME : _gettree(name, node)

int node_count = 0;				// 节点数量

// 加前缀
inline void add_suffix(string& name, const string& clonename) {

	string suffix;
	int pos = clonename.find_last_of('.');
	if (pos != string::npos)
		suffix = clonename.substr(pos + 1);

	if (!suffix.empty() && atoi(suffix.c_str()) == 0)
		name += "." + suffix;
}

struct tree_t
{
	tree_t* parent = 0;
	string name;					// 名字
	int index = 0;					// 索引
	std::map<std::string, std::string> ab;		// 特性字典
	std::map<std::string, std::string> kv;		// 属性字典
	std::map<std::string, tree_t*> children;	// 子字典
	std::vector<tree_t*>	childrenlist;		// 子列表，用于json/xml等格式转化
			
	tree_t() {}

	static int getdepth(tree_t* tree, int depth = 0)
	{
		//PRINT("getdepth: " << tree->name);
		if (tree->parent)
			return getdepth(tree->parent, depth + 1);
		else
			return depth;
	}
	static inline int genid()
	{
		return ++ node_count;
	}

	inline int getindex()
	{
		return index;
		/*int id, index;
		PRINTV(name);
		ASSERT(2 == sscanf_s(name.c_str(), "%d_%d", &id, &index));
		return id;*/
	}
	void operator += (const tree_t& t)
	{
		// 暂时拷贝，以后可以做运算

		for (auto it : t.kv)
		{
			kv[it.first] = it.second;
		}
		for (auto& it : t.children)
		{
			tree_t* ntree = new tree_t();
			ntree->index = children.size() + 1;
			ntree->name = to_string(tree_t::genid()) + "_" + to_string(ntree->index);
			// add_suffix(ntree->name, it.first);
			children[ntree->name] = ntree;
			ntree->parent = this;

			*ntree += *it.second;
		}
	}

	static void clear(tree_t* ot)
	{
		if (!ot) return;

		for (auto it : ot->children)
		{
			clear(it.second);
		}
		delete(ot);
	}
};

// -----------------------------------------------------------------------
tree_t* gtree = 0;					// 暂时使用全局树
vector<tree_t*>	work_stack;				// 工作栈
std::string		cur_property = "pr1";		// 当前属性
extern void _crt_array(code& cd, tree_t* tree, const string& pre, int depth, const string& selector);
extern void _crt_sequ(code& cd, tree_t* tree, const string& pre);
extern tree_t* _gettree(const string& name, tree_t* tree);

// -----------------------------------------------------------------------
static void _tree(code& cd, tree_t* tree, const string& pre, int depth = 0)
{
	work_stack.push_back(tree);
	//PRINT(pre << "{");
	cd.next();

	std::string key, val;
	std::string* pstr = &key;
	string selector;
	while (!cd.eoc()) {
		char c = cd.cur();
		//PRINT("c=" << c );

		// 注解
		if (c == '#') {
			cd.nextline();
			//cd.next();
			continue;
		}

		// PHG表达式
		else if (c == '(' && pstr != &val)
		{
			int bracket_d = 1;
			string phg_expr = "";
			while (true) {
				char nc = cd.next();

				if (nc == '(')
					bracket_d++;
				else if (nc == ')')
					bracket_d--;

				if (bracket_d == 0)
					break;

				phg_expr.push_back(nc);
			}
			{// do expr
				work_stack.push_back(tree);
				ScePHG::dostring(phg_expr.c_str());
			}
			cd.next();
		}

		// 选择子
		else if (c == '?' && pstr == &key) {
			cd.next();
			selector = getstring(cd,'[');
			cd.ptr--; // move back
		}

		// 节点开始
		else if (c == '{' || c == '[' || c == '<') {
			if (!val.empty()){
				if (key.empty())
					key = "pr" + to_string(tree->kv.size() + 1); // default porperty name
				tree->kv[key] = val;
			}
			
			if (c == '{') {
				if (val != "")
				{
					SYNTAXERR("missing ';' to end an property(k:v) line!");
					cd.ptr = 0;
				}
				if (key.empty())
				{
					key = to_string(tree_t::genid()) + "_" + to_string(tree->children.size() + 1);
				}
				tree_t* ntree = new tree_t;
				ntree->index = tree->children.size() + 1;
				ntree->name = key;
				tree->children[key] = ntree;
				ntree->parent = tree;

				//PRINT(pre << key << " : ");
				_tree(cd, ntree, pre + "\t", depth + 1);
			}
			else if (c == '[') // 阵列
			{
				_crt_array(cd, tree, pre, depth + 1, selector);
			}
			else if (c == '<') // 序列
			{
				_crt_sequ(cd, tree, pre);
			}
			val = "";
			key = "";
			pstr = &key;
		}

		// 节点结束
		else if (c == ';' || c == '}' || c == '>' || c == '\n' || c == '\r') {
			if (!key.empty() || !val.empty()) {
				if (val.empty())
				{// inhert
					tree_t* t = GET_NODE(key, ROOT);
					if (t) {
						//PRINT("inhert0: " << key);
						//add_suffix(tree->name, key);
						(*tree) += (*t);
					}
				}
				else
				{
					tree->kv[key] = val;
					//PRINT(pre << key << " : " << val);
				}
				val = "";
				key = "";
				pstr = &key;
			}
			cd.next();
			if (c == '}' || c == '>') {
				return;
			}
		}

		// 逗号间隔,不能作为property的结尾！
		else if (c == ',' && pstr != &val) {
			if (!key.empty()) 
			{// inhert
				tree_t* t = GET_NODE(key, ROOT);
				if (t)
				{
					//add_suffix(tree->name, key);
					//PRINT("inhert1: " << key);
					(*tree) += (*t);
				}
			}
			val = "";
			key = "";
			pstr = &key;

			cd.next();
		}

		// 名字 与 数值/子节点
		else if (c == ':') {
			if (key.empty()) 
			{
				//SYNTAXERR("key is missing before ':' !");cd.ptr = 0;return;
				key = "pr" + to_string(tree->kv.size() + 1); // default porperty name
			}
			if (pstr == &val) {
				SYNTAXERR("':' is in value!");
				cd.ptr = 0;
				return;
			}
			pstr = &val;
			cd.next4();
		}

		// 字符串
		else if (c == '\'' || c == '\"') {

			cd.next();
			*pstr += getstring(cd);
		}

		// default
		else {
			*pstr += cd.cur();
			cd.next4();
		}
	}
}

// get tree by name
static tree_t* _gettree(const string& name, tree_t* tree = gtree)
{
	{
		auto it = tree->children.find(name);
		if (it != tree->children.end())
		{
			return it->second;
		}
	}
	for (auto it : tree->children) {
		tree_t* ret = _gettree(name, it.second);
		if (ret)
			return ret;
	}
	return 0;
}
int select(int ind, int rnd, crstr selector)
{
	if (selector.empty())
		return 2; // select all

	int pa, len;
	sscanf_s(selector.c_str(), "%d/%d", &pa, &len);
	ASSERT(len != 0);

	if (rnd % len == ind) // 随机选择一个
	{
		PRINT("selected!")
		return 1; // select one
	}

	PRINT("select failed! " << ind)
	return 0;
}

// 阵列
static void _crt_array(code& cd, tree_t* tree, const string& pre, int depth, const string& selector)
{
	cd.next();
	int index = 0;
	int rnd = rand();
	//vector<string> nodes;
	string node;
	while (!cd.eoc()) {
		char c = cd.cur();
		//PRINT(c);
		tree_t* ntree = 0;
		if (c == '#') {
			cd.nextline();
			cd.next();
			continue;
		}
		else if (c == '<' || c == '>') {
			SYNTAXERR("'<' or '>' is not allowed in '[]', use '{}'!");
			cd.ptr = 0;
			return;
		}
		else if (c == ':')
		{
			if (node == "") {
				SYNTAXERR("key is missing before ':'!");
				cd.ptr = 0;
				return;
			}
		}
		else if (c == '{')
		{
			index++;
			if (node.empty())
			{
				node = to_string(tree_t::genid()) + "." + to_string(tree->children.size()+1);
			}
			work_stack.push_back(tree);
			ntree = new tree_t;
			ntree->index = tree->children.size() + 1;
			ntree->name = node;

			tree->children[ntree->name] = ntree;
			ntree->parent = tree;

			//PRINT("_crt_array >>");
			_tree(cd, ntree, pre, depth + 1);
			// PRINT("_crt_array << " << cd.cur())
			if (int ret = select(index, rnd, selector); ret) {
				if (ret == 1)
				{
					cd.next();
					return;
				}
			}
			else
			{
				tree->children[ntree->name] = 0;
				ntree->parent = 0;
				tree_t::clear(ntree);
			}
			node = "";
		}
		else if (c == ',' || c == ']')
		{
		}
		else
			node += c;

		c = cd.cur();
		if (c == ',' || c == ']')
		{
			if (!ntree)
				index++;

			//nodes.push_back(node);
			if (!ntree)
			{// create node
				work_stack.push_back(tree);
				ntree = new tree_t;
				ntree->index = tree->children.size() + 1;
				ntree->name = to_string(tree_t::genid()) + "_" + to_string(ntree->index);
				{// inhert
					tree_t* t = GET_NODE(node, ROOT);
					if (t) {
						//PRINT("inhert2: " << node);
						// add_suffix(tree->name, node);
						(*ntree) += (*t);
					}
				}
				
				//PRINTV(ntree->name)
				if (int ret = select(index, rnd, selector); ret) {

					tree->children[ntree->name] = ntree;
					ntree->parent = tree;
					if (ret == 1)
					{
						cd.next();
						PRINTV(ret)
						return;
					}
				}
				else
				{
					tree_t::clear(ntree);
				}
				ntree = 0; // clear
			}
			node = "";

			if (c == ']')
			{
				cd.next();
				return;
			}
		}

		cd.next();
	}
}

// 序列
static void _crt_sequ(code& cd, tree_t* tree, const string& pre)
{
	cd.next();
	
	//vector<string> nodes;
	string node;
	while (!cd.eoc()) {
		char c = cd.cur();
		//PRINT(c)
		tree_t* ntree = 0;
		if (c == '#') {
			cd.nextline();
			cd.next();
			continue;
		}
		else if (c == '[' || c == ']') {
			SYNTAXERR("'[' or ']' is not allowed in '<>', use '{}'!");
			cd.ptr = 0;
			return;
		}
		else if (c == ':')
		{
			if (node == "") {
				SYNTAXERR("key is missing before ':'!");
				cd.ptr = 0;
				return;
			}
		}
		else if (c == '{')
		{
			if (node.empty())
			{
				node = to_string(tree_t::genid()) + "_" + to_string(tree->children.size() + 1);
			}

			work_stack.push_back(tree);
			ntree = new tree_t;
			ntree->index = tree->children.size() + 1;
			ntree->name = node;
			tree->children[ntree->name] = ntree;
			ntree->parent = tree;

			PRINT(pre << ntree->name << " : ");

			_tree(cd, ntree, pre, tree_t::getdepth(tree) + 1);
			tree = ntree; // parent->child
			//PRINTV(cd.cur())
			node = "";
		}
		else if (c == ',' || c == '>')
		{
		}
		else
			node += c;

		c = cd.cur();
		if (c == ',' || c == '>')
		{
			//nodes.push_back(node);
			if (!ntree)
			{// create node
				work_stack.push_back(tree);
				ntree = new tree_t;
				ntree->index = tree->children.size() + 1;
				ntree->name = to_string(tree_t::genid()) + "_" + to_string(tree->children.size() + 1);
				{// inhert
					tree_t* t = GET_NODE(node, ROOT);
					if (t) {
						//PRINT("inhert3: " << node)
						// add_suffix(tree->name, node);
						(*ntree) += (*t);
					}
				}
				tree->children[ntree->name] = ntree;
				ntree->parent = tree;
				tree = ntree; // parent->child

				ntree = 0; // clear
			}
			node = "";

			if (c == '>')
			{
				cd.next();
				return;
			}
		}
		cd.next();
	}
}

// 入口
void _tree(code& cd)
{
	//tree_t* tree = new tree_t;
	tree_t::clear(gtree);
	gtree = new tree_t;
	gtree->name = "root";
	_tree(cd, gtree, "", 0);

	//tree_t::clear(tree);
}

// ------------------------------------
// 寻源式加法
// 两个节点之和为最近的相同祖先
// ------------------------------------
bool porperty_intree(tree_t* tree, const char* key, crstr val)
{
	if (auto& it = tree->kv.find(key); it != tree->kv.end())
	{
		return  it->second == val;
	}
	// children
	for (auto it : tree->children) {
		if (porperty_intree(it.second, key, val))
		{
			return true;
		}
	}
	return false;
}

// 在节点树上搜索加法规则
tree_t* walk_addtreeEX(tree_t* tree, crstr v_a, crstr v_b, const char* key)
{
	// children
	for (auto it : tree->children) {
		if (porperty_intree(tree, key, v_a) && porperty_intree(tree, key, v_b)) {

			return walk_addtreeEX(it.second, v_a, v_b, key);
		}
	}
	return tree;
}

const char* walk_addtree(tree_t* tree, crstr v_a, crstr v_b, const char* key)
{
	if (tree->children.size() >= 2)
	{
		int findcnt = 0;
		for (auto it : tree->children) {
			if (it.second->children.empty() && it.second->kv[key] == v_a)
			{
				findcnt++;
			}
			if (it.second->children.empty() && it.second->kv[key] == v_b)
			{
				findcnt++;
			}
		}

		if (findcnt == 2)
		{
			return tree->kv[key].c_str();
		}
	}

	for (auto it : tree->children) {
		if (const char* c = walk_addtree(it.second, v_a, v_b, key); c != 0)
		{
			return c;
		}
	}
	return 0;
}

// ------------------------------------
// node walker
// ------------------------------------
void node_walker(tree_t* tree, std::function<void(tree_t*)> fun)
{
	fun(tree);

	// children
	for (auto& it : tree->children) {
		node_walker(it.second, fun);
	}
}

// ====================================
// API
// ====================================
API(api_me)
{
	ASSERT(args == 1);
	
	SPARAM(node);
	NODE* me = _gettree(node, ROOT);
	if(me)
		work_stack.push_back(me);
	return 0;
}
API(api_bye)
{
	work_stack.clear();
	return 0;
}

API(api_on)
{
	ASSERT(args == 1);

	SPARAM(on);
	cur_property = on;

	return 0;
}
API(array)
{
	ASSERT(ME);
	tree_t* ntree = new tree_t;
	
	if (args == 1)
	{
		SPARAM(clonenode);
		tree_t* t = _gettree(clonenode);
		if (t)
		{
			// add_suffix(ntree->name, clonenode);
			(*ntree) += (*t);
		}
	}
	if (ME->parent && (!cd.iter.empty() && cd.iter.back() > 1))
	{
		int id = tree_t::genid();
		ntree->name = to_string(id) + "_" + to_string(ME->parent->children.size() + 1);
		ntree->index = ME->parent->children.size() + 1;
		ME->parent->children[ntree->name] = ntree;
		ntree->parent = ME->parent;
	}
	else
	{
		int id = tree_t::genid();
		ntree->name = to_string(id) + "_" + to_string(ME->children.size() + 1);
		ntree->index = ME->children.size() + 1;
		ME->children[ntree->name] = ntree;
		ntree->parent = ME;
	}
	ME = ntree;

	return 0;
}
API(sequ)
{
	ASSERT(ME);
	tree_t* ntree = new tree_t;
	ntree->index = ME->children.size() + 1;
	int id = tree_t::genid();
	ntree->name = to_string(id) + "_" + to_string(ME->children.size() + 1);
	
	if (args == 1)
	{
		SPARAM(clonenode);
		tree_t* t = _gettree(clonenode, ROOT);
		if (t)
		{
			PRINTV(clonenode);
			// add_suffix(ntree->name, clonenode);
			(*ntree) += (*t);
		}
	}
	{
		ME->children[ntree->name] = ntree;
		ntree->parent = ME;
	}
	work_stack.push_back(ntree);

	return 0;
}
void property(tree_t* tree, const string& key, const string& val, const string& filter = "")
{
	const char* p = 0;
	if(!filter.empty())
		p = filter.c_str();
	if (p == 0 ||
		(*p) != '!' && tree->name.find(filter) != std::string::npos ||
		(*p) == '!' && tree->name.find(p + 1) == std::string::npos)
		tree->kv[key] = val;

	// children
	for (auto& it : tree->children) {
		property(it.second, key, val, filter);
	}
}
API(property)
{
	string& key = GET_SPARAM(1);
	string& val = GET_SPARAM(2);
	string filter = "";
	if(args >= 3)
		filter = GET_SPARAM(3);
	property(ROOT, key, val, filter);

	POP_SPARAM;
	return 0;
}

void dump(tree_t* tree, const string& pre = "")
{
	PRINT(pre << tree->name << ":{")
	{
		for (auto& it : tree->kv)
		{
			PRINT(pre << "\t" << it.first << ":" << it.second);
		}
	}
	// children
	for (auto& it : tree->children) {
		dump(it.second, pre + "\t");
	}
	PRINT(pre << "}")
}
API(dump)
{
	PRINT("------------- DUMP ----------------");
	if (ROOT)
		dump(ROOT);
	return 0;
}

// -----------------------------------
// 数据输出 I/O
// -----------------------------------
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
	sscanf_s(str.c_str(), "{x:%d,y:%d,width:%d,height:%d}", &rect.x, &rect.y, &rect.width, &rect.height);
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
				string str = it->second;
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

// ===================================
// REG APIs
// ===================================
void NODE_REG_API()
{
	REG_API(iam, api_me);			// ME
	REG_API(bye, api_bye);			// ME = NULL(正在放弃中...)
	REG_API(on, api_on);			// 当前属性 (正在放弃中...)
	REG_API(array, array);			// 节点阵列 (正在放弃中...)
	REG_API(sequ, sequ);			// 节点序列 (正在放弃中...)

	REG_API(prop, property);		// 添加属性 (正在放弃中...)

	/// stream io
	
	REG_API(getival, getival);		// 获得int value
	REG_API(getfval, getfval);		// 获得float value
	REG_API(getstr, getstr);		// 获得string
	REG_API(getvec3, getvec3);		// 获得vec3
	REG_API(getrect, getrect);		// 获得RECT

	REG_API(dump, dump);
}
