/*file_handling.h*/
#ifndef	FILE_HANDLING_H
#define FILE_HANDLING_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <ncurses.h>

#include "command_handling.h" 
#include "entry.h"
#include "chained_table.h"
#include "utils.h"
#include "signal_handling.h"

int prev_dir(char **p_cwd);
int next_dir(char **p_cwd, char *dir_name);
int open_file(bool* exec_in_term, char *cwd, char *file_name, chained_table_str* ct);
char* get_extension(char *file_name);
int file_name_len(char* path);
int get_path_length(char* cwd, char* file_name);
int extract_file_name(char** buff, const char* path);
int append_to_path(char* new_path, const char* old_path, const char* filename);
int file_buff_cp(int del_after_copy, const char* dest, char** file_buffer, int* size);
int delete_selection(char** file_buffer, int* num_selected);

#endif
