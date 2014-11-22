### Testing
## Table of Contents
- [Google Test](#google-test)

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
