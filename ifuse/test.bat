@echo off
echo Attempting to mount iphone storage to 'tmnt' folder...

ifuse tmnt

rem below is an example of how to access the storage container of a specific app
rem ifuse --container com.any.appid tmnt

pause
