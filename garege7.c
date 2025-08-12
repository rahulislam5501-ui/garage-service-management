#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define USER_FILE "users.txt"
#define CUSTOMER_FILE "customers.txt"
#define VEHICLE_FILE "vehicles.txt"
#define JOB_FILE "jobs.txt"
#define INVOICE_FILE "invoices.txt"

#define BLUE "\x1b[34m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"

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
    char email[50];
} Customer;

typedef struct {
    char id[10];
    char customerId[10];
    char brand[30];
    char model[30];
    char plate[15];
    char year[5];
} Vehicle;

typedef struct {
    char id[10];
    char customerId[10];
    char vehicleId[10];
    char mechanicId[10];
    char description[100];
    char status[20];
    char notes[100];
    float estimatedCost;
} Job;

typedef struct {
    char id[10];
    char jobId[10];
    float cost;
    float discount;
    float total;
    char date[15];
} Invoice;

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
    char buf[500];
    int c = 0;
    while (fgets(buf, sizeof(buf), fp)) c++;
    fclose(fp);
    return c;
}

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
        printf("Invalid username or password! Attempts left: %d\n", 3 - tries);
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

    rewind(fp);
    while (fgets(buf, 200, fp)) c++;

    printf("\nEnter username: ");
    scanf("%29s", u.username);

    rewind(fp);
    while (fgets(buf, 200, fp)) {
        User temp_u;
        sscanf(buf, "%9[^,],%29[^,],%29[^,],%14[^\n]", temp_u.id, temp_u.username, temp_u.password, temp_u.role);
        if (strcmp(temp_u.username, u.username) == 0) {
            printf("Username already exists. Please choose a different username.\n");
            fclose(fp);
            pauseScreen();
            return;
        }
    }

    printf("Enter password: ");
    scanf("%29s", u.password);
    hashPassword(u.password);

    if (c == 0) {
        strcpy(u.role, "Admin");
        printf("Role set to Admin (first user)\n");
    } else {
        printf("Enter role (Admin/Mechanic/Customer): ");
        scanf("%14s", u.role);
        if (strcmp(u.role, "Admin") != 0 && strcmp(u.role, "Mechanic") != 0 && strcmp(u.role, "Customer") != 0) {
            printf("Invalid role. User not registered.\n");
            fclose(fp);
            pauseScreen();
            return;
        }
    }

    generateId("USR", c + 1, u.id);

    fprintf(fp, "%s,%s,%s,%s\n", u.id, u.username, u.password, u.role);
    fclose(fp);

    printf("User registered successfully! ID: %s\n", u.id);
    pauseScreen();
}

void changePassword(char *uid) {
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) {
        printf("Error opening user file.\n");
        return;
    }

    User arr[100];
    int c = 0;
    char line[200];

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%29[^,],%29[^,],%14[^\n]", arr[c].id, arr[c].username, arr[c].password, arr[c].role);
        c++;
    }
    fclose(fp);

    int found = 0;
    for (int i = 0; i < c; i++) {
        if (!strcmp(arr[i].id, uid)) {
            char np[30];
            printf("Enter new password: ");
            scanf("%29s", np);
            hashPassword(np);
            strcpy(arr[i].password, np);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("User ID not found.\n");
        pauseScreen();
        return;
    }

    fp = fopen(USER_FILE, "w");
    if (!fp) {
        printf("Error opening user file for writing.\n");
        return;
    }
    for (int i = 0; i < c; i++)
        fprintf(fp, "%s,%s,%s,%s\n", arr[i].id, arr[i].username, arr[i].password, arr[i].role);
    fclose(fp);

    printf("Password changed successfully!\n");
    pauseScreen();
}

void addCustomer() {
    FILE *fp = fopen(CUSTOMER_FILE, "a+");
    if (!fp) {
        printf("Error opening customer file.\n");
        return;
    }

    Customer cust;
    int cnt = fileLineCount(CUSTOMER_FILE);

    printf("\nEnter customer name: ");
    clearInputBuffer();
    fgets(cust.name, sizeof(cust.name), stdin);
    cust.name[strcspn(cust.name, "\n")] = '\0';

    printf("Enter phone number: ");
    scanf("%14s", cust.phone);
    printf("Enter email: ");
    scanf("%49s", cust.email);

    generateId("CUST", cnt + 1, cust.id);

    fprintf(fp, "%s,%s,%s,%s\n", cust.id, cust.name, cust.phone, cust.email);
    fclose(fp);

    printf("Customer added successfully! ID: %s\n", cust.id);
    pauseScreen();
}

int customerExists(char *id) {
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

void editCustomer() {
    FILE *fp = fopen(CUSTOMER_FILE, "r");
    if (!fp) {
        printf("No customers found.\n");
        return;
    }

    Customer arr[100];
    int c = 0;
    char line[200];

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", arr[c].id, arr[c].name, arr[c].phone, arr[c].email);
        c++;
    }
    fclose(fp);

    char id[10];
    printf("\nEnter Customer ID to edit: ");
    scanf("%9s", id);

    int found = 0;
    for (int i = 0; i < c; i++) {
        if (!strcmp(arr[i].id, id)) {
            printf("Current Name: %s\n", arr[i].name);
            printf("Enter new name (or press Enter to keep current): ");
            clearInputBuffer();
            char newName[50];
            fgets(newName, sizeof(newName), stdin);
            newName[strcspn(newName, "\n")] = '\0';
            if (strlen(newName) > 0) strcpy(arr[i].name, newName);

            printf("Current Phone: %s\n", arr[i].phone);
            printf("Enter new phone (or press Enter to keep current): ");
            char newPhone[15];
            fgets(newPhone, sizeof(newPhone), stdin);
            newPhone[strcspn(newPhone, "\n")] = '\0';
            if (strlen(newPhone) > 0) strcpy(arr[i].phone, newPhone);

            printf("Current Email: %s\n", arr[i].email);
            printf("Enter new email (or press Enter to keep current): ");
            char newEmail[50];
            fgets(newEmail, sizeof(newEmail), stdin);
            newEmail[strcspn(newEmail, "\n")] = '\0';
            if (strlen(newEmail) > 0) strcpy(arr[i].email, newEmail);

            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Customer ID not found.\n");
        pauseScreen();
        return;
    }

    fp = fopen(CUSTOMER_FILE, "w");
    if (!fp) {
        printf("Error opening customer file for writing.\n");
        return;
    }
    for (int i = 0; i < c; i++)
        fprintf(fp, "%s,%s,%s,%s\n", arr[i].id, arr[i].name, arr[i].phone, arr[i].email);
    fclose(fp);

    printf("Customer information updated successfully!\n");
    pauseScreen();
}

void deleteCustomer() {
    FILE *fp_read = fopen(CUSTOMER_FILE, "r");
    if (!fp_read) {
        printf("No customers found.\n");
        return;
    }

    FILE *fp_write = fopen("temp_customers.txt", "w");
    if (!fp_write) {
        printf("Error creating temporary file.\n");
        fclose(fp_read);
        return;
    }

    char id_to_delete[10];
    printf("\nEnter Customer ID to delete: ");
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
        printf("Customer deleted successfully!\n");
    } else {
        printf("Customer ID not found.\n");
    }
    pauseScreen();
}

void searchCustomer() {
    FILE *fp = fopen(CUSTOMER_FILE, "r");
    if (!fp) {
        printf("No customers found.\n");
        return;
    }

    char search_term[50];
    printf("\nEnter Customer ID or Name to search: ");
    clearInputBuffer();
    fgets(search_term, sizeof(search_term), stdin);
    search_term[strcspn(search_term, "\n")] = '\0';

    Customer c;
    char line[200];
    int found = 0;

    printf("\n--- Search Results ---\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%49[^,],%14[^,],%49[^\n]", c.id, c.name, c.phone, c.email);
        if (strstr(c.id, search_term) || strstr(c.name, search_term)) {
            printf("ID: %s, Name: %s, Phone: %s, Email: %s\n", c.id, c.name, c.phone, c.email);
            found = 1;
        }
    }
    fclose(fp);

    if (!found) {
        printf("No matching customers found.\n");
    }
    pauseScreen();
}

void addVehicle() {
    FILE *fp = fopen(VEHICLE_FILE, "a+");
    if (!fp) {
        printf("Error opening vehicle file.\n");
        return;
    }

    Vehicle v;
    int cnt = fileLineCount(VEHICLE_FILE);

    printf("\nEnter customer ID: ");
    scanf("%9s", v.customerId);

    if (!customerExists(v.customerId)) {
        printf("Invalid customer ID! Please add the customer first.\n");
        fclose(fp);
        pauseScreen();
        return;
    }

    printf("Enter brand: ");
    scanf("%29s", v.brand);
    printf("Enter model: ");
    scanf("%29s", v.model);
    printf("Enter plate number: ");
    scanf("%14s", v.plate);
    printf("Enter manufacturing year: ");
    scanf("%4s", v.year);

    generateId("VEH", cnt + 1, v.id);

    fprintf(fp, "%s,%s,%s,%s,%s,%s\n", v.id, v.customerId, v.brand, v.model, v.plate, v.year);
    fclose(fp);

    printf("Vehicle added successfully! ID: %s\n", v.id);
    pauseScreen();
}

int vehicleExists(char *id) {
    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) return 0;

    Vehicle v;
    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^\n]", v.id, v.customerId, v.brand, v.model, v.plate, v.year);
        if (!strcmp(v.id, id)) {
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
        printf("No vehicles found.\n");
        return;
    }

    Vehicle arr[100];
    int c = 0;
    char line[200];

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^\n]", arr[c].id, arr[c].customerId, arr[c].brand, arr[c].model, arr[c].plate, arr[c].year);
        c++;
    }
    fclose(fp);

    char id[10];
    printf("\nEnter Vehicle ID to edit: ");
    scanf("%9s", id);

    int found = 0;
    for (int i = 0; i < c; i++) {
        if (!strcmp(arr[i].id, id)) {
            printf("Current Brand: %s\n", arr[i].brand);
            printf("Enter new brand (or press Enter to keep current): ");
            clearInputBuffer();
            char newBrand[30];
            fgets(newBrand, sizeof(newBrand), stdin);
            newBrand[strcspn(newBrand, "\n")] = '\0';
            if (strlen(newBrand) > 0) strcpy(arr[i].brand, newBrand);

            printf("Current Model: %s\n", arr[i].model);
            printf("Enter new model (or press Enter to keep current): ");
            char newModel[30];
            fgets(newModel, sizeof(newModel), stdin);
            newModel[strcspn(newModel, "\n")] = '\0';
            if (strlen(newModel) > 0) strcpy(arr[i].model, newModel);

            printf("Current Plate: %s\n", arr[i].plate);
            printf("Enter new plate (or press Enter to keep current): ");
            char newPlate[15];
            fgets(newPlate, sizeof(newPlate), stdin);
            newPlate[strcspn(newPlate, "\n")] = '\0';
            if (strlen(newPlate) > 0) strcpy(arr[i].plate, newPlate);

            printf("Current Year: %s\n", arr[i].year);
            printf("Enter new year (or press Enter to keep current): ");
            char newYear[5];
            fgets(newYear, sizeof(newYear), stdin);
            newYear[strcspn(newYear, "\n")] = '\0';
            if (strlen(newYear) > 0) strcpy(arr[i].year, newYear);

            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Vehicle ID not found.\n");
        pauseScreen();
        return;
    }

    fp = fopen(VEHICLE_FILE, "w");
    if (!fp) {
        printf("Error opening vehicle file for writing.\n");
        return;
    }
    for (int i = 0; i < c; i++)
        fprintf(fp, "%s,%s,%s,%s,%s,%s\n", arr[i].id, arr[i].customerId, arr[i].brand, arr[i].model, arr[i].plate, arr[i].year);
    fclose(fp);

    printf("Vehicle information updated successfully!\n");
    pauseScreen();
}

void deleteVehicle() {
    FILE *fp_read = fopen(VEHICLE_FILE, "r");
    if (!fp_read) {
        printf("No vehicles found.\n");
        return;
    }

    FILE *fp_write = fopen("temp_vehicles.txt", "w");
    if (!fp_write) {
        printf("Error creating temporary file.\n");
        fclose(fp_read);
        return;
    }

    char id_to_delete[10];
    printf("\nEnter Vehicle ID to delete: ");
    scanf("%9s", id_to_delete);

    char line[200];
    Vehicle v;
    int found = 0;

    while (fgets(line, sizeof(line), fp_read)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^\n]", v.id, v.customerId, v.brand, v.model, v.plate, v.year);
        if (strcmp(v.id, id_to_delete) != 0) {
            fprintf(fp_write, "%s,%s,%s,%s,%s,%s\n", v.id, v.customerId, v.brand, v.model, v.plate, v.year);
        } else {
            found = 1;
        }
    }

    fclose(fp_read);
    fclose(fp_write);

    remove(VEHICLE_FILE);
    rename("temp_vehicles.txt", VEHICLE_FILE);

    if (found) {
        printf("Vehicle deleted successfully!\n");
    } else {
        printf("Vehicle ID not found.\n");
    }
    pauseScreen();
}

void searchVehicle() {
    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) {
        printf("No vehicles found.\n");
        return;
    }

    char search_term[30];
    printf("\nEnter Vehicle Plate Number or Brand to search: ");
    clearInputBuffer();
    fgets(search_term, sizeof(search_term), stdin);
    search_term[strcspn(search_term, "\n")] = '\0';

    Vehicle v;
    char line[200];
    int found = 0;

    printf("\n--- Search Results ---\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%29[^,],%29[^,],%14[^,],%4[^\n]", v.id, v.customerId, v.brand, v.model, v.plate, v.year);
        if (strstr(v.plate, search_term) || strstr(v.brand, search_term)) {
            printf("ID: %s, Customer ID: %s, Brand: %s, Model: %s, Plate: %s, Year: %s\n", v.id, v.customerId, v.brand, v.model, v.plate, v.year);
            found = 1;
        }
    }
    fclose(fp);

    if (!found) {
        printf("No matching vehicles found.\n");
    }
    pauseScreen();
}

void viewAllVehicles() {
    FILE *fp = fopen(VEHICLE_FILE, "r");
    if (!fp) {
        printf("No vehicles found!\n");
        return;
    }
    char line[200];
    printf("\n--- Vehicles ---\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
    pauseScreen();
}

int mechanicExists(char *id) {
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) return 0;

    User u;
    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%29[^,],%29[^,],%14[^\n]", u.id, u.username, u.password, u.role);
        if (!strcmp(u.id, id) && !strcmp(u.role, "Mechanic")) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void addJob(char *currentUserId, char *currentUserRole) {
    FILE *fp = fopen(JOB_FILE, "a+");
    if (!fp) {
        printf("Error opening job file.\n");
        return;
    }

    Job j;
    int cnt = fileLineCount(JOB_FILE);

    printf("\nEnter customer ID: ");
    scanf("%9s", j.customerId);
    if (!customerExists(j.customerId)) {
        printf("Invalid customer ID! Job not added.\n");
        fclose(fp);
        pauseScreen();
        return;
    }

    printf("Enter vehicle ID: ");
    scanf("%9s", j.vehicleId);
    if (!vehicleExists(j.vehicleId)) {
        printf("Invalid vehicle ID! Job not added.\n");
        fclose(fp);
        pauseScreen();
        return;
    }

    if (strcmp(currentUserRole, "Admin") == 0) {
        printf("Enter mechanic ID: ");
        scanf("%9s", j.mechanicId);
        if (!mechanicExists(j.mechanicId)) {
            printf("Invalid mechanic ID! Job not added.\n");
            fclose(fp);
            pauseScreen();
            return;
        }
    } else if (strcmp(currentUserRole, "Mechanic") == 0) {
        strcpy(j.mechanicId, currentUserId);
    } else {
        printf("Unauthorized to add jobs.\n");
        fclose(fp);
        pauseScreen();
        return;
    }

    printf("Enter job description: ");
    clearInputBuffer();
    fgets(j.description, sizeof(j.description), stdin);
    j.description[strcspn(j.description, "\n")] = '\0';
    strcpy(j.status, "Pending");
    strcpy(j.notes, "No notes yet.");
    printf("Enter estimated cost: ");
    scanf("%f", &j.estimatedCost);

    generateId("JOB", cnt + 1, j.id);

    fprintf(fp, "%s,%s,%s,%s,%s,%s,%.2f,%s\n", j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, j.estimatedCost, j.notes);
    fclose(fp);

    printf("Job added successfully! ID: %s\n", j.id);
    pauseScreen();
}

void updateJobStatus(char *currentUserId, char *currentUserRole) {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf("No jobs found.\n");
        return;
    }

    Job arr[200];
    int c = 0;
    char line[500];

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%99[^\n]", arr[c].id, arr[c].customerId, arr[c].vehicleId,
               arr[c].mechanicId, arr[c].description, arr[c].status, &arr[c].estimatedCost, arr[c].notes);
        c++;
    }
    fclose(fp);

    char id[10];
    printf("Enter Job ID to update: ");
    scanf("%9s", id);

    int found = 0;
    for (int i = 0; i < c; i++) {
        if (!strcmp(arr[i].id, id)) {
            if (strcmp(currentUserRole, "Mechanic") == 0 && strcmp(arr[i].mechanicId, currentUserId) != 0) {
                printf("You are not authorized to update this job.\n");
                pauseScreen();
                return;
            }
            printf("Current status: %s\n", arr[i].status);
            printf("Enter new status (Pending/InProgress/Completed): ");
            scanf("%19s", arr[i].status);

            printf("Current notes: %s\n", arr[i].notes);
            printf("Add/Update notes (or press Enter to keep current): ");
            clearInputBuffer();
            char newNotes[100];
            fgets(newNotes, sizeof(newNotes), stdin);
            newNotes[strcspn(newNotes, "\n")] = '\0';
            if (strlen(newNotes) > 0) strcpy(arr[i].notes, newNotes);

            printf("Current Estimated Cost: %.2f\n", arr[i].estimatedCost);
            printf("Enter new estimated cost (or enter 0 to keep current): ");
            float newCost;
            scanf("%f", &newCost);
            if (newCost > 0) arr[i].estimatedCost = newCost;

            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Job ID not found.\n");
        pauseScreen();
        return;
    }

    fp = fopen(JOB_FILE, "w");
    if (!fp) {
        printf("Error opening job file for writing.\n");
        return;
    }
    for (int i = 0; i < c; i++)
        fprintf(fp, "%s,%s,%s,%s,%s,%s,%.2f,%s\n", arr[i].id, arr[i].customerId, arr[i].vehicleId, arr[i].mechanicId, arr[i].description, arr[i].status, arr[i].estimatedCost, arr[i].notes);
    fclose(fp);

    printf("Job status and details updated successfully!\n");
    pauseScreen();
}

void viewMyJobs(char *mechanicId) {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf("No jobs found!\n");
        return;
    }
    char line[500];
    Job j;
    int found = 0;
    printf("\n--- Your Assigned Jobs ---\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%99[^\n]", j.id, j.customerId, j.vehicleId,
               j.mechanicId, j.description, j.status, &j.estimatedCost, j.notes);
        if (!strcmp(j.mechanicId, mechanicId)) {
            printf("Job ID: %s, Customer ID: %s, Vehicle ID: %s, Description: %s, Status: %s, Est. Cost: %.2f, Notes: %s\n",
                   j.id, j.customerId, j.vehicleId, j.description, j.status, j.estimatedCost, j.notes);
            found = 1;
        }
    }
    fclose(fp);
    if (!found) {
        printf("No jobs assigned to you.\n");
    }
    pauseScreen();
}

void viewCustomerJobs(char *customerId) {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf("No jobs found!\n");
        return;
    }
    char line[500];
    Job j;
    int found = 0;
    printf("\n--- Your Service History ---\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%99[^\n]", j.id, j.customerId, j.vehicleId,
               j.mechanicId, j.description, j.status, &j.estimatedCost, j.notes);
        if (!strcmp(j.customerId, customerId)) {
            printf("Job ID: %s, Vehicle ID: %s, Mechanic ID: %s, Description: %s, Status: %s, Est. Cost: %.2f, Notes: %s\n",
                   j.id, j.vehicleId, j.mechanicId, j.description, j.status, j.estimatedCost, j.notes);
            found = 1;
        }
    }
    fclose(fp);
    if (!found) {
        printf("No service history found for your account.\n");
    }
    pauseScreen();
}

void searchJob() {
    FILE *fp = fopen(JOB_FILE, "r");
    if (!fp) {
        printf("No jobs found.\n");
        return;
    }

    char search_term[100];
    printf("\nEnter Job ID, Customer ID, Mechanic ID, or Description to search: ");
    clearInputBuffer();
    fgets(search_term, sizeof(search_term), stdin);
    search_term[strcspn(search_term, "\n")] = '\0';

    Job j;
    char line[500];
    int found = 0;

    printf("\n--- Search Results ---\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%9[^,],%9[^,],%9[^,],%9[^,],%99[^,],%19[^,],%f,%99[^\n]", j.id, j.customerId, j.vehicleId,
               j.mechanicId, j.description, j.status, &j.estimatedCost, j.notes);
        if (strstr(j.id, search_term) || strstr(j.customerId, search_term) ||
            strstr(j.mechanicId, search_term) || strstr(j.description, search_term)) {
            printf("Job ID: %s, Customer ID: %s, Vehicle ID: %s, Mechanic ID: %s, Description: %s, Status: %s, Est. Cost: %.2f, Notes: %s\n",
                   j.id, j.customerId, j.vehicleId, j.mechanicId, j.description, j.status, j.estimatedCost, j.notes);
            found = 1;
        }
    }
    fclose(fp);

    if (!found) {
        printf("No matching jobs found.\n");
    }
    pauseScreen();
}

void generateInvoice() {
    FILE *fp_inv = fopen(INVOICE_FILE, "a+");
    if (!fp_inv) {
        printf("Error opening invoice file.\n");
        return;
    }

    FILE *fp_job = fopen(JOB_FILE, "r");
    if (!fp_job) {
        printf("No jobs available to generate invoices.\n");
        fclose(fp_inv);
        return