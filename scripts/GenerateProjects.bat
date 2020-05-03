@echo off
:: %~dp0 - makes sure file is run from it's location

pushd %~dp0\..\
call vendor\bin\premake\premake5.exe vs2019
popd

PAUSE