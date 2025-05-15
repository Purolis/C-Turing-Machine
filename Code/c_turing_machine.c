// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>


// Macros
#define STR_SIZE 100
#define TAPE_SIZE 100
#define TRANSITION_SIZE 5


// Turing Machine Structure
struct TuringMachine {
	char * state;
	char head;
	char * tape;
	char symbol1;
	char symbol2;
	int headCounter;
	char * finalState;
};


// Regular Helper Declarations
char * userLanguageInput();
struct TuringMachine * createTuringMachine(char *, char *);
char * getUserInput(char *);
char * getLanguage();
int checkLanguageLength();
void parseTransitions(struct TuringMachine *, char *);


// Language Acceptor Declarations
void runLanguageAcceptor();
char * transitionLanguageFunctions(struct TuringMachine *);
void languageState0(char *, struct TuringMachine *);
void languageState1(char *, struct TuringMachine *);
void languageState2(char *, struct TuringMachine *);
void languageState3(char *, struct TuringMachine *);


// Digit Doubler Declarations
void runDigitDoubler();
char * transitionDoublerFunctions(struct TuringMachine *);
void doublerState0(char *, struct TuringMachine *);
void doublerState1(char *, struct TuringMachine *);
void doublerState2(char *, struct TuringMachine *);


// Main
int main() {	
	runLanguageAcceptor();
	sleep(3);
	runDigitDoubler();
}


// Function that holds all of the language acceptor logic.
void runLanguageAcceptor() {


	// Grabs the tape and language given
	printf("\n***************RUNNING LANGUAGE ACCEPTOR*************\n");
	char * language = getLanguage();
	char * tapeInput = getUserInput("\nPlease enter a tape that you would like to test out: ");
	char * tape = malloc(TAPE_SIZE * sizeof(char));
	

	// Creates the tape into a tape with "null" spaces on each side
	sprintf(tape, "%s%s%s", "_", tapeInput, "_");


	// Creates a turing machine from the language and the tape
	struct TuringMachine * tm = createTuringMachine(language, tape);
	
	
	// While the state isn't in the final state, go through the turing machine
	char * transition = transitionLanguageFunctions(tm);
	parseTransitions(tm, transition);
	while (tm->state[1] != tm->finalState[1] && transition != "INVALID") {
		transition = transitionLanguageFunctions(tm);
		parseTransitions(tm, transition);
	}


	// If transition comes back as invalid, print.
	if (transition == "INVALID") {
		printf("\n***************INVALID TAPE DETECTED***************\n");
	} 
	
	
	// Else print valid tape since every other one should be valid.
	else {
		printf("\n****************VALID TAPE DETECTED*****************\n");
	}


	// Free everything that we don't use anymore and we malloced for
	free(language);
	free(tapeInput);
	free(tape);
	free(tm);
	free(transition);

}


// Main function that controls the transitions
char * transitionLanguageFunctions(struct TuringMachine * tm) {
	
	
	// Prints out all of the useful information about the current tape
	printf("\n**********************PRINTING**********************\n");
	printf("Current State: %s\n", tm->state);
	printf("Current Head: %c\n", tm->head);
	printf("Current Tape: %s\n", tm->tape);
	printf("Current Head Counter: %d\n", tm->headCounter);
	sleep(1);

	
	// Creates a temp variable for transition and goes through the tape once
	char * transition = malloc(TRANSITION_SIZE * sizeof(char));
	if (tm->state[1] == '0') {
		languageState0(transition, tm);
	}
	else if (tm->state[1] == '1') {
		languageState1(transition, tm);
	}

	else if (tm->state[1] == '2') {
		languageState2(transition, tm);
	}

	else if (tm->state[1] == '3') {
		languageState3(transition, tm);	
	}
	else {
		transition = "INVALID";
	}

	
	// Return the transition at the end.
	return transition;
}


// Logic for the language state 0 transitions
void languageState0(char * transition, struct TuringMachine * tm) {
	if (tm->head == tm->symbol1) {
		sprintf(transition, "%s%c%c", "q1", '0', 'R');
	}
	else if (tm->head == '1') {			
		sprintf(transition, "%s%c%c", "q3", '1', 'R');
	}
}


// Logic for the language state 1 transitions
void languageState1(char * transition, struct TuringMachine * tm) {
	if (tm->head == tm->symbol1) {
		sprintf(transition, "%s%c%c", "q1", tm->symbol1, 'R');
	}
	else if (tm->head == '1') {
		sprintf(transition, "%s%c%c", "q1", '1', 'R');
	}
	else if (tm->head == tm->symbol2) {
		sprintf(transition, "%s%c%c", "q2", '1', 'L');
	}
}


// Logic for the language state 2 transitions
void languageState2(char * transition, struct TuringMachine * tm) {
	if (tm->head == '1') {
		sprintf(transition, "%s%c%c", "q2", '1', 'L');	
	}
	else if (tm->head == tm->symbol1) {
		sprintf(transition, "%s%c%c", "q2", tm->symbol1, 'L');
	}
	else if (tm->head == '0') {
		sprintf(transition, "%s%c%c", "q0", '0', 'R');
	}
}


// Logic for the language state 3 transitions
void languageState3(char * transition, struct TuringMachine* tm) {
	if (tm->head == '1') {
		sprintf(transition, "%s%c%c", "q3", '1', 'R');
	}
	else if (tm->head == '_') {
		sprintf(transition, "%s%c%c", "q4", '_', 'R');
	}
}


// Function that runs the digit doubler
void runDigitDoubler() {

	
	// Grabs the digit input from the user and creates the tape with null spaces
	printf("\n******************RUNNING DIGIT DOUBLER****************\n");
	char * digitInput = getUserInput("\nPlease enter a digit that is compiled of 1 digit that you would like to double (example: 11 or 3333): ");
	char * tape = malloc(TAPE_SIZE * sizeof(char));
	sprintf(tape, "%s%s%s", "_______________", digitInput, "_______________");
	
	
	// Creates the turing machine with the digit input and tape, manually sets final state and headCounter to line up with null spaces
	struct TuringMachine * tm = createTuringMachine(digitInput, tape);
	tm->finalState = "q3";
	tm->headCounter = 15;
	
	
	// While the state isn't in the final state, go through the tape
	char * transition = transitionDoublerFunctions(tm);
	parseTransitions(tm, transition);
	while (tm->state[1] != tm->finalState[1] && transition != "INVALID") {
		transition = transitionDoublerFunctions(tm);
		parseTransitions(tm, transition);
	}

	// If transition is invalid, print
	if (transition == "INVALID") {
		printf("\n***************INVALID TAPE DETECTED***************\n");
	}
       
	
	// Else print valid tape
	else {
		printf("\n****************VALID TAPE DETECTED*****************\n");
	}


	// Free all up the variables that use malloc since we don't use them anymore
	free(digitInput);
	free(tape);
	free(tm);
	free(transition);
}


// Function that does all of the transition logic
char * transitionDoublerFunctions(struct TuringMachine * tm) {
	
	
	// Prints out the important tape information
	printf("\n**********************PRINTING**********************\n");
	printf("Current State: %s\n", tm->state);
	printf("Current Head: %c\n", tm->head);
	printf("Current Tape: %s\n", tm->tape);
	printf("Current Head Counter: %d\n", tm->headCounter);
	sleep(1);
	

	// Holds all of the transition logic and saves it into a temp variable transition
	char * transition = malloc(TRANSITION_SIZE * sizeof(char));
	if (tm->state[1] == '0') {
		doublerState0(transition, tm);
	}
	else if (tm->state[1] == '1') {
		doublerState1(transition, tm);
	}

	else if (tm->state[1] == '2') {
		doublerState2(transition, tm);
	}

	else {
		transition = "INVALID";
	}


	// Returns transition
	return transition;
}


// Transition logic for doubler state 0
void doublerState0(char * transition, struct TuringMachine * tm) {
	if (tm->head == tm->symbol1) {
		sprintf(transition, "%s%c%c", "q0", 'x', 'R');
	}
	else if (tm->head == '_') {			
		sprintf(transition, "%s%c%c", "q1", '_', 'L');
	}
}


// Transition logic for doubler state 1
void doublerState1(char * transition, struct TuringMachine * tm) {
	if (tm->head == tm->symbol1) {
		sprintf(transition, "%s%c%c", "q1", tm->symbol1, 'L');
	}
	else if (tm->head == 'x') {			
		sprintf(transition, "%s%c%c", "q2", tm->symbol1, 'R');
	}
	else if (tm->head == '_') {
		sprintf(transition, "%s%c%c", "q3", '_', 'R');
	}
}


// Transition logic for doubler state 2
void doublerState2(char * transition, struct TuringMachine * tm) {
	if (tm->head == tm->symbol1) {
		sprintf(transition, "%s%c%c", "q2", tm->symbol1, 'R');
	}
	else if (tm->head == '_') {			
		sprintf(transition, "%s%c%c", "q1", tm->symbol1, 'L');
	}
}


// Parses the transition and saves into the structure
void parseTransitions(struct TuringMachine * tm, char * transition) {
	char * state = malloc(3 * sizeof(char));
	sprintf(state, "%c%c", transition[0], transition[1]);
	tm->state = state;
	tm->tape[tm->headCounter] = transition[2];
	if (transition[3] == 'R') {
		tm->headCounter++;
	}
	else {
		tm->headCounter--;
	}
	tm->head = tm->tape[tm->headCounter];
}


// Grabs the language user input and return
char * userLanguageInput() {
	char * languageui = malloc(STR_SIZE * sizeof(char));
	languageui[STR_SIZE - 1] = '\0';
	printf("\nPlease enter a language consisting of two or one variables, Language(x^my^m, m>=1: ");
	scanf("%s", languageui);
	return languageui;
}


// Grabs the language and checks the length
int checkLanguageLength(char * language) {
	int counter = 0;
	while (language[counter] != '\0') {
		counter++;
	}
	return counter;
}


// Grabs the user input and while language input is not 2 repeat the user input
char * getLanguage() {
	char * language = userLanguageInput();
	int len = checkLanguageLength(language);
	while (len != 2) {
		printf("\nPlease enter a language consisting of two or one variables: ");
		language = userLanguageInput();
		len = checkLanguageLength(language);
	}
	return language;
}


// Creates the turing machine base 
struct TuringMachine * createTuringMachine(char * language, char * tape) {
	struct TuringMachine * tm = malloc(sizeof(struct TuringMachine));
	tm->symbol1 = language[0];
	tm->symbol2 = language[1];
	tm->state = "q0";
	tm->tape = tape;
	tm->head = language[0];
	tm->headCounter = 1;
	tm->finalState = "q4";
	return tm;
}


// Grabs user input and returns
char * getUserInput(char * prompt) {
	char * ui = malloc(STR_SIZE * sizeof(char));
	ui[STR_SIZE - 1] = '\0';
	printf("%s", prompt);
	scanf("%s", ui);
	return ui;
}
