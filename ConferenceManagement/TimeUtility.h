#include <iostream>
#include <string>

using namespace std;

class Time
{
public:
	// Constructor
	Time(int hours_r, int minutes_r, int seconds_r);
	// Destructor
	~Time();

	// Method name : getString
	// Input(s) : void
	// Output : string
	string getString();

	// Method name : getNextScheduledTime
	// Input(s) : int
	// Output : string
	string getNextScheduledTime(int timeDuration_r);

private:
	int hours;
	int minutes;
	int seconds;
};
