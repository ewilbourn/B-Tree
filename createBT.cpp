//Emily Wilbourn
//CSC 310
//B-tree Project
//5/8/2021
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char *argv[])
{
	//create a program that takes the name of the sequential file and the name
	//of the B-tree file to create from the command line, in this order.
	
	//argc needs to equal 3 because we need to include name of program, name of
	//sequential file, and name of the B-tree file.
	if (argc == 3)		
	{
		string sequential_name = argv[1];
		string btree_name = argv[2];	
	}
	else
	{
		cout << "Not enough arguments were entered." << endl;
		cout << "Enter the name of a sequential file and a B-tree file." << endl;
	}
	return 0;
}
