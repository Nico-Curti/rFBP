#!/usr/bin/env powershell

Remove-Item build -Force -Recurse -ErrorAction SilentlyContinue
New-Item -Path .\build -ItemType directory -Force
Set-Location build

cmake -G "Visual Studio 15 2017 Win64" "-DCMAKE_BUILD_TYPE=Release" ..
cmake --build . --config Release

Set-Location ..
