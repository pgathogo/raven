REM Set up Microsoft Visual Studio 2019, where <arch> is amd64, x86, etc.
CALL "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64
SET _ROOT=C:\QT632
SET PATH=%_ROOT%\bin;C:\Program Files\CMake\bin;C:\Program Files\Git\usr\bin;C:\Python310;..\qwt5\lib;%PATH%
SET _ROOT=
cmd /k
