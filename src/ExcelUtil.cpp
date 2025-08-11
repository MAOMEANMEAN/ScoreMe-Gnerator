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

// Main Excel operations
void ExcelUtils::writeExcel(const std::string& filename, const std::vector<Student>& students) {
    try {
        // Create directory if it doesn't exist
        std::filesystem::path filePath(filename);
        std::filesystem::create_directories(filePath.parent_path());

        xlnt::workbook wb;
        xlnt::worksheet ws = wb.active_sheet();
        ws.title("Student Grades");

        // Write headers
        auto headers = getExcelHeaders();
        for (size_t i = 0; i < headers.size(); ++i) {
            ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(i + 1), 1)).value(headers[i]);
        }

        // Format header row
        formatExcelHeader(ws);

        // Write student data
        for (size_t i = 0; i < students.size(); ++i) {
            writeStudentToExcel(ws, students[i], static_cast<int>(i + 2));
        }

        // Save with error handling
        try {
            wb.save(filename);
            cout << "✅ Excel file '" << filename << "' created successfully!" << endl;
        } catch (const xlnt::exception& e) {
            cerr << "⚠️ Excel save warning: " << e.what() << endl;
            cerr << "📝 Note: Data processing completed, but file save encountered issues." << endl;
        }
    }
    catch (const exception& e) {
        cerr << "❌ Error writing Excel file: " << e.what() << endl;
        
        // Try alternative approach - create a simple CSV instead
        try {
            string csvFilename = filename.substr(0, filename.find_last_of('.')) + "_backup.csv";
            writeCSVBackup(csvFilename, students);
        } catch (...) {
            cerr << "❌ Failed to create backup CSV as well." << endl;
        }
    }
}

std::vector<Student> ExcelUtils::readExcelToVector(const std::string& filename) {
    std::vector<Student> students;
    
    try {
        if (!fileExists(filename)) {
            cerr << "📁 File '" << filename << "' does not exist!" << endl;
            
            // Try to create the file with sample data
            cout << "🔄 Creating sample file with default data..." << endl;
            auto sampleStudents = Student::createSampleData();
            writeExcel(filename, sampleStudents);
            return sampleStudents;
        }

        xlnt::workbook wb;
        wb.load(filename);
        xlnt::worksheet ws = wb.active_sheet();

        // Check if worksheet has data
        if (ws.highest_row() <= 1) {
            cout << "📄 Excel file is empty, creating sample data..." << endl;
            auto sampleStudents = Student::createSampleData();
            writeExcel(filename, sampleStudents);
            return sampleStudents;
        }

        // Skip header row and read data
        for (int rowNum = 2; rowNum <= static_cast<int>(ws.highest_row()); ++rowNum) {
            try {
                Student student = readStudentFromExcel(ws, rowNum);
                students.push_back(student);
            }
            catch (const exception& e) {
                cerr << "⚠️ Error reading row " << rowNum << ": " << e.what() << endl;
                continue;
            }
        }
    }
    catch (const exception& e) {
        cerr << "❌ Error reading Excel file: " << e.what() << endl;
        
        // Return sample data as fallback
        cout << "🔄 Using sample data as fallback..." << endl;
        return Student::createSampleData();
    }

    return students;
}

void ExcelUtils::readExcel(const std::string& filename) {
    auto students = readExcelToVector(filename);
    
    if (students.empty()) {
        cout << "📄 No student data found in the file." << endl;
        return;
    }

    cout << "✅ Successfully read " << students.size() << " students from " << filename << endl;
    MenuUtils::displayTable(students);
}

// Enhanced Excel operations
void ExcelUtils::writeExcelWithTimestamp(const std::string& baseFilename, const std::vector<Student>& students) {
    string timestampFilename = generateTimestampFilename(baseFilename);
    writeExcel(timestampFilename, students);
}

void ExcelUtils::createBackup(const std::string& sourceFilename, const std::vector<Student>& students) {
    try {
        // Create backup directory if it doesn't exist
        std::filesystem::create_directories("data/backups");
        
        string backupFilename = "data/backups/backup_" + generateTimestampFilename(sourceFilename);
        writeExcel(backupFilename, students);
        
        cout << "💾 Backup created: " << backupFilename << endl;
    }
    catch (const exception& e) {
        cerr << "❌ Failed to create backup: " << e.what() << endl;
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

        // Write headers starting from row 8
        auto headers = getExcelHeaders();
        for (size_t i = 0; i < headers.size(); ++i) {
            ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(i + 1), 8)).value(headers[i]);
        }

        // Write student data
        for (size_t i = 0; i < students.size(); ++i) {
            writeStudentToExcel(ws, students[i], static_cast<int>(i + 9));
        }

        try {
            wb.save(filename);
            cout << "📊 Grade report exported to: " << filename << endl;
        } catch (const xlnt::exception& e) {
            cerr << "⚠️ Excel export warning: " << e.what() << endl;
            // Create CSV backup
            string csvFilename = filename.substr(0, filename.find_last_of('.')) + "_report.csv";
            writeCSVReport(csvFilename, students);
        }
    }
    catch (const exception& e) {
        cerr << "❌ Error creating grade report: " << e.what() << endl;
    }
}

// Import operations
bool ExcelUtils::importStudentData(const std::string& filename, std::vector<Student>& students) {
    try {
        if (!fileExists(filename)) {
            cerr << "📁 File '" << filename << "' does not exist!" << endl;
            
            // Create the file with current student data for future imports
            cout << "🔄 Creating Excel file with current data for future use..." << endl;
            writeExcel(filename, students);
            cout << "✅ Excel file created successfully!" << endl;
            return true;
        }

        auto importedStudents = readExcelToVector(filename);
        if (importedStudents.empty()) {
            cerr << "📄 No valid student data found in the file." << endl;
            return false;
        }

        // Clear existing students and replace with imported data
        students.clear();
        students = importedStudents;
        
        cout << "✅ Successfully imported " << importedStudents.size() << " students." << endl;
        cout << "🔄 Updated grade calculations for all students." << endl;
        
        // Ensure all students have updated grades
        for (auto& student : students) {
            student.updateAllGrades();
        }
        
        return true;
    }
    catch (const exception& e) {
        cerr << "❌ Error importing student data: " << e.what() << endl;
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

        // Check if file has expected headers
        auto expectedHeaders = getExcelHeaders();
        for (size_t i = 0; i < expectedHeaders.size() && i < 10; ++i) { // Check first 10 headers
            try {
                auto cell = ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(i + 1), 1));
                string cellValue = cell.to_string();
                if (cellValue != expectedHeaders[i]) {
                    return false;
                }
            }
            catch (...) {
                return false;
            }
        }

        return true;
    }
    catch (...) {
        return false;
    }
}

// Utility methods
std::string ExcelUtils::generateTimestampFilename(const std::string& baseFilename) {
    string timestamp = getCurrentTimestamp();
    
    // Replace spaces and colons with underscores for filename safety
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

// Helper methods for Excel formatting
void ExcelUtils::formatExcelHeader(xlnt::worksheet& ws) {
    auto headers = getExcelHeaders();
    for (size_t i = 0; i < headers.size(); ++i) {
        auto cell = ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(i + 1), 1));
        cell.font(xlnt::font().bold(true));
    }
}

void ExcelUtils::writeStudentToExcel(xlnt::worksheet& ws, const Student& student, int row) {
    int col = 1;
    
    try {
        // Basic information
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
    }
    catch (const exception& e) {
        cerr << "⚠️ Warning writing student to Excel row " << row << ": " << e.what() << endl;
    }
}

Student ExcelUtils::readStudentFromExcel(xlnt::worksheet& ws, int row) {
    int col = 1;
    
    try {
        // Read basic information with safe string conversion
        string studentId = ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(col++), row)).to_string();
        string name = ws.cell(xlnt::cell_reference(static_cast<xlnt::column_t>(col++), row)).to_string();
        
        // Safe integer conversion
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
        
        // Create student object and update grades
        Student student(studentId, name, age, gender, dateOfBirth, email, scores);
        student.updateAllGrades(); // Ensure grades are calculated
        return student;
    }
    catch (const exception& e) {
        throw runtime_error("Error reading student data from row " + to_string(row) + ": " + e.what());
    }
}

// Additional helper methods for CSV backup
void ExcelUtils::writeCSVBackup(const std::string& filename, const std::vector<Student>& students) {
    try {
        ofstream csvFile(filename);
        if (!csvFile.is_open()) {
            throw runtime_error("Cannot create CSV backup file");
        }
        
        // Write headers
        auto headers = getExcelHeaders();
        for (size_t i = 0; i < headers.size(); ++i) {
            csvFile << headers[i];
            if (i < headers.size() - 1) csvFile << ",";
        }
        csvFile << "\n";
        
        // Write student data
        for (const auto& student : students) {
            csvFile << student.getStudentId() << ","
                   << student.getName() << ","
                   << student.getAge() << ","
                   << student.getGender() << ","
                   << student.getDateOfBirth() << ","
                   << student.getEmail();
            
            auto scores = student.getSubjectScores();
            for (const auto& score : scores) {
                csvFile << "," << score;
            }
            
            csvFile << "," << student.getAverageScore()
                   << "," << student.getLetterGrade()
                   << "," << student.getGpa()
                   << "," << student.getRemark()
                   << "," << student.getFormattedTimestamp() << "\n";
        }
        
        csvFile.close();
        cout << "📄 CSV backup created: " << filename << endl;
    }
    catch (const exception& e) {
        cerr << "❌ Failed to create CSV backup: " << e.what() << endl;
    }
}

void ExcelUtils::writeCSVReport(const std::string& filename, const std::vector<Student>& students) {
    try {
        ofstream csvFile(filename);
        if (!csvFile.is_open()) {
            throw runtime_error("Cannot create CSV report file");
        }
        
        // Write report header
        csvFile << "GRADE REPORT - " << getCurrentTimestamp() << "\n\n";
        
        // Write statistics
        int totalStudents = static_cast<int>(students.size());
        int passingStudents = 0;
        double totalAverage = 0.0;
        
        for (const auto& student : students) {
            if (student.hasPassingGrade()) passingStudents++;
            totalAverage += student.getAverageScore();
        }
        
        double classAverage = totalStudents > 0 ? totalAverage / totalStudents : 0.0;
        double passRate = totalStudents > 0 ? (static_cast<double>(passingStudents) / totalStudents) * 100.0 : 0.0;
        
        csvFile << "Total Students," << totalStudents << "\n";
        csvFile << "Passing Students (50+)," << passingStudents << "\n";
        csvFile << "Pass Rate," << passRate << "%\n";
        csvFile << "Class Average," << classAverage << "\n\n";
        
        // Write student data headers
        auto headers = getExcelHeaders();
        for (size_t i = 0; i < headers.size(); ++i) {
            csvFile << headers[i];
            if (i < headers.size() - 1) csvFile << ",";
        }
        csvFile << "\n";
        
        // Write student data
        for (const auto& student : students) {
            csvFile << student.getStudentId() << ","
                   << student.getName() << ","
                   << student.getAge() << ","
                   << student.getGender() << ","
                   << student.getDateOfBirth() << ","
                   << student.getEmail();
            
            auto scores = student.getSubjectScores();
            for (const auto& score : scores) {
                csvFile << "," << score;
            }
            
            csvFile << "," << student.getAverageScore()
                   << "," << student.getLetterGrade()
                   << "," << student.getGpa()
                   << "," << student.getRemark()
                   << "," << student.getFormattedTimestamp() << "\n";
        }
        
        csvFile.close();
        cout << "📊 CSV report created: " << filename << endl;
    }
    catch (const exception& e) {
        cerr << "❌ Failed to create CSV report: " << e.what() << endl;
    }
}