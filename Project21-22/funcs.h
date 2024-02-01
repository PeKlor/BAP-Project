#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define MAX_CHARS 50

struct route
{
    char startLocation[MAX_CHARS];
    int length;
    int upHill;
    int downHill;
    int maxAltitude;
    int minAltitude;
    char endLocation[MAX_CHARS];
};


struct route *readFromInput(int *n)
{
    int i;
    struct route *R;

    scanf("%d ", n);
    R = (struct route *) malloc(*n * sizeof(struct route));

    for(i=0; i<*n; i++)
    {
        fflush(stdin);
        fgets(R[i].startLocation, MAX_CHARS, stdin);
        R[i].startLocation[strcspn(R[i].startLocation, "\n")] = '\0';
        fflush(stdin);
        scanf("%d ", &R[i].length);
        scanf("%d ", &R[i].upHill);
        scanf("%d ", &R[i].downHill);
        scanf("%d ", &R[i].maxAltitude);
        scanf("%d ", &R[i].minAltitude);
        fflush(stdin);
        fgets(R[i].endLocation, MAX_CHARS, stdin);
        R[i].endLocation[strcspn(R[i].endLocation, "\n")] = '\0';
    }
    return R;
}


void selectionSortDescendingLength(struct route R[], int n)
{
    int i, j, minpos;
    struct route temp;

    for(i=n-1; i>0; i--)
    {
        minpos = 0;
        for(j=1; j<=i; j++)
        {
            if(R[j].length < R[minpos].length)
            {
                minpos = j;
            }
        }
        temp = R[i];
        R[i] = R[minpos];
        R[minpos] = temp;
    }
}


int isCyclic(struct route r)
{
    if(!strcmp(r.startLocation, r.endLocation))
        return 1;
    return 0;
}


float timeNaismith(struct route r, int acyclic)
{
    float time;
    int uH;

    uH = r.upHill;
    if(acyclic)
        uH = r.downHill;
    time = r.length/65.0 + uH/10.0;
    return time;
}


void printRoute(struct route r, int acyclic)
{
    char sL[MAX_CHARS];
    char eL[MAX_CHARS];
    int uH;
    int dH;
    char routePhrase[110] = "From ";

    strcpy(sL,r.startLocation);
    strcpy(eL,r.endLocation);
    uH = r.upHill;
    dH = r.downHill;
    if(acyclic)
    {
        strcpy(sL,r.endLocation);
        strcpy(eL,r.startLocation);
        uH = r.downHill;
        dH = r.upHill;
    }

    strcat(routePhrase,sL);
    strcat(routePhrase," to ");
    strcat(routePhrase,eL);

    fputs(routePhrase, stdout);
    fflush(stdout);
    printf("\nUphill %d, Downhill %d\n", uH, dH);
    printf("Max altitude %d, Min altitude %d\n", r.maxAltitude, r.minAltitude);
    printf("Length %d, Estimated time %d min\n\n", r.length, (int) round(timeNaismith(r, acyclic)));
    fflush(stdout);
}


void printData(struct route R[], int n)
{
    int i;
    selectionSortDescendingLength(R, n);
    for(i=0; i<n; i++)
    {
        printRoute(R[i], 0);
        if(!isCyclic(R[i]))
            printRoute(R[i], 1);
    }
}


void findShort(struct route R[], int n)
{
    int L, i;
    scanf("%d", &L);
    selectionSortDescendingLength(R, n);
    for(i=0; i<n; i++)
    {
        if(R[i].length <= L)
        {
            printRoute(R[i], 0);
            if(!isCyclic(R[i]))
                printRoute(R[i], 1);
        }
    }
}


void findLocation(struct route R[], int n)
{
    int i;
    char location[MAX_CHARS];
    fgets(location, MAX_CHARS, stdin);
    location[strcspn(location, "\n")] = '\0';
    selectionSortDescendingLength(R, n);
    for(i=0; i<n; i++)
    {
        if(!strcmp(location, R[i].startLocation))
            printRoute(R[i], 0);
        else if(!strcmp(location, R[i].endLocation))
            printRoute(R[i], 1);
    }
}


void findUpHill(struct route R[], int n)
{
    int i;
    selectionSortDescendingLength(R, n);
    for(i=0; i<n; i++)
    {
        if(!isCyclic(R[i]))
        {
            if(R[i].upHill > R[i].downHill)
                printRoute(R[i], 0);
            else
                printRoute(R[i], 1);
        }
    }
}


void findDownHill(struct route R[], int n)
{
    int i;
    selectionSortDescendingLength(R, n);
    for(i=0; i<n; i++)
    {
        if(!isCyclic(R[i]))
        {
            if(R[i].downHill > R[i].upHill)
                printRoute(R[i], 0);
            else
                printRoute(R[i], 1);
        }
    }
}


void findAltitude(struct route R[], int n)
{
    int i;
    int minA;
    int maxA;

    scanf("%d %d ", &minA, &maxA);
    selectionSortDescendingLength(R, n);
    for(i=0; i<n; i++)
    {
        if((R[i].minAltitude >= minA) && (R[i].maxAltitude <= maxA))
        {
            printRoute(R[i], 0);
            if(!isCyclic(R[i]))
                printRoute(R[i], 1);
        }
    }
}


void findTimely(struct route R[], int n)
{
    int T, i;
    scanf("%d ", &T);
    selectionSortDescendingLength(R, n);
    for(i=0; i<n; i++)
    {
        if(timeNaismith(R[i], 0) <= T)
            printRoute(R[i], 0);
        if(!isCyclic(R[i]) && (timeNaismith(R[i], 1) <= T))
            printRoute(R[i], 1);
    }

}


struct route *readFromFile(int *n)
{
    char S[MAX_CHARS];
    FILE *fp;
    struct route *R;
    int i;

    fgets(S, MAX_CHARS, stdin);
    S[strcspn(S, "\n")] = '\0';
    if((fp = fopen(S,"r")) != NULL)
    {
        //fflush(stdin);
        fscanf(fp,"%d ",n);
        R = (struct route *) malloc(*n * sizeof(struct route));

        for(i=0; i<*n; i++)
        {
            //fflush(fp);
            fgets(R[i].startLocation, MAX_CHARS, fp);
            R[i].startLocation[strcspn(R[i].startLocation, "\n")] = '\0';
            //fflush(fp);
            fscanf(fp,"%d ",&R[i].length);
            fscanf(fp,"%d ",&R[i].upHill);
            fscanf(fp,"%d ",&R[i].downHill);
            fscanf(fp,"%d ",&R[i].maxAltitude);
            fscanf(fp,"%d ",&R[i].minAltitude);
            //fflush(fp);
            fgets(R[i].endLocation, MAX_CHARS, fp);
            R[i].endLocation[strcspn(R[i].endLocation, "\n")] = '\0';
        }
        fclose(fp);
    }
    return R;
}


void saveRoute(struct route r, int acyclic, FILE *fp)
{
    char sL[MAX_CHARS];
    char eL[MAX_CHARS];
    int uH;
    int dH;
    char routePhrase[110] = "From ";

    strcpy(sL,r.startLocation);
    strcpy(eL,r.endLocation);
    uH = r.upHill;
    dH = r.downHill;
    if(acyclic)
    {
        strcpy(sL,r.endLocation);
        strcpy(eL,r.startLocation);
        uH = r.downHill;
        dH = r.upHill;
    }

    strcat(routePhrase,sL);
    strcat(routePhrase," to ");
    strcat(routePhrase,eL);

    fputs(routePhrase, fp);
    //fflush(fp);
    fprintf(fp, "\nUphill %d, Downhill %d\n", uH, dH);
    fprintf(fp, "Max altitude %d, Min altitude %d\n", r.maxAltitude, r.minAltitude);
    fprintf(fp, "Length %d, Estimated time %d min\n\n", r.length, (int) round(timeNaismith(r, acyclic)));
    //fflush(fp);
}


void saveData(struct route R[], int n)
{
    char S[MAX_CHARS];
    FILE *fp;
    int i;

    fgets(S, MAX_CHARS, stdin);
    S[strcspn(S, "\n")] = '\0';
    if((fp = fopen(S,"w")) != NULL)
    {
        fflush(stdin);
        selectionSortDescendingLength(R, n);
        for(i=0; i<n; i++)
        {
            saveRoute(R[i], 0, fp);
            if(!isCyclic(R[i]))
                saveRoute(R[i], 1, fp);
        }
    }
}

