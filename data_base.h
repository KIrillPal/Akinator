#ifndef DATA_BASE_H
#define DATA_BASE_H

#include <stdio.h>
#include <string.h>
#include "akinator_tree.h"

struct NameData
{
	char* data;
	size_t size;
	size_t capacity;
};

unsigned int namesAppend(NameData* names, const char* str);

void readDataBase(const char* data_path, AkiTree* tree, NameData* names);
void saveDataBase(const char* data_path, AkiTree* tree, NameData* names);

#endif