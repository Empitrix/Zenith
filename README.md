## BUILD


build:
```bash
cmake -DAPP="blink" -DMCU="STM32F103x8" -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
make -C Release VERBOSE=1 -j
```

<!--
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
-->

## Add a new board
If you want to add a new board first create a folder in `src/boards/<board-name>` witch contains `HAL` , `CMSIS`, `Core` and after that update the `SUPPORTED_BOARDS` in the `CMakeLists.txt` file that exists in the `src/boards`

```plaintext
 boards
└──  BLUE-PILL
    ├──  CMSIS
    │   ├──  Headers
    │   ├──  Include
    │   └──  STM32F1xx
    │       ├──  Include
    │       └──  Source
    │           └──  Templates
    ├──  Core
    │   ├──  Inc
    │   ├── 󱧼 Src
    │   └──  Startup
    └──  HAL
        ├──  Inc
        │   └──  Legacy
        └── 󱧼 Src
```


## Add a new MCU
If you want to add a new mcu first create a folder in `src/mcu/<mcu-name>` witch contains `HAL` , `CMSIS`, `Core` and after that update the `SUPPORTED_MCUS` in the `CMakeLists.txt` file that exists in the `src/mcu`

```plaintext
 mcu
└──  stm32f1xx
    ├──  CMSIS
    │   ├──  Headers
    │   ├──  Include
    │   └──  STM32F1xx
    │       ├──  Include
    │       └──  Source
    │           └──  Templates
    ├──  Core
    │   ├──  Inc
    │   ├── 󱧼 Src
    │   └──  Startup
    └──  HAL
        ├──  Inc
        │   └──  Legacy
        └── 󱧼 Src
```

> [!NOTE]
> If MCU name starts with `STM32` only the first seven character will be extracted e.g. `STM32F1` and two `x` will be added.
> input: `STM32F103x8`, directory: `STM32F1xx`


## Add a new app
If you want to add a new app, create a folder in `src/apps` with the name of the `app` and after that update the `SUPPORTED_APPLICATIONS` in `CMakeLists.txt` that located in the `src/apps`

