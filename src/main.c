#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <getopt.h>

#include "vams_parse.h"
#include "vpp.h"
#include "xvpi.h"
#include "xvpi_elaborate.h"
int verbose=0;
int tree_print = 0;

int main (int argc, char **argv)
{
	const char *include_path_list [argc];
	int include_path_count = 0;
	int print_preprocessed_source_and_exit = 0;
	int debug = 0;
	int retval = EXIT_SUCCESS;
	int ch;
	int i;

	if (xvpi_init(argc, argv) == -1)
		return -1;

	while ((ch = getopt(argc, argv, "I:EtdhvV")) != -1) {
		switch (ch) {
		case 'I':
			include_path_list[include_path_count++] = optarg;
			break;
		case 'E':
			print_preprocessed_source_and_exit = 1;
			break;
		case 't':
			tree_print = 1;
			break;
		case 'd':
			debug = 1;
			break;
		case 'v':
                        verbose = 1;
			break;
		case 'V':
			vpi_printf("%s\n(checkout id and build date: %s)\n", xvpi.vlog_info.product, xvpi.vlog_info.version);
			return EXIT_SUCCESS;
		case 'h':
		case '?':
		default:
			vpi_printf("usage: %s [-v|t|d|V|h] <input file names>\n", argv[0]);
			return EXIT_FAILURE;
		}
	}

	include_path_list[include_path_count] = NULL;
	argc -= optind;
	argv += optind;

	if (argc < 1) {
		err("no input files!\n");
		return EXIT_FAILURE;
	}

	for (i=0; i<argc; i++) {
		const char *fname = argv[i];
		const char *fname_suffix = strrchr(fname, '.');
		if (fname_suffix != NULL && (strcasecmp(fname_suffix, ".vpi") == 0)) {
			if (xvpi_application_load(fname) == NULL) {
				retval = EXIT_FAILURE;
				break;
			}
		} else {
			char *buf;
			int len = vpp_preprocess_to_parsebuffer(&buf, fname, include_path_list);
			if (print_preprocessed_source_and_exit) {
				fwrite(buf, len, 1, stdout);
				free(buf);
				retval = EXIT_FAILURE;
				break;
			}
			if (vams_parse(buf, len, debug, tree_print) != 1) {
				free(buf);
				retval = EXIT_FAILURE;
				break;
			}
			free(buf);
		}
	}

	if (!print_preprocessed_source_and_exit) {
		xvpi_elaborate();
		xvpi_trigger_callbacks(NULL, cbEndOfCompile);
		xvpi_shutdown();
	}

	return retval;
}



