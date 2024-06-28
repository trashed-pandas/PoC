#!/bin/bash

################################################################################
# Server System Cron:
#
#   * * * * * root  /usr/local/bin/tunnel.sh 2>>/var/log/tunnel.log
#
# This script (tunnel.sh) goes in: /usr/local/bin
#
################################################################################

if ! /usr/bin/ps ux | /usr/bin/grep nerdalicious | /usr/bin/grep -q -v grep; then
  /usr/bin/ssh -N -l empty nerdalicious.com -R localhost:60022:localhost:22 -R localhost:63389:localhost:3389 >>/var/log/tunnel.log
fi

################################################################################
# Client:
#
#   ssh $JUMPHOST -L 63389:localhost:63389 -L 60022:localhost:60022
#   ssh localhost -p 60022
#   /usr/bin/rdesktop -f localhost:63389
################################################################################
