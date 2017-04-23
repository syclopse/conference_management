#include <iostream>
#include "ConferenceManager.h"

using namespace std;

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		cout << "Provide input file." << endl;
	}
	else
	{
		string fileName(argv[1]);
		cout << "File name : " << fileName.c_str() << endl;
		ConferenceManager conferenceManager;
		conferenceManager.scheduleConference(fileName);
	}
	cout << endl;
	cout << "Please press a key ...." << endl;
	getchar();
	return 0;
}