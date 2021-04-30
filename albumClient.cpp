#include "album.cpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm> //for sorting our vector
#include <stdint.h>
#include <cstdint>
#include <cassert>
using namespace std;

bool compareAlbums(Album a, Album b);
int main()
{
	/////////// INSTANTIATE VALUES /////////////////
	
	//values for album a
	String upc = "123";
	String artist = "Addy";
	String title = "Country Roads";

	//values for album b
	String upc1 = "456";
	String artist1 = "Henry";
	String title1 = "Hello World";

	//filestream objet to test reading in Albums
	fstream f ("album.baby", ios::in);
	Album file_album;
	vector<Album> v; 
	////////// TESTING ///////////
	
	//test constructor when we assign all values to album initially
	Album a (upc, artist, title);
	string upc_str = a.getUPC();
	cout << "UPC value for album a: " << upc_str << endl;
	cout << "Printing album a: " << a << endl;

	//test storing contents of 1 album in another album object
	Album b (upc1, artist1, title1);
	cout << "Printing album b: " << b << endl;

	//new Album object that will hold contents of Album b
	Album c (b);
	cout << "Printing album c (same as b): " << c << endl;

	//new Album object that will hold contens of Album a
	Album d;
	d = a;
	cout << "Printing album d (same as a): " << d << endl;
	

	//Test reading in album.baby file 
	cout << "\nReading in album.baby. \nAlbums: " << endl;
	while (f >> file_album)
	{
		v.push_back(file_album);
		cout << file_album << endl;
	}
	//sort the vector of albums by the UP code
	sort(v.begin(), v.end(),compareAlbums);
	cout << "\nSorted albums from album.baby along with their record sizes: " << endl;
	for (int i = 0; i < v.size(); i++)
	{
		int recordSize = v[i].recordSize();
		cout << v[i] << endl; 
		cout << "size: " << recordSize << endl;
		cout << endl;
	}

	int d_address = (intptr_t) &d;
	cout << std::hex << d_address << endl;	
//	Album *d_copy;
//	d_copy = &d;
//	cout << d_copy << endl;
//	int addr = &(*(d_copy));
	//cout << addr << endl;
	//uintptr_t ip = reinterpret_cast<uintptr_t>(&d_copy);
	//cout << "0x" << std::hex << ip << endl;
	//int *j = reinterpret_cast<int*>(ip);
	//assert(j == &d_copy);
	
	return 0;
}

//got the idea for this from: https://stackoverflow.com/questions/4892680/sorting-a-vector-of-structs
bool compareAlbums(Album a, Album b)
{
	return a < b;
}
