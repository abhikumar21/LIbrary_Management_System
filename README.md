# Library_Management_System
This is a C++ program for a Library Management System. It allows users (students and faculty) to borrow and return books, pay fines, and view their borrowing history. The system enforces borrowing rules, calculates fines for overdue books, and stores data in text files.
The Librarian can manage adding new books, updating books, removing books from the database, adding new users, deleting old users.

To run the program:

1. Extract the contents of the zip file to a folder.
2. Open a terminal and navigate to the folder containing main.cpp.
3. Compile the code using the following command: g++ main.cpp -o library_management
4. Run the compiled program: ./library_management


Follow the on-screen instructions:

Enter your User ID to log in(given in users.txt file).

Use the menu to borrow books, return books, pay fines, or view your borrowing history.

Files included in the zip:
1. main.cpp: The main program file.
2. books.txt: Stores the list of books (created automatically if it doesn't exist).
3. users.txt: Stores user information (created automatically if it doesn't exist).
4. borrowed_books.txt: Tracks currently borrowed books (created automatically if it doesn't exist).
5. borrowing_history.txt: Logs borrowing and return history (created automatically if it doesn't exist).

Borrowing Rules:

1. Students can borrow up to 3 books and must return them within 15 days. Overdue fines: Rs 10 per day after 15 days.
2. Faculty can borrow up to 5 books and must return them within 60 days. Overdue fines: not applicable.
