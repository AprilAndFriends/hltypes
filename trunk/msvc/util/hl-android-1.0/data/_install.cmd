@echo off

set MsBuildCppDir=
echo.

rem On 64-bit machines, Visual Studio and MsBuild is in the (x86) directory. So try that last.
IF EXIST "%ProgramFiles%" set MsBuildCppDir=%ProgramFiles%\MSBuild\Microsoft.Cpp\v4.0\%1\Platforms
IF EXIST "%ProgramFiles(x86)%" set MsBuildCppDir=%ProgramFiles(x86)%\MSBuild\Microsoft.Cpp\v4.0\%1\Platforms

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

	IF NOT EXIST "%MsBuildCppDir%" (
		echo.Failed to find the MsBuild directories that should have been installed with 'Visual Studio'.
		echo."%MsBuildCppDir%"
		pause
		goto cleanup
	)

	IF NOT EXIST "%MsBuildCppDir%\Android" (
		IF NOT EXIST "%MsBuildCppDir%\Android-x86" (
			goto start
		)
	)
	echo.An HL-Android MSBuild toolset already exists.
	echo.Continuing will delete the version already installed.
	rd "%MsBuildCppDir%\Android" /s
	IF EXIST "%MsBuildCppDir%\Android" (
		goto error
	)
	rd "%MsBuildCppDir%\Android-x86" /s /q
	IF EXIST "%MsBuildCppDir%\Android-x86" (
		goto error
	)
	
	echo.
)
	
:start
echo.Installing Android MSBuild files:

md "%MsBuildCppDir%\Android"
xcopy "data\Android\*.*" "%MsBuildCppDir%\Android" /E
md "%MsBuildCppDir%\Android-x86"
xcopy "data\Android-x86\*.*" "%MsBuildCppDir%\Android-x86" /E

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
