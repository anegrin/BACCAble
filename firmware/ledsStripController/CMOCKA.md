### cmocka testing is WIP

install cmocka then:

mkdir build

rm -f build/test*;gcc -DDASHBOARD_MESSAGE_MAX_LENGTH=18 -DPRINTF_INCLUDE_CONFIG_H -ICore/Inc/ Core/Src/printf.c Core/Src/uart_proto.c Test/Src/test_uart.c -o build/test_uart -l:libcmocka.a; ./build/test_uart
rm -f build/test*;gcc -DBUILD_VERSION=\"test\" -DPRINTF_INCLUDE_CONFIG_H -DBHbaccable -ICore/Inc/ Core/Src/printf.c Core/Src/dashboard.c Test/Src/test_dashboard.c -o build/test_dashboard -l:libcmocka.a; ./build/test_dashboard

osx:
rm -f build/test*;gcc -DDASHBOARD_MESSAGE_MAX_LENGTH=18 -DPRINTF_INCLUDE_CONFIG_H -ICore/Inc/ Core/Src/printf.c Core/Src/uart_proto.c Test/Src/test_uart.c -o build/test_uart -L$(brew --prefix cmocka)/lib -lcmocka; ./build/test_uart
rm -f build/test*;gcc -DBUILD_VERSION=\"test\" -DPRINTF_INCLUDE_CONFIG_H -DBHbaccable -ICore/Inc/ Core/Src/printf.c Core/Src/dashboard.c Test/Src/test_dashboard.c -o build/test_dashboard -L$(brew --prefix cmocka)/lib -lcmocka; ./build/test_dashboard