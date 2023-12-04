#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cstring>

using namespace std;

class Book {
public:
    char title[50];
    char author[50];
    int id;
    bool available;

    void displayBook() {
        cout << "ID: " << id << "\nTitle: " << title << "\nAuthor: " << author << "\nAvailable: " << (available ? "Yes" : "No") << "\n\n";
    }

    void inputBook() {
        cout << "Enter Book ID: ";
        cin >> id;
        cout << "Enter Book Title: ";
        cin.ignore();
        cin.getline(title, 50);
        cout << "Enter Author Name: ";
        cin.getline(author, 50);
        available = true;
    }
};

class Library {
public:
    void addBook(Book b) {
        ofstream outFile("library.txt", ios::app | ios::binary);
        outFile.write(reinterpret_cast<char*>(&b), sizeof(Book));
        outFile.close();
    }

    void displayAllBooks() {
        ifstream inFile("library.txt", ios::binary);
        if (!inFile) {
            cout << "File not found!" << endl;
            return;
        }

        Book b;
        cout << "\n\t\t\tLibrary Management System\n\n";
        cout << setw(5) << "ID" << setw(30) << "Title" << setw(20) << "Author" << setw(10) << "Available" << "\n\n";
        while (inFile.read(reinterpret_cast<char*>(&b), sizeof(Book))) {
            b.displayBook();
        }
        inFile.close();
    }

    void borrowBook(int bookID) {
        fstream file("library.txt", ios::in | ios::out | ios::binary);
        if (!file) {
            cout << "File not found!" << endl;
            return;
        }

        Book b;
        bool found = false;
        while (file.read(reinterpret_cast<char*>(&b), sizeof(Book))) {
            if (b.id == bookID && b.available) {
                found = true;
                int pos = -1 * static_cast<int>(sizeof(Book));
                file.seekp(pos, ios::cur);
                b.available = false;
                file.write(reinterpret_cast<char*>(&b), sizeof(Book));
                cout << "Book borrowed successfully!" << endl;
                break;
            }
        }
        if (!found) {
            cout << "Book not found or not available for borrowing." << endl;
        }
        file.close();
    }

    void returnBook(int bookID) {
        fstream file("library.txt", ios::in | ios::out | ios::binary);
        if (!file) {
            cout << "File not found!" << endl;
            return;
        }

        Book b;
        bool found = false;
        while (file.read(reinterpret_cast<char*>(&b), sizeof(Book))) {
            if (b.id == bookID && !b.available) {
                found = true;
                int pos = -1 * static_cast<int>(sizeof(Book));
                file.seekp(pos, ios::cur);
                b.available = true;
                file.write(reinterpret_cast<char*>(&b), sizeof(Book));
                cout << "Book returned successfully!" << endl;
                break;
            }
        }
        if (!found) {
            cout << "Book not found or already available." << endl;
        }
        file.close();
    }
};

int main() {
    Library library;
    Book book;
    int choice, bookID;

    do {
        cout << "\n\t\t\tLibrary Management System\n\n";
        cout << "1. Add a Book\n";
        cout << "2. Display All Books\n";
        cout << "3. Borrow a Book\n";
        cout << "4. Return a Book\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                book.inputBook();
                library.addBook(book);
                cout << "Book added successfully!\n";
                break;
            case 2:
                library.displayAllBooks();
                break;
            case 3:
                cout << "Enter the ID of the book you want to borrow: ";
                cin >> bookID;
                library.borrowBook(bookID);
                break;
            case 4:
                cout << "Enter the ID of the book you want to return: ";
                cin >> bookID;
                library.returnBook(bookID);
                break;
            case 0:
                cout << "Exiting the program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please enter a valid option.\n";
        }
    } while (choice != 0);

    return 0;
}