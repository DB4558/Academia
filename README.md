
Limitations:
1)The Max buffer size allocated is 2048 bytes
2)Student can enroll in max 5 subjects,after that connection closed by server
3)Faculty can offer max 10 courses,after that connection closed by server
4)Only after admin add student/faculty,he/she can login
so Admin is the first module to be executed

Default passwords:
1)Admin:userid:admin, paswword:12345
2)Student:userid should start with MT ,password:student
3)Faculty:userid should start with FF,password:faculty
4)Course id:CC

Setup:
step 1 make a directory data
step 2 compile and run start.c
step 3 compile and run server.c(which has acccess to module.c)
step 4 compile and run client.c
Max clients is set to 5(but this limitation is overcomed as new process created at background)
clients can be run simultaneously
