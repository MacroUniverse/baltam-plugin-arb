g++ -shared -fPIC -DBV_USE_DYN_LOADER -D SLS_USE_ARB -I /c/baltamatica/ -I /c/baltamatica/bex/include -I windows/include -I windows/include/flint -L /c/baltamatica/bex/lib -L windows/bin-arb4win -o main.dll main.cpp -l arb-2 -l mpfr-6 -l flint -l gmp-23 -l bex

# no lib
# g++ -shared -fPIC -DBV_USE_DYN_LOADER -D SLS_USE_ARB -I /c/baltamatica/ -I /c/baltamatica/bex/include -I windows/include -L /c/baltamatica/bex/lib -L windows/bin-arb4win -o main.dll main.cpp -l bex
