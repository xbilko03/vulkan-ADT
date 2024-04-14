#include "class_image.hpp"

namespace details {
	void  vkImageManager::AssignDataRaw(unsigned long long inputID, std::string inputData)
	{
		std::string rawData = this->bufferMap[inputID].attachedMemoryData;
		std::string token;
		std::string output = "";
		char* testOutput = (char*)malloc(5000000);
		unsigned long long pos = rawData.find("_") + 1;
		rawData.erase(0, pos);


		testOutput = strtok((char*)rawData.c_str(), "!");
		std::cout << "transformed ... " << testOutput << std::endl;
		return;
		while ((pos = rawData.find("!")) != std::string::npos)
		{
			token = rawData.substr(0, pos);
			rawData.erase(0, pos + 1);
			//int partialValue = stoi(token);
			//output += (char*)partialValue;
		}
		std::cout << "transformed ... " << rawData.size() << std::endl;
		
	}
}