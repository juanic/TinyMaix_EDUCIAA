/*
 * Syscalls.h
 *
 *  Created on: 19 oct. 2018
 *      Author: pena
 */

#ifndef INC_SYSCALLS_H_
#define INC_SYSCALLS_H_

void *_sbrk(int incr);
int _kill(int pid, int sig);
int _getpid(void);
void _exit(int errCode);

#endif /* INC_SYSCALLS_H_ */
