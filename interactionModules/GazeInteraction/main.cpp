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

typedef struct point{
	float x, y;
} Point;

typedef struct region
{
	int id;
	Point topLeft, bottomRight;
} Region;

bool VerifyPointMy(Region R, Point P){
	return  (P.x <= R.bottomRight.x) && (P.x >= R.topLeft.x) && 
			(P.y >= R.bottomRight.y) && (P.y <= R.topLeft.y);
}


int main(int argc, char const *argv[])
{
	json UserKeyList;

	UserKeyList.push_back({
		{"user", "all"},
		{"key", {
			{ {"id", 1},
			  {"topLeft", {{"x", -6}, {"y", 4}}},
			  {"bottomRight", {{"x", 2}, {"y", -4}}} },

			{ {"id", 2},
			  {"topLeft", {{"x", -2}, {"y", -1}}},
			  {"bottomRight", {{"x", 6}, {"y", -7}}} }
		}}
	});

	UserKeyList.push_back({
		{"user", "marina"},
		{"key", {
			{ {"id", 1},
			  {"topLeft", {{"x", -6}, {"y", 4}}},
			  {"bottomRight", {{"x", 2}, {"y", -4}}} },

			{ {"id", 3},
			  {"topLeft", {{"x", -2}, {"y", -1}}},
			  {"bottomRight", {{"x", 6}, {"y", -7}}} }
		}}
	});

	//cout << setw(4) << UserKeyList << endl;

	Point p1, p2, p3, p4, p5, p6;
	p1.x = -3; p1.y =  2; 
	p2.x =  5; p2.y =  2; 
	p3.x =  2; p3.y = -3; 
	p4.x = -8; p4.y =  6; 
	p5.x = -3; p5.y = -3.5; 
	p6.x = -2; p6.y = -6; 

	for (auto& item : UserKeyList)
	{
		cout << item["user"] << endl;
		for (auto& key : item["key"])
		{
			cout << key["id"] << endl;
		}
	}





	return 0;
}




