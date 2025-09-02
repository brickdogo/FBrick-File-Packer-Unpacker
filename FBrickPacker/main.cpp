/*
Made by: Brickdogo
Create date: 09/02/2025 - September 02 2025

This is just simple program to packing & unpacking files (No compression)
*/

#include <iostream>
#include <string>
#include <stdlib.h>
#include "FBrick_PackerUnpacker.hpp"

FBrick_PackerUnpacker PackUnpack;

void CreateForm(std::string str, std::string& strOut) {
	std::cout << str << ": ";
	std::getline(std::cin, strOut);
	std::cout << "\n";
}

void App() {
	std::string Operation, InputFolder, OutputFolder, FileName;
	CreateForm("FBrick Packer V0.1!\n1= Packing Files\n2= Unpacking File\n", Operation);

	if (Operation == "1") {
		CreateForm("Drag or Write Input Folder Here", InputFolder);
		CreateForm("Drag or Write Folder Path for Output Packed File", OutputFolder);
		CreateForm("Packed File Name", FileName);

		PackUnpack.CreatePack(InputFolder, OutputFolder, FileName);
	}
	else if (Operation == "2") {
		CreateForm("Drag Packed File Here", FileName);
		CreateForm("Drag of Write Folder Path for Output Unpacked Files", OutputFolder);

		PackUnpack.UnpackFile(FileName, OutputFolder);
	}
	else {
		std::cout << "Please Re-enter Valid Input!!" << std::endl;
	}
	std::cout << "\n";
	App();
}

int main() {
	PackUnpack.SetDebug(false);

	PackUnpack.SetWaterMark("FBrick Packer V0.1!!\n");

	PackUnpack.SetCharFlag_1(225);
	PackUnpack.SetCharFlag_2(255);

	App();

	return 0;
}