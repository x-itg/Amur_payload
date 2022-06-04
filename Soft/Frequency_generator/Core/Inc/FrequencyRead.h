#ifndef __FREQ_READ

#define __FREQ_READ

#include "string.h"
#include "stdio.h"


#define TIMCLOCK   80000000
#define PSCALAR    0
#define numval   500
#define MIN(a,b) ((a) < (b) ? (a) : (b))












//prototype functions
void frequencySequence(void);
void pwmStart(void);
void freqInit(void);


#endif
