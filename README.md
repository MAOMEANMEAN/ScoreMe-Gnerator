# 🎓 ScoreME-Generator  

A powerful and efficient **C++ command-line tool** designed to automate and simplify student grade management.  

ScoreME-Generator streamlines the entire process of handling student data—from importing scores from Excel to generating beautifully formatted reports.  
Built for **speed** and **reliability**, it empowers educators to focus on what truly matters: their students.  

---

## 🌟 Key Features  

- **Seamless Excel Integration**: Import student names, IDs, and scores directly from `.xlsx` files. Process multiple classes at once.  
- **Automated Grade Calculation**: Automatically computes averages and assigns letter grades based on predefined rules.  
- **Clean & Formatted Output**: Results displayed in **beautiful, paginated tables** in the CLI.  
- **Effortless Reporting**: Export final grades, averages, and reports to Excel with one command.  
- **Powerful CLI Filters**: Filter results (e.g., failing students, top performers, or search by ID).  
- **Batch Processing**: Handle multiple classes or subjects simultaneously.  
- **Automated Backups**: Built-in backup system prevents data loss.  
- **Robust Build System**: Modular and portable with **CMake**.  

---

## 🛠️ Technologies Used  

- **C++** → High performance & reliability  
- **CMake** → Cross-platform build system  
- **[xlnt](https://github.com/tfussell/xlnt)** → Read/write Excel `.xlsx` files  
- **[tabulate](https://github.com/p-ranav/tabulate)** → Pretty CLI tables  

---

## 📂 Project Structure  

ScoreME-Generator/
├── build/ # Compiled binaries & build files
├── data/ # Default Excel files & backups
│ ├── students.xlsx # Sample input file
│ └── backups/ # Automated backups
├── include/ # Header files (.hpp)
│ ├── Admin.hpp
│ ├── ExcelUtil.hpp
│ ├── GradeUtil.hpp
│ ├── MenuUtils.hpp
│ ├── Person.hpp
│ └── Student.hpp
├── src/ # Source files (.cpp) except main.cpp
│ ├── Admin.cpp
│ ├── ExcelUtil.cpp
│ ├── GradeUtil.cpp
│ ├── MenuUtils.cpp
│ ├── Person.cpp
│ └── Student.cpp
├── main.cpp # Entry point of the program
├── CMakeLists.txt # Main CMake build script
└── README.md # Documentation


---

## 🚀 Getting Started  

---

## 🖥️ Development Environment Setup  

To build and run **ScoreME-Generator**, you need to set up your environment properly.  

### 🔹 Step 1: Install CMake  

Download and install the latest stable version of **CMake**:  
👉 [CMake Windows Installer](https://cmake.org/files/v3.22/cmake-3.22.0-rc2-windows-x86_64.msi)  

Or visit the full release list:  
👉 [CMake Downloads](https://cmake.org/files/v3.22/)  

---

### 🔹 Step 2: Install Visual Studio Build Tools (Windows)  

You need a **C++ compiler**. If you are on Windows, install:  
👉 [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/?q=build+tools)  

During installation, select:  
- **Desktop development with C++**  
- Make sure to include **MSVC** and **Windows SDK**  

For Linux/macOS → Use `g++` or `clang` (usually pre-installed).  

---

### 🔹 Step 3: Install VS Code Extensions  

To make development easier in **Visual Studio Code**, install the following extensions:  

- 🔧 **CMake Tools** → Build & configure CMake projects directly in VS Code  
- ⚙️ **CMake** → Language support for `CMakeLists.txt`  
- 📊 **Excel Viewer** (XLSX Viewer) → Open and view `.xlsx` files inside VS Code  
- 📑 **CSV Viewer** → Easy table-like view for `.csv` files  

---

### 🔹 Step 4: Verify Installation  

Check versions in your terminal:  

```bash
cmake --version
g++ --version   # or clang --version


### ✅ Prerequisites  

Make sure you have installed:  
- **Git**  
- **CMake** (v3.10+)  
- **C++ Compiler** (GCC / MSVC)  

### 📥 Installation & Building  

**1. Clone the Repository**  

```bash
- git clone https://github.com/your-username/ScoreME-Generator.git
- cd ScoreME-Generator
---

## 🏗️ Build & First Run (Windows with VS Code)  

Follow these steps if you are using **Visual Studio Code** with CMake Tools:  

### 🔹 Step 1: Open Project  
- Clone the repo and open it in **VS Code**  
- Press **`Ctrl + Shift + P`** → search for `CMake: Select Active Folder`  
- Choose your project folder  

### 🔹 Step 2: Configure Project  
- Go to **Configure** in the CMake Tools menu  
- Select **Visual Studio Community 2022 - Release - x86 architecture**  

### 🔹 Step 3: Select Build Type  
- Choose **Release**  

### 🔹 Step 4: Build Project  
- Click **Build → ALL_BUILD**  
- Wait until the build finishes with:  code 0

### 🔹 Step 5: Run the Program  

After build, navigate to the executable:  

```powershell
# First Run
- cd .\build\
- cd .\Release\
- .\ScoreME-Generator.exe
# Second Run
- Launch (▶ Run button) in VS Code (CMake Tools extension)

---

## 📖 Usage  

Once you run the program, you will see the **Main Menu**:  

- **Admin Login** → Manage students, import/export Excel, generate reports  
  - Default Username: `scoreme.pro`  
  - Default Password: `prome@123`  
- **Student Login** → Students can log in to view their own grades  
- **Exit** → Quit the program  

### Admin Dashboard Options  
- 📌 Manage Students → Add, edit, delete, search  
- 📂 Import Excel Data → Load `.xlsx` file  
- 📤 Export Grade Report → Save clean report to Excel  
- 💾 Backup Data → Create timestamped backup  

---


### 🧑‍🎓 Student Dashboard  

When a student logs in, they can access their personal dashboard.  

Available options:  

- 🔍 **Search by Student ID**  
  - Students can enter their ID to quickly access their grade record.  
  - Prevents confusion if multiple classes exist.  

- 👀 **View Grades**  
  - Displays a clean, paginated table of all subjects.  
  - Includes:  
    - Subject Name  
    - Score  
    - Letter Grade (A, B, C, D, F)  
    - GPA (if applicable)  
  - Example:  
    ```
    +------------+-------+-------+
    | Subject    | Score | Grade |
    +------------+-------+-------+
    | Math       | 85    | B     |
    | Physics    | 92    | A     |
    | Chemistry  | 76    | C     |
    +------------+-------+-------+
    GPA: 3.2
    ```
---

## 🎯 Final Notes  

ScoreME-Generator is built with the mission to **save time for educators** and **empower students** with instant access to their grades.  
We’re continuously improving this project—adding new features, refining the interface, and making it even more reliable.  

If you find this project helpful, feel free to ⭐ the repository and share it with others.  
Your feedback and contributions are always welcome! 🚀
