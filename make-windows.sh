g++ -shared -fPIC -DBV_USE_DYN_LOADER -D SLS_USE_ARB -I ../../../include/ -I windows/include -L windows/bin -o main.so main.cpp -l arb -l gmp -l flint -l mpfr
