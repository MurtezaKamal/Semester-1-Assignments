#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defining the max number of users and sessions
#define MAX_USERS 100
#define MAX_SESSIONS 100
#define MAX_ENROLLS 100

typedef struct // structure to store both tutors and students in 1 file
{
    char name[50];
    char id[50];
    char password[50];
    char title[50];
} User;

typedef struct //structure for the sesisons
{
    char sessionCode[20];
    char title[20];
    char day[20];
    char startTime[20];
    char location[20];
    char tutorCode[20];
} Session;

typedef struct
{
    char studentCode[20];
    char studentName[20];
    char sessionCode[20];
    char tutorCode[20];
    char location[20];
} Enroll;

User users[MAX_USERS]; //Array for the users
Session sessions[MAX_SESSIONS]; //Array for the session
Enroll enrolls[MAX_ENROLLS]; //Array for the students enrolling

int userCount = 0; //number of users currently registered
int sessionCount = 0; // number of sessions currently registered
int enrollCount = 0; // number of students enrolled in a course
char line[100];
void adminPage();
void tutorPage(char tutorCode[100]);
void studentPage(char studentCode[100]);
void loadSessions();
void loginPage();
void loadUsers();
void loadStudents();
void userRegistration(const char* role);
void addSession();
void defaultTutors();
void defaultSessions();
void defaultStudents();
void viewTutorSessions();
void viewAllSession();
void viewStudentSession();
void enrollStudent();
void unregisterTutor(id);
void unregisterStudent(id);
void unenrollStudent(id);


void adminPage() {
    while (1) {
        int achoice;
        printf("\n ======= Admin Page ============\n");
        printf("Here are your options!\n");
        printf("1. Register a tutor\n");
        printf("2. Register a student\n");
        printf("3. Add session(s)\n");
        printf("4. Enroll a student into a session\n");
        printf("5. View all sessions\n");
        printf("6. Unregister a tutor\n");
        printf("7. Unregister a student\n");
        printf("8. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &achoice);

        switch (achoice) {
        case 1: //tutor register
            system("cls");
            userRegistration("Tutor");
            break;
        case 2: // student register
            userRegistration("Student");
            break;
        case 3: // adding sessions
            addSession();
            break;
        case 4: // enrolling students
            system("cls");
            enrollStudent();
            break;
        case 5: // view all sessions
            system("cls");
            viewAllSession();
            break;
        case 6:
            system("cls"); //tutor unregister
            printf("Enter the tutor ID: ");
            scanf("%s", line);
            unregisterTutor(line);
            break;
        case 7: // student unregister
            system("cls");
            printf("Enter the student ID: ");
            scanf("%s", line);
            unregisterStudent(line);
            break;
        case 8: //log out
            printf("Logging out...\n\n\n");
            system("cls");
            main();
        default: //invalid
            printf("Invalid option chosen\n");
            break;
        }
    }
}

void tutorPage(char tutorCode[100]) {
    while (1) {
        int tchoice; //tchoice - tutor choice
        printf("\n ======= Tutor Page ============ \n");
        printf("Welcome Tutor! \n");
        printf("1. View your sessions\n");
        printf("2. Log out\n");
        printf("Enter your choice: ");
        scanf("%d", &tchoice);

        switch (tchoice) {
        case 1:
            system("cls");
            viewTutorSessions(tutorCode);
            break;
        case 2:
            printf("Logging out....\n");
            system("cls");
            main();
            break;
        case 3:
            //changePassword();
            break;
        default:
            printf("Invalid option chosen\n");
        }
    }
}

void studentPage(char studentCode[100]) {
    while (1) {
        int schoice; //schoice - student choice
        printf("\n ============= Student Page ============= \n");
        printf("Welcome student! \n");
        printf("Here are your options! \n");
        printf("1. View your sessions\n");
        printf("2. Enroll into a session \n");
        printf("3. Log out\n");
        printf("Enter your choice: ");
        scanf("%d", &schoice);

        switch (schoice) {
        case 1:
            viewStudentSession(studentCode);
            break;
        case 2:
            enrollStudent();
            system("cls");
            break;
        case 3:
            printf("Logging out...\n\n");
            system("cls");
            main();
            break;
        default:
            printf("Invalid option chosen\n");
        }
    }
}

//login page for all users
void loginPage() {
    char id[20];
    char password[20];
    int flag = 0;
    char loggedInID[20];
    char loggedInRole[20];

    while (!flag) {
        printf("=== Login ===\n");
        printf("Enter ID: ");
        scanf("%s", id);
        printf("Enter Password: ");
        scanf("%s", password);

        //checks whether user is admin
        if (strcmp(id, "admin") == 0 && strcmp(password, "password") == 0) {
            flag = 1;
            strcpy(loggedInID, id);
            strcpy(loggedInRole, "admin");
            break;
        }
        //checks whether user is tutor
        FILE* tutorFile = fopen("tutors.txt", "r");
        if (tutorFile) {
            char line[100];
            while (fgets(line, sizeof(line), tutorFile)) {
                char tutorID[20];
                char tutorPassword[20];
                char tutorName[20];
                char tutorTitle[20];
                sscanf(line, "%[^,],%[^,],%[^,],%[^,\n]", tutorID, tutorPassword, tutorName, tutorTitle);
                if (strcmp(tutorID, id) == 0 && strcmp(tutorPassword, password) == 0) {
                    flag = 1;
                    strcpy(loggedInID, id);
                    strcpy(loggedInRole, "tutor");
                    break;
                }
            }
            fclose(tutorFile);
        }

        //checks whether user is student
        if (!flag) {
            FILE* studentFile = fopen("students.txt", "r");
            if (studentFile) {
                char line[100];
                while (fgets(line, sizeof(line), studentFile)) {
                    char studentID[20];
                    char studentPassword[20];
                    char studentName[20];
                    sscanf(line, "%[^,],%[^,],%[^,\n]", studentID, studentPassword, studentName);
                    if (strcmp(studentID, id) == 0 && strcmp(studentPassword, password) == 0) {
                        flag = 1;
                        strcpy(loggedInID, id);
                        strcpy(loggedInRole, "student");
                        break;
                    }
                }
                fclose(studentFile);
            }
        }


        if (!flag) {
            printf("Invalid ID or Password. Please try again.\n");
        }
    }

    if (strcmp(loggedInRole, "admin") == 0) {
        adminPage();
    }
    else if (strcmp(loggedInRole, "tutor") == 0) {
        tutorPage(loggedInID);
    }
    else if (strcmp(loggedInRole, "student") == 0) {
        studentPage(loggedInID);
    }
}


//registering new users (tutor or student)
void userRegistration(const char* role) {
    if (userCount >= MAX_USERS) {
        printf("User limit reached!\n");
        return;
    }

    // This checks whether the role assigned is a tutor
    if (strcmp(role, "Tutor") == 0) {
        FILE* tutorFile = fopen("tutors.txt", "r");
        if (tutorFile == NULL) {
            printf("File not found\n");
            return;
        }

        User newUser;
        printf("\n=========== Register %s =========\n", role);
        printf("Enter the %s ID: ", role);
        scanf(" %[^\n]", newUser.id);

        // Checks if tutor already exists in the file
        char line[100];
        while (fgets(line, sizeof(line), tutorFile)) {
            char id[20];
            sscanf(line, "%[^,]", id);
            if (strcmp(id, newUser.id) == 0) {
                printf("Error: Tutor already registered!\n\n");
                fclose(tutorFile);
                return;
            }
        }

        printf("Enter the %s password: ", role);
        scanf("%19s", newUser.password);
        printf("Enter the %s name: ", role);
        scanf("%19s", newUser.name);
        printf("Enter the title: ");
        scanf("%19s", newUser.title);

        users[userCount++] = newUser;
        printf("%s %s has been registered successfully\n", newUser.id, role);

        fclose(tutorFile);
        tutorFile = fopen("tutors.txt", "a");
        if (tutorFile == NULL) {
            printf("File not found\n");
            return;
        }

        fprintf(tutorFile, "%s,%s,%s,%s\n", newUser.id, newUser.password, newUser.name, newUser.title);
        fclose(tutorFile);
        system("cls");

        printf("\n========= Recent Registration =========\n");
        printf("%s ID: %s\n", role, newUser.id);
        printf("%s password: %s\n", role, newUser.password);
        printf("%s name: %s\n", role, newUser.name);
        printf("%s title: %s\n", role, newUser.title);
        printf("\n======================================\n");
    }
    //this checks whether role assigned is student
    else if (strcmp(role, "Student") == 0) {
        FILE* studentFile = fopen("students.txt", "r");
        if (studentFile == NULL) {
            printf("File not found\n");
            return;
        }

        User newUser;

        printf("\n=========== Register %s =========\n", role);
        printf("Enter the %s ID: ", role);
        scanf(" %[^\n]", newUser.id);
        //checks if student already exists
        char line[100];
        while (fgets(line, sizeof(line), studentFile)) {
            char id[20];
            sscanf(line, "%[^,]", id);
            if (strcmp(id, newUser.id) == 0) {
                printf("Error: Student already registered!\n\n");
                fclose(studentFile);
                return;
            }
        }

        printf("Enter the %s password: ", role);
        scanf("%19s", newUser.password);
        printf("Enter the %s name: ", role);
        scanf("%19s", newUser.name);

        users[userCount++] = newUser;
        printf("%s %s has been registered successfully\n", newUser.id, role);
        fclose(studentFile);
        studentFile = fopen("students.txt", "a");
        fprintf(studentFile, "%s,%s,%s\n", newUser.id, newUser.password, newUser.name);
        fclose(studentFile);
        system("cls");

        printf("\n========= Recent Registration =========\n");
        printf("%s ID: %s\n", role, newUser.id);
        printf("%s password: %s\n", role, newUser.password);
        printf("%s name: %s\n", role, newUser.name);
        printf("\n======================================\n");
    }
}


// adding a new session
void addSession() {
    system("cls");
    if (sessionCount >= MAX_SESSIONS) {
        printf("Maximum sessions reached");
        return;
    }

    Session newSession;
    printf("\n ========= Adding Sessions ==========\n");
    printf("Enter the session code: ");
    scanf("%s", newSession.sessionCode);
    for (int i = 0; i < sessionCount; i++) {
        if (strcmp(sessions[i].sessionCode, newSession.sessionCode) == 0) {
            printf("Error: Session already exists!\n\n");
            return;
        }
    }
    printf("Enter the session title: ");
    scanf("%s", newSession.title);
    printf("Enter the session day: ");
    scanf("%s", newSession.day);
    printf("Enter the session start time: ");
    scanf("%s", newSession.startTime);
    printf("Enter the session location: ");
    scanf("%s", newSession.location);
    printf("Enter the tutor code: ");
    scanf("%s", newSession.tutorCode);

    for (int i = 0; i < sessionCount; i++) {
        if (strcmp(sessions[i].tutorCode, newSession.tutorCode) == 0) {
            printf("\n Error: Tutor has already been assigned a session!\n\n");
            return;
        }
    }

    sessions[sessionCount++] = newSession;
    printf("The session(s) have been added successfully!");

    FILE* fp;
    fp = fopen("sessions.txt", "a");
    if (fp == NULL) {
        printf("File not found");
        return 0;
    }
    fprintf(fp, "% s, % s, % s, % s, % s, % s \n", newSession.sessionCode, newSession.title, newSession.day, newSession.startTime, newSession.location,
        newSession.tutorCode);
    fclose(fp);
    system("cls");

    printf("\n ============ Recently added session ==============\n");
    printf("\n Session Code: %s\n", newSession.sessionCode);
    printf("Title: %s\n", newSession.title);
    printf("Day: %s\n", newSession.day);
    printf("Start time: %s\n", newSession.startTime);
    printf("Location: %s\n", newSession.location);
    printf("\n====================================================\n");
}

void viewAllSession() {
    printf("\n ================== Existing Sessions ================= \n\n");
    printf("Session code   Title                      Day       Start time   Location           Tutor code\n");
    printf("--------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < sessionCount; i++) {
        printf("%-14s %-25s %-10s %-12s %-20s %s\n",
            sessions[i].sessionCode,
            sessions[i].title,
            sessions[i].day,
            sessions[i].startTime,
            sessions[i].location,
            sessions[i].tutorCode);
    }

    printf("\n================================================================================================\n");



    printf("\n ================== Enrolled Students =====================\n");
    printf("Student name       Session code     Tutor Code      Location\n");
    printf("------------------------------------------------------------\n");

    for (int j = 0; j < enrollCount; j++) {
        printf("%-18s %-15s %-15s %-s\n",
            enrolls[j].studentName,
            enrolls[j].sessionCode,
            enrolls[j].tutorCode,
            enrolls[j].location);
    }

    printf("============================================================\n");
}


// display the session assigned to the tutor

void viewTutorSessions(char tutorCode[100]) {
    system("cls");
    printf("====== Your Session ============\n\n");
    printf("Session code   \t\tTitle              Day      Start time   Location\n");
    printf("----------------------------------------------------------------------------\n");

    for (int i = 0; i < 100; i++) {
        if (strcmp(sessions[i].tutorCode, tutorCode) == 0) {
            printf("%-14s\t %-20s %-11s %-12s %-s\n", sessions[i].sessionCode, sessions[i].title, sessions[i].day, sessions[i].startTime, sessions[i].location);
            break;
        }
    }

    printf("-----------------------------------------------------------------------------\n");

    printf("\n =========== Students enrolled ================\n");
    printf("Student name       Session code     Tutor Code      Location\n");
    printf("------------------------------------------------------------\n");

    for (int j = 0; j < enrollCount; j++) {
        if (strcmp(enrolls[j].tutorCode, tutorCode) == 0) {
            // Check for duplicates before printing
            int isDuplicate = 0;
            for (int k = 0; k < j; k++) {
                if (strcmp(enrolls[k].studentName, enrolls[j].studentName) == 0 &&
                    strcmp(enrolls[k].sessionCode, enrolls[j].sessionCode) == 0 &&
                    strcmp(enrolls[k].tutorCode, enrolls[j].tutorCode) == 0 &&
                    strcmp(enrolls[k].location, enrolls[j].location) == 0) {
                    isDuplicate = 1;
                    break;
                }
            }

            if (!isDuplicate) {
                // Print the student information
                printf("% -18s % -15s % -15s % -s\n",
                    enrolls[j].studentName,
                    enrolls[j].sessionCode,
                    enrolls[j].tutorCode,
                    enrolls[j].location);
            }
        }
    }
}

void viewStudentSession(char studentCode[100]) {
    printf("\n========== Student Session ==========\n");
    printf("Your session:\n");
    printf("TP number   Session Code   TutorID   Location\n");
    printf("---------------------------------------------\n");

    int sessions = 0;  // Counter to track the number of sessions found for the student

    for (int i = 0; i < 100; i++) {
        if (strcmp(enrolls[i].studentCode, studentCode) == 0) {
            printf("%-11s %-14s %-9s %-s\n", enrolls[i].studentCode, enrolls[i].sessionCode, enrolls[i].tutorCode, enrolls[i].location);
            sessions++;
        }
    }

    if (sessions == 0) {
        printf("No sessions found for the student.\n");
    }

    printf("---------------------------------------------\n");
}



void enrollStudent() { //first part is showing all the available sessions
    printf("\n======================== Available Sessions ==================================\n\n");
    printf("Session code   Title                      Day       Start time   Location           Tutor code\n");
    printf("-------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < sessionCount; i++) {
        printf("%-14s %-25s %-10s %-12s %-20s %s\n",
            sessions[i].sessionCode,
            sessions[i].title,
            sessions[i].day,
            sessions[i].startTime,
            sessions[i].location,
            sessions[i].tutorCode);
    }
    printf("\n================================================================================================\n");

    //enrolling student
    if (enrollCount >= MAX_ENROLLS) {
        printf("Maximum students enrolled!");
        return;
    }

    Enroll newEnroll;
    printf("\n ============= Enroll Student ==============\n");
    printf("Enter the student TP number: ");
    scanf("%s", newEnroll.studentCode);
    printf("Enter the session code: ");
    scanf("%s", newEnroll.sessionCode);
    for (int i = 0; i < enrollCount; i++) {
        if (strcmp(enrolls[i].studentCode, newEnroll.studentCode) == 0 && strcmp(enrolls[i].sessionCode, newEnroll.sessionCode) == 0) {
            printf("\n Error: Already enrolled in this session!\n\n");
            return;
        }
    }
    printf("Enter the student name: ");
    scanf("%s", newEnroll.studentName);
    printf("Enter the tutor code: ");
    scanf("%s", newEnroll.tutorCode);
    printf("Enter the location: ");
    scanf("%s", newEnroll.location);
    enrolls[enrollCount++] = newEnroll;
    printf("Student has been enrolled to session %s successfully", newEnroll.sessionCode);

    FILE* enrollFile;
    enrollFile = fopen("enrolls.txt", "a");
    if (enrollFile == NULL) {
        printf("File not found!");
        return;
    }
    fprintf(enrollFile, "%s, %s, %s, %s, %s \n", newEnroll.studentCode, newEnroll.studentName, newEnroll.sessionCode, newEnroll.tutorCode, newEnroll.location);
    fclose(enrollFile);
    system("cls");

    printf("\n================== Recent Enrollment ==============\n");
    printf("Student TP number: %s\n", newEnroll.studentCode);
    printf("Student name: %s\n", newEnroll.studentName);
    printf("Session code: %s\n", newEnroll.sessionCode);
    printf("Tutor code: %s\n", newEnroll.tutorCode);
    printf("Location: %s\n", newEnroll.location);
    printf("======================================================\n");
}

void loadUsers() {
    FILE* fp;
    fp = fopen("tutors.txt", "r");
    if (fp == NULL) {
        printf("File not found\n");
        return;
    }

    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^\n]\n", users[userCount].id, users[userCount].password,
        users[userCount].name, users[userCount].title) == 4) {
        userCount++;
    }

    fclose(fp);
}

void saveTutorsToFile() {
    FILE* tutorFile = fopen("tutors.txt", "w");
    if (tutorFile == NULL) {
        printf("File not found!\n");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        fprintf(tutorFile, "%s,%s,%s,%s\n", users[i].id, users[i].password, users[i].name, users[i].title);
    }

    fclose(tutorFile);
}

void saveStudentToFile() {
    FILE* studentFile = fopen("students.txt", "w");
    if (studentFile = NULL) {
        printf("File not found!\n");
        return;
    }
    for (int i = 0; i < userCount; i++) {
        fprintf(studentFile, "%s,%s,%s\n", users[i].id, users[i].password, users[i].name);
    }
    fclose(studentFile);
}

void loadSessions() {
    FILE* sessionFile = fopen("sessions.txt", "r");
    if (sessionFile == NULL) {
        printf("File not found\n");
        return;
    }

    char line[100];
    sessionCount = 0;

    while (fgets(line, sizeof(line), sessionFile)) {
        Session newSession;
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,\n]",
            newSession.sessionCode,
            newSession.title,
            newSession.day,
            newSession.startTime,
            newSession.location,
            newSession.tutorCode);

        sessions[sessionCount++] = newSession;
    }

    fclose(sessionFile);
}

void saveSessiontoFile() {
    FILE* enrollFile;
    enrollFile = fopen("enrolls.txt", "w");
    if (enrollFile == NULL) {
        printf("File not found!");
    }

    for (int j = 0; j < enrollCount; j++) {
        fprintf(enrollFile, "%s,%s,%s,%s,%s", enrolls[j].studentCode, enrolls[j].studentName, enrolls[j].sessionCode, enrolls[j].tutorCode, enrolls[j].location);
    }
    fclose(enrollFile);
}




void loadStudents() {
    FILE* enrollFile = fopen("enrolls.txt", "r");
    if (!enrollFile) {
        printf("File not found!\n");
        return;
    }

    while (enrollCount < MAX_ENROLLS && fscanf(enrollFile, "%19[^,],%19[^,],%19[^,],%19[^,],%19[^\n]",
        enrolls[enrollCount].studentCode,
        enrolls[enrollCount].studentName,
        enrolls[enrollCount].sessionCode,
        enrolls[enrollCount].tutorCode,
        enrolls[enrollCount].location) == 5) {
        enrollCount++;
    }

    fclose(enrollFile);
}


void defaultSessions() {
    FILE* sessionFile;
    sessionFile = fopen("sessions.txt", "r+");
    if (sessionFile == NULL) {
        printf("File not found!");
        return;
    }

    char line[100];
    int flag = 0;

    while (fgets(line, sizeof(line), sessionFile)) {
        if (strstr(line, "PYP-101") != NULL) {
            flag = 1;
            break;
        }
    }

    if (!flag) {
        sessionFile = fopen("sessions.txt", "a");
        fprintf(sessionFile, "PYP101,Python Programming,Saturday,9.00am,C-01-01,TID1\n");
        fprintf(sessionFile, "JAV102,Java Programming,Sunday,9.00am,C-01-02,TID2\n");
        fprintf(sessionFile, "CPL103,C Programming,Saturday,2.00pm,C-01-03,TID3\n");
        fprintf(sessionFile, "WEB104,Web Development,Sunday,2.00pm,C-01-04,TID4\n");
        fprintf(sessionFile, "CSP105,C Sharp Programming,Monday,7.00pm,C-01-05,TID5\n");

        fclose(sessionFile);
    }
    else {
        fclose(sessionFile);
    }
}



void defaultTutors() {
    FILE* tutorFile;
    tutorFile = fopen("tutors.txt", "r+");
    if (tutorFile == NULL) {
        printf("File not found!");
        return;
    }

    char line[100];
    int flag = 0;

    // checks if default data is already in code
    while (fgets(line, sizeof(line), tutorFile)) {
        if (strstr(line, "TID1") != NULL) {
            flag = 1;
            break;
        }
    }

    // If TID1 does not exist, print the default data
    if (!flag) {

        fclose(tutorFile);
        tutorFile = fopen("tutors.txt", "a");
        fprintf(tutorFile, "TID1,apu123,Albert,Web Development\n");
        fprintf(tutorFile, "TID2,apu456,Amad,C Sharp Programming\n");
        fprintf(tutorFile, "TID3,apu789,Steve, Python Programming\n");

        fclose(tutorFile);
    }
    else {
        fclose(tutorFile);
    }
}


void defaultStudents() {
    FILE* studentFile;
    studentFile = fopen("students.txt", "r+");
    if (studentFile == NULL) {
        printf("File not found!");
        return 0;
    }

    char line[200];
    int flag = 0;

    while (fgets(line, sizeof(line), studentFile)) {
        if (strstr(line, "TP1") != NULL) {
            flag = 1;
            break;
        }
    }

    if (!flag) {
        fclose(studentFile);
        studentFile = fopen("students.txt", "a");
        fprintf(studentFile, "TP1,pass,Abdullah\n");
        fprintf(studentFile, "TP2,apu,Yogeswaran\n");

        fclose(studentFile);
    }
    else {
        fclose(studentFile);
    }
}
void defaultEnrolls() {
    FILE* enrollFile;
    enrollFile = fopen("enrolls.txt", "r+");
    if (enrollFile == NULL) {
        printf("File not found!");
        return 0;
    }

    char line[200];
    int flag = 0;

    while (fgets(line, sizeof(line), enrollFile)) {
        if (strstr(line, "TP1") != NULL) {
            flag = 1;
            break;
        }
    }

    if (!flag) {
        fclose(enrollFile);
        enrollFile = fopen("enrolls.txt", "a");
        fprintf(enrollFile, "TP1,Abdullah,CSP-105,TID2,C-01-5\n");
        fprintf(enrollFile, "TP2,Yogeswaran,PYP101,TID1,C-01-01\n");

        fclose(enrollFile);
    }
    else {
        fclose(enrollFile);
    }
}
// ============================================= Additional Features ========================================================================
void unregisterTutor(const char* id) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].id, id) == 0) {
            // shifting method which allows the tutor to be removed without causing errors.
            for (int j = i; j < userCount - 1; j++) {
                users[j] = users[j + 1];
            }
            userCount--; //userCount goes down
            printf("Tutor with ID '%s' has been unregistered successfully.\n", id);

            // This removes the targeted tutor from the main file
            FILE* tutorFile = fopen("tutors.txt", "r");
            FILE* tempFile = fopen("temp.txt", "w");
            if (tutorFile == NULL && tempFile == NULL) {
                printf("File not found\n");
                return;
            }
            // This moves all data but the requird to a temporary file called "temp.txt" wirh the use of variable line
            char line[100];
            while (fgets(line, sizeof(line), tutorFile)) {
                char tutorID[20];
                sscanf(line, "%[^,]", tutorID);
                if (strcmp(tutorID, id) != 0) {
                    fprintf(tempFile, "%s", line);
                }
            }

            fclose(tutorFile);
            fclose(tempFile);

            // Tutors.txt gets deleted and replaced with the temporary file
            remove("tutors.txt");
            rename("temp.txt", "tutors.txt");

            return;
        }
    }
    printf("Tutor with ID '%s' not found.\n", id);
}

void unregisterStudent(const char* id) {
    FILE* studentFile = fopen("students.txt", "r");
    FILE* tempFile = fopen("temp.txt", "w");

    if (studentFile == NULL && tempFile == NULL) {
        printf("File not found!\n");
        return;
    }
    // Used to write the contents of students.txt into temp if the id is not matched
    char line[100];
    while (fgets(line, sizeof(line), studentFile)) {
        char studentID[20];
        sscanf(line, "%[^,]", studentID);
        if (strcmp(studentID, id) != 0) {
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(studentFile);
    fclose(tempFile);
    remove("students.txt");
    rename("temp.txt", "students.txt");
    printf("\n Student has been unregistered successfully!\n");
    return;
}

int main() {
    defaultTutors();
    defaultSessions();
    defaultStudents();
    defaultEnrolls();
    loadUsers(); //this loads the users currently in the file
    loadStudents(); //this loads the students currently enrolled in a session
    loadSessions();//this loads the sessions currently in the file
    printf("\n ======================================================== \n");
    printf("\n \t Welcome to APU's Programming Cafe\n");
    printf("\n ======================================================== \n\n");
    loginPage(); //function for the login page
    return 0;
}
