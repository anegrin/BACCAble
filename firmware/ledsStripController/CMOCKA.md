### cmocka testing is WIP

install cmocka then:

gcc -DPRINTF_INCLUDE_CONFIG_H -ICore/Inc/ Core/Src/printf.c Core/Src/dashboard.c Test/Src/test.c -o build/test -l:libcmocka.a; ./build/test