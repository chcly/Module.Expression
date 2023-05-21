# Expression

Postfix based text compiler to evaluate functions and store variables.

## Testing

The Test directory is setup to work with [googletest](https://github.com/google/googletest).

## Building

![A1](https://github.com/chcly/Module.Expression/actions/workflows/build-linux.yml/badge.svg)
![A2](https://github.com/chcly/Module.Expression/actions/workflows/build-windows.yml/badge.svg)

```sh
mkdir build
cd build
cmake .. -DExpression_BUILD_TEST=ON -DExpression_AUTO_RUN_TEST=ON
make
```

### Optional defines

| Option                        | Description                                          | Default |
| :---------------------------- | :--------------------------------------------------- | :-----: |
| Expression_BUILD_TEST         | Build the unit test program.                         |   ON    |
| Expression_AUTO_RUN_TEST      | Automatically run the test program.                  |   OFF   |
| Expression_USE_STATIC_RUNTIME | Build with the MultiThreaded(Debug) runtime library. |   ON    |
