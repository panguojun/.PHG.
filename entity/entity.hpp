/**************************************************************************
*							ʵ����Դ
*					ʵ����Դ�ǿɻ��Ƶĳ�������
**************************************************************************/
#define	MODEL res(ent).md

struct tree_t;
namespace entity {
	struct transform_t { vec3 p; quaternion q; vec3 pyr; vec3 s = vec3::ONE; };
	struct enityres_t
	{
		int type = 1; // 1.subemsh 2. node
		// Я��������
		bool vis = true;	// �ɼ���
		transform_t trans;	// �ռ�任
		float angle;		// ��λ��
		string md;			// ģ��
		string cstr;		// Լ��
		submesh sm;			// submesh
	
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
	vector<enityres_t*> reslist;			// ��Դ�б�
	void clearres()
	{
		for (auto it : reslist)
		{
			if (it)
				delete it;
		}
		reslist.clear();
	}

	inline	enityres_t& res(ENT& ent)
	{
		if (ent.resid == -1)
		{
			reslist.push_back(new enityres_t());
			ent.resid = reslist.size() - 1;
			//PRINT("create res id=" << resid);
		}
		if (ent.resid >= reslist.size())
		{
			throw(std::runtime_error("������ setup()��������"));
		}
		return *reslist[ent.resid];
	}


	// *********************************************************************
	// Setup tree, generate entities
	// *********************************************************************
#define KEY_VAL(val) if (auto& it = tree->kv.find(val); it != tree->kv.end())

	// data convert
	inline int stoint(crstr sval)
	{
		return atoi(sval.c_str());
	}
	inline real storeal(crstr sval)
	{
		return atof(sval.c_str());
	}
	inline vec3 stovec(crstr sval)
	{
		vec3 ret;
		sscanf(sval.c_str(), "%f,%f,%f", &ret.x, &ret.y, &ret.z);
		return ret;
	}
	inline quaternion stoq(crstr sval)
	{
		quaternion ret;
		sscanf(sval.c_str(), "%f,%f,%f,%f", &ret.x, &ret.y, &ret.z, &ret.w);
		return ret;
	}
	inline string v3tos(crvec v)
	{
		stringstream ss;
		ss << v.x << "," << v.y << "," << v.z;
		return ss.str();
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
				KEY_VAL("p") // raw position
				{
					p += stovec(it->second);
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
				KEY_VAL("pit") // pitch
				{
					quaternion _q;
					real ang = storeal(it->second) * PI / 180.0f;
					_q.fromangleaxis(ang, vec::UX);
					q = _q * q;
				}
				KEY_VAL("yaw") // yaw
				{
					quaternion _q;
					real ang = storeal(it->second) * PI / 180.0f;
					_q.fromangleaxis(ang, vec::UY);
					q = _q * q;
				}
				KEY_VAL("rol") // roll
				{
					quaternion _q;
					real ang = storeal(it->second) * PI / 180.0f;
					_q.fromangleaxis(ang, vec::UZ);
					q = _q * q;
				}
				KEY_VAL("q") // q
				{
					quaternion _q;
					_q = stoq(it->second);
					q = _q * q;
				}
				KEY_VAL("s") // scale
				{
					s = storeal(it->second);
				}
			}
			//PRINTVEC3(p);
			trans = {
				parent.p + (parent.q * vec3::UX) * parent.s.x * p.x + (parent.q * vec3::UY) * parent.s.y * p.y + (parent.q * vec3::UZ) * parent.s.z * p.z,
				q * parent.q,
				parent.pyr + pyr, // ���ӣ�
				parent.s * s
			};
			tree->kv["p"] = v3tos(trans.p);
		}
		{// ��ӵ������б�
			//ent.sval = tree->name;
			KEY_VAL("vis") // vis
			{
				if (it->second == "false")
				{
					res(ent).vis = false;
				}
			}
			KEY_VAL("cstr") {
				res(ent).cstr = it->second;
			}
			gvarmapstack.addvar(tree->name.c_str(), ent);
		}

		// children
		for (auto it : tree->children) {
			setup(it.second, trans);
		}
	}
}

// ------------------------------------
// API
// ------------------------------------
void ENTITY_REG_API()
{
}