if [ "$#" -eq 1 ]; then
    if [ "$1" == "debug" ]; then
        make DEBUG=1 && bin/debug/test < data
    else
        make && bin/test < data
    fi
else
    make DEBUG=1 && bin/debug/test < data
fi

