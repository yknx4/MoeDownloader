@echo Updating Deployment
@echo .........................
@echo .........................
@echo Cleaning
del Deployed\Release\x64\*.* /q
del Deployed\Debug\x64\*.* /q
del Deployed\Release\x86\*.* /q
del Deployed\Debug\x86\*.* /q
@echo off
@echo on
@echo .........................
@echo .........................
@echo Updating SourceDLL
@echo off
 
@echo  on
@echo .........................
@echo .........................
@echo Updating GUI
@echo off
copy Gui\bin\x64\Release\*.*  Deployed\Release\x64\ 
copy Gui\bin\x64\Debug\*.*  Deployed\Debug\x64\ 
copy Gui\bin\x86\Release\*.*  Deployed\Release\x86\ 
copy Gui\bin\x86\Debug\*.*  Deployed\Debug\x86\ 
@echo  on
@echo .........................
@echo .........................
@echo Updating Downloaders
@echo off
copy x64\Release\*.*  Deployed\Release\x64\ 
copy x64\Debug\*.*  Deployed\Debug\x64\ 
copy Release\*.*  Deployed\Release\x86\ 
copy Debug\*.*  Deployed\Debug\x86\ 
@echo on
@echo .........................
@echo .........................
@echo Cleaning
del Deployed\Release\x64\*.ilk
del Deployed\Release\x64\*.txt
del Deployed\Release\x64\*.xml
del Deployed\Release\x64\*.metagen
del Deployed\Release\x64\*.pdb
del Deployed\Debug\x64\*.ilk
del Deployed\Debug\x64\*.txt
del Deployed\Debug\x64\*.xml
del Deployed\Debug\x64\*.metagen
del Deployed\Debug\x64\*.pdb
del Deployed\Release\x86\*.ilk
del Deployed\Release\x86\*.txt
del Deployed\Release\x86\*.xml
del Deployed\Release\x86\*.metagen
del Deployed\Release\x86\*.pdb
del Deployed\Debug\x86\*.ilk
del Deployed\Debug\x86\*.txt
del Deployed\Debug\x86\*.xml
del Deployed\Debug\x86\*.metagen
del Deployed\Debug\x86\*.pdb
@echo off
@echo on
@echo .........................
@echo .........................
@echo Finished
@echo off
pause
