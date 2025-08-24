#include "Admin.hpp"
#include "MenuUtils.hpp"
#include "ExcelUtil.hpp"
#include "GradeUtil.hpp"
#include <iostream>
#include <algorithm>  // Make sure this is included
#include <fstream>
#include <set>

// Add these includes for file dialog
#ifdef _WIN32
    #include <windows.h>
    #include <commdlg.h>
    #include <shlobj.h>
#else
    #include <unistd.h>
    #include <sys/wait.h>
#endif

using namespace std;  // Add this to avoid std:: prefix issues

// Static member definitions
const std::string Admin::DEFAULT_ADMIN_USERNAME = "scoreme.pro";
const std::string Admin::DEFAULT_ADMIN_PASSWORD = "prome@123";

// Constructors
Admin::Admin() : Person(DEFAULT_ADMIN_USERNAME, DEFAULT_ADMIN_PASSWORD, "Administrator") {}

Admin::Admin(const std::string& username, const std::string& password, const std::string& name)
    : Person(username, password, name) {}

// Override virtual methods
bool Admin::login() {
    MenuUtils::printHeader("ADMIN LOGIN");
    
    std::string inputUsername = MenuUtils::getStringInput("Username: ");
    std::string inputPassword = MenuUtils::getHiddenInput("Password: ");
    
    if (validateCredentials(inputUsername, inputPassword)) {
        MenuUtils::printSuccess("Login successful! Welcome, Administrator!");
        return true;
    } else {
        MenuUtils::printError("Invalid admin credentials!");
        return false;
    }
}

void Admin::showMenu() {
    MenuUtils::clearScreen();
    MenuUtils::printHeader("ADMIN DASHBOARD");
    
    std::vector<std::string> adminMenuOptions = {
        "Student Management (Limited)",
        "System Information",
        "Help & Documentation",
        "Sign Out"
    };
    
    MenuUtils::printMenu(adminMenuOptions);
    MenuUtils::printWarning("Note: Limited functionality without student data access.");
    MenuUtils::printInfo("For full admin features, use the main application flow.");
    
    int choice = MenuUtils::getMenuChoice(4);
    
    switch (choice) {
        case 1:
            MenuUtils::printWarning("Student Management requires data access through main application.");
            break;
        case 2:
            MenuUtils::printInfo("ScoreME Generator v1.0");
            MenuUtils::printInfo("Features: Student Management, Excel Import/Export, Grade Reports");
            break;
        case 3:
            MenuUtils::printInfo("Default Admin Credentials:");
            MenuUtils::printInfo("Username: scoreme.pro");
            MenuUtils::printInfo("Password: prome@123");
            break;
        case 4:
            MenuUtils::printInfo("Signing out from admin dashboard...");
            return;
        default:
            MenuUtils::printError("Invalid choice!");
            break;
    }
    
    if (choice != 4) {
        MenuUtils::pauseScreen();
    }
}

// ENHANCED: showMenuWithData with improved import functionality
void Admin::showMenuWithData(std::vector<Student>& students) {
    int choice;
    
    do {
        MenuUtils::clearScreen();
        MenuUtils::printAdminMenu();
        
        // Show current data status
        if (!students.empty()) {
            string studentStatus = "📊 Current database: " + to_string(students.size()) + " students loaded";
            cout << string(25, ' ');
            MenuUtils::printSuccess(studentStatus);
        } else {
            string studentStatus = "📊 No student data loaded - consider importing from Excel file";
            cout << string(15, ' ');
            MenuUtils::printWarning(studentStatus);
        }
        cout << endl;
        
        choice = MenuUtils::getMenuChoice(5);
        
        switch (choice) {
            case 1:
                manageStudents(students);
                break;
            case 2: {
                MenuUtils::clearScreen();  // Clear screen when entering import section
                // Enhanced import with multiple options
                std::vector<std::string> importMainOptions = {
                    "Import Excel File (Browse Computer)",
                    "Show Recent Files",
                    "Back to Admin Menu"
                };
                MenuUtils::printMenu(importMainOptions);
                int importChoice = MenuUtils::getMenuChoice(3);
                switch (importChoice) {
                    case 1:
                        MenuUtils::clearScreen();  // Clear screen before file import
                        importExcelData(students, "data/students.xlsx");
                        break;
                    case 2:
                        MenuUtils::clearScreen();  // Clear screen before showing files
                        showRecentFiles();
                        MenuUtils::pauseScreen();
                        break;
                    case 3:
                        break; // Back to admin menu
                }
                
                if (importChoice != 3) {
                    MenuUtils::pauseScreen();
                }
                break;
            }
            case 3:
                exportData(students, "data/grade_report.xlsx");
                MenuUtils::pauseScreen();
                break;
                
            case 4:
                backupData(students);
                MenuUtils::pauseScreen();
                break;
                
            case 5:
                MenuUtils::printInfo("Signing out from admin dashboard...");
                return;
        }
        
        if (choice != 5) {
            if (!MenuUtils::askContinue()) {
                break;
            }
        }
    } while (choice != 5);
}

std::string Admin::getRole() const {
    return "Administrator";
}

// NEW: Function to reorder all student IDs sequentially
void Admin::reorderStudentIds(std::vector<Student>& students) {
    // Sort students by current ID to maintain some order
    sort(students.begin(), students.end(),
        [](const Student& a, const Student& b) {
            string idA = a.getStudentId();
            string idB = b.getStudentId();
            
            // Extract numeric part and compare
            int numA = 0, numB = 0;
            if (idA.length() >= 6 && idA.substr(0, 3) == "STU") {
                try { numA = stoi(idA.substr(3)); } catch(...) {}
            }
            if (idB.length() >= 6 && idB.substr(0, 3) == "STU") {
                try { numB = stoi(idB.substr(3)); } catch(...) {}
            }
            
            return numA < numB;
        });
    
    // Reassign IDs sequentially starting from STU001
    for (size_t i = 0; i < students.size(); ++i) {
        int newIdNum = static_cast<int>(i + 1);
        string newId = "STU" + string(3 - to_string(newIdNum).length(), '0') + to_string(newIdNum);
        students[i].setStudentId(newId);
    }
}

std::string Admin::generateNextStudentId(const std::vector<Student>& students) {
    int nextId = static_cast<int>(students.size()) + 1;
    
    string nextIdStr = "STU" + string(3 - to_string(nextId).length(), '0') + to_string(nextId);
    return nextIdStr;
}

bool Admin::isValidGmail(const std::string& email) {
    return email.find("gmail.com") != string::npos;
}

void Admin::saveCredentialsToExcel(const std::vector<Student>& students) {
    try {
        ExcelUtils::writeCredentialsExcel("data/student_credentials.xlsx", students);
        MenuUtils::printInfo("Student credentials saved to data/student_credentials.xlsx");
    } catch (const exception& e) {
        MenuUtils::printWarning("Failed to save credentials: " + string(e.what()));
    }
}

std::string Admin::openFileDialog() {
#ifdef _WIN32
    // Windows file dialog
    OPENFILENAMEA ofn;
    char szFile[260] = {0};
    
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Excel Files\0*.xlsx;*.xls\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    ofn.lpstrTitle = "Select Excel File to Import";
    
    if (GetOpenFileNameA(&ofn)) {
        return string(szFile);
    }
    return ""; // User cancelled
    
#else
    // Linux/Mac - use zenity or kdialog if available
    MenuUtils::printInfo("Opening file selection dialog...");
    
    // Try zenity first (GNOME)
    string command = "zenity --file-selection --title='Select Excel File' --file-filter='Excel files (xlsx,xls) | *.xlsx *.xls' 2>/dev/null";
    FILE* pipe = popen(command.c_str(), "r");
    
    if (pipe != nullptr) {
        char buffer[1024];
        string result = "";
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
        pclose(pipe);
        
        // Remove newline from result
        if (!result.empty() && result.back() == '\n') {
            result.pop_back();
        }
        
        if (!result.empty()) {
            return result;
        }
    }
    
    // Try kdialog (KDE)
    command = "kdialog --getopenfilename ~ 'Excel files (*.xlsx *.xls)' 2>/dev/null";
    pipe = popen(command.c_str(), "r");
    
    if (pipe != nullptr) {
        char buffer[1024];
        string result = "";
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
        pclose(pipe);
        
        // Remove newline from result
        if (!result.empty() && result.back() == '\n') {
            result.pop_back();
        }
        
        if (!result.empty()) {
            return result;
        }
    }
    
    MenuUtils::printWarning("File dialog not available. Please enter the file path manually.");
    return MenuUtils::getStringInput("Enter full path to Excel file: ");
#endif
}

void Admin::showRecentFiles() {
    MenuUtils::printHeader("RECENT FILES & LOCATIONS");
    MenuUtils::printInfo("Common Excel file locations:");
    MenuUtils::printInfo("• Desktop: ~/Desktop/ or %USERPROFILE%\\Desktop\\");
    MenuUtils::printInfo("• Downloads: ~/Downloads/ or %USERPROFILE%\\Downloads\\");
    MenuUtils::printInfo("• Documents: ~/Documents/ or %USERPROFILE%\\Documents\\");
    MenuUtils::printInfo("• Current directory: ./");
    MenuUtils::printInfo("\nFiles in data directory:");
    
#ifdef _WIN32
    string command = "dir /b data\\*.xlsx 2>nul";
#else
    string command = "ls -la data/*.xlsx 2>/dev/null || echo 'No Excel files found in data directory'";
#endif
    
    system(command.c_str());
    cout << endl;
}

void Admin::manageStudents(std::vector<Student>& students) {
    int choice;
    do {
        MenuUtils::clearScreen();
        MenuUtils::printHeader("STUDENT MANAGEMENT");
        
        vector<string> studentMenu = {
            "View All Students",
            "Add New Student",
            "Edit Student Info",
            "Delete Student",
            "Search Student",
            "Show Failing Students",
            "Sort Students by Score",
            "Back to Admin Dashboard"
        };
        
        MenuUtils::printMenu(studentMenu);
        choice = MenuUtils::getMenuChoice(8);
        
        switch (choice) {
            case 1:
                viewAllStudents(students);
                break;
            case 2:
                addNewStudent(students);
                break;
            case 3:
                editStudentInfo(students);
                break;
            case 4:
                deleteStudent(students);
                break;
            case 5:
                searchStudent(students);
                break;
            case 6:
                showFailingStudents(students);
                break;
            case 7:
                sortStudentsByScore(students);
                break;
            case 8:
                return;
        }
        
        if (choice != 8) {
            MenuUtils::pauseScreen();
        }
    } while (choice != 8);
}

void Admin::viewAllStudents(const std::vector<Student>& students) {
    MenuUtils::printHeader("ALL STUDENTS");
    if (students.empty()) {
        MenuUtils::printWarning("No students found!");
        return;
    }
    MenuUtils::displayTable(students);
}

void Admin::addNewStudent(std::vector<Student>& students) {
    MenuUtils::printHeader("ADD NEW STUDENT");
    
    string studentId = generateNextStudentId(students);
    MenuUtils::printInfo("Auto-generated Student ID : " + studentId);
    
    string name = MenuUtils::getStringInput("Student Name : ");
    if (isStudentExists(name, students)) {
        MenuUtils::printError("Student with this name already exists!");
        return;
    }
    
    string username = MenuUtils::getStringInput("Username for login : ");
    string password = MenuUtils::getStringInput("Password for login : ");
    
    int age = MenuUtils::getIntInput("Age : ");
    string gender = MenuUtils::getStringInput("Gender : ");
    string dob = MenuUtils::getStringInput("Date of Birth (YYYY-MM-DD) : ");
    
    string email;
    do {
        email = MenuUtils::getStringInput("Gmail : ");
        if (!isValidGmail(email)) {
            MenuUtils::printError("Invalid email! Email must contain 'gmail.com'");
        }
    } while (!isValidGmail(email));
    
    vector<double> scores(7);
    vector<string> subjects = GradeUtil::getSubjectNames();
    
    MenuUtils::printInfo("Enter scores for all subjects :");
    for (size_t i = 0; i < subjects.size(); ++i) {
        scores[i] = MenuUtils::getDoubleInput(subjects[i] + " score : ");
        if (!GradeUtil::isValidScore(scores[i])) {
            MenuUtils::printError("Invalid score! Score must be between 0-100.");
            i--; // Retry current subject
        }
    }
    
    students.emplace_back(username, password, studentId, name, age, gender, dob, email, scores);
    MenuUtils::printSuccess("Student added successfully!");
    MenuUtils::printInfo("Student ID: " + studentId);
    MenuUtils::printInfo("Login credentials - Username : " + username + ", Password : " + password);
    
    try {
        ExcelUtils::writeExcel("data/students.xlsx", students);
        saveCredentialsToExcel(students);
        MenuUtils::printInfo("Data saved to Excel files.");
    } catch (const exception& e) {
        MenuUtils::printWarning("Student added but failed to save to Excel : " + string(e.what()));
    }
}

void Admin::editStudentInfo(std::vector<Student>& students) {
    MenuUtils::printHeader("EDIT STUDENT INFO");
    
    string searchId = MenuUtils::getStringInput("Enter Student ID to edit : ");
    Student* student = findStudentById(students, searchId);
    
    if (!student) {
        MenuUtils::printError("Student not found!");
        return;
    }
    
    MenuUtils::printInfo("Current student info :");
    MenuUtils::displayStudentDetails(*student);
    
    vector<string> editMenu = {
        "Edit Name",
        "Edit Age",
        "Edit Gender",
        "Edit Date of Birth",
        "Edit Email",
        "Edit Scores",
        "Cancel"
    };
    
    MenuUtils::printMenu(editMenu);
    int choice = MenuUtils::getMenuChoice(7);
    
    switch (choice) {
        case 1: {
            string newName = MenuUtils::getStringInput("New name : ");
            student->setName(newName);
            break;
        }
        case 2: {
            int newAge = MenuUtils::getIntInput("New age : ");
            student->setAge(newAge);
            break;
        }
        case 3: {
            string newGender = MenuUtils::getStringInput("New gender : ");
            student->setGender(newGender);
            break;
        }
        case 4: {
            string newDob = MenuUtils::getStringInput("New date of birth (YYYY-MM-DD) : ");
            student->setDateOfBirth(newDob);
            break;
        }
        case 5: {
            string newEmail;
            do {
                newEmail = MenuUtils::getStringInput("New Gmail : ");
                if (!isValidGmail(newEmail)) {
                    MenuUtils::printError("Invalid email! Email must contain 'gmail.com'");
                }
            } while (!isValidGmail(newEmail));
            student->setEmail(newEmail);
            break;
        }
        case 6: {
            vector<double> newScores(7);
            vector<string> subjects = GradeUtil::getSubjectNames();
            
            for (size_t i = 0; i < subjects.size(); ++i) {
                newScores[i] = MenuUtils::getDoubleInput(subjects[i] + " New score : ");
                if (!GradeUtil::isValidScore(newScores[i])) {
                    MenuUtils::printError("Invalid score! Score must be between 0-100.");
                    i--; // Retry current subject
                }
            }
            student->setSubjectScores(newScores);
            break;
        }
        case 7:
            return;
    }
    
    if (choice != 7) {
        MenuUtils::printSuccess("Student information updated successfully!");
        
        try {
            ExcelUtils::writeExcel("data/students.xlsx", students);
            saveCredentialsToExcel(students);
            MenuUtils::printInfo("Data saved to Excel files.");
        } catch (const exception& e) {
            MenuUtils::printWarning("Student updated but failed to save to Excel: " + string(e.what()));
        }
    }
}

void Admin::deleteStudent(std::vector<Student>& students) {
    MenuUtils::printHeader("DELETE STUDENT");
    
    string searchId = MenuUtils::getStringInput("Enter Student ID to delete: ");
    auto it = find_if(students.begin(), students.end(),
        [&searchId](const Student& s) { return s.getStudentId() == searchId; });
    
    if (it != students.end()) {
        MenuUtils::printInfo("Student found:");
        MenuUtils::displayStudentDetails(*it);
        
        string confirm = MenuUtils::getStringInput("Are you sure you want to delete this student? (yes/no): ");
        if (confirm == "yes" || confirm == "y" || confirm == "Y") {
            // Remove the student
            students.erase(it);
            
            reorderStudentIds(students);
            
            MenuUtils::printSuccess("Student deleted successfully!");
            MenuUtils::printInfo("All student IDs have been reordered to maintain sequence.");
            
            if (!students.empty()) {
                MenuUtils::printInfo("Updated student list:");
                MenuUtils::displayTable(students);
            }
            
            try {
                ExcelUtils::writeExcel("data/students.xlsx", students);
                saveCredentialsToExcel(students);
                MenuUtils::printInfo("Data saved to Excel files.");
            } catch (const exception& e) {
                MenuUtils::printWarning("Student deleted but failed to save to Excel: " + string(e.what()));
            }
        } else {
            MenuUtils::printInfo("Deletion cancelled.");
        }
    } else {
        MenuUtils::printError("Student not found!");
    }
}

void Admin::searchStudent(const std::vector<Student>& students) {
    MenuUtils::printHeader("SEARCH STUDENT");
    
    string searchTerm = MenuUtils::getStringInput("Enter Student ID or Name: ");
    bool found = false;
    
    for (const auto& student : students) {
        if (student.getStudentId() == searchTerm || student.getName() == searchTerm) {
            MenuUtils::displayStudentDetails(student);
            found = true;
            break;
        }
    }
    
    if (!found) {
        MenuUtils::printError("Student not found!");
    }
}

void Admin::showFailingStudents(const std::vector<Student>& students) {
    MenuUtils::printHeader("FAILING STUDENTS");
    
    vector<Student> failingStudents;
    for (const auto& student : students) {
        if (!student.hasPassingGrade()) {
            failingStudents.push_back(student);
        }
    }
    
    if (failingStudents.empty()) {
        MenuUtils::printSuccess("No failing students found!");
    } else {
        MenuUtils::displayFailingStudents(failingStudents);
    }
}

void Admin::sortStudentsByScore(std::vector<Student>& students) {
    MenuUtils::printHeader("SORT STUDENTS BY SCORE");
    
    string order = MenuUtils::getStringInput("Sort order (asc/desc): ");
    
    if (order == "asc") {
        sort(students.begin(), students.end(),
            [](const Student& a, const Student& b) {
                return a.getAverageScore() < b.getAverageScore();
            });
    } else {
        sort(students.begin(), students.end(),
            [](const Student& a, const Student& b) {
                return a.getAverageScore() > b.getAverageScore();
            });
    }
    
    // After sorting, reorder IDs to maintain sequence
    reorderStudentIds(students);
    
    MenuUtils::printSuccess("Students sorted successfully!");
    MenuUtils::printInfo("Student IDs have been reordered to maintain sequence.");
    MenuUtils::displayTable(students);
    
    try {
        ExcelUtils::writeExcel("data/students.xlsx", students);
        saveCredentialsToExcel(students);
        MenuUtils::printInfo("Sorted data saved to Excel files.");
    } catch (const exception& e) {
        MenuUtils::printWarning("Students sorted but failed to save to Excel: " + string(e.what()));
    }
}

void Admin::importExcelData(std::vector<Student>& students, const std::string& defaultFilename) {
    MenuUtils::printHeader("IMPORT EXCEL DATA");
    
    vector<string> importOptions = {
        "Browse and Select File from Computer",
        "Use Default File (data/students.xlsx)",
        "Cancel Import"
    };
    
    MenuUtils::printInfo("Import Options:");
    MenuUtils::printMenu(importOptions);
    
    int choice = MenuUtils::getMenuChoice(3);
    string selectedFile;
    
    switch (choice) {
        case 1: {
            // File browser option
            selectedFile = openFileDialog();
            if (selectedFile.empty()) {
                MenuUtils::printWarning("No file selected. Import cancelled.");
                return;
            }
            MenuUtils::printInfo("Selected file: " + selectedFile);
            break;
        }
        case 2: {
            // Default file
            selectedFile = defaultFilename;
            break;
        }
        case 3: {
            MenuUtils::printInfo("Import cancelled.");
            return;
        }
        default: {
            MenuUtils::printError("Invalid choice!");
            return;
        }
    }
    
    // Validate file exists
    if (!ExcelUtils::fileExists(selectedFile)) {
        MenuUtils::printError("File does not exist: " + selectedFile);
        
        // Offer to create sample file
        string createSample = MenuUtils::getStringInput("Would you like to create a sample Excel file? (y/n): ");
        if (createSample == "y" || createSample == "Y") {
            try {
                // Create sample data
                auto sampleStudents = Student::createSampleData();
                ExcelUtils::writeExcel(selectedFile, sampleStudents);
                saveCredentialsToExcel(sampleStudents);
                MenuUtils::printSuccess("Sample Excel file created: " + selectedFile);
                MenuUtils::printInfo("You can now edit this file and re-import it.");
            } catch (const exception& e) {
                MenuUtils::printError("Failed to create sample file: " + string(e.what()));
            }
        }
        return;
    }
    
    try {
        // Show file information
        MenuUtils::printInfo("Importing from: " + selectedFile);
        MenuUtils::showLoadingAnimation("Reading Excel file", 1500);
        
        // Backup current data before import
        if (!students.empty()) {
            string backupChoice = MenuUtils::getStringInput("Backup current data before import? (y/n): ");
            if (backupChoice == "y" || backupChoice == "Y") {
                backupData(students);
            }
        }
        
        // Import the data
        if (ExcelUtils::importStudentData(selectedFile, students)) {
            // After import, reorder IDs to ensure proper sequence
            reorderStudentIds(students);
            
            MenuUtils::printSuccess("Data imported successfully from " + selectedFile + "!");
            MenuUtils::printInfo("Total students imported: " + to_string(students.size()));
            MenuUtils::printInfo("Student IDs have been reordered to maintain sequence.");
            
            // Update all grades after import
            for (auto& student : students) {
                student.updateAllGrades();
            }
            MenuUtils::printInfo("All grades have been recalculated!");
            
            // Save credentials after import
            saveCredentialsToExcel(students);
            
            // Show preview of imported data
            if (!students.empty()) {
                MenuUtils::printInfo("Preview of imported data (first 5 students):");
                vector<Student> preview;
                // FIXED LINE 799 - using safer alternative to std::min
                int previewCount = (students.size() < 5) ? static_cast<int>(students.size()) : 5;
                for (int i = 0; i < previewCount; i++) {
                    preview.push_back(students[i]);
                }
                MenuUtils::displayTable(preview);
                
                if (students.size() > 5) {
                    MenuUtils::printInfo("... and " + to_string(students.size() - 5) + " more students");
                }
            }
            
            // Auto-save to default location
            try {
                ExcelUtils::writeExcel("data/students.xlsx", students);
                MenuUtils::printInfo("Data also saved to default location: data/students.xlsx");
            } catch (...) {
                MenuUtils::printWarning("Could not save to default location.");
            }
            
        } else {
            MenuUtils::printError("Failed to import data from " + selectedFile);
            
            // Show helpful information about Excel format
            MenuUtils::printInfo("Excel file should contain columns:");
            auto headers = ExcelUtils::getExcelHeaders();
            for (size_t i = 0; i < headers.size() && i < 10; i++) {
                MenuUtils::printInfo("  " + to_string(i+1) + ". " + headers[i]);
            }
            MenuUtils::printInfo("Note: Username and Password are stored separately in credentials file");
        }
        
    } catch (const exception& e) {
        MenuUtils::printError("Import error: " + string(e.what()));
        MenuUtils::printInfo("Please check if the Excel file format is correct.");
        
        // Offer format validation
        string validateChoice = MenuUtils::getStringInput("Would you like to validate the file format? (y/n): ");
        if (validateChoice == "y" || validateChoice == "Y") {
            if (ExcelUtils::validateExcelFormat(selectedFile)) {
                MenuUtils::printSuccess("File format appears to be correct.");
            } else {
                MenuUtils::printError("File format validation failed.");
                MenuUtils::printInfo("Please ensure the Excel file has the correct column headers.");
            }
        }
    }
    cout << endl;
}

void Admin::exportData(const std::vector<Student>& students, const std::string& filename) {
    MenuUtils::printHeader("EXPORT DATA");
    
    if (students.empty()) {
        MenuUtils::printWarning("No student data to export!");
        return;
    }
    
    try {
        // Ensure all students have updated grades
        vector<Student> updatedStudents = students;
        for (auto& student : updatedStudents) {
            student.updateAllGrades();
        }
        
        ExcelUtils::exportGradeReport(filename, updatedStudents);
        MenuUtils::printSuccess("Grade report exported successfully to " + filename + "!");
        
        // Also create a regular Excel file
        string regularFilename = "data/students_export.xlsx";
        ExcelUtils::writeExcel(regularFilename, updatedStudents);
        MenuUtils::printInfo("Regular Excel file also created: " + regularFilename);
        
        // Export credentials
        saveCredentialsToExcel(updatedStudents);
        
        // Show summary
        int totalStudents = static_cast<int>(students.size());
        int passingStudents = 0;
        for (const auto& student : students) {
            if (student.hasPassingGrade()) passingStudents++;
        }
        
        MenuUtils::printInfo("Export Summary:");
        MenuUtils::printInfo("- Total students exported: " + to_string(totalStudents));
        MenuUtils::printInfo("- Passing students (50+): " + to_string(passingStudents));
        MenuUtils::printInfo("- Files created: " + filename + ", " + regularFilename + ", student_credentials.xlsx");
        
    } catch (const exception& e) {
        MenuUtils::printError("Failed to export data: " + string(e.what()));
    }
}

void Admin::backupData(const std::vector<Student>& students) {
    MenuUtils::printHeader("BACKUP DATA");
    
    if (students.empty()) {
        MenuUtils::printWarning("No student data to backup!");
        return;
    }
    
    try {
        // Create timestamped backup
        ExcelUtils::createBackup("students.xlsx", students);
        
        // Also create a simple backup in the data folder
        string simpleBackup = "data/students_backup.xlsx";
        ExcelUtils::writeExcel(simpleBackup, students);
        
        // Backup credentials
        saveCredentialsToExcel(students);
        
        MenuUtils::printSuccess("Data backup created successfully!");
        MenuUtils::printInfo("Backup files created in data/backups/ and data/ directories");
        MenuUtils::printInfo("Total students backed up: " + to_string(students.size()));
        
    } catch (const exception& e) {
        MenuUtils::printError("Failed to create backup: " + string(e.what()));
    }
}

// Utility methods
bool Admin::isValidStudentId(const std::string& id, const std::vector<Student>& students) {
    return find_if(students.begin(), students.end(),
        [&id](const Student& s) { return s.getStudentId() == id; }) == students.end();
}

bool Admin::isStudentExists(const std::string& name, const std::vector<Student>& students) {
    return find_if(students.begin(), students.end(),
        [&name](const Student& s) { return s.getName() == name; }) != students.end();
}

Student* Admin::findStudentById(std::vector<Student>& students, const std::string& id) {
    auto it = find_if(students.begin(), students.end(),
        [&id](const Student& s) { return s.getStudentId() == id; });
    return (it != students.end()) ? &(*it) : nullptr;
}

Student* Admin::findStudentByName(std::vector<Student>& students, const std::string& name) {
    auto it = find_if(students.begin(), students.end(),
        [&name](const Student& s) { return s.getName() == name; });
    return (it != students.end()) ? &(*it) : nullptr;
}