#ifndef COMMONHELPERS_H_
#define COMMONHELPERS_H_

// Function Prototypes:

// Uses the time.h library to obtain current year information
// Get the current 4 - digit year from the system
int currentYear(void);

// Empty the standard input buffer
void clearStandardInputBuffer(void);

// Get a valid integer
// Return an integer value and receives no arguments
int getInteger(void);

// Get a valid positive integer
// Return an integer value and receives no arguments
int getPositiveInteger(void);


// Get a valid positive integer
// Return an integer value or zero and receives no arguments
int getNotNegativeInteger(void);


// Get a valid double value from the keyboard
// Return a double value and receives no arguments
double getDouble(void);

// Get a valid positive double value from the keyboard
// Return a positive double value and receives no arguments
double getPositiveDouble(void);

// Return an integer value between lower-bound to upper-bound
int getIntFromRange(int min_num, int max_num);

// Return a single character value and receives one argument
char getCharOption(char* list);

// Get a C string value with a length between Min and Max range
// Range specified in the 2nd and 3rd arguments received(inclusive)
void getCString(char* str2, int min_range, int max_range);

// Get a C string value with a length between Min and Max range
// Range specified in the 2nd and 3rd arguments received(inclusive)
void getCString_userlogin(char* str2, int min_range, int max_range);

// Get a C string value with a length between Min and Max range
// Range specified in the 2nd and 3rd arguments received(inclusive)
void getCString_password(char* str2, int min_range, int max_range);

// Get a C string value with a length between Min and Max range
// Range specified in the 2nd and 3rd arguments received(inclusive)
void getCString_country(char* str2, int min_range, int max_range);
#endif