all: bin/sdks

clean:
	rm -f src/*.o

clean_h: clean
	rm -rf bin

bin:
	mkdir -p bin

debug:: CFLAGS += -D 'DEBUG'
debug:: all

src/%.o: src/%.c
	cc -o $@ -c $< ${CFLAGS}

sdks_deps := $(patsubst %, src/%.o, idx io main sdks)
bin/sdks: bin ${sdks_deps}
	cc -o bin/sdks ${sdks_deps} ${CFLAGS}
