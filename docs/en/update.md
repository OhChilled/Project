# Project Update Instructions (Update & Rollback)

## 1. Preparation for Update

Before starting the update, it is necessary to:

- make sure the program is not running
- close all active instances of the application
- verify that the current version is working correctly

---

## 2. Creating Backups

Before updating, you need to create backups:

### Executable File
copy builduntitled2.exe builduntitled2_backup.exe

---

## 3. Compatibility Check

Before updating, you need to check:

- whether CMake requirements have changed
- whether a newer version of the compiler is needed
- whether the program logic has changed
- whether the new version is compatible with old results

---

## 4. Downtime Planning

Since the project is a console application, downtime is minimal.

Updates are performed between program runs.

---

## 5. Update Process

### 5.1 Code Update
git pull

### 5.2 Project Rebuild
cd build
cmake ..
cmake --build .
---

## 6. Stopping Services

Some services in the project are not used.

Before updating, it is enough to:
- close the application
- make sure it is not running

---

## 7. Deploying New Code

After performing git pull and rebuilding, the new version of the application is considered deployed.

---

## 8. Data Migration

Data migration is not required, as the project does not use a DBMS.

---

## 9. Updating Configurations

Configuration files are not used in the project.

---

## 10. Post-Update Check

After updating, it is necessary to:

Run the application:

Windows:
.untitled2.exe

Linux:
./untitled2

Check:
- the application starts
- no errors occur
- calculation results are correct

---

## 11. Criteria for a Successful Update

An update is considered successful if:

- the code has been updated without conflicts
- the build completed without errors
- the program runs
- the results are correct

---

## 12. Rollback Procedure

In case of an error, you need to:

### Option 1 — via Git

git log --oneline
git checkout "name_of_working_rollback"

cd build
cmake ..
cmake --build .

---

### Option 2 — via Backup

del builduntitled2.exe
move /Y builduntitled2_backup.exe builduntitled2.exe