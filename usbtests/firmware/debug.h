/*
 * debug.h
 *
 *  Created on: 2010-04-24
 *      Author: kharg
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdio.h>

#if DEBUG == 1

#define PRINTF(f, ...)		{ printf(f, ##__VA_ARGS__); }

#else

#define PRINTF(f, ...)		{ }

#endif /* DEBUG */
#endif /* DEBUG_H_ */
