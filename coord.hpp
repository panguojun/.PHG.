struct coord
{
	vec3 ux = vec3::UX;
	vec3 uy = vec3::UY;
	vec3 uz = vec3::UZ;

	coord() {}
	coord(crvec _ux, crvec _uy, crvec _uz)
	{
		ux = _ux; uy = _uy; uz = _uz;
	}

	// C1*C2*C3* ... *Cn * v （transfrom)
	vec3 operator * (crvec v)
	{
		return ux * v.x + uy * v.y + uz * v.z;
	}
	coord operator * (coord& c)
	{
		ux = ux * c.ux.x + uy * c.ux.y + uz * c.ux.z;
		uy = ux * c.uy.x + uy * c.uy.y + uz * c.uy.z;
		ux = ux * c.uz.x + uy * c.uz.y + uz * c.uz.z;
	}
	// v/C1/C2/C3/ ... /Cn（projection)
	friend vec3 operator / (crvec v, const coord& c)
	{
		return vec3(v.dot(c.ux), v.dot(c.uy), v.dot(c.uz));
	}
	coord operator / (const coord& c)
	{
		coord rc;
		rc.ux = vec3(ux.dot(c.ux), ux.dot(c.uy), ux.dot(c.uz));
		rc.uy = vec3(uy.dot(c.ux), uy.dot(c.uy), uy.dot(c.uz));
		rc.uz = vec3(uz.dot(c.ux), uz.dot(c.uy), uz.dot(c.uz));
		return rc;
	}
	void dump()
	{
		PRINT("-------");
		PRINT("ux: " << ux.x << "," << ux.y << "," << ux.z);
		PRINT("uy: " << uy.x << "," << uy.y << "," << uy.z);
		PRINT("uz: " << uz.x << "," << uz.y << "," << uz.z);
	}
};
int test()
{
	coord c1(vec3::UY, vec3::UZ, vec3::UX);
	vec3 v = vec3(1,0,0) / c1;
	PRINTVEC3(v)

	return 0;
}