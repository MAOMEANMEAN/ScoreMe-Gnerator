#include "MenuUtils.hpp"
#include "GradeUtil.hpp"
#include "ExcelUtil.hpp" // Added for import instructions
#include <tabulate/table.hpp>
#include <iostream>
#include <iomanip>
#include <limits>
#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

using namespace std;
using namespace tabulate;

// Color codes
const std::string MenuUtils::RESET = "\033[0m";
const std::string MenuUtils::RED = "\033[31m";
const std::string MenuUtils::GREEN = "\033[32m";
const std::string MenuUtils::YELLOW = "\033[33m";
const std::string MenuUtils::BLUE = "\033[34m";
const std::string MenuUtils::MAGENTA = "\033[35m";
const std::string MenuUtils::CYAN = "\033[36m";
const std::string MenuUtils::WHITE = "\033[37m";
const std::string MenuUtils::BOLD = "\033[1m";

// ADDED: Helper function to get grade color
Color MenuUtils::getGradeColor(const std::string& grade) {
    if (grade == "A" || grade == "B" || grade == "C") {
        return Color::green;    // A-C: Green (excellent)
    }
    else if (grade == "D" || grade == "E") {
        return Color::yellow;   // D-E: Orange/Yellow (acceptable)  
    }
    else if (grade == "F") {
        return Color::red;      // F: Red (failing)
    }
    return Color::white;        // Default
}

// Enhanced loading animation
void MenuUtils::showLoadingAnimation(const std::string& message, int duration) {
    clearScreen();
    
    // Center the loading message vertically
    for (int i = 0; i < 10; i++) cout << "\n";
    
    // Center horizontally and show message
    cout << string(25, ' ') << GREEN << message << RESET << endl;
    cout << string(25, ' ') << "Loading: [";
    
    // Progress bar animation
    for (int i = 0; i <= 20; i++) {
        if (i <= 20) {
            cout << GREEN << "█" << RESET;
        }
        cout.flush();
        
        #ifdef _WIN32
            Sleep(duration / 20);
        #else
            usleep((duration * 1000) / 20);
        #endif
    }
    
    cout << "] " << GREEN << "100%" << RESET << endl;
    
    // Success message
    cout << endl << string(20, ' ') << GREEN << "✅ Login Successful! ✅" << RESET << endl;
    
    #ifdef _WIN32
        Sleep(1500);
    #else
        usleep(1500000);
    #endif
}

// Enhanced Welcome with USER PORTAL style
void MenuUtils::printWelcome() {
    system("cls");
    
    // Center spacing for console (adjust if needed)
    cout << string(10, '\n'); // Top spacing to center vertically
    
    // ISTAD PRE-GEN4 with character-by-character animation
    string text = R"(  ██╗███████╗████████╗ █████╗ ██████╗     ██████╗ ██████╗ ███████╗      ██████╗ ███████╗███╗   ██╗██╗  ██╗
  ██║██╔════╝╚══██╔══╝██╔══██╗██╔══██╗    ██╔══██╗██╔══██╗██╔════╝     ██╔════╝ ██╔════╝████╗  ██║██║  ██║
  ██║███████╗   ██║   ███████║██║  ██║    ██████╔╝██████╔╝█████╗       ██║  ███╗█████╗  ██╔██╗ ██║███████║
  ██║╚════██║   ██║   ██╔══██║██║  ██║    ██╔═══╝ ██╔══██╗██╔══╝       ██║   ██║██╔══╝  ██║╚██╗██║╚════██║
  ██║███████║   ██║   ██║  ██║██████╔╝    ██║     ██║  ██║███████╗     ╚██████╔╝███████╗██║ ╚████║     ██║
  ╚═╝╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═════╝     ╚═╝     ╚═╝  ╚═╝╚══════╝      ╚═════╝ ╚══════╝╚═╝  ╚═══╝     ╚═╝)";

    // Add leading spaces to center horizontally
    cout << "        "; // Adjust spacing as needed
    
    cout << "\033[94m"; // Blue color
    
    // REALLY FAST animation - NO DELAYS AT ALL!
    for (int i = 0; i < text.length(); i++) {
        if (text[i] == '\n') {
            cout << text[i];
            cout << "        "; // Add spacing for next line
            // NO DELAY FOR LINE BREAKS - INSTANT!
        } else {
            cout << text[i];
            cout.flush();
            // NO DELAYS FOR ANY CHARACTERS - REALLY FAST!
        }
    }
    cout << "\033[0m" << endl;
    cout << endl;

    // Centered loading animation
    cout << "                                               "; // Center the loading text
    cout << RED << "Loading";
    
    for (int i = 0; i < 3; i++) {
        #ifdef _WIN32
            Sleep(400);  // Windows Sleep function (milliseconds)
        #else
            system("sleep 1");
        #endif
        cout << ".";
        cout.flush();
    }
    cout << RESET << endl;

    // Wait for 2 seconds
    #ifdef _WIN32
        Sleep(2000);
    #else
        system("sleep 2");
    #endif

    #ifdef _WIN32
        Sleep(2000);
    #else
        usleep(2000);
    #endif

    system("cls");

    // Simple welcome message with hash borders (no animation) - CENTERED
    cout << GREEN << endl;
    cout << "                            #############################################################################" << endl;
    cout << "                            #                                                                           #" << endl;
    cout << "                            #                      WELCOME TO SCOREME GENERATOR                         #" << endl;
    cout << "                            #                                                                           #" << endl;
    cout << "                            #                   Student Grade Management System                         #" << endl;
    cout << "                            #                                                                           #" << endl;
    cout << "                            #############################################################################" << endl;
    cout << RESET << endl;

    // Small delay before developer credit
    #ifdef _WIN32
        Sleep(1000);
    #else
        usleep(1000000);
    #endif

    // Animated developer credit - CENTERED
    cout << YELLOW << "                                           ";
    string developer = "Developed by ISTAD Pre-Gen4 GroupI";
    for (int i = 0; i < developer.length(); i++) {
        cout << developer[i];
        cout.flush();
        #ifdef _WIN32
            Sleep(40);
        #else
            usleep(40000);
        #endif
    }
    cout << RESET << endl;
    cout << endl;

    #ifdef _WIN32
        Sleep(2000);
    #else
        usleep(2000000);
    #endif
}

// Enhanced Main Menu with cyberpunk/sci-fi aesthetic
void MenuUtils::printMainMenu() {
    clearScreen();
    
    // Add some top spacing
    cout << string(6, '\n');
    
    // Main Menu header with double lines
    cout << string(10, ' ') << CYAN << "══════════════════════════════════════════════════════════════════════════════════════════════════════════" << RESET << endl;
    
    // ASCII art "MAIN MENU" with proper centering
    cout << MAGENTA;
    cout << "                   $$\\      $$\\  $$$$$$\\  $$$$$$\\ $$\\   $$\\       $$\\      $$\\ $$$$$$$$\\ $$\\   $$\\ $$\\   $$\\ " << endl;
    cout << "                   $$$\\    $$$ |$$  __$$\\ \\_$$  _|$$$\\  $$ |      $$$\\    $$$ |$$  _____|$$$\\  $$ |$$ |  $$ |" << endl;
    cout << "                   $$$$\\  $$$$ |$$ /  $$ |  $$ |  $$$$\\ $$ |      $$$$\\  $$$$ |$$ |      $$$$\\ $$ |$$ |  $$ |" << endl;
    cout << "                   $$\\$$\\$$ $$ |$$$$$$$$ |  $$ |  $$ $$\\$$ |      $$\\$$\\$$ $$ |$$$$$\\    $$ $$\\$$ |$$ |  $$ |" << endl;
    cout << "                   $$ \\$$$  $$ |$$  __$$ |  $$ |  $$ \\$$$$ |      $$ \\$$$  $$ |$$  __|   $$ \\$$$$ |$$ |  $$ |" << endl;
    cout << "                   $$ |\\$  /$$ |$$ |  $$ |  $$ |  $$ |\\$$$ |      $$ |\\$  /$$ |$$ |      $$ |\\$$$ |$$ |  $$ |" << endl;
    cout << "                   $$ | \\_/ $$ |$$ |  $$ |$$$$$$\\ $$ | \\$$ |      $$ | \\_/ $$ |$$$$$$$$\\ $$ | \\$$ |\\$$$$$$  |" << endl;
    cout << "                   \\__|     \\__|\\__|  \\__|\\______|\\__|  \\__|      \\__|     \\__|\\________|\\__|  \\__| \\______/ " << endl;
    cout << RESET;
    
    cout << string(10, ' ') << CYAN << "══════════════════════════════════════════════════════════════════════════════════════════════════════════" << RESET << endl;
    cout << endl;
    cout << endl;
    
      // Calculate center position for the menu box (64 characters wide)
    int consoleWidth = 120; // Adjust this to your console width
    int menuBoxWidth = 64;
    int centerOffset = (consoleWidth - menuBoxWidth) / 2;
    
    // Enhanced menu table with cyberpunk styling and colors - FIXED LAYOUT
    cout << string(centerOffset, ' ') << CYAN << "╭────────────────────────────────────────────────────────────────╮" << RESET << endl;
    cout << string(centerOffset, ' ') << CYAN << "│" << RESET << string(27, ' ') << BOLD << WHITE << "Menu Options" << RESET << string(25, ' ') << CYAN << "│" << RESET << endl;
    cout << string(centerOffset, ' ') << CYAN << "├─────┬──────────────────────────────────────────────────────────┤" << RESET << endl;
    cout << string(centerOffset, ' ') << CYAN << "│ " << BOLD << WHITE << "No" << RESET << "  " << CYAN << "│" << RESET << string(4, ' ') << BOLD << WHITE << "Options" << RESET << string(47, ' ') << CYAN << "│" << RESET << endl;
    cout << string(centerOffset, ' ') << CYAN << "╞═════╪══════════════════════════════════════════════════════════╡" << RESET << endl;
    
    // Admin Login with enhanced styling
    cout << string(centerOffset, ' ') << CYAN << "│ " << YELLOW << "1" << RESET << "   " << CYAN << "│ " << RESET;
    cout << GREEN << "🔐" << RESET << " " << GREEN << "Admin Login" << RESET;
    cout << string(43, ' ') << CYAN << "│" << RESET << endl;
    
    cout << string(centerOffset, ' ') << CYAN << "├─────┼──────────────────────────────────────────────────────────┤" << RESET << endl;
    
    // Student Login
    cout << string(centerOffset, ' ') << CYAN << "│ " << YELLOW << "2" << RESET << "   " << CYAN << "│ " << RESET;
    cout << BLUE << "👨‍🎓" << RESET << " " << BLUE << "Student Login" << RESET;
    cout << string(39, ' ') << CYAN << "│" << RESET << endl;
    
    cout << string(centerOffset, ' ') << CYAN << "├─────┼──────────────────────────────────────────────────────────┤" << RESET << endl;
    
    // Exit option
    cout << string(centerOffset, ' ') << CYAN << "│ " << YELLOW << "3" << RESET << "   " << CYAN << "│ " << RESET;
    cout << RED << "🚪" << RESET << " " << RED << "Exit" << RESET;
    cout << string(50, ' ') << CYAN << "│" << RESET << endl;
    
    cout << string(centerOffset, ' ') << CYAN << "╰─────┴──────────────────────────────────────────────────────────╯" << RESET << endl;
    cout << endl;
    
    // Add some cyberpunk flair at the bottom
    cout << string(35, ' ') << MAGENTA << "▼" << RESET << " " << CYAN << "System Ready" << RESET << " " << MAGENTA << "▼" << RESET << endl;
    cout << endl;
}

// Enhanced Admin Dashboard
void MenuUtils::printAdminDashboard() {
    clearScreen();
    
    // Admin Dashboard ASCII Art
    cout << string(3, '\n');
    cout << MAGENTA << endl;
    cout << "            █████╗ ██████╗ ███╗   ███╗██╗███╗   ██╗    ██████╗  █████╗ ███████╗██╗  ██╗██████╗  ██████╗  █████╗ ██████╗ ██████╗ " << endl;
    cout << "           ██╔══██╗██╔══██╗████╗ ████║██║████╗  ██║    ██╔══██╗██╔══██╗██╔════╝██║  ██║██╔══██╗██╔═══██╗██╔══██╗██╔══██╗██╔══██╗" << endl;
    cout << "           ███████║██║  ██║██╔████╔██║██║██╔██╗ ██║    ██║  ██║███████║███████╗███████║██████╔╝██║   ██║███████║██████╔╝██║  ██║" << endl;
    cout << "           ██╔══██║██║  ██║██║╚██╔╝██║██║██║╚██╗██║    ██║  ██║██╔══██║╚════██║██╔══██║██╔══██╗██║   ██║██╔══██║██╔══██╗██║  ██║" << endl;
    cout << "           ██║  ██║██████╔╝██║ ╚═╝ ██║██║██║ ╚████║    ██████╔╝██║  ██║███████║██║  ██║██████╔╝╚██████╔╝██║  ██║██║  ██║██████╔╝" << endl;
    cout << "           ╚═╝  ╚═╝╚═════╝ ╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝    ╚═════╝ ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═════╝  ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝ " << endl;
    cout << RESET << endl;
}

// Enhanced Student Dashboard
void MenuUtils::printStudentDashboard() {
    clearScreen();
    
    // STU DASHBOARD ASCII Art
    cout << string(3, '\n');
    cout << BLUE << endl;
    cout << "          ███████╗████████╗██╗   ██╗    ██████╗  █████╗ ███████╗██╗  ██╗██████╗  ██████╗  █████╗ ██████╗ ██████╗ " << endl;
    cout << "          ██╔════╝╚══██╔══╝██║   ██║    ██╔══██╗██╔══██╗██╔════╝██║  ██║██╔══██╗██╔═══██╗██╔══██╗██╔══██╗██╔══██╗" << endl;
    cout << "          ███████╗   ██║   ██║   ██║    ██║  ██║███████║███████╗███████║██████╔╝██║   ██║███████║██████╔╝██║  ██║" << endl;
    cout << "          ╚════██║   ██║   ██║   ██║    ██║  ██║██╔══██║╚════██║██╔══██║██╔══██╗██║   ██║██╔══██║██╔══██╗██║  ██║" << endl;
    cout << "          ███████║   ██║   ╚██████╔╝    ██████╔╝██║  ██║███████║██║  ██║██████╔╝╚██████╔╝██║  ██║██║  ██║██████╔╝" << endl;
    cout << "          ╚══════╝   ╚═╝    ╚═════╝     ╚═════╝ ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═════╝  ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝ " << endl;
    cout << RESET << endl;
}

void MenuUtils::printAdminMenu() {
    printAdminDashboard();
    
    int consoleWidth = 120; // Adjust this to your console width
    int menuBoxWidth = 64;
    int centerOffset = (consoleWidth - menuBoxWidth) / 2;
    
    // Enhanced menu table with cyberpunk styling and colors - FIXED LAYOUT
    cout << string(centerOffset, ' ') << CYAN << "╭────────────────────────────────────────────────────────────────╮" << RESET << endl;
    cout << string(centerOffset, ' ') << CYAN << "│" << RESET << string(28, ' ') << BOLD << WHITE << "Admin Options" << RESET << string(23, ' ') << CYAN << "│" << RESET << endl;
    cout << string(centerOffset, ' ') << CYAN << "├─────┬──────────────────────────────────────────────────────────┤" << RESET << endl;
    cout << string(centerOffset, ' ') << CYAN << "│ " << BOLD << WHITE << "No" << RESET << "  " << CYAN << "│" << RESET << string(4, ' ') << BOLD << WHITE << "Options" << RESET << string(47, ' ') << CYAN << "│" << RESET << endl;
    cout << string(centerOffset, ' ') << CYAN << "╞═════╪══════════════════════════════════════════════════════════╡" << RESET << endl;
    
    // Option 1 - Manage Students
    cout << string(centerOffset, ' ') << CYAN << "│ " << YELLOW << "1" << RESET << "   " << CYAN << "│ " << RESET;
    cout << GREEN << "👥" << RESET << " " << GREEN << "Manage Students" << RESET;
    cout << string(39, ' ') << CYAN << "│" << RESET << endl;
    
    cout << string(centerOffset, ' ') << CYAN << "├─────┼──────────────────────────────────────────────────────────┤" << RESET << endl;
    
    // Option 2 - Import Excel Data
    cout << string(centerOffset, ' ') << CYAN << "│ " << YELLOW << "2" << RESET << "   " << CYAN << "│ " << RESET;
    cout << BLUE << "📁" << RESET << " " << BLUE << "Import Excel Data" << RESET;
    cout << string(37, ' ') << CYAN << "│" << RESET << endl;
    
    cout << string(centerOffset, ' ') << CYAN << "├─────┼──────────────────────────────────────────────────────────┤" << RESET << endl;
    
    // Option 3 - Export Grade Report
    cout << string(centerOffset, ' ') << CYAN << "│ " << YELLOW << "3" << RESET << "   " << CYAN << "│ " << RESET;
    cout << YELLOW << "📈" << RESET << " " << YELLOW << "Export Grade Report" << RESET;
    cout << string(35, ' ') << CYAN << "│" << RESET << endl;
    
    cout << string(centerOffset, ' ') << CYAN << "├─────┼──────────────────────────────────────────────────────────┤" << RESET << endl;
    
    // Option 4 - Backup Data
    cout << string(centerOffset, ' ') << CYAN << "│ " << YELLOW << "4" << RESET << "   " << CYAN << "│ " << RESET;
    cout << MAGENTA << "💾" << RESET << " " << MAGENTA << "Backup Data" << RESET;
    cout << string(43, ' ') << CYAN << "│" << RESET << endl;
    
    cout << string(centerOffset, ' ') << CYAN << "├─────┼──────────────────────────────────────────────────────────┤" << RESET << endl;
    
    // Option 5 - Sign Out
    cout << string(centerOffset, ' ') << CYAN << "│ " << YELLOW << "5" << RESET << "   " << CYAN << "│ " << RESET;
    cout << RED << "🚪" << RESET << " " << RED << "Sign Out" << RESET;
    cout << string(46, ' ') << CYAN << "│" << RESET << endl;
    
    cout << string(centerOffset, ' ') << CYAN << "╰─────┴──────────────────────────────────────────────────────────╯" << RESET << endl;
    cout << endl;
}

// Enhanced Student Menu
void MenuUtils::printStudentMenu() {
    printStudentDashboard();
    
    int consoleWidth = 120; // Adjust this to your console width
    int menuBoxWidth = 64;
    int centerOffset = (consoleWidth - menuBoxWidth) / 2;
    
    // Enhanced menu table with cyberpunk styling and colors - FIXED LAYOUT
    cout << string(centerOffset, ' ') << CYAN << "╭────────────────────────────────────────────────────────────────╮" << RESET << endl;
    cout << string(centerOffset, ' ') << CYAN << "│" << RESET << string(27, ' ') << BOLD << WHITE << "Student Options" << RESET << string(22, ' ') << CYAN << "│" << RESET << endl;
    cout << string(centerOffset, ' ') << CYAN << "├─────┬──────────────────────────────────────────────────────────┤" << RESET << endl;
    cout << string(centerOffset, ' ') << CYAN << "│ " << BOLD << WHITE << "No" << RESET << "  " << CYAN << "│" << RESET << string(4, ' ') << BOLD << WHITE << "Options" << RESET << string(47, ' ') << CYAN << "│" << RESET << endl;
    cout << string(centerOffset, ' ') << CYAN << "╞═════╪══════════════════════════════════════════════════════════╡" << RESET << endl;
    
    // Admin Login with enhanced styling
    cout << string(centerOffset, ' ') << CYAN << "│ " << YELLOW << "1" << RESET << "   " << CYAN << "│ " << RESET;
    cout << GREEN << "🔍" << RESET << " " << GREEN << "Search Your Data" << RESET;
    cout << string(38, ' ') << CYAN << "│" << RESET << endl;
    
    cout << string(centerOffset, ' ') << CYAN << "├─────┼──────────────────────────────────────────────────────────┤" << RESET << endl;
    
    // Student Login
    cout << string(centerOffset, ' ') << CYAN << "│ " << YELLOW << "2" << RESET << "   " << CYAN << "│ " << RESET;
    cout << BLUE << "📊" << RESET << " " << BLUE << "View Your Grades" << RESET;
    cout << string(38, ' ') << CYAN << "│" << RESET << endl;
    
    cout << string(centerOffset, ' ') << CYAN << "├─────┼──────────────────────────────────────────────────────────┤" << RESET << endl;
    
    // Exit option
    cout << string(centerOffset, ' ') << CYAN << "│ " << YELLOW << "3" << RESET << "   " << CYAN << "│ " << RESET;
    cout << RED << "🚪" << RESET << " " << RED << "Sign Out" << RESET;
    cout << string(46, ' ') << CYAN << "│" << RESET << endl;
    
    cout << string(centerOffset, ' ') << CYAN << "╰─────┴──────────────────────────────────────────────────────────╯" << RESET << endl;
    cout << endl;
}

// Enhanced Thank You Message
void MenuUtils::printThankYou() {
    clearScreen();
    
    // Add vertical spacing
    cout << string(5, '\n');
    
    // Thank you ASCII art
    cout << CYAN << endl;
    cout << "                ████████╗██╗  ██╗ █████╗ ███╗   ██╗██╗  ██╗    ██╗   ██╗ ██████╗ ██╗   ██╗" << endl;
    cout << "                ╚══██╔══╝██║  ██║██╔══██╗████╗  ██║██║ ██╔╝    ╚██╗ ██╔╝██╔═══██╗██║   ██║" << endl;
    cout << "                   ██║   ███████║███████║██╔██╗ ██║█████╔╝      ╚████╔╝ ██║   ██║██║   ██║" << endl;
    cout << "                   ██║   ██╔══██║██╔══██║██║╚██╗██║██╔═██╗       ╚██╔╝  ██║   ██║██║   ██║" << endl;
    cout << "                   ██║   ██║  ██║██║  ██║██║ ╚████║██║  ██╗       ██║   ╚██████╔╝╚██████╔╝" << endl;
    cout << "                   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝       ╚═╝    ╚═════╝  ╚═════╝ " << endl;
    cout << RESET << endl;
    
    cout << "                                      " << GREEN << "FOR USING OUR SYSTEM!" << RESET << endl;
    cout << endl;
    cout << "                                    " << YELLOW << "ScoreME Generator v1.0" << RESET << endl;
    cout << "                                 " << MAGENTA << "Developed by ISTAD Pre-Gen4 GroupI" << RESET << endl;
    cout << endl;
    
    // Animation
    cout << "                                          ";
    for (int i = 0; i < 5; i++) {
        cout << GREEN << "★ " << RESET;
        cout.flush();
        #ifdef _WIN32
            Sleep(300);
        #else
            usleep(300000);
        #endif
    }
    cout << endl;
    
    cout << "                                    " << GREEN << "Have a wonderful day! 👋" << RESET << endl;
    cout << endl;
    
    #ifdef _WIN32
        Sleep(3000);
    #else
        usleep(3000000);
    #endif
}


// Display methods
void MenuUtils::displayTable(const std::vector<Student>& students) {
    if (students.empty()) {
        printWarning("No students to display!");
        return;
    }
    
    Table table;
    table.add_row({"ID", "Name", "Age", "Gender", "Email", "Average", "Grade", "GPA", "Remark"});
    
    for (const auto& student : students) {
        table.add_row({
            student.getStudentId(),
            student.getName(),
            to_string(student.getAge()),
            student.getGender(),
            student.getEmail(),
            to_string(static_cast<int>(student.getAverageScore() * 100) / 100.0),
            student.getLetterGrade(),
            to_string(student.getGpa()),
            student.getRemark()
        });
    }
    
    table[0].format().font_style({FontStyle::bold}).font_color(Color::cyan);
    
    // Apply color coding based on new grading system
    for (size_t i = 1; i < table.size(); ++i) {
        std::string grade = students[i-1].getLetterGrade();
        std::string remark = students[i-1].getRemark();
        
        // Color the entire row based on grade
        Color gradeColor = getGradeColor(grade);
        table[i].format().font_color(gradeColor);
        
        // Make failing students more prominent
        if (remark == "Fail") {
            table[i].format().font_style({FontStyle::bold});
        }
    }
    
    cout << table << endl;
    
    // Display color legend
    printColorLegend();
}

void MenuUtils::displayStudentDetails(const Student& student) {
    Table table;
    table.add_row({"Field", "Value"});
    
    table.add_row({"Student ID", student.getStudentId()});
    table.add_row({"Name", student.getName()});
    table.add_row({"Age", to_string(student.getAge())});
    table.add_row({"Gender", student.getGender()});
    table.add_row({"Date of Birth", student.getDateOfBirth()});
    table.add_row({"Email", student.getEmail()});
    
    // Subject scores
    auto subjects = GradeUtil::getSubjectNames();
    auto scores = student.getSubjectScores();
    
    for (size_t i = 0; i < subjects.size() && i < scores.size(); ++i) {
        table.add_row({subjects[i], to_string(scores[i])});
    }
    
    table.add_row({"Average Score", to_string(student.getAverageScore())});
    table.add_row({"Letter Grade", student.getLetterGrade()});
    table.add_row({"GPA", to_string(student.getGpa())});
    table.add_row({"Remark", student.getRemark()});
    table.add_row({"Last Updated", student.getFormattedTimestamp()});
    
    table[0].format().font_style({FontStyle::bold}).font_color(Color::cyan);
    
    // Color the grade and remark rows based on new system
    for (size_t i = 1; i < table.size(); ++i) {
        std::string fieldName = table[i][0].get_text();
        
        if (fieldName == "Letter Grade") {
            Color gradeColor = getGradeColor(student.getLetterGrade());
            table[i].format().font_color(gradeColor).font_style({FontStyle::bold});
        }
        else if (fieldName == "Remark") {
            if (student.getRemark() == "Pass") {
                table[i].format().font_color(Color::green).font_style({FontStyle::bold});
            } else {
                table[i].format().font_color(Color::red).font_style({FontStyle::bold});
            }
        }
        else if (fieldName == "Average Score") {
            Color gradeColor = getGradeColor(student.getLetterGrade());
            table[i].format().font_color(gradeColor);
        }
    }
    
    cout << table << endl;
}

void MenuUtils::displayGradeReport(const std::vector<Student>& students) {
    printHeader("GRADE REPORT");
    
    if (students.empty()) {
        printWarning("No students to display!");
        return;
    }
    
    // Summary statistics
    int totalStudents = students.size();
    int passingStudents = 0;
    int gradeA = 0, gradeB = 0, gradeC = 0, gradeD = 0, gradeE = 0, gradeF = 0;
    double totalAverage = 0.0;
    
    for (const auto& student : students) {
        if (student.hasPassingGrade()) {
            passingStudents++;
        }
        totalAverage += student.getAverageScore();
        
        // Count grades
        std::string grade = student.getLetterGrade();
        if (grade == "A") gradeA++;
        else if (grade == "B") gradeB++;
        else if (grade == "C") gradeC++;
        else if (grade == "D") gradeD++;
        else if (grade == "E") gradeE++;
        else if (grade == "F") gradeF++;
    }
    
    double classAverage = totalAverage / totalStudents;
    double passRate = (static_cast<double>(passingStudents) / totalStudents) * 100.0;
    
    Table summaryTable;
    summaryTable.add_row({"Statistic", "Value"});
    summaryTable.add_row({"Total Students", to_string(totalStudents)});
    summaryTable.add_row({"Passing Students (50+)", to_string(passingStudents)});
    summaryTable.add_row({"Failing Students (<50)", to_string(totalStudents - passingStudents)});
    summaryTable.add_row({"Pass Rate", to_string(static_cast<int>(passRate * 100) / 100.0) + "%"});
    summaryTable.add_row({"Class Average", to_string(static_cast<int>(classAverage * 100) / 100.0)});
    
    // Grade distribution
    summaryTable.add_row({"Grade A (90-100)", to_string(gradeA)});
    summaryTable.add_row({"Grade B (80-89)", to_string(gradeB)});
    summaryTable.add_row({"Grade C (70-79)", to_string(gradeC)});
    summaryTable.add_row({"Grade D (60-69)", to_string(gradeD)});
    summaryTable.add_row({"Grade E (50-59)", to_string(gradeE)});
    summaryTable.add_row({"Grade F (<50)", to_string(gradeF)});
    
    summaryTable[0].format().font_style({FontStyle::bold}).font_color(Color::magenta);
    
    // Color the grade distribution rows
    for (size_t i = 1; i < summaryTable.size(); ++i) {
        std::string field = summaryTable[i][0].get_text();
        if (field.find("Grade A") == 0 || field.find("Grade B") == 0 || field.find("Grade C") == 0) {
            summaryTable[i].format().font_color(Color::green);
        }
        else if (field.find("Grade D") == 0 || field.find("Grade E") == 0) {
            summaryTable[i].format().font_color(Color::yellow);
        }
        else if (field.find("Grade F") == 0) {
            summaryTable[i].format().font_color(Color::red);
        }
        else if (field.find("Failing") == 0) {
            summaryTable[i].format().font_color(Color::red);
        }
        else if (field.find("Passing") == 0) {
            summaryTable[i].format().font_color(Color::green);
        }
    }
    
    cout << summaryTable << endl;
    
    printSeparator();
    displayTable(students);
}

void MenuUtils::displayFailingStudents(const std::vector<Student>& students) {
    if (students.empty()) {
        printSuccess("No failing students found!");
        return;
    }
    
    printError("FAILING STUDENTS (<50 Average) - " + to_string(students.size()) + " found:");
    displayTable(students);
}

// Color legend function
void MenuUtils::printColorLegend() {
    cout << "\n" << BOLD << "Grade Color Legend:" << RESET << endl;
    cout << GREEN << "● A-C: Excellent Performance" << RESET << endl;
    cout << YELLOW << "● D-E: Acceptable Performance" << RESET << endl;
    cout << RED << "● F: Failing Performance (<50)" << RESET << endl;
    cout << endl;
}

// Menu display methods
void MenuUtils::printMenu(const std::vector<std::string>& items) {
    Table table;
    table.add_row({"No", "Menu"});
    
    for (size_t i = 0; i < items.size(); i++) {
        table.add_row({to_string(i + 1), items[i]});
    }
    
    table[0].format().font_style({FontStyle::bold}).font_color(Color::cyan);
    cout << table << endl;
}

// Colored output methods
void MenuUtils::printColored(const std::string& text, const std::string& color) {
    cout << color << text << RESET << endl;
}

void MenuUtils::printSuccess(const std::string& message) {
    cout << GREEN << "✅ " << message << RESET << endl;
}

void MenuUtils::printError(const std::string& message) {
    cout << RED << "❌ " << message << RESET << endl;
}

void MenuUtils::printWarning(const std::string& message) {
    cout << YELLOW << "⚠️ " << message << RESET << endl;
}

void MenuUtils::printInfo(const std::string& message) {
    cout << BLUE << "ℹ️ " << message << RESET << endl;
}

// Input methods
int MenuUtils::getMenuChoice(int maxOptions) {
    int choice;
    while (true) {
        cout << string(35, ' ') << CYAN << ">> Choose your option (1-" << maxOptions << "): " << RESET;
        cin >> choice;
        
        if (cin.fail() || choice < 1 || choice > maxOptions) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << string(35, ' ') << RED << "❌ Invalid choice! Please enter a number between 1 and " << to_string(maxOptions) << RESET << endl;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }
    }
}

std::string MenuUtils::getHiddenInput(const std::string& prompt) {
    cout << prompt;
    
    string input;
    
#ifdef _WIN32
    // Windows implementation using _getch()
    char ch;
    while ((ch = _getch()) != '\r') {  // '\r' is Enter key on Windows
        if (ch == '\b') {  // Backspace
            if (!input.empty()) {
                input.pop_back();
                cout << "\b \b";  // Move back, print space, move back again
            }
        } else {
            input += ch;
            cout << '*';  // Print asterisk instead of actual character
        }
    }
    cout << endl;
#else
    // Unix/Linux implementation
    struct termios oldt, newt;
    
    // Get current terminal settings
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    
    // Disable echo
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    // Read input
    getline(cin, input);
    
    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    
    cout << endl;
#endif
    
    return input;
}

bool MenuUtils::askContinue() {
    string response;
    cout << YELLOW << "Do you want to continue? (Y/N): " << RESET;
    getline(cin, response);
    
    return (response == "Y" || response == "y" || response == "yes" || response == "Yes");
}

std::string MenuUtils::getStringInput(const std::string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

int MenuUtils::getIntInput(const std::string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            printError("Invalid input! Please enter a valid number.");
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

double MenuUtils::getDoubleInput(const std::string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            printError("Invalid input! Please enter a valid number.");
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

// Utility methods
void MenuUtils::clearScreen() {
#ifdef _WIN32
    system("cls");  // Windows clear screen command
#else
    system("clear");  // Unix/Linux clear screen command
#endif
}

void MenuUtils::pauseScreen() {
    cout << CYAN << "\nPress Enter to continue..." << RESET;
    cin.get();
}

void MenuUtils::printSeparator() {
    cout << CYAN << string(60, '=') << RESET << endl;
}

void MenuUtils::printHeader(const std::string& title) {
    printSeparator();
    cout << BOLD << CYAN << "               " << title << RESET << endl;
    printSeparator();
}