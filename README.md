# json_compare
### Description
This program compares two branches in the Alt Linux distribution package database using the public REST API available at https://rdb.altlinux.org/api

# Installation
1. Download source and change directory into source: `git clone https://github.com/DmitryTD/BA.git && cd BA`
2. Add the path for the library search directory: `export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.`
# Usage
Run the program from the command line
`./json_compare`

The result of the program is recorded in `Result.json`
