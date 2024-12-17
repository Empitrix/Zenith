## BUILD


build:
```bash
cmake -DAPP="blink" -DMCU="STM32F103x8" -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
make -C Release VERBOSE=1 -j
```

> [!NOTE]
> You can only build the projct when only one target specified for MCU you can add `-DMCU=<name>` and for board you can add `-DBOARD=<name>`


Default application is `blink` in located `/src/apps/blink`



## How to add a new board?
add HALL (`Src` and `Inc`) in `src/boards/<board-name>/HALL`
add HALL (`Src` and `Inc`) in `src/boards/<board-name>/Core`
add HALL (`Src` and `Inc`) in `src/boards/<board-name>/CMSIS`

## How to add a new MCU?
add HALL (`Src` and `Inc`) in `src/mcu/<board-name>/HALL`
add HALL (`Src` and `Inc`) in `src/mcu/<board-name>/Core`
add HALL (`Src` and `Inc`) in `src/mcu/<board-name>/CMSIS`


> [!NOTE]
> After adding each `board`, `MCU` or `app`, update the `SUPPORTED_BOARDS`, `SUPPORTED_MCUS` and `SUPPORTED_APPLICATIONS` located in `CMakeLists.txt`
