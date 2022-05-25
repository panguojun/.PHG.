/**********************************************************************************
*						使用PHG来解析JSON
*						暂时不能解析浮点数表达式如1E-2
*						解析完毕形成一棵树加上属性KV
**********************************************************************************/

namespace JSON_PARSER
{
	inline std::string read_arraynumbers(code& cd)
	{
		cd.next();
		std::string content;
		while (!cd.eoc()) {
			char c = cd.cur();
			if (c != ']')
			{
				content += c;
				cd.next();
				continue;
			}
			break;
		}
		return content;
	}

	inline void _walk_JSON(code& cd, tree_t* tree, const string& pre, int depth = 0)
	{
		PRINT(pre << "{");
		cd.next();

		std::string key, val;
		std::string* pstr = &key;
		int index = 0;
		while (!cd.eoc()) {
			char c = cd.cur();
			//PRINT("c=" << c );
			if (c == '#') {
				cd.nextline();
				//cd.next();
				continue;
			}
			else if (c == '(')
			{
				int bracket_depth = 1;
				string phg_expr = "";
				while (true) {
					char nc = cd.next();

					if (nc == '(')
						bracket_depth++;
					else if (nc == ')')
						bracket_depth--;

					if (bracket_depth == 0)
						break;

					phg_expr.push_back(nc);
				}
				PRINTV(phg_expr);
				{// do expr
					work_stack.push_back(tree);
					ScePHG::dostring(phg_expr.c_str());
				}
				cd.next();
			}
			else if (depth != 0 && c == '[')
			{
				*pstr += read_arraynumbers(cd);
				cd.next();
			}
			else if (c == '{' || c == '<' || c == '[') {
				index++;
				if (key.empty())
				{
					key = to_string(depth + 1) + "." + to_string(index);
				}
				{
					tree_t* ntree = new tree_t;
					ntree->name = key;
					tree->children[key] = ntree;
					ntree->parent = tree;

					PRINT(pre << key << " : ");
					_walk_JSON(cd, ntree, pre + "\t", depth + 1);
				}

				val = "";
				key = "";
				pstr = &key;
			}
			else if (c == ',' || c == ';' || c == '}' || c == ']' || c == '>' || c == '\n' || c == '\r') {
				if (!key.empty() || !val.empty()) {

					index++;
					if (val.empty())
					{
						val = key;
						key = to_string(index);
					}
					//PRINTV(val);
					tree->kv[key] = val;
					PRINT(pre << key << " : " << val);

					val = "";
					key = "";
					pstr = &key;

				}
				cd.next();
				if (c == '}' || c == ']') {
					PRINT(pre << "}");
					return;
				}
			}
			else if (c == ':') {

				pstr = &val;
				cd.next4();
			}
			else if (c == '\'' || c == '\"') {

				cd.next();
				*pstr += getstring(cd);
			}
			else {
				*pstr += cd.cur();
				cd.next4();
			}
		}
	}

	tree_t* jsontree = 0;

	inline void _walk_JSON(code& cd)
	{
		tree_t::clear(jsontree);
		jsontree = new tree_t;
		jsontree->name = "root";
		_walk_JSON(cd, jsontree, "", 0);
	}

	// to JSON raw mode
	void tojson_raw(tree_t* tree, std::stringstream& jsn, const string& pre = "")
	{
		if (jsn.str().back() == '}')
			jsn << ",\n";

		if (tree->parent == 0)
			jsn << "{\n";
		else
			jsn << pre << "\"" << fixedname(tree->name) << "\":{\n";
		{
			for (auto& it : tree->kv)
			{
				{
					if (jsn.str().back() == '\"')
						jsn << ",\n";
					if (it.second.front() == '{')
						jsn << pre << "\t\"" << fixedname(it.first) << "\":" << it.second;
					else
						jsn << pre << "\t\"" << fixedname(it.first) << "\":\"" << fixedname(it.second) << "\"";
				}
			}
		}
		if (jsn.str().back() == '\"' && tree->children.size() > 0)
			jsn << ",\n";

		// children
		for (auto& it : tree->children) {
			tojson_raw(it.second, jsn, pre + "\t");
		}
		jsn << "\n" << pre << "}";
	}

	// to JSON raw mode with list
	void tojson_raw_list(tree_t* tree, std::stringstream& jsn, const string& pre = "")
	{
		if (jsn.str().back() == '}' || jsn.str().back() == ']')
			jsn << ",\n";

		if (tree->parent == 0) // 根节点
			jsn << "{\n";
		else 
		{
			// 数组
			if (tree->parent->childrenlist.empty())
			{
				jsn << pre << "\"" << fixedname(tree->name) << "\":[\n";
			}

			// 带有属性
			if (tree->childrenlist.empty() || !tree->kv.empty())
			{
				if (jsn.str().back() != '[')
					jsn << pre;
				jsn << "{\n";
			}
		}

		{// 属性
			for (auto& it : tree->kv)
			{
				{
					if (jsn.str().back() == '\"')
						jsn << ",\n";
					{
						if (it.second.front() == '{')
							jsn << pre << "\t\"" << fixedname(it.first) << "\":" << it.second;
						else
							jsn << pre << "\t\"" << fixedname(it.first) << "\":\"" << fixedname(it.second) << "\"";
					}
				}
			}
		}

		// 在属性与子节点之间
		if (jsn.str().back() == '\"' && tree->children.size() > 0)
			jsn << ",\n";

		// 数组模式
		if (!tree->childrenlist.empty())
		{
			if (!tree->kv.empty())
			{
				jsn << ",\n" << pre << "\"" << "array" << "\":[\n";
			}
		}
		// children
		for (auto& it : tree->children) {
			tojson_raw_list(it.second, jsn, pre + "\t");
		}

		// 数组模式收尾
		if (!tree->childrenlist.empty())
		{
			jsn << "\n" << pre << "]";
		}

		// 收尾
		if (tree->childrenlist.empty() || !tree->kv.empty())
		{
			if (jsn.str().back() == ']')
			{
				jsn << "}";
			}
			else
				jsn << "\n" << pre << "}";
		}
	}

	void tojson_raw(NODE* me)
	{
		stringstream jsn;
		tojson_raw_list(me, jsn);

		PRINT(jsn.str());
	}

	void _paserJSON(NODE* me)
	{
		
	}

	// from JSON
	void fromJSON(NODE* me, const string& json)
	{
		tree = _walk_JSON;
		ScePHG::dostring(json.c_str());

		_paserJSON(me);
	}
}