#Makefile for Verilog-A compiler front end
#Usages:
#make [DEBUG=0|1] [release] //build binary EXE
#make test gengold=1  //To generate golden
#make test [verbose=1]//To run the test [print details]

CC := gcc
CXX := g++
CFLAGS   := -MD -Wall -I../3rdparty/include -I. -D__XVPI__ -pthread -fPIC
CXXFLAGS := -MD -Wall -I../3rdparty/include -I. -D__XVPI__ -pthread -fPIC -std=c++11
LDFLAGS  := -MD -Wall -ldl -lm -lpthread -fPIC
PATH := $(PWD)/../3rdparty/bin:$(PATH)
WGET := wget -T 30 -t 0 -c
EXE:=xyce_vams
VAfiles:=switchcase.va diode.va ifelse.va ifelse2.va operats.va xor2.va contrib.va macro.va vco.va branches.va
DEBUG ?= 1
ifeq ($(DEBUG),1)
    CFLAGS += -DDEBUG -ggdb
    CXXFLAGS += -DDEBUG -ggdb -D__USE_XOPEN2K8 #add '__USE_XOPEN2K8' with gcc 4.7 pass
else
    CFLAGS += -DNDEBUG -O3
    CXXFLAGS += -DNDEBUG -O3 -D__USE_XOPEN2K8
endif    

all : $(EXE)

release: $(EXE)
	@mkdir -p ../bin && cp -f $(EXE) ../bin/

$(EXE) : main.o vams_parse.o vpp.o xvpi.o xvpi_elaborate.o vpi_user.o dump-vpi.o  xyce_vacomp.o xyce_cxxgen.o ../3rdparty/lib/libdparse.a
	@echo "LINK: $@"
	@$(CXX) $(LDFLAGS) -o $@ $^

# Do not remove generated sources automatically in each build.
.SECONDARY : $($(wildcard *.l):.l=.c) $($(wildcard *.g):.g=.c) $($(wildcard *.gperf):.gperf=.h)  $($(wildcard *.gperf):.gperf=.c)

vpp.c : vpp_compiler_directive_lookup.h

vams_parse.c : vams_keyword_lookup.h

%.c : %.l 
	@echo "GENERATE: $@"
	@flex -o$@ $<

%.c : %.g ../3rdparty/bin/make_dparser
	@echo "GENERATE: $@"
	@../3rdparty/bin/make_dparser -l -v $< -o $@

%.o : %.c ../3rdparty/include/dparse.h
	@echo "COMPILE: $@"
	@$(CC) $(CFLAGS) -c $< -o $@

%.o : %.cc
	@echo "COMPILE C++: $@"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

test : $(EXE) 
	@failed=0 && cd ../benchmark && ln -sf ../src/$(EXE) . && \
	for vaf in $(VAfiles); do \
	  vafb=`basename $$vaf .va` && \
	  if [ -n "$(gengold)" ]; then\
	    ./$(EXE) $$vaf > ref_$$vafb.log; \
	    mv N_DEV_COGENDA_$$vafb.h ref_N_DEV_COGENDA_$$vafb.h; \
	    mv N_DEV_COGENDA_$$vafb.C ref_N_DEV_COGENDA_$$vafb.C; \
	  else \
	    ./$(EXE) $$vaf > tag_$$vafb.log; \
	    ret_diff1=`diff ref_$$vafb.log tag_$$vafb.log`; \
	    ret_diff2=`diff ref_N_DEV_COGENDA_$$vafb.h N_DEV_COGENDA_$$vafb.h`; \
	    ret_diff3=`diff ref_N_DEV_COGENDA_$$vafb.C N_DEV_COGENDA_$$vafb.C`; \
	    if [ -n "$$ret_diff1" -o -n "$$ret_diff2" -o -n "$$ret_diff3" ]; then \
	      failed=1 && echo "$$vaf failed."; \
	      if [ -n "$(verbose)" ]; then \
		diff ref_$$vafb.log tag_$$vafb.log; \
		diff ref_N_DEV_COGENDA_$$vafb.h N_DEV_COGENDA_$$vafb.h; \
		diff ref_N_DEV_COGENDA_$$vafb.C N_DEV_COGENDA_$$vafb.C; \
	      fi \
	    else \
	      rm tag_$$vafb.log N_DEV_COGENDA_$$vafb.*; \
	    fi \
	  fi \
	done && \
	if [ $$failed = 1 ]; then \
	  echo "FAIL" && exit 1; \
	else \
	  echo "PASS" && exit 0; fi 

../3rdparty/include/dparse.h ../3rdparty/lib/libdparse.a : ../3rdparty/bin/make_dparser
../3rdparty/bin/make_dparser :
	( cd ../3rdparty/ && $(MAKE) -f Makefile.dparser-cvs )

distclean : realclean
	@echo "REMOVE: 3rdparty sources and binaries."
	-@rm -rf 3rdparty/bin
	-@rm -rf 3rdparty/include
	-@rm -rf 3rdparty/info
	-@rm -rf 3rdparty/lib
	-@rm -rf 3rdparty/man
	-@rm -rf 3rdparty/share
	-@rm -rf 3rdparty/src

realclean : clean
	@echo "REMOVE: generated sources."
	-@rm -f vpp.c
	-@rm -f vams_keyword_lookup.h
	-@rm -f vams_parse.c

clean :
	@echo "REMOVE: dependency, object and binary files."
	-@rm -rf *.d *.dSYM *.o vams
	@echo "REMOVE: temporary and log files."
	-@find . -name "*.log" -delete
	-@find . -name "*.valgrind" -delete
	@echo "REMOVE: compiled VPI files."
	-@find . -name "*.vpi" -delete
	@echo "REMOVE: parsetree dump file."
	-@rm -f parsetree

-include $(wildcard *.d) dummy


