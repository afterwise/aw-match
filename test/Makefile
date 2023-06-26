
export CFLAGS += -Wall -Wextra

test: test.o ../libaw-match.a aw-fs/libaw-fs.a aw-utf8/libaw-utf8.a
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.x aw-fs/aw-fs.h aw-utf8/aw-utf8.h
	$(CC) $(CFLAGS) -I.. -Iaw-utf8 -xc -c $< -o $@

aw-fs/libaw-fs.a:
	$(MAKE) -Caw-fs

aw-utf8/libaw-utf8.a:
	$(MAKE) -Caw-utf8

../libaw-match.a: CFLAGS += -Itest/aw-utf8

../libaw-match.a: aw-utf8/aw-utf8.h
	$(MAKE) -C..

aw-fs/aw-fs.h:
	git clone git@github.com:afterwise/aw-fs.git && \
	touch -c $@

aw-utf8/aw-utf8.h:
	git clone git@github.com:afterwise/aw-utf8.git && \
	touch -c $@

.PHONY: clean
clean:
	rm -f test test.o
	rm -rf aw-fs aw-utf8

