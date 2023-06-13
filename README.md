# json_compare
### Description
This program compares two branches in the Alt Linux distribution package database using the public REST API available at https://rdb.altlinux.org/api

# Installation
1. `apt-get install curl-devel`
2. `apt-get install nlohmann-json-devel`
3. `apt-get install gcc-c++`
4. Download source and change directory into source: `git clone https://github.com/DmitryTD/BA.git && cd BA`
5. `g++ -c -Wall -Werror -fpic lib.cpp`
6. `g++ -shared lib.o -o libtesttlib.so`
7. `g++ -L./ main.cpp -o json_compare -ltesttlib -lcurl`

# Usage
Add the path for the library search directory: `export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.`

Run the program from the command line
`./json_compare`

The result of the program is recorded in `Result.json`
