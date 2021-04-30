
#include "btree.h"

using namespace std;

//default constructor
BTree::BTree()
{

}

void BTree::writeHeader (char * fileName)
{

}

//insert key into B-tree
void BTree::insert (keyType key)
{

}

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

bool BTree::search (string key)
{
	return false;
}

keyType BTree::retrieve (string key)
{
	keyType k;
	return k;
}

//method to return number of read/writes in part 2
void BTree::totalio() const
{

}

//public countLeaves
int BTree::countLeaves()
{
	return 0;
}

//PRIVATE METHODS
void BTree::printTree (int rootAddr)
{
	if (rootAddr != -1)
        {
                BTNode dummy = getNode(rootAddr);
                printNode(rootAddr);
                for (int i = 0;  i <= dummy.currSize;  i++)
                        printTree(dummy.child[i]);
        }
}
void BTree::inorder (int rootAddr)
{

}

void BTree::reverse (int rootAddr)
{

}
int BTree::findAddr (keyType key, BTNode t, int tAddr)
{
	return 0;
}
int BTree::findpAddr(keyType key, BTNode t, int tAddr)
{
	return 0;
}
void BTree::insert (keyType key, int recAddr, int oneAddr, int twoAddr)
{

}
BTNode BTree::getNode (int recAddr)
{
	BTNode n;
	return n;
}
void BTree::printNode(int recAddr)
{

}
void BTree::placeNode (keyType k,int recAddr,int oneAddr,int twoAddr)
{

}
bool BTree::isLeaf (int recAddr)
{
	return false;
}
bool BTree::isLeaf(BTNode t)
{
	return false;
}
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
void BTree::adjRoot (keyType rootElem, int oneAddr, int twoAddr)
{

}
void BTree::splitNode (keyType& key,int recAddr,int& oneAddr,int& twoAddr)
{

}
bool BTree::search (string key, BTNode t, int tAddr)
{
	bool found = false;
	return found;
}
	
