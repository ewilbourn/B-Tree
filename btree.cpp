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
	inorder(rootAddr);	
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
          	{
	              cout << "child[i]: " << dummy.child[i] << endl;
		      printTree(dummy.child[i]);
		}
        }
}

//print an inorder traversal of the tree
void BTree::inorder (int rootAddr)
{	
	if (rootAddr != -1)
        {
                BTNode dummy = getNode(rootAddr);
                for (int i = 0;  i <= dummy.currSize;  i++)
          	{
	              cout << "child[i]: " << dummy.child[i] << endl;
		      inorder(dummy.child[i]);
		}
        	printNode(rootAddr);
        }
	
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

		cout << "root elements BEFORE the split: " << endl;
		printNode(rootAddr);
//		placeNode (key,recAddr,oneAddr,twoAddr);
		splitNode (key,recAddr,oneAddr,twoAddr);
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
	cout << "\tis our node a leaf?" << std::boolalpha << isLeaf(n) << endl;
	cout << "\trecAddr: " << recAddr << "\n" << endl;
	for (int i = 0; i < n.currSize; i++)
	{
		cout << "\tcontents["  << i << "]" << endl;
		cout << "\t" << n.contents[i] << endl;	
	}
	for (int i = 0; i < ORDER; i++)
	{
		cout << "n.child[" << i << "]: " << n.child[i] << endl;
	}
}

//after retrieving the addresses for the left and right children (oneAddr/twoAddr)
void BTree::placeNode (keyType k,int recAddr,int oneAddr,int twoAddr)
{
	
	cout << "ONEADDR BEING PASSED INTO PLACENODE: " << oneAddr << endl;
	cout << "TWOADDR BEING PASSED INTO PLACENODE: " << twoAddr << endl;

	cout << "root contents when first entering placeNode: " << endl;
	printNode(rootAddr);	
	//create node for recAddr
	BTNode n = getNode(recAddr);

	//create a set of all contents in our contents array and add our new key to
	//the set; this will be used to figure out which value needs to be promoted.
	set<keyType> s (n.contents, n.contents+n.currSize);
	s.insert(k);
	set<keyType>::iterator it = s.begin();

	//CREATE 3 NEW NODES FOR THE SPLIT
	BTNode parent; //recAddr
	parent.currSize = 0;

	//used if rootAddr != recAddr
	BTNode new_parent;	
	
	BTNode left_child; //oneAddr 
	left_child.currSize = 0;

	BTNode right_child; //twoAddr
	right_child.currSize = 0;

	//find the middle index, or the value that is being promoted in our set.
	it = s.begin();
	int middle_index = (ORDER-1)/2;
	advance(it, middle_index);
	keyType p_album = *it;
	it = s.begin();
	
	cout << "root contents: " << endl;
	printNode(rootAddr);	
	//fill in the contents arrays for the parent/left_child/right_child nodes
	for (int i = 0; i < s.size(); i++)
	{
		cout << "\nroot contents: " << endl;
		printNode(rootAddr);	
		cout << endl;
		keyType album = *it;
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
	
	cout << "root contents before setting children of parent node: " << endl;
	printNode(rootAddr);	

	//set the children of parent node
	parent.child[0] = oneAddr;
	parent.child[1] = twoAddr;

	cout << "root contents before setting children of l/r child nodes: " << endl;
	printNode(rootAddr);	
	//fill leaves for new children
	for(int i = 0; i < ORDER; i++)
	{
		left_child.child[i] = -1;
		right_child.child[i] = -1;
	}

	cout << "\nroot contents after setting children of l/r child nodes: " << endl;
	printNode(rootAddr);	
	cout << "\nrootAddr: " << rootAddr << endl;
	cout << "address of node being split: " << recAddr << endl;

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
	cout << "root contents before getting the parent : " << endl;
	printNode(rootAddr);
	
	BTNode split_node_parent;		
	if (pAddr != -1)
	{
		//get the node for the parent of the node being split.
		split_node_parent = getNode(pAddr);
	}
	
	//update the root with the new root, if our node being split is a root
	if (rootAddr == recAddr)	
		adjRoot (parent.contents[0], oneAddr, twoAddr);
	cout << "root contents: " << endl;
	printNode(rootAddr);	

	//ALGORITHM FOR RAISING A NODE TO AN ALREADY OCCUPIED NODE? NOT NEW ROOT.
	//1. Create a set of Pairs with our parent node and repective children.
	
	if (rootAddr != recAddr)
	{
		cout << "rootAddr != recAddr" << endl;
		
		//now, make pairs for all the elements in the contents array	
		//add all Pairs in ValArray to a set of Pairs
		set<Pair>s_pairs;
		set<Pair>::iterator p_it = s_pairs.begin(); //iterator for our set of Pairs
		
		//create the pair for the raised node and its children
		Pair p_raised;
		p_raised.element = parent.contents[0];
		p_raised.loffset = oneAddr;
		p_raised.roffset = twoAddr;
		s_pairs.insert(p_raised);
		
		cout << "RAISED NODE" << endl;
		cout << "p_raised.element: " << p_raised.element << endl;
		cout << "p_raised.loffset: " << p_raised.loffset << endl;
		cout << "p_raised.roffset: " << p_raised.roffset << endl;
		cout << endl;
	
		cout << "****************************************************" << endl;
		cout << "creating the Pairs: " << endl;
		//create the Pairs
		for (int i = 0; i < split_node_parent.currSize; i++)
		{
			cout << "PAIR FOR LOOP: i = " << i << endl; 
			cout << "\nroot contents: " << endl;
			printNode(rootAddr);	
			cout << "\np_album: " << p_album << endl;
			cout << "n.contents[i]: " << split_node_parent.contents[i] << endl;
		
			//if the key is supposed to be the first node inserted into the
			//parent node, then execute this bit
			if ((p_album < split_node_parent.contents[i]) && i == 0)
			{
				cout << "p_album < n.contents[i] && i == 0: " << endl;
				cout <<  p_album << " < " << split_node_parent.contents[i] << endl;
				//create the pair for the current thing in contents[i]
				//with the adjusted left child offset
				Pair p_parent;
				cout << "Pair element: " << split_node_parent.contents[i] << endl;
				cout << "Pair loffset: " << p_raised.roffset << endl;
				cout << "Pair roffset: " << split_node_parent.child[i+1] << endl;
				p_parent.element = split_node_parent.contents[i];
				p_parent.loffset = p_raised.roffset;
				p_parent.roffset = split_node_parent.child[i+1];
				cout << "before insert." << endl;
				s_pairs.insert(p_parent);
				cout << "after insert" << endl;
			}
			//if the key is supposed to be the last node inserted into the parent node
			else if ((split_node_parent.contents[i] < p_album) && i == split_node_parent.currSize)
			{	
				//create the pair for the current element in contents[i]
				//while adjusting the right offset to be the same as the 
				//left offset for the p_raised Pair	
				Pair p_parent1;
				p_parent1.element = split_node_parent.contents[i];
				p_parent1.loffset = split_node_parent.child[i];
				p_parent1.roffset = p_raised.loffset;
				s_pairs.insert(p_parent1);	
			}
			else
			{
				//create the pair for the other elements in contents[i]
				Pair p;
				p.element = split_node_parent.contents[i];
				p.loffset = split_node_parent.child[i];
				p.roffset = split_node_parent.child[i+1];
				s_pairs.insert(p);
			}
		}
		cout << "FINISHED MAKING ALL PAIRS" << endl;
		//if the key is being raised to the middle of the parent, then fix the right offset
		//of the element to the left of the key. Also, fix the left offset of the element to the
		//right of the key.
		
		//reset position of the Pair set iterator
		p_it = s_pairs.begin();	
		cout << "FINISHED RESETING ITERATOR POSITION" << endl;
		keyType pair_key = parent.contents[0];
		cout << "LOOKING FOR KEY IN SET OF PAIRS" << endl;
		cout << "number of Pairs: " << s_pairs.size() << endl;
		//find the key in the set of Pairs
		for (int i = 0; i < s_pairs.size(); i++)
		{
			Pair curr_elem = *p_it;
			
			cout << "curr_elem: " << curr_elem.element << endl;
			//when we find the middle key in the set of Pairs, we need to adjust the offsets 
			//for the adjacent elements in the set of Pairs.
			if (curr_elem.element == pair_key && s_pairs.size() > 1)
			{
				cout << "if curr_elem.element == pair_key: " << endl;
				cout << "curr_elem.element: " << curr_elem.element << endl;
				cout << "pair_key: " << pair_key << endl;
				
				cout << "before decrement the iterator" << endl;
				
				int amount_to_advance = 1;
				if (i != 0)
				{
					//decrement the iterator to look at the Pair to left of current Pair
					p_it--;
					cout << "after decrement the iterator" << endl;
				
					//fix right offset of the left Pair
					Pair left = *p_it;
					cout << "left.element: " << left.element << endl;
					left.roffset = curr_elem.loffset;
	
					//remove the thing being pointed to by the iterator so that we can add 
					//back the new Pair with the corrected offsets to the set.
					cout << "erasing p_it" << endl;
					s_pairs.erase(p_it);	
					cout << "inserting p_it" << endl;	
					s_pairs.insert(left);
					amount_to_advance = 2;
				}
				//increment the pair iterator twice so that we can point to the to the 
				//right of the current Pair object.
				advance(p_it, amount_to_advance);
				
				//fix left offset of the right Pair
				Pair right = *p_it;
				right.loffset = curr_elem.roffset;
				cout << "erase and insert stuff" << endl;	
				//remove the thing being pointed to by the iterator so that we can add 
				//back the new Pair with the corrected offsets to the set.
				s_pairs.erase(p_it);		
				cout << "erased p_it" << endl;
				s_pairs.insert(right);
				cout << "after erase and insert stuff" << endl;	
			}
			cout << "increment pair iterator" << endl;
			//increment the iterator	
			p_it++;					
		} 
		/////////////////////////////////////////////////////////////////////////////////
		//
		//
		//Now, the pairs have been made. Correct the parent node that's being written to the file.
		
		//reset position of the Pair set iterator
		p_it = s_pairs.begin();	
		//BTNode new_parent;	
		new_parent.currSize = 0;
		cout << "\n\nprinting final contents of parent node: " << endl;
		//put the contents of our Pairs into a new Node that will be entered into the tree
		for (int i = 0; i < s_pairs.size(); i++)
		{	
			if (parent.currSize == ORDER-1)
			{
				treeFile.seekg(0, ios::end);
		
				//update oneAddr and twoAddr - which is the left and right child respectively	
				oneAddr = treeFile.tellg();
				twoAddr = oneAddr+sizeof(BTNode);
	
				//call splitNode recursively to split the new full parents	
				splitNode(k, pAddr, oneAddr, twoAddr);
			}
			
			Pair curr_pair = *p_it;
			new_parent.contents[i] = curr_pair.element;
			cout << "new_parent.contents[" << i << "]: " << new_parent.contents[i] << endl;
			new_parent.child[i] = curr_pair.loffset;
			cout << "new_parent.child[" << i << "]: " << new_parent.child[i] << endl;
			new_parent.child[i+1] = curr_pair.roffset;
			cout << "new_parent.child[" << i+1 << "]: " << new_parent.child[i+1] << endl;
			new_parent.currSize+=1;
			cout << "new_parent.currSize: " << new_parent.currSize << endl;
			cout << endl;	
			p_it++;
		}
		//fill up rest of children nodes for new_parent with -1's
		if (new_parent.currSize < ORDER-1)
		{
			for (int i = new_parent.currSize+1; i < ORDER; i++)
			{
				new_parent.child[i] = -1;
			}
		}
		cout << "ALL CHILDREN OF THE NEW_PARENT: " << endl;
		for (int i = 0; i < ORDER; i++)
		{
			cout << new_parent.child[i] << endl;
		}			
		cout << endl;
		//update the root with the new root, if our node is a root
		if(rootAddr == pAddr)
		{
			cout << "rootAddr  == pAddr" << endl;
			root = new_parent;
			
			cout << "PRINT NEW_PARENT NODE" << endl;	
			for (int i = 0; i < new_parent.currSize; i++)
			{
				cout << "contents[" << i << "]: " << new_parent.contents[i] << endl;
			//	cout << "child[" << i << "]: " << new_parent.child[i] << endl;
			}
	
			cout << endl;
			//FIX THE HEADER NODE ROOT ADDRESS
			treeFile.seekg(0, ios::beg);
			BTNode header1;
			header1.child[0] = rootAddr;
			treeFile.write((char*)&header1, sizeof(BTNode));//else, this is 
		}
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
	
		if (rootAddr != recAddr)
		{
			cout << "PRINTING WHEN PUSHING NODE TO ALREADY OCCUPIED ROOT" << endl;
			//move the file pointer to the parent node position
			treeFile.seekg(pAddr);
			cout << "writing new parent" << endl;	
			cout << "new_parent.currSize: " << new_parent.currSize << endl;
			cout << "treeFile.tellp(): " << treeFile.tellp() << endl;
			treeFile.write((char*) &new_parent, sizeof(BTNode));
		
			cout << "rootAddr: " << rootAddr << endl;
			printNode(rootAddr);
		}
	
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

//method to update the root when we split the root
//precondition: pass in the element that will next be stored in the root,
//the left child address (oneAddr), and the right child address (twoAddr)
//postcondition: return nothing, but the root is rewritten to our treefile
//and our header file is updated
void BTree::adjRoot (keyType rootElem, int oneAddr, int twoAddr)
{
	cout << "ADJUSTING ROOT" << endl;
	cout << "print root before updating it: " << endl;
	printNode(rootAddr);
	cout << "oneAddr: " << oneAddr << endl;
	cout << "twoAddr: " << twoAddr << endl;

	//increment the height by 1.
	height += 1;

	BTNode new_root;
	new_root.currSize = 1;
	new_root.contents[0] = rootElem;
	new_root.child[0] = oneAddr; //left child address
	new_root.child[1] = twoAddr; //right child address

	//fill in the rest of the children addresses
	for (int i = 2; i < ORDER; i++)
	{
		new_root.child[i] = -1;
	}
	
	root = new_root;	

	cout << "print root after updating it: " << endl;
	printNode(rootAddr);

	//fix the header node to reflect this update
	treeFile.seekg(0, ios::beg);
	BTNode header;
	header.child[0] = rootAddr;
	treeFile.write((char*)&header, sizeof(BTNode));//else, this is 

	//increment writes to the file
	write += 1;

	//move the file pointer to the root node position and overwrite the root
	treeFile.seekg(rootAddr);
	cout << "writing new parent" << endl;	
	cout << "treeFile.tellp(): " << treeFile.tellp() << endl;
	treeFile.write((char*) &new_root, sizeof(BTNode));
	
	//increment writes to the file
	write += 1;

	cout << "print root after updating it: " << endl;
	printNode(rootAddr);

	cout << "LEAVE ADJUSTING ROOT" << endl;
}

//method to split a node when we try to add a record to a full node
void BTree::splitNode (keyType& key,int recAddr,int& oneAddr,int& twoAddr)
{
	cout << "SPLITTING NODE" << endl;
	cout << "root contents immediately after entering splitNode: " << endl;
	printNode(rootAddr);	

	treeFile.seekg(0, ios::end);
	
	//update oneAddr and twoAddr - which is the left and right child respectively	
	oneAddr = treeFile.tellg();
	twoAddr = oneAddr+sizeof(BTNode);

	cout << "root contents after defining oneAddr and twoAddr: " << endl;
	printNode(rootAddr);	
	cout << "SPLITTING NODE - ONEADDR: " << oneAddr << endl;
	cout << "SPLITTING NODE - TWOADDR: " << twoAddr << endl;

	BTNode n = getNode(recAddr);
	cout << "root contents after calling getNode(recAddr): " << endl;
	printNode(rootAddr);	

	keyType album = n.contents[0]; 
	cout << "far left element in split node: " << album << endl;

	cout << "root contents right before entering placeNode: " << endl;
	printNode(rootAddr);	
	
	//call placeNode to perform the actual split/placing of node
	placeNode (key, recAddr, oneAddr, twoAddr);
	
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
	
