/*
 * commands.h
 *
 *  Created on: 05.03.2009
 *      Author: chh
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "main.h"

// Commands
#define COMMAND_HELLO       "hello"
#define COMMAND_HELP        "help"
#define COMMAND_STATUS      "status"
#define COMMAND_TIME        "time"
#define COMMAND_INDY        "indy"
#define COMMAND_IMPERIAL    "imperial"

void command_eval(const char *command);

#endif /* COMMANDS_H_ */
