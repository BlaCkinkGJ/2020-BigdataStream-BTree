// btree.h

#ifndef BTREE_H
#define BTREE_H
/*
template <class keyType>
class BTree;
*/
#include "recfile.h"
#include "fixfld.h"
#include "indbuff.h"
#include "btnode.h"

// btree needs to be able to pack, unpack, read and
// 	write index records
// 	also, handle the insert, delete, split, merge,
//	growth of the tree
//	a pool of nodes will be managed
//	keep at least a branch of the tree in memory
//	

template <class keyType>
class BTree
	// this is the full version of the BTree
{
public:
	BTree(int order, int keySize = sizeof(keyType), int unique = 1);
	~BTree();
	int Open(char * name, int mode);
	int Create(char * name, int mode);
	int Close();
	int Insert(const keyType key, const int recAddr);
	int Remove(const keyType key, const int recAddr = -1);
	int Search(const keyType key, const int recAddr = -1);
	int Delete(const keyType key, const int recAddr = -1);
	void Print(ostream &);
	void Print(ostream &, int nodeAddr, int level);
protected:
    typedef BTreeNode<keyType> BTNode;
	BTNode* FindLeaf(const keyType key);
	// load a branch into memory down to the leaf with key
	BTNode * NewNode();
	BTNode * Fetch(const int recaddr);
	int Store(BTNode *);
	BTNode Root;
	int Height; // height of tree
	int Order; // order of tree
	int PoolSize;
	BTNode ** Nodes; // pool of available nodes
					 // Nodes[1] is level 1, etc. (see FindLeaf)
					 // Nodes[Height-1] is leaf
	FixedFieldBuffer Buffer;
	RecordFile<BTNode> BTreeFile;

	friend ostream& operator<< (ostream& os, BTree& bt) {
        os << "BTree of height " << bt.Height << " is " << endl;
        bt.Root.Print(os);
        if (bt.Height>1)
            for (int i = 0; i<bt.Root.numKeys(); i++)
            {
                bt.Print(os, bt.Root.RecAddrs[i], 2);
            }
        os << "end of BTree" << endl;
        return os;
	}
};
#endif
