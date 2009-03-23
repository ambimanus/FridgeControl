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
#define COMMAND_HELLO		"hello"
#define COMMAND_HELP		"help"
#define COMMAND_STATUS		"status"
#define COMMAND_TIME		"time"
#define COMMAND_INDY		"indy"
#define COMMAND_IMPERIAL	"imperial"
#define COMMAND_COOLING		"cooling"
#define COMMAND_WARMING		"warming"
#define COMMAND_SET_T_MIN	"set t_min"
#define COMMAND_SET_T_MAX	"set t_max"
#define COMMAND_DSC			"dsc"
#define COMMAND_TLR			"tlr"

// Parameters
#define PARAM_DSC_LOAD		"load"
#define PARAM_DSC_UNLOAD	"unload"

void command_eval(const char *command);

#endif /* COMMANDS_H_ */
