@echo off
echo ================================================
echo   ALGONQUIN COLLEGE - COMPILERS - A1 CODER
echo   Student: James   Language: James
echo ================================================

del CODED.txt ORIGINAL.txt 2>nul

echo.
echo Running Cypher...
compilers.exe 1 1 input\README.txt CODED.txt

echo.
echo Running Decypher (verification)...
compilers.exe 1 0 CODED.txt ORIGINAL.txt

echo.
echo Verifying round-trip...
fc ORIGINAL.txt input\README.txt > nul 2>&1

if %errorlevel% == 0 (
    echo SUCCESS: Round-trip verified! ORIGINAL.txt matches README.txt
) else (
    echo ERROR: Files do not match!
)

echo.
echo A1 completed successfully!
pause