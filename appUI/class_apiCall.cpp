/*
* Name		    : class_apiCall.cpp
* Project	    : A Debugging Tool for Vulkan API (VkDebugger)
* Description   : Source file for data class that represents a single Vulkan function call
*
* Author        : Jozef Bilko (xbilko03), supervised by Ing. Ján Pečiva Ph.D.
*/
#include "class_apiCall.hpp"
namespace details {
	/* parse a stream of function parameters and have them saved */
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