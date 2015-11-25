#include <iostream>
#include <fstream>
#include <stdio.h>
#include <Windows.h>

using namespace std;

#pragma pack(1)
typedef struct {
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
} bmpPixel;

int main(int argc, char* argv[])
{
	 if (argc < 2) {
        // Tell the user how to run the program
        std::cerr << "Usage: " << argv[0] << " NAME" << std::endl;
        return 1;
    }

	ifstream im(argv[1],ios::in|ios::binary);//just to open some image as binary file
	if (!im)
	{
		cout << " There is no such file" << endl;
		getchar();
		return 0;
	}
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;//this two components are in a lib <windows.h>
	im.read((char*)(&bfh),sizeof(bfh));
	im.read((char*)(&bih),sizeof(bih));

	// Display file information
	/*
	cout << "\n FILHEADER\n";
    cout << "\n File type: " << bfh.bfType << bfh.bfType << endl;
    cout << " File size: " << bfh.bfSize << endl;
    cout << " Offset(adress of beggining of the image information): " << bfh.bfOffBits << endl;

	cout << "\n INFOHEADER\n";
    cout << "\n Header size: " << bih.biSize << endl;
    cout << " Image width: " << bih.biWidth << endl;
    cout << " Image height: " << bih.biHeight << endl;
    cout << " Number of bytes for pixel: " << bih.biBitCount << endl;
    cout << " Used compression: " << bih.biCompression << endl;
    cout << " Image size: " << bih.biSizeImage<< endl;
    cout << " Horizontal resolution: " << bih.biXPelsPerMeter << endl;
    cout << " Vertical resolution: " << bih.biYPelsPerMeter << endl;
    cout << " Number of colors in the color palette: " << bih.biClrUsed << endl;
    cout << " Number of important colors used: " << bih.biClrImportant << endl;
    */

	bmpPixel **map = new bmpPixel * [bih.biHeight];
	for (int i = 0; i < bih.biHeight; i++) {
		map[i] = new bmpPixel[bih.biWidth];
	}
	
	// Rows in a bmp file are aligned on a 4 byte boundary i.e., 
	// Zero padding bytes are added at the end of each line to make it a multiple of 4
	// We need to ignore these bytes while reading pixel color bytes 
	int nPaddedBytes = 4 - ((bih.biWidth * 3) % 4) ;
	char cPaddedBytes [4];
	
	//so step by step read exact pixel colors
	for(int y = 0; y < bih.biHeight; y++){
		for(int x = 0; x < (bih.biWidth); x++){
			im.read((char*)(&map[y][x]),sizeof(bmpPixel));
		}
		if (nPaddedBytes != 0){
			// Ignore the padded bytes
			im.read((char *)&cPaddedBytes, nPaddedBytes);
		}
	}

	//Get a console handle
    HWND myconsole = GetConsoleWindow();
    //Get a handle to device context
    HDC mydc = GetDC(myconsole);

    //Choose any color
    COLORREF COLOR= RGB(255,255,255); 
	//Draw pixels
	for(int x = 0; x < bih.biWidth; x++){
		int yDraw = 0;
		for(int y = bih.biHeight - 1; y >= 0; y--){

			COLOR = RGB(map[y][x].rgbRed, map[y][x].rgbGreen, map[y][x].rgbBlue)  ;
			SetPixel(mydc,x, yDraw++,COLOR);
		}
	}
    ReleaseDC(myconsole, mydc);

	// Free memory
	for (int i = 0; i < bih.biHeight; i++) {
		delete [] map[i];
	}
	delete [] map;

	cin.ignore();
	// getchar();

	return 0;
}
