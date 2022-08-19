REM Set up Microsoft Visual Studio 2019, where <arch> is amd64, x86, etc.
CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" amd64
SET _ROOT=C:\Qt\6.3.1\Src
SET PATH=%_ROOT%\qtbase\bin;C:\Program Files\CMake\bin;C:\Program Files\Git\usr\bin;C:\Python310;%PATH%
SET _ROOT=
