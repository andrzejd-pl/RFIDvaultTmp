#pragma once
#pragma warning(disable : 4996)

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <fstream>

using namespace std;

class  CodeContainer
{
public:

	CodeContainer() {}
	~CodeContainer() {}

	void WriteToContainer(Uid &card) {
		vector<Uid> tab;
		ReadFromContainer(tab);
		remove("container.txt");
		tab.push_back(card);

		fstream file("container.txt", ios::out);
		if (file.good())
		{
			for (auto x : tab) {
				wholecard = to_string(x.size) + ':';
				for (int i = 0; i < x.size; i++)
				{
					//itoa(card.uidByte[i], reinterpret_cast<char*>(&buffer), 10);
					wholecard = wholecard + to_string(x.uidByte[i]) + ':';
				}
				file << wholecard << endl;
			}
		}
		file.close();
	}

	void ReadFromContainer(vector<Uid> &tab) {

		fstream file("container.txt", ios::in);
		if (file.good())
		{
			//int  size = 0;
			string napis = "";
			Uid tmp;

			while (!file.eof())
			{
				getline(file, napis, ':');
				cout << napis << endl;
				tmp.size = atoi(napis.c_str());
				for (int i = 0; i < tmp.size; i++) {
					getline(file, napis, ':');
					tmp.uidByte[i] = atoi(napis.c_str());
				}
				tab.push_back(tmp);
			}
			file.close();
		}

	}


private:
	string buffer, wholecard;

};

