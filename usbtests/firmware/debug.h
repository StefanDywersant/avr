/**
 *
 *
 * @author Karol Maciaszek <karol.maciaszek@gmail.com>
 * $Id$
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdio.h>

#if DEBUG == 1

#define PRINTF(f, ...)		{ printf(f, ##__VA_ARGS__); }

void debug_init(void);

#else

#define PRINTF(f, ...)		{ }

#endif /* DEBUG */
#endif /* DEBUG_H_ */
