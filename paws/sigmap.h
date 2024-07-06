
// Prevent the header being included multiple times.
#ifndef SIGMAP_H
#define SIGMAP_H

// Associates the signal number with its string name.
struct sigmap_element {
	int number;
	char *name;
};

extern const struct sigmap_element sigmap[];

const struct sigmap_element *find_sigmap_element(int signal);

#endif // SIGMAP_H
