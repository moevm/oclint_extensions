# oclint_extensions

## Что это?

Кастомные правила для oclint. Список реализованных правил:
* слишком много последовательных конструкций if (TooManyConsecutiveIfStatementsRule)

## Установка, сборка и использование

Требования и зависимости:
* [Требования и зависимости oclint](https://oclint-docs.readthedocs.io/en/stable/intro/build.html)
* CMake &mdash; для сборки
* Python &mdash; для tester.py
* bear: `sudo apt install bear` &mdash; генератор compile_commands.json
* ... ??

```bash
./build-oclint.sh
mkdir build
cd build
cmake ../
cmake --build .
sudo cmake --install .
```

## Использование

tester.py принимает в качестве параметра путь до работы студента (например, Ivanov_Ivan_cw/src), генерирует compile_commands.json и проверяет каждый файл с помощью oclint.

```
python3 tester.py examples/ex1
```