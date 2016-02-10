// Copyright 2016 Yahoo Inc.
// Licensed under the terms of the New-BSD license. Please see LICENSE file in the project root for terms.
#ifndef _callbacklib_h_
#define _callbacklib_h_

typedef void (*report_fn)(void *ctx, int kind, const char *arg1, int len1,
        const char *arg2, int len2, const char *arg3, int len3);

int magicCFunction(void *ctx, report_fn reportBack);

#endif //_callbacklib_h_
