#pragma once

#include "Person.hpp"
#include "Student.hpp"
#include <vector>

class Admin : public Person {
private:
    static const std::string DEFAULT_ADMIN_USERNAME;
    static const std::string DEFAULT_ADMIN_PASSWORD;

public:
    // Constructors
    Admin();
    Admin(const std::string& username, const std::string& password, const std::string& name);
    
    // Override virtual methods from Person
    bool login() override;
    void showMenu() override;
    std::string getRole() const override;
    
    // Admin-specific methods
    void showMenuWithData(std::vector<Student>& students);
    void manageStudents(std::vector<Student>& students);
    void viewAllStudents(const std::vector<Student>& students);
    void addNewStudent(std::vector<Student>& students);
    void editStudentInfo(std::vector<Student>& students);
    void deleteStudent(std::vector<Student>& students);
    void searchStudent(const std::vector<Student>& students);
    void showFailingStudents(const std::vector<Student>& students);
    void sortStudentsByScore(std::vector<Student>& students);
    
    // ENHANCED: Data management methods with file dialog support
    void importExcelData(std::vector<Student>& students, const std::string& filename);
    void exportData(const std::vector<Student>& students, const std::string& filename);
    void backupData(const std::vector<Student>& students);
    
    // NEW: File dialog and template methods
    std::string openFileDialog();
    void showRecentFiles();
    void createImportTemplate();
    
    // Enhanced functionality methods
    std::string generateNextStudentId(const std::vector<Student>& students);
    bool isValidGmail(const std::string& email);
    void saveCredentialsToExcel(const std::vector<Student>& students);
    void reorderStudentIds(std::vector<Student>& students);
    
    // Utility methods
    bool isValidStudentId(const std::string& id, const std::vector<Student>& students);
    bool isStudentExists(const std::string& name, const std::vector<Student>& students);
    Student* findStudentById(std::vector<Student>& students, const std::string& id);
    Student* findStudentByName(std::vector<Student>& students, const std::string& name);
};