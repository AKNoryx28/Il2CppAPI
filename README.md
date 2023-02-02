# Build static library
- Clone this repo
    ```sh
    git clone https://github.com/AKNoryx28/Il2CppAPI -b master
    cd Il2CppAPI
    git submodule update --init --recursive
    ```
- build
    ```sh
    # Linux
    chmod a+x build.sh
    ./build.sh <path/to/ndk>
    ```
- Output out/<arch>/libIl2CppAPI.a

## `CMakeLists.txt`
```cmake
add_library(il2cppapi-lib STATIC IMPORTED)
set_target_properties(il2cppapi-lib
                    PROPERTIES IMPORTED_LOCATION
                    ${CMAKE_SOURCE_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI}/libIl2CppAPI.a)

target_link_libraries(
    ...
    l2cppapi-lib
)
```

## `Android.mk`
```mk
...

include $(CLEAR_VARS)
LOCAL_MODULE            := libil2cppapi
LOCAL_SRC_FILES         := out/$(TARGET_ARCH_ABI)/libIl2CppAPI.a
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_STATIC_LIBRARIES += libil2cppapi

```
