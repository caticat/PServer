NAME=server
LINK=g++
CC=gcc
#GCC_VERSIO=$(shell gcc --versio | head -1)

CCFLAGS+=-g
CCFLAGS+=-Wall
CCFLAGS+=-I/usr/local/include/boost-1_52

LINKOPTS+=-llua
LINKOPTS+=-ldl #lua链接报错处理
#LINKOPTS+=-fPIC #动态库编译
#LINKOPTS+=-shared #动态库编译

CPPFILES=	main.cpp \
			const_define.cpp \
			log_manager.cpp \
			plog.cpp \
			plua.cpp \
			config.cpp \
			swig_wrap.cpp \
			luatest.cpp \
			sql_manager.cpp \
			psignal.cpp

$(NAME):$(CPPFILES:.cpp=.o)
	$(LINK) -o $(NAME) $(CPPFILES:.cpp=.o) $(LINKOPTS)

.cpp.o:
	$(CC) -c $< $(CCFLAGS)

clean:
	rm -f $(CPPFILES:.cpp=.o)
