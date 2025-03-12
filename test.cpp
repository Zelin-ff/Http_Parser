#include <iostream>
#include <memory>
#include <sstream>

struct Test {
	Test() { std::cout << "Constructed" << std::endl; }
	~Test() { std::cout << "disConstructed" << std::endl; }
};

int test()
{
	std::string str = "123 zelin";
	std::stringstream ss(str);
	int code;
	std::string name;
	ss >> code >> name;

	std::cout << ss.str() << std::endl;
	std::cout << "code: " << code << std::endl;
	std::cout << "name: " << name << std::endl;

	return 0;
}



