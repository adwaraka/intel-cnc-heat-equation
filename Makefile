ARCH := intel64
M_UNAME := $(shell uname -m)
ifeq ($(M_UNAME), i686)
ARCH := ia32
endif

ifeq (,$(CNC_INSTALL_DIR))
$(info Please estblish CnC environment variables before using this Makefile.)
$(info E.g. by running cncvars.sh or cncvars.csh)
$(info More information is available in 'Getting Started > Running the samples')
$(error CNC_INSTALL_DIR is not set)
endif

SOURCES := heat_matrix.cpp

TARGETS := heat_matrix

CNCFILE := heat_matrix.cnc

DEST_OBJS=$(SOURCES:.cpp=.o)

GEN_HEADER=$(CNCFILE:.cnc=.h)

HINTSFILE := heat_matrix_codinghints.txt

OUTPUTFILE := 2DHeatEquation_parallel.dat

OPT := -O2

all:  heat_matrix

heat_matrix: $(DEST_OBJS)
	$(CXX) -o $@ $(DEST_OBJS) -L$(CNC_INSTALL_DIR)/lib/$(ARCH) -lcnc -ltbb -ltbbmalloc

%.o: %.cpp $(GEN_HEADER)
	$(CXX) -c -I$(CNC_INSTALL_DIR)/include $(OPT) -o $@ $<

$(GEN_HEADER): $(CNCFILE)
	$(CNC_INSTALL_DIR)/bin/$(ARCH)/cnc $(CNCFILE)

clean:
	rm -f $(TARGETS) $(DEST_OBJS) $(GEN_HEADER) $(HINTSFILE) $(OUTPUTFILE)
