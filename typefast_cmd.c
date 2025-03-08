// INCLUDE LIBRARIES
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

// LOGIN/REGISTER definitions
#define MAX_USERS 10
#define CREDENTIAL_LENGTH 30
#define FILE_NAME "users.dat"

typedef struct
{
    char username[CREDENTIAL_LENGTH];
    char password[CREDENTIAL_LENGTH];
    float avg_wpm;
    float highest_wpm;
    float avg_accuracy;
    float highest_accuracy;
    int tests_taken;
} User;

User users[MAX_USERS]; // Array of users
int user_count = 0;    // Number of registered users

// COLOR Definitions
const char *YELLOW = "\033[1;33m";
const char *BLUE = "\033[1;34m";
const char *GREEN = "\033[1;32m";
const char *CYAN = "\033[1;36m";
const char *RED = "\033[1;31m";
const char *WHITE = "\033[1;37m";
const char *COLOR_END = "\033[0m"; // Resets color to default

#define BLUE_T 1
#define GREEN_T 2
#define CYAN_T 3
#define RED_T 4
#define YELLOW_T 6
#define WHITE_T 7

// WORDS/PARA/QUOTES DEFINTION
#define MAX_WORDS 500        // Max words in words.txt
#define MAX_QUOTES 15        // Max quotes in quotes.txt
#define MAX_PARAGRAPHS 20    // Max paragraphs in para.txt
#define WORD_LENGTH 50       // Max length of each word
#define MAX_TEXT_LENGTH 2000 // Enough to store all words with spaces

typedef enum
{
    WORDS,
    QUOTES,
    PARAGRAPHS,
    LESSON1,
    LESSON2,
    LESSON3,
    LESSON4
} Mode;

// Function Prototypes
void setConsoleTitle();
void setColor(int);
void setCursorPos(int, int);
int getConsoleWidth();
void register_user();
int login_user();
void hide_cursor();
void show_cursor();
void input_credentials(char *, char *);
void get_password(char *, int);
int menu_navigation(char *[], int);
void save_users_to_file();
void load_users_from_file();
void generateRandomText(Mode mode, int numWords, char *result);
void startTypingTest(char *text, int user_index);
void update_stats(const char *username, float wpm, float accuracy);
void view_stats(const char *username);
void start_timed_mode(int user_index);
void showTimer(int timeLeft);

int main()
{
    system("cls");
    setConsoleTitle();
    load_users_from_file(); // Load existing users from file

    printf("%sWelcome to TYPEFAST.cmd%s\n", YELLOW, COLOR_END);
    int user_index = -1; // -1 indicates no user is logged in

    char *OptionsDefaultRegLogout[] = {"Register/Sign Up", "Login/Sign In", "Take a typing test (Guest Mode)", "Exit"};
    char *OptionsGuestMode[] = {"Timed Test", "Challenge Mode"};
    char *OptionsChallenge[] = {"Random Words", "Quotes", "Paragraphs"};
    char *OptionsLogin[] = {"Start Typing", "Your Typing Stats", "Logout"}; // Logged-in menu options
    char *OptionsType[] = {"Timed", "Random Words", "Quotes", "Paragraphs", "Lessons"};
    char *OptionsLessons[]={"Lesson1","Lesson2","Lesson3","Lesson4"};

    while (1)
    {
        int option;
        if (user_index == -1) // No user logged in
        {
            option = menu_navigation(OptionsDefaultRegLogout, sizeof(OptionsDefaultRegLogout) / sizeof(OptionsDefaultRegLogout[0]));
        }
        else // User is logged in
        {
            option = menu_navigation(OptionsLogin, sizeof(OptionsLogin) / sizeof(OptionsLogin[0]));
        }

        switch (option)
        {
        case 1:
            if (user_index == -1) // Register option
            {
                register_user();
            }
            else // Start Typing option
            {
                int optionTyp = menu_navigation(OptionsType, sizeof(OptionsType) / sizeof(OptionsType[0]));

                char result[MAX_TEXT_LENGTH];
                int numWords = 50;

                switch (optionTyp)
                {
                case 1:
                    start_timed_mode(user_index);
                    break;
                case 2:
                    generateRandomText(0, 20, result);
                    startTypingTest(result, user_index);
                    break;
                case 3:
                    generateRandomText(1, numWords, result);
                    startTypingTest(result, user_index);
                    break;
                case 4:
                    generateRandomText(2, numWords, result);
                    startTypingTest(result, user_index);
                    break;
                case 5:
                    int option_lessons = menu_navigation(OptionsLessons, sizeof(OptionsLessons) / sizeof(OptionsLessons[0]));
                    
                    switch(option_lessons)
                    {
                        case 1:
                            generateRandomText(LESSON1, numWords, result);
                            startTypingTest(result,user_index);
                            break;
                        case 2:
                            generateRandomText(LESSON2, numWords, result);
                            startTypingTest(result,user_index);
                            break;
                        case 3:
                            generateRandomText(LESSON3, numWords, result);
                            startTypingTest(result,user_index);
                            break;
                        case 4:
                            generateRandomText(LESSON4, numWords, result);
                            startTypingTest(result,user_index);
                            break;

                    }

                default:
                    break;
                }
            }
            break;
        case 2:
            if (user_index == -1) // Login option
            {
                user_index = login_user();
                if (user_index >= 0)
                {
                    printf("%sLogin successful! Welcome, %s%s%s!%s\n", GREEN, COLOR_END, YELLOW, users[user_index].username, COLOR_END);
                }
                else
                {
                    printf("%sLogin Failed! Incorrect username or password%s\n", RED, COLOR_END);
                }
            }
            else // View Stats option
            {
                view_stats(users[user_index].username);
            }
            break;
        case 3:
            if (user_index == -1) // Guest Mode option
            {
                int optionGuest = menu_navigation(OptionsGuestMode, sizeof(OptionsGuestMode) / sizeof(OptionsGuestMode[0]));
                switch (optionGuest)
                {
                case 1:
                    start_timed_mode(user_index);
                    // run timed test
                    break;
                case 2:
                    int optionCh = menu_navigation(OptionsChallenge, sizeof(OptionsChallenge) / sizeof(OptionsChallenge[0]));

                    char result[MAX_TEXT_LENGTH];
                    int numWords = 50;

                    switch (optionCh)
                    {
                    case 1:
                        generateRandomText(0, 20, result);
                        startTypingTest(result, user_index);
                        break;
                    case 2:
                        generateRandomText(1, numWords, result);
                        startTypingTest(result, user_index);
                        break;
                    case 3:
                        generateRandomText(2, numWords, result);
                        startTypingTest(result, user_index);
                        break;
                    }
                    break;
                }
            }
            else // Logout option
            {
                user_index = -1; // Reset user_index to indicate logout
                printf("%sLogged out successfully!%s\n", BLUE, COLOR_END);
            }
            break;
        case 4:
            if (user_index == -1) // Exit option
            {
                printf("%s\nExiting program.%s\n", BLUE, COLOR_END);
                return 0;
            }
            break;
        }
    }

    getch();
    return 0;
}

// Load users from binary file
void load_users_from_file()
{
    FILE *file = fopen(FILE_NAME, "rb");
    if (file != NULL)
    {
        fread(&user_count, sizeof(int), 1, file);
        fread(users, sizeof(User), user_count, file);
        fclose(file);
        // printf("Users loaded from file successfully. Total users: %d\n", user_count); // Debug
    }
    else
    {
        // printf("No existing user file found. Starting with 0 users.\n"); // Debug
    }
}
// Save users to binary file
void save_users_to_file()
{
    FILE *file = fopen(FILE_NAME, "wb");
    if (file != NULL)
    {
        fwrite(&user_count, sizeof(int), 1, file);
        fwrite(users, sizeof(User), user_count, file);
        fclose(file);
        // printf("Users saved to file successfully. Total users: %d\n", user_count); // Debug
    }
    else
    {
        // printf("Error saving users to file!\n"); // Debug
    }
}

// Register new user
void register_user()
{
    if (user_count == MAX_USERS)
    {
        printf("\n%sMaximum %d users are supported! No more registrations allowed!!!%s\n", RED, MAX_USERS, COLOR_END);
        return;
    }

    char temp_username[CREDENTIAL_LENGTH];
    char temp_password[CREDENTIAL_LENGTH];

    printf("\n%sRegister a new user: %s\n", CYAN, COLOR_END);

    while (1)
    {
        printf("%s\nEnter username: %s", CYAN, COLOR_END);
        fgets(temp_username, CREDENTIAL_LENGTH, stdin);
        temp_username[strcspn(temp_username, "\n")] = '\0'; // Remove newline

        int is_unique = 1;
        for (int i = 0; i < user_count; i++)
        {
            if (strcmp(temp_username, users[i].username) == 0)
            {
                is_unique = 0;
                printf("\n%sUsername already taken! Please choose another.%s\n", RED, COLOR_END);
                break;
            }
        }

        if (is_unique)
        {
            break;
        }
    }

    printf("%sEnter password (masking enabled): %s", CYAN, COLOR_END);
    get_password(temp_password, CREDENTIAL_LENGTH);

    strcpy(users[user_count].username, temp_username);
    strcpy(users[user_count].password, temp_password);
    users[user_count].avg_wpm = 0;
    users[user_count].highest_wpm = 0;
    users[user_count].tests_taken = 0;
    users[user_count].avg_accuracy = 0;
    users[user_count].highest_accuracy = 0;
    user_count++;

    save_users_to_file(); // Save updated users to file
    printf("%s\nRegistration successful! User: '%s'%s\n", GREEN, temp_username, COLOR_END);
}
// Login user
int login_user()
{
    char username[CREDENTIAL_LENGTH];
    char password[CREDENTIAL_LENGTH];

    printf("\n%sLogin: %s\n", CYAN, COLOR_END);

    input_credentials(username, password);

    for (int i = 0; i < user_count; i++)
    {
        // printf("Checking user: %s\n", users[i].username); // Debug
        if (strcmp(username, users[i].username) == 0 && strcmp(password, users[i].password) == 0)
        {
            // printf("Login successful! User: %s\n", username); // Debug
            return i;
        }
    }

    // printf("Login failed! User not found: %s\n", username); // Debug
    return -1;
}

// Get user credentials
void input_credentials(char *username, char *password)
{
    printf("\n%sEnter username: %s", CYAN, COLOR_END);
    fgets(username, CREDENTIAL_LENGTH, stdin);
    username[strcspn(username, "\n")] = '\0'; // Remove newline

    printf("%sEnter password (masking enabled): %s", CYAN, COLOR_END);
    get_password(password, CREDENTIAL_LENGTH);
}

// Password masking
void get_password(char *password, int maxLength)
{
    int i = 0;
    char ch;

    while (1)
    {
        ch = getch();
        if (ch == 13) // Enter key
        {
            break;
        }
        else if (ch == 8) // Backspace
        {
            if (i > 0)
            {
                printf("\b \b");
                i--;
            }
        }
        else if (i < maxLength - 1)
        {
            password[i++] = ch;
            printf("*");
        }
    }

    password[i] = '\0';
    printf("\n");
}

// Set Console Title
void setConsoleTitle()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTitle("TYPEFAST.cmd");
}

int getConsoleWidth()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwSize.X;
}

// Set Cursor Position
void setCursorPos(int x, int y)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {x, y};
    SetConsoleCursorPosition(hConsole, coord);
}

void setColor(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Menu Navigation
int menu_navigation(char *options[], int size)
{
    int selected = 1;
    int ch;

    while (1)
    {
        for (int i = 0; i < size; i++)
        {
            printf("%s%s%d. %s%s\n",
                   selected == i + 1 ? "\033[1;42m" : "",
                   selected == i + 1 ? ">> " : "   ",
                   i + 1,
                   options[i],
                   COLOR_END);
        }

        ch = getch();
        if (ch)
        {
            system("cls");
        }
        if (ch == 224) // Arrow keys
        {
            ch = getch();
            if (ch == 72 && selected > 1) // Up arrow
                selected--;
            else if (ch == 80 && selected < size) // Down arrow
                selected++;
        }
        else if (ch == 13) // Enter key
        {
            return selected;
        }
    }
}

void generateRandomText(Mode mode, int numWords, char *result)
{
    FILE *file;
    char words[MAX_WORDS][WORD_LENGTH];
    char quotes[MAX_QUOTES][MAX_TEXT_LENGTH];
    char paragraphs[MAX_PARAGRAPHS][MAX_TEXT_LENGTH];
    char lesson1[MAX_PARAGRAPHS][MAX_TEXT_LENGTH]; // Add this line
    char lesson2[MAX_PARAGRAPHS][MAX_TEXT_LENGTH];
    char lesson3[MAX_PARAGRAPHS][MAX_TEXT_LENGTH];
    char lesson4[MAX_PARAGRAPHS][MAX_TEXT_LENGTH];
    int count = 0;

    switch (mode)
    {
    case WORDS:
        file = fopen("words.txt", "r");
        break;
    case QUOTES:
        file = fopen("quotes.txt", "r");
        break;
    case PARAGRAPHS:
        file = fopen("para.txt", "r");
        break;
    case LESSON1: // Add this case
        file = fopen("lesson1.txt", "r");
        break;
    case LESSON2: // Add this case
        file = fopen("lesson2.txt", "r");
        break;
    case LESSON3: // Add this case
        file = fopen("lesson3.txt", "r");
        break;
    case LESSON4: // Add this case
        file = fopen("lesson4.txt", "r");
        break;
    default:
        printf("Invalid mode!\n");
        return;
    }

    if (!file)
    {
        printf("Error opening file.\n");
        return;
    }

    if (mode == WORDS)
    {
        while (count < MAX_WORDS && fscanf(file, "%49s", words[count]) == 1)
        {
            count++;
        }
    }
    else
    {
        while (count < (mode == QUOTES ? MAX_QUOTES : MAX_PARAGRAPHS) &&
               fgets(mode == QUOTES ? quotes[count] :
                    (mode == LESSON1 ? lesson1[count] :
                     (mode == LESSON2 ? lesson2[count] :
                      (mode == LESSON3 ? lesson3[count] : 
                        (mode== LESSON4 ? lesson3[count]: paragraphs[count])))),
                     MAX_TEXT_LENGTH, file))
        {
            count++;
        }
    }
    fclose(file);

    if (count == 0)
    {
        printf("No data found in file.\n");
        return;
    }
    

    srand(time(NULL));
    result[0] = '\0';

    if (mode == WORDS)
    {
        for (int i = 0; i < numWords; i++)
        {
            int randIndex = rand() % count;
            strcat(result, words[randIndex]);
            if (i < numWords - 1)
            {
                strcat(result, " ");
            }
        }
    }
    else
    {
        int randIndex = rand() % count;
        strcat(result, mode == QUOTES ? quotes[randIndex] : (mode == LESSON1 ? lesson1[randIndex] : 
            (mode == LESSON2 ? lesson2[randIndex] :
            (mode == LESSON3 ? lesson3[randIndex] : 
              (mode== LESSON4 ? lesson3[randIndex]: paragraphs[randIndex])))));
    }
}

void startTypingTest(char *text, int user_index)
{
    system("cls");
    // int user_index_typing = login_user();
    printf("%s", text);
    setCursorPos(0, 0);

    int len = strlen(text);
    int i = 0, mistakes = 0, totalKeystrokes = 0;
    int x = 0, y = 0;

    time_t startTime = time(NULL);

    int console_width = getConsoleWidth();

    while (i < len)
    {
        char ch = getch();

        if (ch == 27) // ESC key pressed
        {
            system("cls");
            return; // Go back to mode selection
        }

        if (ch == 8) // Backspace handling
        {
            if (i > 0)
            {
                i--;

                // Handle cursor movement when backspacing across lines
                if (x == 0 && y > 0)
                {
                    y--;
                    x = console_width - 1; // Move to the last character of the previous line
                }
                else
                {
                    x--;
                }

                setCursorPos(x, y);
                setColor(WHITE_T);
                printf("%c", text[i]); // Restore original character
                setCursorPos(x, y);
            }
            continue;
        }

        totalKeystrokes++;

        if (ch == ' ' && text[i] != ' ')
        {
            mistakes++;
            continue;
        }

        if (ch == text[i])
        {
            setColor(GREEN_T);
        }
        else
        {
            setColor(RED_T);
            mistakes++;
        }

        printf("%c", ch);
        setColor(WHITE_T);

        i++;

        // Handle cursor movement across lines
        if (x == console_width - 1) // End of line reached
        {
            x = 0;
            y++;
        }
        else
        {
            x++;
        }

        setCursorPos(x, y);
    }

    time_t endTime = time(NULL);
    double elapsedTime = difftime(endTime, startTime);
    double wordsTyped = len / 5.0;
    double WPM = (wordsTyped / elapsedTime) * 60.0;
    double accuracy = ((double)(totalKeystrokes - mistakes) / totalKeystrokes) * 100.0;

    if (user_index >= 0)
    {
        update_stats(users[user_index].username, WPM, accuracy);
    }

    printf("\n%sTyping Complete!%s\n", BLUE, COLOR_END);
    printf("TOTAL TIME: %.2f seconds\n", elapsedTime);
    printf("Words Per Minute (WPM): %.2f\n", WPM);
    printf("ACCURACY: %.2f%%\n", accuracy);
    printf("MISTAKES: %d\n", mistakes);
    printf("\n");
    printf("%sPress Enter to Return to Main Menu: %s", CYAN, COLOR_END);
    getch();
    system("cls");
}
void update_stats(const char *username, float wpm, float accuracy)
{
    FILE *file = fopen(FILE_NAME, "rb+");
    if (!file)
    {
        printf("Error opening file!\n");
        return;
    }

    // Read the user count from the file
    int file_user_count;
    fread(&file_user_count, sizeof(int), 1, file);

    User user;
    int found = 0;

    // Iterate through each user in the file
    for (int i = 0; i < file_user_count; i++)
    {
        fread(&user, sizeof(User), 1, file);

        if (strcmp(user.username, username) == 0)
        {
            found = 1;

            // Update user stats
            user.tests_taken++;
            user.avg_wpm = ((user.avg_wpm * (user.tests_taken - 1)) + wpm) / user.tests_taken;
            user.avg_accuracy = ((user.avg_accuracy * (user.tests_taken - 1)) + accuracy) / user.tests_taken;
            if (wpm > user.highest_wpm)
            {
                user.highest_wpm = wpm;
            }
            if (accuracy > user.highest_accuracy)
            {
                user.highest_accuracy = accuracy;
            }

            // Move the file pointer back to the start of the current user's record
            fseek(file, -(long)sizeof(User), SEEK_CUR);
            fwrite(&user, sizeof(User), 1, file); // Write updated user data
            break;
        }
    }

    fclose(file);

    if (!found)
    {
        printf("User not found: %s\n", username); // Debug
    }
}
void view_stats(const char *username)
{
    FILE *file = fopen(FILE_NAME, "rb");
    if (!file)
    {
        printf("Error opening file!\n");
        return;
    }

    // Read the user count from the file
    int file_user_count;
    fread(&file_user_count, sizeof(int), 1, file);

    User user;
    int found = 0;

    // Iterate through each user in the file
    for (int i = 0; i < file_user_count; i++)
    {
        fread(&user, sizeof(User), 1, file);

        if (strcmp(user.username, username) == 0)
        {
            found = 1;
            printf("%s\nStats for: %s%s\n\n", BLUE, user.username, COLOR_END);
            printf("TESTS TAKEN: %d\n", user.tests_taken);
            printf("AVERAGE WPM: %.2f\n", user.avg_wpm);
            printf("HIGHEST WPM: %s%.2f%s\n", GREEN, user.highest_wpm, COLOR_END);
            printf("AVERAGE ACCURACY: %.2f%%\n", user.avg_accuracy);
            printf("HIGHEST ACCURACY: %s%.2f%%%s\n", GREEN, user.highest_accuracy, COLOR_END);
            break;
        }
    }

    fclose(file);

    if (!found)
    {
        printf("User not found: %s\n", username); // Debug
    }
}
void start_timed_mode(int user_index)
{
    system("cls"); // Clear screen

    char result[MAX_TEXT_LENGTH];
    generateRandomText(WORDS, 50, result); // Generate 50 random words
    int len = strlen(result);

    printf("%s", result); // Display the random text
    setCursorPos(0, 0);   // Move cursor to the start of text

    int i = 0, mistakes = 0, totalKeystrokes = 0;
    time_t startTime = time(NULL);   // Start time
    time_t endTime = startTime + 15; // Set time limit to 15 seconds

    int lastTimeLeft = 15; // Track last displayed time to reduce updates

    while (i < len)
    {
        time_t currentTime = time(NULL);
        int timeLeft = (int)(endTime - currentTime);

        // Update countdown timer only if time has changed
        if (timeLeft != lastTimeLeft)
        {
            showTimer(timeLeft); // Update the timer without moving the cursor
            lastTimeLeft = timeLeft;
        }

        if (timeLeft <= 0)
        {
            break; // Stop input if time runs out
        }

        // Non-blocking input check
        if (_kbhit()) // Check if a key is pressed
        {
            char ch = _getch(); // Get the pressed key

            // Handle ESC key
            if (ch == 27) // ESC key pressed
            {
                system("cls");
                return; // Go back to main menu
            }

            // Handle Backspace
            if (ch == 8) // ASCII code for Backspace
            {
                if (i > 0)
                {
                    i--;
                    setCursorPos(i, 0); // Move cursor back
                    setColor(WHITE_T);
                    printf("%c", result[i]); // Restore original character
                    setCursorPos(i, 0);      // Keep cursor at corrected position
                }
                continue;
            }

            totalKeystrokes++; // Increase total keystroke count

            // Handle Spacebar in the middle of a word (incorrect space)
            if (ch == ' ' && result[i] != ' ')
            {
                mistakes++;
                continue;
            }

            // Check if correct character is typed
            if (ch == result[i])
            {
                setColor(GREEN_T);
            }
            else
            {
                setColor(RED_T);
                mistakes++;
            }

            setCursorPos(i, 0);      // Ensure cursor stays at correct position before printing
            printf("%c", result[i]); // Print the correct text at its place
            setCursorPos(i + 1, 0);  // Move cursor forward to next position
            setColor(WHITE_T);
            i++;
        }
    }

    // Calculate WPM and Accuracy
    double elapsedTime = 15 - (endTime - time(NULL)); // Total time taken (up to 15 seconds)
    double wordsTyped = i / 5.0;                      // Average word length is 5
    double WPM = (wordsTyped / elapsedTime) * 60.0;
    double accuracy = ((double)(totalKeystrokes - mistakes) / totalKeystrokes) * 100.0;

    // Clear the area below the text for results
    int textLines = (len / getConsoleWidth()) + 1; // Calculate number of lines occupied by text
    setCursorPos(0, textLines + 2);                // Position results below the text

    // Display final results
    printf("\n%sTIMES'S UP!%s\n\n", BLUE, COLOR_END);
    printf("Words Per Minute (WPM): %.2f\n", WPM);
    printf("ACCURACY: %.2f%%\n", accuracy);
    // printf("Total Mistakes: %d\n", mistakes);

    // Update user stats if logged in
    if (user_index >= 0)
    {
        update_stats(users[user_index].username, WPM, accuracy);
    }

    printf("\n%sPress Enter to Return to Main Menu: %s\n", CYAN, COLOR_END);
    while (_getch() != 13)
        ; // Wait for Enter key
    system("cls");
}

void hide_cursor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE; // Hide the cursor
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void show_cursor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = TRUE; // Show the cursor
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}
void showTimer(int timeLeft)
{
    static int lastTimeLeft = -1; // Track last displayed time to reduce updates

    if (timeLeft != (lastTimeLeft + 1))
    {
        // Move to the timer position (without affecting the cursor in the typing area)
        COORD cursorPos;
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        cursorPos = csbi.dwCursorPosition; // Save the current cursor position

        // Update the timer at a fixed position (e.g., row 10)
        setCursorPos(0, 10); // Position timer at the bottom of the screen
        setColor(WHITE_T);
        printf("%sTime Left: %2d seconds  %s", YELLOW, timeLeft, COLOR_END); // Use spaces to clear previous text

        // Restore the cursor to the typing area
        SetConsoleCursorPosition(hConsole, cursorPos);

        lastTimeLeft = timeLeft;
    }
}