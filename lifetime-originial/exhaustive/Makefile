# include platform-specific definitions
include ../Makefile.defs

SRCS=src/exhaustive.cpp
SETUP_SRCS=src/setup.cpp
OBJS=$(SRCS:.cpp=.o)
SETUP_OBJS=$(SETUP_SRCS:.cpp=.o)
DEPS=$(SRCS:.cpp=.d)
SETUP_DEPS=$(SETUP_SRCS:.cpp=.d)

INCS+=-Iinclude -I../mcs/include -I../utilities/blockFiller
LIBS+=-lmcs -lhotSpot -lblockFiller -lParquetFP -lABKCommon -lpower
LDPATH+=-L../mcs -L../utilities/hotSpot -L../utilities/blockFiller -L../utilities/fp/ParquetFP -L../utilities/fp/ABKCommon -L../utilities/orion

all: exhaustive setup

exhaustive: $(OBJS)
	$(CC) $< -o $@ $(CFLAGS) $(INCS) $(LIBS) $(LDPATH)

setup: $(SETUP_OBJS)
	$(CC) $< -o $@ $(CFLAGS) $(INCS) $(LIBS) $(LDPATH)

clean:
	$(RM) exhaustive setup $(OBJS) $(DEPS)

%.o: %.cpp
	$(CC) $< -o $@ $(CFLAGS) -c $(INCS) -MP -MMD

# include dependencies
-include $(DEPS)
-include $(SETUP_DEPS)
