@echo off
set /p varSprName=Nom du sprite :
set /p varIdxStart=Premier :
set /p varIdxEnd=Dernier :
set /p varFrames=Nb de frames :
FOR /L %%i IN (%varIdxStart%,1,%varIdxEnd%) DO echo %varFrames%, %varSprName% + %%i, >> aaa.txt
