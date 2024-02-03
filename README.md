# OCLint extensions

## Как собрат и установить?

В одну строку:
```
sudo ./install-deps.sh && ./build-oclint.sh && ./install-oclint.sh && mkdir build && cd build && cmake ../ && cmake --build . && sudo cmake --install . && cd ..
```

То же самое, только в человекочитаемом виде:
```bash
sudo ./install-deps.sh

./build-oclint.sh
./install-oclint.sh

mkdir build
cd build
cmake ../
cmake --build .
sudo cmake --install .
```

Больше информации в [Wiki](https://github.com/moevm/oclint_extensions/wiki/Сборка-и-установка)

Папка `examples` содержит примеры "проектов" с плохим кодом, а также скрипты `test-gcc.sh` и
`test-makefile.sh`. В качестве первого аргумента они принимают путь до папки с исходным кодом,
а последующие аргументы передаются в OCLint, позволяя изменять его поведение. Первый скрипт
предназначем для проектов, состоящих из одного файла с исходным кодом, а второй скрипт - для
проектов с использованием Makefile. 
