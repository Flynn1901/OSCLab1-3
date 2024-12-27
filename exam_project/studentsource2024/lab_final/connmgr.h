//
// Created by flynn on 24-12-24.
//

#ifndef CONNMGR_H
#define CONNMGR_H\

#include <stdio.h>
#include <stdlib.h>
#include "lib/tcpsock.h"
#include <pthread.h>
#include "config.h"
#include <inttypes.h>
#include "sbuffer.h"
#include <time.h>

void *run_connmgr(void*);
void *read_thread(void*);
void delay(int a);
#endif //CONNMGR_H
