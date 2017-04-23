#include <string>

using namespace std;

class Talk
{
public:
	// Constructor
	Talk(string title_r, string name_r, int time_r);
	// Destructor
	~Talk();

	// Method name : setScheduled
	// Input(s) : bool
	// Output : void
	void setScheduled(bool scheduled_r);

	// Method name : setScheduledTime
	// Input(s) : string
	// Output : void
	void setScheduledTime(string scheduledTime_r);

	// Method name : getScheduledTime
	// Input(s) : void
	// Output : string
	string getScheduledTime();

	// Method name : getTimeDuration
	// Input(s) : void
	// Output : int
	int getTimeDuration();

	// Method name : getTitle
	// Input(s) : void
	// Output : string
	string getTitle();

	// Method name : isScheduled
	// Input(s) : void
	// Output : bool
	bool isScheduled();

private:
	string title;
	string name;
	int timeDuration;
	bool scheduled;
	string scheduledTime;
};
