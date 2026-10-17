@echo off
chcp 65001 > nul
setlocal

echo =======================================================
echo          CHAY KIEM THU TU DONG - BTL KTLT
echo =======================================================

cd /d "%~dp0.."

where python >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo [Loi] Khong tim thay Python trong PATH. Vui long cai dat Python 3.
    pause
    exit /b 1
)

python scripts_auto_test\runner.py %*

set EXIT_CODE=%ERRORLEVEL%
echo.
if %EXIT_CODE% equ 0 (
    echo =^> KET QUA: TAT CA TESTCASE DEU VUOT QUA!
) else (
    echo =^> KET QUA: CO TESTCASE THAT BAI HOAC CO LOI. Vui long kiem tra file log.
)
echo.

endlocal
exit /b %EXIT_CODE%

