//
// Kathryn Hodge
//

#include "pintos_thread.h"

// Forward declaration. This function is implemented in reaction-runner.c,
// but you needn't care what it does. Just be sure it's called when
// appropriate within reaction_o()/reaction_h().
void make_water();

struct reaction {
	int numberOfOxygen;
	int numberOfHydrogen;
	//int hydroflag;
	struct lock theLock;
	struct condition readyToMakeWater;
	struct condition doneMakingWater;
};

void
reaction_init(struct reaction *reaction)
{
	cond_init(&reaction->readyToMakeWater);
	cond_init(&reaction->doneMakingWater);
	reaction->numberOfOxygen = 0;
	//reaction->hydroflag = 0;
	lock_init(&reaction->theLock);
}

void
reaction_h(struct reaction *reaction)
{
	lock_acquire(&reaction->theLock);
	reaction->numberOfHydrogen = reaction->numberOfHydrogen + 1;
	// if (reaction->hydroflag == 1) {
	// 	reaction->hydroflag = 0;
	// } else {
	// 	reaction->hydroflag = 1;
	// }
	printf("HydroFunction: NumberOfHydro: %d \n", reaction->numberOfHydrogen);
	printf("HydroFunction: NumberOfOxygen: %d \n", reaction->numberOfOxygen);
	cond_broadcast(&reaction->readyToMakeWater, &reaction->theLock);
	cond_wait(&reaction->doneMakingWater, &reaction->theLock);
	lock_release(&reaction->theLock);
}

void
reaction_o(struct reaction *reaction)
{
	lock_acquire(&reaction->theLock);
	reaction->numberOfOxygen = reaction->numberOfOxygen + 1;
	printf("OxygenFunction: NumberOfHydro: %d \n", reaction->numberOfHydrogen);
	printf("OxygenFunction: NumberOfOxygen: %d \n", reaction->numberOfOxygen);
	while(reaction->numberOfHydrogen < 2 || reaction->numberOfOxygen < 1) {
	//	if (reaction->hydroflag == 0) {
			cond_wait(&reaction->readyToMakeWater, &reaction->theLock);
	//	} else {
	//		break;
	//	}
	}
	cond_broadcast(&reaction->doneMakingWater, &reaction->theLock);
    make_water();
    // Book keeping
    reaction->numberOfHydrogen = reaction->numberOfHydrogen - 2;
	reaction->numberOfOxygen = reaction->numberOfOxygen - 1;
	lock_release(&reaction->theLock);
}















