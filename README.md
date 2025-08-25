# 🎓 ScoreME Generator

A powerful and automated student score management system designed to replace error-prone manual processes with seamless Excel integration, automated grade calculation, and effortless reporting.

---

## 🚀 The Problem It Solves

In today's educational environment, many still rely on traditional, manual methods for student management. This leads to several challenges:

- **Time-Consuming Manual Entry**: Manually inputting scores and calculating grades is tedious and prone to errors.  
- **Data Loss**: Storing data on physical paper or in single, un-backed-up files is risky.  
- **Version Control Nightmares**: Tracking changes in Excel without a system often results in formula errors and inconsistent data.  
- **Repetitive Reporting**: Generating reports requires rebuilding charts, reformatting layouts, and pulling data manually.  

**ScoreME Generator** eliminates these pain points by providing a streamlined, automated solution.

---

## ✨ Core Features

- 📁 **Seamless Excel Integration**: Import student data directly from `.xlsx` files and export beautifully formatted reports.  
- 🧮 **Automated Grade Calculation**: Computes averages, letter grades, and GPA automatically.  
- 🖥️ **Clean CLI Output**: Results displayed in paginated, readable terminal tables.  
- 📈 **Effortless Reporting**: Export final grades and summary reports to Excel with a single command.  
- 🔍 **Powerful Filtering**: Instantly view failing students, top performers, or search by ID.  
- 🛠️ **Robust Build System**: Built with **CMake** for reliable cross-platform builds.  

---

## 🖥️ Getting Started

### ✅ Prerequisites
Make sure you have the following installed:
- A **C++17 (or later) compiler** (GCC, Clang, MSVC)  
- **CMake** (v3.14+) → [Download Here](https://cmake.org/download/)  
- **Git**  

> ℹ️ Dependencies (`xlnt` and `tabulate`) are downloaded and linked automatically via CMake.

---

### ⚙️ Build Instructions

Clone the repository:
```bash
git clone <your-repository-url>
cd ScoreME-Generator
