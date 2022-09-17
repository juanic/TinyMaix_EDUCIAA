/*
 * Syscalls.c
 *
 *  Created on: 19 oct. 2018
 *      Author: pena
 */

#include "Syscalls.h"
#include <errno.h>


# undef errno

extern int errno;
extern char *__bot_heap;
extern char *__top_heap;
char * __env [1] = { 0 };
char ** environ = __env ;

void *_sbrk(int incr)
{
	char * prev_heap_end;
	static char * lastHeapAssigned = (char *) &__bot_heap;
	prev_heap_end = lastHeapAssigned;
	lastHeapAssigned += incr;
	if (lastHeapAssigned >= (char*) (& __top_heap))
	{
		lastHeapAssigned = prev_heap_end;
		errno = ENOMEM;
		abort();
	}
	return ( void *) prev_heap_end;
}

int _kill(int pid, int sig)
{
	errno = EINVAL ;
	return -1;
}

int _getpid(void)
{
	return 1;
}

void _exit(int errCode)
{
	//Chip_RGU_TriggerReset (RGU_CORE_RST);
}

int _execve (char *name , char ** argv , char ** env)
{
	errno = ENOMEM ;
	return -1;
}

int _fork (void)
{
	errno = EAGAIN ;
	return -1;
}

int _wait ( int * status ){
	errno = ECHILD ;
	return -1;
}
/*
int _fstat ( int file , struct stat *st){
	st -> st_mode = S_IFCHR ;
	return 0;
}

int _stat ( char * file , struct stat *st){
st -> st_mode = S_IFCHR ;
return 0;
}

int _isatty ( int file ){
	return 1;
}

int _link ( char *old , char * new ){
	errno = EMLINK ;
	return -1;
}

int _unlink ( char * name ){
	errno = ENOENT ;
	return -1;
}

int _open ( const char *name , int flags , int mode ){
	return -1;
}

int _lseek ( int file , int offset , int whence ){
	return 0;
}

int _read ( int file , char *ptr , int len )
{
	uint8_t newLine [2] = {'\r', '\n'};
	int readed = 0;
	bool endLine = false ;
	if ( file == STDIN_FILENO ){
		while (( readed < len ) && (! endLine )){
			Chip_UART_ReadBlocking ( LPC_USART2 , ptr + readed , 1);
			endLine = (*( ptr + readed ) == '\n') || (*( ptr + readed ) == '\r');
			if( endLine )
				Chip_UART_SendBlocking ( LPC_USART2 , newLine , 2);
			else
				Chip_UART_SendBlocking ( LPC_USART2 , ptr + readed , 1);
			readed ++;
		}
		len = readed ;
	}
		else
		len = 0;
		return len;
}


int _write (int file , char *ptr , int len)
{
	if (( file == STDOUT_FILENO ) || ( file == STDERR_FILENO ))
		Chip_UART_SendBlocking ( LPC_USART2 , ptr , len );
	else
		len = 0;
	return len ;
}
*/
int _close (int file )
{
	return -1;
}

int _times ( struct tms * buf )
{
	return -1;
}

int _gettimeofday ( struct timeval *tv , struct timezone *tz)
{
	return -1;
}
