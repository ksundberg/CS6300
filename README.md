CPSL
====

An optimizing compiler for the cpsl syntax. The cpsl syntax is similar to fortran syntax.

Building
--------

CPSL uses cmake as its build system. cmake is a cross-platform  out of source build system. Meaning you will create another
location where you can compile the source. You first create a directory then run cmake referencing the base project location.
Afterwards you can run make.

    cd CS6300
    mkdir build
    cd build
    cmake ..
    make

### Project dependencies

There are several third party dependencies that cpsl requires to build.

* Bison
* Flex
* Boost

Allowed options:
----------------

    -h [ --help ]                 produce help message
    -i [ --input ] arg            input cpsl file
    -o [ --output ] arg           output asm file
    -c [ --comments ]             output assembly comments
    -a [ --ast ]                  output ast digraph
    -f [ --flowgraph ]            output flowgraph
    -F [ --flowgraph-optimized ]  output optimized flowgraph

Generating graph visualizations
-------------------------------

Several compiler flags output to stdout a graphviz digraph. You can convert these into an image format using the
graphviz dot binary. The various types of graphs that can be generated are flowgraphs before and after optimizations and
the abstract syntax tree (AST).

    ./cpsl -f foo.cpsl
    ./cpsl -f foo.cpsl | dot -Tpng | feh -

FlowGraphs
----------

Flowgraphs are a compiler internal structure of blocks of code and how they interact. Each node contains a set of
instructions and links to other nodes. There are two types of links: 'br' and 'j'. 'br' signifies a branch and 'j' signifies a jump.

Debugging
---------

There are several utilities to help debug and understand events in the compiler. Logging and Comments are two built in
features for debugging.

### Comments

In order to add comments to a basic block you need to insert a ThreeAddressInstruction::Comment
with the string desired to be output. The generated assembly will have a commented section with your desired string.

### Logging

All logging in cpsl is done through easylogpp. By including logging.h you get the LOG macro which takes various
arguments determining on your log level you want output. Some, but not all, of the output levels are:

* DEBUG
* INFO
* WARN
* FATAL

```
LOG(DEBUG) << "Printing myVar " << myVar;
LOG(INFO) << "Some helpful information";
LOG(WARN) << "This might be bad and you should look at it";
LOG(FATAL) << "An unrecoverable error has occurred";
```

a cpsl.log file will be generated in the directory that cpsl is run. You can customize log output by viewing and
editing log/log.conf. By default cpsl will look for this path relative to the binary for customization.

A default log looks something like this:

    18:38:32 [Type.cpp:59|cpsl_file:23] Adding a type called 'name_t'

Testing
-------

Testing is a very important feature to adding confidance in chamnges. There are two methods of testing in cpsl:
integration and unit tests. The unit test coverage is somewhat lacking currently. The integration tests are very
comprehensive at this point however.

### Integration Tests

The integration tester is automatically copied to your out of source build from cmake. In order to run the tester
several paths will need to be set up. The most important environment settings are

* CPSLDIR - the directory where the cpsl binary resides. defaults to '../' (the parent directory).
* MARSDIR - the directory where the mars binary lives. defaults to current working directory

Once all variables point to correct locations, you can simply run the test script to run the full integration suite.

    ./tester.sh

However, not always do you want to run the full test suite. This is why the first argument to the script will be used as
a prefix for test names that you want to run. So if you wanted to run all the 'simple' test cases you could use this command:

    ./tester.sh simple

All output is placed in the 'Result' directory. The output is compared against the 'Base' directory. Test cases are
stored in the 'TestFiles' directory. Generated assembly output is placed in the 'asm' folder.

### Unit tests

The unit test framework is a header only include called catch.
Unit tests can be run from the cpsl_test binary that is created. To add or alter unit tests, see the Testing/UnitTests
directory.

Style Guidelines
----------------

Code must conform to the clang-format configuration in the root directory of the project. To run a format on the code,
you can use the following find commands:

    find . -name '*.cpp' -print0 | xargs -0 clang-format -i
    find . -name '*.hpp' -print0 | xargs -0 clang-format -i
    find . -name '*.h' -print0 | xargs -0 clang-format -i

