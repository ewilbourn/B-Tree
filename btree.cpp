#include <algorithm> //used to sort array
#include "btree.h"
#include <set> //used in splitting the node
#include <iterator> //std::advance; used in splitting node
#include <fstream> //for file objects
using namespace std;

//default constructor
BTree::BTree()
{

}

void BTree::writeHeader (char * fileName)
{
	//set the treeFile object where our BTree is stored
	treeFile.open(fileName, ios::trunc | ios::in|ios::out|ios::binary);

	//set file pointer to beginning of the file
	treeFile.seekp(0);

	//rootAddr needs to be the size of a BTNode because it will
	//come immediately after the header node in our btree file	
	rootAddr = sizeof(BTNode);

	BTNode header;
	header.child[0] = rootAddr;
	treeFile.write((char* ) &header, sizeof(BTNode));	

	//instantiate root initial size
	root.currSize = 0;
		
	//instantiate all leaf values to be -1
	for (int i = 0; i < ORDER; i++)
		root.child[i] = -1;
	
	//write root node to the file
	treeFile.write((char* ) &root, sizeof(BTNode));	

	//define address of root node
	cout << "rootaddr: " << rootAddr << endl;
	cout << endl;
}

//insert key into B-tree
void BTree::insert (keyType key)
{
//	int recAddr = treeFile.tellp();
//	cout << "redAddr: " << recAddr << endl;
	insert(key,rootAddr, -1, -1);
}



//use when updating BT
void BTree::reset (char * filename)
{
	
}

void BTree::close ()
{

}

//print preorder traversal of tree
void BTree::printTree()
{
	cout << "-------- B-tree of height " << height << " --------" << endl;
	cout << "rootAddr: " << rootAddr << endl;
	printTree(rootAddr);
}

//print inorder traversal of tree
void BTree::inorder()
{

}

//print postorder traversal of tree
void BTree::reverse()
{

}

//get the height of the tree
int BTree::getHeight()
{
	return height;
}

bool BTree::search (keyType key)
{
	return search (key, root, rootAddr);
}

keyType BTree::retrieve (string key)
{
	keyType k;
	return k;
}

//method to return number of read/writes in part 2
void BTree::totalio() const
{
	int totalseeks = read + write;
	cout << "total seeks: " << totalseeks << endl;
	cout << "read: " << read << endl;
	cout << "write: " << write << endl; 
}

//public countLeaves
int BTree::countLeaves()
{
	return countLeaves(rootAddr);
}

//PRIVATE METHODS

//print a preorder traversal of the tree
void BTree::printTree (int rAddr)
{
	if (rAddr != -1)
        {
                BTNode dummy = getNode(rAddr);
                printNode(rAddr);
                for (int i = 0;  i <= dummy.currSize;  i++)
                        printTree(dummy.child[i]);
        }
}

//print an inorder traversal of the tree
void BTree::inorder (int rootAddr)
{

}
//print a postorder traversal of the tree
void BTree::reverse (int rootAddr)
{

}
//find address where the new key should be added to
//precondition: pass in the key we are looking for, the root, and the address of the root
//postcondition: return the address of the node that we should put the key into
int BTree::findAddr (keyType key, BTNode t, int tAddr)
{	
	int nodeAddr;
	cout << "IN FIND ADDR" << endl;
	cout << "key we are finding a slot for: " << key << endl;
		
	//search through contents to see if content == key; if same, return true.
	//if key < content, go to the child. 	
	if (tAddr != -1)
        {
		//look through the contents of the node to see if the key can be found
		for (int i = 0;  i < t.currSize;  i++)
                {
			if (key < t.contents[i] && !isLeaf(t))
			{
				cout << "key < t.contents[i] && !isLeaf" << endl;
				//get the node of the child we are going to explore 
                		BTNode dummy = getNode(t.child[i]);

				//make a recursive call to search the child node
				return findAddr(key, dummy, t.child[i]);
			}
		}
		//this means the key is greater than all values in the contents array. If it's
		//not a leaf, explore it's children. Else, it does not exist in the BTree.
		if (!isLeaf(t))
		{
			BTNode dummy = getNode(t.child[t.currSize]);
			return findAddr(key,dummy,t.child[t.currSize]);
		}
        }
	else
		return -1;
	//if leaf and haven't found a slot, return tAddr
	return tAddr;
}
//find the address of the parent of the given B-tree node
//Used in place node, which is called when splitting the node.
//precondition: pass in the keyType object, the root node, and the 
//address of the child we're looking for
//postcondition: return the parent of the childAddr we passed in.
int BTree::findpAddr(keyType key, BTNode t, int tAddr, int childAddr)
{
	//Iterate through the contents of the current contents array.
	//We need to look at all the addresses stored in the child array of the 
	//current element in the contents array.
	

	/*
 		1. Get the current album at contents[i]
		2. Compare the key to our album at contents[i]
			a. If key is less than contents[i], then see if the value
			in child[i] is the same as childAddr. If it is, then return
			tAddr. Else, keep looking.
			b. if i == 
  	*/

	//CASE FOR IF CHILDADDR == ROOTADDR??

	//if key < contents of value and child is not -1, then 


	//KINDA LIKE PRINT TREE TRAVERSAL
	//
	//RECURSIVELY CALL METHOD UNTIL WE FIND IT.

	//loop through child array for current node
	for(int i = 0; i < t.currSize+1; i++)
	{
		if (t.child[i] == childAddr)
			return tAddr;	
	}
	for (int i = 0; i < t.currSize; i++)
	{
		//if key is less than current contents, then go left.
		if (key < t.contents[i] && !isLeaf(t))
		{
			BTNode newNode = getNode(t.child[i]);
			return findpAddr(key, newNode, t.child[i], childAddr);
		}
	}
	//by this point, we iterated through all the left children of contents[i]
	//elements and need to look at the final child element in the child array.
	if (!isLeaf(t))
	{
		BTNode newNode = getNode(t.child[t.currSize]);
		return findpAddr(key,newNode,t.child[t.currSize], childAddr);
	}

	//return -1 to indicate that the parent address was never found 
	return -1;
}
//
void BTree::insert (keyType key, int recAddr, int oneAddr, int twoAddr)
{
	BTNode n = getNode(recAddr);
	//need this to be less than order-1 because initially currSize will be 0,
	//and after adding a new node, this will be full (at size ORDER-1)
	cout << "INSERTING VALUE" << endl;

	if(!isLeaf(n))
	{
		cout << "NOT LEAF" << endl;	
		recAddr = findAddr(key, root, rootAddr);	
		n = getNode(recAddr);
	}
	if (n.currSize < (ORDER-1))	
	{
		cout << "recAddr: " << recAddr << endl;
		//add value to contents array, increment currSize, 
		//and then sort contents
		cout << "Size before inserting " << n.currSize << endl;
		n.contents[n.currSize] = key;
		cout << "n.contents[n.currSize]: " << n.contents[n.currSize] << endl;
		n.currSize+=1;
		sort(n.contents, n.contents+n.currSize);
		cout << "current size: " << n.currSize << endl;

		//write value to our treefile
		cout << "About to clear" << endl;
		treeFile.clear();
		cout << "treeFile.tellp(): " << treeFile.tellp() << endl;
		treeFile.seekg(recAddr);
		cout << "treeFile.tellp(): " << treeFile.tellp() << endl;
		treeFile.write((char*) &n, sizeof(BTNode));

		//update root address
		if(recAddr == rootAddr)
			root = n;
			
		//print location of the file pointer
		cout << "treeFile.tellp(): " << treeFile.tellp() << endl;
		cout << endl;
	}
	//if not, we will need to split the node and then add the value to the
	//correct node with the placeNode method	
	else
	{
		cout << "SPLITTING" << endl;
		int oneAddr = -1;
		int twoAddr = -1;
		splitNode (key,recAddr,oneAddr,twoAddr);
		placeNode (key,recAddr,oneAddr,twoAddr);
	}
}

//return back the BTNode object given an address
BTNode BTree::getNode (int recAddr)
{
	BTNode n;
	
	//get the address of the node in the file 
	treeFile.seekg(recAddr);

	//read in BTNode from the binary file
	treeFile.read((char *) &n, sizeof(BTNode));

	return n;
}
//print out a single node
//precondition: pass in the address of the record we're observing
//postcondition: nothing is returned, but the contents of the node are printed
void BTree::printNode(int recAddr)
{
	BTNode n = getNode(recAddr);
	cout << "\t***node of size " << n.currSize << "***" << endl;
	for (int i = 0; i < n.currSize; i++)
	{
		cout << n.contents[i] << endl;	
	}
}

//resets the tree/promotes the root after splitting a node
void BTree::placeNode (keyType k,int recAddr,int oneAddr,int twoAddr)
{
	//create node for recAddr
	BTNode n = getNode(recAddr);

	//add all Pairs in ValArray to a set of Pairs
/*	set<Pair>s;
	set<Pair>::iterator it = s.begin(); //iterator for our set of Pairs
	//create the Pairs
	for (int i = 0; i < n.currSize; i++)
	{
		Pair p;
		p.element = n.contents[i];
		p.loffset = n.child[i];
		p.roffset = n.child[i+1];
		s.insert(p);
	}*/

	set<keyType> s (n.contents, n.contents+n.currSize);
	s.insert(k);
	set<keyType>::iterator it = s.begin();


	//CREATE 3 NEW NODES FOR THE SPLIT
	BTNode parent; //recAddr
	parent.currSize = 0;

	BTNode left_child; //oneAddr 
	left_child.currSize = 0;

	BTNode right_child; //twoAddr
	right_child.currSize = 0;

	//find the middle index, or the value that is being promoted.
	it = s.begin();
	int middle_index = (ORDER-1)/2;
	advance(it, middle_index);
	Album p_album = *it;
	it = s.begin();
	
	cout << "\n\n in PlaceNode: " << endl;
	cout << "s.size(): " << s.size() << endl;
	
	//fill in the contents arrays for the parent/left_child/right_child nodes
	for (int i = 0; i < s.size(); i++)
	{
		Album album = *it;
		if(album < p_album)
		{
			cout << "Adding " << album << " to left child." << endl;
			left_child.contents[left_child.currSize] = album;
			left_child.currSize+=1;
		}
		else if(album == p_album)
		{
			cout << "Adding " << album << " to parent." << endl;
			parent.contents[parent.currSize] = album;
			parent.currSize+=1;
		}		
		else
		{
			cout << "Adding " << album << " to right child." << endl;
			right_child.contents[right_child.currSize] = album;
			right_child.currSize+=1;
		}
		it++;
	}
	//set the children of parent node
	parent.child[0] = oneAddr;
	parent.child[1] = twoAddr;

	//fill leaves for new children
	for(int i = 0; i < ORDER; i++)
	{
		left_child.child[i] = -1;
		right_child.child[i] = -1;
	}

	cout << "rootAddr: " << rootAddr << endl;
	cout << "address of node being split: " << recAddr << endl;

	//update the root with the new root, if our node is a root
	if(rootAddr == recAddr)
	{
		cout << "rootAddr  == recAddr" << endl;
		root = parent;
		rootAddr = recAddr;
		//FIX THE HEADER NODE ROOT ADDRESS
		treeFile.seekg(0, ios::beg);
		BTNode header;
		header.child[0] = rootAddr;
		treeFile.write((char*)&header, sizeof(BTNode));//else, this is 
	}

	//ALGORITHM FOR RAISING A NODE TO AN ALREADY OCCUPIED NODE? NOT NEW ROOT.
	//1. Create a set of Pairs with our parent node and repective children.
	
	else
	{
		cout << "rootAddr != recAddr" << endl;
		//Get the album in the far left slot of the node being split.
		//aka, get the album in the first index of the contents array of the
		//node being split.
		keyType album = n.contents[0]; 
		cout << "far left element in split node: " << album << endl;

		//Make a call to pAddr to get the address of the parent node to the
		//node we are splitting. 
		int pAddr = findpAddr(album, root, rootAddr, recAddr);
		cout << "address of node being split: " << recAddr << endl;
		cout << "address of parent node for the node being split:" << pAddr << endl;
		
			
	}
	cout << "writing new nodes in split node" << endl;
	//seek to the end of the file to write our new nodes there
	treeFile.seekg(0, ios::end);	
	oneAddr = treeFile.tellp();
	cout << "writing left child" << endl;
	cout << "treeFile.tellp(): " << treeFile.tellp() << endl;
	treeFile.write((char*) &left_child, sizeof(BTNode));

	//write right_child immediately after the left_child	
	twoAddr = treeFile.tellp();
	cout << "writing right child" << endl;
	cout << "treeFile.tellp(): " << treeFile.tellp() << endl;
	treeFile.write((char*) &right_child, sizeof(BTNode));

	//move the file pointer to the parent node position
	treeFile.seekg(recAddr);
	cout << "writing new parent" << endl;	
	cout << "treeFile.tellp(): " << treeFile.tellp() << endl;
	treeFile.write((char*) &parent, sizeof(BTNode));

	cout << "\n\n" << endl;
}
//method to determine if the current node is a leaf or not
//precondition: pass in the address of the record
//postcondition: return a boolean
bool BTree::isLeaf (int recAddr)
{
	BTNode n = getNode(recAddr);
	return isLeaf(n);
}
//method to determine if the current node is a leaf or not
//precondition: pass in the BTNode of the current record
//postcondition: return a boolean that tells us if we have a leaf or not
bool BTree::isLeaf(BTNode t)
{
	return t.child[0] == -1 ? true : false;
}
//method to count the leaves in a b-tree
//precondition: pass in the address of the record we're looking
//postcondition: return an integer with the number of leaves in a b-tree
int BTree::countLeaves(int recAddr)
{
	BTNode node = getNode(recAddr);
	bool isLeaf = true;
	int sum = 0;
	for(int i = 0; i <= node.currSize; i++)
	{
		if(node.child[i] != -1)
		{
			isLeaf = false;
			sum += countLeaves(node.child[i]);
		}
	}
	return isLeaf ? 1 : sum;
}

//method to adjust the root when we have a split that messes up the root
void BTree::adjRoot (keyType rootElem, int oneAddr, int twoAddr)
{

}

//method to split a node when we try to add a record to a full node
void BTree::splitNode (keyType& key,int recAddr,int& oneAddr,int& twoAddr)
{
	cout << "SPLITTING NODE" << endl;

	//create node for recAddr
	BTNode n = getNode(recAddr);

	//add all values in ValArray to a set
	set<keyType> s (n.contents, n.contents+n.currSize);

	//add new key to the set
	s.insert(key);

	//iterate through the set to find the middle element (the element that
	//will ultimately be promoted). This gets the index that the middle element
	//is stored in.
	int middle_index = (ORDER-1)/2;
	set<keyType>::iterator it = s.begin();

	//Use the advance method to move the iterator where we need it
	//The advance moves the iterator to the position of the middle_index value,
	//so if we start at index = 0, we want it to move middle_index-1 slots forward.
	advance(it, middle_index);

	//set the keyType parent equal to the element being promoted.
	keyType parent_album = *it;

	treeFile.seekg(0, ios::end);
	
	//update oneAddr and twoAddr - which is the left and right child respectively	
	oneAddr = treeFile.tellg();
	twoAddr = oneAddr+sizeof(BTNode);

	//update the key to be the parent_album so that we can use it in the placeNode method
//	key = parent_album;
}
//method to search through the tree when given a transaction file 
//initially -- pass in the string key (from the transaction file), root, and
//address of the root 
//updated -- pass in the album object so that this isn't stuck to only processing
//Album object
//postcondition: return a boolean indicating if the value was found or not
bool BTree::search (keyType key, BTNode t, int tAddr)
{
	cout << "Searching for " << key << endl;
	cout << "Searching in " << tAddr << endl;
	bool found = false;
	//search through contents to see if content == key; if same, return true.
	//if key < content, go to the child. 	
	if (tAddr != -1)
        {
		cout << "Size = " << t.currSize << endl;
		//look through the contents of the node to see if the key can be found
		for (int i = 0;  i < t.currSize;  i++)
                {
			cout << "Checking " << t.contents[i].getUPC() << endl;
			if (key == t.contents[i])
			{
				cout << "TRUE!!!!!!!!!!!!!!!!" << endl;
				return true;
			}
			else if (key < t.contents[i] && !isLeaf(t))
			{
				cout << "key < t.contents[i] && !isLeaf" << endl;
				//get the node of the child we are going to explore 
                		BTNode dummy = getNode(t.child[i]);

				//make a recursive call to search the child node
				return search(key, dummy, t.child[i]);
			}
		}
		//this means the key is greater than all values in the contents array. If it's
		//not a leaf, explore it's children. Else, it does not exist in the BTree.
		if (!isLeaf(t))
		{
			cout << "Checking last child" << endl;
			BTNode dummy = getNode(t.child[t.currSize]);
			return search(key,dummy,t.child[t.currSize]);
		}
        }
	//if leaf and can't find it, return false 
	return found;
}
	
