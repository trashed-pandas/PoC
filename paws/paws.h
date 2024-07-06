
/*
 * CC = /usr/bin/clang
 * CFLAGS = -pedantic -std=c17 -Wall -Wextra -O0 -g
 * 
 * POSIX.1-2017
 * SUSv4
 */

// man 7 feature_test_macros
// man 7 standards
#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700

#include <errno.h>
#include <error.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>

