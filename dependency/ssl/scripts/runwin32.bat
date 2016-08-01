SET TMPFILE=..\scripts\tmpfile
SET DINCDIR_WIN32=..\include-win32
SET DINCDIR_WIN32_OPENSSL=..\include-win32\openssl
SET CONFIGSCRIPT=..\scripts\Configure-win32.bat

if not exist %CONFIGSCRIPT% (
  echo %CONFIGSCRIPT% not found
  goto quit
)

if not exist Configure (
  echo Run this script under openssl source root
  goto quit
)

if exist $DINCDIR_WIN32_OPENSSL% (
  rmdir /S /Q %DINCDIR_WIN32_OPENSSL%
)

if exist $DINCDIR_WIN32% (
  rmdir /S /Q %DINCDIR_WIN32%
)

md %DINCDIR_WIN32%

md %DINCDIR_WIN32_OPENSSL%

call %CONFIGSCRIPT%
if not errorlevel 0 (
  echo %CONFIGSCRIPT% failed
  goto quit
)

call ms\do_ms.bat
if not errorlevel 0 (
  echo ms\do_ms.bat failed
  goto quit
)

goto SKIPMAKE

nmake -f ms\nt.mak
if not errorlevel 0 (
  echo nmake -f ms\nt.mak failed
  goto quit
)

:SKIPMAKE

copy crypto\buildinf.h %DINCDIR_WIN32%

echo /***** Added by runwin32.bat */ > %TMPFILE%
echo #define NOCRYPT >> %TMPFILE%
echo #define _WINSOCKAPI_ >> %TMPFILE%
echo. >> %TMPFILE%
echo. >> %TMPFILE%

copy %TMPFILE% + crypto\opensslconf.h %DINCDIR_WIN32_OPENSSL%\opensslconf.h

copy crypto\LPdir_win32.c %DINCDIR_WIN32%

copy crypto\LPdir_win.c %DINCDIR_WIN32%

:QUIT


