NAME=	libraknet.a

OUT=	../../lib/$(NAME)

SRCS=	$(shell find -type f -iname "*.cpp")

OBJS=	$(SRCS:.cpp=.o)

all: $(OBJS)
	g++ -c -m32 $(OBJS) -lpthread -Wwrite-strings
	ar rc $(OUT) *.o
	rm *.o

clean:
	rm -f $(OBJS)
