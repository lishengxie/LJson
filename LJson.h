#ifndef LJSON_H
#define LJSON_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <initializer_list>

namespace LJson {

using std::string;

// JsonObject的类型
enum JsonType {
    T_FALSE=0, T_TRUE, T_NULL, T_INT, T_FLOAT, T_STRING, T_ARRAY, T_OBJECT
};

class JsonObject {
private:
    struct InitType;
    /* next和prev分别指向前一个对象和后一个对象 */
    JsonObject *next, *prev;
    /* Object和Array类型对象需要设置child指针 */
    JsonObject *child;
    /* Object的类型 */
    JsonType type;
    /* String对象的值 */
    char *valueString;
    /* 整数对象的值 */
    int valueInt;
    /* 浮点数的值 */
    double valueDouble;
    /* 键值对的键 */
    char *key;

    friend class Json;

public:
    JsonObject() {
        next = nullptr;
        prev = nullptr;
        child = nullptr;
        valueString = nullptr;
        valueInt = 0;
        valueDouble = 0;
        key = nullptr;
    }
    ~JsonObject() = default;

    bool operator==(const JsonObject& old) {
        return &old==this;
    }

    bool operator!=(const JsonObject& old) {
        return &old!=this;
    }

    friend std::ostream& operator << (std::ostream& os, JsonObject& object) {
        os << object.toString();
        return os;
    }
    
    // CREATE
    static JsonObject* createNULL();
    static JsonObject* createTrue();
    static JsonObject* createFalse();
    static JsonObject* createInt(int number);
    static JsonObject* createFloat(double number);
    static JsonObject* createString(const char* value);
    static JsonObject* createArray();
    static JsonObject* createObject();
    // TODO: 避免循环插入，如将一个节点的子节点再插入该节点等
    /* Add item to the object (the current object is T_OBJECT) */
    void addItem(const char* iKey, JsonObject* item);
    /* Add item to the object (the current object is T_ARRAY) */
    void addItem(JsonObject* item);
    void addEmptySubObjectToObject(const char* iKey);
    void addEmptySubArrayToObject(const char* iKey);
    void addEmptySubObjectToArray();
    void addEmptySubArrayToArray();
    void add(const char *iKey, int number);
    void add(const char *iKey, double number);
    void add(const char *iKey, const char *strValue);
    void add(const char *iKey, bool boolValue);
    void add(const char *iKey, std::nullptr_t nullValue);
    void add(int number);
    void add(double number);
    void add(const char *strValue);
    void add(bool boolValue);
    void add(std::nullptr_t nullValue);

    // DELETE
    static void deleteNode(JsonObject *node);
    void remove(const char* iKey);
    void remove(int idx);

    // UPDATE
    // 赋值操作符假设json被赋值的json对象已经存在
    JsonObject& operator=(int number);
    JsonObject& operator=(double number);
    JsonObject& operator=(const char* strValue);
    JsonObject& operator=(bool boolValue);
    JsonObject& operator=(std::nullptr_t nullValue);
    JsonObject& operator=(std::initializer_list<InitType> initList);

    // READ
    JsonObject& operator[] (const char* iKey);
    JsonObject& operator[] (int idx);

    // Other util function
    // to string
    string toString();
    // return the keys
    std::vector<string> getKeys();

    bool isInt();
    bool isBool();
    bool isNull();
    bool isString();
    bool isArray();
    bool isObject();
    int size();
    int getInt();
    double getFloat();
    const char* getString();
    bool getBool();

private:
    // 从initializer_list初始化来的都是ARRAY节点, 判断是否是OBJECT节点类型并进行转换
    // 判断标准为：ARRAY的每个子节点都是一个长度为2的ARRAY，且每个子节点的第一个子节点都是STRING
    bool checkArrayIsObject();
    // 将当前对象由ARRAY转为OBJECT
    void transferArrayToObject();
    void checkAndTransfer();

    static void deleteSingleNode(JsonObject *node);

    struct InitType {
        JsonObject *root;
        JsonType type;
        InitType(int number) {
            root = JsonObject::createInt(number);
            type = T_INT;
        }
        InitType(double number) {
            root = JsonObject::createFloat(number);
            type = T_FLOAT;
        }
        InitType(bool boolValue) {
            if (boolValue) {
                root = JsonObject::createTrue();
                type = T_TRUE;
            } else {
                root = JsonObject::createFalse();
                type = T_FALSE;
            }
        }
        InitType(std::nullptr_t nullValue) {
            root = JsonObject::createNULL();
            type = T_NULL;
        }
        InitType(const char* strValue) {
            root = JsonObject::createString(strValue);
            type = T_STRING;
        }
        InitType(std::initializer_list<InitType> initList) {
            root = JsonObject::createArray();
            for (auto iter=initList.begin(); iter!=initList.end(); iter++) {
                root->addItem(iter->root);
            }
        }
    };
};


class JsonParser {
public:
    JsonParser() = default;

    JsonObject* fromString(const char* value);

private:
    void init(const char* value);
    void trim_right();
    void skip_comment();
    char get_next_token();
    bool is_esc_consumed(size_t pos);

    JsonObject* parse();
    JsonObject* parseNULL();
    JsonObject* parseBool();
    JsonObject* parseNumber();
    JsonObject* parseString();
    JsonObject* parseArray();
    JsonObject* parseObject();

private:
    string m_str;
    size_t m_idx{};

    string getString();
};

class Json{
private:
    JsonObject *root;
    JsonParser parser;

public:
    Json() {
        root = JsonObject::createObject();
    }
    explicit Json(const char *value) : Json() {
        JsonObject::deleteNode(root);
        root = parser.fromString(value);
    }
    explicit Json(JsonObject* object) {
        root = object;
    }
    ~Json() {
        JsonObject::deleteNode(root);
    }

    friend std::ostream& operator << (std::ostream& os, Json& object) {
        os << object.root->toString();
        return os;
    }

    void addItem(const char* iKey, JsonObject* item);
    void addItem(JsonObject* item);
    void addEmptySubObjectToObject(const char* iKey);
    void addEmptySubArrayToObject(const char* iKey);
    void addEmptySubObjectToArray();
    void addEmptySubArrayToArray();
    void add(const char *iKey, int number);
    void add(const char *iKey, double number);
    void add(const char *iKey, const char *strValue);
    void add(const char *iKey, bool boolValue);
    void add(const char *iKey, std::nullptr_t nullValue);
    void add(int number);
    void add(double number);
    void add(const char *strValue);
    void add(bool boolValue);
    void add(std::nullptr_t nullValue);

    void remove(const char* iKey);
    void remove(int idx);

    Json& operator=(std::initializer_list<JsonObject::InitType> initList);

    JsonObject& operator[] (const char* iKey);
    JsonObject& operator[] (int idx);
    string toString();

    std::vector<string> getKeys();
};

extern JsonObject npos;

}

#endif