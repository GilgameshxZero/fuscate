@REM Proxy file that calls nmake with evaluated wildcards.
@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION

@REM Careful to not exceed the 8192 string limit. We do not
@REM explicitly list *.hpp files here for that reason.
SET "INCL_PCH=..\rain\include\*"
FOR /F "delims=" %%I IN ('DIR /B /S /AD ..\rain\include') DO (
	SET "INCL_PCH=!INCL_PCH! %%I\*"
)
SET "INCL=..\include\* %INCL_PCH%"
FOR /F "delims=" %%I IN ('DIR /B /S /AD ..\include') DO (
	SET "INCL=!INCL! %%I\*"
)
@REM We choose to list explicitly the *.cpp files here as
@REM they typically number fewer, and is required for the
@REM compiler.
SET "PROJ_SRC_WC=..\src\*.cpp"
FOR /F "delims=" %%I IN ('DIR /B /S /AD ..\src') DO (
	SET "PROJ_SRC_WC=!PROJ_SRC_WC! %%I\*.cpp"
)

@REM Suppresses CMD terminate prompt and error code.
nmake /C %* || (
	SET "LEVEL=!ERRORLEVEL!"
	CALL;
	EXIT /B !LEVEL!
)
ENDLOCAL
