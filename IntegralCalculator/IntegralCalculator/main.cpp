#include <iostream>
#include <exception>
using namespace std;

//----Problems' IDs----------
#define FX 1
#define TEMP 2

//----Exact Solutions----------
const double EXACT_FX = 0.7912404536792011;
const double EXACT_TEMP_INTEGRAL = 2816;
const double EXACT_AVG = 58.66667;

//----Exceptions Classes----------
class incompatibleMethodException : public exception { //Invalid method exception.
public:
	virtual const char* what() const throw() {
		return "The selected method cannot solve the selected integral.";
	}
};

class unequallySpacedPointsException : public exception { //Unequally Spaced Data Exception
public:
	virtual const char* what() const throw() {
		return "Cannot use the selected method with an unequally-spaced data.";
	}
};

class divideByZeroException : public exception { //Dividing by Zero Exception
public:
	virtual const char* what() const throw() {
		return "Cannot divide by zero.";
	}
};


//----Point Structs----------
struct Point { //2D Point Struct.
	double x;
	double y;
	Point(double x = 0, double y = 0) :x(x), y(y) {}
};

struct Point3D { //3D Point Struct.
	double x;
	double y;
	double z;
	Point3D(double x = 0, double y = 0, double z = 0) :x(x), y(y), z(z) {}
};

ostream &operator<<(ostream &stream, const Point &point) { //Printing 2D Point Operator.
	stream << "(" << point.x << ", " << point.y << ")";
	return stream;
}
ostream &operator<<(ostream &stream, const Point3D &point) { //Printing 3D Point Operator.
	stream << "(" << point.x << ", " << point.y << ", " << point.z << ")";
	return stream;
}

//----Sample Points----------
Point samplePoints[7] = { 
	Point(0, 2),
	Point(0.05, 1.8555),
	Point(0.15, 1.5970),
	Point(0.25, 1.3746),
	Point(0.35, 1.1831),
	Point(0.475, 0.9808),
	Point(0.6, 0.8131)
};

Point3D samplePoints3D[9] = {
	Point3D(0, 0, 72), //0
	Point3D(0, 4, 64), //1
	Point3D(0, 8, 24), //2
	Point3D(3, 0, 54), //3
	Point3D(3, 4, 70), //4
	Point3D(3, 8, 54), //5
	Point3D(6, 0, 0),  //6
	Point3D(6, 4, 40), //7
	Point3D(6, 8, 48)  //8
};

//----Helper Functions----------
void displayProblemsMenu();
void displayExitMenu();
int getSelection(int min, int max);

//----Integral Functions----------
double trapezoidal(Point *points, int ni, int nf); //Computing Integral using Trapezoidal rule.
double simpson13(Point *points, int ni, int nf); //Computing Integral using Simpson 1/3 rule.
double simpson38(Point *points, int ni, int nf); //Computing Integral using Simpson 3/8 rule.
double computeWithBestMethod(Point *points, int ni, int nf); //Selecting the appropriate method.
double getIntegral(Point *points, int ni, int nf); //Getting Integral from Sample Points.
double getMultipleIntegral(Point3D *points, int w, int h, int xi, int xf, int yi, int yf, int seg); //Getting Multiple Integral from Sample Points.

int main(int argc, char **argv) {

	while (1){
		system("cls");
		displayProblemsMenu(); //Problem Selection Menu.
		int selectedProblem = getSelection(1, 3); //Get Selected Method.
		if (selectedProblem == 3)
			exit(0);
		else {
			try{
				double solution, error;
				system("cls");
				cout << "----------------STEPS-----------------------" << endl;
				switch (selectedProblem){
				case FX: //Solving Single Integral Porblem.
					solution = getIntegral(samplePoints, 0, 7); //Computing the Inegral.
					cout << "--------------------------------------------" << endl;
					cout << endl << "Calculated integral of the selected problem: " << solution << endl;
					cout << "Exact integral: " << EXACT_FX << endl;
					if (solution == 0)
						throw divideByZeroException();
					error = (solution - EXACT_FX) / solution; // Calculating the Error.
					if (error < 0) error *= -1;
					cout << "Relative error: ~" << error * 100.0 << "%" << endl;
					break;

				case TEMP: //Solving Multiple Integral Porblem.
					solution = getMultipleIntegral(samplePoints3D, 8, 6, 0, 8, 0, 6, 2); //Computing the Inegral.
					cout << "--------------------------------------------" << endl;
					double averageTemp = solution / (6.0 * 8.0);
					cout << endl << "Calculated integral of the selected problem: " << solution << endl;
					cout << "Exact integral: " << EXACT_TEMP_INTEGRAL << endl;
					if (solution == 0)
						throw divideByZeroException();
					error = (solution - EXACT_TEMP_INTEGRAL) / solution; // Calculating the Error.
					if (error < 0) error *= -1;
					cout << "Integral relative error: ~" << error * 100.0 << "%" << endl << endl;
					cout << "Calculated average temperature: " << averageTemp << endl;
					cout << "Exact average temperature: " << EXACT_AVG << endl;
					if (averageTemp == 0)
						throw divideByZeroException();
					error = (averageTemp - EXACT_AVG) / averageTemp; // Calculating the Error.
					if (error < 0) error *= -1;
					cout << "Temperature relative error: ~" << error * 100.0 << "%" << endl;
					
					break;
				}		

			} catch (divideByZeroException &e){
				cout << e.what() << endl;
			} catch (incompatibleMethodException &e){
				cout << e.what() << endl;
			}
			cout << endl;
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

void displayProblemsMenu() { //Printing Method Selection Menu.
	cout << "Select the integral you want to calcualte:" << endl
		<< "1) f(x) = 2*e^-1.5x  from 0 to 0.6" << endl
		<< "2) T(x,y) = 2xy + 2x - x^2 - 2y^2 + 72 from (0, 0) to (8, 6)" << endl
		<< "3) Quit." << endl;
}

void displayExitMenu() { //Printing Exit Menu.
	cout << "Select one of the following actions:" << endl
		<< "1) Solve another integral." << endl
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

double trapezoidal(Point *points, int ni, int nf) { //Computing Integral using Trapezoidal rule.
	double result = 0;
	if (nf - ni < 1)
		throw incompatibleMethodException();

	for (int i = ni + 2; i <= nf; i++){ //Checking for equally spaced data.
		if ((points[i].x - points[i - 1].x) - (points[i - 1].x - points[i - 2].x) > numeric_limits<double>::epsilon())
			throw unequallySpacedPointsException();
	}


	cout << "Computing the integral of the point(s) ";
	for (int i = ni; i <= nf; i++){
		cout << points[i];
		if (i != nf)
			cout << ", ";
	}
	cout << " using trapezoidal rule." << endl;

	//Integral Computaion:
	result = points[ni].y + points[nf].y; //f(x0) + f(xn).
	double fmid = 0;
	for (int i = ni + 1; i < nf; i++) //f(x_i) where i is between 0 and n.
		fmid += points[i].y;
	fmid *= 2;

	result += fmid;

	result /= 2 * (nf - ni); //Divide by 2n;
	result *= (points[nf].x - points[ni].x); //Multiply by (b - a);

	cout << "Computed result = " << result << endl << endl;
	return result;

}

double simpson13(Point *points, int ni, int nf) { //Computing Integral using Simpson 1/3 rule.
	double result = 0;

	if (nf - ni < 2 || (ni - nf) % 2 != 0)
		throw incompatibleMethodException();

	for (int i = ni + 2; i <= nf; i++){ //Checking for equally spaced data.
		if ((points[i].x - points[i - 1].x) - (points[i - 1].x - points[i - 2].x) > numeric_limits<double>::epsilon())
			throw unequallySpacedPointsException();
	}
		

	cout << "Computing the integral of the point(s) ";
	for (int i = ni; i <= nf; i++){
		cout << points[i];
		if (i != nf)
			cout << ", ";
	}
	cout << " using Simpson's 1/3 rule." << endl;

	//Integral Computaion:
	result = points[ni].y + points[nf].y; //f(x0) + f(xn).

	double fodd = 0;
	for (int i = ni + 1; i < nf; i += 2) //f(x_i) where i is odd.
		fodd += points[i].y;
	fodd *= 4;

	double feven = 0;
	for (int i = ni + 2; i < nf; i += 2) //f(x_i) where i is even.
		feven += points[i].y;
	feven *= 2;

	result += (feven + fodd);
	
	result /= (3.0 * (nf - ni)); //Divide by 3n.

	result *= (points[nf].x - points[ni].x); //Multiply by (b - a).

	cout << "Computed result = " << result << endl << endl;
	return result;
}

double simpson38(Point *points, int ni, int nf) { //Computing Integral using Simpson 3/8 rule.
	double result = 0;

	if (nf - ni < 2 || (ni - nf) % 3)
		throw incompatibleMethodException();

	for (int i = ni + 2; i <= nf; i++){ //Checking for equally spaced data.
		if ((points[i].x - points[i - 1].x) - (points[i - 1].x - points[i - 2].x) > numeric_limits<double>::epsilon())
			throw unequallySpacedPointsException();
	}

	cout << "Computing the integral of the point(s) ";
	for (int i = ni; i <= nf; i++){
		cout << points[i];
		if (i != nf)
			cout << ", ";
	}
	cout << " using Simpson's 3/8 rule." << endl;

	//Integral Computaion:
	result = points[ni].y + points[nf].y; //f(x0) + f(xn).

	double f1 = 0;
	for (int i = ni + 1; i < nf; i += 3) //f(x_i) where i is 1, 4, 7..
		f1 += points[i].y;
	f1 *= 3;

	double f2 = 0;
	for (int i = ni + 2; i < nf; i += 3) //f(x_i) where i is 2, 5, 8..
		f2 += points[i].y;
	f2 *= 3;

	double f3 = 0;
	for (int i = ni + 3; i < nf; i += 3) //f(x_i) where i is 3, 6, 9..
		f3 += points[i].y;
	f3 *= 2;

	
	result += (f1 + f2 + f3);

	result /= 8.0; //Divide by 8.

	result *= (points[nf].x - points[ni].x); //Multiply by (b - a).

	cout << "Computed result = " << result << endl << endl;
	return result;
}


double getIntegral(Point *points, int ni, int nf) { //Getting Integral from Sample Points.
	double result = 0;
	int start = ni, end = start + 1;
	while (end < nf){ //Dividing the data to intervals.
		end = start + 1;
		double delta = points[end].x - points[end - 1].x;
		double diff = delta;
		double comp = diff - delta;
		if (comp < 0)
			comp *= -1.0;
		while (comp < numeric_limits<double>::epsilon()){
			end++;
			diff = points[end].x - points[end - 1].x;
			comp = diff - delta;
			if (comp < 0)
				comp *= -1.0;		
		}
		result += computeWithBestMethod(points, start, end - 1); //Computing using the appropriate method.
		if (end != nf)
			end--;

		start = end;
		
	}
	return result;
}


double getMultipleIntegral(Point3D *points, int w, int h, int xi, int xf, int yi, int yf, int seg) { //Getting Multiple Integral from Sample Points.
	Point xIntegrals[100]; //Integrals along the x-axis.
	double result = 0;
	int deltax = (xf - xi) / seg;
	int deltay = (yf - yi) / seg;


	for (int i = 0; i <= seg; i++){
		cout << "Computing integral along x-axis at y = " << i * deltay << "." << endl;
		Point rowPoints[100];
		for (int j = 0; j <= seg; j++){ //Collecting the points along the x-axis at y = i.
			Point3D point3d = points[i * deltay + j];
			rowPoints[j] = Point(j * deltax, point3d.z);
		}
		double rowIntegral = getIntegral(rowPoints, 0, seg); //Computing the integral along the x-axis at y = i.
		xIntegrals[i] = Point(i * deltay, rowIntegral);
	}

	cout << "Computing final integral along the y-axis." << endl;
	result = getIntegral(xIntegrals, 0, seg); //Integrating the results along the y-axis.

	return result;
}

double computeWithBestMethod(Point *points, int ni, int nf) { //Computing the integral using the appropriate method.
	int size = nf - ni;
	if (size == 1){
		return trapezoidal(points, ni, nf);
	} else if (size % 2 == 0){
		return simpson13(points, ni, nf);
	} else if (size % 3 == 0){
		return simpson38(points, ni, nf);
	} else{
		return trapezoidal(points, ni, nf);
	}
}