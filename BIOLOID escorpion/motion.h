#ifndef __MOTION_H_
#define __MOTION_H_
/* ==========================================================================
 
   COMPONENT:        Global motion variable definitions
   AUTHOR:           Peter Lanius
   DESCRIPTION:      This component defines the motion pages and related
                     global variables. Auto-generated from the *.mtn file.
 
========================================================================== */
 
#include <stdint.h>
#include <avr/pgmspace.h>
#include "global.h"
 
// Array showing which Dynamixel servos are enabled in motion file 
const uint8 AX12_ENABLED[MAX_AX12_SERVOS] = {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0}; 

struct // init 
{ 
   const uint8 JointFlexibility[18]; 
   const uint8 NextPage; 
   const uint8 ExitPage; 
   const uint8 RepeatTime; 
   const uint8 SpeedRate10; 
   const uint8 InertialForce; 
   const uint8 Steps; 
   const uint32 StepValues[1][6]; 
   const uint16 PauseTime[1]; 
   const uint16 PlayTime[1]; 
} MotionPage1 PROGMEM = { 
{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5}, 
0, 0, 1, 10, 32, 1, 
{{2148532736,2148532736,2148532736,2148532736,2148532736,2148532736}}, 
{0}, {1000} 
}; 

struct // sleep 
{ 
   const uint8 JointFlexibility[18]; 
   const uint8 NextPage; 
   const uint8 ExitPage; 
   const uint8 RepeatTime; 
   const uint8 SpeedRate10; 
   const uint8 InertialForce; 
   const uint8 Steps; 
   const uint32 StepValues[2][6]; 
   const uint16 PauseTime[2]; 
   const uint16 PlayTime[2]; 
} MotionPage2 PROGMEM = { 
{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5}, 
0, 0, 2, 10, 32, 2, 
{{2148532635,2563377853,2148532545,2945653147,1389732460,2043658802},
 {2148532635,2563377853,2148532545,2945653147,1389732480,2043658802}}, 
{0,0}, {1000,1000} 
}; 

struct // jam 
{ 
   const uint8 JointFlexibility[18]; 
   const uint8 NextPage; 
   const uint8 ExitPage; 
   const uint8 RepeatTime; 
   const uint8 SpeedRate10; 
   const uint8 InertialForce; 
   const uint8 Steps; 
   const uint32 StepValues[6][6]; 
   const uint16 PauseTime[6]; 
   const uint16 PlayTime[6]; 
} MotionPage3 PROGMEM = { 
{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5}, 
0, 0, 1, 27, 32, 6, 
{{2316222876,2567963135,2148532736,2148737436,2148532866,2505431844},
 {1855074716,2567963135,2148532736,2148737436,2148532866,2505431844},
 {2316222876,2567963135,2148532736,2148737436,2148532866,2505431844},
 {1855074716,2567963135,2148532736,2148737436,2148532866,2505431844},
 {2316222876,2567963135,2148532736,2148737436,2148532866,2505431844},
 {1808957852,2567963135,2148532736,2148737436,2148532866,2794939166}}, 
{0,0,32,0,0,0}, {2040,312,312,312,312,2040} 
}; 

struct // base 
{ 
   const uint8 JointFlexibility[18]; 
   const uint8 NextPage; 
   const uint8 ExitPage; 
   const uint8 RepeatTime; 
   const uint8 SpeedRate10; 
   const uint8 InertialForce; 
   const uint8 Steps; 
   const uint32 StepValues[1][6]; 
   const uint16 PauseTime[1]; 
   const uint16 PlayTime[1]; 
} MotionPage4 PROGMEM = { 
{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5}, 
0, 0, 1, 10, 32, 1, 
{{1808957852,2567963135,2148532736,2148737436,2148532866,2794939166}}, 
{0}, {1000} 
}; 

struct // wake 
{ 
   const uint8 JointFlexibility[18]; 
   const uint8 NextPage; 
   const uint8 ExitPage; 
   const uint8 RepeatTime; 
   const uint8 SpeedRate10; 
   const uint8 InertialForce; 
   const uint8 Steps; 
   const uint32 StepValues[2][6]; 
   const uint16 PauseTime[2]; 
   const uint16 PlayTime[2]; 
} MotionPage5 PROGMEM = { 
{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5}, 
0, 0, 1, 10, 32, 2, 
{{2148532636,2567963135,2148532736,2148737436,2148532866,3369659023},
 {1808957852,2567963135,2148532736,2148737436,2148532866,2794939166}}, 
{0,0}, {1672,712} 
}; 

struct // attack 
{ 
   const uint8 JointFlexibility[18]; 
   const uint8 NextPage; 
   const uint8 ExitPage; 
   const uint8 RepeatTime; 
   const uint8 SpeedRate10; 
   const uint8 InertialForce; 
   const uint8 Steps; 
   const uint32 StepValues[3][6]; 
   const uint16 PauseTime[3]; 
   const uint16 PlayTime[3]; 
} MotionPage7 PROGMEM = { 
{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5}, 
0, 0, 1, 16, 32, 3, 
{{2148530588,2567676555,2148532666,2438144412,2148532866,2723402335},
 {2148532636,2567553735,2148532616,2610028996,2232377986,3654871607},
 {2148530588,2567676555,2148532666,2438144412,2148532866,2744554212}}, 
{24,464,0}, {1000,72,1000} 
}; 

struct // attack ready 
{ 
   const uint8 JointFlexibility[18]; 
   const uint8 NextPage; 
   const uint8 ExitPage; 
   const uint8 RepeatTime; 
   const uint8 SpeedRate10; 
   const uint8 InertialForce; 
   const uint8 Steps; 
   const uint32 StepValues[4][6]; 
   const uint16 PauseTime[4]; 
   const uint16 PlayTime[4]; 
} MotionPage8 PROGMEM = { 
{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5}, 
0, 0, 1, 15, 32, 4, 
{{2148530588,2567615145,2148532626,2610110876,2148532866,3407145636},
 {2148530588,2567656085,2148532636,2568167836,2148532866,2954328827},
 {2148530588,2567676555,2148532646,2526224796,2148532866,2618755899},
 {2148530588,2567676555,2148532666,2438144412,2148532866,2744554212}}, 
{0,0,0,0}, {1496,1000,1000,1000} 
}; 

struct // f walk fast 
{ 
   const uint8 JointFlexibility[18]; 
   const uint8 NextPage; 
   const uint8 ExitPage; 
   const uint8 RepeatTime; 
   const uint8 SpeedRate10; 
   const uint8 InertialForce; 
   const uint8 Steps; 
   const uint32 StepValues[4][6]; 
   const uint16 PauseTime[4]; 
   const uint16 PlayTime[4]; 
} MotionPage10 PROGMEM = { 
{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5}, 
0, 0, 1, 15, 32, 4, 
{{1813152245,2815427141,2144336313,2270431733,2144479874,2505431844},
 {1813152245,2815283711,2144336353,2522089973,1850735234,2505431844},
 {1813152096,2190332415,2148532706,2442154366,1854931586,2505431844},
 {1813152096,2190475845,2148532646,2274382206,2148676226,2505431844}}, 
{0,0,0,0}, {208,104,216,104} 
}; 

struct // f walk normal 
{ 
   const uint8 JointFlexibility[18]; 
   const uint8 NextPage; 
   const uint8 ExitPage; 
   const uint8 RepeatTime; 
   const uint8 SpeedRate10; 
   const uint8 InertialForce; 
   const uint8 Steps; 
   const uint32 StepValues[4][6]; 
   const uint16 PauseTime[4]; 
   const uint16 PlayTime[4]; 
} MotionPage11 PROGMEM = { 
{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5}, 
0, 0, 1, 10, 32, 4, 
{{1813152245,2815427141,2144336313,2270431733,2144479874,2505431844},
 {1813152245,2815283711,2144336353,2522089973,1850735234,2505431844},
 {1813152096,2190332415,2148532706,2442154366,1854931586,2505431844},
 {1813152096,2190475845,2148532646,2274382206,2148676226,2505431844}}, 
{0,0,0,0}, {208,104,216,104} 
}; 

struct // f walk slow 
{ 
   const uint8 JointFlexibility[18]; 
   const uint8 NextPage; 
   const uint8 ExitPage; 
   const uint8 RepeatTime; 
   const uint8 SpeedRate10; 
   const uint8 InertialForce; 
   const uint8 Steps; 
   const uint32 StepValues[4][6]; 
   const uint16 PauseTime[4]; 
   const uint16 PlayTime[4]; 
} MotionPage12 PROGMEM = { 
{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5}, 
0, 0, 1, 4, 32, 4, 
{{1813152245,2815427141,2144336313,2144602613,2144479874,2505431844},
 {1813152245,2815283711,2144336403,2522089973,1850735234,2505431844},
 {1813152096,2190332415,2148532736,2442154366,1854931586,2505431844},
 {1813152096,2190475845,2148532646,2064667006,2148676226,2505431844}}, 
{0,0,0,0}, {216,216,216,216} 
}; 

struct // b walk fast 
{ 
   const uint8 JointFlexibility[18]; 
   const uint8 NextPage; 
   const uint8 ExitPage; 
   const uint8 RepeatTime; 
   const uint8 SpeedRate10; 
   const uint8 InertialForce; 
   const uint8 Steps; 
   const uint32 StepValues[4][6]; 
   const uint16 PauseTime[4]; 
   const uint16 PlayTime[4]; 
} MotionPage14 PROGMEM = { 
{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5}, 
0, 0, 1, 15, 32, 4, 
{{1813152096,2190475845,2148532646,2274382206,2148676226,2505431844},
 {1813152096,2190332415,2148532706,2442154366,1854931586,2505431844},
 {1813152245,2815283711,2144336353,2522089973,1850735234,2505431844},
 {1813152245,2815427141,2144336313,2270431733,2144479874,2505431844}}, 
{0,0,0,0}, {104,216,104,208} 
}; 

struct // b walk normal 
{ 
   const uint8 JointFlexibility[18]; 
   const uint8 NextPage; 
   const uint8 ExitPage; 
   const uint8 RepeatTime; 
   const uint8 SpeedRate10; 
   const uint8 InertialForce; 
   const uint8 Steps; 
   const uint32 StepValues[4][6]; 
   const uint16 PauseTime[4]; 
   const uint16 PlayTime[4]; 
} MotionPage15 PROGMEM = { 
{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5}, 
0, 0, 1, 10, 32, 4, 
{{1813152096,2190475845,2148532646,2274382206,2148676226,2505431844},
 {1813152096,2190332415,2148532706,2442154366,1854931586,2505431844},
 {1813152245,2815283711,2144336353,2522089973,1850735234,2505431844},
 {1813152245,2815427141,2144336313,2270431733,2144479874,2505431844}}, 
{0,0,0,0}, {104,216,104,208} 
}; 

struct // b walk slow 
{ 
   const uint8 JointFlexibility[18]; 
   const uint8 NextPage; 
   const uint8 ExitPage; 
   const uint8 RepeatTime; 
   const uint8 SpeedRate10; 
   const uint8 InertialForce; 
   const uint8 Steps; 
   const uint32 StepValues[4][6]; 
   const uint16 PauseTime[4]; 
   const uint16 PlayTime[4]; 
} MotionPage16 PROGMEM = { 
{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5}, 
0, 0, 1, 4, 32, 4, 
{{1813152096,2190475845,2148532646,2274382206,2148676226,2505431844},
 {1813152096,2190332415,2148532706,2442154366,1854931586,2505431844},
 {1813152245,2815283711,2144336353,2522089973,1850735234,2505431844},
 {1813152245,2815427141,2144336313,2270431733,2144479874,2505431844}}, 
{0,0,0,0}, {216,216,216,208} 
}; 

struct // right turn 
{ 
   const uint8 JointFlexibility[18]; 
   const uint8 NextPage; 
   const uint8 ExitPage; 
   const uint8 RepeatTime; 
   const uint8 SpeedRate10; 
   const uint8 InertialForce; 
   const uint8 Steps; 
   const uint32 StepValues[5][6]; 
   const uint16 PauseTime[5]; 
   const uint16 PlayTime[5]; 
} MotionPage18 PROGMEM = { 
{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5}, 
19, 0, 1, 15, 32, 5, 
{{1813152156,2148389376,2148532736,2148737436,1854931586,2505431844},
 {1813152105,2144193023,2144336383,2144336283,1850735234,2505431844},
 {1813152106,2148532736,2148532736,2148532636,2148532866,2505431844},
 {1813152106,2148532736,2148532666,2148532636,2148532866,2505431844},
 {1813152256,2148532736,2148532666,2148737436,2148532866,2505431844}}, 
{0,0,0,0,0}, {72,72,72,72,72} 
}; 

struct // right turn 
{ 
   const uint8 JointFlexibility[18]; 
   const uint8 NextPage; 
   const uint8 ExitPage; 
   const uint8 RepeatTime; 
   const uint8 SpeedRate10; 
   const uint8 InertialForce; 
   const uint8 Steps; 
   const uint32 StepValues[5][6]; 
   const uint16 PauseTime[5]; 
   const uint16 PlayTime[5]; 
} MotionPage19 PROGMEM = { 
{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5}, 
0, 0, 1, 15, 32, 5, 
{{1813152256,2148532736,2148532736,2442338716,2148532866,2505431844},
 {1813152256,2777678336,2148532736,2442338816,2148532866,2505431844},
 {1813152256,2777678336,2148532736,2148737536,2148532866,2505431844},
 {1813152256,2777678406,2148532736,2148737536,2148676226,2505431844},
 {1813152256,2567963206,2148532736,2148737436,2148676226,2505431844}}, 
{0,0,0,0,0}, {72,72,72,72,72} 
}; 

struct // left turn 
{ 
   const uint8 JointFlexibility[18]; 
   const uint8 NextPage; 
   const uint8 ExitPage; 
   const uint8 RepeatTime; 
   const uint8 SpeedRate10; 
   const uint8 InertialForce; 
   const uint8 Steps; 
   const uint32 StepValues[5][6]; 
   const uint16 PauseTime[5]; 
   const uint16 PlayTime[5]; 
} MotionPage21 PROGMEM = { 
{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5}, 
22, 0, 1, 15, 32, 5, 
{{1813152256,2567963206,2148532736,2148737436,2148676226,2505431844},
 {1813152256,2777678406,2148532736,2148737536,2148676226,2505431844},
 {1813152256,2777678336,2148532736,2148737536,2148532866,2505431844},
 {1813152256,2777678336,2148532736,2442338816,2148532866,2505431844},
 {1813152256,2148532736,2148532736,2442338716,2148532866,2505431844}}, 
{0,0,0,0,0}, {72,72,72,72,72} 
}; 

struct // left turn   
{ 
   const uint8 JointFlexibility[18]; 
   const uint8 NextPage; 
   const uint8 ExitPage; 
   const uint8 RepeatTime; 
   const uint8 SpeedRate10; 
   const uint8 InertialForce; 
   const uint8 Steps; 
   const uint32 StepValues[5][6]; 
   const uint16 PauseTime[5]; 
   const uint16 PlayTime[5]; 
} MotionPage22 PROGMEM = { 
{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5}, 
0, 0, 1, 15, 32, 5, 
{{1813152256,2148532736,2148532666,2148737436,2148532866,2505431844},
 {1813152106,2148532736,2148532666,2148532636,2148532866,2505431844},
 {1813152106,2148532736,2148532736,2148532636,2148532866,2505431844},
 {1813152106,2148389376,2148532736,2148532636,1854931586,2505431844},
 {1813152156,2148389376,2148532736,2148737436,1854931586,2505431844}}, 
{0,0,0,0,0}, {72,72,72,72,72} 
}; 

// Number of active motion pages in this file 
const uint8 ACTIVE_MOTION_PAGES = 17; 

// Min and max values for the servo values 
const uint16 SERVO_MAX_VALUES[18] = {552,592,512,671,512,711,512,512,531,702,641,512,532,692,642,871,797,827}; 
const uint16 SERVO_MIN_VALUES[18] = {431,472,352,511,312,511,511,511,321,492,511,382,331,492,512,487,504,512}; 

#endif /* MOTION_H_ */