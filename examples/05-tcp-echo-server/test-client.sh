#!/usr/bin/env sh

curl -L http://code.jquery.com/jquery-1.10.2.js | nc 0.0.0.0 7000 1>/dev/null

# If spitting this out to terminal, client gets backed up 
# and connection ends up getting closed before all chunks were written. (see ./main.c read_cb)
# The client connecting next gets the remaining text stuck in the pipe.
# curl -L http://code.jquery.com/jquery-1.10.2.js | nc 0.0.0.0 7000
