#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void PrintBasic(ofstream &fout)
{
	fout << "" << endl;
}

int main(int argc, char* argv[])
{
	string path = "";
	string file = "";
	if (argc > 1)// если передаем аргументы, то argc будет больше 1(в зависимости от кол-ва аргументов)
	{
		path = argv[1];
		path += "Engine\\Source\\";
		file = path + "Compiler\\Lib.cpp";
		cout << "Project files: " << path << endl;// вывод второй строки из массива указателей на строки(нумерация в строках начинается с 0 )
		ofstream fout;
		fout.open(file);
		fout << "#include \"stdafx.h\"" << endl;
		fout << "#include \"Compiler\\Compiler.h\"" << endl;
		fout << "namespace Lib" << endl;
		fout << "{" << endl;

		fout << "void InitFunc(Compiler *comp)" << endl;
		fout << "{" << endl;
		fout << "comp->AddFunc(\"int int::operator+(int)\");" << endl;

		fout << "}" << endl;

		fout << "}" << endl;
		fout.close();
	}
	else
	{
		cout << "Not arguments" << endl;
	}
	return 0;
}