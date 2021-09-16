#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "commonHelpers.h"
#include "account.h"
#include "ticket.h"

// Uses the time.h library to obtain current year information
// Get the current 4 - digit year from the system
int currentYear(void) {
    time_t currentTime = time(NULL);
    return localtime(&currentTime)->tm_year + 1900;
}

// Empty the standard input buffer
void clearStandardInputBuffer(void) {
    while (getchar() != '\n')
    {
        ; // On purpose: do nothing
    }
}

// Get a valid integer
// Return an integer value and receives no arguments
int getInteger(void) {
    int num = 0;
    char str1 = 'a';
    while (str1 != '\n') {
        scanf("%d%c", &num, &str1);
        if (str1 != '\n') {
            while (getchar() != '\n');
            printf("ERROR: Value must be an integer: ");
        }
    }
    return num;
}

// Get a valid positive integer
// Return an integer value and receives no arguments
int getPositiveInteger(void) {
    int  p = 1, i, len, num, m;
    char str1[100], str0[100] = { 0 };
    while (p) {
        i = 0, m = 0, num = 0, len = 0;
        strcpy(str1, str0);
        while ((str1[len] = getchar()) != '\n')
            len++;
        if (len == 1) {
            if (isdigit(str1[0]))
                m = 2;
            else
                m = 1;
        }
        else if (len > 1) {
            if (str1[0] != '-' && str1[0] != '+' && (!isdigit(str1[0])))
                m = 1;
            else {
                for (i = 1; i < len; i++) {
                    if (isdigit(str1[i]))
                        m = 2;
                    else {
                        m = 1;
                        break;
                    }
                }
            }
        }
        if (m == 1) {
            printf("ERROR: Value must be an integer: ");
            p = 1;
        }
        else if (m == 2) {
            num = atoi(str1);
            if (num <= 0) {
                printf("ERROR: Value must be an positive integer: ");
                p = 1;
            }
            else
                break;
        }
    }
    return num;
}

// Get a valid positive integer
// Return an integer value or zero and receives no arguments
int getNotNegativeInteger(void) {
    int  p = 1, i, num, m;
    char str1[100], str0[100] = { 0 };
    while (p) {
        i = 0, m = 0, num = 0;
        strcpy(str1, str0);
        while ((str1[i] = getchar()) != '\n') {
            if ((str1[0] == '-' || str1[0] == '+' || isdigit(str1[0]))) {
                m = 1;
                i++;
            }
            else if (str1[i]) {
                m = 1;
                i++;
            }
            else
                m = 2;
        }

        if (m == 2) {
            printf("ERROR: Value must be an integer: ");
            p = 1;
        }
        else if (m == 1) {
            num = atoi(str1);
            if (num < 0) {
                printf("ERROR: Value must be positive or zero: ");
                p = 1;
            }
            else
                break;
        }
        else if (m == 0)
            printf("ERROR: Please input an integer: ");
    }
    return num;
}

// Get a valid double value from the keyboard
// Return a double value and receives no arguments
double getDouble(void) {
    double num = 0.0;
    char str1 = 'a';
    while (str1 != '\n') {
        scanf("%lf%c", &num, &str1);
        if (str1 != '\n') {
            while (getchar() != '\n');
            printf("ERROR: Value must be a double floating-point number: ");
        }
    }
    return num;
}

// Get a valid positive double value from the keyboard
// Return a positive double value and receives no arguments
double getPositiveDouble(void) {
    double num = 0.0;
    char str1 = 'a';
    while (str1 != '\n' || num <= 0) {
        scanf("%lf%c", &num, &str1);
        if (str1 != '\n') {
            while (getchar() != '\n');
            printf("ERROR: Value must be a double floating-point number: ");
        }
        else if (str1 == '\n' && num <= 0)
            printf("ERROR: Value must be a positive double floating-point number: ");
    }
    return num;
}

// Return an integer value between lower-bound to upper-bound.
int getIntFromRange(int min_num, int max_num) {
    int num = 0, p = 1, m, len = 0;
    char str1[100] = { 0 }, str2[100] = { 0 };
    while (p) {
        m = 0, len = 0;
        strcpy(str1, str2);
        while ((str1[len] = getchar()) != '\n') {
            if (len == 0 && (str1[len] == '-' || str1[len] == '+'))
                m = 2;
            else if (isdigit(str1[len]))
                m = 2;
            else if (isdigit(str1[len]) == 0)
                m = 1;
            len++;
        }
        if (m == 0)
            printf("ERROR: Please input an integer: ");
        if (m == 1) {
            printf("ERROR: Value must be an integer: ");
            continue;
        }
        else if (m == 2) {
            num = atoi(str1);
            if (num< min_num || num>max_num) {
                printf("ERROR: Value must be between %d and %d inclusive: ",
                    min_num, max_num);
                continue;
            }
            else if (num >= min_num && num <= max_num)
                break;
            num = 0;
        }
    }
    return num;
}

// Return a single character value and receives one argument
char getCharOption(char* list) {
    int  p = 1, i, len, m;
    char str1[100];
    while (p) {
        m = 0;
        len = 0;
        i = 0;
        while ((str1[i] = getchar()) != '\n')
            i++;
        if (i > 1) {
            printf("ERROR: Character must be one of [%s]: ", list);
            continue;
        }
        while (list[len] != 0)
            len++;
        for (i = 0; i < len; i++) {
            if (list[i] == str1[0])
                m = 1;
        }
        if (m == 1)
            break;
        else if (m == 0) {
            printf("ERROR: Character must be one of [%s]: ", list);
            p = 1;
        }
    }
    return str1[0];
}

// Get a C string value with a length between Min and Max range
// Range specified in the 2nd and 3rd arguments received(inclusive)
void getCString(char* str2, int min_range, int max_range) {
    int  p = 1, i;
    char str1[100];
    while (p) {
        i = 0;
        scanf("%[^\n]", str1);
        while (str1[i] != '\0')
            i++;
        if (i < min_range || i> max_range) {
            if ((min_range == max_range) && (i != max_range))
                printf("ERROR: String length must be exactly %d chars: ", max_range);
            else if ((min_range < max_range) && (i > max_range))
                printf("ERROR: String length must be no more than %d chars: ", max_range);
            else if ((min_range < max_range) && (i < min_range))
                printf("ERROR: String length must be between %d and %d chars: ",
                    min_range, max_range);
            while (getchar() != '\n');
            p = 1;
        }
        else if (i >= min_range && i <= max_range) {
            strcpy(str2, str1);
            p = 0;
            while (getchar() != '\n');
        }
    }
    return;
}

// Get a C string value with a length between Min and Max range
// Range specified in the 2nd and 3rd arguments received(inclusive)
void getCString_userlogin(char* str2, int min_range, int max_range) {
    int  p = 1, i = 0, len;
    char str1[100], str0[100] = { 0 };
    while (p) {
        len = 0;
        strcpy(str1, str0);
        scanf("%[^\n]", str1);
        while (str1[len] != '\0')
            len++;
        if (len < min_range || len> max_range) {
            if ((min_range == max_range) && (len != max_range))
                printf("ERROR: String length must be exactly %d chars: ", max_range);
            else if ((min_range < max_range) && (len > max_range))
                printf("ERROR: String length must be no more than %d chars: ", max_range);
            else if ((min_range < max_range) && (len < min_range))
                printf("ERROR: String length must be between %d and %d chars: ",
                    min_range, max_range);
        }
        else if (len >= min_range && len <= max_range) {
            for (i = 0; i < len; i++) {
                if (str1[i] == ' ') {
                    printf("ERROR:  The user login must NOT contain whitespace characters.\n");
                    printf("Enter user login (%d chars max): ", max_range);
                    p = 1;
                    break;
                }
                else {
                    strcpy(str2, str1);
                    p = 0;
                }
            }
        }
        while (getchar() != '\n');
    }
    return;
}

// Get a C string value with a length between Min and Max range
// Range specified in the 2nd and 3rd arguments received(inclusive)
void getCString_password(char* str2, int min_range, int max_range) {
    int  p = 1, i, t, num_digit, num_upper, num_lower, num_symbol;
    char str1[100], str0[100] = { 0 }, first[20] = { '!','@','#','$','%','^','&','*' };
    while (p) {
        num_digit = 0, num_upper = 0, num_lower = 0, num_symbol = 0, i = 0;
        strcpy(str1, str0);
        scanf("%[^\n]", str1);
        while (str1[i] != '\0') {
            if (isdigit(str1[i]))
                num_digit++;
            if (isupper(str1[i]))
                num_upper++;
            if (islower(str1[i]))
                num_lower++;
            for (t = 0; t < 20; t++) {
                if (str1[i] == first[t]) {
                    num_symbol++;
                    break;
                }
            }
            i++;
        }
        if (i < min_range || i> max_range) {
            if ((min_range == max_range) && (i != max_range))
                printf("ERROR: String length must be exactly %d chars: ", max_range);
            else if ((min_range < max_range) && (i > max_range))
                printf("ERROR: String length must be no more than %d chars: ", max_range);
            else if ((min_range < max_range) && (i < min_range))
                printf("ERROR: String length must be between %d and %d chars: ",
                    min_range, max_range);
            while (getchar() != '\n');
            p = 1;
            continue;
        }
        else if (i >= min_range && i <= max_range) {
            if (num_digit < 2 || num_upper < 2 || num_lower < 2 || num_symbol < 2) {
                printf("SECURITY: Password must contain 2 of each:\n");
                printf("          Digit: 0-9\n");
                printf("          UPPERCASE character\n");
                printf("          lowercase character\n");
                printf("          symbol character: !@#$%%^&*\n");
                if (min_range == max_range)
                    printf("Enter the password (must be %d chars in length): ", min_range);
                else if (min_range < max_range)
                    printf("Enter the password (must be between %d and %d chars"
                        "in length): ", min_range, max_range);
                p = 1;
                while (getchar() != '\n');
                continue;
            }
            else {
                strcpy(str2, str1);
                p = 0;
                while (getchar() != '\n');
            }
        }
    }
    return;
}


// Get a C string value with a length between Min and Max range
// Range specified in the 2nd and 3rd arguments received(inclusive)
void getCString_country(char* str2, int min_range, int max_range) {
    int  p = 1, i;
    char str1[100], str0[100] = { 0 };
    while (p) {
        i = 0;
        strcpy(str1, str0);
        scanf("%[^\n]", str1);
        while (str1[i] != '\0')
            i++;
        if (i < min_range || i> max_range) {
            if ((min_range == max_range) && (i != max_range))
                printf("ERROR: String length must be exactly %d chars: ", max_range);
            else if ((min_range < max_range) && (i > max_range))
                printf("ERROR: String length must be no more than %d chars: ", max_range);
            else if ((min_range < max_range) && (i < min_range))
                printf("ERROR: String length must be between %d and %d chars: ",
                    min_range, max_range);
            while (getchar() != '\n');
            p = 1;
        }
        else if (i >= min_range && i <= max_range) {
            for (i = 0; i < sizeof(str1); i++) {
                str2[i] = toupper(str1[i]);
            }
            p = 0;
            while (getchar() != '\n');
        }
    }
    return;
}