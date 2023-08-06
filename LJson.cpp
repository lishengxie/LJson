#include "LJson.h"
#include <cstring>
#include <stdexcept>

namespace LJson {

LJson::JsonObject npos = JsonObject();

// Functions for JsonObject class
JsonObject* JsonObject::createNULL() {
    auto object = new JsonObject();
    object->type = T_NULL;
    return object;
}

JsonObject* JsonObject::createTrue() {
    auto object = new JsonObject();
    object->type = T_TRUE;
    return object;
}

JsonObject* JsonObject::createFalse() {
    auto object = new JsonObject();
    object->type = T_FALSE;
    return object;
}

JsonObject* JsonObject::createInt(int number) {
    auto object = new JsonObject();
    object->type = T_INT;
    object->valueInt = number;
    return object;
}

JsonObject* JsonObject::createFloat(double number) {
    auto object = new JsonObject();
    object->type = T_FLOAT;
    object->valueDouble = number;
    return object;
}

JsonObject* JsonObject::createString(const char *value) {
    auto object = new JsonObject();
    object->type = T_STRING;

    char *out = new char[strlen(value) + 1];
    strcpy(out, value);

    object->valueString = out;
    return object;
}

JsonObject* JsonObject::createArray() {
    auto object = new JsonObject();
    object->type = T_ARRAY;
    return object;
}

JsonObject* JsonObject::createObject() {
    auto object = new JsonObject();
    object->type = T_OBJECT;
    return object;
}

void JsonObject::addItem(const char* iKey, JsonObject* item) {
    if(item == nullptr){
        return;
    }
    if (this->type != T_OBJECT) {
        throw std::logic_error("must be json object to add key-value pair");
    }

    // set key for item
    char* out = new char[strlen(iKey) + 1];
    strcpy(out, iKey);
    item->key = out;

    // no child yet
    if (this->child == nullptr) {
        this->child = item;
    } else {
        // 尾插法, 检索更快
        JsonObject* pChild = this->child;
        while (pChild!= nullptr && pChild->next!=nullptr){
            pChild = pChild->next;
        }

        pChild->next = item;
        item->prev = pChild;
    }
}

void JsonObject::addItem(JsonObject* item) {
    if(item == nullptr){
        return;
    }
    if (this->type != T_ARRAY) {
        throw std::logic_error("must be json array to add item");
    }

    // no child yet
    if (this->child == nullptr) {
        this->child = item;
    } else {
        // 尾插法, 检索更快
        JsonObject* pChild = this->child;
        while (pChild!= nullptr && pChild->next!=nullptr){
            pChild = pChild->next;
        }

        pChild->next = item;
        item->prev = pChild;
    }
}

void JsonObject::addEmptySubObjectToObject(const char* iKey) {
    JsonObject *object = JsonObject::createObject();
    addItem(iKey, object);
}

void JsonObject::addEmptySubArrayToObject(const char *iKey) {
    JsonObject *object = JsonObject::createArray();
    addItem(iKey, object);
}

void JsonObject::addEmptySubObjectToArray() {
    JsonObject *object = JsonObject::createObject();
    addItem(object);
}

void JsonObject::addEmptySubArrayToArray() {
    JsonObject *object = JsonObject::createArray();
    addItem(object);
}

void JsonObject::add(const char *iKey, int number) {
    JsonObject *object = JsonObject::createInt(number);
    addItem(iKey, object);
}

void JsonObject::add(const char *iKey, double number){
    JsonObject *object = JsonObject::createFloat(number);
    addItem(iKey, object);
}

void JsonObject::add(const char *iKey, const char *strValue){
    JsonObject *object = JsonObject::createString(strValue);
    addItem(iKey, object);
}

void JsonObject::add(const char *iKey, bool boolValue) {
    JsonObject *object;
    if(boolValue) {
        object = JsonObject::createTrue();
    } else {
        object = JsonObject::createFalse();
    }
    addItem(iKey, object);
}

void JsonObject::add(const char *iKey, std::nullptr_t nullValue) {
    JsonObject *object = JsonObject::createNULL();
    addItem(iKey, object);
}

void JsonObject::add(int number) {
    JsonObject *object = JsonObject::createInt(number);
    addItem(object);
}

void JsonObject::add(double number) {
    JsonObject *object = JsonObject::createFloat(number);
    addItem(object);
}

void JsonObject::add(const char *strValue) {
    JsonObject *object = JsonObject::createString(strValue);
    addItem(object);
}

void JsonObject::add(bool boolValue) {
    JsonObject *object;
    if (boolValue){
        object = JsonObject::createTrue();
    } else {
        object = JsonObject::createFalse();
    }
    addItem(object);
}

void JsonObject::add(std::nullptr_t nullValue){
    JsonObject *object = JsonObject::createNULL();
    addItem(object);
}

JsonObject& JsonObject::operator=(int number) {
    if (*this == LJson::npos) {
        std::cerr << "can not assign to non-existent object\n";
        return *this;
    }
    if (this->type == T_OBJECT || this->type == T_ARRAY) {
        JsonObject::deleteNode(this->child);
        this->child = nullptr;
    }
    this->type = T_INT;
    this->valueInt = number;
    return *this;
}

JsonObject& JsonObject::operator=(double number) {
    if (*this == LJson::npos) {
        std::cerr << "can not assign to non-existent object\n";
        return *this;
    }
    if (this->type == T_OBJECT || this->type == T_ARRAY) {
        JsonObject::deleteNode(this->child);
        this->child = nullptr;
    }
    this->type = T_FLOAT;
    this->valueDouble = number;
    return *this;
}

JsonObject& JsonObject::operator=(const char* strValue) {
    if (*this == LJson::npos) {
        std::cerr << "can not assign to non-existent object\n";
        return *this;
    }
    if (this->type == T_OBJECT || this->type == T_ARRAY) {
        JsonObject::deleteNode(this->child);
        this->child = nullptr;
    }

    this->type = T_STRING;
    delete this->valueString;
    if (strValue != nullptr) {
        this->valueString = new char[strlen(strValue) + 1];
        strcpy(this->valueString, strValue);
    }

    return *this;
}

JsonObject& JsonObject::operator=(bool boolValue) {
    if (*this == LJson::npos) {
        std::cerr << "can not assign to non-existent object\n";
        return *this;
    }
    if (this->type == T_OBJECT || this->type == T_ARRAY) {
        JsonObject::deleteNode(this->child);
        this->child = nullptr;
    }
    if (boolValue) {
        this->type = T_TRUE;
    } else {
        this->type = T_FALSE;
    }
    return *this;
}

JsonObject& JsonObject::operator=(std::nullptr_t nullValue) {
    if (*this == LJson::npos) {
        std::cerr << "can not assign to non-existent object\n";
        return *this;
    }
    if (this->type == T_OBJECT || this->type == T_ARRAY) {
        JsonObject::deleteNode(this->child);
        this->child = nullptr;
    }
    this->type = T_NULL;
    return *this;
}

JsonObject& JsonObject::operator=(std::initializer_list<InitType> initList) {
    if (*this == LJson::npos) {
        std::cerr << "can not assign to non-existent object\n";
        return *this;
    }

    InitType initType = InitType(initList);
    initType.root->checkAndTransfer();

    if (this->type == T_OBJECT || this->type == T_ARRAY) {
        JsonObject::deleteNode(this->child);
        this->child = nullptr;
    }
    this->type = initType.root->type;
    this->valueInt = initType.root->valueInt;
    this->valueDouble = initType.root->valueDouble;

    if (initType.root->valueString != nullptr) {
        delete this->valueString;
        this->valueString = new char[strlen(initType.root->valueString) + 1];
        strcpy(this->valueString, initType.root->valueString);
    }
    this->child = initType.root->child;
    initType.root->child = nullptr;
    JsonObject::deleteSingleNode(initType.root);

    return *this;
}

void JsonObject::deleteNode(JsonObject *node) {
    JsonObject *nextNode;
    JsonObject *curNode = node;
    while (curNode != nullptr) {
        nextNode = curNode->next;
        if (curNode->child) {
            deleteNode(curNode->child);
        }
        if (curNode->valueString) {
            delete [](curNode->valueString);
            curNode->valueString = nullptr;
        }
        if (curNode->key) {
            delete [](curNode->key);
            curNode->key = nullptr;
        }
        delete curNode;
        curNode = nextNode;
    }
}

void JsonObject::remove(const char* iKey) {
    JsonObject* cur = &(*this)[iKey];
    if (*cur == LJson::npos){
        return;
    }
    // delete the item
    // if `cur` is the head of child list
    if (cur->prev == nullptr) {
        if (cur->next == nullptr) {
            this->child = nullptr;
        } else {
            this->child = cur->next;
        }
        delete cur;
    } else {
        cur->prev->next = cur->next;
        if (cur->next != nullptr) {
            cur->next->prev = cur->prev;
        }
    }
}

void JsonObject::remove(int idx) {
    JsonObject* cur = &(*this)[idx];
    if (*cur == LJson::npos){
        return;
    }
    // delete the item
    // if `cur` is the head of child list
    if (cur->prev == nullptr) {
        if (cur->next == nullptr) {
            this->child = nullptr;
        } else {
            this->child = cur->next;
        }
        delete cur;
    } else {
        cur->prev->next = cur->next;
        if (cur->next != nullptr) {
            cur->next->prev = cur->prev;
        }
    }
}

JsonObject& JsonObject::operator[] (const char* iKey) {
    JsonObject *cur = this->child;
    while (cur != nullptr) {
        if(strcmp(iKey, cur->key) == 0) {
            return *cur;
        }
        cur = cur->next;
    }
    JsonObject *object = JsonObject::createNULL();
    this->addItem(iKey, object);
    return *object;
}

JsonObject& JsonObject::operator[] (int idx) {
    if (this->type != T_ARRAY) {
        std::cerr << "Error: must be array object to use int idx.\n";
        return LJson::npos;
    }

    JsonObject *cur = this->child;
    for (int i=0; i<idx && cur!=nullptr; i++) {
        cur = cur->next;
    }
    
    if(cur == nullptr) {
        return LJson::npos;
    }

    return *cur;
}

string JsonObject::toString() {
    std::ostringstream os;

    switch (this->type) {
        case T_NULL: {
            os << "null";
            break;
        }
        case T_TRUE: {
            os << "true";
            break;
        }
        case T_FALSE: {
            os << "false";
            break;
        }
        case T_INT: {
            os << this->valueInt;
            break;
        }
        case T_FLOAT: {
            os << this->valueDouble;
            break;
        }
        case T_STRING: {
            os << '\"' << this->valueString << '\"';
            break;
        }
        case T_ARRAY: {
            os << '[';

            JsonObject* cur = this->child;
            while (cur != nullptr) {
                os << cur->toString();
                if (cur->next != nullptr) {
                    os << ",";
                }
                cur = cur -> next;
            }

            os << ']';
            break;
        }
        case T_OBJECT: {
            os << '{';

            JsonObject* cur = this->child;
            while (cur != nullptr) {
                os << '\"' << cur->key << '\"';
                os << ':';
                os << cur->toString();
                if (cur->next != nullptr) {
                    os << ',';
                }
                cur = cur -> next;
            }

            os << '}';
            break;
        }
        default:
            return "";
    }

    return os.str();
}

std::vector<string> JsonObject::getKeys() {
    std::vector<string> keysVec;
    JsonObject *cur = this->child;
    while ( cur != nullptr ) {
        keysVec.emplace_back(cur->key);
        cur = cur -> next;
    }
    return keysVec;
}

bool JsonObject::isInt() {
    return type == T_INT;
}

bool JsonObject::isBool() {
    return (type == T_TRUE) || (type==T_FALSE);
}

bool JsonObject::isNull() {
    return type == T_NULL;
}

bool JsonObject::isString() {
    return type == T_FLOAT;
}

bool JsonObject::isArray() {
    return type == T_ARRAY;
}

bool JsonObject::isObject() {
    return type == T_OBJECT;
}

int JsonObject::size() {
    if (this->type != T_OBJECT && this->type != T_ARRAY) {
        return 0;
    }
    JsonObject *cur = this->child;
    int length = 0;
    while(cur != nullptr) {
        length += 1;
        cur = cur->next;
    }
    return length;
}

int JsonObject::getInt() {
    return valueInt;
}
double JsonObject::getFloat() {
    return valueDouble;
}

const char* JsonObject::getString() {
    return valueString;
}

bool JsonObject::getBool() {
    if(type==T_TRUE)
        return true;
    else
        return false;
}

bool JsonObject::checkArrayIsObject() {
    if (this->type != T_ARRAY) {
        return false;
    }
    bool is_an_object = true;
    JsonObject *cur = this->child;
    while (cur != nullptr) {
        if (cur->type != T_ARRAY) {
            is_an_object = false;
            break;
        }
        if (cur->size() != 2) {
            is_an_object = false;
            break;
        }
        if (cur->child->type != T_STRING) {
            is_an_object = false;
            break;
        }
        cur = cur->next;
    }

    return is_an_object;
}

void JsonObject::transferArrayToObject() {
    this->type = T_OBJECT;
    JsonObject *cur = this->child;
    while (cur!= nullptr) {
        JsonObject *firstChild = cur->child;
        JsonObject *secondChild = cur->child->next;
        delete []cur->key;
        cur->key = new char[strlen(firstChild->valueString) + 1];
        strcpy(cur->key, firstChild->valueString);

        if (secondChild->type != T_ARRAY) {
            cur->type = secondChild->type;
            cur->valueInt = secondChild->valueInt;
            cur->valueDouble = secondChild->valueDouble;
            if (secondChild->valueString != nullptr) {
                delete []cur->valueString;
                cur->valueString = new char[strlen(secondChild->valueString) + 1];
                strcpy(cur->valueString, secondChild->valueString);
            }
            cur->child = nullptr;
            JsonObject::deleteSingleNode(firstChild);
            JsonObject::deleteSingleNode(secondChild);
        } else {
            if (secondChild->checkArrayIsObject()) {
                cur->type = T_OBJECT;
                secondChild->transferArrayToObject();
            } else {
                cur->type = T_ARRAY;
            }
            cur->child = secondChild->child;
            JsonObject::deleteSingleNode(secondChild);
            JsonObject::deleteSingleNode(firstChild);
        }

        cur = cur->next;
    }
}

void JsonObject::checkAndTransfer() {
    if (this->checkArrayIsObject()) {
        this->transferArrayToObject();
    }
    JsonObject *curNode = this->child;
    while (curNode != nullptr) {
        curNode->checkAndTransfer();
        curNode = curNode->next;
    }
}

void JsonObject::deleteSingleNode(JsonObject *node){
    if (node == nullptr || (*node) == LJson::npos) {
        return;
    }
    if (node->valueString != nullptr) {
        delete node->valueString;
    }
    if (node->key != nullptr) {
        delete node->key;
    }
    node->prev = nullptr;
    node->next = nullptr;
    node->child = nullptr;
    delete node;
}

// Function for JsonParser class
JsonObject* JsonParser::fromString(const char *value) {
    init(value);
    return parse();
}

void JsonParser::init(const char* value){
    m_str = value;
    m_idx = 0;
    trim_right();
}

void JsonParser::trim_right() {
    if(!m_str.empty()) {
        for(auto iter = m_str.rbegin(); iter != m_str.rend(); iter++){
            if(isspace(*iter) != 0) {
                m_str.erase(iter.base() - 1); /* 反向迭代器转为迭代器 */
            } else {
                break;
            }
        }
    }
}

void JsonParser::skip_comment() {
    if (m_str.compare(m_idx, 2, R"(//)") == 0) {
        while (true) {
            auto next_pos = m_str.find('\n', m_idx);
            if (next_pos == string::npos) {
                throw std::logic_error("invalid comment area!");
            }
            // 查看下一行是否还有注释
            m_idx = next_pos + 1;
            while (isspace(m_str[m_idx])) {
                m_idx++;
            }

            if (m_str.compare(m_idx, 2, R"(//)") != 0) {
                break;
            }
        }
    } else if (m_str.compare(m_idx, 2, R"(/*)") == 0) {
        auto next_pos = m_str.find(R"(*/)", m_idx);
        if (next_pos == string::npos) {
            throw std::logic_error("invalid comment area!");
        }
        m_idx = next_pos + 1;
        while (isspace(m_str[m_idx])) {
            m_idx++;
        }
    }
}

char JsonParser::get_next_token() {
    while (std::isspace(m_str[m_idx])) {
        m_idx++;
    }
    if (m_idx >= m_str.size())
        throw std::logic_error("unexpected character in parse json");
    // 跳过注释
    skip_comment();
    return m_str[m_idx];
}

bool JsonParser::is_esc_consumed(size_t pos) {
    size_t end_pos = pos;
    while(pos != string::npos && m_str[pos] == '\\') {
        pos--;
    }
    auto cnt = end_pos - pos;
    return cnt % 2 == 0;
}

JsonObject* JsonParser::parse() {
    char token = get_next_token();

    if (token == 'n') {
        return parseNULL();
    }
    if (token == 't' || token == 'f') {
        return parseBool();
    }
    if (token == '-' || std::isdigit(token)) {
        return parseNumber();
    }
    if (token == '\"') {
        return parseString();
    }
    if (token == '[') {
        return parseArray();
    }
    if (token == '{') {
        return parseObject();
    }

    throw std::logic_error("unexpected character in parsing json.");
}

JsonObject* JsonParser::parseNULL() {
    static JsonObject o_instance;
    if (m_str.compare(m_idx, 4, "null") == 0) {
        m_idx += 4;
        return o_instance.createNULL();
    }

    throw std::logic_error("parse null error.");
}

JsonObject* JsonParser::parseBool() {
    static JsonObject o_instance;
    if (m_str.compare(m_idx, 4, "true") == 0) {
        m_idx += 4;
        return o_instance.createTrue();
    }
    if (m_str.compare(m_idx, 5, "false") == 0) {
        m_idx += 5;
        return o_instance.createFalse();
    }

    throw std::logic_error("parse bool error.");
}

JsonObject* JsonParser::parseNumber() {
    static JsonObject o_instance;

    auto pos = m_idx;
    // 整数部分
    if (m_str[m_idx] == '-') {
        m_idx++;
    }
    if (std::isdigit(m_str[m_idx])){ 
        while (std::isdigit(m_str[m_idx]))
            m_idx++;
    } else {
        throw std::logic_error("invalid character in number");
    }

    if (m_str[m_idx] != '.') {
        return o_instance.createInt((int)strtol(m_str.c_str() + pos, nullptr, 10));
    }

    // decimal part
    if (m_str[m_idx] == '.') {
        m_idx++;
        if (!std::isdigit(m_str[m_idx])) {
            throw std::logic_error("at least one digit required in parse float part!");
        }
        while (std::isdigit(m_str[m_idx]))
            m_idx++;
    }

    return o_instance.createFloat(strtod(m_str.c_str() + pos, nullptr));
}

JsonObject* JsonParser::parseString() {
    static JsonObject o_instance;
    string value = getString();
    return o_instance.createString(value.c_str());
}

string JsonParser::getString() {
    get_next_token();
    auto pre_pos = ++m_idx;
    auto pos = m_str.find('\"', m_idx);
    if (pos != string::npos) {
        // 判断是否为转义的双引号
        while (true) {
            if (m_str[pos - 1] != '\\') {
                break;
            }

            // 如果是转义字符
            if (is_esc_consumed(pos - 1)) {
                break;
            }
            pos = m_str.find('"', pos + 1);
            if (pos == string::npos) {
                throw std::logic_error(R"(expected left '"' in parse string)");
            }
        }
        m_idx = pos + 1;
        string value = m_str.substr(pre_pos, pos - pre_pos);

        return value;
    }

    throw std::logic_error("parse bool error.");
}

JsonObject* JsonParser::parseArray() {
    static JsonObject o_instance;
    JsonObject *oArray = o_instance.createArray();
    m_idx++;

    char ch = get_next_token();
    if (ch == ']') {
        m_idx++;
        return oArray;
    }

    while (true) {
        oArray->addItem(parse());
        ch = get_next_token();
        if (ch == ']') {
            m_idx++;
            break;
        }

        if (ch != ',') {
            throw std::logic_error("expected ',' in parse list");
        }
        
        m_idx++;
    }

    return oArray;
}

JsonObject* JsonParser::parseObject() {
    static JsonObject o_instance;
    JsonObject *oObject = o_instance.createObject();
    m_idx++;

    char ch = get_next_token();
    if (ch == '}') {
        m_idx++;
        return oObject;
    }

    while (true) {
        string key = getString();
        ch = get_next_token();
        if (ch != ':') {
            throw std::logic_error("expected ':' in parse list");
        }
        m_idx++;

        oObject->addItem(key.c_str(), parse());
        ch = get_next_token();
        if (ch == '}') {
            m_idx++;
            break;
        }
        if (ch != ',')//没有结束，此时必须为逗号
        {
            throw std::logic_error("expected ',' in parse dict");
        }
        m_idx++;
    }

    return oObject;
}

// Function for Json class
void Json::addItem(const char *iKey, LJson::JsonObject *item) {
    root->addItem(iKey, item);
}

void Json::addItem(LJson::JsonObject *item) {
    root->addItem(item);
}

void Json::remove(const char* iKey){
    root->remove(iKey);
}
void Json::remove(int idx) {
    root->remove(idx);
}

void Json::addEmptySubObjectToObject(const char* iKey) {
    root->addEmptySubObjectToObject(iKey);
}

void Json::addEmptySubArrayToObject(const char* iKey) {
    root->addEmptySubArrayToObject(iKey);
}

void Json::addEmptySubObjectToArray() {
    root->addEmptySubObjectToArray();
}

void Json::addEmptySubArrayToArray() {
    root->addEmptySubArrayToArray();
}

void Json::add(const char *iKey, int number) {
    root->add(iKey, number);
}

void Json::add(const char *iKey, double number) {
    root->add(iKey, number);
}

void Json::add(const char *iKey, const char *strValue) {
    root->add(iKey, strValue);
}

void Json::add(const char *iKey, bool boolValue) {
    root->add(iKey, boolValue);
}

void Json::add(const char *iKey, std::nullptr_t nullValue) {
    root->add(iKey, nullValue);
}

void Json::add(int number) {
    root->add(number);
}

void Json::add(double number) {
    root->add(number);
}

void Json::add(const char *strValue) {
    root->add(strValue);
}

void Json::add(bool boolValue) {
    root->add(boolValue);
}

void Json::add(std::nullptr_t nullValue) {
    root->add(nullValue);
}

Json& Json::operator=(std::initializer_list<JsonObject::InitType> initList) {
    root->operator=(initList);
    return *this;
}

JsonObject& Json::operator[](const char *iKey) {
    return (*root)[iKey];
}

JsonObject& Json::operator[](int idx) {
    return (*root)[idx];
}

string Json::toString() {
    return root->toString();
}

std::vector<string> Json::getKeys() {
    return root->getKeys();
}

}