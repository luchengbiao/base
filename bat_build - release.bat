
rem set environment variables:  
@set _devenv="C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\devenv.com"

rem set compile log  
@set _log="%~dp0compileResults.log"  
  
@echo [%DATE% %Time%] Start compile sequence >%_log%  
@echo Used compile configuration is %buildAnyCPU% >>%_log% 

rem ***** Start compile base_project *****
@set _base_project="%~dp0base_project\base_project.sln"  
@%_devenv% %_base_project% /rebuild Release /out %_log%

rem ***** Start compile agora *****
@set _agora="%~dp0agora\agora.sln"  
@%_devenv% %_agora% /rebuild Release /out %_log%

rem ***** Start compile nim *****
@set _nim="%~dp0nim\nim.sln"  
@%_devenv% %_nim% /rebuild Release /out %_log%

rem ***** Start compile tool_kits *****
@set _tool_kits="%~dp0tool_kits\tool_kits.sln"  
@%_devenv% %_tool_kits% /rebuild Release /out %_log%