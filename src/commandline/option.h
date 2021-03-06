#if !defined(_RSYS_OPTION_H_)
#define _RSYS_OPTION_H_

#include <vector>

namespace Executor
{
typedef enum option_kind {
    /* this option has no argument; it evaluates to `def' if provided */
    opt_no_arg,
    /* option is provided if next element in argv does not start with `-' */
    opt_optional,
    /* argument is part of argument, ie., -p4 */
    opt_sticky,
    /* argument is next value in argv */
    opt_sep,
    /* ignore this option */
    opt_ignore,
    /* ignore this option and its argument */
    opt_sep_ignore,
} option_kind_t;

typedef struct option
{
    /* text of the option.  does not include the `-' prefix */
    std::string text;

    /* description of the option */
    std::string desc;

    /* kind of argument */
    option_kind_t kind;

#if 0  
  /* NOTE: this was never implemented, so every time someone added an option
     to the database, there was potential to get totally confused.  The whole
     way this is set up is error prone and ugly, and the fact that we had a
     misleading unused field in here for so long is just a testament to
     these problems. */
  /* default value of argument */
  char *def;
#endif

    /* value of option if it is specified with no argument */
    std::string opt_val;
} option_t;

typedef std::vector<option_t> option_vec;

typedef enum priority {
    pri_appl_config_file,
    pri_command_line,
    pri_env_var,
    pri_x_resource,
    pri_dwrite,
    pri_default,
} priority_t;

typedef struct opt_val
{
    std::string text;

    /* temporary value (ie., those specified in by an application
     specific configuration file */
    std::string t_val;
    priority_t t_pri;

    std::string val;
    priority_t pri;
} opt_val_t;

typedef std::vector<opt_val_t> opt_database_t;

void opt_init(void);
void opt_shutdown(void);

void opt_register(std::string new_interface, option_vec opts);

/* provide a function to parse specified arguments */
opt_database_t opt_alloc_db(void);
bool opt_parse(opt_database_t &db, option_vec opts,
              int *argc, char *argv[]);

/* returns true if options was specified */
bool opt_int_val(opt_database_t &db, std::string opt, int *retval,
                bool *parse_error_p);
bool opt_bool_val(opt_database_t &db, std::string opt, bool *retval,
                bool *parse_error_p);

bool opt_val(opt_database_t &db, std::string opt, std::string *retval);
void opt_put_val(opt_database_t &db, const std::string &opt, std::string value,
                 priority_t pri, int temp_val_p);

void opt_put_int_val(opt_database_t &db, const std::string &opt, int value,
                     priority_t pri, int temp_val_p);

char *opt_help_message(void);
void opt_register_pre_note(std::string note);

extern uint32_t parse_drive_opt(const char *opt_name, const char *opt_value);
extern FILE *executor_dir_fopen(const char *file, const char *perm);
extern int executor_dir_remove(const char *file);
}
#endif /* !_RSYS_OPTION_H_ */
