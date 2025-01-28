#ifndef __PARSER_H__
#define __PARSER_H__

#include "expression.h"
#include "tokenizer.h"

Expression *parse_expression(Tokenizer *tokenizer);

#endif
