## Process this file with automake to generate Makefile.in.

INCLUDES = -I$(top_srcdir)/include
LIBS = 

noinst_LTLIBRARIES = libconvert.la


## All source files used when building this submodule, including all
## headers.

ConvertSources = \
	3dfaceconvert.cpp \
	arcconvert.cpp \
	circleconvert.cpp \
	convert.cpp \
	convert_funcs.h \
	ellipseconvert.cpp \
	layerdata.cpp \
	lineconvert.cpp \
	linesegment.cpp \
	linesegment.h \
	lwpolylineconvert.cpp \
	pointconvert.cpp \
	polylineconvert.cpp \
	solidconvert.cpp \
	traceconvert.cpp

libconvert_la_SOURCES = \
	$(ConvertSources)



## Installation information.

libconvertincdir = $(includedir)/dime/convert
libconvertinc_HEADERS = \
	$(top_srcdir)/include/dime/convert/convert.h \
	$(top_srcdir)/include/dime/convert/layerdata.h

install-libconvertincHEADERS: $(libconvertinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libconvertincdir)
	@list='$(libconvertinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libconvertincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libconvertincdir)/$$f; \
	done

uninstall-libconvertincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libconvertinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libconvertincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libconvertincdir)/$$f; \
	done

