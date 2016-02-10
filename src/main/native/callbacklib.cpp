// Copyright 2016 Yahoo Inc.
// Licensed under the terms of the New-BSD license. Please see LICENSE file in the project root for terms.
#include <string.h>
#include "callbacklib.h"

int magicCFunction(void *ctx, report_fn reportBack) {
    const char *c1 = "Hello There";
    const char *c2 = "I'm bored";
    const char *c3 = "Foo";
    (*reportBack)(ctx, 1, c1, strlen(c1), c2, strlen(c2), c3, strlen(c3));

    return 0;
}

