#ifndef _LOWPOWERMODE_H_
#define _LOWPOWERMODE_H_

#include "main.h"
// Transitions the system into stop mode to conserve energy
void Entry_StopMode(void);
// Resumes system operations from stop mode
void Exit_StopMode(void);
// Transitions the system into standby mode to conserve energy
void Entry_StandbyMode(void);
// Resumes system operations from standby mode
void Exit_StandbyMode(void);

#endif /*_LOWPOWERMODE_H_*/