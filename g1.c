#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>

// File definitions for persistent storage
#define USER_FILE "users.txt"
#define CUSTOMER_FILE "customers.txt"
#define VEHICLE_FILE "vehicles.txt"
#define JOB_FILE "jobs.txt"
#define INVOICE_FILE "invoices.txt"
#define PART_FILE "parts.txt"
#define SUPPLIER_FILE "suppliers.txt"
#define APPOINTMENT_FILE "appointments.txt"
#define JOB_PART_FILE "job_parts.txt"
#define LOG_FILE "system.log"
#define FINANCE_FILE "finance.txt"
#define EXPENSES_FILE "expenses.txt"
#define REMINDERS_FILE "reminders.txt"
#define REORDER_LIST_FILE "reorder_list.txt"
#define NOTIFICATION_FILE "notifications.txt"
#define BACKUP_DIR "backup"


// ANSI escape codes for colored console output
// Created by rahul gsms
#define BLUE "\x1b[34m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define RED "\x1b[31m"
#define CYAN "\x1b[36m"
#define MAGENTA "\x1b[35m"
#define RESET "\x1b[0m"

// Global function prototypes for improved code structure and readability
void clearInputBuffer();
void pauseScreen();
void clearScreen();
void hashPassword(char *pwd);
int fileLineCount(const char *fname);
void generateId(const char *prefix, int num, char *id);
void writeLog(const char* message);
void getCurrentDate(char *date);
int isDateValid(const char *date);
float calculateAverageCost(const char *jobDescription);
void showDashboard();
void createNotification(const char* recipientId, const char* message);
void viewNotifications(const char* uid);
void backupData();
void restoreData();
void calculateNextServiceDate(const char* lastServiceDate, char* nextServiceDate);
int daysBetweenDates(const char *date1_str, const char *date2_str);
int getDaysInMonth(int month, int year);
int getNextServiceInterval(const char *vehicleId);

// User Management Prototypes
int login(char *role, char *uid);
void registerUser();
void changePassword(char *uid);
void viewAllUsers();
void changeUserRole();
int getRoleLevel(const char *role);

// Customer Management Prototypes
void addCustomer();
int customerExists(const char *id);
void editCustomer();
void deleteCustomer();
void searchCustomer();
void viewAllCustomers();
void sendFeedbackRequest();

// Vehicle Management Prototypes
void addVehicle();
int vehicleExists(const char *id);
void editVehicle();
void deleteVehicle();
void searchVehicle();
void viewAllVehicles();
void viewCustomerVehicles(const char* customerId);

// Job Management Prototypes
int mechanicExists(const char *id);
void addJob(const char *currentUserId, const char *currentUserRole);
void updateJobStatus(const char *currentUserId, const char *currentUserRole);
void viewMyJobs(const char *mechanicId);
void viewCustomerJobs(const char *customerId);
void searchJob();
void viewAllJobs();
void viewPendingJobs();
void viewCompletedJobs();

// Invoice Management Prototypes
void generateInvoice();
void viewAllInvoices();

// Inventory Management Prototypes
void addPartToJob();
void updateJobLaborCost();
void addPart();
void editPart();
void viewInventory();
int partExists(const char *id);
float getPartPrice(const char *partId);
void deductPartFromInventory(const char *partId, int quantity);
void checkLowStock();
void viewReorderList();
void clearReorderList();

// Supplier Management Prototypes
void addSupplier();
void viewAllSuppliers();
int supplierExists(const char *id);

// Appointment Management Prototypes
void scheduleAppointment(const char *currentUserId, const char *currentUserRole);
void viewAllAppointments();
void scheduleServiceReminder();
void viewAllReminders();

// Financial Management Prototypes
void addExpense();
void viewAllExpenses();
void generateProfitLossReport();

// Reporting Prototypes
void viewJobSummaryReport();
void mechanicPerformanceReport();
void customerLifetimeValueReport();
void monthlyRevenueReport();
void partUsageReport();
void supplierPerformanceReport();
void customerRetentionReport();

// Menu Prototypes
void adminMenu(const char *uid);
void seniorMechanicMenu(const char *uid);
void juniorMechanicMenu(const char *uid);
void customerMenu(const char *uid);
void printLogo();

// Structs for data modeling
typedef struct {
    char id[10];
    char username[30];
    char password[30];
    char role[20];
} User;

typedef struct {
    char id[10];
    char name[50];
    char phone[15];
    char email[50];
} Customer;

typedef struct {
    char id[10];
    char customerId[10];
    char brand[30];
    char model[30];
    char plate[15];
    char year[5];
    char lastServiceDate[15];
} Vehicle;

typedef struct {
    char id[10];
    char customerId[10];
    char vehicleId[10];
    char mechanicId[10];
    char description[100];
    char status[20];
    float estimatedCost;
    float laborCost;
    float partsCost;
    char notes[200];
    char startDate[15];
    char endDate[15];
} Job;

typedef struct {
    char id[10];
    char jobId[10];
    float totalCost;
    float discount;
    float total;
    char date[15];
} Invoice;

typedef struct {
    char id[10];
    char name[50];
    int quantity;
    float price;
    char supplierId[10];
    int reorderLevel;
} Part;

typedef struct {
    char id[10];
    char name[50];
    char phone[15];
    char email[50];
} Supplier;

typedef struct {
    char id[10];
    char customerId[10];
    char vehicleId[10];
    char date[15];
    char time[10];
    char description[100];
    char status[20];
} Appointment;

typedef struct {
    char jobId[10];
    char partId[10];
    int quantity;
    float price;
} JobPart;

typedef struct {
    char id[10];
    char type[20]; // 'Revenue' or 'Expense'
    float amount;
    char date[15];
    char description[100];
} Transaction;

typedef struct {
    char id[10];
    char customerId[10];
    char message[200];
    char date[15];
    char status[20]; // 'Scheduled', 'Sent'
} Reminder;

typedef struct {
    char partId[10];
    int neededQuantity;
    char date[15];
} ReorderItem;

typedef struct {
    char id[10];
    char recipientId[10];
    char message[200];
    char date[15];
    int isRead; // 0 for unread, 1 for read
} Notification;


// Utility Functions
// Clears the input buffer to prevent issues with subsequent scanf calls
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Pauses the screen until the user presses Enter
void pauseScreen() {
    printf(BLUE "\nPress Enter to continue..." RESET);
    clearInputBuffer();
    getchar();
}

// Clears the console screen
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// A simple hashing function for passwords
void hashPassword(char *pwd) {
    for (int i = 0; pwd[i] != '\0'; i++) pwd[i] += 3;
}

// Counts the number of lines in a file
int fileLineCount(const char *fname) {
    FILE *fp = fopen(fname, "r");
    if (!fp) return 0;
    char buf[500];
    int c = 0;
    while (fgets(buf, sizeof(buf), fp)) c++;
    fclose(fp);
    return c;
}

// Generates a unique ID based on a prefix and a number
void generateId(const char *prefix, int num, char *id) {
    sprintf(id, "%s%04d", prefix, num);
}

// Writes a log message with a timestamp
void writeLog(const char* message) {
    FILE *fp = fopen(LOG_FILE, "a");
    if (fp) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        fprintf(fp, "[%d-%02d-%02d %02d:%02d:%02d] %s\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, message);
        fclose(fp);
    }
}

// Gets the current date in YYYY-MM-DD format
void getCurrentDate(char *date) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

// Validates a date string format YYYY-MM-DD
int isDateValid(const char *date) {
    int year, month, day;
    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3) {
        return 0;
    }
    if (year < 1900 || year > 2100 || month < 1 || month > 12 || day < 1 || day > 31) {
        return 0;
    }
    // Basic check, could be more robust for specific months
    return 1;
}

// Dynamically calculates an estimated cost based on past job data
float calculateAverageCost(const char *jobDescription) {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) return 0.0;
    
    Job j;
    char line[500];
    float total_cost = 0;
    int count = 0;

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", 
                j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost, 
                &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        
        if (strstr(j.description, jobDescription) && strcmp(j.status, "Completed") == 0) {
            total_cost += j.laborCost + j.partsCost;
            count++;
        }
    }
    fclose(fp);

    if (count > 0) {
        return total_cost / count;
    }
    return 0.0;
}

// Calculates the number of days between two dates (YYYY-MM-DD)
int daysBetweenDates(const char *date1_str, const char *date2_str) {
    struct tm tm1 = {0}, tm2 = {0};
    sscanf(date1_str, "%d-%d-%d", &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday);
    sscanf(date2_str, "%d-%d-%d", &tm2.tm_year, &tm2.tm_mon, &tm2.tm_mday);
    
    tm1.tm_year -= 1900;
    tm1.tm_mon -= 1;
    tm2.tm_year -= 1900;
    tm2.tm_mon -= 1;

    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);

    if (t1 == -1 || t2 == -1) {
        return -1; // Error
    }

    return (int)difftime(t2, t1) / (60 * 60 * 24);
}

// Gets the number of days in a given month of a given year
int getDaysInMonth(int month, int year) {
    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            return 29;
        } else {
            return 28;
        }
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    } else {
        return 31;
    }
}

// Calculates the next service date based on a fixed interval
void calculateNextServiceDate(const char* lastServiceDate, char* nextServiceDate) {
    if (strcmp(lastServiceDate, "N/A") == 0) {
        strcpy(nextServiceDate, "N/A");
        return;
    }

    int year, month, day;
    sscanf(lastServiceDate, "%d-%d-%d", &year, &month, &day);

    // Assume service interval is 6 months
    month += 6;
    if (month > 12) {
        month -= 12;
        year++;
    }

    // Adjust day for end-of-month issues
    int days_in_new_month = getDaysInMonth(month, year);
    if (day > days_in_new_month) {
        day = days_in_new_month;
    }

    sprintf(nextServiceDate, "%d-%02d-%02d", year, month, day);
}

// Shows a dashboard with key metrics
void showDashboard() {
    int total_jobs = 0, pending_jobs = 0, completed_jobs = 0;
    float monthly_revenue = 0;
    int reorder_items = 0;

    char current_date[15];
    getCurrentDate(current_date);
    int current_year, current_month, current_day;
    sscanf(current_date, "%d-%d-%d", &current_year, &current_month, &current_day);

    // Get job stats and monthly revenue
    FILE *fp_job = fopen(JOB_FILE, "r");
    if (fp_job) {
        char line[500];
        Job j;
        while (fgets(line, sizeof(line), fp_job)) {
            sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", j.id, j.customerId, j.vehicleId,
                 j.mechanicId, j.description, j.status, &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
            total_jobs++;
            if (strcmp(j.status, "Pending") == 0) pending_jobs++;
            if (strcmp(j.status, "Completed") == 0) {
                completed_jobs++;
                int job_year, job_month, job_day;
                sscanf(j.endDate, "%d-%d-%d", &job_year, &job_month, &job_day);
                if (job_year == current_year && job_month == current_month) {
                    monthly_revenue += (j.laborCost + j.partsCost);
                }
            }
        }
        fclose(fp_job);
    }

    // Get low stock alerts
    FILE *fp_reorder = fopen(REORDER_LIST_FILE, "r");
    if (fp_reorder) {
        char line[200];
        while (fgets(line, sizeof(line), fp_reorder)) {
            reorder_items++;
        }
        fclose(fp_reorder);
    }
    
    printf("\n" CYAN "--- Main Dashboard ---" RESET "\n");
    printf(BLUE "------------------------" RESET "\n");
    printf(MAGENTA "  Total Jobs: %d\n" RESET, total_jobs);
    printf(MAGENTA "  Pending Jobs: %d\n" RESET, pending_jobs);
    printf(MAGENTA "  Completed Jobs: %d\n" RESET, completed_jobs);
    printf(MAGENTA "  Monthly Revenue: %.2f\n" RESET, monthly_revenue);
    printf(MAGENTA "  Low Stock Alerts: %d\n" RESET, reorder_items);
    printf(BLUE "------------------------" RESET "\n");
}

// Creates a notification for a specific user
void createNotification(const char* recipientId, const char* message) {
    FILE *fp = fopen(NOTIFICATION_FILE, "a+");
    if (!fp) return;

    Notification n;
    int cnt = fileLineCount(NOTIFICATION_FILE);
    generateId("NOT", cnt + 1, n.id);
    strcpy(n.recipientId, recipientId);
    strcpy(n.message, message);
    getCurrentDate(n.date);
    n.isRead = 0;

    fprintf(fp, "%s,%s,%s,%s,%d\n", n.id, n.recipientId, n.message, n.date, n.isRead);
    fclose(fp);
}

// Views unread notifications for a user and marks them as read
void viewNotifications(const char* uid) {
    FILE *fp_read = fopen(NOTIFICATION_FILE, "r");
    if (!fp_read) {
        printf(GREEN "You have no new notifications.\n" RESET);
        pauseScreen();
        return;
    }

    FILE *fp_write = fopen("temp_notifications.txt", "w");
    if (!fp_write) {
        fclose(fp_read);
        return;
    }

    Notification n;
    char line[300];
    int new_notifications = 0;

    printf("\n" MAGENTA "--- Your Notifications ---" RESET "\n");
    while(fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%9[^,],%199[^,],%14[^,],%d", n.id, n.recipientId, n.message, n.date, &n.isRead);
        if (strcmp(n.recipientId, uid) == 0 && n.isRead == 0) {
            printf(CYAN "[NEW] (%s) %s\n" RESET, n.date, n.message);
            n.isRead = 1;
            new_notifications++;
        } else if (strcmp(n.recipientId, uid) == 0 && n.isRead == 1) {
            printf(YELLOW "[READ] (%s) %s\n" RESET, n.date, n.message);
        }
        fprintf(fp_write, "%s,%s,%s,%s,%d\n", n.id, n.recipientId, n.message, n.date, n.isRead);
    }
    
    if (new_notifications == 0) {
        printf(GREEN "No new notifications found.\n" RESET);
    }
    
    fclose(fp_read);
    fclose(fp_write);
    remove(NOTIFICATION_FILE);
    rename("temp_notifications.txt", NOTIFICATION_FILE);
    pauseScreen();
}


// Backs up all data files
void backupData() {
    printf(CYAN "Creating data backup...\n" RESET);

    // Here, the mkdir() function is used according to the operating system.
    // If _WIN32 is defined, _mkdir() is used for Windows.
    // Otherwise, mkdir() is used for POSIX (Linux/macOS).
    #ifdef _WIN32
    if (_mkdir(BACKUP_DIR) == 0) {
        printf(GREEN "Backup directory created successfully.\n" RESET);
    } else {
        printf(YELLOW "Backup directory already exists or could not be created.\n" RESET);
    }
    #else
    if (mkdir(BACKUP_DIR, 0777) == 0) {
        printf(GREEN "Backup directory created successfully.\n" RESET);
    } else {
        printf(YELLOW "Backup directory already exists or could not be created.\n" RESET);
    }
    #endif

    char cmd[200];

    // The system() function is used to copy each file.
    // 'copy' is used for Windows, and 'cp' is used for Linux/macOS.
    #ifdef _WIN32
    sprintf(cmd, "copy %s %s\\", USER_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "copy %s %s\\", CUSTOMER_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "copy %s %s\\", VEHICLE_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "copy %s %s\\", JOB_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "copy %s %s\\", INVOICE_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "copy %s %s\\", PART_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "copy %s %s\\", SUPPLIER_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "copy %s %s\\", APPOINTMENT_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "copy %s %s\\", JOB_PART_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "copy %s %s\\", FINANCE_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "copy %s %s\\", EXPENSES_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "copy %s %s\\", REMINDERS_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "copy %s %s\\", REORDER_LIST_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "copy %s %s\\", NOTIFICATION_FILE, BACKUP_DIR); system(cmd);
    #else
    sprintf(cmd, "cp %s %s/", USER_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "cp %s %s/", CUSTOMER_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "cp %s %s/", VEHICLE_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "cp %s %s/", JOB_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "cp %s %s/", INVOICE_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "cp %s %s/", PART_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "cp %s %s/", SUPPLIER_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "cp %s %s/", APPOINTMENT_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "cp %s %s/", JOB_PART_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "cp %s %s/", FINANCE_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "cp %s %s/", EXPENSES_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "cp %s %s/", REMINDERS_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "cp %s %s/", REORDER_LIST_FILE, BACKUP_DIR); system(cmd);
    sprintf(cmd, "cp %s %s/", NOTIFICATION_FILE, BACKUP_DIR); system(cmd);
    #endif

    printf(GREEN "All data files have been backed up to the '%s' directory.\n" RESET, BACKUP_DIR);
    writeLog("Data backup completed.");
    pauseScreen();
}

// Restores data from the backup
void restoreData() {
    char confirm;
    printf(YELLOW "WARNING: This will overwrite all current data with the backup.\n" RESET);
    printf(YELLOW "Are you sure you want to proceed? (y/n): " RESET);
    scanf(" %c", &confirm);
    if (tolower(confirm) != 'y') {
        printf(RED "Restore cancelled.\n" RESET);
        pauseScreen();
        return;
    }

    char cmd[200];

    #ifdef _WIN32
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, USER_FILE, USER_FILE); system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, CUSTOMER_FILE, CUSTOMER_FILE); system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, VEHICLE_FILE, VEHICLE_FILE); system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, JOB_FILE, JOB_FILE); system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, INVOICE_FILE, INVOICE_FILE); system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, PART_FILE, PART_FILE); system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, SUPPLIER_FILE, SUPPLIER_FILE); system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, APPOINTMENT_FILE, APPOINTMENT_FILE); system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, JOB_PART_FILE, JOB_PART_FILE); system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, FINANCE_FILE, FINANCE_FILE); system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, EXPENSES_FILE, EXPENSES_FILE); system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, REMINDERS_FILE, REMINDERS_FILE); system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, REORDER_LIST_FILE, REORDER_LIST_FILE); system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, NOTIFICATION_FILE, NOTIFICATION_FILE); system(cmd);
    #else
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, USER_FILE, USER_FILE); system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, CUSTOMER_FILE, CUSTOMER_FILE); system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, VEHICLE_FILE, VEHICLE_FILE); system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, JOB_FILE, JOB_FILE); system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, INVOICE_FILE, INVOICE_FILE); system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, PART_FILE, PART_FILE); system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, SUPPLIER_FILE, SUPPLIER_FILE); system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, APPOINTMENT_FILE, APPOINTMENT_FILE); system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, JOB_PART_FILE, JOB_PART_FILE); system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, FINANCE_FILE, FINANCE_FILE); system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, EXPENSES_FILE, EXPENSES_FILE); system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, REMINDERS_FILE, REMINDERS_FILE); system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, REORDER_LIST_FILE, REORDER_LIST_FILE); system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, NOTIFICATION_FILE, NOTIFICATION_FILE); system(cmd);
    #endif

    printf(GREEN "Data has been restored from backup successfully.\n" RESET);
    writeLog("Data restore completed.");
    pauseScreen();
}

// User Management Functions
// Handles user login with password hashing and role-based access
int login(char *role, char *uid) {
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) {
        printf(RED "No users found. Please register an admin first.\n" RESET);
        return 0;
    }

    char user[30], pass[30];
    int tries = 0;

    while (tries < 3) {
        printf(YELLOW "\nUsername: " RESET);
        scanf("%29s", user);
        printf(YELLOW "Password: " RESET);
        scanf("%29s", pass);
        hashPassword(pass);

        User u;
        char line[200];
        rewind(fp);
        int found = 0;
        while (fgets(line, 200, fp)) {
            sscanf(line, "%9[^,],%29[^,],%29[^,],%19[^\n]", u.id, u.username, u.password, u.role);
            if (!strcmp(u.username, user) && !strcmp(u.password, pass)) {
                strcpy(role, u.role);
                strcpy(uid, u.id);
                found = 1;
                break;
            }
        }
        if (found) {
            fclose(fp);
            char log_msg[100];
            sprintf(log_msg, "User %s (%s) logged in.", u.username, u.role);
            writeLog(log_msg);
            // Show notifications on login
            if (strcmp(role, "Customer") == 0) {
                viewNotifications(uid);
            }
            return 1;
        }

        tries++;
        printf(RED "Invalid username or password! Attempts left: %d\n" RESET, 3 - tries);
    }
    fclose(fp);
    writeLog("Login failed after 3 attempts.");
    return 0;
}

// Registers a new user with a specific role
void registerUser() {
    FILE *fp = fopen(USER_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening user file.\n" RESET);
        return;
    }

    User u;
    char buf[200];
    int c = 0;

    rewind(fp);
    while (fgets(buf, 200, fp)) c++;

    printf(YELLOW "\nEnter username: " RESET);
    scanf("%29s", u.username);

    rewind(fp);
    while (fgets(buf, 200, fp)) {
        User temp_u;
        sscanf(buf, "%9[^,],%29[^,],%29[^,],%19[^\n]", temp_u.id, temp_u.username, temp_u.password, temp_u.role);
        if (strcmp(temp_u.username, u.username) == 0) {
            printf(RED "Username already exists. Please choose a different username.\n" RESET);
            fclose(fp);
            pauseScreen();
            return;
        }
    }

    printf(YELLOW "Enter password: " RESET);
    scanf("%29s", u.password);
    hashPassword(u.password);

    if (c == 0) {
        strcpy(u.role, "Admin");
        printf(GREEN "Role set to Admin (first user)\n" RESET);
    } else {
        int role_choice;
        printf(YELLOW "Enter role (1: Admin / 2: SeniorMechanic / 3: JuniorMechanic / 4: Customer): " RESET);
        scanf("%d", &role_choice);
        switch (role_choice) {
            case 1: strcpy(u.role, "Admin"); break;
            case 2: strcpy(u.role, "SeniorMechanic"); break;
            case 3: strcpy(u.role, "JuniorMechanic"); break;
            case 4: strcpy(u.role, "Customer"); break;
            default:
                printf(RED "Invalid role choice. User not registered.\n" RESET);
                fclose(fp);
                pauseScreen();
                return;
        }
    }

    generateId("USR", c + 1, u.id);

    fprintf(fp, "%s,%s,%s,%s\n", u.id, u.username, u.password, u.role);
    fclose(fp);

    printf(GREEN "User registered successfully! ID: %s\n" RESET, u.id);
    char log_msg[100];
    sprintf(log_msg, "New user %s (%s) registered with ID %s.", u.username, u.role, u.id);
    writeLog(log_msg);
    pauseScreen();
}

// Allows a user to change their password
void changePassword(char *uid) {
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) {
        printf(RED "Error opening user file.\n" RESET);
        return;
    }

    User arr[100];
    int c = 0;
    char line[200];

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%29[^,],%29[^,],%19[^\n]", arr[c].id, arr[c].username, arr[c].password, arr[c].role);
        c++;
    }
    fclose(fp);

    int found = 0;
    for (int i = 0; i < c; i++) {
        if (!strcmp(arr[i].id, uid)) {
            char np[30];
            printf(YELLOW "Enter new password: " RESET);
            scanf("%29s", np);
            hashPassword(np);
            strcpy(arr[i].password, np);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf(RED "User ID not found.\n" RESET);
        pauseScreen();
        return;
    }

    fp = fopen(USER_FILE, "w");
    if (!fp) {
        printf(RED "Error opening user file for writing.\n" RESET);
        return;
    }
    for (int i = 0; i < c; i++)
        fprintf(fp, "%s,%s,%s,%s\n", arr[i].id, arr[i].username, arr[i].password, arr[i].role);
    fclose(fp);

    printf(GREEN "Password changed successfully!\n" RESET);
    char log_msg[100];
    sprintf(log_msg, "Password changed for user ID %s.", uid);
    writeLog(log_msg);
    pauseScreen();
}

void viewAllUsers() {
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) {
        printf(RED "No users found!\n" RESET);
        return;
    }
    char line[200];
    printf("\n" CYAN "--- System Users ---" RESET "\n");
    printf("ID,Username,Role\n");
    User u;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%29[^,],%29[^,],%19[^\n]", u.id, u.username, u.password, u.role);
        printf("%s,%s,%s\n", u.id, u.username, u.role);
    }
    fclose(fp);
    pauseScreen();
}

int getRoleLevel(const char *role) {
    if (strcmp(role, "Admin") == 0) return 3;
    if (strcmp(role, "SeniorMechanic") == 0) return 2;
    if (strcmp(role, "JuniorMechanic") == 0) return 1;
    return 0; // Customer
}

void changeUserRole() {
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) {
        printf(RED "Error opening user file.\n" RESET);
        return;
    }

    User arr[100];
    int c = 0;
    char line[200];

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%29[^,],%29[^,],%19[^\n]", arr[c].id, arr[c].username, arr[c].password, arr[c].role);
        c++;
    }
    fclose(fp);

    char id[10];
    printf(YELLOW "\nEnter User ID to change role: " RESET);
    scanf("%9s", id);

    int found = 0;
    for (int i = 0; i < c; i++) {
        if (!strcmp(arr[i].id, id)) {
            printf("Current Role: %s\n", arr[i].role);
            int new_role_choice;
            printf(YELLOW "Enter new role (1: Admin / 2: SeniorMechanic / 3: JuniorMechanic / 4: Customer): " RESET);
            scanf("%d", &new_role_choice);
            switch (new_role_choice) {
                case 1: strcpy(arr[i].role, "Admin"); break;
                case 2: strcpy(arr[i].role, "SeniorMechanic"); break;
                case 3: strcpy(arr[i].role, "JuniorMechanic"); break;
                case 4: strcpy(arr[i].role, "Customer"); break;
                default:
                    printf(RED "Invalid role choice. Role not changed.\n" RESET);
                    pauseScreen();
                    return;
            }
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf(RED "User ID not found.\n" RESET);
        pauseScreen();
        return;
    }

    fp = fopen(USER_FILE, "w");
    if (!fp) {
        printf(RED "Error opening user file for writing.\n" RESET);
        return;
    }
    for (int i = 0; i < c; i++)
        fprintf(fp, "%s,%s,%s,%s\n", arr[i].id, arr[i].username, arr[i].password, arr[i].role);
    fclose(fp);

    printf(GREEN "User role updated successfully!\n" RESET);
    char log_msg[100];
    sprintf(log_msg, "Role for user ID %s changed.", id);
    writeLog(log_msg);
    pauseScreen();
}


// Customer Management Functions
// Adds a new customer to the system
void addCustomer() {
    FILE *fp = fopen(CUSTOMER_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening customer file.\n" RESET);
        return;
    }

    Customer cust;
    int cnt = fileLineCount(CUSTOMER_FILE);

    printf(YELLOW "\nEnter customer name: " RESET);
    clearInputBuffer();
    fgets(cust.name, sizeof(cust.name), stdin);
    cust.name[strcspn(cust.name, "\n")] = '\0';

    printf(YELLOW "Enter phone number: " RESET);
    scanf("%14s", cust.phone);
    printf(YELLOW "Enter email: " RESET);
    scanf("%49s", cust.email);

    generateId("CUST", cnt + 1, cust.id);

    fprintf(fp, "%s,%s,%s,%s\n", cust.id, cust.name, cust.phone, cust.email);
    fclose(fp);

    printf(GREEN "Customer added successfully! ID: %s\n" RESET, cust.id);
    char log_msg[100];
    sprintf(log_msg, "New customer %s added with ID %s.", cust.name, cust.id);
    writeLog(log_msg);
    pauseScreen();
}

// Checks if a customer ID exists
int customerExists(const char *id) {
    FILE *fp = fopen(CUSTOMER_FILE, "r");
    if (!fp) return 0;

    Customer c;
    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", c.id, c.name, c.phone, c.email);
        if (!strcmp(c.id, id)) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// Edits an existing customer's information
void editCustomer() {
    FILE *fp = fopen(CUSTOMER_FILE, "r");
    if (!fp) {
        printf(RED "No customers found.\n" RESET);
        return;
    }

    Customer arr[200];
    int c = 0;
    char line[200];

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", arr[c].id, arr[c].name, arr[c].phone, arr[c].email);
        c++;
    }
    fclose(fp);

    char id[10];
    printf(YELLOW "\nEnter Customer ID to edit: " RESET);
    scanf("%9s", id);

    int found = 0;
    for (int i = 0; i < c; i++) {
        if (!strcmp(arr[i].id, id)) {
            printf("Current Name: %s\n", arr[i].name);
            printf(YELLOW "Enter new name (or press Enter to keep current): " RESET);
            clearInputBuffer();
            char newName[50];
            fgets(newName, sizeof(newName), stdin);
            newName[strcspn(newName, "\n")] = '\0';
            if (strlen(newName) > 0) strcpy(arr[i].name, newName);

            printf("Current Phone: %s\n", arr[i].phone);
            printf(YELLOW "Enter new phone (or press Enter to keep current): " RESET);
            char newPhone[15];
            fgets(newPhone, sizeof(newPhone), stdin);
            newPhone[strcspn(newPhone, "\n")] = '\0';
            if (strlen(newPhone) > 0) strcpy(arr[i].phone, newPhone);

            printf("Current Email: %s\n", arr[i].email);
            printf(YELLOW "Enter new email (or press Enter to keep current): " RESET);
            char newEmail[50];
            fgets(newEmail, sizeof(newEmail), stdin);
            newEmail[strcspn(newEmail, "\n")] = '\0';
            if (strlen(newEmail) > 0) strcpy(arr[i].email, newEmail);

            found = 1;
            break;
        }
    }

    if (!found) {
        printf(RED "Customer ID not found.\n" RESET);
        pauseScreen();
        return;
    }

    fp = fopen(CUSTOMER_FILE, "w");
    if (!fp) {
        printf(RED "Error opening customer file for writing.\n" RESET);
        return;
    }
    for (int i = 0; i < c; i++)
        fprintf(fp, "%s,%s,%s,%s\n", arr[i].id, arr[i].name, arr[i].phone, arr[i].email);
    fclose(fp);

    printf(GREEN "Customer updated successfully!\n" RESET);
    char log_msg[100];
    sprintf(log_msg, "Customer with ID %s updated.", id);
    writeLog(log_msg);
    pauseScreen();
}

// Deletes a customer
void deleteCustomer() {
    FILE *fp_read = fopen(CUSTOMER_FILE, "r");
    if (!fp_read) {
        printf(RED "No customers found.\n" RESET);
        return;
    }

    char id[10];
    printf(YELLOW "\nEnter Customer ID to delete: " RESET);
    scanf("%9s", id);

    FILE *fp_write = fopen("temp_customers.txt", "w");
    if (!fp_write) {
        fclose(fp_read);
        return;
    }

    Customer cust;
    char line[200];
    int found = 0;

    while (fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", cust.id, cust.name, cust.phone, cust.email);
        if (strcmp(cust.id, id) == 0) {
            found = 1;
        } else {
            fprintf(fp_write, "%s,%s,%s,%s\n", cust.id, cust.name, cust.phone, cust.email);
        }
    }

    fclose(fp_read);
    fclose(fp_write);

    if (found) {
        remove(CUSTOMER_FILE);
        rename("temp_customers.txt", CUSTOMER_FILE);
        printf(GREEN "Customer deleted successfully!\n" RESET);
        char log_msg[100];
        sprintf(log_msg, "Customer with ID %s deleted.", id);
        writeLog(log_msg);
    } else {
        remove("temp_customers.txt");
        printf(RED "Customer ID not found.\n" RESET);
    }
    pauseScreen();
}

// Searches for a customer by name or ID
void searchCustomer() {
    FILE *fp = fopen(CUSTOMER_FILE, "r");
    if (!fp) {
        printf(RED "No customers found.\n" RESET);
        return;
    }

    char search_term[50];
    printf(YELLOW "\nEnter customer name or ID to search: " RESET);
    clearInputBuffer();
    fgets(search_term, sizeof(search_term), stdin);
    search_term[strcspn(search_term, "\n")] = '\0';

    Customer cust;
    char line[200];
    int found = 0;

    printf("\n" CYAN "--- Search Results ---" RESET "\n");
    printf("ID\tName\t\tPhone\t\tEmail\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", cust.id, cust.name, cust.phone, cust.email);
        if (strstr(cust.name, search_term) || strstr(cust.id, search_term)) {
            printf("%s\t%s\t\t%s\t%s\n", cust.id, cust.name, cust.phone, cust.email);
            found = 1;
        }
    }

    if (!found) {
        printf(YELLOW "No matching customers found.\n" RESET);
    }

    fclose(fp);
    pauseScreen();
}

// Views all customers
void viewAllCustomers() {
    FILE *fp = fopen(CUSTOMER_FILE, "r");
    if (!fp) {
        printf(RED "No customers found!\n" RESET);
        return;
    }
    Customer c;
    char line[200];
    printf("\n" CYAN "--- All Customers ---" RESET "\n");
    printf("ID\tName\t\tPhone\t\tEmail\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", c.id, c.name, c.phone, c.email);
        printf("%s\t%s\t\t%s\t%s\n", c.id, c.name, c.phone, c.email);
    }
    fclose(fp);
    pauseScreen();
}

// Sends a feedback request to a customer
void sendFeedbackRequest() {
    char customerId[10];
    printf(YELLOW "Enter Customer ID to send feedback request: " RESET);
    scanf("%9s", customerId);
    if (customerExists(customerId)) {
        char message[200];
        sprintf(message, "Your recent service has been completed. Please provide your valuable feedback. Thank you!");
        createNotification(customerId, message);
        printf(GREEN "Feedback request sent successfully.\n" RESET);
    } else {
        printf(RED "Customer ID not found.\n" RESET);
    }
    pauseScreen();
}


// Vehicle Management Functions
// Adds a new vehicle
void addVehicle() {
    FILE *fp = fopen(VEHICLE_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening vehicle file.\n" RESET);
        return;
    }

    Vehicle v;
    int cnt = fileLineCount(VEHICLE_FILE);
    char customerId[10];

    printf(YELLOW "\nEnter Customer ID for the vehicle: " RESET);
    scanf("%9s", customerId);
    if (!customerExists(customerId)) {
        printf(RED "Customer ID not found. Please add the customer first.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    }
    strcpy(v.customerId, customerId);

    printf(YELLOW "Enter vehicle brand: " RESET);
    scanf("%29s", v.brand);
    printf(YELLOW "Enter vehicle model: " RESET);
    scanf("%29s", v.model);
    printf(YELLOW "Enter license plate number: " RESET);
    scanf("%14s", v.plate);
    printf(YELLOW "Enter year: " RESET);
    scanf("%4s", v.year);
    strcpy(v.lastServiceDate, "N/A");

    generateId("VEH", cnt + 1, v.id);

    fprintf(fp, "%s,%s,%s,%s,%s,%s,%s\n", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
    fclose(fp);

    printf(GREEN "Vehicle added successfully! ID: %s\n" RESET, v.id);
    char log_msg[100];
    sprintf(log_msg, "New vehicle %s %s added for customer %s.", v.brand, v.model, v.customerId);
    writeLog(log_msg);
    pauseScreen();
}

// Checks if a vehicle ID exists
int vehicleExists(const char *id) {
    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) return 0;

    Vehicle v;
    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
        if (!strcmp(v.id, id)) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// Edits an existing vehicle's information
void editVehicle() {
    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) {
        printf(RED "No vehicles found.\n" RESET);
        return;
    }

    Vehicle arr[200];
    int c = 0;
    char line[200];

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]", arr[c].id, arr[c].customerId, arr[c].brand, arr[c].model, arr[c].plate, arr[c].year, arr[c].lastServiceDate);
        c++;
    }
    fclose(fp);

    char id[10];
    printf(YELLOW "\nEnter Vehicle ID to edit: " RESET);
    scanf("%9s", id);

    int found = 0;
    for (int i = 0; i < c; i++) {
        if (!strcmp(arr[i].id, id)) {
            printf("Current Plate: %s\n", arr[i].plate);
            printf(YELLOW "Enter new plate (or press Enter to keep current): " RESET);
            clearInputBuffer();
            char newPlate[15];
            fgets(newPlate, sizeof(newPlate), stdin);
            newPlate[strcspn(newPlate, "\n")] = '\0';
            if (strlen(newPlate) > 0) strcpy(arr[i].plate, newPlate);
            
            printf("Current Last Service Date: %s\n", arr[i].lastServiceDate);
            printf(YELLOW "Enter new last service date (YYYY-MM-DD, or press Enter): " RESET);
            char newDate[15];
            fgets(newDate, sizeof(newDate), stdin);
            newDate[strcspn(newDate, "\n")] = '\0';
            if (strlen(newDate) > 0 && isDateValid(newDate)) {
                strcpy(arr[i].lastServiceDate, newDate);
            } else if (strlen(newDate) > 0) {
                printf(RED "Invalid date format. Keeping current date.\n" RESET);
            }

            found = 1;
            break;
        }
    }

    if (!found) {
        printf(RED "Vehicle ID not found.\n" RESET);
        pauseScreen();
        return;
    }

    fp = fopen(VEHICLE_FILE, "w");
    if (!fp) {
        printf(RED "Error opening vehicle file for writing.\n" RESET);
        return;
    }
    for (int i = 0; i < c; i++)
        fprintf(fp, "%s,%s,%s,%s,%s,%s,%s\n", arr[i].id, arr[i].customerId, arr[i].brand, arr[i].model, arr[i].plate, arr[i].year, arr[i].lastServiceDate);
    fclose(fp);

    printf(GREEN "Vehicle updated successfully!\n" RESET);
    char log_msg[100];
    sprintf(log_msg, "Vehicle with ID %s updated.", id);
    writeLog(log_msg);
    pauseScreen();
}

// Deletes a vehicle
void deleteVehicle() {
    FILE *fp_read = fopen(VEHICLE_FILE, "r");
    if (!fp_read) {
        printf(RED "No vehicles found.\n" RESET);
        return;
    }

    char id[10];
    printf(YELLOW "\nEnter Vehicle ID to delete: " RESET);
    scanf("%9s", id);

    FILE *fp_write = fopen("temp_vehicles.txt", "w");
    if (!fp_write) {
        fclose(fp_read);
        return;
    }

    Vehicle v;
    char line[200];
    int found = 0;

    while (fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
        if (strcmp(v.id, id) == 0) {
            found = 1;
        } else {
            fprintf(fp_write, "%s,%s,%s,%s,%s,%s,%s\n", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
        }
    }

    fclose(fp_read);
    fclose(fp_write);

    if (found) {
        remove(VEHICLE_FILE);
        rename("temp_vehicles.txt", VEHICLE_FILE);
        printf(GREEN "Vehicle deleted successfully!\n" RESET);
        char log_msg[100];
        sprintf(log_msg, "Vehicle with ID %s deleted.", id);
        writeLog(log_msg);
    } else {
        remove("temp_vehicles.txt");
        printf(RED "Vehicle ID not found.\n" RESET);
    }
    pauseScreen();
}

// Searches for a vehicle by plate or ID
void searchVehicle() {
    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) {
        printf(RED "No vehicles found.\n" RESET);
        return;
    }

    char search_term[15];
    printf(YELLOW "\nEnter vehicle plate or ID to search: " RESET);
    scanf("%14s", search_term);

    Vehicle v;
    char line[200];
    int found = 0;

    printf("\n" CYAN "--- Search Results ---" RESET "\n");
    printf("ID\tCust ID\tBrand\t\tModel\t\tPlate\t\tYear\tLast Service\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
        if (strstr(v.plate, search_term) || strstr(v.id, search_term)) {
            printf("%s\t%s\t%s\t\t%s\t\t%s\t%s\t%s\n", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
            found = 1;
        }
    }

    if (!found) {
        printf(YELLOW "No matching vehicles found.\n" RESET);
    }

    fclose(fp);
    pauseScreen();
}

// Views all vehicles
void viewAllVehicles() {
    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) {
        printf(RED "No vehicles found!\n" RESET);
        return;
    }
    Vehicle v;
    char line[200];
    printf("\n" CYAN "--- All Vehicles ---" RESET "\n");
    printf("ID\tCust ID\tBrand\t\tModel\t\tPlate\t\tYear\tLast Service\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
        printf("%s\t%s\t%s\t\t%s\t\t%s\t%s\t%s\n", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
    }
    fclose(fp);
    pauseScreen();
}

// Views all vehicles for a specific customer
void viewCustomerVehicles(const char* customerId) {
    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) {
        printf(RED "No vehicles found!\n" RESET);
        return;
    }
    Vehicle v;
    char line[200];
    int found = 0;
    printf("\n" CYAN "--- Your Vehicles ---" RESET "\n");
    printf("ID\tBrand\t\tModel\t\tPlate\t\tYear\tLast Service\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
        if (strcmp(v.customerId, customerId) == 0) {
            printf("%s\t%s\t\t%s\t\t%s\t%s\t%s\n", v.id, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
            found = 1;
        }
    }
    if (!found) {
        printf(YELLOW "No vehicles found for your account.\n" RESET);
    }
    fclose(fp);
    pauseScreen();
}


// Job Management Functions
// Checks if a mechanic ID exists
int mechanicExists(const char *id) {
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) return 0;

    User u;
    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%29[^,],%29[^,],%19[^\n]", u.id, u.username, u.password, u.role);
        if (!strcmp(u.id, id) && (strcmp(u.role, "SeniorMechanic") == 0 || strcmp(u.role, "JuniorMechanic") == 0)) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// Adds a new job to the system
void addJob(const char *currentUserId, const char *currentUserRole) {
    FILE *fp = fopen(JOB_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening job file.\n" RESET);
        return;
    }

    Job j;
    int cnt = fileLineCount(JOB_FILE);
    char customerId[10], vehicleId[10], mechanicId[10];

    printf(YELLOW "\nEnter Customer ID: " RESET);
    scanf("%9s", customerId);
    if (!customerExists(customerId)) {
        printf(RED "Customer ID not found.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    }
    strcpy(j.customerId, customerId);

    printf(YELLOW "Enter Vehicle ID: " RESET);
    scanf("%9s", vehicleId);
    if (!vehicleExists(vehicleId)) {
        printf(RED "Vehicle ID not found.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    }
    strcpy(j.vehicleId, vehicleId);
    
    // Assign mechanic based on user role
    if (strcmp(currentUserRole, "Admin") == 0 || strcmp(currentUserRole, "SeniorMechanic") == 0) {
        printf(YELLOW "Enter Mechanic ID: " RESET);
        scanf("%9s", mechanicId);
        if (!mechanicExists(mechanicId)) {
            printf(RED "Mechanic ID not found.\n" RESET);
            fclose(fp);
            pauseScreen();
            return;
        }
        strcpy(j.mechanicId, mechanicId);
    } else {
        // Junior Mechanics are assigned to themselves
        strcpy(j.mechanicId, currentUserId);
    }

    printf(YELLOW "Enter job description: " RESET);
    clearInputBuffer();
    fgets(j.description, sizeof(j.description), stdin);
    j.description[strcspn(j.description, "\n")] = '\0';
    
    j.estimatedCost = calculateAverageCost(j.description);
    j.laborCost = 0.0;
    j.partsCost = 0.0;
    strcpy(j.status, "Pending");
    strcpy(j.notes, "N/A");
    getCurrentDate(j.startDate);
    strcpy(j.endDate, "N/A");

    generateId("JOB", cnt + 1, j.id);

    fprintf(fp, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, j.estimatedCost, j.laborCost, j.partsCost, j.notes, j.startDate, j.endDate);
    fclose(fp);

    printf(GREEN "Job added successfully! ID: %s\n" RESET, j.id);
    char log_msg[100];
    sprintf(log_msg, "New job %s created for customer %s by %s.", j.id, j.customerId, j.mechanicId);
    writeLog(log_msg);
    createNotification(j.mechanicId, "You have been assigned a new job.");
    pauseScreen();
}

// Updates the status of a job
void updateJobStatus(const char *currentUserId, const char *currentUserRole) {
    FILE *fp_read = fopen(JOB_FILE, "r");
    if (!fp_read) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }

    Job arr[200];
    int c = 0;
    char line[500];

    while (fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", arr[c].id, arr[c].customerId, arr[c].vehicleId, arr[c].mechanicId, arr[c].description, arr[c].status, &arr[c].estimatedCost, &arr[c].laborCost, &arr[c].partsCost, arr[c].notes, arr[c].startDate, arr[c].endDate);
        c++;
    }
    fclose(fp_read);

    char jobId[10];
    printf(YELLOW "\nEnter Job ID to update: " RESET);
    scanf("%9s", jobId);

    int found = 0;
    for (int i = 0; i < c; i++) {
        // Check if the user has permission to update this job
        if (!strcmp(arr[i].id, jobId) && (getRoleLevel(currentUserRole) >= 2 || strcmp(arr[i].mechanicId, currentUserId) == 0)) {
            printf("Current Status: %s\n", arr[i].status);
            int status_choice;
            printf(YELLOW "Enter new status (1: Pending / 2: In-Progress / 3: Completed / 4: Cancelled): " RESET);
            scanf("%d", &status_choice);
            switch (status_choice) {
                case 1: strcpy(arr[i].status, "Pending"); break;
                case 2: strcpy(arr[i].status, "In-Progress"); break;
                case 3: 
                    strcpy(arr[i].status, "Completed"); 
                    getCurrentDate(arr[i].endDate);
                    // Update vehicle's last service date on completion
                    FILE *fp_veh_read = fopen(VEHICLE_FILE, "r");
                    if (fp_veh_read) {
                        FILE *fp_veh_write = fopen("temp_vehicles.txt", "w");
                        if (fp_veh_write) {
                            Vehicle v;
                            char veh_line[200];
                            while(fgets(veh_line, sizeof(veh_line), fp_veh_read)) {
                                sscanf(veh_line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
                                if (strcmp(v.id, arr[i].vehicleId) == 0) {
                                    strcpy(v.lastServiceDate, arr[i].endDate);
                                }
                                fprintf(fp_veh_write, "%s,%s,%s,%s,%s,%s,%s\n", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
                            }
                            fclose(fp_veh_read);
                            fclose(fp_veh_write);
                            remove(VEHICLE_FILE);
                            rename("temp_vehicles.txt", VEHICLE_FILE);
                            printf(GREEN "Vehicle last service date updated.\n" RESET);
                        } else {
                            fclose(fp_veh_read);
                        }
                    }
                    createNotification(arr[i].customerId, "Your job has been completed. Check your invoice.");
                    break;
                case 4: strcpy(arr[i].status, "Cancelled"); break;
                default:
                    printf(RED "Invalid status choice. Status not changed.\n" RESET);
                    pauseScreen();
                    return;
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        printf(RED "Job ID not found or you do not have permission to update it.\n" RESET);
        pauseScreen();
        return;
    }

    FILE *fp_write = fopen(JOB_FILE, "w");
    if (!fp_write) {
        printf(RED "Error opening job file for writing.\n" RESET);
        return;
    }
    for (int i = 0; i < c; i++) {
        fprintf(fp_write, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", arr[i].id, arr[i].customerId, arr[i].vehicleId, arr[i].mechanicId, arr[i].description, arr[i].status, arr[i].estimatedCost, arr[i].laborCost, arr[i].partsCost, arr[i].notes, arr[i].startDate, arr[i].endDate);
    }
    fclose(fp_write);

    printf(GREEN "Job status updated successfully!\n" RESET);
    char log_msg[100];
    sprintf(log_msg, "Status for job %s updated to %s.", jobId, arr[found-1].status);
    writeLog(log_msg);
    pauseScreen();
}

// Views all jobs assigned to a specific mechanic
void viewMyJobs(const char *mechanicId) {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found!\n" RESET);
        return;
    }
    Job j;
    char line[500];
    int found = 0;
    printf("\n" CYAN "--- Your Assigned Jobs ---" RESET "\n");
    printf("Job ID\tCust ID\tVehicle ID\tStatus\t\tDescription\t\t\t\tStart Date\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strcmp(j.mechanicId, mechanicId) == 0) {
            printf("%s\t%s\t%s\t\t%s\t\t%-30.30s\t%s\n", j.id, j.customerId, j.vehicleId, j.status, j.description, j.startDate);
            found = 1;
        }
    }
    if (!found) {
        printf(YELLOW "No jobs assigned to you.\n" RESET);
    }
    fclose(fp);
    pauseScreen();
}

// Views all jobs for a specific customer
void viewCustomerJobs(const char *customerId) {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found!\n" RESET);
        return;
    }
    Job j;
    char line[500];
    int found = 0;
    printf("\n" CYAN "--- Your Job History ---" RESET "\n");
    printf("Job ID\tVehicle ID\tStatus\t\tDescription\t\t\t\tStart Date\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strcmp(j.customerId, customerId) == 0) {
            printf("%s\t%s\t\t%s\t\t%-30.30s\t%s\n", j.id, j.vehicleId, j.status, j.description, j.startDate);
            found = 1;
        }
    }
    if (!found) {
        printf(YELLOW "No jobs found for your account.\n" RESET);
    }
    fclose(fp);
    pauseScreen();
}

// Searches for a job by ID or description
void searchJob() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found!\n" RESET);
        return;
    }

    char search_term[100];
    printf(YELLOW "\nEnter job ID or description to search: " RESET);
    clearInputBuffer();
    fgets(search_term, sizeof(search_term), stdin);
    search_term[strcspn(search_term, "\n")] = '\0';

    Job j;
    char line[500];
    int found = 0;

    printf("\n" CYAN "--- Search Results ---" RESET "\n");
    printf("Job ID\tCust ID\tVehicle ID\tStatus\t\tDescription\t\t\t\tStart Date\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strstr(j.id, search_term) || strstr(j.description, search_term)) {
            printf("%s\t%s\t%s\t\t%s\t\t%-30.30s\t%s\n", j.id, j.customerId, j.vehicleId, j.status, j.description, j.startDate);
            found = 1;
        }
    }

    if (!found) {
        printf(YELLOW "No matching jobs found.\n" RESET);
    }
    fclose(fp);
    pauseScreen();
}

// Views all jobs
void viewAllJobs() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found!\n" RESET);
        return;
    }
    Job j;
    char line[500];
    printf("\n" CYAN "--- All Jobs ---" RESET "\n");
    printf("Job ID\tCust ID\tMech ID\tStatus\t\tDescription\t\t\t\tStart Date\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        printf("%s\t%s\t%s\t%s\t\t%-30.30s\t%s\n", j.id, j.customerId, j.mechanicId, j.status, j.description, j.startDate);
    }
    fclose(fp);
    pauseScreen();
}

// Views all pending jobs
void viewPendingJobs() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found!\n" RESET);
        return;
    }
    Job j;
    char line[500];
    int found = 0;
    printf("\n" CYAN "--- Pending Jobs ---" RESET "\n");
    printf("Job ID\tCust ID\tMech ID\tDescription\t\t\t\tStart Date\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strcmp(j.status, "Pending") == 0) {
            printf("%s\t%s\t%s\t%-30.30s\t%s\n", j.id, j.customerId, j.mechanicId, j.description, j.startDate);
            found = 1;
        }
    }
    if (!found) {
        printf(YELLOW "No pending jobs found.\n" RESET);
    }
    fclose(fp);
    pauseScreen();
}

// Views all completed jobs
void viewCompletedJobs() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found!\n" RESET);
        return;
    }
    Job j;
    char line[500];
    int found = 0;
    printf("\n" CYAN "--- Completed Jobs ---" RESET "\n");
    printf("Job ID\tCust ID\tMech ID\tDescription\t\t\t\tEnd Date\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strcmp(j.status, "Completed") == 0) {
            printf("%s\t%s\t%s\t%-30.30s\t%s\n", j.id, j.customerId, j.mechanicId, j.description, j.endDate);
            found = 1;
        }
    }
    if (!found) {
        printf(YELLOW "No completed jobs found.\n" RESET);
    }
    fclose(fp);
    pauseScreen();
}


// Invoice Management Functions
// Generates an invoice for a completed job
void generateInvoice() {
    FILE *fp_job_read = fopen(JOB_FILE, "r");
    if (!fp_job_read) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }

    char jobId[10];
    printf(YELLOW "\nEnter Job ID to generate invoice: " RESET);
    scanf("%9s", jobId);

    Job j;
    char job_line[500];
    int job_found = 0;
    while (fgets(job_line, sizeof(job_line), fp_job_read)) {
        sscanf(job_line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strcmp(j.id, jobId) == 0) {
            job_found = 1;
            break;
        }
    }
    fclose(fp_job_read);

    if (!job_found) {
        printf(RED "Job ID not found.\n" RESET);
        pauseScreen();
        return;
    }

    if (strcmp(j.status, "Completed") != 0) {
        printf(RED "Invoice can only be generated for completed jobs.\n" RESET);
        pauseScreen();
        return;
    }

    FILE *fp_inv = fopen(INVOICE_FILE, "a+");
    if (!fp_inv) {
        printf(RED "Error opening invoice file.\n" RESET);
        return;
    }

    Invoice inv;
    int cnt = fileLineCount(INVOICE_FILE);
    generateId("INV", cnt + 1, inv.id);
    strcpy(inv.jobId, jobId);
    getCurrentDate(inv.date);

    // Calculate total cost
    inv.totalCost = j.laborCost + j.partsCost;
    printf(YELLOW "Total Cost (Labor + Parts): %.2f\n" RESET, inv.totalCost);
    printf(YELLOW "Enter discount percentage (e.g., 10 for 10%%): " RESET);
    scanf("%f", &inv.discount);
    if (inv.discount < 0 || inv.discount > 100) {
        printf(RED "Invalid discount. Setting to 0.\n" RESET);
        inv.discount = 0.0;
    }
    inv.total = inv.totalCost - (inv.totalCost * inv.discount / 100.0);

    // Get customer and vehicle details for printing
    Customer cust;
    FILE *fp_cust = fopen(CUSTOMER_FILE, "r");
    if (fp_cust) {
        char cust_line[200];
        while (fgets(cust_line, sizeof(cust_line), fp_cust)) {
            sscanf(cust_line, "%9[^,],%49[^,],%14[^,],%49[^\n]", cust.id, cust.name, cust.phone, cust.email);
            if (strcmp(cust.id, j.customerId) == 0) break;
        }
        fclose(fp_cust);
    }

    Vehicle veh;
    FILE *fp_veh = fopen(VEHICLE_FILE, "r");
    if (fp_veh) {
        char veh_line[200];
        while (fgets(veh_line, sizeof(veh_line), fp_veh)) {
            sscanf(veh_line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]", veh.id, veh.customerId, veh.brand, veh.model, veh.plate, veh.year, veh.lastServiceDate);
            if (strcmp(veh.id, j.vehicleId) == 0) break;
        }
        fclose(fp_veh);
    }
    
    // Print the invoice
    printf("\n" CYAN "--- Invoice %s ---" RESET "\n", inv.id);
    printf(BLUE "-----------------------------------" RESET "\n");
    printf(MAGENTA "Invoice Date: %s\n" RESET, inv.date);
    printf(MAGENTA "Job ID: %s\n" RESET, inv.jobId);
    printf(MAGENTA "-----------------------------------\n" RESET);
    printf(YELLOW "Customer: %s (ID: %s)\n" RESET, cust.name, cust.id);
    printf(YELLOW "Vehicle: %s %s (%s)\n" RESET, veh.year, veh.brand, veh.plate);
    printf(YELLOW "Description: %s\n" RESET, j.description);
    printf(MAGENTA "-----------------------------------\n" RESET);
    printf(CYAN "Labor Cost: \t\t%.2f\n" RESET, j.laborCost);
    printf(CYAN "Parts Cost: \t\t%.2f\n" RESET, j.partsCost);
    printf(CYAN "-----------------------------------\n" RESET);
    printf(GREEN "Subtotal: \t\t%.2f\n" RESET, inv.totalCost);
    printf(GREEN "Discount (%.0f%%): \t\t-%.2f\n" RESET, inv.discount, inv.totalCost * inv.discount / 100.0);
    printf(GREEN "Total Due: \t\t%.2f\n" RESET, inv.total);
    printf(BLUE "-----------------------------------" RESET "\n");

    // Save invoice and finance transaction
    fprintf(fp_inv, "%s,%s,%.2f,%.2f,%.2f,%s\n", inv.id, inv.jobId, inv.totalCost, inv.discount, inv.total, inv.date);
    fclose(fp_inv);

    FILE *fp_finance = fopen(FINANCE_FILE, "a");
    if (fp_finance) {
        Transaction t;
        int t_cnt = fileLineCount(FINANCE_FILE);
        generateId("TRA", t_cnt + 1, t.id);
        strcpy(t.type, "Revenue");
        t.amount = inv.total;
        strcpy(t.date, inv.date);
        sprintf(t.description, "Payment for Invoice %s (Job %s)", inv.id, inv.jobId);
        fprintf(fp_finance, "%s,%s,%.2f,%s,%s\n", t.id, t.type, t.amount, t.date, t.description);
        fclose(fp_finance);
    }

    printf(GREEN "Invoice generated and saved successfully!\n" RESET);
    char log_msg[100];
    sprintf(log_msg, "Invoice %s generated for Job %s.", inv.id, inv.jobId);
    writeLog(log_msg);
    pauseScreen();
}

// Views all invoices
void viewAllInvoices() {
    FILE *fp = fopen(INVOICE_FILE, "r");
    if (!fp) {
        printf(RED "No invoices found!\n" RESET);
        return;
    }
    Invoice inv;
    char line[200];
    printf("\n" CYAN "--- All Invoices ---" RESET "\n");
    printf("Inv ID\tJob ID\tTotal Cost\tDiscount\tTotal\tDate\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%f,%f,%f,%14[^\n]", inv.id, inv.jobId, &inv.totalCost, &inv.discount, &inv.total, inv.date);
        printf("%s\t%s\t%.2f\t\t%.2f\t\t%.2f\t%s\n", inv.id, inv.jobId, inv.totalCost, inv.discount, inv.total, inv.date);
    }
    fclose(fp);
    pauseScreen();
}


// Inventory Management Functions
// Adds parts used to a job
void addPartToJob() {
    FILE *fp_read = fopen(JOB_FILE, "r");
    if (!fp_read) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }

    Job arr[200];
    int c = 0;
    char line[500];
    while (fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", arr[c].id, arr[c].customerId, arr[c].vehicleId, arr[c].mechanicId, arr[c].description, arr[c].status, &arr[c].estimatedCost, &arr[c].laborCost, &arr[c].partsCost, arr[c].notes, arr[c].startDate, arr[c].endDate);
        c++;
    }
    fclose(fp_read);

    char jobId[10];
    printf(YELLOW "\nEnter Job ID to add parts to: " RESET);
    scanf("%9s", jobId);

    int found = 0;
    for (int i = 0; i < c; i++) {
        if (!strcmp(arr[i].id, jobId) && (strcmp(arr[i].status, "In-Progress") == 0 || strcmp(arr[i].status, "Pending") == 0)) {
            found = 1;
            char partId[10];
            int quantity;
            float price;

            printf(YELLOW "Enter Part ID: " RESET);
            scanf("%9s", partId);
            if (!partExists(partId)) {
                printf(RED "Part ID not found.\n" RESET);
                pauseScreen();
                return;
            }

            printf(YELLOW "Enter quantity used: " RESET);
            scanf("%d", &quantity);
            if (quantity <= 0) {
                printf(RED "Invalid quantity.\n" RESET);
                pauseScreen();
                return;
            }

            price = getPartPrice(partId);
            arr[i].partsCost += price * quantity;

            // Log the part usage
            FILE *fp_job_part = fopen(JOB_PART_FILE, "a");
            if (fp_job_part) {
                fprintf(fp_job_part, "%s,%s,%d,%.2f\n", jobId, partId, quantity, price);
                fclose(fp_job_part);
                printf(GREEN "Part added to job successfully.\n" RESET);
                deductPartFromInventory(partId, quantity);
            } else {
                printf(RED "Error logging job part usage.\n" RESET);
            }
            break;
        }
    }

    if (!found) {
        printf(RED "Job ID not found or job is not in a modifiable status.\n" RESET);
        pauseScreen();
        return;
    }

    FILE *fp_write = fopen(JOB_FILE, "w");
    if (!fp_write) return;
    for (int i = 0; i < c; i++) {
        fprintf(fp_write, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", arr[i].id, arr[i].customerId, arr[i].vehicleId, arr[i].mechanicId, arr[i].description, arr[i].status, arr[i].estimatedCost, arr[i].laborCost, arr[i].partsCost, arr[i].notes, arr[i].startDate, arr[i].endDate);
    }
    fclose(fp_write);

    char log_msg[100];
    sprintf(log_msg, "Parts added to job %s.", jobId);
    writeLog(log_msg);
    pauseScreen();
}

// Updates labor cost for a job
void updateJobLaborCost() {
    FILE *fp_read = fopen(JOB_FILE, "r");
    if (!fp_read) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }

    Job arr[200];
    int c = 0;
    char line[500];
    while (fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", arr[c].id, arr[c].customerId, arr[c].vehicleId, arr[c].mechanicId, arr[c].description, arr[c].status, &arr[c].estimatedCost, &arr[c].laborCost, &arr[c].partsCost, arr[c].notes, arr[c].startDate, arr[c].endDate);
        c++;
    }
    fclose(fp_read);

    char jobId[10];
    printf(YELLOW "\nEnter Job ID to update labor cost: " RESET);
    scanf("%9s", jobId);

    int found = 0;
    for (int i = 0; i < c; i++) {
        if (!strcmp(arr[i].id, jobId) && (strcmp(arr[i].status, "In-Progress") == 0 || strcmp(arr[i].status, "Pending") == 0)) {
            found = 1;
            float labor;
            printf(YELLOW "Enter new labor cost: " RESET);
            scanf("%f", &labor);
            if (labor >= 0) {
                arr[i].laborCost = labor;
                printf(GREEN "Labor cost updated successfully.\n" RESET);
            } else {
                printf(RED "Invalid labor cost. Labor cost not updated.\n" RESET);
            }
            break;
        }
    }

    if (!found) {
        printf(RED "Job ID not found or job is not in a modifiable status.\n" RESET);
        pauseScreen();
        return;
    }

    FILE *fp_write = fopen(JOB_FILE, "w");
    if (!fp_write) return;
    for (int i = 0; i < c; i++) {
        fprintf(fp_write, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", arr[i].id, arr[i].customerId, arr[i].vehicleId, arr[i].mechanicId, arr[i].description, arr[i].status, arr[i].estimatedCost, arr[i].laborCost, arr[i].partsCost, arr[i].notes, arr[i].startDate, arr[i].endDate);
    }
    fclose(fp_write);

    char log_msg[100];
    sprintf(log_msg, "Labor cost updated for job %s.", jobId);
    writeLog(log_msg);
    pauseScreen();
}


// Adds a new part to inventory
void addPart() {
    FILE *fp = fopen(PART_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening part file.\n" RESET);
        return;
    }

    Part p;
    int cnt = fileLineCount(PART_FILE);

    printf(YELLOW "\nEnter part name: " RESET);
    clearInputBuffer();
    fgets(p.name, sizeof(p.name), stdin);
    p.name[strcspn(p.name, "\n")] = '\0';
    
    printf(YELLOW "Enter quantity: " RESET);
    scanf("%d", &p.quantity);
    printf(YELLOW "Enter price per unit: " RESET);
    scanf("%f", &p.price);
    printf(YELLOW "Enter supplier ID: " RESET);
    scanf("%9s", p.supplierId);
    if (!supplierExists(p.supplierId)) {
        printf(RED "Supplier ID not found. Please add the supplier first.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    }
    printf(YELLOW "Enter reorder level: " RESET);
    scanf("%d", &p.reorderLevel);

    generateId("PRT", cnt + 1, p.id);

    fprintf(fp, "%s,%s,%d,%.2f,%s,%d\n", p.id, p.name, p.quantity, p.price, p.supplierId, p.reorderLevel);
    fclose(fp);

    printf(GREEN "Part added successfully! ID: %s\n" RESET, p.id);
    char log_msg[100];
    sprintf(log_msg, "New part %s added with ID %s.", p.name, p.id);
    writeLog(log_msg);
    pauseScreen();
}

// Edits an existing part
void editPart() {
    FILE *fp_read = fopen(PART_FILE, "r");
    if (!fp_read) {
        printf(RED "No parts found.\n" RESET);
        return;
    }
    
    Part arr[200];
    int c = 0;
    char line[200];
    while(fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d", arr[c].id, arr[c].name, &arr[c].quantity, &arr[c].price, arr[c].supplierId, &arr[c].reorderLevel);
        c++;
    }
    fclose(fp_read);

    char partId[10];
    printf(YELLOW "\nEnter Part ID to edit: " RESET);
    scanf("%9s", partId);
    
    int found = 0;
    for(int i=0; i<c; i++) {
        if(strcmp(arr[i].id, partId) == 0) {
            printf("Current Quantity: %d\n", arr[i].quantity);
            printf(YELLOW "Enter new quantity: " RESET);
            scanf("%d", &arr[i].quantity);
            
            printf("Current Price: %.2f\n", arr[i].price);
            printf(YELLOW "Enter new price: " RESET);
            scanf("%f", &arr[i].price);
            
            printf("Current Reorder Level: %d\n", arr[i].reorderLevel);
            printf(YELLOW "Enter new reorder level: " RESET);
            scanf("%d", &arr[i].reorderLevel);
            
            found = 1;
            break;
        }
    }
    
    if(!found) {
        printf(RED "Part ID not found.\n" RESET);
        pauseScreen();
        return;
    }
    
    FILE *fp_write = fopen(PART_FILE, "w");
    if (!fp_write) return;
    for (int i = 0; i < c; i++) {
        fprintf(fp_write, "%s,%s,%d,%.2f,%s,%d\n", arr[i].id, arr[i].name, arr[i].quantity, arr[i].price, arr[i].supplierId, arr[i].reorderLevel);
    }
    fclose(fp_write);
    
    printf(GREEN "Part updated successfully!\n" RESET);
    char log_msg[100];
    sprintf(log_msg, "Part with ID %s updated.", partId);
    writeLog(log_msg);
    pauseScreen();
}

// Views all parts in inventory
void viewInventory() {
    FILE *fp = fopen(PART_FILE, "r");
    if (!fp) {
        printf(RED "No parts found!\n" RESET);
        return;
    }
    Part p;
    char line[200];
    printf("\n" CYAN "--- Current Inventory ---" RESET "\n");
    printf("ID\tName\t\t\tQuantity\tPrice\t\tSupplier ID\tReorder Level\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d", p.id, p.name, &p.quantity, &p.price, p.supplierId, &p.reorderLevel);
        printf("%s\t%-20.20s\t%d\t\t%.2f\t\t%s\t\t%d\n", p.id, p.name, p.quantity, p.price, p.supplierId, p.reorderLevel);
    }
    fclose(fp);
    pauseScreen();
}

// Checks if a part ID exists
int partExists(const char *id) {
    FILE *fp = fopen(PART_FILE, "r");
    if (!fp) return 0;
    Part p;
    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d", p.id, p.name, &p.quantity, &p.price, p.supplierId, &p.reorderLevel);
        if (!strcmp(p.id, id)) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// Gets the price of a part
float getPartPrice(const char *partId) {
    FILE *fp = fopen(PART_FILE, "r");
    if (!fp) return 0.0;
    Part p;
    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d", p.id, p.name, &p.quantity, &p.price, p.supplierId, &p.reorderLevel);
        if (!strcmp(p.id, partId)) {
            fclose(fp);
            return p.price;
        }
    }
    fclose(fp);
    return 0.0;
}

// Deducts parts from inventory and checks for low stock
void deductPartFromInventory(const char *partId, int quantity) {
    FILE *fp_read = fopen(PART_FILE, "r");
    if (!fp_read) return;

    Part arr[200];
    int c = 0;
    char line[200];
    while(fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d", arr[c].id, arr[c].name, &arr[c].quantity, &arr[c].price, arr[c].supplierId, &arr[c].reorderLevel);
        c++;
    }
    fclose(fp_read);

    int found_idx = -1;
    for(int i=0; i<c; i++) {
        if(strcmp(arr[i].id, partId) == 0) {
            found_idx = i;
            if (arr[i].quantity >= quantity) {
                arr[i].quantity -= quantity;
                printf(GREEN "Successfully deducted %d units of part %s.\n" RESET, quantity, partId);
                // Check if stock is now low
                if (arr[i].quantity <= arr[i].reorderLevel) {
                    char msg[100];
                    sprintf(msg, "Low stock alert: Part %s (%s) is below reorder level. Current stock: %d", arr[i].id, arr[i].name, arr[i].quantity);
                    writeLog(msg);
                    FILE *fp_reorder = fopen(REORDER_LIST_FILE, "a+");
                    if (fp_reorder) {
                        ReorderItem ri;
                        strcpy(ri.partId, arr[i].id);
                        ri.neededQuantity = arr[i].reorderLevel - arr[i].quantity + 1;
                        getCurrentDate(ri.date);
                        fprintf(fp_reorder, "%s,%d,%s\n", ri.partId, ri.neededQuantity, ri.date);
                        fclose(fp_reorder);
                    }
                    printf(YELLOW "Low stock alert triggered for part %s.\n" RESET, arr[i].id);
                }
            } else {
                printf(RED "Not enough stock to deduct %d units of part %s. Current stock: %d\n" RESET, quantity, partId, arr[i].quantity);
                // Don't update the file if there's an error
                return;
            }
            break;
        }
    }
    
    if (found_idx == -1) {
        printf(RED "Part ID not found in inventory.\n" RESET);
        return;
    }
    
    FILE *fp_write = fopen(PART_FILE, "w");
    if (!fp_write) return;
    for (int i = 0; i < c; i++) {
        fprintf(fp_write, "%s,%s,%d,%.2f,%s,%d\n", arr[i].id, arr[i].name, arr[i].quantity, arr[i].price, arr[i].supplierId, arr[i].reorderLevel);
    }
    fclose(fp_write);
}

// Checks for low stock and creates a reorder list
void checkLowStock() {
    FILE *fp = fopen(PART_FILE, "r");
    if (!fp) {
        printf(RED "No parts found to check.\n" RESET);
        return;
    }
    
    FILE *fp_reorder = fopen(REORDER_LIST_FILE, "w");
    if (!fp_reorder) {
        fclose(fp);
        return;
    }
    
    Part p;
    char line[200];
    int low_stock_found = 0;
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d", p.id, p.name, &p.quantity, &p.price, p.supplierId, &p.reorderLevel);
        if (p.quantity <= p.reorderLevel) {
            low_stock_found = 1;
            ReorderItem ri;
            strcpy(ri.partId, p.id);
            ri.neededQuantity = p.reorderLevel - p.quantity + 1;
            getCurrentDate(ri.date);
            fprintf(fp_reorder, "%s,%d,%s\n", ri.partId, ri.neededQuantity, ri.date);
        }
    }
    
    fclose(fp);
    fclose(fp_reorder);
    
    if (low_stock_found) {
        printf(YELLOW "Low stock items have been added to the reorder list.\n" RESET);
        writeLog("Low stock check completed. Reorder list updated.");
    } else {
        printf(GREEN "No low stock items found.\n" RESET);
        remove(REORDER_LIST_FILE);
    }
    pauseScreen();
}

// Views the reorder list
void viewReorderList() {
    FILE *fp = fopen(REORDER_LIST_FILE, "r");
    if (!fp) {
        printf(GREEN "Reorder list is empty.\n" RESET);
        return;
    }
    
    ReorderItem ri;
    char line[200];
    printf("\n" CYAN "--- Reorder List ---" RESET "\n");
    printf("Part ID\tNeeded Quantity\tDate\n");
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%d,%14[^\n]", ri.partId, &ri.neededQuantity, ri.date);
        printf("%s\t%d\t\t%s\n", ri.partId, ri.neededQuantity, ri.date);
    }
    fclose(fp);
    pauseScreen();
}

// Clears the reorder list
void clearReorderList() {
    if (remove(REORDER_LIST_FILE) == 0) {
        printf(GREEN "Reorder list cleared successfully.\n" RESET);
        writeLog("Reorder list cleared.");
    } else {
        printf(RED "Error clearing reorder list or file does not exist.\n" RESET);
    }
    pauseScreen();
}


// Supplier Management Functions
// Adds a new supplier
void addSupplier() {
    FILE *fp = fopen(SUPPLIER_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening supplier file.\n" RESET);
        return;
    }

    Supplier s;
    int cnt = fileLineCount(SUPPLIER_FILE);

    printf(YELLOW "\nEnter supplier name: " RESET);
    clearInputBuffer();
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = '\0';

    printf(YELLOW "Enter phone number: " RESET);
    scanf("%14s", s.phone);
    printf(YELLOW "Enter email: " RESET);
    scanf("%49s", s.email);

    generateId("SUP", cnt + 1, s.id);

    fprintf(fp, "%s,%s,%s,%s\n", s.id, s.name, s.phone, s.email);
    fclose(fp);

    printf(GREEN "Supplier added successfully! ID: %s\n" RESET, s.id);
    char log_msg[100];
    sprintf(log_msg, "New supplier %s added with ID %s.", s.name, s.id);
    writeLog(log_msg);
    pauseScreen();
}

// Views all suppliers
void viewAllSuppliers() {
    FILE *fp = fopen(SUPPLIER_FILE, "r");
    if (!fp) {
        printf(RED "No suppliers found!\n" RESET);
        return;
    }
    Supplier s;
    char line[200];
    printf("\n" CYAN "--- All Suppliers ---" RESET "\n");
    printf("ID\tName\t\tPhone\t\tEmail\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", s.id, s.name, s.phone, s.email);
        printf("%s\t%s\t\t%s\t%s\n", s.id, s.name, s.phone, s.email);
    }
    fclose(fp);
    pauseScreen();
}

// Checks if a supplier ID exists
int supplierExists(const char *id) {
    FILE *fp = fopen(SUPPLIER_FILE, "r");
    if (!fp) return 0;
    Supplier s;
    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", s.id, s.name, s.phone, s.email);
        if (!strcmp(s.id, id)) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}


// Appointment Management Functions
// Schedules a new appointment
void scheduleAppointment(const char *currentUserId, const char *currentUserRole) {
    FILE *fp = fopen(APPOINTMENT_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening appointment file.\n" RESET);
        return;
    }

    Appointment a;
    int cnt = fileLineCount(APPOINTMENT_FILE);
    char customerId[10];

    if (strcmp(currentUserRole, "Customer") == 0) {
        strcpy(customerId, currentUserId);
    } else {
        printf(YELLOW "\nEnter Customer ID for the appointment: " RESET);
        scanf("%9s", customerId);
    }

    if (!customerExists(customerId)) {
        printf(RED "Customer ID not found.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    }
    strcpy(a.customerId, customerId);

    printf(YELLOW "Enter Vehicle ID: " RESET);
    scanf("%9s", a.vehicleId);
    if (!vehicleExists(a.vehicleId)) {
        printf(RED "Vehicle ID not found.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    }

    printf(YELLOW "Enter appointment date (YYYY-MM-DD): " RESET);
    scanf("%14s", a.date);
    if (!isDateValid(a.date)) {
        printf(RED "Invalid date format. Appointment not scheduled.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    }

    printf(YELLOW "Enter appointment time (HH:MM): " RESET);
    scanf("%9s", a.time);

    printf(YELLOW "Enter reason for appointment: " RESET);
    clearInputBuffer();
    fgets(a.description, sizeof(a.description), stdin);
    a.description[strcspn(a.description, "\n")] = '\0';
    
    strcpy(a.status, "Scheduled");

    generateId("APP", cnt + 1, a.id);

    fprintf(fp, "%s,%s,%s,%s,%s,%s,%s\n", a.id, a.customerId, a.vehicleId, a.date, a.time, a.description, a.status);
    fclose(fp);

    printf(GREEN "Appointment scheduled successfully! ID: %s\n" RESET, a.id);
    char log_msg[100];
    sprintf(log_msg, "Appointment %s scheduled for customer %s.", a.id, a.customerId);
    writeLog(log_msg);
    // Notify admin/senior mechanic of new appointment
    createNotification("ADM0001", "A new appointment has been scheduled.");
    pauseScreen();
}

// Views all appointments
void viewAllAppointments() {
    FILE *fp = fopen(APPOINTMENT_FILE, "r");
    if (!fp) {
        printf(RED "No appointments found!\n" RESET);
        return;
    }
    Appointment a;
    char line[200];
    printf("\n" CYAN "--- All Appointments ---" RESET "\n");
    printf("ID\tCust ID\tVehicle ID\tDate\t\tTime\tDescription\t\t\tStatus\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%14[^,],%9[^,],%99[^,],%19[^\n]", a.id, a.customerId, a.vehicleId, a.date, a.time, a.description, a.status);
        printf("%s\t%s\t%s\t\t%s\t%s\t%-20.20s\t%s\n", a.id, a.customerId, a.vehicleId, a.date, a.time, a.description, a.status);
    }
    fclose(fp);
    pauseScreen();
}

// Schedules a service reminder for a customer
void scheduleServiceReminder() {
    FILE *fp_read = fopen(VEHICLE_FILE, "r");
    if (!fp_read) {
        printf(RED "No vehicles found to schedule reminders.\n" RESET);
        return;
    }
    
    FILE *fp_write = fopen(REMINDERS_FILE, "a+");
    if (!fp_write) {
        fclose(fp_read);
        printf(RED "Error opening reminders file.\n" RESET);
        return;
    }
    
    Vehicle v;
    char line[200];
    int reminders_scheduled = 0;
    while(fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
        
        char nextServiceDate[15];
        calculateNextServiceDate(v.lastServiceDate, nextServiceDate);
        
        char current_date[15];
        getCurrentDate(current_date);
        
        int days_until = daysBetweenDates(current_date, nextServiceDate);
        
        if (days_until > 0 && days_until <= 30) {
            Reminder r;
            int cnt = fileLineCount(REMINDERS_FILE);
            generateId("REM", cnt + 1, r.id);
            strcpy(r.customerId, v.customerId);
            sprintf(r.message, "Reminder: Your %s %s (%s) is due for service on %s.", v.year, v.brand, v.plate, nextServiceDate);
            strcpy(r.date, current_date);
            strcpy(r.status, "Scheduled");
            
            fprintf(fp_write, "%s,%s,%s,%s,%s\n", r.id, r.customerId, r.message, r.date, r.status);
            reminders_scheduled++;
            createNotification(r.customerId, r.message);
        }
    }
    
    fclose(fp_read);
    fclose(fp_write);
    
    printf(GREEN "Scheduled %d service reminders.\n" RESET, reminders_scheduled);
    char log_msg[100];
    sprintf(log_msg, "%d service reminders scheduled.", reminders_scheduled);
    writeLog(log_msg);
    pauseScreen();
}

// Views all service reminders
void viewAllReminders() {
    FILE *fp = fopen(REMINDERS_FILE, "r");
    if (!fp) {
        printf(GREEN "No reminders found.\n" RESET);
        return;
    }
    
    Reminder r;
    char line[200];
    printf("\n" CYAN "--- Service Reminders ---" RESET "\n");
    printf("ID\tCust ID\tDate\t\tStatus\tMessage\n");
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%199[^,],%14[^,],%19[^\n]", r.id, r.customerId, r.message, r.date, r.status);
        printf("%s\t%s\t%s\t%s\t%s\n", r.id, r.customerId, r.date, r.status, r.message);
    }
    
    fclose(fp);
    pauseScreen();
}


// Financial Management Functions
// Adds an expense to the system
void addExpense() {
    FILE *fp_exp = fopen(EXPENSES_FILE, "a+");
    if (!fp_exp) {
        printf(RED "Error opening expenses file.\n" RESET);
        return;
    }
    FILE *fp_finance = fopen(FINANCE_FILE, "a");
    if (!fp_finance) {
        fclose(fp_exp);
        printf(RED "Error opening finance file.\n" RESET);
        return;
    }
    
    Transaction t;
    int cnt = fileLineCount(FINANCE_FILE);

    printf(YELLOW "\nEnter expense amount: " RESET);
    scanf("%f", &t.amount);
    printf(YELLOW "Enter expense date (YYYY-MM-DD): " RESET);
    scanf("%14s", t.date);
    if (!isDateValid(t.date)) {
        printf(RED "Invalid date format. Expense not added.\n" RESET);
        fclose(fp_exp);
        fclose(fp_finance);
        return;
    }
    printf(YELLOW "Enter expense description: " RESET);
    clearInputBuffer();
    fgets(t.description, sizeof(t.description), stdin);
    t.description[strcspn(t.description, "\n")] = '\0';
    
    strcpy(t.type, "Expense");
    generateId("TRA", cnt + 1, t.id);
    
    fprintf(fp_exp, "%s,%.2f,%s,%s\n", t.id, t.amount, t.date, t.description);
    fprintf(fp_finance, "%s,%s,%.2f,%s,%s\n", t.id, t.type, t.amount, t.date, t.description);
    
    fclose(fp_exp);
    fclose(fp_finance);
    
    printf(GREEN "Expense added successfully!\n" RESET);
    char log_msg[100];
    sprintf(log_msg, "New expense of %.2f added.", t.amount);
    writeLog(log_msg);
    pauseScreen();
}

// Views all expenses
void viewAllExpenses() {
    FILE *fp = fopen(EXPENSES_FILE, "r");
    if (!fp) {
        printf(RED "No expenses found!\n" RESET);
        return;
    }
    
    char id[10], date[15], description[100];
    float amount;
    char line[200];
    printf("\n" CYAN "--- All Expenses ---" RESET "\n");
    printf("ID\tAmount\tDate\t\tDescription\n");
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%f,%14[^,],%99[^\n]", id, &amount, date, description);
        printf("%s\t%.2f\t%s\t%s\n", id, amount, date, description);
    }
    
    fclose(fp);
    pauseScreen();
}

// Generates a profit/loss report
void generateProfitLossReport() {
    FILE *fp = fopen(FINANCE_FILE, "r");
    if (!fp) {
        printf(RED "No financial data found.\n" RESET);
        return;
    }
    
    float total_revenue = 0.0;
    float total_expenses = 0.0;
    
    Transaction t;
    char line[200];
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%19[^,],%f,%14[^,],%99[^\n]", t.id, t.type, &t.amount, t.date, t.description);
        if (strcmp(t.type, "Revenue") == 0) {
            total_revenue += t.amount;
        } else if (strcmp(t.type, "Expense") == 0) {
            total_expenses += t.amount;
        }
    }
    
    fclose(fp);
    
    printf("\n" CYAN "--- Profit & Loss Report ---" RESET "\n");
    printf(BLUE "------------------------------" RESET "\n");
    printf(GREEN "Total Revenue: \t\t%.2f\n" RESET, total_revenue);
    printf(RED "Total Expenses: \t\t%.2f\n" RESET, total_expenses);
    printf(BLUE "------------------------------\n" RESET);
    float profit = total_revenue - total_expenses;
    if (profit >= 0) {
        printf(GREEN "Net Profit: \t\t%.2f\n" RESET, profit);
    } else {
        printf(RED "Net Loss: \t\t%.2f\n" RESET, profit);
    }
    printf(BLUE "------------------------------" RESET "\n");
    pauseScreen();
}


// Reporting Functions
void viewJobSummaryReport() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found!\n" RESET);
        return;
    }
    
    Job j;
    char line[500];
    int total_jobs = 0, pending = 0, in_progress = 0, completed = 0, cancelled = 0;
    float total_revenue = 0.0, total_parts = 0.0, total_labor = 0.0;
    
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        total_jobs++;
        if (strcmp(j.status, "Pending") == 0) pending++;
        if (strcmp(j.status, "In-Progress") == 0) in_progress++;
        if (strcmp(j.status, "Completed") == 0) {
            completed++;
            total_revenue += j.laborCost + j.partsCost;
            total_parts += j.partsCost;
            total_labor += j.laborCost;
        }
        if (strcmp(j.status, "Cancelled") == 0) cancelled++;
    }
    fclose(fp);
    
    printf("\n" CYAN "--- Job Summary Report ---" RESET "\n");
    printf(BLUE "------------------------------" RESET "\n");
    printf(MAGENTA "Total Jobs: \t\t%d\n" RESET, total_jobs);
    printf(YELLOW "Pending: \t\t%d\n" RESET, pending);
    printf(YELLOW "In-Progress: \t\t%d\n" RESET, in_progress);
    printf(GREEN "Completed: \t\t%d\n" RESET, completed);
    printf(RED "Cancelled: \t\t%d\n" RESET, cancelled);
    printf(BLUE "------------------------------\n" RESET);
    printf(GREEN "Total Revenue: \t\t%.2f\n" RESET, total_revenue);
    printf(GREEN "Total Parts Cost: \t%.2f\n" RESET, total_parts);
    printf(GREEN "Total Labor Cost: \t%.2f\n" RESET, total_labor);
    printf(BLUE "------------------------------" RESET "\n");
    pauseScreen();
}

void mechanicPerformanceReport() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found!\n" RESET);
        return;
    }
    
    char mechanicId[10], description[100];
    float labor_cost, parts_cost;
    int completed_jobs = 0;
    float total_revenue = 0.0;
    
    printf(YELLOW "\nEnter Mechanic ID for report: " RESET);
    scanf("%9s", mechanicId);
    
    Job j;
    char line[500];
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strcmp(j.mechanicId, mechanicId) == 0 && strcmp(j.status, "Completed") == 0) {
            completed_jobs++;
            total_revenue += j.laborCost + j.partsCost;
        }
    }
    fclose(fp);
    
    printf("\n" CYAN "--- Mechanic Performance Report ---" RESET "\n");
    printf(BLUE "-----------------------------------" RESET "\n");
    printf(MAGENTA "Mechanic ID: \t\t%s\n" RESET, mechanicId);
    printf(GREEN "Completed Jobs: \t%d\n" RESET, completed_jobs);
    printf(GREEN "Total Revenue Generated: %.2f\n" RESET, total_revenue);
    printf(BLUE "-----------------------------------" RESET "\n");
    
    pauseScreen();
}

void customerLifetimeValueReport() {
    FILE *fp_job = fopen(JOB_FILE, "r");
    if (!fp_job) {
        printf(RED "No jobs found!\n" RESET);
        return;
    }
    
    char customerId[10];
    float lifetime_value = 0.0;
    
    printf(YELLOW "\nEnter Customer ID for report: " RESET);
    scanf("%9s", customerId);
    
    Job j;
    char line[500];
    while(fgets(line, sizeof(line), fp_job)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strcmp(j.customerId, customerId) == 0 && strcmp(j.status, "Completed") == 0) {
            lifetime_value += j.laborCost + j.partsCost;
        }
    }
    fclose(fp_job);
    
    printf("\n" CYAN "--- Customer Lifetime Value Report ---" RESET "\n");
    printf(BLUE "--------------------------------------" RESET "\n");
    printf(MAGENTA "Customer ID: \t\t%s\n" RESET, customerId);
    printf(GREEN "Lifetime Value: \t%.2f\n" RESET, lifetime_value);
    printf(BLUE "--------------------------------------" RESET "\n");
    
    pauseScreen();
}

void monthlyRevenueReport() {
    FILE *fp_finance = fopen(FINANCE_FILE, "r");
    if (!fp_finance) {
        printf(RED "No financial data found.\n" RESET);
        return;
    }
    
    float monthly_revenue[12] = {0.0};
    Transaction t;
    char line[200];
    
    int current_year, current_month, current_day;
    char current_date[15];
    getCurrentDate(current_date);
    sscanf(current_date, "%d-%d-%d", &current_year, &current_month, &current_day);

    while(fgets(line, sizeof(line), fp_finance)) {
        sscanf(line, "%9[^,],%19[^,],%f,%14[^,],%99[^\n]", t.id, t.type, &t.amount, t.date, t.description);
        int year, month, day;
        sscanf(t.date, "%d-%d-%d", &year, &month, &day);
        
        if (strcmp(t.type, "Revenue") == 0 && year == current_year) {
            monthly_revenue[month - 1] += t.amount;
        }
    }
    fclose(fp_finance);
    
    printf("\n" CYAN "--- Monthly Revenue Report (%d) ---" RESET "\n", current_year);
    printf(BLUE "-----------------------------------" RESET "\n");
    for(int i = 0; i < 12; i++) {
        printf(GREEN "Month %d: \t\t%.2f\n" RESET, i + 1, monthly_revenue[i]);
    }
    printf(BLUE "-----------------------------------" RESET "\n");
    
    pauseScreen();
}

void partUsageReport() {
    FILE *fp = fopen(JOB_PART_FILE, "r");
    if (!fp) {
        printf(RED "No parts usage data found.\n" RESET);
        return;
    }
    
    // Simple report: count total usage of each part. Can be expanded for more detail.
    char partId[10], jobId[10];
    int quantity, total_quantity = 0;
    float price;
    char line[200];
    
    printf("\n" CYAN "--- Part Usage Report ---" RESET "\n");
    printf(BLUE "---------------------------\n" RESET);
    printf("Part ID\tQuantity Used\n");

    // A simple, memory-inefficient way for demonstration. For large data, a hash map or sorting would be better.
    // Read all data into an array and then process
    JobPart arr[500];
    int c = 0;
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%d,%f", arr[c].jobId, arr[c].partId, &arr[c].quantity, &arr[c].price);
        c++;
    }
    fclose(fp);

    for (int i = 0; i < c; i++) {
        if (strcmp(arr[i].partId, "PROCESSED") != 0) {
            total_quantity = arr[i].quantity;
            for (int j = i + 1; j < c; j++) {
                if (strcmp(arr[i].partId, arr[j].partId) == 0) {
                    total_quantity += arr[j].quantity;
                    strcpy(arr[j].partId, "PROCESSED"); // Mark as processed
                }
            }
            printf("%s\t%d\n", arr[i].partId, total_quantity);
        }
    }
    
    printf(BLUE "---------------------------\n" RESET);
    pauseScreen();
}

void supplierPerformanceReport() {
    FILE *fp_part = fopen(PART_FILE, "r");
    if (!fp_part) {
        printf(RED "No parts found!\n" RESET);
        return;
    }
    
    // This is a basic report. A more complex one would involve job completion times, etc.
    char supplierId[10];
    int total_parts = 0;
    float total_cost = 0.0;
    
    printf(YELLOW "\nEnter Supplier ID for report: " RESET);
    scanf("%9s", supplierId);
    
    Part p;
    char line[200];
    while(fgets(line, sizeof(line), fp_part)) {
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d", p.id, p.name, &p.quantity, &p.price, p.supplierId, &p.reorderLevel);
        if (strcmp(p.supplierId, supplierId) == 0) {
            total_parts += p.quantity;
            total_cost += p.quantity * p.price;
        }
    }
    fclose(fp_part);
    
    printf("\n" CYAN "--- Supplier Performance Report ---" RESET "\n");
    printf(BLUE "-----------------------------------" RESET "\n");
    printf(MAGENTA "Supplier ID: \t\t%s\n" RESET, supplierId);
    printf(GREEN "Total Parts from Supplier: %d\n" RESET, total_parts);
    printf(GREEN "Total Cost from Supplier: %.2f\n" RESET, total_cost);
    printf(BLUE "-----------------------------------" RESET "\n");
    
    pauseScreen();
}

void customerRetentionReport() {
    FILE *fp_job = fopen(JOB_FILE, "r");
    if (!fp_job) {
        printf(RED "No job data found.\n" RESET);
        return;
    }

    // A very basic report, could be expanded. Counts returning customers.
    char customerId[10];
    char line[500];
    
    // Use a temporary file to store unique customer IDs
    FILE *fp_unique = fopen("temp_unique_customers.txt", "w+");
    if (!fp_unique) {
        fclose(fp_job);
        return;
    }
    
    // First pass: find all unique customers
    while(fgets(line, sizeof(line), fp_job)) {
        sscanf(line, "%9[^,],%9[^,],%*s", j.id, j.customerId);
        rewind(fp_unique);
        int found = 0;
        char unique_id[10];
        while(fgets(unique_id, sizeof(unique_id), fp_unique)) {
            unique_id[strcspn(unique_id, "\n")] = '\0';
            if (strcmp(unique_id, j.customerId) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            fprintf(fp_unique, "%s\n", j.customerId);
        }
    }
    
    int total_customers = 0;
    rewind(fp_unique);
    while(fgets(line, sizeof(line), fp_unique)) {
        total_customers++;
    }
    
    int returning_customers = 0;
    rewind(fp_unique);
    while(fgets(line, sizeof(line), fp_unique)) {
        line[strcspn(line, "\n")] = '\0';
        int job_count = 0;
        rewind(fp_job);
        while(fgets(job_line, sizeof(job_line), fp_job)) {
            sscanf(job_line, "%*s,%9[^,]", customerId);
            if (strcmp(customerId, line) == 0) {
                job_count++;
            }
        }
        if (job_count > 1) {
            returning_customers++;
        }
    }
    
    fclose(fp_job);
    fclose(fp_unique);
    remove("temp_unique_customers.txt");
    
    printf("\n" CYAN "--- Customer Retention Report ---" RESET "\n");
    printf(BLUE "---------------------------------" RESET "\n");
    printf(MAGENTA "Total Customers: \t\t%d\n" RESET, total_customers);
    printf(GREEN "Returning Customers: \t%d\n" RESET, returning_customers);
    float retention_rate = (total_customers > 0) ? ((float)returning_customers / total_customers) * 100 : 0.0;
    printf(GREEN "Retention Rate: \t\t%.2f%%\n" RESET, retention_rate);
    printf(BLUE "---------------------------------" RESET "\n");

    pauseScreen();
}


// Menu Functions
// Admin menu
void adminMenu(const char *uid) {
    clearScreen();
    int ch;
    do {
        printLogo();
        showDashboard();
        printf("\n" CYAN "--- Admin Menu ---" RESET "\n");
        printf("1. Manage Users\n");
        printf("2. Manage Customers\n");
        printf("3. Manage Vehicles\n");
        printf("4. Manage Jobs\n");
        printf("5. Manage Inventory\n");
        printf("6. Manage Suppliers\n");
        printf("7. Manage Appointments & Reminders\n");
        printf("8. Financial Management\n");
        printf("9. Reporting\n");
        printf("10. Backup & Restore Data\n");
        printf("11. Change Password\n");
        printf("0. Logout\n");
        printf(YELLOW "Enter choice: " RESET);
        scanf("%d", &ch);
        clearInputBuffer();
        clearScreen();
        switch (ch) {
            case 1:
                do {
                    printf("\n" CYAN "--- User Management ---" RESET "\n");
                    printf("1. Register New User\n");
                    printf("2. View All Users\n");
                    printf("3. Change User Role\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter choice: " RESET);
                    scanf("%d", &ch);
                    clearInputBuffer();
                    clearScreen();
                    switch (ch) {
                        case 1: registerUser(); break;
                        case 2: viewAllUsers(); break;
                        case 3: changeUserRole(); break;
                    }
                } while(ch != 0);
                ch = -1;
                break;
            case 2:
                do {
                    printf("\n" CYAN "--- Customer Management ---" RESET "\n");
                    printf("1. Add Customer\n");
                    printf("2. Edit Customer\n");
                    printf("3. Delete Customer\n");
                    printf("4. Search Customer\n");
                    printf("5. View All Customers\n");
                    printf("6. Send Feedback Request\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter choice: " RESET);
                    scanf("%d", &ch);
                    clearInputBuffer();
                    clearScreen();
                    switch (ch) {
                        case 1: addCustomer(); break;
                        case 2: editCustomer(); break;
                        case 3: deleteCustomer(); break;
                        case 4: searchCustomer(); break;
                        case 5: viewAllCustomers(); break;
                        case 6: sendFeedbackRequest(); break;
                    }
                } while(ch != 0);
                ch = -1;
                break;
            case 3:
                do {
                    printf("\n" CYAN "--- Vehicle Management ---" RESET "\n");
                    printf("1. Add Vehicle\n");
                    printf("2. Edit Vehicle\n");
                    printf("3. Delete Vehicle\n");
                    printf("4. Search Vehicle\n");
                    printf("5. View All Vehicles\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter choice: " RESET);
                    scanf("%d", &ch);
                    clearInputBuffer();
                    clearScreen();
                    switch (ch) {
                        case 1: addVehicle(); break;
                        case 2: editVehicle(); break;
                        case 3: deleteVehicle(); break;
                        case 4: searchVehicle(); break;
                        case 5: viewAllVehicles(); break;
                    }
                } while(ch != 0);
                ch = -1;
                break;
            case 4:
                do {
                    printf("\n" CYAN "--- Job Management ---" RESET "\n");
                    printf("1. Add Job\n");
                    printf("2. Update Job Status\n");
                    printf("3. View Pending Jobs\n");
                    printf("4. View Completed Jobs\n");
                    printf("5. Search Job\n");
                    printf("6. View All Jobs\n");
                    printf("7. Generate Invoice\n");
                    printf("8. View All Invoices\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter choice: " RESET);
                    scanf("%d", &ch);
                    clearInputBuffer();
                    clearScreen();
                    switch (ch) {
                        case 1: addJob(uid, "Admin"); break;
                        case 2: updateJobStatus(uid, "Admin"); break;
                        case 3: viewPendingJobs(); break;
                        case 4: viewCompletedJobs(); break;
                        case 5: searchJob(); break;
                        case 6: viewAllJobs(); break;
                        case 7: generateInvoice(); break;
                        case 8: viewAllInvoices(); break;
                    }
                } while(ch != 0);
                ch = -1;
                break;
            case 5:
                do {
                    printf("\n" CYAN "--- Inventory Management ---" RESET "\n");
                    printf("1. Add Part to Inventory\n");
                    printf("2. Edit Part\n");
                    printf("3. View Inventory\n");
                    printf("4. Add Part to Job\n");
                    printf("5. Update Job Labor Cost\n");
                    printf("6. Check Low Stock\n");
                    printf("7. View Reorder List\n");
                    printf("8. Clear Reorder List\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter choice: " RESET);
                    scanf("%d", &ch);
                    clearInputBuffer();
                    clearScreen();
                    switch (ch) {
                        case 1: addPart(); break;
                        case 2: editPart(); break;
                        case 3: viewInventory(); break;
                        case 4: addPartToJob(); break;
                        case 5: updateJobLaborCost(); break;
                        case 6: checkLowStock(); break;
                        case 7: viewReorderList(); break;
                        case 8: clearReorderList(); break;
                    }
                } while(ch != 0);
                ch = -1;
                break;
            case 6:
                do {
                    printf("\n" CYAN "--- Supplier Management ---" RESET "\n");
                    printf("1. Add Supplier\n");
                    printf("2. View All Suppliers\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter choice: " RESET);
                    scanf("%d", &ch);
                    clearInputBuffer();
                    clearScreen();
                    switch (ch) {
                        case 1: addSupplier(); break;
                        case 2: viewAllSuppliers(); break;
                    }
                } while(ch != 0);
                ch = -1;
                break;
            case 7:
                do {
                    printf("\n" CYAN "--- Appointments & Reminders ---" RESET "\n");
                    printf("1. Schedule Appointment\n");
                    printf("2. View All Appointments\n");
                    printf("3. Schedule Service Reminders\n");
                    printf("4. View All Reminders\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter choice: " RESET);
                    scanf("%d", &ch);
                    clearInputBuffer();
                    clearScreen();
                    switch (ch) {
                        case 1: scheduleAppointment(uid, "Admin"); break;
                        case 2: viewAllAppointments(); break;
                        case 3: scheduleServiceReminder(); break;
                        case 4: viewAllReminders(); break;
                    }
                } while(ch != 0);
                ch = -1;
                break;
            case 8:
                do {
                    printf("\n" CYAN "--- Financial Management ---" RESET "\n");
                    printf("1. Add Expense\n");
                    printf("2. View All Expenses\n");
                    printf("3. Generate Profit/Loss Report\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter choice: " RESET);
                    scanf("%d", &ch);
                    clearInputBuffer();
                    clearScreen();
                    switch (ch) {
                        case 1: addExpense(); break;
                        case 2: viewAllExpenses(); break;
                        case 3: generateProfitLossReport(); break;
                    }
                } while(ch != 0);
                ch = -1;
                break;
            case 9:
                do {
                    printf("\n" CYAN "--- Reporting ---" RESET "\n");
                    printf("1. Job Summary Report\n");
                    printf("2. Mechanic Performance Report\n");
                    printf("3. Customer Lifetime Value Report\n");
                    printf("4. Monthly Revenue Report\n");
                    printf("5. Part Usage Report\n");
                    printf("6. Supplier Performance Report\n");
                    printf("7. Customer Retention Report\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter choice: " RESET);
                    scanf("%d", &ch);
                    clearInputBuffer();
                    clearScreen();
                    switch (ch) {
                        case 1: viewJobSummaryReport(); break;
                        case 2: mechanicPerformanceReport(); break;
                        case 3: customerLifetimeValueReport(); break;
                        case 4: monthlyRevenueReport(); break;
                        case 5: partUsageReport(); break;
                        case 6: supplierPerformanceReport(); break;
                        case 7: customerRetentionReport(); break;
                    }
                } while(ch != 0);
                ch = -1;
                break;
            case 10:
                do {
                    printf("\n" CYAN "--- Backup & Restore ---" RESET "\n");
                    printf("1. Backup Data\n");
                    printf("2. Restore Data\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter choice: " RESET);
                    scanf("%d", &ch);
                    clearInputBuffer();
                    clearScreen();
                    switch (ch) {
                        case 1: backupData(); break;
                        case 2: restoreData(); break;
                    }
                } while(ch != 0);
                ch = -1;
                break;
            case 11: changePassword(uid); break;
        }
    } while (ch != 0);
}

// Senior Mechanic menu
void seniorMechanicMenu(const char *uid) {
    clearScreen();
    int ch;
    do {
        printLogo();
        showDashboard();
        printf("\n" CYAN "--- Senior Mechanic Menu ---" RESET "\n");
        printf("1. View My Jobs\n");
        printf("2. Add Job\n");
        printf("3. Update Job Status\n");
        printf("4. Manage Inventory\n");
        printf("5. Manage Appointments\n");
        printf("6. Generate Invoice\n");
        printf("7. View All Invoices\n");
        printf("8. Change Password\n");
        printf("0. Logout\n");
        printf(YELLOW "Enter choice: " RESET);
        scanf("%d", &ch);
        clearInputBuffer();
        clearScreen();
        switch (ch) {
            case 1: viewMyJobs(uid); break;
            case 2: addJob(uid, "SeniorMechanic"); break;
            case 3: updateJobStatus(uid, "SeniorMechanic"); break;
            case 4:
                do {
                    printf("\n" CYAN "--- Inventory Management ---" RESET "\n");
                    printf("1. Add Part to Inventory\n");
                    printf("2. Edit Part\n");
                    printf("3. View Inventory\n");
                    printf("4. Add Part to Job\n");
                    printf("5. Update Job Labor Cost\n");
                    printf("6. Check Low Stock\n");
                    printf("7. View Reorder List\n");
                    printf("8. Clear Reorder List\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter choice: " RESET);
                    scanf("%d", &ch);
                    clearInputBuffer();
                    clearScreen();
                    switch (ch) {
                        case 1: addPart(); break;
                        case 2: editPart(); break;
                        case 3: viewInventory(); break;
                        case 4: addPartToJob(); break;
                        case 5: updateJobLaborCost(); break;
                        case 6: checkLowStock(); break;
                        case 7: viewReorderList(); break;
                        case 8: clearReorderList(); break;
                    }
                } while(ch != 0);
                ch = -1;
                break;
            case 5:
                do {
                    printf("\n" CYAN "--- Appointments ---" RESET "\n");
                    printf("1. Schedule Appointment\n");
                    printf("2. View All Appointments\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter choice: " RESET);
                    scanf("%d", &ch);
                    clearInputBuffer();
                    clearScreen();
                    switch (ch) {
                        case 1: scheduleAppointment(uid, "SeniorMechanic"); break;
                        case 2: viewAllAppointments(); break;
                    }
                } while(ch != 0);
                ch = -1;
                break;
            case 6: generateInvoice(); break;
            case 7: viewAllInvoices(); break;
            case 8: changePassword(uid); break;
        }
    } while (ch != 0);
}

// Junior Mechanic menu
void juniorMechanicMenu(const char *uid) {
    clearScreen();
    int ch;
    do {
        printLogo();
        showDashboard();
        printf("\n" CYAN "--- Junior Mechanic Menu ---" RESET "\n");
        printf("1. View My Jobs\n");
        printf("2. Update Job Status\n");
        printf("3. Manage Inventory\n");
        printf("4. View All Appointments\n");
        printf("5. Change Password\n");
        printf("0. Logout\n");
        printf(YELLOW "Enter choice: " RESET);
        scanf("%d", &ch);
        clearInputBuffer();
        clearScreen();
        switch (ch) {
            case 1: viewMyJobs(uid); break;
            case 2: updateJobStatus(uid, "JuniorMechanic"); break;
            case 3:
                do {
                    printf("\n" CYAN "--- Inventory Management ---" RESET "\n");
                    printf("1. View Inventory\n");
                    printf("2. Add Part to Job\n");
                    printf("3. Update Job Labor Cost\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter choice: " RESET);
                    scanf("%d", &ch);
                    clearInputBuffer();
                    clearScreen();
                    switch (ch) {
                        case 1: viewInventory(); break;
                        case 2: addPartToJob(); break;
                        case 3: updateJobLaborCost(); break;
                    }
                } while(ch != 0);
                ch = -1;
                break;
            case 4: viewAllAppointments(); break;
            case 5: changePassword(uid); break;
        }
    } while (ch != 0);
}

// Customer menu
void customerMenu(const char *uid) {
    clearScreen();
    int ch;
    do {
        printLogo();
        printf("\n" CYAN "--- Customer Menu ---" RESET "\n");
        printf("1. View My Vehicles\n");
        printf("2. View My Jobs\n");
        printf("3. Schedule an Appointment\n");
        printf("4. View My Notifications\n");
        printf("5. Change Password\n");
        printf("0. Logout\n");
        printf(YELLOW "Enter choice: " RESET);
        scanf("%d", &ch);
        clearInputBuffer();
        clearScreen();
        switch (ch) {
            case 1: viewCustomerVehicles(uid); break;
            case 2: viewCustomerJobs(uid); break;
            case 3: scheduleAppointment(uid, "Customer"); break;
            case 4: viewNotifications(uid); break;
            case 5: changePassword(uid); break;
        }
    } while (ch != 0);
}

// Print logo at the start
void printLogo() {
    printf(CYAN "========================================\n" RESET);
    printf(CYAN "  ___                  _   _            \n" RESET);
    printf(CYAN " / _ \\ _ __   __ _  __| | | | __ _  ___ \n" RESET);
    printf(CYAN "| | | | '_ \\ / _` |/ _` | | |/ _` |/ __|\n" RESET);
    printf(CYAN "| |_| | | | | (_| | (_| | | | (_| |\\__ \\\n" RESET);
    printf(CYAN " \\___/|_| |_|\\__,_|\\__,_| |_|\\__,_|___/\n" RESET);
    printf(CYAN "========================================\n" RESET);
    printf(MAGENTA "  Vehicle Maintenance Management System\n" RESET);
    printf(CYAN "========================================\n" RESET);
}

// Main function
int main() {
    printLogo();
    int ch;
    char role[20], uid[10];
    
    // Check if user file exists. If not, prompt to register admin.
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) {
        do {
            printf(RED "No users found. Please register an admin first.\n" RESET);
            printf("1. Register Admin\n0. Exit\n" YELLOW "Enter choice: " RESET);
            scanf("%d", &ch);

            if (ch == 1) {
                printf("\n" GREEN "Register Admin Account:\n" RESET);
                registerUser();
            } else if (ch == 0) {
                break;
            } else {
                printf(RED "Invalid choice. Please try again.\n" RESET);
                pauseScreen();
            }
        } while (ch != 1 && ch != 0);
    } else {
        fclose(fp);
    }
    
    do {
        clearScreen();
        printLogo();
        printf("1. Login\n2. Register New User\n0. Exit\n" YELLOW "Enter choice: " RESET);
        scanf("%d", &ch);

        if (ch == 1) {
            if (login(role, uid)) {
                if (!strcmp(role, "Admin")) adminMenu(uid);
                else if (!strcmp(role, "SeniorMechanic")) seniorMechanicMenu(uid);
                else if (!strcmp(role, "JuniorMechanic")) juniorMechanicMenu(uid);
                else if (!strcmp(role, "Customer")) customerMenu(uid);
            } else {
                printf(RED "Login failed after 3 attempts!\n" RESET);
                pauseScreen();
            }
        } else if (ch == 2) {
            registerUser();
        } else if (ch == 0) {
            break;
        } else {
            printf(RED "Invalid choice. Please try again.\n" RESET);
            pauseScreen();
        }
    } while (ch != 0);

    return 0;
}