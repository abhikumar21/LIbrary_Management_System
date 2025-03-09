#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <ctime>
#include <vector>
#include <iomanip>




using namespace std;

// Book Class
class Book {
public:
    string title, author, publisher, year, ISBN, available;
    Book(string t, string a, string p, string y, string i, string avail="1") : title(t), author(a), publisher(p), year(y), ISBN(i), available(avail) {}
};

// Account Class to Track Borrowed Books
class Account {
    public:
        vector<string> borrowedBooks;
        int fine;
    
        Account() : fine(0) {}
    
        void borrowBook(string bookID) { borrowedBooks.push_back(bookID); }
    
        void returnBook(string bookID) {
            borrowedBooks.erase(remove(borrowedBooks.begin(), borrowedBooks.end(), bookID), borrowedBooks.end());
        }
    
        void payFines(int pay) {
   
            int paymentChoice;
            cout << "Choose a payment method:\n";
            cout << "1. Credit/Debit Card\n2. UPI\n3. Net Banking\nEnter choice: ";
            cin >> paymentChoice;

            cout<<"Pay overdue amount: "<<pay<<endl;
            string proceedChoice;
            cout<<"Continue Payment"<<"\n1. yes\n2. no"<<endl;
            cout<<"Enter your choice to proceed: ";
            cin>>proceedChoice;
            if(proceedChoice=="2") {
                cout<<"Process terminated"<<endl;
                return;
            }
        
            if (paymentChoice < 1 || paymentChoice > 3) {
                cout << "Invalid payment method. Try again.\n";
                return;
            }
            string paymentId;
            cout<<"Enter your payment id(any string): ";
            cin>>paymentId;
            string password;
            cout<<"Enter 4 digit password(any string): ";
            cin>>password;

            cout << "\nProcessing payment...\n";
        
            cout << "Payment successful!\nFines cleared.\n";
        }
        
    };
    
    class User {
    public:
        string name, userID;
        int paidAmount=0;
        Account account;
        
        User(string uid, string uname, int pay=0) : userID(uid), name(uname), paidAmount(pay) {}
    
        virtual void borrowBook(Book &book) = 0;
        virtual void returnBook(Book &book) = 0;
        virtual ~User() {}
    
    protected:
    void logReturn(string bookISBN, string borrowTime) {
        ofstream file("borrowing_history.txt", ios::app);
        if (file.is_open()) {
            time_t now = time(0);
            char *returnTime = ctime(&now); // Get current time as string
            returnTime[strlen(returnTime) - 1] = '\0'; // Remove newline from ctime output

            // Write userID, bookISBN, borrowTime, and returnTime to the file
            file << userID << "," << bookISBN << "," << borrowTime << "," << returnTime << "\n";
            file.close();
        }
    }
     
        void logBorrowing(string bookISBN) {
            ofstream file("borrowed_books.txt", ios::app);
            if (file.is_open()) {
                time_t now = time(0);
                char *dt = ctime(&now); // Convert time to string
                dt[strlen(dt) - 1] = '\0'; // Remove newline from ctime output
    
                file << userID << "," << bookISBN << "," << dt << "\n";
                file.close();
            }
        }

    public:
    
    void showBorrowingHistory() {
        ifstream file("borrowing_history.txt");
        if (!file.is_open()) {
            cout << "Error: Could not open borrowing history file.\n";
            return;
        }

        string line;
        bool found = false;

        cout << "Borrowing History for User: " << userID << "\n";
        cout << "Book ISBN\tBorrow Time\t\tReturn Time\n";
        cout << "------------------------------------------------------------\n";

        while (getline(file, line)) {
            istringstream ss(line);
            string uid, isbn, borrowTime, returnTime;

            // Parse the line into userID, ISBN, borrowTime, and returnTime
            getline(ss, uid, ',');
            getline(ss, isbn, ',');
            getline(ss, borrowTime, ',');
            getline(ss, returnTime, ',');

            if (uid == userID) {
                found = true;
                cout << isbn << "\t" << borrowTime << "\t" << returnTime << "\n";
            }
        }

        if (!found) {
            cout << "No borrowing history found for this user.\n";
        }

        file.close();
    }

    int countBooks() {
        ifstream file("borrowed_books.txt");
        if (!file.is_open()) {
            cout << "Error opening file." << endl;
            return -1; // Return -1 to indicate an error
        }
    
        int count = 0;
        string line;
    
        while (getline(file, line)) { // Read each line from the file
            int commaPos = line.find(','); // Find the first comma
            if (commaPos != -1) { // Check if a comma was found
                string currentUserID = line.substr(0, commaPos); // Extract user ID
                if (currentUserID == userID) { // Compare with the target user ID
                    count++; // Increase count if it matches
                }
            }
        }
        file.close();
        return count;
    }
       
        void userHistory() {
            ifstream file("borrowed_books.txt");
            string line;
            bool found = false;

            if(!file.is_open()) {
                cout<<"error opening file!\n";
                return;
            }
            int totalFine=0;
            time_t now = time(0);
            int idx = 1;
            cout<<"Borrowed Books: "<<endl;
            while(getline(file, line)) {
                istringstream ss(line);
                string uid, isbn, timeStr;
                getline(ss, uid, ',');
                getline(ss, isbn, ',');
                getline(ss, timeStr, ',');

                if(uid == userID) {
                    found = true;
                    struct tm borrowTime = {};
                    istringstream timeStream(timeStr);
                    timeStream >> get_time(&borrowTime, "%a %b %d %H:%M:%S %Y");

                    if (timeStream.fail()) {
                        cout << "Failed to parse time for ISBN: " << isbn << endl;
                        continue;
                    }

                    time_t borrowEpoch = mktime(&borrowTime);
                    double seconds = difftime(now, borrowEpoch);
                    int days = seconds / (60);

                    cout<<idx<<". "<<isbn<<" | Borrowed for "<<days<<" days.\n";
                    if(uid[0]=='S') {
                        if((days)>15) {
                            int fine = ((days)-15)*10;
                            // cout<<"| Overdue | Fine: Rs"<<fine<<endl;
                            totalFine += fine;
                        }
                    }
                    idx++;
                }
            }
            
            if (!found) {
                cout << "No borrowed books found for user: " << userID << "\n";
            }
            cout<<"Total Fine: "<<totalFine-paidAmount;
            cout<<endl;
            file.close();
        }

        int getMaxOverdue() {
            ifstream file("borrowed_books.txt");
            string line;
            bool found = false;

            if(!file.is_open()) {
                cout<<"error opening file!\n";
                return 0;
            }
            time_t now = time(0);
            int idx = 1;
            int maxDays = 0;
            while(getline(file, line)) {
                istringstream ss(line);
                string uid, isbn, timeStr;
                getline(ss, uid, ',');
                getline(ss, isbn, ',');
                getline(ss, timeStr, ',');

                if(uid == userID) {
                    found = true;
                    struct tm borrowTime = {};
                    istringstream timeStream(timeStr);
                    timeStream >> get_time(&borrowTime, "%a %b %d %H:%M:%S %Y");

                    if (timeStream.fail()) {
                        cout << "Failed to parse time for ISBN: " << isbn << endl;
                        continue;
                    }

                    time_t borrowEpoch = mktime(&borrowTime);
                    double seconds = difftime(now, borrowEpoch);
                    int days = seconds / (60);
                    maxDays = max(maxDays, days);
                }
            }
            file.close();
            return maxDays;
        }

        int getOverdueAmount() {
            ifstream file("borrowed_books.txt");
            string line;
            bool found = false;

            if(!file.is_open()) {
                cout<<"error opening file!\n";
                return 0;
            }
            int totalFine=0;
            time_t now = time(0);
            int idx = 1;
            while(getline(file, line)) {
                istringstream ss(line);
                string uid, isbn, timeStr;
                getline(ss, uid, ',');
                getline(ss, isbn, ',');
                getline(ss, timeStr, ',');

                if(uid == userID) {
                    found = true;
                    struct tm borrowTime = {};
                    istringstream timeStream(timeStr);
                    timeStream >> get_time(&borrowTime, "%a %b %d %H:%M:%S %Y");

                    if (timeStream.fail()) {
                        cout << "Failed to parse time for ISBN: " << isbn << endl;
                        continue;
                    }

                    time_t borrowEpoch = mktime(&borrowTime);
                    double seconds = difftime(now, borrowEpoch);
                    int days = seconds / (60);

                    if(uid[0]=='S') {
                        if((days)>15) {
                            int fine = ((days)-15)*10;
                            totalFine += fine;
                        }
                    }
                    idx++;
                }
            }
            file.close();
            return totalFine-paidAmount;
        }
    
        void removeBorrowRecord(string bookISBN) {
            ifstream file("borrowed_books.txt");
            vector<string> lines;
            string line;
    
            if (file.is_open()) {
                while (getline(file, line)) {
                    istringstream ss(line);
                    string uid, isbn, timestamp;
                    
                    getline(ss, uid, ',');
                    getline(ss, isbn, ',');
                    
                    if (uid != userID || isbn != bookISBN) { 
                        lines.push_back(line); // Keep all other entries
                    }
                }
                file.close();
            }
    
            // Rewrite file with updated records
            ofstream outFile("borrowed_books.txt", ios::trunc);
            if (outFile.is_open()) {
                for (const string &entry : lines) {
                    outFile << entry << "\n";
                }
                outFile.close();
            }
        }
    };
    
    // Student Class
    class Student : public User {
    public:
        Student(string uid, string uname) : User(uid, uname) {}
    
        void borrowBook(Book &book) override {
            if (account.fine > 0) {
                cout << "You have outstanding fines. Please pay them before borrowing.\n";
                return;
            }
            if (account.borrowedBooks.size() < 3 && book.available == "1") {
                account.borrowBook(book.ISBN);
                book.available = "0";
                logBorrowing(book.ISBN);
                cout << "Book borrowed successfully!\n";
            } else {
                cout << "Cannot borrow more books or book is unavailable.\n";
            }
        }
    
        void returnBook(Book &book) override {
            book.available = "1";
            account.returnBook(book.ISBN);
    
            // Find the borrow record to get the borrow time
            ifstream inFile("borrowed_books.txt");
            string line;
            string borrowTime;
    
            if (inFile.is_open()) {
                while (getline(inFile, line)) {
                    istringstream ss(line);
                    string uid, isbn, timeStr;
                    getline(ss, uid, ',');
                    getline(ss, isbn, ',');
                    getline(ss, timeStr, ',');
    
                    if (uid == userID && isbn == book.ISBN) {
                        borrowTime = timeStr; // Save the borrow time
                        break;
                    }
                }
                inFile.close();
            }
    
            // Log the return details
            if (!borrowTime.empty()) {
                logReturn(book.ISBN, borrowTime);
            }
    
            removeBorrowRecord(book.ISBN); // Remove the borrow record
            cout << "Book returned successfully!\n";
        }
    };
    
    // Faculty Class
    class Faculty : public User {
    public:
        Faculty(string uid, string uname) : User(uid, uname) {}
    
        void borrowBook(Book &book) override {
            if (account.borrowedBooks.size() < 5 && book.available=="1") {
                account.borrowBook(book.ISBN);
                book.available = "0";
                logBorrowing(book.ISBN);
                cout << "Book borrowed successfully!\n";
            } else {
                cout << "Cannot borrow more books or book is unavailable.\n";
            }
        }
    
        void returnBook(Book &book) override {
            book.available = "1";
            account.returnBook(book.ISBN);
            removeBorrowRecord(book.ISBN);
            cout << "Book returned successfully!\n";
        }
    };


// Librarian Class (Handles Library Management)
class Librarian : public User {
public:
    vector<Book> books;
    unordered_map<string, User*> users; // userID -> User pointer

    Librarian(): User("L101", "Admin", 0) {};

    void borrowBook(Book &book) override{
        cout<<"Librarian do not borrow books\n";
    }
    void returnBook(Book &book) override{
        cout<<"Librarians do not return books.\n";
    }

    void addBook(string title, string author, string publisher, string year, string ISBN, string availability) {
        books.push_back(Book(title, author, publisher, year, ISBN, availability));
    }

    void removeBook(string ISBN) {
        books.erase(remove_if(books.begin(), books.end(), [&](Book &b) { return b.ISBN == ISBN; }), books.end());
    }

    void saveBooksToFile() {
        ofstream outFile("books.txt");
        for (auto &book : books) {
            outFile << book.title << "," << book.author << "," << book.publisher << "," <<book.year<<","<< book.ISBN << "," << book.available << "\n";
        }
        outFile.close();
    }

    void loadBooksFromFile() {
        ifstream inFile("books.txt");
        if (!inFile) {
            cout << "Error: Could not open books.txt file.\n";
            return;
        }
        books.clear();
        string line, title, author, publisher, year, ISBN, available;
        while (getline(inFile, line)) {
            stringstream ss(line);
            getline(ss, title, ',');
            getline(ss, author, ',');
            getline(ss, publisher, ',');
            getline(ss, year, ',');
            getline(ss, ISBN, ',');
            getline(ss, available, ',');
            books.push_back(Book(title, author, publisher, year, ISBN, available));
        }
        inFile.close();
    }

    void saveUsersToFile() {
        ofstream outFile("users.txt");
        for (auto &entry : users) {
            outFile << entry.first << "," << entry.second->name << "," << entry.second->paidAmount << "\n";
        }
        outFile.close();
    }
    

    void loadUsersFromFile() {
        ifstream inFile("users.txt");
        if (!inFile) {
            cout << "Error: Could not open users.txt file.\n";
            return;
        }
    
        users.clear();
        string line;
    
        while (getline(inFile, line)) {
            stringstream ss(line);
            string userID, name, paidAmountStr;
    
            // Extract userID
            getline(ss, userID, ',');
    
            // Extract name
            getline(ss, name, ',');
    
            // Extract paidAmount
            getline(ss, paidAmountStr);
    
            // Convert paidAmount to integer
            int paidAmount;
            try {
                paidAmount = stoi(paidAmountStr);
            } catch (...) {
                cout << "Error: Invalid amount in line: " << line << endl;
                continue;
            }
    
            // Create user object based on ID type
            if (userID[0] == 'S') {
                users[userID] = new Student(userID, name);
            } else if (userID[0] == 'F') {
                users[userID] = new Faculty(userID, name);
            } else if(userID[0]=='L'){
                users[userID] = new Librarian();
            }
    
            users[userID]->paidAmount = paidAmount;
        }
    
        inFile.close();
    }
    

    User* authenticateUser(string userID) {
        if (users.find(userID) != users.end()) {
            cout<<"\nWelcome "<<users[userID]->name<<"!"<<endl;
            return users[userID];
        }
        cout << "Invalid User ID!\n";
        return nullptr;
    }
};

// Main Function
int main() {
    Librarian librarian;
    librarian.loadBooksFromFile();
    librarian.loadUsersFromFile();
    
    int choice;
    string userID;
    User* currentUser = nullptr;
    while(currentUser==nullptr) {
        cout << "Enter your User ID: ";
        cin >> userID;
        currentUser = librarian.authenticateUser(userID);
    }

    if(currentUser->userID[0]=='L'){
        while(true) {

            int Lchoice;
            cout<<"1. Add Book in Library"<<endl;
            cout<<"2. Update Book Details in Library"<<endl;
            cout<<"3. Remove Book from Library"<<endl;
            cout<<"4. Add new User"<<endl;
            cout<<"5. Delete User"<<endl;
            cout<<"6. Exit"<<endl;
            cout<<"Enter your choice: ";
            cin >> Lchoice;
            cout<<endl;

            if (Lchoice == 1) {
                string title, author, publisher, year, ISBN, availability;
                cin.ignore();
                cout << "Enter Book Title: ";
                getline(cin, title);
                cout << "Enter Author Name: ";
                getline(cin, author);
                cout << "Enter Publisher Name: ";
                getline(cin, publisher);
                cout << "Enter Year Published: ";
                getline(cin, year);
                cout << "Enter ISBN Number: ";
                getline(cin, ISBN);
                cout<<"Status\n1. Available\n2. Reserved\nEnter your choice: ";
                getline(cin, availability);
            
                librarian.addBook(title, author, publisher, year, ISBN, availability);
                librarian.saveBooksToFile();
                cout << "\nYour Book has been added!!\n" << endl;
            }
            else if (Lchoice == 2) { // Update Book Details
                cout << "Enter ISBN of the book to update: ";
                string isbn;
                cin >> isbn;
                cin.ignore();
                for (auto &book : librarian.books) {
                    if (book.ISBN == isbn) {
                        cout << "Enter new title (leave blank to keep unchanged): ";
                        string newTitle;
                        getline(cin, newTitle);
                        if (!newTitle.empty()) book.title = newTitle;
            
                        cout << "Enter new author (leave blank to keep unchanged): ";
                        string newAuthor;
                        getline(cin, newAuthor);
                        if (!newAuthor.empty()) book.author = newAuthor;
            
                        cout << "Enter new publisher (leave blank to keep unchanged): ";
                        string newPublisher;
                        getline(cin, newPublisher);
                        if (!newPublisher.empty()) book.publisher = newPublisher;

                        cout << "Enter updated year (leave blank to keep unchanged): ";
                        string newYear;
                        getline(cin, newYear);
                        if (!newYear.empty()) book.year = newYear;

                        cout<<"Status\n1. Available\n2. Reserved\nEnter your choice(leave blank to keep unchanged): ";
                        string newAvailability;
                        getline(cin, newAvailability);
                        if(!newAvailability.empty()) book.available = newAvailability;
            
                        librarian.saveBooksToFile();
                        cout << "Book details updated successfully!\n";
                        break;
                    }
                }
            }
            else if (Lchoice == 3) { // Remove Book from Library
                cout << "Enter ISBN of book to remove: ";
                string isbn;
                cin >> isbn;
                librarian.removeBook(isbn);
                librarian.saveBooksToFile();
                cout << "Book removed successfully!\n";
            }
            else if (Lchoice == 4) { // Add New User
                cout << "Enter User ID: ";
                string userID, name;
                cin >> userID;
                cin.ignore();
                cout << "Enter User Name: ";
                getline(cin, name);
                
                if (userID[0] == 'S') {
                    librarian.users[userID] = new Student(userID, name);
                } else if (userID[0] == 'F') {
                    librarian.users[userID] = new Faculty(userID, name);
                } else{
                    cout<<"This userID is not allowed"<<endl;
                }
                
                librarian.users[userID]->paidAmount = 0; // Initialize paidAmount to 0
                librarian.saveUsersToFile();
                cout << "\nNew user added successfully!\n"<<endl;
            }
            else if (Lchoice == 5) { // Delete User
                cout << "Enter User ID to delete: ";
                string userID;
                cin >> userID;
                if (librarian.users.find(userID) != librarian.users.end()) {
                    delete librarian.users[userID];
                    librarian.users.erase(userID);
                    librarian.saveUsersToFile();
                    cout << "User deleted successfully!\n"<<endl;;
                } else {
                    cout << "User ID not found!\n"<<endl;
                }
            }
            else if(Lchoice==6) {
                break;
            }
            else{
                cout<<"Invalid Input!"<<endl;
            }  

        continue;
      }

    }

    else{
        while (true) {
            cout << "\nLibrary System Menu:\n";
            cout << "1. Show current Borrowing status/fines\n2. Borrow a book\n3. Return a book\n4. Pay dues\n5. Borrowing History\n6. Exit\nEnter your choice: ";
            cin >> choice;
            cout<<endl;
            
            if(choice == 1) {
                currentUser->userHistory();
            }
            else if (choice == 2) {
                int fines = currentUser->getOverdueAmount();
                if(fines > 0) {
                    cout<<"Your overdue amount is: "<<fines<<endl;
                    cout<<"You cannot borrow a book without paying your dues!!"<<endl;
                    continue;
                }
                int numBooksBorrowed = currentUser->countBooks();
                if(currentUser->userID[0]=='S' && numBooksBorrowed >= 3) {
                    cout<<"You cannot borrow more than 3 books at a time!!"<<endl;
                    continue;
                }
                else if(currentUser->userID[0]=='F') {
                    if(numBooksBorrowed >= 5) {
                        cout<<"You cannot borrow more than 5 books at a time!!"<<endl;
                        continue;
                    }
                    else if(currentUser->getMaxOverdue() > 60) {
                        cout<<"You have an overdue of more than 60 days!!"<<endl;
                        cout<<"Return the book to borrow new one"<<endl;
                    }
                }
                cout << "Available books:\n";
                for (size_t i = 0; i < librarian.books.size(); i++) {
                    if (librarian.books[i].available=="1") {
                        cout << i + 1 << ". " << librarian.books[i].title << "\n";
                    }
                }
                cout << "Select book number to borrow: ";
                int bookChoice;
                cin >> bookChoice;
                if (bookChoice >= 1 && bookChoice <= (int)librarian.books.size() && librarian.books[bookChoice - 1].available=="1") {
                    currentUser->borrowBook(librarian.books[bookChoice - 1]);
                    librarian.saveBooksToFile();
                } else {
                    cout << "Invalid choice or book not available!!\n";
                }
            } else if (choice == 3) {
                int fines = currentUser->getOverdueAmount();
                if(fines > 0) {
                    cout<<"Your overdue amount is: "<<fines<<endl;
                    cout<<"Pay dues before returning your book!!"<<endl;
                    continue;
                }
                cout << "Enter ISBN of book to return: ";
                string isbn;
                cin >> isbn;
                for (auto &book : librarian.books) {
                    if (book.ISBN == isbn && book.available!="1") {
                        currentUser->returnBook(book);
                        librarian.saveBooksToFile();
                        break;
                    }
                }
            } 
            else if(choice==4) {
                int pay = currentUser->getOverdueAmount();
                currentUser->account.payFines(pay);
                currentUser->paidAmount = pay;
                librarian.saveUsersToFile();
            }
            else if(choice==5) {
                currentUser->showBorrowingHistory();
            }
            else if (choice == 6) {
                librarian.saveBooksToFile();
                librarian.saveUsersToFile();
                break;
            } 
            else {
                cout << "Invalid choice. Try again.\n";
            }
        }
    }

    return 0;
}
