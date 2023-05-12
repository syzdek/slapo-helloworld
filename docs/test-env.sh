#!/bin/sh
#
#   OpenLDAP Hello World Overlay
#   Copyright (c) 2023 David M. Syzdek <david@syzdek.net>
#   All rights reserved.
#
#   Dominus vobiscum. Et cum spiritu tuo.
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted only as authorized by the OpenLDAP
#   Public License.
#
#   A copy of this license is available in the file LICENSE in the
#   top-level directory of the distribution or, alternatively, at
#   <http://www.OpenLDAP.org/license.html>.
#
#   This script starts the slapd daemon on the maintainer's test environment.
#

/tmp/slapo-helloworld/libexec/slapd \
   -d conns,filter,config,acl,stats,sync \
   -f /tmp/slapo-helloworld/etc/openldap/slapd.conf-test-env \
   -h "ldap://localhost" \
   "${@}"
exit $?

# end of script
