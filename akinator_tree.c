#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "akinator_tree.h"

#define LIST_NOLIST -2

int AkiTreeCtor(AkiTree* tree)
{
	tree->capacity = 0;
	tree->size = 0;
	tree->data = NULL;
	return 0;
}

int AkiTreeDtor(AkiTree* tree)
{
	free(tree->data);

	tree->capacity = 0;
	tree->size = 0;
	tree->data = NULL;
	return 0;
}

int AkiTreeEnlarge(AkiTree* tree, size_t cap)
{
	if (tree->capacity < cap)
	{
		tree->data = (AkiTreeNode*)realloc(tree->data, cap * sizeof(AkiTreeNode));
		tree->capacity = cap;
	}

	return 0;
}

int AkiTreeSetValue(AkiTree * tree, size_t index, unsigned int value)
{
	tree->data[index].value = value;
	return 0;
}

unsigned int AkiTreeGetValue(AkiTree * tree, size_t index)
{
	return tree->data[index].value;
}

size_t AkiTreeGetLeft(AkiTree* tree, unsigned int index)
{
	return tree->data[index].left;
}

size_t AkiTreeGetRight(AkiTree* tree, unsigned int index)
{
	return tree->data[index].right;
}

size_t AkiTreeAddValue(AkiTree* tree, unsigned int value)
{
	if (tree->size == tree->capacity)
	{
		size_t cap = tree->capacity ? tree->capacity * 2 : 4;
		AkiTreeEnlarge(tree, cap);
	}

	tree->data[tree->size].value = value;
	tree->data[tree->size].left  = AKITREE_NOPTR;
	tree->data[tree->size].right = AKITREE_NOPTR;

	return tree->size++;
}

size_t AkiTreePushLeft(AkiTree* tree, unsigned int index, unsigned int value)
{
	size_t node = AkiTreeAddValue(tree, value);
	tree->data[index].left = node;
	return node;
}

size_t AkiTreePushRight(AkiTree* tree, unsigned int index, unsigned int value)
{
	size_t node = AkiTreeAddValue(tree, value);
	tree->data[index].right = node;
	return node;
}

void printTree(FILE* fout, AkiTree* tree, wchar_t* values, size_t ptr, int* number)
{
	int mynum = *number;
	(*number)++;

	if (ptr == AKITREE_NOPTR)
	{
		fwprintf(fout, L"    p%d[label = \"NULL\"]\n", mynum);
		return;
	}

	if (mynum == 1)
		fwprintf(fout, L"digraph DebugTree {\n");

	fwprintf(fout, L"    p%d[shape = none label = <\n", mynum);
	fwprintf(fout,
		L"	<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">	  \n\
			<TR>																	 	  \n\
				<TD COLSPAN = \"2\"><FONT COLOR = \"blue\"><B>\"%ls\"</B></FONT></TD>     \n\
			</TR>																		  \n\
			<TR>																		  \n\
				<TD  WIDTH=\"60\" HEIGHT=\"25\">%lld</TD>								  \n\
				<TD  WIDTH=\"60\" HEIGHT=\"25\">%lld</TD>								  \n\
			</TR>																		  \n\
		</TABLE >>\n]",
		values + tree->data[ptr].value, tree->data[ptr].left, tree->data[ptr].right);

	fwprintf(fout, L"\n    p%d -> p%d\n", mynum, *number);
	printTree(fout, tree, values, tree->data[ptr].left, number);

	fwprintf(fout, L"\n    p%d -> p%d\n", mynum, *number);
	printTree(fout, tree, values, tree->data[ptr].right, number);



	if (mynum == 1)
		fwprintf(fout, L"}\n");
}
