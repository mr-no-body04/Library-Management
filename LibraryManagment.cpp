#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <windows.h>
#include <conio.h>
#include <algorithm>
#include <iomanip>
using namespace std;

// ======================
//    Color Management
// ======================
class ColorManager {
public:
    static void setColor(int color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }
    
    static void red() { setColor(12); }
    static void green() { setColor(10); }
    static void blue() { setColor(9); }
    static void yellow() { setColor(14); }
    static void white() { setColor(15); }
    static void cyan() { setColor(11); }
    static void purple() { setColor(13); }
    static void reset() { setColor(7); }
};

// ======================
//     Display Utilities
// ======================
class MenuDesign {
public:
    static void clearScreen() {
        system("cls");
    }
    
    static void header(string title) {
        ColorManager::purple();
        cout << "\n\t" << string(60, '=') 
             << "\n\t" << centerAlign(title, 60)
             << "\n\t" << string(60, '=') << endl;
        ColorManager::reset();
    }
    
    static void sectionTitle(string title) {
        ColorManager::cyan();
        cout << "\n  » " << title << "\n";
        ColorManager::reset();
        cout << string(60, '-') << endl;
    }
    
  static  void loadingBar(const string& message = "Loading") {
    MenuDesign::clearScreen();
    MenuDesign::header(message);

    const int barWidth = 30;

    ColorManager::cyan(); // Start bar in cyan color

    cout << "\n\t[";
    for (int i = 0; i < barWidth; ++i) cout << " ";
    cout << "]";
    cout << "\r\t[";

    for (int i = 0; i <= barWidth; ++i) {
        cout << "#";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(50));
    }

    cout << "] ";

    ColorManager::green(); // Done text in green
    cout << "Done!\n";

    ColorManager::reset(); // Reset to default terminal color
    this_thread::sleep_for(chrono::milliseconds(400));
}

    
    static string centerAlign(string text, int width) {
        int padding = (width - text.length()) / 2;
        if(padding <= 0) return text;
        return string(padding, ' ') + text + string(padding, ' ');
    }
};


// ======================
//      Core Classes
// ======================
class Book {
private:
    string title;
    string author;
    string ISBN;
    bool isIssued;
    string issueDate;

public:
    Book(string t = "", string a = "", string isbn = "", bool issued = false)
        : title(t), author(a), ISBN(isbn), isIssued(issued) {}

    // Getters
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getISBN() const { return ISBN; }
    bool getIsIssued() const { return isIssued; }

    // Setters
    void issue(string date) {
        isIssued = true;
        issueDate = date;
    }
    
    void returnBook() {
        isIssued = false;
        issueDate = "";
    }
};

// ======================
//    User Classe
// ======================
class User {
protected:
    string username;
    string password;
    string role;

public:
    User(string u = "", string p = "", string r = "Student") 
        : username(u), password(p), role(r) {}
    
    virtual string getRole() { return role; }
    string getUsername() const { return username; }
    bool checkPassword(string p) const { return password == p; }
    virtual ~User() = default;
};

class Admin : public User {
public:
    Admin(string u, string p) : User(u, p, "Admin") {}
};

// ======================
//    Study Room Clas
// ======================
class StudyRoom {
    int roomNumber;
    bool isAvailable;
    string bookedBy;
    string bookingTime;

public:
    StudyRoom(int num) : roomNumber(num), isAvailable(true) {}
    
    void book(string user, string time) {
        isAvailable = false;
        bookedBy = user;
        bookingTime = time;
    }
    
    void release() {
        isAvailable = true;
        bookedBy = "";
        bookingTime = "";
    }
    
    string getStatus() const {
        return isAvailable ? "Available" : "Booked by " + bookedBy;
    }
    
    int getNumber() const { return roomNumber; }
};

// ======================
//    Custom Data Structures
// ======================
class BookList {
private:
    struct BookNode {
        Book data;
        BookNode* next;
        BookNode(const Book& b) : data(b), next(nullptr) {}
    };
    
    BookNode* head;
    int count;

public:
    BookList() : head(nullptr), count(0) {}
    
    void insert(const Book& book) {
        BookNode* newNode = new BookNode(book);
        newNode->next = head;
        head = newNode;
        count++;
    }
    
    bool remove(const string& ISBN) {
        BookNode* curr = head;
        BookNode* prev = nullptr;
        
        while(curr) {
            if(curr->data.getISBN() == ISBN) {
                if(prev) prev->next = curr->next;
                else head = curr->next;
                delete curr;
                count--;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }

    vector<Book> getAllBooks() const {
        vector<Book> result;
        BookNode* curr = head;
        while(curr) {
            result.push_back(curr->data);
            curr = curr->next;
        }
        return result;
    }

    int size() const { return count; }
    
    void clear() {
        while(head) {
            BookNode* temp = head;
            head = head->next;
            delete temp;
        }
        count = 0;
    }
};

class StudyRoomQueue {
private:
    struct Node {
        StudyRoom* room;
        Node* next;
        Node(StudyRoom* r) : room(r), next(nullptr) {}
    };
    
    Node* front;
    Node* rear;

public:
    StudyRoomQueue() : front(nullptr), rear(nullptr) {}
    
    void enqueue(StudyRoom* room) {
        Node* newNode = new Node(room);
        if(rear) rear->next = newNode;
        rear = newNode;
        if(!front) front = rear;
    }
    
    StudyRoom* dequeue() {
        if(!front) return nullptr;
        Node* temp = front;
        StudyRoom* room = temp->room;
        front = front->next;
        if(!front) rear = nullptr;
        delete temp;
        return room;
    }
    
    bool isEmpty() const { return front == nullptr; }
    
    size_t size() const {
        size_t count = 0;
        Node* curr = front;
        while(curr) {
            count++;
            curr = curr->next;
        }
        return count;
    }
};

// ======================
//    Library System
// ======================
class LibrarySystem {
private:
    BookList books;
    vector<User*> users;
    StudyRoomQueue availableRooms;
    vector<StudyRoom> studyRooms;
    
    int totalBooks;
    int issuedBooks;
    int registeredUsers;

    void quickSort(vector<Book>& arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }

    int partition(vector<Book>& arr, int low, int high) {
        Book pivot = arr[high];
        int i = low - 1;

        for (int j = low; j <= high - 1; j++) {
            if (arr[j].getTitle() < pivot.getTitle()) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);
        return i + 1;
    }

    bool isSorted(const vector<Book>& arr) {
        for(size_t i = 1; i < arr.size(); i++) {
            if(arr[i-1].getTitle() > arr[i].getTitle()) return false;
        }
        return true;
    }

    void bubbleSort(vector<Book>& arr) {
        bool swapped;
        for(size_t i = 0; i < arr.size()-1; i++) {
            swapped = false;
            for(size_t j = 0; j < arr.size()-i-1; j++) {
                if(arr[j].getTitle() > arr[j+1].getTitle()) {
                    swap(arr[j], arr[j+1]);
                    swapped = true;
                }
            }
            if(!swapped) break;
        }
    }

    int linearSearch(const string& title) {
        vector<Book> bookVec = books.getAllBooks();
        for(size_t i = 0; i < bookVec.size(); i++) {
            if(bookVec[i].getTitle() == title) return i;
        }
        return -1;
    }

    int binarySearch(const string& title) {
        vector<Book> bookVec = books.getAllBooks();
        int left = 0, right = bookVec.size() - 1;
        while(left <= right) {
            int mid = left + (right - left)/2;
            if(bookVec[mid].getTitle() == title) return mid;
            if(bookVec[mid].getTitle() < title) left = mid + 1;
            else right = mid - 1;
        }
        return -1;
    }

    // Password input handler
    string getPasswordInput() {
        string pass;
        char ch;
        while((ch = _getch()) != '\r') { // Read until Enter key
            if(ch == '\b') { // Handle backspace
                if(!pass.empty()) {
                    cout << "\b \b";
                    pass.pop_back();
                }
            } else {
                pass.push_back(ch);
                cout << '*';
            }
        }
        cout << endl;
        return pass;
    }

    // Username existence check
    bool userExists(const string& username) {
        for(User* u : users) {
            if(u->getUsername() == username) {
                return true;
            }
        }
        return false;
    }

public:
    LibrarySystem() : totalBooks(0), issuedBooks(0), registeredUsers(0) {
        initializeRooms();
        createDefaultUsers();
        addDefaultBooks();
    }

    void createDefaultUsers() {
        users.push_back(new Admin("admin", "admin123"));
        users.push_back(new User("student", "student123"));
        registeredUsers = 2;
    }

    void addDefaultBooks() {
        books.insert(Book("Chemistry", "Einstein", "111"));
        books.insert(Book("Physics", "Newton", "222"));
        books.insert(Book("Mathematics", "Gauss", "333"));
        totalBooks = 3;
    }

    // ======================
    //    Core Functionality
    // ======================
    void addBook(const Book& book) {
        books.insert(book);
        totalBooks++;
    }

    bool deleteBook(const string& ISBN) {
        if(books.remove(ISBN)) {
            totalBooks--;
            return true;
        }
        return false;
    }

    void addBookMenu() {
        MenuDesign::clearScreen();
        MenuDesign::header("Add a New Book");

        string title, author, isbn;
        cout << "\n\tEnter Book Title: ";
        cin.ignore();
        getline(cin, title);
        cout << "\tEnter Author Name: ";
        getline(cin, author);
        cout << "\tEnter ISBN: ";
        getline(cin, isbn);

        if(title.empty() || author.empty() || isbn.empty()) {
            ColorManager::red();
            cout << "\n\t✘ All fields are required.\n";
        } else {
            addBook(Book(title, author, isbn));
            ColorManager::green();
            cout << "\n\t✔ Book added successfully!\n";
        }
        ColorManager::reset();
        waitForKey();
    }

    
    void sortBooksBubble() {
        vector<Book> bookVec = books.getAllBooks();
        if(!bookVec.empty()) {
            bubbleSort(bookVec);
            books.clear();
            for(const auto& b : bookVec) books.insert(b);
            ColorManager::green();
            cout << "\n\tBooks sorted using Bubble Sort!\n";
        } else {
            ColorManager::red();
            cout << "\n\tNo books to sort!\n";
        }
        waitForKey();
    }

    void sortBooksByTitle() {
        vector<Book> bookVec = books.getAllBooks();
        if(!bookVec.empty()) {
            quickSort(bookVec, 0, bookVec.size()-1);
            books.clear();
            for(const auto& b : bookVec) books.insert(b);
            ColorManager::green();
            cout << "\n\tBooks sorted using Quick Sort!\n";
        } else {
            ColorManager::red();
            cout << "\n\tNo books to sort!\n";
        }
        waitForKey();
    }

    void searchBookLinear() {
        MenuDesign::clearScreen();
        MenuDesign::header("Linear Search Books");
        
        string title;
        cout << "\n\tEnter book title to search: ";
        cin.ignore();
        getline(cin, title);

        int result = linearSearch(title);
        vector<Book> bookVec = books.getAllBooks();
        if(result != -1 && result < static_cast<int>(bookVec.size())) {
            ColorManager::green();
            cout << "\n\tBook found!\n";
            ColorManager::cyan();
            cout << "\tTitle: " << bookVec[result].getTitle() << endl;
            cout << "\tAuthor: " << bookVec[result].getAuthor() << endl;
            cout << "\tISBN: " << bookVec[result].getISBN() << endl;
        } else {
            ColorManager::red();
            cout << "\n\tBook not found!\n";
        }
        ColorManager::reset();
        waitForKey();
    }

    void searchBookByTitle() {
        vector<Book> bookVec = books.getAllBooks();
        if(!bookVec.empty() && !isSorted(bookVec)) {
            quickSort(bookVec, 0, bookVec.size()-1);
            books.clear();
            for(const auto& b : bookVec) books.insert(b);
        }

        MenuDesign::clearScreen();
        MenuDesign::header("Binary Search Books");
        string title;
        cout << "\n\tEnter book title to search: ";
        cin.ignore();
        getline(cin, title);

        int result = binarySearch(title);
        if(result != -1) {
            ColorManager::green();
            cout << "\n\tBook found!\n";
            ColorManager::cyan();
            cout << "\tTitle: " << bookVec[result].getTitle() << endl;
            cout << "\tAuthor: " << bookVec[result].getAuthor() << endl;
            cout << "\tISBN: " << bookVec[result].getISBN() << endl;
        } else {
            ColorManager::red();
            cout << "\n\tBook not found!\n";
        }
        ColorManager::reset();
        waitForKey();
    }

    bool issueBook(string isbn, string user) {
        vector<Book> bookVec = books.getAllBooks();
        for(auto& book : bookVec) {
            if(book.getISBN() == isbn && !book.getIsIssued()) {
                book.issue(getCurrentDate());
                books.remove(isbn);
                books.insert(book);
                issuedBooks++;
                return true;
            }
        }
        return false;
    }

    bool returnBook(string isbn) {
        vector<Book> bookVec = books.getAllBooks();
        for(auto& book : bookVec) {
            if(book.getISBN() == isbn && book.getIsIssued()) {
                book.returnBook();
                books.remove(isbn);
                books.insert(book);
                issuedBooks--;
                return true;
            }
        }
        return false;
    }

    void displayBooks() {
        vector<Book> bookVec = books.getAllBooks();
        MenuDesign::sectionTitle("Book Inventory");
        ColorManager::yellow();
        cout << setw(5) << "No." << setw(20) << "Title" 
             << setw(15) << "Author" << setw(10) << "ISBN"
             << setw(12) << "Status\n";
        ColorManager::reset();
        
        int counter = 1;
        for(const Book& b : bookVec) {
            cout << setw(5) << counter++
                 << setw(20) << b.getTitle()
                 << setw(15) << b.getAuthor()
                 << setw(10) << b.getISBN()
                 << setw(12) << (b.getIsIssued() ? "Issued" : "Available") 
                 << endl;
        }
    }

    // ======================
    //    Study Room System
    // ======================
    void initializeRooms() {
    // Reserve vector capacity upfront to prevent reallocation
    studyRooms.reserve(5);  // For 5 rooms
    for(int i=1; i<=5; i++) {
        studyRooms.emplace_back(i);  // Add room to vector
        availableRooms.enqueue(&studyRooms.back());  // Store valid pointer
    }
}

void bookStudyRoom(User* user) {
    if(availableRooms.isEmpty()) {
        ColorManager::red();
        cout << "\n\tNo available study rooms!\n";
        waitForKey();  // Add wait here to show message
        return;
    }

    StudyRoom* room = availableRooms.dequeue();
    if(room) {
        room->book(user->getUsername(), getCurrentTime());
        ColorManager::green();
        cout << "\n\tRoom " << room->getNumber() << " booked!\n";
    }
    waitForKey();  // Ensure message is visible
}

    void displayStudyRooms() {
        MenuDesign::sectionTitle("Study Room Status");
        ColorManager::yellow();
        cout << setw(10) << "Room No." << setw(20) << "Status\n";
        ColorManager::reset();
        
        for(const StudyRoom& sr : studyRooms) {
            cout << setw(10) << sr.getNumber() 
                 << setw(20) << sr.getStatus() << endl;
        }
    }

    // ======================
    //    Utility Functions
    // ======================
    string getCurrentDate() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        return to_string(ltm->tm_mday) + "/" + 
               to_string(1 + ltm->tm_mon) + "/" + 
               to_string(1900 + ltm->tm_year);
    }

    string getCurrentTime() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        return to_string(ltm->tm_hour) + ":" + 
               to_string(ltm->tm_min) + ":" + 
               to_string(ltm->tm_sec);
    }

    // ======================
    //    User Management
    // ======================
    bool removeUser(const string& username) {
        auto it = users.begin();
        while(it != users.end()) {
            if((*it)->getUsername() == username) {
                delete *it;
                it = users.erase(it);
                registeredUsers--;
                return true;
            }
            ++it;
        }
        return false;
    }

    // ======================
    //    Statistics System
    // ======================
    void displayStatistics() {
        vector<Book> bookVec = books.getAllBooks();
        MenuDesign::sectionTitle("System Statistics");
        ColorManager::yellow();
        cout << setw(25) << "Total Books: " << bookVec.size() << endl
             << setw(25) << "Issued Books: " << issuedBooks << endl
             << setw(25) << "Available Books: " << bookVec.size() - issuedBooks << endl
             << setw(25) << "Registered Users: " << registeredUsers << endl
             << setw(25) << "Available Study Rooms: " << availableRooms.size() << endl;
        ColorManager::reset();
        waitForKey();  
    }

    // ======================
    //      Main Interface
    // ======================
    void mainMenu(User* user) {
        while(true) {
            MenuDesign::clearScreen();
            MenuDesign::header("Main Menu - " + user->getUsername());
            
            ColorManager::green();
            cout << "\n\t1. Book Inventory\n\t2. Issue Book\n\t3. Return Book"
                 << "\n\t4. Study Rooms\n\t5. System Statistics\n\t6. User Management"
                 << "\n\t7. Logout\n";
            
            ColorManager::cyan();
            cout << "\n\tEnter choice: ";
            ColorManager::reset();
            int choice;
            cin >> choice;
            cin.ignore();
            
            switch(choice) {
                case 1: bookInventoryMenu(); break;
                case 2: issueBookMenu(); break;
                case 3: returnBookMenu(); break;
                case 4: studyRoomMenu(user); break;
                case 5: displayStatistics(); break;
                case 6: userManagementMenu(); break;
                case 7: return;
                default: invalidChoice();
            }
        }
    }

    void waitForKey() {
    ColorManager::cyan();
    cout << "\n\tPress any key to continue...";
    ColorManager::reset();
    _getch(); // wait for key press
}

void bookInventoryMenu() {
    while (true) {
        MenuDesign::clearScreen();
        MenuDesign::header("Book Inventory Management");

        ColorManager::green();
        cout << "\n\t1. View Books\n\t2. Add Book\n\t3. Sort Books (Quick Sort)"
             << "\n\t4. Sort Books (Bubble Sort)\n\t5. Search Book (Binary Search)"
             << "\n\t6. Search Book (Linear Search)\n\t7. Back to Main Menu\n";

        ColorManager::cyan();
        cout << "\n\tEnter your choice: ";
        ColorManager::reset();
        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                displayBooks();
                waitForKey();
                break;
            case 2:
                addBookMenu();
                break;
            case 3:
                sortBooksByTitle();
                break;
            case 4:
                sortBooksBubble();
                break;
            case 5:
                searchBookByTitle();
                break;
            case 6:
                searchBookLinear();
                break;
            case 7:
                return;
            default:
                invalidChoice();
        }
    }
}

void issueBookMenu() {
    MenuDesign::clearScreen();
    MenuDesign::header("Book Issuing System");

    string isbn, user;
    cout << "\n\tEnter ISBN: ";
    cin >> isbn;
    cout << "\tEnter username: ";
    cin >> user;

    if(issueBook(isbn, user)) {
        ColorManager::green();
        cout << "\n\t✔ Book issued successfully!\n";
    } else {
        ColorManager::red();
        cout << "\n\t✘ Book not available or invalid ISBN!\n";
    }

    ColorManager::reset();
    waitForKey();
}

    // ... Additional menu functions ...
 void returnBookMenu() {
    MenuDesign::clearScreen();
    MenuDesign::header("Book Return System");

    string isbn;
    cout << "\n\tEnter ISBN of the book to return: ";
    cin >> isbn;

    if(returnBook(isbn)) {
        ColorManager::green();
        cout << "\n\t✔ Book returned successfully!\n";
    } else {
        ColorManager::red();
        cout << "\n\t✘ Invalid ISBN or the book was not issued.\n";
    }

    ColorManager::reset();
    waitForKey();
}

  void studyRoomMenu(User* user) {
    MenuDesign::clearScreen();
    MenuDesign::header("Study Room Management");

    ColorManager::green();
    cout << "\n\t1. View Study Rooms\n"
         << "\t2. Book a Study Room\n"
         << "\t3. Back to Main Menu\n";

    ColorManager::cyan();
    cout << "\n\tEnter your choice: ";
    ColorManager::reset();
    int choice;
    cin >> choice;

    MenuDesign::clearScreen();

    switch(choice) {
        case 1:
            MenuDesign::header("Current Study Room Availability");
            displayStudyRooms();
            break;
        case 2:
            MenuDesign::header("Study Room Booking");
            bookStudyRoom(user);
            break;
        case 3:
            ColorManager::yellow();
            cout << "\n\tReturning to main menu...\n";
            break;
        default:
            invalidChoice();
            return;
    }

    ColorManager::reset();
    waitForKey();
}

void userManagementMenu() {
        while(true) {
            MenuDesign::clearScreen();
            MenuDesign::header("User Management Panel");

            ColorManager::green();
            cout << "\n\t1. Register New User\n"
                 << "\t2. Remove User\n"
                 << "\t3. Back to Main Menu\n";

            ColorManager::cyan();
            cout << "\n\tSelect an option: ";
            ColorManager::reset();
            int choice;
            cin >> choice;

            switch(choice) {
                case 1:
                    registerUserMenu();
                    break;
                case 2:
                    removeUserMenu();
                    break;
                case 3:
                    return;
                default:
                    invalidChoice();
            }
        }
    }

    void registerUserMenu() {
        MenuDesign::clearScreen();
        MenuDesign::header("User Registration");
        
        string username, password;
        ColorManager::cyan();
        cout << "\n\tEnter username: ";
        ColorManager::reset();
        cin >> username;
        
        if(userExists(username)) {
            ColorManager::red();
            cout << "\tUsername already exists!\n";
            waitForKey();
            return;
        }

        ColorManager::cyan();
        cout << "\tEnter password: ";
        ColorManager::reset();
        password = getPasswordInput();

        users.push_back(new User(username, password));
        registeredUsers++;
        ColorManager::green();
        cout << "\n\tRegistration successful!\n";
        waitForKey();
    }

    void removeUserMenu() {
        MenuDesign::clearScreen();
        MenuDesign::header("Remove User");
        
        string username;
        ColorManager::cyan();
        cout << "\n\tEnter username to remove: ";
        ColorManager::reset();
        cin >> username;

        if(removeUser(username)) {
            ColorManager::green();
            cout << "\n\tUser removed successfully!\n";
        } else {
            ColorManager::red();
            cout << "\n\tUser not found!\n";
        }
        waitForKey();
    }

    User* login() {
        MenuDesign::clearScreen();
        MenuDesign::header("User Login");
        
        string username, password;
        ColorManager::cyan();
        cout << "\n\tUsername: ";
        ColorManager::reset();
        cin >> username;
        
        ColorManager::cyan();
        cout << "\tPassword: ";
        ColorManager::reset();
        password = getPasswordInput();

        for(User* u : users) {
            if(u->getUsername() == username && u->checkPassword(password)) {
                return u;
            }
        }
        return nullptr;
    }


    void run() {
        while(true) {
            MenuDesign::clearScreen();
            MenuDesign::header("Library Management System");
            
            ColorManager::green();
            cout << "\n\t1. Login\n\t2. Register\n\t3. Exit\n";
            
            ColorManager::cyan();
            cout << "\n\tEnter choice: ";
            ColorManager::reset();
            int choice;
            cin >> choice;

            switch(choice) {
                case 1: {
                    User* user = login();
                    if(user) {
                        MenuDesign::loadingBar();
                        mainMenu(user);
                    } else {
                        ColorManager::red();
                        cout << "\n\tInvalid credentials!\n";
                        this_thread::sleep_for(chrono::seconds(1));
                    }
                    break;
                }
                case 2: registerUserMenu(); break;
                case 3: return;
                default: invalidChoice();
            }
        }
    }

    void invalidChoice() {
        ColorManager::red();
        cout << "\n\tInvalid choice! Please try again.\n";
        this_thread::sleep_for(chrono::seconds(1));
    }

    ~LibrarySystem() {
    for (User* u : users) delete u;
}
};

// ======================
//        Main
// ======================
int main() {
    LibrarySystem library;
    library.run();
    return 0;
}