#include <iostream>
#include <cstring>
using namespace std;

class String{
    char* data; // 存储串
    int maxSize; // 最大存储容量
    int curLength; // 串的长度
    void resize(int len); // 扩大数组空间
public:
    String(const char* str = nullptr); // 构造函数
    String(const String& str); // 拷贝构造函数
    ~String(){delete [] data;} // 析构函数
    int capacity() const {return maxSize;} // 最大存储容量
    int size() const {return curLength;} // 求串长度
    bool empty() const {return curLength == 0;} // 判空
    int compare(const String& s) const; // 比较当前串和串s的大小
    String subStr(int pos, int num) const; // 从pos位置开始取长度为num的子串
    int bfFind(const String& s, int pos = 0) const; // 朴素的模式匹配算法
    String& insert(int pos, const String& s); // 在pos位置插入串s
    String& erase(int pos, int num); // 删除从pos开始的num个字符
    const char* toCharStr() const {return data;} // 获取字符数组data
    int kmpFind(const String& t, int pos = 0); // 改进的模式匹配算法
    void getNext(const String& t, int *next); // 获取next数组
    void getNextVal(const String& t, int *nextVal); // 获取nextVal数组
    bool operator==(const String& str) const; // 重载==，判断两个串是否相等
    String& operator+(const String& str); // 重载+，用于串的连接
    String& operator=(const String& str); // 重载=，用于串间赋值
    char& operator[](int n) const; // 重载[]，通过下标取串中字符
    friend istream& operator>>(istream& cin, String& str); // 重载>>，用于输入串
    friend ostream& operator<<(ostream& cout, String& str); // 重载<<，用于输出串
};

class outOfRange:public exception{ // 用于检查范围的有效性
public:
    const char* what()const throw(){
        return "ERROR! OUT OF RANGE.\n";
    }
};

class badSize:public exception{ // 用于检查长度的有效性
public:
    const char* what()const throw(){
        return "ERROR! BAD SIZE.\n";
    }
};

String::String(const char* str) {
    maxSize = 2*strlen(str);
    data = new char[maxSize + 1]; // +1 for null terminator
    strcpy(data, str);
    curLength = strlen(data);
}

String::String(const String& str) {
    maxSize = str.maxSize;
    curLength = str.curLength;
    data = new char[maxSize + 1]; // +1 for null terminator
    strcpy(data, str.data);
}

int String::compare(const String& s) const {
    int i = 0;
    while(s.data[i] != '\0' || this->data[i] != '\0'){
        if(this->data[i] > s.data[i]) return 1; // 当前串大于s
        if(this->data[i] < s.data[i]) return -1; // 当前串小于s
        i++;
    }
    if (this->data[i] == '\0' && s.data[i] != '\0') return -1; // 当前串小于s
    if (this->data[i] != '\0' && s.data[i] == '\0') return 1; // 当前串大于s
    return 0; // 两个串相等
}

String String::subStr(int pos, int num) const {
    int i;
    String tmp("");
    if(pos > curLength || pos < 0) throw outOfRange();
    if(num < 0) throw badSize();
    if(num > curLength - pos) num = curLength - pos; // 限制num的大小
    delete [] tmp.data; // 释放tmp本来的存储空间
    tmp.maxSize = tmp.curLength = num;
    tmp.data = new char[num + 1]; // 申请大小为num + 1的空间
    for(i = 0; i < num; i++) // 长度为num的子串赋值给tmp
        tmp.data[i] = data[pos + i];
    tmp.data[i] = '\0'; // 添加字符串结束符
    return tmp;
}

String& String::insert(int pos, const String& s){
    if(pos > curLength || pos < 0) throw outOfRange();
    if(curLength + s.curLength > maxSize) resize(2*(curLength + s.curLength)); // 扩大数组空间
    for(int i = curLength; i >= pos; i--) data[i + s.curLength] = data[i]; // 将pos位置及之后的字符后移s.curLength个位置
    for(int j = 0; j < s.curLength; j++) data[pos + j] = s.data[j]; // 将s插入到pos位置
    curLength += s.curLength; // 更新当前串长度
    return *this;
}

String& String::erase(int pos, int num){
    if(pos > curLength || pos < 0) throw outOfRange();
    if(num < 0) throw badSize();
    if (num > curLength - pos)
        num = curLength - pos; // 限制num的大小
    for(int i = pos; i <= curLength - num; i++)
        data[i] = data[i + num]; // 将pos位置之后的字符前移num个位置
    curLength -= num; // 更新当前串长度
    return *this;
}

void String::resize(int len){
    maxSize = len;
    char* temp = new char[maxSize + 1];
    strcpy(temp, data); // 将原有数据复制到新空间
    delete[] data; // 释放原有空间
    data = temp; // 更新data指针
}

String& String::operator+(const String& str){
    if (curLength + str.size() > maxSize)
        resize(2*(curLength + str.size()));
    strcat(data, str.data); // 连接两个串
    curLength += str.size(); // 更新当前串长度
    return *this;
}

String& String::operator=(const String& str){
    if (this == &str) return *this; // 防止自赋值
    delete[] data; // 释放原有空间
    maxSize = str.maxSize;
    curLength = str.curLength;
    data = new char[maxSize + 1]; // +1 for null terminator
    strcpy(data, str.data); // 复制数据
    return *this;
}

bool String::operator==(const String& str) const {
    if (curLength != str.curLength) return false;
    return strcmp(data, str.data) ? false : true; // 使用strcmp比较两个串
}

inline char& String::operator[](int n) const {
    if (n < 0 || n >= curLength) throw outOfRange();
    else return data[n]; // 返回指定位置的字符
}

istream& operator>>(istream& cin, String& str) {
    char* temp = new char[1000]; // 假设最大输入长度为999
    cin >> temp;
    str.maxSize = 2 * strlen(temp);
    str.data = new char[str.maxSize + 1];
    strcpy(str.data, temp); // 复制输入的字符串
    str.curLength = strlen(temp);
    delete [] temp; // 释放临时空间
    return cin;
}

ostream& operator<<(ostream& cout, String& str) {
    cout << str.data; // 输出字符串
    return cout;
}

int String::bfFind(const String& s, int pos) const {
    int i = 0, j = 0;
    if(curLength < s.curLength) return -1; // 如果当前串长度小于s的长度，返回-1
    while (i < curLength && j < s.curLength){
        if(data[i] == s.data[j])
            i++, j++; // 如果当前字符相等，继续比较下一个字符
        else{
            i = i - j + 1; // 如果不相等，i回退到下一个可能的匹配位置
            j = 0; // j回到s的起始位置
        }
    }
    if(j >= s.curLength) return (i - s.curLength); // 如果j遍历完s，返回匹配的起始位置
    return -1; // 如果没有找到匹配，返回-1
}

int String::kmpFind(const String& t, int pos){
    if(t.curLength == 0) return 0;
    if(curLength < t.curLength) return -1; // 如果当前串长度小于t的长度，返回-1
    int i = 0, j = 0;
    int* next = new int[t.curLength]; // 创建next数组
    getNext(t, next);
    while(i < curLength && j < t.curLength){
        if(j == -1 || data[i] == t.data[j])
            i++, j++;
        else j = next[j]; // 如果不匹配，使用next数组跳过不必要的比较
    }
    delete [] next; // 释放next数组
    if(j >= t.curLength) return (i - t.curLength); // 如果j遍历完t，返回匹配的起始位置
    else return -1; // 如果没有找到匹配，返回-1
}

void String::getNext(const String& t, int *next){
    int i = 0, j = -1;
    next[0] = -1; // 初始化next数组
    while(i < t.curLength - 1){
        if((j == -1) || (t[i] == t[j])){
            ++i, ++j;
            next[i] = j; // 如果匹配成功，更新next数组
        }else j = next[j];
    }
}

void String::getNextVal(const String& t, int *nextVal){
    int i = 0, j = -1;
    nextVal[0] = -1;
    while(i < t.curLength - 1){
        if((j == -1) || (t[i] == t[j])){
            ++i, ++j;
            if(t[i] != t[j]) nextVal[i] = j;
            else nextVal[i] = nextVal[j];
        }else j = nextVal[j];
    }
}

int main(){
    cout << "=== String Class Test Suite ===" << endl;
    
    // Test 1: Constructors
    cout << "\n1. Testing Constructors:" << endl;
    String s1("Hello");
    String s2(s1);
    String s3("");
    cout << "s1: " << s1 << " (size: " << s1.size() << ")" << endl;
    cout << "s2 (copy): " << s2 << " (size: " << s2.size() << ")" << endl;
    cout << "s3 (empty): " << s3 << " (empty: " << s3.empty() << ")" << endl;
    
    // Test 2: Basic Functions
    cout << "\n2. Testing Basic Functions:" << endl;
    cout << "s1 capacity: " << s1.capacity() << endl;
    cout << "s1 as char*: " << s1.toCharStr() << endl;
    
    // Test 3: Comparison
    cout << "\n3. Testing Comparison:" << endl;
    String s4("Hello");
    String s5("World");
    cout << "s1 == s4: " << (s1 == s4) << endl;
    cout << "s1 compare s5: " << s1.compare(s5) << endl;
    
    // Test 4: Substring
    cout << "\n4. Testing Substring:" << endl;
    String sub = s1.subStr(1, 3);
    cout << "s1.subStr(1,3): " << sub << endl;
    
    // Test 5: Insert and Erase
    cout << "\n5. Testing Insert and Erase:" << endl;
    String s6("Hello");
    s6.insert(5, String(" World"));
    cout << "After insert: " << s6 << endl;
    s6.erase(5, 5);
    cout << "After erase: " << s6 << endl;
    
    // Test 6: Operators
    cout << "\n6. Testing Operators:" << endl;
    String s7("Hi");
    String s8(" there");
    s7 = s7 + s8;
    cout << "Concatenation: " << s7 << endl;
    cout << "s7[0]: " << s7[0] << endl;
    
    // Test 7: Pattern Matching
    cout << "\n7. Testing Pattern Matching:" << endl;
    String text("abcabcabcabc");
    String pattern("abcab");
    cout << "Text: " << text << endl;
    cout << "Pattern: " << pattern << endl;
    cout << "BF Find: " << text.bfFind(pattern) << endl;
    cout << "KMP Find: " << text.kmpFind(pattern) << endl;
    
    // Test 8: Next Array
    cout << "\n8. Testing Next Array:" << endl;
    int* next = new int[pattern.size()];
    text.getNext(pattern, next);
    cout << "Next array for pattern '" << pattern << "': ";
    for(int i = 0; i < pattern.size(); i++) {
        cout << next[i] << " ";
    }
    cout << endl;
    delete[] next;
    
    // Test 9: Exception Handling
    cout << "\n9. Testing Exception Handling:" << endl;
    try {
        s1[10]; // Should throw outOfRange
    } catch(const outOfRange& e) {
        cout << "Caught exception: " << e.what();
    }
    
    try {
        s1.subStr(0, -1); // Should throw badSize
    } catch(const badSize& e) {
        cout << "Caught exception: " << e.what();
    }
    
    // Test 10: Complex Pattern
    cout << "\n10. Testing Complex Pattern:" << endl;
    String complexText("ababcababa");
    String complexPattern("ababa");
    cout << "Complex text: " << complexText << endl;
    cout << "Complex pattern: " << complexPattern << endl;
    cout << "KMP Find: " << complexText.kmpFind(complexPattern) << endl;
    
    cout << "\n=== All Tests Completed ===" << endl;
    
    return 0;
}