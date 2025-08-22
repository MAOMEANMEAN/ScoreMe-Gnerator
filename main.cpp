#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include "Student.hpp"
#include "Admin.hpp"
#include "Person.hpp"
#include "ExcelUtil.hpp"
#include "MenuUtils.hpp"

using namespace std;

class ScoreMEApp {
private:
    vector<Student> registeredStudents;
    Admin admin;
    
public:
    ScoreMEApp() {
        loadExistingDataOrCreateSample();
    }
    
    void loadExistingDataOrCreateSample() {
        try {
            // First, try to load existing Excel data
            if (ExcelUtils::fileExists("data/students.xlsx")) {
                MenuUtils::printInfo("Loading existing student data...");
                registeredStudents = ExcelUtils::readExcelToVector("data/students.xlsx");
                
                setupMissingStudentLoginCredentials();
                MenuUtils::printSuccess("Loaded " + to_string(registeredStudents.size()) + " students from Excel file!");
            } else {
                MenuUtils::printInfo("No existing data found. Creating sample data...");
                initializeStudentAccounts();
                createSampleExcelFiles();
            }
        } catch (const exception& e) {
            MenuUtils::printError("Error loading data: " + string(e.what()));
            MenuUtils::printInfo("Creating fresh sample data...");
            initializeStudentAccounts();
        }
    }
    
    void setupMissingStudentLoginCredentials() {
        bool hasChanges = false;
        
        for (auto& student : registeredStudents) {
            string currentUsername = student.getUsername();
            string currentPassword = student.getPassword();
            if (currentUsername.empty() && currentPassword.empty()) {
                string name = student.getName();
                
                // Only set default credentials for known sample students WITHOUT any credentials
                if (name == "Theara Lavy") {
                    student.setUsername("theara lavy");
                    student.setPassword("lavy123");
                    hasChanges = true;
                    MenuUtils::printInfo("Set default login for " + name);
                } else if (name == "Sarun Lisa") {
                    student.setUsername("sarun lisa");
                    student.setPassword("lisa123");
                    hasChanges = true;
                    MenuUtils::printInfo("Set default login for " + name);
                } else if (name == "Nai Sophanny") {
                    student.setUsername("nai sophanny");
                    student.setPassword("sophanny123");
                    hasChanges = true;
                    MenuUtils::printInfo("Set default login for " + name);
                } else if (name == "Phong Nodiya") {
                    student.setUsername("phong nodiya");
                    student.setPassword("nodiya123");
                    hasChanges = true;
                    MenuUtils::printInfo("Set default login for " + name);
                } else if (name == "Sarah Davis") {
                    student.setUsername("sarah davis");
                    student.setPassword("sarah123");
                    hasChanges = true;
                    MenuUtils::printInfo("Set default login for " + name);
                } else if (name == "Chan Dara") {
                    student.setUsername("chan dara");
                    student.setPassword("dara123");
                    hasChanges = true;
                    MenuUtils::printInfo("Set default login for " + name);
                } else if (name == "Nat Sitha") {
                    student.setUsername("nat sitha");
                    student.setPassword("sitha123");
                    hasChanges = true;
                    MenuUtils::printInfo("Set default login for " + name);
                } else if (name == "Tep Thida") {
                    student.setUsername("tep thida");
                    student.setPassword("thida123");
                    hasChanges = true;
                    MenuUtils::printInfo("Set default login for " + name);
                } else if (name == "Rong Ravuth") {
                    student.setUsername("rong ravuth");
                    student.setPassword("ravuth123");
                    hasChanges = true;
                    MenuUtils::printInfo("Set default login for " + name);
                } else if (name == "Phy Sovanna") {
                    student.setUsername("phy sovanna");
                    student.setPassword("sovanna123");
                    hasChanges = true;
                    MenuUtils::printInfo("Set default login for " + name);
                } else {
                    string firstName = name.substr(0, name.find(' '));
                    if (firstName.empty()) firstName = name;
                    transform(firstName.begin(), firstName.end(), firstName.begin(), ::tolower);
                    student.setUsername(firstName);
                    student.setPassword(firstName + "123");
                    hasChanges = true;
                    
                    MenuUtils::printInfo("Auto-generated login for " + name + " - Username: " + firstName + ", Password: " + firstName + "123");
                }
            } else {
                MenuUtils::printInfo("Preserving existing credentials for " + student.getName() + " (Username: " + currentUsername + ")");
            }
        }
        
        // Save changes only if we set new credentials for students who had none
        if (hasChanges) {
            try {
                // Save main data WITHOUT credentials
                ExcelUtils::writeExcel("data/students.xlsx", registeredStudents);
                // Save credentials SEPARATELY
                ExcelUtils::writeCredentialsExcel("data/student_credentials.xlsx", registeredStudents);
                MenuUtils::printInfo("Updated login credentials saved separately (admin-set credentials preserved).");
            } catch (const exception& e) {
                MenuUtils::printWarning("Could not save credential updates: " + string(e.what()));
            }
        } else {
            MenuUtils::printInfo("All students already have credentials - no changes made.");
        }
    }
    
    void initializeStudentAccounts() {
        // Create sample student data only when no existing data & Set login credentials for sample students only
        registeredStudents = Student::createSampleData();
        setupMissingStudentLoginCredentials();
    }
    
    void createSampleExcelFiles() {
        try {
            // Create sample Excel files with student data (WITHOUT credentials in main files)
            ExcelUtils::writeExcel("data/students.xlsx", registeredStudents);
            ExcelUtils::writeExcel("data/persons.xlsx", registeredStudents);
            
            // Create separate credentials file
            ExcelUtils::writeCredentialsExcel("data/student_credentials.xlsx", registeredStudents);
            
            MenuUtils::printSuccess("Sample Excel files created successfully!");
            MenuUtils::printInfo("Main data saved to: data/students.xlsx and data/persons.xlsx");
            MenuUtils::printInfo("Login credentials saved to: data/student_credentials.xlsx");
        } catch (const exception& e) {
            MenuUtils::printError("Error creating sample Excel files: " + string(e.what()));
        }
    }
    
    void saveDataOnExit() {
        try {
            // Save main data WITHOUT credentials
            ExcelUtils::writeExcel("data/students.xlsx", registeredStudents);
            // Save credentials separately
            ExcelUtils::writeCredentialsExcel("data/student_credentials.xlsx", registeredStudents);
            MenuUtils::printSuccess("All data saved successfully!");
            MenuUtils::printInfo("Student data saved to: data/students.xlsx");
            MenuUtils::printInfo("Login credentials saved to: data/student_credentials.xlsx");
        } catch (const exception& e) {
            MenuUtils::printError("Error saving data: " + string(e.what()));
        }
    }
    
    void run() {
        MenuUtils::printWelcome();
        
        int choice;
        do {
            MenuUtils::clearScreen();
            MenuUtils::printMainMenu();
            choice = MenuUtils::getMenuChoice(3);
            
            switch (choice) {
                case 1:
                    handleAdminLogin();
                    break;
                case 2:
                    handleStudentLogin();
                    break;
                case 3:
                    // Save data before exit
                    MenuUtils::printInfo("Saving all data...");
                    saveDataOnExit();
                    MenuUtils::printThankYou();
                    break;
            }
            
            if (choice != 3) {
                MenuUtils::pauseScreen();
            }
            
        } while (choice != 3);
    }
    
private:
    void handleAdminLogin() {
        MenuUtils::clearScreen();
        
        if (admin.login()) {
            MenuUtils::showLoadingAnimation("Loading Admin Dashboard", 2000);
            admin.showMenuWithData(registeredStudents);
        } else {
            MenuUtils::printError("Admin login failed!");
        }
    }
    
    void handleStudentLogin() {
        MenuUtils::clearScreen();
        MenuUtils::printHeader("STUDENT LOGIN");
        
        string username = MenuUtils::getStringInput("Username: ");
        string password = MenuUtils::getHiddenInput("Password: ");
        
        // Find student with matching credentials
        Student* loggedInStudent = nullptr;
        for (auto& student : registeredStudents) {
            if (student.validateCredentials(username, password)) {
                loggedInStudent = &student;
                break;
            }
        }
        
        if (loggedInStudent) {
            MenuUtils::printSuccess("Login successful! Welcome, " + loggedInStudent->getName() + "!");
            MenuUtils::pauseScreen();
            
            // Show loading animation before entering student dashboard
            MenuUtils::showLoadingAnimation("Loading Student Dashboard", 2000);
            
            loggedInStudent->showMenu();
        } else {
            MenuUtils::printError("Invalid student credentials!");
            MenuUtils::printError("Please check your username and password.");
            MenuUtils::printInfo("If you forgot your credentials, please contact the administrator.");
            MenuUtils::printInfo("\nDEBUG: Available student accounts:");
            for (const auto& student : registeredStudents) {
                if (!student.getUsername().empty()) {
                    MenuUtils::printInfo("Student: " + student.getName() + " | Username: " + student.getUsername());
                }
            }
        }
    }
};

// Function to create sample data when called with command line argument
void createSampleDataFiles() {
    try {
        auto students = Student::createSampleData();
        
        // Create main Excel files WITHOUT credentials
        ExcelUtils::writeExcel("data/students.xlsx", students);
        ExcelUtils::writeExcel("data/persons.xlsx", students);
        
        // Create separate credentials file
        ExcelUtils::writeCredentialsExcel("data/student_credentials.xlsx", students);
        
        MenuUtils::printSuccess("Sample Excel files created successfully in data/ directory!");
        MenuUtils::printInfo("Main data files: data/students.xlsx, data/persons.xlsx");
        MenuUtils::printInfo("Credentials file: data/student_credentials.xlsx");
    } catch (const exception& e) {
        MenuUtils::printError("Error creating sample files: " + string(e.what()));
    }
}

int main(int argc, char* argv[]) {
    try {
        // Check if we're being called to create sample data
        if (argc > 1 && string(argv[1]) == "--create-sample-data") {
            createSampleDataFiles();
            return 0;
        }
        
        ScoreMEApp app;
        app.run();
    } catch (const exception& e) {
        MenuUtils::printError("An error occurred: " + string(e.what()));
        return 1;
    }
    
    return 0;
}