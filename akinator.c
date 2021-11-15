#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <locale.h>
#include <wchar.h>
#include <windows.h>
#include "akinator_tree.h"
#include "data_base.h"

#define VALUE(names, index) (names->data + AkiTreeGetValue(tree, index))

void getArg(wchar_t* buf, int max_length)
{
	fgetws(buf, max_length, stdin);

	bool failed = 0;
	int length = wcslen(buf);

	while (buf[length - 1] != L'\n')
	{
		failed = 1;

		fgetws(buf, max_length, stdin);
		length = wcslen(buf);
	}
	if (failed)
	{
		wprintf(L"Ты весьма перестарался. Мы больше %d символов не принимаем\n", max_length - 1);
		getArg(buf, max_length);
	}
	else buf[length - 1] = L'\0';
}

void addItem(AkiTree* tree, size_t index, NameData * names)
{
	const int MAX_TEXT_LENGTH = 32;

	wchar_t     name[MAX_TEXT_LENGTH + 1] = { 0 };
	wchar_t question[MAX_TEXT_LENGTH + 1] = { 0 };


	wprintf(L"Ну и что ж ты загадал?\nЭто ");
	getArg(name, MAX_TEXT_LENGTH);

	wprintf(L"И чем объект '%ls' отличается от объекта '%ls'?\nОн ", VALUE(names, index), name);
	getArg(question, MAX_TEXT_LENGTH);


	AkiTreePushLeft  (tree, index, namesAppend(names, name));
	AkiTreePushRight (tree, index, AkiTreeGetValue(tree, index));
	AkiTreeSetValue(tree, index, namesAppend(names, question));

	wprintf(L"Ладно. Ты победил, кожаный мешок... Но я это запомню\n\n");
}

void play(AkiTree* tree, NameData* names, bool* tree_updated)
{
	wprintf(L"Ну поехали. Для ответа вводим 'да' и 'нет' или 'д' и 'н'\n");

	size_t ptr = 0;
	wchar_t answer[10];

	while (true)
	{
		wprintf(L"Этот объект %ls?\n", VALUE(names, ptr));

		do
		{
			wscanf(L"%ls%*lc", answer);

			if (answer[0] == L'д')
			{
				size_t nxt = AkiTreeGetLeft(tree, ptr);

				if (nxt == AKITREE_NOPTR)
				{
					wprintf(L"Ахахах я мудрец! Я снова угадал\n\n");
					*tree_updated = 0;
					return;
				}
				else ptr = nxt;

			}
			else if (answer[0] == L'н')
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

		} while (answer[0] != L'д' && answer[0] != L'н');
	}
}

int main(int arc, const char* argv[])
{
	setlocale(LC_ALL, "");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	fwide(stdin , 1);
	fwide(stdout, 1);

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

		AkiTreeAddValue(&tree, namesAppend(&names, L"неясно что"));
	}
	wprintf(L"Добро пожаловать! ");

	int top_printing = 1;

	while (true)
	{
		if (top_printing)
		{
			wprintf(L"Хотите (и) - играть, (п) - описать предмет, (д) - dump, (о) - выход?\n");
			top_printing = 0;
		}

		wchar_t command = 0;

		wscanf(L"%lc%*lc", &command);
		
		switch (command)
		{
		case L'и': 
		{
			bool is_upd = 0;
			play(&tree, &names, &is_upd);

			if (arc > 1)
				saveDataBase(argv[1], &tree, &names);

			top_printing = 1;
			break;
		}
		case L'о': 	
		{
			AkiTreeDtor(&tree);
			free(names.data);
			wprintf(L"Покедова!\n");
			return 0;
		}
		case L'д':
		{
			int number = 1;
			FILE* viz = fopen("debug.gv", "w");

			fwide(viz, 1);

			printTree(viz, &tree, names.data, 0, &number);
			fclose(viz);

			system("dot debug.gv -Tpng -o debug.png");

			wprintf(L"Ваше дерево успешно отрисовано\n\n");

			top_printing = 1;
			break;
		}

		default:
			wprintf(L"Какой такой '%lc'?\n", command);
			break;
		}
	}

	return 0;
}