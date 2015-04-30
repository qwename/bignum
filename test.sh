g++ -c -std=c++11 -Wall test.cc -o test.o
g++ test.o bin/bignum.a -o test

if [ "$#" -eq 1 ]; then
    if [ "$1" == "debug" ]; then
        OPTBIN := 
        make DEBUG=1 && ./test < data
    else
        make && ./test < data
    fi
else
    make DEBUG=1
    ./test < data
fi

