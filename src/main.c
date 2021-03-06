#include <stdio.h>
#include <argp.h>
#include "interface_stats.h"


const char *argp_program_version = "myBM 1.0";
const char *argp_program_bug_address = "<bug-gnu-utils@gnu.org>";

/* program general documentation. appears in help. */
static char doc[] =
  "this program helps check interface data usage (txbytes per sec)";

/* A description of the arguments we accept. */
static char args_doc[] = "interface_name";

/* The options we understand. */
static struct argp_option options[] = {
  {"verbose",  'v', 0,      0,  "Produce verbose output" },
  {"quiet",    'q', 0,      0,  "Don't produce any output" },
  {"silent",   's', 0,      OPTION_ALIAS },
  {"output",   'o', "FILE", 0,
   "Output to FILE instead of standard output" },
  { 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments
{
  char *args[1];                /* arg1 & arg2 */
  int silent, verbose;
  char *output_file;
};


/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;
  switch (key)
    {
    case 'q': case 's':
      arguments->silent = 1;
      break;
    case 'v':
      arguments->verbose = 1;
      break;
    case 'o':
      arguments->output_file = arg;
      break;

    case ARGP_KEY_ARG:
      if (state->arg_num >=1)
        /* Too many arguments. */
        argp_usage (state);

      arguments->args[state->arg_num] = arg;

      break;

    case ARGP_KEY_END:
      if (state->arg_num < 1)
        /* Not enough arguments. */
        argp_usage (state);
        printf("erririririie");

      break;

    default:
        printf("asdadasda");
        return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc };


int main(int argc , char *argv[]){
    printf("Hi!\n");
    struct arguments arguments;
   
    /* Default values. */
    arguments.silent = 0;
    arguments.verbose = 0;
    arguments.output_file = "-";

    /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
    argp_parse (&argp, argc, argv, 0, 0, &arguments);
    printf ("ARG1 = %s\nOUTPUT_FILE = %s\n"
        "VERBOSE = %s\nSILENT = %s\n",
        arguments.args[0],
        arguments.output_file,
        arguments.verbose ? "yes" : "no",
        arguments.silent ? "yes" : "no"); 

    float data_usage = 0;
    run_calc_forever(arguments.args[0]);

    return 0;
}