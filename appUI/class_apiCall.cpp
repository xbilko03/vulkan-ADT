#include "class_apiCall.hpp"
namespace details {

	void apiCall::assignParameter(std::string input)
	{
		std::string s = input;
		size_t pos = 0;
		std::string name;
		std::string value;

		pos = s.find('=');
		name = s.substr(0, pos);
		s.erase(0, pos + 1);
		value = s.substr(0, pos);
		s.erase(0, pos + 1);
		parameters.push_back(ParamStr(name,value));
	}
}