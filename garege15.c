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
void backupData() {
    printf(CYAN "Creating data backup...\n" RESET);

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
    #ifdef _WIN32
    sprintf(cmd, "copy %s %s\\", USER_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "copy %s %s\\", CUSTOMER_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "copy %s %s\\", VEHICLE_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "copy %s %s\\", JOB_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "copy %s %s\\", INVOICE_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "copy %s %s\\", PART_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "copy %s %s\\", SUPPLIER_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "copy %s %s\\", APPOINTMENT_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "copy %s %s\\", JOB_PART_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "copy %s %s\\", FINANCE_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "copy %s %s\\", EXPENSES_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "copy %s %s\\", REMINDERS_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "copy %s %s\\", REORDER_LIST_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "copy %s %s\\", NOTIFICATION_FILE, BACKUP_DIR);
    system(cmd);
    #else
    sprintf(cmd, "cp %s %s/", USER_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "cp %s %s/", CUSTOMER_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "cp %s %s/", VEHICLE_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "cp %s %s/", JOB_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "cp %s %s/", INVOICE_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "cp %s %s/", PART_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "cp %s %s/", SUPPLIER_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "cp %s %s/", APPOINTMENT_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "cp %s %s/", JOB_PART_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "cp %s %s/", FINANCE_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "cp %s %s/", EXPENSES_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "cp %s %s/", REMINDERS_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "cp %s %s/", REORDER_LIST_FILE, BACKUP_DIR);
    system(cmd);
    sprintf(cmd, "cp %s %s/", NOTIFICATION_FILE, BACKUP_DIR);
    system(cmd);
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
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, USER_FILE, USER_FILE);
    system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, CUSTOMER_FILE, CUSTOMER_FILE);
    system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, VEHICLE_FILE, VEHICLE_FILE);
    system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, JOB_FILE, JOB_FILE);
    system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, INVOICE_FILE, INVOICE_FILE);
    system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, PART_FILE, PART_FILE);
    system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, SUPPLIER_FILE, SUPPLIER_FILE);
    system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, APPOINTMENT_FILE, APPOINTMENT_FILE);
    system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, JOB_PART_FILE, JOB_PART_FILE);
    system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, FINANCE_FILE, FINANCE_FILE);
    system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, EXPENSES_FILE, EXPENSES_FILE);
    system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, REMINDERS_FILE, REMINDERS_FILE);
    system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, REORDER_LIST_FILE, REORDER_LIST_FILE);
    system(cmd);
    sprintf(cmd, "copy %s\\%s %s", BACKUP_DIR, NOTIFICATION_FILE, NOTIFICATION_FILE);
    system(cmd);
    #else
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, USER_FILE, USER_FILE);
    system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, CUSTOMER_FILE, CUSTOMER_FILE);
    system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, VEHICLE_FILE, VEHICLE_FILE);
    system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, JOB_FILE, JOB_FILE);
    system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, INVOICE_FILE, INVOICE_FILE);
    system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, PART_FILE, PART_FILE);
    system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, SUPPLIER_FILE, SUPPLIER_FILE);
    system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, APPOINTMENT_FILE, APPOINTMENT_FILE);
    system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, JOB_PART_FILE, JOB_PART_FILE);
    system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, FINANCE_FILE, FINANCE_FILE);
    system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, EXPENSES_FILE, EXPENSES_FILE);
    system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, REMINDERS_FILE, REMINDERS_FILE);
    system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, REORDER_LIST_FILE, REORDER_LIST_FILE);
    system(cmd);
    sprintf(cmd, "cp %s/%s %s", BACKUP_DIR, NOTIFICATION_FILE, NOTIFICATION_FILE);
    system(cmd);
    #endif

    printf(GREEN "Data restore completed successfully from '%s'.\n" RESET, BACKUP_DIR);
    writeLog("Data restore completed.");
    pauseScreen();
}

// Function to get the service interval for a vehicle based on its type or age
int getNextServiceInterval(const char *vehicleId) {
    // This is a placeholder. A real implementation would look up vehicle
    // type/make/model from a file or database to determine the interval.
    return 6; // Returns 6 months as a default
}

// User Management Functions

// Handles user login
int login(char *role, char *uid) {
    char username[30];
    char password[30];
    User u;
    int attempts = 0;

    while (attempts < 3) {
        printf(YELLOW "\nEnter username: " RESET);
        scanf("%s", username);
        printf(YELLOW "Enter password: " RESET);
        scanf("%s", password);
        clearInputBuffer();

        FILE *fp = fopen(USER_FILE, "r");
        if (!fp) {
            printf(RED "Error: User database not found.\n" RESET);
            return 0;
        }

        hashPassword(password);
        int found = 0;
        char line[100];
        while (fgets(line, sizeof(line), fp)) {
            sscanf(line, "%9[^,],%29[^,],%29[^,],%19[^\n]", u.id, u.username, u.password, u.role);
            if (strcmp(u.username, username) == 0 && strcmp(u.password, password) == 0) {
                strcpy(role, u.role);
                strcpy(uid, u.id);
                printf(GREEN "Login successful! Welcome, %s.\n" RESET, u.username);
                writeLog("User logged in successfully.");
                found = 1;
                break;
            }
        }
        fclose(fp);

        if (found) return 1;
        printf(RED "Invalid username or password. Please try again.\n" RESET);
        attempts++;
    }

    return 0;
}

// Function to register a new user
void registerUser() {
    User u;
    int choice;
    printf(GREEN "\nRegister New User Account:\n" RESET);

    // Get username
    printf(YELLOW "Enter username: " RESET);
    scanf("%s", u.username);
    clearInputBuffer();
    
    // Check if username already exists
    FILE *fp_read = fopen(USER_FILE, "r");
    if (fp_read) {
        char line[100];
        char temp_user[30];
        while(fgets(line, sizeof(line), fp_read)) {
            sscanf(line, "%*[^,],%29[^,],%*[^,],%*[^,]", temp_user);
            if(strcmp(temp_user, u.username) == 0) {
                printf(RED "Error: Username already exists. Please choose a different one.\n" RESET);
                fclose(fp_read);
                pauseScreen();
                return;
            }
        }
        fclose(fp_read);
    }
    

    // Get password
    printf(YELLOW "Enter password: " RESET);
    scanf("%s", u.password);
    clearInputBuffer();
    hashPassword(u.password);

    // Get role from a numbered list
    printf(YELLOW "Select role:\n" RESET);
    printf("1. Admin\n");
    printf("2. SeniorMechanic\n");
    printf("3. JuniorMechanic\n");
    printf("4. Customer\n");
    printf(YELLOW "Enter choice: " RESET);
    scanf("%d", &choice);
    clearInputBuffer();

    switch (choice) {
        case 1: strcpy(u.role, "Admin"); break;
        case 2: strcpy(u.role, "SeniorMechanic"); break;
        case 3: strcpy(u.role, "JuniorMechanic"); break;
        case 4: strcpy(u.role, "Customer"); break;
        default: 
            printf(RED "Invalid choice. Defaulting to Customer role.\n" RESET);
            strcpy(u.role, "Customer"); 
            break;
    }

    // Generate user ID
    int user_count = fileLineCount(USER_FILE);
    generateId("U", user_count + 1, u.id);

    // Write to file
    FILE *fp = fopen(USER_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening file for writing.\n" RESET);
        return;
    }
    fprintf(fp, "%s,%s,%s,%s\n", u.id, u.username, u.password, u.role);
    fclose(fp);

    printf(GREEN "User registered successfully! User ID: %s\n" RESET, u.id);
    writeLog("New user registered.");
    pauseScreen();
}


// Function to change a user's password
void changePassword(char *uid) {
    char new_password[30];
    User u;
    int found = 0;
    
    FILE *fp_read = fopen(USER_FILE, "r");
    if (!fp_read) {
        printf(RED "Error: User database not found.\n" RESET);
        return;
    }

    // Read all users into memory
    FILE *fp_write = fopen("temp_users.txt", "w");
    if (!fp_write) {
        fclose(fp_read);
        printf(RED "Error creating temp file.\n" RESET);
        return;
    }
    
    char line[100];
    while(fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%29[^,],%29[^,],%19[^\n]", u.id, u.username, u.password, u.role);
        if(strcmp(u.id, uid) == 0) {
            printf(YELLOW "Enter new password for user %s: " RESET, u.username);
            scanf("%s", new_password);
            clearInputBuffer();
            hashPassword(new_password);
            strcpy(u.password, new_password);
            printf(GREEN "Password changed successfully.\n" RESET);
            found = 1;
        }
        fprintf(fp_write, "%s,%s,%s,%s\n", u.id, u.username, u.password, u.role);
    }
    
    fclose(fp_read);
    fclose(fp_write);

    remove(USER_FILE);
    rename("temp_users.txt", USER_FILE);

    if (!found) {
        printf(RED "User not found.\n" RESET);
    }
    writeLog("Password changed.");
    pauseScreen();
}

// Function to view all users
void viewAllUsers() {
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) {
        printf(RED "No users found. Please register an admin first.\n" RESET);
        pauseScreen();
        return;
    }

    User u;
    printf("\n" CYAN "--- All Users ---" RESET "\n");
    printf(BLUE "ID      Username      Role\n" RESET);
    printf(BLUE "---------------------------------\n" RESET);
    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%29[^,],%29[^,],%19[^\n]", u.id, u.username, u.password, u.role);
        printf("%-7s %-15s %s\n", u.id, u.username, u.role);
    }
    fclose(fp);
    pauseScreen();
}

// Function to change a user's role
void changeUserRole() {
    char user_id[10];
    int choice;
    User u;
    int found = 0;

    printf(YELLOW "Enter User ID to change role for: " RESET);
    scanf("%s", user_id);
    clearInputBuffer();
    
    FILE *fp_read = fopen(USER_FILE, "r");
    if (!fp_read) {
        printf(RED "Error: User database not found.\n" RESET);
        return;
    }

    FILE *fp_write = fopen("temp_users.txt", "w");
    if (!fp_write) {
        fclose(fp_read);
        printf(RED "Error creating temp file.\n" RESET);
        return;
    }
    
    char line[100];
    while(fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%29[^,],%29[^,],%19[^\n]", u.id, u.username, u.password, u.role);
        if(strcmp(u.id, user_id) == 0) {
            found = 1;
            printf(YELLOW "Current role for %s: %s\n" RESET, u.username, u.role);
            printf(YELLOW "Select new role:\n" RESET);
            printf("1. Admin\n");
            printf("2. SeniorMechanic\n");
            printf("3. JuniorMechanic\n");
            printf("4. Customer\n");
            printf(YELLOW "Enter choice: " RESET);
            scanf("%d", &choice);
            clearInputBuffer();
            
            switch (choice) {
                case 1: strcpy(u.role, "Admin"); break;
                case 2: strcpy(u.role, "SeniorMechanic"); break;
                case 3: strcpy(u.role, "JuniorMechanic"); break;
                case 4: strcpy(u.role, "Customer"); break;
                default: 
                    printf(RED "Invalid choice. Role not changed.\n" RESET);
                    break;
            }
        }
        fprintf(fp_write, "%s,%s,%s,%s\n", u.id, u.username, u.password, u.role);
    }
    
    fclose(fp_read);
    fclose(fp_write);

    remove(USER_FILE);
    rename("temp_users.txt", USER_FILE);
    
    if (found) {
        printf(GREEN "User role updated successfully.\n" RESET);
    } else {
        printf(RED "User not found.\n" RESET);
    }
    writeLog("User role changed.");
    pauseScreen();
}

// Helper function to determine role hierarchy
int getRoleLevel(const char *role) {
    if (strcmp(role, "Admin") == 0) return 4;
    if (strcmp(role, "SeniorMechanic") == 0) return 3;
    if (strcmp(role, "JuniorMechanic") == 0) return 2;
    if (strcmp(role, "Customer") == 0) return 1;
    return 0; // Unknown role
}


// Customer Management Functions

// Function to add a new customer
void addCustomer() {
    Customer c;
    printf(GREEN "\nAdd New Customer:\n" RESET);
    printf(YELLOW "Enter customer name: " RESET);
    scanf(" %[^\n]", c.name);
    clearInputBuffer();
    printf(YELLOW "Enter phone number: " RESET);
    scanf(" %[^\n]", c.phone);
    clearInputBuffer();
    printf(YELLOW "Enter email: " RESET);
    scanf(" %[^\n]", c.email);
    clearInputBuffer();
    
    int customer_count = fileLineCount(CUSTOMER_FILE);
    generateId("C", customer_count + 1, c.id);
    
    FILE *fp = fopen(CUSTOMER_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening file.\n" RESET);
        return;
    }
    fprintf(fp, "%s,%s,%s,%s\n", c.id, c.name, c.phone, c.email);
    fclose(fp);
    
    printf(GREEN "Customer added successfully! Customer ID: %s\n" RESET, c.id);
    writeLog("New customer added.");
    pauseScreen();
}

// Function to check if a customer exists
int customerExists(const char *id) {
    FILE *fp = fopen(CUSTOMER_FILE, "r");
    if (!fp) return 0;
    
    Customer c;
    char line[100];
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", c.id, c.name, c.phone, c.email);
        if(strcmp(c.id, id) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// Function to edit an existing customer
void editCustomer() {
    char customer_id[10];
    Customer c;
    int found = 0;
    
    printf(YELLOW "Enter Customer ID to edit: " RESET);
    scanf("%s", customer_id);
    clearInputBuffer();

    FILE *fp_read = fopen(CUSTOMER_FILE, "r");
    if (!fp_read) {
        printf(RED "Error: Customer database not found.\n" RESET);
        return;
    }

    FILE *fp_write = fopen("temp_customers.txt", "w");
    if (!fp_write) {
        fclose(fp_read);
        printf(RED "Error creating temp file.\n" RESET);
        return;
    }
    
    char line[100];
    while(fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", c.id, c.name, c.phone, c.email);
        if(strcmp(c.id, customer_id) == 0) {
            found = 1;
            printf(YELLOW "Editing customer %s...\n" RESET, c.id);
            printf(YELLOW "Enter new name (current: %s): " RESET, c.name);
            scanf(" %[^\n]", c.name);
            clearInputBuffer();
            printf(YELLOW "Enter new phone (current: %s): " RESET, c.phone);
            scanf(" %[^\n]", c.phone);
            clearInputBuffer();
            printf(YELLOW "Enter new email (current: %s): " RESET, c.email);
            scanf(" %[^\n]", c.email);
            clearInputBuffer();
            printf(GREEN "Customer updated successfully.\n" RESET);
        }
        fprintf(fp_write, "%s,%s,%s,%s\n", c.id, c.name, c.phone, c.email);
    }
    
    fclose(fp_read);
    fclose(fp_write);

    remove(CUSTOMER_FILE);
    rename("temp_customers.txt", CUSTOMER_FILE);
    
    if (!found) {
        printf(RED "Customer not found.\n" RESET);
    }
    writeLog("Customer details updated.");
    pauseScreen();
}

// Function to delete an existing customer
void deleteCustomer() {
    char customer_id[10];
    Customer c;
    int found = 0;
    
    printf(YELLOW "Enter Customer ID to delete: " RESET);
    scanf("%s", customer_id);
    clearInputBuffer();

    FILE *fp_read = fopen(CUSTOMER_FILE, "r");
    if (!fp_read) {
        printf(RED "Error: Customer database not found.\n" RESET);
        return;
    }

    FILE *fp_write = fopen("temp_customers.txt", "w");
    if (!fp_write) {
        fclose(fp_read);
        printf(RED "Error creating temp file.\n" RESET);
        return;
    }
    
    char line[100];
    while(fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", c.id, c.name, c.phone, c.email);
        if(strcmp(c.id, customer_id) == 0) {
            found = 1;
            printf(GREEN "Customer %s deleted.\n" RESET, c.name);
        } else {
            fprintf(fp_write, "%s,%s,%s,%s\n", c.id, c.name, c.phone, c.email);
        }
    }
    
    fclose(fp_read);
    fclose(fp_write);

    remove(CUSTOMER_FILE);
    rename("temp_customers.txt", CUSTOMER_FILE);
    
    if (!found) {
        printf(RED "Customer not found.\n" RESET);
    }
    writeLog("Customer deleted.");
    pauseScreen();
}

// Function to search for a customer by ID or name
void searchCustomer() {
    char query[50];
    Customer c;
    int found = 0;
    
    printf(YELLOW "Enter Customer ID or name to search: " RESET);
    scanf(" %[^\n]", query);
    clearInputBuffer();
    
    FILE *fp = fopen(CUSTOMER_FILE, "r");
    if (!fp) {
        printf(RED "Error: Customer database not found.\n" RESET);
        pauseScreen();
        return;
    }
    
    printf("\n" CYAN "--- Search Results ---" RESET "\n");
    printf(BLUE "ID      Name                Phone           Email\n" RESET);
    printf(BLUE "-----------------------------------------------------\n" RESET);
    
    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", c.id, c.name, c.phone, c.email);
        if (strstr(c.id, query) || strstr(c.name, query)) {
            printf("%-7s %-19s %-15s %s\n", c.id, c.name, c.phone, c.email);
            found = 1;
        }
    }
    fclose(fp);
    
    if (!found) {
        printf(RED "No customers found matching the query.\n" RESET);
    }
    
    pauseScreen();
}

// Function to view all customers
void viewAllCustomers() {
    FILE *fp = fopen(CUSTOMER_FILE, "r");
    if (!fp) {
        printf(RED "No customers found.\n" RESET);
        pauseScreen();
        return;
    }
    
    Customer c;
    printf("\n" CYAN "--- All Customers ---" RESET "\n");
    printf(BLUE "ID      Name                Phone           Email\n" RESET);
    printf(BLUE "-----------------------------------------------------\n" RESET);
    
    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", c.id, c.name, c.phone, c.email);
        printf("%-7s %-19s %-15s %s\n", c.id, c.name, c.phone, c.email);
    }
    fclose(fp);
    
    pauseScreen();
}

// Function to send a feedback request to a customer (placeholder)
void sendFeedbackRequest() {
    char customer_id[10];
    printf(YELLOW "Enter Customer ID to send feedback request to: " RESET);
    scanf("%s", customer_id);
    clearInputBuffer();

    if (customerExists(customer_id)) {
        printf(GREEN "Feedback request sent to customer %s.\n" RESET, customer_id);
        createNotification(customer_id, "Please provide feedback on your recent service. Your opinion matters!");
    } else {
        printf(RED "Customer not found.\n" RESET);
    }
    pauseScreen();
}

// Vehicle Management Functions

// Function to add a new vehicle
void addVehicle() {
    Vehicle v;
    printf(GREEN "\nAdd New Vehicle:\n" RESET);
    printf(YELLOW "Enter customer ID for the vehicle: " RESET);
    scanf("%s", v.customerId);
    clearInputBuffer();
    
    if (!customerExists(v.customerId)) {
        printf(RED "Error: Customer ID not found.\n" RESET);
        pauseScreen();
        return;
    }
    
    printf(YELLOW "Enter brand: " RESET);
    scanf(" %[^\n]", v.brand);
    clearInputBuffer();
    printf(YELLOW "Enter model: " RESET);
    scanf(" %[^\n]", v.model);
    clearInputBuffer();
    printf(YELLOW "Enter license plate: " RESET);
    scanf(" %[^\n]", v.plate);
    clearInputBuffer();
    printf(YELLOW "Enter year: " RESET);
    scanf(" %[^\n]", v.year);
    clearInputBuffer();
    
    // Set last service date to N/A for a new vehicle
    strcpy(v.lastServiceDate, "N/A");
    
    int vehicle_count = fileLineCount(VEHICLE_FILE);
    generateId("V", vehicle_count + 1, v.id);
    
    FILE *fp = fopen(VEHICLE_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening file.\n" RESET);
        return;
    }
    fprintf(fp, "%s,%s,%s,%s,%s,%s,%s\n", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
    fclose(fp);
    
    printf(GREEN "Vehicle added successfully! Vehicle ID: %s\n" RESET, v.id);
    writeLog("New vehicle added.");
    pauseScreen();
}

// Function to check if a vehicle exists
int vehicleExists(const char *id) {
    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) return 0;
    
    Vehicle v;
    char line[100];
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]", 
               v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
        if(strcmp(v.id, id) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// Function to edit an existing vehicle
void editVehicle() {
    char vehicle_id[10];
    Vehicle v;
    int found = 0;
    
    printf(YELLOW "Enter Vehicle ID to edit: " RESET);
    scanf("%s", vehicle_id);
    clearInputBuffer();

    FILE *fp_read = fopen(VEHICLE_FILE, "r");
    if (!fp_read) {
        printf(RED "Error: Vehicle database not found.\n" RESET);
        return;
    }

    FILE *fp_write = fopen("temp_vehicles.txt", "w");
    if (!fp_write) {
        fclose(fp_read);
        printf(RED "Error creating temp file.\n" RESET);
        return;
    }
    
    char line[100];
    while(fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]", 
               v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
        if(strcmp(v.id, vehicle_id) == 0) {
            found = 1;
            printf(YELLOW "Editing vehicle %s...\n" RESET, v.id);
            printf(YELLOW "Enter new brand (current: %s): " RESET, v.brand);
            scanf(" %[^\n]", v.brand);
            clearInputBuffer();
            printf(YELLOW "Enter new model (current: %s): " RESET, v.model);
            scanf(" %[^\n]", v.model);
            clearInputBuffer();
            printf(YELLOW "Enter new license plate (current: %s): " RESET, v.plate);
            scanf(" %[^\n]", v.plate);
            clearInputBuffer();
            printf(YELLOW "Enter new year (current: %s): " RESET, v.year);
            scanf(" %[^\n]", v.year);
            clearInputBuffer();
            printf(GREEN "Vehicle updated successfully.\n" RESET);
        }
        fprintf(fp_write, "%s,%s,%s,%s,%s,%s,%s\n", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
    }
    
    fclose(fp_read);
    fclose(fp_write);

    remove(VEHICLE_FILE);
    rename("temp_vehicles.txt", VEHICLE_FILE);
    
    if (!found) {
        printf(RED "Vehicle not found.\n" RESET);
    }
    writeLog("Vehicle details updated.");
    pauseScreen();
}

// Function to delete an existing vehicle
void deleteVehicle() {
    char vehicle_id[10];
    Vehicle v;
    int found = 0;
    
    printf(YELLOW "Enter Vehicle ID to delete: " RESET);
    scanf("%s", vehicle_id);
    clearInputBuffer();

    FILE *fp_read = fopen(VEHICLE_FILE, "r");
    if (!fp_read) {
        printf(RED "Error: Vehicle database not found.\n" RESET);
        return;
    }

    FILE *fp_write = fopen("temp_vehicles.txt", "w");
    if (!fp_write) {
        fclose(fp_read);
        printf(RED "Error creating temp file.\n" RESET);
        return;
    }
    
    char line[100];
    while(fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]", 
               v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
        if(strcmp(v.id, vehicle_id) == 0) {
            found = 1;
            printf(GREEN "Vehicle %s deleted.\n" RESET, v.id);
        } else {
            fprintf(fp_write, "%s,%s,%s,%s,%s,%s,%s\n", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
        }
    }
    
    fclose(fp_read);
    fclose(fp_write);

    remove(VEHICLE_FILE);
    rename("temp_vehicles.txt", VEHICLE_FILE);
    
    if (!found) {
        printf(RED "Vehicle not found.\n" RESET);
    }
    writeLog("Vehicle deleted.");
    pauseScreen();
}

// Function to search for a vehicle by ID, customer ID, or plate number
void searchVehicle() {
    char query[50];
    Vehicle v;
    int found = 0;
    
    printf(YELLOW "Enter Vehicle ID, Customer ID, or plate number to search: " RESET);
    scanf(" %[^\n]", query);
    clearInputBuffer();
    
    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) {
        printf(RED "Error: Vehicle database not found.\n" RESET);
        pauseScreen();
        return;
    }
    
    printf("\n" CYAN "--- Search Results ---" RESET "\n");
    printf(BLUE "ID      CustID  Brand   Model   Plate           Year\n" RESET);
    printf(BLUE "-----------------------------------------------------\n" RESET);
    
    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]", 
               v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
        if (strstr(v.id, query) || strstr(v.customerId, query) || strstr(v.plate, query)) {
            printf("%-7s %-7s %-7s %-7s %-15s %s\n", v.id, v.customerId, v.brand, v.model, v.plate, v.year);
            found = 1;
        }
    }
    fclose(fp);
    
    if (!found) {
        printf(RED "No vehicles found matching the query.\n" RESET);
    }
    
    pauseScreen();
}

// Function to view all vehicles
void viewAllVehicles() {
    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) {
        printf(RED "No vehicles found.\n" RESET);
        pauseScreen();
        return;
    }
    
    Vehicle v;
    printf("\n" CYAN "--- All Vehicles ---" RESET "\n");
    printf(BLUE "ID      CustID  Brand   Model   Plate           Year\n" RESET);
    printf(BLUE "-----------------------------------------------------\n" RESET);
    
    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]", 
               v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
        printf("%-7s %-7s %-7s %-7s %-15s %s\n", v.id, v.customerId, v.brand, v.model, v.plate, v.year);
    }
    fclose(fp);
    
    pauseScreen();
}

// View vehicles for a specific customer
void viewCustomerVehicles(const char* customerId) {
    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) {
        printf(RED "No vehicles found for this customer.\n" RESET);
        pauseScreen();
        return;
    }

    Vehicle v;
    int found = 0;
    printf("\n" CYAN "--- Your Vehicles ---" RESET "\n");
    printf(BLUE "ID      Brand   Model   Plate           Year\n" RESET);
    printf(BLUE "-----------------------------------------------------\n" RESET);

    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]",
               v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
        if (strcmp(v.customerId, customerId) == 0) {
            printf("%-7s %-7s %-7s %-15s %s\n", v.id, v.brand, v.model, v.plate, v.year);
            found = 1;
        }
    }
    fclose(fp);

    if (!found) {
        printf(RED "No vehicles found for this customer.\n" RESET);
    }
    
    pauseScreen();
}

// Job Management Functions

// Checks if a mechanic exists
int mechanicExists(const char *id) {
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) return 0;
    
    User u;
    char line[100];
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%29[^,],%29[^,],%19[^\n]", u.id, u.username, u.password, u.role);
        if(strcmp(u.id, id) == 0 && (strcmp(u.role, "SeniorMechanic") == 0 || strcmp(u.role, "JuniorMechanic") == 0)) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// Function to add a new job
void addJob(const char *currentUserId, const char *currentUserRole) {
    Job j;
    
    printf(GREEN "\nAdd New Job:\n" RESET);
    printf(YELLOW "Enter customer ID: " RESET);
    scanf("%s", j.customerId);
    clearInputBuffer();
    
    if (!customerExists(j.customerId)) {
        printf(RED "Error: Customer ID not found.\n" RESET);
        pauseScreen();
        return;
    }
    
    printf(YELLOW "Enter vehicle ID: " RESET);
    scanf("%s", j.vehicleId);
    clearInputBuffer();
    
    if (!vehicleExists(j.vehicleId)) {
        printf(RED "Error: Vehicle ID not found.\n" RESET);
        pauseScreen();
        return;
    }

    if (getRoleLevel(currentUserRole) >= 3) {
        printf(YELLOW "Enter mechanic ID (assigned to this job): " RESET);
        scanf("%s", j.mechanicId);
        clearInputBuffer();
        if (!mechanicExists(j.mechanicId)) {
            printf(RED "Error: Invalid mechanic ID. Assigning to current user.\n" RESET);
            strcpy(j.mechanicId, currentUserId);
        }
    } else {
        strcpy(j.mechanicId, currentUserId);
    }
    
    printf(YELLOW "Enter job description (e.g., 'Oil Change', 'Brake Repair'): " RESET);
    scanf(" %[^\n]", j.description);
    clearInputBuffer();

    strcpy(j.status, "Pending");
    j.estimatedCost = calculateAverageCost(j.description);
    j.laborCost = 0.0;
    j.partsCost = 0.0;
    
    printf(YELLOW "Enter any notes: " RESET);
    scanf(" %[^\n]", j.notes);
    clearInputBuffer();
    
    getCurrentDate(j.startDate);
    strcpy(j.endDate, "N/A");

    int job_count = fileLineCount(JOB_FILE);
    generateId("J", job_count + 1, j.id);
    
    FILE *fp = fopen(JOB_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening file.\n" RESET);
        return;
    }
    fprintf(fp, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", 
            j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, 
            j.estimatedCost, j.laborCost, j.partsCost, j.notes, j.startDate, j.endDate);
    fclose(fp);
    
    printf(GREEN "Job added successfully! Job ID: %s\n" RESET, j.id);
    writeLog("New job added.");
    
    // Create notification for the mechanic
    char notification_msg[200];
    sprintf(notification_msg, "A new job (%s) has been assigned to you: %s", j.id, j.description);
    createNotification(j.mechanicId, notification_msg);

    pauseScreen();
}

// Function to update the status of a job
void updateJobStatus(const char *currentUserId, const char *currentUserRole) {
    char job_id[10];
    int choice;
    Job j;
    int found = 0;

    printf(YELLOW "Enter Job ID to update: " RESET);
    scanf("%s", job_id);
    clearInputBuffer();

    FILE *fp_read = fopen(JOB_FILE, "r");
    if (!fp_read) {
        printf(RED "Error: Job database not found.\n" RESET);
        return;
    }

    FILE *fp_write = fopen("temp_jobs.txt", "w");
    if (!fp_write) {
        fclose(fp_read);
        printf(RED "Error creating temp file.\n" RESET);
        return;
    }
    
    char line[500];
    while(fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", 
               j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, 
               &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if(strcmp(j.id, job_id) == 0) {
            // Check if user has permission to update this job
            if (strcmp(currentUserRole, "Admin") != 0 && strcmp(currentUserId, j.mechanicId) != 0) {
                printf(RED "Error: You do not have permission to update this job.\n" RESET);
                fprintf(fp_write, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", 
                    j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, 
                    j.estimatedCost, j.laborCost, j.partsCost, j.notes, j.startDate, j.endDate);
                continue;
            }

            found = 1;
            printf(YELLOW "Current status for Job %s: %s\n" RESET, j.id, j.status);
            printf(YELLOW "Select new status:\n" RESET);
            printf("1. In Progress\n");
            printf("2. Completed\n");
            printf("3. Cancelled\n");
            printf(YELLOW "Enter choice: " RESET);
            scanf("%d", &choice);
            clearInputBuffer();

            // Store old status for logging
            char old_status[20];
            strcpy(old_status, j.status);

            switch (choice) {
                case 1: 
                    strcpy(j.status, "In Progress"); 
                    printf(GREEN "Job status updated to 'In Progress'.\n" RESET);
                    break;
                case 2:
                    strcpy(j.status, "Completed");
                    getCurrentDate(j.endDate);
                    
                    // Update the last service date of the vehicle
                    FILE *fp_v_read = fopen(VEHICLE_FILE, "r");
                    if (fp_v_read) {
                        FILE *fp_v_write = fopen("temp_vehicles.txt", "w");
                        if (fp_v_write) {
                            Vehicle v;
                            char v_line[100];
                            while(fgets(v_line, sizeof(v_line), fp_v_read)) {
                                sscanf(v_line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]", 
                                       v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
                                if(strcmp(v.id, j.vehicleId) == 0) {
                                    strcpy(v.lastServiceDate, j.endDate);
                                }
                                fprintf(fp_v_write, "%s,%s,%s,%s,%s,%s,%s\n", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
                            }
                            fclose(fp_v_read);
                            fclose(fp_v_write);
                            remove(VEHICLE_FILE);
                            rename("temp_vehicles.txt", VEHICLE_FILE);
                        }
                    }

                    // Create notification for the customer
                    char notification_msg[200];
                    sprintf(notification_msg, "Your job (%s) is completed. Please check the invoice.", j.id);
                    createNotification(j.customerId, notification_msg);
                    printf(GREEN "Job status updated to 'Completed'.\n" RESET);
                    break;
                case 3: 
                    strcpy(j.status, "Cancelled"); 
                    printf(GREEN "Job status updated to 'Cancelled'.\n" RESET);
                    break;
                default: 
                    printf(RED "Invalid choice. Status not changed.\n" RESET);
                    break;
            }
            writeLog("Job status updated.");
        }
        fprintf(fp_write, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", 
                j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, 
                j.estimatedCost, j.laborCost, j.partsCost, j.notes, j.startDate, j.endDate);
    }
    
    fclose(fp_read);
    fclose(fp_write);

    remove(JOB_FILE);
    rename("temp_jobs.txt", JOB_FILE);
    
    if (!found) {
        printf(RED "Job not found.\n" RESET);
    }
    
    pauseScreen();
}

// Function to view jobs assigned to a specific mechanic
void viewMyJobs(const char *mechanicId) {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found.\n" RESET);
        pauseScreen();
        return;
    }

    Job j;
    int found = 0;
    printf("\n" CYAN "--- My Assigned Jobs ---" RESET "\n");
    printf(BLUE "ID      CustID  VehicleID   Description                  Status      Est. Cost\n" RESET);
    printf(BLUE "-----------------------------------------------------------------------------------\n" RESET);
    
    char line[500];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", 
               j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, 
               &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strcmp(j.mechanicId, mechanicId) == 0) {
            printf("%-7s %-7s %-11s %-28s %-11s %.2f\n", j.id, j.customerId, j.vehicleId, j.description, j.status, j.estimatedCost);
            found = 1;
        }
    }
    fclose(fp);

    if (!found) {
        printf(RED "No jobs assigned to you.\n" RESET);
    }
    pauseScreen();
}

// Function to view jobs for a specific customer
void viewCustomerJobs(const char *customerId) {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found for this customer.\n" RESET);
        pauseScreen();
        return;
    }

    Job j;
    int found = 0;
    printf("\n" CYAN "--- Your Jobs ---" RESET "\n");
    printf(BLUE "ID      VehicleID   Description                  Status      Est. Cost\n" RESET);
    printf(BLUE "----------------------------------------------------------------------\n" RESET);
    
    char line[500];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", 
               j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, 
               &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strcmp(j.customerId, customerId) == 0) {
            printf("%-7s %-11s %-28s %-11s %.2f\n", j.id, j.vehicleId, j.description, j.status, j.estimatedCost);
            found = 1;
        }
    }
    fclose(fp);

    if (!found) {
        printf(RED "No jobs found for you.\n" RESET);
    }
    pauseScreen();
}

// Function to search for a job by ID, customer ID, or vehicle ID
void searchJob() {
    char query[50];
    Job j;
    int found = 0;
    
    printf(YELLOW "Enter Job ID, Customer ID, or Vehicle ID to search: " RESET);
    scanf(" %[^\n]", query);
    clearInputBuffer();
    
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "Error: Job database not found.\n" RESET);
        pauseScreen();
        return;
    }
    
    printf("\n" CYAN "--- Search Results ---" RESET "\n");
    printf(BLUE "ID      CustID  VehicleID   MechanicID  Description                  Status\n" RESET);
    printf(BLUE "--------------------------------------------------------------------------\n" RESET);
    
    char line[500];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", 
               j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, 
               &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strstr(j.id, query) || strstr(j.customerId, query) || strstr(j.vehicleId, query)) {
            printf("%-7s %-7s %-11s %-11s %-28s %s\n", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status);
            found = 1;
        }
    }
    fclose(fp);
    
    if (!found) {
        printf(RED "No jobs found matching the query.\n" RESET);
    }
    
    pauseScreen();
}

// Function to view all jobs
void viewAllJobs() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found.\n" RESET);
        pauseScreen();
        return;
    }

    Job j;
    printf("\n" CYAN "--- All Jobs ---" RESET "\n");
    printf(BLUE "ID      CustID  VehicleID   MechanicID  Description                  Status      Cost\n" RESET);
    printf(BLUE "----------------------------------------------------------------------------------------\n" RESET);
    
    char line[500];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", 
               j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, 
               &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        printf("%-7s %-7s %-11s %-11s %-28s %-11s %.2f\n", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, j.laborCost + j.partsCost);
    }
    fclose(fp);
    
    pauseScreen();
}

// Function to view all pending jobs
void viewPendingJobs() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found.\n" RESET);
        pauseScreen();
        return;
    }

    Job j;
    int found = 0;
    printf("\n" CYAN "--- Pending Jobs ---" RESET "\n");
    printf(BLUE "ID      CustID  VehicleID   MechanicID  Description                  Start Date\n" RESET);
    printf(BLUE "----------------------------------------------------------------------------------\n" RESET);
    
    char line[500];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", 
               j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, 
               &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strcmp(j.status, "Pending") == 0) {
            printf("%-7s %-7s %-11s %-11s %-28s %s\n", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.startDate);
            found = 1;
        }
    }
    fclose(fp);
    
    if (!found) {
        printf(GREEN "No pending jobs found.\n" RESET);
    }
    pauseScreen();
}

// Function to view all completed jobs
void viewCompletedJobs() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found.\n" RESET);
        pauseScreen();
        return;
    }

    Job j;
    int found = 0;
    printf("\n" CYAN "--- Completed Jobs ---" RESET "\n");
    printf(BLUE "ID      CustID  VehicleID   Description                  Completion Date\n" RESET);
    printf(BLUE "-----------------------------------------------------------------------------\n" RESET);
    
    char line[500];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", 
               j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, 
               &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strcmp(j.status, "Completed") == 0) {
            printf("%-7s %-7s %-11s %-28s %s\n", j.id, j.customerId, j.vehicleId, j.description, j.endDate);
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
    char job_id[10];
    Job j;
    int found = 0;

    printf(YELLOW "Enter Job ID to generate invoice for: " RESET);
    scanf("%s", job_id);
    clearInputBuffer();
    
    FILE *fp_job = fopen(JOB_FILE, "r");
    if (!fp_job) {
        printf(RED "Error: Job database not found.\n" RESET);
        pauseScreen();
        return;
    }
    
    char line[500];
    while (fgets(line, sizeof(line), fp_job)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", 
               j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, 
               &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if (strcmp(j.id, job_id) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fp_job);

    if (!found) {
        printf(RED "Job not found.\n" RESET);
        pauseScreen();
        return;
    }

    if (strcmp(j.status, "Completed") != 0) {
        printf(YELLOW "Cannot generate invoice for a job that is not completed.\n" RESET);
        pauseScreen();
        return;
    }
    
    Invoice inv;
    inv.totalCost = j.laborCost + j.partsCost;
    inv.discount = 0.0; // Placeholder for future feature
    inv.total = inv.totalCost - inv.discount;
    strcpy(inv.jobId, j.id);
    getCurrentDate(inv.date);
    
    int invoice_count = fileLineCount(INVOICE_FILE);
    generateId("INV", invoice_count + 1, inv.id);

    FILE *fp_inv = fopen(INVOICE_FILE, "a+");
    if (!fp_inv) {
        printf(RED "Error opening invoice file.\n" RESET);
        return;
    }
    fprintf(fp_inv, "%s,%s,%.2f,%.2f,%.2f,%s\n", 
            inv.id, inv.jobId, inv.totalCost, inv.discount, inv.total, inv.date);
    fclose(fp_inv);

    printf(GREEN "\n--- Invoice Details ---" RESET "\n");
    printf(BLUE "Invoice ID: %s\n" RESET, inv.id);
    printf(BLUE "Job ID: %s\n" RESET, inv.jobId);
    printf(BLUE "Job Description: %s\n" RESET, j.description);
    printf(BLUE "Labor Cost: %.2f\n" RESET, j.laborCost);
    printf(BLUE "Parts Cost: %.2f\n" RESET, j.partsCost);
    printf(BLUE "Total Cost: %.2f\n" RESET, inv.totalCost);
    printf(BLUE "Discount: %.2f\n" RESET, inv.discount);
    printf(GREEN "Final Total: %.2f\n" RESET, inv.total);
    printf(BLUE "Date: %s\n" RESET, inv.date);

    // Add transaction for revenue
    Transaction t;
    generateId("REV", fileLineCount(FINANCE_FILE) + 1, t.id);
    strcpy(t.type, "Revenue");
    t.amount = inv.total;
    strcpy(t.date, inv.date);
    sprintf(t.description, "Invoice for job %s", j.id);
    
    FILE *fp_finance = fopen(FINANCE_FILE, "a+");
    if (fp_finance) {
        fprintf(fp_finance, "%s,%s,%.2f,%s,%s\n", t.id, t.type, t.amount, t.date, t.description);
        fclose(fp_finance);
    }

    writeLog("Invoice generated.");
    pauseScreen();
}

// Function to view all invoices
void viewAllInvoices() {
    FILE *fp = fopen(INVOICE_FILE, "r");
    if (!fp) {
        printf(RED "No invoices found.\n" RESET);
        pauseScreen();
        return;
    }
    
    Invoice inv;
    printf("\n" CYAN "--- All Invoices ---" RESET "\n");
    printf(BLUE "Invoice ID  Job ID    Total Cost  Date\n" RESET);
    printf(BLUE "-----------------------------------------\n" RESET);
    
    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%f,%*f,%f,%14[^\n]", inv.id, inv.jobId, &inv.totalCost, &inv.total, inv.date);
        printf("%-10s  %-8s  %-11.2f %s\n", inv.id, inv.jobId, inv.total, inv.date);
    }
    fclose(fp);
    
    pauseScreen();
}


// Inventory Management Functions

// Function to add a part to a job, updating the job's parts cost
void addPartToJob() {
    char job_id[10];
    char part_id[10];
    int quantity;
    Job j;
    int found_job = 0;
    
    printf(YELLOW "Enter Job ID: " RESET);
    scanf("%s", job_id);
    clearInputBuffer();
    
    printf(YELLOW "Enter Part ID: " RESET);
    scanf("%s", part_id);
    clearInputBuffer();

    printf(YELLOW "Enter quantity: " RESET);
    scanf("%d", &quantity);
    clearInputBuffer();
    
    if (!partExists(part_id)) {
        printf(RED "Error: Part not found.\n" RESET);
        pauseScreen();
        return;
    }

    if (quantity <= 0) {
        printf(RED "Error: Quantity must be greater than zero.\n" RESET);
        pauseScreen();
        return;
    }
    
    float part_price = getPartPrice(part_id);
    float cost = part_price * quantity;

    // Update the job's part cost and log the transaction
    FILE *fp_read = fopen(JOB_FILE, "r");
    if (!fp_read) {
        printf(RED "Error: Job database not found.\n" RESET);
        return;
    }

    FILE *fp_write = fopen("temp_jobs.txt", "w");
    if (!fp_write) {
        fclose(fp_read);
        printf(RED "Error creating temp file.\n" RESET);
        return;
    }
    
    char line[500];
    while(fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", 
               j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, 
               &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if(strcmp(j.id, job_id) == 0) {
            j.partsCost += cost;
            found_job = 1;
            
            // Add a record to job_parts.txt
            FILE *fp_job_parts = fopen(JOB_PART_FILE, "a+");
            if (fp_job_parts) {
                JobPart jp;
                strcpy(jp.jobId, j.id);
                strcpy(jp.partId, part_id);
                jp.quantity = quantity;
                jp.price = part_price;
                fprintf(fp_job_parts, "%s,%s,%d,%.2f\n", jp.jobId, jp.partId, jp.quantity, jp.price);
                fclose(fp_job_parts);
            }
            
            // Deduct from inventory
            deductPartFromInventory(part_id, quantity);
            
            printf(GREEN "Part added to job successfully. New parts cost: %.2f\n" RESET, j.partsCost);
            writeLog("Part added to job.");
        }
        fprintf(fp_write, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", 
                j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, 
                j.estimatedCost, j.laborCost, j.partsCost, j.notes, j.startDate, j.endDate);
    }
    
    fclose(fp_read);
    fclose(fp_write);

    remove(JOB_FILE);
    rename("temp_jobs.txt", JOB_FILE);

    if (!found_job) {
        printf(RED "Job not found.\n" RESET);
    }
    
    pauseScreen();
}

// Function to update the labor cost of a job
void updateJobLaborCost() {
    char job_id[10];
    float labor_cost;
    Job j;
    int found = 0;

    printf(YELLOW "Enter Job ID to update labor cost: " RESET);
    scanf("%s", job_id);
    clearInputBuffer();
    
    printf(YELLOW "Enter new labor cost: " RESET);
    scanf("%f", &labor_cost);
    clearInputBuffer();
    
    if (labor_cost < 0) {
        printf(RED "Error: Labor cost cannot be negative.\n" RESET);
        pauseScreen();
        return;
    }

    FILE *fp_read = fopen(JOB_FILE, "r");
    if (!fp_read) {
        printf(RED "Error: Job database not found.\n" RESET);
        return;
    }

    FILE *fp_write = fopen("temp_jobs.txt", "w");
    if (!fp_write) {
        fclose(fp_read);
        printf(RED "Error creating temp file.\n" RESET);
        return;
    }
    
    char line[500];
    while(fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", 
               j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, 
               &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        if(strcmp(j.id, job_id) == 0) {
            j.laborCost = labor_cost;
            found = 1;
            printf(GREEN "Labor cost for job %s updated successfully.\n" RESET, j.id);
            writeLog("Job labor cost updated.");
        }
        fprintf(fp_write, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", 
                j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, 
                j.estimatedCost, j.laborCost, j.partsCost, j.notes, j.startDate, j.endDate);
    }
    
    fclose(fp_read);
    fclose(fp_write);

    remove(JOB_FILE);
    rename("temp_jobs.txt", JOB_FILE);
    
    if (!found) {
        printf(RED "Job not found.\n" RESET);
    }
    
    pauseScreen();
}

// Function to add a new part to inventory
void addPart() {
    Part p;
    printf(GREEN "\nAdd New Part to Inventory:\n" RESET);
    printf(YELLOW "Enter part name: " RESET);
    scanf(" %[^\n]", p.name);
    clearInputBuffer();
    printf(YELLOW "Enter initial quantity: " RESET);
    scanf("%d", &p.quantity);
    clearInputBuffer();
    printf(YELLOW "Enter price per unit: " RESET);
    scanf("%f", &p.price);
    clearInputBuffer();
    printf(YELLOW "Enter supplier ID: " RESET);
    scanf("%s", p.supplierId);
    clearInputBuffer();
    
    if (!supplierExists(p.supplierId)) {
        printf(RED "Error: Supplier not found.\n" RESET);
        pauseScreen();
        return;
    }

    printf(YELLOW "Enter reorder level: " RESET);
    scanf("%d", &p.reorderLevel);
    clearInputBuffer();
    
    int part_count = fileLineCount(PART_FILE);
    generateId("P", part_count + 1, p.id);
    
    FILE *fp = fopen(PART_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening file.\n" RESET);
        return;
    }
    fprintf(fp, "%s,%s,%d,%.2f,%s,%d\n", p.id, p.name, p.quantity, p.price, p.supplierId, p.reorderLevel);
    fclose(fp);
    
    printf(GREEN "Part added successfully! Part ID: %s\n" RESET, p.id);
    writeLog("New part added to inventory.");
    pauseScreen();
}

// Function to edit an existing part in inventory
void editPart() {
    char part_id[10];
    Part p;
    int found = 0;
    
    printf(YELLOW "Enter Part ID to edit: " RESET);
    scanf("%s", part_id);
    clearInputBuffer();

    FILE *fp_read = fopen(PART_FILE, "r");
    if (!fp_read) {
        printf(RED "Error: Part database not found.\n" RESET);
        return;
    }

    FILE *fp_write = fopen("temp_parts.txt", "w");
    if (!fp_write) {
        fclose(fp_read);
        printf(RED "Error creating temp file.\n" RESET);
        return;
    }
    
    char line[100];
    while(fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d", p.id, p.name, &p.quantity, &p.price, p.supplierId, &p.reorderLevel);
        if(strcmp(p.id, part_id) == 0) {
            found = 1;
            printf(YELLOW "Editing part %s...\n" RESET, p.id);
            printf(YELLOW "Enter new name (current: %s): " RESET, p.name);
            scanf(" %[^\n]", p.name);
            clearInputBuffer();
            printf(YELLOW "Enter new quantity (current: %d): " RESET, p.quantity);
            scanf("%d", &p.quantity);
            clearInputBuffer();
            printf(YELLOW "Enter new price (current: %.2f): " RESET, p.price);
            scanf("%f", &p.price);
            clearInputBuffer();
            printf(YELLOW "Enter new reorder level (current: %d): " RESET, p.reorderLevel);
            scanf("%d", &p.reorderLevel);
            clearInputBuffer();
            printf(GREEN "Part updated successfully.\n" RESET);
        }
        fprintf(fp_write, "%s,%s,%d,%.2f,%s,%d\n", p.id, p.name, p.quantity, p.price, p.supplierId, p.reorderLevel);
    }
    
    fclose(fp_read);
    fclose(fp_write);

    remove(PART_FILE);
    rename("temp_parts.txt", PART_FILE);
    
    if (!found) {
        printf(RED "Part not found.\n" RESET);
    }
    writeLog("Part details updated.");
    pauseScreen();
}

// Function to view all parts in inventory
void viewInventory() {
    FILE *fp = fopen(PART_FILE, "r");
    if (!fp) {
        printf(RED "No parts found.\n" RESET);
        pauseScreen();
        return;
    }
    
    Part p;
    printf("\n" CYAN "--- Inventory ---" RESET "\n");
    printf(BLUE "ID      Name                Quantity    Price       Supplier    Reorder Level\n" RESET);
    printf(BLUE "--------------------------------------------------------------------------------\n" RESET);
    
    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d", p.id, p.name, &p.quantity, &p.price, p.supplierId, &p.reorderLevel);
        printf("%-7s %-19s %-11d %-11.2f %-11s %d\n", p.id, p.name, p.quantity, p.price, p.supplierId, p.reorderLevel);
    }
    fclose(fp);
    
    pauseScreen();
}

// Checks if a part exists
int partExists(const char *id) {
    FILE *fp = fopen(PART_FILE, "r");
    if (!fp) return 0;
    
    Part p;
    char line[100];
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d", p.id, p.name, &p.quantity, &p.price, p.supplierId, &p.reorderLevel);
        if(strcmp(p.id, id) == 0) {
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
    char line[100];
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d", p.id, p.name, &p.quantity, &p.price, p.supplierId, &p.reorderLevel);
        if(strcmp(p.id, partId) == 0) {
            fclose(fp);
            return p.price;
        }
    }
    fclose(fp);
    return 0.0;
}

// Deducts parts from inventory and checks for low stock
void deductPartFromInventory(const char *partId, int quantity) {
    Part p;
    int found = 0;
    
    FILE *fp_read = fopen(PART_FILE, "r");
    if (!fp_read) {
        printf(RED "Error: Part database not found.\n" RESET);
        return;
    }

    FILE *fp_write = fopen("temp_parts.txt", "w");
    if (!fp_write) {
        fclose(fp_read);
        printf(RED "Error creating temp file.\n" RESET);
        return;
    }
    
    char line[100];
    while(fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d", p.id, p.name, &p.quantity, &p.price, p.supplierId, &p.reorderLevel);
        if(strcmp(p.id, partId) == 0) {
            p.quantity -= quantity;
            found = 1;
        }
        fprintf(fp_write, "%s,%s,%d,%.2f,%s,%d\n", p.id, p.name, p.quantity, p.price, p.supplierId, p.reorderLevel);
    }
    
    fclose(fp_read);
    fclose(fp_write);

    remove(PART_FILE);
    rename("temp_parts.txt", PART_FILE);
    
    if (found) {
        checkLowStock();
    }
}

// Checks for low stock and adds to reorder list
void checkLowStock() {
    FILE *fp = fopen(PART_FILE, "r");
    if (!fp) return;
    
    FILE *fp_reorder = fopen(REORDER_LIST_FILE, "a+");
    if (!fp_reorder) {
        fclose(fp);
        return;
    }
    
    Part p;
    char line[100];
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%d,%f,%9[^,],%d", p.id, p.name, &p.quantity, &p.price, p.supplierId, &p.reorderLevel);
        if (p.quantity <= p.reorderLevel) {
            int already_on_list = 0;
            // Check if already on the list to avoid duplicates
            FILE *fp_temp = fopen(REORDER_LIST_FILE, "r");
            if (fp_temp) {
                char temp_line[100];
                char temp_part_id[10];
                while(fgets(temp_line, sizeof(temp_line), fp_temp)) {
                    sscanf(temp_line, "%9[^,],", temp_part_id);
                    if (strcmp(temp_part_id, p.id) == 0) {
                        already_on_list = 1;
                        break;
                    }
                }
                fclose(fp_temp);
            }
            
            if (!already_on_list) {
                ReorderItem ri;
                strcpy(ri.partId, p.id);
                ri.neededQuantity = p.reorderLevel * 2; // Example reorder quantity
                getCurrentDate(ri.date);
                fprintf(fp_reorder, "%s,%d,%s\n", ri.partId, ri.neededQuantity, ri.date);
                
                // Create notification for admin/manager
                char notification_msg[200];
                sprintf(notification_msg, "Low stock alert: %s (ID: %s) is below reorder level.", p.name, p.id);
                // Assuming an admin with ID "U0001" exists for this notification.
                createNotification("U0001", notification_msg);
            }
        }
    }
    fclose(fp);
    fclose(fp_reorder);
}

// Views the reorder list
void viewReorderList() {
    FILE *fp = fopen(REORDER_LIST_FILE, "r");
    if (!fp) {
        printf(GREEN "Reorder list is empty.\n" RESET);
        pauseScreen();
        return;
    }
    
    ReorderItem ri;
    printf("\n" CYAN "--- Reorder List ---" RESET "\n");
    printf(BLUE "Part ID     Needed Quantity Date\n" RESET);
    printf(BLUE "----------------------------------\n" RESET);
    
    char line[100];
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%d,%14[^\n]", ri.partId, &ri.neededQuantity, ri.date);
        printf("%-11s %-16d %s\n", ri.partId, ri.neededQuantity, ri.date);
    }
    fclose(fp);
    
    pauseScreen();
}

// Clears the reorder list
void clearReorderList() {
    FILE *fp = fopen(REORDER_LIST_FILE, "w");
    if (fp) {
        fclose(fp);
        printf(GREEN "Reorder list cleared successfully.\n" RESET);
        writeLog("Reorder list cleared.");
    } else {
        printf(RED "Error clearing reorder list.\n" RESET);
    }
    pauseScreen();
}


// Supplier Management Functions

// Function to add a new supplier
void addSupplier() {
    Supplier s;
    printf(GREEN "\nAdd New Supplier:\n" RESET);
    printf(YELLOW "Enter supplier name: " RESET);
    scanf(" %[^\n]", s.name);
    clearInputBuffer();
    printf(YELLOW "Enter phone number: " RESET);
    scanf(" %[^\n]", s.phone);
    clearInputBuffer();
    printf(YELLOW "Enter email: " RESET);
    scanf(" %[^\n]", s.email);
    clearInputBuffer();
    
    int supplier_count = fileLineCount(SUPPLIER_FILE);
    generateId("S", supplier_count + 1, s.id);
    
    FILE *fp = fopen(SUPPLIER_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening file.\n" RESET);
        return;
    }
    fprintf(fp, "%s,%s,%s,%s\n", s.id, s.name, s.phone, s.email);
    fclose(fp);
    
    printf(GREEN "Supplier added successfully! Supplier ID: %s\n" RESET, s.id);
    writeLog("New supplier added.");
    pauseScreen();
}

// Function to view all suppliers
void viewAllSuppliers() {
    FILE *fp = fopen(SUPPLIER_FILE, "r");
    if (!fp) {
        printf(RED "No suppliers found.\n" RESET);
        pauseScreen();
        return;
    }
    
    Supplier s;
    printf("\n" CYAN "--- All Suppliers ---" RESET "\n");
    printf(BLUE "ID      Name                Phone           Email\n" RESET);
    printf(BLUE "-----------------------------------------------------\n" RESET);
    
    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", s.id, s.name, s.phone, s.email);
        printf("%-7s %-19s %-15s %s\n", s.id, s.name, s.phone, s.email);
    }
    fclose(fp);
    
    pauseScreen();
}

// Checks if a supplier exists
int supplierExists(const char *id) {
    FILE *fp = fopen(SUPPLIER_FILE, "r");
    if (!fp) return 0;
    
    Supplier s;
    char line[100];
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", s.id, s.name, s.phone, s.email);
        if(strcmp(s.id, id) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}


// Appointment Management Functions

// Function to schedule a new appointment
void scheduleAppointment(const char *currentUserId, const char *currentUserRole) {
    Appointment app;
    
    printf(GREEN "\nSchedule New Appointment:\n" RESET);
    if (strcmp(currentUserRole, "Customer") == 0) {
        strcpy(app.customerId, currentUserId);
    } else {
        printf(YELLOW "Enter customer ID for the appointment: " RESET);
        scanf("%s", app.customerId);
        clearInputBuffer();
    }

    if (!customerExists(app.customerId)) {
        printf(RED "Error: Customer ID not found.\n" RESET);
        pauseScreen();
        return;
    }
    
    printf(YELLOW "Enter vehicle ID: " RESET);
    scanf("%s", app.vehicleId);
    clearInputBuffer();
    
    if (!vehicleExists(app.vehicleId)) {
        printf(RED "Error: Vehicle ID not found.\n" RESET);
        pauseScreen();
        return;
    }

    printf(YELLOW "Enter appointment date (YYYY-MM-DD): " RESET);
    scanf("%s", app.date);
    clearInputBuffer();
    if (!isDateValid(app.date)) {
        printf(RED "Invalid date format. Please use YYYY-MM-DD.\n" RESET);
        pauseScreen();
        return;
    }
    
    printf(YELLOW "Enter appointment time (HH:MM): " RESET);
    scanf("%s", app.time);
    clearInputBuffer();
    
    printf(YELLOW "Enter reason for appointment: " RESET);
    scanf(" %[^\n]", app.description);
    clearInputBuffer();
    
    strcpy(app.status, "Scheduled");
    
    int appointment_count = fileLineCount(APPOINTMENT_FILE);
    generateId("APP", appointment_count + 1, app.id);
    
    FILE *fp = fopen(APPOINTMENT_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening file.\n" RESET);
        return;
    }
    fprintf(fp, "%s,%s,%s,%s,%s,%s,%s\n", app.id, app.customerId, app.vehicleId, app.date, app.time, app.description, app.status);
    fclose(fp);
    
    printf(GREEN "Appointment scheduled successfully! Appointment ID: %s\n" RESET, app.id);
    writeLog("New appointment scheduled.");
    pauseScreen();
}

// Function to view all appointments
void viewAllAppointments() {
    FILE *fp = fopen(APPOINTMENT_FILE, "r");
    if (!fp) {
        printf(RED "No appointments found.\n" RESET);
        pauseScreen();
        return;
    }
    
    Appointment app;
    printf("\n" CYAN "--- All Appointments ---" RESET "\n");
    printf(BLUE "ID      CustID  VehicleID   Date        Time    Description                  Status\n" RESET);
    printf(BLUE "--------------------------------------------------------------------------------------\n" RESET);
    
    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%14[^,],%9[^,],%99[^,],%19[^\n]", 
               app.id, app.customerId, app.vehicleId, app.date, app.time, app.description, app.status);
        printf("%-7s %-7s %-11s %-11s %-7s %-28s %s\n", 
               app.id, app.customerId, app.vehicleId, app.date, app.time, app.description, app.status);
    }
    fclose(fp);
    
    pauseScreen();
}

// Function to schedule service reminders
void scheduleServiceReminder() {
    char vehicle_id[10];
    Vehicle v;
    int found = 0;
    
    printf(YELLOW "Enter Vehicle ID to schedule a service reminder for: " RESET);
    scanf("%s", vehicle_id);
    clearInputBuffer();

    FILE *fp_read = fopen(VEHICLE_FILE, "r");
    if (!fp_read) {
        printf(RED "Error: Vehicle database not found.\n" RESET);
        return;
    }
    
    char line[100];
    while(fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^,],%14[^\n]", 
               v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
        if(strcmp(v.id, vehicle_id) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fp_read);

    if (!found) {
        printf(RED "Vehicle not found.\n" RESET);
        pauseScreen();
        return;
    }

    char next_service_date[15];
    calculateNextServiceDate(v.lastServiceDate, next_service_date);
    
    if (strcmp(next_service_date, "N/A") == 0) {
        printf(RED "Cannot schedule a reminder without a last service date.\n" RESET);
        pauseScreen();
        return;
    }

    // Schedule the reminder for the customer
    Reminder r;
    int reminder_count = fileLineCount(REMINDERS_FILE);
    generateId("REM", reminder_count + 1, r.id);
    strcpy(r.customerId, v.customerId);
    sprintf(r.message, "Reminder: Your %s %s is due for service on %s.", v.brand, v.model, next_service_date);
    strcpy(r.date, next_service_date);
    strcpy(r.status, "Scheduled");

    FILE *fp = fopen(REMINDERS_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening reminders file.\n" RESET);
        return;
    }
    fprintf(fp, "%s,%s,%s,%s,%s\n", r.id, r.customerId, r.message, r.date, r.status);
    fclose(fp);

    printf(GREEN "Service reminder scheduled for customer %s on %s.\n" RESET, r.customerId, r.date);
    writeLog("Service reminder scheduled.");
    pauseScreen();
}

// Function to view all scheduled reminders
void viewAllReminders() {
    FILE *fp = fopen(REMINDERS_FILE, "r");
    if (!fp) {
        printf(RED "No reminders found.\n" RESET);
        pauseScreen();
        return;
    }
    
    Reminder r;
    printf("\n" CYAN "--- All Reminders ---" RESET "\n");
    printf(BLUE "ID      CustID  Date        Status      Message\n" RESET);
    printf(BLUE "--------------------------------------------------------\n" RESET);
    
    char line[200];
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%199[^,],%14[^,],%19[^\n]", 
               r.id, r.customerId, r.message, r.date, r.status);
        printf("%-7s %-7s %-11s %-11s %s\n", r.id, r.customerId, r.date, r.status, r.message);
    }
    fclose(fp);
    
    pauseScreen();
}


// Financial Management Functions

// Function to add a new expense
void addExpense() {
    Transaction t;
    printf(GREEN "\nAdd New Expense:\n" RESET);
    
    printf(YELLOW "Enter amount: " RESET);
    scanf("%f", &t.amount);
    clearInputBuffer();
    
    printf(YELLOW "Enter description: " RESET);
    scanf(" %[^\n]", t.description);
    clearInputBuffer();
    
    generateId("EXP", fileLineCount(FINANCE_FILE) + 1, t.id);
    strcpy(t.type, "Expense");
    getCurrentDate(t.date);
    
    FILE *fp = fopen(FINANCE_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening file.\n" RESET);
        return;
    }
    fprintf(fp, "%s,%s,%.2f,%s,%s\n", t.id, t.type, t.amount, t.date, t.description);
    fclose(fp);
    
    printf(GREEN "Expense added successfully! Expense ID: %s\n" RESET, t.id);
    writeLog("New expense added.");
    pauseScreen();
}

// Function to view all expenses
void viewAllExpenses() {
    FILE *fp = fopen(FINANCE_FILE, "r");
    if (!fp) {
        printf(RED "No financial records found.\n" RESET);
        pauseScreen();
        return;
    }

    Transaction t;
    int found = 0;
    printf("\n" CYAN "--- All Expenses ---" RESET "\n");
    printf(BLUE "ID      Amount      Date        Description\n" RESET);
    printf(BLUE "-----------------------------------------------------\n" RESET);
    
    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%19[^,],%f,%14[^,],%99[^\n]", t.id, t.type, &t.amount, t.date, t.description);
        if (strcmp(t.type, "Expense") == 0) {
            printf("%-7s %-11.2f %-11s %s\n", t.id, t.amount, t.date, t.description);
            found = 1;
        }
    }
    fclose(fp);
    
    if (!found) {
        printf(GREEN "No expenses found.\n" RESET);
    }
    pauseScreen();
}

// Generates a profit and loss report
void generateProfitLossReport() {
    FILE *fp = fopen(FINANCE_FILE, "r");
    if (!fp) {
        printf(RED "No financial records found.\n" RESET);
        pauseScreen();
        return;
    }

    Transaction t;
    float total_revenue = 0.0;
    float total_expenses = 0.0;
    
    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%19[^,],%f,%14[^,],%99[^\n]", t.id, t.type, &t.amount, t.date, t.description);
        if (strcmp(t.type, "Revenue") == 0) {
            total_revenue += t.amount;
        } else if (strcmp(t.type, "Expense") == 0) {
            total_expenses += t.amount;
        }
    }
    fclose(fp);
    
    float net_profit = total_revenue - total_expenses;
    
    printf("\n" CYAN "--- Profit & Loss Report ---" RESET "\n");
    printf(BLUE "----------------------------\n" RESET);
    printf(GREEN "Total Revenue: %.2f\n" RESET, total_revenue);
    printf(RED "Total Expenses: %.2f\n" RESET, total_expenses);
    
    if (net_profit >= 0) {
        printf(GREEN "Net Profit: %.2f\n" RESET, net_profit);
    } else {
        printf(RED "Net Loss: %.2f\n" RESET, net_profit);
    }
    printf(BLUE "----------------------------\n" RESET);
    
    pauseScreen();
}

// Reporting Functions

// Generates a summary report of jobs
void viewJobSummaryReport() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No job records found.\n" RESET);
        pauseScreen();
        return;
    }

    int total_jobs = 0, pending_jobs = 0, in_progress_jobs = 0, completed_jobs = 0;
    float total_revenue = 0.0, total_labor_cost = 0.0, total_parts_cost = 0.0;
    
    Job j;
    char line[500];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", 
               j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, 
               &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
        
        total_jobs++;
        if (strcmp(j.status, "Pending") == 0) pending_jobs++;
        else if (strcmp(j.status, "In Progress") == 0) in_progress_jobs++;
        else if (strcmp(j.status, "Completed") == 0) {
            completed_jobs++;
            total_labor_cost += j.laborCost;
            total_parts_cost += j.partsCost;
            total_revenue += j.laborCost + j.partsCost;
        }
    }
    fclose(fp);
    
    printf("\n" CYAN "--- Job Summary Report ---" RESET "\n");
    printf(BLUE "--------------------------------\n" RESET);
    printf("Total Jobs: %d\n", total_jobs);
    printf("Pending Jobs: %d\n", pending_jobs);
    printf("In Progress Jobs: %d\n", in_progress_jobs);
    printf("Completed Jobs: %d\n", completed_jobs);
    printf("Total Revenue from Completed Jobs: %.2f\n", total_revenue);
    printf("Total Labor Cost: %.2f\n", total_labor_cost);
    printf("Total Parts Cost: %.2f\n", total_parts_cost);
    printf(BLUE "--------------------------------\n" RESET);
    
    pauseScreen();
}

// Generates a report on mechanic performance
void mechanicPerformanceReport() {
    FILE *fp_users = fopen(USER_FILE, "r");
    if (!fp_users) {
        printf(RED "User database not found.\n" RESET);
        pauseScreen();
        return;
    }
    
    FILE *fp_jobs = fopen(JOB_FILE, "r");
    if (!fp_jobs) {
        fclose(fp_users);
        printf(RED "Job database not found.\n" RESET);
        pauseScreen();
        return;
    }

    printf("\n" CYAN "--- Mechanic Performance Report ---" RESET "\n");
    printf(BLUE "Mechanic ID  Name            Completed Jobs  Total Revenue  Average Job Time (Days)\n" RESET);
    printf(BLUE "----------------------------------------------------------------------------------------\n" RESET);
    
    char line_user[100];
    while (fgets(line_user, sizeof(line_user), fp_users)) {
        User u;
        sscanf(line_user, "%9[^,],%29[^,],%29[^,],%19[^\n]", u.id, u.username, u.password, u.role);
        if (getRoleLevel(u.role) < 2) continue; // Only show mechanics

        int completed_jobs = 0;
        float total_revenue = 0.0;
        float total_days = 0.0;
        
        Job j;
        char line_job[500];
        fseek(fp_jobs, 0, SEEK_SET); // Reset job file pointer
        while (fgets(line_job, sizeof(line_job), fp_jobs)) {
            sscanf(line_job, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%f,%f,%199[^,],%14[^,],%14[^\n]", 
                   j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, 
                   &j.estimatedCost, &j.laborCost, &j.partsCost, j.notes, j.startDate, j.endDate);
            
            if (strcmp(j.mechanicId, u.id) == 0 && strcmp(j.status, "Completed") == 0) {
                completed_jobs++;
                total_revenue += j.laborCost + j.partsCost;
                total_days += daysBetweenDates(j.startDate, j.endDate);
            }
        }
        
        float avg_days = (completed_jobs > 0) ? (total_days / completed_jobs) : 0;
        printf("%-11s  %-15s %-15d %-14.2f %.2f\n", u.id, u.username, completed_jobs, total_revenue, avg_days);
    }
    
    fclose(fp_users);
    fclose(fp_jobs);
    
    pauseScreen();
}

// Generates a report on customer lifetime value
void customerLifetimeValueReport() {
    FILE *fp_customers = fopen(CUSTOMER_FILE, "r");
    if (!fp_customers) {
        printf(RED "Customer database not found.\n" RESET);
        pauseScreen();
        return;
    }

    FILE *fp_jobs = fopen(JOB_FILE, "r");
    if (!fp_jobs) {
        fclose(fp_customers);
        printf(RED "Job database not found.\n" RESET);
        pauseScreen();
        return;
    }

    printf("\n" CYAN "--- Customer Lifetime Value Report ---" RESET "\n");
    printf(BLUE "Customer ID  Name                Total Spent\n" RESET);
    printf(BLUE "----------------------------------------------\n" RESET);
    
    char line_customer[100];
    while (fgets(line_customer, sizeof(line_customer), fp_customers)) {
        Customer c;
        sscanf(line_customer, "%9[^,],%49[^,],%*[^,],%*[^,]", c.id, c.name);

        float total_spent = 0.0;
        Job j;
        char line_job[500];
        fseek(fp_jobs, 0, SEEK_SET); // Reset job file pointer
        while (fgets(line_job, sizeof(line_job), fp_jobs)) {
            sscanf(line_job, "%9[^,],%9[^,],%*[^,],%*[^,],%*[^,],%19[^,],%*f,%f,%f,%*[^,],%*[^,],%*[^,]", 
                   j.id, j.customerId, j.status, &j.laborCost, &j.partsCost);
            
            if (strcmp(j.customerId, c.id) == 0 && strcmp(j.status, "Completed") == 0) {
                total_spent += j.laborCost + j.partsCost;
            }
        }
        
        printf("%-11s  %-19s %.2f\n", c.id, c.name, total_spent);
    }
    
    fclose(fp_customers);
    fclose(fp_jobs);
    
    pauseScreen();
}

// Generates a report of monthly revenue
void monthlyRevenueReport() {
    FILE *fp = fopen(FINANCE_FILE, "r");
    if (!fp) {
        printf(RED "No financial records found.\n" RESET);
        pauseScreen();
        return;
    }
    
    float monthly_revenue[12] = {0};
    int current_year, month, day, year;
    
    char current_date[15];
    getCurrentDate(current_date);
    sscanf(current_date, "%d-%*d-%*d", &current_year);
    
    Transaction t;
    char line[200];
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%*[^,],%19[^,],%f,%d-%d-%d,%99[^\n]", t.type, &t.amount, &year, &month, &day, t.description);
        if (strcmp(t.type, "Revenue") == 0 && year == current_year) {
            monthly_revenue[month-1] += t.amount;
        }
    }
    fclose(fp);
    
    printf("\n" CYAN "--- Monthly Revenue Report (%d) ---" RESET "\n", current_year);
    printf(BLUE "Month       Revenue\n" RESET);
    printf(BLUE "--------------------------\n" RESET);
    
    char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    for (int i = 0; i < 12; i++) {
        printf("%-11s %.2f\n", months[i], monthly_revenue[i]);
    }
    
    pauseScreen();
}

// Generates a report of part usage
void partUsageReport() {
    FILE *fp_parts = fopen(PART_FILE, "r");
    if (!fp_parts) {
        printf(RED "Part database not found.\n" RESET);
        pauseScreen();
        return;
    }

    FILE *fp_job_parts = fopen(JOB_PART_FILE, "r");
    if (!fp_job_parts) {
        fclose(fp_parts);
        printf(RED "Job Parts database not found.\n" RESET);
        pauseScreen();
        return;
    }

    printf("\n" CYAN "--- Part Usage Report ---" RESET "\n");
    printf(BLUE "Part ID     Name                Quantity Used\n" RESET);
    printf(BLUE "------------------------------------------------\n" RESET);
    
    Part p;
    char line_part[100];
    while (fgets(line_part, sizeof(line_part), fp_parts)) {
        sscanf(line_part, "%9[^,],%49[^,],%*d,%*f,%*[^,],%*d", p.id, p.name);

        int quantity_used = 0;
        JobPart jp;
        char line_jp[100];
        fseek(fp_job_parts, 0, SEEK_SET); // Reset job parts file pointer
        while (fgets(line_jp, sizeof(line_jp), fp_job_parts)) {
            sscanf(line_jp, "%*[^,],%9[^,],%d,%*f", jp.jobId, jp.partId, &jp.quantity);
            if (strcmp(jp.partId, p.id) == 0) {
                quantity_used += jp.quantity;
            }
        }
        printf("%-11s %-19s %d\n", p.id, p.name, quantity_used);
    }
    
    fclose(fp_parts);
    fclose(fp_job_parts);
    
    pauseScreen();
}

// Generates a report on supplier performance
void supplierPerformanceReport() {
    FILE *fp_suppliers = fopen(SUPPLIER_FILE, "r");
    if (!fp_suppliers) {
        printf(RED "Supplier database not found.\n" RESET);
        pauseScreen();
        return;
    }

    FILE *fp_parts = fopen(PART_FILE, "r");
    if (!fp_parts) {
        fclose(fp_suppliers);
        printf(RED "Part database not found.\n" RESET);
        pauseScreen();
        return;
    }

    printf("\n" CYAN "--- Supplier Performance Report ---" RESET "\n");
    printf(BLUE "Supplier ID Name                Total Parts Supplied\n" RESET);
    printf(BLUE "-------------------------------------------------------\n" RESET);
    
    Supplier s;
    char line_supplier[100];
    while (fgets(line_supplier, sizeof(line_supplier), fp_suppliers)) {
        sscanf(line_supplier, "%9[^,],%49[^,],%*[^,],%*[^,]", s.id, s.name);

        int total_parts = 0;
        Part p;
        char line_part[100];
        fseek(fp_parts, 0, SEEK_SET); // Reset part file pointer
        while (fgets(line_part, sizeof(line_part), fp_parts)) {
            sscanf(line_part, "%*[^,],%*[^,],%*d,%*f,%9[^,],%d", p.supplierId, &p.reorderLevel);
            // Assuming initial quantity is a good proxy for parts supplied
            if (strcmp(p.supplierId, s.id) == 0) {
                total_parts += p.reorderLevel;
            }
        }
        printf("%-11s %-19s %d\n", s.id, s.name, total_parts);
    }
    
    fclose(fp_suppliers);
    fclose(fp_parts);
    
    pauseScreen();
}

// Generates a report on customer retention
void customerRetentionReport() {
    FILE *fp_customers = fopen(CUSTOMER_FILE, "r");
    if (!fp_customers) {
        printf(RED "Customer database not found.\n" RESET);
        pauseScreen();
        return;
    }

    FILE *fp_jobs = fopen(JOB_FILE, "r");
    if (!fp_jobs) {
        fclose(fp_customers);
        printf(RED "Job database not found.\n" RESET);
        pauseScreen();
        return;
    }

    printf("\n" CYAN "--- Customer Retention Report ---" RESET "\n");
    printf(BLUE "Customer ID  Name                Jobs Completed\n" RESET);
    printf(BLUE "-------------------------------------------------\n" RESET);
    
    Customer c;
    char line_customer[100];
    while (fgets(line_customer, sizeof(line_customer), fp_customers)) {
        sscanf(line_customer, "%9[^,],%49[^,],%*[^,],%*[^,]", c.id, c.name);

        int job_count = 0;
        Job j;
        char line_job[500];
        fseek(fp_jobs, 0, SEEK_SET); // Reset job file pointer
        while (fgets(line_job, sizeof(line_job), fp_jobs)) {
            sscanf(line_job, "%*[^,],%9[^,],%*[^,],%*[^,],%*[^,],%19[^,],%*f,%*f,%*f,%*[^,],%*[^,],%*[^,]", 
                   j.id, j.customerId, j.status);
            
            if (strcmp(j.customerId, c.id) == 0 && strcmp(j.status, "Completed") == 0) {
                job_count++;
            }
        }
        
        printf("%-11s  %-19s %d\n", c.id, c.name, job_count);
    }
    
    fclose(fp_customers);
    fclose(fp_jobs);
    
    pauseScreen();
}


// Menu Functions

// Admin Menu
void adminMenu(const char *uid) {
    int choice;
    do {
        printf("\n" MAGENTA "--- Admin Menu ---" RESET "\n");
        printf("1. Manage Users\n");
        printf("2. Manage Customers\n");
        printf("3. Manage Vehicles\n");
        printf("4. Manage Jobs\n");
        printf("5. Manage Inventory\n");
        printf("6. Manage Suppliers\n");
        printf("7. Manage Appointments\n");
        printf("8. Financial Management\n");
        printf("9. View Reports\n");
        printf("10. Backup Data\n");
        printf("11. Restore Data\n");
        printf("12. View Notifications\n");
        printf("13. Change My Password\n");
        printf("0. Logout\n");
        printf(YELLOW "Enter choice: " RESET);
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch (choice) {
            case 1:
                // User Management Sub-menu
                do {
                    printf("\n" CYAN "--- User Management ---" RESET "\n");
                    printf("1. Register New User\n");
                    printf("2. View All Users\n");
                    printf("3. Change User Role\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter choice: " RESET);
                    scanf("%d", &choice);
                    clearInputBuffer();
                    switch (choice) {
                        case 1: registerUser(); break;
                        case 2: viewAllUsers(); break;
                        case 3: changeUserRole(); break;
                    }
                } while (choice != 0);
                break;
            case 2:
                // Customer Management Sub-menu
                do {
                    printf("\n" CYAN "--- Customer Management ---" RESET "\n");
                    printf("1. Add Customer\n");
                    printf("2. Edit Customer\n");
                    printf("3. Delete Customer\n");
                    printf("4. Search Customer\n");
                    printf("5. View All Customers\n");
                    printf("6. View Customer Jobs\n");
                    printf("7. View Customer Vehicles\n");
                    printf("8. Send Feedback Request\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter choice: " RESET);
                    scanf("%d", &choice);
                    clearInputBuffer();
                    switch (choice) {
                        case 1: addCustomer(); break;
                        case 2: editCustomer(); break;
                        case 3: deleteCustomer(); break;
                        case 4: searchCustomer(); break;
                        case 5: viewAllCustomers(); break;
                        case 6: { char custId[10]; printf("Enter Customer ID: "); scanf("%s", custId); clearInputBuffer(); viewCustomerJobs(custId); } break;
                        case 7: { char custId[10]; printf("Enter Customer ID: "); scanf("%s", custId); clearInputBuffer(); viewCustomerVehicles(custId); } break;
                        case 8: sendFeedbackRequest(); break;
                    }
                } while (choice != 0);
                break;
            case 3:
                // Vehicle Management Sub-menu
                do {
                    printf("\n" CYAN "--- Vehicle Management ---" RESET "\n");
                    printf("1. Add Vehicle\n");
                    printf("2. Edit Vehicle\n");
                    printf("3. Delete Vehicle\n");
                    printf("4. Search Vehicle\n");
                    printf("5. View All Vehicles\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter choice: " RESET);
                    scanf("%d", &choice);
                    clearInputBuffer();
                    switch (choice) {
                        case 1: addVehicle(); break;
                        case 2: editVehicle(); break;
                        case 3: deleteVehicle(); break;
                        case 4: searchVehicle(); break;
                        case 5: viewAllVehicles(); break;
                    }
                } while (choice != 0);
                break;
            case 4:
                // Job Management Sub-menu
                do {
                    printf("\n" CYAN "--- Job Management ---" RESET "\n");
                    printf("1. Add New Job\n");
                    printf("2. Update Job Status\n");
                    printf("3. View All Jobs\n");
                    printf("4. View Pending Jobs\n");
                    printf("5. View Completed Jobs\n");
                    printf("6. Search Job\n");
                    printf("7. Update Job Labor Cost\n");
                    printf("8. Add Part to Job\n");
                    printf("9. Generate Invoice\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter choice: " RESET);
                    scanf("%d", &choice);
                    clearInputBuffer();
                    switch (choice) {
                        case 1: addJob(uid, "Admin"); break;
                        case 2: updateJobStatus(uid, "Admin"); break;
                        case 3: viewAllJobs(); break;
                        case 4: viewPendingJobs(); break;
                        case 5: viewCompletedJobs(); break;
                        case 6: searchJob(); break;
                        case 7: updateJobLaborCost(); break;
                        case 8: addPartToJob(); break;
                        case 9: generateInvoice(); break;
                    }
                } while (choice != 0);
                break;
            case 5:
                // Inventory Management Sub-menu
                do {
                    printf("\n" CYAN "--- Inventory Management ---" RESET "\n");
                    printf("1. Add Part\n");
                    printf("2. Edit Part\n");
                    printf("3. View Inventory\n");
                    printf("4. View Reorder List\n");
                    printf("5. Clear Reorder List\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter choice: " RESET);
                    scanf("%d", &choice);
                    clearInputBuffer();
                    switch (choice) {
                        case 1: addPart(); break;
                        case 2: editPart(); break;
                        case 3: viewInventory(); break;
                        case 4: viewReorderList(); break;
                        case 5: clearReorderList(); break;
                    }
                } while (choice != 0);
                break;
            case 6:
                // Supplier Management Sub-menu
                do {
                    printf("\n" CYAN "--- Supplier Management ---" RESET "\n");
                    printf("1. Add Supplier\n");
                    printf("2. View All Suppliers\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter choice: " RESET);
                    scanf("%d", &choice);
                    clearInputBuffer();
                    switch (choice) {
                        case 1: addSupplier(); break;
                        case 2: viewAllSuppliers(); break;
                    }
                } while (choice != 0);
                break;
            case 7:
                // Appointment Management Sub-menu
                do {
                    printf("\n" CYAN "--- Appointment Management ---" RESET "\n");
                    printf("1. Schedule Appointment\n");
                    printf("2. View All Appointments\n");
                    printf("3. Schedule Service Reminder\n");
                    printf("4. View All Reminders\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter choice: " RESET);
                    scanf("%d", &choice);
                    clearInputBuffer();
                    switch (choice) {
                        case 1: scheduleAppointment(uid, "Admin"); break;
                        case 2: viewAllAppointments(); break;
                        case 3: scheduleServiceReminder(); break;
                        case 4: viewAllReminders(); break;
                    }
                } while (choice != 0);
                break;
            case 8:
                // Financial Management Sub-menu
                do {
                    printf("\n" CYAN "--- Financial Management ---" RESET "\n");
                    printf("1. Add Expense\n");
                    printf("2. View All Expenses\n");
                    printf("3. Generate Profit & Loss Report\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter choice: " RESET);
                    scanf("%d", &choice);
                    clearInputBuffer();
                    switch (choice) {
                        case 1: addExpense(); break;
                        case 2: viewAllExpenses(); break;
                        case 3: generateProfitLossReport(); break;
                    }
                } while (choice != 0);
                break;
            case 9:
                // Reporting Sub-menu
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
                    scanf("%d", &choice);
                    clearInputBuffer();
                    switch (choice) {
                        case 1: viewJobSummaryReport(); break;
                        case 2: mechanicPerformanceReport(); break;
                        case 3: customerLifetimeValueReport(); break;
                        case 4: monthlyRevenueReport(); break;
                        case 5: partUsageReport(); break;
                        case 6: supplierPerformanceReport(); break;
                        case 7: customerRetentionReport(); break;
                    }
                } while (choice != 0);
                break;
            case 10: backupData(); break;
            case 11: restoreData(); break;
            case 12: viewNotifications(uid); break;
            case 13: changePassword(uid); break;
            case 0:
                printf(GREEN "Logging out...\n" RESET);
                break;
            default:
                printf(RED "Invalid choice. Please try again.\n" RESET);
                pauseScreen();
        }
    } while (choice != 0);
}

// Senior Mechanic Menu
void seniorMechanicMenu(const char *uid) {
    int choice;
    do {
        printf("\n" MAGENTA "--- Senior Mechanic Menu ---" RESET "\n");
        printf("1. Add New Job\n");
        printf("2. Update Job Status\n");
        printf("3. View My Jobs\n");
        printf("4. View Pending Jobs\n");
        printf("5. Add Part to Job\n");
        printf("6. Update Job Labor Cost\n");
        printf("7. View Inventory\n");
        printf("8. View All Customers\n");
        printf("9. View All Vehicles\n");
        printf("10. View Notifications\n");
        printf("11. Change My Password\n");
        printf("0. Logout\n");
        printf(YELLOW "Enter choice: " RESET);
        scanf("%d", &choice);
        clearInputBuffer();

        switch(choice) {
            case 1: addJob(uid, "SeniorMechanic"); break;
            case 2: updateJobStatus(uid, "SeniorMechanic"); break;
            case 3: viewMyJobs(uid); break;
            case 4: viewPendingJobs(); break;
            case 5: addPartToJob(); break;
            case 6: updateJobLaborCost(); break;
            case 7: viewInventory(); break;
            case 8: viewAllCustomers(); break;
            case 9: viewAllVehicles(); break;
            case 10: viewNotifications(uid); break;
            case 11: changePassword(uid); break;
            case 0:
                printf(GREEN "Logging out...\n" RESET);
                break;
            default:
                printf(RED "Invalid choice. Please try again.\n" RESET);
                pauseScreen();
        }
    } while (choice != 0);
}

// Junior Mechanic Menu
void juniorMechanicMenu(const char *uid) {
    int choice;
    do {
        printf("\n" MAGENTA "--- Junior Mechanic Menu ---" RESET "\n");
        printf("1. View My Jobs\n");
        printf("2. Update Job Status\n");
        printf("3. Add Part to Job\n");
        printf("4. Update Job Labor Cost\n");
        printf("5. View Inventory\n");
        printf("6. View Notifications\n");
        printf("7. Change My Password\n");
        printf("0. Logout\n");
        printf(YELLOW "Enter choice: " RESET);
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: viewMyJobs(uid); break;
            case 2: updateJobStatus(uid, "JuniorMechanic"); break;
            case 3: addPartToJob(); break;
            case 4: updateJobLaborCost(); break;
            case 5: viewInventory(); break;
            case 6: viewNotifications(uid); break;
            case 7: changePassword(uid); break;
            case 0:
                printf(GREEN "Logging out...\n" RESET);
                break;
            default:
                printf(RED "Invalid choice. Please try again.\n" RESET);
                pauseScreen();
        }
    } while (choice != 0);
}

// Customer Menu
void customerMenu(const char *uid) {
    int choice;
    do {
        printf("\n" MAGENTA "--- Customer Menu ---" RESET "\n");
        printf("1. Schedule New Appointment\n");
        printf("2. View My Jobs\n");
        printf("3. View My Vehicles\n");
        printf("4. View Notifications\n");
        printf("5. Change My Password\n");
        printf("0. Logout\n");
        printf(YELLOW "Enter choice: " RESET);
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: scheduleAppointment(uid, "Customer"); break;
            case 2: viewCustomerJobs(uid); break;
            case 3: viewCustomerVehicles(uid); break;
            case 4: viewNotifications(uid); break;
            case 5: changePassword(uid); break;
            case 0:
                printf(GREEN "Logging out...\n" RESET);
                break;
            default:
                printf(RED "Invalid choice. Please try again.\n" RESET);
                pauseScreen();
        }
    } while (choice != 0);
}

// Function to print the ASCII art logo
void printLogo() {
    printf(CYAN "\n");
    printf("  ______ __  __ ____  \n");
    printf(" |  ____|  \\/  / __ \\ \n");
    printf(" | |__  | \\  / | |  | |\n");
    printf(" |  __| | |\\/| | |  | |\n");
    printf(" | |____| |  | | |__| |\n");
    printf(" |______|_|  |_|\\____/ \n");
    printf(RESET "\n");
}


// Main Function
int main() {
    int ch;
    char role[20], uid[10];
    
    // Check if any admin user exists
    FILE *fp = fopen(USER_FILE, "r");
    int hasAdmin = 0;
    if (fp) {
        User u;
        char line[100];
        while (fgets(line, sizeof(line), fp)) {
            sscanf(line, "%*[^,],%*[^,],%*[^,],%19[^\n]", u.role);
            if (strcmp(u.role, "Admin") == 0) {
                hasAdmin = 1;
                break;
            }
        }
        fclose(fp);
    }

    do {
        system("cls || clear");
        printLogo();
        printf(GREEN "Welcome to the Garage Management System\n\n" RESET);
        
        if (!hasAdmin) {
            printf(YELLOW "No users found. Please register an admin first.\n" RESET);
            printf("1. Register Admin\n");
            printf("0. Exit\n");
            printf(YELLOW "Enter choice: " RESET);
            scanf("%d", &ch);

            if (ch == 1) {
                printf("\n" GREEN "Register Admin Account:\n" RESET);
                registerUser();
                hasAdmin = 1; // Assume admin is successfully registered for next loop
            } else if (ch == 0) {
                break;
            } else {
                printf(RED "Invalid choice. Please try again.\n" RESET);
                pauseScreen();
            }
        } else {
            printf("1. Login\n");
            printf("2. Register New User\n");
            printf("0. Exit\n");
            printf(YELLOW "Enter choice: " RESET);
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
    } while (ch != 0);

    return 0;
}
