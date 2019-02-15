@ECHO OFF
SETLOCAL

mkdir   .\vendor
cd      .\vendor

if NOT EXIST ".\htmlcxx" (
    (New-Object Net.WebClient).DownloadFile('https://netix.dl.sourceforge.net/project/htmlcxx/htmlcxx/0.86/htmlcxx-0.86.tar.gz', 'htmlcxx-0.86.tar.gz')
    tar xvzf htmlcxx-0.86.tar.gz
    del htmlcxx-0.86.tar.gz
    REN htmlcxx-0.86 htmlcxx
)

cd ..