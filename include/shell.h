#ifndef SHELL_H_
#define SHELL_H_

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdbool.h>

#include <dyn_array.h>

// Struct to hold parsed user commands
typedef struct {
	unsigned int num_cmds;
	char** cmds;
}Commands_t;

///
/// Print working directory of system/shell
///	 error checks the system call else returns control to caller
/// \param none
/// \return none
///
void pwd();
 
///
/// Change directory given a passed in file
///  error checks passed in file, assuring it does exist and it is dir
/// \param none
///
void cd(char *file);

///
/// To run commands based on the parsed commands stored in the cmd struct
/// \param cmd command struct with parsed commands from user
/// \return none
///
void run_commands(Commands_t* cmd);

///
/// Parses incoming commands made by the user
/// \param cmd command struct that holds all entered commands by user
/// \return true for success of parsing, false for failure to parse command
///
bool parse_user_input (const char* input, Commands_t** cmd);

///
/// Destorys the command struct, which inludes all entered by user
/// \param cmd pointer to the allocated command struct of entered commands by user
/// \return none
///
void destroy_commands(Commands_t** cmd);

///
/// Runs ls with an optional parameter
/// \param file c-string of the file/directory/whatever you want to ls (NULL for cwd)
/// \return error code, <0 on failure, 0 on success
///
int ls(char* file);

///
/// prints contents of the secified file to stdout
/// \param file c-string of the file to print
/// \return error code, < 0 on failure, 0 on success
///
int cat(char* file);

#endif

