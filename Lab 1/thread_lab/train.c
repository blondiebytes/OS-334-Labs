//
// Kathryn Hodge
// 

#include "pintos_thread.h"
struct station {
	int isTrainHere;
	int peopleBoarded;
	int peopleSeated;
	int numOfPeopleWaiting;
	int numOfPeopleOriginallyWaiting;
	int trainSize;
	struct lock theLock;
	struct condition trainAvaliableCV;
	struct condition onBoardCV;
	
};

void
station_init(struct station *station)
{
	station->isTrainHere = 0;
	station->numOfPeopleWaiting = 0;
	station->peopleBoarded = 0;
	station->peopleSeated = 0;
	station->trainSize = 0;
	station->numOfPeopleOriginallyWaiting = 0;
	lock_init(&station->theLock);
	cond_init(&station->trainAvaliableCV);
	cond_init(&station->onBoardCV);
}

void
station_load_train(struct station *station, int count)
{
	lock_acquire(&station->theLock);
	station->peopleBoarded = 0;
	station->peopleSeated = 0;
	station->trainSize = count;
	station->isTrainHere = 1;
	station->numOfPeopleOriginallyWaiting = station->numOfPeopleWaiting;
	while ((station->numOfPeopleWaiting > 0 && station->peopleSeated < station->trainSize
		&& station->isTrainHere == 1) || (station->numOfPeopleWaiting == 0 &&
		station->peopleSeated < station->numOfPeopleOriginallyWaiting)) {
		cond_broadcast(&station->trainAvaliableCV, &station->theLock);
		cond_wait(&station->onBoardCV,  &station->theLock);
	}
	station->isTrainHere = 0;
	station->numOfPeopleOriginallyWaiting = 0;
	lock_release(&station->theLock);
}

void
station_wait_for_train(struct station *station)
{
	lock_acquire(&station ->theLock);
	station->numOfPeopleWaiting = station->numOfPeopleWaiting + 1;
	while (station->peopleBoarded + station->peopleSeated >= station->trainSize 
		|| station->isTrainHere == 0) {
		cond_wait(&station->trainAvaliableCV, &station->theLock);
	}
	station->numOfPeopleWaiting = station->numOfPeopleWaiting - 1;
	station->peopleBoarded+=1;
	lock_release(&station->theLock);
}

void
station_on_board(struct station *station)
{
	lock_acquire(&station->theLock);
	station->peopleBoarded-=1;
	station->peopleSeated+=1;
	if (!((station->numOfPeopleWaiting > 0 && station->peopleSeated < station->trainSize
		&& station->isTrainHere == 1) || (station->numOfPeopleWaiting == 0 &&
		station->peopleSeated < station->numOfPeopleOriginallyWaiting))) {
		cond_signal(&station->onBoardCV,  &station->theLock);
	}
	lock_release(&station->theLock);
}















