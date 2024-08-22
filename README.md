# 1. bootstrap dependencies from vcpkg

## 1.1. Windows
```bat
init_deps.bat
```

## 1.2. Linux/macOS
```bash
./init_deps.sh
```


# 2. configure project
```
cd test_modbus
```

## 2.1. Windows
```bat
1_configure.bat
```

## 2.2. Linux/macOS
```bash
./1_configure.sh
```


# 3. build project

## 3.1 Windows
```bat
2_build.bat
```

## 3.2. Linux/macOS
```bash
./2_build.sh
```


# 4. run examples
## 3.1 Windows
```bat
project\output\Windows\AMD64\Debug\server.exe
project\output\Windows\AMD64\Debug\client.exe
```

## 3.2. Linux/macOS
```bash
./project/output/Windows/AMD64/Debug/server
./project/output/Windows/AMD64/Debug/client
```

