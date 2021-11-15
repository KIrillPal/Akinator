#include "data_base.h"

#include <stdlib.h>

unsigned int namesAppend(NameData* names, const wchar_t* str)
{
	size_t new_size = names->size + wcslen(str) + 1;
	if (new_size > names->capacity)
	{
		if (names->capacity == 0)
			names->capacity = 16;

		while (names->capacity < new_size)
			names->capacity *= 2;

		names->data = (wchar_t*)realloc(names->data, names->capacity * sizeof(wchar_t));
	}

	unsigned int index = names->size;

	wcscpy(names->data + names->size, str);
	names->size = new_size;

	return index;
}

void readDataBase(const char* data_path, AkiTree* tree, NameData* names)
{
	FILE* data  = fopen(data_path, "rb");

	fread (&tree->capacity, sizeof(size_t), 1, data);
	tree->size = tree->capacity;
	tree->data = (AkiTreeNode*) calloc(tree->capacity, sizeof(AkiTreeNode));
	fread (tree->data, sizeof(AkiTreeNode), tree->size, data);


	fread (&names->capacity, sizeof(size_t), 1, data);
	names->size = names->capacity;
	names->data = (wchar_t*)calloc(names->capacity, sizeof(wchar_t));
	fread (names->data, sizeof(wchar_t), names->size, data);

	fclose(data);
}

void saveDataBase(const char* data_path, AkiTree* tree, NameData* names)
{
	FILE* data = fopen(data_path, "wb");

	fwrite (&tree->size, sizeof(size_t), 1, data);
	fwrite ( tree->data, sizeof(AkiTreeNode), tree->size, data);


	fwrite (&names->size, sizeof(size_t), 1, data);
	fwrite ( names->data, sizeof(wchar_t), names->size, data);

	fclose(data);
}