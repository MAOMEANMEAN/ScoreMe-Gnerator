#include "ExcelUtil.hpp"
#include "MenuUtils.hpp"
#include "GradeUtil.hpp"
#include "Student.hpp"
#include <xlnt/xlnt.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <filesystem>

using namespace std;

// Main Excel operations (WITHOUT credentials)
void ExcelUtils::writeExcel(const std::string& filename, const std::vector<Student>& students) {
    try {
        // Create directory if it doesn't exist
        std::filesystem::path filePath(filename);
        std::filesystem::create_directories(filePath.parent_path());

        xlnt::workbook wb;
        xlnt::worksheet ws = wb.active_sheet();
        ws.title("Student Grades");

        // Write headers WITHOUT username and password
        auto headers = getExcelHeaders();
        for (size_t i = 0; i < headers.size(); ++i) {
            ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(i + 1), 1)).value(headers[i]);
        }

        // Format header row
        formatExcelHeader(ws);

        // Write student data WITHOUT credentials
        for (size_t i = 0; i < students.size(); ++i) {
            writeStudentToExcel(ws, students[i], static_cast<int>(i + 2));
        }

        // Save Excel file
        wb.save(filename);
        MenuUtils::printSuccess("Excel file '" + filename + "' created successfully!");
        
    } catch (const exception& e) {
        MenuUtils::printError("Error writing Excel file: " + string(e.what()));
        throw; // Re-throw exception for caller to handle
    }
}

// Separate credentials Excel file (ONLY username, password, basic info)
void ExcelUtils::writeCredentialsExcel(const std::string& filename, const std::vector<Student>& students) {
    try {
        // Create directory if it doesn't exist
        std::filesystem::path filePath(filename);
        std::filesystem::create_directories(filePath.parent_path());

        xlnt::workbook wb;
        xlnt::worksheet ws = wb.active_sheet();
        ws.title("Student Credentials");

        // Write headers for credentials ONLY
        auto credHeaders = getCredentialHeaders();
        for (size_t i = 0; i < credHeaders.size(); ++i) {
            ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(i + 1), 1)).value(credHeaders[i]);
        }

        // Format header row
        for (int col = 1; col <= static_cast<int>(credHeaders.size()); ++col) {
            auto cell = ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(col), 1));
            cell.font(xlnt::font().bold(true));
        }

        // Write credential data ONLY
        for (size_t i = 0; i < students.size(); ++i) {
            int row = static_cast<int>(i + 2);
            int col = 1;
            
            ws.cell(xlnt::cell_reference(col++, row)).value(students[i].getStudentId());
            ws.cell(xlnt::cell_reference(col++, row)).value(students[i].getName());
            ws.cell(xlnt::cell_reference(col++, row)).value(students[i].getUsername());
            ws.cell(xlnt::cell_reference(col++, row)).value(students[i].getPassword());
            ws.cell(xlnt::cell_reference(col++, row)).value(students[i].getEmail());
            ws.cell(xlnt::cell_reference(col++, row)).value(students[i].getFormattedTimestamp());
        }

        // Save Excel file
        wb.save(filename);
        MenuUtils::printSuccess("Credentials file '" + filename + "' created successfully!");
        
    } catch (const exception& e) {
        MenuUtils::printError("Error writing credentials Excel file: " + string(e.what()));
        throw; // Re-throw exception for caller to handle
    }
}

std::vector<Student> ExcelUtils::readExcelToVector(const std::string& filename) {
    std::vector<Student> students;
    
    try {
        if (!fileExists(filename)) {
            MenuUtils::printWarning("File '" + filename + "' does not exist!");
            
            // Create the file with sample data
            MenuUtils::printInfo("Creating sample file with default data...");
            auto sampleStudents = Student::createSampleData();
            writeExcel(filename, sampleStudents);
            // Also create credentials file
            writeCredentialsExcel("data/student_credentials.xlsx", sampleStudents);
            return sampleStudents;
        }

        xlnt::workbook wb;
        wb.load(filename);
        xlnt::worksheet ws = wb.active_sheet();

        // Check if worksheet has data
        if (ws.highest_row() <= 1) {
            MenuUtils::printInfo("Excel file is empty, creating sample data...");
            auto sampleStudents = Student::createSampleData();
            writeExcel(filename, sampleStudents);
            // Also create credentials file
            writeCredentialsExcel("data/student_credentials.xlsx", sampleStudents);
            return sampleStudents;
        }

        // Try to load credentials from separate file
        std::map<std::string, std::pair<std::string, std::string>> credentialsMap;
        loadCredentialsFromFile("data/student_credentials.xlsx", credentialsMap);

        // Skip header row and read data
        for (int rowNum = 2; rowNum <= static_cast<int>(ws.highest_row()); ++rowNum) {
            try {
                Student student = readStudentFromExcel(ws, rowNum);
                
                // Set credentials from credentials file if available
                auto credIt = credentialsMap.find(student.getStudentId());
                if (credIt != credentialsMap.end()) {
                    student.setUsername(credIt->second.first);
                    student.setPassword(credIt->second.second);
                }
                
                students.push_back(student);
            } catch (const exception& e) {
                MenuUtils::printWarning("Error reading row " + to_string(rowNum) + ": " + e.what());
                continue;
            }
        }
        
    } catch (const exception& e) {
        MenuUtils::printError("Error reading Excel file: " + string(e.what()));
        
        // Return sample data as fallback
        MenuUtils::printInfo("Using sample data as fallback...");
        return Student::createSampleData();
    }

    return students;
}

void ExcelUtils::readExcel(const std::string& filename) {
    auto students = readExcelToVector(filename);
    
    if (students.empty()) {
        MenuUtils::printWarning("No student data found in the file.");
        return;
    }

    MenuUtils::printSuccess("Successfully read " + to_string(students.size()) + " students from " + filename);
    MenuUtils::displayTable(students);
}

// Enhanced Excel operations
void ExcelUtils::writeExcelWithTimestamp(const std::string& baseFilename, const std::vector<Student>& students) {
    string timestampFilename = generateTimestampFilename(baseFilename);
    writeExcel(timestampFilename, students);
    // Also create credentials file with timestamp
    string credTimestampFilename = generateTimestampFilename("student_credentials.xlsx");
    writeCredentialsExcel("data/" + credTimestampFilename, students);
}

void ExcelUtils::createBackup(const std::string& sourceFilename, const std::vector<Student>& students) {
    try {
        // Create backup directory if it doesn't exist
        std::filesystem::create_directories("data/backups");
        
        string backupFilename = "data/backups/backup_" + generateTimestampFilename(sourceFilename);
        writeExcel(backupFilename, students);
        
        // Also backup credentials
        string credBackupFilename = "data/backups/backup_" + generateTimestampFilename("student_credentials.xlsx");
        writeCredentialsExcel(credBackupFilename, students);
        
        MenuUtils::printSuccess("Backup created: " + backupFilename);
        MenuUtils::printSuccess("Credentials backup created: " + credBackupFilename);
        
    } catch (const exception& e) {
        MenuUtils::printError("Failed to create backup: " + string(e.what()));
        throw;
    }
}

void ExcelUtils::exportGradeReport(const std::string& filename, const std::vector<Student>& students) {
    try {
        // Create directory if it doesn't exist
        std::filesystem::path filePath(filename);
        std::filesystem::create_directories(filePath.parent_path());

        xlnt::workbook wb;
        xlnt::worksheet ws = wb.active_sheet();
        ws.title("Grade Report");

        // Add report title
        ws.cell("A1").value("GRADE REPORT - " + getCurrentTimestamp());
        ws.merge_cells("A1:I1");
        
        // Add summary statistics
        int totalStudents = static_cast<int>(students.size());
        int passingStudents = 0;
        double totalAverage = 0.0;
        
        for (const auto& student : students) {
            if (student.hasPassingGrade()) {
                passingStudents++;
            }
            totalAverage += student.getAverageScore();
        }
        
        double classAverage = totalStudents > 0 ? totalAverage / totalStudents : 0.0;
        double passRate = totalStudents > 0 ? (static_cast<double>(passingStudents) / totalStudents) * 100.0 : 0.0;
        
        ws.cell("A3").value("Total Students: " + to_string(totalStudents));
        ws.cell("A4").value("Passing Students (50+): " + to_string(passingStudents));
        ws.cell("A5").value("Pass Rate: " + to_string(static_cast<int>(passRate * 100) / 100.0) + "%");
        ws.cell("A6").value("Class Average: " + to_string(static_cast<int>(classAverage * 100) / 100.0));

        // Write headers starting from row 8 (WITHOUT credentials)
        auto headers = getExcelHeaders();
        for (size_t i = 0; i < headers.size(); ++i) {
            ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(i + 1), 8)).value(headers[i]);
        }

        // Write student data WITHOUT credentials
        for (size_t i = 0; i < students.size(); ++i) {
            writeStudentToExcel(ws, students[i], static_cast<int>(i + 9));
        }

        wb.save(filename);
        MenuUtils::printSuccess("Grade report exported to: " + filename);
        
    } catch (const exception& e) {
        MenuUtils::printError("Error creating grade report: " + string(e.what()));
        throw;
    }
}

// Import operations
bool ExcelUtils::importStudentData(const std::string& filename, std::vector<Student>& students) {
    try {
        if (!fileExists(filename)) {
            MenuUtils::printWarning("File '" + filename + "' does not exist!");
            
            // Create the file with current student data for future imports
            MenuUtils::printInfo("Creating Excel file with current data for future use...");
            writeExcel(filename, students);
            // Also create credentials file
            writeCredentialsExcel("data/student_credentials.xlsx", students);
            MenuUtils::printSuccess("Excel files created successfully!");
            return true;
        }

        auto importedStudents = readExcelToVector(filename);
        if (importedStudents.empty()) {
            MenuUtils::printWarning("No valid student data found in the file.");
            return false;
        }

        // Clear existing students and replace with imported data
        students.clear();
        students = importedStudents;
        
        MenuUtils::printSuccess("Successfully imported " + to_string(importedStudents.size()) + " students.");
        MenuUtils::printInfo("Updated grade calculations for all students.");
        
        // Ensure all students have updated grades
        for (auto& student : students) {
            student.updateAllGrades();
        }
        
        return true;
        
    } catch (const exception& e) {
        MenuUtils::printError("Error importing student data: " + string(e.what()));
        return false;
    }
}

bool ExcelUtils::validateExcelFormat(const std::string& filename) {
    try {
        if (!fileExists(filename)) {
            return false;
        }

        xlnt::workbook wb;
        wb.load(filename);
        xlnt::worksheet ws = wb.active_sheet();

        auto expectedHeaders = getExcelHeaders();
        for (size_t i = 0; i < expectedHeaders.size() && i < 10; ++i) { // Check first 10 headers
            try {
                auto cell = ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(i + 1), 1));
                string cellValue = cell.to_string();
                if (cellValue != expectedHeaders[i]) {
                    return false;
                }
            } catch (...) {
                return false;
            }
        }

        return true;
        
    } catch (...) {
        return false;
    }
}

// NEW: Helper method to load credentials from separate file
void ExcelUtils::loadCredentialsFromFile(const std::string& credFilename, std::map<std::string, std::pair<std::string, std::string>>& credentialsMap) {
    try {
        if (!fileExists(credFilename)) {
            return; // No credentials file exists
        }

        xlnt::workbook wb;
        wb.load(credFilename);
        xlnt::worksheet ws = wb.active_sheet();

        // Skip header row and read credentials
        for (int rowNum = 2; rowNum <= static_cast<int>(ws.highest_row()); ++rowNum) {
            try {
                string studentId = ws.cell(xlnt::cell_reference(1, rowNum)).to_string();
                string username = ws.cell(xlnt::cell_reference(3, rowNum)).to_string(); // Username is column 3
                string password = ws.cell(xlnt::cell_reference(4, rowNum)).to_string(); // Password is column 4
                
                if (!studentId.empty() && !username.empty() && !password.empty()) {
                    credentialsMap[studentId] = make_pair(username, password);
                }
            } catch (...) {
                // Skip invalid rows
                continue;
            }
        }
        
    } catch (const exception& e) {
        MenuUtils::printWarning("Could not load credentials from " + credFilename + ": " + e.what());
    }
}

// Utility methods
std::string ExcelUtils::generateTimestampFilename(const std::string& baseFilename) {
    string timestamp = getCurrentTimestamp();
    
    for (char& c : timestamp) {
        if (c == ' ' || c == ':') {
            c = '_';
        }
    }
    
    size_t dotPos = baseFilename.find_last_of('.');
    if (dotPos != string::npos) {
        return baseFilename.substr(0, dotPos) + "_" + timestamp + baseFilename.substr(dotPos);
    }
    return baseFilename + "_" + timestamp;
}

std::string ExcelUtils::getCurrentTimestamp() {
    auto now = time(0);
    auto tm = *localtime(&now);
    
    ostringstream oss;
    oss << put_time(&tm, "%Y-%m-%d_%H-%M-%S");
    return oss.str();
}

bool ExcelUtils::fileExists(const std::string& filename) {
    return std::filesystem::exists(filename);
}

// Updated headers WITHOUT username and password (for main Excel files)
std::vector<std::string> ExcelUtils::getExcelHeaders() {
    vector<string> headers = {
        "Student ID", "Name", "Age", "Gender", "Date of Birth", "Email"
    };
    
    // Add subject headers
    auto subjects = GradeUtil::getSubjectNames();
    headers.insert(headers.end(), subjects.begin(), subjects.end());
    
    // Add calculated fields
    headers.insert(headers.end(), {
        "Average Score", "Letter Grade", "GPA", "Remark", "Last Updated"
    });
    
    return headers;
}

// Headers for credentials file ONLY
std::vector<std::string> ExcelUtils::getCredentialHeaders() {
    return {
        "Student ID", "Name", "Username", "Password", "Email", "Created Date"
    };
}

// Helper methods for Excel formatting
void ExcelUtils::formatExcelHeader(xlnt::worksheet& ws) {
    auto headers = getExcelHeaders();
    for (size_t i = 0; i < headers.size(); ++i) {
        auto cell = ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(i + 1), 1));
        cell.font(xlnt::font().bold(true));
    }
}

// Write student data WITHOUT username and password
void ExcelUtils::writeStudentToExcel(xlnt::worksheet& ws, const Student& student, int row) {
    int col = 1;
    
    try {
        // Write basic information (NO username/password)
        ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(col++), row)).value(student.getStudentId());
        ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(col++), row)).value(student.getName());
        ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(col++), row)).value(student.getAge());
        ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(col++), row)).value(student.getGender());
        ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(col++), row)).value(student.getDateOfBirth());
        ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(col++), row)).value(student.getEmail());
        
        // Subject scores
        auto scores = student.getSubjectScores();
        for (const auto& score : scores) {
            ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(col++), row)).value(score);
        }
        
        // Calculated fields
        ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(col++), row)).value(student.getAverageScore());
        ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(col++), row)).value(student.getLetterGrade());
        ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(col++), row)).value(student.getGpa());
        ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(col++), row)).value(student.getRemark());
        ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(col++), row)).value(student.getFormattedTimestamp());
        
    } catch (const exception& e) {
        MenuUtils::printWarning("Warning writing student to Excel row " + to_string(row) + ": " + e.what());
    }
}

// Read student data (credentials will be empty from main Excel files)
Student ExcelUtils::readStudentFromExcel(xlnt::worksheet& ws, int row) {
    int col = 1;
    
    try {
        // Note: No username/password in main Excel files, so they'll be empty initially
        string username = "";  // Will be set later from credentials file
        string password = "";  // Will be set later from credentials file
        
        // Read basic information
        string studentId = ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(col++), row)).to_string();
        string name = ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(col++), row)).to_string();
        
        int age = 20; // default
        try {
            age = static_cast<int>(ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(col), row)).value<double>());
        } catch (...) {
            age = 20;
        }
        col++;
        
        string gender = ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(col++), row)).to_string();
        string dateOfBirth = ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(col++), row)).to_string();
        string email = ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(col++), row)).to_string();
        
        // Read subject scores with error handling
        vector<double> scores;
        auto subjects = GradeUtil::getSubjectNames();
        for (size_t i = 0; i < subjects.size(); ++i) {
            try {
                double score = ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(col++), row)).value<double>();
                scores.push_back(score);
            } catch (...) {
                scores.push_back(0.0); // default score
            }
        }
        
        // Create student WITHOUT credentials (they'll be set from credentials file)
        Student student(username, password, studentId, name, age, gender, dateOfBirth, email, scores);
        student.updateAllGrades(); // Ensure grades are calculated
        return student;
        
    } catch (const exception& e) {
        throw runtime_error("Error reading student data from row " + to_string(row) + ": " + e.what());
    }
}