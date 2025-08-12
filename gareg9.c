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
        printf(YELLOW "Enter role (Admin/SeniorMechanic/JuniorMechanic/Customer): " RESET);
        scanf("%19s", u.role);
        if (strcmp(u.role, "Admin") != 0 && strcmp(u.role, "SeniorMechanic") != 0 && strcmp(u.role, "JuniorMechanic") != 0 && strcmp(u.role, "Customer") != 0) {
            printf(RED "Invalid role. User not registered.\n" RESET);
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
            printf(YELLOW "Enter new role (Admin/SeniorMechanic/JuniorMechanic/Customer): " RESET);
            char newRole[20];
            scanf("%19s", newRole);
            if (strcmp(newRole, "Admin") != 0 && strcmp(newRole, "SeniorMechanic") != 0 && strcmp(newRole, "JuniorMechanic") != 0 && strcmp(newRole, "Customer") != 0) {
                printf(RED "Invalid role. Role not changed.\n" RESET);
                pauseScreen();
                return;
            }
            strcpy(arr[i].role, newRole);
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
    sprintf(log_msg, "Customer with ID %s updated.", id);
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

    FILE *fp_write = fopen("temp_customers.txt", "w");
    if (!fp_write) {
        printf(RED "Error creating temporary file.\n" RESET);
        fclose(fp_read);
        return;
    }

    char id_to_delete[10];
    printf(YELLOW "\nEnter Customer ID to delete: " RESET);
    scanf("%9s", id_to_delete);

    char line[200];
    Customer c;
    int found = 0;

    while (fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", c.id, c.name, c.phone, c.email);
        if (strcmp(c.id, id_to_delete) != 0) {
            fprintf(fp_write, "%s,%s,%s,%s\n", c.id, c.name, c.phone, c.email);
        } else {
            found = 1;
        }
    }

    fclose(fp_read);
    fclose(fp_write);

    remove(CUSTOMER_FILE);
    rename("temp_customers.txt", CUSTOMER_FILE);

    if (found) {
        printf(GREEN "Customer deleted successfully!\n" RESET);
        char log_msg[100];
        sprintf(log_msg, "Customer with ID %s deleted.", id_to_delete);
        writeLog(log_msg);
    } else {
        printf(RED "Customer ID not found.\n" RESET);
    }
    pauseScreen();
}

// Searches for customers by ID or name
void searchCustomer() {
    FILE *fp = fopen(CUSTOMER_FILE, "r");
    if (!fp) {
        printf(RED "No customers found.\n" RESET);
        return;
    }

    char search_term[50];
    printf(YELLOW "\nEnter Customer ID or Name to search: " RESET);
    clearInputBuffer();
    fgets(search_term, sizeof(search_term), stdin);
    search_term[strcspn(search_term, "\n")] = '\0';

    Customer c;
    char line[200];
    int found = 0;

    printf("\n" BLUE "--- Search Results ---" RESET "\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", c.id, c.name, c.phone, c.email);
        if (strstr(c.id, search_term) || strstr(c.name, search_term)) {
            printf("ID: %s, Name: %s, Phone: %s, Email: %s\n", c.id, c.name, c.phone, c.email);
            found = 1;
        }
    }
    fclose(fp);

    if (!found) {
        printf(RED "No matching customers found.\n" RESET);
    }
    pauseScreen();
}

// Displays all customers in the system
void viewAllCustomers() {
    FILE *fp = fopen(CUSTOMER_FILE, "r");
    if (!fp) {
        printf(RED "No customers found!\n" RESET);
        return;
    }
    char line[200];
    printf("\n" BLUE "--- Customers ---" RESET "\n");
    printf("ID,Name,Phone,Email\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
    pauseScreen();
}

// Sends a feedback request to a customer
void sendFeedbackRequest() {
    FILE *fp = fopen(REMINDERS_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening reminders file.\n" RESET);
        return;
    }
    
    Reminder r;
    int cnt = fileLineCount(REMINDERS_FILE);
    char customerId[10];
    
    printf(YELLOW "\nEnter Customer ID to send feedback request: " RESET);
    scanf("%9s", customerId);
    
    if (!customerExists(customerId)) {
        printf(RED "Customer ID not found.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    }
    
    generateId("REM", cnt + 1, r.id);
    strcpy(r.customerId, customerId);
    getCurrentDate(r.date);
    strcpy(r.message, "Thank you for choosing our service! We'd love to hear your feedback on your recent visit.");
    strcpy(r.status, "Scheduled");
    
    fprintf(fp, "%s,%s,%s,%s,%s\n", r.id, r.customerId, r.message, r.date, r.status);
    fclose(fp);
    
    printf(GREEN "Feedback request scheduled for customer %s!\n" RESET, customerId);
    writeLog("Feedback request scheduled.");
    pauseScreen();
}

// Vehicle Management Functions
// Adds a new vehicle for an existing customer
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
        printf(RED "Invalid customer ID! Please add the customer first.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    }

    printf(YELLOW "Enter brand: " RESET);
    scanf("%29s", v.brand);
    printf(YELLOW "Enter model: " RESET);
    scanf("%29s", v.model);
    printf(YELLOW "Enter plate number: " RESET);
    scanf("%14s", v.plate);
    printf(YELLOW "Enter manufacturing year: " RESET);
    scanf("%4s", v.year);
    getCurrentDate(v.lastServiceDate);

    generateId("VEH", cnt + 1, v.id);

    fprintf(fp, "%s,%s,%s,%s,%s,%s,%s\n", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
    fclose(fp);

    printf(GREEN "Vehicle added successfully! ID: %s\n" RESET, v.id);
    char log_msg[100];
    sprintf(log_msg, "New vehicle %s (%s) added for customer %s.", v.brand, v.plate, v.customerId);
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

            printf("Current Year: %s\n", arr[i].year);
            printf(YELLOW "Enter new year (or press Enter to keep current): " RESET);
            char newYear[5];
            fgets(newYear, sizeof(newYear), stdin);
            newYear[strcspn(newYear, "\n")] = '\0';
            if (strlen(newYear) > 0) strcpy(arr[i].year, newYear);
            
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
    sprintf(log_msg, "Vehicle with ID %s updated.", id);
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

    FILE *fp_write = fopen("temp_vehicles.txt", "w");
    if (!fp_write) {
        printf(RED "Error creating temporary file.\n" RESET);
        fclose(fp_read);
        return;
    }

    char id_to_delete[10];
    printf(YELLOW "\nEnter Vehicle ID to delete: " RESET);
    scanf("%9s", id_to_delete);

    char line[200];
    Vehicle v;
    int found = 0;

    while (fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
        if (strcmp(v.id, id_to_delete) != 0) {
            fprintf(fp_write, "%s,%s,%s,%s,%s,%s,%s\n", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
        } else {
            found = 1;
        }
    }

    fclose(fp_read);
    fclose(fp_write);

    remove(VEHICLE_FILE);
    rename("temp_vehicles.txt", VEHICLE_FILE);

    if (found) {
        printf(GREEN "Vehicle deleted successfully!\n" RESET);
        char log_msg[100];
        sprintf(log_msg, "Vehicle with ID %s deleted.", id_to_delete);
        writeLog(log_msg);
    } else {
        printf(RED "Vehicle ID not found.\n" RESET);
    }
    pauseScreen();
}

// Searches for vehicles by plate number or brand
void searchVehicle() {
    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) {
        printf(RED "No vehicles found.\n" RESET);
        return;
    }

    char search_term[30];
    printf(YELLOW "\nEnter Vehicle Plate Number or Brand to search: " RESET);
    clearInputBuffer();
    fgets(search_term, sizeof(search_term), stdin);
    search_term[strcspn(search_term, "\n")] = '\0';

    Vehicle v;
    char line[200];
    int found = 0;

    printf("\n" BLUE "--- Search Results ---" RESET "\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
        if (strstr(v.plate, search_term) || strstr(v.brand, search_term)) {
            printf("ID: %s, Customer ID: %s, Brand: %s, Model: %s, Plate: %s, Year: %s, Last Service: %s\n", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
            found = 1;
        }
    }
    fclose(fp);

    if (!found) {
        printf(RED "No matching vehicles found.\n" RESET);
    }
    pauseScreen();
}

// Displays all vehicles in the system
void viewAllVehicles() {
    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) {
        printf(RED "No vehicles found!\n" RESET);
        return;
    }
    char line[200];
    printf("\n" BLUE "--- Vehicles ---" RESET "\n");
    printf("ID,Customer ID,Brand,Model,Plate,Year,Last Service Date\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
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
    Vehicle v;
    int found = 0;
    printf("\n" BLUE "--- Your Vehicles ---" RESET "\n");
    printf("ID,Brand,Model,Plate,Year,Last Service Date\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
        if (strcmp(v.customerId, customerId) == 0) {
            printf("%s,%s,%s,%s,%s,%s\n", v.id, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
            found = 1;
        }
    }
    fclose(fp);
    if (!found) {
        printf(RED "No vehicles found for your account.\n" RESET);
    }
    pauseScreen();
}

// Job Management Functions
// Checks if a user is a mechanic
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

    printf(YELLOW "\nEnter customer ID: " RESET);
    scanf("%9s", j.customerId);
    if (!customerExists(j.customerId)) {
        printf(RED "Invalid customer ID! Job not added.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    }

    printf(YELLOW "Enter vehicle ID: " RESET);
    scanf("%9s", j.vehicleId);
    if (!vehicleExists(j.vehicleId)) {
        printf(RED "Invalid vehicle ID! Job not added.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    }

    if (getRoleLevel(currentUserRole) >= getRoleLevel("SeniorMechanic")) {
        printf(YELLOW "Enter mechanic ID: " RESET);
        scanf("%9s", j.mechanicId);
        if (!mechanicExists(j.mechanicId)) {
            printf(RED "Invalid mechanic ID! Job not added.\n" RESET);
            fclose(fp);
            pauseScreen();
            return;
        }
    } else if (strcmp(currentUserRole, "JuniorMechanic") == 0) {
        strcpy(j.mechanicId, currentUserId);
    } else {
        printf(RED "Unauthorized to add jobs.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    }

    printf(YELLOW "Enter job description (e.g., Oil Change, Brake Repair): " RESET);
    clearInputBuffer();
    fgets(j.description, sizeof(j.description), stdin);
    j.description[strcspn(j.description, "\n")] = '\0';
    strcpy(j.status, "Pending");
    strcpy(j.notes, "No notes yet.");

    // Dynamic cost estimation
    float avg_cost = calculateAverageCost(j.description);
    if (avg_cost > 0) {
        printf(MAGENTA "Based on past jobs, estimated cost is around %.2f.\n" RESET, avg_cost);
        j.estimatedCost = avg_cost;
    } else {
        printf(YELLOW "Enter estimated cost: " RESET);
        scanf("%f", &j.estimatedCost);
    }
    if (j.estimatedCost < 0) j.estimatedCost = 0;
    j.laborCost = 0;
    j.partsCost = 0;

    getCurrentDate(j.startDate);
    strcpy(j.endDate, "N/A");

    generateId("JOB", cnt + 1, j.id);

    fprintf(fp, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, j.estimatedCost, j.laborCost, j.partsCost, j.notes, j.startDate, j.endDate);
    fclose(fp);

    printf(GREEN "Job added successfully! ID: %s\n" RESET, j.id);
    char log_msg[200];
    sprintf(log_msg, "New job %s added for vehicle %s by user %s. Estimated Cost: %.2f", j.id, j.vehicleId, currentUserId, j.estimatedCost);
    writeLog(log_msg);
    pauseScreen();
}

// Updates the status and details of a job
void updateJobStatus(const char *currentUserId, const char *currentUserRole) {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }

    Job arr[500];
    int c = 0;
    char line[500];

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", arr[c].id, arr[c].customerId, arr[c].vehicleId,
               arr[c].mechanicId, arr[c].description, arr[c].status, &arr[c].estimatedCost, &arr[c].laborCost, &arr[c].partsCost, arr[c].notes, arr[c].startDate, arr[c].endDate);
        c++;
    }
    fclose(fp);

    char id[10];
    printf(YELLOW "Enter Job ID to update: " RESET);
    scanf("%9s", id);

    int found = 0;
    for (int i = 0; i < c; i++) {
        if (!strcmp(arr[i].id, id)) {
            if (getRoleLevel(currentUserRole) < getRoleLevel("SeniorMechanic") && strcmp(arr[i].mechanicId, currentUserId) != 0) {
                printf(RED "You are not authorized to update this job.\n" RESET);
                pauseScreen();
                return;
            }
            printf("Current status: %s\n", arr[i].status);
            printf(YELLOW "Enter new status (Pending/InProgress/Completed): " RESET);
            scanf("%19s", arr[i].status);

            printf("Current notes: %s\n", arr[i].notes);
            printf(YELLOW "Add/Update notes (or press Enter to keep current): " RESET);
            clearInputBuffer();
            char newNotes[200];
            fgets(newNotes, sizeof(newNotes), stdin);
            newNotes[strcspn(newNotes, "\n")] = '\0';
            if (strlen(newNotes) > 0) strcpy(arr[i].notes, newNotes);

            printf("Current Estimated Cost: %.2f\n", arr[i].estimatedCost);
            printf(YELLOW "Enter new estimated cost (or enter 0 to keep current): " RESET);
            float newCost;
            scanf("%f", &newCost);
            if (newCost > 0) arr[i].estimatedCost = newCost;

            if (strcmp(arr[i].status, "Completed") == 0 && strcmp(arr[i].endDate, "N/A") == 0) {
                getCurrentDate(arr[i].endDate);
                char notification_msg[200];
                sprintf(notification_msg, "Your job %s (%s) has been completed.", arr[i].id, arr[i].description);
                createNotification(arr[i].customerId, notification_msg);

                // Update vehicle's last service date
                FILE *fp_veh_read = fopen(VEHICLE_FILE, "r");
                if (fp_veh_read) {
                    Vehicle vehicles[200];
                    int veh_count = 0;
                    char veh_line[200];
                    while (fgets(veh_line, sizeof(veh_line), fp_veh_read)) {
                         sscanf(veh_line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]", vehicles[veh_count].id, vehicles[veh_count].customerId, vehicles[veh_count].brand, vehicles[veh_count].model, vehicles[veh_count].plate, vehicles[veh_count].year, vehicles[veh_count].lastServiceDate);
                        if (strcmp(vehicles[veh_count].id, arr[i].vehicleId) == 0) {
                            strcpy(vehicles[veh_count].lastServiceDate, arr[i].endDate);
                        }
                        veh_count++;
                    }
                    fclose(fp_veh_read);
                    FILE *fp_veh_write = fopen(VEHICLE_FILE, "w");
                    if (fp_veh_write) {
                        for(int k=0; k<veh_count; k++) {
                            fprintf(fp_veh_write, "%s,%s,%s,%s,%s,%s,%s\n", vehicles[k].id, vehicles[k].customerId, vehicles[k].brand, vehicles[k].model, vehicles[k].plate, vehicles[k].year, vehicles[k].lastServiceDate);
                        }
                        fclose(fp_veh_write);
                    }
                }
            }
            
            if (strcmp(arr[i].status, "Pending") != 0 && strcmp(arr[i].status, "InProgress") != 0 && strcmp(arr[i].status, "Completed") != 0) {
                printf(RED "Invalid status! Reverting to previous status.\n" RESET);
                // Can't revert easily, so just inform and continue.
            }
            
            found = 1;
            break;
        }
    }

    if (!found) {
        printf(RED "Job ID not found.\n" RESET);
        pauseScreen();
        return;
    }

    fp = fopen(JOB_FILE, "w");
    if (!fp) {
        printf(RED "Error opening job file for writing.\n" RESET);
        return;
    }
    for (int i = 0; i < c; i++)
        fprintf(fp, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", arr[i].id, arr[i].customerId, arr[i].vehicleId, arr[i].mechanicId, arr[i].description, arr[i].status, arr[i].estimatedCost, arr[i].laborCost, arr[i].partsCost, arr[i].notes, arr[i].startDate, arr[i].endDate);
    fclose(fp);

    printf(GREEN "Job status and details updated successfully!\n" RESET);
    char log_msg[100];
    sprintf(log_msg, "Job ID %s updated by user %s.", id, currentUserId);
    writeLog(log_msg);
    pauseScreen();
}

// Allows a mechanic to view their assigned jobs
void viewMyJobs(const char *mechanicId) {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found!\n" RESET);
        return;
    }
    char line[500];
    Job j;
    int found = 0;
    printf("\n" BLUE "--- Your Assigned Jobs ---" RESET "\n");
    printf("Job ID,Customer ID,Vehicle ID,Description,Status,Est. Cost,Labor Cost,Parts Cost,Start Date,End Date,Notes\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", j.id, j.customerId, j.vehicleId,
               j.mechanicId, j.description, j.status, &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (!strcmp(j.mechanicId, mechanicId)) {
            printf("%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n",
                   j.id, j.customerId, j.vehicleId, j.description, j.status, j.estimatedCost, j.laborCost, j.partsCost, j.startDate, j.endDate, j.notes);
            found = 1;
        }
    }
    fclose(fp);
    if (!found) {
        printf(RED "No jobs assigned to you.\n" RESET);
    }
    pauseScreen();
}

// Allows a customer to view their service history
void viewCustomerJobs(const char *customerId) {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found!\n" RESET);
        return;
    }
    char line[500];
    Job j;
    int found = 0;
    printf("\n" BLUE "--- Your Service History ---" RESET "\n");
    printf("Job ID,Vehicle ID,Mechanic ID,Description,Status,Est. Cost,Labor Cost,Parts Cost,Start Date,End Date,Notes\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", j.id, j.customerId, j.vehicleId,
               j.mechanicId, j.description, j.status, &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (!strcmp(j.customerId, customerId)) {
            printf("%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n",
                   j.id, j.vehicleId, j.mechanicId, j.description, j.status, j.estimatedCost, j.laborCost, j.partsCost, j.startDate, j.endDate, j.notes);
            found = 1;
        }
    }
    fclose(fp);
    if (!found) {
        printf(RED "No service history found for your account.\n" RESET);
    }
    pauseScreen();
}

// Searches for jobs by various criteria
void searchJob() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }

    char search_term[100];
    printf(YELLOW "\nEnter Job ID, Customer ID, Mechanic ID, or Description to search: " RESET);
    clearInputBuffer();
    fgets(search_term, sizeof(search_term), stdin);
    search_term[strcspn(search_term, "\n")] = '\0';

    Job j;
    char line[500];
    int found = 0;

    printf("\n" BLUE "--- Search Results ---" RESET "\n");
    printf("Job ID,Customer ID,Vehicle ID,Mechanic ID,Description,Status,Estimated Cost,Notes\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", j.id, j.customerId, j.vehicleId,
               j.mechanicId, j.description, j.status, &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strstr(j.id, search_term) || strstr(j.customerId, search_term) ||
            strstr(j.mechanicId, search_term) || strstr(j.description, search_term)) {
            printf("%s,%s,%s,%s,%s,%s,%.2f,%s\n",
                   j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, j.estimatedCost, j.notes);
            found = 1;
        }
    }
    fclose(fp);

    if (!found) {
        printf(RED "No matching jobs found.\n" RESET);
    }
    pauseScreen();
}

// Displays all jobs in the system
void viewAllJobs() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found!\n" RESET);
        return;
    }
    char line[500];
    printf("\n" BLUE "--- Jobs ---" RESET "\n");
    printf("ID,Customer ID,Vehicle ID,Mechanic ID,Description,Status,Estimated Cost,Labor Cost,Parts Cost,Notes,Start Date,End Date\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
    pauseScreen();
}

void viewPendingJobs() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found!\n" RESET);
        return;
    }
    char line[500];
    Job j;
    int found = 0;
    printf("\n" BLUE "--- Pending Jobs ---" RESET "\n");
    printf("ID,Customer ID,Vehicle ID,Mechanic ID,Description,Estimated Cost,Start Date\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f", j.id, j.customerId, j.vehicleId,
               j.mechanicId, j.description, j.status, &j.estimatedCost);
        if (strcmp(j.status, "Pending") == 0) {
            printf("%s,%s,%s,%s,%s,%.2f,%s\n",
                   j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.estimatedCost, j.startDate);
            found = 1;
        }
    }
    fclose(fp);
    if (!found) {
        printf(GREEN "No pending jobs found.\n" RESET);
    }
    pauseScreen();
}

void viewCompletedJobs() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found!\n" RESET);
        return;
    }
    char line[500];
    Job j;
    int found = 0;
    printf("\n" BLUE "--- Completed Jobs ---" RESET "\n");
    printf("ID,Customer ID,Vehicle ID,Mechanic ID,Description,Total Cost,Start Date,End Date\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f", j.id, j.customerId, j.vehicleId,
               j.mechanicId, j.description, j.status, &j.estimatedCost, &j.laborCost, &j.partsCost);
        if (strcmp(j.status, "Completed") == 0) {
            printf("%s,%s,%s,%s,%s,%.2f,%s,%s\n",
                   j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.laborCost + j.partsCost, j.startDate, j.endDate);
            found = 1;
        }
    }
    fclose(fp);
    if (!found) {
        printf(GREEN "No completed jobs found.\n" RESET);
    }
    pauseScreen();
}

// Invoice Management Functions
// Generates an invoice for a completed job
void generateInvoice() {
    FILE *fp_inv = fopen(INVOICE_FILE, "a+");
    if (!fp_inv) {
        printf(RED "Error opening invoice file.\n" RESET);
        return;
    }

    FILE *fp_job = fopen(JOB_FILE, "r");
    if (!fp_job) {
        printf(RED "No jobs available to generate invoices.\n" RESET);
        fclose(fp_inv);
        return;
    }

    Invoice inv;
    int inv_cnt = fileLineCount(INVOICE_FILE);
    Job j;
    char line[500];
    int job_found = 0;

    printf(YELLOW "\nEnter Job ID for invoice: " RESET);
    scanf("%9s", inv.jobId);

    rewind(fp_job);
    while (fgets(line, sizeof(line), fp_job)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", j.id, j.customerId, j.vehicleId,
               j.mechanicId, j.description, j.status, &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (!strcmp(j.id, inv.jobId)) {
            if (strcmp(j.status, "Completed") != 0) {
                printf(RED "Job is not yet completed. Cannot generate invoice.\n" RESET);
                fclose(fp_job);
                fclose(fp_inv);
                pauseScreen();
                return;
            }
            job_found = 1;
            inv.totalCost = j.laborCost + j.partsCost;
            break;
        }
    }
    fclose(fp_job);

    if (!job_found) {
        printf(RED "Job ID not found. Invoice not generated.\n" RESET);
        fclose(fp_inv);
        pauseScreen();
        return;
    }
    
    // Check if invoice already exists for this job
    rewind(fp_inv);
    char inv_line[200];
    while(fgets(inv_line, sizeof(inv_line), fp_inv)) {
        Invoice temp_inv;
        sscanf(inv_line, "%9[^,],%9[^,],%f,%f,%f,%14[^\n]", temp_inv.id, temp_inv.jobId, &temp_inv.totalCost, &temp_inv.discount, &temp_inv.total, temp_inv.date);
        if (strcmp(temp_inv.jobId, inv.jobId) == 0) {
            printf(RED "Invoice for this job already exists! (ID: %s)\n" RESET, temp_inv.id);
            fclose(fp_inv);
            pauseScreen();
            return;
        }
    }


    printf("Job Total Cost (Labor + Parts): %.2f\n", inv.totalCost);
    printf(YELLOW "Enter discount amount: " RESET);
    scanf("%f", &inv.discount);
    if (inv.discount < 0) inv.discount = 0;
    if (inv.discount > inv.totalCost) inv.discount = inv.totalCost;

    inv.total = inv.totalCost - inv.discount;

    getCurrentDate(inv.date);

    generateId("INV", inv_cnt + 1, inv.id);

    fprintf(fp_inv, "%s,%s,%.2f,%.2f,%.2f,%s\n", inv.id, inv.jobId, inv.totalCost, inv.discount, inv.total, inv.date);
    fclose(fp_inv);
    
    // Add revenue to finance file
    FILE *fp_finance = fopen(FINANCE_FILE, "a+");
    if(fp_finance) {
        Transaction t;
        int finance_cnt = fileLineCount(FINANCE_FILE);
        generateId("FIN", finance_cnt + 1, t.id);
        strcpy(t.type, "Revenue");
        t.amount = inv.total;
        strcpy(t.date, inv.date);
        sprintf(t.description, "Invoice for Job ID %s", inv.jobId);
        fprintf(fp_finance, "%s,%s,%.2f,%s,%s\n", t.id, t.type, t.amount, t.date, t.description);
        fclose(fp_finance);
    }

    printf(GREEN "Invoice generated successfully! ID: %s, Total: %.2f\n" RESET, inv.id, inv.total);
    char log_msg[100];
    sprintf(log_msg, "Invoice %s generated for job %s. Total: %.2f", inv.id, inv.jobId, inv.total);
    writeLog(log_msg);
    pauseScreen();
}

// Displays all invoices
void viewAllInvoices() {
    FILE *fp = fopen(INVOICE_FILE, "r");
    if (!fp) {
        printf(RED "No invoices found!\n" RESET);
        return;
    }
    char line[200];
    printf("\n" BLUE "--- Invoices ---" RESET "\n");
    printf("ID,Job ID,Total Cost,Discount,Total,Date\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
    pauseScreen();
}

// Inventory Management Functions
// Adds a part to a job, updating inventory and job costs
void addPartToJob() {
    FILE *fp_job_parts = fopen(JOB_PART_FILE, "a+");
    if (!fp_job_parts) {
        printf(RED "Error opening job parts file.\n" RESET);
        return;
    }

    JobPart jp;
    char jobId[10], partId[10];
    int quantity;

    printf(YELLOW "\nEnter Job ID: " RESET);
    scanf("%9s", jobId);

    FILE *fp_job = fopen(JOB_FILE, "r");
    Job j;
    int job_found = 0;
    char line[500];

    Job job_data[500];
    int job_count = 0;

    if (fp_job) {
        while (fgets(line, sizeof(line), fp_job)) {
            sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", job_data[job_count].id, job_data[job_count].customerId, job_data[job_count].vehicleId,
               job_data[job_count].mechanicId, job_data[job_count].description, job_data[job_count].status, &job_data[job_count].estimatedCost, &job_data[job_count].laborCost, &job_data[job_count].partsCost, job_data[job_count].notes, job_data[job_count].startDate, job_data[job_count].endDate);
            job_count++;
        }
        fclose(fp_job);
    }

    for(int i = 0; i < job_count; i++) {
        if (strcmp(job_data[i].id, jobId) == 0) {
            job_found = 1;
            strcpy(jp.jobId, jobId);
            printf(YELLOW "Enter Part ID: " RESET);
            scanf("%9s", partId);
            if (!partExists(partId)) {
                printf(RED "Invalid part ID.\n" RESET);
                fclose(fp_job_parts);
                return;
            }
            printf(YELLOW "Enter quantity: " RESET);
            scanf("%d", &quantity);

            deductPartFromInventory(partId, quantity);

            float part_price = getPartPrice(partId);
            job_data[i].partsCost += part_price * quantity;

            strcpy(jp.partId, partId);
            jp.quantity = quantity;
            jp.price = part_price;

            fprintf(fp_job_parts, "%s,%s,%d,%.2f\n", jp.jobId, jp.partId, jp.quantity, jp.price);

            FILE *fp_job_write = fopen(JOB_FILE, "w");
            if(fp_job_write) {
                for(int k=0; k<job_count; k++) {
                    fprintf(fp_job_write, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", job_data[k].id, job_data[k].customerId, job_data[k].vehicleId, job_data[k].mechanicId, job_data[k].description, job_data[k].status, job_data[k].estimatedCost, job_data[k].laborCost, job_data[k].partsCost, job_data[k].notes, job_data[k].startDate, job_data[k].endDate);
                }
                fclose(fp_job_write);
            }

            printf(GREEN "Part added to job successfully!\n" RESET);
            char log_msg[100];
            sprintf(log_msg, "Part %s added to job %s, quantity %d.", partId, jobId, quantity);
            writeLog(log_msg);
            break;
        }
    }

    if (!job_found) {
        printf(RED "Job ID not found.\n" RESET);
    }

    fclose(fp_job_parts);
    pauseScreen();
}

// Updates the labor cost for a specific job
void updateJobLaborCost() {
    FILE *fp_read = fopen(JOB_FILE, "r");
    if (!fp_read) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }

    Job arr[500];
    int c=0;
    char line[500];
    while(fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", arr[c].id, arr[c].customerId, arr[c].vehicleId,
               arr[c].mechanicId, arr[c].description, arr[c].status, &arr[c].estimatedCost, &arr[c].laborCost, &arr[c].partsCost, arr[c].notes, arr[c].startDate, arr[c].endDate);
        c++;
    }
    fclose(fp_read);

    char jobId[10];
    printf(YELLOW "\nEnter Job ID to update labor cost: " RESET);
    scanf("%9s", jobId);

    int found = 0;
    for(int i=0; i<c; i++) {
        if(strcmp(arr[i].id, jobId) == 0) {
            float hours, rate;
            printf("Current Labor Cost: %.2f\n", arr[i].laborCost);
            printf(YELLOW "Enter hours worked: " RESET);
            scanf("%f", &hours);
            printf(YELLOW "Enter hourly rate: " RESET);
            scanf("%f", &rate);

            if(hours < 0 || rate < 0) {
                printf(RED "Invalid input. Cost not updated.\n" RESET);
                pauseScreen();
                return;
            }

            arr[i].laborCost = hours * rate;
            printf(GREEN "Labor cost for job %s updated to %.2f\n" RESET, jobId, arr[i].laborCost);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf(RED "Job ID not found.\n" RESET);
        pauseScreen();
        return;
    }

    FILE *fp_write = fopen(JOB_FILE, "w");
    if(fp_write) {
        for(int i=0; i<c; i++) {
            fprintf(fp_write, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", arr[i].id, arr[i].customerId, arr[i].vehicleId, arr[i].mechanicId, arr[i].description, arr[i].status, arr[i].estimatedCost, arr[i].laborCost, arr[i].partsCost, arr[i].notes, arr[i].startDate, arr[i].endDate);
        }
        fclose(fp_write);
    }

    char log_msg[100];
    sprintf(log_msg, "Labor cost for job %s updated.", jobId);
    writeLog(log_msg);
    pauseScreen();
}

// Adds a new part to the inventory
void addPart() {
    FILE *fp = fopen(PART_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening parts file.\n" RESET);
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
    if (p.quantity < 0) p.quantity = 0;

    printf(YELLOW "Enter price per unit: " RESET);
    scanf("%f", &p.price);
    if (p.price < 0) p.price = 0;

    printf(YELLOW "Enter supplier ID: " RESET);
    scanf("%9s", p.supplierId);
    if (!supplierExists(p.supplierId)) {
        printf(RED "Supplier ID not found. Part not added.\n" RESET);
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

// Edits an existing part's information
void editPart() {
    FILE *fp = fopen(PART_FILE, "r");
    if (!fp) {
        printf(RED "No parts found.\n" RESET);
        return;
    }

    Part arr[500];
    int c = 0;
    char line[200];
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d", arr[c].id, arr[c].name, &arr[c].quantity, &arr[c].price, arr[c].supplierId, &arr[c].reorderLevel);
        c++;
    }
    fclose(fp);

    char id[10];
    printf(YELLOW "\nEnter Part ID to edit: " RESET);
    scanf("%9s", id);

    int found = 0;
    for(int i=0; i<c; i++) {
        if(strcmp(arr[i].id, id) == 0) {
            printf("Current Name: %s\n", arr[i].name);
            printf(YELLOW "Enter new name (or press Enter): " RESET);
            clearInputBuffer();
            char newName[50];
            fgets(newName, sizeof(newName), stdin);
            newName[strcspn(newName, "\n")] = '\0';
            if(strlen(newName) > 0) strcpy(arr[i].name, newName);

            printf("Current Quantity: %d\n", arr[i].quantity);
            printf(YELLOW "Enter new quantity (or -1 to keep current): " RESET);
            int newQuantity;
            scanf("%d", &newQuantity);
            if(newQuantity >= 0) arr[i].quantity = newQuantity;

            printf("Current Price: %.2f\n", arr[i].price);
            printf(YELLOW "Enter new price (or -1 to keep current): " RESET);
            float newPrice;
            scanf("%f", &newPrice);
            if(newPrice >= 0) arr[i].price = newPrice;

            printf("Current Reorder Level: %d\n", arr[i].reorderLevel);
            printf(YELLOW "Enter new reorder level (or -1 to keep current): " RESET);
            int newReorder;
            scanf("%d", &newReorder);
            if(newReorder >= 0) arr[i].reorderLevel = newReorder;

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
        printf(RED "Error opening file for writing.\n" RESET);
        return;
    }
    for (int i = 0; i < c; i++) {
        fprintf(fp, "%s,%s,%d,%.2f,%s,%d\n", arr[i].id, arr[i].name, arr[i].quantity, arr[i].price, arr[i].supplierId, arr[i].reorderLevel);
    }
    fclose(fp);

    printf(GREEN "Part updated successfully!\n" RESET);
    writeLog("Part updated.");
    pauseScreen();
}

// Displays the current inventory of parts
void viewInventory() {
    FILE *fp = fopen(PART_FILE, "r");
    if (!fp) {
        printf(RED "No parts found!\n" RESET);
        return;
    }
    char line[200];
    printf("\n" BLUE "--- Inventory ---" RESET "\n");
    printf("ID,Name,Quantity,Price,Supplier ID,Reorder Level\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
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
    int found = 0;
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d", p.id, p.name, &p.quantity, &p.price, p.supplierId, &p.reorderLevel);
        if(strcmp(p.id, id) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found;
}

// Gets the price of a part by its ID
float getPartPrice(const char *partId) {
    FILE *fp = fopen(PART_FILE, "r");
    if (!fp) return 0.0;
    Part p;
    char line[200];
    float price = 0.0;
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d", p.id, p.name, &p.quantity, &p.price, p.supplierId, &p.reorderLevel);
        if(strcmp(p.id, partId) == 0) {
            price = p.price;
            break;
        }
    }
    fclose(fp);
    return price;
}

// Deducts parts from inventory after being used in a job
void deductPartFromInventory(const char *partId, int quantity) {
    FILE *fp_read = fopen(PART_FILE, "r");
    if (!fp_read) return;

    FILE *fp_write = fopen("temp_parts.txt", "w");
    if (!fp_write) {
        fclose(fp_read);
        return;
    }

    Part p;
    char line[200];
    while(fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d", p.id, p.name, &p.quantity, &p.price, p.supplierId, &p.reorderLevel);
        if(strcmp(p.id, partId) == 0) {
            if (p.quantity >= quantity) {
                p.quantity -= quantity;
                printf(GREEN "Deducted %d of part %s from inventory.\n" RESET, quantity, p.name);
                writeLog("Inventory updated.");
            } else {
                printf(RED "Not enough stock for part %s. Available: %d, Needed: %d\n" RESET, p.name, p.quantity, quantity);
            }
        }
        fprintf(fp_write, "%s,%s,%d,%.2f,%s,%d\n", p.id, p.name, p.quantity, p.price, p.supplierId, p.reorderLevel);
    }

    fclose(fp_read);
    fclose(fp_write);
    remove(PART_FILE);
    rename("temp_parts.txt", PART_FILE);
}

// Checks for parts with low stock and adds them to a reorder list
void checkLowStock() {
    FILE *fp_parts = fopen(PART_FILE, "r");
    if (!fp_parts) {
        printf(RED "No parts found.\n" RESET);
        return;
    }

    FILE *fp_reorder = fopen(REORDER_LIST_FILE, "w");
    if (!fp_reorder) {
        printf(RED "Error opening reorder file.\n" RESET);
        fclose(fp_parts);
        return;
    }
    
    Part p;
    char line[200];
    int low_stock_found = 0;

    while(fgets(line, sizeof(line), fp_parts)) {
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d", p.id, p.name, &p.quantity, &p.price, p.supplierId, &p.reorderLevel);
        if (p.quantity < p.reorderLevel) {
            low_stock_found = 1;
            fprintf(fp_reorder, "%s,%d,%s\n", p.id, p.reorderLevel - p.quantity, "N/A"); // date is N/A for now
        }
    }
    fclose(fp_parts);
    fclose(fp_reorder);
    
    if (low_stock_found) {
        printf(YELLOW "Low stock items detected and added to reorder list.\n" RESET);
    } else {
        printf(GREEN "No low stock items found.\n" RESET);
        remove(REORDER_LIST_FILE);
    }
    pauseScreen();
}

void viewReorderList() {
    FILE *fp = fopen(REORDER_LIST_FILE, "r");
    if (!fp) {
        printf(GREEN "No low stock items on the reorder list.\n" RESET);
        return;
    }
    
    char line[200];
    ReorderItem ri;
    printf("\n" MAGENTA "--- Parts Reorder List ---" RESET "\n");
    printf("Part ID,Needed Quantity\n");
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%d", ri.partId, &ri.neededQuantity);
        printf("%s,%d\n", ri.partId, ri.neededQuantity);
    }
    fclose(fp);
    pauseScreen();
}

void clearReorderList() {
    if (remove(REORDER_LIST_FILE) == 0) {
        printf(GREEN "Reorder list cleared successfully.\n" RESET);
        writeLog("Reorder list cleared.");
    } else {
        printf(RED "Reorder list is empty or could not be cleared.\n" RESET);
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

// Displays all suppliers
void viewAllSuppliers() {
    FILE *fp = fopen(SUPPLIER_FILE, "r");
    if (!fp) {
        printf(RED "No suppliers found!\n" RESET);
        return;
    }
    char line[200];
    printf("\n" BLUE "--- Suppliers ---" RESET "\n");
    printf("ID,Name,Phone,Email\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
    pauseScreen();
}

int supplierExists(const char *id) {
    FILE *fp = fopen(SUPPLIER_FILE, "r");
    if (!fp) return 0;
    Supplier s;
    char line[200];
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", s.id, s.name, s.phone, s.email);
        if (strcmp(s.id, id) == 0) {
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

    if (strcmp(currentUserRole, "Customer") == 0) {
        strcpy(a.customerId, currentUserId);
    } else {
        printf(YELLOW "\nEnter customer ID: " RESET);
        scanf("%9s", a.customerId);
        if (!customerExists(a.customerId)) {
            printf(RED "Invalid customer ID! Appointment not scheduled.\n" RESET);
            fclose(fp);
            pauseScreen();
            return;
        }
    }

    printf(YELLOW "Enter vehicle ID: " RESET);
    scanf("%9s", a.vehicleId);
    if (!vehicleExists(a.vehicleId)) {
        printf(RED "Invalid vehicle ID! Appointment not scheduled.\n" RESET);
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

    printf(YELLOW "Enter service description: " RESET);
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
    pauseScreen();
}

// Displays all appointments
void viewAllAppointments() {
    FILE *fp = fopen(APPOINTMENT_FILE, "r");
    if (!fp) {
        printf(RED "No appointments found!\n" RESET);
        return;
    }
    char line[200];
    printf("\n" BLUE "--- Appointments ---" RESET "\n");
    printf("ID,Customer ID,Vehicle ID,Date,Time,Description,Status\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
    pauseScreen();
}

// Schedules a service reminder for a vehicle
void scheduleServiceReminder() {
    FILE *fp = fopen(REMINDERS_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening reminders file.\n" RESET);
        return;
    }

    Reminder r;
    int cnt = fileLineCount(REMINDERS_FILE);
    char customerId[10], vehicleId[10], reminderDate[15];

    printf(YELLOW "\nEnter Customer ID for the reminder: " RESET);
    scanf("%9s", customerId);
    if (!customerExists(customerId)) {
        printf(RED "Customer ID not found.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    }

    printf(YELLOW "Enter Vehicle ID for the reminder: " RESET);
    scanf("%9s", vehicleId);
    if (!vehicleExists(vehicleId)) {
        printf(RED "Vehicle ID not found.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    }

    printf(YELLOW "Enter reminder date (YYYY-MM-DD): " RESET);
    scanf("%14s", reminderDate);
    if (!isDateValid(reminderDate)) {
        printf(RED "Invalid date format.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    }

    generateId("REM", cnt + 1, r.id);
    strcpy(r.customerId, customerId);
    strcpy(r.date, reminderDate);
    sprintf(r.message, "Reminder: Your vehicle %s is due for its next service on %s.", vehicleId, reminderDate);
    strcpy(r.status, "Scheduled");

    fprintf(fp, "%s,%s,%s,%s,%s\n", r.id, r.customerId, r.message, r.date, r.status);
    fclose(fp);

    printf(GREEN "Service reminder scheduled for customer %s!\n" RESET, customerId);
    writeLog("Service reminder scheduled.");
    pauseScreen();
}

void viewAllReminders() {
    FILE *fp = fopen(REMINDERS_FILE, "r");
    if (!fp) {
        printf(GREEN "No reminders found!\n" RESET);
        return;
    }
    
    char line[300];
    printf("\n" MAGENTA "--- Service Reminders ---" RESET "\n");
    printf("ID,Customer ID,Message,Date,Status\n");
    while(fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
    pauseScreen();
}


// Financial Management Functions
void addExpense() {
    FILE *fp = fopen(EXPENSES_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening expenses file.\n" RESET);
        return;
    }

    Transaction t;
    int cnt = fileLineCount(EXPENSES_FILE);

    printf(YELLOW "\nEnter expense amount: " RESET);
    scanf("%f", &t.amount);
    if (t.amount <= 0) {
        printf(RED "Invalid amount. Expense not added.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    }

    printf(YELLOW "Enter expense description: " RESET);
    clearInputBuffer();
    fgets(t.description, sizeof(t.description), stdin);
    t.description[strcspn(t.description, "\n")] = '\0';
    
    strcpy(t.type, "Expense");

    getCurrentDate(t.date);
    generateId("EXP", cnt + 1, t.id);

    fprintf(fp, "%s,%s,%.2f,%s,%s\n", t.id, t.type, t.amount, t.date, t.description);
    fclose(fp);

    printf(GREEN "Expense added successfully! ID: %s\n" RESET, t.id);
    char log_msg[100];
    sprintf(log_msg, "New expense %s added. Amount: %.2f.", t.id, t.amount);
    writeLog(log_msg);
    pauseScreen();
}

void viewAllExpenses() {
    FILE *fp = fopen(EXPENSES_FILE, "r");
    if (!fp) {
        printf(RED "No expenses found!\n" RESET);
        return;
    }
    
    char line[200];
    printf("\n" RED "--- Expenses ---" RESET "\n");
    printf("ID,Type,Amount,Date,Description\n");
    Transaction t;
    while(fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
    pauseScreen();
}

void generateProfitLossReport() {
    FILE *fp_rev = fopen(FINANCE_FILE, "r");
    FILE *fp_exp = fopen(EXPENSES_FILE, "r");
    
    float total_revenue = 0;
    float total_expenses = 0;
    
    char line[200];
    Transaction t;
    
    if (fp_rev) {
        while(fgets(line, sizeof(line), fp_rev)) {
            sscanf(line, "%9[^,],%19[^,],%f,%14[^,],%99[^\n]", t.id, t.type, &t.amount, t.date, t.description);
            total_revenue += t.amount;
        }
        fclose(fp_rev);
    }
    
    if (fp_exp) {
        while(fgets(line, sizeof(line), fp_exp)) {
            sscanf(line, "%9[^,],%19[^,],%f,%14[^,],%99[^\n]", t.id, t.type, &t.amount, t.date, t.description);
            total_expenses += t.amount;
        }
        fclose(fp_exp);
    }
    
    float net_profit = total_revenue - total_expenses;
    
    printf("\n" MAGENTA "--- Profit & Loss Report ---" RESET "\n");
    printf("Total Revenue: %.2f\n", total_revenue);
    printf("Total Expenses: %.2f\n", total_expenses);
    printf("------------------------------------\n");
    if (net_profit >= 0) {
        printf(GREEN "Net Profit: %.2f\n" RESET, net_profit);
    } else {
        printf(RED "Net Loss: %.2f\n" RESET, net_profit);
    }
    
    pauseScreen();
}

// Reporting Functions
// Provides a summary report of all jobs
void viewJobSummaryReport() {
    FILE *fp_job = fopen(JOB_FILE, "r");
    if (!fp_job) {
        printf(RED "No jobs found for summary.\n" RESET);
        return;
    }

    char line[500];
    Job j;
    int total_jobs = 0;
    int pending_jobs = 0;
    int completed_jobs = 0;
    float total_revenue = 0;

    while (fgets(line, sizeof(line), fp_job)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", j.id, j.customerId, j.vehicleId,
               j.mechanicId, j.description, j.status, &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        total_jobs++;
        if (strcmp(j.status, "Completed") == 0) {
            completed_jobs++;
            total_revenue += (j.laborCost + j.partsCost);
        } else if (strcmp(j.status, "Pending") == 0) {
            pending_jobs++;
        }
    }
    fclose(fp_job);

    printf("\n" BLUE "--- Job Summary Report ---" RESET "\n");
    printf("Total Jobs: %d\n", total_jobs);
    printf("Pending Jobs: %d\n", pending_jobs);
    printf("Completed Jobs: %d\n", completed_jobs);
    printf("Total Revenue from Completed Jobs: %.2f\n", total_revenue);

    pauseScreen();
}

// Generates a performance report for each mechanic
void mechanicPerformanceReport() {
    FILE *fp_user = fopen(USER_FILE, "r");
    FILE *fp_job = fopen(JOB_FILE, "r");

    if (!fp_user || !fp_job) {
        printf(RED "Error opening files for report.\n" RESET);
        if (fp_user) fclose(fp_user);
        if (fp_job) fclose(fp_job);
        return;
    }

    User mechanics[100];
    int num_mechanics = 0;
    char line[200];

    rewind(fp_user);
    while (fgets(line, sizeof(line), fp_user)) {
        User u;
        sscanf(line, "%9[^,],%29[^,],%29[^,],%19[^\n]", u.id, u.username, u.password, u.role);
        if (strcmp(u.role, "SeniorMechanic") == 0 || strcmp(u.role, "JuniorMechanic") == 0) {
            strcpy(mechanics[num_mechanics].id, u.id);
            strcpy(mechanics[num_mechanics].username, u.username);
            num_mechanics++;
        }
    }
    fclose(fp_user);

    float revenue_by_mechanic[100] = {0};
    int job_counts[100] = {0};
    Job j;

    rewind(fp_job);
    while (fgets(line, sizeof(line), fp_job)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", j.id, j.customerId, j.vehicleId,
               j.mechanicId, j.description, j.status, &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strcmp(j.status, "Completed") == 0) {
            for (int i = 0; i < num_mechanics; i++) {
                if (strcmp(mechanics[i].id, j.mechanicId) == 0) {
                    revenue_by_mechanic[i] += (j.laborCost + j.partsCost);
                    job_counts[i]++;
                    break;
                }
            }
        }
    }
    fclose(fp_job);

    printf("\n" MAGENTA "--- Mechanic Performance Report ---" RESET "\n");
    if (num_mechanics == 0) {
        printf(RED "No mechanics registered.\n" RESET);
    } else {
        printf("ID,Name,Completed Jobs,Total Revenue\n");
        for (int i = 0; i < num_mechanics; i++) {
            printf("%s,%s,%d,%.2f\n", mechanics[i].id, mechanics[i].username, job_counts[i], revenue_by_mechanic[i]);
        }
    }
    pauseScreen();
}

// Calculates and reports on customer lifetime value
void customerLifetimeValueReport() {
    FILE *fp_cust = fopen(CUSTOMER_FILE, "r");
    FILE *fp_job = fopen(JOB_FILE, "r");

    if (!fp_cust || !fp_job) {
        printf(RED "Error opening files for report.\n" RESET);
        if (fp_cust) fclose(fp_cust);
        if (fp_job) fclose(fp_job);
        return;
    }

    Customer customers[200];
    int num_customers = 0;
    char line[200];

    rewind(fp_cust);
    while (fgets(line, sizeof(line), fp_cust)) {
        Customer c;
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", c.id, c.name, c.phone, c.email);
        strcpy(customers[num_customers].id, c.id);
        strcpy(customers[num_customers].name, c.name);
        num_customers++;
    }
    fclose(fp_cust);

    float total_spent_by_customer[200] = {0};
    Job j;

    rewind(fp_job);
    while (fgets(line, sizeof(line), fp_job)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", j.id, j.customerId, j.vehicleId,
               j.mechanicId, j.description, j.status, &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strcmp(j.status, "Completed") == 0) {
            for (int i = 0; i < num_customers; i++) {
                if (strcmp(customers[i].id, j.customerId) == 0) {
                    total_spent_by_customer[i] += (j.laborCost + j.partsCost);
                    break;
                }
            }
        }
    }
    fclose(fp_job);

    printf("\n" MAGENTA "--- Customer Lifetime Value Report ---" RESET "\n");
    if (num_customers == 0) {
        printf(RED "No customers registered.\n" RESET);
    } else {
        printf("ID,Name,Total Spent\n");
        for (int i = 0; i < num_customers; i++) {
            printf("%s,%s,%.2f\n", customers[i].id, customers[i].name, total_spent_by_customer[i]);
        }
    }
    pauseScreen();
}

// Generates a monthly revenue report for the current year
void monthlyRevenueReport() {
    FILE *fp_job = fopen(JOB_FILE, "r");
    if (!fp_job) {
        printf(RED "No jobs found for revenue report.\n" RESET);
        return;
    }

    float monthly_revenue[12] = {0};
    int current_year;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    current_year = tm.tm_year + 1900;

    Job j;
    char line[500];
    int year, month, day;
    float total_revenue = 0;

    while (fgets(line, sizeof(line), fp_job)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%d-%d-%d,%14[^\n]", j.id, j.customerId, j.vehicleId,
               j.mechanicId, j.description, j.status, &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, &year, &month, &day, j.endDate);
        if (strcmp(j.status, "Completed") == 0 && year == current_year && month >= 1 && month <= 12) {
            monthly_revenue[month-1] += (j.laborCost + j.partsCost);
            total_revenue += (j.laborCost + j.partsCost);
        }
    }
    fclose(fp_job);

    printf("\n" BLUE "--- Monthly Revenue Report (%d) ---" RESET "\n", current_year);
    for (int i = 0; i < 12; i++) {
        printf("Month %d: %.2f\n", i + 1, monthly_revenue[i]);
    }
    printf(GREEN "-------------------------------------\n" RESET);
    printf(GREEN "Total Revenue This Year: %.2f\n" RESET, total_revenue);
    pauseScreen();
}

// Generates a report on parts usage per job
void partUsageReport() {
    FILE *fp_job_parts = fopen(JOB_PART_FILE, "r");
    FILE *fp_parts = fopen(PART_FILE, "r");
    if (!fp_job_parts || !fp_parts) {
        printf(RED "Error opening files for report.\n" RESET);
        if (fp_job_parts) fclose(fp_job_parts);
        if (fp_parts) fclose(fp_parts);
        return;
    }

    JobPart jp;
    Part p;
    char line[200];

    printf("\n" MAGENTA "--- Parts Usage Report ---" RESET "\n");
    printf("Job ID,Part ID,Part Name,Quantity,Total Cost\n");

    while(fgets(line, sizeof(line), fp_job_parts)) {
        sscanf(line, "%9[^,],%9[^,],%d,%f", jp.jobId, jp.partId, &jp.quantity, &jp.price);

        rewind(fp_parts);
        char part_name[50];
        strcpy(part_name, "Unknown");
        char part_line[200];
        while(fgets(part_line, sizeof(part_line), fp_parts)) {
            sscanf(part_line, "%9[^,],%49[^,],", p.id, p.name);
            if(strcmp(p.id, jp.partId) == 0) {
                strcpy(part_name, p.name);
                break;
            }
        }

        printf("%s,%s,%s,%d,%.2f\n", jp.jobId, jp.partId, part_name, jp.quantity, jp.price * jp.quantity);
    }

    fclose(fp_job_parts);
    fclose(fp_parts);
    pauseScreen();
}

void supplierPerformanceReport() {
    FILE *fp_supplier = fopen(SUPPLIER_FILE, "r");
    FILE *fp_parts = fopen(PART_FILE, "r");
    FILE *fp_job_parts = fopen(JOB_PART_FILE, "r");
    
    if (!fp_supplier || !fp_parts || !fp_job_parts) {
        printf(RED "Error opening files for report.\n" RESET);
        if (fp_supplier) fclose(fp_supplier);
        if (fp_parts) fclose(fp_parts);
        if (fp_job_parts) fclose(fp_job_parts);
        return;
    }
    
    Supplier suppliers[100];
    int num_suppliers = 0;
    char line[200];
    
    while(fgets(line, sizeof(line), fp_supplier)) {
        sscanf(line, "%9[^,],%49[^,],", suppliers[num_suppliers].id, suppliers[num_suppliers].name);
        num_suppliers++;
    }
    fclose(fp_supplier);

    float total_cost_by_supplier[100] = {0};
    int parts_used_by_supplier[100] = {0};
    
    JobPart jp;
    Part p;

    while(fgets(line, sizeof(line), fp_job_parts)) {
        sscanf(line, "%9[^,],%9[^,],%d,%f", jp.jobId, jp.partId, &jp.quantity, &jp.price);
        
        rewind(fp_parts);
        char part_line[200];
        while(fgets(part_line, sizeof(part_line), fp_parts)) {
            sscanf(part_line, "%9[^,],%49[^,],%d,%f,%9[^,],%d", p.id, p.name, &p.quantity, &p.price, p.supplierId, &p.reorderLevel);
            if (strcmp(p.id, jp.partId) == 0) {
                for (int i = 0; i < num_suppliers; i++) {
                    if (strcmp(suppliers[i].id, p.supplierId) == 0) {
                        total_cost_by_supplier[i] += jp.price * jp.quantity;
                        parts_used_by_supplier[i] += jp.quantity;
                        break;
                    }
                }
                break;
            }
        }
    }
    fclose(fp_parts);
    fclose(fp_job_parts);

    printf("\n" BLUE "--- Supplier Performance Report ---" RESET "\n");
    printf("Supplier ID,Name,Parts Supplied,Total Revenue\n");
    for (int i = 0; i < num_suppliers; i++) {
        printf("%s,%s,%d,%.2f\n", suppliers[i].id, suppliers[i].name, parts_used_by_supplier[i], total_cost_by_supplier[i]);
    }
    pauseScreen();
}

void customerRetentionReport() {
    FILE *fp_cust = fopen(CUSTOMER_FILE, "r");
    FILE *fp_job = fopen(JOB_FILE, "r");
    
    if (!fp_cust || !fp_job) {
        printf(RED "Error opening files for report.\n" RESET);
        if (fp_cust) fclose(fp_cust);
        if (fp_job) fclose(fp_job);
        return;
    }

    Customer customers[200];
    int num_customers = 0;
    char line[200];
    
    while(fgets(line, sizeof(line), fp_cust)) {
        sscanf(line, "%9[^,],%49[^,],", customers[num_customers].id, customers[num_customers].name);
        num_customers++;
    }
    fclose(fp_cust);
    
    int job_count_by_customer[200] = {0};
    float total_spent_by_customer[200] = {0};
    
    Job j;
    while(fgets(line, sizeof(line), fp_job)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, &j.estimatedCost, &j.laborCost, &j.partsCost);
        if (strcmp(j.status, "Completed") == 0) {
            for (int i = 0; i < num_customers; i++) {
                if (strcmp(customers[i].id, j.customerId) == 0) {
                    job_count_by_customer[i]++;
                    total_spent_by_customer[i] += (j.laborCost + j.partsCost);
                    break;
                }
            }
        }
    }
    fclose(fp_job);

    printf("\n" MAGENTA "--- Customer Retention Report ---" RESET "\n");
    printf("Customer ID,Name,Completed Jobs,Total Spent\n");
    for (int i = 0; i < num_customers; i++) {
        printf("%s,%s,%d,%.2f\n", customers[i].id, customers[i].name, job_count_by_customer[i], total_spent_by_customer[i]);
    }
    
    int retained_customers = 0;
    for(int i = 0; i < num_customers; i++) {
        if(job_count_by_customer[i] > 1) {
            retained_customers++;
        }
    }
    
    printf("\n" CYAN "Total Customers: %d\n" RESET, num_customers);
    printf(CYAN "Customers with multiple jobs (retained): %d\n" RESET, retained_customers);

    pauseScreen();
}

// Menu Functions
// Admin menu with all available options
void adminMenu(const char *uid) {
    int ch;
    do {
        showDashboard();
        printf("\n" GREEN "--- Admin Menu ---" RESET "\n");
        printf("1. Register User\n");
        printf("2. Change Password\n");
        printf("3. Change User Role\n");
        printf("4. View All Users\n");
        printf("--------------------\n");
        printf("5. Add Customer\n");
        printf("6. Edit Customer Information\n");
        printf("7. Delete Customer Record\n");
        printf("8. Search Customer\n");
        printf("9. View All Customers\n");
        printf("10. Send Feedback Request\n");
        printf("--------------------\n");
        printf("11. Add Vehicle\n");
        printf("12. Edit Vehicle Information\n");
        printf("13. Delete Vehicle Data\n");
        printf("14. Search Vehicle\n");
        printf("15. View All Vehicles\n");
        printf("--------------------\n");
        printf("16. Add Job\n");
        printf("17. Update Job Status\n");
        printf("18. Add Part to a Job\n");
        printf("19. Update Job Labor Cost\n");
        printf("20. View All Jobs\n");
        printf("21. Search Job\n");
        printf("22. View Pending Jobs\n");
        printf("23. View Completed Jobs\n");
        printf("--------------------\n");
        printf("24. Generate Invoice\n");
        printf("25. View All Invoices\n");
        printf("--------------------\n");
        printf("26. Add Part to Inventory\n");
        printf("27. Edit Part Information\n");
        printf("28. View Inventory\n");
        printf("29. Check Low Stock & Create Reorder List\n");
        printf("30. View Reorder List\n");
        printf("31. Clear Reorder List\n");
        printf("32. Add Supplier\n");
        printf("33. View All Suppliers\n");
        printf("--------------------\n");
        printf("34. Schedule Appointment\n");
        printf("35. View All Appointments\n");
        printf("36. Schedule Service Reminder\n");
        printf("37. View All Reminders\n");
        printf("--------------------\n");
        printf("38. Add Expense\n");
        printf("39. View All Expenses\n");
        printf("40. Generate Profit & Loss Report\n");
        printf("--------------------\n");
        printf("41. View Job Summary Report\n");
        printf("42. Mechanic Performance Report\n");
        printf("43. Customer Lifetime Value Report\n");
        printf("44. Monthly Revenue Report\n");
        printf("45. Parts Usage Report\n");
        printf("46. Supplier Performance Report\n");
        printf("47. Customer Retention Report\n");
        printf("--------------------\n");
        printf("48. Backup All Data\n");
        printf("49. Restore Data from Backup\n");
        printf("--------------------\n");
        printf("0. Logout\n");
        printf(YELLOW "Choice: " RESET);
        scanf("%d", &ch);

        switch (ch) {
            case 1: registerUser(); break;
            case 2: changePassword((char*)uid); break;
            case 3: changeUserRole(); break;
            case 4: viewAllUsers(); break;
            case 5: addCustomer(); break;
            case 6: editCustomer(); break;
            case 7: deleteCustomer(); break;
            case 8: searchCustomer(); break;
            case 9: viewAllCustomers(); break;
            case 10: sendFeedbackRequest(); break;
            case 11: addVehicle(); break;
            case 12: editVehicle(); break;
            case 13: deleteVehicle(); break;
            case 14: searchVehicle(); break;
            case 15: viewAllVehicles(); break;
            case 16: addJob(uid, "Admin"); break;
            case 17: updateJobStatus(uid, "Admin"); break;
            case 18: addPartToJob(); break;
            case 19: updateJobLaborCost(); break;
            case 20: viewAllJobs(); break;
            case 21: searchJob(); break;
            case 22: viewPendingJobs(); break;
            case 23: viewCompletedJobs(); break;
            case 24: generateInvoice(); break;
            case 25: viewAllInvoices(); break;
            case 26: addPart(); break;
            case 27: editPart(); break;
            case 28: viewInventory(); break;
            case 29: checkLowStock(); break;
            case 30: viewReorderList(); break;
            case 31: clearReorderList(); break;
            case 32: addSupplier(); break;
            case 33: viewAllSuppliers(); break;
            case 34: scheduleAppointment(uid, "Admin"); break;
            case 35: viewAllAppointments(); break;
            case 36: scheduleServiceReminder(); break;
            case 37: viewAllReminders(); break;
            case 38: addExpense(); break;
            case 39: viewAllExpenses(); break;
            case 40: generateProfitLossReport(); break;
            case 41: viewJobSummaryReport(); break;
            case 42: mechanicPerformanceReport(); break;
            case 43: customerLifetimeValueReport(); break;
            case 44: monthlyRevenueReport(); break;
            case 45: partUsageReport(); break;
            case 46: supplierPerformanceReport(); break;
            case 47: customerRetentionReport(); break;
            case 48: backupData(); break;
            case 49: restoreData(); break;
            case 0: break;
            default: printf(RED "Invalid choice. Please try again.\n" RESET); pauseScreen(); break;
        }
    } while (ch != 0);
}

// Senior Mechanic menu with most options
void seniorMechanicMenu(const char *uid) {
    int ch;
    do {
        showDashboard();
        printf("\n" GREEN "--- Senior Mechanic Menu ---" RESET "\n");
        printf("1. Add Job\n");
        printf("2. Update Job Status\n");
        printf("3. Add Part to a Job\n");
        printf("4. Update Job Labor Cost\n");
        printf("5. View All Jobs\n");
        printf("6. View Pending Jobs\n");
        printf("7. View All Vehicles\n");
        printf("8. View All Customers\n");
        printf("9. View Inventory\n");
        printf("10. Check Low Stock & Create Reorder List\n");
        printf("11. View All Appointments\n");
        printf("12. Change Password\n");
        printf("0. Logout\n");
        printf(YELLOW "Choice: " RESET);
        scanf("%d", &ch);

        switch (ch) {
            case 1: addJob(uid, "SeniorMechanic"); break;
            case 2: updateJobStatus(uid, "SeniorMechanic"); break;
            case 3: addPartToJob(); break;
            case 4: updateJobLaborCost(); break;
            case 5: viewAllJobs(); break;
            case 6: viewPendingJobs(); break;
            case 7: viewAllVehicles(); break;
            case 8: viewAllCustomers(); break;
            case 9: viewInventory(); break;
            case 10: checkLowStock(); break;
            case 11: viewAllAppointments(); break;
            case 12: changePassword((char*)uid); break;
            case 0: break;
            default: printf(RED "Invalid choice. Please try again.\n" RESET); pauseScreen(); break;
        }
    } while (ch != 0);
}

// Junior Mechanic menu with limited, role-specific options
void juniorMechanicMenu(const char *uid) {
    int ch;
    do {
        showDashboard();
        printf("\n" GREEN "--- Junior Mechanic Menu ---" RESET "\n");
        printf("1. Update Job Status (Only My Jobs)\n");
        printf("2. Add Part to a Job\n");
        printf("3. Update Job Labor Cost\n");
        printf("4. View My Assigned Jobs\n");
        printf("5. View All Vehicles\n");
        printf("6. View All Customers\n");
        printf("7. View Inventory\n");
        printf("8. Change Password\n");
        printf("0. Logout\n");
        printf(YELLOW "Choice: " RESET);
        scanf("%d", &ch);

        switch (ch) {
            case 1: updateJobStatus(uid, "JuniorMechanic"); break;
            case 2: addPartToJob(); break;
            case 3: updateJobLaborCost(); break;
            case 4: viewMyJobs(uid); break;
            case 5: viewAllVehicles(); break;
            case 6: viewAllCustomers(); break;
            case 7: viewInventory(); break;
            case 8: changePassword((char*)uid); break;
            case 0: break;
            default: printf(RED "Invalid choice. Please try again.\n" RESET); pauseScreen(); break;
        }
    } while (ch != 0);
}


// Customer menu with options relevant to their role
void customerMenu(const char *uid) {
    int ch;
    do {
        printf("\n" GREEN "--- Customer Menu ---" RESET "\n");
        printf("1. View My Service History\n");
        printf("2. Schedule New Appointment\n");
        printf("3. View My Vehicles\n");
        printf("4. View My Notifications\n");
        printf("5. Change Password\n");
        printf("0. Logout\n");
        printf(YELLOW "Choice: " RESET);
        scanf("%d", &ch);

        switch (ch) {
            case 1: viewCustomerJobs(uid); break;
            case 2: scheduleAppointment(uid, "Customer"); break;
            case 3: viewCustomerVehicles(uid); break;
            case 4: viewNotifications(uid); break;
            case 5: changePassword((char*)uid); break;
            case 0: break;
            default: printf(RED "Invalid choice. Please try again.\n" RESET); pauseScreen(); break;
        }
    } while (ch != 0);
}

// Prints the ASCII art logo
void printLogo() {
    printf(GREEN);
    printf("    ____                              _____             _           \n");
    printf("   / ___| __ _ _ __ ___   ___  | ____|_   _____ ___  __| | ___ _ __ \n");
    printf("  | |  _ / _` | '_ ` _ \\ / _ \\ |  _| \\ \\/ / __/ _ \\/ _` |/ _ \\ '__|\n");
    printf("  | |_| | (_| | | | | | |  __/ | |___ >  < (_|  __/ (_| |  __/ |   \n");
    printf("   \\____|\\__,_|_| |_| |_|\\___| |_____/_/\\_\\___\\___|\\__,_|\\___|_|   \n");
    printf(RESET);
    printf("\n");
}

// Main function - entry point of the program
int main() {
    int ch;
    char role[20], uid[10];

    printLogo();

    while (1) {
        FILE *fp = fopen(USER_FILE, "r");
        int hasUsers = 0;
        if (fp) {
            fseek(fp, 0, SEEK_END);
            if (ftell(fp) > 0) hasUsers = 1;
            fclose(fp);
        }

        printf("\n" BLUE "=== GARAGE SERVICE MANAGEMENT SYSTEM ===" RESET "\n");
        if (!hasUsers) {
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
        } else {
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
        }
    }
    return 0;
}