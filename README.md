Google Translate CLI
=======================
[![Status](https://img.shields.io/badge/status-working-green.svg)]()
[![Cirrus CI](https://api.cirrus-ci.com/github/neverwin/google-translate-cli.svg?branch=master)](https://cirrus-ci.com/github/neverwin/google-translate-cli)

Up-to-date command-line translation program written in C++14 using Boost

**WARNING** The project is for research purpose only. Abuse and commercial use are forbidden. Use it at your own risk.

## Build

Prerequisites are:

* Boost 1.66+
* OpenSSL
* Compiler with C++14
* CMake 3.0+

Build:

~~~bash
mkdir build && cd build
cmake .. && make
~~~

## Usage

translate [language_from language_to]

Example:

~~~bash
$ echo hello world | translate en zh-cn
你好，世界
~~~

## Other References (maybe outdated)

* <https://github.com/matheuss/google-translate-api>

* <https://github.com/matheuss/google-translate-token>

* <https://github.com/soimort/translate-shell>

