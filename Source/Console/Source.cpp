#include <iostream>
using namespace std;

#include "../JSON/JSON.hpp"
using namespace JSON;


#include <regex>


void main()
{
	auto t = JSON::Parse("{\"a\":null,\"b\":1}");

	cout << (string)t << endl;

	// JSON::Object obj; // = {{5, true}, "ololo"};
	// {
	// 	// obj["a"] = true;
	// 	// obj["b"] = 5;
	// 	// obj["c"] = "ololo";
	// }
	// 
	// std::cout << (std::string)obj << std::endl;

	system("pause");
}