#include "ConferenceManager.h"
#include "TimeUtility.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

ConferenceManager::ConferenceManager()
{
}

ConferenceManager::~ConferenceManager()
{
}

vector<vector<Talk>> ConferenceManager::scheduleConference(string fileName_r)
{
	vector<string> talkList = getTalkListFromFile(fileName_r);
	return scheduleConference(talkList);
}

vector<vector<Talk>> ConferenceManager::scheduleConference(vector<string> talkList)
{
	vector<Talk> talksList = validateAndCreateTalks(talkList);
	return getScheduleConferenceTrack(talksList);
}

vector<string> ConferenceManager::getTalkListFromFile(string fileName_r)
{
	vector<string> talkList;

	// Open the file
	ifstream inputFile(fileName_r);
	string line;

	if (inputFile.is_open())
	{
		while (getline(inputFile, line))
		{
			talkList.push_back(line);
		}
		inputFile.close();
	}
	else
	{
		cout << "Unable to open file : " << fileName_r << endl;
	}

	return talkList;
}

int ConferenceManager::getTotalTalksTime(vector<Talk> talksList)
{
	if (talksList.empty())
	{
		return 0;
	}

	int totalTime = 0;

	for (vector<Talk>::iterator talkItr = talksList.begin(); talkItr < talksList.end(); ++talkItr)
	{
		totalTime += (*talkItr).getTimeDuration();
	}
	return totalTime;
}

vector<Talk> ConferenceManager::validateAndCreateTalks(vector<string> talkList)
{
	// If talkList is empty then handle it.
	if (talkList.empty())
	{
		cout << "talkList is empty." << endl;
		exit(0);
	}

	vector<Talk> validTalksList;
	int talkCount = -1;
	string minSuffix = "min";
	string lightningSuffix = "lightning";
	string name = "";
	string timeStr = "";

	// Iterate vector and validate time
	for (vector<string>::iterator strVectItr = talkList.begin(); strVectItr < talkList.end(); ++strVectItr)
	{
		// If talk does not have any space, means either title or time is missing.
		if ((*strVectItr).rfind(" ") == string::npos)
		{
			cout << "Invalid talk." << endl;
			exit(0);
		}
		else
		{
			name = (*strVectItr).substr(0, (*strVectItr).rfind(" "));
			timeStr = (*strVectItr).substr((*strVectItr).rfind(" ") + 1);

			if (name.empty())
			{
				cout << "Invalid talk name." << endl;
				exit(0);
			}
			else if ((timeStr.rfind(minSuffix) == string::npos) &&
				(timeStr.rfind(lightningSuffix) == string::npos))
			{
				cout << "Invalid talk time" << timeStr.c_str() << ". Time must be in min or in lightning." << endl;
				exit(0);
			}
			else
			{
				talkCount++;
				int time = 0;

				// Parse time from the time string
				if (timeStr.rfind(minSuffix) != string::npos)
				{
					time = atoi(timeStr.substr(0, timeStr.rfind(minSuffix)).c_str());
				}
				else if (timeStr.rfind(lightningSuffix) != string::npos)
				{
					string lightningTime = timeStr.substr(0, timeStr.rfind(lightningSuffix));
					if (lightningTime.empty())
					{
						time = 5;
					}
					else
					{
						time = atoi(lightningTime.c_str()) * 5;
					}
				}

				// Add talk to valid talk list
				validTalksList.push_back(Talk((*strVectItr), name, time));
			}
		}
	}
	return validTalksList;
}

struct Compare
{
	bool operator()(Talk& first, Talk& second) const
	{
		return first.getTimeDuration() > second.getTimeDuration();
	}
};

vector<vector<Talk>> ConferenceManager::getScheduleConferenceTrack(vector<Talk> talksList)
{
	// Find the total possible days
	int perDayMinTime = 6 * 60;
	int totalTalksTime = getTotalTalksTime(talksList);
	int totalPossibleDays = totalTalksTime / perDayMinTime;

	// Sort the talksList
	vector<Talk> talksListForOperation(talksList);

	sort(talksListForOperation.begin(), talksListForOperation.end(), Compare());

	// Find possible combinations for the morning sesssion
	vector< vector<Talk> > combForMornSessions = findPossibleCombSession(talksListForOperation, totalPossibleDays, true);

	// Remove all the scheduled talks for combForMornSessions, from the talksListForOperation
	for (vector< vector<Talk> >::iterator talkVecVecIter = combForMornSessions.begin();
	talkVecVecIter < combForMornSessions.end(); ++talkVecVecIter)
	{
		for (vector<Talk>::iterator talkVecIter = (*talkVecVecIter).begin();
		talkVecIter < (*talkVecVecIter).end(); ++talkVecIter)
		{
			for (int iCount = talksListForOperation.size() - 1; iCount >= 0; --iCount)
			{
				if ((*talkVecIter).getTitle().compare(talksListForOperation.at(iCount).getTitle()) == 0)
				{
					talksListForOperation.erase(talksListForOperation.begin() + iCount);
				}
			}
		}
	}

	// Find possible combinations for the morning sesssion
	vector< vector<Talk> > combForEveSessions = findPossibleCombSession(talksListForOperation, totalPossibleDays, false);

	// Remove all the scheduled talks for combForEveSessions, from the talksListForOperation
	for (vector< vector<Talk> >::iterator talkVecVecIter = combForEveSessions.begin();
	talkVecVecIter < combForEveSessions.end(); ++talkVecVecIter)
	{
		for (vector<Talk>::iterator talkVecIter = (*talkVecVecIter).begin();
		talkVecIter < (*talkVecVecIter).end(); ++talkVecIter)
		{
			for (int iCount = talksListForOperation.size() - 1; iCount >= 0; --iCount)
			{
				if ((*talkVecIter).getTitle().compare(talksListForOperation.at(iCount).getTitle()) == 0)
				{
					talksListForOperation.erase(talksListForOperation.begin() + iCount);
				}
			}
		}
	}

	// Check if the talksListForOperation is not empty, then try to fill all the remaining talks in evening session
	int maxSessionTimeLimit = 240;

	if (!talksListForOperation.empty())
	{
		vector<Talk> scheduledTalkList;

		for (vector< vector<Talk> >::iterator talkVecVecIter = combForEveSessions.begin();
		talkVecVecIter < combForEveSessions.end(); ++talkVecVecIter)
		{
			int totalTime = getTotalTalksTime((*talkVecVecIter));

			for (vector<Talk>::iterator talkItr = talksListForOperation.begin();
			talkItr < talksListForOperation.end(); ++talkItr)
			{
				int talkTime = (*talkItr).getTimeDuration();

				if ((talkTime + totalTime) <= maxSessionTimeLimit)
				{
					(*talkVecVecIter).push_back(*talkItr);
					(*talkItr).setScheduled(true);
					scheduledTalkList.push_back(*talkItr);
				}
			}

			for (vector<Talk>::iterator talkVecIter = scheduledTalkList.begin();
			talkVecIter < scheduledTalkList.end(); ++talkVecIter)
			{
				for (int iCount = talksListForOperation.size() - 1; iCount >= 0; --iCount)
				{
					if ((*talkVecIter).getTitle().compare(talksListForOperation.at(iCount).getTitle()) == 0)
					{
						talksListForOperation.erase(talksListForOperation.begin() + iCount);
					}
				}
			}

			if (talksListForOperation.empty())
			{
				break;
			}
		}
	}

	// If talksListForOperation is still not empty, its mean  the conference can not be scheduled with the provided data
	if (!talksListForOperation.empty())
	{
		cout << "Unable to schedule all task for conferencing." << endl;
		exit(0);
	}

	// Schedule the day event from morning session and evening session
	return getScheduledTalksList(combForMornSessions, combForEveSessions);
}

vector<vector<Talk>> ConferenceManager::findPossibleCombSession(vector<Talk> talksListForOperation, int totalPossibleDays, bool morningSession)
{
	int minSessionTimeLimit = 180;
	int maxSessionTimeLimit = 240;

	if (morningSession)
	{
		maxSessionTimeLimit = minSessionTimeLimit;
	}

	int talkListSize = talksListForOperation.size();
	vector< vector<Talk> > possibleCombinationsOfTalks;
	int possibleCombinationCount = 0;

	// Loop to get combination for total possible days
	// Check one by one from each talk to get possible combination
	for (int count = 0; count < talkListSize; ++count)
	{
		int startPoint = count;
		int totalTime = 0;
		vector<Talk> possibleCombinationList;

		// Loop to get possible combination
		while (startPoint != talkListSize)
		{
			int currentCount = startPoint;
			startPoint++;

			Talk currentTalk = talksListForOperation.at(currentCount);
			if (currentTalk.isScheduled())
			{
				continue;
			}

			int talkTime = currentTalk.getTimeDuration();
			// If the current talk time is greater than maxSessionTimeLimit or
			// sum of the current time and total of talk time added in list is greater than maxSessionTimeLimit
			// then continue
			if (talkTime > maxSessionTimeLimit || talkTime + totalTime > maxSessionTimeLimit)
			{
				continue;
			}

			possibleCombinationList.push_back(currentTalk);
			totalTime += talkTime;

			// If total time is completed for this session than break this loop
			if (morningSession)
			{
				if (totalTime == maxSessionTimeLimit)
				{
					break;
				}
			}
			else if (totalTime >= minSessionTimeLimit)
			{
				break;
			}
		}

		// Valid session time for morning session is equal to maxSessionTimeLimit
		// Valid session time for evening session is less then or equal to maxSessionTimeLimit and greater
		// than or equal to minSessionTimeLimit

		bool validSession = false;
		if (morningSession)
		{
			validSession = (totalTime == maxSessionTimeLimit);
		}
		else
		{
			validSession = (totalTime >= minSessionTimeLimit && totalTime <= maxSessionTimeLimit);
		}

		// If session is valid than add this session in the possible combination list and set
		// all added talk as scheduled
		if (validSession)
		{
			possibleCombinationsOfTalks.push_back(possibleCombinationList);
			for (vector<Talk>::iterator posCombListVecIter = possibleCombinationList.begin();
			posCombListVecIter < possibleCombinationList.end(); ++posCombListVecIter)
			{
				for (vector<Talk>::iterator talksListForOperationItr = talksListForOperation.begin();
				talksListForOperationItr < talksListForOperation.end(); ++talksListForOperationItr)
				{
					if ((*posCombListVecIter).getTitle().compare((*talksListForOperationItr).getTitle()) == 0)
					{
						(*talksListForOperationItr).setScheduled(true);
					}
				}
				(*posCombListVecIter).setScheduled(true);
			}
			possibleCombinationCount++;
			if (possibleCombinationCount == totalPossibleDays)
			{
				break;
			}
		}
	}

	return possibleCombinationsOfTalks;
}

vector<vector<Talk>> ConferenceManager::getScheduledTalksList(vector<vector<Talk>> combForMornSessions,
	vector<vector<Talk>> combForEveSessions)
{
	vector< vector<Talk> > scheduledTalksList;
	int totalPossibleDays = combForMornSessions.size();

	// For loop to schedule event for all days
	for (int dayCount = 0; dayCount < totalPossibleDays; ++dayCount)
	{
		vector<Talk> talkList;

		int trackCount = dayCount + 1;
		Time timeObj(9, 0, 0);
		string scheduledTime(timeObj.getString());

		cout << "Track " << trackCount << ":" << endl;

		// Morning Session - set the scheduled time in the talk and get the next time using time duration of current talk
		vector<Talk> mornSessionTalkList = combForMornSessions.at(dayCount);
		for (vector<Talk>::iterator talkItr = mornSessionTalkList.begin();
		talkItr < mornSessionTalkList.end(); ++talkItr)
		{
			(*talkItr).setScheduledTime(scheduledTime);
			cout << scheduledTime << " " << (*talkItr).getTitle() << endl;
			scheduledTime = timeObj.getNextScheduledTime((*talkItr).getTimeDuration());
			talkList.push_back((*talkItr));
		}

		// Scheduled lunch time for 60 mins
		int lunchTimeDuration = 60;
		Talk lunchTalk("Lunch", "Lunch", 60);
		lunchTalk.setScheduledTime(scheduledTime);
		talkList.push_back(lunchTalk);
		cout << scheduledTime << " " << "Lunch" << endl;
		scheduledTime = timeObj.getNextScheduledTime(lunchTimeDuration);

		// Evening Session - set the scheduled time in the talk and get the next time using time duration of current talk
		vector<Talk> eveSessionTalkList = combForEveSessions.at(dayCount);
		for (vector<Talk>::iterator talkItr = eveSessionTalkList.begin();
		talkItr < eveSessionTalkList.end(); ++talkItr)
		{
			(*talkItr).setScheduledTime(scheduledTime);
			talkList.push_back(*talkItr);
			cout << scheduledTime << " " << (*talkItr).getTitle() << endl;
			scheduledTime = timeObj.getNextScheduledTime((*talkItr).getTimeDuration());
		}

		// Scheduled networking event at the end of session, time duration is just to initialize the talk object
		Talk networkingTalk("Networking Event", "Networking Event", 60);
		networkingTalk.setScheduledTime(scheduledTime);
		talkList.push_back(networkingTalk);
		cout << scheduledTime << " " << "Networking Event" << endl;

		scheduledTalksList.push_back(talkList);
	}
	return scheduledTalksList;
}
