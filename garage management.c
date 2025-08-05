
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USER_FILE "users.txt"
#define CUSTOMER_FILE "customers.txt"
#define VEHICLE_FILE "vehicles.txt"
#define JOB_FILE "jobs.txt"
#define INVOICE_FILE "invoices.txt"

// ===================== STRUCTURES =====================
typedef struct {
    char id[10];
    char username[30];
    char password[30];
    char role[15];
} User;

typedef struct {
    char id[10];
    char name[50];
    char phone[15];
} Customer;

typedef struct {
    char id[10];
    char customerId[10];
    char brand[30];
    char model[30];
    char plate[15];
} Vehicle;

typedef struct {
    char id[10];
    char customerId[10];
    char vehicleId[10];
    char mechanicId[10];
    char description[100];
    char status[20];
} Job;

typedef struct {
    char id[10];
    char jobId[10];
    float cost;
    float discount;
    float total;
} Invoice;

// ===================== UTILITY FUNCTIONS =====================
void generateId(char *prefix, int num, char *id) {
    sprintf(id, "%s%03d", prefix, num);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pauseScreen() {
    printf("\nPress Enter to continue...");
    clearInputBuffer();
    getchar();
}

void hashPassword(char *pwd) {
    for (int i = 0; pwd[i] != '\0'; i++) pwd[i] += 3;
}

int fileLineCount(const char *fname) {
    FILE *fp = fopen(fname, "r");
    if (!fp) return 0;
    char buf[200];
    int c = 0;
    while (fgets(buf, sizeof(buf), fp)) c++;
    fclose(fp);
    return c;
}

// ===================== AUTHENTICATION =====================
int login(char *role, char *uid) {
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) {
        printf("No users found. Please register an admin first.\n");
        return 0;
    }

    char user[30], pass[30];
    int tries = 0;

    while (tries < 3) {
        printf("\nUsername: ");
        scanf("%29s", user);
        printf("Password: ");
        scanf("%29s", pass);
        hashPassword(pass);

        User u;
        char line[200];
        rewind(fp);
        int found = 0;
        while (fgets(line, 200, fp)) {
            sscanf(line, "%9[^,],%29[^,],%29[^,],%14[^\n]", u.id, u.username, u.password, u.role);
            if (!strcmp(u.username, user) && !strcmp(u.password, pass)) {
                strcpy(role, u.role);
                strcpy(uid, u.id);
                found = 1;
                break;
            }
        }
        if (found) {
            fclose(fp);
            return 1;
        }

        tries++;
        printf("Invalid! Attempts left:%d\n", 3 - tries);
    }
    fclose(fp);
    return 0;
}

void registerUser() {
    FILE *fp = fopen(USER_FILE, "a+");
    if (!fp) return;

    User u;
    char buf[200];
    int c = 0;

    while (fgets(buf, 200, fp)) c++;

    printf("\nEnter username: ");
    scanf("%29s", u.username);
    printf("Enter password: ");
    scanf("%29s", u.password);
    hashPassword(u.password);

    if (c == 0) {
        strcpy(u.role, "Admin"); // First user MUST be Admin
        printf("Role set to Admin (first user)\n");
    } else {
        printf("Enter role (Admin/Mechanic/Customer): ");
        scanf("%14s", u.role);
    }

    generateId("USR", c + 1, u.id);

    fprintf(fp, "%s,%s,%s,%s\n", u.id, u.username, u.password, u.role);
    fclose(fp);

    printf("User registered! ID:%s\n", u.id);
    pauseScreen();
}

void changePassword(char *uid) {
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) return;

    User arr[100];
    int c = 0;
    char line[200];

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%29[^,],%29[^,],%14[^\n]", arr[c].id, arr[c].username, arr[c].password, arr[c].role);
        c++;
    }
    fclose(fp);

    for (int i = 0; i < c; i++) {
        if (!strcmp(arr[i].id, uid)) {
            char np[30];
            printf("Enter new password: ");
            scanf("%29s", np);
            hashPassword(np);
            strcpy(arr[i].password, np);
        }
    }

    fp = fopen(USER_FILE, "w");
    for (int i = 0; i < c; i++)
        fprintf(fp, "%s,%s,%s,%s\n", arr[i].id, arr[i].username, arr[i].password, arr[i].role);
    fclose(fp);

    printf("Password changed!\n");
    pauseScreen();
}

// ===================== CUSTOMER MANAGEMENT =====================
void addCustomer() {
    FILE *fp = fopen(CUSTOMER_FILE, "a+");
    if (!fp) return;

    Customer c;
    int cnt = fileLineCount(CUSTOMER_FILE);

    printf("\nEnter name: ");
    clearInputBuffer();
    fgets(c.name, sizeof(c.name), stdin);
    c.name[strcspn(c.name, "\n")] = '\0';

    printf("Enter phone: ");
    scanf("%14s", c.phone);

    generateId("CUST", cnt + 1, c.id);

    fprintf(fp, "%s,%s,%s\n", c.id, c.name, c.phone);
    fclose(fp);

    printf("Customer added! ID:%s\n", c.id);
    pauseScreen();
}

int customerExists(char *id) {
    FILE *fp = fopen(CUSTOMER_FILE, "r");
    if (!fp) return 0;

    Customer c;
    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%14[^\n]", c.id, c.name, c.phone);
        if (!strcmp(c.id, id)) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// ===================== VEHICLE MANAGEMENT =====================
void addVehicle() {
    FILE *fp = fopen(VEHICLE_FILE, "a+");
    if (!fp) return;

    Vehicle v;
    int cnt = fileLineCount(VEHICLE_FILE);

    printf("\nEnter customerID: ");
    scanf("%9s", v.customerId);

    if (!customerExists(v.customerId)) {
        printf("Invalid customer ID!\n");
        pauseScreen();
        return;
    }

    printf("Brand: ");
    scanf("%29s", v.brand);
    printf("Model: ");
    scanf("%29s", v.model);
    printf("Plate: ");
    scanf("%14s", v.plate);

    generateId("VEH", cnt + 1, v.id);

    fprintf(fp, "%s,%s,%s,%s,%s\n", v.id, v.customerId, v.brand, v.model, v.plate);
    fclose(fp);

    printf("Vehicle added! ID:%s\n", v.id);
    pauseScreen();
}

int vehicleExists(char *id) {
    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) return 0;

    Vehicle v;
    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^\n]", v.id, v.customerId, v.brand, v.model, v.plate);
        if (!strcmp(v.id, id)) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// ===================== JOB MANAGEMENT =====================
void addJob(char *mechanicId) {
    FILE *fp = fopen(JOB_FILE, "a+");
    if (!fp) return;

    Job j;
    int cnt = fileLineCount(JOB_FILE);

    printf("\nEnter customerID: ");
    scanf("%9s", j.customerId);
    if (!customerExists(j.customerId)) {
        printf("Invalid customer ID!\n");
        pauseScreen();
        return;
    }

    printf("Enter vehicleID: ");
    scanf("%9s", j.vehicleId);
    if (!vehicleExists(j.vehicleId)) {
        printf("Invalid vehicle ID!\n");
        pauseScreen();
        return;
    }

    strcpy(j.mechanicId, mechanicId);
    printf("Enter job description: ");
    clearInputBuffer();
    fgets(j.description, sizeof(j.description), stdin);
    j.description[strcspn(j.description, "\n")] = '\0';
    strcpy(j.status, "Pending");

    generateId("JOB", cnt + 1, j.id);

    fprintf(fp, "%s,%s,%s,%s,%s,%s\n", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status);
    fclose(fp);

    printf("Job added! ID:%s\n", j.id);
    pauseScreen();
}

void updateJobStatus() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) return;

    Job arr[200];
    int c = 0;
    char line[300];

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^\n]", arr[c].id, arr[c].customerId, arr[c].vehicleId,
               arr[c].mechanicId, arr[c].description, arr[c].status);
        c++;
    }
    fclose(fp);

    char id[10];
    printf("Enter Job ID to update: ");
    scanf("%9s", id);

    for (int i = 0; i < c; i++) {
        if (!strcmp(arr[i].id, id)) {
            printf("Current status: %s\n", arr[i].status);
            printf("Enter new status (Pending/InProgress/Completed): ");
            scanf("%19s", arr[i].status);
        }
    }

    fp = fopen(JOB_FILE, "w");
    for (int i = 0; i < c; i++)
        fprintf(fp, "%s,%s,%s,%s,%s,%s\n", arr[i].id, arr[i].customerId, arr[i].vehicleId, arr[i].mechanicId, arr[i].description, arr[i].status);
    fclose(fp);

    printf("Job status updated!\n");
    pauseScreen();
}

// ===================== INVOICE MANAGEMENT =====================
void generateInvoice() {
    FILE *fp = fopen(INVOICE_FILE, "a+");
    if (!fp) return;

    Invoice inv;
    int cnt = fileLineCount(INVOICE_FILE);

    printf("\nEnter Job ID: ");
    scanf("%9s", inv.jobId);

    printf("Enter cost: ");
    scanf("%f", &inv.cost);
    printf("Enter discount: ");
    scanf("%f", &inv.discount);
    inv.total = inv.cost - inv.discount;

    generateId("INV", cnt + 1, inv.id);

    fprintf(fp, "%s,%s,%.2f,%.2f,%.2f\n", inv.id, inv.jobId, inv.cost, inv.discount, inv.total);
    fclose(fp);

    printf("Invoice generated! ID:%s Total:%.2f\n", inv.id, inv.total);
    pauseScreen();
}

// ===================== REPORTS =====================
void viewAllCustomers() {
    FILE *fp = fopen(CUSTOMER_FILE, "r");
    if (!fp) {
        printf("No customers found!\n");
        return;
    }
    char line[200];
    printf("\n--- Customers ---\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
    pauseScreen();
}

void viewAllJobs() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf("No jobs found!\n");
        return;
    }
    char line[300];
    printf("\n--- Jobs ---\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
    pauseScreen();
}

// ===================== MENUS =====================
void adminMenu(char *uid) {
    int ch;
    do {
        printf("\n--- Admin Menu ---\n");
        printf("1. Register User\n");
        printf("2. Add Customer\n");
        printf("3. Add Vehicle\n");
        printf("4. View All Customers\n");
        printf("5. View All Jobs\n");
        printf("6. Generate Invoice\n");
        printf("7. Change Password\n");
        printf("0. Logout\n");
        printf("Choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: registerUser(); break;
            case 2: addCustomer(); break;
            case 3: addVehicle(); break;
            case 4: viewAllCustomers(); break;
            case 5: viewAllJobs(); break;
            case 6: generateInvoice(); break;
            case 7: changePassword(uid); break;
        }
    } while (ch != 0);
}

void mechanicMenu(char *uid) {
    int ch;
    do {
        printf("\n--- Mechanic Menu ---\n");
        printf("1. Add Job\n");
        printf("2. Update Job Status\n");
        printf("3. View All Jobs\n");
        printf("4. Change Password\n");
        printf("0. Logout\n");
        printf("Choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: addJob(uid); break;
            case 2: updateJobStatus(); break;
            case 3: viewAllJobs(); break;
            case 4: changePassword(uid); break;
        }
    } while (ch != 0);
}

void customerMenu(char *uid) {
    int ch;
    do {
        printf("\n--- Customer Menu ---\n");
        printf("1. View All Jobs\n");
        printf("2. Change Password\n");
        printf("0. Logout\n");
        printf("Choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: viewAllJobs(); break;
            case 2: changePassword(uid); break;
        }
    } while (ch != 0);
}

// ===================== MAIN =====================
int main() {
    int ch;
    char role[15], uid[10];

    while (1) {
        FILE *fp = fopen(USER_FILE, "r");
        int hasUsers = (fp != NULL);
        if (fp) {
            fseek(fp, 0, SEEK_END);
            if (ftell(fp) == 0) hasUsers = 0;  // Empty file
            fclose(fp);
        }

        printf("\n=== GARAGE MANAGEMENT SYSTEM ===\n");
        if (!hasUsers) {
            printf("No users found. Please register an admin first.\n");
            printf("1. Register Admin\n0. Exit\nEnter choice: ");
            scanf("%d", &ch);

            if (ch == 1) {
                printf("\nRegister Admin Account:\n");
                registerUser();  // Only Admin should be allowed initially
            } else if (ch == 0) break;
        } else {
            printf("1. Login\n0. Exit\nEnter choice: ");
            scanf("%d", &ch);

            if (ch == 1) {
                if (login(role, uid)) {
                    if (!strcmp(role, "Admin")) adminMenu(uid);
                    else if (!strcmp(role, "Mechanic")) mechanicMenu(uid);
                    else if (!strcmp(role, "Customer")) customerMenu(uid);
                } else {
                    printf("Login failed after 3 attempts!\n");
                }
            } else if (ch == 0) break;
        }
    }
    return 0;
}
