
export CFLAGS += -Wall -Wextra

test: test.o ../libaw-match.a aw-utf8/libaw-utf8.a
	clang -o $@ $^

%.o: %.c aw-utf8/aw-utf8.h
	clang -I.. -Iaw-utf8 -c $< -o $@

aw-utf8/libaw-utf8.a:
	$(MAKE) -Caw-utf8

../libaw-match.a: CFLAGS += -Itest/aw-utf8

../libaw-match.a: aw-utf8/aw-utf8.h
	$(MAKE) -C..

aw-utf8/aw-utf8.h:
	git clone git@github.com:afterwise/aw-utf8.git && \
	touch -c $@

.PHONY: clean
clean:
	rm -f test test.o
	rm -rf aw-utf8

