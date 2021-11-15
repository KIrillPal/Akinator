#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "akinator_tree.h"
#include "data_base.h"

#define VALUE(names, index) (names->data + AkiTreeGetValue(tree, index))

void getArg(char* buf, int max_length)
{
	fgets(buf, max_length, stdin);

	bool failed = 0;
	int length = strlen(buf);

	while (buf[length - 1] != '\n')
	{
		failed = 1;

		fgets(buf, max_length, stdin);
		length = strlen(buf);
	}
	if (failed)
	{
		printf("Sorry, max length: %d characters. Try again\n", max_length - 1);
		getArg(buf, max_length);
	}
	else buf[length - 1] = '\0';
}

void addItem(AkiTree* tree, size_t index, NameData * names)
{
	const int MAX_TEXT_LENGTH = 32;

	char     name[MAX_TEXT_LENGTH + 1] = { 0 };
	char question[MAX_TEXT_LENGTH + 1] = { 0 };


	printf("What you've thinked?\n");
	getArg(name, MAX_TEXT_LENGTH);

	printf("What difference between '%s' and '%s'?\nIt's ", VALUE(names, index), name);
	getArg(question, MAX_TEXT_LENGTH);


	AkiTreePushLeft  (tree, index, namesAppend(names, name));
	AkiTreePushRight (tree, index, AkiTreeGetValue(tree, index));
	AkiTreeSetValue(tree, index, namesAppend(names, question));

	printf("Ok. You won, but I'll remember it\n\n");
}

void play(AkiTree* tree, NameData* names, bool* tree_updated)
{
	printf("Let's play. Print 'yes' and 'no' or 'y' and 'n' to answer\n");

	size_t ptr = 0;
	char answer[10];

	while (true)
	{
		printf("Is it %s?\n", VALUE(names, ptr));

		do
		{
			scanf("%s%*c", answer);

			if (answer[0] == 'y')
			{
				size_t nxt = AkiTreeGetLeft(tree, ptr);

				if (nxt == AKITREE_NOPTR)
				{
					printf("I won!\n\n");
					*tree_updated = 0;
					return;
				}
				else ptr = nxt;

			}
			else if (answer[0] == 'n')
			{
				size_t nxt = AkiTreeGetRight(tree, ptr);

				if (nxt == AKITREE_NOPTR)
				{
					addItem(tree, ptr, names);
					*tree_updated = 1;
					return;
				}
				else ptr = nxt;

			}

		} while (answer[0] != 'y' && answer[0] != 'n');
	}
}

int main(int arc, const char* argv[])
{
	NameData names;
	AkiTree tree;

	struct stat data_stat;

	if (arc > 1 && stat(argv[1], &data_stat) == 0)
	{
		readDataBase(argv[1], &tree, &names);
	}
	else
	{
		names.size = 0;
		names.capacity = 0;
		names.data = NULL;

		AkiTreeCtor(&tree);

		AkiTreeAddValue(&tree, namesAppend(&names, "something"));
	}
	printf("Welcome! ");

	int top_printing = 1;

	while (true)
	{
		if (top_printing)
		{
			printf("Do you want to (p) - play, (g) - get definition, (d) - dump, (o) - exit?\n");
			top_printing = 0;
		}

		char command = 0;

		scanf("%c%*c", &command);
		
		switch (command)
		{
		case 'p': 
		{
			bool is_upd = 0;
			play(&tree, &names, &is_upd);

			if (arc > 1)
				saveDataBase(argv[1], &tree, &names);

			top_printing = 1;
			break;
		}
		case 'o': 	
		{
			AkiTreeDtor(&tree);
			free(names.data);
			printf("Goodbye!\n");
			return 0;
		}
		case 'd':
		{
			int number = 1;
			FILE* viz = fopen("debug.gv", "w");
			printTree(viz, &tree, names.data, 0, &number);
			fclose(viz);

			system("dot debug.gv -Tpng -o debug.png");

			printf("Tree was successfully drawn\n\n");

			top_printing = 1;
			break;
		}

		default:
			printf("Unknown: '%c'\n", command);
			break;
		}
	}

	return 0;
}