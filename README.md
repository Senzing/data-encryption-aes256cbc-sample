# data-encryption-AES256CBC-sample
Sample encryption plugins for a G2 data repository.

## Building the plugins

### Linux

```
cd src
cmake -DCMAKE_BUILD_TYPE=Release setup .
make all
make install
```

### Mac OS

```
cd src
export CC=clang
export CXX=clang++
cmake -DCMAKE_BUILD_TYPE=Release setup .
gmake all
gmake install
```

### Windows

```
cd src
cmake -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 14 2015" -A x64 setup .
```
Build the project through visual studio, or via the command line.

