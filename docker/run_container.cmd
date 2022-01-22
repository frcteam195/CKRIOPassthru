@echo off&setlocal
for %%i in ("%~dp0..") do set "folder=%%~fi"
docker run -it --rm --net=host ^
    --volume="%folder%:/build/CKRIOPassthru" ^
	guitar24t/ck-roborio-buildenv:amd64 ^
	/bin/bash -c "cd CKRIOPassthru && bash"