# Compiler settings - Can be customized.
CC = g++
CXXFLAGS = -std=c++14 -I.

# Makefile settings - Can be customized.

APPNAME = myApp
EXT = .cpp
SRCDIR = ./src
OBJDIR = ./src/obj


# Linking lib
LDFLAGS = -lwthttp -lwt -lwtdbo -lpthread -lIce++11

############## Do not change anything from here downwards! #############
SRC = $(wildcard $(SRCDIR)/*$(EXT))
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)
DEP = $(OBJ:$(OBJDIR)/%.o=%.d)


# UNIX-based OS variables & settings
RM = rm
DELOBJ = $(OBJ)



all: $(APPNAME)

# Builds the app
$(APPNAME): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Creates the dependecy rules
%.d: $(SRCDIR)/%$(EXT)
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:%.d=$(OBJDIR)/%.o) >$@

# Includes all .h files
-include $(DEP)

# Building rule for .o files and its .c/.cpp in combination with all .h
$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) -o $@ -c $<

################### Cleaning rules ###################
# Cleans complete project
.PHONY: clean
c:
	$(RM) $(DELOBJ) $(DEP) $(APPNAME)

################## RUN #################

r:
	./$(APPNAME) $(RLIB)
d:
	gdb ./$(APPNAME)
