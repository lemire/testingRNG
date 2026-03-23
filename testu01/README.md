Assuming Linux or macOS. Build from the project root:

```
cmake -B build
cmake --build build
```

TestU01 is built automatically from the included `TestU01.zip` archive.

Then go to a bash shell and type:

```
cd build/testu01
./bigcrushall.sh
```
This runs for a very long time. Testing a single
RNG might take about 2 hours.
 The ``runtests.sh`` scripts executes small crush, and crush,
but these batteries of tests are included in big crush itself.

A fast test inspired by O'Neill's:

```
./linearcomplexity.sh
```
