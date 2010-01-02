## Process this file with automake to generate Makefile.in.

LIBS = 
INCLUDES = -I$(top_srcdir)/include


noinst_LTLIBRARIES = libobjects.la


## All source files used when building this submodule, including all
## headers.

ObjectsSources = \
	Object.cpp Object.h \
	UnknownObject.cpp UnknownObject.h

libobjects_la_SOURCES = \
	$(ObjectsSources)



## Installation information.

libobjectsincdir = $(includedir)/dime/objects
libobjectsinc_HEADERS = \
	../../include/dime/objects/Object.h \
	../../include/dime/objects/UnknownObject.h

install-libobjectsincHEADERS: $(libobjectsinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libobjectsincdir)
	@list='$(libobjectsinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libobjectsincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libobjectsincdir)/$$f; \
	done

uninstall-libobjectsincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libobjectsinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libobjectsincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libobjectsincdir)/$$f; \
	done

