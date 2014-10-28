NAME=server
LINK=g++
CC=gcc
#GCC_VERSIO=$(shell gcc --versio | head -1)

CCFLAGS+=-g
CCFLAGS+=-Wall
CCFLAGS+=-I/usr/local/include/boost-1_52
CCFLAGS+=-I/usr/include/mysql

#LINKOPTS+=-L/usr/lib/mysql
LINKOPTS+=-lmysqlclient
LINKOPTS+=-lboost_system
LINKOPTS+=-lboost_thread
LINKOPTS+=-llua
LINKOPTS+=-ldl #lua¡¥Ω”±®¥Ì¥¶¿Ì
#LINKOPTS+=-fPIC #∂ØÃ¨ø‚±‡“Î
#LINKOPTS+=-shared #∂ØÃ¨ø‚±‡“Î

CPPFILES=	main.cpp \
			const_define.cpp \
			log_manager.cpp \
			plog.cpp \
			plua.cpp \
			config.cpp \
			swig_wrap.cpp \
			luatest.cpp \
			sql_manager.cpp \
			psignal.cpp \
			time_manager.cpp \
			event_manager.cpp \
			pevent.cpp \
			db.cpp \
			db_manager.cpp \
			server.cpp

$(NAME):$(CPPFILES:.cpp=.o)
	$(LINK) -o $(NAME) $(CPPFILES:.cpp=.o) $(LINKOPTS)

.cpp.o:
	$(CC) -c $< $(CCFLAGS)

clean:
	rm -f $(CPPFILES:.cpp=.o) $(NAME)
