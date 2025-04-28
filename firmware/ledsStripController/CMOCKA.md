### cmocka testing is WIP

install cmocka then:

mkdir build

 rm -f build/*;gcc -DPRINTF_INCLUDE_CONFIG_H -DBHbaccable -ICore/Inc/ Core/Src/printf.c Core/Src/dashboard.c Test/Src/test_dashboard.c -o build/test_dashboard -l:libcmocka.a; ./build/test_dashboard

osx:

 rm -f build/test*;gcc -DPRINTF_INCLUDE_CONFIG_H -DBHbaccable -ICore/Inc/ Core/Src/printf.c Core/Src/dashboard.c Test/Src/test_dashboard.c -o build/test_dashboard -L$(brew --prefix cmocka)/lib -lcmocka; ./build/test_dashboard