# Metamod-r [![Download](https://camo.githubusercontent.com/3d98e8552f23d02a71bc35672904ccc9a1e2201ae6eff9c892539a57035b290d/68747470733a2f2f696d672e736869656c64732e696f2f6769746875622f72656c656173652f74686541736d6f6461692f6d6574616d6f642d722e737667)](https://github.com/theAsmodai/metamod-r/releases/latest) [![Downloads](https://camo.githubusercontent.com/a49efb6ce75429709e735ce0c7ba994ace3f6c93241fc2b42ea523dea39c2abc/68747470733a2f2f696d672e736869656c64732e696f2f6769746875622f646f776e6c6f6164732f74686541736d6f6461692f6d6574616d6f642d722f746f74616c3f636f6c6f723d696d706f7274616e74)]() [![Percentage of issues still open](http://isitmaintained.com/badge/open/theAsmodai/metamod-r.svg)](http://isitmaintained.com/project/theAsmodai/metamod-r "Percentage of issues still open") [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

**Metamod-r** is based on the original version of [Metamod](http://metamod.org/) written by _Will Day_ for Half-Life 1 with some improvements from [Jussi Kivilinna](https://github.com/jkivilin) ([Metamod-p](https://github.com/jkivilin/metamod-p)). This product contains a large number of performance optimizations and more pure code. The core was written using JIT compiler.

**Metamod-r is recommended to be run with [ReHLDS](https://github.com/dreamstalker/ReHLDS) (`API 3.1+`). 
There is compatible with original `HLDS`**

|HLDS | [ReHLDS](https://github.com/dreamstalker/ReHLDS) |  OS | Download |
|---------| -------|    --- | ---   |
| :heavy_check_mark: | :heavy_check_mark: `API 3.1+` |![](https://i.imgur.com/AzhAYR4.png)   ![](https://i.imgur.com/t23p9tU.png)   |  [![Download](https://camo.githubusercontent.com/3d98e8552f23d02a71bc35672904ccc9a1e2201ae6eff9c892539a57035b290d/68747470733a2f2f696d672e736869656c64732e696f2f6769746875622f72656c656173652f74686541736d6f6461692f6d6574616d6f642d722e737667)](https://github.com/theAsmodai/metamod-r/releases/latest)

[![Official Site](https://img.shields.io/badge/Link-Official%20site-3CB371.svg?longCache=true&style=flat-square)](https://metamod-r.org/)
[![Experimental](https://img.shields.io/badge/status-experimental-orange.svg?style=flat-square)](https://github.com/theAsmodai/metamod-r/)
[![GitHub issues](https://img.shields.io/github/issues/theAsmodai/metamod-r.svg?longCache=true&style=flat-square)](https://github.com/theAsmodai/metamod-r/issues)
[![GitHub forks](https://img.shields.io/github/forks/theAsmodai/metamod-r.svg?longCache=true&style=flat-square)](https://github.com/theAsmodai/metamod-r/network)
[![GitHub stars](https://img.shields.io/github/stars/theAsmodai/metamod-r.svg?longCache=true&style=flat-square)](https://github.com/theAsmodai/metamod-r/stargazers)


Distributed under
[![GitHub license](https://img.shields.io/github/license/theAsmodai/metamod-r.svg?longCache=true&style=flat-square)](https://github.com/theAsmodai/metamod-r/blob/master/LICENSE).

## Documentation
* All actual documentation in ![en](https://i.imgur.com/rm67tUZ.png) **English** and ![ru](https://i.imgur.com/ItziiKg.png) **Russian** languages is placed at [this link](https://github.com/theAsmodai/metamod-r/wiki).

### Supported games
* ![en](https://i.imgur.com/rm67tUZ.png) Actual [list of supported games](https://github.com/theAsmodai/metamod-r/wiki/Supported-games).
* ![ru](https://i.imgur.com/ItziiKg.png) Актуальный [список поддерживаемых игр](https://github.com/theAsmodai/metamod-r/wiki/Поддерживаемые-игры).

## Build instructions
### Checking requirements
There are several software requirements for building Metamod-r:

#### Windows
<pre>
Visual Studio 2015 (C++14 standard) and later
</pre>

#### Linux
<pre>
git >= 1.8.5
cmake >= 3.10
GCC >= 4.9.2 (Optional)
ICC >= 15.0.1 20141023 (Optional)
LLVM (Clang) >= 6.0 (Optional)
</pre>

### Building

#### Windows
Use `Visual Studio` to build, open `msvc/metamod.sln` and just select from the solution configurations list `Release` or `Debug`

#### Linux

* Optional options using `build.sh --compiler=[gcc] --jobs=[N] -D[option]=[ON or OFF]` (without square brackets)

<pre>
-c=|--compiler=[icc|gcc|clang]  - Select preferred C/C++ compiler to build
-j=|--jobs=[N]                  - Specifies the number of jobs (commands) to run simultaneously (For faster building)

<sub>Definitions (-D)</sub>
DEBUG                           - Enables debugging mode
USE_STATIC_LIBSTDC              - Enables static linking library libstdc++
</pre>

* ICC          <pre>./build.sh --compiler=intel</pre>
* LLVM (Clang) <pre>./build.sh --compiler=clang</pre>
* GCC          <pre>./build.sh --compiler=gcc</pre>

##### Checking build environment (Debian / Ubuntu)

<details>
<summary>Click to expand</summary>

<ul>
<li>
Installing required packages
<pre>
sudo dpkg --add-architecture i386
sudo apt-get update
sudo apt-get install -y gcc-multilib g++-multilib
sudo apt-get install -y build-essential
sudo apt-get install -y libc6-dev libc6-dev-i386
</pre>
</li>

<li>
Select the preferred C/C++ Compiler installation
<pre>
1) sudo apt-get install -y gcc g++
2) sudo apt-get install -y clang
</pre>
</li>
</ul>

</details>
