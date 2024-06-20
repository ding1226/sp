CC = gcc
CFLAGS = -Wall -g

SRCS = main.c lexer.c compiler.c
OBJS = main.o lexer.o compiler.o


TARGET = compiler


all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJS) $(TARGET)


rebuild: clean all
