

#include "paws.h"
#include "sigmap.h"

// A collection of all the signal number to name pairs. The order of entries is not important.
// This is setup as a vector so you can loop through until: !sigmap[i]->number
const struct sigmap_element sigmap[] = {
	{SIGHUP,    "SIGHUP"},
	{SIGINT,    "SIGINT"},
	{SIGQUIT,   "SIGQUIT"},
	{SIGILL,    "SIGILL"},
	{SIGTRAP,   "SIGTRAP"},
	{SIGABRT,   "SIGABRT"},
#ifdef SIGIOT
	{SIGIOT,    "SIGIOT"},
#endif
	{SIGBUS,    "SIGBUS"},
	{SIGFPE,    "SIGFPE"},
	{SIGKILL,   "SIGKILL"},
	{SIGUSR1,   "SIGUSR1"},
	{SIGSEGV,   "SIGSEGV"},
	{SIGUSR2,   "SIGUSR2"},
	{SIGPIPE,   "SIGPIPE"},
	{SIGALRM,   "SIGALRM"},
	{SIGTERM,   "SIGTERM"},
#ifdef SIGSTKFLT
	{SIGSTKFLT, "SIGSTKFLT"},
#endif
#ifdef SIGCLD
	{SIGCLD,    "SIGCLD"},
#endif
	{SIGCHLD,   "SIGCHLD"},
	{SIGCONT,   "SIGCONT"},
	{SIGSTOP,   "SIGSTOP"},
	{SIGTSTP,   "SIGTSTP"},
	{SIGTTIN,   "SIGTTIN"},
	{SIGTTOU,   "SIGTTOU"},
	{SIGURG,    "SIGURG"},
	{SIGXCPU,   "SIGXCPU"},
	{SIGXFSZ,   "SIGXFSZ"},
	{SIGVTALRM, "SIGVTALRM"},
	{SIGPROF,   "SIGPROF"},
#ifdef SIGWINCH
	{SIGWINCH,  "SIGWINCH"},
#endif
#ifdef SIGPOLL
	{SIGPOLL,   "SIGPOLL"},
#endif
#ifdef SIGIO
	{SIGIO,     "SIGIO"},
#endif
#ifdef SIGPWR
	{SIGPWR,    "SIGPWR"},
#endif
	{SIGSYS,    "SIGSYS"},
#ifdef SIGUNUSED
	{SIGUNUSED, "SIGUNUSED"},
#endif
	{0, NULL}
};



/***************************************************************************************************************************************************************
 *
 * const struct sigmap_element *find_sigmap_element(int signal);
 *
 *  Inputs: The number of the signal you want more information on.
 *  Outputs: A pointer to the matching sigmap_element.
 *  Purpose: Find and return the matching sigmap_element from the sigmap data structure.
 *  Strategy: 
 *  	- Loop through the sigmap data structure looking for a match 
 *
 ***************************************************************************************************************************************************************/
const struct sigmap_element *find_sigmap_element(int signal){

	int i = 0;

	// - Loop through the sigmap data structure looking for a match 
	while(sigmap[i].number){
		if(signal == sigmap[i].number){
			break;
		}
		i++;
	}

	return(&sigmap[i]);
}
