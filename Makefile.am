## Process this file with automake to generate Makefile.in.

DXF2VRMLDIR = dxf2vrml
DXFSPHEREDIR = dxfsphere


EXAMPLEPROGDIRS = $(DXF2VRMLDIR) $(DXFSPHEREDIR)

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src $(EXAMPLEPROGDIRS)

cvs-update:
	@cd $(top_srcdir) && cvs update -dP

