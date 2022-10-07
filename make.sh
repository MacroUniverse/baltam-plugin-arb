g++ -shared -fPIC -DBV_USE_DYN_LOADER -D SLS_USE_ARB -I ../../../include/ -L ./bin -o main.so main.cpp  -l flint-arb
mkdir -p ../../../../plugins/arb
cp main.so ../../../../plugins/arb
cp bin/* ../../../../lib/
