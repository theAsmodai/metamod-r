# vi: set ts=4 sw=4 :
# vim: set tw=75 :

# MetaMod toplevel makefile
# Copyright (c) 2001-2003 Will Day <willday@hpgx.net>
#
# some sections as demonstated in GNU's make.info documentation

include Config.mak

ifeq "$(USER)" "willday"
include priv/top.mak
endif

.DEFAULT:
	for i in $(SUBDIRS); do \
		$(MAKE) -C $$i $@ || exit; \
	done

.PHONY:	subdirs dlls $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean cleanall:
	for i in $(SUBDIRS); do \
		$(MAKE) -C $$i cleanall || exit; \
	done
	-rm dlls/* dlls/debug/*
