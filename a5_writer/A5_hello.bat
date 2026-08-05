:: A5 SCRIPT
echo off
echo ================================================
echo   ALGONQUIN COLLEGE - COMPILERS - A5 WRITER
echo   Student: James   Language: J-Lang
echo ================================================

if exist compilers.exe del compilers.exe
if exist CODED_hello.txt del CODED_hello.txt

echo.
echo Compiling...
gcc -o compilers.exe ^
    Compilers.c ^
	Main5Writer.c ^
	Step5Writer.c ^
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
echo Step 1: Encrypting writer_hello.txt -^> CODED_hello.txt
echo ================================================
compilers.exe 1 1 writer_hello.txt CODED_hello.txt
echo.

echo ================================================
echo Step 2: Running Parser on ENCRYPTED file
echo ================================================
compilers.exe 5 CODED_hello.txt
echo.

echo ================================================
echo A5 Writer Hello test completed.
echo ================================================
pause