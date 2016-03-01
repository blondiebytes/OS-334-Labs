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
	lock_init(&reaction->theLock);
}

void
reaction_h(struct reaction *reaction)
{
	lock_acquire(&reaction->theLock);
	reaction->numberOfHydrogen = reaction->numberOfHydrogen + 1;
	cond_broadcast(&reaction->readyToMakeWater, &reaction->theLock);
	cond_wait(&reaction->doneMakingWater, &reaction->theLock);
	lock_release(&reaction->theLock);
}

void
reaction_o(struct reaction *reaction)
{
	lock_acquire(&reaction->theLock);
	reaction->numberOfOxygen = reaction->numberOfOxygen + 1;
	while(reaction->numberOfHydrogen < 2 || reaction->numberOfOxygen < 1) {
			cond_wait(&reaction->readyToMakeWater, &reaction->theLock);
	}
    make_water();
    cond_signal(&reaction->doneMakingWater, &reaction->theLock);
    cond_signal(&reaction->doneMakingWater, &reaction->theLock);
    reaction->numberOfHydrogen = reaction->numberOfHydrogen - 2;
	reaction->numberOfOxygen = reaction->numberOfOxygen - 1;
	lock_release(&reaction->theLock);
}















