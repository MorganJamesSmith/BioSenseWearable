#include "cli.h"

#include <string.h>

#define CLI_IN_BUFFER_LEN 256

/** String to clear the screen of a VT100 and bring the cursor home. */
static const char *cli_clear_str = "\x1B[2J\x1B[H";

static const char *cli_unknown_str_0 = "Unknown command \"";
static const char *cli_unknown_str_1 = "\"\n";

static void cli_init_callback(void *context)
{
    struct cli_desc_t *cli = (struct cli_desc_t*)context;

    cli->console->write_string(cli_clear_str);
    cli->console->write_string(cli->prompt);
}

void init_cli(struct cli_desc_t *cli, const struct cli_io_funcs_t *console,
              const char *prompt, const struct cli_func_desc_t *const functions,
              char delim)
{
    cli->prompt = prompt;
    cli->console = console;
    cli->functions = functions;
    cli->delim = delim;

    cli->console->set_ready_callback(&cli_init_callback, (void*)cli);
}

static void cli_help(uint8_t argc, char **argv, struct cli_desc_t *cli)
{
    if (argc > 2) {
        cli->console->write_string("Use \"help\" to list all commands or "
                                   "\"help <command>\" to get information on a "
                                   "specific command.\n");
        return;
    } else if (argc == 2) {
        // Get help for a specific command
        for (const struct cli_func_desc_t *cmd = cli->functions;
             cmd->func != NULL; cmd++) {
            if (!strcasecmp(argv[1], cmd->name)) {
                cli->console->write_string(cmd->help_string);
                cli->console->write_string("\n");
                return;
            }
        }

        cli->console->write_string(cli_unknown_str_0);
        cli->console->write_string(argv[1]);
        cli->console->write_string(cli_unknown_str_1);
    } else {
        cli->console->write_string("Use \"help <command>\" to get information "
                                   "on a specific command.\n");
    }

    cli->console->write_string("\nAvailable Commands:\n");
    for (const struct cli_func_desc_t *cmd = cli->functions;
         cmd->func != NULL; cmd++) {
        cli->console->write_string(cmd->name);
        cli->console->write_string("\n");
    }
}

static void cli_line_callback(struct cli_desc_t *cli, char *line)
{
    /* Count the number of space separated tokens in the line */
    uint8_t num_args = 1;
    for (uint16_t i = 0; i < strlen(line); i++) {
        num_args += (line[i] == ' ');
    }

    /* Create a an array of pointers to each token */
    // Array is one larger to leave room for NULL pointer
    char *args[num_args + 1];
    for (uint16_t i = 0; (args[i] = strsep(&line, " ")) != NULL; i++);

    /* Call the specified function */
    uint8_t command_found = 0;
    if (args[0] == NULL) {
        // Empty line
        command_found = 1;
    } else if (!strcasecmp(args[0], "help")) {
        // Print the help string for a function
        cli_help(num_args, args, cli);
        command_found = 1;
    } else if (!strcasecmp(args[0], "clear")) {
        // Clear the screen
        cli->console->write_string(cli_clear_str);
        command_found = 1;
    } else {
        // Search function list
        for (const struct cli_func_desc_t *cmd = cli->functions;
             cmd->func != NULL; cmd++) {
            if (!strcasecmp(args[0], cmd->name)) {
                cmd->func(num_args, args, cli->console);

                command_found = 1;
                break;
            }
        }
    }

    /* If the command was not found and is not empty, print an angry message. */
    if (!command_found) {
        cli->console->write_string(cli_unknown_str_0);
        cli->console->write_string(args[0]);
        cli->console->write_string(cli_unknown_str_1);
    }

    /* Print the prompt. */
    cli->console->write_string(cli->prompt);
    return;
}

void cli_service(struct cli_desc_t *cli)
{
    char cli_in_buffer[CLI_IN_BUFFER_LEN];

    if ((cli->delim != '\0') && cli->console->has_delim(cli->delim)) {
        cli->console->read_line_delim(cli->delim, cli_in_buffer,
                                      CLI_IN_BUFFER_LEN);
        cli_line_callback(cli, cli_in_buffer);
    } else if ((cli->delim == '\0') && cli->console->has_line()) {
        cli->console->read_line(cli_in_buffer, CLI_IN_BUFFER_LEN);
        cli_line_callback(cli, cli_in_buffer);
    }
}
