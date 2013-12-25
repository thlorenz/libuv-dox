#!/usr/bin/env sh

make && curl -L http://code.jquery.com/jquery-1.10.2.js | ./main test.js 1>/dev/null
