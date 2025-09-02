#include "FBrick_FileManager.hpp"

// String Correction
// When user drag and drop their folder directly into console, the console will written as "file path".
// Just for safety, some function can cause error when that symbol exist,
// so this function will remove the "Quotation Mark"
std::string FBrick_FileManager::RemoveQuotationMarks(std::string str) {
	std::string new_str;
	bool skip = false;

	for (uint32_t i = 0; i < str.length(); i++) {
		skip = false;
		if (i == 0 && str[0] == char(34)) {
			skip = true;
		}
		if (i == str.length() - 1 && str[str.length() - 1] == char(34)) {
			skip = true;
		}
		if (!skip) {
			new_str += str[i];
		}
	}
	return new_str;
}

// This is just for adding \\ at the end of path, so that mean this is folder
std::string FBrick_FileManager::CheckBackSlashAtEnd(std::string str) {
	if (str[str.length() - 1] != char(92) || str[str.length() - 1] != char(47)) {
		return str + "\\";
	}
	return str;
}