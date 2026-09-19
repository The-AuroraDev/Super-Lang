#ifndef SUPER_KEYWORDS_H
#define SUPER_KEYWORDS_H

#include "token.h"

TokenKind keyword_lookup(const char *start, size_t length);

#endif