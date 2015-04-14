#include <iostream>
#include <cmath>
#include <functional>
#include <exception>
#include <vector>

using namespace std;


//----Methods' IDs----------
#define GAUSS 1
#define JACOBI 2


//----Stopping Criteria----------
const double EPSILON = 0.0001;
const int MAX_ITERATIONS = 10000;


//----Exceptions Classes----------
class incompatibleMethodException : public exception {
public:
	virtual const char* what() const throw() {
		return "The selected method cannot solve the selected equations or very slow.";
	}
};

class divideByZeroException : public exception {
public:
	virtual const char* what() const throw() {
		return "Cannot divide by zero";
	}
};

//----Matrix Class----------
class Matrix4x4 {
public:
	Matrix4x4(double a00 = 0, double a01 = 0, double a02 = 0,
		double a10 = 0, double a11 = 0, double a12 = 0,
		double a20 = 0, double a21 = 0, double a22 = 0) {

	}
};



//----Solution Struct----------
struct Solution {
	double x0;
	double x1;
	double x2;
	double error;
	int iterations;
};

//----Equation Function Object----------
typedef function<double(double)> Equation;


//----Helper Functions----------
void initEquations();
void displayMethodsMenu();
void displayExitMenu();
int getSelection(int min, int max);


//----Solution Computation Functions----------
double getMaxError(double a0, double a1, double a2);
Solution findRootByGauss(Matrix4x4 coeff);
Solution findRootByJacobi(Equation e0, Equation e1, Equation e2);


//----Equations' Definitions----------
Equation equations[4]; //Roots Equations. 
Matrix4x4 coefficients; //Equation Coefficients.


int main(int argc, char **argv) {
	initEquations(); //Initialize Equations' Definitions.
	while (1){
		system("cls");
		displayMethodsMenu(); //Method Selection Menu.
		int selectedMethod = getSelection(1, 3); //Get Selected Method.
		if (selectedMethod == 3)
			exit(0);
		else {
			try{
				Solution solution;
				switch (selectedMethod){
				case GAUSS:
					cout << "Solving using Gauss-Jordan method: " << endl;
					solution = findRootByGauss(coefficients); //Compute the root using Gauss-Jordan Method.
					break;
				case JACOBI:
					cout << "Solving using the Jacobi Method: " << endl;
					solution = findRootByJacobi(equations[0], equations[1], equations[2]); //Compute the root using the Jacobi Method.
					break;
				
				}
				system("cls");
				cout << "Solutions of the equations using the selected method is: " << endl //Print the result.
					<< "x0 = " << solution.x0 << endl
					<< "x1 = " << solution.x1 << endl
					<< "x2 = " << solution.x2 << endl
					<< "Error: ~" << solution.error * 100.0 << " %" << endl
					<< "Number of iterations: " << solution.iterations << endl;

			} catch (divideByZeroException &e){
				cout << e.what() << endl;
			} catch (incompatibleMethodException &e){
				cout << e.what() << endl;
			}

			system("pause");
			system("cls");

			displayExitMenu(); //Next Action Menu.
			int exitOption = getSelection(1, 2); //Get Selected Action.
			if (exitOption == 2)
				exit(0);
		} 
	}

	return 0;
}

void initEquations() {
	equations[1] = [](double x) {
		return 0.0; //x0 Equation.
	};
	equations[2] = [](double x) {
		return 0.0; //x1 Equation.
	};
	equations[3] = [](double x) {
		return 0.0; //x2 Equation.
	};

	coefficients = Matrix4x4(); //Coefficients Matrix.

}

void displayMethodsMenu() { //Printing Method Selection Menu.
	cout << "Equations: " << endl
		<< "10x1 + 2x2 -  x3 = 27" << endl
		<< "-3x1 - 6x2 + 2x3 = -61.5" << endl
		<< "  x1 +  x2 + 5x3 = -21.5 " << endl
		<<"Select the method you want to solve with:" << endl
		<< "1) Gauss-Jordan Method." << endl
		<< "2) The Jacobi Method." << endl
		<< "3) Quit." << endl;
}

void displayExitMenu() { //Printing Exit Menu.
	cout << "Select one of the following actions:" << endl
		<< "1) Solve with another method." << endl
		<< "2) Quit." << endl;
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

double getMaxError(double a0, double a1, double a2) { //Getting the maximum error.
	if (a0 > a1){
		if (a0 > a2)
			return a0;
		else
			return a2;
	} else if (a2 > a1){
		return a2;
	} else{
		return a1;
	}
}

Solution findRootByGauss(Matrix4x4 coeff) { //Computing the solution using Gauss-Jordan Elimination Method.
	Solution solution;
	solution.x0 = 0;
	solution.x1 = 0;
	solution.x2 = 0;
	solution.error = 0;
	solution.iterations = 0;
	return solution;
}

Solution findRootByJacobi(Equation e0, Equation e1, Equation e2) { //Computing the solution using the Jacobi Method.
	Solution solution;
	solution.x0 = 0;
	solution.x1 = 0;
	solution.x2 = 0;
	solution.error = 0;
	solution.iterations = 0;
	return solution;
}