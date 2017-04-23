#include "TimeUtility.h"
#include <sstream>

Time::Time(int hours_r, int minutes_r, int seconds_r)
{
	hours = hours_r;
	minutes = minutes_r;
	seconds = seconds_r;
}

Time::~Time()
{

}

string Time::getString()
{
	stringstream stringStream;
	stringStream << this->hours;
	stringStream << ":";
	stringStream << this->minutes;
	stringStream << ":";
	stringStream << this->seconds;
	return stringStream.str();
}

string Time::getNextScheduledTime(int timeDuration_r)
{
	int totalMinutes = this->minutes + timeDuration_r;
	if (totalMinutes >= 60)
	{
		totalMinutes -= 60;
		this->hours = this->hours + 1;
		this->minutes = totalMinutes;
	}
	else
	{
		this->minutes = totalMinutes;
	}

	return this->getString();
}
