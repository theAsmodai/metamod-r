@setlocal enableextensions enabledelayedexpansion
@echo off

set repodir="D:\GitHub\metamod\metamod"
set url_commit=
set branch_remote=origin

:: Get remote url
FOR /F "tokens=2 delims=@" %%i IN ('git -C D:\GitHub\metamod\metamod config remote.origin.url') DO (
	set url_commit=%%i
)


set source_string1=git@github.com:s1lentq/metamod.git
set source_string2=https://github.com/s1lentq/metamod.git


If NOT "%source_string1%"=="%source_string1:bitbucket.org=%" (
	echo source_string1 = ECTb!
) ELSE (
	echo source_string1 = HETY!
)


If NOT "%source_string2%"=="%source_string2:bitbucket.org=%" (
	echo source_string2 = ECTb!
) ELSE (
	echo source_string2 = HETY!
)



IF [!url_commit!] == [] (
	FOR /F "tokens=1" %%i IN ('"git -C "%repodir%\." config remote.!branch_remote!.url"') DO (
		set url_commit=%%i
	)

	:: strip .git
	if "x!url_commit:~-4!"=="x.git" (
		set url_commit=!url_commit:~0,-4!
	)
	:: append extra string
	set url_commit=!url_commit!/commit/
) ELSE (
	:: strip .git
	if "x!url_commit:~-4!"=="x.git" (
		set url_commit=!url_commit:~0,-4!
	)
	:: replace : to /
	set url_commit=!url_commit::=/!
	:: append extra string
	set url_commit=https://!url_commit!/commit/
)





::echo Url: = "%url_commit%"


::D:\GitHub\metamod>git -C D:\GitHub\metamod\metamod config remote.origin.url
::https://bitbucket.org/rehlds/metamod

pause