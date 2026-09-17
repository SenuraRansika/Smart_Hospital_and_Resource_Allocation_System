#include <stdio.h>

#define NUM_SPECIALTIES 4
#define NUM_WARDS 4
#define MAX_BEDS_PER_WARD 20

int wardID[NUM_WARDS] = {1, 2, 3, 4};
char wardName[NUM_WARDS][30] = {"General Ward", "Paediatric Ward", "Surgical Ward", "ICU"};
float wardDailyRate[NUM_WARDS] = {3000.00, 6000.00, 12000.00, 25000.00};
int wardCapacity[NUM_WARDS] = {20, 10, 10, 5};

int bedOccupancy[NUM_WARDS][MAX_BEDS_PER_WARD] = {0};

int specialtyID[NUM_SPECIALTIES] = {1, 2, 3, 4};
char specialtyName[NUM_SPECIALTIES][30] = {"General Practice (OPD)", "Paediatrics", "Cardiology", "Neurology"};
float baseFee[NUM_SPECIALTIES] = {1500.00, 2500.00, 4500.00, 5000.00};
int consultTime[NUM_SPECIALTIES] = {15, 20, 30, 30};
int dailyCap[NUM_SPECIALTIES] = {30, 20, 12, 10};

int main() {
    printf("Smart Hospital System - Starting...\n");
    return 0;
}
