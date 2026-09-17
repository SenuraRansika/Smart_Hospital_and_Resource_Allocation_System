#include <stdio.h>

#define NUM_SPECIALTIES 4

int specialtyID[NUM_SPECIALTIES] = {1, 2, 3, 4};
char specialtyName[NUM_SPECIALTIES][30] = {"General Practice (OPD)", "Paediatrics", "Cardiology", "Neurology"};
float baseFee[NUM_SPECIALTIES] = {1500.00, 2500.00, 4500.00, 5000.00};
int consultTime[NUM_SPECIALTIES] = {15, 20, 30, 30};
int dailyCap[NUM_SPECIALTIES] = {30, 20, 12, 10};

int main() {
    printf("Smart Hospital System - Starting...\n");
    return 0;
}
