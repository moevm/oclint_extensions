# oclint_extensions

## Что это?

Кастомные правила для oclint. Список реализованных правил:
* слишком много последовательных конструкций if (TooManyConsecutiveIfStatementsRule)

## Установка, сборка и использование

### Создание заготовок для новых правил внутри репозитория oclint

Для начала склонируйте куда-нибудь [репозиторий oclint](https://github.com/oclint/oclint).

Далее необходимо добавить "заготовки" для новых правил. Другими словами, необходимо создать исходники для новых правил и внести их в CMakeLists oclint'а. Новые правила относятся к группе "etu". Для этого выполните следующее:

```bash
cd oclint-scripts
./scaffoldRule -t ASTVisitor -c etu -n"too many consecutive if statements" -p 2 TooManyConsecutiveIfStatementsRule
```

### Копирование исходников с новыми правилами в репозиторий oclint

Из репозитория oclint_extensions выполните:

```bash
cp -r ./oclint-rules YOUR_OCLINT_DIRECTORY_GOES_HERE
```

### Сборка и установка

Вернитесь в репозиторий oclint. Выполните следующее:
```bash
cd oclint-scripts
./make -release # этот скрипт начнет скачивание oclint-json-compilation-database размером в 514 МБ. имейте в виду

cd ..
cd build/oclint-release
sudo cp ./bin/oclint* /usr/local/bin/
sudo cp -rp ./lib/* /usr/local/lib/ 
```

### Использование

```
oclint --rule=TooManyConsecutiveIfStatements examples/ex*.c
```

## P.S.

Очень странный способ установки всего этого. В теории можно сделать форк oclint'а, ноооооо сомнительно. Должен же быть какой-то более простой способ.
Критикуйте, предлагайте.