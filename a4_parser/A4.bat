:: A4 SCRIPT
echo off
echo ================================================
echo   ALGONQUIN COLLEGE - COMPILERS - A4 PARSER
echo   Student: James   Language: J-Lang
echo ================================================

if exist compilers.exe del compilers.exe
if exist CODED.txt del CODED.txt

echo.
echo Compiling...
gcc -o compilers.exe ^
    Compilers.c ^
	Main4Parser.c ^
	Step4Parser.c ^
    Main3Scanner.c ^
    Step3Scanner.c ^
    Main2Reader.c ^
    Step2Reader.c ^
    Main1Coder.c ^
    Step1Coder.c ^
	-Wl,--allow-multiple-definition

if not exist compilers.exe (
    echo.
    echo ERROR: Compilation failed!
    pause
    exit /b 1
)

echo Compilation successful!
echo.

echo ================================================
echo Step 1: Encrypting test_j.txt -^> CODED.txt
echo ================================================
compilers.exe 1 1 test_j.txt CODED.txt
echo.

echo ================================================
echo Step 2: Running Parser on ENCRYPTED file
echo ================================================
compilers.exe 4 CODED.txt
echo.

echo ================================================
echo A4 Parser completed.
echo ================================================
pause