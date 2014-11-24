### Testing
## Table of Contents
- [Google Test](#google-test)
- [Unit Tests](#unit-tests)
- [Output Verification Tests](#output-verification-tests)
  - [Running Verification Tests](#running-verification-tests) 
  - [Adding New Tests](#adding-new-tests)
  - [Sample Output](#sample-output)

### Google Test
##Setting up google test on ububtu 14.04

```bash
wget http://googletest.googlecode.com/files/gtest-1.7.0.zip
unzip gtest-1.7.0.zip
cd gtest-1.7.0
./configure
cmake .
make
```

```bash
sudo cp -a include/gtest /usr/include
sudo cp -a lib/.libs/* /usr/lib/
```

If everything goes ok, then run the following.

```bash
sudo ldconfig -v | grep gtest
```

you should see the following output
```bash
libgtest.so.0 -> libgtest.so.0.0.0
libgtest_main.so.0 -> libgtest_main.so.0.0.0
```
### Unit Tests
- [Google Test Primer](https://code.google.com/p/googletest/wiki/Primer)

Unit tests are added to the Testing/Unittests directory and are auto discoverd by cmake.

To run the unit tests first make sure that the compiler has been built than run the following.
```bash
./cpsl -test
```
### Output Verification Tests
The output verification tests are simple integration tests that compare the output of a .cpsl program with a given expected output defined in a txt file.

#### Running Verification Tests
First ensure that you have built the cpsl compiler. Once built, execute the following to run the verification tests. (Note: you must have python 2.7 or greater installed)
```bash
  python verifyTestFilesOutput.py [PATH_TO_MARS_ON_YOUR_SYSTEM]
```

#### Adding New Tests
To add a new test for a .cpsl file. Simple add the .cpsl file to the CpslFiles directory.
Next, create a file of the following form.
```bash
[CPSL_FILE_NAME]_ExpectedOutput.txt
```
For example, if we had a cpsl file names simple.cpsl, then the resulting expected output file would be.
```
simple_ExpectedOutput.txt.
```
One created simple add the output you expect to the file. Something to note, if your program generates a blank at either the begining or the end of the program, then make sure to add it to the expected output file.

#### Sample Output
If everything is as expected, then one would see the following output.
```
---------------------------------------------------------------------------
Attempting to compile CpslFiles/ifelseif_simple.cpsl
Successfully compiled CpslFiles/ifelseif_simple.cpsl

Attempting to execute asm file in mars
Finished executing asm file in mars

Beginning comparison of CpslFiles/ifelseif_simple.cpsl with TestFilesCorrectOutput/ifelseif_simple_ExpectedOutput.txt
The length of the program's output is as expected
The source program's output is as expected
Finished comparison of CpslFiles/ifelseif_simple.cpsl

---------------------------------------------------------------------------
```
If something goes wrong, then there are three cases.

##### Case 1.
The expected output and source output are of the same length, but do not match.
In the following, line 2 should be blank but is not.
```
---------------------------------------------------------------------------
Attempting to compile CpslFiles/ifelseif_simple.cpsl
Successfully compiled CpslFiles/ifelseif_simple.cpsl

Attempting to execute asm file in mars
Finished executing asm file in mars

Beginning comparison of CpslFiles/ifelseif_simple.cpsl with TestFilesCorrectOutput/ifelseif_simple_ExpectedOutput.txt

The length of the program's output is as expected

	Invalid output at line 2: i less than 4!
	Expected: 

The source program's output does not match the expected output

Finished comparison of CpslFiles/ifelseif_simple.cpsl

---------------------------------------------------------------------------
```
##### Case 2 and 3.
Case 2 and 3 deal with additional lines in both the source and expected output.
In the following, the expected output is larger than the source output.
```
---------------------------------------------------------------------------
Attempting to compile CpslFiles/ifelseif_simple.cpsl
Successfully compiled CpslFiles/ifelseif_simple.cpsl

Attempting to execute asm file in mars
Finished executing asm file in mars

Beginning comparison of CpslFiles/ifelseif_simple.cpsl with TestFilesCorrectOutput/ifelseif_simple_ExpectedOutput.txt

The length of the program's output is 2 but expected a length of 5
Output of CpslFiles/ifelseif_simple.cpsl is shorter than expected
	Contains:  1: i less than 5!
	Missing:   2: i less than 4!
	Missing:   3: i less than 3!
	Missing:   4: i less than 2!
	Missing:   5: i less than 1!
The source program's output does not match the expected output

Finished comparison of CpslFiles/ifelseif_simple.cpsl

---------------------------------------------------------------------------
```
