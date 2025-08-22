#pragma once
#include <string>
#include <vector>
#include <map>
#include "Student.hpp"

// Forward declaration to avoid circular dependency
namespace xlnt {
    class worksheet;
}

class ExcelUtils {
public:
    // Main Excel operations
    static void writeExcel(const std::string& filename, const std::vector<Student>& students);
    static std::vector<Student> readExcelToVector(const std::string& filename);
    static void readExcel(const std::string& filename);
    
    // NEW: Credentials Excel operations (separate file for credentials only)
    static void writeCredentialsExcel(const std::string& filename, const std::vector<Student>& students);
    
    // Enhanced Excel operations
    static void writeExcelWithTimestamp(const std::string& baseFilename, const std::vector<Student>& students);
    static void createBackup(const std::string& sourceFilename, const std::vector<Student>& students);
    static void exportGradeReport(const std::string& filename, const std::vector<Student>& students);
    
    // Import operations
    static bool importStudentData(const std::string& filename, std::vector<Student>& students);
    static bool validateExcelFormat(const std::string& filename);
    
    // Utility methods
    static std::string generateTimestampFilename(const std::string& baseFilename);
    static std::string getCurrentTimestamp();
    static bool fileExists(const std::string& filename);
    
    // Excel column headers (two versions: with and without credentials)
    static std::vector<std::string> getExcelHeaders(); // Without credentials
    static std::vector<std::string> getCredentialHeaders(); // Only for credentials file
    
    // NEW: Helper method to load credentials from separate file
    static void loadCredentialsFromFile(const std::string& credFilename, std::map<std::string, std::pair<std::string, std::string>>& credentialsMap);
    
private:
    // Helper methods for Excel formatting
    static void formatExcelHeader(xlnt::worksheet& ws);
    static void writeStudentToExcel(xlnt::worksheet& ws, const Student& student, int row);
    static Student readStudentFromExcel(xlnt::worksheet& ws, int row);
};