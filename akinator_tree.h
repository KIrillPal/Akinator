#ifndef TREE_H
#define TREE_H

#define AKITREE_NOPTR -1

struct AkiTreeNode {
	size_t left, right;
	unsigned int value;
};

struct AkiTree
{
	AkiTreeNode* data;
	size_t capacity;
	size_t size;
};

int AkiTreeCtor (AkiTree* tree);
int AkiTreeDtor (AkiTree* tree);

int AkiTreeSetValue (AkiTree* tree, size_t index, unsigned int value);
unsigned int AkiTreeGetValue (AkiTree* tree, size_t index);

size_t AkiTreeGetLeft  (AkiTree* tree, unsigned int value);
size_t AkiTreeGetRight (AkiTree* tree, unsigned int value);

size_t AkiTreeAddValue  (AkiTree* tree, unsigned int value);
size_t AkiTreePushLeft  (AkiTree* tree, unsigned int index, unsigned int value);
size_t AkiTreePushRight (AkiTree* tree, unsigned int index, unsigned int value);

int AkiTreeEnlarge(AkiTree* tree, size_t cap);

void printTree(FILE* fout, AkiTree* tree, char* values, size_t ptr, int* number);

#endif
