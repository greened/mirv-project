BUILDDIR := /home/$(USER)/mirv/obj
#FLAVORS := debug opt
FLAVORS := debug

CC  := clang
CXX := clang++
#CC  := gcc
#CXX := g++

IFLAGS = -I$(SRCDIR)/src/Library -I$(FINAL_BUILDDIR)/include -I$(HOME)/usr/include
DFLAGS = -DBOOST_MPL_LIMIT_VECTOR_SIZE=50 -DBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
# -DBOOST_SP_NO_SP_CONVERTIBLE=1

#debug_CFLAGS := -g -Wall -Werror
#debug_CXXFLAGS := -g -Wall -Werror
debug_CFLAGS := -g -Wall -Wno-parentheses
debug_CXXFLAGS := -g -Wall -Wno-parentheses -std=c++17
debug_CPPFLAGS = -O0 $(IFLAGS) $(DFLAGS) -std=c++17

#opt_CFLAGS := -O2 -Wall -Werror
#opt_CXXFLAGS := -O2 -Wall -Werror
opt_CFLAGS := -O2 -Wall -Wno-parentheses
opt_CXXFLAGS := -O2 -Wall -Wno-parentheses -std=c++17
opt_CPPFLAGS = -O2 $(IFLAGS) $(DFLAGS) -std=c++17

VERBOSE := 1
#MDEBUG=1

# Use a custom LLVM for debugging.
#LLVM_PATH=$(HOME)/install
