#ifndef FILECORRECTER_H
#define FILECORRECTER_H
#pragma once
#include <filesystem>
#include <sstream>
#include <fstream>
#include <string>

class FileCorrecter
{
private:
	const std::wstring wstrFinalTag = L"</boost_serialization>";

public:
	inline bool IsFileSerializationInvalid(const std::experimental::filesystem::path& pathToCheck)
	{
		//We assume the file actually exists.
		std::wifstream inputStream(pathToCheck);
		std::wstringstream contents;
		contents << inputStream.rdbuf();
		return contents.str().rfind(wstrFinalTag) == std::wstring::npos;
	}
	
	inline void FixFileWithInvalidSerialization(const std::experimental::filesystem::path& originalFilePath)
	{
		//Again, we assume the file exists.
		if (IsFileSerializationInvalid(originalFilePath))
		{
//The file has not serialized properly, so fix it by adding the end tag.
			std::wofstream fixingFileStream(originalFilePath, std::ios::app);
			fixingFileStream << wstrFinalTag;
		}
			return;
}
};

#endif // !