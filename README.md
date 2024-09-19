# 1. build.py usage
```shell
> python3 build.py --help
vcpkg bootstrap/cmake generate/cmake build

options:
  -h, --help            show this help message and exit
  --vcpkg-root-dir VCPKG_ROOT_DIR
                        set vcpkg root dir path (default build/vcpkg)
  --vcpkg-triplet VCPKG_TRIPLET
                        set vcpkg triplet (default x64-windows-static)
  --vcpkg-bootstrap VCPKG_BOOTSTRAP
                        fetch vcpkg and bootstrap it (default False)
  --cmake-build-dir CMAKE_BUILD_DIR
                        set cmake build dir (default build)
  --cmake-generate CMAKE_GENERATE
                        cmake generate build system (default False)
  --cmake-build CMAKE_BUILD
                        cmake build (default False)
  --cmake-config CMAKE_CONFIG
                        cmake build config (default Debug)
```

# 2. bootstrap dependencies, run cmake generate, then build
- first time, or vcpkg.json/vcpkg-configuration.json changed
```shell
> python3 build.py --vcpkg-bootstrap=True --cmake-generate=True --cmake-build=True
```

# 3. run cmake generate, then build
- CMakeLists.txt or file(s) added/removed
```shell
> python3 build.py --cmake-generate=True --cmake-build=True
```

# 4. build only
```shell
> python3 build.py --cmake-build=True
```

# 5. about vcpkg triplet
- auto detect or set values supported by [microsoft/vcpkg triplets](https://github.com/microsoft/vcpkg/tree/master/triplets)
```python
def default_triplet() -> str:
    arch = {
        'x86': 'x86',
        'i386': 'x86',
        'AMD64': 'x64',
        'x86_64': 'x64',
        'arm': 'arm',
        'armv7l': 'arm',
        'arm64': 'arm64',
        'aarch64': 'arm64',
    }[platform.machine()]

    os_name = platform.system()
    if os_name == 'Windows':
        return f'{arch}-windows-static'
    elif os_name == 'Darwin':
        return f'{arch}-osx'
    elif os_name == 'Linux':
        return f'{arch}-linux'
    else:
        logging.error('unsupported os: %s', os_name)
        return ''
```