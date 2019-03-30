//g++ create.cpp -g -lstdc++fs -o result;./result;rm result
#include <experimental/filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <array>
#include <cmath>
#include <thread>
#include "bitmap.h"
namespace fs = std::experimental::filesystem;
using namespace std;


std::string replace(std::string base, const std::string from, const std::string to){
    std::string SecureCopy = base;
    for (size_t start_pos = SecureCopy.find(from); start_pos != std::string::npos; start_pos = SecureCopy.find(from, start_pos)){
        SecureCopy.replace(start_pos, from.length(), to);
    }
    return SecureCopy;
}

bool is_number(const std::string &s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

vector<string> split(string str, string character){
    vector<string> result;
    long long s=0;
    long long i=0;
    while(i<str.length()){
        if(str[i]==character[0]||i==str.length()-1){
            long long x=i-s;
            if(i==str.length()-1){
                x++;
            }
            result.push_back(str.substr(s,x));
            s=i+1;
        }
        i++;
    }
    return result;
}

int main()
{
	ofstream fout("../dataset.in");
	for (const auto & entry : fs::directory_iterator("./")){
		string filename=replace(entry.path(),"./","");
		if(!is_number(string(1,filename[0]))){
			continue;
		}
//		int count=0;
		for (const auto & image : fs::directory_iterator(entry.path())){
			vector<string> splitName=split(image.path().string(),".");
			if(splitName.size()>1){
				string newName=splitName[splitName.size()-2];
				Bitmap image;
				vector<vector<Pixel>> bmp;
				Pixel rgb;
				image.open("."+newName+".bmp");
				bool validBmp = image.isImage();
				if (validBmp == true){
					bmp = image.toPixelMatrix();
					for (int y = 0; y < 16; y++){
						for (int x = 0; x < 16; x++){
							fout << ((float((255-(float(bmp[y][x].red+bmp[y][x].green+bmp[y][x].blue)/3))/255))) << " ";
						}
					}
					fout << stoi(split(newName,"/")[1]);
					fout << endl;
				}/*
				if(++count>=25){
					break;
				}*/
			}
		}
	}
	fs::remove("../tests.in");
	fs::copy("../dataset.in","../tests.in");
	
/*
	ofstream fout("test.out");
	Bitmap image;
	vector<vector<Pixel>> bmp;
	Pixel rgb;

	//read a file example.bmp and convert it to a pixel matrix
	image.open("0.bmp");

	//verify that the file opened was a valid image
	bool validBmp = image.isImage();

	if (validBmp == true)
	{
		bmp = image.toPixelMatrix();
		for (int y = 0; y < 128; y++)
		{
			for (int x = 0; x < 128; x++)
			{
				fout << (int(round((255-(float(bmp[y][x].red+bmp[y][x].green+bmp[y][x].blue)/3))/255))) << " ";
			}
//			fout << endl;
		}
		
  //  image.fromPixelMatrix(bmp);
   // image.save("example.bmp");

	}
	*/
	return 0;
}