### cmocka testing is WIP

install cmocka then:

mkdir build

 rm -rf build/*;gcc -DPRINTF_INCLUDE_CONFIG_H -nostdinc -ICore/Inc/ Core/Src/printf.c Core/Src/dashboard.c Test/Src/test.c -o build/test -l:libcmocka.a; ./build/test

osx:

 rm -rf build/*;gcc -DPRINTF_INCLUDE_CONFIG_H -ICore/Inc/ Core/Src/printf.c Core/Src/dashboard.c Test/Src/test.c -o build/test -L$(brew --prefix cmocka)/lib -lcmocka; ./build/test