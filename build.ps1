#!/usr/bin/env pwsh

$number_of_build_workers=8

if (Get-Command "cl.exe" -ErrorAction SilentlyContinue) {
  $vstype = "Professional"
  if (Test-Path "C:\Program Files (x86)\Microsoft Visual Studio\2019\${vstype}\Common7\Tools") {
  }
  else {
    $vstype = "Enterprise"
    if (Test-Path "C:\Program Files (x86)\Microsoft Visual Studio\2019\${vstype}\Common7\Tools") {
    }
    else {
      $vstype = "Community"
    }
  }
  Write-Verbose "Found VS 2019 ${vstype}"
  Push-Location "C:\Program Files (x86)\Microsoft Visual Studio\2019\${vstype}\Common7\Tools"
  cmd /c "VsDevCmd.bat -arch=x64 & set" |
    ForEach-Object {
    if ($_ -match "=") {
      $v = $_.split("="); set-item -force -path "ENV:\$($v[0])"  -value "$($v[1])"
    }
  }
  Pop-Location
  Write-Verbose "Visual Studio 2019 ${vstype} Command Prompt variables set.`n"
}
else {
  Write-Verbose "No Compiler found"
}


# DEBUG
Remove-Item .\build_win_debug -Force -Recurse -ErrorAction SilentlyContinue
New-Item -Path .\build_win_debug -ItemType directory -Force
Set-Location build_win_debug
cmake -G "Visual Studio 16 2019" -T "host=x64" -A "x64" "-DCMAKE_BUILD_TYPE=Debug" ..
cmake --build . --config Debug --parallel ${number_of_build_workers} --target install
Set-Location ..

# RELEASE
Remove-Item .\build_win_release -Force -Recurse -ErrorAction SilentlyContinue
New-Item -Path .\build_win_release -ItemType directory -Force
Set-Location build_win_release
cmake -G "Visual Studio 16 2019" -T "host=x64" -A "x64" "-DCMAKE_BUILD_TYPE=Release" ..
cmake --build . --config Release --parallel ${number_of_build_workers} --target install
Set-Location ..

# Download atanherf file
Write-Host "Downloading atanherf interpolation coefficients..." -ForegroundColor Yellow
Set-Location scripts
python3 download_atanherf.py
Set-Location ..
