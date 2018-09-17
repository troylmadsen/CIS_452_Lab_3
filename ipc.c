#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * Spawns a child and waits for signals from it.
 * @Version 1.0
 * @Author Troy Madsen
 * @Author Tim DeVries
 * @Date: 2018/09/13
 */

void parentSigHandler( int signum ) {
	if ( signum == SIGUSR1 ) {
		printf( "Received a SIGUSER1 signal.\n" );
		fflush( stdout );
	}
	else if ( signum == SIGUSR2 ) {
		printf( "Received a SIGUSER2 signal.\n" );
		fflush( stdout );
	}
	else {
		printf( "\nClosing \"gracefully\" *stab stab*\n" );
		exit( 0 );
	}
}

void childSigHandler( int signum ) {
	printf( "\nChild %d terminated.\n", getpid() );
	exit( 0 );
}

int main() {
	/* Parent PID for sending signals */
	pid_t parent = getpid();

	/* Make baby */
	int pid = fork();

	if ( pid == 0 ) {
		signal( SIGINT, childSigHandler );

		/* Wait for time send signal to parent */
		while ( true ) {
			/* Sleep for 1 to 5 seconds */
			int time = ( rand() % 5 ) + 1;
			sleep( time );

			/* Send parent SIGUSR1 or SIGUSR2 */
			kill( parent, ( ( rand() % 2 ) + 5 ) * 2 );
		}
	}
	else {
		/* Install signal handlers */
		signal( SIGINT, parentSigHandler );
		signal( SIGUSR1, parentSigHandler );
		signal( SIGUSR2, parentSigHandler );

		/* Display child PID */
		printf( "Made baby #%d\n", pid );

		/* Wait for signal */
		while ( true ) {
			printf( "Waiting...\t" );
			fflush( stdout );
			pause();
		}
	}

	return 0;
}
