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

class zeroDiagonalException : public exception {
public:
	virtual const char* what() const throw() {
		return "Cannot eliminate zero diagonal elements";
	}
};

//----Matrix Class----------
struct Row4 {
	double element[4];
};

struct Column3 {
	double element[3];
};

Row4 operator+(Row4 rowA, Row4 rowB) {
	Row4 result;
	result.element[0] = rowA.element[0] + rowB.element[0];
	result.element[1] = rowA.element[1] + rowB.element[1];
	result.element[2] = rowA.element[2] + rowB.element[2];
	result.element[3] = rowA.element[3] + rowB.element[3];
	return result;
}

Row4 operator*(Row4 rowA, double scale) {
	Row4 result;
	result.element[0] = scale * rowA.element[0];
	result.element[1] = scale * rowA.element[1];
	result.element[2] = scale * rowA.element[2];
	result.element[3] = scale * rowA.element[3];
	return result;
}


class Matrix3x4 {
	Row4 rows[3];

public:
	Matrix3x4(double a00 = 0, double a01 = 0, double a02 = 0, double a03 = 0,
		double a10 = 0, double a11 = 0, double a12 = 0, double a13 = 0,
		double a20 = 0, double a21 = 0, double a22 = 0, double a23 = 0) {
		rows[0].element[0] = a00; rows[0].element[1] = a01; rows[0].element[2] = a02; rows[0].element[3] = a03;
		rows[1].element[0] = a10; rows[1].element[1] = a11; rows[1].element[2] = a12; rows[1].element[3] = a13;
		rows[2].element[0] = a20; rows[2].element[1] = a21; rows[2].element[2] = a22; rows[2].element[3] = a23;
	}
	
	Row4 addRow(int indexA, int indexB, double scale = 1.0) {
		return (rows[indexA] = rows[indexA] + (rows[indexB] * scale));
	}

	Column3 getColumn(int index) const {
		Column3 column;
		for (int i = 0; i < 3; i++)
			column.element[i] = rows[i].element[index];
		return column;
	}

	double getElement(int row, int column) const {
		return rows[row].element[column];
	}

	double getPivot(int index) const{
		return rows[index].element[index];
	}

	void multiplyRow(int index, double scale) {
		rows[index] = rows[index] * scale;
	}
	
	void setRow(int index, Row4 row) {
		rows[index] = row;
	}
	
	void setRow(int index, double a0 = 0.0, double a1 = 0.0, double a2 = 0.0, double a3 = 0.0) {
		rows[index].element[0] = a0;
		rows[index].element[1] = a1;
		rows[index].element[2] = a2;
		rows[index].element[3] = a3;
	}
	
	void setElement(int row, int column, double value = 0.0) {
		rows[row].element[column] = value;
	}

	void swapRows(int rowA, int rowB) {
		Row4 temp = rows[rowA];
		rows[rowA] = rows[rowB];
		rows[rowB] = temp;
	}

	bool hasZeroDiagonalElement() const {
		return (rows[0].element[0] == 0 || rows[1].element[1] == 0 || rows[2].element[2] == 0);
	}

	bool hasZeroDiagonalElement(int index) const {
		return rows[index].element[index] == 0;
	}

	void eliminateZeroDiagonalElements() {
		Matrix3x4 original = *this;
		double iterations = 0;
		if (hasZeroDiagonalElement(0)){
			swapRows(0, 1);
			if (hasZeroDiagonalElement(0))
				swapRows(0, 2);
		}
		if (hasZeroDiagonalElement(1)){
			swapRows(1, 2);
			if (hasZeroDiagonalElement(1)){
				swapRows(0, 1);
				if (hasZeroDiagonalElement(0))
					swapRows(0, 2);
			}
		}
			
		if (hasZeroDiagonalElement()){
			*this = original;
			throw zeroDiagonalException();
		}
	}

	bool isReducedEchelonForm() const {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				if (j == i){
					continue;
				} else{
					if (rows[i].element[j] != 0)
						return false;
				}
		
		return true;
	}

	Row4 getRow(int index) const {
		return rows[index];
	}

	void removeEpsilons() {
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 4; j++)
				if ((rows[i].element[j] <= numeric_limits<double>::epsilon() * 10.0 && rows[i].element[j] > 0)
					|| (rows[i].element[j] >= numeric_limits<double>::epsilon() * -10.0 && rows[i].element[j] < 0))
					rows[i].element[j] = 0;
		}
	}

	void normalizePivots() {
		for (int i = 0; i < 3; i++){
			double pivot = getPivot(i);
			if (pivot == 0)
				throw divideByZeroException();
			rows[i] = rows[i] * (1.0 / pivot);
		}
	}
};

ostream &operator<<(ostream &stream, const Row4 &row) {
	stream << row.element[0] << "\t" << row.element[1] << "\t" << row.element[2] << "\t" << row.element[3] << endl;
	return stream;
}

ostream &operator<<(ostream &stream, const Matrix3x4 &matrix) {
	Row4 matrixRows[3];
	matrixRows[0] = matrix.getRow(0);
	matrixRows[1] = matrix.getRow(1);
	matrixRows[2] = matrix.getRow(2);
	stream << matrixRows[0] << matrixRows[1] << matrixRows[2];
	return stream;
}

//----Solution Struct----------
struct Solution {
	double x0;
	double x1;
	double x2;
	double error;
	int iterations;
};

//----Equation Function Object----------
typedef function<double(double, double)> Equation;


//----Helper Functions----------
void initEquations();
void displayMethodsMenu();
void displayExitMenu();
int getSelection(int min, int max);


//----Solution Computation Functions----------
double getMaxError(double a0, double a1, double a2);
Solution findRootByGauss(Matrix3x4 coeff);
Solution findRootByJacobi(Equation e0, Equation e1, Equation e2);


//----Equations' Definitions----------
Equation equations[4]; //Roots Equations. 
Matrix3x4 coefficients; //Equation Coefficients.


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
					<< "x2 = " << solution.x2 << endl;
				if (selectedMethod != GAUSS){
					cout << "Error: ~" << solution.error * 100.0 << " %" << endl
						 << "Number of iterations: " << solution.iterations << endl;
				}

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
	equations[1] = [](double x1, double x2) {
		return ((27 - 2 * x1 + x2) / 10.0); //x0 Equation.
	};
	equations[2] = [](double x0, double x2) {
		return ((-61.5 + 3 * x0 - 2 * x2) / -6.0); //x1 Equation.
	};
	equations[3] = [](double x0, double x1) {
		return ((-21.5 - x0 - x1) / 5.0); //x2 Equation.
	};

	coefficients = Matrix3x4(10,  2, -1, 27, //Coefficients Matrix.
							 -3, -6,  2, -61.5,
							  1,  1,  5, -21.5); 
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

Solution findRootByGauss(Matrix3x4 coeff) { //Computing the solution using Gauss-Jordan Elimination Method.
	Solution solution;
	solution.x0 = 0; solution.x1 = 0; solution.x2 = 0; solution.error = 0; solution.iterations = 0;
	coeff.eliminateZeroDiagonalElements();
	for (int i = 0; i < 3; i++){
		double pivot = coeff.getPivot(i);
		for (int j = 0; j < 3; j++){
			if (i == j)
				continue;
			double numerator = coeff.getElement(j, i);
			double factor = -1 * numerator / pivot;
			coeff.addRow(j, i, factor);
			coeff.removeEpsilons();
		}
	}
	coeff.normalizePivots();
	Column3 solutionColumn = coeff.getColumn(3);
	solution.x0 = solutionColumn.element[0];
	solution.x1 = solutionColumn.element[1];
	solution.x2 = solutionColumn.element[2];
	return solution;
}

Solution findRootByJacobi(Equation e0, Equation e1, Equation e2) { //Computing the solution using the Jacobi Method.
	Solution solution;
	solution.x0 = 0; solution.x1 = 0; solution.x2 = 0; solution.error = 0; solution.iterations = 0;

	return solution;
}