# OCLint extensions

## Как собрать и установить?

```bash
./scripts/build.sh
```

Папка `examples` содержит примеры для проверки работоспособности критериев.

Примеры запуска проверки:

```bash
docker run --rm -it -v ./examples/ex-goto:/app/solution:ro oclint_checker
```

Можно передавать аргументы:

```bash
docker run --rm -it -v ./examples/ex-goto:/app/solution:ro oclint_checker --rule StringCompareRule --rule GotoStatementRule # проверяет код на эти два критерия
```
