#pragma once
#pragma warning(disable : 4996)

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <sstream>
#include "MFRC522.h"

using namespace std;

using Uid = MFRC522::Uid;

struct Card {
	Uid uid;
	string name;
	bool validated;
};

class  CodeContainer
{
public:

	CodeContainer() {}
	~CodeContainer() {}

	void WriteToContainer(Card &card) {
		vector<Card> tab = ReadFromContainer();
		remove("/var/www/rfidVault/data/knownCards");
		tab.push_back(card);

		fstream file("/var/www/rfidVault/data/knownCards", ios::out);
		if (file.good())
		{
			for (auto x : tab) {
				wholecard = to_string(x.uid.size) + ':';
				for (int i = 0; i < x.uid.size; i++)
				{
					//itoa(card.uidByte[i], reinterpret_cast<char*>(&buffer), 10);
					wholecard = wholecard + to_string(x.uid.uidByte[i]) + ':';
				}
				wholecard[wholecard.length() - 2] = ';';

				file << wholecard << x.name << ";";
				if (x.validated)
					file << "1" << endl;
				else
					file << "0" << endl;

			}
		}
		file.close();
	}

	vector<Card> ReadFromContainer() {
		vector<Card> tab;
		fstream file("/var/www/rfidVault/data/knownCards", ios::in);
		if (file.good())
		{
			//int  size = 0;
			string napis = "";

			while (!file.eof())
			{
				Card tmp;
				getline(file, napis, ':');
				cout << napis << endl;
				tmp.uid.size = atoi(napis.c_str());
				string buff;
				istringstream buffStream(buff);
				getline(file, buff, ';');
				for (int i = 0; i < tmp.uid.size; i++) {
					getline(buffStream, napis, ':');
					tmp.uid.uidByte[i] = atoi(napis.c_str());
				}

				getline(file, buff, ';');
				tmp.name = buff;
				getline(file, buff);

				if (buff == "1")
					tmp.validated = true;
				else
					tmp.validated = false;

				tab.push_back(tmp);
			}
			file.close();
		}

		return tab;
	}


private:
	string buffer, wholecard;

};

