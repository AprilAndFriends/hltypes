ACLOCAL_AMFLAGS = -I m4
SUBDIRS = src include/hltypes
EXTRA_DIST = autogen.sh

lib_LTLIBRARIES = libhltypes.la
libhltypes_la_SOURCES = 
libhltypes_la_LIBADD =  $(LIBOBJS) \
	src/libhltypes_src.la
libhltypes_la_LDFLAGS = -version-info @LT_VERSION@

