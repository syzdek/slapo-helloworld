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

RELEASEDATE		= 2023/04/30
PKGVERSION		= 0.0
LTVERSION		= 0:0:0

OPENLDAP_VERSION	?= 2.5.14
OPENLDAP_DOWNLOAD	?= https://www.openldap.org/software/download/OpenLDAP/openldap-release/openldap-$(OPENLDAP_VERSION).tgz


LIBTOOL			?= libtool
INSTALL			?= /usr/bin/install
CC			?= gcc
CFLAGS_EXTRA		+= -g -O2 -W -Wall -Wextra -Wno-unknown-pragmas
CPPFLAGS_EXTRA		+= -I../../../include \
			   -I../../../servers/slapd \
			   -DSLAPD_OVER_HELLOWORLD=SLAPD_MOD_DYNAMIC
LDFLAGS_EXTRA		+=
NUMJOBS			?= 4

prefix			?= /usr/local
exec_prefix		?= $(prefix)
libdir			?= $(exec_prefix)/lib
libexecdir		?= $(exec_prefix)/libexec
moduledir		?= $(libexecdir)/openldap
mandir			?= $(exec_prefix)/share/man
man5dir			?= $(mandir)/man5
sysconfdir		?= $(prefix)/etc/openldap


TEST_TARGET		= openldap/helloworld-$(OPENLDAP_VERSION)
TEST_FILES		= openldap/contrib/slapd-modules/helloworld/GNUmakefile \
			  openldap/contrib/slapd-modules/helloworld/helloworld.c \
			  openldap/contrib/slapd-modules/helloworld/docs/slapo-helloworld.5.in


.PHONY: all clean distclean install test-env test-env-install uninstall html


.SUFFIXES: .c .o .lo


all: helloworld.la docs/slapo-helloworld.5


helloworld.lo: helloworld.c
	@rm -f $(@)
	$(LIBTOOL) --tag=CC --mode=compile $(CC) $(CFLAGS) $(CFLAGS_EXTRA) \
	   $(CPPFLAGS) $(CPPFLAGS_EXTRA) -o helloworld.lo -c helloworld.c


helloworld.la: helloworld.lo
	@rm -f $(@)
	$(LIBTOOL) --tag=CC --mode=link $(CC) $(LDFLAGS) $(LDFLAGS_EXTRA) \
	   -version-info $(LTVERSION) \
	   -rpath $(moduledir) -module -o helloworld.la helloworld.lo


docs/slapo-helloworld.5: docs/slapo-helloworld.5.in
	@rm -f $(@)
	sed \
	   -e 's,RELEASEDATE,$(RELEASEDATE),g' \
	   -e 's,LDVERSION,$(PKGVERSION),g' \
	   -e 's,ETCDIR,$(sysconfdir),g' \
	   docs/slapo-helloworld.5.in \
	   > $(@)
	@touch $(@)


docs/slapo-helloworld.5.html: GNUmakefile docs/slapo-helloworld.5
	@rm -f $(@)
	cat docs/slapo-helloworld.5 \
	   |groff -mandoc -Thtml \
	   |sed -e 's/<!--.*-->//g' \
	   |sed -e 's!<a href.*</a><br>!!g' \
	   > $(@)
	@touch $(@)


docs/slapo-helloworld.5.txt: docs/slapo-helloworld.5
	@rm -f $(@)
	man docs/slapo-helloworld.5 |col -b > $(@)
	@touch $(@)


html: docs/slapo-helloworld.5.html docs/slapo-helloworld.5.txt


install: helloworld.la docs/slapo-helloworld.5
	mkdir -p $(DESTDIR)/$(moduledir)
	mkdir -p $(DESTDIR)$(man5dir)
	$(LIBTOOL) --mode=install $(INSTALL) -c helloworld.la $(DESTDIR)/$(moduledir)/helloworld.la
	$(INSTALL) -m 644 docs/slapo-helloworld.5 $(DESTDIR)$(man5dir)


uninstall:
	$(LIBTOOL) --mode=uninstall rm -f $(DESTDIR)/$(moduledir)/helloworld.la
	$(LIBTOOL) --mode=uninstall rm -f $(DESTDIR)$(man5dir)/slapo-helloworld.5


clean:
	rm -rf *.o *.lo *.la .libs docs/*.5
	rm -Rf openldap/contrib/slapd-modules/helloworld/


distclean: clean
	rm -Rf openldap openldap-$(OPENLDAP_VERSION).tgz


#
#  The following targets setup the developer's test environment
#

test-env: $(TEST_FILES)
	make -C openldap/contrib/slapd-modules/helloworld prefix=/tmp/slapo-helloworld


test-env-install: test-env $(TEST_TARGET)-install
	make -C openldap/contrib/slapd-modules/helloworld prefix=/tmp/slapo-helloworld install
	$(INSTALL) -m 644 docs/slapd.conf-test-env /tmp/slapo-helloworld/etc/openldap


openldap-$(OPENLDAP_VERSION).tgz:
	wget -O $(@) $(OPENLDAP_DOWNLOAD)
	touch $(@)


$(TEST_TARGET)-downloaded: openldap-$(OPENLDAP_VERSION).tgz
	rm -Rf openldap
	rm -Rf openldap-$(OPENLDAP_VERSION)
	gzip -cd openldap-$(OPENLDAP_VERSION).tgz |tar -xf - \
	   || rm -Rf openldap-$(OPENLDAP_VERSION)
	mv openldap-$(OPENLDAP_VERSION) openldap
	touch $(@)


$(TEST_TARGET)-configure: $(TEST_TARGET)-downloaded
	rm -f $(@)
	cd openldap && ./configure \
	   --prefix=/tmp/slapo-helloworld \
	   --enable-local \
	   --enable-slapd \
	   --enable-dynacl \
	   --enable-aci \
	   --enable-cleartext \
	   --enable-crypt \
	   --enable-syslog \
	   --enable-dynamic \
	   --enable-modules \
           --enable-accesslog=mod \
	   --enable-ppolicy=mod \
	   --enable-mdb=mod \
	   --enable-null=mod \
	   CFLAGS="-I/tmp/slapo-helloworld/include -I/opt/local/include" \
	   CPPFLAGS="-I/tmp/slapo-helloworld/include -I/opt/local/include" \
	   LDFLAGS="-L/tmp/slapo-helloworld/lib -L/opt/local/lib" \
	   || rm -Rf ../openldap
	touch $(@)


$(TEST_TARGET)-depend: $(TEST_TARGET)-configure
	rm -f $(@)
	cd openldap && make -j $(NUMJOBS) depend
	touch $(@)


$(TEST_TARGET)-all: $(TEST_TARGET)-depend
	rm -f $(@)
	cd openldap && make -j $(NUMJOBS)
	touch $(@)


$(TEST_TARGET)-install: $(TEST_TARGET)-all
	rm -f $(@)
	cd openldap && make -j $(NUMJOBS) install
	mkdir /tmp/slapo-helloworld/var/openldap-data
	touch $(@)


openldap/contrib/slapd-modules/helloworld/GNUmakefile: GNUmakefile $(TEST_TARGET)-all
	mkdir -p openldap/contrib/slapd-modules/helloworld
	cp -p GNUmakefile $(@)
	touch $(@)


openldap/contrib/slapd-modules/helloworld/helloworld.c: helloworld.c $(TEST_TARGET)-all
	mkdir -p openldap/contrib/slapd-modules/helloworld
	cp -p helloworld.c $(@)
	touch $(@)


openldap/contrib/slapd-modules/helloworld/docs/slapo-helloworld.5.in: docs/slapo-helloworld.5.in $(TEST_TARGET)-all
	mkdir -p openldap/contrib/slapd-modules/helloworld/docs
	cp -p docs/slapo-helloworld.5.in $(@)
	touch $(@)


# end of makefile
