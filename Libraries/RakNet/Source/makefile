NAME=	libraknet.a

OUT=	../../lib/$(NAME)

SRCS=	$(shell find -type f -iname "*.cpp")

OBJS=	$(SRCS:.cpp=.o)

all: $(OBJS)
	g++ -c $(OBJS) -Wwrite-strings
	ar rc $(OUT) *.o
	rm *.o

clean:
	rm -f $(OBJS)
