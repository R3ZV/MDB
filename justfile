CC := "clang"
FLAGS := "-Wall -Wextra"

dirs:
    @mkdir bin -p

build dbg="": dirs
    @{{CC}} {{FLAGS}} {{ if dbg == "dbg" { "-DDEBUG" } else { "" } }} ./src/*.c -o ./bin/mdb

run ARGS: build
    @./bin/mdb {{ARGS}}

src_files := "`eza ./src/*.c | grep -v main.c`"
test: dirs
    @{{CC}} {{FLAGS}} {{src_files}} ./tests/*.c -o ./bin/tests
    @./bin/tests

clean:
    @rm -r bin
