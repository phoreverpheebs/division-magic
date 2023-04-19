
HSC = ghc
HSCFLAGS = -Wall
CC = gcc
CCFLAGS = -Wall
BASENAME = rem
CTARGET = remc
HSTARGET = remhs

TARGETS = $(CTARGET) $(HSTARGET)

.PHONY: all clean clean-all
all: c hs
c: $(CTARGET)
hs: $(HSTARGET)

$(CTARGET): $(BASENAME).c
	$(CC) $^ $(CCFLAGS) -o $@

$(HSTARGET): $(BASENAME).hs
	$(HSC) $^ $(HSCFLAGS) -o $@

clean:
	@$(RM) -vf *.o *.hi

clean-all:
	@$(RM) -vf *.o *.hi $(TARGETS)
