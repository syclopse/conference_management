#include "talk.h"

Talk::Talk(string title_r, string name_r, int time_r) : scheduled(false)
{
	title.assign(title_r);
	name.assign(name_r);
	timeDuration = time_r;
}

Talk::~Talk()
{
}

void Talk::setScheduled(bool scheduled_r)
{
	this->scheduled = scheduled_r;
}

void Talk::setScheduledTime(string scheduledTime_r)
{
	this->scheduledTime.assign(scheduledTime_r);
}

string Talk::getScheduledTime()
{
	return scheduledTime;
}

int Talk::getTimeDuration()
{
	return timeDuration;
}

string Talk::getTitle()
{
	return title;
}

bool Talk::isScheduled()
{
	return scheduled;
}
