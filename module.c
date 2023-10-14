#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <sys/stat.h>

// --------------#defines-----------------------------------------
#define BUF_SIZE 2048
#define SMALL_BUF_SIZE 50
#define BIG_BUF_SIZE 100

// ---------------------global variables-------------------------------------------
int student_count, faculty_count, course_count;
int students_fd, faculty_fd, course_fd, details_fd; // Moved file descriptor declarations here
struct flock flock_struct;
// --------------------structures--------------------------------------------
typedef struct
{
    char name[SMALL_BUF_SIZE], fathername[SMALL_BUF_SIZE],email[SMALL_BUF_SIZE], username[SMALL_BUF_SIZE], password[SMALL_BUF_SIZE], homeaddress[BIG_BUF_SIZE],curaddress[BIG_BUF_SIZE];
    char courses_enrolled[5][SMALL_BUF_SIZE];
    int age, courses_enrolled_count;
    int status; 
} student_struct;

typedef struct
{
    char name[SMALL_BUF_SIZE], department[SMALL_BUF_SIZE], designation[SMALL_BUF_SIZE], email[SMALL_BUF_SIZE], contact[SMALL_BUF_SIZE],address[BIG_BUF_SIZE], username[SMALL_BUF_SIZE], password[SMALL_BUF_SIZE];
    char courses_offered[10][SMALL_BUF_SIZE];
    int courses_offered_count;
    int status;
} faculty_struct;

typedef struct
{
    char name[SMALL_BUF_SIZE], course_id[SMALL_BUF_SIZE], department[SMALL_BUF_SIZE], offered_by[SMALL_BUF_SIZE];
    int no_of_seats, available_seats, credits;
    int status; 
} course_struct;

// --------------------Function declarations--------------------------------------------
bool read_record(int filefd, void *add, int index, size_t size);
// bool read_lock(int filefd, int index, size_t size, bool lock); // Changed from bool to int
void open_files();

// --------------------Function Definitions--------------------------------

void open_files()
{
    students_fd = open("data/students.dat", O_RDWR | O_CREAT, 0666);
    faculty_fd = open("data/faculty.dat", O_RDWR | O_CREAT, 0666);
    course_fd = open("data/course.dat", O_RDWR | O_CREAT, 0666);
    details_fd = open("data/details.dat", O_RDWR);
}

int is_number(char *str)
{
    int length = strlen(str);

    for (int i = 0; i < length; i++)
    {
        if (!isdigit(str[i]))
        {
            return 0;
        }
    }

    return 1;
}

void reset_str(char *str, int size)
{
    for (int i = 0; i < size; i++)
    {
        str[i] = '\0';
    }
}
char *substr(char *arr, int begin, int len)
{
    char *res = (char *)malloc(len + 1); // Allocate memory using malloc
    if (res == NULL)
    {
        return NULL; // Return NULL if memory allocation fails
    }

    for (int i = 0; i < len; i++)
    {
        res[i] = *(arr + begin + i);
    }
    res[len] = '\0';

    return res;
}
bool read_lock(int filefd, int index, size_t size)
{
    flock_struct.l_type = F_RDLCK;
    flock_struct.l_whence = SEEK_SET;

    if (index != -1)
    {
        flock_struct.l_start = index * size;
        flock_struct.l_len = size;
    }
    else
    {
        flock_struct.l_start = 0;
        flock_struct.l_len = 0;
    }

    if (fcntl(filefd, F_SETLKW, &flock_struct) == -1)
    {
        return false;
    }

    return true;
}
bool unlock_file(int filefd, int index, size_t size)
{
    flock_struct.l_type = F_UNLCK;
    flock_struct.l_whence = SEEK_SET;

    if (index != -1)
    {
        flock_struct.l_start = index * size;
        flock_struct.l_len = size;
    }
    else
    {
        flock_struct.l_start = 0;
        flock_struct.l_len = 0;
    }

    if (fcntl(filefd, F_SETLKW, &flock_struct) == -1)
    {
        return false;
    }

    return true;
}

bool write_lock(int filefd, int index, size_t size)
{
    flock_struct.l_type = F_WRLCK;
    flock_struct.l_whence = SEEK_SET;

    if (index != -1)
    {
        flock_struct.l_start = index * size;
        flock_struct.l_len = size;
    }
    else
    {
        flock_struct.l_start = 0;
        flock_struct.l_len = 0;
    }

    if (fcntl(filefd, F_SETLKW, &flock_struct) == -1)
    {
        return false;
    }

    return true;
}
void tostring_student(student_struct *student, char *ret)
{
    char age[SMALL_BUF_SIZE];
    sprintf(age, "%d", student->age);

    char status[SMALL_BUF_SIZE];
    if (student->status)
    {
        strcpy(status, "Active\r\n");
    }
    else
    {
        strcpy(status, "Inactive\r\n");
    }

    sprintf(ret, "Name: %s\nFather Name:%s \nEmail: %s\nAge: %s\nHome Address: %s\nCurrent Address:%s\nStatus: %s", student->name, student->fathername,student->email, age, student->homeaddress,student->curaddress ,status);
}
void tostring_faculty(faculty_struct *faculty, char *ret)
{
    sprintf(ret, "Name: %s\nEmail: %s \nContact:%s \nDepartment: %s\nAddress: %s\nDesignation: %s\r\n",
            faculty->name, faculty->email, faculty->contact,faculty->department, faculty->address, faculty->designation);
           char status[SMALL_BUF_SIZE];
    if (faculty->status)
    {
        strcpy(status, "Active\r\n");
    }
    else
    {
        strcpy(status, "Inactive\r\n");
    }
}
void tostring_course(course_struct *course_data, char *ret)
{
    char no_of_seats[SMALL_BUF_SIZE];
    char credits[SMALL_BUF_SIZE];
    char no_of_available_seats[SMALL_BUF_SIZE];

    sprintf(no_of_seats, "%d", course_data->no_of_seats);
    sprintf(credits, "%d", course_data->credits);
    sprintf(no_of_available_seats, "%d", course_data->available_seats);

    sprintf(ret, "Name: %s\nDepartment: %s\nOffered By: %s\nTotal Seats: %s\nCredits: %s\nAvailable Seats: %s\nStatus: %s\r\n",
            course_data->name, course_data->department, course_data->offered_by, no_of_seats, credits, no_of_available_seats,
            (course_data->status) ? "Active" : "Inactive");
}
char *indexed_tostring_char_array(char **array, int n)
{
    char *ret_string = (char *)malloc(BUF_SIZE);
    if (ret_string == NULL)
    {
        // Handle memory allocation failure
        return NULL;
    }
    reset_str(ret_string, BUF_SIZE);

    for (int i = 0; i < n; i++)
    {
        char temp[BIG_BUF_SIZE];
        reset_str(temp, BIG_BUF_SIZE);
        snprintf(temp, BIG_BUF_SIZE, "%d: %s\n", i, array[i]);
        strncat(ret_string, temp, 2 * 1000);

        // Check for buffer overflow
        if (strlen(ret_string) >= BUF_SIZE)
        {
            // Handle buffer overflow
            free(ret_string);
            return NULL;
        }
    }

    return ret_string;
}
char *tostring_char_array(char *array[], int n)
{
    char *ret_string = (char *)malloc(BUF_SIZE * sizeof(char));
    char temp[BUF_SIZE];
    reset_str(ret_string, BUF_SIZE);
    for (int i = 0; i < n; i++)
    {
        reset_str(temp, BUF_SIZE);
        sprintf(temp, "%s\n", array[i]);
        strcat(ret_string, temp);
    }
    return ret_string;
}
int validate_student_id(char *username)
{
    char student_index_str[50];
    int student_index;

    // Extract the numeric part of the username.
    if (sscanf(username, "MT%d", &student_index) != 1)
    {
        return -1;
    }
    printf("index: %d", student_index);
    if (student_index < 0 ||  student_index >= student_count)
    {
        return -1;
    }
    printf("index: %d", student_index);
    return student_index;
}
int validate_faculty_id(char *username)
{
    int faculty_index;

    if (sscanf(username, "FF%d", &faculty_index) != 1)
    {
        return -1; // Username format is incorrect.
    }

    if (faculty_index < 0 || faculty_index >= faculty_count)
    {
        return -1; // Invalid faculty ID or username.
    }

    return faculty_index;
}
int validate_course_id(char *course_id)
{
    int course_index;

    if (sscanf(course_id, "CC%d", &course_index) != 1)
    {
        return -1; // Course ID format is incorrect.
    }

    if (course_index < 0 || course_index >= course_count)
    {
        return -1; // Invalid course ID or course_id.
    }

    return course_index;
}
int write_client(int clientfd, char *data)
{
    int wrote;
    if ((wrote = write(clientfd, data, strlen(data)) == -1))
    {
        perror("write");
        return -1;
    }
    return wrote;
}
int read_client(int clientfd, char *data)
{
    int read_bytes;
    if ((read_bytes = read(clientfd, data, BUF_SIZE)) == -1)
    {
        perror("read");
        return -1;
    }
    return read_bytes;
}
void print_int_array(int *arr, int n)
{
    printf("********************\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\n", arr[i]);
    }
    printf("*********************\n");
}
void print_char_array(char **strings, int n)
{
    printf("********************\n");
    for (int i = 0; i < n; i++)
    {
        printf("%s\n", strings[i]);
    }
    printf("*********************\n");
}
void read_lines_from_file_fd(char **list, int fd, int n)
{
    for (int i = 0; i < n; i++)
    {
        char *temp = (char *)malloc(BUF_SIZE);
        if (temp == NULL)
        {
            // Handle memory allocation failure
            return;
        }

        int str_ind = 0;
        char t;
        while (1)
        {
            if (read(fd, &t, sizeof(t)) > 0)
            {
                if (t == '\n')
                {
                    temp[str_ind] = '\0';
                    list[i] = temp;
                    break;
                }
                else
                {
                    temp[str_ind++] = t;
                }
            }
            else
            {
                free(temp); // Free allocated memory if there is an error.
                break;
            }
        }
    }

    // Print the array (print_char_array is assumed to be defined).
    printf("**********************\n");
    for (int i = 0; i < n; i++)
    {
        printf("%s\n", list[i]);
        free(list[i]); // Free each line.
    }
    printf("**********************\n");

    free(list); // Free the list of lines.
}

void load_details()
{
    int facultyfd, studentsfd;
    lseek(details_fd, 0, SEEK_SET);
    printf("Getting read lock for details\n");
    read_lock(details_fd, -1, sizeof(student_count));
    printf("Got lock for details\n");
    read(details_fd, &student_count, sizeof(student_count));
    read(details_fd, &faculty_count, sizeof(faculty_count));
    read(details_fd, &course_count, sizeof(course_count));
    printf("read\n");
    unlock_file(details_fd, -1, sizeof(int));
    printf("returning from load details\n");
}

bool login(int clientfd, char *username, int *user_type)
{
    char revd[BUF_SIZE];
    int read_bytes;
    printf("Asking username\n");
    strcpy(revd, "WELCOME TO ACADEMIA PORTAL OF IIITB\n\nUserID: \r\n");
    if (write(clientfd, revd, strlen(revd)) == -1)
    {
        perror("write");
        return false;
    }
    reset_str(revd, BUF_SIZE);
    if ((read_bytes = read(clientfd, revd, BUF_SIZE)) == -1)
    {
        perror("read");
        return false;
    }
    revd[read_bytes] = '\0';
    strcpy(username, revd);
    printf("%d received: %s\n", read_bytes, revd);
    if (strcmp(revd, "admin") == 0)
    {
        *user_type = 1;
        printf("username valid, asking for password\n");
        reset_str(revd, BUF_SIZE);
        strcpy(revd, "Password: \r\n");
        if (write(clientfd, revd, strlen(revd)) == -1)
        {
            perror("write");
            return false;
        }
        printf("Asking Password: \n");
        reset_str(revd, BUF_SIZE);
        if ((read(clientfd, revd, BUF_SIZE)) == -1)
        {
            perror("read");
            return false;
        }
        printf("Password Received: \n");
        if (strcmp(revd, "12345") == 0)
        {
            reset_str(revd, BUF_SIZE);
            strcpy(revd, "login success\r\n");
            if (write(clientfd, revd, strlen(revd)) == -1)
            {
                perror("write");
                return false;
            }
            return true;
        }
    }
    else if (revd[0] == 'M' && revd[1] == 'T')
    {
        *user_type = 2;
        printf("username valid, asking for password\n");
        reset_str(revd, BUF_SIZE);
        strcpy(revd, "Password: \r\n");
        if (write(clientfd, revd, strlen(revd)) == -1)
        {
            perror("write");
            return false;
        }
        printf("Asking Password: \n");
        reset_str(revd, BUF_SIZE);
        if ((read(clientfd, revd, BUF_SIZE)) == -1)
        {
            perror("read");
            return false;
        }
        printf("Password Received: \n");
        int index;
        index = validate_student_id(username);
        if (index == -1)
        {
            strcpy(revd, "INVALID USERNAME");
            write(clientfd, revd, strlen(revd));
            return false;
        }
        student_struct student_data;
        if (!read_record(students_fd, &student_data, index, sizeof(student_struct)))
        {
            strcpy(revd, "Some error occurred");
            write(clientfd, revd, strlen(revd));
            return false;
        }
        printf("read password %s\n", student_data.password);
        if (!student_data.status)
        {
            strcpy(revd, "Inactive Student not allowed\n\r");
            write(clientfd, revd, strlen(revd));
            return false;
        }
        if (strcmp(revd, student_data.password) == 0)
        {
            reset_str(revd, BUF_SIZE);
            strcpy(revd, "login success\r\n");
            if (write(clientfd, revd, strlen(revd)) == -1)
            {
                perror("write");
                return false;
            }
            return true;
        }
        else
        {
            strcpy(revd, "Invalid password\n\r");
            write(clientfd, revd, strlen(revd));
            return false;
        }
    }
    else if (revd[0] == 'F' && revd[1] == 'F')
    {
        *user_type = 3;
        printf("username valid, asking for password\n");
        reset_str(revd, BUF_SIZE);
        strcpy(revd, "Password: \r\n");
        if (write(clientfd, revd, strlen(revd)) == -1)
        {
            perror("write");
            return false;
        }
        printf("Asking Password: \n");
        reset_str(revd, BUF_SIZE);
        if ((read(clientfd, revd, BUF_SIZE)) == -1)
        {
            perror("read");
            return false;
        }
        printf("Password Received: \n");
        int index = validate_faculty_id(username);
        if (index == -1)
        {
            strcpy(revd, "INVALID USERNAME");
            write(clientfd, revd, strlen(revd));
            return false;
        }
        faculty_struct faculty_data;
        if (!read_record(faculty_fd, &faculty_data, index, sizeof(faculty_struct)))
        {
            strcpy(revd, "Some error occurred");
            write(clientfd, revd, strlen(revd));
            return false;
        }
        if (strcmp(revd, faculty_data.password) == 0)
        {
            reset_str(revd, BUF_SIZE);
            strcpy(revd, "login success\r\n");
            if (write(clientfd, revd, strlen(revd)) == -1)
            {
                perror("write");
                return false;
            }
            return true;
        }
    }
    else
    {
        strcpy(revd, "WRONG USERNAME");
        write(clientfd, revd, strlen(revd));
        return false;
    }

    return false;
}
bool cin_student(int clientfd, student_struct *student)
{
    // Read data
    char buf[BUF_SIZE];

    if (write_client(clientfd, "Name:\r\n") == -1)
        return false;
    reset_str(buf, BUF_SIZE);
    if (read_client(clientfd, buf) == -1)
        return false;
    strcpy(student->name, buf);
    
    if (write_client(clientfd, "Father Name:\r\n") == -1)
        return false;
    reset_str(buf, BUF_SIZE);
    if (read_client(clientfd, buf) == -1)
        return false;
    strcpy(student->fathername, buf);

    if (write_client(clientfd, "Email:\r\n") == -1)
        return false;
    reset_str(buf, BUF_SIZE);
    if (read_client(clientfd, buf) == -1)
        return false;
    strcpy(student->email, buf);

    if (write_client(clientfd, "Age:\r\n") == -1)
        return false;
    reset_str(buf, BUF_SIZE);
    if (read_client(clientfd, buf) == -1)
        return false;
    student->age = atoi(buf);

    if (write_client(clientfd, "Home Address:\r\n") == -1)
        return false;
    reset_str(buf, BUF_SIZE);
    if (read_client(clientfd, buf) == -1)
        return false;
    strcpy(student->homeaddress, buf);
    
    if (write_client(clientfd, "Current Address:\r\n") == -1)
        return false;
    reset_str(buf, BUF_SIZE);
    if (read_client(clientfd, buf) == -1)
        return false;
    strcpy(student->curaddress, buf);


    for (int i = 0; i < 5; i++)
        reset_str(student->courses_enrolled[i], SMALL_BUF_SIZE);

    student->courses_enrolled_count = 0;

    char username[SMALL_BUF_SIZE];
    snprintf(username, SMALL_BUF_SIZE, "MT%d", student_count);
    strcpy(student->username, username);

    student->status = true;

    return true;
}
bool read_student_id(int clientfd, char *buf, int *student_index)
{
    if (write_client(clientfd, "Give the Student_id\r\n") == -1)
        return false;
    reset_str(buf, BUF_SIZE);
    if (read_client(clientfd, buf) == -1)
        return false;

    if ((*student_index = validate_student_id(buf)) == -1)
    {
        write_client(clientfd, "WRONG student id\n\r");
        return false;
    }
    // printf("Student index2: %d\n", *student_index);
    return true;
}
bool read_faculty_id(int clientfd, char *buf, int *faculty_index)
{
    if (write_client(clientfd, "Give the Faculty_id\r\n") == -1)
        return false;
    reset_str(buf, BUF_SIZE);
    if (read_client(clientfd, buf) == -1)
        return false;

    if ((*faculty_index = validate_faculty_id(buf)) == -1)
    {
        write_client(clientfd, "WRONG Faculty id\r\n");
        return false;
    }
    return true;
}
void write_student(int clientfd, student_struct *student, int index)
{
    // If 'index' is -1, it means a new student is being added.
    if (index == -1)
    {
        strcpy(student->password, "student");
        lseek(students_fd, 0, SEEK_END);
    }
    else
    {
        lseek(students_fd, index * sizeof(student_struct), SEEK_SET);
    }
    printf("Getting write lock for student :\n");
    write_lock(students_fd, index, sizeof(student_struct));
    printf("Got write lock for student :\n");
    write(students_fd, student, sizeof(student_struct));
    unlock_file(students_fd, index, sizeof(student_struct));
    load_details();
    if (index == -1)
    {
        student_count++;
        lseek(details_fd, 0, SEEK_SET);
        printf("Getting write lock for details :\n");
        write_lock(details_fd, 0, sizeof(int));
        printf("Got write lock for details :\n");
        write(details_fd, &student_count, sizeof(student_count));
        unlock_file(details_fd, 0, sizeof(int));
    }
}
bool cin_faculty(int clientfd, faculty_struct *faculty)
{
    char buf[BUF_SIZE];

    if (write_client(clientfd, "Name:\r\n") == -1)
    {
        perror("write");
        return false;
    }
    reset_str(buf, BUF_SIZE);
    if (read_client(clientfd, buf) == -1)
    {
        perror("read");
        return false;
    }
    strcpy(faculty->name, buf);

    if (write_client(clientfd, "Department:\r\n") == -1)
    {
        perror("write");
        return false;
    }
    reset_str(buf, BUF_SIZE);
    if (read_client(clientfd, buf) == -1)
    {
        perror("read");
        return false;
    }
    strcpy(faculty->department, buf);

    if (write_client(clientfd, "Email:\r\n") == -1)
    {
        perror("write");
        return false;
    }
    reset_str(buf, BUF_SIZE);
    if (read_client(clientfd, buf) == -1)
    {
        perror("read");
        return false;
    }
    strcpy(faculty->email, buf);
    
    if (write_client(clientfd, "Contact:\r\n") == -1)
    {
        perror("write");
        return false;
    }
    reset_str(buf, BUF_SIZE);
    if (read_client(clientfd, buf) == -1)
    {
        perror("read");
        return false;
    }
    strcpy(faculty->contact, buf);

    if (write_client(clientfd, "Designation:\r\n") == -1)
    {
        perror("write");
        return false;
    }
    reset_str(buf, BUF_SIZE);
    if (read_client(clientfd, buf) == -1)
    {
        perror("read");
        return false;
    }
    strcpy(faculty->designation, buf);

    if (write_client(clientfd, "Address:\r\n") == -1)
    {
        perror("write");
        return false;
    }
    reset_str(buf, BUF_SIZE);
    if (read_client(clientfd, buf) == -1)
    {
        perror("read");
        return false;
    }
    strcpy(faculty->address, buf);

    char username[SMALL_BUF_SIZE];
    sprintf(username, "FF%d", faculty_count); // Assuming 'faculty_count' is defined.
    strcpy(faculty->username, username);

    for (int i = 0; i < 10; i++)
    {
        reset_str(faculty->courses_offered[i], SMALL_BUF_SIZE);
    }

    faculty->courses_offered_count = 0;
    return true;
}

void write_faculty(int clientfd, faculty_struct *faculty, int index)
{
    if (index == -1)
    {
        strcpy(faculty->password, "faculty");
        lseek(faculty_fd, 0, SEEK_END);
    }
    else
    {
        lseek(faculty_fd, index * sizeof(faculty_struct), SEEK_SET);
    }

    printf("Getting write lock for Faculty:\n");
    write_lock(faculty_fd, index, sizeof(faculty_struct));
    printf("Got write lock for Faculty:\n");

    write(faculty_fd, faculty, sizeof(faculty_struct));
    unlock_file(faculty_fd, index, sizeof(faculty_struct));

    if (index == -1)
    {
        faculty_count++;
        lseek(details_fd, sizeof(int), SEEK_SET);
        printf("Getting write lock for details:\n");
        write_lock(details_fd, 1, sizeof(int));
        printf("Got write lock for details:\n");
        write(details_fd, &faculty_count, sizeof(int));
        unlock_file(details_fd, 1, sizeof(int));
    }
}

bool cin_course(int clientfd, course_struct *course, char *faculty_id)
{
    char buf[BUF_SIZE];

    if (write_client(clientfd, "Name:\r\n") == -1)
        return false;

    reset_str(buf, BUF_SIZE);
    if (read_client(clientfd, buf) == -1)
        return false;
    strcpy(course->name, buf);

    if (write_client(clientfd, "Department:\r\n") == -1)
        return false;
    reset_str(buf, BUF_SIZE);
    if (read_client(clientfd, buf) == -1)
    {
        perror("read");
        return false;
    }
    strcpy(course->department, buf);

    if (write_client(clientfd, "Seats:\r\n") == -1)
    {
        perror("write");
        return false;
    }
    reset_str(buf, BUF_SIZE);
    if (read_client(clientfd, buf) == -1)
    {
        perror("read");
        return false;
    }
    if (is_number(buf))
    {
        course->no_of_seats = atoi(buf);
        course->available_seats = course->no_of_seats;
    }

    if (write_client(clientfd, "Credits:\r\n") == -1)
    {
        perror("write");
        return false;
    }
    reset_str(buf, BUF_SIZE);
    if (read_client(clientfd, buf) == -1)
    {
        perror("read");
        return false;
    }
    if (is_number(buf))
    {
        course->credits = atoi(buf);
    }

    course->status = true;

    strcpy(course->offered_by, faculty_id);

    char course_id[SMALL_BUF_SIZE];
    // strcpy(username, "MT");
    sprintf(course_id, "CC%d", (course_count));
    strcpy(course->course_id, course_id);

    course_count++;

    return true;
}
void write_course(int clientfd, course_struct *course, int index)
{
    if (index == -1)
    {
        lseek(course_fd, 0, SEEK_END);
    }
    else
    {
        lseek(course_fd, index * sizeof(course_struct), SEEK_SET);
    }

    printf("Getting write lock for course:\n");
    write_lock(course_fd, index, sizeof(course_struct));
    printf("Got write lock for course:\n");
    write(course_fd, course, sizeof(course_struct));
    unlock_file(course_fd, index, sizeof(course_struct));
    load_details();

    if (index == -1)
    {
        (course_count)++;
        lseek(details_fd, 2 * sizeof(int), SEEK_SET);

        printf("Getting write lock for details:\n");
        write_lock(details_fd, 2, sizeof(int));
        printf("Got write lock for details:\n");
        write(details_fd, &course_count, sizeof(int));
        unlock_file(details_fd, 2, sizeof(int));
    }
}
bool read_record(int filefd, void *add, int index, size_t size)
{
    printf("index: %d size: %zu\n", index, size);
    printf("lseek: %ld\n", lseek(filefd, index * size, SEEK_SET));
    printf("locking file for reading: %d\n", filefd);
    read_lock(filefd, index, size);
    printf("Got the lock for reading: %d\n", filefd);

    ssize_t bytes_read = read(filefd, add, size);

    if (bytes_read == -1)
    {
        perror("read");
        unlock_file(filefd, index, size);
        return false;
    }

    unlock_file(filefd, index, size);
    return true;
}

void handle_admin(int clientfd, char *username)
{
    char revd[BUF_SIZE], buf[BUF_SIZE];

    while (1)
    {
        load_details();
        

        snprintf(buf, BUF_SIZE, "************Admin*******\n1. Add Student\n2. View Student Detail\n3. Add Faculty\n4. View Faculty Detail\n5. Activate Student\n6. Inactivate Student\n7. Modify Student Detail\n8. Modify Faculty Detail\n9.Press e to logout and exit \n\nStudents: %d\tFaculties: %d\tCourses: %d\r\n", student_count, faculty_count, course_count);

        if (write(clientfd, buf, strlen(buf)) == -1)
        {
            perror("Write");
            return;
        }

        if (read(clientfd, revd, sizeof(revd)) < 1)
        {
            perror("Read");
            return;
        }
        switch (revd[0])
        {
        case '1':
        {
            if (write_client(clientfd, "Adding Student\r\n") == -1)
            {
                return;
            }
            student_struct student;
            cin_student(clientfd, &student);
            write_student(clientfd, &student, -1);
            snprintf(buf, sizeof(buf), "SUCCESSFULL%s\r\n", student.username);
            if (write_client(clientfd, buf) == -1)
            {
                return;
            }
        }
        break;
        case '2':
        {
            int student_index;
            if (!read_student_id(clientfd, buf, &student_index))
                break;
            student_struct *student_data = (student_struct *)malloc(sizeof(student_struct));
            if (!read_record(students_fd, student_data, student_index, sizeof(student_struct)))
            {
                write_client(clientfd, "ERROR reading file");
                break;
            }
            // Generate and send student details.
            char details_msg[BUF_SIZE];
            tostring_student(student_data, details_msg);
            if (write_client(clientfd, details_msg) == -1)
            {
                free(student_data);
                break;
            }
            free(student_data);
            sleep(3);
        }
        break;
        case '3':
        {
            if (write_client(clientfd, "Adding Faculty\r\n") == -1)
            {
                return;
            }
            faculty_struct faculty;
            cin_faculty(clientfd, &faculty);
            write_faculty(clientfd, &faculty, -1);
            snprintf(buf, sizeof(buf), "SUCCESSFULL%s\r\n", faculty.username);
            if (write_client(clientfd, buf) == -1)
            {
                return;
            }
        }
        break;
        case '4':
        {
            int faculty_index;
            if (!read_faculty_id(clientfd, buf, &faculty_index))
                break;
            faculty_struct *faculty_data = (faculty_struct *)malloc(sizeof(faculty_struct));
            if (!read_record(faculty_fd, faculty_data, faculty_index, sizeof(faculty_struct)))
            {
                write_client(clientfd, "ERROR reading file");
                free(faculty_data);
                break;
            }
            // Generate and send faculty details.
            char details_msg[BUF_SIZE];
            tostring_faculty(faculty_data, details_msg);
            if (write_client(clientfd, details_msg) == -1)
            {
                free(faculty_data);
                break;
            }
            free(faculty_data);
            sleep(3);
        }
        break;
        case '5':
        {
            int student_index;
            if (!read_student_id(clientfd, buf, &student_index))
                break;
            student_struct *student_data = (student_struct *)malloc(sizeof(student_struct));
            if (!read_record(students_fd, student_data, student_index, sizeof(student_struct)))
            {
                write_client(clientfd, "ERROR reading file");
                free(student_data);
                break;
            }
            student_data->status = true;
            student_struct student_data_to_write = *student_data;
            write_student(clientfd, &student_data_to_write, student_index);
            if (write_client(clientfd, "Student Activated\r\n") == -1)
            {
                free(student_data);
                break;
            }
            // Generate and send updated student details.
            char details_msg[BUF_SIZE];
            tostring_student(student_data, details_msg);
            if (write_client(clientfd, details_msg) == -1)
            {
                free(student_data);
                break;
            }
            free(student_data);
            sleep(3);
        }
        break;
        case '6':
        {
            int student_index;
            if (!read_student_id(clientfd, buf, &student_index))
                break;
            student_struct *student_data = (student_struct *)malloc(sizeof(student_struct));
            if (!read_record(students_fd, student_data, student_index, sizeof(student_struct)))
            {
                write_client(clientfd, "ERROR reading file");
                free(student_data);
                break;
            }
            student_data->status = false;
            student_struct student_data_to_write = *student_data;
            write_student(clientfd, &student_data_to_write, student_index);
            if (write_client(clientfd, "Student Deactivated\r\n") == -1)
            {
                free(student_data);
                break;
            }
            // Generate and send updated student details.
            char details_msg[BUF_SIZE];
            tostring_student(student_data, details_msg);
            if (write_client(clientfd, details_msg) == -1)
            {
                free(student_data);
                break;
            }
            free(student_data);
            sleep(3);
        }
        break;
        case '7':
        {
            if (write_client(clientfd, "UPDATE Student\r\n") == -1)
            {
                break;
            }
            int student_index;
            if (!read_student_id(clientfd, buf, &student_index))
                break;
            student_struct *student_data = (student_struct *)malloc(sizeof(student_struct));
            if (!read_record(students_fd, student_data, student_index, sizeof(student_struct)))
            {
                write_client(clientfd, "ERROR reading file");
                free(student_data);
                break;
            }
            student_struct student_data_to_write = *student_data;
            // Generate and send current student details.
            char details_msg[BUF_SIZE];
            tostring_student(student_data, details_msg);
            if (write_client(clientfd, details_msg) == -1)
            {
                free(student_data);
                break;
            }
            cin_student(clientfd, &student_data_to_write);
            write_student(clientfd, &student_data_to_write, student_index);
            if (write_client(clientfd, "Student Updated\r\n") == -1)
            {
                free(student_data);
                break;
            }
            free(student_data);
            sleep(3);
        }
        break;
        case '8':
        {
            if (write_client(clientfd, "UPDATE Faculty\r\n") == -1)
            {
                break;
            }
            int faculty_index;
            if (!read_faculty_id(clientfd, buf, &faculty_index))
                break;
            faculty_struct *faculty_data = (faculty_struct *)malloc(sizeof(faculty_struct));
            if (!read_record(faculty_fd, faculty_data, faculty_index, sizeof(faculty_struct)))
            {
                write_client(clientfd, "ERROR reading file");
                free(faculty_data);
                break;
            }
            faculty_struct faculty_data_to_write = *faculty_data;
            // Generate and send current faculty details.
            char details_msg[BUF_SIZE];
            tostring_faculty(faculty_data, details_msg);
            if (write_client(clientfd, details_msg) == -1)
            {
                free(faculty_data);
                break;
            }
            cin_faculty(clientfd, &faculty_data_to_write);
            write_faculty(clientfd, &faculty_data_to_write, faculty_index);
            if (write_client(clientfd, "Faculty Updated\r\n") == -1)
            {
                free(faculty_data);
                break;
            }
            free(faculty_data);
            sleep(3);
        }
        break;
        case 'e':
            return;
        default:
            break;
        }
    }
}
void handle_student(int clientfd, char *username)
{
    char revd[BUF_SIZE], buf[BUF_SIZE];
    student_struct student_data_main;
    int student_index;
    student_index = validate_student_id(username);
    read_record(students_fd, &student_data_main, student_index, sizeof(student_struct));
	int count=0;
    while (1)
    {
        load_details();

        char msg[2 * BUF_SIZE];
        snprintf(msg, sizeof(msg), "****** Welcome Student %s********\n1. Enroll New Course\n2. View Enrolled Course Details\n3.Drop Course \n4. Reset Password\n5. Signout \r\nStudents: %d\tFaculties: %d\tCourses: %d\r\n", student_data_main.name, student_count, faculty_count, course_count);

        if (write(clientfd, msg, strlen(msg)) == -1)
        {
            perror("Write");
            return;
        }

        if (read(clientfd, revd, sizeof(revd)) < 1)
        {
            perror("Read");
            return;
        }

        switch (revd[0])
    {
 
       
        case '1':
        {
            count++;
            if(count>=6)
            {
            	printf("\nAlready registered for max course\n");
            	break;
            }
            char **courses_list;
            courses_list = (char **)malloc((course_count + 3) * sizeof(char *));
            for (int i = 0; i < course_count + 3; i++)
            {
                courses_list[i] = (char *)malloc(BUF_SIZE);
            }

            course_struct course;
            int active_courses = 0;
            snprintf(courses_list[active_courses++], BUF_SIZE, "%s: %s", "Option: ", "Course Name");
            for (int i = 0; i < course_count; i++)
            {
                read_record(course_fd, &course, i, sizeof(course_struct));
                if (course.status)
                {
                    snprintf(courses_list[active_courses++], BUF_SIZE, "%d: %s", i, course.name);
                }
            }
            snprintf(courses_list[active_courses++], BUF_SIZE, "%s", "Enter the Option");
            char *course_list_string;
            course_list_string = tostring_char_array(courses_list, active_courses);
            write_client(clientfd, course_list_string);
            free(course_list_string);
            reset_str(buf, BUF_SIZE);
            read_client(clientfd, buf);
            if (!is_number(buf))
            {
                write_client(clientfd, "Invalid Input\r\n");
                break;
            }
            int course_index = atoi(buf);
            if (course_index >= course_count)
            {
                write_client(clientfd, "Not a valid Course\r\n");
                break;
            }
            course_struct course_data;
            if (!read_record(course_fd, &course_data, course_index, sizeof(course_struct)))
            {
                fprintf(stderr, "Error reading course file\n");
                break;
            }
            tostring_course(&course_data, buf);
            if (write_client(clientfd, buf) == -1)
            {
                break;
            }
            sleep(3);
            fprintf(stdout, "Seats Available: %d\n", course_data.available_seats);
            course_data.available_seats--;
            write_course(clientfd, &course_data, course_index);
            strncpy(student_data_main.courses_enrolled[student_data_main.courses_enrolled_count++], course_data.course_id, SMALL_BUF_SIZE);
            write_student(clientfd, &student_data_main, student_index);
            write_client(clientfd, "Added the Course\r\n");
        }
        break;
        case '3':
        {
            char *courses_opted_string;
            char **courses_opted;
            courses_opted = (char **)malloc(student_data_main.courses_enrolled_count * sizeof(char *));
            for (int i = 0; i < student_data_main.courses_enrolled_count; i++)
            {
                courses_opted[i] = (char *)malloc(SMALL_BUF_SIZE);
                strncpy(courses_opted[i], student_data_main.courses_enrolled[i], SMALL_BUF_SIZE);
            }
            courses_opted_string = indexed_tostring_char_array(courses_opted, student_data_main.courses_enrolled_count);
            write_client(clientfd, courses_opted_string);
            reset_str(buf, BUF_SIZE);
            read_client(clientfd, buf);
            if (!is_number(buf))
            {
                write_client(clientfd, "Invalid Input\r\n");
                break;
            }
            int option = atoi(buf);
            char course_id[SMALL_BUF_SIZE];
            strncpy(course_id, student_data_main.courses_enrolled[option], SMALL_BUF_SIZE);
            for (int i = option; i < student_data_main.courses_enrolled_count - 1; i++)
            {
                strncpy(student_data_main.courses_enrolled[i], student_data_main.courses_enrolled[i + 1], SMALL_BUF_SIZE);
            }
            student_data_main.courses_enrolled_count--;
            write_student(clientfd, &student_data_main, student_index);
            course_struct course_data;
            int course_index = validate_course_id(course_id);
            if (!read_record(course_fd, &course_data, course_index, sizeof(course_struct)))
            {
                fprintf(stderr, "Error reading course file\n");
                break;
            }
            course_data.available_seats++;
            write_course(clientfd, &course_data, course_index);
            write_client(clientfd, "Dropped Course\r\n");
        }
        break;
        case '2':
        {
            char *courses_opted_string;
            char **courses_opted;
            courses_opted = (char **)malloc(student_data_main.courses_enrolled_count * sizeof(char *));
            for (int i = 0; i < student_data_main.courses_enrolled_count; i++)
            {
                courses_opted[i] = (char *)malloc(SMALL_BUF_SIZE);
                strncpy(courses_opted[i], student_data_main.courses_enrolled[i], SMALL_BUF_SIZE);
            }
            courses_opted_string = indexed_tostring_char_array(courses_opted, student_data_main.courses_enrolled_count);
            write_client(clientfd, courses_opted_string);
            reset_str(buf, BUF_SIZE);
            read_client(clientfd, buf);
            if (!is_number(buf))
            {
                write_client(clientfd, "Invalid Input\r\n");
                break;
            }
            int option = atoi(buf);
            int course_index = validate_course_id(student_data_main.courses_enrolled[option]);
            course_struct course_data;
            if (!read_record(course_fd, &course_data, course_index, sizeof(course_struct)))
            {
                fprintf(stderr, "Error reading course file\n");
                break;
            }
            tostring_course(&course_data, buf);
            if (write_client(clientfd, buf) == -1)
            {
                break;
            }
            sleep(3);
        }
        
        break;
        case '4':
        {
            reset_str(buf, BUF_SIZE);
            if (write_client(clientfd, "Enter the new Password\r\n") == -1)
            {
                break;
            }
            read_client(clientfd, buf);
            strncpy(student_data_main.password, buf, SMALL_BUF_SIZE);

            write_student(clientfd, &student_data_main, student_index);
            write_client(clientfd, "Password Updated\r\n");
            tostring_student(&student_data_main, buf);
            write_client(clientfd, buf);
            sleep(3);
        }
        break;
        case '5':
        L1:    return;
        default:
            break;
        }
    }
}

void handle_faculty(int clientfd, char *username)
{
    char revd[BUF_SIZE], buf[BUF_SIZE];
    faculty_struct faculty_data_main;
    int faculty_index;
    faculty_index = validate_faculty_id(username);
    read_record(faculty_fd, &faculty_data_main, faculty_index, sizeof(faculty_struct));

    while (1)
    {
        load_details();

        char msg[2 * BUF_SIZE];
        sprintf(msg, "***********Welcome  Faculty  %s***********\n1.Add Course\n2. View Courses Offered\n3. Remove Course\n4. Update Course Detail\n5. Reset Password\n6. Signout\r\nStudents: %d\tFaculties: %d\tCourses: %d\r\n", faculty_data_main.name, student_count, faculty_count, course_count);

        if (write(clientfd, msg, strlen(msg)) == -1)
        {
            perror("Write");
            return;
        }

        if (read(clientfd, revd, sizeof(revd)) < 1)
        {
            perror("Read");
            return;
        }
        switch (revd[0])
        {
        case '2':
        {
            char courses_string[BUF_SIZE];
            char temp[BUF_SIZE];
            reset_str(courses_string, BUF_SIZE);
            for (int i = 0; i < faculty_data_main.courses_offered_count; i++)
            {
                reset_str(temp, BUF_SIZE);
                sprintf(temp, "%d: %s\n", i, faculty_data_main.courses_offered[i]);
                strcat(courses_string, temp);
            }
            if (write(clientfd, courses_string, strlen(courses_string)) == -1)
            {
                break;
            }
            int course_index_of_course;
            reset_str(buf, BUF_SIZE);
            if (read(clientfd, buf, sizeof(buf)) == -1)
            {
                break;
            }
            if (!is_number(buf))
            {
                if (write(clientfd, "WRONG option\r\n", strlen("WRONG option\r\n")) == -1)
                {
                    break;
                }
                break;
            }
            course_index_of_course = atoi(buf);
            int course_index;
            course_index = validate_course_id(faculty_data_main.courses_offered[course_index_of_course]);
            if (course_index < 0)
            {
                printf("Course error\n");
                break;
            }
            course_struct course_data;
            read_record(course_fd, &course_data, course_index, sizeof(course_struct));
            tostring_course(&course_data, buf);
            if (write(clientfd, buf, strlen(buf)) == -1)
            {
                break;
            }
            sleep(3);
        }
        break;
        case '1':
        {
            if (faculty_data_main.courses_offered_count >= 11)
            {
                if (write(clientfd, "Cannot Add any more courses(Max 10)\r\n", strlen("Cannot Add any more courses(Max 10)\r\n")) == -1)
                {
                    return;
                }
            }
            if (write(clientfd, "Adding a new Course\r\n", strlen("Adding a new Course\r\n")) == -1)
            {
                break;
            }
            course_struct course;
            cin_course(clientfd, &course, username);
            write_course(clientfd, &course, -1);

            faculty_struct *faculty_data = (faculty_struct *)malloc(sizeof(faculty_struct));
            if (!read_record(faculty_fd, faculty_data, faculty_index, sizeof(faculty_struct)))
            {
                if (write(clientfd, "ERROR reading file", strlen("ERROR reading file")) == -1)
                {
                    break;
                }
                break;
            }
            faculty_struct faculty_data_to_write = *faculty_data;
            strcpy(faculty_data_to_write.courses_offered[faculty_data_to_write.courses_offered_count], course.course_id);
            faculty_data_to_write.courses_offered_count++;
            write_faculty(clientfd, &faculty_data_to_write, faculty_index);
            faculty_data_main = faculty_data_to_write;
            free(faculty_data);
            sprintf(buf, "SUCCESSfully added course %s\r\n", course.course_id);
            if (write(clientfd, buf, strlen(buf)) == -1)
            {
                return;
            }
            sleep(3);
        }
        break;
        case '3':
        {
            char courses_string[BUF_SIZE];
            char temp[BUF_SIZE];
            reset_str(courses_string, BUF_SIZE);
            for (int i = 0; i < faculty_data_main.courses_offered_count; i++)
            {
                reset_str(temp, BUF_SIZE);
                sprintf(temp, "%d: %s\n", i, faculty_data_main.courses_offered[i]);
                strcat(courses_string, temp);
            }
            if (write(clientfd, courses_string, strlen(courses_string)) == -1)
            {
                break;
            }
            int course_index_of_course;
            reset_str(buf, BUF_SIZE);
            if (read(clientfd, buf, sizeof(buf)) == -1)
            {
                break;
            }
            if (!is_number(buf))
            {
                if (write(clientfd, "WRONG option\r\n", strlen("WRONG option\r\n")) == -1)
                {
                    break;
                }
                break;
            }
            course_index_of_course = atoi(buf);
            int course_index;
            course_index = validate_course_id(faculty_data_main.courses_offered[course_index_of_course]);
            if (course_index < 0)
            {
                printf("Course error\n");
                break;
            }
            course_struct course_data;
            read_record(course_fd, &course_data, course_index, sizeof(course_struct));
            if (course_data.available_seats != course_data.no_of_seats && course_data.status)
            {
                if (write(clientfd, "There are students opted for this course. Please ask them to drop\r\n", strlen("There are students opted for this course. Please ask them to drop\r\n")) == -1)
                {
                    break;
                }
                break;
            }
            course_data.status = !course_data.status;
            write_course(clientfd, &course_data, course_index);
            reset_str(buf, BUF_SIZE);
            tostring_course(&course_data, buf);
            if (write(clientfd, buf, strlen(buf)) == -1)
            {
                break;
            }
            if (write(clientfd, "Updated Successfully\r\n", strlen("Updated Successfully\r\n")) == -1)
            {
                break;
            }
            sleep(3);
        }
        break;
        case '4':
        {
            char courses_string[BUF_SIZE];
            char temp[BUF_SIZE];
            reset_str(courses_string, BUF_SIZE);
            for (int i = 0; i < faculty_data_main.courses_offered_count; i++)
            {
                reset_str(temp, BUF_SIZE);
                sprintf(temp, "%d: %s\n", i, faculty_data_main.courses_offered[i]);
                strcat(courses_string, temp);
            }
            if (write(clientfd, courses_string, strlen(courses_string)) == -1)
            {
                break;
            }
            int course_index_of_course;
            reset_str(buf, BUF_SIZE);
            if (read(clientfd, buf, sizeof(buf)) == -1)
            {
                break;
            }
            if (!is_number(buf))
            {
                if (write(clientfd, "WRONG option\r\n", strlen("WRONG option\r\n")) == -1)
                {
                    break;
                }
                break;
            }
            course_index_of_course = atoi(buf);
            int course_index;
            course_index = validate_course_id(faculty_data_main.courses_offered[course_index_of_course]);
            if (course_index < 0)
            {
                printf("Course error\n");
                break;
            }
            course_struct course_data;
            read_record(course_fd, &course_data, course_index, sizeof(course_struct));
            bool course_status_bk = course_data.status;
            int available_seats = course_data.available_seats;
            char temp_course_id[SMALL_BUF_SIZE];
            strcpy(temp_course_id, course_data.course_id);
            cin_course(clientfd, &course_data, faculty_data_main.username);
            course_data.status = course_status_bk;
            course_data.available_seats = available_seats;
            strcpy(course_data.course_id, temp_course_id);
            printf("course index: %d\n", course_index);
            write_course(clientfd, &course_data, course_index);
            if (write(clientfd, "Updated Successfully\r\n", strlen("Updated Successfully\r\n")) == -1)
            {
                printf("client write error\n");
                break;
            }
            sleep(3);
        }
        break;
        case '5':
        {
            reset_str(buf, BUF_SIZE);
            if (write(clientfd, "Give the Password\r\n", strlen("Give the Password\r\n")) == -1)
            {
                break;
            }
            if (read(clientfd, buf, sizeof(buf)) == -1)
            {
                break;
            }
            strcpy(faculty_data_main.password, buf);
            write_faculty(clientfd, &faculty_data_main, faculty_index);
            if (write(clientfd, "Password Updated\r\n", strlen("Password Updated\r\n")) == -1)
            {
                break;
            }
            tostring_faculty(&faculty_data_main, buf);
            if (write(clientfd, buf, strlen(buf)) == -1)
            {
                break;
            }
            sleep(3);
        }
        break;
        case '6':
            return;
        default:
            break;
        }
    }
}

int handle_client(int clientfd, struct sockaddr_in client_address)
{
    open_files();
    load_details();
    // sleep(1);

    char username[BUF_SIZE];
    int user_type;
    printf("Handling client\n");
    if (login(clientfd, username, &user_type))
    {
        printf("Login Success: %s%d\n", username, user_type);
        switch (user_type)
        {
        case 1:
            handle_admin(clientfd, username);
            break;
        case 2:
            handle_student(clientfd, username);
            break;
        case 3:
            handle_faculty(clientfd, username);
            break; // Added missing 'break' statement
        default:
            // Handle the default case if needed
            break;
        }
    }
    return 0;
}
