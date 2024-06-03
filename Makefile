CC=gcc
CFLAGS_TESTBIN=-Og -fsanitize=address -Wfatal-errors -Wall -Werror -Werror=return-type -Wextra -g -Wpedantic -std=gnu99
CFLAGS=-O3 -flto -march=native -DNDEBUG -fomit-frame-pointer -std=gnu99
MAKEFLAGS += -j$(nproc)
TARGETS=helloworld
TARGETS2=binding
TARGETS3=listen
TARGETS4=connecting
TARGETS5=client
TARGETS6=dumbclient
TARGETS7=listen-DGRAM
TARGETS8=client-DGRAM

TESTTARGET=helloworld-t
TESTTARGET2=binding-t
TESTTARGET3=listen-t
TESTTARGET4=connecting-t
TESTTARGET5=client-t
TESTTARGET6=dumbclient-t
TESTTARGET7=listen-DGRAM-t
TESTTARGET8=client-DGRAM-t

SRCS=hello.c
SRCS2=binding.c
SRCS3=listen.c
SRCS4=connecting.c
SRCS5=client.c
SRCS6=dumbclient.c
SRCS7=listen-DGRAM.c
SRCS8=client-DGRAM.c

all: release
clean:
	rm -f bin/$(TARGETS) bin/$(TARGETS2) bin/$(TARGETS3) bin/$(TARGETS4) bin/$(TARGETS5) bin/$(TARGETS6) bin/$(TARGETS7) bin/$(TARGETS8)
	rm -f test/$(TESTTARGET) test/$(TESTTARGET2) test/$(TESTTARGET3) test/$(TESTTARGET4) test/$(TESTTARGET5) test/$(TESTTARGET6) test/$(TESTTARGET7) test/$(TESTTARGET8) 

tests:
	$(CC) $(CFLAGS_TESTBIN) $(SRCS) -o test/$(TESTTARGET)
	$(CC) $(CFLAGS_TESTBIN) $(SRCS2) -o test/$(TESTTARGET2)
	$(CC) $(CFLAGS_TESTBIN) $(SRCS3) -o test/$(TESTTARGET3)
	$(CC) $(CFLAGS_TESTBIN) $(SRCS4) -o test/$(TESTTARGET4)
	$(CC) $(CFLAGS_TESTBIN) $(SRCS5) -o test/$(TESTTARGET5)
	$(CC) $(CFLAGS_TESTBIN) $(SRCS6) -o test/$(TESTTARGET6)
	$(CC) $(CFLAGS_TESTBIN) $(SRCS7) -o test/$(TESTTARGET7)
	$(CC) $(CFLAGS_TESTBIN) $(SRCS8) -o test/$(TESTTARGET8)

release:
	$(CC) $(CFLAGS) $(SRCS) -o bin/$(TARGETS)
	$(CC) $(CFLAGS) $(SRCS2) -o bin/$(TARGETS2)
	$(CC) $(CFLAGS) $(SRCS3) -o bin/$(TARGETS3)
	$(CC) $(CFLAGS) $(SRCS4) -o bin/$(TARGETS4)
	$(CC) $(CFLAGS) $(SRCS5) -o bin/$(TARGETS5)
	$(CC) $(CFLAGS) $(SRCS6) -o bin/$(TARGETS6)
	$(CC) $(CFLAGS) $(SRCS7) -o bin/$(TARGETS7)
	$(CC) $(CFLAGS) $(SRCS8) -o bin/$(TARGETS8)
