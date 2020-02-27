TARGET = bin/moonrabbit
CFLAGS = $(shell ncursesw6-config --cflags)
#CFLAGS = -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600
LIBS= $(shell ncursesw6-config --libs)
#LIBS = -Wl,-O1,--sort-common,--as-needed,-z,relro,-z,now -lncursesw
INCL = -I ./include
OBJS = \
 ext_table.o \
 cmd_handling.o \
 utils.o \
 signal_handling.o \
 file_operations.o \
 file_type.o \
 main.o

REBUILDABLES = $(OBJS) $(TARGET)

all : $(TARGET)

clean :
	rm -f $(REBUILDABLES)

$(TARGET) : $(OBJS)
	gcc -o $(TARGET) $^ $(CFLAGS) $(LIBS)

%.o : src/%.c
	gcc -c $< $(CFLAGS) $(INCL)