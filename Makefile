VERSION  = 0.1.4

SRC      = src
ifeq ($(PLATFORM),WIN32)
CC       = i686-w64-mingw32-gcc
else ifeq ($(PLATFORM),WIN64)
CC       = x86_64-w64-mingw32-gcc
else ifeq ($(PLATFORM),UNIX)
CC       = gcc
else ifeq ($(PLATFORM),LINUX)
CC       = gcc
else ifeq ($(PLATFORM),)
PLATFORM=LINUX
CC       = gcc
else
$(error Unknown platform '$(PLATFORM)')
endif
ifeq ($(shell which $(CC)),)
$(error "No $(CC) in PATH, please make sure it's installed")
endif
CFLAGS   = -D _PROJECT_VERSION='"$(VERSION)"' -I'$(SRC)/include/'
LDFLAGS  = 
ifeq ($(PLATFORM),$(filter $(PLATFORM),WIN32 WIN64))
LDFLAGS  += \
    -lurlmon \
    -lwininet
endif
OBJFILES = $(SRC)/utils.o $(SRC)/configfile.o $(SRC)/keyboard.o $(SRC)/updater.o $(SRC)/ui.o $(SRC)/main.o
TARGET   = open-typer

all: clean $(TARGET)

debug: CFLAGS+="-D DEBUG -g"

debug: all

$(TARGET): $(OBJFILES)
	@echo LD $@
	@$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

%.o: %.c
	@echo CC $@
	@$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@echo CLEAN $(SRC)/
	@rm -f $(OBJFILES)
	@echo CLEAN ./
	@rm -f $(TARGET) $(TARGET).exe *~
