/**********************************************************************************
*						解析XML文件
*						TODO: 如何通用化？
**********************************************************************************/
inline string tovec3(crvec v)
{
	stringstream ss;
	ss << "{\"x\":" << v.x << ",\"y\":" << v.y << ",\"z\":" << v.z << "}";
	return ss.str();
}
inline string torect(int x, int y, int width, int height)
{
	stringstream ss;
	ss << "{\"x\":\"" << x << "\",\"y\":\"" << y << "\",\"width\":\"" << width << "\",\"height\":\"" << height << "\"}";
	return ss.str();
}

bool fromXML(crstr filename, NODE* me)
{
	//std::filesystem::path resFolderPath = PlatformFramework::GetSingleton()->GetResourceFolderPath();
	std::string fullpath = "C:\\Users\\18858\\Documents\\_LAB\\ZEXE\\DATA\\" + filename;
	PRINT(fullpath);
	TiXmlDocument doc(fullpath.c_str());
	if (!doc.LoadFile())
		return false;

	NODE* cabgroup = new NODE;
	me->children["Cabinets"] = cabgroup;
	cabgroup->parent = me;
	cabgroup->name = "Cabinets";
	
	TiXmlElement* elem_cab = doc.RootElement()->FirstChildElement("Cabinet");
	int cabindex = 0;
	while (elem_cab)
	{
		PRINT("----------------Cabinet------------------")
		PRINT(elem_cab->Attribute("name"));
		NODE* cab = new NODE;
		//string name = std::string("\'") + elem_cab->Attribute("name") + to_string(++cabindex) + "\'";
		cab->name = "Cabinet" + to_string(++cabindex);
		cabgroup->children[cab->name] = cab;
		cab->parent = cabgroup;
		cabgroup->childrenlist.push_back(cab);
		{
			vec2 sz;
			vec3 pos;
			sscanf(elem_cab->Attribute("x"), "%f", &(pos.x));
			sscanf(elem_cab->Attribute("y"), "%f", &(pos.z));
			sscanf(elem_cab->Attribute("width"), "%f", &(sz.x));
			sscanf(elem_cab->Attribute("length"), "%f", &(sz.y));
			cab->kv["rect"] = torect(pos.x, pos.z, sz.x, sz.y);
		}

		int devindex = 0;
		TiXmlElement* elem_dev = elem_cab->FirstChildElement("Device");
		while (elem_dev)
		{
			std::string name = std::string("\'") + elem_dev->Attribute("name") + to_string(++devindex) + "\'";
			{
				NODE* dev = new NODE;
				cab->children[name] = dev;
				cab->childrenlist.push_back(dev);
				dev->parent = cab;
				dev->name = name;

				vec2 sz;
				vec3 pos;
				sscanf(elem_dev->Attribute("x"), "%f", &(pos.x));
				sscanf(elem_dev->Attribute("y"), "%f", &(pos.z));
				sscanf(elem_dev->Attribute("width"), "%f", &(sz.x));
				sscanf(elem_dev->Attribute("length"), "%f", &(sz.y));
				dev->kv["rect"] = torect(pos.x, pos.z, sz.x, sz.y);
			}
			elem_dev = elem_dev->NextSiblingElement();
		}
		elem_cab = elem_cab->NextSiblingElement();
	}
	return true;
}