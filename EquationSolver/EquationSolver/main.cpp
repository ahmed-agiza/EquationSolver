#include <iostream>
#include <cmath>

using namespace std;

double f1(double x) {
	return (pow(x, 3) - 8 * pow(x, 2) + 12 * x - 4);
}

double f2(double x) {
	return (-12 - 21 * x + 18 * pow(x, 2) - 2.75 * pow(x, 3));
}

int main(int argc, char **argv) {
	char selection = ' ';
	cout << "Select the equation you want to solve:" << endl
		<< "1) x^3 - 8x^2 + 12x = 4" << endl
		<< "2) f(x) = -12 - 21x + 18x^2 - 2.75x^3" << endl
		<< "3) f(x) = 6x - 4x^2 + 0.5 x^3 - 2" << endl
		<< "4) ln(x^4) =  0.7" << endl
		<< "5) 7sin(x) = e^x" << endl
		<< "6) Quit." << endl;

	while (selection != 'q' && selection != '6'){
		cout << "Input: ";
		cin >> selection;
		cout << "Captured " << selection << endl;
		switch (selection){
		case '1':
			cout << "1." << endl;
			break;
		case '2':
			cout << "2." << endl;
			break;
		case '3':
			cout << "3." << endl;
			break;
		case '4':
			cout << "4." << endl;
			break;
		case '5':
			cout << "5." << endl;
		case '6':
			break;
		default:
			cout << "Invalid option." << endl << selection << endl;
		}
	}

	return 0;
}