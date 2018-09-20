# LUA: C API Tests

Simple test for calling a [Lua](https://www.lua.org) script from plain C, in other words, using Lua as a extension scripting language in C

Lua test script also calls a C function

In this test, both calls (from C and from Lua), get multiple arguments and return multiple values

This test was run with Lua 5.3 using static library `liblua53.a` for MinGW 32-bit (*mingw-4* in Lua download archive)

Context: Windows 10, MinGW gcc 6.3 (not [Mingw-w64](http://mingw-w64.org)), command line for building:

`gcc -ggdb -o0 *.c -llua53 -L. -o lua-test.exe`

