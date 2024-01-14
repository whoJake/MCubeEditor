#pragma once
#include <fstream>
#include <optional>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

class FileIO
{
public:
    FileIO() = delete;

	struct FilePathData
    {
		std::string dir;
		std::string fileName;
		std::string ext;
	};

	static std::optional<std::vector<char>> try_read_file(const std::string& filename)
    {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);
		std::optional<std::vector<char>> result;

		if (!file.is_open())
			return result;

		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		result = buffer;

		file.close();
		return result;
	}

	static FilePathData parse_file_path(const std::string& filepath)
    {
		FilePathData result{ };

		std::string* curPart = &result.ext;

		for(auto it = filepath.crbegin(); it != filepath.crend(); ++it)
        {
			const char c = *it;
			if(c == '.' && curPart == &result.ext)
            {
				curPart = &result.fileName;
				continue;
			}
			if(c == '/') {
				if(curPart == &result.fileName)
                {
					curPart = &result.dir;
					continue;
				}
				// Case where there is no file extension so fileName is replaced by the existing ext and ext is removed
				if(curPart == &result.ext)
                { 
					curPart = &result.dir;
					result.fileName = result.ext;
					result.ext.clear();
					continue;
				}
			}
			*curPart += c;
		}

		std::reverse(result.dir.begin(), result.dir.end());
		std::reverse(result.fileName.begin(), result.fileName.end());
		std::reverse(result.ext.begin(), result.ext.end());

		return result;
	}
};