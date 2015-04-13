#include <iostream>
#include <cmath>
#include <functional>
#include <exception>

using namespace std;

#define BISECTION 1
#define SECANT 2
#define FALSEP 3
#define NEWTON 4

const double EPSILON = 0.0000001;
const int MAX_ITERATIONS = 100;

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

struct Solution {
	double root;
	double error;
	int iterations;
};

typedef function<double(double)> Formula;

void initFormulae();
void displayEquationsMenu();
void displayMethodsMenu();
void displayExitMenu();
int getSelection(int min, int max);

bool rootExists(Formula fx, double xl, double xh);
Solution findRootByBisection(Formula fx);
Solution findRootBySecant(Formula fx);
Solution findRootByFalsePosition(Formula fx);
Solution findRootByNewton(Formula fx, Formula dfx);

Formula formulae[6], dformulae[6], f1, f2, f3, f4, f5, df1, df2, df3, df4, df5;

void testRealRoots() {
	cout << "1) " << f1(0.474572) << "  " << f1(1.3691) << endl
		<< "2) " << f2(-0.41469) << "  " << f2(2.2198) << "  " << f2(4.7403) << endl
		<< "3) " << f3(0.474572) << "  " << f3(1.3691) << "  " << f3(6.1563) << endl
		<< "4) " << f4(-1.1912) << "  " << f4(1.1912) << endl
		<< "5) " << f5(-25.1327412287166) << "   " << f5(-21.9911485751688) << "   " << f5(-18.8495559206084) << endl
		<< f5(-15.707693894778) << "  " << f5(-12.5663701161672) << "  " << f5(-9.42478948913899) << endl
		<< f5(-6.28291845844457) << " " << f5(-3.14772834675799) << "  " << f5(0.17017999375836) << "  " << f5(1.89305902941532) << endl;
}

int main(int argc, char **argv) {
	initFormulae();
	while (1){
		system("cls");
		displayEquationsMenu();
		int selectedEquation = getSelection(1, 6);
		if (selectedEquation == 6)
			exit(0);
		
		while (1){
			system("cls");
			displayMethodsMenu();
			int selectedMethod = getSelection(1, 6);
			if (selectedMethod == 6)
				exit(0);
			else if (selectedMethod != 5){
				try{
					Solution solution;
					switch (selectedMethod){
					case BISECTION:
						cout << "Solving using Bisection method: " << endl;
						solution = findRootByBisection(formulae[selectedEquation]);
						break;
					case SECANT:
						cout << "Solving using Secant Method: " << endl;
						solution = findRootBySecant(formulae[selectedEquation]);
						break;
					case FALSEP:
						cout << "Solving using False-Position Method: " << endl;
						solution = findRootByFalsePosition(formulae[selectedEquation]);
						break;
					case NEWTON:
						cout << "Solving using Newton-Raphson Method: " << endl;
						solution = findRootByNewton(formulae[selectedEquation], dformulae[selectedEquation]);
					}
					system("cls");
					cout << "Solution of the selected equation using the selected method is: " << endl
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

				displayExitMenu();
				int exitOption = getSelection(1, 3);
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

void initFormulae() {
	formulae[1] = f1 = [](double x) {
		return (pow(x, 3) - 8 * pow(x, 2) + 12 * x - 4);
	};

	formulae[2] = f2 = [](double x) {
		return (-12 - 21 * x + 18 * pow(x, 2) - 2.75 * pow(x, 3));
	};

	formulae[3] = f3 = [](double x) {
		return (6 * x - 4 * pow(x, 2) + 0.5 * pow(x, 3) - 2);
	};

	formulae[4] = f4 = [](double x) {
		if (pow(x, 4) == 0)
			throw logZeroException();
		return log(pow(x, 4)) - 0.7;
	};

	formulae[5] = f5 = [](double x) {
		return (7 * sin(x) - exp(x));
	};

	dformulae[1] = df1 = [](double x) {
		return (3 * pow(x, 3) - 16 * x + 12);
	};

	dformulae[2] = df2 = [](double x) {
		return (-21 + 36 * x - 8.25 * pow(x, 2));
	};

	dformulae[3] = df3 = [](double x) {
		return (6 - 8 * x + 1.5 * pow(x, 2));
	};

	dformulae[4] = df4 = [](double x) {
		if (x == 0)
			throw divideByZeroException();
		return (3 / x);
	};

	dformulae[5] = df5 = [](double x) {
		return (7 * cos(x) - exp(x));
	};
}

void displayEquationsMenu() {
	cout << "Select the equation you want to solve:" << endl
		<< "1) x^3 - 8x^2 + 12x = 4" << endl
		<< "2) f(x) = -12 - 21x + 18x^2 - 2.75x^3" << endl
		<< "3) f(x) = 6x - 4x^2 + 0.5 x^3 - 2" << endl
		<< "4) ln(x^4) =  0.7" << endl
		<< "5) 7sin(x) = e^x" << endl
		<< "6) Quit." << endl;
}

void displayMethodsMenu() {
	cout << "Select the method you want to solve with:" << endl
		<< "1) Bisection Method." << endl
		<< "2) Secant Method." << endl
		<< "3) False-Position Method." << endl
		<< "4) Newton-Raphson Method." << endl
		<< "5) Previous Menu." << endl
		<< "6) Quit." << endl;
}

void displayExitMenu() {
	cout << "Select one of the following actions:" << endl
		<< "1) Solve with another method." << endl
		<< "2) Solve another equation." << endl
		<< "3) Quit." << endl;
}

int getSelection(int min, int max) {
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

bool rootExists(Formula fx, double xl, double xh) {
	return (fx(xl) * fx(xh) <= 0);
}

Solution findRootByBisection(Formula fx) {
	double xl, xh, oldRoot = 0, newRoot = 0, error = DBL_MAX;
	Solution solution;
	solution.error = 0;
	solution.iterations = 0;

	do{
		cout << "Enter the lower bound guess: ";
		cin >> xl;
		cout << "Enter the higher bound guess: ";
		cin >> xh;
		if (!rootExists(fx, xl, xh))
			cout << "No root is found between the specified boundaries or the boundaries enclose two roots." << endl;
		else
			oldRoot = (xl + xh) / 2.0;
	} while(!rootExists(fx, xl, xh));

	int iterations = 0;
	while (error > EPSILON && fx(newRoot) != 0){
		if (iterations > MAX_ITERATIONS)
			throw incompatibleMethodException();
		if (rootExists(fx, xl, oldRoot)){
			newRoot = (xl + oldRoot) / 2.0;
		} else if (rootExists(fx, oldRoot, xh) ){
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

		error = ((oldRoot - newRoot) / newRoot);

		if (error < 0)
			error *= -1;

		oldRoot = newRoot;
		iterations++;
	}
	solution.root = newRoot;
	if (fx(newRoot) != 0)
		solution.error = error;
	solution.iterations = iterations;
	return solution;
}

Solution findRootBySecant(Formula fx) {
	double oldRoot0, oldRoot1, newRoot = 0, error = DBL_MAX;
	Solution solution;
	solution.error = 0;
	solution.iterations = 0;

	auto nextRoot = [](Formula fxn, double x0, double x1) {
		double numerator = (x1 - x0);
		double denumerator = fxn(x1) - fxn(x0);
		
		if (denumerator <= numeric_limits<double>::epsilon() && denumerator > 0)
			return x1;
		else if (denumerator == 0)
			throw divideByZeroException();
		return x1 - (fxn(x1) * (numerator / denumerator));
	};


	
	cout << "Enter the lower bound guess: ";
	cin >> oldRoot0;
	cout << "Enter the higher bound guess: ";
	cin >> oldRoot1;
	
	oldRoot1 = nextRoot(fx, oldRoot0, oldRoot1);
	int iterations = 0;
	while (error > EPSILON && fx(newRoot) != 0){
		if (iterations > MAX_ITERATIONS)
			throw incompatibleMethodException();
		newRoot = nextRoot(fx, oldRoot0, oldRoot1);

		error = ((oldRoot1 - newRoot) / newRoot);
		if (error < 0)
			error *= -1;

		oldRoot0 = oldRoot1;
		oldRoot1 = newRoot;
		iterations++;
	}
	solution.root = newRoot;
	if (fx(newRoot) != 0)
		solution.error = error;
	solution.iterations = iterations;
	return solution;
}

Solution findRootByFalsePosition(Formula fx) {
	Solution solution;
	solution.error = 0;
	solution.root = 0;
	solution.iterations = 0;
	return solution;
}

Solution findRootByNewton(Formula fx, Formula dfx) {
	Solution solution;
	solution.error = 0;
	solution.root = 0;
	solution.iterations = 0;
	return solution;
}