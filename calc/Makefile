CFLAGS = -g -O2 -Wall ${DP} -DGTK_DISABLE_DEPRECATED=1 -DGDK_DISABLE_DEPRECATED=1 -DG_DISABLE_DEPRECATED=1 `pkg-config --cflags gtk+-2.0`
LDLIBS = `pkg-config --libs gtk+-2.0`
LDFLAGS = -g

all : calc 

calc : calc.o
	$(CC) $(LDFLAGS) -o calc calc.o $(LDLIBS)

clean :
	rm -f calc.o calc

