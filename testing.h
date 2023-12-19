#ifndef TESTING_H
# define TESTING_H
# include <unistd.h>
pid_t	run_command(char **command, int input, int output, int *status);
#endif
