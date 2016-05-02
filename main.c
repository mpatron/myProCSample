#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include "hello.h"
#include "helloworld.h"
#include "logger.h"

/* The name of this program. */
const char* program_name;

void print_usage (FILE* stream, int exit_code) {
  fprintf (stream, "Usage:  %s options [ inputfile ... ]\n", program_name);
  fprintf (stream,
           "  -h  --help             Display this usage information.\n"
           "  -o  --output filename  Write output to file.\n"
           "  -v  --verbose          Print verbose messages.\n");
  exit (exit_code);
}

static void la_fin_du_monde(int signal)  {
	LOG_PRINT("Fin du programme non prévu : %d", signal);
}

int main(int argc, char **argv) {
	char username[50];
	char password[50];
	char dbname[50];
	bool verbose = false;
	int next_option;
	
	/* Remember the name of the program, to incorporate in messages.
     The name is stored in argv[0].  */
	program_name = argv[0];
	
	LOG_PRINT("Le programme %s est demarré.", program_name);
	
   /* On trappe les signaux suivants : Interruption batch */
   signal(SIGINT  , la_fin_du_monde);
   signal(SIGQUIT , la_fin_du_monde);
   signal(SIGSEGV , la_fin_du_monde);
   signal(SIGTERM , la_fin_du_monde);
   signal(SIGPWR  , la_fin_du_monde);	
	
	memset(username,0,sizeof(username));
	memset(password,0,sizeof(password));
	memset(dbname,0,sizeof(dbname));

	static struct option long_options[] = {
		{"user", required_argument, NULL, 'u'},
		{"password", required_argument, NULL, 'p'},
		{"dbname", required_argument,NULL, 'd'},
		{"verbose", no_argument, NULL, 'v'},
		{"help", no_argument, NULL, 'h'},
		{0, 0, NULL, 0}
	};
	do {		
		/* getopt_long stores the option index here. */
		int option_index = 0;
		next_option = getopt_long (argc, argv, "u:p:d:vh", long_options, &option_index);

		switch (next_option) {
			case 'h':   /* -h or --help */
			/* User has requested usage information.  Print it to standard
			 output, and exit with exit code zero (normal termination).  */
			print_usage (stdout, 0);
			case 'u':
				strcpy(username,optarg);
				break;
			case 'p':
				strcpy(password,optarg);
				break;
			case 'd':
				strcpy(dbname,optarg);
				break;
			case 'v':
				Hello();
				verbose = true;
				break;
			case '?':
				/* The user specified an invalid option.  */
				/* Print usage information to standard error, and exit with exit
				code one (indicating abnormal termination).  */
				print_usage (stderr, EXIT_FAILURE);
				break;
			case -1:
				break;
			default:
				abort ();
		}
	} while (next_option != -1);

	/* Done with options.  OPTIND points to first non-option argument.
     For demonstration purposes, print them if the verbose option was
     specified.  */
	if (verbose) {
		int i;
		for (i = optind; i < argc; ++i) {
		  printf ("Argument: %s\n", argv[i]);
		}
	}

	/*Regles d'usage*/
	/*Si pas d'argument, affichage de l'usage.*/
	if(argc==1) {
		print_usage (stderr, EXIT_FAILURE);
	}
	/*Si -u -p et -d sont renseignés alors action sinon affichage de l'usage.*/
	if(username[0]!=0 && password[0]!=0) {	
		myoracle(username,password,dbname);
	} else {
		print_usage (stderr, EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}
