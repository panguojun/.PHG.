/**************************************************************************
*						节点树的解析
*					节点树就是把个体组成家族
*					包括阵列，序列, 选择子等重要概念
**************************************************************************/
#define NODE		tree_t
#define ROOT		ScePHG::gtree
#define ME			work_stack.back()
#define GET_NODE(key, node)	key == "me" ? ME : _gettree(key, node)

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
	string name;								// 名字
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

	void operator += (const tree_t& t)
	{
		// 暂时拷贝，以后可以做运算

		for (auto it : t.kv)
		{
			kv[it.first] = it.second;
		}
		int depth = getdepth(this);
		for (auto& it : t.children)
		{
			tree_t* ntree = new tree_t();
			
			ntree->name = to_string(depth + 1) + "_" + to_string(children.size() + 1);
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
tree_t* gtree = 0;				// 暂时使用全局树
vector<tree_t*>	work_stack;		// 工作栈

extern void _crt_array(code& cd, tree_t* tree, const string& pre, int depth, const string& selector);
extern void _crt_sequ(code& cd, tree_t* tree, const string& pre, int depth);
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
	int index = 0;
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
			if (key.empty())
			{
				key = to_string(depth + 1) + "_" + to_string(tree->children.size() + 1);
			}
			
			if (c == '{') {
				{
					if (val != "")
					{
						SYNTAXERR("missing ';' to end an property(k:v) line!");
						cd.ptr = 0;
					}
				}
				tree_t* ntree = new tree_t;
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
				_crt_sequ(cd, tree, pre, depth + 1);
			}
			val = "";
			key = "";
			pstr = &key;
		}

		// 节点结束
		else if (c == ';' || c == '}' || c == '>' || c == '\n' || c == '\r') {
			if (!key.empty() || !val.empty()) {

				tree_t* t = 0;
				if (val.empty())
				{// inhert
					t = GET_NODE(key, ROOT);
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
			{// inhert
				if (key.empty()) key = "me";
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
				node = to_string(depth + 1) + "." + to_string(index);
			}
			work_stack.push_back(tree);
			ntree = new tree_t;
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
			index++;

			//nodes.push_back(node);
			if (!ntree)
			{// create node
				work_stack.push_back(tree);
				ntree = new tree_t;
				ntree->name = to_string(depth + 1) + "_" + to_string(index);
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
static void _crt_sequ(code& cd, tree_t* tree, const string& pre, int depth)
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
			depth = tree_t::getdepth(tree);
			if (node.empty())
			{
				node = to_string(depth + 1) + "_" + to_string(tree->children.size() + 1);
			}

			work_stack.push_back(tree);
			ntree = new tree_t;
			ntree->name = node;
			tree->children[ntree->name] = ntree;
			ntree->parent = tree;

			PRINT(pre << ntree->name << " : ");

			_tree(cd, ntree, pre, depth + 1);
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
				depth = tree_t::getdepth(tree);
				ntree->name = to_string(depth + 1) + "_" + to_string(tree->children.size() + 1);
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
// API
// ------------------------------------
API(cur)
{
	ASSERT(args == 1);
	
	SPARAM(node);
	NODE* me = _gettree(node, ROOT);
	if(me)
		work_stack.push_back(me);
	return 0;
}
API(array)
{
	ASSERT(ME);
	tree_t* ntree = new tree_t;
	int depth = tree_t::getdepth(ME);

	ntree->name = to_string(depth + 1) + "_" + to_string(ME->children.size() + 1);
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
		ME->parent->children[ntree->name] = ntree;
		ntree->parent = ME->parent;
	}
	else
	{
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
	int depth = tree_t::getdepth(ME);
	ntree->name = to_string(depth + 1) + "_" + to_string(ME->children.size() + 1);
	
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
void addprop(tree_t* tree, const string& key, const string& val, const string& filter = "")
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
		addprop(it.second, key, val, filter);
	}
}
API(addprop)
{
	string& key = GET_SPARAM(1);
	string& val = GET_SPARAM(2);
	string filter = "";
	if(args >= 3)
		filter = GET_SPARAM(3);
	addprop(ROOT, key, val, filter);

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

// node walker
void node_walker(tree_t* tree, std::function<void(tree_t*)> fun)
{
	fun(tree);

	// children
	for (auto& it : tree->children) {
		node_walker(it.second, fun);
	}
}
