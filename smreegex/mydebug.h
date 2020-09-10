#pragma once
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

string get_debugstr_fromfile(string filename) {
	string filetext;
	char buffer;
	ifstream myfile;
	myfile.open(filename, ios::in);
	while (!myfile.eof())
	{
		buffer = myfile.get();;
		filetext += buffer;
	}
	myfile.close();
	return filetext;
}
