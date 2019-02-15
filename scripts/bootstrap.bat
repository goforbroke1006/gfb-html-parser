@ECHO OFF
SETLOCAL

if NOT EXIST ".\vendor" mkdir .\vendor
cd .\vendor

if NOT EXIST ".\htmlcxx" (
    powershell -Command "Invoke-WebRequest https://netix.dl.sourceforge.net/project/htmlcxx/htmlcxx/0.86/htmlcxx-0.86.tar.gz -OutFile htmlcxx-0.86.tar.gz"
    tar xvzf htmlcxx-0.86.tar.gz
    del htmlcxx-0.86.tar.gz
    REN htmlcxx-0.86 htmlcxx
)

cd ..