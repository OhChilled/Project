@echo off
if not exist backup mkdir backup
if exist build\untitled2.exe copy build\untitled2.exe backup\untitled2_backup.exe
echo Backup completed.