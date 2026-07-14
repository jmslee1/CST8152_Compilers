:: A3 SCRIPT
echo off
echo '--------------------------------------------'
echo '-  [A3: Scanner - J Lang] -'
echo '--------------------------------------------'

if exist compilers.exe del compilers.exe

echo.
echo Compiling...

gcc -o compilers.exe ^
    Compilers.c ^
    Main3Scanner.c ^
    Step3Scanner.c ^
    Main2Reader.c ^
    Step2Reader.c ^
    Main1Coder.c ^
    Step1Coder.c

if not exist compilers.exe (
    echo.
    echo ERROR: Compilation failed!
    pause
    exit /b 1
)

echo Compilation successful!
echo.

:: Run the Scanner (Option 3)
echo Running Scanner...
compilers.exe 3 test_j.txt > out.txt 2> err.txt

echo.
echo ================================================
echo Scanner Output:
echo ================================================
type out.txt

echo.
echo ================================================
echo Errors (if any):
echo ================================================
type err.txt

echo.
echo ================================================
echo A3 Scanner completed.
echo ================================================
pause