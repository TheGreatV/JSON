#include <iostream>
using namespace std;

#include "../JSON/JSON.hpp"
using namespace JSON;


#include <regex>


void main()
{
	JSON::Object obj;
	{
		obj["a"] = true;
		obj["b"] = 5;
		obj["c"] = "ololo";
		obj["top kek"] = {{5, true}, "ololo"};
	}
	
	std::cout << obj.Stringify() << std::endl;

	auto parsed = JSON::Parse(obj.Stringify());
	cout << parsed.Stringify() << endl;


	system("pause");
}