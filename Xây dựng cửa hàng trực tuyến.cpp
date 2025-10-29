#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
using namespace std;

// Hàm helper để format số với dấu chấm phân cách hàng nghìn
string formatNumber(double num) {
    stringstream ss;
    ss << fixed << setprecision(0) << num;
    string str = ss.str();    
    size_t dotPos = str.find('.');
    if (dotPos != string::npos) str = str.substr(0, dotPos);    
    int len = str.length();
    string result = "";
    for (int i = 0; i < len; i++) {
        if (i > 0 && (len - i) % 3 == 0) result += ".";
        result += str[i];
    }
    return result;
}

class Product {
private:
    string id, name, description;
    double price;
    int stock;
public:
    Product() : id(""), name(""), description(""), price(0.0), stock(0) {}
    Product(string i, string n, string d, double p, int s) : id(i), name(n), description(d), price(p), stock(s) {}
    string getId() const { return id; }
    string getName() const { return name; }
    string getDescription() const { return description; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }
    void setId(const string& i) { id = i; }
    void setName(const string& n) { name = n; }
    void setDescription(const string& d) { description = d; }
    void setPrice(double p) { price = p; }
    void setStock(int s) { stock = s; }
    void display() const {
        cout << "Ma san pham: " << id << endl << "Ten san pham: " << name << endl;
        cout << "Mo ta san pham: " << (description.length() > 20 ? description.substr(0, 20) + "..." : description) << endl;
        cout << "Gia san pham: " << formatNumber(price) << " VND" << endl << "So luong ton kho: " << stock << endl << "------------------------" << endl;
    }
};

struct Node { Product data; Node* next; };

class Store {
private:
    Node* head;
public:
    Store() : head(nullptr) {}
    ~Store() {
        while(head) { Node* tmp = head; head = head->next; delete tmp; }
    }
    Product* findById(const string& ID) {
        for (Node* temp = head; temp; temp = temp->next)
            if (temp->data.getId() == ID) return &(temp->data);
        return nullptr;
    }
    bool addProduct(const Product& p) {
        if (findById(p.getId())) { cout << "San pham da ton tai!" << endl; return false; }
        Node* n = new Node{p, nullptr};
        if (!head) head = n;
        else {
            Node* t = head;
            while (t->next) t = t->next;
            t->next = n;
        }
        cout << "Them san pham thanh cong!" << endl;
        return true;
    }
    bool editProduct(const string& id) {
        Product* p = findById(id);
        if (!p) { cout << "Khong tim thay san pham voi ID: " << id << endl; return false; }
        cout << "\n=== THONG TIN HIEN TAI ===" << endl; p->display();
        cout << "\n=== NHAP THONG TIN MOI ===" << endl;
        string tmp;
        cout << "Ma san pham moi (" << p->getId() << "): "; getline(cin, tmp);
        if (!tmp.empty()) p->setId(tmp);
        cout << "Ten san pham moi (" << p->getName() << "): "; getline(cin, tmp);
        if (!tmp.empty()) p->setName(tmp);
        cout << "Mo ta san pham moi (" << p->getDescription() << "): "; getline(cin, tmp);
        if (!tmp.empty()) p->setDescription(tmp);
        cout << "Gia san pham moi (" << p->getPrice() << "): "; getline(cin, tmp);
        if (!tmp.empty()) p->setPrice(stod(tmp));
        cout << "So luong ton kho moi (" << p->getStock() << "): "; getline(cin, tmp);
        if (!tmp.empty()) p->setStock(stoi(tmp));
        cout << "Cap nhat thanh cong!" << endl;
        return true;
    }
    bool removeProduct(const string& id) {
        for (Node* t = head, *prev = nullptr; t; prev = t, t = t->next) {
            if (t->data.getId() == id) {
                cout << "\n=== THONG TIN SAN PHAM SE XOA ===" << endl; t->data.display();
                char c; cout << "Ban co chac chan muon xoa? (y/n): "; cin >> c; cin.ignore();
                if (c == 'y' || c == 'Y') {
                    (prev ? prev->next : head) = t->next;
                    cout << "Da xoa san pham: " << t->data.getName() << endl;
                    delete t; return true;
                } else { cout << "Da huy thao tac xoa!" << endl; return false; }
            }
        }
        cout << "Khong tim thay san pham voi ID: " << id << endl;
        return false;
    }
    void displayProducts() const {
        if (!head) { cout << "Danh sach san pham trong!" << endl; return; }
        cout << "\n";
        cout << "========================================================================================================\n";
        cout << "| STT |  Ma SP  |     Ten San Pham      |           Mo Ta            |    Gia    | Ton Kho |\n";
        cout << "========================================================================================================\n";
        int stt = 1;
        for (Node* c = head; c; c = c->next, stt++) {
            string desc = c->data.getDescription();
            if (desc.length() > 26) desc = desc.substr(0, 23) + "...";
            string name = c->data.getName();
            if (name.length() > 22) name = name.substr(0, 19) + "...";
            cout << "| " << setw(3) << left << stt
                 << " | " << setw(7) << left << c->data.getId()
                 << " | " << setw(22) << left << name
                 << " | " << setw(26) << left << desc
                 << " | " << setw(9) << right << formatNumber(c->data.getPrice()) << " |"
                 << " " << setw(7) << right << c->data.getStock() << " |\n";
        }
        cout << "========================================================================================================\n";
    }
    void searchProductByName(const string& name) const {
        cout << "\n=== KET QUA TIM KIEM ===" << endl;
        bool found = false;
        for (Node* t = head; t; t = t->next) {
            if (t->data.getName().find(name) != string::npos) { t->data.display(); found = true; }
        }
        if (!found) cout << "Khong tim thay san pham voi ten: " << name << endl;
    }
};

class Customer {
private:
    string id, name, email, address, phone;
public:
    Customer() : id(""), name(""), email(""), address(""), phone("") {}
    string getId() const { return id; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    void input() {
        cout << "Nhap ma KH: "; getline(cin, id);
        cout << "Nhap ho ten: "; getline(cin, name);
        cout << "Nhap email: "; getline(cin, email);
        cout << "Nhap dia chi: "; getline(cin, address);
        cout << "Nhap sdt: "; getline(cin, phone);
    }
    void display() const {
        cout << "\n--- THONG TIN KHACH HANG ---\n" << "Ma KH: " << id << endl
             << "Ho ten: " << name << endl << "Email: " << email << endl
             << "Dia chi: " << address << endl << "SDT: " << phone << endl;
    }
};

class Cart {
private:
    Product items[50];
    int qty[50], count;
public:
    Cart() : count(0) {}
    bool add(const Product& p, int quantity) {
        if (count >= 50) { cout << "Gio hang da day!\n"; return false; }
        for (int i = 0; i < count; ++i) {
            if (items[i].getId() == p.getId()) { qty[i] += quantity;
                cout << "Da cong them " << quantity << " vao san pham trong gio.\n"; return true; }
        }
        items[count] = p; qty[count] = quantity; ++count;
        cout << "Da them '" << p.getName() << "' vao gio (" << quantity << ").\n"; return true;
    }
    bool remove(const string& id, Store& store) {
        for (int i = 0; i < count; ++i) {
            if (items[i].getId() == id) {
                Product* p = store.findById(id);
                if (p) p->setStock(p->getStock() + qty[i]);
                cout << "Da xoa san pham '" << items[i].getName() << "' khoi gio.\n";
                for (int j = i; j < count - 1; ++j) { items[j] = items[j+1]; qty[j] = qty[j+1]; }
                --count; return true;
            }
        }
        cout << "Khong tim thay san pham trong gio voi ID: " << id << endl; return false;
    }
    double getTotal() const {
        double s = 0.0;
        for (int i = 0; i < count; ++i) s += items[i].getPrice() * qty[i];
        return s;
    }
    void display() const {
        if (count == 0) { cout << "Gio hang trong.\n"; return; }
        cout << "\n";
        cout << "================================================================================\n";
        cout << "| STT |     Ten San Pham      |    Gia    | So luong |   Thanh Tien    |\n";
        cout << "================================================================================\n";
        for (int i = 0; i < count; ++i) {
            string name = items[i].getName();
            if (name.length() > 22) name = name.substr(0, 19) + "...";
            double totalItem = items[i].getPrice() * qty[i];
            cout << "| " << setw(3) << left << i+1
                 << " | " << setw(22) << left << name
                 << " | " << setw(9) << right << formatNumber(items[i].getPrice())
                 << " | " << setw(8) << right << qty[i]
                 << " | " << setw(14) << right << formatNumber(totalItem) << " |\n";
        }
        cout << "================================================================================\n";
        cout << "|                                                  Tong tien: " << setw(14) << right << formatNumber(getTotal()) << " VND |\n";
        cout << "================================================================================\n";
    }
    void clear() { count = 0; }
    bool empty() const { return count == 0; }
    int getCount() const { return count; }
    Product getProductAt(int i) const { return (i >= 0 && i < count) ? items[i] : Product(); }
    int getQtyAt(int i) const { return (i >= 0 && i < count) ? qty[i] : 0; }
};

struct OrderItem { Product productSnapshot; int quantity; };

class Review {
private:
    string productId, customerId, customerName, comment;
    int rating; // 1-5 stars
    string reviewDate;
public:
    Review() : productId(""), customerId(""), customerName(""), comment(""), rating(0), reviewDate("") {}
    Review(string pid, string cid, string cname, string cmt, int r, string date)
        : productId(pid), customerId(cid), customerName(cname), comment(cmt), rating(r), reviewDate(date) {}
       
    string getProductId() const { return productId; }
    string getCustomerId() const { return customerId; }
    string getCustomerName() const { return customerName; }
    string getComment() const { return comment; }
    int getRating() const { return rating; }
    string getReviewDate() const { return reviewDate; }
       
    void display() const {
        cout << "  " << customerName << " (" << customerId << "): ";
        for (int i = 0; i < rating; i++) cout << "★";
        for (int i = rating; i < 5; i++) cout << "☆";
        cout << " (" << rating << "/5)" << endl;
        if (!comment.empty()) cout << "    \"" << comment << "\"" << endl;
        cout << "    Ngay: " << reviewDate << endl;
    }
};

class Order {
private:
    string orderId, customerId, status;
    OrderItem items[50];
    int itemCount;
    double totalAmount;
public:
    Order() : itemCount(0), totalAmount(0.0), status("Chua xac nhan"), orderId(""), customerId("") {}
    void createFromCart(const Cart& cart, const string& custId, double finalTotal) {
        customerId = custId; totalAmount = finalTotal; itemCount = cart.getCount(); status = "Dang xu ly";
        for (int i = 0; i < itemCount; i++) { items[i].productSnapshot = cart.getProductAt(i); items[i].quantity = cart.getQtyAt(i); }
    }
    void display() const {
        cout << "\n--- Don hang: " << orderId << " ---" << endl << "Trang thai: " << status << endl
             << "Ma KH: " << customerId << endl << "Chi tiet san pham:" << endl;
        for (int i = 0; i < itemCount; i++) {
            cout << "  " << i + 1 << ". " << items[i].productSnapshot.getName() << " (ID: "
                 << items[i].productSnapshot.getId() << ")" << endl;
            cout << "     So luong: " << items[i].quantity << " | Don gia: " << items[i].productSnapshot.getPrice() << endl;
        }
        cout << "Tong thanh toan: " << totalAmount << " VND" << endl << "------------------------" << endl;
    }
    string getOrderId() const { return orderId; }
    string getCustomerId() const { return customerId; }
    string getStatus() const { return status; }
    int getItemCount() const { return itemCount; }
    OrderItem getItemAt(int index) const { return items[index]; }
    void setOrderId(const string& id) { orderId = id; }
    void setStatus(const string& newStatus) { status = newStatus; }
};

// Khai báo biến toàn cục
const int MAX_CUSTOMERS = 20, MAX_ORDERS = 100, MAX_REVIEWS = 1000;
Customer customers[MAX_CUSTOMERS];
Cart carts[MAX_CUSTOMERS];
Order allOrders[MAX_ORDERS];
Review reviews[MAX_REVIEWS];
int customerCount = 0, orderCount = 0, reviewCount = 0, nextOrderId = 1001;

// Hàm helper
int findCustomerIndexById(const string& id) {
    for (int i = 0; i < customerCount; ++i) if (customers[i].getId() == id) return i; return -1;
}

int findCustomerIndexByEmail(const string& email) {
    for (int i = 0; i < customerCount; ++i) if (customers[i].getEmail() == email) return i; return -1;
}

// Hàm hiển thị đánh giá sản phẩm
void displayProductReviews(const string& productId, Store& store) {
    Product* product = store.findById(productId);
    if (!product) {
        cout << "Khong tim thay san pham voi ID: " << productId << endl;
        return;
    }
    
    cout << "\n=== DANH GIA SAN PHAM: " << product->getName() << " ===" << endl;
    bool found = false;
    int totalRating = 0, reviewWithRating = 0;
    
    for (int i = 0; i < reviewCount; i++) {
        if (reviews[i].getProductId() == productId) {
            reviews[i].display();
            cout << endl;
            found = true;
            totalRating += reviews[i].getRating();
            reviewWithRating++;
        }
    }
    
    if (!found) {
        cout << "Chua co danh gia nao cho san pham nay." << endl;
    } else {
        double avgRating = (double)totalRating / reviewWithRating;
        cout << "=== TONG QUAN ===" << endl;
        cout << "Diem trung binh: " << fixed << setprecision(1) << avgRating << "/5 (" << reviewWithRating << " danh gia)" << endl;
    }
}

// Hàm thêm đánh giá sản phẩm
void addProductReview(int customerIndex, Store& store) {
    if (customerIndex == -1) {
        cout << "Ban can dang nhap de danh gia san pham." << endl;
        return;
    }
    
    cout << "Nhap ma san pham muon danh gia: ";
    string productId;
    getline(cin, productId);
    
    Product* product = store.findById(productId);
    if (!product) {
        cout << "Khong tim thay san pham voi ma: " << productId << endl;
        return;
    }
    
    // Kiểm tra xem khách hàng đã mua sản phẩm này chưa
    bool hasPurchased = false;
    for (int i = 0; i < orderCount; i++) {
        if (allOrders[i].getCustomerId() == customers[customerIndex].getId() && 
            allOrders[i].getStatus() == "Da giao") {
            for (int j = 0; j < allOrders[i].getItemCount(); j++) {
                if (allOrders[i].getItemAt(j).productSnapshot.getId() == productId) {
                    hasPurchased = true;
                    break;
                }
            }
        }
        if (hasPurchased) break;
    }
    
    if (!hasPurchased) {
        cout << "Ban can mua va nhan duoc san pham nay truoc khi danh gia." << endl;
        return;
    }
    
    cout << "\n=== DANH GIA SAN PHAM: " << product->getName() << " ===" << endl;
    
    int rating;
    do {
        cout << "Chon so sao (1-5): ";
        cin >> rating;
        cin.ignore();
        if (rating < 1 || rating > 5) {
            cout << "So sao phai tu 1 den 5!" << endl;
        }
    } while (rating < 1 || rating > 5);
    
    string comment;
    cout << "Nhap binh luan (co the bo trong): ";
    getline(cin, comment);
    
    // Lấy ngày hiện tại
    time_t now = time(0);
    tm* localTime = localtime(&now);
    string date = to_string(localTime->tm_mday) + "/" + 
                  to_string(localTime->tm_mon + 1) + "/" + 
                  to_string(localTime->tm_year + 1900);
    
    // Thêm đánh giá
    if (reviewCount < MAX_REVIEWS) {
        reviews[reviewCount] = Review(productId, 
                                    customers[customerIndex].getId(),
                                    customers[customerIndex].getName(),
                                    comment, rating, date);
        reviewCount++;
        cout << "Cam on ban da danh gia san pham!" << endl;
    } else {
        cout << "He thong danh gia da day." << endl;
    }
}

void displayMainMenu() {
    cout << "\n===== CUA HANG TRUC TUYEN =====\n"
         << "1. Quan ly san pham (Admin)\n"
         << "2. Dang ky khach hang\n"
         << "3. Dang nhap\n"
         << "4. Xem thong tin KH\n"
         << "5. Them san pham vao gio\n"
         << "6. Xoa san pham khoi gio\n"
         << "7. Xem gio hang\n"
         << "8. Thanh toan (Tao don hang)\n"
         << "9. Xem lich su don hang\n"
         << "10. Cap nhat trang thai don hang (Admin)\n"
         << "11. Danh gia san pham\n"
         << "12. Xem danh gia san pham\n"
         << "0. Thoat\nChon: ";
}

void displayStoreMenu() {
    cout << "\n--- QUAN LY SAN PHAM ---\n1. Them san pham\n2. Hien thi danh sach\n3. Sua san pham\n"
         << "4. Xoa san pham\n5. Tim kiem\n0. Tro ve\nChon: ";
}

int main() {
    Store store;
    int currentUser = -1, mainChoice;
    
    // Thêm một số sản phẩm mẫu
    store.addProduct(Product("SP001", "iPhone 15", "Dien thoai thong minh Apple", 25000000, 10));
    store.addProduct(Product("SP002", "Samsung Galaxy S24", "Dien thoai thong minh Samsung", 20000000, 15));
    store.addProduct(Product("SP003", "MacBook Air", "Laptop Apple M2", 35000000, 5));
    
    do {
        displayMainMenu();
        cin >> mainChoice;
        cin.ignore();
        
        if (mainChoice == 1) {
            int sm;
            do {
                displayStoreMenu();
                cin >> sm;
                cin.ignore();
                if (sm == 1) {
                    string id, name, desc; double price; int stock;
                    cout << "Nhap ma sp: "; getline(cin, id);
                    cout << "Nhap ten sp: "; getline(cin, name);
                    cout << "Nhap mo ta: "; getline(cin, desc);
                    cout << "Nhap gia: "; cin >> price;
                    cout << "Nhap ton kho: "; cin >> stock;
                    cin.ignore();
                    store.addProduct(Product(id, name, desc, price, stock));
                } else if (sm == 2) store.displayProducts();
                else if (sm == 3) { string id; cout << "Nhap ID sp can sua: "; getline(cin, id); store.editProduct(id); }
                else if (sm == 4) { string id; cout << "Nhap ID sp can xoa: "; getline(cin, id); store.removeProduct(id); }
                else if (sm == 5) { string q; cout << "Nhap ten tim kiem: "; getline(cin, q); store.searchProductByName(q); }
            } while (sm != 0);
        }
        else if (mainChoice == 2) {
            if (customerCount >= MAX_CUSTOMERS) cout << "Da dat gioi han!\n";
            else {
                Customer c; c.input();
                if (findCustomerIndexById(c.getId()) != -1) cout << "Ma KH da ton tai!\n";
                else { customers[customerCount] = c; carts[customerCount] = Cart();
                    cout << "Dang ky thanh cong! Ma KH: " << c.getId() << "\n"; ++customerCount; }
            }
        }
        else if (mainChoice == 3) {
            cout << "Dang nhap - Nhap email: "; string email; getline(cin, email);
            int idx = findCustomerIndexByEmail(email);
            if (idx == -1) cout << "Khong tim thay khach hang.\n";
            else { currentUser = idx; cout << "Dang nhap thanh cong. Xin chao " << customers[currentUser].getName() << "!\n"; }
        }
        else if (mainChoice == 4) {
            if (currentUser == -1) cout << "Ban chua dang nhap.\n";
            else customers[currentUser].display();
        }
        else if (mainChoice == 5) {
            if (currentUser == -1) cout << "Phai dang nhap.\n";
            else {
                store.displayProducts(); cout << "Nhap ID san pham can them: ";
                string id; getline(cin, id); Product* p = store.findById(id);
                if (!p) cout << "Khong tim thay san pham.\n";
                else {
                    cout << "Nhap so luong: "; int q; cin >> q; cin.ignore();
                    if (q <= 0) cout << "So luong phai > 0\n";
                    else if (q > p->getStock()) cout << "Khong du hang. Ton: " << p->getStock() << "\n";
                    else { p->setStock(p->getStock() - q); carts[currentUser].add(*p, q); }
                }
            }
        }
        else if (mainChoice == 6) {
            if (currentUser == -1) cout << "Phai dang nhap.\n";
            else { carts[currentUser].display(); cout << "Nhap ID san pham can xoa: ";
                   string id; getline(cin, id); carts[currentUser].remove(id, store); }
        }
        else if (mainChoice == 7) { if (currentUser == -1) cout << "Phai dang nhap.\n"; else carts[currentUser].display(); }
        else if (mainChoice == 8) {
            if (currentUser == -1) cout << "Phai dang nhap de thanh toan.\n";
            else if (carts[currentUser].empty()) cout << "Gio hang trong. Khong the thanh toan.\n";
            else if (orderCount >= MAX_ORDERS) cout << "He thong da qua tai don hang. Vui long thu lai sau.\n";
            else {
                double total = carts[currentUser].getTotal();
                cout << "Tong tien tam tinh: " << formatNumber(total) << " VND\n";
                cout << "Nhap ma giam gia (bo trong neu khong co): ";
                string discountCode; getline(cin, discountCode);
                if (discountCode == "SALE10") { total *= 0.9; cout << "Da ap dung giam gia 10%. Tong con lai: " << formatNumber(total) << " VND\n"; }
                else if (!discountCode.empty()) cout << "Ma giam gia khong hop le.\n";
                cout << "Tong tien can thanh toan: " << formatNumber(total) << " VND\nNhap so tien ban tra: ";
                double paid; cin >> paid; cin.ignore();
                if (paid < total) cout << "Thanh toan that bai! So tien khong du.\n";
                else {
                    allOrders[orderCount].setOrderId("HD" + to_string(nextOrderId));
                    allOrders[orderCount].createFromCart(carts[currentUser], customers[currentUser].getId(), total);
                    orderCount++; nextOrderId++; carts[currentUser].clear();
                    cout << "Thanh toan thanh cong! Da tao don hang voi ma: HD" << nextOrderId-1 << endl
                         << "Tien tra lai: " << formatNumber(paid - total) << " VND\nCam on ban da mua sam!\n";
                }
            }
        }
        else if (mainChoice == 9) {
            if (currentUser == -1) cout << "Ban chua dang nhap.\n";
            else {
                cout << "\n=== LICH SU DON HANG CUA " << customers[currentUser].getName().substr(0, 20) << " ===\n";
                bool foundOrder = false;
                for (int i = 0; i < orderCount; i++) {
                    if (allOrders[i].getCustomerId() == customers[currentUser].getId()) {
                        allOrders[i].display(); foundOrder = true;
                    }
                }
                if (!foundOrder) cout << "Ban chua co don hang nao.\n";
            }
        }
        else if (mainChoice == 10) {
            cout << "Nhap ma don hang (vi du: HD1001): "; string orderId; getline(cin, orderId);
            int idx = -1;
            for (int i = 0; i < orderCount; i++) if (allOrders[i].getOrderId() == orderId) { idx = i; break; }
            if (idx == -1) cout << "Khong tim thay don hang voi ma: " << orderId << endl;
            else {
                cout << "Don hang hien tai:"; allOrders[idx].display();
                cout << "Chon trang thai moi (1: Da giao, 2: Da huy): ";
                int sc; cin >> sc; cin.ignore();
                if (sc == 1) { allOrders[idx].setStatus("Da giao"); cout << "Da cap nhat trang thai -> Da giao.\n"; }
                else if (sc == 2) {
                    if (allOrders[idx].getStatus() != "Da huy") {
                        allOrders[idx].setStatus("Da huy");
                        cout << "Dang hoan tra so luong san pham vao kho..." << endl;
                        for (int i = 0; i < allOrders[idx].getItemCount(); i++) {
                            OrderItem item = allOrders[idx].getItemAt(i);
                            Product* p = store.findById(item.productSnapshot.getId());
                            if (p) { p->setStock(p->getStock() + item.quantity);
                                     cout << "  + Hoan " << item.quantity << " san pham '" << p->getName() << "'\n"; }
                        }
                        cout << "Da cap nhat trang thai -> Da huy va hoan kho.\n";
                    } else cout << "Don hang nay da bi huy truoc do.\n";
                } else cout << "Lua chon khong hop le.\n";
            }
        }
        else if (mainChoice == 11) {
            addProductReview(currentUser, store);
        }
        else if (mainChoice == 12) {
            cout << "Nhap ma san pham de xem danh gia: ";
            string productId;
            getline(cin, productId);
            displayProductReviews(productId, store);
        }
        
    } while (mainChoice != 0);
    cout << "Thoat chuong trinh.\n";
    return 0;
}

