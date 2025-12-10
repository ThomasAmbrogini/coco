#pragma once

#define assert(expression) if(!expression) { __asm volatile("bkpt #0"); };

