g++ -shared -fPIC -DBV_USE_DYN_LOADER -D SLS_USE_ARB -I ../../../include/ -I windows/include -L ../../../lib -L windows/bin -o main.dll main.cpp -l arb-2 -l flint -l libbex
mkdir -p ../../../../plugins/arb
cp main.so ../../../../plugins/arb
cp windows/bin/* ../../../../lib/
