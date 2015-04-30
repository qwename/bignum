g++ -c -std=c++11 -Wall test.cc -o test.o
if [ "$?" -ne 0 ]; then
    exit
fi

if [ "$#" -ge 1 ]; then
    if [ "$1" == "debug" ]; then
        make DEBUG=1 && \
        g++ test.o bin/debug/bignum.a -o test && \
        ./test $2 < data
    elif [ "$1" == "speed" ]; then
        make SPEED=1 && \
        g++ test.o bin/bignum.a -o test && \
        ./test $2 < data
    else
        make && \
        g++ test.o bin/bignum.a -o test && \
        ./test $1 < data
    fi
else
    make DEBUG=1 && \
    g++ test.o bin/debug/bignum.a -o test && \
    ./test < data
fi

