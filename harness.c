// ECE 309 Project 1 - Program Code
// Scarlett Nguyen - smnguye3

// Including standard libraries for basic functions.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Setting the maximum numbers
#define MAX_INPUT 256
#define HISTORY_SIZE 5

// Defines a structure 'Turn' to hold one user and model response.
typedef struct {
    // Stores user and model response for the current turn.
    char user[MAX_INPUT];
    char assistant[MAX_INPUT];
} Turn;

// Defines a function that removes the newline character added with fgets().
void trim_newline(char *text) {
    // Finds the current length of the string.
    size_t length = strlen(text);
    // Checks if the string isn't empty and the last character is a newline.
    if (length > 0 && text[length - 1] == '\n') {
        // Replaces the newline with a null.
        text[length - 1] = '\0';
    }
}

// Defines a function that copies a string while also converting every character to lowercase. 
void lowercase_copy(const char *source, char *destination, size_t size) {
    // Loop counter
    size_t i;
    // Copies characters over one at a time until the end or it is full.
    for (i = 0; i + 1 < size && source[i] != '\0'; i++) {
        // Converts to lowercase
        destination[i] = (char)tolower((unsigned char)source[i]);
    }
    // Adds null to end string
    destination[i] = '\0';
}

// Defomes a function that checks whether a complete word is appearing in the string
int contains_word(const char *text, const char *word) {
    // Creates a lowercase copy of input
    char lower_text[MAX_INPUT];
    // Creates a lowercase copy of target word
    char lower_word[MAX_INPUT];
    char *token;

    // Converts the entire input to lowercase
    lowercase_copy(text, lower_text, sizeof(lower_text));
    //Converts the search word to lowercase
    lowercase_copy(word, lower_word, sizeof(lower_word));

    token = strtok(lower_text, " \t.,!?;:\"'()[]{}<>-/\\");
    while (token != NULL) {
        if (strcmp(token, lower_word) == 0) {
            return 1;
        }
        token = strtok(NULL, " \t.,!?;:\"'()[]{}<>-/\\");
    }
    return 0;
}

// Defines calculator commands
int try_calculator(const char *input, char *response, size_t response_size) {
    double left;
    double right;
    char op;
    char extra;
    int matched;

    matched = sscanf(input, "calc %lf %c %lf %c", &left, &op, &right, &extra);
    if (matched != 3) {
        return 0;
    }

    // Checks which mathematical operator was used
    switch (op) {
        case '+':
            snprintf(response, response_size, "Tool result: %.2f", left + right);
            break;
        case '-':
            snprintf(response, response_size, "Tool result: %.2f", left - right);
            break;
        case '*':
            snprintf(response, response_size, "Tool result: %.2f", left * right);
            break;
        case '/':
            if (right == 0.0) {
                snprintf(response, response_size, "Tool error: division by zero is not allowed.");
            } else {
                snprintf(response, response_size, "Tool result: %.2f", left / right);
            }
            break;
        default:
            snprintf(response, response_size, "Tool error: supported operators are +, -, *, and /.");
            break;
    }

    return 1;
}

// Defines how the model will respond to user input
void mock_model(const char *input, char *response, size_t response_size) {
    if (try_calculator(input, response, response_size)) {
        return;
    }

    if (contains_word(input, "hello")) {
        snprintf(response, response_size, "Hello! I am the mock model.");
    } else if (strcmp(input, "history") == 0) {
        snprintf(response, response_size, "History is shown by the harness after this response.");
    } else {
        snprintf(response, response_size, "Echo: %s", input);
    }
}

// Defines function that will add a conversation turn  to the history
void add_turn(Turn history[], int *count, const char *user, const char *assistant) {
    int i;

    if (*count < HISTORY_SIZE) {
        strncpy(history[*count].user, user, MAX_INPUT - 1);
        history[*count].user[MAX_INPUT - 1] = '\0';
        strncpy(history[*count].assistant, assistant, MAX_INPUT - 1);
        history[*count].assistant[MAX_INPUT - 1] = '\0';
        (*count)++;
        return;
    }

    for (i = 1; i < HISTORY_SIZE; i++) {
        history[i - 1] = history[i];
    }

    strncpy(history[HISTORY_SIZE - 1].user, user, MAX_INPUT - 1);
    history[HISTORY_SIZE - 1].user[MAX_INPUT - 1] = '\0';
    strncpy(history[HISTORY_SIZE - 1].assistant, assistant, MAX_INPUT - 1);
    history[HISTORY_SIZE - 1].assistant[MAX_INPUT - 1] = '\0';
}


// Defines function that will print all currerntly stored conversation turns
void print_history(const Turn history[], int count) {
    int i;

    printf("--- Conversation History (%d turn%s) ---\n", count, count == 1 ? "" : "s");
    for (i = 0; i < count; i++) {
        printf("Turn %d\n", i + 1);
        printf("  User: %s\n", history[i].user);
        printf("  Assistant: %s\n", history[i].assistant);
    }
    printf("---------------------------------------\n");
}

// Defines main function where program execution will begin
int main(void) {
    char input[MAX_INPUT];
    char response[MAX_INPUT];
    Turn history[HISTORY_SIZE];
    int history_count = 0;

    printf("ECE 309 Mini LLM Harness\n");
    printf("Type 'exit' to quit.\n");
    printf("Type 'history' to view the last 5 conversation turns.\n");
    printf("Calculator format: calc <number> <operator> <number>\n");

    // Starts an infinite loop to accept messages until quit
    while (1) {
        printf("You: ");

        // Reads one line of the input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nInput ended. Exiting safely.\n");
            break;
        }

        trim_newline(input);

        // Checks if the user entered exit and ends if they did
        if (strcmp(input, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        }

        // Sends the user's input to the model and prints the model's response
        mock_model(input, response, sizeof(response));
        printf("Model: %s\n", response);

        // Saves user input and model response to one conversation turn
        add_turn(history, &history_count, input, response);

        // Checks if the user requested the conversation histoyr
        if (strcmp(input, "history") == 0) {
            // Prints the currently stored conversation history
            print_history(history, history_count);
        }
    }

    return 0;
}
