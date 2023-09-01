REM Copyright 2023 David A. Greene

REM This file is part of the MIRV project.

REM MIRV is free software: you can redistribute it and/or modify it under the
REM terms of the GNU Affero General Public License as published by the Free
REM Software Foundation, either version 3 of the License, or (at your option)
REM any later version.

REM MIRV is distributed in the hope that it will be useful, but WITHOUT ANY
REM WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
REM FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
REM details.

REM You should have received a copy of the GNU General Public License along with
REM Foobar. If not, see <https://www.gnu.org/licenses/>.

@echo off

for /f "tokens=*" %%G in ('"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -Property InstallationPath') do ^
set "PATH=%PATH%;%%G\Common7\IDE;%cd%\build\dev\Debug"
set "PROMPT=(Debug) %PROMPT%"
