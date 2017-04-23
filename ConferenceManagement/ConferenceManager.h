#include "talk.h"
#include <vector>

using namespace std;

class ConferenceManager
{
public:
	// Constructor
	ConferenceManager();
	// Destructor
	~ConferenceManager();

	// Method name : scheduleConference
	// Input(s) : string
	// Output : vector of vector of Talk
	vector< vector<Talk> > scheduleConference(string fileName_r);

	// Method name : scheduleConference
	// Input(s) : vector of string
	// Output : vector of vector of Talk
	vector< vector<Talk> > scheduleConference(vector<string> talkList);

	// Method name : getTalkListFromFile
	// Input(s) : string
	// Output : vector of string
	vector<string> getTalkListFromFile(string fileName_r);

	// Method name : getTotalTalksTime
	// Input(s) : vector of Talk
	// Output : int
	int getTotalTalksTime(vector<Talk> talksList);

private:
	// Method name : validateAndCreateTalks
	// Input(s) : vector of string
	// Output : vector of Talk
	vector<Talk> validateAndCreateTalks(vector<string> talkList);

	// Method name : getScheduleConferenceTrack
	// Input(s) : vector of Talk
	// Output : vector of vector of Talk
	vector< vector<Talk> > getScheduleConferenceTrack(vector<Talk> talksList);

	// Method name : findPossibleCombSession
	// Input(s) : vector of Talk, int, bool
	// Output : vector of vector of Talk
	vector< vector<Talk> > findPossibleCombSession(vector<Talk> talksListForOperation,
		int totalPossibleDays, bool morningSession);

	// Method name : getScheduledTalksList
	// Input(s) : vector of Talk, vector of Talk
	// Output : vector of vector of Talk
	vector< vector<Talk> > getScheduledTalksList(vector< vector<Talk> > combForMornSessions,
		vector< vector<Talk> > combForEveSessions);
};