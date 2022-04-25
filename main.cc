#include "com.hpp"
#include "SCEPHG.hpp"
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
		//out = "{'name':'ver', 'dat':[1, 0, 0, 4, 5, 6, 1, 0, 0, 4, 5, 6,11, 10, 10, 14, 25, 16]}";
	}
	else if (msg == "cmd")
	{
		ScePHG::dostring(body.c_str());
		PRINT("");
	}
}

// --------------------------------------------------------------------------------------
int main(int nargs, char* args[]) {
	if (nargs < 2)
		return 0;
	
	ScePHG::setup();
	ScePHG::dofile(args[1]);
	getchar();
	return 0;
}