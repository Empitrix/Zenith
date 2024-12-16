## BUILD


For debug build:
```bash
cmake -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Debug -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
make -C Debug VERBOSE=1 -j
```

For release build:
```bash
cmake -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
make -C Release VERBOSE=1 -j
```



## How to add a new board?
### Update HAL
Add `Inc` and `Src` folders to `<project>/Drivers/<BOARD>_HAL_Driver/*`

### Update Core
Add `Inc`, `Src`, `Startup` folders to `<project>/Core/<BOARD>_HAL_Driver/*`

after that update `SUPPORTED_BOARDS` in the root `CMakeLists.txt` and add the board name (separate by `;`).

### Update CMSIS
Make sure that `CMSIS/Device` folder is updated. (`CMSIS/Device/ST/<BOARD>/Include`)


