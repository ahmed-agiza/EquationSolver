#include <iostream>
#include <cmath>
#include <functional>
#include <exception>

using namespace std;


//----Methods' IDs----------
#define BISECTION 1
#define SECANT 2
#define FALSEP 3
#define NEWTON 4

//----Stopping Criteria----------
const double EPSILON = 0.0000001;
const int MAX_ITERATIONS = 10000;


//----Exceptions Classes----------
class incompatibleMethodException : public exception {
public:
	virtual const char* what() const throw() {
		return "The selected method cannot solve the selected equation or very slow.";
	}
};

class divideByZeroException : public exception {
public:
	virtual const char* what() const throw() {
		return "Cannot divide by zero";
	}
};

class logZeroException : public exception {
public:
	virtual const char* what() const throw() {
		return "Cannot compute ln(0).";
	}
};


//----Solution Struct----------
struct Solution {
	double root;
	double error;
	int iterations;
};

//----Formula Function Object----------
typedef function<double(double)> Formula;


//----Helper Functions----------
void initFormulae();
void displayEquationsMenu();
void displayMethodsMenu();
void displayExitMenu();
int getSelection(int min, int max);


//----Root Computation Functions----------
bool rootExists(Formula fx, double xl, double xh);
Solution findRootByBisection(Formula fx);
Solution findRootBySecant(Formula fx);
Solution findRootByFalsePosition(Formula fx);
Solution findRootByNewton(Formula fx, Formula dfx);


//----Equations' Evaluators----------
Formula formulae[6], //Original Equations. 
		dformulae[6]; //Derivatives Equations.


int main(int argc, char **argv) {
	initFormulae(); //Initialize Equations' Evaluators.
	while (1){
		system("cls");
		displayEquationsMenu(); //Equation Selection Menu.
		int selectedEquation = getSelection(1, 6); //Get Selected Equation.
		if (selectedEquation == 6)
			exit(0);
		
		while (1){
			system("cls");
			displayMethodsMenu(); //Method Selection Menu.
			int selectedMethod = getSelection(1, 6); //Get Selected Method.
			if (selectedMethod == 6)
				exit(0);
			else if (selectedMethod != 5){
				try{
					Solution solution;
					switch (selectedMethod){
					case BISECTION:
						cout << "Solving using Bisection method: " << endl;
						solution = findRootByBisection(formulae[selectedEquation]); //Compute the root using Bisection Method.
						break;
					case SECANT:
						cout << "Solving using Secant Method: " << endl;
						solution = findRootBySecant(formulae[selectedEquation]); //Compute the root using Secant Method.
						break;
					case FALSEP:
						cout << "Solving using False-Position Method: " << endl;
						solution = findRootByFalsePosition(formulae[selectedEquation]); //Compute the root using False-Position Method.
						break;
					case NEWTON:
						cout << "Solving using Newton-Raphson Method: " << endl;
						solution = findRootByNewton(formulae[selectedEquation], dformulae[selectedEquation]); //Compute the root using Newton-Raphson Method.
					}
					system("cls");
					cout << "Solution of the selected equation using the selected method is: " << endl //Print the result.
						<< "x = " << solution.root << endl
						<< "Error: ~" << solution.error * 100.0 << " %" << endl
						<< "Number of iterations: " << solution.iterations << endl;

				} catch (divideByZeroException &e){
					cout << e.what() << endl;
				} catch (incompatibleMethodException &e){
					cout << e.what() << endl;
				} catch (logZeroException &e){
					cout << e.what() << endl;
				}

				system("pause");
				system("cls");

				displayExitMenu(); //Next Action Menu.
				int exitOption = getSelection(1, 3); //Get Selected Action.
				if (exitOption == 3)
					exit(0);
				else if (exitOption == 2)
					break;
			} else
				break;
		}
	}
	return 0;
}
int jj = 0;
void initFormulae() {
	formulae[1] = [](double x) {
		return (pow(x, 3) - 8 * pow(x, 2) + 12 * x - 4); //First Equation.
	};

	formulae[2] = [](double x) {
		return (-12 - 21 * x + 18 * pow(x, 2) - 2.75 * pow(x, 3)); //Second Equation.
	};

	formulae[3] = [](double x) {
		return (6 * x - 4 * pow(x, 2) + 0.5 * pow(x, 3) - 2); //Third Equation.
	};

	formulae[4] = [](double x) {
		if (pow(x, 4) == 0)
			throw logZeroException();
		return log(pow(x, 4)) - 0.7; //Fourth Equation.
	};

	formulae[5] = [](double x) {
		return (7 * sin(x) - exp(x)); //Fifth Equation.
	};

	dformulae[1] = [](double x) {
		return (3 * pow(x, 3) - 16 * x + 12); //First Equation Derivative.
	};

	dformulae[2] = [](double x) {
		return (-21 + 36 * x - 8.25 * pow(x, 2)); //Second Equation Derivative.
	};

	dformulae[3] = [](double x) {
		return (6 - 8 * x + 1.5 * pow(x, 2));  //Third Equation Derivative.
	};

	dformulae[4] = [](double x) {
		if (x == 0)
			throw divideByZeroException();
		return (3 / x);  //Fourth Equation Derivative.
	};

	dformulae[5] = [](double x) {
		return (7 * cos(x) - exp(x));  //Fifth Equation Derivative.
	};
}

void displayEquationsMenu() { //Printing Equation Selection Menu.
	cout << "Select the equation you want to solve:" << endl
		<< "1) x^3 - 8x^2 + 12x = 4" << endl
		<< "2) f(x) = -12 - 21x + 18x^2 - 2.75x^3" << endl
		<< "3) f(x) = 6x - 4x^2 + 0.5 x^3 - 2" << endl
		<< "4) ln(x^4) =  0.7" << endl
		<< "5) 7sin(x) = e^x" << endl
		<< "6) Quit." << endl;
}

void displayMethodsMenu() { //Printing Method Selection Menu.
	cout << "Select the method you want to solve with:" << endl
		<< "1) Bisection Method." << endl
		<< "2) Secant Method." << endl
		<< "3) False-Position Method." << endl
		<< "4) Newton-Raphson Method." << endl
		<< "5) Previous Menu." << endl
		<< "6) Quit." << endl;
}

void displayExitMenu() { //Printing Exit Menu.
	cout << "Select one of the following actions:" << endl
		<< "1) Solve with another method." << endl
		<< "2) Solve another equation." << endl
		<< "3) Quit." << endl;
}

int getSelection(int min, int max) { //Geting User Selected Option.
	char selection = ' ';
	bool selected = false;

	while (!selected){
		cout << "Input: ";
		cin >> selection;
		if (selection >= (min + 48) && selection <= (max + 48))
			selected = true;
		else
			cout << "Invalid option. " << endl << selection << endl;
	}

	char buf[2];
	buf[0] = selection;
	buf[1] = 0;

	return atoi(buf);
}

bool rootExists(Formula fx, double xl, double xh) { //Checking the Existence of root in interval for Closed Methods.
	return (fx(xl) * fx(xh) <= 0);
}

Solution findRootByBisection(Formula fx) { //Computing the root using Bisection Method. 
	double xl, xh, oldRoot = 0, newRoot = 1, error = DBL_MAX;
	Solution solution;
	solution.error = 0;
	solution.iterations = 0;

	do{ //Getting initial guesses.
		cout << "Enter the lower bound guess: ";
		cin >> xl;
		cout << "Enter the higher bound guess: ";
		cin >> xh;
		if (!rootExists(fx, xl, xh))
			cout << "No root is found between the specified boundaries or the boundaries enclose two roots." << endl;
		else
			oldRoot = (xl + xh) / 2.0;
	} while(!rootExists(fx, xl, xh)); //Validating Existence of root between the guesses.

	int iterations = 0; //Number of iterations.
	while (error > EPSILON){ //Iterations loop.
		if (iterations > MAX_ITERATIONS)
			throw incompatibleMethodException();
		if (rootExists(fx, xl, oldRoot)){
			newRoot = (xl + oldRoot) / 2.0;
		} else if (rootExists(fx, oldRoot, xh) ){ //Checking the new sub-interval.
			newRoot = (oldRoot + xh) / 2.0;
		} else{
			cout << "Unexpected error." << endl;
			exit(1);
		}

		if (rootExists(fx, xl, newRoot))
			xh = newRoot;
		else if (rootExists(fx, newRoot, xh))
			xl = newRoot;
		else{
			cout << "Unexpected error." << endl;
			exit(1);
		}

		error = ((oldRoot - newRoot) / newRoot); //Error computation.

		if (error < 0)
			error *= -1;

		oldRoot = newRoot;
		iterations++;
	}
	solution.root = newRoot;
	if (fx(newRoot) != 0)
		solution.error = error;
	else
		solution.error = 0;
	solution.iterations = iterations;
	return solution;
}

Solution findRootBySecant(Formula fx) { //Computing the root using Secant Method.
	double oldRoot0, oldRoot1, newRoot = 1, error = DBL_MAX;
	Solution solution;
	solution.error = 0;
	solution.iterations = 0;

	auto nextRoot = [](Formula fxn, double x0, double x1) { //Next root computation function.
		double numerator = (x1 - x0);
		double denumerator = fxn(x1) - fxn(x0);
		
		if (denumerator <= numeric_limits<double>::epsilon() && denumerator > 0)
			return x1;
		else if (denumerator == 0)
			throw divideByZeroException();

		return x1 - (fxn(x1) * (numerator / denumerator));
	};


	
	//Getting Initial Guesses.
	cout << "Enter the lower bound guess: ";
	cin >> oldRoot0;
	cout << "Enter the higher bound guess: ";
	cin >> oldRoot1;
	
	oldRoot1 = nextRoot(fx, oldRoot0, oldRoot1); //Initial computation.
	int iterations = 0;
	while (error > EPSILON){ //Iterations loop.
		if (iterations > MAX_ITERATIONS)
			throw incompatibleMethodException();
		newRoot = nextRoot(fx, oldRoot0, oldRoot1); //Computing next root.

		error = ((oldRoot1 - newRoot) / newRoot); //Error computation.
		if (error < 0)
			error *= -1;

		oldRoot0 = oldRoot1;
		oldRoot1 = newRoot;
		iterations++;
	}
	solution.root = newRoot;
	if (fx(newRoot) != 0)
		solution.error = error;
	else
		solution.error = 0;
	solution.iterations = iterations;
	return solution;
}

Solution findRootByFalsePosition(Formula fx) { //Computing the root using False-Position Method.
	double xl, xh, oldRoot, newRoot = 1, error = DBL_MAX;
	Solution solution;
	solution.error = 0;
	solution.iterations = 0;

	auto nextRoot = [](Formula fxn, double x0, double x1) { //Next root computation function.
		double numerator = (x1 - x0);
		double denumerator = fxn(x1) - fxn(x0);

		if (denumerator <= numeric_limits<double>::epsilon() && denumerator > 0)
			return x1;
		else if (denumerator == 0)
			throw divideByZeroException();

		return x1 - (fxn(x1) * (numerator / denumerator));
	};

	do{ //Getting inital guesses.
		cout << "Enter the lower bound guess: ";
		cin >> xl;
		cout << "Enter the higher bound guess: ";
		cin >> xh;
		if (!rootExists(fx, xl, xh))
			cout << "No root is found between the specified boundaries or the boundaries enclose two roots." << endl;
		else
			oldRoot = nextRoot(fx, xl, xh);
	} while (!rootExists(fx, xl, xh));

	int iterations = 0;
	while (error > EPSILON){ //Iterations loop.
		if (iterations > MAX_ITERATIONS)
			throw incompatibleMethodException();
		if (rootExists(fx, xl, oldRoot)){ //Checking the new sub-interval.
			newRoot = nextRoot(fx, xl, oldRoot);
		} else if (rootExists(fx, oldRoot, xh)){
			newRoot = nextRoot(fx, oldRoot, xh);
		} else{
			cout << "Unexpected error." << endl;
			exit(1);
		}

		if (rootExists(fx, xl, newRoot))
			xh = newRoot;
		else if (rootExists(fx, newRoot, xh))
			xl = newRoot;
		else{
			cout << "Unexpected error." << endl;
			exit(1);
		}

		error = ((oldRoot - newRoot) / newRoot); //Error computation.

		if (error < 0)
			error *= -1;

		oldRoot = newRoot;
		iterations++;
	}
	solution.root = newRoot;
	if(fx(newRoot) != 0)
		solution.error = error;
	else
		solution.error = 0;
	solution.iterations = iterations;
	return solution;
}

Solution findRootByNewton(Formula fx, Formula dfx) {//Computing the root using Newton-Raphson Method.
	double x0, newRoot = 1, error = DBL_MAX;
	Solution solution;
	solution.error = 0;
	solution.iterations = 0;

	auto nextRoot = [](Formula fxn, Formula dfxn, double x0) { //Next root computation function.
		double numerator = fxn(x0);
		double denumerator = dfxn(x0);

		if (denumerator <= numeric_limits<double>::epsilon() && denumerator > 0)
			return x0;
		else if (denumerator == 0)
			throw divideByZeroException();

		return x0 - (numerator / denumerator);
	};

	//Getting the initial guess.
	cout << "Enter the initial guess: ";
	cin >> x0;
	x0 = nextRoot(fx, dfx, x0);
	

	int iterations = 0;
	while (error > EPSILON){ //Itertaions loop.
		if (iterations > MAX_ITERATIONS)
			throw incompatibleMethodException();
		newRoot = nextRoot(fx, dfx, x0); //Computing next root.

		error = ((newRoot - x0) / newRoot); //Error computation.

		if (error < 0)
			error *= -1;

		x0 = newRoot;
		iterations++;
	}
	solution.root = newRoot;
	if (fx(newRoot) != 0)
		solution.error = error;
	else
		solution.error = 0;
	solution.iterations = iterations;
	return solution;
}