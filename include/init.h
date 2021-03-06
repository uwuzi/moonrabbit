#ifndef INIT_H
#define INIT_H
#include <utils.h>
#include <ext_table.h>

int parse_cmd_args(int argc, char* argv[], char config_path[])
{
	bool user_cfg = 0;
	while (argc > 1) {
		if (argv[1][0] == '-') {
			if (argv[1][1] == 'c') {
				if (argc > 2) {
					user_cfg = 1;
					strcpy(config_path, argv[2]);
					argc--;
					argv++;
				}
				else {
					fprintf(stderr, "%s -c [/path/to/config]\n", argv[0]);
					exit(EXIT_FAILURE);
				}
			}
			else {
				fprintf(stderr, "Error: unrecognized option %s\n", argv[1]);
				exit(EXIT_FAILURE);
			}
			argc--;
			argv++;
		}
		else
			break;
	}
	if (!user_cfg) {
		char username[128] ;
		getlogin_r(username,128);
		strcpy(config_path, "/home/");
		strcat(config_path, username);
		strcat(config_path, "/.config/moonrabbit/config");
	}
	return 1;
}

// Check that file exists! Print error otherwise.
// Ignore: lines that are blank or commented with '#'
// 3 "Modes" during parsing -- reading programs, reading filetypes, and reading extensions
int parse_config_file(char* config_path, ext_table *ct)
{
	FILE *stream;
	char *line = NULL;
	size_t len = 0;
	size_t nread;
	bool reading_programs = 0;
	bool reading_file_types = 0;
	bool reading_extensions = 0;

	char current_program_path[PATH_MAX];

	stream = fopen(config_path,"r");
	if (!stream) {
		endwin();
		fprintf(stderr, "ERROR: Could not locate at default location $USER/.config/moonrabbit/config.\nPut one there or specify a full path with -c option.)\n");
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	ext_table_init(ct);

	while ((nread = getline(&line, &len, stream)) != -1) {
		if (line[0] == '#' || line[0] == '\n') {
		}
		else if (line[0] == '}') {
			reading_extensions = 0;
		}
		else if (line[0] == '[') {
			int k = 0;
			while (line[k] != ']') {
				k++;
			}
			k--;
			char section_title[k];
			section_title[k] = '\0';
			for (int i = 0; i < k; i++) {
				section_title[i] = line[i+1];
			}
			if (!strcmp(section_title,"Programs")) {
				reading_programs = 1;
				reading_file_types = 0;
				reading_extensions = 0;
			}
			else if (!strcmp(section_title,"File Types")) {
				reading_programs = 0;
				reading_file_types = 1;
				reading_extensions = 0;
			}
			else if (!strcmp(section_title,"End")) {
				reading_programs = 0;
				reading_file_types = 0;
				reading_extensions = 0;
			}
		}
		else {
			if (reading_programs) {

			}
			else if (reading_extensions) { // This HAS TO BE BEFORE *else if (reading_file_types)*
				for (int i = 0; i < strlen(line); i++) {
					if (line[i] == ' ' || line[i] == '\t') {
						continue;
					}
					else if (line[i] == '.') {
						int start = i;
						i++;
						while (line[i] != '.' && i < strlen(line)) {
							i++;
						}
						int end = i;
						int ext_len = end - start - 1;
						char ext[ext_len];
						ext[ext_len] = '\0';
						for (int j = 0; j < ext_len; j++) {
							ext[j] = line[start+j];
						}
						str_remove_outer_ws(ext);
						ext_table_ins_ext(ct, current_program_path, ext);
						i--;
					}
				}
			}
			else if (reading_file_types) {
				int exec_in_term = line[0] == '$' ? 1 : 0;

				int k = 0;
				while (line[k] != '{') {
					k++;
				}
				char program_path[k-exec_in_term];
				for (int i = 0; i < k-exec_in_term; i++) {
					program_path[i] = line[i+exec_in_term];
				}
				program_path[k-exec_in_term] = '\0';
				str_remove_outer_ws(program_path);
				ext_table_new_prog(ct, program_path);
				if (exec_in_term == 1) {
					ct->list[ct->size-1].exec_in_term = 1;
				}
				else {
					ct->list[ct->size-1].exec_in_term = 0;
				}
				reading_extensions = 1;
				strcpy(current_program_path, program_path);
			}
		}
	}
	free(line);
	fclose(stream);
	return 1;
}

#endif
