#include <iostream>
#include <fstream>
using namespace std;

// Function declarations
bool compare(char* a, char* b);
void addVehicle(char id[], char type[], float& capacity, char driverName[], char** availability);
void editVehicle();
void loadAvailability(char Calendar[100][30], int& totalVehicles);
void saveDataToCalendarFile(char Calendar[100][30], int& totalVehicles);
void loadVehicleIDs(char vehicleIDs[100][20], int& total);
int findVehicleIndex(char vehicleIDs[100][20], int total, char* id);
void scheduleTrip();
void viewFleetStatus();
void viewtripHistory();

int main() {
	int choice = 0;
	do {
		char id[100];
		char type[100];
		float capacity;
		char driverName[100];
		char** availability = new char* [2];
		for (int i = 0; i < 2; i++) {
			availability[i] = new char[30];
		}

		cout << "------------ WELCOME TO VEHICLE FLEET MANAGEMENT SYSTEM ------------" << endl;
		cout << "Menu: \n1. Add New Vehicle\n2. Edit Existing Vehicle Data\n3. Schedule a Trip\n4. View Fleet Status\n5. View Trip History\n6. Exit\n";

		while (true) {
			cout << "Enter choice (1-6): ";
			cin >> choice;

			if (cin.fail()) {
				cout << "Invalid input! Please enter a valid integer." << endl;
				cin.clear();
				cin.ignore(10000, '\n');
			}
			else if (choice < 1 || choice > 6) {
				cout << "Please enter a number between 1 and 6." << endl;
			}
			else {
				cin.ignore();
				break;
			}
		}

		if (choice == 1) {
			addVehicle(id, type, capacity, driverName, availability);
		}
		else if (choice == 2) {
			editVehicle();
		}
		else if (choice == 3) {
			scheduleTrip();
		}
		else if (choice == 4) {
			viewFleetStatus();
		}
		else if (choice == 5) {
			viewtripHistory();
		}
		else if (choice == 6)
		{
			cout << "Good Bye!. Exiting Program!..........";
		}

		for (int i = 0; i < 2; i++) {
			delete[] availability[i];
		}
		delete[] availability;

		cout << endl;

	} while (choice != 6);

	return 0;
}

bool compare(char* a, char* b) {
	int i = 0;
	while (a[i] != '\0' && b[i] != '\0') {
		if (a[i] != b[i]) return false;
		i++;
	}
	return a[i] == '\0' && b[i] == '\0';
}

void addVehicle(char id[], char type[], float& capacity, char driverName[], char** availability) {
	cout << "\n--- Add New Vehicle ---\n";
	cout << "Enter Vehicle ID: ";
	cin.getline(id, 99);

	ifstream fin("vehicles.txt");
	ofstream fout2("calendar.txt", ios::app);
	bool idExists = false;

	if (fin) {
		char existing_id[100], existing_type[100], existing_drivername[100], existing_status[100];
		float existing_capacity;

		while (fin >> existing_id >> existing_type >> existing_capacity >> existing_drivername >> existing_status) {
			if (compare(existing_id, id)) {
				idExists = true;
				break;
			}
		}
		fin.close();
	}

	if (idExists) {
		cout << "Vehicle ID already exists!" << endl;
		return;
	}

	cout << "Enter Vehicle Type: ";
	cin.getline(type, 99);
	while (true) {
		cout << "Enter an capacity (in tons): ";
		cin >> capacity;
		if (cin.fail()) {
			cout << "Invalid input! Please enter a valid number." << endl;
			cin.clear();
			cin.ignore(10000, '\n');
		}
		else if (capacity < 1) {
			cout << "Please enter a number greater than 0" << endl;
		}
		else {

			break;
		}
	}

	cin.ignore();
	cout << "Enter Driver Name: ";
	cin.getline(driverName, 99);

	ofstream fout("vehicles.txt", ios::app);
	if (!fout) {
		cout << "Error opening file for writing." << endl;
		return;
	}

	fout << id << " " << type << " " << capacity << " " << driverName << " " << "------------------------------" << endl;
	fout2 << id << ":";
	for (int i = 0; i < 30; i++)
		fout2 << "-";
	fout2 << endl;
	fout2.close();
	fout.close();
	cout << "Vehicle Added Successfully." << endl;
}

void editVehicle() {
	char id[100];
	char new_type[100];
	float new_capacity;
	char new_driverName[100];

	cout << "\n----- Edit Vehicle ------" << endl;

	cout << "Enter Vehicle Id you want to Edit: ";
	cin.getline(id, 99);

	ifstream fin("vehicles.txt");
	ofstream fout("temp.txt");
	bool flag = false;

	if (!fin) {
		cout << "Error in the opening of file!.....";
		return;
	}

	char vId[100], vType[100], vDriverName[100], vStatus[100];
	float vCapacity;

	while (fin >> vId >> vType >> vCapacity >> vDriverName >> vStatus) {
		if (compare(vId, id)) {
			flag = true;
			cin.ignore();
			cout << "Enter new Type: ";
			cin.getline(new_type, 99);
			while (true) {
				cout << "Enter a new capacity (in tons): ";
				cin >> new_capacity;
				if (cin.fail()) {
					cout << "Invalid input! Please enter a valid number." << endl;
					cin.clear();
					cin.ignore(10000, '\n');
				}
				else if (new_capacity < 1) {
					cout << "Please enter a number greater than 0" << endl;
				}
				else {

					break;
				}
			}
			cin.ignore();
			cout << "Enter New Driver Name: ";
			cin.getline(new_driverName, 99);
			fout << vId << " " << new_type << " " << new_capacity << " " << new_driverName << " " << vStatus << " ";
			fout << endl;
		}
		else {
			fout << vId << " " << vType << " " << vCapacity << " " << vDriverName << " " << vStatus << " ";
			fout << endl;
		}
	}

	fin.close();
	fout.close();
	remove("vehicles.txt");
	rename("temp.txt", "vehicles.txt");

	if (flag)
		cout << "Vehicle updated successfully." << endl;
	else
		cout << "Vehicle ID not found." << endl;
}

void loadAvailability(char Calendar[100][30], int& totalVehicles) {
	ifstream fin("calendar.txt");
	char temp[100];
	totalVehicles = 0;

	while (fin.getline(temp, 99, ':')) {

		for (int j = 0; j < 30; j++) {
			fin >> noskipws >> Calendar[totalVehicles][j];
		}
		fin.ignore(1000, '\n'); // skip remaining line
		totalVehicles++;
	}
	fin.close();
}

void saveDataToCalendarFile(char Calendar[100][30], int& totalVehicles) {
	ifstream fin("vehicles.txt");
	ofstream fout("calendar.txt");
	char id[100];
	int index = 0;

	while (fin >> id) {
		fin.ignore(1000, '\n');
		fout << id << ": ";
		for (int j = 0; j < 30; j++) {
			fout << Calendar[index][j];
		}
		fout << endl;
		index++;
		if (index >= totalVehicles) break;
	}
	fin.close();
	fout.close();
}

void loadVehicleIDs(char vehicleIDs[100][20], int& total) {
	ifstream fin("vehicles.txt");
	total = 0;
	while (fin >> vehicleIDs[total]) {
		fin.ignore(1000, '\n');
		total++;
	}
	fin.close();
}

int findVehicleIndex(char vehicleIDs[100][20], int total, char* id) {
	for (int i = 0; i < total; i++) {
		if (compare(vehicleIDs[i], id))
			return i;
	}
	return -1;
}

void scheduleTrip() {
	char tripID[20], vehicleID[20], source[30], destination[30];
	int day, distance;
	float cost, fuel;
	char vehicleIDs[100][20];
	char calendar[100][30];
	int totalVehicles;

	loadAvailability(calendar, totalVehicles);
	loadVehicleIDs(vehicleIDs, totalVehicles);

	cout << "Enter Trip ID: ";
	cin >> tripID;
	cout << "Enter Vehicle ID: ";
	cin >> vehicleID;
	int index = findVehicleIndex(vehicleIDs, totalVehicles, vehicleID);
	if (index == -1) {
		cout << "Vehicle ID not found." << endl;
		return;
	}
	cout << "Enter Source City: ";
	cin >> source;
	cout << "Enter Destination City: ";
	cin >> destination;


	while (true) {
		cout << "Enter Distance (km): ";
		cin >> distance;
		if (cin.fail()) {
			cout << "Invalid input! Please enter a valid integer." << endl;
			cin.clear();
			cin.ignore(10000, '\n');
		}
		else if (distance < 1) {
			cout << "Please enter a positive distance ." << endl;
		}
		else {
			break;
		}
	}


	while (true) {
		cout << "Enter Fuel Used (liters): ";
		cin >> fuel;
		if (cin.fail()) {
			cout << "Invalid input! Please enter a valid number." << endl;
			cin.clear();
			cin.ignore(10000, '\n');
		}
		else if (fuel < 1) {
			cout << "Please enter a positive number ." << endl;
		}
		else {
			break;
		}
	}

	while (true) {
		cout << "Enter Trip Day (1-30): ";
		cin >> day;
		if (cin.fail()) {
			cout << "Invalid input! Please enter a valid integer." << endl;
			cin.clear();
			cin.ignore(10000, '\n');
		}
		else if (day < 1 || day > 300) {
			cout << "Please enter a day between 1 and 30." << endl;
		}
		else {

			break;
		}
	}


	if (calendar[index][day - 1] == 'T') {
		cout << "Vehicle is already scheduled on this day." << endl;
		return;
	}

	calendar[index][day - 1] = 'T';
	saveDataToCalendarFile(calendar, totalVehicles);

	cost = fuel * 275;

	ofstream fout("trips.txt", ios::app);
	fout << tripID << " " << vehicleID << " " << source << " " << destination << " "
		<< distance << " " << fuel << " " << cost << " " << day << endl;
	fout.close();

	cout << "Trip scheduled successfully! \nFuel Cost: Rs. " << cost << endl;
}

void viewFleetStatus() {
	int totalVehicles;
	char vehicleIDs[100][20];
	char calendar[100][30];

	loadAvailability(calendar, totalVehicles);
	loadVehicleIDs(vehicleIDs, totalVehicles);

	cout << "\nVehicle Availability Calendar:\n\t";
	for (int d = 0; d < 30; d++)
		cout << d + 1 << " ";
	cout << "\n";

	for (int i = 0; i < totalVehicles; i++) {
		cout << vehicleIDs[i] << ": ";
		for (int j = 0; j < 30; j++) {
			cout << calendar[i][j] << " ";
		}
		cout << endl;
	}
}

void viewtripHistory() {
	ifstream fin("trips.txt");
	ifstream fin2("trips.txt");
	if (!fin.is_open()) {
		cout << "Error opening File!" << endl;
	}
	else {
		char trip_id[40], vehicle_id[40], source[40], destination[40];
		float distance = 0, liters = 0, date = 0, price = 0;

		if (!(fin2 >> trip_id))
		{
			cout << "No trip History!" << endl;
		}
		else
		{
			cout << "Trip History: " << endl;
			while (fin >> trip_id >> vehicle_id >> source >> destination >> distance >> liters >> price >> date) {
				cout << "Trip Id: " << trip_id << "\nVehicle ID: " << vehicle_id << "\nSource: " << source << "\nDestination: " << destination
					<< "\nDistance: " << distance << " km\nLiters: " << liters << "\nDate: " << date << "\nPrice: Rs. " << price << endl << endl;
			}
		}
	}
}