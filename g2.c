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
        // Safe parsing with strtok
        char* token;
        char temp_line[500];
        strcpy(temp_line, line);
        
        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(j.id, token, sizeof(j.id)-1); j.id[sizeof(j.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.customerId, token, sizeof(j.customerId)-1); j.customerId[sizeof(j.customerId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.vehicleId, token, sizeof(j.vehicleId)-1); j.vehicleId[sizeof(j.vehicleId)-1] = '\0';

        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.mechanicId, token, sizeof(j.mechanicId)-1); j.mechanicId[sizeof(j.mechanicId)-1] = '\0';

        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.description, token, sizeof(j.description)-1); j.description[sizeof(j.description)-1] = '\0';

        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.status, token, sizeof(j.status)-1); j.status[sizeof(j.status)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; j.estimatedCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.laborCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.partsCost = atof(token);

        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.notes, token, sizeof(j.notes)-1); j.notes[sizeof(j.notes)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.startDate, token, sizeof(j.startDate)-1); j.startDate[sizeof(j.startDate)-1] = '\0';

        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(j.endDate, token, sizeof(j.endDate)-1); j.endDate[sizeof(j.endDate)-1] = '\0';
        
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
            // Safely parse the job data
            char temp_line[500];
            strcpy(temp_line, line);
            char* token;

            token = strtok(temp_line, ",");
            if (!token) continue; strncpy(j.id, token, sizeof(j.id)-1); j.id[sizeof(j.id)-1] = '\0';
            
            token = strtok(NULL, ",");
            if (!token) continue; strncpy(j.customerId, token, sizeof(j.customerId)-1); j.customerId[sizeof(j.customerId)-1] = '\0';
            
            token = strtok(NULL, ",");
            if (!token) continue; strncpy(j.vehicleId, token, sizeof(j.vehicleId)-1); j.vehicleId[sizeof(j.vehicleId)-1] = '\0';
            
            token = strtok(NULL, ",");
            if (!token) continue; strncpy(j.mechanicId, token, sizeof(j.mechanicId)-1); j.mechanicId[sizeof(j.mechanicId)-1] = '\0';

            token = strtok(NULL, ",");
            if (!token) continue; strncpy(j.description, token, sizeof(j.description)-1); j.description[sizeof(j.description)-1] = '\0';
            
            token = strtok(NULL, ",");
            if (!token) continue; strncpy(j.status, token, sizeof(j.status)-1); j.status[sizeof(j.status)-1] = '\0';

            token = strtok(NULL, ",");
            if (!token) continue; j.estimatedCost = atof(token);
            
            token = strtok(NULL, ",");
            if (!token) continue; j.laborCost = atof(token);
            
            token = strtok(NULL, ",");
            if (!token) continue; j.partsCost = atof(token);
            
            token = strtok(NULL, ",");
            if (!token) continue; strncpy(j.notes, token, sizeof(j.notes)-1); j.notes[sizeof(j.notes)-1] = '\0';

            token = strtok(NULL, ",");
            if (!token) continue; strncpy(j.startDate, token, sizeof(j.startDate)-1); j.startDate[sizeof(j.startDate)-1] = '\0';

            token = strtok(NULL, "\n");
            if (!token) continue; strncpy(j.endDate, token, sizeof(j.endDate)-1); j.endDate[sizeof(j.endDate)-1] = '\0';
            
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
        // Safely parse the notification data
        char temp_line[300];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(n.id, token, sizeof(n.id)-1); n.id[sizeof(n.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(n.recipientId, token, sizeof(n.recipientId)-1); n.recipientId[sizeof(n.recipientId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(n.message, token, sizeof(n.message)-1); n.message[sizeof(n.message)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(n.date, token, sizeof(n.date)-1); n.date[sizeof(n.date)-1] = '\0';

        token = strtok(NULL, "\n");
        if (!token) continue; n.isRead = atoi(token);

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
            // Safely parse user data
            char temp_line[200];
            strcpy(temp_line, line);
            char* token;

            token = strtok(temp_line, ",");
            if (!token) continue; strncpy(u.id, token, sizeof(u.id)-1); u.id[sizeof(u.id)-1] = '\0';
            
            token = strtok(NULL, ",");
            if (!token) continue; strncpy(u.username, token, sizeof(u.username)-1); u.username[sizeof(u.username)-1] = '\0';
            
            token = strtok(NULL, ",");
            if (!token) continue; strncpy(u.password, token, sizeof(u.password)-1); u.password[sizeof(u.password)-1] = '\0';
            
            token = strtok(NULL, "\n");
            if (!token) continue; strncpy(u.role, token, sizeof(u.role)-1); u.role[sizeof(u.role)-1] = '\0';
            
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
        char temp_line[200];
        strcpy(temp_line, buf);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(temp_u.id, token, sizeof(temp_u.id)-1); temp_u.id[sizeof(temp_u.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(temp_u.username, token, sizeof(temp_u.username)-1); temp_u.username[sizeof(temp_u.username)-1] = '\0';

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
        // Safely parse user data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(arr[c].id, token, sizeof(arr[c].id)-1); arr[c].id[sizeof(arr[c].id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(arr[c].username, token, sizeof(arr[c].username)-1); arr[c].username[sizeof(arr[c].username)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(arr[c].password, token, sizeof(arr[c].password)-1); arr[c].password[sizeof(arr[c].password)-1] = '\0';
        
        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(arr[c].role, token, sizeof(arr[c].role)-1); arr[c].role[sizeof(arr[c].role)-1] = '\0';

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
    for (int i = 0; i < c; i++) fprintf(fp, "%s,%s,%s,%s\n", arr[i].id, arr[i].username, arr[i].password, arr[i].role);
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
        // Safely parse user data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(u.id, token, sizeof(u.id)-1); u.id[sizeof(u.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(u.username, token, sizeof(u.username)-1); u.username[sizeof(u.username)-1] = '\0';
        
        token = strtok(NULL, ","); // Skip password
        
        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(u.role, token, sizeof(u.role)-1); u.role[sizeof(u.role)-1] = '\0';
        
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
        // Safely parse user data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(arr[c].id, token, sizeof(arr[c].id)-1); arr[c].id[sizeof(arr[c].id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(arr[c].username, token, sizeof(arr[c].username)-1); arr[c].username[sizeof(arr[c].username)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(arr[c].password, token, sizeof(arr[c].password)-1); arr[c].password[sizeof(arr[c].password)-1] = '\0';
        
        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(arr[c].role, token, sizeof(arr[c].role)-1); arr[c].role[sizeof(arr[c].role)-1] = '\0';

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
    for (int i = 0; i < c; i++) fprintf(fp, "%s,%s,%s,%s\n", arr[i].id, arr[i].username, arr[i].password, arr[i].role);
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
        // Safely parse customer data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(c.id, token, sizeof(c.id)-1); c.id[sizeof(c.id)-1] = '\0';

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
        // Safely parse customer data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(arr[c].id, token, sizeof(arr[c].id)-1); arr[c].id[sizeof(arr[c].id)-1] = '\0';

        token = strtok(NULL, ",");
        if (!token) continue; strncpy(arr[c].name, token, sizeof(arr[c].name)-1); arr[c].name[sizeof(arr[c].name)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(arr[c].phone, token, sizeof(arr[c].phone)-1); arr[c].phone[sizeof(arr[c].phone)-1] = '\0';
        
        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(arr[c].email, token, sizeof(arr[c].email)-1); arr[c].email[sizeof(arr[c].email)-1] = '\0';

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
    for (int i = 0; i < c; i++) fprintf(fp, "%s,%s,%s,%s\n", arr[i].id, arr[i].name, arr[i].phone, arr[i].email);
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
        fclose(fp_read);
        printf(RED "Error creating temporary file.\n" RESET);
        return;
    }

    char id[10];
    printf(YELLOW "\nEnter Customer ID to delete: " RESET);
    scanf("%9s", id);

    Customer c;
    char line[200];
    int found = 0;
    while (fgets(line, sizeof(line), fp_read)) {
        // Safely parse customer data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(c.id, token, sizeof(c.id)-1); c.id[sizeof(c.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(c.name, token, sizeof(c.name)-1); c.name[sizeof(c.name)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(c.phone, token, sizeof(c.phone)-1); c.phone[sizeof(c.phone)-1] = '\0';
        
        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(c.email, token, sizeof(c.email)-1); c.email[sizeof(c.email)-1] = '\0';

        if (strcmp(c.id, id) == 0) {
            found = 1;
            continue; // Skip writing this line to the new file
        }
        fprintf(fp_write, "%s,%s,%s,%s\n", c.id, c.name, c.phone, c.email);
    }
    fclose(fp_read);
    fclose(fp_write);

    if (found) {
        remove(CUSTOMER_FILE);
        rename("temp_customers.txt", CUSTOMER_FILE);
        printf(GREEN "Customer with ID %s deleted successfully.\n" RESET, id);
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
    
    Customer c;
    char line[200];
    int found = 0;
    
    printf("\n" CYAN "--- Search Results ---" RESET "\n");
    printf("ID,Name,Phone,Email\n");
    while (fgets(line, sizeof(line), fp)) {
        // Safely parse customer data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(c.id, token, sizeof(c.id)-1); c.id[sizeof(c.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(c.name, token, sizeof(c.name)-1); c.name[sizeof(c.name)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(c.phone, token, sizeof(c.phone)-1); c.phone[sizeof(c.phone)-1] = '\0';
        
        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(c.email, token, sizeof(c.email)-1); c.email[sizeof(c.email)-1] = '\0';

        if (strstr(c.name, search_term) || strstr(c.id, search_term)) {
            printf("%s,%s,%s,%s\n", c.id, c.name, c.phone, c.email);
            found = 1;
        }
    }
    fclose(fp);
    if (!found) {
        printf(YELLOW "No customers found matching the search term.\n" RESET);
    }
    pauseScreen();
}

// Views all customers in the system
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
        // Safely parse customer data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(c.id, token, sizeof(c.id)-1); c.id[sizeof(c.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(c.name, token, sizeof(c.name)-1); c.name[sizeof(c.name)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(c.phone, token, sizeof(c.phone)-1); c.phone[sizeof(c.phone)-1] = '\0';
        
        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(c.email, token, sizeof(c.email)-1); c.email[sizeof(c.email)-1] = '\0';
        
        printf("%s,%s,%s,%s\n", c.id, c.name, c.phone, c.email);
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
        sprintf(message, "Dear customer, we would love to hear your feedback on our recent service. Please reply to this message with your thoughts.");
        createNotification(customerId, message);
        printf(GREEN "Feedback request sent to customer %s.\n" RESET, customerId);
        char log_msg[100];
        sprintf(log_msg, "Feedback request sent to customer ID %s.", customerId);
        writeLog(log_msg);
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
    int cnt = fileLineCount(VEHICLE_FILE);
    printf(YELLOW "Enter vehicle brand: " RESET);
    scanf("%29s", v.brand);
    printf(YELLOW "Enter vehicle model: " RESET);
    scanf("%29s", v.model);
    printf(YELLOW "Enter license plate: " RESET);
    scanf("%14s", v.plate);
    printf(YELLOW "Enter year: " RESET);
    scanf("%4s", v.year);
    strcpy(v.lastServiceDate, "N/A"); // Initial service date
    generateId("VEH", cnt + 1, v.id);
    fprintf(fp, "%s,%s,%s,%s,%s,%s,%s\n", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
    fclose(fp);
    printf(GREEN "Vehicle added successfully! ID: %s\n" RESET, v.id);
    char log_msg[100];
    sprintf(log_msg, "New vehicle %s %s added for customer %s with ID %s.", v.brand, v.model, v.customerId, v.id);
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
        // Safely parse vehicle data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(v.id, token, sizeof(v.id)-1); v.id[sizeof(v.id)-1] = '\0';
        
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
        // Safely parse vehicle data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;
        
        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(arr[c].id, token, sizeof(arr[c].id)-1); arr[c].id[sizeof(arr[c].id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(arr[c].customerId, token, sizeof(arr[c].customerId)-1); arr[c].customerId[sizeof(arr[c].customerId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(arr[c].brand, token, sizeof(arr[c].brand)-1); arr[c].brand[sizeof(arr[c].brand)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(arr[c].model, token, sizeof(arr[c].model)-1); arr[c].model[sizeof(arr[c].model)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(arr[c].plate, token, sizeof(arr[c].plate)-1); arr[c].plate[sizeof(arr[c].plate)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(arr[c].year, token, sizeof(arr[c].year)-1); arr[c].year[sizeof(arr[c].year)-1] = '\0';

        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(arr[c].lastServiceDate, token, sizeof(arr[c].lastServiceDate)-1); arr[c].lastServiceDate[sizeof(arr[c].lastServiceDate)-1] = '\0';
        
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
            printf("Current License Plate: %s\n", arr[i].plate);
            printf(YELLOW "Enter new license plate (or press Enter to keep current): " RESET);
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
    for (int i = 0; i < c; i++) fprintf(fp, "%s,%s,%s,%s,%s,%s,%s\n", arr[i].id, arr[i].customerId, arr[i].brand, arr[i].model, arr[i].plate, arr[i].year, arr[i].lastServiceDate);
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
        fclose(fp_read);
        printf(RED "Error creating temporary file.\n" RESET);
        return;
    }

    char id[10];
    printf(YELLOW "\nEnter Vehicle ID to delete: " RESET);
    scanf("%9s", id);

    Vehicle v;
    char line[200];
    int found = 0;
    while (fgets(line, sizeof(line), fp_read)) {
        // Safely parse vehicle data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(v.id, token, sizeof(v.id)-1); v.id[sizeof(v.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.customerId, token, sizeof(v.customerId)-1); v.customerId[sizeof(v.customerId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.brand, token, sizeof(v.brand)-1); v.brand[sizeof(v.brand)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.model, token, sizeof(v.model)-1); v.model[sizeof(v.model)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.plate, token, sizeof(v.plate)-1); v.plate[sizeof(v.plate)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.year, token, sizeof(v.year)-1); v.year[sizeof(v.year)-1] = '\0';

        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(v.lastServiceDate, token, sizeof(v.lastServiceDate)-1); v.lastServiceDate[sizeof(v.lastServiceDate)-1] = '\0';

        if (strcmp(v.id, id) == 0) {
            found = 1;
            continue; // Skip writing this line to the new file
        }
        fprintf(fp_write, "%s,%s,%s,%s,%s,%s,%s\n", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
    }
    fclose(fp_read);
    fclose(fp_write);

    if (found) {
        remove(VEHICLE_FILE);
        rename("temp_vehicles.txt", VEHICLE_FILE);
        printf(GREEN "Vehicle with ID %s deleted successfully.\n" RESET, id);
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
    printf(YELLOW "\nEnter vehicle license plate or ID to search: " RESET);
    scanf("%14s", search_term);
    
    Vehicle v;
    char line[200];
    int found = 0;
    
    printf("\n" CYAN "--- Search Results ---" RESET "\n");
    printf("ID,Customer ID,Brand,Model,Plate,Year,Last Service\n");
    while (fgets(line, sizeof(line), fp)) {
        // Safely parse vehicle data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(v.id, token, sizeof(v.id)-1); v.id[sizeof(v.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.customerId, token, sizeof(v.customerId)-1); v.customerId[sizeof(v.customerId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.brand, token, sizeof(v.brand)-1); v.brand[sizeof(v.brand)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.model, token, sizeof(v.model)-1); v.model[sizeof(v.model)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.plate, token, sizeof(v.plate)-1); v.plate[sizeof(v.plate)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.year, token, sizeof(v.year)-1); v.year[sizeof(v.year)-1] = '\0';

        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(v.lastServiceDate, token, sizeof(v.lastServiceDate)-1); v.lastServiceDate[sizeof(v.lastServiceDate)-1] = '\0';
        
        if (strstr(v.plate, search_term) || strstr(v.id, search_term)) {
            printf("%s,%s,%s,%s,%s,%s,%s\n", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
            found = 1;
        }
    }
    fclose(fp);
    if (!found) {
        printf(YELLOW "No vehicles found matching the search term.\n" RESET);
    }
    pauseScreen();
}

// Views all vehicles in the system
void viewAllVehicles() {
    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) {
        printf(RED "No vehicles found!\n" RESET);
        return;
    }
    char line[200];
    printf("\n" CYAN "--- All Vehicles ---" RESET "\n");
    printf("ID,Customer ID,Brand,Model,Plate,Year,Last Service\n");
    Vehicle v;
    while (fgets(line, sizeof(line), fp)) {
        // Safely parse vehicle data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(v.id, token, sizeof(v.id)-1); v.id[sizeof(v.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.customerId, token, sizeof(v.customerId)-1); v.customerId[sizeof(v.customerId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.brand, token, sizeof(v.brand)-1); v.brand[sizeof(v.brand)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.model, token, sizeof(v.model)-1); v.model[sizeof(v.model)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.plate, token, sizeof(v.plate)-1); v.plate[sizeof(v.plate)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.year, token, sizeof(v.year)-1); v.year[sizeof(v.year)-1] = '\0';

        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(v.lastServiceDate, token, sizeof(v.lastServiceDate)-1); v.lastServiceDate[sizeof(v.lastServiceDate)-1] = '\0';
        
        printf("%s,%s,%s,%s,%s,%s,%s\n", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
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
    char line[200];
    printf("\n" CYAN "--- Vehicles for Customer %s ---" RESET "\n", customerId);
    printf("ID,Brand,Model,Plate,Year,Last Service\n");
    Vehicle v;
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        // Safely parse vehicle data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;
        
        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(v.id, token, sizeof(v.id)-1); v.id[sizeof(v.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.customerId, token, sizeof(v.customerId)-1); v.customerId[sizeof(v.customerId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.brand, token, sizeof(v.brand)-1); v.brand[sizeof(v.brand)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.model, token, sizeof(v.model)-1); v.model[sizeof(v.model)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.plate, token, sizeof(v.plate)-1); v.plate[sizeof(v.plate)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.year, token, sizeof(v.year)-1); v.year[sizeof(v.year)-1] = '\0';
        
        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(v.lastServiceDate, token, sizeof(v.lastServiceDate)-1); v.lastServiceDate[sizeof(v.lastServiceDate)-1] = '\0';
        
        if (strcmp(v.customerId, customerId) == 0) {
            printf("%s,%s,%s,%s,%s,%s\n", v.id, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
            found = 1;
        }
    }
    fclose(fp);
    if (!found) {
        printf(YELLOW "No vehicles found for this customer.\n" RESET);
    }
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
        // Safely parse user data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(u.id, token, sizeof(u.id)-1); u.id[sizeof(u.id)-1] = '\0';

        token = strtok(NULL, ",");
        token = strtok(NULL, ",");
        
        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(u.role, token, sizeof(u.role)-1); u.role[sizeof(u.role)-1] = '\0';

        if (!strcmp(u.id, id) && (strcmp(u.role, "JuniorMechanic") == 0 || strcmp(u.role, "SeniorMechanic") == 0)) {
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
    char customerId[10], vehicleId[10], mechanicId[10];

    // Get customer ID
    printf(YELLOW "\nEnter Customer ID for the job: " RESET);
    scanf("%9s", customerId);
    if (!customerExists(customerId)) {
        printf(RED "Customer ID not found.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    }
    strcpy(j.customerId, customerId);

    // Get vehicle ID
    printf(YELLOW "Enter Vehicle ID for the job: " RESET);
    scanf("%9s", vehicleId);
    if (!vehicleExists(vehicleId)) {
        printf(RED "Vehicle ID not found.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    }
    strcpy(j.vehicleId, vehicleId);

    // Get mechanic ID
    if (getRoleLevel(currentUserRole) >= 2) { // Admin or SeniorMechanic
        printf(YELLOW "Enter Mechanic ID to assign (or 'self' to assign to yourself): " RESET);
        scanf("%9s", mechanicId);
        if (strcmp(mechanicId, "self") == 0) {
            strcpy(j.mechanicId, currentUserId);
        } else if (!mechanicExists(mechanicId)) {
            printf(RED "Mechanic ID not found.\n" RESET);
            fclose(fp);
            pauseScreen();
            return;
        } else {
            strcpy(j.mechanicId, mechanicId);
        }
    } else {
        // JuniorMechanic can only assign to themselves
        strcpy(j.mechanicId, currentUserId);
        printf(GREEN "Job assigned to you (%s).\n" RESET, currentUserId);
    }

    int cnt = fileLineCount(JOB_FILE);
    generateId("JOB", cnt + 1, j.id);
    
    printf(YELLOW "Enter job description: " RESET);
    clearInputBuffer();
    fgets(j.description, sizeof(j.description), stdin);
    j.description[strcspn(j.description, "\n")] = '\0';
    
    // Auto-calculate estimated cost
    j.estimatedCost = calculateAverageCost(j.description);
    if (j.estimatedCost > 0) {
        printf(CYAN "Based on past jobs, estimated cost is: %.2f\n" RESET, j.estimatedCost);
    } else {
        printf(YELLOW "Could not find a similar past job to estimate cost. Please enter manually.\n" RESET);
        printf(YELLOW "Enter estimated cost: " RESET);
        scanf("%f", &j.estimatedCost);
        clearInputBuffer();
    }

    j.laborCost = 0.0;
    j.partsCost = 0.0;
    strcpy(j.status, "Pending");
    strcpy(j.notes, "N/A");
    getCurrentDate(j.startDate);
    strcpy(j.endDate, "N/A");

    fprintf(fp, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n",
            j.id, j.customerId, j.vehicleId, j.mechanicId, j.description,
            j.status, j.estimatedCost, j.laborCost, j.partsCost, j.notes,
            j.startDate, j.endDate);
    fclose(fp);
    printf(GREEN "Job added successfully! ID: %s\n" RESET, j.id);
    char log_msg[100];
    sprintf(log_msg, "New job %s added for customer %s.", j.id, j.customerId);
    writeLog(log_msg);
    pauseScreen();
}

// Updates the status of a job
void updateJobStatus(const char *currentUserId, const char *currentUserRole) {
    FILE *fp_read = fopen(JOB_FILE, "r");
    if (!fp_read) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }
    FILE *fp_write = fopen("temp_jobs.txt", "w");
    if (!fp_write) {
        fclose(fp_read);
        printf(RED "Error creating temporary file.\n" RESET);
        return;
    }

    char jobId[10];
    printf(YELLOW "\nEnter Job ID to update status: " RESET);
    scanf("%9s", jobId);

    Job j;
    char line[500];
    int found = 0;
    while (fgets(line, sizeof(line), fp_read)) {
        // Safely parse job data
        char temp_line[500];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(j.id, token, sizeof(j.id)-1); j.id[sizeof(j.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.customerId, token, sizeof(j.customerId)-1); j.customerId[sizeof(j.customerId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.vehicleId, token, sizeof(j.vehicleId)-1); j.vehicleId[sizeof(j.vehicleId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.mechanicId, token, sizeof(j.mechanicId)-1); j.mechanicId[sizeof(j.mechanicId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.description, token, sizeof(j.description)-1); j.description[sizeof(j.description)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.status, token, sizeof(j.status)-1); j.status[sizeof(j.status)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; j.estimatedCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.laborCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.partsCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.notes, token, sizeof(j.notes)-1); j.notes[sizeof(j.notes)-1] = '\0';

        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.startDate, token, sizeof(j.startDate)-1); j.startDate[sizeof(j.startDate)-1] = '\0';

        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(j.endDate, token, sizeof(j.endDate)-1); j.endDate[sizeof(j.endDate)-1] = '\0';
        
        if (strcmp(j.id, jobId) == 0) {
            // Check if user is authorized to update this job
            if (getRoleLevel(currentUserRole) < 2 && strcmp(j.mechanicId, currentUserId) != 0) {
                printf(RED "You are not authorized to update this job.\n" RESET);
                fprintf(fp_write, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", j.id, j.customerId, j.vehicleId,
                    j.mechanicId, j.description, j.status, j.estimatedCost, j.laborCost, j.partsCost,
                    j.notes, j.startDate, j.endDate);
                continue;
            }
            
            found = 1;
            char newStatus[20];
            printf("Current Status: %s\n", j.status);
            printf(YELLOW "Enter new status (Pending/InProgress/Completed): " RESET);
            scanf("%19s", newStatus);
            if (strcmp(newStatus, "Pending") != 0 && strcmp(newStatus, "InProgress") != 0 && strcmp(newStatus, "Completed") != 0) {
                printf(RED "Invalid status. Status not updated.\n" RESET);
                fprintf(fp_write, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", j.id, j.customerId, j.vehicleId,
                    j.mechanicId, j.description, j.status, j.estimatedCost, j.laborCost, j.partsCost,
                    j.notes, j.startDate, j.endDate);
                continue;
            }
            strcpy(j.status, newStatus);
            if (strcmp(newStatus, "Completed") == 0) {
                getCurrentDate(j.endDate);
                char notification_msg[200];
                sprintf(notification_msg, "Your job with ID %s for vehicle %s is now completed.", j.id, j.vehicleId);
                createNotification(j.customerId, notification_msg);
            } else {
                strcpy(j.endDate, "N/A");
            }
        }
        fprintf(fp_write, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", j.id, j.customerId, j.vehicleId,
            j.mechanicId, j.description, j.status, j.estimatedCost, j.laborCost, j.partsCost,
            j.notes, j.startDate, j.endDate);
    }
    fclose(fp_read);
    fclose(fp_write);

    if (found) {
        remove(JOB_FILE);
        rename("temp_jobs.txt", JOB_FILE);
        printf(GREEN "Job status updated successfully!\n" RESET);
        char log_msg[100];
        sprintf(log_msg, "Status of job %s updated.", jobId);
        writeLog(log_msg);
    } else {
        remove("temp_jobs.txt");
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
    printf("\n" CYAN "--- Jobs assigned to you (%s) ---" RESET "\n", mechanicId);
    printf("ID,Customer ID,Vehicle ID,Description,Status,Estimated Cost,Labor Cost,Parts Cost,Notes,Start Date,End Date\n");
    Job j;
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        // Safely parse job data
        char temp_line[500];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(j.id, token, sizeof(j.id)-1); j.id[sizeof(j.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.customerId, token, sizeof(j.customerId)-1); j.customerId[sizeof(j.customerId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.vehicleId, token, sizeof(j.vehicleId)-1); j.vehicleId[sizeof(j.vehicleId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.mechanicId, token, sizeof(j.mechanicId)-1); j.mechanicId[sizeof(j.mechanicId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.description, token, sizeof(j.description)-1); j.description[sizeof(j.description)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.status, token, sizeof(j.status)-1); j.status[sizeof(j.status)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; j.estimatedCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.laborCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.partsCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.notes, token, sizeof(j.notes)-1); j.notes[sizeof(j.notes)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.startDate, token, sizeof(j.startDate)-1); j.startDate[sizeof(j.startDate)-1] = '\0';
        
        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(j.endDate, token, sizeof(j.endDate)-1); j.endDate[sizeof(j.endDate)-1] = '\0';

        if (strcmp(j.mechanicId, mechanicId) == 0) {
            printf("%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", j.id, j.customerId, j.vehicleId, j.description,
                   j.status, j.estimatedCost, j.laborCost, j.partsCost, j.notes, j.startDate, j.endDate);
            found = 1;
        }
    }
    fclose(fp);
    if (!found) {
        printf(YELLOW "No jobs found assigned to you.\n" RESET);
    }
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
    printf("\n" CYAN "--- Jobs for Customer %s ---" RESET "\n", customerId);
    printf("ID,Vehicle ID,Mechanic ID,Description,Status,Estimated Cost,Labor Cost,Parts Cost,Notes,Start Date,End Date\n");
    Job j;
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        // Safely parse job data
        char temp_line[500];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(j.id, token, sizeof(j.id)-1); j.id[sizeof(j.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.customerId, token, sizeof(j.customerId)-1); j.customerId[sizeof(j.customerId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.vehicleId, token, sizeof(j.vehicleId)-1); j.vehicleId[sizeof(j.vehicleId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.mechanicId, token, sizeof(j.mechanicId)-1); j.mechanicId[sizeof(j.mechanicId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.description, token, sizeof(j.description)-1); j.description[sizeof(j.description)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.status, token, sizeof(j.status)-1); j.status[sizeof(j.status)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; j.estimatedCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.laborCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.partsCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.notes, token, sizeof(j.notes)-1); j.notes[sizeof(j.notes)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.startDate, token, sizeof(j.startDate)-1); j.startDate[sizeof(j.startDate)-1] = '\0';
        
        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(j.endDate, token, sizeof(j.endDate)-1); j.endDate[sizeof(j.endDate)-1] = '\0';

        if (strcmp(j.customerId, customerId) == 0) {
            printf("%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", j.id, j.vehicleId, j.mechanicId, j.description,
                   j.status, j.estimatedCost, j.laborCost, j.partsCost, j.notes, j.startDate, j.endDate);
            found = 1;
        }
    }
    fclose(fp);
    if (!found) {
        printf(YELLOW "No jobs found for this customer.\n" RESET);
    }
    pauseScreen();
}

// Searches for a job by ID or description
void searchJob() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found.\n" RESET);
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
    printf("ID,Customer ID,Vehicle ID,Mechanic ID,Description,Status,Est. Cost,Labor Cost,Parts Cost,Notes,Start Date,End Date\n");
    while (fgets(line, sizeof(line), fp)) {
        // Safely parse job data
        char temp_line[500];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(j.id, token, sizeof(j.id)-1); j.id[sizeof(j.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.customerId, token, sizeof(j.customerId)-1); j.customerId[sizeof(j.customerId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.vehicleId, token, sizeof(j.vehicleId)-1); j.vehicleId[sizeof(j.vehicleId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.mechanicId, token, sizeof(j.mechanicId)-1); j.mechanicId[sizeof(j.mechanicId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.description, token, sizeof(j.description)-1); j.description[sizeof(j.description)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.status, token, sizeof(j.status)-1); j.status[sizeof(j.status)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; j.estimatedCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.laborCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.partsCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.notes, token, sizeof(j.notes)-1); j.notes[sizeof(j.notes)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.startDate, token, sizeof(j.startDate)-1); j.startDate[sizeof(j.startDate)-1] = '\0';
        
        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(j.endDate, token, sizeof(j.endDate)-1); j.endDate[sizeof(j.endDate)-1] = '\0';

        if (strstr(j.id, search_term) || strstr(j.description, search_term)) {
            printf("%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description,
                   j.status, j.estimatedCost, j.laborCost, j.partsCost, j.notes, j.startDate, j.endDate);
            found = 1;
        }
    }
    fclose(fp);
    if (!found) {
        printf(YELLOW "No jobs found matching the search term.\n" RESET);
    }
    pauseScreen();
}

// Views all jobs in the system
void viewAllJobs() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found!\n" RESET);
        return;
    }
    char line[500];
    printf("\n" CYAN "--- All Jobs ---" RESET "\n");
    printf("ID,Customer ID,Vehicle ID,Mechanic ID,Description,Status,Est. Cost,Labor Cost,Parts Cost,Notes,Start Date,End Date\n");
    Job j;
    while (fgets(line, sizeof(line), fp)) {
        // Safely parse job data
        char temp_line[500];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(j.id, token, sizeof(j.id)-1); j.id[sizeof(j.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.customerId, token, sizeof(j.customerId)-1); j.customerId[sizeof(j.customerId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.vehicleId, token, sizeof(j.vehicleId)-1); j.vehicleId[sizeof(j.vehicleId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.mechanicId, token, sizeof(j.mechanicId)-1); j.mechanicId[sizeof(j.mechanicId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.description, token, sizeof(j.description)-1); j.description[sizeof(j.description)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.status, token, sizeof(j.status)-1); j.status[sizeof(j.status)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; j.estimatedCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.laborCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.partsCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.notes, token, sizeof(j.notes)-1); j.notes[sizeof(j.notes)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.startDate, token, sizeof(j.startDate)-1); j.startDate[sizeof(j.startDate)-1] = '\0';
        
        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(j.endDate, token, sizeof(j.endDate)-1); j.endDate[sizeof(j.endDate)-1] = '\0';

        printf("%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description,
               j.status, j.estimatedCost, j.laborCost, j.partsCost, j.notes, j.startDate, j.endDate);
    }
    fclose(fp);
    pauseScreen();
}

// Views all jobs with a "Pending" status
void viewPendingJobs() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found!\n" RESET);
        return;
    }
    char line[500];
    printf("\n" CYAN "--- Pending Jobs ---" RESET "\n");
    printf("ID,Customer ID,Vehicle ID,Mechanic ID,Description,Est. Cost,Notes,Start Date\n");
    Job j;
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        // Safely parse job data
        char temp_line[500];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(j.id, token, sizeof(j.id)-1); j.id[sizeof(j.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.customerId, token, sizeof(j.customerId)-1); j.customerId[sizeof(j.customerId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.vehicleId, token, sizeof(j.vehicleId)-1); j.vehicleId[sizeof(j.vehicleId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.mechanicId, token, sizeof(j.mechanicId)-1); j.mechanicId[sizeof(j.mechanicId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.description, token, sizeof(j.description)-1); j.description[sizeof(j.description)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.status, token, sizeof(j.status)-1); j.status[sizeof(j.status)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; j.estimatedCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.laborCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.partsCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.notes, token, sizeof(j.notes)-1); j.notes[sizeof(j.notes)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.startDate, token, sizeof(j.startDate)-1); j.startDate[sizeof(j.startDate)-1] = '\0';

        if (strcmp(j.status, "Pending") == 0) {
            printf("%s,%s,%s,%s,%.2f,%s,%s\n", j.id, j.customerId, j.vehicleId, j.description, j.estimatedCost, j.notes, j.startDate);
            found = 1;
        }
    }
    fclose(fp);
    if (!found) {
        printf(YELLOW "No pending jobs found.\n" RESET);
    }
    pauseScreen();
}

// Views all jobs with a "Completed" status
void viewCompletedJobs() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found!\n" RESET);
        return;
    }
    char line[500];
    printf("\n" CYAN "--- Completed Jobs ---" RESET "\n");
    printf("ID,Customer ID,Vehicle ID,Mechanic ID,Description,Labor Cost,Parts Cost,End Date\n");
    Job j;
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        // Safely parse job data
        char temp_line[500];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(j.id, token, sizeof(j.id)-1); j.id[sizeof(j.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.customerId, token, sizeof(j.customerId)-1); j.customerId[sizeof(j.customerId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.vehicleId, token, sizeof(j.vehicleId)-1); j.vehicleId[sizeof(j.vehicleId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.mechanicId, token, sizeof(j.mechanicId)-1); j.mechanicId[sizeof(j.mechanicId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.description, token, sizeof(j.description)-1); j.description[sizeof(j.description)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.status, token, sizeof(j.status)-1); j.status[sizeof(j.status)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; j.estimatedCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.laborCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.partsCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.notes, token, sizeof(j.notes)-1); j.notes[sizeof(j.notes)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.startDate, token, sizeof(j.startDate)-1); j.startDate[sizeof(j.startDate)-1] = '\0';
        
        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(j.endDate, token, sizeof(j.endDate)-1); j.endDate[sizeof(j.endDate)-1] = '\0';

        if (strcmp(j.status, "Completed") == 0) {
            printf("%s,%s,%s,%s,%s,%.2f,%.2f,%s\n", j.id, j.customerId, j.vehicleId, j.mechanicId,
                   j.description, j.laborCost, j.partsCost, j.endDate);
            found = 1;
        }
    }
    fclose(fp);
    if (!found) {
        printf(YELLOW "No completed jobs found.\n" RESET);
    }
    pauseScreen();
}

// Invoice Management Functions
// Generates an invoice for a completed job
void generateInvoice() {
    char jobId[10];
    printf(YELLOW "\nEnter Job ID to generate an invoice for: " RESET);
    scanf("%9s", jobId);

    FILE *fp_job = fopen(JOB_FILE, "r");
    if (!fp_job) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }

    Job j;
    char line[500];
    int found = 0;
    while (fgets(line, sizeof(line), fp_job)) {
        // Safely parse job data
        char temp_line[500];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(j.id, token, sizeof(j.id)-1); j.id[sizeof(j.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.customerId, token, sizeof(j.customerId)-1); j.customerId[sizeof(j.customerId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.vehicleId, token, sizeof(j.vehicleId)-1); j.vehicleId[sizeof(j.vehicleId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.mechanicId, token, sizeof(j.mechanicId)-1); j.mechanicId[sizeof(j.mechanicId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.description, token, sizeof(j.description)-1); j.description[sizeof(j.description)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.status, token, sizeof(j.status)-1); j.status[sizeof(j.status)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; j.estimatedCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.laborCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.partsCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.notes, token, sizeof(j.notes)-1); j.notes[sizeof(j.notes)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.startDate, token, sizeof(j.startDate)-1); j.startDate[sizeof(j.startDate)-1] = '\0';
        
        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(j.endDate, token, sizeof(j.endDate)-1); j.endDate[sizeof(j.endDate)-1] = '\0';

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
        printf(RED "Invoice can only be generated for a completed job.\n" RESET);
        pauseScreen();
        return;
    }

    FILE *fp_invoice = fopen(INVOICE_FILE, "a+");
    if (!fp_invoice) {
        printf(RED "Error opening invoice file.\n" RESET);
        return;
    }

    Invoice inv;
    int cnt = fileLineCount(INVOICE_FILE);
    generateId("INV", cnt + 1, inv.id);
    strcpy(inv.jobId, j.id);
    inv.totalCost = j.laborCost + j.partsCost;
    
    printf(YELLOW "Enter discount amount (e.g., 10.00): " RESET);
    scanf("%f", &inv.discount);
    inv.total = inv.totalCost - inv.discount;
    getCurrentDate(inv.date);

    fprintf(fp_invoice, "%s,%s,%.2f,%.2f,%.2f,%s\n", inv.id, inv.jobId, inv.totalCost, inv.discount, inv.total, inv.date);
    fclose(fp_invoice);

    printf(GREEN "Invoice generated successfully! Invoice ID: %s\n" RESET, inv.id);
    printf(CYAN "--- INVOICE ---" RESET "\n");
    printf("Invoice ID: %s\n", inv.id);
    printf("Job ID: %s\n", inv.jobId);
    printf("Date: %s\n", inv.date);
    printf("Total Cost: %.2f\n", inv.totalCost);
    printf("Discount: %.2f\n", inv.discount);
    printf("Total Amount Due: %.2f\n", inv.total);
    printf("A copy has been sent to the customer via notification.\n");
    
    char notification_msg[200];
    sprintf(notification_msg, "A new invoice (ID: %s) for your job %s has been generated. Total amount due is %.2f.", inv.id, inv.jobId, inv.total);
    createNotification(j.customerId, notification_msg);

    char log_msg[100];
    sprintf(log_msg, "Invoice %s generated for job %s.", inv.id, inv.jobId);
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
    char line[200];
    printf("\n" CYAN "--- All Invoices ---" RESET "\n");
    printf("Invoice ID,Job ID,Total Cost,Discount,Total,Date\n");
    Invoice inv;
    while (fgets(line, sizeof(line), fp)) {
        // Safely parse invoice data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(inv.id, token, sizeof(inv.id)-1); inv.id[sizeof(inv.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(inv.jobId, token, sizeof(inv.jobId)-1); inv.jobId[sizeof(inv.jobId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; inv.totalCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; inv.discount = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; inv.total = atof(token);
        
        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(inv.date, token, sizeof(inv.date)-1); inv.date[sizeof(inv.date)-1] = '\0';
        
        printf("%s,%s,%.2f,%.2f,%.2f,%s\n", inv.id, inv.jobId, inv.totalCost, inv.discount, inv.total, inv.date);
    }
    fclose(fp);
    pauseScreen();
}

// Inventory Management Functions
// Adds a part to an existing job
void addPartToJob() {
    char jobId[10], partId[10];
    int quantity;
    printf(YELLOW "\nEnter Job ID to add a part to: " RESET);
    scanf("%9s", jobId);
    printf(YELLOW "Enter Part ID to add: " RESET);
    scanf("%9s", partId);
    printf(YELLOW "Enter quantity: " RESET);
    scanf("%d", &quantity);

    // Deduct from inventory and get price
    deductPartFromInventory(partId, quantity);
    float partPrice = getPartPrice(partId);
    if (partPrice < 0) {
        printf(RED "Error: Part not found or invalid.\n" RESET);
        return;
    }
    float totalPartCost = partPrice * quantity;

    // Check if job exists
    int job_exists = 0;
    FILE* fp_job = fopen(JOB_FILE, "r");
    if (fp_job) {
        char line[500];
        Job j;
        while(fgets(line, sizeof(line), fp_job)) {
            char temp_line[500];
            strcpy(temp_line, line);
            char* token;
            token = strtok(temp_line, ",");
            if (!token) continue; strncpy(j.id, token, sizeof(j.id)-1); j.id[sizeof(j.id)-1] = '\0';
            if (strcmp(j.id, jobId) == 0) {
                job_exists = 1;
                break;
            }
        }
        fclose(fp_job);
    }
    if (!job_exists) {
        printf(RED "Job ID not found.\n" RESET);
        pauseScreen();
        return;
    }

    FILE *fp_job_part = fopen(JOB_PART_FILE, "a+");
    if (!fp_job_part) {
        printf(RED "Error opening job parts file.\n" RESET);
        return;
    }
    fprintf(fp_job_part, "%s,%s,%d,%.2f\n", jobId, partId, quantity, partPrice);
    fclose(fp_job_part);

    // Update job's parts cost
    FILE *fp_read = fopen(JOB_FILE, "r");
    FILE *fp_write = fopen("temp_jobs.txt", "w");
    if (!fp_read || !fp_write) {
        printf(RED "Error updating job file.\n" RESET);
        if (fp_read) fclose(fp_read);
        if (fp_write) fclose(fp_write);
        return;
    }

    Job j;
    char line2[500];
    while (fgets(line2, sizeof(line2), fp_read)) {
        char temp_line[500];
        strcpy(temp_line, line2);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(j.id, token, sizeof(j.id)-1); j.id[sizeof(j.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.customerId, token, sizeof(j.customerId)-1); j.customerId[sizeof(j.customerId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.vehicleId, token, sizeof(j.vehicleId)-1); j.vehicleId[sizeof(j.vehicleId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.mechanicId, token, sizeof(j.mechanicId)-1); j.mechanicId[sizeof(j.mechanicId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.description, token, sizeof(j.description)-1); j.description[sizeof(j.description)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.status, token, sizeof(j.status)-1); j.status[sizeof(j.status)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; j.estimatedCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.laborCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.partsCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.notes, token, sizeof(j.notes)-1); j.notes[sizeof(j.notes)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.startDate, token, sizeof(j.startDate)-1); j.startDate[sizeof(j.startDate)-1] = '\0';
        
        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(j.endDate, token, sizeof(j.endDate)-1); j.endDate[sizeof(j.endDate)-1] = '\0';

        if (strcmp(j.id, jobId) == 0) {
            j.partsCost += totalPartCost;
        }
        fprintf(fp_write, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", j.id, j.customerId, j.vehicleId,
                j.mechanicId, j.description, j.status, j.estimatedCost, j.laborCost, j.partsCost, j.notes,
                j.startDate, j.endDate);
    }
    fclose(fp_read);
    fclose(fp_write);
    remove(JOB_FILE);
    rename("temp_jobs.txt", JOB_FILE);
    printf(GREEN "Part added to job %s. Total parts cost for this job is now %.2f.\n" RESET, jobId, j.partsCost);
    char log_msg[100];
    sprintf(log_msg, "Part %s added to job %s.", partId, jobId);
    writeLog(log_msg);
    pauseScreen();
}

// Updates the labor cost of an existing job
void updateJobLaborCost() {
    FILE *fp_read = fopen(JOB_FILE, "r");
    if (!fp_read) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }
    FILE *fp_write = fopen("temp_jobs.txt", "w");
    if (!fp_write) {
        fclose(fp_read);
        printf(RED "Error creating temporary file.\n" RESET);
        return;
    }

    char jobId[10];
    printf(YELLOW "\nEnter Job ID to update labor cost for: " RESET);
    scanf("%9s", jobId);

    Job j;
    char line[500];
    int found = 0;
    while (fgets(line, sizeof(line), fp_read)) {
        char temp_line[500];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(j.id, token, sizeof(j.id)-1); j.id[sizeof(j.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.customerId, token, sizeof(j.customerId)-1); j.customerId[sizeof(j.customerId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.vehicleId, token, sizeof(j.vehicleId)-1); j.vehicleId[sizeof(j.vehicleId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.mechanicId, token, sizeof(j.mechanicId)-1); j.mechanicId[sizeof(j.mechanicId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.description, token, sizeof(j.description)-1); j.description[sizeof(j.description)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.status, token, sizeof(j.status)-1); j.status[sizeof(j.status)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; j.estimatedCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.laborCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.partsCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.notes, token, sizeof(j.notes)-1); j.notes[sizeof(j.notes)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.startDate, token, sizeof(j.startDate)-1); j.startDate[sizeof(j.startDate)-1] = '\0';
        
        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(j.endDate, token, sizeof(j.endDate)-1); j.endDate[sizeof(j.endDate)-1] = '\0';

        if (strcmp(j.id, jobId) == 0) {
            found = 1;
            float newLaborCost;
            printf("Current Labor Cost: %.2f\n", j.laborCost);
            printf(YELLOW "Enter new labor cost: " RESET);
            scanf("%f", &newLaborCost);
            j.laborCost = newLaborCost;
        }
        fprintf(fp_write, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", j.id, j.customerId, j.vehicleId,
                j.mechanicId, j.description, j.status, j.estimatedCost, j.laborCost, j.partsCost, j.notes,
                j.startDate, j.endDate);
    }
    fclose(fp_read);
    fclose(fp_write);

    if (found) {
        remove(JOB_FILE);
        rename("temp_jobs.txt", JOB_FILE);
        printf(GREEN "Labor cost for job %s updated successfully!\n" RESET, jobId);
        char log_msg[100];
        sprintf(log_msg, "Labor cost for job %s updated.", jobId);
        writeLog(log_msg);
    } else {
        remove("temp_jobs.txt");
        printf(RED "Job ID not found.\n" RESET);
    }
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
    printf(GREEN "Part added to inventory successfully! ID: %s\n" RESET, p.id);
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
    Part arr[200];
    int c = 0;
    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        // Safely parse part data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(arr[c].id, token, sizeof(arr[c].id)-1); arr[c].id[sizeof(arr[c].id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(arr[c].name, token, sizeof(arr[c].name)-1); arr[c].name[sizeof(arr[c].name)-1] = '\0';

        token = strtok(NULL, ",");
        if (!token) continue; arr[c].quantity = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue; arr[c].price = atof(token);

        token = strtok(NULL, ",");
        if (!token) continue; strncpy(arr[c].supplierId, token, sizeof(arr[c].supplierId)-1); arr[c].supplierId[sizeof(arr[c].supplierId)-1] = '\0';

        token = strtok(NULL, "\n");
        if (!token) continue; arr[c].reorderLevel = atoi(token);

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
            if (scanf("%d", &newQuantity) == 1) arr[i].quantity = newQuantity;
            printf("Current Price: %.2f\n", arr[i].price);
            printf(YELLOW "Enter new price: " RESET);
            float newPrice;
            if (scanf("%f", &newPrice) == 1) arr[i].price = newPrice;
            printf("Current Supplier ID: %s\n", arr[i].supplierId);
            printf(YELLOW "Enter new supplier ID (or press Enter to keep current): " RESET);
            clearInputBuffer();
            char newSupplierId[10];
            fgets(newSupplierId, sizeof(newSupplierId), stdin);
            newSupplierId[strcspn(newSupplierId, "\n")] = '\0';
            if (strlen(newSupplierId) > 0) {
                if (supplierExists(newSupplierId)) {
                    strcpy(arr[i].supplierId, newSupplierId);
                } else {
                    printf(RED "Supplier ID not found. Keeping old supplier ID.\n" RESET);
                }
            }
            printf("Current Reorder Level: %d\n", arr[i].reorderLevel);
            printf(YELLOW "Enter new reorder level: " RESET);
            int newReorderLevel;
            if (scanf("%d", &newReorderLevel) == 1) arr[i].reorderLevel = newReorderLevel;
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
        printf(RED "Error opening parts file for writing.\n" RESET);
        return;
    }
    for (int i = 0; i < c; i++) fprintf(fp, "%s,%s,%d,%.2f,%s,%d\n", arr[i].id, arr[i].name, arr[i].quantity, arr[i].price, arr[i].supplierId, arr[i].reorderLevel);
    fclose(fp);
    printf(GREEN "Part information updated successfully!\n" RESET);
    char log_msg[100];
    sprintf(log_msg, "Part with ID %s updated.", id);
    writeLog(log_msg);
    pauseScreen();
}

// Views all parts in the inventory
void viewInventory() {
    FILE *fp = fopen(PART_FILE, "r");
    if (!fp) {
        printf(RED "No parts found!\n" RESET);
        return;
    }
    char line[200];
    printf("\n" CYAN "--- Inventory ---" RESET "\n");
    printf("ID,Name,Quantity,Price,Supplier ID,Reorder Level\n");
    Part p;
    while (fgets(line, sizeof(line), fp)) {
        // Safely parse part data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(p.id, token, sizeof(p.id)-1); p.id[sizeof(p.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(p.name, token, sizeof(p.name)-1); p.name[sizeof(p.name)-1] = '\0';

        token = strtok(NULL, ",");
        if (!token) continue; p.quantity = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue; p.price = atof(token);

        token = strtok(NULL, ",");
        if (!token) continue; strncpy(p.supplierId, token, sizeof(p.supplierId)-1); p.supplierId[sizeof(p.supplierId)-1] = '\0';

        token = strtok(NULL, "\n");
        if (!token) continue; p.reorderLevel = atoi(token);
        
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
        // Safely parse part data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(p.id, token, sizeof(p.id)-1); p.id[sizeof(p.id)-1] = '\0';
        
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
        // Safely parse part data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(p.id, token, sizeof(p.id)-1); p.id[sizeof(p.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(p.name, token, sizeof(p.name)-1); p.name[sizeof(p.name)-1] = '\0';

        token = strtok(NULL, ",");
        if (!token) continue; p.quantity = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue; p.price = atof(token);
        
        if (!strcmp(p.id, partId)) {
            fclose(fp);
            return p.price;
        }
    }
    fclose(fp);
    return -1.0;
}

// Deducts parts from inventory and checks for low stock
void deductPartFromInventory(const char *partId, int quantity) {
    FILE *fp_read = fopen(PART_FILE, "r");
    if (!fp_read) {
        printf(RED "Error opening parts file.\n" RESET);
        return;
    }
    FILE *fp_write = fopen("temp_parts.txt", "w");
    if (!fp_write) {
        fclose(fp_read);
        printf(RED "Error creating temporary file.\n" RESET);
        return;
    }

    Part p;
    char line[200];
    int found = 0;
    while (fgets(line, sizeof(line), fp_read)) {
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(p.id, token, sizeof(p.id)-1); p.id[sizeof(p.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(p.name, token, sizeof(p.name)-1); p.name[sizeof(p.name)-1] = '\0';

        token = strtok(NULL, ",");
        if (!token) continue; p.quantity = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue; p.price = atof(token);

        token = strtok(NULL, ",");
        if (!token) continue; strncpy(p.supplierId, token, sizeof(p.supplierId)-1); p.supplierId[sizeof(p.supplierId)-1] = '\0';

        token = strtok(NULL, "\n");
        if (!token) continue; p.reorderLevel = atoi(token);

        if (strcmp(p.id, partId) == 0) {
            found = 1;
            if (p.quantity >= quantity) {
                p.quantity -= quantity;
                printf(GREEN "Deducted %d units of part %s from inventory.\n" RESET, quantity, partId);
                if (p.quantity < p.reorderLevel) {
                    printf(YELLOW "Warning: Part %s is now below reorder level. Adding to reorder list.\n" RESET, p.id);
                    // Add to reorder list if not already there
                    FILE* fp_reorder = fopen(REORDER_LIST_FILE, "a+");
                    if (fp_reorder) {
                        int already_listed = 0;
                        char reorder_line[100];
                        rewind(fp_reorder);
                        while(fgets(reorder_line, sizeof(reorder_line), fp_reorder)) {
                            char listed_part_id[10];
                            sscanf(reorder_line, "%9[^,]", listed_part_id);
                            if (strcmp(listed_part_id, p.id) == 0) {
                                already_listed = 1;
                                break;
                            }
                        }
                        if (!already_listed) {
                            char date[15];
                            getCurrentDate(date);
                            fprintf(fp_reorder, "%s,%d,%s\n", p.id, p.reorderLevel * 2, date);
                        }
                        fclose(fp_reorder);
                    }
                }
            } else {
                printf(RED "Error: Not enough stock for part %s. Available: %d\n" RESET, partId, p.quantity);
                fprintf(fp_write, "%s,%s,%d,%.2f,%s,%d\n", p.id, p.name, p.quantity, p.price, p.supplierId, p.reorderLevel);
                fclose(fp_read);
                fclose(fp_write);
                remove("temp_parts.txt");
                return;
            }
        }
        fprintf(fp_write, "%s,%s,%d,%.2f,%s,%d\n", p.id, p.name, p.quantity, p.price, p.supplierId, p.reorderLevel);
    }
    fclose(fp_read);
    fclose(fp_write);

    if (found) {
        remove(PART_FILE);
        rename("temp_parts.txt", PART_FILE);
    } else {
        remove("temp_parts.txt");
        printf(RED "Part ID not found.\n" RESET);
    }
}

// Views all parts below their reorder level
void checkLowStock() {
    FILE *fp = fopen(PART_FILE, "r");
    if (!fp) {
        printf(RED "No parts found!\n" RESET);
        return;
    }
    char line[200];
    printf("\n" MAGENTA "--- Low Stock Alert ---" RESET "\n");
    printf("ID,Name,Quantity,Reorder Level\n");
    Part p;
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        // Safely parse part data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(p.id, token, sizeof(p.id)-1); p.id[sizeof(p.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(p.name, token, sizeof(p.name)-1); p.name[sizeof(p.name)-1] = '\0';

        token = strtok(NULL, ",");
        if (!token) continue; p.quantity = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue; p.price = atof(token);

        token = strtok(NULL, ",");
        if (!token) continue; strncpy(p.supplierId, token, sizeof(p.supplierId)-1); p.supplierId[sizeof(p.supplierId)-1] = '\0';

        token = strtok(NULL, "\n");
        if (!token) continue; p.reorderLevel = atoi(token);
        
        if (p.quantity < p.reorderLevel) {
            printf(RED "%s,%s,%d,%d\n" RESET, p.id, p.name, p.quantity, p.reorderLevel);
            found = 1;
        }
    }
    fclose(fp);
    if (!found) {
        printf(GREEN "No low stock items found.\n" RESET);
    }
    pauseScreen();
}

// Views the list of parts to be reordered
void viewReorderList() {
    FILE *fp = fopen(REORDER_LIST_FILE, "r");
    if (!fp) {
        printf(GREEN "Reorder list is empty.\n" RESET);
        return;
    }
    char line[100];
    printf("\n" CYAN "--- Reorder List ---" RESET "\n");
    printf("Part ID,Needed Quantity,Date Added\n");
    ReorderItem ri;
    while (fgets(line, sizeof(line), fp)) {
        // Safely parse reorder data
        sscanf(line, "%9[^,],%d,%14[^\n]", ri.partId, &ri.neededQuantity, ri.date);
        printf("%s,%d,%s\n", ri.partId, ri.neededQuantity, ri.date);
    }
    fclose(fp);
    pauseScreen();
}

// Clears the reorder list
void clearReorderList() {
    FILE *fp = fopen(REORDER_LIST_FILE, "w"); // Open in write mode to clear the file
    if (!fp) {
        printf(RED "Error opening reorder list file.\n" RESET);
        return;
    }
    fclose(fp);
    printf(GREEN "Reorder list cleared successfully.\n" RESET);
    writeLog("Reorder list cleared.");
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
    char line[200];
    printf("\n" CYAN "--- All Suppliers ---" RESET "\n");
    printf("ID,Name,Phone,Email\n");
    Supplier s;
    while (fgets(line, sizeof(line), fp)) {
        // Safely parse supplier data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(s.id, token, sizeof(s.id)-1); s.id[sizeof(s.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(s.name, token, sizeof(s.name)-1); s.name[sizeof(s.name)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(s.phone, token, sizeof(s.phone)-1); s.phone[sizeof(s.phone)-1] = '\0';
        
        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(s.email, token, sizeof(s.email)-1); s.email[sizeof(s.email)-1] = '\0';
        
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
        // Safely parse supplier data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(s.id, token, sizeof(s.id)-1); s.id[sizeof(s.id)-1] = '\0';
        
        if (!strcmp(s.id, id)) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// Appointment Management Functions
// Schedules an appointment for a customer
void scheduleAppointment(const char *currentUserId, const char *currentUserRole) {
    FILE *fp = fopen(APPOINTMENT_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening appointment file.\n" RESET);
        return;
    }
    Appointment a;
    int cnt = fileLineCount(APPOINTMENT_FILE);
    generateId("APT", cnt + 1, a.id);

    // If a customer is logged in, use their ID
    if (strcmp(currentUserRole, "Customer") == 0) {
        strcpy(a.customerId, currentUserId);
        printf(GREEN "Appointment for yourself (%s).\n" RESET, a.customerId);
    } else {
        printf(YELLOW "\nEnter Customer ID for the appointment: " RESET);
        scanf("%9s", a.customerId);
        if (!customerExists(a.customerId)) {
            printf(RED "Customer ID not found. Please add the customer first.\n" RESET);
            fclose(fp);
            pauseScreen();
            return;
        }
    }

    printf(YELLOW "Enter Vehicle ID for the appointment: " RESET);
    scanf("%9s", a.vehicleId);
    if (!vehicleExists(a.vehicleId)) {
        printf(RED "Vehicle ID not found. Please add the vehicle first.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    }
    
    char date[15];
    printf(YELLOW "Enter appointment date (YYYY-MM-DD): " RESET);
    scanf("%14s", date);
    if (!isDateValid(date)) {
        printf(RED "Invalid date format. Please use YYYY-MM-DD.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    }
    strcpy(a.date, date);
    
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
    sprintf(log_msg, "New appointment %s scheduled for customer %s.", a.id, a.customerId);
    writeLog(log_msg);
    pauseScreen();
}

// Views all appointments
void viewAllAppointments() {
    FILE *fp = fopen(APPOINTMENT_FILE, "r");
    if (!fp) {
        printf(RED "No appointments scheduled.\n" RESET);
        return;
    }
    char line[200];
    printf("\n" CYAN "--- All Appointments ---" RESET "\n");
    printf("ID,Customer ID,Vehicle ID,Date,Time,Description,Status\n");
    Appointment a;
    while (fgets(line, sizeof(line), fp)) {
        // Safely parse appointment data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;
        
        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(a.id, token, sizeof(a.id)-1); a.id[sizeof(a.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(a.customerId, token, sizeof(a.customerId)-1); a.customerId[sizeof(a.customerId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(a.vehicleId, token, sizeof(a.vehicleId)-1); a.vehicleId[sizeof(a.vehicleId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(a.date, token, sizeof(a.date)-1); a.date[sizeof(a.date)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(a.time, token, sizeof(a.time)-1); a.time[sizeof(a.time)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(a.description, token, sizeof(a.description)-1); a.description[sizeof(a.description)-1] = '\0';
        
        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(a.status, token, sizeof(a.status)-1); a.status[sizeof(a.status)-1] = '\0';
        
        printf("%s,%s,%s,%s,%s,%s,%s\n", a.id, a.customerId, a.vehicleId, a.date, a.time, a.description, a.status);
    }
    fclose(fp);
    pauseScreen();
}

// Schedules a service reminder for a customer's vehicle
void scheduleServiceReminder() {
    char vehicleId[10];
    printf(YELLOW "Enter Vehicle ID to schedule a service reminder: " RESET);
    scanf("%9s", vehicleId);

    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) {
        printf(RED "No vehicles found.\n" RESET);
        return;
    }
    Vehicle v;
    char line[200];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        // Safely parse vehicle data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(v.id, token, sizeof(v.id)-1); v.id[sizeof(v.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.customerId, token, sizeof(v.customerId)-1); v.customerId[sizeof(v.customerId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.brand, token, sizeof(v.brand)-1); v.brand[sizeof(v.brand)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.model, token, sizeof(v.model)-1); v.model[sizeof(v.model)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.plate, token, sizeof(v.plate)-1); v.plate[sizeof(v.plate)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(v.year, token, sizeof(v.year)-1); v.year[sizeof(v.year)-1] = '\0';
        
        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(v.lastServiceDate, token, sizeof(v.lastServiceDate)-1); v.lastServiceDate[sizeof(v.lastServiceDate)-1] = '\0';

        if (strcmp(v.id, vehicleId) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found) {
        printf(RED "Vehicle ID not found.\n" RESET);
        pauseScreen();
        return;
    }

    char nextServiceDate[15];
    calculateNextServiceDate(v.lastServiceDate, nextServiceDate);
    
    FILE *fp_rem = fopen(REMINDERS_FILE, "a+");
    if (!fp_rem) {
        printf(RED "Error opening reminders file.\n" RESET);
        return;
    }

    Reminder rem;
    int cnt = fileLineCount(REMINDERS_FILE);
    generateId("REM", cnt + 1, rem.id);
    strcpy(rem.customerId, v.customerId);
    sprintf(rem.message, "Reminder: Your vehicle %s (%s) is due for service on or around %s.", v.brand, v.plate, nextServiceDate);
    strcpy(rem.date, nextServiceDate);
    strcpy(rem.status, "Scheduled");

    fprintf(fp_rem, "%s,%s,%s,%s,%s\n", rem.id, rem.customerId, rem.message, rem.date, rem.status);
    fclose(fp_rem);
    
    printf(GREEN "Service reminder scheduled for vehicle %s. Next service date: %s.\n" RESET, vehicleId, nextServiceDate);
    char log_msg[100];
    sprintf(log_msg, "Service reminder scheduled for vehicle %s.", vehicleId);
    writeLog(log_msg);
    pauseScreen();
}

// Views all scheduled reminders
void viewAllReminders() {
    FILE *fp = fopen(REMINDERS_FILE, "r");
    if (!fp) {
        printf(RED "No reminders scheduled.\n" RESET);
        return;
    }
    char line[300];
    printf("\n" CYAN "--- All Service Reminders ---" RESET "\n");
    printf("ID,Customer ID,Message,Date,Status\n");
    Reminder rem;
    while (fgets(line, sizeof(line), fp)) {
        // Safely parse reminder data
        sscanf(line, "%9[^,],%9[^,],%199[^,],%14[^,],%19[^\n]", rem.id, rem.customerId, rem.message, rem.date, rem.status);
        printf("%s,%s,%s,%s,%s\n", rem.id, rem.customerId, rem.message, rem.date, rem.status);
    }
    fclose(fp);
    pauseScreen();
}

// Financial Management Functions
// Adds a new expense to the system
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
    
    printf(YELLOW "\nEnter expense description: " RESET);
    clearInputBuffer();
    fgets(t.description, sizeof(t.description), stdin);
    t.description[strcspn(t.description, "\n")] = '\0';
    
    printf(YELLOW "Enter amount: " RESET);
    scanf("%f", &t.amount);
    
    getCurrentDate(t.date);
    
    fprintf(fp, "%s,%s,%.2f,%s,%s\n", t.id, t.type, t.amount, t.date, t.description);
    fclose(fp);

    printf(GREEN "Expense added successfully! ID: %s\n" RESET, t.id);
    char log_msg[100];
    sprintf(log_msg, "New expense %s added.", t.id);
    writeLog(log_msg);
    pauseScreen();
}

// Views all expenses
void viewAllExpenses() {
    FILE *fp = fopen(EXPENSES_FILE, "r");
    if (!fp) {
        printf(RED "No expenses found.\n" RESET);
        return;
    }
    char line[200];
    printf("\n" CYAN "--- All Expenses ---" RESET "\n");
    printf("ID,Type,Amount,Date,Description\n");
    Transaction t;
    while (fgets(line, sizeof(line), fp)) {
        // Safely parse transaction data
        sscanf(line, "%9[^,],%19[^,],%f,%14[^,],%99[^\n]", t.id, t.type, &t.amount, t.date, t.description);
        printf("%s,%s,%.2f,%s,%s\n", t.id, t.type, t.amount, t.date, t.description);
    }
    fclose(fp);
    pauseScreen();
}

// Generates a profit and loss report
void generateProfitLossReport() {
    float total_revenue = 0.0, total_expenses = 0.0;
    
    // Calculate total revenue from invoices
    FILE *fp_inv = fopen(INVOICE_FILE, "r");
    if (fp_inv) {
        Invoice inv;
        char line[200];
        while (fgets(line, sizeof(line), fp_inv)) {
            // Safely parse invoice data
            sscanf(line, "%9[^,],%9[^,],%f,%f,%f,%14[^\n]", inv.id, inv.jobId, &inv.totalCost, &inv.discount, &inv.total, inv.date);
            total_revenue += inv.total;
        }
        fclose(fp_inv);
    }

    // Calculate total expenses from expenses file
    FILE *fp_exp = fopen(EXPENSES_FILE, "r");
    if (fp_exp) {
        Transaction t;
        char line[200];
        while (fgets(line, sizeof(line), fp_exp)) {
            // Safely parse transaction data
            sscanf(line, "%9[^,],%19[^,],%f,%14[^,],%99[^\n]", t.id, t.type, &t.amount, t.date, t.description);
            total_expenses += t.amount;
        }
        fclose(fp_exp);
    }

    float net_profit = total_revenue - total_expenses;

    printf("\n" MAGENTA "--- Profit & Loss Report ---" RESET "\n");
    printf("Total Revenue: %.2f\n", total_revenue);
    printf("Total Expenses: %.2f\n", total_expenses);
    printf("Net Profit/Loss: %.2f\n", net_profit);
    printf(BLUE "------------------------------" RESET "\n");
    
    writeLog("Profit and Loss report generated.");
    pauseScreen();
}

// Reporting Prototypes
// Views a job summary report
void viewJobSummaryReport() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found!\n" RESET);
        return;
    }
    
    int total_jobs = 0, pending_jobs = 0, in_progress_jobs = 0, completed_jobs = 0;
    float total_revenue = 0.0;
    
    char line[500];
    Job j;
    while (fgets(line, sizeof(line), fp)) {
        // Safely parse job data
        char temp_line[500];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(j.id, token, sizeof(j.id)-1); j.id[sizeof(j.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.customerId, token, sizeof(j.customerId)-1); j.customerId[sizeof(j.customerId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.vehicleId, token, sizeof(j.vehicleId)-1); j.vehicleId[sizeof(j.vehicleId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.mechanicId, token, sizeof(j.mechanicId)-1); j.mechanicId[sizeof(j.mechanicId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.description, token, sizeof(j.description)-1); j.description[sizeof(j.description)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.status, token, sizeof(j.status)-1); j.status[sizeof(j.status)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; j.estimatedCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.laborCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.partsCost = atof(token);
        
        total_jobs++;
        if (strcmp(j.status, "Pending") == 0) pending_jobs++;
        else if (strcmp(j.status, "InProgress") == 0) in_progress_jobs++;
        else if (strcmp(j.status, "Completed") == 0) {
            completed_jobs++;
            total_revenue += (j.laborCost + j.partsCost);
        }
    }
    fclose(fp);
    
    printf("\n" MAGENTA "--- Job Summary Report ---" RESET "\n");
    printf("Total Jobs: %d\n", total_jobs);
    printf("Pending Jobs: %d\n", pending_jobs);
    printf("In-Progress Jobs: %d\n", in_progress_jobs);
    printf("Completed Jobs: %d\n", completed_jobs);
    printf("Total Revenue from Completed Jobs: %.2f\n", total_revenue);
    printf(BLUE "--------------------------" RESET "\n");
    
    writeLog("Job summary report generated.");
    pauseScreen();
}

// Mechanic performance report
void mechanicPerformanceReport() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }

    FILE *fp_users = fopen(USER_FILE, "r");
    if (!fp_users) {
        fclose(fp);
        printf(RED "No users found.\n" RESET);
        return;
    }

    // A simple array to store mechanic performance data
    struct {
        char id[10];
        char name[30];
        int completed_jobs;
        float total_revenue;
        int active;
    } mechanics[100];
    int mec_count = 0;

    // Load mechanics
    char line[200];
    while(fgets(line, sizeof(line), fp_users)) {
        User u;
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(u.id, token, sizeof(u.id)-1); u.id[sizeof(u.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(u.username, token, sizeof(u.username)-1); u.username[sizeof(u.username)-1] = '\0';
        
        token = strtok(NULL, ",");
        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(u.role, token, sizeof(u.role)-1); u.role[sizeof(u.role)-1] = '\0';
        
        if (strcmp(u.role, "JuniorMechanic") == 0 || strcmp(u.role, "SeniorMechanic") == 0) {
            strcpy(mechanics[mec_count].id, u.id);
            strcpy(mechanics[mec_count].name, u.username);
            mechanics[mec_count].completed_jobs = 0;
            mechanics[mec_count].total_revenue = 0.0;
            mechanics[mec_count].active = 0;
            mec_count++;
        }
    }
    fclose(fp_users);

    // Process jobs
    rewind(fp);
    while (fgets(line, sizeof(line), fp)) {
        Job j;
        char temp_line[500];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(j.id, token, sizeof(j.id)-1); j.id[sizeof(j.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.customerId, token, sizeof(j.customerId)-1); j.customerId[sizeof(j.customerId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.vehicleId, token, sizeof(j.vehicleId)-1); j.vehicleId[sizeof(j.vehicleId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.mechanicId, token, sizeof(j.mechanicId)-1); j.mechanicId[sizeof(j.mechanicId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.description, token, sizeof(j.description)-1); j.description[sizeof(j.description)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.status, token, sizeof(j.status)-1); j.status[sizeof(j.status)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; j.estimatedCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.laborCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.partsCost = atof(token);
        
        if (strcmp(j.status, "Completed") == 0) {
            for (int i = 0; i < mec_count; i++) {
                if (strcmp(mechanics[i].id, j.mechanicId) == 0) {
                    mechanics[i].completed_jobs++;
                    mechanics[i].total_revenue += (j.laborCost + j.partsCost);
                    break;
                }
            }
        }
    }
    fclose(fp);

    printf("\n" MAGENTA "--- Mechanic Performance Report ---" RESET "\n");
    printf("ID,Name,Completed Jobs,Total Revenue\n");
    for (int i = 0; i < mec_count; i++) {
        printf("%s,%s,%d,%.2f\n", mechanics[i].id, mechanics[i].name, mechanics[i].completed_jobs, mechanics[i].total_revenue);
    }
    printf(BLUE "-----------------------------------" RESET "\n");
    writeLog("Mechanic performance report generated.");
    pauseScreen();
}

// Customer lifetime value report
void customerLifetimeValueReport() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }

    FILE *fp_cust = fopen(CUSTOMER_FILE, "r");
    if (!fp_cust) {
        fclose(fp);
        printf(RED "No customers found.\n" RESET);
        return;
    }

    // A simple array to store customer performance data
    struct {
        char id[10];
        char name[50];
        float total_spent;
    } customers[200];
    int cust_count = 0;

    // Load customers
    char line[200];
    while(fgets(line, sizeof(line), fp_cust)) {
        Customer c;
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(c.id, token, sizeof(c.id)-1); c.id[sizeof(c.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(c.name, token, sizeof(c.name)-1); c.name[sizeof(c.name)-1] = '\0';

        strcpy(customers[cust_count].id, c.id);
        strcpy(customers[cust_count].name, c.name);
        customers[cust_count].total_spent = 0.0;
        cust_count++;
    }
    fclose(fp_cust);

    // Process jobs to calculate spending
    rewind(fp);
    while (fgets(line, sizeof(line), fp)) {
        Job j;
        char temp_line[500];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(j.id, token, sizeof(j.id)-1); j.id[sizeof(j.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.customerId, token, sizeof(j.customerId)-1); j.customerId[sizeof(j.customerId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.vehicleId, token, sizeof(j.vehicleId)-1); j.vehicleId[sizeof(j.vehicleId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.mechanicId, token, sizeof(j.mechanicId)-1); j.mechanicId[sizeof(j.mechanicId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.description, token, sizeof(j.description)-1); j.description[sizeof(j.description)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.status, token, sizeof(j.status)-1); j.status[sizeof(j.status)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; j.estimatedCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.laborCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; j.partsCost = atof(token);
        
        if (strcmp(j.status, "Completed") == 0) {
            for (int i = 0; i < cust_count; i++) {
                if (strcmp(customers[i].id, j.customerId) == 0) {
                    customers[i].total_spent += (j.laborCost + j.partsCost);
                    break;
                }
            }
        }
    }
    fclose(fp);

    printf("\n" MAGENTA "--- Customer Lifetime Value Report ---" RESET "\n");
    printf("ID,Name,Total Spent\n");
    for (int i = 0; i < cust_count; i++) {
        printf("%s,%s,%.2f\n", customers[i].id, customers[i].name, customers[i].total_spent);
    }
    printf(BLUE "------------------------------------" RESET "\n");
    writeLog("Customer lifetime value report generated.");
    pauseScreen();
}

// Monthly revenue report
void monthlyRevenueReport() {
    FILE *fp = fopen(INVOICE_FILE, "r");
    if (!fp) {
        printf(RED "No invoices found.\n" RESET);
        return;
    }
    
    struct {
        char month[8]; // YYYY-MM
        float revenue;
    } monthly_data[120]; // Assuming up to 10 years of data
    int count = 0;

    char line[200];
    while(fgets(line, sizeof(line), fp)) {
        Invoice inv;
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(inv.id, token, sizeof(inv.id)-1); inv.id[sizeof(inv.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(inv.jobId, token, sizeof(inv.jobId)-1); inv.jobId[sizeof(inv.jobId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; inv.totalCost = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; inv.discount = atof(token);
        
        token = strtok(NULL, ",");
        if (!token) continue; inv.total = atof(token);
        
        token = strtok(NULL, "\n");
        if (!token) continue; strncpy(inv.date, token, sizeof(inv.date)-1); inv.date[sizeof(inv.date)-1] = '\0';
        
        char month_str[8];
        strncpy(month_str, inv.date, 7);
        month_str[7] = '\0';

        int found = 0;
        for (int i = 0; i < count; i++) {
            if (strcmp(monthly_data[i].month, month_str) == 0) {
                monthly_data[i].revenue += inv.total;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(monthly_data[count].month, month_str);
            monthly_data[count].revenue = inv.total;
            count++;
        }
    }
    fclose(fp);

    printf("\n" MAGENTA "--- Monthly Revenue Report ---" RESET "\n");
    printf("Month,Revenue\n");
    for (int i = 0; i < count; i++) {
        printf("%s,%.2f\n", monthly_data[i].month, monthly_data[i].revenue);
    }
    printf(BLUE "------------------------------" RESET "\n");
    writeLog("Monthly revenue report generated.");
    pauseScreen();
}

// Part usage report
void partUsageReport() {
    FILE *fp = fopen(JOB_PART_FILE, "r");
    if (!fp) {
        printf(RED "No parts used in jobs.\n" RESET);
        return;
    }
    
    struct {
        char partId[10];
        char partName[50];
        int total_quantity;
    } part_usage[200];
    int count = 0;

    char line[100];
    while(fgets(line, sizeof(line), fp)) {
        JobPart jp;
        char temp_line[100];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(jp.jobId, token, sizeof(jp.jobId)-1); jp.jobId[sizeof(jp.jobId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(jp.partId, token, sizeof(jp.partId)-1); jp.partId[sizeof(jp.partId)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; jp.quantity = atoi(token);

        int found = 0;
        for(int i = 0; i < count; i++) {
            if (strcmp(part_usage[i].partId, jp.partId) == 0) {
                part_usage[i].total_quantity += jp.quantity;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(part_usage[count].partId, jp.partId);
            part_usage[count].total_quantity = jp.quantity;
            // Get part name
            FILE* fp_part = fopen(PART_FILE, "r");
            if (fp_part) {
                char part_line[200];
                Part p;
                while(fgets(part_line, sizeof(part_line), fp_part)) {
                    sscanf(part_line, "%9[^,],%49[^,]", p.id, p.name);
                    if (strcmp(p.id, jp.partId) == 0) {
                        strcpy(part_usage[count].partName, p.name);
                        break;
                    }
                }
                fclose(fp_part);
            }
            count++;
        }
    }
    fclose(fp);

    printf("\n" MAGENTA "--- Part Usage Report ---" RESET "\n");
    printf("Part ID,Part Name,Total Quantity Used\n");
    for (int i = 0; i < count; i++) {
        printf("%s,%s,%d\n", part_usage[i].partId, part_usage[i].partName, part_usage[i].total_quantity);
    }
    printf(BLUE "---------------------------" RESET "\n");
    writeLog("Part usage report generated.");
    pauseScreen();
}

// Supplier performance report
void supplierPerformanceReport() {
    FILE *fp = fopen(PART_FILE, "r");
    if (!fp) {
        printf(RED "No parts found.\n" RESET);
        return;
    }

    FILE *fp_supp = fopen(SUPPLIER_FILE, "r");
    if (!fp_supp) {
        fclose(fp);
        printf(RED "No suppliers found.\n" RESET);
        return;
    }

    struct {
        char id[10];
        char name[50];
        int parts_supplied;
        float total_value;
    } suppliers[100];
    int supp_count = 0;

    // Load suppliers
    char line[200];
    while(fgets(line, sizeof(line), fp_supp)) {
        Supplier s;
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(s.id, token, sizeof(s.id)-1); s.id[sizeof(s.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(s.name, token, sizeof(s.name)-1); s.name[sizeof(s.name)-1] = '\0';

        strcpy(suppliers[supp_count].id, s.id);
        strcpy(suppliers[supp_count].name, s.name);
        suppliers[supp_count].parts_supplied = 0;
        suppliers[supp_count].total_value = 0.0;
        supp_count++;
    }
    fclose(fp_supp);

    // Process parts data
    rewind(fp);
    while (fgets(line, sizeof(line), fp)) {
        Part p;
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(p.id, token, sizeof(p.id)-1); p.id[sizeof(p.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(p.name, token, sizeof(p.name)-1); p.name[sizeof(p.name)-1] = '\0';

        token = strtok(NULL, ",");
        if (!token) continue; p.quantity = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue; p.price = atof(token);

        token = strtok(NULL, ",");
        if (!token) continue; strncpy(p.supplierId, token, sizeof(p.supplierId)-1); p.supplierId[sizeof(p.supplierId)-1] = '\0';
        
        for (int i = 0; i < supp_count; i++) {
            if (strcmp(suppliers[i].id, p.supplierId) == 0) {
                suppliers[i].parts_supplied += p.quantity;
                suppliers[i].total_value += (p.quantity * p.price);
                break;
            }
        }
    }
    fclose(fp);

    printf("\n" MAGENTA "--- Supplier Performance Report ---" RESET "\n");
    printf("ID,Name,Parts Supplied,Total Value\n");
    for (int i = 0; i < supp_count; i++) {
        printf("%s,%s,%d,%.2f\n", suppliers[i].id, suppliers[i].name, suppliers[i].parts_supplied, suppliers[i].total_value);
    }
    printf(BLUE "-----------------------------------" RESET "\n");
    writeLog("Supplier performance report generated.");
    pauseScreen();
}

// Customer retention report
void customerRetentionReport() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }

    struct {
        char customerId[10];
        int job_count;
    } customer_jobs[200];
    int count = 0;

    char line[500];
    while(fgets(line, sizeof(line), fp)) {
        Job j;
        char temp_line[500];
        strcpy(temp_line, line);
        char* token;

        token = strtok(temp_line, ",");
        if (!token) continue; strncpy(j.id, token, sizeof(j.id)-1); j.id[sizeof(j.id)-1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) continue; strncpy(j.customerId, token, sizeof(j.customerId)-1); j.customerId[sizeof(j.customerId)-1] = '\0';

        int found = 0;
        for (int i = 0; i < count; i++) {
            if (strcmp(customer_jobs[i].customerId, j.customerId) == 0) {
                customer_jobs[i].job_count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(customer_jobs[count].customerId, j.customerId);
            customer_jobs[count].job_count = 1;
            count++;
        }
    }
    fclose(fp);

    int new_customers = 0;
    int returning_customers = 0;
    for (int i = 0; i < count; i++) {
        if (customer_jobs[i].job_count == 1) {
            new_customers++;
        } else {
            returning_customers++;
        }
    }

    printf("\n" MAGENTA "--- Customer Retention Report ---" RESET "\n");
    printf("Total Unique Customers: %d\n", count);
    printf("New Customers (1 job): %d\n", new_customers);
    printf("Returning Customers (>1 job): %d\n", returning_customers);
    printf(BLUE "---------------------------------" RESET "\n");
    writeLog("Customer retention report generated.");
    pauseScreen();
}

// Menu Prototypes
void adminMenu(const char *uid) {
    int choice;
    do {
        printf("\n" CYAN "--- Admin Menu ---" RESET "\n");
        printf("1. View Dashboard\n");
        printf("2. Manage Users\n");
        printf("3. Manage Customers\n");
        printf("4. Manage Vehicles\n");
        printf("5. Manage Jobs\n");
        printf("6. Manage Invoices\n");
        printf("7. Manage Inventory\n");
        printf("8. Manage Suppliers\n");
        printf("9. Manage Appointments & Reminders\n");
        printf("10. Manage Finance\n");
        printf("11. View Reports\n");
        printf("12. Backup/Restore Data\n");
        printf("0. Logout\n");
        printf(YELLOW "Enter your choice: " RESET);
        scanf("%d", &choice);
        clearInputBuffer();
        switch (choice) {
            case 1:
                showDashboard();
                break;
            case 2:
                // User Management
                int user_choice;
                do {
                    printf("\n" MAGENTA "--- User Management ---" RESET "\n");
                    printf("1. Register New User\n");
                    printf("2. Change Password\n");
                    printf("3. View All Users\n");
                    printf("4. Change User Role\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter your choice: " RESET);
                    scanf("%d", &user_choice);
                    clearInputBuffer();
                    switch (user_choice) {
                        case 1:
                            registerUser();
                            break;
                        case 2:
                            changePassword(uid);
                            break;
                        case 3:
                            viewAllUsers();
                            break;
                        case 4:
                            changeUserRole();
                            break;
                    }
                } while (user_choice != 0);
                break;
            case 3:
                // Customer Management
                int cust_choice;
                do {
                    printf("\n" MAGENTA "--- Customer Management ---" RESET "\n");
                    printf("1. Add Customer\n");
                    printf("2. Edit Customer\n");
                    printf("3. Delete Customer\n");
                    printf("4. Search Customer\n");
                    printf("5. View All Customers\n");
                    printf("6. Send Feedback Request\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter your choice: " RESET);
                    scanf("%d", &cust_choice);
                    clearInputBuffer();
                    switch (cust_choice) {
                        case 1:
                            addCustomer();
                            break;
                        case 2:
                            editCustomer();
                            break;
                        case 3:
                            deleteCustomer();
                            break;
                        case 4:
                            searchCustomer();
                            break;
                        case 5:
                            viewAllCustomers();
                            break;
                        case 6:
                            sendFeedbackRequest();
                            break;
                    }
                } while (cust_choice != 0);
                break;
            case 4:
                // Vehicle Management
                int veh_choice;
                do {
                    printf("\n" MAGENTA "--- Vehicle Management ---" RESET "\n");
                    printf("1. Add Vehicle\n");
                    printf("2. Edit Vehicle\n");
                    printf("3. Delete Vehicle\n");
                    printf("4. Search Vehicle\n");
                    printf("5. View All Vehicles\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter your choice: " RESET);
                    scanf("%d", &veh_choice);
                    clearInputBuffer();
                    switch (veh_choice) {
                        case 1:
                            addVehicle();
                            break;
                        case 2:
                            editVehicle();
                            break;
                        case 3:
                            deleteVehicle();
                            break;
                        case 4:
                            searchVehicle();
                            break;
                        case 5:
                            viewAllVehicles();
                            break;
                    }
                } while (veh_choice != 0);
                break;
            case 5:
                // Job Management
                int job_choice;
                do {
                    printf("\n" MAGENTA "--- Job Management ---" RESET "\n");
                    printf("1. Add Job\n");
                    printf("2. Update Job Status\n");
                    printf("3. View All Jobs\n");
                    printf("4. View Pending Jobs\n");
                    printf("5. View Completed Jobs\n");
                    printf("6. Search Job\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter your choice: " RESET);
                    scanf("%d", &job_choice);
                    clearInputBuffer();
                    switch (job_choice) {
                        case 1:
                            addJob(uid, "Admin");
                            break;
                        case 2:
                            updateJobStatus(uid, "Admin");
                            break;
                        case 3:
                            viewAllJobs();
                            break;
                        case 4:
                            viewPendingJobs();
                            break;
                        case 5:
                            viewCompletedJobs();
                            break;
                        case 6:
                            searchJob();
                            break;
                    }
                } while (job_choice != 0);
                break;
            case 6:
                // Invoice Management
                int inv_choice;
                do {
                    printf("\n" MAGENTA "--- Invoice Management ---" RESET "\n");
                    printf("1. Generate Invoice\n");
                    printf("2. View All Invoices\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter your choice: " RESET);
                    scanf("%d", &inv_choice);
                    clearInputBuffer();
                    switch (inv_choice) {
                        case 1:
                            generateInvoice();
                            break;
                        case 2:
                            viewAllInvoices();
                            break;
                    }
                } while (inv_choice != 0);
                break;
            case 7:
                // Inventory Management
                int inv_choice_2;
                do {
                    printf("\n" MAGENTA "--- Inventory Management ---" RESET "\n");
                    printf("1. Add Part to Inventory\n");
                    printf("2. Edit Part Information\n");
                    printf("3. View Inventory\n");
                    printf("4. Add Part to Job\n");
                    printf("5. Update Job Labor Cost\n");
                    printf("6. Check Low Stock\n");
                    printf("7. View Reorder List\n");
                    printf("8. Clear Reorder List\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter your choice: " RESET);
                    scanf("%d", &inv_choice_2);
                    clearInputBuffer();
                    switch (inv_choice_2) {
                        case 1:
                            addPart();
                            break;
                        case 2:
                            editPart();
                            break;
                        case 3:
                            viewInventory();
                            break;
                        case 4:
                            addPartToJob();
                            break;
                        case 5:
                            updateJobLaborCost();
                            break;
                        case 6:
                            checkLowStock();
                            break;
                        case 7:
                            viewReorderList();
                            break;
                        case 8:
                            clearReorderList();
                            break;
                    }
                } while (inv_choice_2 != 0);
                break;
            case 8:
                // Supplier Management
                int supp_choice;
                do {
                    printf("\n" MAGENTA "--- Supplier Management ---" RESET "\n");
                    printf("1. Add Supplier\n");
                    printf("2. View All Suppliers\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter your choice: " RESET);
                    scanf("%d", &supp_choice);
                    clearInputBuffer();
                    switch (supp_choice) {
                        case 1:
                            addSupplier();
                            break;
                        case 2:
                            viewAllSuppliers();
                            break;
                    }
                } while (supp_choice != 0);
                break;
            case 9:
                // Appointment Management
                int apt_choice;
                do {
                    printf("\n" MAGENTA "--- Appointment Management ---" RESET "\n");
                    printf("1. Schedule Appointment\n");
                    printf("2. View All Appointments\n");
                    printf("3. Schedule Service Reminder\n");
                    printf("4. View All Reminders\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter your choice: " RESET);
                    scanf("%d", &apt_choice);
                    clearInputBuffer();
                    switch (apt_choice) {
                        case 1:
                            scheduleAppointment(uid, "Admin");
                            break;
                        case 2:
                            viewAllAppointments();
                            break;
                        case 3:
                            scheduleServiceReminder();
                            break;
                        case 4:
                            viewAllReminders();
                            break;
                    }
                } while (apt_choice != 0);
                break;
            case 10:
                // Financial Management
                int fin_choice;
                do {
                    printf("\n" MAGENTA "--- Financial Management ---" RESET "\n");
                    printf("1. Add Expense\n");
                    printf("2. View All Expenses\n");
                    printf("3. Generate Profit & Loss Report\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter your choice: " RESET);
                    scanf("%d", &fin_choice);
                    clearInputBuffer();
                    switch (fin_choice) {
                        case 1:
                            addExpense();
                            break;
                        case 2:
                            viewAllExpenses();
                            break;
                        case 3:
                            generateProfitLossReport();
                            break;
                    }
                } while (fin_choice != 0);
                break;
            case 11:
                // Reporting
                int rep_choice;
                do {
                    printf("\n" MAGENTA "--- Reporting ---" RESET "\n");
                    printf("1. Job Summary Report\n");
                    printf("2. Mechanic Performance Report\n");
                    printf("3. Customer Lifetime Value Report\n");
                    printf("4. Monthly Revenue Report\n");
                    printf("5. Part Usage Report\n");
                    printf("6. Supplier Performance Report\n");
                    printf("7. Customer Retention Report\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter your choice: " RESET);
                    scanf("%d", &rep_choice);
                    clearInputBuffer();
                    switch (rep_choice) {
                        case 1:
                            viewJobSummaryReport();
                            break;
                        case 2:
                            mechanicPerformanceReport();
                            break;
                        case 3:
                            customerLifetimeValueReport();
                            break;
                        case 4:
                            monthlyRevenueReport();
                            break;
                        case 5:
                            partUsageReport();
                            break;
                        case 6:
                            supplierPerformanceReport();
                            break;
                        case 7:
                            customerRetentionReport();
                            break;
                    }
                } while (rep_choice != 0);
                break;
            case 12:
                // Backup/Restore
                int br_choice;
                do {
                    printf("\n" MAGENTA "--- Backup/Restore ---" RESET "\n");
                    printf("1. Backup Data\n");
                    printf("2. Restore Data\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter your choice: " RESET);
                    scanf("%d", &br_choice);
                    clearInputBuffer();
                    switch (br_choice) {
                        case 1:
                            backupData();
                            break;
                        case 2:
                            restoreData();
                            break;
                    }
                } while (br_choice != 0);
                break;
        }
    } while (choice != 0);
}

void seniorMechanicMenu(const char *uid) {
    int choice;
    do {
        printf("\n" CYAN "--- Senior Mechanic Menu ---" RESET "\n");
        printf("1. View Dashboard\n");
        printf("2. Manage Customers\n");
        printf("3. Manage Vehicles\n");
        printf("4. Manage Jobs\n");
        printf("5. Manage Inventory\n");
        printf("6. Manage Appointments & Reminders\n");
        printf("7. Change My Password\n");
        printf("0. Logout\n");
        printf(YELLOW "Enter your choice: " RESET);
        scanf("%d", &choice);
        clearInputBuffer();
        switch (choice) {
            case 1:
                showDashboard();
                break;
            case 2:
                // Customer Management (Limited)
                int cust_choice;
                do {
                    printf("\n" MAGENTA "--- Customer Management ---" RESET "\n");
                    printf("1. Add Customer\n");
                    printf("2. Edit Customer\n");
                    printf("3. Search Customer\n");
                    printf("4. View All Customers\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter your choice: " RESET);
                    scanf("%d", &cust_choice);
                    clearInputBuffer();
                    switch (cust_choice) {
                        case 1:
                            addCustomer();
                            break;
                        case 2:
                            editCustomer();
                            break;
                        case 3:
                            searchCustomer();
                            break;
                        case 4:
                            viewAllCustomers();
                            break;
                    }
                } while (cust_choice != 0);
                break;
            case 3:
                // Vehicle Management (Limited)
                int veh_choice;
                do {
                    printf("\n" MAGENTA "--- Vehicle Management ---" RESET "\n");
                    printf("1. Add Vehicle\n");
                    printf("2. Edit Vehicle\n");
                    printf("3. Search Vehicle\n");
                    printf("4. View All Vehicles\n");
                    printf("0. Back\n");
                    scanf("%d", &veh_choice);
                    clearInputBuffer();
                    switch (veh_choice) {
                        case 1:
                            addVehicle();
                            break;
                        case 2:
                            editVehicle();
                            break;
                        case 3:
                            searchVehicle();
                            break;
                        case 4:
                            viewAllVehicles();
                            break;
                    }
                } while (veh_choice != 0);
                break;
            case 4:
                // Job Management
                int job_choice;
                do {
                    printf("\n" MAGENTA "--- Job Management ---" RESET "\n");
                    printf("1. Add Job\n");
                    printf("2. Update Job Status\n");
                    printf("3. View My Jobs\n");
                    printf("4. View Pending Jobs\n");
                    printf("5. Search Job\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter your choice: " RESET);
                    scanf("%d", &job_choice);
                    clearInputBuffer();
                    switch (job_choice) {
                        case 1:
                            addJob(uid, "SeniorMechanic");
                            break;
                        case 2:
                            updateJobStatus(uid, "SeniorMechanic");
                            break;
                        case 3:
                            viewMyJobs(uid);
                            break;
                        case 4:
                            viewPendingJobs();
                            break;
                        case 5:
                            searchJob();
                            break;
                    }
                } while (job_choice != 0);
                break;
            case 5:
                // Inventory Management (Limited)
                int inv_choice;
                do {
                    printf("\n" MAGENTA "--- Inventory Management ---" RESET "\n");
                    printf("1. View Inventory\n");
                    printf("2. Add Part to Job\n");
                    printf("3. Update Job Labor Cost\n");
                    printf("4. Check Low Stock\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter your choice: " RESET);
                    scanf("%d", &inv_choice);
                    clearInputBuffer();
                    switch (inv_choice) {
                        case 1:
                            viewInventory();
                            break;
                        case 2:
                            addPartToJob();
                            break;
                        case 3:
                            updateJobLaborCost();
                            break;
                        case 4:
                            checkLowStock();
                            break;
                    }
                } while (inv_choice != 0);
                break;
            case 6:
                // Appointment Management (Limited)
                int apt_choice;
                do {
                    printf("\n" MAGENTA "--- Appointment Management ---" RESET "\n");
                    printf("1. Schedule Appointment\n");
                    printf("2. View All Appointments\n");
                    printf("0. Back\n");
                    printf(YELLOW "Enter your choice: " RESET);
                    scanf("%d", &apt_choice);
                    clearInputBuffer();
                    switch (apt_choice) {
                        case 1:
                            scheduleAppointment(uid, "SeniorMechanic");
                            break;
                        case 2:
                            viewAllAppointments();
                            break;
                    }
                } while (apt_choice != 0);
                break;
            case 7:
                changePassword(uid);
                break;
        }
    } while (choice != 0);
}

void juniorMechanicMenu(const char *uid) {
    int choice;
    do {
        printf("\n" CYAN "--- Junior Mechanic Menu ---" RESET "\n");
        printf("1. View My Jobs\n");
        printf("2. Update My Job Status\n");
        printf("3. View Inventory\n");
        printf("4. Add Part to My Job\n");
        printf("5. Update My Job Labor Cost\n");
        printf("6. Change My Password\n");
        printf("0. Logout\n");
        printf(YELLOW "Enter your choice: " RESET);
        scanf("%d", &choice);
        clearInputBuffer();
        switch (choice) {
            case 1:
                viewMyJobs(uid);
                break;
            case 2:
                updateJobStatus(uid, "JuniorMechanic");
                break;
            case 3:
                viewInventory();
                break;
            case 4:
                addPartToJob();
                break;
            case 5:
                updateJobLaborCost();
                break;
            case 6:
                changePassword(uid);
                break;
        }
    } while (choice != 0);
}

void customerMenu(const char *uid) {
    int choice;
    do {
        printf("\n" CYAN "--- Customer Menu ---" RESET "\n");
        printf("1. View My Jobs\n");
        printf("2. View My Vehicles\n");
        printf("3. Schedule Appointment\n");
        printf("4. View My Notifications\n");
        printf("5. Change My Password\n");
        printf("0. Logout\n");
        printf(YELLOW "Enter your choice: " RESET);
        scanf("%d", &choice);
        clearInputBuffer();
        switch (choice) {
            case 1:
                viewCustomerJobs(uid);
                break;
            case 2:
                viewCustomerVehicles(uid);
                break;
            case 3:
                scheduleAppointment(uid, "Customer");
                break;
            case 4:
                viewNotifications(uid);
                break;
            case 5:
                changePassword(uid);
                break;
        }
    } while (choice != 0);
}

void printLogo() {
    printf(CYAN "\n"
           "   _______________________________________ \n"
           "  /  _  _  _  _  _  _  _  _  _  _  _  _  \\\n"
           " | / \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\/ \\ |\n"
           " |/   \\/   \\/   \\/   \\/   \\/   \\/   \\/   \\|\n"
           "|        GARAGE MANAGEMENT SYSTEM         |\n"
           "|\\ /   /\\   /\\   /\\   /\\   /\\   /\\   /||\n"
           "| \\_\\_/_\\_/_\\_/_\\_\\_/_\\_/_\\_/_\\_/_\\_/||\n"
           "\\_____________________________________/ \n" RESET);
}

int main() {
    int ch;
    char role[20], uid[10];

    // Create files if they don't exist
    FILE *fp;
    fp = fopen(USER_FILE, "a"); fclose(fp);
    fp = fopen(CUSTOMER_FILE, "a"); fclose(fp);
    fp = fopen(VEHICLE_FILE, "a"); fclose(fp);
    fp = fopen(JOB_FILE, "a"); fclose(fp);
    fp = fopen(INVOICE_FILE, "a"); fclose(fp);
    fp = fopen(PART_FILE, "a"); fclose(fp);
    fp = fopen(SUPPLIER_FILE, "a"); fclose(fp);
    fp = fopen(APPOINTMENT_FILE, "a"); fclose(fp);
    fp = fopen(JOB_PART_FILE, "a"); fclose(fp);
    fp = fopen(LOG_FILE, "a"); fclose(fp);
    fp = fopen(FINANCE_FILE, "a"); fclose(fp);
    fp = fopen(EXPENSES_FILE, "a"); fclose(fp);
    fp = fopen(REMINDERS_FILE, "a"); fclose(fp);
    fp = fopen(REORDER_LIST_FILE, "a"); fclose(fp);
    fp = fopen(NOTIFICATION_FILE, "a"); fclose(fp);
    
    printLogo();

    while (1) {
        printf("\n" CYAN "--- Main Menu ---" RESET "\n");
        if (fileLineCount(USER_FILE) == 0) {
            printf(YELLOW "No users found. Please register an admin first.\n" RESET);
            printf("1. Register Admin\n");
            printf("0. Exit\n");
            printf(YELLOW "Enter choice: " RESET);
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
    }

    return 0;
}