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
    printf(MAGENTA "  Total Jobs: %d\n" RESET, total_jobs);
    printf(MAGENTA "  Pending Jobs: %d\n" RESET, pending_jobs);
    printf(MAGENTA "  Completed Jobs: %d\n" RESET, completed_jobs);
    printf(MAGENTA "  Monthly Revenue: %.2f\n" RESET, monthly_revenue);
    printf(MAGENTA "  Low Stock Alerts: %d\n" RESET, reorder_items);
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
    
    char userId[10];
    char newRole[20];
    int found = 0;

    printf(YELLOW "\nEnter user ID to change role: " RESET);
    scanf("%9s", userId);

    for (int i = 0; i < c; i++) {
        if (strcmp(arr[i].id, userId) == 0) {
            printf(YELLOW "Current role for %s is %s.\n" RESET, arr[i].username, arr[i].role);
            printf(YELLOW "Enter new role (Admin/SeniorMechanic/JuniorMechanic/Customer): " RESET);
            scanf("%19s", newRole);
            if (strcmp(newRole, "Admin") != 0 && strcmp(newRole, "SeniorMechanic") != 0 && strcmp(newRole, "JuniorMechanic") != 0 && strcmp(newRole, "Customer") != 0) {
                printf(RED "Invalid role. Role not changed.\n" RESET);
            } else {
                strcpy(arr[i].role, newRole);
                printf(GREEN "Role for %s changed to %s successfully!\n" RESET, arr[i].username, arr[i].role);
                found = 1;
                char log_msg[100];
                sprintf(log_msg, "Role of user %s changed to %s.", arr[i].username, newRole);
                writeLog(log_msg);
            }
            break;
        }
    }

    if (!found) {
        printf(RED "User with ID %s not found.\n" RESET, userId);
    }

    fp = fopen(USER_FILE, "w");
    if (!fp) {
        printf(RED "Error opening user file for writing.\n" RESET);
        return;
    }
    for (int i = 0; i < c; i++) {
        fprintf(fp, "%s,%s,%s,%s\n", arr[i].id, arr[i].username, arr[i].password, arr[i].role);
    }
    fclose(fp);
    pauseScreen();
}

int getRoleLevel(const char *role) {
    if (strcmp(role, "Admin") == 0) return 4;
    if (strcmp(role, "SeniorMechanic") == 0) return 3;
    if (strcmp(role, "JuniorMechanic") == 0) return 2;
    if (strcmp(role, "Customer") == 0) return 1;
    return 0;
}

// Customer Management Functions
void addCustomer() {
    FILE *fp = fopen(CUSTOMER_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening customer file.\n" RESET);
        return;
    }
    Customer c;
    int count = fileLineCount(CUSTOMER_FILE);
    generateId("CUS", count + 1, c.id);
    
    printf(YELLOW "\nEnter customer name: " RESET);
    clearInputBuffer();
    fgets(c.name, sizeof(c.name), stdin);
    c.name[strcspn(c.name, "\n")] = 0;

    printf(YELLOW "Enter customer phone: " RESET);
    scanf("%14s", c.phone);
    printf(YELLOW "Enter customer email: " RESET);
    scanf("%49s", c.email);

    fprintf(fp, "%s,%s,%s,%s\n", c.id, c.name, c.phone, c.email);
    fclose(fp);
    printf(GREEN "Customer added successfully! ID: %s\n" RESET, c.id);
    char log_msg[100];
    sprintf(log_msg, "New customer %s added with ID %s.", c.name, c.id);
    writeLog(log_msg);
    pauseScreen();
}

int customerExists(const char *id) {
    FILE *fp = fopen(CUSTOMER_FILE, "r");
    if (!fp) return 0;
    char line[200];
    while(fgets(line, sizeof(line), fp)) {
        if (strstr(line, id) == line) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void editCustomer() {
    FILE *fp = fopen(CUSTOMER_FILE, "r");
    if (!fp) {
        printf(RED "Error opening customer file.\n" RESET);
        return;
    }

    Customer arr[100];
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

    char searchId[10];
    int found = 0;
    printf(YELLOW "\nEnter customer ID to edit: " RESET);
    scanf("%9s", searchId);

    for (int i = 0; i < c; i++) {
        if (strcmp(arr[i].id, searchId) == 0) {
            printf(YELLOW "Editing customer: %s\n" RESET, arr[i].name);
            printf(YELLOW "Enter new name (%s): " RESET, arr[i].name);
            clearInputBuffer();
            fgets(arr[i].name, sizeof(arr[i].name), stdin);
            arr[i].name[strcspn(arr[i].name, "\n")] = 0;
            
            printf(YELLOW "Enter new phone (%s): " RESET, arr[i].phone);
            scanf("%14s", arr[i].phone);
            printf(YELLOW "Enter new email (%s): " RESET, arr[i].email);
            scanf("%49s", arr[i].email);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf(RED "Customer with ID %s not found.\n" RESET, searchId);
        pauseScreen();
        return;
    }

    fp = fopen(CUSTOMER_FILE, "w");
    if (!fp) {
        printf(RED "Error opening customer file for writing.\n" RESET);
        return;
    }
    for (int i = 0; i < c; i++) {
        fprintf(fp, "%s,%s,%s,%s\n", arr[i].id, arr[i].name, arr[i].phone, arr[i].email);
    }
    fclose(fp);
    printf(GREEN "Customer details updated successfully!\n" RESET);
    char log_msg[100];
    sprintf(log_msg, "Customer with ID %s updated.", searchId);
    writeLog(log_msg);
    pauseScreen();
}

void deleteCustomer() {
    char searchId[10];
    printf(YELLOW "\nEnter customer ID to delete: " RESET);
    scanf("%9s", searchId);

    FILE *fp_read = fopen(CUSTOMER_FILE, "r");
    if (!fp_read) {
        printf(RED "Error opening customer file.\n" RESET);
        return;
    }

    FILE *fp_write = fopen("temp_customers.txt", "w");
    if (!fp_write) {
        printf(RED "Error creating temporary file.\n" RESET);
        fclose(fp_read);
        return;
    }

    char line[200];
    int found = 0;
    while (fgets(line, sizeof(line), fp_read)) {
        if (strstr(line, searchId) == line) {
            found = 1;
            continue;
        }
        fputs(line, fp_write);
    }

    fclose(fp_read);
    fclose(fp_write);

    if (found) {
        remove(CUSTOMER_FILE);
        rename("temp_customers.txt", CUSTOMER_FILE);
        printf(GREEN "Customer with ID %s deleted successfully.\n" RESET, searchId);
        char log_msg[100];
        sprintf(log_msg, "Customer with ID %s deleted.", searchId);
        writeLog(log_msg);
    } else {
        remove("temp_customers.txt");
        printf(RED "Customer with ID %s not found.\n" RESET, searchId);
    }
    pauseScreen();
}

void searchCustomer() {
    char searchTerm[50];
    printf(YELLOW "\nEnter customer name, ID, or phone to search: " RESET);
    scanf("%49s", searchTerm);

    FILE *fp = fopen(CUSTOMER_FILE, "r");
    if (!fp) {
        printf(RED "No customers found.\n" RESET);
        return;
    }

    char line[200];
    int found = 0;
    printf("\n" CYAN "--- Search Results ---" RESET "\n");
    printf("ID,Name,Phone,Email\n");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, searchTerm)) {
            printf("%s", line);
            found = 1;
        }
    }

    if (!found) {
        printf(RED "No matching customers found.\n" RESET);
    }
    fclose(fp);
    pauseScreen();
}

void viewAllCustomers() {
    FILE *fp = fopen(CUSTOMER_FILE, "r");
    if (!fp) {
        printf(RED "No customers found.\n" RESET);
        return;
    }

    char line[200];
    printf("\n" CYAN "--- All Customers ---" RESET "\n");
    printf("ID,Name,Phone,Email\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
    pauseScreen();
}

void sendFeedbackRequest() {
    char customerId[10];
    char feedbackMsg[200];
    printf(YELLOW "\nEnter customer ID to send feedback request: " RESET);
    scanf("%9s", customerId);
    clearInputBuffer();

    if (!customerExists(customerId)) {
        printf(RED "Customer with ID %s not found.\n" RESET, customerId);
        pauseScreen();
        return;
    }

    printf(YELLOW "Enter your message for the customer: " RESET);
    fgets(feedbackMsg, sizeof(feedbackMsg), stdin);
    feedbackMsg[strcspn(feedbackMsg, "\n")] = 0;

    createNotification(customerId, feedbackMsg);
    printf(GREEN "Feedback request sent to customer %s successfully.\n" RESET, customerId);
    char log_msg[100];
    sprintf(log_msg, "Feedback request sent to customer %s.", customerId);
    writeLog(log_msg);
    pauseScreen();
}

// Vehicle Management Functions
void addVehicle() {
    FILE *fp = fopen(VEHICLE_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening vehicle file.\n" RESET);
        return;
    }
    Vehicle v;
    char customerId[10];
    int count = fileLineCount(VEHICLE_FILE);
    generateId("VEH", count + 1, v.id);

    printf(YELLOW "\nEnter customer ID for the vehicle: " RESET);
    scanf("%9s", customerId);
    if (!customerExists(customerId)) {
        printf(RED "Customer with ID %s does not exist.\n" RESET, customerId);
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

    fprintf(fp, "%s,%s,%s,%s,%s,%s,%s\n", v.id, v.customerId, v.brand, v.model, v.plate, v.year, v.lastServiceDate);
    fclose(fp);
    printf(GREEN "Vehicle added successfully! ID: %s\n" RESET, v.id);
    char log_msg[100];
    sprintf(log_msg, "New vehicle %s %s added for customer %s.", v.brand, v.model, v.customerId);
    writeLog(log_msg);
    pauseScreen();
}

int vehicleExists(const char *id) {
    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) return 0;
    char line[200];
    while(fgets(line, sizeof(line), fp)) {
        if (strstr(line, id) == line) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void editVehicle() {
    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) {
        printf(RED "Error opening vehicle file.\n" RESET);
        return;
    }

    Vehicle arr[100];
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

    char searchId[10];
    int found = 0;
    printf(YELLOW "\nEnter vehicle ID to edit: " RESET);
    scanf("%9s", searchId);

    for (int i = 0; i < c; i++) {
        if (strcmp(arr[i].id, searchId) == 0) {
            printf(YELLOW "Editing vehicle: %s %s\n" RESET, arr[i].brand, arr[i].model);
            printf(YELLOW "Enter new brand (%s): " RESET, arr[i].brand);
            scanf("%29s", arr[i].brand);
            printf(YELLOW "Enter new model (%s): " RESET, arr[i].model);
            scanf("%29s", arr[i].model);
            printf(YELLOW "Enter new plate (%s): " RESET, arr[i].plate);
            scanf("%14s", arr[i].plate);
            printf(YELLOW "Enter new year (%s): " RESET, arr[i].year);
            scanf("%4s", arr[i].year);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf(RED "Vehicle with ID %s not found.\n" RESET, searchId);
        pauseScreen();
        return;
    }

    fp = fopen(VEHICLE_FILE, "w");
    if (!fp) {
        printf(RED "Error opening vehicle file for writing.\n" RESET);
        return;
    }
    for (int i = 0; i < c; i++) {
        fprintf(fp, "%s,%s,%s,%s,%s,%s,%s\n", arr[i].id, arr[i].customerId, arr[i].brand, arr[i].model, arr[i].plate, arr[i].year, arr[i].lastServiceDate);
    }
    fclose(fp);
    printf(GREEN "Vehicle details updated successfully!\n" RESET);
    char log_msg[100];
    sprintf(log_msg, "Vehicle with ID %s updated.", searchId);
    writeLog(log_msg);
    pauseScreen();
}

void deleteVehicle() {
    char searchId[10];
    printf(YELLOW "\nEnter vehicle ID to delete: " RESET);
    scanf("%9s", searchId);

    FILE *fp_read = fopen(VEHICLE_FILE, "r");
    if (!fp_read) {
        printf(RED "Error opening vehicle file.\n" RESET);
        return;
    }

    FILE *fp_write = fopen("temp_vehicles.txt", "w");
    if (!fp_write) {
        printf(RED "Error creating temporary file.\n" RESET);
        fclose(fp_read);
        return;
    }

    char line[200];
    int found = 0;
    while (fgets(line, sizeof(line), fp_read)) {
        if (strstr(line, searchId) == line) {
            found = 1;
            continue;
        }
        fputs(line, fp_write);
    }

    fclose(fp_read);
    fclose(fp_write);

    if (found) {
        remove(VEHICLE_FILE);
        rename("temp_vehicles.txt", VEHICLE_FILE);
        printf(GREEN "Vehicle with ID %s deleted successfully.\n" RESET, searchId);
        char log_msg[100];
        sprintf(log_msg, "Vehicle with ID %s deleted.", searchId);
        writeLog(log_msg);
    } else {
        remove("temp_vehicles.txt");
        printf(RED "Vehicle with ID %s not found.\n" RESET, searchId);
    }
    pauseScreen();
}

void searchVehicle() {
    char searchTerm[50];
    printf(YELLOW "\nEnter vehicle plate, ID, brand, or model to search: " RESET);
    scanf("%49s", searchTerm);

    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) {
        printf(RED "No vehicles found.\n" RESET);
        return;
    }

    char line[200];
    int found = 0;
    printf("\n" CYAN "--- Search Results ---" RESET "\n");
    printf("ID,Customer ID,Brand,Model,Plate,Year,Last Service\n");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, searchTerm)) {
            printf("%s", line);
            found = 1;
        }
    }

    if (!found) {
        printf(RED "No matching vehicles found.\n" RESET);
    }
    fclose(fp);
    pauseScreen();
}

void viewAllVehicles() {
    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) {
        printf(RED "No vehicles found.\n" RESET);
        return;
    }

    char line[200];
    printf("\n" CYAN "--- All Vehicles ---" RESET "\n");
    printf("ID,Customer ID,Brand,Model,Plate,Year,Last Service\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
    pauseScreen();
}

void viewCustomerVehicles(const char* customerId) {
    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) {
        printf(RED "No vehicles found.\n" RESET);
        return;
    }

    char line[200];
    int found = 0;
    printf("\n" CYAN "--- Your Vehicles ---" RESET "\n");
    printf("ID,Brand,Model,Plate,Year,Last Service\n");
    while (fgets(line, sizeof(line), fp)) {
        char temp_line[200];
        strcpy(temp_line, line);
        char* token = strtok(temp_line, ",");
        char vehicle_id[10];
        if (token) strcpy(vehicle_id, token);

        token = strtok(NULL, ",");
        if (token && strcmp(token, customerId) == 0) {
            printf("%s", line);
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
int mechanicExists(const char *id) {
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) return 0;
    char line[200];
    while(fgets(line, sizeof(line), fp)) {
        if (strstr(line, id) == line) {
            char* token = strtok(line, ",");
            token = strtok(NULL, ",");
            token = strtok(NULL, ",");
            token = strtok(NULL, "\n");
            if (strcmp(token, "Admin") == 0 || strcmp(token, "SeniorMechanic") == 0 || strcmp(token, "JuniorMechanic") == 0) {
                fclose(fp);
                return 1;
            }
        }
    }
    fclose(fp);
    return 0;
}

void addJob(const char *currentUserId, const char *currentUserRole) {
    FILE *fp = fopen(JOB_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening job file.\n" RESET);
        return;
    }
    Job j;
    int count = fileLineCount(JOB_FILE);
    generateId("JOB", count + 1, j.id);

    char customerId[10], vehicleId[10], mechanicId[10];

    if (strcmp(currentUserRole, "Customer") == 0) {
        strcpy(j.customerId, currentUserId);
        printf(YELLOW "Enter your vehicle ID: " RESET);
        scanf("%9s", j.vehicleId);
    } else {
        printf(YELLOW "Enter customer ID: " RESET);
        scanf("%9s", j.customerId);
        printf(YELLOW "Enter vehicle ID: " RESET);
        scanf("%9s", j.vehicleId);
    }
    clearInputBuffer();

    if (!customerExists(j.customerId) || !vehicleExists(j.vehicleId)) {
        printf(RED "Customer or Vehicle ID does not exist.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    }
    
    printf(YELLOW "Enter mechanic ID (or leave blank for unassigned): " RESET);
    fgets(j.mechanicId, sizeof(j.mechanicId), stdin);
    j.mechanicId[strcspn(j.mechanicId, "\n")] = 0;
    if (strlen(j.mechanicId) > 0 && !mechanicExists(j.mechanicId)) {
        printf(RED "Mechanic ID does not exist or is not a mechanic.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    } else if (strlen(j.mechanicId) == 0) {
        strcpy(j.mechanicId, "N/A");
    }

    printf(YELLOW "Enter job description: " RESET);
    fgets(j.description, sizeof(j.description), stdin);
    j.description[strcspn(j.description, "\n")] = 0;

    strcpy(j.status, "Pending");
    j.laborCost = 0.0;
    j.partsCost = 0.0;
    j.estimatedCost = calculateAverageCost(j.description);
    strcpy(j.notes, "N/A");
    getCurrentDate(j.startDate);
    strcpy(j.endDate, "N/A");
    
    fprintf(fp, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, j.estimatedCost, j.laborCost, j.partsCost, j.notes, j.startDate, j.endDate);
    fclose(fp);
    printf(GREEN "Job created successfully! ID: %s\n" RESET, j.id);
    char log_msg[100];
    sprintf(log_msg, "New job %s created for customer %s.", j.id, j.customerId);
    writeLog(log_msg);
    createNotification(j.customerId, "A new job has been created for your vehicle.");
    if (strcmp(j.mechanicId, "N/A") != 0) {
        createNotification(j.mechanicId, "You have been assigned a new job.");
    }
    pauseScreen();
}

void updateJobStatus(const char *currentUserId, const char *currentUserRole) {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "Error opening job file.\n" RESET);
        return;
    }

    Job arr[100];
    int c = 0;
    char line[500];
    while (fgets(line, sizeof(line), fp)) {
        // Safely parse job data
        char temp_line[500];
        strcpy(temp_line, line);
        char* token;
        token = strtok(temp_line, ","); if (!token) continue; strncpy(arr[c].id, token, sizeof(arr[c].id)-1); arr[c].id[sizeof(arr[c].id)-1] = '\0';
        token = strtok(NULL, ","); if (!token) continue; strncpy(arr[c].customerId, token, sizeof(arr[c].customerId)-1); arr[c].customerId[sizeof(arr[c].customerId)-1] = '\0';
        token = strtok(NULL, ","); if (!token) continue; strncpy(arr[c].vehicleId, token, sizeof(arr[c].vehicleId)-1); arr[c].vehicleId[sizeof(arr[c].vehicleId)-1] = '\0';
        token = strtok(NULL, ","); if (!token) continue; strncpy(arr[c].mechanicId, token, sizeof(arr[c].mechanicId)-1); arr[c].mechanicId[sizeof(arr[c].mechanicId)-1] = '\0';
        token = strtok(NULL, ","); if (!token) continue; strncpy(arr[c].description, token, sizeof(arr[c].description)-1); arr[c].description[sizeof(arr[c].description)-1] = '\0';
        token = strtok(NULL, ","); if (!token) continue; strncpy(arr[c].status, token, sizeof(arr[c].status)-1); arr[c].status[sizeof(arr[c].status)-1] = '\0';
        token = strtok(NULL, ","); if (!token) continue; arr[c].estimatedCost = atof(token);
        token = strtok(NULL, ","); if (!token) continue; arr[c].laborCost = atof(token);
        token = strtok(NULL, ","); if (!token) continue; arr[c].partsCost = atof(token);
        token = strtok(NULL, ","); if (!token) continue; strncpy(arr[c].notes, token, sizeof(arr[c].notes)-1); arr[c].notes[sizeof(arr[c].notes)-1] = '\0';
        token = strtok(NULL, ","); if (!token) continue; strncpy(arr[c].startDate, token, sizeof(arr[c].startDate)-1); arr[c].startDate[sizeof(arr[c].startDate)-1] = '\0';
        token = strtok(NULL, "\n"); if (!token) continue; strncpy(arr[c].endDate, token, sizeof(arr[c].endDate)-1); arr[c].endDate[sizeof(arr[c].endDate)-1] = '\0';
        c++;
    }
    fclose(fp);

    char searchId[10];
    int found = 0;
    printf(YELLOW "\nEnter job ID to update: " RESET);
    scanf("%9s", searchId);

    for (int i = 0; i < c; i++) {
        if (strcmp(arr[i].id, searchId) == 0) {
            if (strcmp(currentUserRole, "Admin") != 0 && strcmp(arr[i].mechanicId, currentUserId) != 0) {
                printf(RED "You do not have permission to update this job.\n" RESET);
                pauseScreen();
                return;
            }
            printf(YELLOW "Current status: %s\n" RESET, arr[i].status);
            printf(YELLOW "Enter new status (Pending/InProgress/Completed/Cancelled): " RESET);
            scanf("%19s", arr[i].status);
            if (strcmp(arr[i].status, "Completed") == 0) {
                getCurrentDate(arr[i].endDate);
                createNotification(arr[i].customerId, "Your job has been completed. An invoice has been generated.");
                generateInvoice(searchId);
                
                // Update vehicle's last service date
                FILE *fp_veh = fopen(VEHICLE_FILE, "r");
                if (fp_veh) {
                    Vehicle veh_arr[100];
                    int veh_c = 0;
                    char veh_line[200];
                    while(fgets(veh_line, sizeof(veh_line), fp_veh)) {
                        char temp_veh_line[200];
                        strcpy(temp_veh_line, veh_line);
                        char* token = strtok(temp_veh_line, ","); if(token) strncpy(veh_arr[veh_c].id, token, sizeof(veh_arr[veh_c].id)-1); veh_arr[veh_c].id[sizeof(veh_arr[veh_c].id)-1] = '\0';
                        token = strtok(NULL, ","); if(token) strncpy(veh_arr[veh_c].customerId, token, sizeof(veh_arr[veh_c].customerId)-1); veh_arr[veh_c].customerId[sizeof(veh_arr[veh_c].customerId)-1] = '\0';
                        token = strtok(NULL, ","); if(token) strncpy(veh_arr[veh_c].brand, token, sizeof(veh_arr[veh_c].brand)-1); veh_arr[veh_c].brand[sizeof(veh_arr[veh_c].brand)-1] = '\0';
                        token = strtok(NULL, ","); if(token) strncpy(veh_arr[veh_c].model, token, sizeof(veh_arr[veh_c].model)-1); veh_arr[veh_c].model[sizeof(veh_arr[veh_c].model)-1] = '\0';
                        token = strtok(NULL, ","); if(token) strncpy(veh_arr[veh_c].plate, token, sizeof(veh_arr[veh_c].plate)-1); veh_arr[veh_c].plate[sizeof(veh_arr[veh_c].plate)-1] = '\0';
                        token = strtok(NULL, ","); if(token) strncpy(veh_arr[veh_c].year, token, sizeof(veh_arr[veh_c].year)-1); veh_arr[veh_c].year[sizeof(veh_arr[veh_c].year)-1] = '\0';
                        token = strtok(NULL, "\n"); if(token) strncpy(veh_arr[veh_c].lastServiceDate, token, sizeof(veh_arr[veh_c].lastServiceDate)-1); veh_arr[veh_c].lastServiceDate[sizeof(veh_arr[veh_c].lastServiceDate)-1] = '\0';
                        veh_c++;
                    }
                    fclose(fp_veh);
                    
                    fp_veh = fopen(VEHICLE_FILE, "w");
                    for (int k = 0; k < veh_c; k++) {
                        if (strcmp(veh_arr[k].id, arr[i].vehicleId) == 0) {
                            strcpy(veh_arr[k].lastServiceDate, arr[i].endDate);
                        }
                        fprintf(fp_veh, "%s,%s,%s,%s,%s,%s,%s\n", veh_arr[k].id, veh_arr[k].customerId, veh_arr[k].brand, veh_arr[k].model, veh_arr[k].plate, veh_arr[k].year, veh_arr[k].lastServiceDate);
                    }
                    fclose(fp_veh);
                }
            } else if (strcmp(arr[i].status, "InProgress") == 0) {
                createNotification(arr[i].customerId, "Your job is now in progress.");
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        printf(RED "Job with ID %s not found.\n" RESET, searchId);
        pauseScreen();
        return;
    }

    fp = fopen(JOB_FILE, "w");
    if (!fp) {
        printf(RED "Error opening job file for writing.\n" RESET);
        return;
    }
    for (int i = 0; i < c; i++) {
        fprintf(fp, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", arr[i].id, arr[i].customerId, arr[i].vehicleId, arr[i].mechanicId, arr[i].description, arr[i].status, arr[i].estimatedCost, arr[i].laborCost, arr[i].partsCost, arr[i].notes, arr[i].startDate, arr[i].endDate);
    }
    fclose(fp);
    printf(GREEN "Job status updated successfully!\n" RESET);
    char log_msg[100];
    sprintf(log_msg, "Job %s status updated to %s.", searchId, arr[found ? (i) : 0].status);
    writeLog(log_msg);
    pauseScreen();
}

void viewMyJobs(const char *mechanicId) {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }

    char line[500];
    int found = 0;
    printf("\n" CYAN "--- My Assigned Jobs ---" RESET "\n");
    printf("ID,Customer,Vehicle,Description,Status,Estimated,Labor,Parts,Start,End\n");
    while (fgets(line, sizeof(line), fp)) {
        char temp_line[500];
        strcpy(temp_line, line);
        char* token = strtok(temp_line, ",");
        token = strtok(NULL, ",");
        token = strtok(NULL, ",");
        token = strtok(NULL, ","); // mechanicId token
        if (token && strcmp(token, mechanicId) == 0) {
            printf("%s", line);
            found = 1;
        }
    }

    if (!found) {
        printf(RED "You have no jobs assigned.\n" RESET);
    }
    fclose(fp);
    pauseScreen();
}

void viewCustomerJobs(const char *customerId) {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }

    char line[500];
    int found = 0;
    printf("\n" CYAN "--- Your Jobs ---" RESET "\n");
    printf("ID,Vehicle,Mechanic,Description,Status,Estimated,Labor,Parts,Start,End\n");
    while (fgets(line, sizeof(line), fp)) {
        char temp_line[500];
        strcpy(temp_line, line);
        char* token = strtok(temp_line, ",");
        token = strtok(NULL, ","); // customerId token
        if (token && strcmp(token, customerId) == 0) {
            printf("%s", line);
            found = 1;
        }
    }

    if (!found) {
        printf(RED "No jobs found for your account.\n" RESET);
    }
    fclose(fp);
    pauseScreen();
}

void searchJob() {
    char searchTerm[50];
    printf(YELLOW "\nEnter job ID, description or status to search: " RESET);
    scanf("%49s", searchTerm);

    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }

    char line[500];
    int found = 0;
    printf("\n" CYAN "--- Search Results ---" RESET "\n");
    printf("ID,Customer,Vehicle,Mechanic,Description,Status,Estimated,Labor,Parts,Start,End\n");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, searchTerm)) {
            printf("%s", line);
            found = 1;
        }
    }
    if (!found) {
        printf(RED "No matching jobs found.\n" RESET);
    }
    fclose(fp);
    pauseScreen();
}

void viewAllJobs() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }

    char line[500];
    printf("\n" CYAN "--- All Jobs ---" RESET "\n");
    printf("ID,Customer,Vehicle,Mechanic,Description,Status,Estimated,Labor,Parts,Start,End\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
    pauseScreen();
}

void viewPendingJobs() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }

    char line[500];
    int found = 0;
    printf("\n" CYAN "--- Pending Jobs ---" RESET "\n");
    printf("ID,Customer,Vehicle,Mechanic,Description,Status,Estimated,Labor,Parts,Start,End\n");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "Pending")) {
            printf("%s", line);
            found = 1;
        }
    }

    if (!found) {
        printf(GREEN "No pending jobs found.\n" RESET);
    }
    fclose(fp);
    pauseScreen();
}

void viewCompletedJobs() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }

    char line[500];
    int found = 0;
    printf("\n" CYAN "--- Completed Jobs ---" RESET "\n");
    printf("ID,Customer,Vehicle,Mechanic,Description,Status,Estimated,Labor,Parts,Start,End\n");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "Completed")) {
            printf("%s", line);
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
void generateInvoice(const char* jobId) {
    FILE *fp_job = fopen(JOB_FILE, "r");
    if (!fp_job) return;
    Job j;
    char line[500];
    int found = 0;

    while (fgets(line, sizeof(line), fp_job)) {
        if (strstr(line, jobId) == line) {
            // Safely parse job data
            char temp_line[500];
            strcpy(temp_line, line);
            char* token;
            token = strtok(temp_line, ","); if (!token) break; strncpy(j.id, token, sizeof(j.id)-1); j.id[sizeof(j.id)-1] = '\0';
            token = strtok(NULL, ","); if (!token) break; strncpy(j.customerId, token, sizeof(j.customerId)-1); j.customerId[sizeof(j.customerId)-1] = '\0';
            token = strtok(NULL, ","); if (!token) break; strncpy(j.vehicleId, token, sizeof(j.vehicleId)-1); j.vehicleId[sizeof(j.vehicleId)-1] = '\0';
            token = strtok(NULL, ","); if (!token) break; strncpy(j.mechanicId, token, sizeof(j.mechanicId)-1); j.mechanicId[sizeof(j.mechanicId)-1] = '\0';
            token = strtok(NULL, ","); if (!token) break; strncpy(j.description, token, sizeof(j.description)-1); j.description[sizeof(j.description)-1] = '\0';
            token = strtok(NULL, ","); if (!token) break; strncpy(j.status, token, sizeof(j.status)-1); j.status[sizeof(j.status)-1] = '\0';
            token = strtok(NULL, ","); if (!token) break; j.estimatedCost = atof(token);
            token = strtok(NULL, ","); if (!token) break; j.laborCost = atof(token);
            token = strtok(NULL, ","); if (!token) break; j.partsCost = atof(token);
            token = strtok(NULL, ","); if (!token) break; strncpy(j.notes, token, sizeof(j.notes)-1); j.notes[sizeof(j.notes)-1] = '\0';
            token = strtok(NULL, ","); if (!token) break; strncpy(j.startDate, token, sizeof(j.startDate)-1); j.startDate[sizeof(j.startDate)-1] = '\0';
            token = strtok(NULL, "\n"); if (!token) break; strncpy(j.endDate, token, sizeof(j.endDate)-1); j.endDate[sizeof(j.endDate)-1] = '\0';
            found = 1;
            break;
        }
    }
    fclose(fp_job);

    if (!found) return;

    FILE *fp_inv = fopen(INVOICE_FILE, "a+");
    if (!fp_inv) return;

    Invoice inv;
    int cnt = fileLineCount(INVOICE_FILE);
    generateId("INV", cnt + 1, inv.id);
    strcpy(inv.jobId, j.id);
    inv.totalCost = j.laborCost + j.partsCost;
    inv.discount = 0.0;
    inv.total = inv.totalCost - inv.discount;
    getCurrentDate(inv.date);

    fprintf(fp_inv, "%s,%s,%.2f,%.2f,%.2f,%s\n", inv.id, inv.jobId, inv.totalCost, inv.discount, inv.total, inv.date);
    fclose(fp_inv);

    FILE *fp_fin = fopen(FINANCE_FILE, "a");
    if (fp_fin) {
        fprintf(fp_fin, "%s,Revenue,%.2f,%s,Invoice for job %s\n", inv.id, inv.total, inv.date, j.id);
        fclose(fp_fin);
    }
}

void viewAllInvoices() {
    FILE *fp = fopen(INVOICE_FILE, "r");
    if (!fp) {
        printf(RED "No invoices found.\n" RESET);
        return;
    }

    char line[200];
    printf("\n" CYAN "--- All Invoices ---" RESET "\n");
    printf("Invoice ID,Job ID,Total Cost,Discount,Total,Date\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
    pauseScreen();
}

// Inventory Management Functions
void addPartToJob() {
    char jobId[10], partId[10];
    int quantity;
    printf(YELLOW "\nEnter job ID: " RESET);
    scanf("%9s", jobId);
    printf(YELLOW "Enter part ID: " RESET);
    scanf("%9s", partId);
    printf(YELLOW "Enter quantity: " RESET);
    scanf("%d", &quantity);

    if (!partExists(partId)) {
        printf(RED "Part with ID %s does not exist.\n" RESET, partId);
        pauseScreen();
        return;
    }

    FILE *fp_jp = fopen(JOB_PART_FILE, "a+");
    if (!fp_jp) {
        printf(RED "Error opening job part file.\n" RESET);
        return;
    }

    float price = getPartPrice(partId);
    if (price == -1.0) {
        printf(RED "Error getting part price.\n" RESET);
        fclose(fp_jp);
        pauseScreen();
        return;
    }

    fprintf(fp_jp, "%s,%s,%d,%.2f\n", jobId, partId, quantity, price);
    fclose(fp_jp);

    FILE *fp_job = fopen(JOB_FILE, "r");
    FILE *fp_job_temp = fopen("temp_job.txt", "w");
    if (!fp_job || !fp_job_temp) {
        printf(RED "Error updating job file.\n" RESET);
        return;
    }

    char line[500];
    int found = 0;
    while (fgets(line, sizeof(line), fp_job)) {
        char temp_line[500];
        strcpy(temp_line, line);
        char* token = strtok(temp_line, ",");
        if (token && strcmp(token, jobId) == 0) {
            Job j;
            strcpy(j.id, token);
            token = strtok(NULL, ","); if (!token) break; strcpy(j.customerId, token);
            token = strtok(NULL, ","); if (!token) break; strcpy(j.vehicleId, token);
            token = strtok(NULL, ","); if (!token) break; strcpy(j.mechanicId, token);
            token = strtok(NULL, ","); if (!token) break; strcpy(j.description, token);
            token = strtok(NULL, ","); if (!token) break; strcpy(j.status, token);
            token = strtok(NULL, ","); if (!token) break; j.estimatedCost = atof(token);
            token = strtok(NULL, ","); if (!token) break; j.laborCost = atof(token);
            token = strtok(NULL, ","); if (!token) break; j.partsCost = atof(token);
            token = strtok(NULL, ","); if (!token) break; strcpy(j.notes, token);
            token = strtok(NULL, ","); if (!token) break; strcpy(j.startDate, token);
            token = strtok(NULL, "\n"); if (!token) break; strcpy(j.endDate, token);
            
            j.partsCost += (price * quantity);
            fprintf(fp_job_temp, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, j.estimatedCost, j.laborCost, j.partsCost, j.notes, j.startDate, j.endDate);
            found = 1;
        } else {
            fputs(line, fp_job_temp);
        }
    }
    fclose(fp_job);
    fclose(fp_job_temp);
    remove(JOB_FILE);
    rename("temp_job.txt", JOB_FILE);
    
    deductPartFromInventory(partId, quantity);
    printf(GREEN "Part added to job %s successfully. Parts cost updated.\n" RESET, jobId);
    checkLowStock();
    char log_msg[100];
    sprintf(log_msg, "Part %s added to job %s, quantity %d.", partId, jobId, quantity);
    writeLog(log_msg);
    pauseScreen();
}

void updateJobLaborCost() {
    char jobId[10];
    float laborCost;
    printf(YELLOW "\nEnter job ID to update labor cost: " RESET);
    scanf("%9s", jobId);
    printf(YELLOW "Enter new labor cost: " RESET);
    scanf("%f", &laborCost);

    FILE *fp_job = fopen(JOB_FILE, "r");
    FILE *fp_job_temp = fopen("temp_job.txt", "w");
    if (!fp_job || !fp_job_temp) {
        printf(RED "Error updating job file.\n" RESET);
        return;
    }

    char line[500];
    int found = 0;
    while (fgets(line, sizeof(line), fp_job)) {
        char temp_line[500];
        strcpy(temp_line, line);
        char* token = strtok(temp_line, ",");
        if (token && strcmp(token, jobId) == 0) {
            Job j;
            strcpy(j.id, token);
            token = strtok(NULL, ","); if (!token) break; strcpy(j.customerId, token);
            token = strtok(NULL, ","); if (!token) break; strcpy(j.vehicleId, token);
            token = strtok(NULL, ","); if (!token) break; strcpy(j.mechanicId, token);
            token = strtok(NULL, ","); if (!token) break; strcpy(j.description, token);
            token = strtok(NULL, ","); if (!token) break; strcpy(j.status, token);
            token = strtok(NULL, ","); if (!token) break; j.estimatedCost = atof(token);
            token = strtok(NULL, ","); if (!token) break; j.laborCost = atof(token);
            token = strtok(NULL, ","); if (!token) break; j.partsCost = atof(token);
            token = strtok(NULL, ","); if (!token) break; strcpy(j.notes, token);
            token = strtok(NULL, ","); if (!token) break; strcpy(j.startDate, token);
            token = strtok(NULL, "\n"); if (!token) break; strcpy(j.endDate, token);
            
            j.laborCost = laborCost;
            fprintf(fp_job_temp, "%s,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, j.estimatedCost, j.laborCost, j.partsCost, j.notes, j.startDate, j.endDate);
            found = 1;
        } else {
            fputs(line, fp_job_temp);
        }
    }
    fclose(fp_job);
    fclose(fp_job_temp);
    remove(JOB_FILE);
    rename("temp_job.txt", JOB_FILE);
    
    if (found) {
        printf(GREEN "Labor cost updated for job %s successfully.\n" RESET, jobId);
        char log_msg[100];
        sprintf(log_msg, "Labor cost for job %s updated to %.2f.", jobId, laborCost);
        writeLog(log_msg);
    } else {
        printf(RED "Job with ID %s not found.\n" RESET, jobId);
    }
    pauseScreen();
}

void addPart() {
    FILE *fp = fopen(PART_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening parts file.\n" RESET);
        return;
    }
    Part p;
    int count = fileLineCount(PART_FILE);
    generateId("PAR", count + 1, p.id);

    printf(YELLOW "\nEnter part name: " RESET);
    clearInputBuffer();
    fgets(p.name, sizeof(p.name), stdin);
    p.name[strcspn(p.name, "\n")] = 0;
    
    printf(YELLOW "Enter quantity: " RESET);
    scanf("%d", &p.quantity);
    printf(YELLOW "Enter price: " RESET);
    scanf("%f", &p.price);
    printf(YELLOW "Enter supplier ID: " RESET);
    scanf("%9s", p.supplierId);
    printf(YELLOW "Enter reorder level: " RESET);
    scanf("%d", &p.reorderLevel);

    if (!supplierExists(p.supplierId)) {
        printf(RED "Supplier with ID %s does not exist.\n" RESET, p.supplierId);
        fclose(fp);
        pauseScreen();
        return;
    }

    fprintf(fp, "%s,%s,%d,%.2f,%s,%d\n", p.id, p.name, p.quantity, p.price, p.supplierId, p.reorderLevel);
    fclose(fp);
    printf(GREEN "Part added successfully! ID: %s\n" RESET, p.id);
    char log_msg[100];
    sprintf(log_msg, "New part %s added with ID %s.", p.name, p.id);
    writeLog(log_msg);
    pauseScreen();
}

void editPart() {
    FILE *fp = fopen(PART_FILE, "r");
    if (!fp) {
        printf(RED "Error opening parts file.\n" RESET);
        return;
    }

    Part arr[100];
    int c = 0;
    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        // Safely parse part data
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;
        token = strtok(temp_line, ","); if (!token) continue; strncpy(arr[c].id, token, sizeof(arr[c].id)-1); arr[c].id[sizeof(arr[c].id)-1] = '\0';
        token = strtok(NULL, ","); if (!token) continue; strncpy(arr[c].name, token, sizeof(arr[c].name)-1); arr[c].name[sizeof(arr[c].name)-1] = '\0';
        token = strtok(NULL, ","); if (!token) continue; arr[c].quantity = atoi(token);
        token = strtok(NULL, ","); if (!token) continue; arr[c].price = atof(token);
        token = strtok(NULL, ","); if (!token) continue; strncpy(arr[c].supplierId, token, sizeof(arr[c].supplierId)-1); arr[c].supplierId[sizeof(arr[c].supplierId)-1] = '\0';
        token = strtok(NULL, "\n"); if (!token) continue; arr[c].reorderLevel = atoi(token);
        c++;
    }
    fclose(fp);

    char searchId[10];
    int found = 0;
    printf(YELLOW "\nEnter part ID to edit: " RESET);
    scanf("%9s", searchId);

    for (int i = 0; i < c; i++) {
        if (strcmp(arr[i].id, searchId) == 0) {
            printf(YELLOW "Editing part: %s\n" RESET, arr[i].name);
            printf(YELLOW "Enter new name (%s): " RESET, arr[i].name);
            clearInputBuffer();
            fgets(arr[i].name, sizeof(arr[i].name), stdin);
            arr[i].name[strcspn(arr[i].name, "\n")] = 0;

            printf(YELLOW "Enter new quantity (%d): " RESET, arr[i].quantity);
            scanf("%d", &arr[i].quantity);
            printf(YELLOW "Enter new price (%.2f): " RESET, arr[i].price);
            scanf("%f", &arr[i].price);
            printf(YELLOW "Enter new supplier ID (%s): " RESET, arr[i].supplierId);
            scanf("%9s", arr[i].supplierId);
            printf(YELLOW "Enter new reorder level (%d): " RESET, arr[i].reorderLevel);
            scanf("%d", &arr[i].reorderLevel);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf(RED "Part with ID %s not found.\n" RESET, searchId);
        pauseScreen();
        return;
    }

    fp = fopen(PART_FILE, "w");
    if (!fp) {
        printf(RED "Error opening parts file for writing.\n" RESET);
        return;
    }
    for (int i = 0; i < c; i++) {
        fprintf(fp, "%s,%s,%d,%.2f,%s,%d\n", arr[i].id, arr[i].name, arr[i].quantity, arr[i].price, arr[i].supplierId, arr[i].reorderLevel);
    }
    fclose(fp);
    printf(GREEN "Part details updated successfully!\n" RESET);
    checkLowStock();
    char log_msg[100];
    sprintf(log_msg, "Part with ID %s updated.", searchId);
    writeLog(log_msg);
    pauseScreen();
}

void viewInventory() {
    FILE *fp = fopen(PART_FILE, "r");
    if (!fp) {
        printf(RED "No parts found in inventory.\n" RESET);
        return;
    }

    char line[200];
    printf("\n" CYAN "--- Inventory ---" RESET "\n");
    printf("ID,Name,Quantity,Price,Supplier,Reorder Level\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
    pauseScreen();
}

int partExists(const char *id) {
    FILE *fp = fopen(PART_FILE, "r");
    if (!fp) return 0;
    char line[200];
    while(fgets(line, sizeof(line), fp)) {
        if (strstr(line, id) == line) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

float getPartPrice(const char *partId) {
    FILE *fp = fopen(PART_FILE, "r");
    if (!fp) return -1.0;
    char line[200];
    while(fgets(line, sizeof(line), fp)) {
        char temp_line[200];
        strcpy(temp_line, line);
        char* token = strtok(temp_line, ",");
        if (token && strcmp(token, partId) == 0) {
            token = strtok(NULL, ","); // skip name
            token = strtok(NULL, ","); // skip quantity
            token = strtok(NULL, ","); // price
            if (token) {
                float price = atof(token);
                fclose(fp);
                return price;
            }
        }
    }
    fclose(fp);
    return -1.0;
}

void deductPartFromInventory(const char *partId, int quantity) {
    FILE *fp_read = fopen(PART_FILE, "r");
    if (!fp_read) return;
    FILE *fp_write = fopen("temp_parts.txt", "w");
    if (!fp_write) {
        fclose(fp_read);
        return;
    }

    char line[200];
    while (fgets(line, sizeof(line), fp_read)) {
        char temp_line[200];
        strcpy(temp_line, line);
        char* token = strtok(temp_line, ",");
        if (token && strcmp(token, partId) == 0) {
            Part p;
            strcpy(p.id, token);
            token = strtok(NULL, ","); if (!token) continue; strcpy(p.name, token);
            token = strtok(NULL, ","); if (!token) continue; p.quantity = atoi(token);
            token = strtok(NULL, ","); if (!token) continue; p.price = atof(token);
            token = strtok(NULL, ","); if (!token) continue; strcpy(p.supplierId, token);
            token = strtok(NULL, "\n"); if (!token) continue; p.reorderLevel = atoi(token);
            
            p.quantity -= quantity;
            if (p.quantity < 0) p.quantity = 0;
            fprintf(fp_write, "%s,%s,%d,%.2f,%s,%d\n", p.id, p.name, p.quantity, p.price, p.supplierId, p.reorderLevel);
        } else {
            fputs(line, fp_write);
        }
    }
    fclose(fp_read);
    fclose(fp_write);
    remove(PART_FILE);
    rename("temp_parts.txt", PART_FILE);
}

void checkLowStock() {
    FILE *fp = fopen(PART_FILE, "r");
    if (!fp) return;
    FILE *fp_reorder = fopen(REORDER_LIST_FILE, "w");
    if (!fp_reorder) {
        fclose(fp);
        return;
    }
    
    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        char temp_line[200];
        strcpy(temp_line, line);
        char* token;
        Part p;
        token = strtok(temp_line, ","); if (!token) continue; strncpy(p.id, token, sizeof(p.id)-1); p.id[sizeof(p.id)-1] = '\0';
        token = strtok(NULL, ","); if (!token) continue; strncpy(p.name, token, sizeof(p.name)-1); p.name[sizeof(p.name)-1] = '\0';
        token = strtok(NULL, ","); if (!token) continue; p.quantity = atoi(token);
        token = strtok(NULL, ","); if (!token) continue; p.price = atof(token);
        token = strtok(NULL, ","); if (!token) continue; strncpy(p.supplierId, token, sizeof(p.supplierId)-1); p.supplierId[sizeof(p.supplierId)-1] = '\0';
        token = strtok(NULL, "\n"); if (!token) continue; p.reorderLevel = atoi(token);

        if (p.quantity <= p.reorderLevel) {
            fprintf(fp_reorder, "%s,%s,%d,%d\n", p.id, p.name, p.quantity, p.reorderLevel);
            // createNotification("Admin", "Low stock alert: Part " + p.id + " is running low."); // Placeholder
        }
    }
    fclose(fp);
    fclose(fp_reorder);
}

void viewReorderList() {
    FILE *fp = fopen(REORDER_LIST_FILE, "r");
    if (!fp) {
        printf(GREEN "No low stock parts found.\n" RESET);
        return;
    }

    char line[200];
    printf("\n" CYAN "--- Low Stock Reorder List ---" RESET "\n");
    printf("Part ID,Part Name,Current Quantity,Reorder Level\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
    pauseScreen();
}

void clearReorderList() {
    remove(REORDER_LIST_FILE);
    printf(GREEN "Reorder list cleared successfully.\n" RESET);
    writeLog("Reorder list cleared.");
    pauseScreen();
}

// Supplier Management Functions
void addSupplier() {
    FILE *fp = fopen(SUPPLIER_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening supplier file.\n" RESET);
        return;
    }
    Supplier s;
    int count = fileLineCount(SUPPLIER_FILE);
    generateId("SUP", count + 1, s.id);

    printf(YELLOW "\nEnter supplier name: " RESET);
    clearInputBuffer();
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = 0;
    
    printf(YELLOW "Enter supplier phone: " RESET);
    scanf("%14s", s.phone);
    printf(YELLOW "Enter supplier email: " RESET);
    scanf("%49s", s.email);

    fprintf(fp, "%s,%s,%s,%s\n", s.id, s.name, s.phone, s.email);
    fclose(fp);
    printf(GREEN "Supplier added successfully! ID: %s\n" RESET, s.id);
    char log_msg[100];
    sprintf(log_msg, "New supplier %s added with ID %s.", s.name, s.id);
    writeLog(log_msg);
    pauseScreen();
}

void viewAllSuppliers() {
    FILE *fp = fopen(SUPPLIER_FILE, "r");
    if (!fp) {
        printf(RED "No suppliers found.\n" RESET);
        return;
    }

    char line[200];
    printf("\n" CYAN "--- All Suppliers ---" RESET "\n");
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
    char line[200];
    while(fgets(line, sizeof(line), fp)) {
        if (strstr(line, id) == line) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// Appointment Management Functions
void scheduleAppointment(const char *currentUserId, const char *currentUserRole) {
    FILE *fp = fopen(APPOINTMENT_FILE, "a+");
    if (!fp) {
        printf(RED "Error opening appointment file.\n" RESET);
        return;
    }
    Appointment a;
    int count = fileLineCount(APPOINTMENT_FILE);
    generateId("APP", count + 1, a.id);

    if (strcmp(currentUserRole, "Customer") == 0) {
        strcpy(a.customerId, currentUserId);
        printf(YELLOW "Enter your vehicle ID: " RESET);
        scanf("%9s", a.vehicleId);
    } else {
        printf(YELLOW "Enter customer ID: " RESET);
        scanf("%9s", a.customerId);
        printf(YELLOW "Enter vehicle ID: " RESET);
        scanf("%9s", a.vehicleId);
    }
    clearInputBuffer();

    if (!customerExists(a.customerId) || !vehicleExists(a.vehicleId)) {
        printf(RED "Customer or Vehicle ID does not exist.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    }
    
    printf(YELLOW "Enter appointment date (YYYY-MM-DD): " RESET);
    scanf("%14s", a.date);
    if (!isDateValid(a.date)) {
        printf(RED "Invalid date format.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    }
    
    printf(YELLOW "Enter appointment time (HH:MM): " RESET);
    scanf("%9s", a.time);
    clearInputBuffer();
    
    printf(YELLOW "Enter appointment description: " RESET);
    fgets(a.description, sizeof(a.description), stdin);
    a.description[strcspn(a.description, "\n")] = 0;
    
    strcpy(a.status, "Scheduled");

    fprintf(fp, "%s,%s,%s,%s,%s,%s,%s\n", a.id, a.customerId, a.vehicleId, a.date, a.time, a.description, a.status);
    fclose(fp);
    printf(GREEN "Appointment scheduled successfully! ID: %s\n" RESET, a.id);
    char log_msg[100];
    sprintf(log_msg, "New appointment %s scheduled for customer %s.", a.id, a.customerId);
    writeLog(log_msg);
    createNotification(a.customerId, "Your appointment has been scheduled.");
    pauseScreen();
}

void viewAllAppointments() {
    FILE *fp = fopen(APPOINTMENT_FILE, "r");
    if (!fp) {
        printf(RED "No appointments found.\n" RESET);
        return;
    }

    char line[200];
    printf("\n" CYAN "--- All Appointments ---" RESET "\n");
    printf("ID,Customer ID,Vehicle ID,Date,Time,Description,Status\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
    pauseScreen();
}

void scheduleServiceReminder() {
    char customerId[10], vehicleId[10], nextServiceDate[15];
    printf(YELLOW "\nEnter customer ID for the reminder: " RESET);
    scanf("%9s", customerId);
    printf(YELLOW "Enter vehicle ID: " RESET);
    scanf("%9s", vehicleId);
    clearInputBuffer();
    
    if (!customerExists(customerId) || !vehicleExists(vehicleId)) {
        printf(RED "Customer or Vehicle ID does not exist.\n" RESET);
        pauseScreen();
        return;
    }
    
    FILE *fp_veh = fopen(VEHICLE_FILE, "r");
    if (!fp_veh) {
        printf(RED "Error opening vehicle file.\n" RESET);
        return;
    }

    char line[200];
    char lastServiceDate[15];
    int found = 0;
    while(fgets(line, sizeof(line), fp_veh)) {
        char temp_line[200];
        strcpy(temp_line, line);
        char* token = strtok(temp_line, ",");
        if (token && strcmp(token, vehicleId) == 0) {
            token = strtok(NULL, ","); // customerId
            if (token && strcmp(token, customerId) == 0) {
                token = strtok(NULL, ","); // brand
                token = strtok(NULL, ","); // model
                token = strtok(NULL, ","); // plate
                token = strtok(NULL, ","); // year
                token = strtok(NULL, "\n"); // lastServiceDate
                if (token) strcpy(lastServiceDate, token);
                found = 1;
                break;
            }
        }
    }
    fclose(fp_veh);
    if (!found) {
        printf(RED "Vehicle %s not found for customer %s.\n" RESET, vehicleId, customerId);
        pauseScreen();
        return;
    }

    calculateNextServiceDate(lastServiceDate, nextServiceDate);
    
    FILE *fp_rem = fopen(REMINDERS_FILE, "a+");
    if (!fp_rem) {
        printf(RED "Error opening reminders file.\n" RESET);
        return;
    }
    Reminder r;
    int count = fileLineCount(REMINDERS_FILE);
    generateId("REM", count + 1, r.id);
    strcpy(r.customerId, customerId);
    sprintf(r.message, "Service reminder for your vehicle VEH%s. Next service date: %s", vehicleId, nextServiceDate);
    strcpy(r.date, nextServiceDate);
    strcpy(r.status, "Scheduled");
    
    fprintf(fp_rem, "%s,%s,%s,%s,%s\n", r.id, r.customerId, r.message, r.date, r.status);
    fclose(fp_rem);
    printf(GREEN "Service reminder scheduled for %s successfully.\n" RESET, nextServiceDate);
    char log_msg[100];
    sprintf(log_msg, "Service reminder for customer %s scheduled for %s.", customerId, nextServiceDate);
    writeLog(log_msg);
    pauseScreen();
}

void viewAllReminders() {
    FILE *fp = fopen(REMINDERS_FILE, "r");
    if (!fp) {
        printf(RED "No reminders found.\n" RESET);
        return;
    }

    char line[200];
    printf("\n" CYAN "--- All Reminders ---" RESET "\n");
    printf("ID,Customer ID,Message,Date,Status\n");
    while (fgets(line, sizeof(line), fp)) {
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
    int count = fileLineCount(EXPENSES_FILE);
    generateId("EXP", count + 1, t.id);
    strcpy(t.type, "Expense");
    
    printf(YELLOW "\nEnter expense amount: " RESET);
    scanf("%f", &t.amount);
    clearInputBuffer();
    
    printf(YELLOW "Enter date (YYYY-MM-DD): " RESET);
    scanf("%14s", t.date);
    clearInputBuffer();

    if (!isDateValid(t.date)) {
        printf(RED "Invalid date format.\n" RESET);
        fclose(fp);
        pauseScreen();
        return;
    }
    
    printf(YELLOW "Enter description: " RESET);
    fgets(t.description, sizeof(t.description), stdin);
    t.description[strcspn(t.description, "\n")] = 0;
    
    fprintf(fp, "%s,%s,%.2f,%s,%s\n", t.id, t.type, t.amount, t.date, t.description);
    fclose(fp);
    printf(GREEN "Expense added successfully! ID: %s\n" RESET, t.id);
    char log_msg[100];
    sprintf(log_msg, "New expense %s recorded, amount %.2f.", t.id, t.amount);
    writeLog(log_msg);
    pauseScreen();
}

void viewAllExpenses() {
    FILE *fp = fopen(EXPENSES_FILE, "r");
    if (!fp) {
        printf(RED "No expenses found.\n" RESET);
        return;
    }

    char line[200];
    printf("\n" CYAN "--- All Expenses ---" RESET "\n");
    printf("ID,Type,Amount,Date,Description\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
    pauseScreen();
}

void generateProfitLossReport() {
    float totalRevenue = 0.0;
    float totalExpenses = 0.0;
    float netProfitLoss = 0.0;
    
    FILE *fp_rev = fopen(FINANCE_FILE, "r");
    if (fp_rev) {
        char line[200];
        while(fgets(line, sizeof(line), fp_rev)) {
            char* token = strtok(line, ",");
            token = strtok(NULL, ",");
            token = strtok(NULL, ",");
            if (token) totalRevenue += atof(token);
        }
        fclose(fp_rev);
    }

    FILE *fp_exp = fopen(EXPENSES_FILE, "r");
    if (fp_exp) {
        char line[200];
        while(fgets(line, sizeof(line), fp_exp)) {
            char* token = strtok(line, ",");
            token = strtok(NULL, ",");
            token = strtok(NULL, ",");
            if (token) totalExpenses += atof(token);
        }
        fclose(fp_exp);
    }

    netProfitLoss = totalRevenue - totalExpenses;

    printf("\n" CYAN "--- Profit and Loss Report ---" RESET "\n");
    printf(MAGENTA "Total Revenue: %.2f\n" RESET, totalRevenue);
    printf(MAGENTA "Total Expenses: %.2f\n" RESET, totalExpenses);
    printf(BLUE "------------------------" RESET "\n");
    if (netProfitLoss >= 0) {
        printf(GREEN "Net Profit: %.2f\n" RESET, netProfitLoss);
    } else {
        printf(RED "Net Loss: %.2f\n" RESET, netProfitLoss);
    }
    printf(BLUE "------------------------" RESET "\n");
    writeLog("Profit and Loss report generated.");
    pauseScreen();
}

// Reporting Functions
void viewJobSummaryReport() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }

    int total_jobs = 0, pending = 0, inprogress = 0, completed = 0, cancelled = 0;
    float total_revenue = 0, total_parts = 0, total_labor = 0;
    char line[500];
    while (fgets(line, sizeof(line), fp)) {
        total_jobs++;
        Job j;
        char temp_line[500];
        strcpy(temp_line, line);
        char* token = strtok(temp_line, ",");
        token = strtok(NULL, ","); token = strtok(NULL, ","); token = strtok(NULL, ",");
        token = strtok(NULL, ",");
        token = strtok(NULL, ","); if(token) strcpy(j.status, token);
        token = strtok(NULL, ","); token = strtok(NULL, ",");
        if (token) j.laborCost = atof(token);
        token = strtok(NULL, ",");
        if (token) j.partsCost = atof(token);
        
        if (strcmp(j.status, "Pending") == 0) pending++;
        else if (strcmp(j.status, "InProgress") == 0) inprogress++;
        else if (strcmp(j.status, "Completed") == 0) {
            completed++;
            total_labor += j.laborCost;
            total_parts += j.partsCost;
            total_revenue += (j.laborCost + j.partsCost);
        }
        else if (strcmp(j.status, "Cancelled") == 0) cancelled++;
    }
    fclose(fp);

    printf("\n" CYAN "--- Job Summary Report ---" RESET "\n");
    printf("Total Jobs: %d\n", total_jobs);
    printf("Pending: %d\n", pending);
    printf("In Progress: %d\n", inprogress);
    printf("Completed: %d\n", completed);
    printf("Cancelled: %d\n", cancelled);
    printf(BLUE "------------------------" RESET "\n");
    printf("Total Revenue from Completed Jobs: %.2f\n", total_revenue);
    printf("Total Labor Cost: %.2f\n", total_labor);
    printf("Total Parts Cost: %.2f\n", total_parts);
    writeLog("Job Summary report generated.");
    pauseScreen();
}

void mechanicPerformanceReport() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }
    
    // Using a temporary file to store unique mechanics and their job counts/revenue
    FILE *fp_temp = fopen("temp_mechanic_report.txt", "w+");
    if (!fp_temp) {
        fclose(fp);
        return;
    }

    char line[500];
    while (fgets(line, sizeof(line), fp)) {
        char temp_line[500];
        strcpy(temp_line, line);
        char* token;
        Job j;
        token = strtok(temp_line, ","); if (!token) continue; strncpy(j.id, token, sizeof(j.id)-1); j.id[sizeof(j.id)-1] = '\0';
        token = strtok(NULL, ","); token = strtok(NULL, ",");
        token = strtok(NULL, ","); if (!token) continue; strncpy(j.mechanicId, token, sizeof(j.mechanicId)-1); j.mechanicId[sizeof(j.mechanicId)-1] = '\0';
        token = strtok(NULL, ",");
        token = strtok(NULL, ","); if (!token) continue; strncpy(j.status, token, sizeof(j.status)-1); j.status[sizeof(j.status)-1] = '\0';
        token = strtok(NULL, ","); token = strtok(NULL, ",");
        token = strtok(NULL, ","); if (!token) continue; j.partsCost = atof(token);
        token = strtok(NULL, ","); token = strtok(NULL, ",");
        token = strtok(NULL, "\n"); if (!token) continue; j.laborCost = atof(token);
        
        if (strcmp(j.mechanicId, "N/A") != 0 && strcmp(j.status, "Completed") == 0) {
            // Append or update mechanic's stats in temp file
            char temp_report_line[100];
            int found_in_temp = 0;
            rewind(fp_temp);
            while (fgets(temp_report_line, sizeof(temp_report_line), fp_temp)) {
                if (strstr(temp_report_line, j.mechanicId) == temp_report_line) {
                    // Update existing entry
                    //... (this part is complex and needs more robust file handling)
                    found_in_temp = 1;
                    break;
                }
            }
            if (!found_in_temp) {
                fprintf(fp_temp, "%s,%f,%d\n", j.mechanicId, j.laborCost + j.partsCost, 1);
            }
        }
    }
    fclose(fp);
    
    printf("\n" CYAN "--- Mechanic Performance Report ---" RESET "\n");
    // This part would read from the temp file and display the stats, but the logic is incomplete above
    printf("This feature requires more complex file handling to be fully functional.\n");
    writeLog("Mechanic performance report generated (partial).");
    fclose(fp_temp);
    remove("temp_mechanic_report.txt");
    pauseScreen();
}

void customerLifetimeValueReport() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }

    // This feature also requires more complex file handling and data structures
    printf("\n" CYAN "--- Customer Lifetime Value Report ---" RESET "\n");
    printf("This feature requires more complex data aggregation and file handling to be fully functional.\n");
    writeLog("Customer Lifetime Value report generated (placeholder).");
    fclose(fp);
    pauseScreen();
}

void monthlyRevenueReport() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf(RED "No jobs found.\n" RESET);
        return;
    }

    // This feature requires data aggregation by month
    printf("\n" CYAN "--- Monthly Revenue Report ---" RESET "\n");
    printf("This feature requires data aggregation and more complex logic to be fully functional.\n");
    writeLog("Monthly Revenue report generated (placeholder).");
    fclose(fp);
    pauseScreen();
}

void partUsageReport() {
    FILE *fp = fopen(JOB_PART_FILE, "r");
    if (!fp) {
        printf(RED "No parts usage data found.\n" RESET);
        return;
    }

    // This feature requires data aggregation by part
    printf("\n" CYAN "--- Part Usage Report ---" RESET "\n");
    printf("This feature requires data aggregation and more complex logic to be fully functional.\n");
    writeLog("Part Usage report generated (placeholder).");
    fclose(fp);
    pauseScreen();
}

void supplierPerformanceReport() {
    FILE *fp = fopen(SUPPLIER_FILE, "r");
    if (!fp) {
        printf(RED "No suppliers found.\n" RESET);
        return;
    }

    // This feature requires linking supplier data to part usage data
    printf("\n" CYAN "--- Supplier Performance Report ---" RESET "\n");
    printf("This feature requires linking data from multiple files and more complex logic to be fully functional.\n");
    writeLog("Supplier Performance report generated (placeholder).");
    fclose(fp);
    pauseScreen();
}

void customerRetentionReport() {
    FILE *fp = fopen(CUSTOMER_FILE, "r");
    if (!fp) {
        printf(RED "No customers found.\n" RESET);
        return;
    }

    // This feature requires analyzing repeat customers
    printf("\n" CYAN "--- Customer Retention Report ---" RESET "\n");
    printf("This feature requires analyzing customer job history and more complex logic to be fully functional.\n");
    writeLog("Customer Retention report generated (placeholder).");
    fclose(fp);
    pauseScreen();
}

// Menu Functions
void adminMenu(const char *uid) {
    int ch;
    while (1) {
        printf("\n" CYAN "--- Admin Menu ---" RESET "\n");
        printf("1. Dashboard\n");
        printf("2. Manage Users\n");
        printf("3. Manage Customers\n");
        printf("4. Manage Vehicles\n");
        printf("5. Manage Jobs\n");
        printf("6. Manage Invoices\n");
        printf("7. Manage Inventory\n");
        printf("8. Manage Suppliers\n");
        printf("9. Manage Appointments\n");
        printf("10. Financial Management\n");
        printf("11. Reporting\n");
        printf("12. Backup/Restore Data\n");
        printf("13. Change Password\n");
        printf("0. Logout\n");
        printf(YELLOW "Enter choice: " RESET);
        scanf("%d", &ch);

        switch(ch) {
            case 1: showDashboard(); break;
            case 2: {
                int sub_ch;
                printf("\n" CYAN "--- Manage Users ---" RESET "\n");
                printf("1. View All Users\n");
                printf("2. Register New User\n");
                printf("3. Change User Role\n");
                printf("0. Back\n");
                printf(YELLOW "Enter choice: " RESET);
                scanf("%d", &sub_ch);
                switch(sub_ch) {
                    case 1: viewAllUsers(); break;
                    case 2: registerUser(); break;
                    case 3: changeUserRole(); break;
                    case 0: break;
                    default: printf(RED "Invalid choice.\n" RESET); pauseScreen();
                }
                break;
            }
            case 3: {
                int sub_ch;
                printf("\n" CYAN "--- Manage Customers ---" RESET "\n");
                printf("1. Add Customer\n");
                printf("2. Edit Customer\n");
                printf("3. Delete Customer\n");
                printf("4. Search Customer\n");
                printf("5. View All Customers\n");
                printf("6. Send Feedback Request\n");
                printf("0. Back\n");
                printf(YELLOW "Enter choice: " RESET);
                scanf("%d", &sub_ch);
                switch(sub_ch) {
                    case 1: addCustomer(); break;
                    case 2: editCustomer(); break;
                    case 3: deleteCustomer(); break;
                    case 4: searchCustomer(); break;
                    case 5: viewAllCustomers(); break;
                    case 6: sendFeedbackRequest(); break;
                    case 0: break;
                    default: printf(RED "Invalid choice.\n" RESET); pauseScreen();
                }
                break;
            }
            case 4: {
                int sub_ch;
                printf("\n" CYAN "--- Manage Vehicles ---" RESET "\n");
                printf("1. Add Vehicle\n");
                printf("2. Edit Vehicle\n");
                printf("3. Delete Vehicle\n");
                printf("4. Search Vehicle\n");
                printf("5. View All Vehicles\n");
                printf("0. Back\n");
                printf(YELLOW "Enter choice: " RESET);
                scanf("%d", &sub_ch);
                switch(sub_ch) {
                    case 1: addVehicle(); break;
                    case 2: editVehicle(); break;
                    case 3: deleteVehicle(); break;
                    case 4: searchVehicle(); break;
                    case 5: viewAllVehicles(); break;
                    case 0: break;
                    default: printf(RED "Invalid choice.\n" RESET); pauseScreen();
                }
                break;
            }
            case 5: {
                int sub_ch;
                printf("\n" CYAN "--- Manage Jobs ---" RESET "\n");
                printf("1. Add Job\n");
                printf("2. Update Job Status\n");
                printf("3. Search Jobs\n");
                printf("4. View All Jobs\n");
                printf("5. View Pending Jobs\n");
                printf("6. View Completed Jobs\n");
                printf("7. Add Part to Job\n");
                printf("8. Update Job Labor Cost\n");
                printf("0. Back\n");
                printf(YELLOW "Enter choice: " RESET);
                scanf("%d", &sub_ch);
                switch(sub_ch) {
                    case 1: addJob(uid, "Admin"); break;
                    case 2: updateJobStatus(uid, "Admin"); break;
                    case 3: searchJob(); break;
                    case 4: viewAllJobs(); break;
                    case 5: viewPendingJobs(); break;
                    case 6: viewCompletedJobs(); break;
                    case 7: addPartToJob(); break;
                    case 8: updateJobLaborCost(); break;
                    case 0: break;
                    default: printf(RED "Invalid choice.\n" RESET); pauseScreen();
                }
                break;
            }
            case 6: {
                int sub_ch;
                printf("\n" CYAN "--- Manage Invoices ---" RESET "\n");
                printf("1. View All Invoices\n");
                printf("0. Back\n");
                printf(YELLOW "Enter choice: " RESET);
                scanf("%d", &sub_ch);
                switch(sub_ch) {
                    case 1: viewAllInvoices(); break;
                    case 0: break;
                    default: printf(RED "Invalid choice.\n" RESET); pauseScreen();
                }
                break;
            }
            case 7: {
                int sub_ch;
                printf("\n" CYAN "--- Manage Inventory ---" RESET "\n");
                printf("1. Add Part\n");
                printf("2. Edit Part\n");
                printf("3. View Inventory\n");
                printf("4. View Reorder List\n");
                printf("5. Clear Reorder List\n");
                printf("0. Back\n");
                printf(YELLOW "Enter choice: " RESET);
                scanf("%d", &sub_ch);
                switch(sub_ch) {
                    case 1: addPart(); break;
                    case 2: editPart(); break;
                    case 3: viewInventory(); break;
                    case 4: viewReorderList(); break;
                    case 5: clearReorderList(); break;
                    case 0: break;
                    default: printf(RED "Invalid choice.\n" RESET); pauseScreen();
                }
                break;
            }
            case 8: {
                int sub_ch;
                printf("\n" CYAN "--- Manage Suppliers ---" RESET "\n");
                printf("1. Add Supplier\n");
                printf("2. View All Suppliers\n");
                printf("0. Back\n");
                printf(YELLOW "Enter choice: " RESET);
                scanf("%d", &sub_ch);
                switch(sub_ch) {
                    case 1: addSupplier(); break;
                    case 2: viewAllSuppliers(); break;
                    case 0: break;
                    default: printf(RED "Invalid choice.\n" RESET); pauseScreen();
                }
                break;
            }
            case 9: {
                int sub_ch;
                printf("\n" CYAN "--- Manage Appointments ---" RESET "\n");
                printf("1. Schedule Appointment\n");
                printf("2. View All Appointments\n");
                printf("3. Schedule Service Reminder\n");
                printf("4. View All Reminders\n");
                printf("0. Back\n");
                printf(YELLOW "Enter choice: " RESET);
                scanf("%d", &sub_ch);
                switch(sub_ch) {
                    case 1: scheduleAppointment(uid, "Admin"); break;
                    case 2: viewAllAppointments(); break;
                    case 3: scheduleServiceReminder(); break;
                    case 4: viewAllReminders(); break;
                    case 0: break;
                    default: printf(RED "Invalid choice.\n" RESET); pauseScreen();
                }
                break;
            }
            case 10: {
                int sub_ch;
                printf("\n" CYAN "--- Financial Management ---" RESET "\n");
                printf("1. Add Expense\n");
                printf("2. View All Expenses\n");
                printf("3. Generate Profit & Loss Report\n");
                printf("0. Back\n");
                printf(YELLOW "Enter choice: " RESET);
                scanf("%d", &sub_ch);
                switch(sub_ch) {
                    case 1: addExpense(); break;
                    case 2: viewAllExpenses(); break;
                    case 3: generateProfitLossReport(); break;
                    case 0: break;
                    default: printf(RED "Invalid choice.\n" RESET); pauseScreen();
                }
                break;
            }
            case 11: {
                int sub_ch;
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
                scanf("%d", &sub_ch);
                switch(sub_ch) {
                    case 1: viewJobSummaryReport(); break;
                    case 2: mechanicPerformanceReport(); break;
                    case 3: customerLifetimeValueReport(); break;
                    case 4: monthlyRevenueReport(); break;
                    case 5: partUsageReport(); break;
                    case 6: supplierPerformanceReport(); break;
                    case 7: customerRetentionReport(); break;
                    case 0: break;
                    default: printf(RED "Invalid choice.\n" RESET); pauseScreen();
                }
                break;
            }
            case 12: {
                int sub_ch;
                printf("\n" CYAN "--- Backup/Restore ---" RESET "\n");
                printf("1. Backup Data\n");
                printf("2. Restore Data\n");
                printf("0. Back\n");
                printf(YELLOW "Enter choice: " RESET);
                scanf("%d", &sub_ch);
                switch(sub_ch) {
                    case 1: backupData(); break;
                    case 2: restoreData(); break;
                    case 0: break;
                    default: printf(RED "Invalid choice.\n" RESET); pauseScreen();
                }
                break;
            }
            case 13: changePassword(uid); break;
            case 0: return;
            default: printf(RED "Invalid choice. Please try again.\n" RESET); pauseScreen();
        }
    }
}

void seniorMechanicMenu(const char *uid) {
    int ch;
    while (1) {
        printf("\n" CYAN "--- Senior Mechanic Menu ---" RESET "\n");
        printf("1. Dashboard\n");
        printf("2. View My Notifications\n");
        printf("3. Manage Jobs\n");
        printf("4. View All Customers\n");
        printf("5. View All Vehicles\n");
        printf("6. Manage Inventory\n");
        printf("7. Manage Appointments\n");
        printf("8. Change Password\n");
        printf("0. Logout\n");
        printf(YELLOW "Enter choice: " RESET);
        scanf("%d", &ch);
        switch(ch) {
            case 1: showDashboard(); break;
            case 2: viewNotifications(uid); break;
            case 3: {
                int sub_ch;
                printf("\n" CYAN "--- Manage Jobs ---" RESET "\n");
                printf("1. View My Jobs\n");
                printf("2. Update Job Status\n");
                printf("3. Add Part to Job\n");
                printf("4. Update Job Labor Cost\n");
                printf("5. View All Jobs\n");
                printf("0. Back\n");
                printf(YELLOW "Enter choice: " RESET);
                scanf("%d", &sub_ch);
                switch(sub_ch) {
                    case 1: viewMyJobs(uid); break;
                    case 2: updateJobStatus(uid, "SeniorMechanic"); break;
                    case 3: addPartToJob(); break;
                    case 4: updateJobLaborCost(); break;
                    case 5: viewAllJobs(); break;
                    case 0: break;
                    default: printf(RED "Invalid choice.\n" RESET); pauseScreen();
                }
                break;
            }
            case 4: viewAllCustomers(); break;
            case 5: viewAllVehicles(); break;
            case 6: {
                int sub_ch;
                printf("\n" CYAN "--- Manage Inventory ---" RESET "\n");
                printf("1. Add Part\n");
                printf("2. Edit Part\n");
                printf("3. View Inventory\n");
                printf("4. View Reorder List\n");
                printf("0. Back\n");
                printf(YELLOW "Enter choice: " RESET);
                scanf("%d", &sub_ch);
                switch(sub_ch) {
                    case 1: addPart(); break;
                    case 2: editPart(); break;
                    case 3: viewInventory(); break;
                    case 4: viewReorderList(); break;
                    case 0: break;
                    default: printf(RED "Invalid choice.\n" RESET); pauseScreen();
                }
                break;
            }
            case 7: {
                int sub_ch;
                printf("\n" CYAN "--- Manage Appointments ---" RESET "\n");
                printf("1. View All Appointments\n");
                printf("2. Schedule Service Reminder\n");
                printf("0. Back\n");
                printf(YELLOW "Enter choice: " RESET);
                scanf("%d", &sub_ch);
                switch(sub_ch) {
                    case 1: viewAllAppointments(); break;
                    case 2: scheduleServiceReminder(); break;
                    case 0: break;
                    default: printf(RED "Invalid choice.\n" RESET); pauseScreen();
                }
                break;
            }
            case 8: changePassword(uid); break;
            case 0: return;
            default: printf(RED "Invalid choice. Please try again.\n" RESET); pauseScreen();
        }
    }
}

void juniorMechanicMenu(const char *uid) {
    int ch;
    while (1) {
        printf("\n" CYAN "--- Junior Mechanic Menu ---" RESET "\n");
        printf("1. View My Notifications\n");
        printf("2. View My Jobs\n");
        printf("3. Update Job Status\n");
        printf("4. View All Customers\n");
        printf("5. View All Vehicles\n");
        printf("6. View Inventory\n");
        printf("7. Change Password\n");
        printf("0. Logout\n");
        printf(YELLOW "Enter choice: " RESET);
        scanf("%d", &ch);
        switch(ch) {
            case 1: viewNotifications(uid); break;
            case 2: viewMyJobs(uid); break;
            case 3: updateJobStatus(uid, "JuniorMechanic"); break;
            case 4: viewAllCustomers(); break;
            case 5: viewAllVehicles(); break;
            case 6: viewInventory(); break;
            case 7: changePassword(uid); break;
            case 0: return;
            default: printf(RED "Invalid choice. Please try again.\n" RESET); pauseScreen();
        }
    }
}

void customerMenu(const char *uid) {
    int ch;
    while (1) {
        printf("\n" CYAN "--- Customer Menu ---" RESET "\n");
        printf("1. View My Notifications\n");
        printf("2. View My Vehicles\n");
        printf("3. View My Jobs\n");
        printf("4. Schedule Appointment\n");
        printf("5. Change Password\n");
        printf("0. Logout\n");
        printf(YELLOW "Enter choice: " RESET);
        scanf("%d", &ch);
        switch(ch) {
            case 1: viewNotifications(uid); break;
            case 2: viewCustomerVehicles(uid); break;
            case 3: viewCustomerJobs(uid); break;
            case 4: scheduleAppointment(uid, "Customer"); break;
            case 5: changePassword(uid); break;
            case 0: return;
            default: printf(RED "Invalid choice. Please try again.\n" RESET); pauseScreen();
        }
    }
}

// Main function
void printLogo() {
    printf(MAGENTA "        ____                                      _____\n" RESET);
    printf(MAGENTA "      / ____|                                   |  ___|\n" RESET);
    printf(MAGENTA "     | |  __ _ __ ___   _ __ _ __ ___   _ __ | |__ _ __ __ _  ___\n" RESET);
    printf(MAGENTA "     | | |_ | '__/ _ \\| '__| '__/ _ \\| '_ \\|  __| '__/ _` |/ __|\n" RESET);
    printf(MAGENTA "     | |__| | | | (_) | |  | | | (_) | | | | |___| | | (_| | (__\n" RESET);
    printf(MAGENTA "      \\_____|_|  \\___/|_|  |_|  \\___/|_| |_|_____|_|  \\__,_|\\___|\n" RESET);
    printf(MAGENTA "                                                                \n" RESET);
    printf(CYAN "                       A Simple Garage Management System\n" RESET);
    printf(BLUE "=================================================================\n" RESET);
}

int main() {
    int ch;
    char role[20], uid[10];

    printLogo();
    
    while (1) {
        FILE *fp = fopen(USER_FILE, "r");
        if (!fp) {
            printf(RED "\nNo users found. Please register an admin first.\n" RESET);
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
            fclose(fp);
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
    printf(CYAN "\nExiting the system. Goodbye!\n" RESET);
    return 0;
}