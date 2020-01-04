
# File: Makefile
# Purpose: compile and link gpf.c.
# 
 
CC=/usr/bin/gcc

FOR=/usr/bin/g77
 
TOP=/usr/lib/DXM

TOPDIR=/shr/ipf-3.27

CINCLUDES =     -I.\
                -I.. \
                -I../ipf \
                -I../ipc \
                -I$(TOP)/lib \
                -I$(TOP)/lib/Xt

CFLAGS =        -g $(CINCLUDES) -DDUNDERSCORE

LINTFLAGS=      $(CINCLUDES)

LDFLAGS =       -g -L../ipf -L../ipc

LIBS = -lipf -lipc \
	/usr/lib/gcc-lib/i486-unknown-linux-gnulibc1/2.7.2.3/libf2c.a \
	/usr/lib/libm.a

#
# the IMAGE variable is used to create the executable name
 
IMAGE = gpf

#
# BPFOBJS is a customized list of modules that are being worked on
# and are present in the current directory
  
OBJS = ipfmain.o \
	ipack_2.o \
	ipack_4.o \
	is_it_vms.o \
	ge_utils.o \
	c_err_exit.o \
	c2f_wraps.o \
	f2c_wraps.o \
	srv_cmdprs.o
 
#
# This is the dependancy relationship for powerflow which
# specifies the link and causes the use of the utility compile 
# procedures below.
# WARNING: 
# The <tab> as the first character signifies the action to
# take for a given dependancy.
# The back slash causes a continuation of the current line 
# even if the line is commented out. 

$(IMAGE) : $(OBJS)
	$(FOR) -o $(IMAGE) $(OBJS) $(LDFLAGS) $(LIBS)
	size $(IMAGE)

#
#
# The following are the compile procedures for any source code specified
# in the dependancy.  These override the system defaults.
# the first procedure is for any fortran code (appendix .f creating .o)
# the second is for an C code (appendix .c creating .o)

.c.o:
	$(CC) -c $(CFLAGS) $< 

# DO NOT DELETE THIS LINE -- make depend depends on it.GS) $<
