/*
 * commands.h
 *
 *  Created on: 05.03.2009
 *      Author: chh
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "main.h"

#define COMMAND_PHASE_COOLING	"phase(cooling);"
#define COMMAND_PHASE_WARMING	"phase(warming);"

void command_eval(const char *command);

#endif /* COMMANDS_H_ */
