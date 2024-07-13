### Academia
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

**Setup:**
step 1 make a directory data//These stores all data files
step 2 compile and run start.c
step 3 compile and run server.c(which has acccess to module.c)
step 4 compile and run client.c
Max clients is set to 5(but this limitation is overcomed as new process created at background)
clients can be run simultaneously

**Admin Interface**
    
      
      
       ```bash
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
     ```bash
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
  ```bash
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


