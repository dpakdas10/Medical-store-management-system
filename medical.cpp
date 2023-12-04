#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include <map>

using namespace std;

class Medicine {
public:
    int id;
    string name;
    int quantity;
    double price;
    double total;
    string expiryDate;

    // Member functions for Medicine
    void addMedicine();
    void displayMedicineStock();
    
};


class Customer {
public:
    int id;
    string name;
    double billAmount;
    vector<Medicine> purchasedItems;
    Customer() : id(0), billAmount(0.0) {}
    // Member functions for Customer
    void displayCustomerInfo();
    void saveCustomerToFile();
};


class Supplier {
public:
    int id;
    string name;
    string contact;
    int quantitySupplied;
    double billAmount;

    // Member functions for Supplier
    void displaySupplierInfo();
};


class SRMMedicalStore {
private:
    vector<Medicine> medicineStock;
    vector<Customer> customers;
    vector<Supplier> suppliers;

public:
    // Member functions for SRMMedicalStore
    void sellMedicine();
    void generateBill();
    void displayMenu();
    void run();
};


bool authenticateUser() {
    std::string username, password;
    
    // Replace this with your actual authentication logic
    std::cout << "Enter username: ";
    std::cin >> username;
    
    std::cout << "Enter password: ";
    std::cin >> password;

    // Example: Check if the username and password match some predefined values
    return (username == "admin" && password == "485");
}


// Member function definitions for Medicine class
void Medicine::addMedicine() {
    // Authenticate the user before allowing access to add medicine
    cout<<"The user name is admin and password in 485"<<endl;
    if (!authenticateUser()) {
        std::cout << "Authentication failed. Access denied.\n";
        return;
    }
    cout << "Enter the number of Medicines to add: ";
    int numMedicines;
    cin >> numMedicines;

    for (int i = 0; i < numMedicines; ++i) {
        cout << "Enter Medicine Details for Medicine " << i + 1 << ":\n";

        cout << "Medicine ID: ";
        cin >> id;

        cout << "Medicine Name: ";
        cin.ignore(); // Clear the buffer
        getline(cin, name);

        cout << "Quantity: ";
        cin >> quantity;

        cout << "Price per piece: ";
        cin >> price;

        cout << "Expiry Date: ";
        cin.ignore(); // Clear the buffer
        getline(cin, expiryDate);

        // Ask for supplier information
        Supplier supplier;
        cout << "\nEnter Supplier Details for Medicine " << i + 1 << ":\n";

        cout << "Supplier ID: ";
        cin >> supplier.id;

        cout << "Supplier Name: ";
        cin.ignore(); // Clear the buffer
        getline(cin, supplier.name);

        cout << "Supplier Contact: ";
        cin >> supplier.contact;

        cout << "Enter supplied quantity: ";
        cin >> supplier.quantitySupplied;

        cout << "Enter the bill amount: ";
        cin >> supplier.billAmount;

        // Save the medicine details to a file (you can modify this part to use a database)
        ofstream outFile("medicine_stock.txt", ios::app);
        ofstream supplierFile("supplier_info.txt", ios::app);

        if (outFile.is_open() && supplierFile.is_open()) {
            // Save medicine details
            outFile << id << "," << name << "," << quantity << "," << price << "," << expiryDate << "\n";
            outFile.close();

            // Save supplier details
            supplierFile << supplier.id << "," << supplier.name << "," << supplier.contact << ","
                         << supplier.quantitySupplied << "," << supplier.billAmount << "\n";
            supplierFile.close();

            cout << "Medicine and Supplier added successfully!\n";
        } else {
            cout << "Unable to open the file(s) for writing.\n";
        }
    }
}


void Customer::saveCustomerToFile() {
    ofstream outFile("customer_info.txt", ios::app);

    if (outFile.is_open()) {
        outFile << id << "," << name << "," << billAmount << "\n";
        outFile.close();
        cout << "Customer information saved to file.\n";
    } else {
        cout << "Unable to open the file for writing.\n";
    }
}


void Medicine::displayMedicineStock() {
    ifstream inFile("medicine_stock.txt");

    if (inFile.is_open()) {
        cout << "\nMedicine Stock:\n";
        cout <<"**************************************************************************\n";
        cout << "ID  | Medicine Name     | Quantity | Price  | Expiry Date\n";
        cout <<"**************************************************************************\n";

        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            string token;

            // Create a local Medicine instance for each line
            Medicine medicine;

            // Tokenize the line using ',' as the delimiter
            getline(ss, token, ',');
            medicine.id = stoi(token);

            getline(ss, medicine.name, ',');
            getline(ss, token, ',');
            medicine.quantity = stoi(token);

            getline(ss, token, ',');
            medicine.price = stod(token);

            getline(ss >> ws, medicine.expiryDate); // Use getline after consuming leading whitespaces

            // Display medicine details from the local instance
            cout << setw(3) << medicine.id << " | " << setw(17) << medicine.name << " | "
                 << setw(8) << medicine.quantity << " | " << setw(6) << fixed << setprecision(2)
                 << medicine.price << " | " << medicine.expiryDate << endl;
        }

        inFile.close();
    } else {
        cout << "Unable to open the file for reading.\n";
    }
    cout << endl << endl;
}


// Member function definitions for Supplier class
void Supplier::displaySupplierInfo() {
    cout<<"The user name is admin and password in 485"<<endl;
    if (!authenticateUser()) {
        cout << "Authentication failed. Access denied.\n";
        return;
    }
    
    // Read supplier information from file
    ifstream supplierFile("supplier_info.txt");

    if (!supplierFile.is_open()) {
        cout << "Error: Unable to open supplier_info.txt.\n";
        return;
    }

    cout << "\nSupplier Information:\n";
    cout << "ID  | Supplier Name     | Contact         | Quantity Supplied | Bill Amount\n";
    cout << "---------------------------------------------------------------------------\n";

    string line;
    map< string, vector <Supplier> >suppliersByName;  // Map to store suppliers by name

    while (getline(supplierFile, line)) {
        stringstream ss(line);
        string token;

        Supplier currentSupplier;  // Create a local Supplier object

        // Tokenize the line using ',' as the delimiter
        getline(ss, token, ',');
        currentSupplier.id = stoi(token);

        getline(ss, currentSupplier.name, ',');
        getline(ss, currentSupplier.contact, ',');
        getline(ss, token, ',');
        currentSupplier.quantitySupplied = stoi(token);
        getline(ss, token, ',');
        currentSupplier.billAmount = stod(token);

        // Add the current supplier to the map, grouped by name
        suppliersByName[currentSupplier.name].push_back(currentSupplier);
    }

    // Display supplier details
    for (const auto& entry : suppliersByName) {
        const string& supplierName = entry.first;
        const vector<Supplier>& supplierList = entry.second;

        for (const Supplier& currentSupplier : supplierList) {
            cout << setw(3) << currentSupplier.id << " | " << setw(17) << supplierName << " | "
                 << setw(16) << currentSupplier.contact << " | " << setw(17) << currentSupplier.quantitySupplied << " | "
                 << setw(11) << fixed << setprecision(2) << currentSupplier.billAmount << "\n";
        }
    }

    supplierFile.close();
}


// Member function definitions for SRMMedicalStore class
void SRMMedicalStore::sellMedicine() {
   // Assuming customers and medicineStock are vectors of Customer and Medicine objects
Customer currentCustomer; // Declare a new customer

cout << "Enter the ID number: ";
cin >> currentCustomer.id;
cout << "Enter customer name: ";
cin.ignore(); // Clear the buffer
getline(cin, currentCustomer.name);

cout << "Enter the number of medicines to buy: ";
int numMedicinesToBuy;
cin >> numMedicinesToBuy;

for (int i = 0; i < numMedicinesToBuy; ++i) {
    ifstream inFile("medicine_stock.txt");

    if (!inFile.is_open()) {
        cout << "Error: Unable to open the medicine stock file.\n";
        return;
    }

    cout << "\nMedicine Stock:\n";
    cout << "ID  | Medicine Name     | Quantity | Price  | Expiry Date\n";
    cout << "**************************************************************************\n";

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string token;

        Medicine currentMedicine; // Declare a new medicine

        // Tokenize the line using ',' as the delimiter
        getline(ss, token, ',');
        currentMedicine.id = stoi(token);

        getline(ss, currentMedicine.name, ',');
        getline(ss, token, ',');
        currentMedicine.quantity = stoi(token);

        getline(ss, token, ',');
        currentMedicine.price = stod(token);

        getline(ss >> ws, currentMedicine.expiryDate); // Use getline after consuming leading whitespaces

        // Display medicine details
        cout << setw(3) << currentMedicine.id << " | " << setw(17) << currentMedicine.name << " | "
             << setw(8) << currentMedicine.quantity << " | " << setw(6) << fixed << setprecision(2)
             << currentMedicine.price << " | " << currentMedicine.expiryDate << endl;
    }
    cout << endl << endl;

    // Close the original inFile after reading medicine information
    inFile.close();

    // Get medicine ID to sell
    cout << "Enter medicine ID to sell: ";
    int medicineId;
    cin >> medicineId;

    // Validate the entered medicine ID against the displayed medicine stock
    bool validMedicineId = false;
    ifstream stockFile("medicine_stock.txt");

    if (stockFile.is_open()) {
        string line;
        while (getline(stockFile, line)) {
            stringstream ss(line);
            string token;

            // Tokenize the line using ',' as the delimiter
            getline(ss, token, ',');
            int stockMedicineId = stoi(token);

            if (medicineId == stockMedicineId) {
                validMedicineId = true;
                break;
            }
        }
        stockFile.close();
    }

    if (!validMedicineId) {
        cout << "Error: Invalid medicine ID.\n";
        return;
    }

    // Declare selectedMedicine before using it
    Medicine selectedMedicine;

    // Read only the necessary medicine details from the file
    ifstream inFile2("medicine_stock.txt");

    if (!inFile2.is_open()) {
        cout << "Error: Unable to open the medicine stock file.\n";
        return;
    }

    bool medicineFound = false;
    while (getline(inFile2, line)) {
        stringstream ss(line);
        string token;

        // Tokenize the line using ',' as the delimiter
        getline(ss, token, ',');
        selectedMedicine.id = stoi(token);

        getline(ss, selectedMedicine.name, ',');
        getline(ss, token, ',');
        selectedMedicine.quantity = stoi(token);

        getline(ss, token, ',');
        selectedMedicine.price = stod(token);

        getline(ss >> ws, selectedMedicine.expiryDate); // Use getline after consuming leading whitespaces

        // Check if the current line corresponds to the selected medicine
        if (selectedMedicine.id == medicineId) {
            medicineFound = true;
            break;
        }
    }

    inFile2.close();

    if (!medicineFound) {
        cout << "Error: Medicine not found with ID " << medicineId << ".\n";
        return;
    }

    cout << "Enter quantity to sell: ";
    int quantityToSell;
    cin >> quantityToSell;

    // Validate the entered quantity
    if (quantityToSell <= 0 || quantityToSell > selectedMedicine.quantity) {
        cout << "Error: Invalid quantity or not enough stock available.\n";
        return;
    }

    // Update stock quantity
    selectedMedicine.quantity -= quantityToSell;

    // Calculate and update bill amount
    double totalAmount = quantityToSell * selectedMedicine.price;
    currentCustomer.billAmount += totalAmount;

    cout << "Sale successful!\n";

    // Optionally display a summary of the transaction
    cout << "Sold " << quantityToSell << " of " << selectedMedicine.name << " to "
         << currentCustomer.name << " for a total of $" << totalAmount << endl;
    inFile.close();

    inFile.open("medicine_stock.txt", ios::in);  // Reopen the file for reading
    ofstream outFile("temp.txt"); // Temporary file to store updated data

    if (inFile.is_open() && outFile.is_open()) {
        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            string token;

            Medicine med; // Temporary medicine object to store each line's data

            // Tokenize the line using ',' as the delimiter
            getline(ss, token, ',');
            med.id = stoi(token);

            getline(ss, med.name, ',');
            getline(ss, token, ',');
            med.quantity = stoi(token);

            getline(ss, token, ',');
            med.price = stod(token);

            getline(ss >> ws, med.expiryDate); // Use getline after consuming leading whitespaces

            // Check if the current line corresponds to the selected medicine
            if (med.id == medicineId) {
                // Update the quantity for the sold medicine
                med.quantity -= quantityToSell;

                // Check if the updated quantity is less than zero, set it to zero
                if (med.quantity < 0) {
                    med.quantity = 0;
                }

                // Write the medicine data to the temporary file
                outFile << med.id << "," << med.name << "," << med.quantity << ","
                        << med.price << "," << med.expiryDate << "\n";

                // Write the sales information to the sales file
                ofstream salesFile("customer_info.txt", ios::app);
                if (salesFile.is_open()) {
                    salesFile << currentCustomer.id << "," << currentCustomer.name << ","
                               << med.id << "," << med.name << ","
                               << quantityToSell << "," << med.price << "," << totalAmount << "\n";
                    salesFile.close();
                } else {
                    cout << "Error: Unable to open the sales file for writing.\n";
                }
            } else {
                // Write unchanged medicine data to the temporary file
                outFile << med.id << "," << med.name << "," << med.quantity << ","
                        << med.price << "," << med.expiryDate << "\n";
            }
        }

        inFile.close();  // Close the file before opening it again
        outFile.close();

        // Rename the temporary file to the original file
        remove("medicine_stock.txt");
        rename("temp.txt", "medicine_stock.txt");

        cout << "Medicine stock file updated.\n";
    } else {
        cout << "Error: Unable to open the medicine stock file for reading or writing.\n";
    }
}
}


void SRMMedicalStore::generateBill() {
    // Read customer information from file
    ifstream customerFile("customer_info.txt");
    if (!customerFile.is_open()) {
        cout << "Error: Unable to open the customer data file.\n";
        return;
    }

    // Read customer data and populate the vector
    customers.clear();  // Clear the existing customer vector

    string line;
    while (getline(customerFile, line)) {
        stringstream ss(line);
        string token;

        Customer customer;

        // Tokenize the line using ',' as the delimiter
        getline(ss, token, ',');
        customer.id = stoi(token);

        getline(ss, customer.name, ',');

        // Continue to read the purchased items
        while (getline(ss, token, ',')) {
            Medicine item;
            item.id = stoi(token);

            getline(ss, item.name, ',');
            getline(ss, token, ',');
            item.quantity = stoi(token);

            getline(ss, token, ',');
            item.price = stod(token);

            getline(ss, token, ',');
            item.total = stod(token);

            customer.purchasedItems.push_back(item);
        }

        // Calculate the total bill amount for the customer
        for (const auto& item : customer.purchasedItems) {
            customer.billAmount += item.total;
        }

        // Add the customer to the vector
        customers.push_back(customer);
    }

    customerFile.close();

    // Get the customer ID for whom the bill needs to be generated
    int customerId;
    cout << "Enter customer ID to generate the bill: ";
    cin >> customerId;

    // Search for the customer in the vector and display all bills if the ID is repeated
    bool customerFound = false;

    cout << "\nBill(s) for Customer ID " << customerId << ":\n";
    cout << "---------------------------------------------------\n";
    cout << "ID  | Medicine Name     | Quantity | Price  | Total\n";
    cout << "---------------------------------------------------\n";
    double totalBillAmount = 0.0;
    string name="";
    for (const auto& customer : customers) {
        if (customer.id == customerId) {
            customerFound = true;
            name=customer.name;
            // Display the items purchased by the customer
            for (const auto& item : customer.purchasedItems) {
                cout << setw(3) << item.id << " | " << setw(17) << item.name << " | "
                    << setw(8) << item.quantity << " | " << setw(6) << fixed << setprecision(2)
                    << item.price << " | " << setw(6) << fixed << setprecision(2) << item.total << "\n";

                // Add the item total to the total bill amount
                totalBillAmount += item.total;
            }

        
        }
}
    cout<<endl;
    // Display the total bill amount for all customers
    if (customerFound) {
        cout << "Total Bill Amount for customer name "<<name<<" is : $" << totalBillAmount << "\n";
    } else {
        cout << "Error: Customer not found with ID " << customerId << ".\n";
        return;
    }
}


void SRMMedicalStore::displayMenu() {
    cout<<"\t\t==================================================\n";
    cout << "\t\t||\t=== SRM Medical Store System ===\t||\n";
	cout<<"\t\t--------------------------------------------------\n";
    cout << "\t\t||\t1. Add Medicine to Stock \t\t||\n";
    cout << "\t\t||\t2. Display Medicine Stock\t\t||\n";
    cout << "\t\t||\t3. Sell Medicine\t\t\t||\n";
    cout << "\t\t||\t4. Generate Bill\t\t\t||\n";
    cout << "\t\t||\t5. Display Supplier Information\t\t||\n";
    cout << "\t\t||\t6. Exit\t\t\t\t\t||\n";
    cout<<"\t\t--------------------------------------------------\n";
}


void SRMMedicalStore::run() {
    int choice;
    char choose;
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                medicineStock.push_back(Medicine());
                medicineStock.back().addMedicine();
                break;
            case 2:
                medicineStock[0].displayMedicineStock(); // Displaying stock for the first medicine for simplicity
                break;
            case 3:
                sellMedicine();
                break;
            case 4:
                generateBill();
                break;
            case 5:
                suppliers[0].displaySupplierInfo(); // Displaying supplier information for the first supplier for simplicity
                break;
            case 6:
                cout << "Exiting the program. Thank you!\n";
                break;     
            default:
                cout << "Invalid choice. Please try again.\n";
        }
        cout << "\t\tDo You Want to Continue [Yes / No] ? :";
        cin >> choose;
        cout<<endl<<endl;
    } while (choose == 'y' || choose == 'Y');
}


int main() {
    system("clear");
    SRMMedicalStore medicalStore;
    medicalStore.run();

    return 0;
}
