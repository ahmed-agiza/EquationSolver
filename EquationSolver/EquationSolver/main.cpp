#include <iostream>
#include <cmath>
#include <functional>
#include <exception>

using namespace std;

class divideByZeroException : public exception {
	virtual const char* what() const throw() {
		return "Cannot divide by zero";
	}
};

typedef std::function<double(double)> Formula;

void displayEquationsMenu();
void displayMethodsMenu();
int getSelection(int min, int max);

double findRootByBisection(Formula fx, double x0, double x1, double eps);
double findRootBySecant(Formula fx, double x0, double x1, double eps);
double findRootByFalsePosition(Formula fx, double x0, double x1, double eps);
double findRootByNewton(Formula fx, Formula dfx, double x0, double eps);

Formula f1 = [](double x) {
	return (pow(x, 3) - 8 * pow(x, 2) + 12 * x - 4);
};

Formula f2 = [](double x) {
	return (-12 - 21 * x + 18 * pow(x, 2) - 2.75 * pow(x, 3));
};

Formula f3 = [](double x) {
	return (6 * x - 4 * pow(x, 2) + 0.5 * pow(x, 3) - 2);
};

Formula f4 = [](double x) {
	return log(pow(x, 4)) - 0.7;
};

Formula f5 = [](double x) {
	return (7 * sin(x) - exp(x));
};

Formula df1 = [](double x) {
	return (3 * pow(x, 3) - 16 * x + 12);
};



Formula df2 = [](double x) {
	return (-21 + 36 * x - 8.25 * pow(x, 2));
};

Formula df3 = [](double x) {
	return (6 - 8 * x + 1.5 * pow(x, 2));
};

Formula df4 = [](double x) {
	if (x == 0)
		throw divideByZeroException();
	return (3 / x);
};

Formula df5 = [](double x) {
	return (7 * cos(x) - exp(x));
};

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
	bool quit = false;

	while (1){
		displayEquationsMenu();
		int selectedEquation = getSelection(1, 6);
		if (selectedEquation == 6)
			exit(0);

		system("cls");

		displayMethodsMenu();
		int selectedMethod = getSelection(1, 6);
		if (selectedMethod == 6)
			exit(0);
		else if (selectedMethod != 5){
			cout << "Solution.." << endl;
		}
	}
	return 0;
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