**Сборка проекта:**

```
git clone https://github.com/GY5e3/YADRO-telecom-test-task.git
```

```
cd YADRO-telecom-test-task
```

```
mkdir build
```

```
cd build
```

```
cmake ..
```

```
cmake --build .
```

**Запуск программы:**

```
./task ../Input/<название_файла>
```

**Запуск тестов:**  
Все тесты:
```
ctest
```
Класс Time:
```
./TimeTests
```
Класс StoiDecorator:
```
./StoiDecoratorTests
```
Класс ClientNameParser:
```
./NameParserTests
```
Класс LogParser:
```
./LogParserTests
```
Класс LogHandler:
```
./LogHandlerTests
```