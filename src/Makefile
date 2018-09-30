CC := gcc
CXX := g++
CFLAGS := -MD -g -O0 -Wall -I../3rdparty/include -I. -D__XVPI__ -pthread -fPIC
CXXFLAGS := -MD -g -O0 -Wall -I../3rdparty/include -I. -D__XVPI__ -pthread -fPIC -std=c++11
LDFLAGS := -MD -g -O0 -Wall -ldl -lm -lpthread -fPIC
PATH := $(PWD)/../3rdparty/bin:$(PATH)
WGET := wget -T 30 -t 0 -c
EXE:=vams.exe
VAfiles:=case.va diode.va ifelse.va operats.va xor2.va

all : $(EXE)

release: $(EXE)
	@mkdir -p ../bin && cp -f $(EXE) ../bin/

$(EXE) : main.o vams_parse.o vpp.o xvpi.o xvpi_elaborate.o vpi_user.o dump-vpi.o  xyce_vacomp.o ../3rdparty/lib/libdparse.a
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

#make test gengold=1  //To generate golden
#make test 	      //To run the test
test : $(EXE) 
	@failed=0 && cd ../benchmark && ln -sf ../src/$(EXE) . && \
	for vaf in $(VAfiles); do \
	  if [ -n "$(gengold)" ]; then\
	    ./$(EXE) $$vaf > ref_$$vaf.log; \
	  else \
	    ./$(EXE) $$vaf > tag_$$vaf.log; \
	    ret_diff=`diff ref_$$vaf.log tag_$$vaf.log`; \
	    if [ -n "$$ret_diff" ]; then \
	      failed=1 && echo "$$vaf failed."; \
	      if [ -n "$(verbose)" ]; then \
		diff ref_$$vaf.log tag_$$vaf.log; \
	      fi \
	    else \
	      rm tag_$$vaf.log; \
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

