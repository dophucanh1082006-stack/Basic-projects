#include <iostream>
#include <cstring>
using namespace std;

struct Book {
    string name;
    Book* next;
};

struct Author {
    string name;
    Book* child;
    Author* next;
};

struct Category {
    string name;
    Author* child;
    Category* next;
};

//Cac ham tao
Category* createCategory(const string& name) {
    Category* cat = new Category;
    cat->name = name;
    cat->child = nullptr;
    cat->next = nullptr;
    return cat;
}

Author* createAuthor(const string& name) {
    Author* auth = new Author;
    auth->name = name;
    auth->child = nullptr;
    auth->next = nullptr;
    return auth;
}

Book* createBook(const string& name) {
    Book* book = new Book;
    book->name = name;
    book->next = nullptr;
    return book;
}

//Them tac gia theo the loai
void addAuthorToCategory(Category* head, const string& cat, const string& authName)
{
    Category* currentCat = head;
    while (currentCat != nullptr && currentCat->name != cat)
    {
        currentCat = currentCat->next;
    }
    if (currentCat == nullptr) cout <<"Category not found\n";

    Author* newAuthor = createAuthor(authName);

    if (currentCat->child == nullptr)
    {
        currentCat->child = newAuthor;
    }
    else
    {
        Author* temp = currentCat->child;
        while(temp->next != nullptr)
        {
            temp= temp->next;
        }
        temp->next = newAuthor;
    }
}

//Them sach theo tac gia
void addBookToAuthor(Author* head, const string& authName,const string& bookName)
{
    Author* currentAuthor = head;
    while (currentAuthor != nullptr && currentAuthor->name != authName)
    {
        currentAuthor = currentAuthor->next;
    }
    if (currentAuthor == nullptr) cout<< "Khong tim thay tac gia\n";

    Book* newBook = createBook(bookName);
    if (currentAuthor->child == nullptr)
    {
        currentAuthor->child= newBook;
    }
    else
    {
        Book* temp= currentAuthor->child;
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }
        temp->next = newBook;
    }
}

//In sach theo tac gia
void printBooksOfAuthor(Author* pAuthor)
{
    Book* currentBook = pAuthor->child;
    cout<<"Tac gia "<< pAuthor->name<< " co nhung cuon sach sau:\n";
    while (currentBook != nullptr)
    {
        cout<<currentBook->name<<endl;
        currentBook= currentBook->next;
    }
}

//In sach theo the loai
void printBooksOfCategory(Category* pCategory)
{
    Author* currentAuthor = pCategory->child;
    cout<<"The loai "<< pCategory->name<< " co cac sach sau:\n";
    while (currentAuthor != nullptr)
    {
        Book* currentBook = currentAuthor->child;
        cout<< "Tac gia "<< currentAuthor->name<<" :"<<endl;
        while (currentBook != nullptr)
        {
            cout<<currentBook->name<<endl;
            currentBook= currentBook->next;
        }
        currentAuthor= currentAuthor->next;
    }
}

//Them the loai moi vao thu vien
void addCategory(Category* Head, const string& name)
{
    Category* currentCat = Head;
    Category* newCat = createCategory(name);
    while (currentCat->next != nullptr)
    {
        currentCat = currentCat->next;
    }
    currentCat->next = newCat;
}

//Tìm kiếm sách
bool TimViTri(Category* head, string tuKhoa) {
    bool found = false;
    Category* cat = head;
    while (cat != nullptr) {
        Author* auth = cat->child;
        while (auth != nullptr) {
            Book* book = auth->child;
            while (book != nullptr) {
                if (book->name == tuKhoa) {
                    cout << "Đường đi: "
                         << cat->name << " > "
                         << auth->name << " > "
                         << book->name << endl;
                    return true;
                }
                book = book->next;
            }
            auth = auth->next;
        }
        cat = cat->next;
    }
    if (!found) {
        cout << "Không tìm thấy sách " << tuKhoa << " trong thư viện.\n";
    }
    return found;
}

// Hàm xóa sách
void deleteBook(Category*& head, const string& authname,const string& bookname ) {
    Category* cat = head;
    while (cat!=nullptr) {
        Author* author = cat->child;
        Author* prevAuthor = nullptr;
        while (author!=nullptr) {
            if (author->name == authname) {
                Book* book = author->child;
                Book* prevBook = nullptr;
                while (book!=nullptr) {
                    if (book->name == bookname) {
                        if (prevBook!=nullptr) 
                             prevBook->next = book->next;
                        else author->child = book->next;
                        delete book;
                        cout << "Đã xóa sách \"" << bookname << "\" của tác giả " << authname << endl;
                        return;
                    }
                    prevBook = book;
                    book = book->next;
                }
            }
            author = author->next;
        }
        cat = cat->next;
    }
    cout << "Không tìm thấy sách \"" << bookname << "\" của tác giả " << authname << endl;
}

//Xóa tác giả khỏi chủ đề
void deleteAuthorInCategory(Category*& head, const string& categoryName, const string& authorName) {
    Category* currentCat = head;
    Category* prevCat = nullptr;
    
    while (currentCat != nullptr && currentCat->name != categoryName) {
        prevCat = currentCat;
        currentCat = currentCat->next;
    }
    
    if (currentCat == nullptr) {
        cout << "Loi: Khong tim thay chu de '" << categoryName << "'\n";
        return;
    }
    
    Author* currentAuthor = currentCat->child;
    Author* prevAuthor = nullptr;
    
    while (currentAuthor != nullptr && currentAuthor->name != authorName) {
        prevAuthor = currentAuthor;
        currentAuthor = currentAuthor->next;
    }
    
    if (currentAuthor == nullptr) {
        cout << "Loi: Khong tim thay tac gia '" << authorName << "' trong chu de '" << categoryName << "'\n";
        return;
    }
    
    Book* currentBook = currentAuthor->child;
    while (currentBook != nullptr) {
        Book* nextBook = currentBook->next;
        delete currentBook;
        currentBook = nextBook;
    }
    
    if (prevAuthor == nullptr) {
        currentCat->child = currentAuthor->next;
    } else {
        prevAuthor->next = currentAuthor->next;
    }
    
    delete currentAuthor;
    cout << "Da xoa tac gia '" << authorName << "' khoi chu de '" << categoryName << "' thanh cong!\n";
}

// Hàm xóa chủ đề khỏ thư viện
void deleteCategory(Category*& head, const string& categoryName) {
    if (head == nullptr) {
        cout << "Thu vien trong!\n";
        return;
    }
    
    Category* currentCat = head;
    Category* prevCat = nullptr;
    
    while (currentCat != nullptr && currentCat->name != categoryName) {
        prevCat = currentCat;
        currentCat = currentCat->next;
    }
    
    if (currentCat == nullptr) {
        cout << "Loi: Khong tim thay chu de '" << categoryName << "'\n";
        return;
    }
    
    Author* currentAuthor = currentCat->child;
    while (currentAuthor != nullptr) {
        Author* nextAuthor = currentAuthor->next;
        
        Book* currentBook = currentAuthor->child;
        while (currentBook != nullptr) {
            Book* nextBook = currentBook->next;
            delete currentBook;
            currentBook = nextBook;
        }
        
        delete currentAuthor;
        currentAuthor = nextAuthor;
    }
    
    if (prevCat == nullptr) {
        head = currentCat->next;
    } else {
        prevCat->next = currentCat->next;
    }
    
    delete currentCat;
    cout << "Da xoa chu de '" << categoryName << "' va toan bo noi dung thanh cong!\n";
}


//Ham print tat ca (test)
void printAll(Category* head) {
    Category* cat = head;
    while (cat != nullptr) {
        cout << "\n=== Category: " << cat->name << " ===\n";
        Author* auth = cat->child;
        while (auth != nullptr) {
            printBooksOfAuthor(auth);
            auth = auth->next;
        }
        cat = cat->next;
    }
}

int main()
{
    Category* scienceFiction = createCategory("Science Fiction");
    Category* thriller = createCategory("Thriller");
    Category* romance = createCategory("Romance");

    //Noi danh sach Category
    scienceFiction->next = thriller;
    thriller->next = romance;

    //Them tac gia
    addAuthorToCategory(scienceFiction, "Science Fiction", "Isaac Asimov");
    addAuthorToCategory(scienceFiction, "Science Fiction", "Liu Cixin");

    addAuthorToCategory(scienceFiction, "Thriller", "Dan Brown");
    addAuthorToCategory(scienceFiction, "Thriller", "Gillian Flynn");

    addAuthorToCategory(scienceFiction, "Romance", "Jane Austen");

    //Them sach
    //Isaac Asimov
    addBookToAuthor(scienceFiction->child, "Isaac Asimov", "Foundation");
    addBookToAuthor(scienceFiction->child, "Isaac Asimov", "I, Robot");
    addBookToAuthor(scienceFiction->child, "Isaac Asimov", "The Caves of Steel");

    //Liu Cixin
    addBookToAuthor(scienceFiction->child, "Liu Cixin", "The Three-Body Problem");
    addBookToAuthor(scienceFiction->child, "Liu Cixin", "The Dark Forest");
    addBookToAuthor(scienceFiction->child, "Liu Cixin", "Death s End");

    //Dan Brown
    addBookToAuthor(thriller->child, "Dan Brown", "The Da Vinci Code");
    addBookToAuthor(thriller->child, "Dan Brown", "Angels & Demons");

    //Gillian Flynn
    addBookToAuthor(thriller->child, "Gillian Flynn", "Gone Girl");
    addBookToAuthor(thriller->child, "Gillian Flynn", "Dark Places");
    addBookToAuthor(thriller->child, "Gillian Flynn", "Sharp Objects");

    //Jane Austen
    addBookToAuthor(romance->child, "Jane Austen", "Pride and Prejudice");
    addBookToAuthor(romance->child, "Jane Austen", "Sense and Sensibility");
    addBookToAuthor(romance->child, "Jane Austen", "Emma");

    //Tìm vị trítrí
    TimViTri(scienceFiction,"Emma" );
    TimViTri(scienceFiction,"Gone Girl" );
    TimViTri(scienceFiction,"One piece" );
    
    //xóa sách
    deleteBook(scienceFiction,"Dan Brown", "Angels & Demons" );
    //xóa tác giả 
    deleteAuthorInCategory(scienceFiction,"Romance","Jane Austen");
    
    //xóa chủ đề khỏi thư viện
    deleteCategory(scienceFiction,"Romance");
    //printBooksOfCategory(scienceFiction);
    //printBooksOfAuthor(romance->child);
    printAll(scienceFiction);
}
