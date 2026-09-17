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
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerPatient();
                break;
            case 2:
                printf("Bed Occupancy - coming soon\n");
                break;
            case 3:
                printf("Priority Queue - coming soon\n");
                break;
            case 4:
                printf("Reports - coming soon\n");
                break;
            case 5:
                printf("Exiting system. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
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

    printf("\nEnter Patient Name: ");
    scanf(" %[^\n]", patientName[i]);

    printf("Enter Patient Age: ");
    scanf("%d", &patientAge[i]);

    printf("Enter Triage Level (1=Normal, 2=Urgent, 3=Critical): ");
    scanf("%d", &triageLevel[i]);

    printf("\nAvailable Specialties:\n");
    for (int j = 0; j < NUM_SPECIALTIES; j++) {
        printf("%d. %s\n", specialtyID[j], specialtyName[j]);
    }
    printf("Select Specialty ID (1-4): ");
    int specID;
    scanf("%d", &specID);
    patientSpecialty[i] = specID - 1;

    int admitChoice;
    printf("Is patient admitted to a ward? (1=Yes, 0=No): ");
    scanf("%d", &admitChoice);

    if (admitChoice == 1) {
        isAdmitted[i] = 1;

        printf("\nAvailable Wards:\n");
        for (int j = 0; j < NUM_WARDS; j++) {
            printf("%d. %s\n", wardID[j], wardName[j]);
        }
        printf("Select Ward ID (1-4): ");
        int wID;
        scanf("%d", &wID);
        int wardIndex = wID - 1;
        patientWard[i] = wardIndex;

        printf("Enter Days Admitted: ");
        scanf("%d", &daysAdmitted[i]);

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
    patientCount++;
}
