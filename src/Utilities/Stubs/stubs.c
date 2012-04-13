/*
 * stubs.c
 *
 *  Created on: Jul 10, 2011
 *      Author: mabl
 */

#include <stdio.h>

#include "ch.h"
#include "hal.h"

#include "stubs.h"

// FIXME: Stubs - HATE THEM
void _exit(int status){
	(void) status;
	chSysHalt();

	while(TRUE){}
}

pid_t _getpid(void){
	return 1;
}

void _kill(pid_t id){
	(void) id;
}

/* guard variables */
int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);}
void __cxa_guard_release (__guard *g) {*(char *)g = 1;}
void __cxa_guard_abort (__guard * g __attribute__((unused))) {}
void __cxa_end_cleanup (void) {}
void __cxa_pure_virtual(void) { while (1); }
