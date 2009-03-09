/*
 * commands.h
 *
 *  Created on: 05.03.2009
 *      Author: chh
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "main.h"

#define COMMAND_HELP		"help"
#define COMMAND_TIME		"time"
#define COMMAND_COOLING		"cooling"
#define COMMAND_WARMING		"warming"

void command_eval(const char *command);

#endif /* COMMANDS_H_ */
