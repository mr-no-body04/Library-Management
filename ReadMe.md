# Library Management System

![C++](https://img.shields.io/badge/C++-17-blue?logo=c%2B%2B)
![Windows](https://img.shields.io/badge/Platform-Windows-lightgrey?logo=windows)

A feature-rich console-based Library Management System implemented in C++ with color-coded interface, data structures, and sorting algorithms.

## Features

- **User Authentication System**
  - Admin and Student roles
  - Secure password input handling
  - User registration and management
- **Book Management**
  - Add/remove books
  - Issue/return books
  - Track book availability
- **Advanced Search & Sort**
  - Quick Sort for book titles
  - Bubble Sort implementation
  - Linear and Binary Search
- **Study Room Booking**
  - Real-time room availability
  - Queue-based reservation system
- **Statistics Dashboard**
  - Book inventory status
  - User statistics
  - Room utilization
- **Color-Coded Interface**
  - Intuitive menu system
  - Loading animations
  - Visual status indicators

## Data Structures & Algorithms

- **Custom Linked List** for book storage
- **Queue Implementation** for room reservations
- **Quick Sort** (O(n log n) for efficient sorting
- **Bubble Sort** for educational comparison
- **Linear & Binary Search** algorithms
- **Vector-based** user management

## Classes Overview

| Class             | Responsibility                          |
|-------------------|-----------------------------------------|
| `Book`            | Book properties and status management   |
| `User`/`Admin`    | User roles and authentication           |
| `StudyRoom`       | Room booking system                     |
| `BookList`        | Linked list book storage                |
| `StudyRoomQueue`  | Queue for room reservations             |
| `ColorManager`    | Console text coloring                   |
| `MenuDesign`      | UI components and formatting            |
| `LibrarySystem`   | Core application logic                  |

## Getting Started

### Prerequisites
- Windows OS
- C++17 compatible compiler (GCC or MinGW recommended)

### Compilation
```bash
g++ -std=c++17 main.cpp -o library_system.cpp
Execution
bash
library_system.cpp

Default Credentials
Admin Access:

text
Username: admin
Password: admin123
Student Access:

text
Username: student
Password: student123

Usage Examples

Book Management:

Add books with title, author, and ISBN
Sort books using different algorithms
Search using linear or binary methods

Room Booking:

View available study rooms
Book rooms using queue system
Automatic room status tracking
System Administration:
Register new users
View system statistics
Manage book inventory

Key Functions
cpp
// Sorting implementations
void quickSort(vector<Book>& arr, int low, int high);
void bubbleSort(vector<Book>& arr);

// Search functions
int linearSearch(const string& title);
int binarySearch(const string& title);

// Room booking
void bookStudyRoom(User* user);
Visual Design
Color-Coded UI:

Purple: Headers and titles
Green: Success messages
Red: Error notifications
Cyan: Section titles and prompts
Yellow: Data tables

UI Elements:

Centered titles
Section dividers
Animated loading bars
Formatted tables


This README includes:

1. Project badges for C++ version and platform
2. Comprehensive feature list
3. Data structures and algorithms used
4. Class overview table
5. Compilation and execution instructions
6. Default credentials
7. Usage examples
8. Key code snippets
9. UI design details

The README is structured to give both technical users and end users all necessary information to understand, compile, and use the application effectively.