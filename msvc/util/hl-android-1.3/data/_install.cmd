@echo off

set MsBuildCppDir=
echo.

rem On 64-bit machines, Visual Studio and MsBuild is in the (x86) directory. So try that last.
IF EXIST "%ProgramFiles%" set MsBuildCppDir=%ProgramFiles%\MSBuild\Microsoft.Cpp\v4.0\%1
IF EXIST "%ProgramFiles(x86)%" set MsBuildCppDir=%ProgramFiles(x86)%\MSBuild\Microsoft.Cpp\v4.0\%1

rem Prepare install.
IF EXIST "%windir%\system32\hl-install-test" (
	rmdir "%windir%\system32\hl-install-test"
)

rem This will fail if we don't have admin privileges.
mkdir "%windir%\system32\hl-install-test" 2>nul
IF ERRORLEVEL 1 (
	
	echo.This batch file needs to be ran with administrative privileges. Since it copies files to the \Program Files directory.
	pause
	Goto cleanup

) ELSE (

	rem Remove that test directory we just made.
	rmdir "%windir%\system32\hl-install-test"

	echo.
)
	
:start
echo.Installing Android MSBuild files:

xcopy "data\MsBuild\*.*" "%MsBuildCppDir%\" /E
xcopy "data\bin\%1\*.*" "%MsBuildCppDir%\Application Type\Android\bin\" /E

IF ERRORLEVEL 1 (
:error
	echo.Problem with copying
	goto cleanup
)

echo.
echo.Done! You will need to close and re-open existing instances of Visual Studio.
echo.

:cleanup
set MsBuildCppDir=
pause
