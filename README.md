# FBrick-File-Packer-Unpacker
This is simple packing &amp; unpacking program without any compression method

### C++ version used is: C++20

## The main function is:
1. CreatePack("Input folder directory that contain your files", "Output directory for packed file", "Name of packed file");
2. UnpackFile("Input packed file", "Output folder directory");

## How Packing Function Work:
1. When the "CreatePack" function called, it will read file path, file name, file size, and put them inside vectors.
2. Then, this function will create pack header (that called descriptor inside code) the structure is:
   - "Watermark"<Flag_1><Flag_2>"File Name 1"<Flag_1>"Size of File 1"<Flag_1><Flag_2>"File Name #" and so on.
3. After that, it will create packed file, the structure be like:
   -<Pack header/descriptor><Data File 1><Data File 2><Data File #> and so on.

## How Unpacking Function Work:
1. When the "UnpackFile" functin called, it will open the file.
2. The reading that packed file will skip watermark (make sure the watermark is same as program you set).
3. Then, it will read what files inside that packed file based structure created before.
4. When the function finished reading file name and file size, it will start creating new individual files based file name and it size.
