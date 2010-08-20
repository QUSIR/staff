set projectname=staff_axis2_module

set target=%1%
if "%target%" == "" (
  echo.
  echo Error: Target is not set.
  exit 1
)

set deploydir=%cd%\..\..\deploy\win_%PROCESSOR_ARCHITECTURE%
set libdir=%deploydir%\axis2\modules\staff

if not EXIST %libdir% mkdir %libdir%
xcopy /Y /S %target%\*.dll %libdir%
xcopy /Y /S config\*.xml %libdir%
