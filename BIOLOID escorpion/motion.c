/*
 * motion.c - functions for executing motion pages  
 *	requires a motion.h file created by the translate_motion Perl script 
 * 
 * Version 0.5		31/10/2011
 * Written by Peter Lanius
 * Please send suggestions and bug fixes to PeterLanius@gmail.com
 */

/*
 * You may freely modify and share this code, as long as you keep this
 * notice intact. Licensed under the Creative Commons BY-SA 3.0 license:
 *
 *   http://creativecommons.org/licenses/by-sa/3.0/
 *
 * Disclaimer: To the extent permitted by law, this work is provided
 * without any warranty. It might be defective, in which case you agree
 * to be responsible for all resulting costs and damages.
 */

#include <util/delay.h>
#include <stdio.h>
#include "global.h"
#include "pose.h"
#include "motion.h"
#include "walk.h"
#include "motion_f.h"
#include "dynamixel.h"
#include "clock.h"

// define the possible states for executeMotionSequence
#define STEP_IN_MOTION		1
#define STEP_IN_PAUSE		2
#define STEP_FINISHED		3
#define PAUSE_FINISHED		4
#define	PAGE_FINISHED		5
#define MOTION_ALARM		6
#define MOTION_STOPPED		7
#define ROBOT_SLIPPED		8

// and create the variables that guide these functions
uint8 motion_state = 7;					// motion state as per above definitions
unsigned long pause_start_time = 0;		// millis() at start of pause time
uint8 repeat_counter = 0;				// number of repeats of page already performed
uint8 exit_flag = 0;					// flag indicating we are on an exit page
uint8 last_joint_flex[NUM_AX12_SERVOS];		// last set of joint flex values
// timing variables
unsigned long step_start_time = 0, step_finish_time = 0, block_time = 0;

// Global variables related to the finite state machine that governs execution
extern volatile uint8 bioloid_command;			// current command
extern volatile uint8 last_bioloid_command;		// last command
extern volatile bool  new_command;				// flag that we got a new command

// global hardware definition variables
extern const uint8 AX12_IDS[NUM_AX12_SERVOS];
extern const uint8 AX12Servos[MAX_AX12_SERVOS];
// global variables that keep the current and next motion page
extern volatile uint8 current_motion_page;
extern volatile uint8 next_motion_page;			// next motion page if we got new command
extern uint8 current_step;						// number of the current motion page step

// should keep the current pose in a global array
extern int16 current_pose[NUM_AX12_SERVOS];

// table of pointers to the motion pages
uint8 * motion_pointer[NUM_MOTION_PAGES+1];

// struct for an unpacked motion page
struct MotionPage
{
	uint8 JointFlex[NUM_AX12_SERVOS]; 
	uint8 NextPage; 
	uint8 ExitPage; 
	uint8 RepeatTime; 
	uint8 SpeedRate10; 
	uint8 InertialForce; 
	uint8 Steps; 
	uint16 StepValues[MAX_MOTION_STEPS][NUM_AX12_SERVOS];
	uint16 PauseTime[MAX_MOTION_STEPS]; 
	uint16 PlayTime[MAX_MOTION_STEPS]; 
} CurrentMotion;


// the new implementation of AVR libc does not allow variables passed to _delay_ms
static inline void delay_ms(uint16 count) {
	while(count--) { 
		_delay_ms(1); 
	} 
}

// Initialize the motion pages by constructing a table of pointers to each page
// Motion pages are stored in Flash (PROGMEM) - see motion.h
void motionPageInit()
{
	// first we need to check file matches the configuration defined
	for (int i=0; i<MAX_AX12_SERVOS; i++)
	{
		if (AX12_ENABLED[i] != AX12Servos[i])
		{
			// configuration does not match
			printf("\nConfiguration of enabled AX-12 servos does not match motion.h. ABORT!\n");
			exit(-1);
		}
	}
	// We also need to check that the number of motion pages matches
	if (ACTIVE_MOTION_PAGES != NUM_MOTION_PAGES)
	{
			// configuration does not match
			printf("\nNumber of active motion pages does not match motion.h. ABORT!\n");
			exit(-1);
	}
	
	// set the initial motion state
	motion_state = MOTION_STOPPED;
	
	// Motion Page pointer assignment to PROGMEM 
	motion_pointer[0] = NULL; 
	motion_pointer[1] = (uint8*) &MotionPage1; 
	motion_pointer[2] = (uint8*) &MotionPage2; 
	motion_pointer[3] = (uint8*) &MotionPage3; 
	motion_pointer[4] = (uint8*) &MotionPage4; 
	motion_pointer[5] = (uint8*) &MotionPage5; 
	motion_pointer[7] = (uint8*) &MotionPage7; 
	motion_pointer[8] = (uint8*) &MotionPage8; 
	motion_pointer[10] = (uint8*) &MotionPage10; 
	motion_pointer[11] = (uint8*) &MotionPage11; 
	motion_pointer[12] = (uint8*) &MotionPage12; 
	motion_pointer[14] = (uint8*) &MotionPage14; 
	motion_pointer[15] = (uint8*) &MotionPage15; 
	motion_pointer[16] = (uint8*) &MotionPage16; 
	motion_pointer[18] = (uint8*) &MotionPage18; 
	motion_pointer[19] = (uint8*) &MotionPage19; 
	motion_pointer[21] = (uint8*) &MotionPage21; 
	motion_pointer[22] = (uint8*) &MotionPage22; 


}

// This function executes robot motions consisting of one or more motion 
// pages defined in motion.h
// It implements a finite state machine to know what it is doing and what to do next
// Code is meant to be reentrant so it can easily be converted to a task with a RTOS
void executeMotionSequence()
{
	uint8 moving_flag, temp1;
	int error_status, comm_status, left_right_step;
	
	// TEST: if ( motion_state != MOTION_STOPPED ) printf("\nMotion State = %i, Walk State = %i, Current Step = %i", motion_state, walkGetWalkState(), current_step);
	
	// check the states in order of likelihood of occurrence
	// the most likely state is that a motion step is still being executed or paused
	if ( motion_state == STEP_IN_MOTION )
	{
		// if walking we can't wait for motion to finish, go by step time instead
		if( walk_getWalkState() != 0 ) {
			if ( (millis()-step_start_time) >= CurrentMotion.PlayTime[current_step-1] ) {
				// step time is up, update state
				motion_state = STEP_FINISHED;
			} else {
				// play time isn't finished yet, return
				return;
			}
		} else {				
			// last state was step in motion - check if finished
			moving_flag = checkMotionStepFinished();
			// finished, update motion state
			if ( moving_flag == 0 ) {
				motion_state = STEP_FINISHED;
				step_finish_time = millis();
			} else {
				// step isn't finished yet, return
				return;
			}
		}		
	} else if( motion_state == STEP_IN_PAUSE ) {
		// check if we still need to wait for pause time to expire
		if ( (millis()-pause_start_time) >= CurrentMotion.PauseTime[current_step-1] )
		{
			// pause is finished, update state
			motion_state = PAUSE_FINISHED;
		} else {
			// pause isn't finished yet, return
			return;
		}		
	}
	
	// Next we check for any movement related alarms - at this point the only way the
	// motion state can be STEP_FINISHED is because it was changed above
	// Given this takes 11ms, that's too long for walking (may have to revisit)
	if ( motion_state == STEP_FINISHED && walk_getWalkState() == 0 )
	{
		// check that executing the last step didn't cause any alarms (takes 5ms)
		for (uint8 i=0; i<NUM_AX12_SERVOS; i++) {
			// ping the servo and unpack error code (if any)
			error_status = dxl_ping(AX12_IDS[i]);
			if(error_status != 0) {
				// there has been an error, disable torque
				comm_status = dxl_write_byte(BROADCAST_ID, DXL_TORQUE_ENABLE, 0);
				printf("\nexecuteMotionSequence Alarm ID%i - Error Code %i\n", AX12_IDS[i], error_status);
				motion_state = MOTION_ALARM;
				return;
			}
		}	
		// all ok, read back current pose (takes 6ms)
		readCurrentPose();	
	}
	
	// We also need to check if we received a RESET command after alarm shutdown
	if ( motion_state == MOTION_ALARM && bioloid_command == COMMAND_RESET )
	{
		// Reset the Dynamixel actuators - reset torque limit and re-enable torque
		comm_status = dxl_write_word(BROADCAST_ID, DXL_TORQUE_LIMIT_L, 0x3FF);
		comm_status = dxl_write_byte(BROADCAST_ID, DXL_TORQUE_ENABLE, 1);
		motion_state = MOTION_STOPPED;
	}
	
	// Now we can figure out what to do next
	// Options are:	1. Switch to ExitPage
	//				2. Respond to a change in walk command (see WALK EXECUTE task in RoboPlus Task sample files)
	//				3. Start Pause Time after step
	//				4. Execute next step in current motion page
	//				5. Go back to first step in current motion page (repeat)
	//				6. Switch to NextPage
	//				7. Respond to a new non-walk command 
	//				8. Nothing to do - wait for new command
	
	// first we deal with state changes at the end of a motion page
	if ( current_step == CurrentMotion.Steps && (motion_state == STEP_FINISHED || motion_state == PAUSE_FINISHED) )
	{
		// check if we just finished an exit page
		if ( exit_flag == 1 )
		{
			// yes, reset flag and change motion state and then return to not complicate things
			exit_flag = 0;
			motion_state = MOTION_STOPPED;
			return;
		}
		
		// we have finished the current page - determine the next motion page
		if ( bioloid_command == COMMAND_STOP )
		{
		// Option 1 - switch to exit page
			if ( CurrentMotion.ExitPage == 0 ) {
				// no exit page, stop
				motion_state = MOTION_STOPPED;
				return;
			} else {
				// need to execute an Exit Page before stopping		
				current_motion_page = CurrentMotion.ExitPage;
				exit_flag = 1;		// flag that we need to stop after the exit page
			}
		} 
		// Option 2 - respond to change in walk command (seamless transitions only)
		else if ( new_command == TRUE ) 
		{
			if ( walk_shift() == 1 ) {
				// walkShift already updates the current motion page
				new_command = FALSE;
			} else {
				// to transition to new command we first need to execute the exit page
				if ( CurrentMotion.ExitPage == 0 ) {
					// no exit page
					current_motion_page = 0;
					motion_state = MOTION_STOPPED;
					return;
				} else {
					// need to execute an Exit Page before new command		
					current_motion_page = CurrentMotion.ExitPage;
					exit_flag = 1;		// flag that we need to stop after the exit page
				}				
			}	
		} 
		// Option 5 - repeat the current motion page
		else if ( CurrentMotion.RepeatTime > repeat_counter )
		{
			// Update step, repeat counter and motion status
			current_step = 1;
			repeat_counter++;
			motion_state = STEP_IN_MOTION;
			// can go straight to executing step 1 since we have executed this page before
			step_start_time = executeMotionStep(current_step);
			return;
		} 
		// Option 6 - switch to NextPage motion page
		else if ( CurrentMotion.NextPage > 0 && CurrentMotion.NextPage <= NUM_MOTION_PAGES )
		{
			current_motion_page = CurrentMotion.NextPage;
		}
		// Nothing else to do - stop motion
		else
		{
			motion_state = MOTION_STOPPED;
			return;
		}

		// in Options 1,2,6 above we had a change of motion page - start execution
		unpackMotion(current_motion_page);
		if ( setMotionPageJointFlexibility() == 0 ) {
			// joint flex values set ok, execute motion
			current_step = 1;
			repeat_counter = 1;
			motion_state = STEP_IN_MOTION;
			step_start_time = executeMotionStep(current_step);
		} else {
			// this shouldn't really happen, but we need to cater to the eventuality
			comm_status = dxl_write_byte(BROADCAST_ID, DXL_TORQUE_ENABLE, 0);
			motion_state = MOTION_ALARM;
		}
		
		// either way we are finished here - return
		return;
	}	
	
	// now we can deal with state changes during page execution
	if ( motion_state == STEP_FINISHED )
	{
		// Option 3 - start pause after step
		if ( CurrentMotion.PauseTime[current_step-1] > 0 && bioloid_command != COMMAND_STOP )
		{
			// set the timer for the pause
			pause_start_time = millis();
			motion_state = STEP_IN_PAUSE;
			return;
		} else {
			// no pause required, go straight to executing next step
			motion_state = PAUSE_FINISHED;
		}
	}	
	
	if ( motion_state == PAUSE_FINISHED )
	{
		// Option 4 - execute next step in this motion page
		if ( current_step < CurrentMotion.Steps )
		{
			// Update step and motion status
			current_step++;
			motion_state = STEP_IN_MOTION;
			step_start_time = executeMotionStep(current_step);
		}
		// should never end up here
		else 
		{
			// reset to default
			motion_state = MOTION_STOPPED;
			current_motion_page = 0;
		}
		return;
	}
	
	// Option 7 - Respond to new command - set associated motion page
	if ( motion_state == MOTION_STOPPED && new_command == TRUE )
	{
		// special case for walk commands we need to get walk ready if we weren't walking before
		if( (last_bioloid_command == COMMAND_STOP || last_bioloid_command > COMMAND_WALK_READY) &&
		    ( bioloid_command >= COMMAND_WALK_FORWARD && bioloid_command < COMMAND_WALK_READY ) ) {
				// this is the only time we wait for a motion to finish before returning to the command loop!
				walk_init();
		} 
		// special case of shifting between walk commands - non-seamless transitions
		else if ( walk_getWalkState() > 0 && (bioloid_command >= COMMAND_WALK_FORWARD && bioloid_command < COMMAND_WALK_READY) )
		{
				// calculate the page number relative to start of previous command
				left_right_step = current_motion_page - COMMAND_WALK_READY_MP;
				temp1 = left_right_step / 12U;
				left_right_step -= temp1 * 12;
				// check if we finished on left or right step
				if ( left_right_step == 10 ) {
					// right step is next
					left_right_step = 2;
				} else {
					// left step is next
					left_right_step = 0;
				}
				// can calculate next motion page as in WALK EXECUTE 
				next_motion_page = (bioloid_command-1)*12 + COMMAND_WALK_READY_MP + left_right_step + 1;
		}
		
		if ( bioloid_command != COMMAND_STOP )
		{
			// unpack the new motion page and start the motion
			unpackMotion(next_motion_page);
			current_motion_page = next_motion_page;
			next_motion_page = 0;
			// also need to set walk state if it's a walk command
			if ( bioloid_command >= COMMAND_WALK_FORWARD && bioloid_command < COMMAND_WALK_READY ) {
				walk_setWalkState(bioloid_command);
			} else {
				// not a walk command, reset walk state
				walk_setWalkState(0);
			}
			
			if ( setMotionPageJointFlexibility() == 0 ) {
				// joint flex values set ok, execute motion
				current_step = 1;
				repeat_counter = 1;
				motion_state = STEP_IN_MOTION;
				step_start_time = executeMotionStep(current_step);
				new_command = FALSE;
			} else {
				// something went wrong when setting compliance slope
				current_motion_page = 0;
				next_motion_page = 0;
				new_command = FALSE;
				motion_state = MOTION_STOPPED;
			}
		} else {
			// execute STOP command
			current_motion_page = 0;
			next_motion_page = 0;
			new_command = FALSE;
			motion_state = MOTION_STOPPED;
		}
	} 
	// Option 8 - Nothing to do - keep waiting for new command
	else {
		return;
	}

}

// This function unpacks a motion stored in program memory (Flash) 
// in a struct stored in RAM to allow execution
// StartPage - number of the motion page to be unpacked
void unpackMotion(int StartPage)
{
	uint8 i, s, num_packed_steps;
	uint32 packed_step_values;
	
	// first we retrieve the Compliance Slope values
	for (i=0; i<NUM_AX12_SERVOS; i++)
	{
		CurrentMotion.JointFlex[i] = pgm_read_byte(motion_pointer[StartPage]+i);
	}
	// next we retrieve the play parameters (each are 1 byte)
	CurrentMotion.NextPage = pgm_read_byte(motion_pointer[StartPage]+NUM_AX12_SERVOS+0);
	CurrentMotion.ExitPage = pgm_read_byte(motion_pointer[StartPage]+NUM_AX12_SERVOS+1);
	CurrentMotion.RepeatTime = pgm_read_byte(motion_pointer[StartPage]+NUM_AX12_SERVOS+2);
	CurrentMotion.SpeedRate10 = pgm_read_byte(motion_pointer[StartPage]+NUM_AX12_SERVOS+3);
	CurrentMotion.InertialForce = pgm_read_byte(motion_pointer[StartPage]+NUM_AX12_SERVOS+4);
	CurrentMotion.Steps = pgm_read_byte(motion_pointer[StartPage]+NUM_AX12_SERVOS+5);
	
	// now we are ready to unpack the Step Values 
	// 3 values are packed into one 32bit integer - so use pgm_read_word twice
	num_packed_steps = NUM_AX12_SERVOS / 3;
	for (s=0; s<CurrentMotion.Steps; s++)
	{
		for (i=0; i<num_packed_steps; i++)
		{
			// higher 16bit
			packed_step_values = pgm_read_word(motion_pointer[StartPage]+NUM_AX12_SERVOS+6+(s*4*num_packed_steps)+4*i+2);
			packed_step_values = packed_step_values << 16;
			// lower 16bit
			packed_step_values += pgm_read_word(motion_pointer[StartPage]+NUM_AX12_SERVOS+6+(s*4*num_packed_steps)+4*i);
			// unpack and store
			CurrentMotion.StepValues[s][3*i+2] = packed_step_values & 0x3FF;
			packed_step_values = packed_step_values >> 11;
			CurrentMotion.StepValues[s][3*i+1] = packed_step_values & 0x3FF;
			packed_step_values = packed_step_values >> 11;
			CurrentMotion.StepValues[s][3*i] = packed_step_values & 0x3FF;
		}
		// Sanity Check - if the values are outside the overall Min/Max values we are probably accessing random memory
		for (i=0; i<NUM_AX12_SERVOS; i++)
		{
			if ( CurrentMotion.StepValues[s][i] > SERVO_MAX_VALUES[i] || CurrentMotion.StepValues[s][i] < SERVO_MIN_VALUES[i] )
			{
				// obviously have unpacked rubbish, stop right here
				printf("\nUnpack Motion Page %i, Step %i - rubbish data. STOP.", StartPage, s+1);
				printf("\nServo ID%i, Step Value = %i, Min = %i, Max = %i \n", AX12_IDS[i], CurrentMotion.StepValues[s][i], SERVO_MIN_VALUES[i],SERVO_MAX_VALUES[i] );
				exit(-1);
			}
		}
		
	}

	// and finally the play and pause times (in ms)
	// both need to be recalculated using the motion speed rate factor
	for (s=0; s<CurrentMotion.Steps; s++)
	{
		CurrentMotion.PauseTime[s] = pgm_read_word(motion_pointer[StartPage]+(NUM_AX12_SERVOS+6+CurrentMotion.Steps*4*num_packed_steps)+(s*2));
		if(CurrentMotion.PauseTime[s] != 0 && CurrentMotion.PauseTime[s] < 6500 ) {
			CurrentMotion.PauseTime[s] = (10*CurrentMotion.PauseTime[s]) / CurrentMotion.SpeedRate10; 
		} else {
			CurrentMotion.PauseTime[s] = 10 * (CurrentMotion.PauseTime[s]/CurrentMotion.SpeedRate10);
		}		
	}		
	for (s=0; s<CurrentMotion.Steps; s++)
	{
		CurrentMotion.PlayTime[s] = pgm_read_word(motion_pointer[StartPage]+(NUM_AX12_SERVOS+6+CurrentMotion.Steps*4*num_packed_steps+CurrentMotion.Steps*2)+(s*2));
		if(CurrentMotion.PlayTime[s] != 0 && CurrentMotion.PlayTime[s] < 6500 ) {
			CurrentMotion.PlayTime[s] = (10*CurrentMotion.PlayTime[s]) / CurrentMotion.SpeedRate10; 
		} else {
			CurrentMotion.PlayTime[s] = 10 * (CurrentMotion.PlayTime[s]/CurrentMotion.SpeedRate10);
		}		
	}		
}

// This function initiates the execution of a motion step in the given motion page
// Step - number of the step to be initiated
// Returns (long) start time of the step
unsigned long executeMotionStep(int Step)
{
	uint16 goalPose[NUM_AX12_SERVOS];
	unsigned long step_start_time;

	// Make sure we never access random memory by accident and damage the robot
	if ( Step > 0 && Step <= CurrentMotion.Steps )
	{
		// create the servo values array 
		for (int j=0; j<NUM_AX12_SERVOS; j++)
			{ goalPose[j] = CurrentMotion.StepValues[Step-1][j]; }
		// take the time
		step_start_time = millis();
		// execute the pose without waiting for completion
		moveToGoalPose(CurrentMotion.PlayTime[Step-1], goalPose, DONT_WAIT_FOR_POSE_FINISH);
		// return the start time to keep track of step timing
		return step_start_time;
	} else {
		// do nothing and return 0
		return 0;
	}
}

// This function initializes the joint flexibility values for the motion page
// Returns (int)  0  - all ok
//				 -1  - communication error
int setMotionPageJointFlexibility()
{
	uint8 complianceSlope;
	int commStatus;

	// now we can process the joint flexibility values
	for (uint8 i=0; i<NUM_AX12_SERVOS; i++) {
		// update is only required if different from last set of values
		if ( last_joint_flex[i] != CurrentMotion.JointFlex[i] )
		{
			// translation is bit shift operation (see AX-12 manual)
			complianceSlope = 1<<CurrentMotion.JointFlex[i]; 
			commStatus = dxl_write_byte(AX12_IDS[i], DXL_CCW_COMPLIANCE_SLOPE, complianceSlope);
			if(commStatus != COMM_RXSUCCESS) {
				// there has been an error, print and break
				printf("\nsetMotionPageJointFlexibility CCW ID%i - ", AX12_IDS[i]);
				dxl_printCommStatus(commStatus);
				return -1;
			}
			commStatus = dxl_write_byte(AX12_IDS[i], DXL_CW_COMPLIANCE_SLOPE, complianceSlope);
			if(commStatus != COMM_RXSUCCESS) {
				// there has been an error, print and break
				printf("\nsetMotionPageJointFlexibility CW ID%i - ", AX12_IDS[i]);
				dxl_printCommStatus(commStatus);
				return -1;
			}
		}
		// update values for next iteration
		last_joint_flex[i] = CurrentMotion.JointFlex[i];
	}
	return 0;
}

// Function to check for any remaining servo movement
// Returns:  (int)	number of servos still moving
int checkMotionStepFinished()
{
	uint8 moving_flag;
	
	// reset the flag
	moving_flag = 0;
		
	for (int i=0; i<NUM_AX12_SERVOS; i++) {
		// keep reading the moving state of servos 
		moving_flag += dxl_read_byte( AX12_IDS[i], DXL_MOVING );
		// if anything still moving - return
		if ( moving_flag == 1) {
			return moving_flag;
		}
	}		
	return 0;
}

// This function executes a single robot motion page defined in motion.h
// It waits for the motion to finish to return control, so it's no good 
// for a command loop
// StartPage - number of the first motion page in the motion
// Returns (int) StartPage of next motion in sequence (0 - no further motions)
int executeMotion(int StartPage)
{
	uint8 complianceSlope;
	int commStatus;
	uint16 goalPose[NUM_AX12_SERVOS];

	// temporary array of timings to keep track of step timing
	unsigned long step_times[MAX_MOTION_STEPS];
	unsigned long pre_step_time, total_time;

	// set the currently executed motion page global variable
	current_motion_page = StartPage;
	
	// first step is to unpack the motion
	unpackMotion(StartPage);
	
	// now we can process the joint flexibility values
	for (uint8 i=0; i<NUM_AX12_SERVOS; i++) {
		// translation is bit shift operation (see AX-12 manual)
		complianceSlope = 1<<CurrentMotion.JointFlex[i]; 
		commStatus = dxl_write_byte(AX12_IDS[i], DXL_CCW_COMPLIANCE_SLOPE, complianceSlope);
		if(commStatus != COMM_RXSUCCESS) {
			// there has been an error, print and break
			printf("executeMotion Joint Flex %i - ", AX12_IDS[i]);
			dxl_printCommStatus(commStatus);
			return 0;
		}
		commStatus = dxl_write_byte(AX12_IDS[i], DXL_CW_COMPLIANCE_SLOPE, complianceSlope);
		if(commStatus != COMM_RXSUCCESS) {
			// there has been an error, print and break
			printf("executeMotion Joint Flex %i - ", AX12_IDS[i]);
			dxl_printCommStatus(commStatus);
			return 0;
		}
	}
	
	total_time = millis();
	
	// in case the motion repeats we need a loop
	for (int r=1; r<=CurrentMotion.RepeatTime; r++)
	{
		// loop over the steps and execute poses
		for (int s=0; s<CurrentMotion.Steps; s++)
		{
			// create the servo values array 
			for (int j=0; j<NUM_AX12_SERVOS; j++)
				{ goalPose[j] = CurrentMotion.StepValues[s][j]; }
			// take the time
			pre_step_time = millis();
			// execute each pose 
			moveToGoalPose(CurrentMotion.PlayTime[s], goalPose, WAIT_FOR_POSE_FINISH);
			// store the time
			step_times[s] = millis() - pre_step_time;
			
			// now pause if required
			if(CurrentMotion.PauseTime[s] > 0) 
				{ delay_ms(CurrentMotion.PauseTime[s]); }
		}
	}
	
	total_time = millis() - total_time; 
	
	// TEST: printf("\nMotion %i Timing :", StartPage);
	// TEST: for (int s=0; s<CurrentMotion.Steps; s++) { printf(" %lu,", step_times[s]); }
	// TEST: printf(" Total: %lu", total_time);
	
	// return the page of the next motion in sequence
	return (int) CurrentMotion.NextPage;	
}

// This function executes the exit page motion for the  
// current motion page
 void executeMotionExitPage()
{
	int	NextPage;
	
	// find the exit page
	NextPage = CurrentMotion.ExitPage;
	// execute the exit page if it exists
	if (NextPage != 0)
	{
		executeMotionSequence(NextPage);
	}
}