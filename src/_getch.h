#ifdef __linux__ 

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

static struct termios told, tnew;

/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
	tcgetattr(0, &told); /* grab old terminal i/o settings */
	tnew = told; /* make new settings same as old settings */
	tnew.c_lflag &= ~ICANON; /* disable buffered i/o */
	tnew.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
	tcsetattr(0, TCSANOW, &tnew); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
	tcsetattr(0, TCSANOW, &told);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}

/* Read 1 character without echo */
char _getch(void) 
{
	return getch_(0);
}

#endif
