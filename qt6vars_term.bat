REM Set up Microsoft Visual Studio 2019, where <arch> is amd64, x86, etc.
REM CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" amd64
cmd /c "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64 && start powershell
SET _ROOT=C:\QT632
SET PATH=%_ROOT%\bin;C:\Program Files\CMake\bin;C:\Program Files\Git\usr\bin;C:\Python310;%PATH%
SET _ROOT=
