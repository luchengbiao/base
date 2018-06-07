
rem set environment variables:  
@set _devenv="C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\devenv.com"

rem set compile log  
@set _log="%~dp0compileResultOfToolKits.log"  
  
@echo [%DATE% %Time%] Start compile sequence >%_log%  
@echo Used compile configuration is %buildAnyCPU% >>%_log% 

rem ***** Start compile tool_kits *****
@set _tool_kits="%~dp0tool_kits\tool_kits.sln"  
@%_devenv% %_tool_kits% /rebuild Debug /out %_log%