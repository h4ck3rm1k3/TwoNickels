## Process this file with automake to generate Makefile.in.

CLEANFILES = 


DISTCLEANFILES = \
        ii_files/*.ii

SUBDIRS = classes entities objects records sections tables util convert .

INCLUDES = -I$(top_srcdir)/include

lib_LTLIBRARIES = libdime.la


DimeSources = \
	Base.cpp Base.h \
	Basic.cpp Basic.h \
	Input.cpp Input.h \
	Layer.cpp Layer.h \
	Model.cpp Model.h \
	Output.cpp Output.h \
	RecordHolder.cpp RecordHolder.h \
	State.cpp State.h

libdime_la_LIBADD = \
	classes/libclasses.la entities/libentities.la objects/libobjects.la \
	records/librecords.la sections/libsections.la tables/libtables.la \
	util/libutil.la convert/libconvert.la

libdime_la_LDFLAGS = \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)


## All source files from the base directory used when building the
## library, including all headers.

libdime_la_SOURCES = \
	$(DimeSources)


libdimeincdir = $(includedir)/dime
libdimeinc_HEADERS = \
	../include/dime/Base.h \
	../include/dime/Basic.h \
	../include/dime/Input.h \
	../include/dime/Layer.h \
	../include/dime/Model.h \
	../include/dime/Output.h \
	../include/dime/RecordHolder.h \
	../include/dime/State.h

# Custom rule for linking a Visual C++ (MS Windows) library.

dime.lib: Makefile \
  $(dime_lib_OBJECTS) \
  $(dime_lib_DEPENDENCIES)
	@basename="dime"; \
	rm -f "$$basename.dll" "$$basename.lib"; \
	echo "Linking $$basename.lib..."; \
	echo $(CC) $(LDFLAGS) $(LIBFLAGS) \
	  $(dime_lib_OBJECTS) \
	  $(dime_lib_LIBADD) $(LIBS); \
	$(CC) $(LDFLAGS) $(LIBFLAGS) \
	  $(dime_lib_OBJECTS) \
	  $(dime_lib_LIBADD) $(LIBS);

install-data-local:
	@$(NORMAL_INSTALL)
	@basename="dime"; \
	for ext in lib; do \
	  file="$$basename.$$ext"; \
	  if test -f "$$file"; then \
	    $(mkinstalldirs) "$(DESTDIR)$(libdir)"; \
	    echo " $(INSTALL_DATA)" "$$file" "$(DESTDIR)$(libdir)/$$file"; \
	    $(INSTALL_DATA) "$$file" "$(DESTDIR)$(libdir)/$$file"; \
	  fi \
	done;

install-exec-local:
	@basename="dime"; \
	for ext in dll pdb; do \
	  file="$$basename.$$ext"; \
	  if test -f "$$file"; then \
	    $(mkinstalldirs) "$(DESTDIR)$(bindir)"; \
	    echo " $(INSTALL_PROGRAM)" "$$file" "$(DESTDIR)$(bindir)/$$file"; \
	    $(INSTALL_PROGRAM) "$$file" "$(DESTDIR)$(bindir)/$$file"; \
	  fi \
	done

install-libdimeincHEADERS: $(libdimeinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libdimeincdir)
	@list='$(libdimeinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libdimeincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libdimeincdir)/$$f; \
	done

uninstall-libdimeincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libdimeinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libdimeincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libdimeincdir)/$$f; \
	done

uninstall-local:
	@basename="dime"; \
	for file in \
	  "$(DESTDIR)$(bindir)/$$basename.dll" \
	  "$(DESTDIR)$(bindir)/$$basename.pdb" \
	  "$(DESTDIR)$(libdir)/$$basename.lib"; do \
	  if test -f "$$file"; then \
	    echo " rm -f" "$$file"; \
	    rm -f "$$file"; \
	  fi \
	done; \
	baseincdir="$(DESTDIR)$(libdimeincdir)"; \
	for dir in \
	  "$$baseincdir/VRMLnodes" \
	  "$$baseincdir/actions" \
	  "$$baseincdir/classes" \
	  "$$baseincdir/convert" \
	  "$$baseincdir/entities" \
	  "$$baseincdir/objects" \
	  "$$baseincdir/records" \
	  "$$baseincdir/sections" \
	  "$$baseincdir/tables" \
	  "$$baseincdir/util" \
	  "$$baseincdir"; do \
	  if test -d "$$dir"; then \
	    echo " rmdir $$dir"; \
	    rmdir "$$dir" || exit 1; \
	  fi \
	done; \
	:

