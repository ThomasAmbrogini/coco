#pragma once

#ifdef DEBUG
#define assert(expression) if(!expression) { __asm volatile("bkpt #0"); };
#else
#define assert(expression) ;
#endif

