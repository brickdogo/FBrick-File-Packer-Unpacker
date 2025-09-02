#pragma once

#include <iostream>
#include <vector>

class FBrick_PackerUnpacker {
private:
	bool Debug = false;
	uint8_t charFlag1 = 225;
	uint8_t charFlag2 = 255;

	std::string STR_WM = "FBrick Packer V0.1\n"; // Just a Watermark :)

	std::vector<std::string> fileName;
	std::vector<std::string> filePath;
	std::vector<std::uint32_t> fileSize;
	std::vector<char> temp_file;

	void GetFilesPath(std::string inputDirectory);
	void WritePack_Temp(std::string _file);
	void ResetVectors();
public:
	void SetDebug(bool state);
	void SetWaterMark(std::string str);
	void SetCharFlag_1(uint8_t n);
	void SetCharFlag_2(uint8_t n);
	void CreatePack(std::string inputDirectory, std::string outputDirectory, std::string nameFile);
	void UnpackFile(std::string fileInput, std::string outputDirectory);
	~FBrick_PackerUnpacker();
};