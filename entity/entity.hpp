/**************************************************************************
*				实体资源
*			实体资源是可绘制的场景对象
**************************************************************************/
struct tree_t;
namespace entity {
	struct transform_t { vec3 p; quaternion q; vec3 pyr; vec3 s = vec3::ONE; };
	struct enityres_t
	{
		int type = 1; // 1.subemsh 2. node
		// 携带的属性
		bool vis = true;	// 可见性
		union {
			struct { // submesh
				transform_t trans;	// 空间变换
				float angle;		// 相位角
				string md;		// 模型
				string cstr;		// 约束
				submesh sm;		// submesh
			};
			tree_t* node;
		};
		enityres_t() {}
		enityres_t(const enityres_t& v)
		{
			vis = v.vis;
			trans = v.trans;
			md = v.md;
			sm = v.sm;
		};
		~enityres_t() {}
	};
	vector<enityres_t*> reslist;			// 资源列表
	void clearres()
	{
		for (auto it : reslist)
		{
			if (it)
				delete it;
		}
		reslist.clear();
	}

	enityres_t& res(ENT& ent)
	{
		if (ent.resid == -1)
		{
			reslist.push_back(new enityres_t());
			ent.resid = reslist.size() - 1;
			//PRINT("create res id=" << resid);
		}
		ASSERT(ent.resid < reslist.size());
		return *reslist[ent.resid];
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
	vec3 stovec(crstr sval)
	{
		vec3 ret;
		sscanf(sval.c_str(), "%f,%f,%f", &ret.x, &ret.y, &ret.z);
		return ret;
	}

	void setup(tree_t* tree, const transform_t& parent)
	{
		work_stack.push_back(tree);

		ENT ent;
		transform_t& trans = res(ent).trans;
		{// transform
			vec3 p;
			quaternion q;
			vec3 pyr;
			vec3 s = vec3::ONE;
			{// transform desc
				KEY_VAL("pos") // raw position
				{
					p = stovec(it->second);
				}
				KEY_VAL("x") // move x
				{
					p += vec3::UX * storeal(it->second);
				}
				KEY_VAL("y") // move y
				{
					p += vec3::UY * storeal(it->second);
				}
				KEY_VAL("z") // move z
				{
					p += vec3::UZ * storeal(it->second);
				}
				KEY_VAL("pyr") // euler angles
				{
					pyr = stovec(it->second);
					q.fromeuler(pyr.x * PI / 180.0f, pyr.y * PI / 180.0f, pyr.z * PI / 180.0f);
				}
				KEY_VAL("pitch") // pitch
				{
					quaternion _q;
					real ang = storeal(it->second);
					_q.fromangleaxis(ang, vec::UX);
					q = _q * q;
				}
				KEY_VAL("yaw") // yaw
				{
					quaternion _q;
					real ang = storeal(it->second);
					_q.fromangleaxis(ang, vec::UY);
					q = _q * q;
				}
				KEY_VAL("roll") // roll
				{
					quaternion _q;
					real ang = storeal(it->second);
					_q.fromangleaxis(ang, vec::UZ);
					q = _q * q;
				}
				KEY_VAL("scl") // scale
				{
					s = stovec(it->second);

					PRINTVEC3(s);
				}
			}
			//PRINTVEC3(p);
			trans = {
				parent.p + (parent.q * vec3::UX) * p.x + (parent.q * vec3::UY) * p.y + (parent.q * vec3::UZ) * p.z,
				q * parent.q,
				parent.pyr + pyr, // 叠加？
				parent.s * s
			};
		}
		{// 添加到变量列表			

			KEY_VAL("md") {
				res(ent).md = it->second;
			}

			KEY_VAL("cstr") {
				res(ent).cstr = it->second;
			}

			//if(CEHCK_CONSTR(res(ent).cstr))
			gvarmapstack.addvar(tree->name.c_str(), ent);
		}

		// children
		for (auto it : tree->children) {
			setup(it.second, res(ent).trans);
		}
	}

}
