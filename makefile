all: bin/sdks

clean:
	rm -rf build

clean_h: clean
	rm -rf bin

debug:: CFLAGS += -D 'DEBUG'
debug:: all

build/%.o: src/%.c
	@mkdir -p build
	cc -o $@ -c $< ${CFLAGS}

sdks_deps := $(patsubst %, build/%.o, idx io main sdks)
bin/sdks: ${sdks_deps}
	@mkdir -p bin
	cc -o bin/sdks ${sdks_deps} ${CFLAGS}
