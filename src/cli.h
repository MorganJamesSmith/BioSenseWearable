#ifndef cli_h
#define cli_h

#include <stdint.h>

/**
 *  Set of IO functions for a CLI
 */
struct cli_io_funcs_t {
    void (*set_ready_callback)(void (*callback)(void*), void *context);
    uint16_t (*write_string)(const char *);
    void (*write_string_blocking)(const char *);
    uint16_t (*write_bytes)(const uint8_t *, uint16_t);
    void (*write_bytes_blocking)(const uint8_t *, uint16_t);
    void (*read_string)(char *, uint16_t);
    uint8_t (*has_delim)(char);
    void (*read_line_delim)(char, char *, uint16_t);
    uint8_t (*has_line)(void);
    void (*read_line)(char *, uint16_t);
};

/**
 *  Descriptor for a function which can be called via a CLI.
 */
struct cli_func_desc_t {
    void (*func)(uint8_t, char**, const struct cli_io_funcs_t*);
    const char *name;
    const char *help_string;
};

/**
 *  Descriptor for a CLI instance.
 */
struct cli_desc_t {
    const char *prompt;
    const struct cli_func_desc_t *functions;
    const struct cli_io_funcs_t *console;
    char delim;
};


extern void init_cli(struct cli_desc_t *cli,
                     const struct cli_io_funcs_t *console, const char *prompt,
                     const struct cli_func_desc_t *const functions, char delim);

extern void cli_service(struct cli_desc_t *cli);

#endif /* cli_h */
