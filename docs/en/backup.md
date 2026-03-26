# Backup & Restore Instructions

## 1. General Information

This document describes the backup strategy for a console application for simulating the docking and rendezvous process of spacecraft.

The project does not use:
- DBMS
- server services
- network services

Subject to backup:
- executable file
- calculation results (if any)
- source code (if needed)

---

## 2. Backup Strategy

The strategy includes:

- regular creation of backups of the executable file
- saving simulation results
- the possibility of quickly restoring a working version

---

## 3. Types of Backups

The project uses:

### Full Backup
The entire executable file and results are copied.

Other types (incremental, differential) are not used due to the simple structure of the project.

---

## 4. Backup Frequency

It is recommended to create backups:

- before each update
- before significant code changes
- after obtaining significant simulation results

---

## 5. Storage and Rotation of Copies

Backups are stored in the folder:

backup/

It is recommended to:
- keep several recent copies
- delete outdated copies to save space

---

## 6. Backup Procedure

### 6.1 Executable File
copy builduntitled2.exe backupuntitled2_backup.exe

### 6.2 Results (if any)
copy results* backup

### 6.3 Configurations
Configuration files are not used in the project.

### 6.4 System Logs
Logs are not used in this project.

---

## 7. Backup Automation

A script is used for automation:

docs/scripts/backup.bat

Example of running:

docsscriptsbackup.bat

---

## 8. Checking Backup Integrity

After creating a backup, you need to:

- check the presence of files in the backup folder
- make sure the files are not corrupted
- if necessary, run the restored version

---

## 9. Recovery Procedure

### 9.1 Full System Recovery

1. Delete the current version:
   del builduntitled2.exe

2. Restore from the backup:
   move /Y backupuntitled2_backup.exe builduntitled2.exe

3. Run the program:
   .untitled2.exe

---

### 9.2 Selective Data Recovery

If you need to restore only the results:

copy backup* results
---

## 10. Recovery Testing

After recovery, it is necessary to:

- run the program
- check for correct operation
- make sure that the results are processed correctly

---

## 11. Criteria for Successful Recovery

Recovery is considered successful if:

- the program runs
- no errors occur
- the calculation results are correct