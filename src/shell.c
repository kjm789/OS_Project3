#include "../include/shell.h"

#define MAX_BUFFER_RD_SIZE 1024
#define MAX_CMD_COUNT 50
#define MAX_CMD_LEN 25


// private function prototypes
int dirwalk(char *dir);

// Run the command, given the command struct of which holds the parsed commands from the user
void run_command(Commands_t* cmd){

	if(strncmp(cmd->cmds[0],"cd",strlen("cd") + 1) == 0){ // Error check, then execute change directory
		if(cmd->num_cmds > 2){
			fprintf(stdout, "cd(): Too many arguments");
		}
		else if(cmd->num_cmds < 2){
			fprintf(stdout, "cd(): Not enough arguments");
		}
		else{
			cd(cmd->cmds[1]);
		}
	}
	else if(strncmp(cmd->cmds[0],"pwd",strlen("pwd") + 1) == 0){ // Error check, then execute print working directory
		if(cmd->num_cmds > 1){
			printf("pwd(): Too many arguments");
		}
		else {
			pwd();
		}
	}

	else if(strncmp(cmd->cmds[0],"ls",strlen("ls") + 1) == 0){ // Error check, then execute list directory/file
		if(cmd->num_cmds > 2){
			fprintf(stdout, "ls(): Too many arguments");
		}
		if(ls(cmd->cmds[1]) < 0){
			fprintf(stdout, "ls(): Error with ls command");
		}
	}

	else if(strncmp(cmd->cmds[0],"cat",strlen("cat") + 1) == 0){ // Error check, then execute cat, or print contents of file
		if(cmd->num_cmds > 2){
			fprintf(stdout, "cat(): Too many arguments");
		}
		if(cat(cmd->cmds[1]) < 0){
			fprintf(stdout, "cat(): Error with cat command");
		}
	}
	else if(strncmp(cmd->cmds[0],"join",strlen("join") + 1) == 0){
		if(cmd->num_cmds != 5){
			fprintf(stdout, "join(): Insufficient arguments => join <file1> <key> <file2> key");
		}
		else if(cmd->num_cmds == 5){
			fprintf(stdout, "Executing Join(SOON)");
		}
		else{
			fprintf(stdout, "join(): Error Occurred join()");
		}
	}
	else{
		printf("Not a command in this shell");
	}
}

// To print the working directory of shell
void pwd(){

	char *cwd;
	char buff[PATH_MAX + 1];
	cwd = getcwd(buff, PATH_MAX + 1 ); // Get the working directory from the system 
	if(cwd != NULL){ // Error check that we were returned a working directory
		fprintf(stdout, "The current working directory is \"%s\"", cwd);
		return;
	}
	else{// Fuck
		fprintf(stdout, "Could not find working directory");
	}
}

// To change directory to incoming parameters
void cd(char *file ){
	if(file) {
		struct stat fileStat;
		if(stat(file, &fileStat) < 0){ // Error check that it is actually a file
			fprintf(stdout,"file \"%s\" does not exist", file);
			return;
		}
		else if(!S_ISDIR(fileStat.st_mode)){ // Error check that is an actually directory we are working with
			fprintf(stdout, "file \"%s\" is not a directory", file);
			return;
		}
		else{
			if(chdir(file)< 0){ // If chdir() returns with a non-zero, we have a problem
				fprintf(stdout, "Error opening directory \"%s\"", file);
				return;
			}
			else{// Success
				fprintf(stdout, "changed into directory \"%s\"", file);
				return;
			}
		}
	}
	fprintf(stdout, "Bad file \"%s\"", file);
}

// List content of working directory if no files/directory
int ls(char *file) {
    if (!file) {
        char *cwd;
        char buff[PATH_MAX + 1];
        cwd = getcwd(buff, PATH_MAX + 1);
        if (dirwalk(cwd) < 0) {
            return -1;
        }
    } else {

        struct stat fileStat;
        if (stat(file, &fileStat) < 0)
            return -1;
        if (!S_ISDIR(fileStat.st_mode)) {
            fprintf(stdout, "%s\n", file);
            return 0;
        } else {
            if (dirwalk(file) < 0) {
                return -1;
            }
        }
    }
    return 0;
}

// to write content of a file to stdout
int cat(char *file) {
    if (!file) { // Error check param
        return -1;
    }
    int fd = open(file, O_RDONLY);
    if (fd < 0) { // Error check file descriptor
        return -1;
    }
    // read contet of file, then write
    char buffer[MAX_BUFFER_RD_SIZE];
    size_t bufferSize = 0;
    bufferSize = read(fd, buffer, MAX_BUFFER_RD_SIZE);
    while (bufferSize >= 1) {
        write(2, buffer, bufferSize);
        bufferSize = read(fd, buffer, MAX_BUFFER_RD_SIZE);
    }
    close(fd);
    return 0;
}

/*
    PRIVATE FUNCTIONS
*/
int dirwalk(char *dir) {
    if (!dir) {
        return -1;
    }

    struct dirent *dp;
    DIR *dfd;

    if ((dfd = opendir(dir)) == NULL) {
        fprintf(stdout, "ls: can't open %s\n", dir);
        return -1;
    }
    while ((dp = readdir(dfd)) != NULL) {
        fprintf(stdout, "%s\n", dp->d_name);
    }
    closedir(dfd);
    return 0;
}

// Parse the user input and store into command struct
bool parse_user_input (const char* input, Commands_t** cmd) {
	
	if(!input || !cmd)
		return false;

	char *string = strdup(input);
	
	*cmd = calloc (1,sizeof(Commands_t));
	(*cmd)->cmds = calloc(MAX_CMD_COUNT,sizeof(char*));

	unsigned int i = 0;
	char *token;
	token = strtok(string, " \n");
	for (; token != NULL && i < MAX_CMD_COUNT; ++i) {
		(*cmd)->cmds[i] = calloc(MAX_CMD_LEN,sizeof(char));
		if (!(*cmd)->cmds[i]) {
			perror("Allocation Error\n");
			return false;
		}	
		strncpy((*cmd)->cmds[i],token, strlen(token) + 1);
		(*cmd)->num_cmds++;
		token = strtok(NULL, " \n");
	}
	free(string);
	return true;
}

// Takes command struct and deconstructs
void destroy_commands(Commands_t** cmd) {

	if(!cmd){
		return;
	}
	
	for (int i = 0; i < (*cmd)->num_cmds; ++i) {
		free((*cmd)->cmds[i]);
	}
	free((*cmd)->cmds);
	free((*cmd));
	*cmd = NULL;
}


