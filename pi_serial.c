#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>



double givenFunction(double x) {
 return 4.0 / (1.0 + x * x);
}

double simpson(int n) {

	
	//lets calculate the leading coefficient in front of bracket (b - a) / 3n

	double interval = pow(10.0, (double) n); 	
	double leadingCo;
	
	double a = 0.0;
	double b = 1.0;
	double c = b - a;
	double d = 3.0 * interval;
	leadingCo = c / d;

	//below calculates inside the parenthesis succeeding the coefficient
	// [f(a) + 4f(x) + 2f(x) .... 4f(x) + f(b)] 
	
	double incrementValue = (b - a) / interval; 

	double sum = 0.0; //initializing sum var
	int i;
	for (i = 1; i <= interval; i+= 2) { //calculates each time the parenthesis calculates 4f(x)
		
		double x = a + i * incrementValue;
		sum += givenFunction(x) * 4.0; //calculates 4f(x) 		
	}
	
	for (i = 2; i <= interval - 1; i+= 2) {
		double y = a + i * incrementValue;
		sum += givenFunction(y) * 2.0; //calculates 2f(x)
	}

	//now we put the alternating 4f(x) and 2f(x) values stored in sum with the first and last f(a) and f(b) terms
	
	double firstTerm = givenFunction(a);
	double lastTerm = givenFunction(b);

	return leadingCo  * (firstTerm + sum + lastTerm); //leading coefficient * [f(a) + 4f(x) + 2f(x).....4f(x) + f(b)] 
	
}
int main(int argc, char *argv[]) {
//error handling 
if (argc != 2) {
	fprintf(stderr, "Incorrect Usage. Run as ./my_serial <number>\n");
	exit(EXIT_FAILURE);
}
int estimate = atoi(argv[1]);

if (estimate == 0 || estimate > 10) {
	fprintf(stderr, "The entered argument is not a valid number.\n");
	exit(EXIT_FAILURE);
}


double argAnswer = simpson(estimate);

printf("The estimated area using Simpson's method is: %.20lf\n", argAnswer);

double errorAnswer = M_PI - argAnswer;
double absoluteError = fabs(errorAnswer);
printf("The error is: %.20lf\n", absoluteError);


//headers for tabulate functionality
 printf("\n%-20s %-20s %-20s %-20s\n", "estimate", "answer", "pi", "error");

//loop for tabulating results

int i;
    for (i = 0; i < 9; i++) {
        int estimateTable = i + 1; // Calculating the estimate value based on the loop index

        double answer = simpson(estimateTable); // Estimate using Simpson's method
        double pi = M_PI; // Exact value of pi
        double error = pi - answer; // Error between Simpson's estimate and actual value of pi
        double absoluteError = fabs(error);

printf("%-20d %-20.15lf %-20.15lf %-20.17lf\n", estimateTable, answer, pi, absoluteError);
	   
 }




return 0;

}













































