#include <fstream>
#include <iostream>
#include <iomanip>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::setw;

typedef struct point
{
	float x, y;
} Point;

typedef struct region
{
	int id;
	Point topLeft;
	Point bottomRight;
} Region;

/*
typedef struct userRegions
{
	std::string user;
	std::vector<Region> regions;
} UserRegions;
*/
bool VerifyPointInRegion(Region R, Point P)
{
	return  (P.x <= R.bottomRight.x) && (P.x >= R.topLeft.x) && 
			(P.y >= R.bottomRight.y) && (P.y <= R.topLeft.y);
}


int main()
{
	json UserKeyList;

	UserKeyList.emplace("user", "marina");
	UserKeyList.push_back({
		"key", {
			{ {"id", 1},
			  {"topLeft", {{"x", -6}, {"y", 4}}},
			  {"bottomRight", {{"x", 2}, {"y", -4}}} },

			{ {"id", 2},
			  {"topLeft", {{"x", -2}, {"y", -1}}},
			  {"bottomRight", {{"x", 6}, {"y", -7}}} },

			{ {"id", 3},
			  {"topLeft", {{"x", -2}, {"y", -1}}},
			  {"bottomRight", {{"x", 6}, {"y", -7}}} }
		}
	});

	//cout << setw(4) << UserKeyList << endl;

	vector<Region> vecRegion;
	string user;

	user = UserKeyList["user"];

	for (auto& key : UserKeyList["key"])
	{
			Region region;
			region.id = key["id"];
			region.topLeft.x = key["topLeft"]["x"];
			region.topLeft.y = key["topLeft"]["y"];
			region.bottomRight.x = key["bottomRight"]["x"];
			region.bottomRight.y = key["bottomRight"]["y"];
			vecRegion.push_back(region);
	}

	cout << user << endl;

	for (auto& reg : vecRegion)
	{
		cout << reg.id << " " << reg.topLeft.x << " " << reg.topLeft.y << " " << reg.bottomRight.x << " " << reg.bottomRight.y <<endl;
	}
	
	vector<Point> vecPoint;

	vecPoint.push_back({-3,2});
	vecPoint.push_back({5,2});
	vecPoint.push_back({2,-3});
	vecPoint.push_back({-8,6});
	vecPoint.push_back({-3,3.5});
	vecPoint.push_back({-2,-6});

	cout << "Regiões ativadas:" << endl;

	for (auto& point : vecPoint)
	{
		for (auto& reg : vecRegion)
		{
			if (VerifyPointInRegion(reg, point))
			{
				cout << "   user = " << user << "  id = " << reg.id << endl;
			}
		}
	}

	return 0;
	
}