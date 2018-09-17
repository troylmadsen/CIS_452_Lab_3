#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_BABIES 2

/*
 * Spawns children and waits for signals from them.
 * @Version 1.0
 * @Author Troy Madsen
 * @Author Tim DeVries
 * @Date: 2018/09/10
 */

static void parentSigHandler( int signum, siginfo_t *siginfo, void *context ) {
	if ( signum == SIGUSR1 ) {
		printf( "Received a SIGUSER1 signal from child %ld.\n", (long)siginfo->si_pid );
		fflush( stdout );
	}
	else if ( signum == SIGUSR2 ) {
		printf( "Received a SIGUSER2 signal from child %ld.\n", (long)siginfo->si_pid );
		fflush( stdout );
	}
	else {
		printf( "\nClosing \"gracefully\" *stab stab*\n" );
		exit( 0 );
	}
}

static void childSigHandler( int signum, siginfo_t *siginfo, void *context ) {
	printf( "\nChild %d terminated.\n", getpid() );
	exit( 0 );
}

int main() {
	/* Parent PID for sending signals */
	pid_t parent = getpid();

	struct sigaction sa;

	sigemptyset( &sa.sa_mask );
	sa.sa_flags = SA_SIGINFO;

	/* Make babies */
	pid_t pid;
	pid_t children[NUM_BABIES];
	for ( int i = 0; i < NUM_BABIES; i++ ) {
		pid = fork();

		/* Children do not make children */
		if ( pid == 0 ) {
			break;
		}

		/* Display child PID */
		printf( "Made baby #%d\n", pid );

		children[i] = pid;
	}

	if ( pid == 0 ) {
		/* Install signal handlers */
		sa.sa_sigaction = &childSigHandler;

		if ( sigaction( SIGINT, &sa, NULL ) == -1 ) {
			perror( "sigaction failure" );
			exit( 1 );
		}

		/* Wait for time send signal to parent */
		unsigned int my_pid = (int)getpid();
		while ( true ) {
			/* Sleep for 1 to 5 seconds */
			int time = ( rand_r( &my_pid ) % 5 ) + 1;
			sleep( time );

			/* Send parent SIGUSR1 or SIGUSR2 */
			kill( parent, ( ( rand_r( &my_pid ) % 2 ) + 5 ) * 2 );
		}
	}
	else {
		/* Install signal handlers */
		sa.sa_sigaction = &parentSigHandler;

		if ( sigaction( SIGINT, &sa, NULL ) == -1 ) {
			perror( "sigaction failure" );
			
			/* Terminate children */
			for ( int i = 0; i < NUM_BABIES; i++ ) {
				kill( children[i], SIGINT );
			}

			exit( 1 );
		}

		if ( sigaction( SIGUSR1, &sa, NULL ) == -1 ) {
			perror( "sigaction failure" );
			
			/* Terminating children */
			for ( int i = 0; i < NUM_BABIES; i++ ) {
				kill( children[i], SIGINT );
			}

			exit( 1 );
		}

		if ( sigaction( SIGUSR2, &sa, NULL ) == -1 ) {
			perror( "sigaction failure" );
			
			/* Terminate children */
			for ( int i = 0; i < NUM_BABIES; i++ ) {
				kill( children[i], SIGINT );
			}

			exit( 1 );
		}

		/* Wait for signal */
		while ( true ) {
			printf( "Waiting...\t" );
			fflush( stdout );
			pause();
		}
	}

	return 0;
}
