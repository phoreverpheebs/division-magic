
HSC = ghc
HSCFLAGS = -Wall
CC = gcc
CCFLAGS = -Wall
BASENAME = rem
CTARGET = remc
HSTARGET = remhs

.PHONY: all clean
all: c hs
c: $(CTARGET)
hs: $(HSTARGET)

$(CTARGET): $(BASENAME).c
	$(CC) $^ $(CCFLAGS) -o $@

$(HSTARGET): $(BASENAME).hs
	$(HSC) $^ $(HSCFLAGS) -o $@

clean:
	@$(RM) -vf *.o *.hi $(CTARGET) $(HSTARGET)
