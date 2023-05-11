

OpenLDAP Hello World Overlay
============================

Copyright (c) 2023 David M. Syzdek <david@syzdek.net>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted only as authorized by the OpenLDAP
Public License.

A copy of this license is available in the file LICENSE in the
top-level directory of the distribution or, alternatively, at
<http://www.OpenLDAP.org/license.html>.


Contents
--------

   * Overview
   * Software Requirements
   * Source Code
   * Package Maintence Notes


Overview
==========

This package is an example overlay for the OpenLDAP slapd daemon.  The
overlay demonstrates:

   * adding to the LDAP schema of the server
   * defining and using slapd.conf/config backend options
   * manipulating add, modify, and search operations


Software Requirements
=====================

   * OpenLDAP >= 2.5.x


Source Code
===========

The source code for this project is maintained using git
(http://git-scm.com).  The following contains information to checkout the
source code from the git repository.

Browse Source:

   * https://github.com/syzdek/slapo-helloworld

Git URLs:

   * https://github.com/syzdek/slapo-helloworld.git

Downloading Source:

      $ cd openldap-x.x.x
      $ git clone https://github.com/syzdek/slapo-helloworld.git \
         contrib/slapd-modules/helloworld

Compiling Source:

      $ cd openldap-x.x.x
      $ ./configure           # add openldap configure options
      $ make depend && make
      $ cd contrib/slapd-modules/helloworld
      $ make
      $ make install DESTDIR= moduledir=/usr/libexec/libexec/openldap

For more information on building and installing using configure, please
read the INSTALL file.

Git Branches:

   * master - Current release of packages.
   * next   - changes staged for next release
   * pu     - proposed updates for next release
   * xx/yy+ - branch for testing new changes before merging to 'pu' branch


Package Maintence Notes
=======================

This is a collection of notes for developers to use when maintaining this
package.

New Release Checklist:

   - Switch to 'master' branch in Git repository.
   - Update PKGVERSION and RELEASEDATE in GNUmakefile.
   - Update date and version in ChangeLog.md.
   - Commit GNUmakefile and ChangeLog.md changes to repository.
   - Create tag in git repository:

           $ git tag -s v${MAJOR}.${MINOR}

   - Push repository to publishing server:

           $ git push --tags origin master next pu

