g++ -shared -fPIC -DBV_USE_DYN_LOADER -D SLS_USE_ARB -I ../../../include/ -o main.so main.cpp  -l flint-arb
