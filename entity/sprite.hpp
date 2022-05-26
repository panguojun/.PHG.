/**************************************************************************
*					精灵
*				可绘制的2D场景对象
**************************************************************************/
struct tree_t;
inline string vec3tos(crvec v)
{
	stringstream ss;
	ss << "{\"x\":" << v.x << ",\"y\":" << v.y << ",\"z\":" << v.z << "}";
	return ss.str();
}
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

	
	void clearres()
	{
		for (auto it : reslist)
			if (it) delete it;
		reslist.clear();
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
		ASSERT(tree);
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
}

// ------------------------------------
// API
// ------------------------------------
API(getspriteloc)
{
	vec3list.clear();
	strlist.clear();
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
				strlist.push_back(vec3tos(vec3(t.p.x, t.p.y, t.ang)));	
				vec3list.emplace_back(t.p.x, t.p.y, t.ang);
			}
		});

	PRINTV(vec3list.size());
	POP_SPARAM;
	return 0;
}
void SPRITE_REG_API()
{
	REG_API(getsprloc, getspriteloc);	// 获得get sprite loc
}
