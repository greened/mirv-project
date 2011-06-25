BUILDDIR := /home/$(USER)/mirv/obj
FLAVORS := debug opt

CC  := gcc-4.6
CXX := g++-4.6

IFLAGS = -I$(SRCDIR)/src/Library -I$(FINAL_BUILDDIR)/include
DFLAGS = -DBOOST_MPL_LIMIT_VECTOR_SIZE=50 -DBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
# -DBOOST_SP_NO_SP_CONVERTIBLE=1

#debug_CFLAGS := -g -Wall -Werror
#debug_CXXFLAGS := -g -Wall -Werror
debug_CFLAGS := -g -Wall
debug_CXXFLAGS := -g -Wall -std=c++0x
debug_CPPFLAGS = -DDEBUG $(IFLAGS) $(DFLAGS) -std=c++0x

#opt_CFLAGS := -O2 -Wall -Werror
#opt_CXXFLAGS := -O2 -Wall -Werror
opt_CFLAGS := -O2 -Wall
opt_CXXFLAGS := -O2 -Wall -std=c++0x
opt_CPPFLAGS = -O2 $(IFLAGS) $(DFLAGS) -std=c++0x

#VERBOSE := 1
#MDEBUG=1

# Use a custom LLVM for debugging.
LLVM_PATH=$(HOME)/install
