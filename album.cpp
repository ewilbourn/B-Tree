
#include "album.h"
#include <iostream>
//constructor for setting the values in current album equal to
//those of another album that's being passed in.
Album::Album (const Album& otherAlbum)
{
	strcpy (UPC, otherAlbum.UPC);
      	strcpy (Artist, otherAlbum.Artist);
       	strcpy (Title, otherAlbum.Title);
}
//constructor for setting the values in current album to the upc,
//artist, and title values being passed in.
Album::Album (String upc, String artist,String title)
{
	strcpy(UPC, upc);
	strcpy(Artist, artist);
	strcpy(Title, title);	
}
//default constructor
Album::Album ()
{
	strcpy (UPC, "\0");  
        strcpy (Artist, "\0"); 
	strcpy (Title, "\0");
}
//overload = operator to set the contents of one album to the 
//contents of another album
Album & Album::operator = (const Album& otherAlbum)
{
	strcpy (UPC, otherAlbum.UPC);
      	strcpy (Artist, otherAlbum.Artist);
       	strcpy (Title, otherAlbum.Title);
	return *this;
}
//overload the < operator to compare Album objects by their UPC 
bool operator < (const Album& a, const Album& b)
{
	int upc_a = stoi(a.UPC);
	int upc_b = stoi(b.UPC);
	return (upc_a < upc_b ? true : false);	
}
//overload the cin >> operator so that we can read in Album objects with ease
istream & operator >> (istream & stream, Album & C)
{	
	//string variable to read in data
	string input;

	//get UPC data
	getline(stream, input);
	strcpy(C.UPC, input.c_str());

	//get Artist data
	getline(stream, input);
	strcpy(C.Artist, input.c_str());

	//get Title data
	getline(stream, input);
	strcpy(C.Title, input.c_str());
	return stream;
}
//overload the cout << operator so that we can print Album objects with ease
ostream & operator << (ostream & stream, Album & C)
{
	cout << C.UPC << "|" << C.Artist << "|" << C.Title;
	return stream;	
}

bool operator == (const Album& a, const Album& b)
{
	return (a.UPC == b.UPC && a.Artist == b.Artist && a.Title == b.Title ? true : false)
}
//return the UPC (which is currently a c-string) as a std::string
string Album::getUPC ()
{
	string s(UPC);
	return s;
}

//get the size of the Album in bytes by converting all the char arrays
//to strings and adding the sizes of the strings
int Album::recordSize ()
{
	int sizeUPC = *(&UPC + 1) - UPC;
	string s_upc = getUPC();
	
	//get string artist
	string s (Artist);
	string s_artist = s;

	//get string title
	string s1 (Title);
	string s_title = s1;

	//total bytes of all strings
	int size = s_upc.size() + s_artist.size() + s_title.size();

	return size;
}
