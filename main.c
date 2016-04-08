#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>


#define BINFILE "RLC_Circuit.bin" // Constant for Bin file
#define N 5 // **NOTE** Change Design to include N as defined number of elements for Array
#define INPUT 1
#define OUTPUT 0
#define NUM_POINTS 100
#define TESTCHANGE 99999

// Structure used for Array includes all needed user entered values.
typedef struct Values
{

    double L, C, Td, Pc;

} RLCVALS;

// Prototype functions.
void fileIO(RLCVALS [], const char *, int);
void binWrite(RLCVALS [], FILE *);
void binRead(RLCVALS [], FILE *);
void printRLCValues(RLCVALS [], int);
void getUserValues(RLCVALS [], int);
int overWriteSelectValues(RLCVALS []);
int fileFetch(const char *);
void createNewRLCVALSFile(RLCVALS [],const char *);
void fillArray(RLCVALS [], double[], double[], int);
double getR(double, double, double, double, double);

int main()
{

    RLCVALS RLCArr[N];
    double xArr[NUM_POINTS];
    double yArr[NUM_POINTS];

    //printf("fileFetch return value: %d", fileFetch(BINFILE));

    fileIO(RLCArr, BINFILE, OUTPUT);

    int selectedValueRow = overWriteSelectValues(RLCArr);

    fileIO(RLCArr, BINFILE, INPUT);

    fillArray(RLCArr, xArr, yArr, selectedValueRow);

    int a;

    for(a = 0; a < NUM_POINTS; a++)
    {
        printf("%d.xArr: %f, yArr: %f\n", a+1 ,xArr[a], yArr[a]);
    }



    //printRLCValues(RLCArr, N);

    return 0;
}

/*----------------------------------------------------------------------------------------
Function: fileIO
Parameters:
    rlcArr: Reference to array that will be utilized
    state: 1 or 0 decides wether fileIO will read or write rlcArr
Returns: N/A
Description: Function will call other functions for orginization purpose of file handling.
-----------------------------------------------------------------------------------------*/
void fileIO(RLCVALS rlcArr[], const char *fName, int state)
{
    FILE *fp;

    printf("\nLoading...\n");

    if (fileFetch(fName) != 1)
    {
        printf("File not found...\n");
        createNewRLCVALSFile(rlcArr, fName);
    }

    switch(state)
    {
    case 0:

        fp = fopen(fName, "rb");

        fseek(fp, 0, SEEK_SET);

        binRead(rlcArr, fp);

        fclose(fp);
        printf("File Read.\n");
        break;

    case 1:

        fp = fopen(fName, "wb");

        binWrite(rlcArr, fp);
        fclose(fp);
        printf("File Written.\n");
        break;

    }

}
/*------------------------------------------------------------------------
Function: binWrite
Parameters:
    RLCVALS arrayRef[]: Reference to array structure
    *fname: Filename pointer
Returns: N/A
Description: Function writes referenced array structure to the file pointed by fName in binary.
-------------------------------------------------------------------------*/
void binWrite(RLCVALS arrayRef[], FILE *fptr)
{
    int a;
    for (a = 0; a < N; a++)
    {
        fwrite(&arrayRef[a], sizeof(arrayRef[a]), 1, fptr);
    }
}

/*--------------------------------------------------------------------------------------------------------
Function: binRead
Parameters:
    arrayRef[]: Reference to array
    *fname: Filename pointer
Returns: N/A
Description: Function reads binary array structure and transfers data to referenced array from pointed fName
----------------------------------------------------------------------------------------------------------*/
void binRead(RLCVALS arrayRef[], FILE *fptr)
{
    int b;
    for (b = 0; b < N; b++)
    {
        fread(&arrayRef[b], sizeof(arrayRef[b]), 1, fptr);
    }
}
/*-------------------------------------------------------------------------------------
Function: printRLCValues
Parameters:
    RLCVALS arrayRef[]: Reference to array struct
    INDEX: number of indexes in structured array.
Returns: N/A
Description: Function prints all the rows from structured array in an orginized fashion
-------------------------------------------------------------------------------------*/
void printRLCValues(RLCVALS arrayRef[], int INDEX)
{
    printf("\nRLC Circuit Values\n");
    printf("--------------------------------\n");

    int a;
    int num = 1;
    for (a = 0; a < INDEX; a++)
    {
        printf("%d. C = %0.2f || L = %0.2f || Td = %0.2f || Pc = %0.2f\n", num ,arrayRef[a].C, arrayRef[a].L, arrayRef[a].Td, arrayRef[a].Pc);
        num ++;
    }
}
/*--------------------------------------------------------------------------
Function: getUserValues
Parameters:
    RLCVALS arrRef[]: Reference to array structure
    INDEX: Index number to write to user entered values
Returns: N/A
Description: Requests users to enter 4 values for Referenced array structure.
--------------------------------------------------------------------------- */
void getUserValues(RLCVALS arrRef[], int INDEX)
{
    printf("Enter Circuit Values in order (C L Td Pc): ");
    scanf("%lf %lf %lf %lf", &arrRef[INDEX].C, &arrRef[INDEX].L, &arrRef[INDEX].Td,&arrRef[INDEX].Pc);
}

/*------------------------------------------------------------------------------------------------
Function: overWriteSelectValues
Parameters:
    RLCVALS arrayRef[]: Reference to array structure
Returns: N/A
Description: Function is for orginization purpose, handles the overwriting and selection of values.
-------------------------------------------------------------------------------------------------*/
int overWriteSelectValues(RLCVALS arrayRef[])
{
    printRLCValues(arrayRef, N);

    printf("\nOverwrite Or Select Values?\n");
    printf("---------------------------------\n");
    printf("1. Overwrite\n");
    printf("2. Select Values\n");

    int selection = -1;
    int PROCESSDONE = -1;
    int selectValue_arrayRef = -1;

    while (selection <= 0 || selection >= 3)
    {
        printf("\nSelection (1 - 2): ");
        scanf("%d", &selection);

        if (selection <= 0 || selection >= 3)
        {
            printf("Invalid number...");
        }
    }

    switch(selection)
    {
    case 1 :
        while (PROCESSDONE != 1)
        {

            printf("Overwrite Selected\n");

            int rowSelect = - 1;

            while(rowSelect <= 0 || rowSelect >= 6)
            {
                printf("\nSelect row to overwrite (1 - 5): ");
                scanf("%d", &rowSelect);

                if (rowSelect <= 0 || (int)rowSelect >= 6)
                {
                    printf("Invalid number.");
                }
            }


            getUserValues(arrayRef, rowSelect-1);

            printRLCValues(arrayRef, N);

            printf("\nOverwrite Another?\n");
            printf("1. Yes\n");
            printf("2. No\n");

            scanf("%d", &PROCESSDONE);
            PROCESSDONE = PROCESSDONE - 1;
            //printf("ProcessDone Value: %d", PROCESSDONE);
        }


    case 2 :
        while ( selectValue_arrayRef <= 0 || selectValue_arrayRef >= 6)
        {
            printf("Select Row to use (1 - 5): ");
            scanf("%d", &selectValue_arrayRef);
        }
        selectValue_arrayRef = selectValue_arrayRef - 1;
        break;
    }

    return selectValue_arrayRef;
}
/*--------------------------------------------------------------------------------
Function: fileFetch
Parameters:
    const char *fName: Character or string used for filelocation/Name
Returns: 1 or 0 based on if a file was fetched
Description: Using fopen in "r" mode in an if statement and returning either 1 or 0
allows us to know if a file exists.
----------------------------------------------------------------------------------*/
int fileFetch(const char *fName)
{
    FILE *fptr;

    if ( (fptr = fopen(fName, "r")) )
    {
        fclose(fptr);
        return 1;
    }
    return 0;
}
/*---------------------------------------------------------------
Function: createNewRLCVALSFile
Parameters:
    RLCVALS arrRef[]: Structured array reference.
    const char *fileName: File name/location
Returns: N/A
Description: Create new RLCVALS structured bin file and
insert the first row of info while also setting the rest of the values to 0
------------------------------------------------------------------*/
void createNewRLCVALSFile(RLCVALS arrRef[], const char *fileName)
{
    FILE *fp;

    printf("Creating new file...\n");

    fp = fopen(fileName, "wb");

    getUserValues(arrRef, 0);

    int a;

    for(a = 1; a < N; a++)
    {
        arrRef[a].C = 0;
        arrRef[a].L = 0;
        arrRef[a].Pc = 0;
        arrRef[a].Td = 0;
    }
    binWrite(arrRef, fp);
    fclose(fp);
}

/*--------------------------------------------------------------------
Function: getR
Parameters:
    varL, varC, varTd, varPc: variables used to calculate R-values
    varR: variable used to store an R-value
Returns:
    The R-value for a given set of parameters.
Description: Calculate the value of R.
---------------------------------------------------------------------*/

double getR(double varL, double varC, double varTd, double varPc, double varR)
{
    double varY;
    varY = exp((-varR*varTd)/(2*varL))*(cos(sqrt((1/(varL*varC))-
                                            (pow(varR/(2*varL), 2)))*varTd))-varPc;
    return varY;
}

/*--------------------------------------------------------------------
Function: fillArray
Parameters:
    varL, varC, varTd, varPc: variables used to use in getR function
    xArr[]: to store x-values.
    yArr[]: to store y-values.
Returns: N/A
Description: Fill the x-value and y-value arrays to use for plotting.
---------------------------------------------------------------------*/

void fillArray(RLCVALS arrRef[], double xArr[], double yArr[], int iNum)
{
    double varRu;
    int ix;
    varRu = sqrt(2/arrRef[iNum].C);
    double x = varRu / NUM_POINTS;
    xArr[0] = 0;

    for(ix=1; ix < NUM_POINTS; ix++)
    {
        xArr[ix] = xArr[ix-1] + x;
    }

    for(ix=0; xArr[ix] < xArr[NUM_POINTS]; ix++)
    {
        yArr[ix] = getR(arrRef[iNum].L, arrRef[iNum].C,arrRef[iNum].Td, arrRef[iNum].Pc, xArr[ix]);
    }
}
