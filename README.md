# ModuleTemplate

Defines a submodule template.

## Layout structure

+ Template
  + CMake/Modules - Provides the submodule link to [CMakeModules](https://github.com/chcly/CMakeModules).
    + [CMake/Modules/ExternalTarget.cmake](https://github.com/chcly/CMakeModules/blob/master/ExternalTarget.cmake#L69) - Provides the means to embed modules.
  + Test/googletest - Provides the submodule link to [googletest](https://github.com/chcly/googletest).
  + [Source](Source) - Should contain the module contents.
  + [Test](Test) - Contains the setup to begin testing the source directory.

## Standalone/Embedded

+ The file [CMake/Configure.cmake](CMake/Configure.cmake) is used to define options specific to this submodule.
+ The if statement in [CMakeLists.txt](https://github.com/chcly/ModuleTemplate/blob/master/CMakeLists.txt#L29) is the primary branch between project types.
+ Declaring an external [target](https://github.com/chcly/CMakeModules/blob/master/ExternalTarget.cmake#L69) in the local Configure [file](CMake/Configure.cmake) allows embedding other projects into the solution.
+ The files [gitupdate.py](gitupdate.py) or [gitupdate.bat](gitupdate.bat) help to automate the process of cloning embedded projects.
+ The file [rename_initial_template.py](CMake/rename_initial_template.py) swaps occurrences of `Template` in the project.

## Naming

+ The Source directory should be renamed to the Module name.
+ The variables `Template_INCLUDE` and `Template_LIBRARY` point to the  contents of the Source directory.
+ Defining a external target switches the include variable to its root source directory.
+ Project level includes are found relative to the module's Source directory name `#include "Source/SomeFile.h"`.

## Testing

The Test directory is setup to work with [googletest](https://github.com/google/googletest).

## Building

![A1](https://github.com/chcly/ModuleTemplate/actions/workflows/build-linux.yml/badge.svg)
![A2](https://github.com/chcly/ModuleTemplate/actions/workflows/build-windows.yml/badge.svg)

```sh
mkdir build
cd build
cmake .. -DTemplate_BUILD_TEST=ON -DTemplate_AUTO_RUN_TEST=ON
make
```

Optional defines.

| Option                      | Description                                          | Default |
|:----------------------------|:-----------------------------------------------------|:-------:|
| Template_BUILD_TEST         | Build the unit test program.                         |   ON    |
| Template_AUTO_RUN_TEST      | Automatically run the test program.                  |   OFF   |
| Template_USE_STATIC_RUNTIME | Build with the MultiThreaded(Debug) runtime library. |   ON    |
