#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

#define PROFILE_FILE "profile.txt"
#define HISTORY_FILE "visit_history.txt"

struct Place
{
    char name[60];
};

struct PlaceList
{
    char title[30];
    int count;
    struct Place items[5];
};

struct MenuItem
{
    char text[40];
};

struct Crisis
{
    char title[30];
    char issueClifton[80];
    char issueTariq[80];
    char suggestion[80];
};

char areaNames[2][15] = {"Clifton", "Tariq Road"};
char areaKeys[2][15] = {"clifton", "tariq road"};

char sectionNames[5][20] =
{
    "Transportation",
    "Healthcare",
    "Entertainment",
    "Restaurants",
    "City Crisis"
};

int visitCount[2][5] =
{
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0}
};



void setColor(int color);
void printHeader(const char title[]);
void printSubHeader(const char title[]);
void printPrompt(const char message[]);
void clearInputBuffer();

void mainMenu(int area);
void transportation(int area);
void healthcare(int area);
void entertainment(int area);
void restaurants(int area);
void parks(int area);
void cinema(int area);
void gamingZones(int area);
void pakistaniFood(int area);
void chineseFood(int area);
void fastFood(int area);
void cityCrisis(int area);

void printPlaces(struct PlaceList *list);
void printMenu(struct MenuItem *items, int count);
void printMenu2D(char menu[][40], int count, char *zeroText);
void printCrisis(struct Crisis *c, int area);

void saveProfile(char *name, int area);
int loadProfile(char *name, int *area);
void logVisit(char *section, int area);
void recordVisit(int area, int section);
void showHistory(void);
void showSummary(void);



void setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void printHeader(const char title[])
{
    int spaces;
    int remaining;

    spaces = (50 - (int)strlen(title)) / 2;
    remaining = 50 - (int)strlen(title) - spaces;

    setColor(11);

    printf("\n+==================================================+\n");

    printf("|%*s%s%*s|\n",
           spaces, "", title, remaining, "");

    printf("+==================================================+\n");

    setColor(7);
}

void printSubHeader(const char title[])
{
    setColor(14);
    printf("\n--- %s ---\n", title);
    setColor(7);
}

void printPrompt(const char message[])
{
    setColor(10);
    printf("%s", message);
    setColor(7);
}

void clearInputBuffer()
{
    int character;

    while ((character = getchar()) != '\n' && character != EOF)
    {
    }
}


void printPlaces(struct PlaceList *list)
{
    int i;

    for (i = 0; i < list->count; i++)
    {
        printf("%d. %s\n", i + 1, list->items[i].name);
    }
}


void printMenu(struct MenuItem *items, int count)
{
    int i;

    printf("\n");

    for (i = 0; i < count; i++)
    {
        printf("%d. %s\n", i + 1, items[i].text);
    }

    printf("0. Back to Main Menu\n");
}



void printMenu2D(char menu[][40], int count, char *zeroText)
{
    int i;

    printf("\n");

    for (i = 0; i < count; i++)
    {
        printf("%d. %s\n", i + 1, menu[i]);
    }

    printf("0. %s\n", zeroText);
}



void printCrisis(struct Crisis *c, int area)
{
    printSubHeader(c->title);

    if (area == 1)
    {
        printf("Issue: %s\n", c->issueClifton);
    }
    else
    {
        printf("Issue: %s\n", c->issueTariq);
    }

    printf("Suggestion: %s\n", c->suggestion);
}



void saveProfile(char *name, int area)
{
    FILE *fp;

    fp = fopen(PROFILE_FILE, "w");

    if (fp == NULL)
    {
        printf("Could not save profile.\n");
        return;
    }

    fprintf(fp, "%s\n%d\n", name, area);

    fclose(fp);
}



int loadProfile(char *name, int *area)
{
    FILE *fp;

    fp = fopen(PROFILE_FILE, "r");

    if (fp == NULL)
    {
        return 0;
    }

    if (fgets(name, 50, fp) == NULL)
    {
        fclose(fp);
        return 0;
    }

    name[strcspn(name, "\n")] = '\0';

    if (fscanf(fp, "%d", area) != 1 ||
        (*area != 1 && *area != 2))
    {
        fclose(fp);
        return 0;
    }

    fclose(fp);

    return 1;
}



void logVisit(char *section, int area)
{
    time_t now;
    char *timeText;
    FILE *fp;

    now = time(NULL);
    timeText = ctime(&now);

    fp = fopen(HISTORY_FILE, "a");

    if (fp == NULL)
    {
        return;
    }

    if (timeText == NULL)
    {
        fclose(fp);
        return;
    }

    fprintf(fp, "[%s] %s | %s",
            timeText,
            areaNames[area - 1],
            section);

    fclose(fp);
}



void recordVisit(int area, int section)
{
    visitCount[area - 1][section]++;

    logVisit(sectionNames[section], area);
}



void showHistory()
{
    char line[200];
    int count = 0;
    FILE *fp;

    printHeader("VISIT HISTORY");

    fp = fopen(HISTORY_FILE, "r");

    if (fp == NULL)
    {
        printf("No visits recorded yet.\n");
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        printf("%s", line);
        count++;
    }

    fclose(fp);

    if (count == 0)
    {
        printf("No visits recorded yet.\n");
    }
    else
    {
        printf("\nTotal records: %d\n", count);
    }
}



void showSummary()
{
    int i;
    int j;

    printHeader("VISIT SUMMARY (THIS SESSION)");

    printf("\n%-20s", "Section");

    for (i = 0; i < 2; i++)
    {
        printf("%-12s", areaNames[i]);
    }

    printf("\n");

    for (j = 0; j < 5; j++)
    {
        printf("%-20s", sectionNames[j]);

        for (i = 0; i < 2; i++)
        {
            printf("%-12d", visitCount[i][j]);
        }

        printf("\n");
    }
}



int main()
{
    char name[50];
    char lastName[50];
    char areaName[50];

    int lastArea;
    int area = 0;
    int i;
    int j;

    printHeader("WELCOME TO URBANVERSE");

    setColor(14);
    printf("              MINI CITY SIMULATOR\n");
    setColor(7);

    if (loadProfile(lastName, &lastArea))
    {
        printf("\nLast visitor: %s (%s)\n",
               lastName, areaNames[lastArea - 1]);
    }

    printPrompt("\nEnter your name: ");

    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    printf("\nAvailable Areas:\n");
    printf("1. %s\n", areaNames[0]);
    printf("2. %s\n", areaNames[1]);

    while (area == 0)
    {
        printPrompt("\nEnter your area name: ");

        fgets(areaName, sizeof(areaName), stdin);
        areaName[strcspn(areaName, "\n")] = '\0';

        for (i = 0; areaName[i] != '\0'; i++)
        {
            areaName[i] =
                (char)tolower((unsigned char)areaName[i]);
        }

        for (j = 0; j < 2; j++)
        {
            if (strcmp(areaName, areaKeys[j]) == 0)
            {
                area = j + 1;
            }
        }

        if (area == 0)
        {
            setColor(12);

            printf("\nArea not found! Please enter Clifton or Tariq Road.\n");

            setColor(7);
        }
    }

    saveProfile(name, area);

    logVisit("Session started", area);

    printHeader("YOUR CITY PROFILE");

    setColor(10);

    printf("Welcome %s!\n", name);
    printf("Selected area: %s\n", areaNames[area - 1]);

    setColor(7);

    printPrompt("\nPress Enter to continue...");
    getchar();

    mainMenu(area);

    logVisit("Session ended", area);

    printHeader("THANK YOU FOR VISITING URBANVERSE");

    return 0;
}



void mainMenu(int area)
{
    int choice;

    char mainItems[7][40] =
    {
        "Transportation",
        "Healthcare",
        "Entertainment",
        "Restaurants",
        "City Crisis Update",
        "View Visit History",
        "View Visit Summary"
    };

    do
    {
        printHeader("URBANVERSE | MAIN MENU");

        printf("Area: %s\n", areaNames[area - 1]);

        printMenu2D(mainItems, 7, "Exit");

        printPrompt("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            clearInputBuffer();
            choice = -1;
        }
        else
        {
            clearInputBuffer();
        }

        switch (choice)
        {
            case 1:
                recordVisit(area, 0);
                transportation(area);
                break;

            case 2:
                recordVisit(area, 1);
                healthcare(area);
                break;

            case 3:
                recordVisit(area, 2);
                entertainment(area);
                break;

            case 4:
                recordVisit(area, 3);
                restaurants(area);
                break;

            case 5:
                recordVisit(area, 4);
                cityCrisis(area);
                break;

            case 6:
                showHistory();
                break;

            case 7:
                showSummary();
                break;

            case 0:
                printf("\nThank you for visiting UrbanVerse!\n");
                break;

            default:
                setColor(12);
                printf("\nInvalid choice! Please try again.\n");
                setColor(7);
        }

    } while (choice != 0);
}



void transportation(int area)
{
    int choice;
    int i;

    char transportMenu[4][40] =
    {
        "Public Buses",
        "Rickshaws",
        "Ride-Hailing Services",
        "Cycling and Walking"
    };

    char busOptions[2][40] =
    {
        "Other local bus services",
        "Local buses and minibuses"
    };

    char destinations[2][80] =
    {
        "Boat Basin, Sea View and Clifton Block areas.",
        "Tariq Road, PECHS and nearby areas."
    };

    char rides[3][40] =
    {
        "Bykea",
        "InDrive",
        "Other available ride services"
    };

    char walkInfo[2][80] =
    {
        "Walking areas include nearby parks and seaside locations.",
        "Walking is suitable for nearby markets and streets."
    };

    char walkTip[2][80] =
    {
        "Use designated pedestrian paths.",
        "Use safe pedestrian crossings."
    };

    do
    {
        printHeader("TRANSPORTATION");

        printf("Area: %s\n", areaNames[area - 1]);

        printMenu2D(transportMenu, 4, "Back to Main Menu");

        printPrompt("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            clearInputBuffer();
            choice = -1;
        }
        else
        {
            clearInputBuffer();
        }

        switch (choice)
        {
            case 1:
                printSubHeader("PUBLIC BUSES");

                printf("Available options around %s:\n",
                       areaNames[area - 1]);

                printf("1. People's Bus Service\n");
                printf("2. %s\n", busOptions[area - 1]);

                printf("Note: Check current routes and stops.\n");
                break;

            case 2:
                printSubHeader("RICKSHAWS");

                printf("Rickshaws are available for short-distance travel.\n");

                printf("Common destinations: %s\n",
                       destinations[area - 1]);

                break;

            case 3:
                printSubHeader("RIDE-HAILING SERVICES");

                printf("Options may include:\n");

                for (i = 0; i < 3; i++)
                {
                    printf("%d. %s\n", i + 1, rides[i]);
                }

                break;

            case 4:
                printSubHeader("CYCLING AND WALKING");

                printf("%s\n", walkInfo[area - 1]);

                printf("Suggestion: %s\n", walkTip[area - 1]);

                break;

            case 0:
                printf("\nReturning to Main Menu...\n");
                break;

            default:
                setColor(12);
                printf("\nInvalid choice!\n");
                setColor(7);
        }

    } while (choice != 0);
}



void healthcare(int area)
{
    struct PlaceList clifton =
    {
        "HEALTHCARE",
        3,
        {
            {"South City Hospital"},
            {"Nearby medical clinics"},
            {"Local pharmacies"}
        }
    };

    struct PlaceList tariq =
    {
        "HEALTHCARE",
        3,
        {
            {"Hill Park General Hospital (nearby PECHS)"},
            {"Nearby medical clinics"},
            {"Local pharmacies"}
        }
    };

    struct PlaceList *list;

    if (area == 1)
    {
        list = &clifton;
    }
    else
    {
        list = &tariq;
    }

    printHeader(list->title);

    printf("Area: %s\n\n", areaNames[area - 1]);

    printPlaces(list);
}


void entertainment(int area)
{
    int choice;

    struct MenuItem items[3] =
    {
        {"Parks and Outdoor Places"},
        {"Cinema and Malls"},
        {"Gaming Zones"}
    };

    do
    {
        printHeader("ENTERTAINMENT");

        printMenu(items, 3);

        printPrompt("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            clearInputBuffer();
            choice = -1;
        }
        else
        {
            clearInputBuffer();
        }

        switch (choice)
        {
            case 1:
                parks(area);
                break;

            case 2:
                cinema(area);
                break;

            case 3:
                gamingZones(area);
                break;

            case 0:
                printf("\nReturning to Main Menu...\n");
                break;

            default:
                setColor(12);
                printf("\nInvalid choice!\n");
                setColor(7);
        }

    } while (choice != 0);
}



void parks(int area)
{
    struct PlaceList clifton =
    {
        "PARKS AND OUTDOOR PLACES",
        3,
        {
            {"Bagh-e-Ibn-e-Qasim"},
            {"Clifton Beach"},
            {"Sea View"}
        }
    };

    struct PlaceList tariq =
    {
        "PARKS AND OUTDOOR PLACES",
        3,
        {
            {"Hill Park (PECHS)"},
            {"Nearby family parks"},
            {"Tariq Road shopping area"}
        }
    };

    struct PlaceList *list;

    if (area == 1)
    {
        list = &clifton;
    }
    else
    {
        list = &tariq;
    }

    printSubHeader(list->title);

    printPlaces(list);
}


void cinema(int area)
{
    struct PlaceList clifton =
    {
        "CINEMA AND MALLS",
        2,
        {
            {"Cinepax - Ocean Mall"},
            {"Dolmen Mall Clifton"}
        }
    };

    struct PlaceList tariq =
    {
        "CINEMA AND MALLS",
        2,
        {
            {"Nearby shopping centres"},
            {"Atrium Cinemas (Saddar - farther away)"}
        }
    };

    struct PlaceList *list;

    if (area == 1)
    {
        list = &clifton;
    }
    else
    {
        list = &tariq;
    }

    printSubHeader(list->title);

    printPlaces(list);
}



void gamingZones(int area)
{
    struct PlaceList clifton =
    {
        "GAMING ZONES",
        2,
        {
            {"Gaming facilities in nearby shopping malls"},
            {"Nearby sports and recreational facilities"}
        }
    };

    struct PlaceList tariq =
    {
        "GAMING ZONES",
        2,
        {
            {"Gaming facilities in nearby shopping centres"},
            {"Nearby sports and recreational facilities"}
        }
    };

    struct PlaceList *list;

    if (area == 1)
    {
        list = &clifton;
    }
    else
    {
        list = &tariq;
    }

    printSubHeader(list->title);

    printPlaces(list);
}



void restaurants(int area)
{
    int choice;

    struct MenuItem items[3] =
    {
        {"Pakistani Cuisine"},
        {"Chinese"},
        {"Fast Food"}
    };

    do
    {
        printHeader("RESTAURANTS");

        printMenu(items, 3);

        printPrompt("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            clearInputBuffer();
            choice = -1;
        }
        else
        {
            clearInputBuffer();
        }

        switch (choice)
        {
            case 1:
                pakistaniFood(area);
                break;

            case 2:
                chineseFood(area);
                break;

            case 3:
                fastFood(area);
                break;

            case 0:
                printf("\nReturning to Main Menu...\n");
                break;

            default:
                setColor(12);
                printf("\nInvalid choice!\n");
                setColor(7);
        }

    } while (choice != 0);
}



void pakistaniFood(int area)
{
    int i;
    int start;

    char food[6][40] =
    {
        "Kolachi (Do Darya)",
        "Boat Basin Food Street",
        "Nearby Pakistani restaurants",
        "Student Biryani (Tariq Road)",
        "Nearby Pakistani restaurants",
        "Local food outlets"
    };

    start = (area - 1) * 3;

    printSubHeader("PAKISTANI CUISINE");

    for (i = 0; i < 3; i++)
    {
        printf("%d. %s\n", i + 1, food[start + i]);
    }
}



void chineseFood(int area)
{
    int i;
    int start;

    char food[4][40] =
    {
        "Chinese restaurants in Boat Basin",
        "Nearby Chinese restaurants",
        "Chop Chop Wok (check nearest branch)",
        "Nearby Chinese restaurants"
    };

    start = (area - 1) * 2;

    printSubHeader("CHINESE FOOD");

    for (i = 0; i < 2; i++)
    {
        printf("%d. %s\n", i + 1, food[start + i]);
    }
}


void fastFood(int area)
{
    int i;
    int start;

    char food[6][40] =
    {
        "KFC",
        "McDonald's",
        "Burger Lab (nearby branches)",
        "KFC",
        "McDonald's",
        "Nearby burger and roll outlets"
    };

    start = (area - 1) * 3;

    printSubHeader("FAST FOOD");

    for (i = 0; i < 3; i++)
    {
        printf("%d. %s\n", i + 1, food[start + i]);
    }
}



void cityCrisis(int area)
{
    int choice;

    struct MenuItem items[4] =
    {
        {"Water Shortage"},
        {"Gas Issue"},
        {"Traffic and Cycling Safety"},
        {"Power Shortage"}
    };

    struct Crisis crisisList[4] =
    {
        {
            "WATER SHORTAGE",
            "Water supply problems in some residential blocks.",
            "Water supply problems in densely populated streets.",
            "Improve water distribution and storage."
        },

        {
            "GAS ISSUE",
            "Gas pressure problems in residential areas.",
            "Gas supply problems in residential streets.",
            "Improve gas infrastructure and maintenance."
        },

        {
            "TRAFFIC AND CYCLING SAFETY",
            "Traffic congestion near commercial and seaside areas.",
            "Traffic congestion around markets and busy roads.",
            "Better traffic management and separate cycling lanes."
        },

        {
            "POWER SHORTAGE",
            "Electricity interruptions in some residential blocks.",
            "Electricity interruptions in residential and commercial streets.",
            "Upgrade electricity infrastructure."
        }
    };

     
do
{
    printHeader("CITY CRISIS UPDATE");

    printf("Area: %s\n", areaNames[area - 1]);

    printMenu(items, 4);

    printPrompt("\nEnter your choice: ");

    if (scanf("%d", &choice) != 1)
    {
        clearInputBuffer();
        choice = -1;
    }
    else
    {
        clearInputBuffer();
    }

    switch (choice)
    {
        case 1:
            printSubHeader("WATER SUPPLY TIMINGS");

            if (area == 1)
            {
                printf("Area: Clifton\n");
                printf("Morning Supply: 6:00 AM - 9:00 AM\n");
                printf("Evening Supply: 5:00 PM - 7:00 PM\n");
                printf("Status: Water pressure may vary between blocks.\n");
            }
            else
            {
                printf("Area: Tariq Road\n");
                printf("Morning Supply: 7:00 AM - 10:00 AM\n");
                printf("Evening Supply: 4:00 PM - 6:00 PM\n");
                printf("Status: Water shortage may occur in some streets.\n");
            }
            break;

        case 2:
            printSubHeader("GAS SUPPLY TIMINGS");

            if (area == 1)
            {
                printf("Area: Clifton\n");
                printf("Morning Supply: 6:00 AM - 9:00 AM\n");
                printf("Evening Supply: 6:00 PM - 10:00 PM\n");
                printf("Status: Low gas pressure may occur in some blocks.\n");
            }
            else
            {
                printf("Area: Tariq Road\n");
                printf("Morning Supply: 5:00 AM - 8:00 AM\n");
                printf("Evening Supply: 7:00 PM - 10:00 PM\n");
                printf("Status: Gas pressure may be low during peak hours.\n");
            }
            break;

        case 3:
            printSubHeader("TRAFFIC UPDATES");

            if (area == 1)
            {
                printf("Area: Clifton\n");
                printf("Boat Basin: Heavy traffic during evening hours.\n");
                printf("Sea View: Increased traffic on weekends.\n");
                printf("Clifton Road: Moderate traffic.\n");
                printf("Suggestion: Avoid peak-hour travel where possible.\n");
            }
            else
            {
                printf("Area: Tariq Road\n");
                printf("Tariq Road Market: Heavy traffic during shopping hours.\n");
                printf("PECHS: Moderate traffic.\n");
                printf("Shahrah-e-Quaideen: Possible traffic congestion.\n");
                printf("Suggestion: Use alternative routes where possible.\n");
            }
            break;

        case 4:
            printSubHeader("POWER SUPPLY TIMINGS");

            if (area == 1)
            {
                printf("Area: Clifton\n");
                printf("Morning: 9:00 AM - 10:00 AM\n");
                printf("Afternoon: No scheduled interruption\n");
                printf("Evening: 7:00 PM - 8:00 PM\n");
                printf("Status: Possible electricity interruption.\n");
            }
            else
            {
                printf("Area: Tariq Road\n");
                printf("Morning: 8:00 AM - 9:00 AM\n");
                printf("Afternoon: 2:00 PM - 3:00 PM\n");
                printf("Evening: No scheduled interruption\n");
                printf("Status: Possible electricity interruption.\n");
            }
            break;

        case 0:
            printf("\nReturning to Main Menu...\n");
            break;

        default:
            setColor(12);
            printf("\nInvalid choice! Please try again.\n");
            setColor(7);
    }

} while (choice != 0);
}