#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    printf(GREEN "Customer information updated successfully!\n" RESET);
    char log_msg[100];
    sprintf(log_msg, "Customer information for ID %s updated.", id);
    writeLog(log_msg);
    pauseScreen();
}

// Deletes a customer from the system
void deleteCustomer() {
    FILE *fp_read = fopen(CUSTOMER_FILE, "r");
    if (!fp_read) {
        printf(RED "No customers found.\n" RESET);
        return;
    }

    FILE *fp_write = fopen("temp.txt", "w");
    if (!fp_write) {
        fclose(fp_read);
        printf(RED "Error creating temporary file.\n" RESET);
        return;
    }

    char id[10];
    printf(YELLOW "\nEnter Customer ID to delete: " RESET);
    scanf("%9s", id);

    Customer cust;
    char line[200];
    int found = 0;

    while (fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", cust.id, cust.name, cust.phone, cust.email);
        if (strcmp(cust.id, id) == 0) {
            found = 1;
            printf(RED "Deleting customer: %s\n" RESET, cust.name);
            char log_msg[100];
            sprintf(log_msg, "Customer %s with ID %s deleted.", cust.name, id);
            writeLog(log_msg);
        } else {
            fprintf(fp_write, "%s", line);
        }
    }

    fclose(fp_read);
    fclose(fp_write);

    if (found) {
        remove(CUSTOMER_FILE);
        rename("temp.txt", CUSTOMER_FILE);
        printf(GREEN "Customer deleted successfully!\n" RESET);
    } else {
        remove("temp.txt");
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

    char query[50];
    printf(YELLOW "\nEnter customer name or ID to search: " RESET);
    clearInputBuffer();
    fgets(query, sizeof(query), stdin);
    query[strcspn(query, "\n")] = '\0';

    Customer cust;
    char line[200];
    int found = 0;

    printf("\n" CYAN "--- Search Results ---" RESET "\n");
    printf("ID,Name,Phone,Email\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", cust.id, cust.name, cust.phone, cust.email);
        if (strstr(cust.name, query) || strstr(cust.id, query)) {
            printf("%s,%s,%s,%s\n", cust.id, cust.name, cust.phone, cust.email);
            found = 1;
        }
    }
    if (!found) {
        printf(RED "No matching customers found.\n" RESET);
    }
    fclose(fp);
    pauseScreen();
}

// Displays all customers
void viewAllCustomers() {
    FILE *fp = fopen(CUSTOMER_FILE, "r");
    if (!fp) {
        printf(RED "No customers found!\n" RESET);
        return;
    }
    char line[200];
    printf("\n" CYAN "--- All Customers ---" RESET "\n");
    printf("ID,Name,Phone,Email\n");
    Customer c;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", c.id, c.name, c.phone, c.email);
        printf("%s,%s,%s,%s\n", c.id, c.name, c.phone, c.email);
    }
    fclose(fp);
    pauseScreen();
}

// Sends a feedback request to a customer
void sendFeedbackRequest() {
    char customer_id[10];
    printf(YELLOW "\nEnter Customer ID to send feedback request: " RESET);
    scanf("%9s", customer_id);

    if (customerExists(customer_id)) {
        char message[200];
        sprintf(message, "Dear customer, we would appreciate it if you could provide feedback on your recent service. Thank you!");
        createNotification(customer_id, message);
        printf(GREEN "Feedback request sent successfully!\n" RESET);
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

    printf(YELLOW "\nEnter customer ID: " RESET);
    scanf("%9s", v.customerId);
    if (!customerExists(v.customerId)) {
        printf(RED "Customer ID not found.\n" RESET);
        fclose(fp);
        return;
    }

    printf(YELLOW "Enter vehicle brand: " RESET);
    scanf("%29s", v.brand);
    printf(YELLOW "Enter vehicle model: " RESET);
    scanf("%29s", v.model);
    printf(YELLOW "Enter plate number: " RESET);
    scanf("%14s", v.plate);
    printf(YELLOW "Enter year: " RESET);
    scanf("%4s", v.year);
    printf(YELLOW "Enter last service date (YYYY-MM-DD, or N/A if unknown): " RESET);
    scanf("%14s", v.lastServiceDate);

    generateId("VEH", cnt + 1, v.id);

    fprintf(fp, "%s,%s,%s,%s,%s,%s,%s\n", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
    fclose(fp);

    printf(GREEN "Vehicle added successfully! ID: %s\n" RESET, v.id);
    char log_msg[100];
    sprintf(log_msg, "New vehicle %s %s added for customer ID %s.", v.brand, v.model, v.customerId);
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
            printf("Current Brand: %s\n", arr[i].brand);
            printf(YELLOW "Enter new brand (or press Enter to keep current): " RESET);
            clearInputBuffer();
            char newBrand[30];
            fgets(newBrand, sizeof(newBrand), stdin);
            newBrand[strcspn(newBrand, "\n")] = '\0';
            if (strlen(newBrand) > 0) strcpy(arr[i].brand, newBrand);

            printf("Current Model: %s\n", arr[i].model);
            printf(YELLOW "Enter new model (or press Enter to keep current): " RESET);
            char newModel[30];
            fgets(newModel, sizeof(newModel), stdin);
            newModel[strcspn(newModel, "\n")] = '\0';
            if (strlen(newModel) > 0) strcpy(arr[i].model, newModel);

            printf("Current Plate: %s\n", arr[i].plate);
            printf(YELLOW "Enter new plate (or press Enter to keep current): " RESET);
            char newPlate[15];
            fgets(newPlate, sizeof(newPlate), stdin);
            newPlate[strcspn(newPlate, "\n")] = '\0';
            if (strlen(newPlate) > 0) strcpy(arr[i].plate, newPlate);
            
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

    printf(GREEN "Vehicle information updated successfully!\n" RESET);
    char log_msg[100];
    sprintf(log_msg, "Vehicle information for ID %s updated.", id);
    writeLog(log_msg);
    pauseScreen();
}

// Deletes a vehicle from the system
void deleteVehicle() {
    FILE *fp_read = fopen(VEHICLE_FILE, "r");
    if (!fp_read) {
        printf(RED "No vehicles found.\n" RESET);
        return;
    }

    FILE *fp_write = fopen("temp.txt", "w");
    if (!fp_write) {
        fclose(fp_read);
        printf(RED "Error creating temporary file.\n" RESET);
        return;
    }

    char id[10];
    printf(YELLOW "\nEnter Vehicle ID to delete: " RESET);
    scanf("%9s", id);

    Vehicle veh;
    char line[200];
    int found = 0;

    while (fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]", veh.id, veh.customerId, veh.brand, veh.model, veh.plate, veh.year, veh.lastServiceDate);
        if (strcmp(veh.id, id) == 0) {
            found = 1;
            printf(RED "Deleting vehicle: %s %s\n" RESET, veh.brand, veh.model);
            char log_msg[100];
            sprintf(log_msg, "Vehicle %s %s with ID %s deleted.", veh.brand, veh.model, id);
            writeLog(log_msg);
        } else {
            fprintf(fp_write, "%s", line);
        }
    }

    fclose(fp_read);
    fclose(fp_write);

    if (found) {
        remove(VEHICLE_FILE);
        rename("temp.txt", VEHICLE_FILE);
        printf(GREEN "Vehicle deleted successfully!\n" RESET);
    } else {
        remove("temp.txt");
        printf(RED "Vehicle ID not found.\n" RESET);
    }
    pauseScreen();
}

// Searches for a vehicle by plate number or ID
void searchVehicle() {
    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) {
        printf(RED "No vehicles found.\n" RESET);
        return;
    }

    char query[20];
    printf(YELLOW "\nEnter vehicle plate or ID to search: " RESET);
    scanf("%19s", query);

    Vehicle veh;
    char line[200];
    int found = 0;

    printf("\n" CYAN "--- Search Results ---" RESET "\n");
    printf("ID,Customer ID,Brand,Model,Plate,Year\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]", veh.id, veh.customerId, veh.brand, veh.model, veh.plate, veh.year, veh.lastServiceDate);
        if (strstr(veh.plate, query) || strstr(veh.id, query)) {
            printf("%s,%s,%s,%s,%s,%s\n", veh.id, veh.customerId, veh.brand, veh.model, veh.plate, veh.year);
            found = 1;
        }
    }
    if (!found) {
        printf(RED "No matching vehicles found.\n" RESET);
    }
    fclose(fp);
    pauseScreen();
}

// Displays all vehicles
void viewAllVehicles() {
    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) {
        printf(RED "No vehicles found!\n" RESET);
        return;
    }
    char line[200];
    printf("\n" CYAN "--- All Vehicles ---" RESET "\n");
    printf("ID,Customer ID,Brand,Model,Plate,Year,Last Service Date\n");
    Vehicle v;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
        printf("%s,%s,%s,%s,%s,%s,%s\n", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
    }
    fclose(fp);
    pauseScreen();
}

// Displays all vehicles for a specific customer
void viewCustomerVehicles(const char* customerId) {
    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) {
        printf(RED "No vehicles found!\n" RESET);
        return;
    }
    char line[200];
    int found = 0;
    printf("\n" CYAN "--- Your Vehicles ---" RESET "\n");
    printf("ID,Brand,Model,Plate,Year,Last Service Date\n");
    Vehicle v;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
        if (strcmp(v.customerId, customerId) == 0) {
            printf("%s,%s,%s,%s,%s,%s\n", v.id, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
            found = 1;
        }
    }
    if (!found) {
        printf(RED "No vehicles found for your account.\n" RESET);
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

// Adds a new job
void addJob(const char *currentUserId, const char *currentUserRole) {
    FILE *fp = fopen(JOB_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening job file.\n" RESET);
        return;
    }

    Job j;
    int cnt = fileLineCount(JOB_FILE);
    generateId("JOB", cnt + 1, j.id);

    printf(YELLOW "\nEnter customer ID: " RESET);
    scanf("%9s", j.customerId);
    if (!customerExists(j.customerId)) {
        printf(RED "Customer ID not found.\n" RESET);
        fclose(fp);
        return;
    }

    printf(YELLOW "Enter vehicle ID: " RESET);
    scanf("%9s", j.vehicleId);
    if (!vehicleExists(j.vehicleId)) {
        printf(RED "Vehicle ID not found.\n" RESET);
        fclose(fp);
        return;
    }

    printf(YELLOW "Enter mechanic ID (or 'N/A'): " RESET);
    scanf("%9s", j.mechanicId);
    if (strcmp(j.mechanicId, "N/A") != 0 && !mechanicExists(j.mechanicId)) {
        printf(RED "Mechanic ID not found.\n" RESET);
        fclose(fp);
        return;
    }
    
    printf(YELLOW "Enter job description: " RESET);
    clearInputBuffer();
    fgets(j.description, sizeof(j.description), stdin);
    j.description[strcspn(j.description, "\n")] = '\0';
    
    strcpy(j.status, "Pending");
    
    j.estimatedCost = calculateAverageCost(j.description);
    j.laborCost = 0.0;
    j.partsCost = 0.0;
    
    printf(YELLOW "Enter any notes: " RESET);
    fgets(j.notes, sizeof(j.notes), stdin);
    j.notes[strcspn(j.notes, "\n")] = '\0';

    getCurrentDate(j.startDate);
    strcpy(j.endDate, "N/A");

    fprintf(fp, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n",
            j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status,
            j.estimatedCost, j.laborCost, j.partsCost, j.notes, j.startDate, j.endDate);
    fclose(fp);
    
    printf(GREEN "Job added successfully! ID: %s\n" RESET, j.id);
    char log_msg[100];
    sprintf(log_msg, "New job %s created for vehicle ID %s.", j.id, j.vehicleId);
    writeLog(log_msg);

    // Notify the assigned mechanic if one was assigned
    if (strcmp(j.mechanicId, "N/A") != 0) {
        char notif_msg[100];
        sprintf(notif_msg, "A new job (%s) has been assigned to you.", j.id);
        createNotification(j.mechanicId, notif_msg);
    }
    pauseScreen();
}

// Updates the status of a job
void updateJobStatus(const char *currentUserId, const char *currentUserRole) {
    FILE *fp_read = fopen(JOB_FILE, "r");
    if (!fp_read) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }

    FILE *fp_write = fopen("temp.txt", "w");
    if (!fp_write) {
        fclose(fp_read);
        return;
    }

    char jobId[10], newStatus[20];
    printf(YELLOW "\nEnter Job ID to update status: " RESET);
    scanf("%9s", jobId);

    printf(YELLOW "Enter new status (Pending/InProgress/Completed/Canceled): " RESET);
    scanf("%19s", newStatus);

    Job j;
    char line[500];
    int found = 0;

    while (fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]",
               j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost,
               &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);

        if (strcmp(j.id, jobId) == 0) {
            found = 1;
            // Check if current user has permission to update
            if (getRoleLevel(currentUserRole) < getRoleLevel("SeniorMechanic") && strcmp(j.mechanicId, currentUserId) != 0) {
                printf(RED "Permission denied. You can only update your own jobs.\n" RESET);
                fprintf(fp_write, "%s", line);
                continue;
            }

            strcpy(j.status, newStatus);
            if (strcmp(newStatus, "Completed") == 0) {
                getCurrentDate(j.endDate);
                char notif_msg[100];
                sprintf(notif_msg, "Your job (%s) is now completed. Please check your invoice.", j.id);
                createNotification(j.customerId, notif_msg);
            }

            fprintf(fp_write, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n",
                    j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status,
                    j.estimatedCost, j.laborCost, j.partsCost, j.notes, j.startDate, j.endDate);
        } else {
            fprintf(fp_write, "%s", line);
        }
    }

    fclose(fp_read);
    fclose(fp_write);

    if (found) {
        remove(JOB_FILE);
        rename("temp.txt", JOB_FILE);
        printf(GREEN "Job status updated successfully!\n" RESET);
        char log_msg[100];
        sprintf(log_msg, "Job ID %s status updated to %s.", jobId, newStatus);
        writeLog(log_msg);
    } else {
        remove("temp.txt");
        printf(RED "Job ID not found.\n" RESET);
    }
    pauseScreen();
}

// Views all jobs assigned to a specific mechanic
void viewMyJobs(const char *mechanicId) {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found!\n" RESET);
        return;
    }
    char line[500];
    int found = 0;
    printf("\n" CYAN "--- My Assigned Jobs ---" RESET "\n");
    printf("ID,Customer ID,Vehicle ID,Status,Description,Start Date\n");
    Job j;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]",
               j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost,
               &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strcmp(j.mechanicId, mechanicId) == 0) {
            printf("%s,%s,%s,%s,%s,%s\n", j.id, j.customerId, j.vehicleId, j.status, j.description, j.startDate);
            found = 1;
        }
    }
    if (!found) {
        printf(GREEN "You have no assigned jobs.\n" RESET);
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
    char line[500];
    int found = 0;
    printf("\n" CYAN "--- Your Jobs ---" RESET "\n");
    printf("ID,Vehicle ID,Status,Description,Start Date,End Date\n");
    Job j;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]",
               j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost,
               &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strcmp(j.customerId, customerId) == 0) {
            printf("%s,%s,%s,%s,%s,%s\n", j.id, j.vehicleId, j.status, j.description, j.startDate, j.endDate);
            found = 1;
        }
    }
    if (!found) {
        printf(GREEN "You have no jobs found for your account.\n" RESET);
    }
    fclose(fp);
    pauseScreen();
}

// Searches for a job by ID or description
void searchJob() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }

    char query[100];
    printf(YELLOW "\nEnter Job ID or Description to search: " RESET);
    clearInputBuffer();
    fgets(query, sizeof(query), stdin);
    query[strcspn(query, "\n")] = '\0';

    Job j;
    char line[500];
    int found = 0;

    printf("\n" CYAN "--- Search Results ---" RESET "\n");
    printf("ID,Customer ID,Vehicle ID,Status,Description,Start Date\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]",
               j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost,
               &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strstr(j.id, query) || strstr(j.description, query)) {
            printf("%s,%s,%s,%s,%s,%s\n", j.id, j.customerId, j.vehicleId, j.status, j.description, j.startDate);
            found = 1;
        }
    }
    if (!found) {
        printf(RED "No matching jobs found.\n" RESET);
    }
    fclose(fp);
    pauseScreen();
}

// Displays all jobs
void viewAllJobs() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found!\n" RESET);
        return;
    }
    char line[500];
    printf("\n" CYAN "--- All Jobs ---" RESET "\n");
    printf("ID,Customer ID,Vehicle ID,Mechanic ID,Description,Status,Start Date\n");
    Job j;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]",
               j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost,
               &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        printf("%s,%s,%s,%s,%s,%s,%s\n", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, j.startDate);
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
    char line[500];
    int found = 0;
    printf("\n" CYAN "--- Pending Jobs ---" RESET "\n");
    printf("ID,Customer ID,Vehicle ID,Mechanic ID,Description,Status,Start Date\n");
    Job j;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]",
               j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost,
               &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strcmp(j.status, "Pending") == 0 || strcmp(j.status, "InProgress") == 0) {
            printf("%s,%s,%s,%s,%s,%s,%s\n", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, j.startDate);
            found = 1;
        }
    }
    if (!found) {
        printf(GREEN "No pending jobs found.\n" RESET);
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
    char line[500];
    int found = 0;
    printf("\n" CYAN "--- Completed Jobs ---" RESET "\n");
    printf("ID,Customer ID,Vehicle ID,Mechanic ID,Description,Status,End Date\n");
    Job j;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]",
               j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost,
               &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strcmp(j.status, "Completed") == 0) {
            printf("%s,%s,%s,%s,%s,%s,%s\n", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, j.endDate);
            found = 1;
        }
    }
    if (!found) {
        printf(GREEN "No completed jobs found.\n" RESET);
    }
    fclose(fp);
    pauseScreen();
}

// Invoice Management Functions
// Generates an invoice for a completed job
void generateInvoice() {
    FILE *fp_job = fopen(JOB_FILE, "r");
    if (!fp_job) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }

    char jobId[10];
    printf(YELLOW "\nEnter Job ID to generate invoice: " RESET);
    scanf("%9s", jobId);

    Job j;
    char line[500];
    int found = 0;

    while (fgets(line, sizeof(line), fp_job)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]",
               j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost,
               &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strcmp(j.id, jobId) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fp_job);

    if (!found) {
        printf(RED "Job ID not found.\n" RESET);
        pauseScreen();
        return;
    }

    if (strcmp(j.status, "Completed") != 0) {
        printf(YELLOW "Invoice can only be generated for completed jobs.\n" RESET);
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
    inv.totalCost = j.laborCost + j.partsCost;
    inv.discount = 0.0;
    inv.total = inv.totalCost;
    getCurrentDate(inv.date);

    // Prompt for discount
    printf(YELLOW "Enter discount amount (in %%): " RESET);
    float discount_percent;
    scanf("%f", &discount_percent);
    inv.discount = inv.totalCost * (discount_percent / 100.0);
    inv.total = inv.totalCost - inv.discount;

    fprintf(fp_inv, "%s,%s,%.2f,%.2f,%.2f,%s\n", inv.id, inv.jobId, inv.totalCost, inv.discount, inv.total, inv.date);
    fclose(fp_inv);

    // Update finance records
    FILE *fp_finance = fopen(FINANCE_FILE, "a");
    if (fp_finance) {
        fprintf(fp_finance, "FIN%04d,Revenue,%.2f,%s,Invoice for job %s\n", fileLineCount(FINANCE_FILE) + 1, inv.total, inv.date, inv.jobId);
        fclose(fp_finance);
    }

    printf(GREEN "\n--- INVOICE GENERATED ---" RESET "\n");
    printf(CYAN "Invoice ID: %s\n" RESET, inv.id);
    printf(CYAN "Job ID: %s\n" RESET, inv.jobId);
    printf(CYAN "Date: %s\n" RESET, inv.date);
    printf(CYAN "Description: %s\n" RESET, j.description);
    printf(CYAN "Labor Cost: %.2f\n" RESET, j.laborCost);
    printf(CYAN "Parts Cost: %.2f\n" RESET, j.partsCost);
    printf(CYAN "Total Cost: %.2f\n" RESET, inv.totalCost);
    printf(CYAN "Discount: %.2f\n" RESET, inv.discount);
    printf(GREEN "Grand Total: %.2f\n" RESET, inv.total);
    printf(GREEN "-------------------------\n" RESET);
    char log_msg[100];
    sprintf(log_msg, "Invoice %s generated for job ID %s.", inv.id, inv.jobId);
    writeLog(log_msg);
    
    // Notify customer
    char notif_msg[100];
    sprintf(notif_msg, "Invoice %s has been generated for your recent service.", inv.id);
    createNotification(j.customerId, notif_msg);
    
    pauseScreen();
}

// Views all invoices
void viewAllInvoices() {
    FILE *fp = fopen(INVOICE_FILE, "r");
    if (!fp) {
        printf(RED "No invoices found!\n" RESET);
        return;
    }
    char line[200];
    printf("\n" CYAN "--- All Invoices ---" RESET "\n");
    printf("ID,Job ID,Total Cost,Discount,Grand Total,Date\n");
    Invoice inv;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%f,%f,%f,%14[^\n]", inv.id, inv.jobId, &inv.totalCost, &inv.discount, &inv.total, inv.date);
        printf("%s,%s,%.2f,%.2f,%.2f,%s\n", inv.id, inv.jobId, inv.totalCost, inv.discount, inv.total, inv.date);
    }
    fclose(fp);
    pauseScreen();
}

// Inventory Management Functions
// Adds a part to a job
void addPartToJob() {
    FILE *fp_job = fopen(JOB_FILE, "r");
    if (!fp_job) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }

    char jobId[10], partId[10];
    int quantity;
    
    printf(YELLOW "\nEnter Job ID to add part: " RESET);
    scanf("%9s", jobId);

    // Check if job exists
    Job j;
    char line[500];
    int job_found = 0;
    FILE *fp_read_temp = fopen(JOB_FILE, "r");
    while (fgets(line, sizeof(line), fp_read_temp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]",
               j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost,
               &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strcmp(j.id, jobId) == 0) {
            job_found = 1;
            break;
        }
    }
    fclose(fp_read_temp);
    if (!job_found) {
        printf(RED "Job ID not found.\n" RESET);
        pauseScreen();
        return;
    }

    printf(YELLOW "Enter Part ID: " RESET);
    scanf("%9s", partId);
    printf(YELLOW "Enter quantity: " RESET);
    scanf("%d", &quantity);

    if (!partExists(partId)) {
        printf(RED "Part ID not found.\n" RESET);
        pauseScreen();
        return;
    }

    float price = getPartPrice(partId);
    if (price == -1.0) {
        printf(RED "Error getting part price.\n" RESET);
        pauseScreen();
        return;
    }

    // Deduct from inventory
    deductPartFromInventory(partId, quantity);

    // Add to job_parts file
    FILE *fp_job_part = fopen(JOB_PART_FILE, "a+");
    if (!fp_job_part) {
        printf(RED "Error opening job parts file.\n" RESET);
        pauseScreen();
        return;
    }
    fprintf(fp_job_part, "%s,%s,%d,%.2f\n", jobId, partId, quantity, price);
    fclose(fp_job_part);

    // Update partsCost in job file
    FILE *fp_read = fopen(JOB_FILE, "r");
    FILE *fp_write = fopen("temp_job.txt", "w");
    while (fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]",
               j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost,
               &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strcmp(j.id, jobId) == 0) {
            j.partsCost += price * quantity;
        }
        fprintf(fp_write, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n",
                j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status,
                j.estimatedCost, j.laborCost, j.partsCost, j.notes, j.startDate, j.endDate);
    }
    fclose(fp_read);
    fclose(fp_write);
    remove(JOB_FILE);
    rename("temp_job.txt", JOB_FILE);

    printf(GREEN "Part added to job successfully!\n" RESET);
    char log_msg[100];
    sprintf(log_msg, "Part %s (x%d) added to job %s.", partId, quantity, jobId);
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

    FILE *fp_write = fopen("temp.txt", "w");
    if (!fp_write) {
        fclose(fp_read);
        return;
    }

    char jobId[10];
    float laborCost;
    printf(YELLOW "\nEnter Job ID to update labor cost: " RESET);
    scanf("%9s", jobId);
    printf(YELLOW "Enter new labor cost: " RESET);
    scanf("%f", &laborCost);

    Job j;
    char line[500];
    int found = 0;

    while (fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]",
               j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost,
               &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strcmp(j.id, jobId) == 0) {
            j.laborCost = laborCost;
            found = 1;
        }
        fprintf(fp_write, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n",
                j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status,
                j.estimatedCost, j.laborCost, j.partsCost, j.notes, j.startDate, j.endDate);
    }

    fclose(fp_read);
    fclose(fp_write);

    if (found) {
        remove(JOB_FILE);
        rename("temp.txt", JOB_FILE);
        printf(GREEN "Labor cost updated successfully!\n" RESET);
        char log_msg[100];
        sprintf(log_msg, "Labor cost for job %s updated to %.2f.", jobId, laborCost);
        writeLog(log_msg);
    } else {
        remove("temp.txt");
        printf(RED "Job ID not found.\n" RESET);
    }
    pauseScreen();
}

// Adds a new part to inventory
void addPart() {
    FILE *fp = fopen(PART_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening parts file.\n" RESET);
        return;
    }

    Part p;
    int cnt = fileLineCount(PART_FILE);
    generateId("PRT", cnt + 1, p.id);

    printf(YELLOW "\nEnter part name: " RESET);
    clearInputBuffer();
    fgets(p.name, sizeof(p.name), stdin);
    p.name[strcspn(p.name, "\n")] = '\0';

    printf(YELLOW "Enter initial quantity: " RESET);
    scanf("%d", &p.quantity);
    printf(YELLOW "Enter price per unit: " RESET);
    scanf("%f", &p.price);
    printf(YELLOW "Enter supplier ID: " RESET);
    scanf("%9s", p.supplierId);
    if (!supplierExists(p.supplierId)) {
        printf(RED "Supplier ID not found.\n" RESET);
        fclose(fp);
        return;
    }
    printf(YELLOW "Enter reorder level: " RESET);
    scanf("%d", &p.reorderLevel);

    fprintf(fp, "%s,%s,%d,%.2f,%s,%d\n", p.id, p.name, p.quantity, p.price, p.supplierId, p.reorderLevel);
    fclose(fp);

    printf(GREEN "Part added to inventory successfully! ID: %s\n" RESET, p.id);
    char log_msg[100];
    sprintf(log_msg, "New part %s (%s) added to inventory.", p.id, p.name);
    writeLog(log_msg);
    pauseScreen();
}

// Edits an existing part's information
void editPart() {
    FILE *fp = fopen(PART_FILE, "r");
    if (!fp) {
        printf(RED "No parts found.\n" RESET);
        return;
    }

    Part arr[200];
    int c = 0;
    char line[200];

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d[^\n]", arr[c].id, arr[c].name, &arr[c].quantity, &arr[c].price, arr[c].supplierId, &arr[c].reorderLevel);
        c++;
    }
    fclose(fp);

    char id[10];
    printf(YELLOW "\nEnter Part ID to edit: " RESET);
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

            printf("Current Quantity: %d\n", arr[i].quantity);
            printf(YELLOW "Enter new quantity: " RESET);
            int newQuantity;
            scanf("%d", &newQuantity);
            arr[i].quantity = newQuantity;

            printf("Current Price: %.2f\n", arr[i].price);
            printf(YELLOW "Enter new price: " RESET);
            float newPrice;
            scanf("%f", &newPrice);
            arr[i].price = newPrice;
            
            found = 1;
            break;
        }
    }

    if (!found) {
        printf(RED "Part ID not found.\n" RESET);
        pauseScreen();
        return;
    }

    fp = fopen(PART_FILE, "w");
    if (!fp) {
        printf(RED "Error opening part file for writing.\n" RESET);
        return;
    }
    for (int i = 0; i < c; i++)
        fprintf(fp, "%s,%s,%d,%.2f,%s,%d\n", arr[i].id, arr[i].name, arr[i].quantity, arr[i].price, arr[i].supplierId, arr[i].reorderLevel);
    fclose(fp);

    printf(GREEN "Part information updated successfully!\n" RESET);
    char log_msg[100];
    sprintf(log_msg, "Part information for ID %s updated.", id);
    writeLog(log_msg);
    pauseScreen();
}

// Displays all parts in inventory
void viewInventory() {
    FILE *fp = fopen(PART_FILE, "r");
    if (!fp) {
        printf(RED "No parts found!\n" RESET);
        return;
    }
    char line[200];
    printf("\n" CYAN "--- Current Inventory ---" RESET "\n");
    printf("ID,Name,Quantity,Price,Supplier ID,Reorder Level\n");
    Part p;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d[^\n]", p.id, p.name, &p.quantity, &p.price, p.supplierId, &p.reorderLevel);
        printf("%s,%s,%d,%.2f,%s,%d\n", p.id, p.name, p.quantity, p.price, p.supplierId, p.reorderLevel);
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
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d[^\n]", p.id, p.name, &p.quantity, &p.price, p.supplierId, &p.reorderLevel);
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
    if (!fp) return -1.0;

    Part p;
    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d[^\n]", p.id, p.name, &p.quantity, &p.price, p.supplierId, &p.reorderLevel);
        if (!strcmp(p.id, partId)) {
            fclose(fp);
            return p.price;
        }
    }
    fclose(fp);
    return -1.0;
}

// Deducts part quantity from inventory
void deductPartFromInventory(const char *partId, int quantity) {
    FILE *fp_read = fopen(PART_FILE, "r");
    if (!fp_read) return;

    FILE *fp_write = fopen("temp.txt", "w");
    if (!fp_write) {
        fclose(fp_read);
        return;
    }

    Part p;
    char line[200];
    while (fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d[^\n]", p.id, p.name, &p.quantity, &p.price, p.supplierId, &p.reorderLevel);
        if (strcmp(p.id, partId) == 0) {
            p.quantity -= quantity;
            if (p.quantity < p.reorderLevel) {
                checkLowStock(); // Trigger a low stock check
            }
        }
        fprintf(fp_write, "%s,%s,%d,%.2f,%s,%d\n", p.id, p.name, p.quantity, p.price, p.supplierId, p.reorderLevel);
    }
    fclose(fp_read);
    fclose(fp_write);
    remove(PART_FILE);
    rename("temp.txt", PART_FILE);
}

// Checks for low stock parts and updates reorder list
void checkLowStock() {
    FILE *fp = fopen(PART_FILE, "r");
    if (!fp) return;

    FILE *fp_reorder = fopen(REORDER_LIST_FILE, "w");
    if (!fp_reorder) {
        fclose(fp);
        return;
    }

    Part p;
    char line[200];
    int count = 0;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d[^\n]", p.id, p.name, &p.quantity, &p.price, p.supplierId, &p.reorderLevel);
        if (p.quantity <= p.reorderLevel) {
            fprintf(fp_reorder, "%s,%d,%s\n", p.id, p.reorderLevel - p.quantity + 5, "N/A"); // Need 5 extra parts
            count++;
        }
    }
    fclose(fp);
    fclose(fp_reorder);

    if (count > 0) {
        printf(YELLOW "Low stock alert: %d items need to be reordered. Check the reorder list.\n" RESET, count);
        char log_msg[100];
        sprintf(log_msg, "Low stock alert: %d items need reordering.", count);
        writeLog(log_msg);
    }
}

// Displays the reorder list
void viewReorderList() {
    FILE *fp = fopen(REORDER_LIST_FILE, "r");
    if (!fp) {
        printf(GREEN "Reorder list is empty.\n" RESET);
        return;
    }
    char line[200];
    printf("\n" CYAN "--- Reorder List ---" RESET "\n");
    printf("Part ID,Needed Quantity,Date\n");
    ReorderItem ri;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%d,%14[^\n]", ri.partId, &ri.neededQuantity, ri.date);
        printf("%s,%d,%s\n", ri.partId, ri.neededQuantity, ri.date);
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
        printf(RED "Reorder list is already empty or could not be cleared.\n" RESET);
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
    generateId("SUP", cnt + 1, s.id);

    printf(YELLOW "\nEnter supplier name: " RESET);
    clearInputBuffer();
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = '\0';
    printf(YELLOW "Enter phone number: " RESET);
    scanf("%14s", s.phone);
    printf(YELLOW "Enter email: " RESET);
    scanf("%49s", s.email);

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
    char line[200];
    printf("\n" CYAN "--- All Suppliers ---" RESET "\n");
    printf("ID,Name,Phone,Email\n");
    Supplier s;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", s.id, s.name, s.phone, s.email);
        printf("%s,%s,%s,%s\n", s.id, s.name, s.phone, s.email);
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
    generateId("APT", cnt + 1, a.id);

    printf(YELLOW "\nEnter customer ID: " RESET);
    scanf("%9s", a.customerId);
    if (!customerExists(a.customerId)) {
        printf(RED "Customer ID not found.\n" RESET);
        fclose(fp);
        return;
    }

    printf(YELLOW "Enter vehicle ID: " RESET);
    scanf("%9s", a.vehicleId);
    if (!vehicleExists(a.vehicleId)) {
        printf(RED "Vehicle ID not found.\n" RESET);
        fclose(fp);
        return;
    }

    printf(YELLOW "Enter appointment date (YYYY-MM-DD): " RESET);
    scanf("%14s", a.date);
    if (!isDateValid(a.date)) {
        printf(RED "Invalid date format.\n" RESET);
        fclose(fp);
        return;
    }

    printf(YELLOW "Enter appointment time (HH:MM): " RESET);
    scanf("%9s", a.time);
    
    printf(YELLOW "Enter appointment description: " RESET);
    clearInputBuffer();
    fgets(a.description, sizeof(a.description), stdin);
    a.description[strcspn(a.description, "\n")] = '\0';
    
    strcpy(a.status, "Scheduled");

    fprintf(fp, "%s,%s,%s,%s,%s,%s,%s\n", a.id, a.customerId, a.vehicleId, a.date, a.time, a.description, a.status);
    fclose(fp);

    printf(GREEN "Appointment scheduled successfully! ID: %s\n" RESET, a.id);
    char log_msg[100];
    sprintf(log_msg, "Appointment %s scheduled for customer ID %s.", a.id, a.customerId);
    writeLog(log_msg);
    
    // Notify customer
    char notif_msg[100];
    sprintf(notif_msg, "Your appointment (%s) has been scheduled on %s at %s.", a.id, a.date, a.time);
    createNotification(a.customerId, notif_msg);
    
    pauseScreen();
}

// Views all appointments
void viewAllAppointments() {
    FILE *fp = fopen(APPOINTMENT_FILE, "r");
    if (!fp) {
        printf(RED "No appointments found!\n" RESET);
        return;
    }
    char line[200];
    printf("\n" CYAN "--- All Appointments ---" RESET "\n");
    printf("ID,Customer ID,Vehicle ID,Date,Time,Description,Status\n");
    Appointment a;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%14[^,],%9[^,],%99[^,],%19[^\n]", a.id, a.customerId, a.vehicleId, a.date, a.time, a.description, a.status);
        printf("%s,%s,%s,%s,%s,%s,%s\n", a.id, a.customerId, a.vehicleId, a.date, a.time, a.description, a.status);
    }
    fclose(fp);
    pauseScreen();
}

// Schedules a service reminder for a customer
void scheduleServiceReminder() {
    FILE *fp = fopen(REMINDERS_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening reminders file.\n" RESET);
        return;
    }

    Reminder r;
    int cnt = fileLineCount(REMINDERS_FILE);
    generateId("REM", cnt + 1, r.id);

    char customer_id[10], last_service_date[15], next_service_date[15];
    printf(YELLOW "\nEnter customer ID: " RESET);
    scanf("%9s", customer_id);
    if (!customerExists(customer_id)) {
        printf(RED "Customer ID not found.\n" RESET);
        fclose(fp);
        return;
    }

    printf(YELLOW "Enter last service date (YYYY-MM-DD): " RESET);
    scanf("%14s", last_service_date);
    calculateNextServiceDate(last_service_date, next_service_date);

    strcpy(r.customerId, customer_id);
    sprintf(r.message, "Reminder: Your next service is due around %s.", next_service_date);
    strcpy(r.date, next_service_date);
    strcpy(r.status, "Scheduled");
    
    fprintf(fp, "%s,%s,%s,%s,%s\n", r.id, r.customerId, r.message, r.date, r.status);
    fclose(fp);

    printf(GREEN "Service reminder scheduled for %s!\n" RESET, next_service_date);
    char log_msg[100];
    sprintf(log_msg, "Service reminder scheduled for customer %s.", customer_id);
    writeLog(log_msg);
    pauseScreen();
}

// Views all service reminders
void viewAllReminders() {
    FILE *fp = fopen(REMINDERS_FILE, "r");
    if (!fp) {
        printf(RED "No reminders found!\n" RESET);
        return;
    }
    char line[300];
    printf("\n" CYAN "--- All Reminders ---" RESET "\n");
    printf("ID,Customer ID,Message,Date,Status\n");
    Reminder r;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%199[^,],%14[^,],%19[^\n]", r.id, r.customerId, r.message, r.date, r.status);
        printf("%s,%s,%s,%s,%s\n", r.id, r.customerId, r.message, r.date, r.status);
    }
    fclose(fp);
    pauseScreen();
}


// Financial Management Functions
// Adds a new expense
void addExpense() {
    FILE *fp = fopen(EXPENSES_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening expenses file.\n" RESET);
        return;
    }

    Transaction t;
    int cnt = fileLineCount(EXPENSES_FILE);
    generateId("EXP", cnt + 1, t.id);
    strcpy(t.type, "Expense");

    printf(YELLOW "\nEnter expense amount: " RESET);
    scanf("%f", &t.amount);
    printf(YELLOW "Enter expense description: " RESET);
    clearInputBuffer();
    fgets(t.description, sizeof(t.description), stdin);
    t.description[strcspn(t.description, "\n")] = '\0';
    getCurrentDate(t.date);

    fprintf(fp, "%s,%s,%.2f,%s,%s\n", t.id, t.type, t.amount, t.date, t.description);
    fclose(fp);

    // Also write to finance file
    FILE *fp_finance = fopen(FINANCE_FILE, "a");
    if (fp_finance) {
        fprintf(fp_finance, "FIN%04d,%s,%.2f,%s,%s\n", fileLineCount(FINANCE_FILE) + 1, t.type, t.amount, t.date, t.description);
        fclose(fp_finance);
    }
    
    printf(GREEN "Expense recorded successfully! ID: %s\n" RESET, t.id);
    char log_msg[100];
    sprintf(log_msg, "New expense of %.2f recorded.", t.amount);
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
    char line[200];
    printf("\n" CYAN "--- All Expenses ---" RESET "\n");
    printf("ID,Amount,Date,Description\n");
    Transaction t;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%19[^,],%f,%14[^,],%99[^\n]", t.id, t.type, &t.amount, t.date, t.description);
        printf("%s,%.2f,%s,%s\n", t.id, t.amount, t.date, t.description);
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
    float total_revenue = 0, total_expenses = 0, profit_loss;
    char line[200];
    Transaction t;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%19[^,],%f,%14[^,],%99[^\n]", t.id, t.type, &t.amount, t.date, t.description);
        if (strcmp(t.type, "Revenue") == 0) {
            total_revenue += t.amount;
        } else if (strcmp(t.type, "Expense") == 0) {
            total_expenses += t.amount;
        }
    }
    fclose(fp);
    profit_loss = total_revenue - total_expenses;

    printf("\n" CYAN "--- Profit & Loss Report ---" RESET "\n");
    printf(GREEN "Total Revenue: %.2f\n" RESET, total_revenue);
    printf(RED "Total Expenses: %.2f\n" RESET, total_expenses);
    if (profit_loss >= 0) {
        printf(GREEN "Net Profit: %.2f\n" RESET, profit_loss);
    } else {
        printf(RED "Net Loss: %.2f\n" RESET, profit_loss);
    }
    printf(CYAN "--------------------------\n" RESET);
    pauseScreen();
}

// Reporting Prototypes
void viewJobSummaryReport() {
    int total_jobs = 0, pending = 0, completed = 0, canceled = 0;
    FILE *fp = fopen(JOB_FILE, "r");
    if (fp) {
        char line[500];
        Job j;
        while (fgets(line, sizeof(line), fp)) {
            sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]",
                   j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost,
                   &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
            total_jobs++;
            if (strcmp(j.status, "Pending") == 0 || strcmp(j.status, "InProgress") == 0) pending++;
            else if (strcmp(j.status, "Completed") == 0) completed++;
            else if (strcmp(j.status, "Canceled") == 0) canceled++;
        }
        fclose(fp);
    }
    printf("\n" CYAN "--- Job Summary Report ---" RESET "\n");
    printf(MAGENTA "Total Jobs: %d\n" RESET, total_jobs);
    printf(YELLOW "Pending/In-Progress Jobs: %d\n" RESET, pending);
    printf(GREEN "Completed Jobs: %d\n" RESET, completed);
    printf(RED "Canceled Jobs: %d\n" RESET, canceled);
    pauseScreen();
}

void mechanicPerformanceReport() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No job data found.\n" RESET);
        return;
    }
    FILE *fp_users = fopen(USER_FILE, "r");
    if (!fp_users) {
        printf(RED "No user data found.\n" RESET);
        return;
    }

    char mechanic_ids[100][10];
    int completed_jobs[100] = {0};
    int mechanic_count = 0;
    
    User u;
    char line[200];
    while (fgets(line, sizeof(line), fp_users)) {
        sscanf(line, "%9[^,],%29[^,],%29[^,],%19[^\n]", u.id, u.username, u.password, u.role);
        if (strcmp(u.role, "SeniorMechanic") == 0 || strcmp(u.role, "JuniorMechanic") == 0) {
            strcpy(mechanic_ids[mechanic_count], u.id);
            mechanic_count++;
        }
    }
    fclose(fp_users);

    Job j;
    rewind(fp);
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]",
               j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost,
               &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strcmp(j.status, "Completed") == 0) {
            for (int i = 0; i < mechanic_count; i++) {
                if (strcmp(j.mechanicId, mechanic_ids[i]) == 0) {
                    completed_jobs[i]++;
                    break;
                }
            }
        }
    }
    fclose(fp);

    printf("\n" CYAN "--- Mechanic Performance Report ---" RESET "\n");
    for (int i = 0; i < mechanic_count; i++) {
        printf("Mechanic ID: %s, Completed Jobs: %d\n", mechanic_ids[i], completed_jobs[i]);
    }
    pauseScreen();
}

void customerLifetimeValueReport() {
    FILE *fp_inv = fopen(INVOICE_FILE, "r");
    if (!fp_inv) {
        printf(RED "No invoice data found.\n" RESET);
        return;
    }
    FILE *fp_job = fopen(JOB_FILE, "r");
    if (!fp_job) {
        fclose(fp_inv);
        printf(RED "No job data found.\n" RESET);
        return;
    }
    
    char customer_ids[200][10];
    float customer_value[200] = {0.0};
    int customer_count = 0;
    
    char line[500];
    Job j;
    while (fgets(line, sizeof(line), fp_job)) {
        sscanf(line, "%9[^,],%9[^,]", j.id, j.customerId);
        int found = 0;
        for (int i = 0; i < customer_count; i++) {
            if (strcmp(customer_ids[i], j.customerId) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(customer_ids[customer_count], j.customerId);
            customer_count++;
        }
    }
    fclose(fp_job);

    Invoice inv;
    rewind(fp_inv);
    while (fgets(line, sizeof(line), fp_inv)) {
        sscanf(line, "%9[^,],%9[^,],%f,%f,%f,%14[^\n]", inv.id, inv.jobId, &inv.totalCost, &inv.discount, &inv.total, inv.date);
        
        rewind(fp_job);
        Job job_link;
        while (fgets(line, sizeof(line), fp_job)) {
             sscanf(line, "%9[^,],%9[^,]", job_link.id, job_link.customerId);
             if (strcmp(job_link.id, inv.jobId) == 0) {
                 for (int i = 0; i < customer_count; i++) {
                     if (strcmp(customer_ids[i], job_link.customerId) == 0) {
                         customer_value[i] += inv.total;
                         break;
                     }
                 }
                 break;
             }
        }
    }
    fclose(fp_inv);
    fclose(fp_job);

    printf("\n" CYAN "--- Customer Lifetime Value Report ---" RESET "\n");
    for (int i = 0; i < customer_count; i++) {
        printf("Customer ID: %s, Total Value: %.2f\n", customer_ids[i], customer_value[i]);
    }
    pauseScreen();
}

void monthlyRevenueReport() {
    FILE *fp = fopen(FINANCE_FILE, "r");
    if (!fp) {
        printf(RED "No financial data found.\n" RESET);
        return;
    }

    float monthly_revenue[12] = {0.0};
    int current_year;
    char line[200];
    Transaction t;
    
    char current_date[15];
    getCurrentDate(current_date);
    sscanf(current_date, "%d", &current_year);
    
    printf(YELLOW "Enter year for report: " RESET);
    int year_choice;
    scanf("%d", &year_choice);
    
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%19[^,],%f,%14[^,],%99[^\n]", t.id, t.type, &t.amount, t.date, t.description);
        int trans_year, trans_month;
        sscanf(t.date, "%d-%d", &trans_year, &trans_month);
        
        if (trans_year == year_choice && strcmp(t.type, "Revenue") == 0) {
            monthly_revenue[trans_month - 1] += t.amount;
        }
    }
    fclose(fp);

    printf("\n" CYAN "--- Monthly Revenue Report for %d ---" RESET "\n", year_choice);
    char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    for (int i = 0; i < 12; i++) {
        printf("%s: %.2f\n", months[i], monthly_revenue[i]);
    }
    pauseScreen();
}

void partUsageReport() {
    FILE *fp = fopen(JOB_PART_FILE, "r");
    if (!fp) {
        printf(RED "No part usage data found.\n" RESET);
        return;
    }
    
    char part_ids[200][10];
    int part_quantities[200] = {0};
    int part_count = 0;
    
    char line[100];
    JobPart jp;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%d,%f", jp.jobId, jp.partId, &jp.quantity, &jp.price);
        int found = 0;
        for (int i = 0; i < part_count; i++) {
            if (strcmp(part_ids[i], jp.partId) == 0) {
                part_quantities[i] += jp.quantity;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(part_ids[part_count], jp.partId);
            part_quantities[part_count] = jp.quantity;
            part_count++;
        }
    }
    fclose(fp);
    
    printf("\n" CYAN "--- Part Usage Report ---" RESET "\n");
    for (int i = 0; i < part_count; i++) {
        printf("Part ID: %s, Quantity Used: %d\n", part_ids[i], part_quantities[i]);
    }
    pauseScreen();
}

void supplierPerformanceReport() {
    FILE *fp = fopen(PART_FILE, "r");
    if (!fp) {
        printf(RED "No part data found.\n" RESET);
        return;
    }
    FILE *fp_suppliers = fopen(SUPPLIER_FILE, "r");
    if (!fp_suppliers) {
        fclose(fp);
        printf(RED "No supplier data found.\n" RESET);
        return;
    }

    char supplier_ids[100][10];
    float total_value[100] = {0.0};
    int supplier_count = 0;
    
    Supplier s;
    char line[200];
    while (fgets(line, sizeof(line), fp_suppliers)) {
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", s.id, s.name, s.phone, s.email);
        strcpy(supplier_ids[supplier_count], s.id);
        supplier_count++;
    }
    fclose(fp_suppliers);

    Part p;
    rewind(fp);
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d[^\n]", p.id, p.name, &p.quantity, &p.price, p.supplierId, &p.reorderLevel);
        for (int i = 0; i < supplier_count; i++) {
            if (strcmp(p.supplierId, supplier_ids[i]) == 0) {
                total_value[i] += (p.price * p.quantity);
                break;
            }
        }
    }
    fclose(fp);

    printf("\n" CYAN "--- Supplier Performance Report ---" RESET "\n");
    for (int i = 0; i < supplier_count; i++) {
        printf("Supplier ID: %s, Total Inventory Value: %.2f\n", supplier_ids[i], total_value[i]);
    }
    pauseScreen();
}

void customerRetentionReport() {
    FILE *fp = fopen(CUSTOMER_FILE, "r");
    if (!fp) {
        printf(RED "No customer data found.\n" RESET);
        return;
    }
    FILE *fp_job = fopen(JOB_FILE, "r");
    if (!fp_job) {
        fclose(fp);
        printf(RED "No job data found.\n" RESET);
        return;
    }
    
    int new_customers = 0, returning_customers = 0;
    char customer_ids[200][10];
    int job_counts[200] = {0};
    int customer_count = 0;
    
    char line[500];
    Job j;
    while (fgets(line, sizeof(line), fp_job)) {
        sscanf(line, "%9[^,],%9[^,]", j.id, j.customerId);
        int found = 0;
        for (int i = 0; i < customer_count; i++) {
            if (strcmp(customer_ids[i], j.customerId) == 0) {
                job_counts[i]++;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(customer_ids[customer_count], j.customerId);
            job_counts[customer_count] = 1;
            customer_count++;
        }
    }
    fclose(fp_job);

    for (int i = 0; i < customer_count; i++) {
        if (job_counts[i] == 1) {
            new_customers++;
        } else {
            returning_customers++;
        }
    }
    
    printf("\n" CYAN "--- Customer Retention Report ---" RESET "\n");
    printf("Total Customers with Jobs: %d\n", customer_count);
    printf("New Customers (1 Job): %d\n", new_customers);
    printf("Returning Customers (>1 Job): %d\n", returning_customers);
    pauseScreen();
}

// Menu Functions
void adminMenu(const char *uid) {
    int choice;
    while (1) {
        showDashboard();
        printf("\n" GREEN "--- ADMIN MENU ---" RESET "\n");
        printf("1. User Management\n");
        printf("2. Customer Management\n");
        printf("3. Vehicle Management\n");
        printf("4. Job Management\n");
        printf("5. Invoice Management\n");
        printf("6. Inventory Management\n");
        printf("7. Supplier Management\n");
        printf("8. Appointment & Reminders\n");
        printf("9. Financial Management\n");
        printf("10. Reporting\n");
        printf("11. Backup & Restore Data\n");
        printf("12. Change Password\n");
        printf("13. View Notifications\n");
        printf("14. Logout\n");
        printf(YELLOW "Enter your choice: " RESET);
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch (choice) {
            case 1: {
                int user_choice;
                printf("\n" CYAN "--- USER MANAGEMENT ---" RESET "\n");
                printf("1. Register New User\n");
                printf("2. Change User Role\n");
                printf("3. View All Users\n");
                printf("4. Go Back\n");
                printf(YELLOW "Enter choice: " RESET);
                scanf("%d", &user_choice);
                switch(user_choice) {
                    case 1: registerUser(); break;
                    case 2: changeUserRole(); break;
                    case 3: viewAllUsers(); break;
                }
                break;
            }
            case 2: {
                int cust_choice;
                printf("\n" CYAN "--- CUSTOMER MANAGEMENT ---" RESET "\n");
                printf("1. Add Customer\n");
                printf("2. Edit Customer\n");
                printf("3. Delete Customer\n");
                printf("4. Search Customer\n");
                printf("5. View All Customers\n");
                printf("6. Send Feedback Request\n");
                printf("7. Go Back\n");
                printf(YELLOW "Enter choice: " RESET);
                scanf("%d", &cust_choice);
                switch(cust_choice) {
                    case 1: addCustomer(); break;
                    case 2: editCustomer(); break;
                    case 3: deleteCustomer(); break;
                    case 4: searchCustomer(); break;
                    case 5: viewAllCustomers(); break;
                    case 6: sendFeedbackRequest(); break;
                }
                break;
            }
            case 3: {
                int veh_choice;
                printf("\n" CYAN "--- VEHICLE MANAGEMENT ---" RESET "\n");
                printf("1. Add Vehicle\n");
                printf("2. Edit Vehicle\n");
                printf("3. Delete Vehicle\n");
                printf("4. Search Vehicle\n");
                printf("5. View All Vehicles\n");
                printf("6. Go Back\n");
                printf(YELLOW "Enter choice: " RESET);
                scanf("%d", &veh_choice);
                switch(veh_choice) {
                    case 1: addVehicle(); break;
                    case 2: editVehicle(); break;
                    case 3: deleteVehicle(); break;
                    case 4: searchVehicle(); break;
                    case 5: viewAllVehicles(); break;
                }
                break;
            }
            case 4: {
                int job_choice;
                printf("\n" CYAN "--- JOB MANAGEMENT ---" RESET "\n");
                printf("1. Add New Job\n");
                printf("2. Update Job Status\n");
                printf("3. Search Job\n");
                printf("4. View All Jobs\n");
                printf("5. View Pending Jobs\n");
                printf("6. View Completed Jobs\n");
                printf("7. Go Back\n");
                printf(YELLOW "Enter choice: " RESET);
                scanf("%d", &job_choice);
                switch(job_choice) {
                    case 1: addJob(uid, "Admin"); break;
                    case 2: updateJobStatus(uid, "Admin"); break;
                    case 3: searchJob(); break;
                    case 4: viewAllJobs(); break;
                    case 5: viewPendingJobs(); break;
                    case 6: viewCompletedJobs(); break;
                }
                break;
            }
            case 5: {
                int inv_choice;
                printf("\n" CYAN "--- INVOICE MANAGEMENT ---" RESET "\n");
                printf("1. Generate Invoice\n");
                printf("2. View All Invoices\n");
                printf("3. Go Back\n");
                printf(YELLOW "Enter choice: " RESET);
                scanf("%d", &inv_choice);
                switch(inv_choice) {
                    case 1: generateInvoice(); break;
                    case 2: viewAllInvoices(); break;
                }
                break;
            }
            case 6: {
                int part_choice;
                printf("\n" CYAN "--- INVENTORY MANAGEMENT ---" RESET "\n");
                printf("1. Add Part to Job\n");
                printf("2. Add New Part\n");
                printf("3. Edit Part\n");
                printf("4. View Inventory\n");
                printf("5. View Reorder List\n");
                printf("6. Clear Reorder List\n");
                printf("7. Go Back\n");
                printf(YELLOW "Enter choice: " RESET);
                scanf("%d", &part_choice);
                switch(part_choice) {
                    case 1: addPartToJob(); break;
                    case 2: addPart(); break;
                    case 3: editPart(); break;
                    case 4: viewInventory(); break;
                    case 5: viewReorderList(); break;
                    case 6: clearReorderList(); break;
                }
                break;
            }
            case 7: {
                int sup_choice;
                printf("\n" CYAN "--- SUPPLIER MANAGEMENT ---" RESET "\n");
                printf("1. Add Supplier\n");
                printf("2. View All Suppliers\n");
                printf("3. Go Back\n");
                printf(YELLOW "Enter choice: " RESET);
                scanf("%d", &sup_choice);
                switch(sup_choice) {
                    case 1: addSupplier(); break;
                    case 2: viewAllSuppliers(); break;
                }
                break;
            }
            case 8: {
                int app_choice;
                printf("\n" CYAN "--- APPOINTMENT & REMINDERS ---" RESET "\n");
                printf("1. Schedule Appointment\n");
                printf("2. View All Appointments\n");
                printf("3. Schedule Service Reminder\n");
                printf("4. View All Reminders\n");
                printf("5. Go Back\n");
                printf(YELLOW "Enter choice: " RESET);
                scanf("%d", &app_choice);
                switch(app_choice) {
                    case 1: scheduleAppointment(uid, "Admin"); break;
                    case 2: viewAllAppointments(); break;
                    case 3: scheduleServiceReminder(); break;
                    case 4: viewAllReminders(); break;
                }
                break;
            }
            case 9: {
                int fin_choice;
                printf("\n" CYAN "--- FINANCIAL MANAGEMENT ---" RESET "\n");
                printf("1. Add Expense\n");
                printf("2. View All Expenses\n");
                printf("3. Generate Profit/Loss Report\n");
                printf("4. Update Job Labor Cost\n");
                printf("5. Go Back\n");
                printf(YELLOW "Enter choice: " RESET);
                scanf("%d", &fin_choice);
                switch(fin_choice) {
                    case 1: addExpense(); break;
                    case 2: viewAllExpenses(); break;
                    case 3: generateProfitLossReport(); break;
                    case 4: updateJobLaborCost(); break;
                }
                break;
            }
            case 10: {
                int rep_choice;
                printf("\n" CYAN "--- REPORTING ---" RESET "\n");
                printf("1. Job Summary Report\n");
                printf("2. Mechanic Performance Report\n");
                printf("3. Customer Lifetime Value Report\n");
                printf("4. Monthly Revenue Report\n");
                printf("5. Part Usage Report\n");
                printf("6. Supplier Performance Report\n");
                printf("7. Customer Retention Report\n");
                printf("8. Go Back\n");
                printf(YELLOW "Enter choice: " RESET);
                scanf("%d", &rep_choice);
                switch(rep_choice) {
                    case 1: viewJobSummaryReport(); break;
                    case 2: mechanicPerformanceReport(); break;
                    case 3: customerLifetimeValueReport(); break;
                    case 4: monthlyRevenueReport(); break;
                    case 5: partUsageReport(); break;
                    case 6: supplierPerformanceReport(); break;
                    case 7: customerRetentionReport(); break;
                }
                break;
            }
            case 11: {
                int data_choice;
                printf("\n" CYAN "--- DATA MANAGEMENT ---" RESET "\n");
                printf("1. Backup Data\n");
                printf("2. Restore Data\n");
                printf("3. Go Back\n");
                printf(YELLOW "Enter choice: " RESET);
                scanf("%d", &data_choice);
                switch(data_choice) {
                    case 1: backupData(); break;
                    case 2: restoreData(); break;
                }
                break;
            }
            case 12: changePassword(uid); break;
            case 13: viewNotifications(uid); break;
            case 14: return;
            default: printf(RED "Invalid choice. Please try again.\n" RESET); pauseScreen();
        }
    }
}

void seniorMechanicMenu(const char *uid) {
    int choice;
    while (1) {
        showDashboard();
        printf("\n" GREEN "--- SENIOR MECHANIC MENU ---" RESET "\n");
        printf("1. View My Jobs\n");
        printf("2. View All Jobs\n");
        printf("3. View Pending Jobs\n");
        printf("4. Update Job Status\n");
        printf("5. Add Part to Job\n");
        printf("6. View All Customers\n");
        printf("7. View All Vehicles\n");
        printf("8. View Inventory\n");
        printf("9. View All Invoices\n");
        printf("10. View Notifications\n");
        printf("11. Change Password\n");
        printf("12. Logout\n");
        printf(YELLOW "Enter your choice: " RESET);
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: viewMyJobs(uid); break;
            case 2: viewAllJobs(); break;
            case 3: viewPendingJobs(); break;
            case 4: updateJobStatus(uid, "SeniorMechanic"); break;
            case 5: addPartToJob(); break;
            case 6: viewAllCustomers(); break;
            case 7: viewAllVehicles(); break;
            case 8: viewInventory(); break;
            case 9: viewAllInvoices(); break;
            case 10: viewNotifications(uid); break;
            case 11: changePassword(uid); break;
            case 12: return;
            default: printf(RED "Invalid choice. Please try again.\n" RESET); pauseScreen();
        }
    }
}

void juniorMechanicMenu(const char *uid) {
    int choice;
    while (1) {
        printf("\n" GREEN "--- JUNIOR MECHANIC MENU ---" RESET "\n");
        printf("1. View My Jobs\n");
        printf("2. Update Job Status\n");
        printf("3. Add Part to Job\n");
        printf("4. View All Customers\n");
        printf("5. View All Vehicles\n");
        printf("6. View Inventory\n");
        printf("7. View Notifications\n");
        printf("8. Change Password\n");
        printf("9. Logout\n");
        printf(YELLOW "Enter your choice: " RESET);
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: viewMyJobs(uid); break;
            case 2: updateJobStatus(uid, "JuniorMechanic"); break;
            case 3: addPartToJob(); break;
            case 4: viewAllCustomers(); break;
            case 5: viewAllVehicles(); break;
            case 6: viewInventory(); break;
            case 7: viewNotifications(uid); break;
            case 8: changePassword(uid); break;
            case 9: return;
            default: printf(RED "Invalid choice. Please try again.\n" RESET); pauseScreen();
        }
    }
}

void customerMenu(const char *uid) {
    int choice;
    while (1) {
        printf("\n" GREEN "--- CUSTOMER MENU ---" RESET "\n");
        printf("1. View My Vehicles\n");
        printf("2. View My Jobs\n");
        printf("3. View All Invoices\n");
        printf("4. Schedule Appointment\n");
        printf("5. View Notifications\n");
        printf("6. Change Password\n");
        printf("7. Logout\n");
        printf(YELLOW "Enter your choice: " RESET);
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: viewCustomerVehicles(uid); break;
            case 2: viewCustomerJobs(uid); break;
            case 3: viewAllInvoices(); break;
            case 4: scheduleAppointment(uid, "Customer"); break;
            case 5: viewNotifications(uid); break;
            case 6: changePassword(uid); break;
            case 7: return;
            default: printf(RED "Invalid choice. Please try again.\n" RESET); pauseScreen();
        }
    }
}

void printLogo() {
    printf(GREEN "**************************************\n" RESET);
    printf(CYAN "* AUTOMOTIVE SERVICE SYSTEM     *\n" RESET);
    printf(GREEN "**************************************\n" RESET);
}

// Main function to run the program
int main() {
    printLogo();
    int choice;
    char role[20], uid[10];
    
    while(1) {
        printf("\n" MAGENTA "1. Login\n" RESET);
        printf(MAGENTA "2. Register\n" RESET);
        printf(MAGENTA "3. Exit\n" RESET);
        printf(YELLOW "Enter your choice: " RESET);
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1:
                if (login(role, uid)) {
                    printf(GREEN "Login successful! Welcome, %s!\n" RESET, role);
                    if (strcmp(role, "Admin") == 0) {
                        adminMenu(uid);
                    } else if (strcmp(role, "SeniorMechanic") == 0) {
                        seniorMechanicMenu(uid);
                    } else if (strcmp(role, "JuniorMechanic") == 0) {
                        juniorMechanicMenu(uid);
                    } else { // Customer
                        customerMenu(uid);
                    }
                } else {
                    printf(RED "Login failed.\n" RESET);
                }
                break;
            case 2:
                registerUser();
                break;
            case 3:
                printf(CYAN "Exiting program.\n" RESET);
                return 0;
            default:
                printf(RED "Invalid choice. Please try again.\n" RESET);
                break;
        }
    }

    return 0;
}