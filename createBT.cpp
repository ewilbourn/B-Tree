//Emily Wilbourn
//CSC 310
//B-tree Project
//5/8/2021
#include <iostream>
#include <fstream>
#include "album.h" //for album object
#include "btree.h" //for btree object


using namespace std;

BTree createBtree(char* sequ_name, char* btree_name);
int main(int argc, char *argv[])
{
	//create a program that takes the name of the sequential file and the name
	//of the B-tree file to create from the command line, in this order.
	
	//argc needs to equal 3 because we need to include name of program, name of
	//sequential file, and name of the B-tree file.
	if (argc == 3)		
	{
		char* sequential_name = argv[1];
		char* btree_name = argv[2];	
		BTree btree = createBtree(sequential_name, btree_name);
	}
	else
	{
		cout << "Not enough arguments were entered." << endl;
		cout << "Enter the name of a sequential file and a B-tree file." << endl;
	}
	return 0;
}
//driver function for the program
//precondition: pass in sequential file and btree file names
//postcondition: btree is createBTree createBtree(char* sequ_name, string btree_name)
BTree createBtree(char* sequ_name, char* btree_name)
{
	fstream input(sequ_name, ios::in);
	
	//Album object used to read albums in from our file
	Album file_album;

	//BTree object to store our BTree
	BTree b;
	b.writeHeader(btree_name);
	int counter = 0;
	Album first;
	while(input >> file_album)
	{
		cout << "new album: " << file_album << endl;	
		b.insert(file_album);
		if(counter == 0)
		{
			first = file_album;
			counter += 1;
		}

		cout << "Inserted " << file_album << endl;
		b.printTree();
		cout << endl << endl;
	}

	BTNode file_node;
	fstream binfile(btree_name, ios::in | ios::binary);
/*
	//print the contents of binary file
	cout << "\nPRINTING TREE" << endl;
	while(binfile.read((char *) &file_node, sizeof(BTNode)))
	{
		cout << "Size:" << file_node.currSize << endl;
		for(int i = 0; i < ORDER-1; i++)
			cout << file_node.contents[i] << endl;
	}
*/
	cout << "is " << file_album << " in our BTree?" << endl;
	cout << std::boolalpha << b.search(first) << endl;

	b.printTree();
	return b;
}
