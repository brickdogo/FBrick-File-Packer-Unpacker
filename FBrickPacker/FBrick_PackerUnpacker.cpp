#include "FBrick_PackerUnpacker.hpp"
#include "FBrick_FileManager.hpp"
#include <fstream>
#include <filesystem>

// Set Debug
void FBrick_PackerUnpacker::SetDebug(bool state) {
	Debug = state;
}

// Set Watermark Function
void FBrick_PackerUnpacker::SetWaterMark(std::string str) {
	STR_WM = str;
}

// Set CharFlag_1 Function
void FBrick_PackerUnpacker::SetCharFlag_1(uint8_t n) {
	charFlag1 = n;
}

// Set CharFlag_2 Function
void FBrick_PackerUnpacker::SetCharFlag_2(uint8_t n) {
	charFlag2 = n;
}

// Read All Files from Input Directory
void FBrick_PackerUnpacker::GetFilesPath(std::string inputDirectory) {
	for (const auto& entry : std::filesystem::directory_iterator(inputDirectory)) {
		// Save File Name
		fileName.push_back(entry.path().filename().generic_string());
		// Save File Path
		filePath.push_back(inputDirectory + entry.path().filename().generic_string());
		// Save File Size
		fileSize.push_back(std::filesystem::file_size(inputDirectory + entry.path().filename().generic_string()));

		// Debug Print
		if (Debug)std::cout << inputDirectory + entry.path().filename().generic_string() << "\n";
	}
}

// Pushback All File Content to temp_file Vector Variable
void FBrick_PackerUnpacker::WritePack_Temp(std::string _file) {
	std::ifstream fin(_file, std::ios::in | std::ios::binary);

	char c;

	while (fin >> std::noskipws >> c) {
		temp_file.push_back(c);
	}
	fin.close();
}

// Write Pack File
void FBrick_PackerUnpacker::CreatePack(std::string inputDirectory, std::string outputDirectory, std::string nameFile) {
	ResetVectors();
	std::string m_inputDirectory;
	std::string m_outputDirectory;

	m_inputDirectory = FBrick_FileManager::RemoveQuotationMarks(inputDirectory);
	m_outputDirectory = FBrick_FileManager::RemoveQuotationMarks(outputDirectory);

	m_inputDirectory = FBrick_FileManager::CheckBackSlashAtEnd(m_inputDirectory);
	m_outputDirectory = FBrick_FileManager::CheckBackSlashAtEnd(m_outputDirectory);

	GetFilesPath(m_inputDirectory);
	uint32_t m_TotalFileCount = filePath.size();
	std::string m_FileDescription = std::to_string(m_TotalFileCount) + char(charFlag1) + char(charFlag2);

	for (uint32_t i = 0; i < m_TotalFileCount; i++) {
		// Create File List and Size
		m_FileDescription += fileName[i] + char(charFlag1) + std::to_string(fileSize[i]) + char(charFlag1) + char(charFlag2);

		// Write File Content to temp_file vector from Function WritePack_Temp
		WritePack_Temp(filePath[i]);
	}
	// Print File List and Size
	if (Debug) std::cout << m_FileDescription << std::endl;

	// Check Target Directory to Write, If Directory Doesn't Exist Then Create New One
	if ((std::filesystem::exists(m_outputDirectory) && std::filesystem::is_directory(m_outputDirectory)) == false) {
		std::filesystem::create_directories(m_outputDirectory);
	}

	// Write Content
	std::ofstream fout(m_outputDirectory + nameFile, std::ios::out | std::ios::binary);
	fout << STR_WM;
	fout << m_FileDescription;
	for (char i : temp_file) {
		fout << i;
	}
	fout.close();
}

// Unpacking
void FBrick_PackerUnpacker::UnpackFile(std::string fileInput, std::string outputDirectory) {
	ResetVectors();
	std::string m_fileInput;
	std::string m_outputDirectory;

	m_fileInput = FBrick_FileManager::RemoveQuotationMarks(fileInput);
	m_outputDirectory = FBrick_FileManager::RemoveQuotationMarks(outputDirectory);

	m_outputDirectory = FBrick_FileManager::CheckBackSlashAtEnd(m_outputDirectory);
	
	uint32_t m_ThisPackSize = std::filesystem::file_size(m_fileInput);
	uint32_t m_FilesCount = 0;
	std::vector<std::string> m_FileName;
	std::vector<uint32_t> m_FileSize;

	std::ifstream fin(m_fileInput, std::ios::in | std::ios::binary);

	char temp;
	std::string tempStr = "";
	bool m_Flag_1 = false;
	bool m_Flag_2 = false;

	// Skipping Water Mark
	uint32_t m_WaterMarkSize = 0;
	while (fin >> std::noskipws >> temp) {
		m_WaterMarkSize++;
		if (m_WaterMarkSize == STR_WM.length() - 1) break;
	}

	// Get File Count
	while (fin >> std::noskipws >> temp) {
		if (temp == char(charFlag1)) m_Flag_1 = true;
		if (temp == char(charFlag2)) m_Flag_2 = true;
		if (m_Flag_1 && m_Flag_2) break;

		tempStr += temp;
	}
	m_FilesCount = std::stoi(tempStr);
	tempStr = "";

	// Debug - Print Total File Count
	if (Debug) std::cout << "File Count to Unpack: " << m_FilesCount << std::endl;

	// Get File Name and Size
	for (uint32_t i = 0; i < m_FilesCount; i++) {
		// Get File Name
		while (fin >> std::noskipws >> temp) {
			if (temp == char(charFlag1)) break;
			tempStr += temp;
		}
		m_FileName.push_back(tempStr);
		tempStr = "";
		
		// Get File Size
		m_Flag_1 = false;
		m_Flag_2 = false;
		while (fin >> std::noskipws >> temp) {
			if (temp == char(charFlag1)) m_Flag_1 = true;
			if (temp == char(charFlag2)) m_Flag_2 = true;
			if (m_Flag_1 && m_Flag_2) break;
			tempStr += temp;
		}
		m_FileSize.push_back(std::stoi(tempStr));
		tempStr = "";
	}

	// Print File Name and Size
	if (Debug) {
		for (int i = 0; i < m_FilesCount; i++) {
			std::cout << m_FileName[i] << " Size: " << m_FileSize[i] << std::endl;
		}
	}

	// Check Target Directory to Write, If Directory Doesn't Exist Then Create New One
	if ((std::filesystem::exists(m_outputDirectory) && std::filesystem::is_directory(m_outputDirectory)) == false) {
		std::filesystem::create_directories(m_outputDirectory);
	}

	// Write Content
	for (uint32_t i = 0; i < m_FilesCount; i++) {
		std::ofstream fout(m_outputDirectory + m_FileName[i], std::ios::out | std::ios::binary);
		uint32_t counterSize = 0;
		while (fin >> std::noskipws >> temp) {
			counterSize++;
			fout << temp;
			if (counterSize >= m_FileSize[i] && i != m_FilesCount - 1) break;
		}
		fout.close();
	}

	// Close Packed File
	fin.close();
}
void FBrick_PackerUnpacker::ResetVectors() {
	// Clear All Vectors
	if (Debug) std::cout << "Clear All Vectors.." << std::endl;
	fileName.clear();
	filePath.clear();
	fileSize.clear();
	temp_file.clear();
}

FBrick_PackerUnpacker::~FBrick_PackerUnpacker() {
	ResetVectors();
}