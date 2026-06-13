#ifndef PARSER_H_GUARD
#define PARSER_H_GUARD
#include "message.h"

BOOL parse(const char *str, size_t len, struct message *output);

#endif
