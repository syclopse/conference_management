#include "gtest/gtest.h"
#include "ConferenceManager.h"

TEST(CMTest, simpletest)
{
	ConferenceManager conferenceManager;
	EXPECT_NE(conferenceManager.scheduleConference("C:\\Users\\DELL\\Desktop\\ConferenceManagement\\ConferenceList.txt").size(), 0);
}