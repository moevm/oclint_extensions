# OCLint extensions

## Как собрать?

1. Собрать докер-образ:

```bash
./scripts/build.sh
```

2. Готово!

## Как запускать?

Папка `examples` содержит примеры для проверки работоспособности критериев.

Примеры запуска проверки:

* Проверка на датасете
```bash
docker run --rm -v ~/code/dataset_pr-cs/:/app/solution/:ro oclint_checker \
    --test dataset --format csv 1> report.csv
```

* Проверка репозитория
```bash
docker run --rm -v ./pr-2024-3384:/app/solution/:ro oclint_checker \
    --test repo --format csv 1> report.csv
```

* Проверка студенческой работы
```bash
docker run --rm -v ./Kashechkin_Oleg_lb1/:/app/solution/:ro oclint_checker \
    --test studwork --format csv 1> report.csv
```

* Проверка теста из `examples`:
``` bash
docker run --rm -v ./examples/ex-string-compare/:/app/solution/:ro oclint_checker \
    --test studwork --format csv 1> report.csv
```
С помощью `--` можно разделить аргументы. Первая половина пойдет нашему oclint_checker'у, вторая половина пойдет самому oclint'у.
Так можно изменять поведение oclint'а. Например:

```bash
docker run --rm -v ./pr-2023-3384/:/app/solution/:ro oclint_checker \
    --test repo --format csv -- --rule MallocSizeof --rule StringCompare \
    --rc=LONG_METHOD=100 1> report.csv
```

## Разработка

Без compile_commands.json ориентироваться в этом всём сложно, поэтому будет полезно собрать всё локально (без докера):

1. Сначала надо собрать oclint, это займет очень много времени:

```bash
sudo scripts/get_oclint.sh
```

2. После этого критерии можно собирать с помощью CMake. Например так:

```bash
mkdir build
cd build
cmake ../
cmake --build .
```

(или можно использовать любое расширение для вашего редактора кода, которое сделает это за вас)

3. Перед запуском тестов критерии нужно еще установить

```bash
cd build
sudo cmake --install .
```

Тесты (или не тесты) запускаются с помощью `scripts/entry.py`. Это буквально точка входа докер образа, поэтому аргументы подаются точно также.
Но поскольку всё запускается локально, входные данные подаются не через докеровский `-v`/`--mount`, а через `-i`/`--input`.
