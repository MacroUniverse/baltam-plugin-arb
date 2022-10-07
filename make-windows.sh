g++ -shared -fPIC -DBV_USE_DYN_LOADER -D SLS_USE_ARB -I ../../../include/ -I windows/include -L ../../../lib -L windows/bin -o main.so main.cpp -l arb -l flint -l libbex
