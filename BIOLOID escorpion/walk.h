#ifndef WALK_H_
#define WALK_H_
// function to update the walk state
void walk_setWalkState(int command);
// function to retrieve the walk state
// Returns (int) walk state
int walk_getWalkState();
// Function that allows 'seamless' transition between certain walk commands
// Handles transitions between 1. WFWD - WFLS - WFRS and
//                             2. WBWD - WBLS - WBRS
// All other transitions between walk commands have to go via their exit page
// Returns:	(int)	shift flag 0 - nothing happened
//							   1 - new motion page set
int walk_shift();
// function to avoid obstacles by turning left until path is clear
// Input:	obstacle flag from last execution
// Returns:	(int) obstacle flag 0 - no obstacle
//								1 - new obstacle, start avoiding
//								2 - currently avoiding obstacle
//							   -1 - finished avoiding
int walk_avoidObstacle(int obstacle_flag);
void walk_init();
#endif /* WALK_H_ */