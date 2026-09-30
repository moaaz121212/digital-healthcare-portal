# Digital Healthcare Patient Portal

Console C++ patient portal using OOP (inheritance, polymorphism) with doctor/patient modes, records, and appointment booking.

> Educational project — passwords are plain text, data is in-memory only (no database).

## Features

- Doctor mode: create/delete patients, update medical records
- Patient mode: view history, book visits, view prescriptions, change password
- Login with 3-attempt lockout

## Concepts used

- Inheritance & virtual functions
- Encapsulation (protected medical data)
- Polymorphism (`PortalSystem`, `MedicalRecord`)

## Build & run

```bash
g++ main.cpp -o portal
./portal
```

On Windows (PowerShell):

```powershell
g++ main.cpp -o portal.exe
.\portal.exe
```

Default admin password: `admin123`
