//  Mai_BasicEducationPortal
//  Created by MATSUHISA MAI on 2017/11/15.
//  Copyright © 2017年 MATSUHISA MAI. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CoursesFile "Courses.txt"
#define AccountsFile "Accounts.txt"
#define StudentsFile "Students.txt"
#define StudentsCoursesFile "StudentsCourses.txt"

//関数の宣言
int userLogin(char userID[],char password[]);
void menuOperator(char userID[]);
void getCertificate(char studentID[],char name[],char gender[],int grade,char address[],int admission_year,int amountOfCouses);
void getCourses(char courses[]);
void getStudentMark(char courses[],char studentID[]);
double getGPA(char userID[]);
int getRanking(char userID[]);
//〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜
char firstChar(char* line);
int strLength(char* line);
char* substring(char* line, int startIndex, int endIndex);
int getTheStartIndex(char* filename, char firstChar);
char** splitString(char* a_str, const char a_delim, int* numberOfComponents);
char** readFile(char* fileAddress, int* numberOfLines);
int numberOfItemsPerRecordInFile(char* fileName);
int convertToNumber(char *);

//要素の宣言
struct Student {
    char* studentID;
    char* name;
    char* gender;
    int grade;
    char* address;
    int admission_year;
    char** courses;
    int numberOfCourses;
};
struct Account {
    char* userID;
    char* password;
};
struct Course {
    char* courseID;
    char* name;
};
struct StudentCourse{
    char* studentID;
    char* courseID;
    int mark;
};

//Array 要素のオブジェクト
struct Student* listOfStudents;
struct Account* listOfAccounts;
struct Course* listOfCourses;
struct StudentCourse* listOfStudentCourses;

//★一つ分の要素を生成する時
struct Student createAStudent(char* studentID, char* name, char* gender, int grade, char* address, int admission_year, char* courses[], int numberOfCourses);
struct Account createAnAccount(char *userID,char *password);
struct Course createACourse(char* courseID,char* name);
struct StudentCourse createAStudentCourse(char* studentID, char* courseID, int mark);

// ★リスト(listOf○○○○○)の中に情報を詰める時
struct Student* getListOfStudentFromFile(char* fileAddress, char* fileName, int* numberOfStudents);
struct Account* getListOfAccountsFromFile(char* fileAddress, char* fileName, int* numberOfAccounts);
struct Course* getListOfCoursesFromFile(char* fileAddress, char* fileName, int* numberOfCourses);
struct StudentCourse* getListOfStudentCoursesFromFile(char* fileAddress, char* fileName, int* numberOfStudentsCourses);

int numberOfStudent = 0;
int numberOfAccount = 0;
int numberOfCourses = 0;
int numberOfStudentsCourses = 0;
//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★

int main(void) {
    //★ファイル名
    char* studentsFileName = StudentsFile;
    char* accountsFileName = AccountsFile;
    char* coursesFileName = CoursesFile;
    char* studentsCoursesFileName = StudentsCoursesFile;
    
    //★ファイル参照先
    char* studentsfileAddress = "/Users/mai/DropboxM/xcode/Mai_BasicEducationPortal/Mai_BasicEducationPortal/Students.txt";
//    int numberOfStudent = 0;
    char* accountsfileAddress = "/Users/mai/DropboxM/xcode/Mai_BasicEducationPortal/Mai_BasicEducationPortal/Accounts.txt";
//    int numberOfAccount = 0;
    char* coursesfileAddress = "/Users/mai/DropboxM/xcode/Mai_BasicEducationPortal/Mai_BasicEducationPortal/Courses.txt";
//    int numberOfCourses = 0;
    char* studentsCoursesfileAddress = "/Users/mai/DropboxM/xcode/Mai_BasicEducationPortal/Mai_BasicEducationPortal/StudentsCourses.txt";
//    int numberOfStudentsCourses = 0;
    
    //★オブジェクトに情報が詰まってる状態
    listOfStudents = getListOfStudentFromFile(studentsfileAddress,studentsFileName, &numberOfStudent);
    listOfAccounts = getListOfAccountsFromFile(accountsfileAddress, accountsFileName, &numberOfAccount);
    listOfCourses = getListOfCoursesFromFile(coursesfileAddress, coursesFileName, &numberOfCourses);
    listOfStudentCourses = getListOfStudentCoursesFromFile(studentsCoursesfileAddress, studentsCoursesFileName,&numberOfStudentsCourses);
    
    int flag;//選択フラグとscanfのエラーチェックフラグ
    char str_userID[256];
    char str_password[256]; //ユーザ名とパスワードの一時変数
    printf("************************************************************\n");
    printf("Please enter your account to login:\n");
    printf("************************************************************\n");
    printf("Type your user ID\n: ");
    scanf("%s",str_userID); //scanfで入力、%Sでストリング
    printf("Type your password\n: ");
    scanf("%s",str_password);
    
    /* 入力されたユーザー名とパスワードをlogin関数に渡す */
    flag = userLogin(str_userID,str_password);
    if(flag==1){
        printf("\n★★★Welcome to Cornerstone International College of Canada★★★\n\n");
        menuOperator(str_userID);
    }
    else{ printf("Your account does not exist. Please try again!\n");
        main();
    }
    
    return 0;
}

//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
//メイン画面

void menuOperator(char userID[]){
    int selectNumber = 0;
    double gpa = 0.0;
    int ranking = 0;
    char answer[10];
    
    printf("************************************************************\n");
    printf("Select from the options:\n\n");
    printf("—-[1] Print my enrolment certificate\n—-[2] Print my courses\n—-[3] Print my transcript\n—-[4] Print my GPA\n—-[5] Print my ranking among all students in the college\n—-[6] List all available courses\n—-[7] List all students\n-—[8] Logout\n-—[9] Exit\n\n");
    printf("************************************************************\n");
    printf("Enter the number corresponding to each item to proceed:\n");
    scanf("%d",&selectNumber);
    
    
    switch (selectNumber) {
        case 1:
            for(int count=0; count<numberOfStudent; count++){
                if(strcmp(userID, listOfStudents[count].studentID)==0){
                getCertificate(listOfStudents[count].studentID,listOfStudents[count].name, listOfStudents[count].gender, listOfStudents[count].grade, listOfStudents[count].address, listOfStudents[count].admission_year, listOfStudents[count].numberOfCourses);
                }
            }//certificate
            printf("----------------------------\n");
            puts("continue? type [Y]\n");
            scanf("%s",answer);
            if(strcmp(answer,"Y")==0){
                menuOperator(userID);
            }
            break;
        case 2:
            for(int count=0; count<numberOfStudent;count++){
                if(strcmp(userID, listOfStudents[count].studentID)==0){
                    printf("\nHi %s",listOfStudents[count].gender);
                    printf(" %s\n",listOfStudents[count].name);
                    printf("You have taken the following courses\n");
                    printf("-----------------------------------------");
                    for(int j=0; j<listOfStudents[count].numberOfCourses; j++){
                        printf("\n%d",j+1);
                        getCourses(listOfStudents[count].courses[j]);
                    }//j
                    printf("\n-----------------------------------------\n");
                }
            }//courses
            puts("continue? type [Y]\n");
            scanf("%s",answer);
            if(strcmp(answer,"Y")==0){
                menuOperator(userID);
            }
            break;
        case 3:
            for(int count=0; count<numberOfStudent;count++){
                if(strcmp(userID, listOfStudents[count].studentID)==0){
                    printf("\nHi %s",listOfStudents[count].gender);
                    printf(" %s\n",listOfStudents[count].name);
                    printf("Here is your transcript:\n");
                    printf("-----------------------------------------\n");
                    for(int j=0; j<listOfStudents[count].numberOfCourses; j++){
                        printf("%d",j+1);
                        getCourses(listOfStudents[count].courses[j]);
                        getStudentMark(listOfStudents[count].courses[j],listOfStudents[count].studentID);
                    }//j
                    printf("-----------------------------------------\n");
                }
            }//transcript
            puts("continue? type [Y]\n");
            scanf("%s",answer);
            if(strcmp(answer,"Y")==0){
                menuOperator(userID);
            }
            break;
        case 4:
            for(int count=0; count<numberOfStudent;count++){
                if(strcmp(userID, listOfStudents[count].studentID)==0){
                    printf("Hi %s",listOfStudents[count].gender);
                    printf(" %s\n",listOfStudents[count].name);
                    printf("----------------------------\n");
                    printf("Your GPA is: ");
                    gpa = getGPA(userID);
                    printf("%.2f",gpa);
                    printf("\n-----------------------------------------\n");
                }
            }//gpa
            puts("continue? type [Y]\n");
            scanf("%s",answer);
            if(strcmp(answer,"Y")==0){
                menuOperator(userID);
            }
            break;
        case 5:
            for(int count=0; count<numberOfStudent;count++){
                if(strcmp(userID, listOfStudents[count].studentID)==0){
                    printf("\nHi %s",listOfStudents[count].gender);
                    printf(" %s\n",listOfStudents[count].name);
                    printf("----------------------------\n");
                    printf("Your GPA is: ");
                    gpa = getGPA(userID);
                    printf("%.2f\n",gpa);
                    ranking = getRanking(userID);
                    printf("%d\n",ranking);
                }
            }
            printf("-----------------------------------------\n");
            //ranking
            puts("continue? type [Y]\n");
            scanf("%s",answer);
            if(strcmp(answer,"Y")==0){
                menuOperator(userID);
            }
            break;
        case 6:
            printf("\nThe following courses are offered in CICCC:\n");
            printf("----------------------------\n");
            for(int i=0; i<numberOfCourses; i++){
                printf("%d",i+1);
                printf(") %s", listOfCourses[i].courseID);
                printf(": %s\n", listOfCourses[i].name);
            }//list all available courses
            printf("----------------------------\n");
            puts("continue? type [Y]\n");
            scanf("%s",answer);
            if(strcmp(answer,"Y")==0){
                menuOperator(userID);
            }
            break;
        case 7:
            printf("\nThere are %d students in CICCC as following: \n",numberOfStudent);
            printf("----------------------------\n");
            for(int ss=0; ss<numberOfStudent; ss++){
                printf("%s", listOfStudents[ss].name);
                printf(" : %s\n", listOfStudents[ss].studentID);
            }//list all students
            printf("----------------------------\n");
            puts("continue? type [Y]\n");
            scanf("%s",answer);
            if(strcmp(answer,"Y")==0){
                menuOperator(userID);
            }
            break;
        case 8:
            puts("\n\n");
            main();
        case 9:
            puts("BYE!");
            break;
        default:
            puts("type number [1] to [9]\n");
            main();
            break;
    }
}
//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★

//ログイン画面
int userLogin(char typedUserID[],char typedPassword[]){
    int flag=0,no=-1,no2=-1;
    
    //★★★ユーザ名取得★★★
    for(int count=0; count<numberOfAccount; count++){ //アカウントリスト全体をループ
        if(strcmp(typedUserID,listOfAccounts[count].userID)==0){
            no=count; //何番目のuserIDかをNo.に知らせる(-1からスタートするので user1はNo.0となる)
            break;
        }
    }
    if(no==-1){
        puts("wrong userID");
    }else{
        puts("correct userID");
    }
    //★★★パスワード確認★★★
    for(int count_p=0; count_p<numberOfAccount; count_p++){
        if(strcmp(typedPassword,listOfAccounts[count_p].password)==0){
            no2=count_p;
            break;
        }
    }
    if(no2!=no){
        puts("miss matched between userID and password");
        flag=0;
    }else{
        flag=1;//パスワードが一致したらflagを1に変更される
        puts("correct password");
    }
    return flag;
}
//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★

void getCertificate(char studentID[],char name[],char gender[],int grade,char address[],int admission_year,int amountOfCouses){
    printf("----------------------------\n");
    printf("Dear Sir/Madam,\n");
    printf("This is to certify that %s", gender);
    printf(" %s",name);
    printf(" with student id %s",studentID);
    printf(" is a student at grade %d",grade);
    printf(" at CICCC.\n");
    printf("He was admitted to our college in %d ",admission_year);
    printf("and has taken %d ",amountOfCouses);
    printf("course(s). Currently he resides at %s \n",address);
    printf("\nIf you have any question, please don’t hesitate to contact us.\nThanks,\nWilliams,\n");
}
//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
void getCourses(char courses[]){
    for(int i=0;i<numberOfCourses;i++){
        if(strcmp(courses,listOfCourses[i].courseID)==0){
            printf(") %s : ",listOfCourses[i].courseID);
            printf("%s",listOfCourses[i].name);
        }
    }
}
//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
void getStudentMark(char courses[],char studentID[]){
    for(int i=0;i<numberOfStudentsCourses;i++){
        if(strcmp(studentID,listOfStudentCourses[i].studentID)==0){
            if(strcmp(courses,listOfStudentCourses[i].courseID)==0){
                printf(" : %d\n",listOfStudentCourses[i].mark);
            }
        }
    }
}
//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
double getGPA(char userID[]){
    double gpa = 0.0;
    int sum = 0;
    int count = 0;
    
    for(int i=0;i<numberOfStudentsCourses;i++){
        if(strcmp(userID,listOfStudentCourses[i].studentID)==0){
            sum += listOfStudentCourses[i].mark;
            count ++;
        }
    }
    
    gpa =  (double) sum /  (double) count;
    return gpa;
}
//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
int getRanking(char userID[]){
    int sum = 0;
    int students=0;
    int studentScore[numberOfStudent];
    int scoreTable[500]={0};
    int ranking=1; //最初は全員1位
    int nextRank = 0;
    
    //学生の数だけ回す
    for(students=0;students<numberOfStudent;students++){
        //マークとった科目数ぶん回す
        for(int i=0;i<numberOfStudentsCourses;i++){
            //学生IDとマーク持ってる学生IDが一致
            if(strcmp(listOfStudents[students].studentID,listOfStudentCourses[i].studentID)==0){
                sum += listOfStudentCourses[i].mark;
            }
        }
        studentScore[students] = sum; //学生一人の持ってる合計スコア
        scoreTable[studentScore[students]]++; //成績表の順に入れていく
        sum =0; // リセット
    }
    
    //スコアMAX値から引いていく
    for(sum=500;sum>=0;sum--){
       if(scoreTable[sum]>0){
        nextRank = ranking + scoreTable[sum];
        scoreTable[sum] = ranking;
        ranking = nextRank;
        }
    }
    
    printf("and therefore your rank is: ");
    for(students=0;students<numberOfStudent;students++){
//        printf("%3d点 ",studentScore[students]);
//        printf("%3d番",scoreTable[studentScore[students]]);
        if(strcmp(userID,listOfStudents[students].studentID)==0){
            ranking = scoreTable[studentScore[students]];
        }
    }
    return ranking;
}
//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
int getTheStartIndex(char* filename, char firstChar){
    if(strcmp(filename, AccountsFile)==0 && firstChar=='1')  //1User:”
    {return 7;}
    else if(strcmp(filename, AccountsFile)==0 && firstChar=='2') //2Pass:”
    {return 7;}
    if(strcmp(filename, StudentsFile)==0 && firstChar=='1')  //1studentID:”
    {return 12;}
    else if(strcmp(filename, StudentsFile)==0 && firstChar=='2') //2name:”
    {return 7;}
    else if(strcmp(filename, StudentsFile)==0 && firstChar=='3') //3gender:”
    {return 9;}
    else if(strcmp(filename, StudentsFile)==0 && firstChar=='4') //4grade:”
    {return 8;}
    else if(strcmp(filename, StudentsFile)==0 && firstChar=='5') //5address:“
    {return 10;}
    else if(strcmp(filename, StudentsFile)==0 && firstChar=='6') //6admission_year:”
    {return 17;}
    else if(strcmp(filename, StudentsFile)==0 && firstChar=='7') //7courses:”
    {return 10;}
    else if(strcmp(filename, CoursesFile)==0 && firstChar=='1') //courseID
    {return 11;}//
    else if(strcmp(filename, CoursesFile)==0 && firstChar=='2') //name
    {return 7;}
    else if(strcmp(filename, StudentsCoursesFile)==0 && firstChar=='1') //1studentID:"
    {return 12;}
    else if(strcmp(filename, StudentsCoursesFile)==0 && firstChar=='2') //2courseID:"
    {return 11;}
    else if(strcmp(filename, StudentsCoursesFile)==0 && firstChar=='3') //3mark:"
    {return 7;}
    else{return 0;}
}

//This method gives the length of the string. [You don't need to know how it deos it exactly]
int strLength(char* line){
    char c = '\n';
    int index = 0;
    int numberOfDoubleQuotes = 0;
    while(numberOfDoubleQuotes<2){
        c = *(line+index);
        index++;
    if(c == '"'){
        numberOfDoubleQuotes++;}
        }
    return index;
}

//This method gives the first character of a string. [You don't need to know how it deos it exactly]
char firstChar(char* line){
    return *line;}

//This method gives the substring of a string given the start and end index. [You don't need to know how it deos it exactly]
char* substring(char* line, int startIndex, int endIndex){
    int len = (endIndex-startIndex)+1;
    char* substring;
    //substring = (char*) malloc((len+1)*sizeof(char));
    substring = (char*) malloc((len+1)*sizeof(char));
    for(int i=0; i<=len; i++){
        substring[i] = '\0';
    }
    for(int i=0; i<len; i++){
        substring[i] = *(line+startIndex+i);
        substring[i] = line[startIndex+i];
        //*(substring+i) = *(line+startIndex+i);
    }
    substring[len+1] = '\0';
    return substring;
}

//This method splits the string and returns its components based on the delimeter. [You don't need to know how it deos it exactly]
char** splitString(char* a_str, const char a_delim, int* numberOfComponents){
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;
    
    /* Count how many elements will be extracted. */
    /* Count how many elements will be extracted. */
    while (*tmp){
        if (a_delim == *tmp){
            count++;
            last_comma = tmp;
        }
        tmp++;}
    
    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);
    *numberOfComponents = (int)count;
    
    /* Add space for terminating null string so caller
     knows where the list of returned strings ends. */
    count++;
    result = malloc(sizeof(char*) * count);
    char* components[count];
    for(int i=0; i<count; i++){
        components[i] = '\0';}
    int counter = 0;
    if (result){
        size_t idx  = 0;
        char* token = strtok(a_str, delim);
        while (token){
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
            counter++;
            components[counter] = token;}
        *(result + idx) = 0;}
    return result;
}

//This method reads the file given the address. [You don't need to know how it deos it exactly]
char** readFile(char* fileAddress, int* numberOfLines){
    int lines_allocated = 128;
    int max_line_len = 100;
    /* Allocate lines of text */
    char **words = (char **)malloc(sizeof(char*)*lines_allocated);
    if (words==NULL){
        fprintf(stderr,"Out of memory (1).\n");
        exit(1);}
    FILE *fp = fopen(fileAddress, "r");
    if (fp == NULL){
        fprintf(stderr,"Error opening file.\n");
        exit(2);}
    int i;
    for (i=0;1;i++){
        int j;
        /* Have we gone over our line allocation? */
        if (i >= lines_allocated){
            int new_size;
            /* Double our allocation and re-allocate */
            new_size = lines_allocated*2;
            words = (char **)realloc(words,sizeof(char*)*new_size);
            if (words==NULL){
                fprintf(stderr,"Out of memory.\n");
                exit(3);}
            lines_allocated = new_size;
        }
        /* Allocate space for the next line */
        words[i] = malloc(max_line_len);
        if (words[i]==NULL){
            fprintf(stderr,"Out of memory (3).\n");
            exit(4);}
        if (fgets(words[i],max_line_len-1,fp)==NULL)
            break;
        /* Get rid of CR or LF at end of line */
        for (j=(int)(strlen(words[i]))-1;j>=0 && (words[i][j]=='\n' || words[i][j]=='\r');j--);
        words[i][j+1]='\0';}
    /* Close file */
    fclose(fp);
    (*numberOfLines) = i;
    return words;
}

//This method converts a string to a number. [You don't need to know how it deos it exactly]
int convertToNumber(char * string){
    int number = atoi( string );
    return number;
}

//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
//それぞれのファイルで何項目あるかを引き出す関数
int numberOfItemsPerRecordInFile(char* fileName){
    if(strcmp(fileName, CoursesFile)==0){
        return 2;}
    else if(strcmp(fileName, AccountsFile)==0){
        return 2;}
    else if(strcmp(fileName, StudentsFile)==0){
        return 7;}
    else if(strcmp(fileName, StudentsCoursesFile)==0){
        return 3;}
    else{
        return 1;}
}
//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
//一つ分の要素を生成する時
struct Student createAStudent(char* studentID, char* name, char* gender, int grade, char* address, int admission_year, char* courses[], int numberOfCourses){
    struct Student* student = (struct Student*)malloc(sizeof(struct Student));
    (*student).studentID = studentID;
    (*student).name = name;
    (*student).gender = gender;
    (*student).grade = grade;
    (*student).address = address;
    (*student).admission_year = admission_year;
    (*student).courses = courses;
    (*student).numberOfCourses = numberOfCourses;
    return *student;
}
struct Account createAnAccount(char *userID,char *password){
    struct Account* account = (struct Account*)malloc(sizeof(struct Account));
    (*account).userID = userID;
    (*account).password = password;
    return *account;
}
struct Course createACourse(char* courseID,char* name){
    struct Course* course = (struct Course*)malloc(sizeof(struct Course));
    (*course).courseID = courseID;
    (*course).name = name;
    return *course;
}
struct StudentCourse createAStudentCourse(char* studentID, char* courseID, int mark){
    struct StudentCourse* studentCourse = (struct StudentCourse*)malloc(sizeof(struct StudentCourse));
    (*studentCourse).studentID = studentID;
    (*studentCourse).courseID = courseID;
    (*studentCourse).mark = mark;
    return *studentCourse;
}
//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
// ★リスト(listOfStudents)の中に情報を詰める
//This is what you need to learn and repeat for other entities. Thie method reads the students in the file of students and return an array of students.
struct Student* getListOfStudentFromFile(char* fileAddress, char* fileName, int* numberOfStudents){
    int numberOfStudentsReadFromFileSoFar = 0;
    int numberOfLinesInTheFile = 0;
    char** lines = readFile(fileAddress, &numberOfLinesInTheFile);
    int numberOfLinesPerRecord = numberOfItemsPerRecordInFile(fileName);
    int numberOfRecords = numberOfLinesInTheFile / numberOfLinesPerRecord;
    struct Student* listOfStudents = (struct Student*) malloc(numberOfRecords*sizeof(struct Student));
    for(int k=0; k<numberOfRecords; k++){
        char* studentID = NULL;
        char* name = NULL;
        char* gender = NULL;
        int grade = 0;
        char* address = NULL;
        int admission_year = 0;
        char** courses= NULL;
        int numberOfCourses = 0;
        for(int i=0; i<numberOfLinesPerRecord; i++){
            int indexOfLine = k*numberOfLinesPerRecord+i;
            if(lines[indexOfLine]!=NULL){
                //printf("\n%s\n", lines[indexOfLine]);
                char firstCharacter = firstChar(lines[indexOfLine]);
                int lineLength = strLength(lines[indexOfLine]);
                int startIndex = getTheStartIndex(fileName, firstChar(lines[indexOfLine]));
                
                if(firstCharacter=='1'){
                    studentID = substring(lines[indexOfLine], startIndex, lineLength-2);}
                else if(firstCharacter=='2'){
                    name = substring(lines[indexOfLine], startIndex, lineLength-2);}
                else if(firstCharacter=='3'){
                    gender = substring(lines[indexOfLine], startIndex, lineLength-2);}
                else if(firstCharacter=='4'){
                    char* gradeStr = substring(lines[indexOfLine], startIndex, lineLength-2);
                    grade = convertToNumber(gradeStr);}
                else if(firstCharacter=='5'){
                    address = substring(lines[indexOfLine], startIndex, lineLength-2);}
                else if(firstCharacter=='6'){
                    char* admissionYearStr = substring(lines[indexOfLine], startIndex, lineLength-2);
                    admission_year = convertToNumber(admissionYearStr);}
                else if(firstCharacter=='7'){
                    int numberOfComponents = 0;
                    char* coursesStr = substring(lines[indexOfLine], startIndex, lineLength-2);
                    courses = splitString(coursesStr, ',', &numberOfComponents);
                    numberOfCourses = numberOfComponents;}
            }//end of if
        }//end of second for
        struct Student student= createAStudent(studentID, name, gender, grade, address, admission_year, courses, numberOfCourses);
        listOfStudents[numberOfStudentsReadFromFileSoFar] = student;
        numberOfStudentsReadFromFileSoFar++;
    }//end of first for
    *numberOfStudents = numberOfStudentsReadFromFileSoFar;
    return listOfStudents;
}

struct Account* getListOfAccountsFromFile(char* fileAddress, char* fileName, int* numberOfAccounts){
    int numberOfAccountsReadFromFileSoFar = 0;
    int numberOfLinesInTheFile = 0;
    char** lines = readFile(fileAddress, &numberOfLinesInTheFile);
    int numberOfLinesPerRecord = numberOfItemsPerRecordInFile(fileName);
    int numberOfRecords = numberOfLinesInTheFile / numberOfLinesPerRecord;
    struct Account* listOfAccounts = (struct Account*) malloc(numberOfRecords*sizeof(struct Account));
    for(int k=0; k<numberOfRecords; k++){
        char* userID = NULL;
        char* password = NULL;
        for(int i=0; i<numberOfLinesPerRecord; i++){
            int indexOfLine = k*numberOfLinesPerRecord+i;
            if(lines[indexOfLine]!=NULL){
                //printf("\n%s\n", lines[indexOfLine]);
                char firstCharacter = firstChar(lines[indexOfLine]);
                int lineLength = strLength(lines[indexOfLine]);
                int startIndex = getTheStartIndex(fileName, firstChar(lines[indexOfLine]));
                if(firstCharacter=='1'){
                    userID = substring(lines[indexOfLine], startIndex, lineLength-2);}
                else if(firstCharacter=='2'){
                    password = substring(lines[indexOfLine], startIndex, lineLength-2);}
            }//end of if
        }//end of second for
        struct Account account= createAnAccount(userID,password);
        listOfAccounts[numberOfAccountsReadFromFileSoFar] = account;
        numberOfAccountsReadFromFileSoFar++;
    }//end of first for
    *numberOfAccounts = numberOfAccountsReadFromFileSoFar;
    return listOfAccounts;
}

struct Course* getListOfCoursesFromFile(char* fileAddress, char* fileName, int* numberOfCourses){
    int numberOfCoursesReadFromFileSoFar = 0;
    int numberOfLinesInTheFile = 0;
    char** lines = readFile(fileAddress, &numberOfLinesInTheFile);
    int numberOfLinesPerRecord = numberOfItemsPerRecordInFile(fileName);
    int numberOfRecords = numberOfLinesInTheFile / numberOfLinesPerRecord;
    struct Course* listOfCourses = (struct Course*) malloc(numberOfRecords*sizeof(struct Course));
    for(int k=0; k<numberOfRecords; k++){
        char* courseID = NULL;
        char* name = NULL;
        for(int i=0; i<numberOfLinesPerRecord; i++){
            int indexOfLine = k*numberOfLinesPerRecord+i;
            if(lines[indexOfLine]!=NULL){
                //printf("\n%s\n", lines[indexOfLine]);
                char firstCharacter = firstChar(lines[indexOfLine]);
                int lineLength = strLength(lines[indexOfLine]);
                int startIndex = getTheStartIndex(fileName, firstChar(lines[indexOfLine]));
                if(firstCharacter=='1'){
                    courseID = substring(lines[indexOfLine], startIndex, lineLength-2);}
                else if(firstCharacter=='2'){
                    name = substring(lines[indexOfLine], startIndex, lineLength-2);}
            }//end of if
        }//end of second for
        struct Course course= createACourse(courseID,name);
        listOfCourses[numberOfCoursesReadFromFileSoFar] = course;
        numberOfCoursesReadFromFileSoFar++;
    }//end of first for
    *numberOfCourses = numberOfCoursesReadFromFileSoFar;
    return listOfCourses;
}

struct StudentCourse* getListOfStudentCoursesFromFile(char* fileAddress, char* fileName, int* numberOfStudentsCourses){
    int numberOfStudentsCoursesReadFromFileSoFar = 0;
    int numberOfLinesInTheFile = 0;
    char** lines = readFile(fileAddress, &numberOfLinesInTheFile);
    int numberOfLinesPerRecord = numberOfItemsPerRecordInFile(fileName);
    int numberOfRecords = numberOfLinesInTheFile / numberOfLinesPerRecord;
    struct StudentCourse* listOfStudentsCourses = (struct StudentCourse*) malloc(numberOfRecords*sizeof(struct StudentCourse));
    for(int k=0; k<numberOfRecords; k++){
        char* studentID = NULL;
        char* courseID = NULL;
        int mark = 0;
        for(int i=0; i<numberOfLinesPerRecord; i++){
            int indexOfLine = k*numberOfLinesPerRecord+i;
            if(lines[indexOfLine]!=NULL){
                //printf("\n%s\n", lines[indexOfLine]);
                char firstCharacter = firstChar(lines[indexOfLine]);
                int lineLength = strLength(lines[indexOfLine]);
                int startIndex = getTheStartIndex(fileName, firstChar(lines[indexOfLine]));
                
                if(firstCharacter=='1'){
                    studentID = substring(lines[indexOfLine], startIndex, lineLength-2);}
                else if(firstCharacter=='2'){
                    courseID = substring(lines[indexOfLine], startIndex, lineLength-2);}
                else if(firstCharacter=='3'){
                    char* markStr = substring(lines[indexOfLine], startIndex, lineLength-2);
                    mark = convertToNumber(markStr);}
            }//end of if
        }//end of second for
        struct StudentCourse studentCourse = createAStudentCourse(studentID, courseID, mark);
        listOfStudentsCourses[numberOfStudentsCoursesReadFromFileSoFar] = studentCourse;
        numberOfStudentsCoursesReadFromFileSoFar++;
    }//end of first for
    *numberOfStudentsCourses = numberOfStudentsCoursesReadFromFileSoFar;
    return listOfStudentsCourses;
}

//？謎にエラーになる失敗作
//struct StudentCourse* getListOfStudentCoursesFromFile(char* fileAddress, char* fileName, int* numberOfStudentsCourses){
//    int numberOfStudentsCoursesReadFromFileSoFar = 0;
//    int numberOfLinesInTheFile = 0;
//    char** lines = readFile(fileAddress, &numberOfLinesInTheFile);
//    int numberOfLinesPerRecord = numberOfItemsPerRecordInFile(fileName);
//    int numberOfRecords = numberOfLinesInTheFile / numberOfLinesPerRecord;
//    struct StudentCourse* listOfStudentsCourses = (struct StudentCourse*) malloc(numberOfRecords*sizeof(struct StudentCourse));
//    for(int k=0; k<numberOfRecords; k++){
//        char* studentID = NULL;
//        char* courseID = NULL;
//        int mark = 0;
//        for(int i=0; i<numberOfLinesPerRecord; i++){
//            int indexOfLine = k*numberOfLinesPerRecord+i;
//            if(lines[indexOfLine]!=NULL){
//                //printf("\n%s\n", lines[indexOfLine]);
//                char firstCharacter = firstChar(lines[indexOfLine]);
//                int lineLength = strLength(lines[indexOfLine]);
//                int startIndex = getTheStartIndex(fileName, firstChar(lines[indexOfLine]));
//                if(firstCharacter=='1'){
//                    studentID = substring(lines[indexOfLine], startIndex, lineLength-2);}
//                else if(firstCharacter=='2'){
//                    courseID = substring(lines[indexOfLine], startIndex, lineLength-2);}
//                else if(firstCharacter=='3'){
//                    char* markStr = substring(lines[indexOfLine], startIndex, lineLength-2);
//                    mark = convertToNumber(markStr);}
//            }//end of if
//        }//end of second for
//        struct StudentCourse studentCourse= createAStudentCourse(studentID, courseID, mark);
//        listOfStudentsCourses[numberOfStudentsCoursesReadFromFileSoFar] = studentCourse;
//        numberOfStudentsCoursesReadFromFileSoFar++;
//    }//end of first for
//    *numberOfStudentsCourses = numberOfStudentsCoursesReadFromFileSoFar;
//    return listOfStudentCourses;
//}

