# Writing a C Compiler - selfhosted edition

This is a full selfhosted implementation of _Nora Sandler_’s [Writing a C Compiler](https://nostarch.com/writing-c-compiler/) written in **Not-Quite-C**, the subset of C covered by the book. The compiler can first be bootstrapped with gcc/clang, [nqcc2](https://github.com/nlsandler/nqcc2/tree/extra-credit/), [wheelcc](https://github.com/romainducrocq/wheelcc/tree/0.3.0) or <ins>your own implementation</ins> and then rebuild itself (multiple times!) on Linux, MacOS and FreeBSD: in result, the selfhosted compiler passes all 20 chapters of the [test suite](https://github.com/nlsandler/writing-a-c-compiler-tests/) with extra credits. This can also be used by other implementations to test that they can compile a large program (26421 loc counted by cloc) using every language feature in WaCC.  
<!---->
This repository continues where `wheelcc` (my previous compiler written in C) left off and migrates the source code from ISO C17 to the Not-Quite-C subset. So far, `wacc-selfhosted` has been tested successfully :white_check_mark: on:  
  
|x86-64 GNU/Linux|aarch64 MacOS <br/> with x86-64 target|x86-64 FreeBSD|
|:---|:---|:---|
|* Debian GNU/Linux 12<br/>* Linux Mint 22.1<br/>* Ubuntu 24.04.3 LTS<br/>* openSUSE Leap 15.6<br/>* Rocky Linux 10.1<br/>* Arch Linux<br/>* EndeavourOS|* Apple M3 macOS<br/>15.2 24C101 arm64|* FreeBSD <br/>15.0-RELEASE-p1|

The sources are organized in `compiler/` for all the C code and `bin/` for the runtime driver as well as scripts for building and testing. Get started by cloning this project with it's submodules.  
```sh
$ git clone --depth 1 --branch master --recurse-submodules --shallow-submodules https://github.com/romainducrocq/wacc-selfhosted
$ cd wacc-selfhosted/bin/
```
You can now go through this guide to install the selfhosted compiler locally at `bin/wacc-selfhosted`.  
> **Important note**: All the scripts in `bin/` have a `--help` flag to print their correct usage.

* [Project requirements](#Project-requirements)
* [Build the bootstrap](#Build-the-bootstrap)
* [Selfhost the compiler](#Selfhost-the-compiler)
* [Run the test suite](#Run-the-test-suite)
* [Full example](#Full-example)
* [Hello World in Not-Quite-C](#Hello-World-in-Not-Quite-C)

## Project requirements

The project is self-contained and the build+runtime requires only libc with POSIX, bash >= 3 and the C toolchain. Make sure that your system has:  
- GNU/Linux: `binutils`, `gcc` >= 8.1.0
- MacOS: `clang` >= 5.0.0
- FreeBSD: `bash`, `binutils`, `clang` >= 5.0.0 (install bash with `$ sudo pkg install bash`)
  
and... that's it!  
  
The compiler sources also include 2 external libraries in `compiler/3rdparty/`: [sds](https://github.com/antirez/sds) for dynamic strings and [stb_ds](https://github.com/nothings/stb/blob/master/stb_ds.h) for dynamic arrays and hashmaps. They are part of the build and were heavely modified to be compiled in Not-Quite-C.  

## Build the bootstrap

The compiler first needs to be bootsrapped with `./build.sh --bootstrap <compiler>`, with \<compiler\> being 1 of 4 possible compilers that can compile Not-Quite-C code:  

1. **gcc/clang**: Simply use gcc, as Not-Quite-C is a valid subset of C17.  
    ```sh
    # the gcc flag uses clang on MacOS and FreeBSD
    $ ./build.sh --bootstrap gcc
    ```
2. **nqcc2**: Use the reference implementation by first building it manually in the `nqcc2/` submodule, which is checked out on branch `extra-credits`. This branch requires OCaml batteries and can thus not be built with the nqcc2-env 5.2.0 sandbox provided in the repo's instructions, see [build-nqcc2.md](https://github.com/romainducrocq/wacc-selfhosted/blob/master/bin/build-nqcc2.md) to build it with OCaml 4.14.0 instead.  
    ```sh
    # build nqcc2 manually following build-nqcc2.md
    $ cat build-nqcc2.md
    # start Rosetta 2 on aarch64 MacOS with `arch -x86_64 zsh`
    $ ./build.sh --bootstrap nqcc2
    # end Rosetta 2 session with `exit`
    ```
3. **wheelcc**: Use my previous implementation written in C by first building it in the `wheelcc/` submodule with the `build-wheelcc.sh` script. Rest assured that nothing escapes the repo, so it won't pollute your machine :)  
    ```sh
    # build wheelcc locally
    $ ./build-wheelcc.sh
    $ ./build.sh --bootstrap wheelcc
    ```
4. **your own implementation**: Or use your own compiler, this is a great way to test it! Pass the path to your compiler to the build script, along with an optional list of arguments that will be forwarded when your compiler is invoked.  
    ```sh
    # this calls `path/to/wacc.exe -c FILE args...`
    $ ./build.sh --bootstrap path/to/wacc.exe arg1 arg2 arg3
    ```
  
The bootstrapping step produces two outputs: (1) `wacc-bootstrap-<compiler>`, the bootstrap compiler executable (for example _wacc-bootstrap-gcc_), it should not be run directly! and (2) `wacc-selfhosted`, a symlink to the driver which consumes the latest compiler executable. This is the actual user program, see `./wacc-selfhosted --help`.  
<!---->
> **Known issue**: When bootstrapping with nqcc2, _wacc-bootstrap-nqcc2_ fails 15 tests (25 on MacOS) in the test suite due to segfaults. It can however still recompile itself, and the subsequent selfhosted builts then pass the full test suite. As this does not occur with gcc or wheelcc, it is likely a bug in nqcc2 not covered in the tests.

## Selfhost the compiler

The bootstrap compiler can now be used to build the selhosted compiler with `./build.sh <in-compiler> <out-compiler>`, where \<in-compiler\> and \<out-compiler\> are the compiler executable to use for building and the name of the compiler executable to build, respectively. Any name can be chosen for the output.  
```sh
# for example if bootstrapped with nqcc2
$ ./build.sh wacc-bootstrap-nqcc2 wacc-executable-1
```
Furthermore, the compiler can rebuild itself multiple times!  
```sh
# we can repeat this step many times
$ ./build.sh wacc-executable-1 wacc-executable-2
$ ./build.sh wacc-executable-2 wacc-executable-3
```
This has created 3 new executables: `wacc-executable-1`, `wacc-executable-2` and `wacc-executable-3` (and just as before, they should not be run directly). Note that the user program remains _wacc-selfhosted_. By default, the driver consumes the executable that was last created, but it can be changed with `./set-exec.sh <compiler>`.  
```sh
# wacc-selfhosted currently uses wacc-executable-3
$ ./set-exec.sh wacc-executable-2
```
Assuming [a simple hello world program](#Hello-World-in-Not-Quite-C), compile and run it.  
```sh
# wacc-selfhosted will now use wacc-executable-2
$ ./wacc-selfhosted hello_world.c
$ ./hello_world
Hello, World!
```

## Run the test suite

Finally, the selhosted compiler can be tested against the test suite with `./test-suite.sh <compiler> [test_options ...]`. This selects the executable to test and forwards the other options to the test-suite in the submodule at `writing-a-c-compiler-tests/test_compiler`.  
```sh
# run the full test suite on wacc-executable-1
$ ./test-suite.sh wacc-executable-1 --chapter 20 --extra-credit
```

## Full example

Let's sum up and put everything together in a complete example!  
```sh
# bootstrap the compiler with gcc
$ ./build.sh --bootstrap gcc

# selfhost the compiler with the bootstrap
$ ./build.sh wacc-bootstrap-{gcc,clang*} wacc-executable-1

# rebuild itself multiple times
$ ./build.sh wacc-executable-1 wacc-executable-2
$ ./build.sh wacc-executable-2 wacc-executable-3
$ ./build.sh wacc-executable-3 wacc-executable-4
$ ./build.sh wacc-executable-4 wacc-executable-5

# run the full test suite
$ ./test-suite.sh wacc-executable-5 --chapter 20 --extra-credit
----------------------------------------------------------------------
(wacc-executable-5) --chapter 20 --extra-credit
----------------------------------------------------------------------
Ran 1617 tests in 51.547s

OK

# compile and run a program (see below)
$ ./wacc-selfhosted hello_world.c
$ ./hello_world
Hello, World!

# print compiler usage
$ ./wacc-selfhosted --help
```

As an extra feature, this implementation handles compile error graciously by outputing comprehensive error messages with file, line, position and explanation to stderr.  
```sh
# a randomly chosen invalid program in the test suite
$ ./wacc-selfhosted ../writing-a-c-compiler-tests/tests/chapter_17/invalid_types/scalar_expressions/or_void.c
/home/user/wacc-selfhosted/writing-a-c-compiler-tests/tests/chapter_17/invalid_types/scalar_expressions/or_void.c:3:27:
error: (no. 507) cannot apply binary operator ‘||’ on operand types ‘int’ and ‘void’
at line 3:                           v~
         | int main(void) { return 1 || (void)2; }
wacc-executable-5: error: compilation failed, see ‘--help’
```
```sh
# and another random invalid program in the test suite
$ ./wacc-selfhosted ../writing-a-c-compiler-tests/tests/chapter_16/invalid_types/string_literal_is_plain_char_pointer.c
/home/user/wacc-selfhosted/writing-a-c-compiler-tests/tests/chapter_16/invalid_types/string_literal_is_plain_char_pointer.c:5:24:
error: (no. 504) illegal cast, cannot convert expression from type ‘char*’ to ‘signed char*’
at line 5:                        v~~~~
         |     signed char *ptr = "foo";
wacc-executable-5: error: compilation failed, see ‘--help’
```

## Hello World in Not-Quite-C

[A simple hello world program](https://github.com/nlsandler/nqcc2/tree/extra-credit#usage-example) in a file `hello_world.c`.  

```c
int puts(char *c);

int main(void) {
    puts("Hello, world!");
}
```

****
> This software was entirely made by (a) human(s) without the use of LLMs.
  
@romainducrocq  
  