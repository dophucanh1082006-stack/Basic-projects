#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits> 
using namespace std;

// --- Data Structures ---
struct Book {
    string id;
    string title;
    int year;
};

struct Node {
    Book data;
    Node* next;
};

// --- Function Prototypes ---
void importBooksFromExternalFile(Node*& head); //Them sach tu file co san
Node* createNode(const Book& book);
void addNode(Node*& head, const Book& book);
void themSachMoi(Node*& head);
Node* read_file(const string& filename);
bool write_file(const string& filename, Node* head);
int countNodes(Node* head);
void tinhTongSoSach(Node* head);
void quickSort(Node*& head);
void timKiemTheoTenSach(Node* head, const string& tenTimKiem);
void displayBooks(Node* head);
void freeLinkedList(Node*& head);

// --- Implementation ---

Node* createNode(const Book& book) {
    Node* newNode = new Node;
    newNode->data = book;
    newNode->next = nullptr;
    return newNode;
}

void addNode(Node*& head, const Book& book) {
    Node* newNode = createNode(book);
    if (head == nullptr) {
        head = newNode;
        return;
    }
    Node* current = head;
    while (current->next != nullptr) {
        current = current->next;
    }
    current->next = newNode;
}

void themSachMoi(Node*& head) {
    Book newBook;
    cout << "\n=== THEM SACH MOI ===" << endl;

    cout << "Nhap ma sach: ";
    getline(cin, newBook.id);

    cout << "Nhap ten sach: ";
    getline(cin, newBook.title);

    cout << "Nhap nam xuat ban: ";
    while(!(cin >> newBook.year)){ 
        cout << "Nam khong hop le. Nhap lai: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    addNode(head, newBook);
    cout << "Da them sach thanh cong!" << endl;
}

Node* read_file(const string& filename) {
    Node* head = nullptr;
    ifstream file(filename);

    if (!file.is_open()) {
        ofstream outfile(filename);
        outfile.close();
        return nullptr;
    }

    string line;
    int bookCount = 0;

    while (getline(file, line)) {
        stringstream ss(line);
        string id, title, year_str;

        // Format assumed: id,title,year
        getline(ss, id, ',');
        getline(ss, title, ',');
        getline(ss, year_str);

        if (!id.empty() && !title.empty() && !year_str.empty()) {
            try {
                int year = stoi(year_str);
                Book book = {id, title, year};
                addNode(head, book);
                bookCount++;
            } catch (const exception& e) {
            }
        }
    }

    file.close();
    if(bookCount > 0) cout << "Da doc " << bookCount << " cuon sach tu file." << endl;
    return head;
}

// Hàm nh?p sách t? file bên ngoài (Import)
void importBooksFromExternalFile(Node*& head) {
    string filename;
    cout << "\n=== IMPORT SACH TU FILE ===" << endl;
    cout << "Nhap ten file (vd: data_cu.txt, backup.dat): ";
    getline(cin, filename); // Ð?c tên file t? ngý?i dùng

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Loi: Khong tim thay file hoac khong the mo file '" << filename << "'!" << endl;
        return;
    }

    string line;
    int successCount = 0;
    int errorCount = 0;

    cout << "Dang doc file..." << endl;

    while (getline(file, line)) {
        // B? qua d?ng tr?ng
        if (line.empty()) continue;

        stringstream ss(line);
        string id, title, year_str;

        // Ð?nh d?ng mong ð?i: id,title,year
        if (getline(ss, id, ',') && getline(ss, title, ',') && getline(ss, year_str)) {
            try {
                int year = stoi(year_str);
                Book newBook = {id, title, year};
                
                // Thêm vào danh sách hi?n t?i
                addNode(head, newBook); 
                successCount++;
            } catch (const exception& e) {
                // L?i n?u nãm không ph?i là s?
                errorCount++;
            }
        } else {
            // L?i n?u d?ng không ðúng ð?nh d?ng d?u ph?y
            errorCount++;
        }
    }

    file.close();

    cout << "--- KET QUA IMPORT ---" << endl;
    cout << "Da them thanh cong: " << successCount << " cuon sach." << endl;
    if (errorCount > 0) {
        cout << "Bo qua: " << errorCount << " dong do sai dinh dang." << endl;
    }
    cout << "======================" << endl;
}

bool write_file(const string& filename, Node* head) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Loi: Khong the tao file " << filename << endl;
        return false;
    }

    Node* current = head;
    int bookCount = 0;
    while (current != nullptr) {
        file << current->data.id << "," << current->data.title << "," << current->data.year << endl;
        current = current->next;
        bookCount++;
    }

    file.close();
    cout << "Da ghi " << bookCount << " cuon sach vao file." << endl;
    return true;
}

int countNodes(Node* head) {
    int count = 0;
    Node* current = head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

void tinhTongSoSach(Node* head) {
    int total = countNodes(head);
    cout << "\n=== THONG KE THU VIEN ===" << endl;
    cout << "Tong so sach: " << total << " cuon" << endl;

    if (total > 0) {
        Node* current = head;
        int minYear = current->data.year;
        int maxYear = current->data.year;

        while (current != nullptr) {
            if (current->data.year < minYear) minYear = current->data.year;
            if (current->data.year > maxYear) maxYear = current->data.year;
            current = current->next;
        }
        cout << "Nam xuat ban som nhat: " << minYear << endl;
        cout << "Nam xuat ban muon nhat: " << maxYear << endl;
    }
    cout << "=========================" << endl;
}

// --- Quick Sort Helper Functions ---
Node* partition(Node* low, Node* high, Node*& newLow, Node*& newHigh) {
    Node* pivot = high;
    Node* prev = nullptr;
    Node* cur = low;
    Node* tail = pivot;

    while (cur != pivot) {
        if (cur->data.year < pivot->data.year) {
            if (newLow == nullptr) newLow = cur;
            prev = cur;
            cur = cur->next;
        } else {
            if (prev != nullptr) prev->next = cur->next;
            Node* temp = cur->next;
            cur->next = nullptr;
            tail->next = cur;
            tail = cur;
            cur = temp;
        }
    }
    if (newLow == nullptr) newLow = pivot;
    newHigh = tail;
    return pivot;
}

Node* quickSortRecur(Node* low, Node* high) {
    if (low == nullptr || low == high) return low;

    Node* newLow = nullptr;
    Node* newHigh = nullptr;
    Node* pivot = partition(low, high, newLow, newHigh);

    if (newLow != pivot) {
        Node* temp = newLow;
        while (temp->next != pivot) temp = temp->next;
        temp->next = nullptr;

        newLow = quickSortRecur(newLow, temp);

        temp = newLow;
        while (temp->next != nullptr) temp = temp->next;
        temp->next = pivot;
    }
    pivot->next = quickSortRecur(pivot->next, newHigh);
    return newLow;
}

void quickSort(Node*& head) {
    if (head == nullptr) return;
    Node* tail = head;
    while (tail->next != nullptr) tail = tail->next;
    head = quickSortRecur(head, tail);
    cout << "Da sap xep sach theo nam xuat ban (Tang dan)." << endl;
}

void timKiemTheoTenSach(Node* head, const string& tenTimKiem) {
    if (head == nullptr) {
        cout << "Danh sach trong!" << endl;
        return;
    }

    Node* current = head;
    int soLuongTimThay = 0;
    string tenTimKiemLower = tenTimKiem;

    transform(tenTimKiemLower.begin(), tenTimKiemLower.end(), tenTimKiemLower.begin(), ::tolower);

    cout << "\n--- KET QUA TIM KIEM (\"" << tenTimKiem << "\") ---\n";
    cout << left << setw(10) << "Ma sach" << setw(40) << "Ten sach" << setw(15) << "Nam XB" << endl;
    cout << string(65, '-') << endl;

    while (current != nullptr) {
        string tenSachLower = current->data.title;
        transform(tenSachLower.begin(), tenSachLower.end(), tenSachLower.begin(), ::tolower);

        if (tenSachLower.find(tenTimKiemLower) != string::npos) {
            cout << left << setw(10) << current->data.id
                 << setw(40) << current->data.title
                 << setw(15) << current->data.year << endl;
            soLuongTimThay++;
        }
        current = current->next;
    }

    if (soLuongTimThay == 0) {
        cout << "Khong tim thay sach nao." << endl;
    }
    cout << string(65, '-') << endl;
}

void displayBooks(Node* head) {
    if (head == nullptr) {
        cout << "Danh sach trong!" << endl;
        return;
    }
    cout << "\n--- DANH SACH SACH ---\n";
    cout << left << setw(10) << "Ma sach" << setw(40) << "Ten sach" << setw(15) << "Nam XB" << endl;
    cout << string(65, '-') << endl;

    Node* current = head;
    int count = 0;
    while (current != nullptr) {
        cout << left << setw(10) << current->data.id
             << setw(40) << current->data.title
             << setw(15) << current->data.year << endl;
        current = current->next;
        count++;
    }
    cout << string(65, '-') << endl;
    cout << "Tong cong: " << count << " cuon." << endl;
}

void freeLinkedList(Node*& head) {
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
    head = nullptr;
}

// --- Main Menu Helper ---
void showMenu() {
    cout << "\n--- QUAN LY SACH ---" << endl;
    cout << "1. Them sach moi (Nhap tay)" << endl;
    cout << "2. Tim kiem sach theo ten" << endl;
    cout << "3. Hien thi tat ca sach" << endl;
    cout << "4. Sap xep sach theo nam (QuickSort)" << endl;
    cout << "5. Xem thong ke" << endl;
    cout << "6. Import sach tu file khac" << endl; // <--- NEW OPTION
    cout << "0. Luu va Thoat" << endl;
    cout << "--------------------" << endl;
}

int main() {
    const string FILENAME = "sach_data.txt";
    Node* listBook = nullptr;

    listBook = read_file(FILENAME);

    int choice = 0;
    do {
        showMenu();
        cout << "Nhap lua chon cua ban: ";

        if (!(cin >> choice)) {
            cout << "Loi: Vui long nhap so!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1:
            themSachMoi(listBook);
            break;
        case 2: {
            string keyword;
            cout << "Nhap ten sach can tim: ";
            getline(cin, keyword);
            timKiemTheoTenSach(listBook, keyword);
            break;
        }
        case 3:
            displayBooks(listBook);
            break;
        case 4:
            quickSort(listBook);
            displayBooks(listBook); 
            break;
        case 5:
            tinhTongSoSach(listBook);
            break;
        case 6: 
            importBooksFromExternalFile(listBook);
            break;
        case 0:
            cout << "Dang luu du lieu..." << endl;
            write_file(FILENAME, listBook);
            freeLinkedList(listBook);
            cout << "Tam biet!" << endl;
            break;
        default:
            cout << "Lua chon khong hop le!" << endl;
        }

    } while (choice != 0);

    return 0;
}

