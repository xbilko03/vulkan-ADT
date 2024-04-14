#include "class_image.hpp"

namespace details {
	std::string vkBufferManager::GetConvertRaw(unsigned long long inputID)
	{
		std::string rawData = this->bufferMap[inputID].attachedMemoryData;
		std::string token;
		std::string output = "";
		unsigned long long pos;

		std::cout << "transforming data ... " << rawData << std::endl;

		return "";

		while ((pos = rawData.find("!")) != std::string::npos)
		{
			//token = rawData.substr(0, pos);
			//rawData = rawData.erase(0, pos + 1);
			//int partialValue = stoi(token);
			//output += (char*)partialValue;
		}
		std::cout << "token " << token << std::endl;
		return output;
	}
}