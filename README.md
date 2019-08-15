# waitgdb
Small shared library that allows you to stop the program on crash to allow attaching a debugger (like gdb) on POSIX systems.

## Installation
`make`

`make install` or `checkinstall`

## Usage
`LD_PRELOAD=/usr/local/lib/waitgdb.so your-app`
