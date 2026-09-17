#include <stdio.h>

#define NUM_SPECIALTIES 4
#define NUM_WARDS 4
#define MAX_BEDS_PER_WARD 20
#define MAX_PATIENTS 100

int specialtyID[NUM_SPECIALTIES] = {1, 2, 3, 4};
char specialtyName[NUM_SPECIALTIES][30] = {"General Practice (OPD)", "Paediatrics", "Cardiology", "Neurology"};
float baseFee[NUM_SPECIALTIES] = {1500.00, 2500.00, 4500.00, 5000.00};
int consultTime[NUM_SPECIALTIES] = {15, 20, 30, 30};
int dailyCap[NUM_SPECIALTIES] = {30, 20, 12, 10};

int wardID[NUM_WARDS] = {1, 2, 3, 4};
char wardName[NUM_WARDS][30] = {"General Ward", "Paediatric Ward", "Surgical Ward", "ICU"};
float wardDailyRate[NUM_WARDS] = {3000.00, 6000.00, 12000.00, 25000.00};
int wardCapacity[NUM_WARDS] = {20, 10, 10, 5};

int bedOccupancy[NUM_WARDS][MAX_BEDS_PER_WARD] = {0};

int patientCount = 0;
char patientName[MAX_PATIENTS][50];
int patientAge[MAX_PATIENTS];
int triageLevel[MAX_PATIENTS];
int patientSpecialty[MAX_PATIENTS];
int isAdmitted[MAX_PATIENTS];
int patientWard[MAX_PATIENTS];
int daysAdmitted[MAX_PATIENTS];
int assignedBedNo[MAX_PATIENTS];
float finalBill[MAX_PATIENTS];

int queueCount[NUM_SPECIALTIES] = {0, 0, 0, 0};

void registerPatient();
float calculateWaitTime(int specIdx);
float calculateSurcharge(int specIdx, int urgency);
float calculateWardCost(int wardIdx, int days, int admitted);
float calculateDiscount(int age, float grossTotal);
void showPriorityQueue();
void displayBeds();
void generateReports();
int getValidInt(const char *prompt, int min, int max);
void getValidName(const char *prompt, char *dest, int destSize);

int main() {
    int choice;

    while (1) {
        printf("\n====================================\n");
        printf(" SMART HOSPITAL MANAGEMENT SYSTEM\n");
        printf("====================================\n");
        printf("1. Register New Patient\n");
        printf("2. View Bed Occupancy\n");
        printf("3. View Priority Queue (Sorted)\n");
        printf("4. Generate Reports\n");
        printf("5. Exit\n");
        printf("====================================\n");

        choice = getValidInt("Enter your choice: ", 1, 5);

        switch (choice) {
            case 1:
                registerPatient();
                break;
            case 2:
                displayBeds();
                break;
            case 3:
                showPriorityQueue();
                break;
            case 4:
                generateReports();
                break;
            case 5:
                printf("Exiting system. Goodbye!\n");
                return 0;
        }
    }
    return 0;
}


int getValidInt(const char *prompt, int min, int max) {
    int value;
    int result;
    int c;

    while (1) {
        printf("%s", prompt);
        result = scanf("%d", &value);

        if (result != 1) {

            while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input. Please enter a whole number.\n");
            continue;
        }


        while ((c = getchar()) != '\n' && c != EOF);

        if (value < min || value > max) {
            printf("Invalid input. Please enter a number between %d and %d.\n", min, max);
            continue;
        }

        return value;
    }
}


void getValidName(const char *prompt, char *dest, int destSize) {
    while (1) {
        printf("%s", prompt);
        if (scanf(" %49[^\n]", dest) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input. Name cannot be empty.\n");
            continue;
        }
        (void)destSize;
        break;
    }
}

float calculateWaitTime(int specIdx) {
    return queueCount[specIdx] * consultTime[specIdx];
}

float calculateSurcharge(int specIdx, int urgency) {
    if (urgency == 1) return 0;
    else if (urgency == 2) return baseFee[specIdx] * 0.20;
    else return baseFee[specIdx] * 0.50;
}

float calculateWardCost(int wardIdx, int days, int admitted) {
    if (admitted == 0) return 0;
    return days * wardDailyRate[wardIdx];
}

float calculateDiscount(int age, float grossTotal) {
    if (age < 5 || age > 65) return grossTotal * 0.15;
    return 0;
}

void registerPatient() {
    if (patientCount >= MAX_PATIENTS) {
        printf("System full. Cannot register more patients.\n");
        return;
    }

    int i = patientCount;

    getValidName("\nEnter Patient Name: ", patientName[i], 50);

    patientAge[i] = getValidInt("Enter Patient Age: ", 0, 120);

    triageLevel[i] = getValidInt("Enter Triage Level (1=Normal, 2=Urgent, 3=Critical): ", 1, 3);

    printf("\nAvailable Specialties:\n");
    for (int j = 0; j < NUM_SPECIALTIES; j++) {
        printf("%d. %s\n", specialtyID[j], specialtyName[j]);
    }
    int specID = getValidInt("Select Specialty ID (1-4): ", 1, NUM_SPECIALTIES);
    patientSpecialty[i] = specID - 1;

    int admitChoice = getValidInt("Is patient admitted to a ward? (1=Yes, 0=No): ", 0, 1);

    if (admitChoice == 1) {
        isAdmitted[i] = 1;

        printf("\nAvailable Wards:\n");
        for (int j = 0; j < NUM_WARDS; j++) {
            printf("%d. %s\n", wardID[j], wardName[j]);
        }
        int wID = getValidInt("Select Ward ID (1-4): ", 1, NUM_WARDS);
        int wardIndex = wID - 1;
        patientWard[i] = wardIndex;

        daysAdmitted[i] = getValidInt("Enter Days Admitted: ", 1, 365);

        int bedFound = -1;
        for (int b = 0; b < wardCapacity[wardIndex]; b++) {
            if (bedOccupancy[wardIndex][b] == 0) {
                bedFound = b;
                break;
            }
        }

        if (bedFound == -1) {
            printf("No beds available in %s! Patient cannot be admitted.\n", wardName[wardIndex]);
            isAdmitted[i] = 0;
            daysAdmitted[i] = 0;
            assignedBedNo[i] = -1;
        } else {
            bedOccupancy[wardIndex][bedFound] = 1;
            assignedBedNo[i] = bedFound;
            printf("Bed #%02d assigned in %s.\n", bedFound + 1, wardName[wardIndex]);
        }

    } else {
        isAdmitted[i] = 0;
        daysAdmitted[i] = 0;
        assignedBedNo[i] = -1;
        patientWard[i] = -1;
    }

    printf("Patient registered successfully!\n");

    float waitTime = calculateWaitTime(patientSpecialty[i]);
    float surcharge = calculateSurcharge(patientSpecialty[i], triageLevel[i]);
    float wardCost = calculateWardCost(patientWard[i], daysAdmitted[i], isAdmitted[i]);
    float grossTotal = baseFee[patientSpecialty[i]] + surcharge + wardCost;
    float discount = calculateDiscount(patientAge[i], grossTotal);
    float finalAmount = grossTotal - discount;

    finalBill[i] = finalAmount;
    queueCount[patientSpecialty[i]]++;

    printf("\n====================================================\n");
    printf(" SMART HOSPITAL ADMISSION & BILL\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("Patient ID              : PAT-%d\n", 1000 + i + 1);
    printf("Patient Name            : %s\n", patientName[i]);
    printf("Age                     : %d Years", patientAge[i]);
    if (patientAge[i] < 5 || patientAge[i] > 65)
        printf(" (15%% Subsidy Eligible)\n");
    else
        printf("\n");
    printf("Specialty               : %s\n", specialtyName[patientSpecialty[i]]);
    if (isAdmitted[i] == 1)
        printf("Assigned Ward           : %s (Bed #%02d)\n", wardName[patientWard[i]], assignedBedNo[i] + 1);
    else
        printf("Assigned Ward           : Not Admitted (Outpatient)\n");
    printf("Urgency Level           : Level %d\n", triageLevel[i]);
    printf("----------------------------------------------------------------------------------------\n");
    printf("Base Consultation Fee   : LKR %.2f\n", baseFee[patientSpecialty[i]]);
    printf("Emergency Surcharge     : LKR %.2f\n", surcharge);
    printf("Ward Stay Cost (%d Days) : LKR %.2f\n", daysAdmitted[i], wardCost);
    printf("----------------------------------------------------------------------------------------\n");
    printf("Gross Total Bill        : LKR %.2f\n", grossTotal);
    printf("Age Subsidy Discount    : LKR -%.2f\n", discount);
    printf("----------------------------------------------------------------------------------------\n");
    printf("Final Payable Amount    : LKR %.2f\n", finalAmount);
    printf("Estimated Waiting Time  : %.2f mins\n", waitTime);
    printf("====================================================\n");

    patientCount++;
}

void showPriorityQueue() {
    if (patientCount == 0) {
        printf("\nNo patients registered yet.\n");
        return;
    }

    int indexArr[MAX_PATIENTS];
    for (int i = 0; i < patientCount; i++) {
        indexArr[i] = i;
    }


    for (int a = 0; a < patientCount - 1; a++) {
        for (int b = 0; b < patientCount - 1 - a; b++) {
            if (triageLevel[indexArr[b]] < triageLevel[indexArr[b + 1]]) {
                int temp = indexArr[b];
                indexArr[b] = indexArr[b + 1];
                indexArr[b + 1] = temp;
            }
        }
    }

    printf("\n==================== PRIORITY QUEUE ====================\n");
    printf("%-12s %-20s %-8s %-15s\n", "Patient ID", "Name", "Age", "Urgency Level");
    printf("----------------------------------------------------------\n");
    for (int i = 0; i < patientCount; i++) {
        int idx = indexArr[i];
        char *urgencyText;
        if (triageLevel[idx] == 3) urgencyText = "Critical";
        else if (triageLevel[idx] == 2) urgencyText = "Urgent";
        else urgencyText = "Normal";

        printf("PAT-%-8d %-20s %-8d Level %d (%s)\n",
               1000 + idx + 1, patientName[idx], patientAge[idx], triageLevel[idx], urgencyText);
    }
    printf("==========================================================\n");
}

void displayBeds() {
    printf("\n==================== BED OCCUPANCY ====================\n");
    for (int w = 0; w < NUM_WARDS; w++) {
        int occupiedCount = 0;

        printf("\n%s (Capacity: %d beds)\n", wardName[w], wardCapacity[w]);
        printf("Beds: ");
        for (int b = 0; b < wardCapacity[w]; b++) {
            printf("%d ", bedOccupancy[w][b]);
            if (bedOccupancy[w][b] == 1) {
                occupiedCount++;
            }
        }
        printf("\n");

        float occupancyPercent = ((float)occupiedCount / wardCapacity[w]) * 100;
        printf("Occupied: %d / %d beds (%.1f%%)\n", occupiedCount, wardCapacity[w], occupancyPercent);
    }
    printf("=========================================================\n");
}
void generateReports() {
    if (patientCount == 0) {
        printf("\nNo patients registered yet.\n");
        return;
    }

    int normalCount = 0, urgentCount = 0, criticalCount = 0;
    float totalRevenue = 0, totalDiscount = 0;
    float highestBill = -1;
    int highestBillIndex = -1;

    for (int i = 0; i < patientCount; i++) {
        // Urgency level counting
        if (triageLevel[i] == 1) normalCount++;
        else if (triageLevel[i] == 2) urgentCount++;
        else if (triageLevel[i] == 3) criticalCount++;

        // Revenue and discount totals
        totalRevenue += finalBill[i];

        float surcharge = calculateSurcharge(patientSpecialty[i], triageLevel[i]);
        float wardCost = calculateWardCost(patientWard[i], daysAdmitted[i], isAdmitted[i]);
        float gross = baseFee[patientSpecialty[i]] + surcharge + wardCost;
        float discount = gross - finalBill[i];
        totalDiscount += discount;

        // Highest paying patient
        if (finalBill[i] > highestBill) {
            highestBill = finalBill[i];
            highestBillIndex = i;
        }
    }

    printf("\n==================== SUMMARY REPORT ====================\n");
    printf("Total Patients Registered : %d\n", patientCount);
    printf("  - Normal (Level 1)   : %d\n", normalCount);
    printf("  - Urgent (Level 2)   : %d\n", urgentCount);
    printf("  - Critical (Level 3) : %d\n", criticalCount);
    printf("----------------------------------------------------------\n");
    printf("Total Revenue Earned   : LKR %.2f\n", totalRevenue);
    printf("Total Discounts Given  : LKR %.2f\n", totalDiscount);
    printf("----------------------------------------------------------\n");
    printf("Ward Occupancy:\n");
    for (int w = 0; w < NUM_WARDS; w++) {
        int occupiedCount = 0;
        for (int b = 0; b < wardCapacity[w]; b++) {
            if (bedOccupancy[w][b] == 1) occupiedCount++;
        }
        float percent = ((float)occupiedCount / wardCapacity[w]) * 100;
        printf("  - %-20s : %.1f%%\n", wardName[w], percent);
    }
    printf("----------------------------------------------------------\n");
    printf("Highest Paying Patient : %s (LKR %.2f)\n", patientName[highestBillIndex], highestBill);
    printf("==========================================================\n");
}
