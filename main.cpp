#include <iostream>
#include "LJson.h"

int main() {
    using namespace std;
    using namespace LJson;

    /*
    auto oJson = Json("{\"refresh_interval\":60,"
                     "\"test_float\":[18.0, 10.0, 5.0],"
                     "\"test_int\":[135355, -1844674407370955161, -935375],"
                     "\"timeout\":12.5,"
                     "\"dynamic_loading\":["
                     "{"
                     "\"so_path\":\"plugins/User.so\", \"load\":false, \"version\":1,"
                     "\"cmd\":["
                     "{\"cmd\":2001, \"class\":\"neb::CmdUserLogin\"},"
                     "{\"cmd\":2003, \"class\":\"neb::CmdUserLogout\"}"
                     "],"
                     "\"module\":["
                     "{\"path\":\"im/user/login\", \"class\":\"neb::ModuleLogin\"},"
                     "{\"path\":\"im/user/logout\", \"class\":\"neb::ModuleLogout\"}"
                     "]"
                     "},"
                     "{"
                     "\"so_path\":\"plugins/ChatMsg.so\", \"load\":false, \"version\":1,"
                     "\"cmd\":["
                     "{\"cmd\":2001, \"class\":\"neb::CmdChat\"}"
                     "],"
                     "\"module\":[]"
                     "}"
                     "]"
                     "}");
    cout << oJson.toString() << endl;
    cout << "------------------------------------------------------------------------" << endl;

    oJson.addEmptySubObject("depend");
    oJson["depend"].add("nebula", "https://github.com/Bwar/Nebula");
    oJson["depend"].addEmptySubArray("bootstrap");
    oJson["depend"]["bootstrap"].add("BEACON");
    oJson["depend"]["bootstrap"].add("LOGIC");
    oJson["depend"]["bootstrap"].add("LOGGER");
    oJson["depend"]["bootstrap"].add("INTERFACE");
    oJson["depend"]["bootstrap"].add("ACCESS");

    oJson["depend"] = R"({"base":{"pp":0,"qq":""},"id":32,"name":"fda"})";


    oJson["depend"] = "hello";
    oJson["depend"] = {"hello", {"world", {1, 1.5}}};

    cout << oJson.toString() << endl;
    cout << "------------------------------------------------------------------------" << endl;
    */
    /*
    auto keys = oJson.getKeys();
    for (auto& key:keys){
        cout << key << endl;
    }

    cout << oJson.toString() << endl;
    cout << "------------------------------------------------------------------------" << endl;

    oJson["depend"]["bootstrap"].removeItem(2);
    cout << oJson.toString() << endl;
    cout << "------------------------------------------------------------------------" << endl;

    oJson["depend"].removeItem("bootstrap");
    cout << oJson.toString() << endl;
    cout << "------------------------------------------------------------------------" << endl;

    JsonParser parser;
    JsonObject *appendObject = parser.fromString(R"({"base":{"pp":0,"qq":""},"id":32,"name":"fda"} )");
    oJson.addItem("add", appendObject);
    cout << oJson.toString() << endl;
    cout << "------------------------------------------------------------------------" << endl;
     */

    auto oJson = Json();
    oJson = {
            {"float",12.3},
            {"int",23},
            {"bool",true},
            {"str","string"},
            {"null",nullptr},
            {"arrFloat",{2.3,8.9,2.3}},
            {"arrBool",{true,false}},
            {"arrObj",{{"xls",{1,0,true, nullptr,"test", {{"testKey", "testVal"}}}},{"lover","xls"}}},
            {"empty-obj",{}}
    };

    double floatVal = oJson["float"].getFloat();
    int intVal = oJson["int"].getInt();
    bool boolVal = oJson["bool"].getBool();
    const char* strVal = oJson["str"].getString();
    double floatValInArray = oJson["arrFloat"][0].getFloat();
    int arrSize = oJson["arrFloat"].size();
    auto oJsonKeys = oJson.getKeys();

    cout << floatVal << endl;
    cout << intVal << endl;
    cout << (boolVal ? "true" : "false") << endl;
    cout << strVal << endl;
    cout << floatValInArray << endl;
    cout << arrSize << endl;
    for (auto& key: oJsonKeys) {
        cout << key << " ";
    }
    cout << endl;
    cout << endl;

    cout << oJson << endl;
    cout << oJson.toString() << endl;
    cout << endl;

    auto oJson2 = Json(R"({"base":{"pp":0,"qq":[-1.9, 1, null, false]},"id":32,"name":"fda"} )");
    cout << oJson2 << endl;

    // 添加
    oJson2["stdStr"]="koko";
    oJson2["strOld"]="ok";
    oJson2["null"]=nullptr;
    oJson2["bool"]=true;
    oJson2["int"]=1000;
    oJson2["double"]=1.43;
    oJson2["base"]["qq"].add(10);
    oJson2["base"]["qq"].add(10.5);
    oJson2["base"]["qq"].add(true);
    oJson2["base"]["qq"].add("test string");
    oJson2["base"]["qq"].add(nullptr);
    oJson2["base"].addEmptySubObjectToObject("emptyObj");
    oJson2["base"]["qq"].addEmptySubObjectToArray();
    oJson2["base"].addEmptySubArrayToObject("emptyArray");
    oJson2["base"]["qq"].addEmptySubArrayToArray();
    oJson2["base"]["emptyArray"].addEmptySubObjectToArray();
    cout << oJson2 << endl;
    cout << endl;

    // 修改
    oJson2["int"] = 10.5;
    oJson2["double"] = 1.5;
    oJson2["bool"] = false;
    oJson2["name"] = "xls";
    oJson2["oJson1"] = {
            {"float",12.3},
            {"int",23},
            {"bool",true},
            {"str","string"},
            {"null",nullptr},
            {"arrFloat",{2.3,8.9,2.3}},
            {"arrBool",{true,false}},
            {"arrObj",{{"xls",{1,0,true, nullptr,"test", {{"testKey", "testVal"}}}},{"lover","xls"}}},
            {"empty-obj",{}}
    };
    cout << oJson2 << endl;

    oJson2.remove("oJson1");
    oJson2["base"]["emptyArray"].remove(0);
    cout << oJson2 << endl;

    return 0;
}