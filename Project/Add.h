#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <fcntl.h>

#include"Database.h"

int AddStudent(struct Student addStudent){
    //implement entire file locking
    //locking struct defined
    struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();
    //locking struct defined
    const char* fileName="Student.txt";
    int fd=open(fileName,O_WRONLY|O_APPEND,0666);
    if(fd==-1){
        printf("Error Adding Student\n");
        return -1;
    }
    //locking start
    fcntl(fd, F_SETLKW, &lock);
    write(fd,&addStudent,sizeof(addStudent));
    lock.l_type=F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
    //locking end
    return 0;
}
int updateStatusStudent(struct Student updateStudent){
    //implement block file locking
    
    struct Student buffStudent;
    const char* fileName="Student.txt";
    int fd=open(fileName,O_WRONLY,0666);
    if(fd==-1){
        printf("Error changing status Student\n");
        return -1;
    }
    while(read(fd,&buffStudent,sizeof(buffStudent))>0){
        if(strcmp(buffStudent.rollno,updateStudent.rollno)==0){
            //locking struct defined
            struct flock lock;
	        lock.l_type = F_WRLCK;
	        lock.l_whence = SEEK_CUR;
	        lock.l_start = 0;
	        lock.l_len = sizeof(buffStudent);
	        lock.l_pid = getpid();
            //locking struct defined
            //locking start
            fcntl(fd, F_SETLKW, &lock);
            lseek(fd,-1*sizeof(buffStudent),SEEK_CUR);
            write(fd,&updateStudent,sizeof(updateStudent));
            lock.l_type=F_UNLCK;
	        fcntl(fd, F_SETLK, &lock);
            //locking end
            return 1;
        }
    }
    return 0;
}

int AddFaculty(struct Faculty addFaculty){
    //implement entire file locking
    //locking struct defined
    struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();
    //locking struct defined
    const char* fileName="Faculty.txt";
    int fd=open(fileName,O_WRONLY|O_APPEND,0666);
    if(fd==-1){
        printf("Error Adding Faculty\n");
        return -1;
    }
    //locking start
    fcntl(fd, F_SETLKW, &lock);
    write(fd,&addFaculty,sizeof(addFaculty));
    lock.l_type=F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
    //locking end
    return 0;
}

int AddCourse(struct Course addCourse){
    //implement entire file locking
    //locking struct defined
    struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();
    //locking struct defined
    const char* fileName="Courses.txt";
    int fd=open(fileName,O_WRONLY|O_APPEND,0666);
    if(fd==-1){
        printf("Error Adding Course\n");
        return -1;
    }
    //locking start
    fcntl(fd, F_SETLKW, &lock);
    write(fd,&addCourse,sizeof(addCourse));
    lock.l_type=F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
    //locking end
    return 0;
}

int removeCourse(const char* removeCourse){
    //implement block file locking
    //locking struct defined
    struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();
    //locking struct defined
    struct Course buffCourse;
    struct Student buffStudent;
    const char* fileName="Courses.txt";
    const char* fileStudentEnroll="StudentsInCourses.txt";
    int fd1=open(fileName,O_RDWR,0666);
    int fd2=open(fileStudentEnroll,O_RDWR,0666);
    if(fd1==-1||fd2==-1){
        printf("Error removing Course\n");
        return -1;
    }
    while(read(fd2,&buffStudent,sizeof(buffStudent))>0){
        if(strcmp(removeCourse,buffStudent.courseEnrolled.course_code)==0){
            buffStudent.courseEnrolled.status=0;
            lseek(fd2,-1*sizeof(buffStudent),SEEK_SET);
            write(fd2,&buffStudent,sizeof(buffStudent));
        }
    }
    while(read(fd1,&buffCourse,sizeof(buffCourse))>0){
        if(strcmp(buffCourse.course_code,removeCourse)==0){
            buffCourse.status=0;
            buffCourse.currentStudentsEnrolled=0;
            //locking start
            fcntl(fd1, F_SETLKW, &lock);
            lseek(fd1,-1*sizeof(buffCourse),SEEK_CUR);
            write(fd1,&removeCourse,sizeof(removeCourse));
            lock.l_type=F_UNLCK;
	        fcntl(fd1, F_SETLK, &lock);
            //LOCKING END
            return 1;
        }
    }
    return 0;
}
int enrollInActiveCourse(const char* enrollInCourse,
                        const char* inpuStudentUID){
    struct Course chkCourse;
    struct Student chkStudent;
    const char* courseFile="Courses.txt";
    const char* studInCourse="StudentsInCourses.txt";
    const char* studentFile="Student.txt";
    int fd1=open(courseFile,O_RDONLY,0666);//read lock
    int fd2=open(studInCourse,O_RDWR|O_CREAT|O_APPEND,0666);//entire file lock
    int fd3=open(studentFile,O_RDONLY,0666);//read lock
    if(fd1==-1||fd2==-1||fd3==-1){
        printf("Error accessing  Databases\n");
        return -1;
    }
    while(read(fd1,&chkCourse,sizeof(chkCourse))>0){
        if(strcmp(chkCourse.course_code,enrollInCourse)==0){
            if(chkCourse.currentStudentsEnrolled<=chkCourse.maxStudentsAllowed){
                chkCourse.currentStudentsEnrolled++;
                lseek(fd1,-1*sizeof(chkCourse),SEEK_CUR);
                write(fd1,&chkCourse,sizeof(chkCourse));
                while(read(fd3,&chkStudent,sizeof(chkStudent))>0){
                    if(strcmp(chkStudent.rollno,inpuStudentUID)==0){
                        strcpy(chkStudent.courseEnrolled.course_code,chkCourse.course_code);
                        strcpy(chkStudent.courseEnrolled.course_name,chkCourse.course_name);
                        strcpy(chkStudent.courseEnrolled.facultyUID,chkCourse.facultyUID);
                        chkStudent.courseEnrolled.credits=chkCourse.credits;
                        chkStudent.courseEnrolled.currentStudentsEnrolled=chkCourse.currentStudentsEnrolled;
                        chkStudent.courseEnrolled.maxStudentsAllowed=chkCourse.maxStudentsAllowed;
                        chkStudent.courseEnrolled.studentIsEnrolled=1;
                        chkStudent.courseEnrolled.status=chkCourse.status;
                        //implement here
                        //locking struct defined
                        struct flock lock;
	                    lock.l_type = F_WRLCK;
	                    lock.l_whence = SEEK_SET;
	                    lock.l_start = 0;
	                    lock.l_len = 0;
	                    lock.l_pid = getpid();
                        //locking struct defined
                        //locking start
                        write(fd2,&chkStudent,sizeof(chkStudent));
                        lock.l_type=F_UNLCK;
	                    fcntl(fd2, F_SETLK, &lock);
                        //locking end
                        printf("course has been added successfully\n");
                        return 1;
                    }
                }
            }
        }
    }    
            
    return 0;
}
int unenrollInActiveCourse(const char* unenrollInCourse,
                        const char* inpuStudentUID){
    struct Course chkCourse;
    struct Student chkStudent;
    const char* studInCourse="StudentsInCourses.txt";//partial part of file locking
    const char* coursesFile="Courses.txt";//partial part of file locking
    
    int fd1=open(studInCourse,O_RDWR,0666);
    int fd2=open(coursesFile,O_RDWR,0666);
    if(fd1==-1||fd2==-1){
        printf("error accessing databases\n");
    }
    while(read(fd1,&chkStudent,sizeof(chkStudent))>0){
        if(strcmp(chkStudent.rollno,inpuStudentUID)==0 &&
            strcmp(chkStudent.courseEnrolled.course_code,unenrollInCourse)==0 &&
            chkStudent.courseEnrolled.studentIsEnrolled==1){
                chkStudent.courseEnrolled.studentIsEnrolled=0;
                chkStudent.courseEnrolled.currentStudentsEnrolled--;
                //locking structure
                struct flock lock1;
	            lock1.l_type = F_WRLCK;
	            lock1.l_whence = SEEK_CUR;
	            lock1.l_start = 0;
	            lock1.l_len = sizeof(chkStudent);
	            lock1.l_pid = getpid();
                //locking structure
                //locking start
                fcntl(fd1, F_SETLKW, &lock1);
                lseek(fd1,-1*sizeof(chkStudent),SEEK_CUR);
                write(fd1,&chkStudent,sizeof(chkStudent));
                lock1.l_type=F_UNLCK;
	            fcntl(fd1, F_SETLK, &lock1);
                //locking end
                printf("student has been unenrolled\n");
                while(read(fd2,&chkCourse,sizeof(chkCourse))){
                    if(strcmp(chkCourse.course_code,unenrollInCourse)==0){
                        if(chkCourse.currentStudentsEnrolled>0){
                            chkCourse.currentStudentsEnrolled--;
                            //locking structure
                            struct flock lock2;
	                        lock2.l_type = F_WRLCK;
	                        lock2.l_whence = SEEK_CUR;
	                        lock2.l_start = 0;
	                        lock2.l_len = sizeof(chkStudent);
	                        lock2.l_pid = getpid();
                            //locking structure
                            //locking start
                            fcntl(fd2, F_SETLKW, &lock2);
                            lseek(fd2,-1*sizeof(chkCourse),SEEK_CUR);
                            write(fd2,&chkCourse,sizeof(chkCourse));
                            lock2.l_type=F_UNLCK;
	                        fcntl(fd2, F_SETLK, &lock2);
                            //locking end
                            printf("course count decremented\n");
                            return 1;
                        }
                    }
                }
            }
        }
    return 0;
}
int viewEnrolledCourses(const char* inputStudentUID, char* buffs){
    //printf("view enrolled called\n");
    //implement read lock here
    struct Student displayEnrolled;
    const char* inputFile="StudentsInCourses.txt";
    int fd=open(inputFile,O_RDONLY,0666);
    if(fd==-1){
        printf("Unable to access database\n");
        return -1;
    }
    while(read(fd,&displayEnrolled,sizeof(displayEnrolled))>0){
        if(strcmp(inputStudentUID,displayEnrolled.rollno)==0 &&
            displayEnrolled.courseEnrolled.studentIsEnrolled==1 &&
            displayEnrolled.courseEnrolled.status==1){
                strcat(buffs,"\n");
                strcat(buffs,"CourseCode:   ");
                strcat(buffs, displayEnrolled.courseEnrolled.course_code);
                strcat(buffs,"\n");
                strcat(buffs, "Course Name:   ");
                strcat(buffs, displayEnrolled.courseEnrolled.course_name);
                strcat(buffs,"\n");
            }
    }
    return 1;
}