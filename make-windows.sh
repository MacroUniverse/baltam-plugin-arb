g++ -shared -fPIC -DBV_USE_DYN_LOADER -D SLS_USE_ARB -I ../../../include/ -I windows/include -L ../../../lib -L windows/bin -o main.dll main.cpp -l arb-2 -l flint -l libbex
cp windows/bin/* ../../../../lib/
