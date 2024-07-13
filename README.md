### Academia

# Academia Management System

## Overview
This project is an Academia Management System implemented using C, Socket Programming, Linux, File System, Inter-Process Communication (IPC), and System Calls. The system provides a user-friendly Linux terminal interface for admins, faculty, and students, incorporating Linux features such as system calls, semaphores, and file locking for concurrency and protection. Socket programming is utilized for client-server communication.

## Features
- **Admin Functionalities**:
  - Add Faculty
  - Remove Faculty
  - Update Faculty
  - Add Student
  - Update Student
  - Remove Student

- **Faculty Functionalities**:
  - Add Courses
  - View Courses with Student Count
  - Update Courses

- **Student Functionalities**:
  - Register for a Course
  - Drop a Course
  - View Registered Courses

## Technologies Used
- **C Programming Language**
- **Socket Programming**
- **Linux**
- **File System**
- **Inter-Process Communication (IPC)**
- **System Calls**

## Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/DB4558/academia.git
   cd academia-management-system
2.**Setup:**

    step 1 make a directory data//These stores all data files
    step 2 compile and run start.c
    step 3 compile and run server.c(which has acccess to module.c)
    step 4 compile and run client.c
    Max clients is set to 5(but this limitation is overcomed as new process created at background)
    clients can be run simultaneously

**Limitations:**
1)The Max buffer size for server allocated is 2048 bytes
2)Student can enroll in max 5 subjects
3)Faculty can offer max 10 courses(including active and inactive)
4)Only after admin add student/faculty,he/she can login
so Admin is the first module to be executed

**Default passwords:**
1)Admin:userid:admin, paswword:12345
2)Student:userid should start with MT ,password:student
3)Faculty:userid should start with FF,password:faculty
4)Course id:CC
Note:All passwords are hardcoaded and can be changed

## Contributions


    Fork the repository
    Create a new branch (git checkout -b feature-branch)
    Commit your changes (git commit -am 'Add new feature')
    Push to the branch (git push origin feature-branch)
    Create a new Pull Request





**Admin Interface**
    
      
      
       
          WELCOME TO ACADEMIA PORTAL OF IIITB
          
          UserID: 
          admin
          Password: 
          12345
          login success
          ************Admin*******
          1. Add Student
          2. View Student Detail
          3. Add Faculty
          4. View Faculty Detail
          5. Activate Student
          6. Inactivate Student
          7. Modify Student Detail
          8. Modify Faculty Detail
          9.Press e to logout and exit 
          
          Students: 0	Faculties: 0	Courses: 0

**Faculty interface:**
     
        WELCOME TO ACADEMIA PORTAL OF IIITB
        
        UserID: 
        FF0
        Password: 
        faculty
        login success
        ***********Welcome  Faculty  Gaurav***********
        1.Add Course
        2. View Courses Offered
        3. Remove Course
        4. Reset Password
        5. Signout
        Students: 1	Faculties: 1	Courses: 0

**Student interface:**
  
    WELCOME TO ACADEMIA PORTAL OF IIITB
    
    UserID: 
    MT0
    Password: 
    student
    login success
    ****** Welcome Student Deepa********
    1. Enroll New Course
    2. View Enrolled Course Details
    3.Drop Course 
    4. Reset Password
    5. Signout 
Students: 1	Faculties: 1	Courses: 0




