#include "com.hpp"
extern std::string serverip;
#include "scene.hpp"
// --------------------------------------------------------------------------------------
void onrequest(const std::string& msg, const std::string& body, std::string& out)
{
	PRINT("onrequest: " << msg << "\n");
	if (msg == "hi")
	{
		stringstream ss;
		ss << "{'name':'ver', 'dat':[";
		if (!estack.empty())
		{
			for (int i = 0; i < estack.back().size(); i++)
			{
				auto& it = estack.back()[i];
				if (i > 0)
					ss << ",";
				ss << int(it.p.x * 100) << "," << int(it.p.y * 100) << "," << int(it.p.z * 100);
			}
		}
		ss << "]}";
		out = ss.str();
	}
	else if (msg == "cmd")
	{
		ScePHG::strlist.clear();
		ScePHG::dostring(body.c_str()); 

		/*stringstream jsn;
		jsn << "{\"" << "nodes" << "\":[\n";
		ScePHG::JSON_PARSER::tojson(ROOT, jsn);
		jsn << "]}\n";
		out = jsn.str();*/
		stringstream ss;
		for (int i = 0; i < ScePHG::strlist.size(); i++)
		{
			auto& it = ScePHG::strlist[i];
			//PRINT(it)
			if (i > 0)
				ss << " ";
			ss << it;
		}
		out = ss.str();
		PRINT(ss.str());
	}
}

// --------------------------------------------------------------------------------------
int main(int nargs, char* args[]) 
{
	ScePHG::setup();

	if (nargs >= 2)
	{
		ScePHG::dofile(args[1]);
	}
	else
	{
		PRINT("======= start http-server: localhost:8080\n");
		std::thread serverthread = std::thread{ servermain, 100 };
		getchar();
	}

	getchar();
	return 0;
}
