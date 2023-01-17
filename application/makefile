########################################################################
####################### Makefile Template ##############################
########################################################################

# Compiler settings - Can be customized.
CC = g++
CXXFLAGS = -std=c++14 -I. -DICE_CPP11_MAPPING

# Makefile settings - Can be customized.
APPNAME = myApp
EXT = .cpp
SRCDIR = ./src
OBJDIR = ./src/obj

# Linking lib
LDFLAGS = -lwthttp -lwt -lwtdbo -lpthread -lIce++11
###	-lwthttp library implements the webserver that implements HTTP(S) and WebSockets
### -lwt library 
### -lwtdbo library 
### -lwtdbopostgres library 
### -lwtdbosqlite3

# Runtime lib
RLIB = --docroot . --http-address 0.0.0.0 --http-port 9090

############## Do not change anything from here downwards! #############
SRC = $(wildcard $(SRCDIR)/*$(EXT))
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)
DEP = $(OBJ:$(OBJDIR)/%.o=%.d)

# UNIX-based OS variables & settings
RM = rm
DELOBJ = $(OBJ)

########################################################################
####################### Targets beginning here #########################
########################################################################

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
