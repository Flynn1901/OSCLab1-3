/**
 * \author BL
 */

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

int write_to_log_process(char *msg);
int create_log_process();
int end_log_process();

#endif