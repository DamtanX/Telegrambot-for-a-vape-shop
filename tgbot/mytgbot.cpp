//#include "functionality.h"
#include "functionality.h"

//numirating data from tables
enum {
    ClientNameSurname = 0,
    Age = 1,
    CallNumber = 2,
    Email = 3,
    PreferedTastes = 4,
    PreferedBrands = 5,
    PreferedNicotinPercent = 6,
    AverageExpenses = 7,
    PurchasesFrequency = 8,
    PreferedContactWay = 9,
    Adress = 10,
    Region = 11,
    KnewAboutUsFrom = 12,
    BoughtBefore = 13,
    BoughtFromUs = 14,
    ClientID = 15

};
enum
{
    Clients = 0,
    Goods = 1,
    preferences = 2,
    Add_Client = 3,
    Show_Clients = 4,
    Change_Client = 5,
    Delete_Client = 6
};


bool delclient = false;

//CLIENTS
std::vector<int> MyPref;
std::vector<int> AddClientsMyPref;
std::vector<std::string> addClients_Data;

//GOODS
std::vector<int> MyPrefGoods = { 0, 2, 3, 4, 5, 6, 7 };

//FINISHED GOODS
std::vector<int> MyPrefFinGoods = { 0, 2, 3, 4, 5, 6, 7, 8 };


int profitdays = 10;
//int SWprofitdays = 10;


int main() {

    


    try {
        // Open a database in read-write mode
        SQLite::Database db("test.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        

        SQLite::Statement find_user(db, "SELECT COUNT(*) FROM owners WHERE TG_ID = :userId");
        

        // Insert some data
        SQLite::Statement insert(db, "INSERT INTO owners (TG_ID) VALUES (?);");
        //  insert.bind(1,43);
        //  insert.exec();  // Executes the INSERT statement

          // Retrieve data
         SQLite::Statement query(db, "SELECT * FROM owners;");
   
         SQLite::Statement clients_data(db, "SELECT * FROM clients");


        // Loop through the results and print them
        while (query.executeStep()) {
            int id = query.getColumn(0);
            // std::string name = query.getColumn(1);
          long long int tg_id = query.getColumn(1);
            std::cout << "ID: " << id << ", TG:  " << tg_id << std::endl;
        }
    
      
        //creating tg bot

    TgBot::Bot bot("YOUR_BOT_TOKEN");
   
    //making functionality 
    std::unordered_map<int64_t, bool> Admin;

    //creating buttons

    

    //start buttons
    bot.getEvents().onCommand("start", [&](TgBot::Message::Ptr message) {

        SQLite::Database db("test.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Statement admin_data(db, u8"SELECT * FROM owners WHERE Permission = 1");
        std::vector<int> admins;
        while (admin_data.executeStep())
        {
            admins.push_back(admin_data.getColumn(1).getInt());
        }
        Admin[message->chat->id] = false;
        for (auto &el : admins)
        {
            if (el == message->chat->id)
            {
                Admin[message->chat->id] = true;
            }
        }

        if (Admin[message->chat->id]) {
            std::cout << SWprofitdaysFun(profitdays, 0) << std::endl;
            try {
                TgBot::KeyboardButton::Ptr btn1(new TgBot::KeyboardButton());
                btn1->text = u8"Клиенты";

                TgBot::KeyboardButton::Ptr btn2(new TgBot::KeyboardButton());
                btn2->text = u8"Товар";

                TgBot::KeyboardButton::Ptr btn3(new TgBot::KeyboardButton());
                btn3->text = u8"Настройки";

                TgBot::ReplyKeyboardMarkup::Ptr keyboard(new TgBot::ReplyKeyboardMarkup());
                keyboard->resizeKeyboard = true;  // Makes buttons fit screen width
                keyboard->keyboard.push_back({ btn1, btn2 });  // Row 1
                keyboard->keyboard.push_back({ btn3 });       // Row 2


                //std::cerr << message->chat->id <<message->chat->description<<std::endl;
                bot.getApi().sendMessage(
                    message->chat->id,
                    u8"Выбери действие: ",
                    false, 0, keyboard);
            }
            catch (std::exception& e) {
                //bot.getApi().sendMessage(user_id, u8"Неверный ввод, нужен числовой параметр для цен и количеств.");
                //goodsAddMode[user_id] = false;
                std::cerr << message->chat->id << " something went wrong\n";
            }
        }
        else {
            bot.getApi().sendMessage(
                message->chat->id,
                u8"You are not an admin");
        }

        //CreateButton client("client", message->chat->id);
        });

   

    std::unordered_map<int64_t, bool> deleteMode;
    std::unordered_map<int64_t, bool> CustomClientColumnsMode;
    std::unordered_map<int64_t, bool> AddMode;
    std::unordered_map<int64_t, bool> ChangeMode;
    int clientIdtochange;
    std::string changecolumn = "";
    int changecounter = 0;
    std::vector<int> ClientPref;
    long int user_id;


    std::unordered_map<int64_t, bool> goodsdeleteMode;
    std::unordered_map<int64_t, bool> goodsAddMode;
    std::unordered_map<int64_t, bool> sold;
    std::unordered_map<int64_t, bool> goodsChangeMode;
    int goodschangecounter = 0;
    int goodIdtochange;
    std::string goodchangecolumn = "";
    std::vector<std::string> goodsdata_toaddd = {};
    int addgoodscounter = 0;
    int soldonemoretime = 0;
    std::string sell_id = "";
    std::string sell_cost = "";
    bool sell_costbool = false;


    std::unordered_map<int64_t, bool> fingoodsAddMode;
    int addfingoodscounter = 0;
    std::vector<std::string> fingoodsdata_toaddd = {};


    std::unordered_map<int64_t, bool> finishedgoodsdeleteMode;

    std::unordered_map<int64_t, bool> profitperiod;

    //query
    bot.getEvents().onCallbackQuery([&](TgBot::CallbackQuery::Ptr query) {
        if (query->data == "button1_pressed") {
            if (AddMode[user_id]) {
                bot.getApi().sendMessage(query->message->chat->id, " - ");

                addClients_Data.push_back(" - ");

                if (AddClientsMyPref.empty())
                {


                    std::string tcolums = "";
                    std::string tvalues = "";

                    for (auto& el : addClients_Data)
                    {
                        tvalues += '\'' + el + "\', ";
                    }
                    tvalues.pop_back();
                    tvalues.pop_back();

                    for (auto& el : MyPref) {
                        if (el == 15)
                        {
                            continue;
                        }
                        std::string ttempt;
                        SQLite::Statement clients_data(db, "SELECT * FROM clients");
                        while (clients_data.executeStep()) {
                            ttempt = clients_data.getColumnName(el);

                        }
                        clients_data.reset();

                        tcolums += '\'' + ttempt + "\', ";
                    }
                    tcolums.pop_back();
                    tcolums.pop_back();

                    SQLite::Statement ADDUSER(db, "INSERT INTO clients (" + tcolums + ") VALUES (" + tvalues + ");");
                    ADDUSER.exec();

                    AddMode[user_id] = false;
                    bot.getApi().sendMessage(user_id, u8"Успешно добавлен.\nназад: /start");


                }
                else if (!AddClientsMyPref.empty())
                {
                    TgBot::InlineKeyboardMarkup::Ptr keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();
                    AddClientFun(keyboard);
                    std::string temp;

                    SQLite::Statement clients_data(db, "SELECT * FROM clients");
                    while (clients_data.executeStep()) {
                        temp = clients_data.getColumnName(AddClientsMyPref.at(0));

                    }
                    clients_data.reset();

                    bot.getApi().sendMessage(user_id, temp, false, 0, keyboard);

                    AddClientsMyPref.erase(AddClientsMyPref.begin());
                }
            }
        }
        if (query->data == "vape_pressed") {
            try {
                goodsdata_toaddd.push_back("1");
                goodsAddMode[user_id] = true;
                TgBot::InlineKeyboardMarkup::Ptr keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();
                AddGoodsFun(keyboard);
                std::string temp;

                SQLite::Statement goods_data(db, "SELECT * FROM goods");
                while (goods_data.executeStep()) {
                    temp = goods_data.getColumnName(MyPrefGoods.at(0));

                }
                goods_data.reset();

                bot.getApi().sendMessage(query->message->chat->id, temp, false, 0, keyboard);

                MyPrefGoods.erase(MyPrefGoods.begin());
            }
            catch (std::exception& e) {
                std::cerr << e.what();
            }
        }
        if (query->data == "liquid_pressed") {
            try {
                goodsdata_toaddd.push_back("2");
                goodsAddMode[user_id] = true;
                TgBot::InlineKeyboardMarkup::Ptr keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();
                AddGoodsFun(keyboard);
                std::string temp;

                SQLite::Statement goods_data(db, "SELECT * FROM goods");
                while (goods_data.executeStep()) {
                    temp = goods_data.getColumnName(MyPrefGoods.at(0));

                }
                goods_data.reset();

                bot.getApi().sendMessage(user_id, temp, false, 0, keyboard);

                MyPrefGoods.erase(MyPrefGoods.begin());
            }
            catch (std::exception& e) {
                std::cerr << e.what();
            }
        }
        if (query->data == "button2_pressed") {

            if (goodsAddMode[user_id]) {

                bot.getApi().sendMessage(query->message->chat->id, " - ");

                goodsdata_toaddd.push_back("0");

                if (!MyPrefGoods.empty())
                {
                    TgBot::InlineKeyboardMarkup::Ptr keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();
                    AddGoodsFun(keyboard);
                    std::string temp;

                    SQLite::Statement goods_data(db, "SELECT * FROM goods");
                    while (goods_data.executeStep()) {
                        temp = goods_data.getColumnName(MyPrefGoods.at(0));

                    }
                    goods_data.reset();

                    bot.getApi().sendMessage(user_id, temp, false, 0, keyboard);

                    MyPrefGoods.erase(MyPrefGoods.begin());
                }
                else if (MyPrefGoods.empty())
                {

                    try {


                        std::string tcolums = "";
                        std::string tvalues = "";

                        for (auto& el : goodsdata_toaddd)
                        {
                            tvalues += '\'' + el + "\', ";
                        }
                        tvalues += "\'0\', \'0\'";
                        //tvalues.pop_back();
                       // tvalues.pop_back();

                        int goodsarr[] = { 0, 1, 2, 3, 4, 5 , 6, 7, 8 };

                        for (auto& el : goodsarr) {

                            std::string ttempt;
                            SQLite::Statement goods_data(db, "SELECT * FROM goods");
                            while (goods_data.executeStep()) {
                                ttempt = goods_data.getColumnName(el);

                            }
                            goods_data.reset();

                            tcolums += '\'' + ttempt + "\', ";
                        }
                        tcolums.pop_back();
                        tcolums.pop_back();




                        SQLite::Statement ADDUSER(db, "INSERT INTO goods (" + tcolums + ") VALUES (" + tvalues + ");");
                        ADDUSER.exec();

                        goodsAddMode[user_id] = false;
                        bot.getApi().sendMessage(user_id, u8"Успешно добавлен.\nназад: /start");
                    }
                    catch (std::exception& e) {
                        bot.getApi().sendMessage(user_id, "Неверный ввод, нужен числовой параметр для цен и количеств.");
                        goodsAddMode[user_id] = false;
                    }


                }
            }

        }

        if (query->data == "vapefin_pressed") {
            try {
                fingoodsdata_toaddd.push_back("1");
                fingoodsAddMode[user_id] = true;
                TgBot::InlineKeyboardMarkup::Ptr keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();
                AddFinGoodsFun(keyboard);
                std::string temp;

                SQLite::Statement fingoods_data(db, "SELECT * FROM finishedgoods");
                while (fingoods_data.executeStep()) {
                    temp = fingoods_data.getColumnName(MyPrefFinGoods.at(0));

                }
                fingoods_data.reset();

                bot.getApi().sendMessage(query->message->chat->id, temp, false, 0, keyboard);

                MyPrefFinGoods.erase(MyPrefFinGoods.begin());
            }
            catch (std::exception& e) {
                std::cerr << e.what();
            }
        }
        if (query->data == "liquidfin_pressed") {
            try {
                fingoodsdata_toaddd.push_back("2");
                fingoodsAddMode[user_id] = true;
                TgBot::InlineKeyboardMarkup::Ptr keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();
                AddFinGoodsFun(keyboard);
                std::string temp;

                SQLite::Statement fingoods_data(db, "SELECT * FROM finishedgoods");
                while (fingoods_data.executeStep()) {
                    temp = fingoods_data.getColumnName(MyPrefFinGoods.at(0));

                }
                fingoods_data.reset();

                bot.getApi().sendMessage(user_id, temp, false, 0, keyboard);

                MyPrefFinGoods.erase(MyPrefFinGoods.begin());
            }
            catch (std::exception& e) {
                std::cerr << e.what();
            }
        }
        if (query->data == "button2fin_pressed") {

            if (fingoodsAddMode[user_id]) {

                bot.getApi().sendMessage(query->message->chat->id, " - ");

                fingoodsdata_toaddd.push_back("0");

                if (!MyPrefFinGoods.empty())
                {
                    TgBot::InlineKeyboardMarkup::Ptr keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();
                    AddFinGoodsFun(keyboard);
                    std::string temp;

                    SQLite::Statement fingoods_data(db, "SELECT * FROM finishedgoods");
                    while (fingoods_data.executeStep()) {
                        temp = fingoods_data.getColumnName(MyPrefFinGoods.at(0));

                    }
                    fingoods_data.reset();

                    bot.getApi().sendMessage(user_id, temp, false, 0, keyboard);

                    MyPrefFinGoods.erase(MyPrefFinGoods.begin());
                }
                else if (MyPrefFinGoods.empty())
                {

                    try {


                        std::string tcolums = "";
                        std::string tvalues = "";

                        for (auto& el : fingoodsdata_toaddd)
                        {
                            tvalues += '\'' + el + "\', ";
                        }
                        tvalues += "\'0\'";
                        //tvalues.pop_back();
                       // tvalues.pop_back();

                        int fingoodsarr[] = { 0, 1, 2, 3, 4, 5 , 6, 7, 8 };

                        for (auto& el : fingoodsarr) {

                            std::string ttempt;
                            SQLite::Statement fingoods_data(db, "SELECT * FROM finishedgoods");
                            while (fingoods_data.executeStep()) {
                                ttempt = fingoods_data.getColumnName(el);

                            }
                            fingoods_data.reset();

                            tcolums += '\'' + ttempt + "\', ";
                        }
                        tcolums.pop_back();
                        tcolums.pop_back();




                        SQLite::Statement ADDUSER(db, "INSERT INTO finishedgoods (" + tcolums + ") VALUES (" + tvalues + ");");
                        ADDUSER.exec();

                        fingoodsAddMode[user_id] = false;
                        bot.getApi().sendMessage(user_id, u8"Успешно добавлен.\nназад: /start");
                    }
                    catch (std::exception& e) {
                        bot.getApi().sendMessage(user_id, "Неверный ввод, нужен числовой параметр для цен и количеств.");
                        fingoodsAddMode[user_id] = false;
                    }


                }
            }

        }
        if (query->data == "cost_pressed") {

            sell_costbool = false;
            std::string ficost;
            SQLite::Statement sell_data(db, u8"SELECT * FROM sorted_table WHERE ID = " + sell_id);
            while (sell_data.executeStep())
            {
                ficost = sell_data.getColumn(6).getString();
                
            }
            sell_data.reset();

            bot.getApi().sendMessage(query->message->chat->id,ficost);

            if (soldonemoretime == 2)
            {
                ++soldonemoretime;

                TgBot::InlineKeyboardMarkup::Ptr keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();


                std::vector<TgBot::InlineKeyboardButton::Ptr> goodsrow;
                TgBot::InlineKeyboardButton::Ptr button11 = std::make_shared<TgBot::InlineKeyboardButton>();
                button11->text = u8" 1 ";
                button11->callbackData = "ammount_pressed";
                goodsrow.push_back(button11);

                keyboard->inlineKeyboard.push_back(goodsrow);

                bot.getApi().sendMessage(query->message->chat->id, u8"Сколько штук продал?", false, 0, keyboard);
                return;
            }

        }
        if (query->data == "ammount_pressed") {

            try {
               
                int sellcount = 1;
                int sellcountfirst = 0;
                std::string sellcolumn;

                SQLite::Statement sell_data(db, u8"SELECT * FROM sorted_table WHERE ID = " + sell_id);
                while (sell_data.executeStep()) {
                    sellcolumn = sell_data.getColumnName(7);
                    sellcountfirst = sell_data.getColumn(7).getInt();
                }
                sell_data.reset();





                std::vector<std::string> ParamsV = {};
                std::vector<std::string> ParamsNames = {};

                for (short int i = 0; i < 10; ++i)
                {
                    while (sell_data.executeStep())
                    {
                        ParamsV.push_back(sell_data.getColumn(i).getString());
                        ParamsNames.push_back(sell_data.getColumnName(i));
                    }
                    sell_data.reset();
                }



                sellcountfirst -= sellcount;
                if (sellcountfirst < 0) {
                    std::exception ex;
                    throw ex;
                }

                SQLite::Statement sell_query(db, "UPDATE goods SET " + sellcolumn + " = " + std::to_string(sellcountfirst) + " WHERE " + ParamsNames.at(1) + " = " + ParamsV.at(1)
                    + " AND " + ParamsNames.at(2) + " = " + "\'" + ParamsV.at(2) + "\'"
                    + " AND " + ParamsNames.at(3) + " = " + "\'" + ParamsV.at(3) + "\'"
                    + " AND " + "\"" + ParamsNames.at(4) + "\"" + " = " + "\'" + ParamsV.at(4) + "\'"
                    + " AND " + "\"" + ParamsNames.at(5) + "\"" + " = " + "\'" + ParamsV.at(5) + "\'"
                    + " AND " + "\"" + ParamsNames.at(6) + "\"" + " = " + "\'" + ParamsV.at(6) + "\'"
                    + " AND " + "\"" + ParamsNames.at(7) + "\"" + " = " + "\'" + ParamsV.at(7) + "\'"
                    + " AND " + "\"" + ParamsNames.at(8) + "\"" + " = " + "\'" + ParamsV.at(8) + "\'");
                sell_query.exec();


                auto now = std::chrono::system_clock::now();
                std::time_t now_c = std::chrono::system_clock::to_time_t(now);
                std::tm local_tm;
                localtime_s(&local_tm, &now_c);

                int year = local_tm.tm_year + 1900;
                int month = local_tm.tm_mon + 1;
                int day = local_tm.tm_mday;
                std::string oformonth = "";
                std::string oforday = "";
                if (month < 10)
                {
                    oformonth = "0";
                }
                if (day < 10)
                {
                    oforday = "0";
                }
                std::string currentDate = std::to_string(year) + oformonth + std::to_string(month) + oforday + std::to_string(day);

                if (!sell_costbool)
                {
                    sell_cost = ParamsV.at(6);
                }

                std::cerr << ParamsNames.at(8) << ParamsNames.at(9) << std::endl;
                SQLite::Statement finished_query(db, "INSERT INTO finishedgoods (\'" + ParamsNames.at(1) + "\', \'"
                    + ParamsNames.at(2) + "\', \'"
                    + ParamsNames.at(3) + "\', \'"
                    + ParamsNames.at(4) + "\', \'"
                    + ParamsNames.at(5) + "\', \'"
                    + ParamsNames.at(6) + "\', \'"
                    + ParamsNames.at(8) + "\', \'"
                    + ParamsNames.at(9) + "\', \'empty\'"
                    + ") VALUES (\'" + ParamsV.at(1) + "\', \'"
                    + ParamsV.at(2) + "\', \'"
                    + ParamsV.at(3) + "\', \'"
                    + ParamsV.at(4) + "\', \'"
                    + ParamsV.at(5) + "\', \'"
                    + sell_cost + "\', \'"
                    + std::to_string(sellcount) + "\', \'" + currentDate + "\', \'0\')"
                );
                finished_query.exec();

                bot.getApi().sendMessage(query->message->chat->id, u8"Успешно изменено");
                sold[query->message->chat->id] = false;

                ParamsV.clear();
                ParamsNames.clear();

                soldonemoretime = 0;
            }
            catch (std::exception& ex) {
                bot.getApi().sendMessage(query->message->chat->id, u8"Неверный ввод, требуются только числа");
                soldonemoretime = 0;
                sold[query->message->chat->id] = false;
            }

        }
        });
    
    //extend buttons

    bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
        TgBot::ReplyKeyboardMarkup::Ptr MainKeyboard(new TgBot::ReplyKeyboardMarkup());

        
        if (StringTools::startsWith(message->text, "/start")) {
            deleteMode[message->chat->id] = false;
            CustomClientColumnsMode[message->chat->id] = false;
            AddMode[message->chat->id] = false;
            ChangeMode[message->chat->id] = false;

            goodsdeleteMode[message->chat->id] = false;
            goodsAddMode[message->chat->id] = false;
            sold[message->chat->id] = false;
            goodsChangeMode[message->chat->id] = false;
            goodsdata_toaddd = {};
            user_id = message->chat->id;
            std::cout << message->chat->id << std::endl;

            finishedgoodsdeleteMode[message->chat->id] = false;
            profitperiod[message->chat->id] = false;
            fingoodsAddMode[user_id] = false;


            return;  // Skip the /start command handler
        }

        //deleting clients
        
         
    

        //clients
        if (StringTools::startsWith(message->text, u8"Клиенты"))
        {
            TgBot::ReplyKeyboardMarkup::Ptr keyboard(new TgBot::ReplyKeyboardMarkup());
            keyboard->resizeKeyboard = true;

            TgBot::KeyboardButton::Ptr btn1(new TgBot::KeyboardButton());
            btn1->text = u8"Добавить клиента";

            TgBot::KeyboardButton::Ptr btn2(new TgBot::KeyboardButton());
            btn2->text = u8"Изменить клиента";

            TgBot::KeyboardButton::Ptr btn3(new TgBot::KeyboardButton());
            btn3->text = u8"Удалить клиента";
            

            keyboard->keyboard.push_back({ btn1, btn2 });  // Row 1
            keyboard->keyboard.push_back({ btn3 });       // Row 2

            MainKeyboard = keyboard;

            
            MyPref.clear();
            std::string Pref = "";
            SQLite::Statement PrefQuery(db, "SELECT * FROM owners WHERE TG_ID = ?;");
            PrefQuery.bind(1, message->chat->id);
            while (PrefQuery.executeStep()) {
                std::string tPref = PrefQuery.getColumn(2);
                Pref = tPref;
            }
            PrefQuery.reset();

            std::stringstream ss(Pref);
            int number;
            MyPref.push_back(15);

            while (ss >> number) {
                
                MyPref.push_back(--number);
            }

            bot.getApi().sendMessage(message->chat->id, ShouldBeIncluded(MyPref), false, 0, keyboard);

            
        }
        //prefferences
        else if (StringTools::startsWith(message->text, u8"Настройки"))
        {
            TgBot::KeyboardButton::Ptr btn1(new TgBot::KeyboardButton());
            btn1->text = u8"Расширить клиентов";

            TgBot::KeyboardButton::Ptr btn2(new TgBot::KeyboardButton());
            btn2->text = u8"Параметры проданного товара";

            TgBot::KeyboardButton::Ptr btn3(new TgBot::KeyboardButton());
            btn3->text = u8"Доход";
         
            
            TgBot::ReplyKeyboardMarkup::Ptr keyboard(new TgBot::ReplyKeyboardMarkup());
            keyboard->resizeKeyboard = true;  // Makes buttons fit screen width
            keyboard->keyboard.push_back({ btn1, btn2});  // Row 1
            keyboard->keyboard.push_back({ btn3 });

            bot.getApi().sendMessage(
                message->chat->id,
                u8"Выбери настройку: \n назад: /start",
                false, 0, keyboard);
        }
        //profit
        else if (message->text == u8"Доход") {

           
            int date_period = SWprofitdaysFun(profitdays, 0) - 1;

            std::string column_date = "";
            SQLite::Statement costil_query(db, u8"SELECT * FROM finishedgoods_sorted_table");
            while (costil_query.executeStep()) {
                column_date = costil_query.getColumnName(8);
            }
            costil_query.reset();

            SQLite::Statement vape_data(db, u8"SELECT * FROM finishedgoods_sorted_table WHERE \"" + column_date + "\" > " + std::to_string(date_period));
            int fingoodcount = 0;
            int fingoodfirstcost = 0;
            int fingoodcost = 0;
            int spendings = 0;
            int profit = 0;
            int clearprofit = 0;
            while (vape_data.executeStep()){
                fingoodcount = vape_data.getColumn(7).getInt();
                fingoodfirstcost = vape_data.getColumn(5).getInt();
                spendings += fingoodcount * fingoodfirstcost;

                fingoodcost = vape_data.getColumn(6).getInt();
                profit += fingoodcost * fingoodcount;
            }
            vape_data.reset();
            clearprofit = profit - spendings;

            bot.getApi().sendMessage(message->chat->id, u8"Доход за последние " + std::to_string(profitdays) + u8" дней\n" u8"Затраты: " + std::to_string(spendings) + u8"тл\nПродано на : " + std::to_string(profit) + u8"тл\nПрофит : " + std::to_string(clearprofit) + u8"тл");
            return;
        }
        //finished goods preferences
        else if (message->text == u8"Параметры проданного товара") {
            TgBot::KeyboardButton::Ptr btn1(new TgBot::KeyboardButton());
            btn1->text = u8"Удалить товар.";

            //TgBot::KeyboardButton::Ptr btn2(new TgBot::KeyboardButton());
            //btn2->text = u8"Изменить товар.";

            TgBot::KeyboardButton::Ptr btn3(new TgBot::KeyboardButton());
            btn3->text = u8"Добавить товар.";

            TgBot::KeyboardButton::Ptr btn4(new TgBot::KeyboardButton());
            btn4->text = u8"Изменить период дохода";


            TgBot::ReplyKeyboardMarkup::Ptr keyboard(new TgBot::ReplyKeyboardMarkup());
            keyboard->resizeKeyboard = true;  // Makes buttons fit screen width
            keyboard->keyboard.push_back({ btn1, btn3 });  // Row 1
            keyboard->keyboard.push_back({ btn4 });

            bot.getApi().sendMessage(
                message->chat->id,
                u8"Выбери параметр для проданного товара: \n назад: /start",
                false, 0, keyboard);
        }
        //deleting clients
        else if (message->text == u8"Удалить клиента") {
            deleteMode[message->chat->id] = true;
            bot.getApi().sendMessage(message->chat->id, u8"Выбери ID клиента:");
            return;
        }
        //change client
        else if (message->text == u8"Изменить клиента") {
            ChangeMode[message->chat->id] = true;
            changecounter = 0;
            bot.getApi().sendMessage(message->chat->id, u8"Выбери ID клиента:");
            return;
        }
        //adding client
        else if (message->text == u8"Добавить клиента") {
            AddMode[message->chat->id] = true;
            user_id = message->chat->id;
            //bot.getApi().sendMessage(message->chat->id, u8"Выбери поля и напиши туда информацию о клиентах\nназад: /start");
            AddClientsMyPref = MyPref;
            addClients_Data.clear();


            TgBot::InlineKeyboardMarkup::Ptr keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();
            AddClientFun(keyboard);
            std::string temp;
            AddClientsMyPref.erase(AddClientsMyPref.begin());

            SQLite::Statement clients_data(db, "SELECT * FROM clients");
            while (clients_data.executeStep()) {
                temp = clients_data.getColumnName(AddClientsMyPref.at(0));

            }
            clients_data.reset();

            bot.getApi().sendMessage(message->chat->id, temp, false, 0, keyboard);


            AddClientsMyPref.erase(AddClientsMyPref.begin());
           // AddClientsMyPref.push_back(808);

            return;
        }

        //Changing client data showcases
        else if (StringTools::startsWith(message->text, u8"Расширить клиентов")) {
            CustomClientColumnsMode[message->chat->id] = true;
            bot.getApi().sendMessage(message->chat->id, u8"Напечатай через пробел ID столобцов, которые ты хочешь увидеть в таблице:\n1. Имя\n2. Возраст\n3. Номер\n4. Почта\n5. Предпочитаемые вкусы\n6. Предпочитаемые бренды\n7. Предпочитаемый % никотина\n8. Средние затраты\n9.частота покупок в месяц\n10. Способ свзязи\n11. Адресс доставки\n12. Район \n13. От кого узнал\n14. Покупал до этого\n15. Покупал у нас\n16. Включить все\n\n назад: /start"
            );
            return;
        }
        //goods
        else if (StringTools::startsWith(message->text, u8"Товар"))
        {
            user_id = message->chat->id;

            TgBot::ReplyKeyboardMarkup::Ptr keyboard(new TgBot::ReplyKeyboardMarkup());
            keyboard->resizeKeyboard = true;

            TgBot::KeyboardButton::Ptr btn1(new TgBot::KeyboardButton());
            btn1->text = u8"Добавить товар";

            TgBot::KeyboardButton::Ptr btn2(new TgBot::KeyboardButton());
            btn2->text = u8"Изменить товар";

            TgBot::KeyboardButton::Ptr btn3(new TgBot::KeyboardButton());
            btn3->text = u8"Удалить товар";

            TgBot::KeyboardButton::Ptr btn4(new TgBot::KeyboardButton());
            btn4->text = u8"Закончившийся товар";

            TgBot::KeyboardButton::Ptr btn5(new TgBot::KeyboardButton());
            btn5->text = u8"Продал";

            try {
                SQLite::Statement temp_data(db, "SELECT * FROM goods");
                std::string costilcounter;
                while (temp_data.executeStep()) {
                    costilcounter = temp_data.getColumnName(6);

                }
                temp_data.reset();

                SQLite::Statement delquery(db, u8"DELETE FROM goods WHERE " + costilcounter +  + " = \'0\'");
                delquery.exec();
            }
            catch (std::exception& e) {
                std::cerr<<"no 0 params"<<std::endl;
            }
               
            

            MyPrefGoods = { 0, 2, 3, 4, 5, 6, 7 };

            keyboard->keyboard.push_back({ btn1, btn2 });  // Row 1
            keyboard->keyboard.push_back({ btn3, btn4 });       // Row 2
            keyboard->keyboard.push_back({ btn5 });

            MainKeyboard = keyboard;


            bot.getApi().sendMessage(message->chat->id, GoodsShouldBeIncluded(MyPrefGoods), false, 0, keyboard);


            }
        //finished goods
        else if (message->text == u8"Закончившийся товар") {
            std::vector<int> fincolumns = { 0, 2, 3, 4, 5, 6, 7, 8 };
            bot.getApi().sendMessage(message->chat->id, FinishedGoodsShouldBeIncluded(fincolumns, SWprofitdaysFun(profitdays, 0)));
        }
        //delete goods
        else if (message->text == u8"Удалить товар") {
                goodsdeleteMode[message->chat->id] = true;
                bot.getApi().sendMessage(message->chat->id, u8"Выбери ID товара:");
                return;
                }
        //change goods
        else if (message->text == u8"Изменить товар") {
            goodsChangeMode[message->chat->id] = true;
            goodschangecounter = 0;
            bot.getApi().sendMessage(message->chat->id, u8"Выбери ID товара:");
            return;
                }
        //add goods
        else if (message->text == u8"Добавить товар") {
           // goodsAddMode[message->chat->id] = true;
           // user_id = message->chat->id;
            addgoodscounter = 0;
            //bot.getApi().sendMessage(message->chat->id, u8"Выбери поля и напиши туда информацию о клиентах\nназад: /start");
            goodsdata_toaddd.clear();
            MyPrefGoods = { 1, 2, 3, 4, 5, 6 };

            TgBot::InlineKeyboardMarkup::Ptr keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();
            
            
            std::vector<TgBot::InlineKeyboardButton::Ptr> goodsrow;
            TgBot::InlineKeyboardButton::Ptr vapebt = std::make_shared<TgBot::InlineKeyboardButton>();
            vapebt->text = u8"Вейп";
            vapebt->callbackData = "vape_pressed";
            goodsrow.push_back(vapebt);

            TgBot::InlineKeyboardButton::Ptr liquidbt = std::make_shared<TgBot::InlineKeyboardButton>();
            liquidbt->text = u8"Жижка";
            liquidbt->callbackData = "liquid_pressed";
            goodsrow.push_back(liquidbt);
            

            keyboard->inlineKeyboard.push_back(goodsrow);// <-----
            
            bot.getApi().sendMessage(message->chat->id, u8"<--Тип-->", false, 0, keyboard);

            // AddClientsMyPref.push_back(808);

            return;
            }
        //sold out
        else if(message->text == u8"Продал"){
            sold[message->chat->id] = true;
            ++soldonemoretime;
            sell_id = "";
            bot.getApi().sendMessage(message->chat->id, u8"Выбери ID товара:");
            return;
        }


        //delete finishedgoods
        else if (message->text == u8"Удалить товар.") {
            finishedgoodsdeleteMode[message->chat->id] = true;
            bot.getApi().sendMessage(message->chat->id, u8"Выбери ID товара:");
            return;
                }
        //add finished goods
        else if (message->text == u8"Добавить товар.") {
            addfingoodscounter = 0;
            fingoodsdata_toaddd.clear();
            MyPrefFinGoods = { 1, 2, 3, 4, 5, 6, 7 };

            TgBot::InlineKeyboardMarkup::Ptr keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();


            std::vector<TgBot::InlineKeyboardButton::Ptr> fingoodsrow;
            TgBot::InlineKeyboardButton::Ptr vapebt = std::make_shared<TgBot::InlineKeyboardButton>();
            vapebt->text = u8"Вейп";
            vapebt->callbackData = "vapefin_pressed";
            fingoodsrow.push_back(vapebt);

            TgBot::InlineKeyboardButton::Ptr liquidbt = std::make_shared<TgBot::InlineKeyboardButton>();
            liquidbt->text = u8"Жижка";
            liquidbt->callbackData = "liquidfin_pressed";
            fingoodsrow.push_back(liquidbt);


            keyboard->inlineKeyboard.push_back(fingoodsrow);// <-----

            bot.getApi().sendMessage(message->chat->id, u8"<--Тип-->", false, 0, keyboard);

            // AddClientsMyPref.push_back(808);

            return;
            }


        //add goods
        else if (message->text == u8"Изменить период дохода") {
            profitperiod[message->chat->id] = true;
            bot.getApi().sendMessage(message->chat->id, u8"Напиши число за последние сколько дней будет отображаться доход:");
            return;
        }

        //any other messages
        else
        {
            if (!fingoodsAddMode[message->chat->id] && !profitperiod[message->chat->id] && !finishedgoodsdeleteMode[message->chat->id] && !goodsChangeMode[message->chat->id] && !sold[message->chat->id] && !goodsAddMode[message->chat->id] && !goodsdeleteMode[message->chat->id] && !ChangeMode[message->chat->id] && !deleteMode[message->chat->id] && !CustomClientColumnsMode[message->chat->id] && !AddMode[message->chat->id])
            {
                bot.getApi().sendMessage(message->chat->id, "You selected: " + message->text);
            }
        }

        // Handle ID input in delete mode
        if (deleteMode[message->chat->id]) {
            try {
                std::string cid = message->text;
                int clientId = std::stoi(cid); // Convert input to integer
                SQLite::Database db("test.db", SQLite::OPEN_READWRITE);
                SQLite::Statement query(db, "DELETE FROM clients WHERE id = ?");
                query.bind(1, clientId);
                int rowsAffected = query.exec();

                if (rowsAffected > 0) {
                    bot.getApi().sendMessage(message->chat->id, u8"Ликвидирован.");
                }
                else {
                    bot.getApi().sendMessage(message->chat->id, u8"Клиент с таким ID не найден.");
                }
            }
            catch (std::exception& e) {
                bot.getApi().sendMessage(message->chat->id, "Неверный ввод, нужен числовой ID.");
            }
            deleteMode[message->chat->id] = false;
            // bot.getApi().sendMessage(message->chat->id, "Deletion mode exited.", false, 0, MainKeyboard);
        }
        //customizing clients
        if (CustomClientColumnsMode[message->chat->id]) {
            try
            {
                std::string options = message->text;
                std::stringstream ss(options);
                int number;
                bool success = true;
                bool its16 = false;
               
                for (char c : options)
                {
                    if (!std::isdigit(c) && !std::isspace(c))
                    {
                        success = false;
                        break;
                    }
                }

                

                // Extract integers from the string
                while (ss >> number) {
                    if (number == 16) {
                        options = "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15";
                        ClientPref.clear();
                        ClientPref = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
                    }
                    else if (number >16 || number <0 || ss.fail())
                    {
                        success = false;
                        bot.getApi().sendMessage(message->chat->id, "Неверный ввод"); break;
                    }
                    ClientPref.push_back(number);
                }
                
                        if (success)
                        {
                            SQLite::Statement query5(db, "UPDATE owners SET OPTIONS = ? WHERE TG_ID = ?");

                            query5.bind(1, options);
                            query5.bind(2, message->chat->id);
                            query5.exec();
                            bot.getApi().sendMessage(message->chat->id, u8"Успешно изменено");
                        }
                        else
                        {
                            bot.getApi().sendMessage(message->chat->id, u8"Неверный ввод");
                        }
                        
            }
            catch (const std::exception& ex)
            {
                bot.getApi().sendMessage(message->chat->id, u8"Неверный ввод");
            }



            CustomClientColumnsMode[message->chat->id] = false;
        }
        //adding client
        if (AddMode[message->chat->id])
        {
            addClients_Data.push_back(message->text);

                 if (AddClientsMyPref.empty())
                 {
                

                std::string tcolums="";
                std::string tvalues = "";

                for (auto &el : addClients_Data)
                {
                    tvalues += '\'' + el + "\', ";
                }
                tvalues.pop_back();
                tvalues.pop_back();

                for (auto& el : MyPref) {
                    
                    std::string ttempt;
                    SQLite::Statement clients_data(db, "SELECT * FROM clients");
                    while (clients_data.executeStep()) {
                        ttempt = clients_data.getColumnName(el);

                    }
                    clients_data.reset();

                    tcolums += '\'' + ttempt + "\', ";
                }
                tcolums.pop_back();
                tcolums.pop_back();

                SQLite::Statement ADDUSER(db, "INSERT INTO clients ("+tcolums+") VALUES ("+tvalues+");");
                ADDUSER.exec();

                AddMode[message->chat->id] = false;
                bot.getApi().sendMessage(message->chat->id, u8"Успешно добавлен.\nназад: /start");


            }
            else if (!AddClientsMyPref.empty())
            {
                TgBot::InlineKeyboardMarkup::Ptr keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();
                AddClientFun(keyboard);
                std::string temp;

                SQLite::Statement clients_data(db, "SELECT * FROM clients");
                while (clients_data.executeStep()) {
                    temp = clients_data.getColumnName(AddClientsMyPref.at(0));

                }
                clients_data.reset();

                bot.getApi().sendMessage(message->chat->id, temp, false, 0, keyboard);

                AddClientsMyPref.erase(AddClientsMyPref.begin());
            }
            
            
                

            
            
            
            

             
              
           
        }
        //change client
        if (ChangeMode[message->chat->id]) {
            try {
                if (changecounter==0)
                {
                    std::string cid = message->text;
                    clientIdtochange = std::stoi(cid); // Convert input to integer
                    ++changecounter;
                    bot.getApi().sendMessage(message->chat->id, u8"Выбери поле которое ты хочешь изменить");
                }
                else if(changecounter == 1)
                {
                    changecolumn = message->text;
                    bot.getApi().sendMessage(message->chat->id, u8"Напиши новые данные");
                    ++changecounter;
                }
                else if (changecounter == 2) {
                    std::string changedata = message->text;
                    SQLite::Statement querychange(db, "UPDATE clients SET "+ changecolumn +" = ? WHERE ID = ?");

                    querychange.bind(1, changedata);
                    querychange.bind(2, clientIdtochange);

                    querychange.exec();

                    changecounter = 0;
                    ChangeMode[message->chat->id] = false;
                    bot.getApi().sendMessage(message->chat->id, u8"Добавлен.\nназад: /start");
                }  
                
            }
            catch (std::exception& e) {
                bot.getApi().sendMessage(message->chat->id, u8"Неверный ввод ID либо поля.");
            }
            
            // bot.getApi().sendMessage(message->chat->id, "Deletion mode exited.", false, 0, MainKeyboard);
        }
        
        //sell smth
        if (sold[message->chat->id])
        {
            if (soldonemoretime == 1)
            {
                try {
                    for (auto& el : message->text) {
                        if (!std::isdigit(el))
                        {
                            std::exception ex;
                            throw ex;
                        }
                    }
                    sell_id = message->text;
                    ++soldonemoretime;

                    TgBot::InlineKeyboardMarkup::Ptr keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();



                    std::vector<TgBot::InlineKeyboardButton::Ptr> goodsrow;
                    TgBot::InlineKeyboardButton::Ptr button1 = std::make_shared<TgBot::InlineKeyboardButton>();
                    button1->text = u8"По заданной цене";
                    button1->callbackData = "cost_pressed";
                    goodsrow.push_back(button1);

                    keyboard->inlineKeyboard.push_back(goodsrow);

                    sell_costbool = true;

                    bot.getApi().sendMessage(message->chat->id, u8"По какой цене? (за шт)", false, 0, keyboard);

                    return;
                }
                catch (std::exception& ex) {
                    bot.getApi().sendMessage(message->chat->id, u8"Неверный ввод, требуются только числа");
                    soldonemoretime = 0;
                    sold[message->chat->id] = false;
                }
            }
            else if (soldonemoretime == 2)
            {
                if (sell_costbool)
                {
                    sell_cost = message->text;
                }
                ++soldonemoretime;

                TgBot::InlineKeyboardMarkup::Ptr keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();


                std::vector<TgBot::InlineKeyboardButton::Ptr> goodsrow;
                TgBot::InlineKeyboardButton::Ptr button11 = std::make_shared<TgBot::InlineKeyboardButton>();
                button11->text = u8" 1 ";
                button11->callbackData = "ammount_pressed";
                goodsrow.push_back(button11);

                keyboard->inlineKeyboard.push_back(goodsrow);


                bot.getApi().sendMessage(message->chat->id, u8"Сколько штук продал?", false, 0, keyboard);
                return;
            }
            else if(soldonemoretime == 3)
            {
                try {
                    for (auto& el : message->text) {
                        if (!std::isdigit(el))
                        {
                            std::exception ex;
                            throw ex;
                        }
                    }
                    int sellcount = std::stoi(message->text);
                    int sellcountfirst = 0;
                    std::string sellcolumn;

                    SQLite::Statement sell_data(db, u8"SELECT * FROM sorted_table WHERE ID = " + sell_id);
                    while (sell_data.executeStep()) {
                        sellcolumn = sell_data.getColumnName(7);
                        sellcountfirst = sell_data.getColumn(7).getInt();
                    }
                    sell_data.reset();



                   

                    std::vector<std::string> ParamsV = {};
                    std::vector<std::string> ParamsNames = {};

                    for (short int i = 0; i < 10; ++i)
                    {
                        while (sell_data.executeStep())
                        {
                            ParamsV.push_back(sell_data.getColumn(i).getString());
                            ParamsNames.push_back(sell_data.getColumnName(i));
                        }
                        sell_data.reset();
                    }



                    sellcountfirst -= sellcount;
                    if (sellcountfirst<0) {
                        std::exception ex;
                        throw ex;
                    }

                    SQLite::Statement sell_query(db, "UPDATE goods SET "+ sellcolumn +" = "+ std::to_string(sellcountfirst) + " WHERE " + ParamsNames.at(1) + " = " + ParamsV.at(1)
                        + " AND " + ParamsNames.at(2) + " = " + "\'" + ParamsV.at(2) + "\'"
                        + " AND " + ParamsNames.at(3) + " = " + "\'" + ParamsV.at(3) + "\'"
                        + " AND " + "\"" + ParamsNames.at(4) + "\"" + " = " + "\'" + ParamsV.at(4) + "\'"
                        + " AND " + "\"" + ParamsNames.at(5) + "\"" + " = " + "\'" + ParamsV.at(5) + "\'"
                        + " AND " + "\"" + ParamsNames.at(6) + "\"" + " = " + "\'" + ParamsV.at(6) + "\'"
                        + " AND " + "\"" + ParamsNames.at(7) + "\"" + " = " + "\'" + ParamsV.at(7) + "\'"
                        + " AND " + "\"" + ParamsNames.at(8) + "\"" + " = " + "\'" + ParamsV.at(8) + "\'");
                    sell_query.exec();


                    auto now = std::chrono::system_clock::now();
                    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
                    std::tm local_tm;
                    localtime_s(&local_tm, &now_c); 

                    int year = local_tm.tm_year + 1900; 
                    int month = local_tm.tm_mon + 1;   
                    int day = local_tm.tm_mday;
                    std::string oformonth = "";
                    std::string oforday = "";
                    if (month<10)
                    {
                        oformonth = "0";
                    }
                    if (day<10)
                    {
                        oforday = "0";
                    }
                    std::string currentDate = std::to_string(year) + oformonth + std::to_string(month) + oforday + std::to_string(day);

                    if (!sell_costbool)
                    {
                        sell_cost = ParamsV.at(6);
                    }

                    std::cerr << ParamsNames.at(8) << ParamsNames.at(9) <<std::endl;
                    SQLite::Statement finished_query(db, "INSERT INTO finishedgoods (\'"  + ParamsNames.at(1) +"\', \'"  
                        + ParamsNames.at(2) + "\', \'"
                        + ParamsNames.at(3) + "\', \'"
                        + ParamsNames.at(4) + "\', \'"
                        + ParamsNames.at(5) + "\', \'"
                        + ParamsNames.at(6) + "\', \'"
                        + ParamsNames.at(8) + "\', \'"
                        + ParamsNames.at(9) + "\', \'empty\'"
                        + ") VALUES (\'" + ParamsV.at(1) + "\', \'"
                        + ParamsV.at(2) + "\', \'"
                        + ParamsV.at(3) + "\', \'"
                        + ParamsV.at(4) + "\', \'"
                        + ParamsV.at(5) + "\', \'"
                        + sell_cost + "\', \'"
                        + std::to_string(sellcount) + "\', \'"+ currentDate +"\', \'0\')"
                    );
                    finished_query.exec();

                    bot.getApi().sendMessage(message->chat->id, u8"Успешно изменено");
                    sold[message->chat->id] = false;

                    ParamsV.clear();
                    ParamsNames.clear();

                    soldonemoretime = 0;
                }
                catch (std::exception &ex) {
                    bot.getApi().sendMessage(message->chat->id, u8"Неверный ввод, требуются только числа");
                    soldonemoretime = 0;
                    sold[message->chat->id] = false;
                }
            }

           
        }
        //delete goods
        if (goodsdeleteMode[message->chat->id]) {
            try {
                std::string cid = message->text;
                int goodId = std::stoi(cid); // Convert input to integer
                SQLite::Database db("test.db", SQLite::OPEN_READWRITE);

                SQLite::Statement compare_data(db, u8"SELECT * FROM sorted_table WHERE ID = "+cid);

                std::vector<std::string> ParamsV = {};
                std::vector<std::string> ParamsNames = {};

                    for (short int i = 0; i < 9; ++i)
                    {
                        while (compare_data.executeStep())
                        {
                            ParamsV.push_back(compare_data.getColumn(i).getString());
                            ParamsNames.push_back(compare_data.getColumnName(i));
                        }
                        compare_data.reset();
                    }
                    

                /*std::string Params = "WHERE ";
                while (compare_data.executeStep())
                {
                    Params += compare_data.getColumnName(1);
                    Params += " = ";
                    Params += compare_data.getColumn(1).getString();
                }
                compare_data.reset();
                
                for (short int i = 2; i < 9; ++i)
                {
                    while (compare_data.executeStep())
                    {
                        Params += " AND ";
                        Params += compare_data.getColumnName(i);
                        Params += " = ";
                        Params += compare_data.getColumn(i).getString();
                    }
                    compare_data.reset();
                }*/
                

                SQLite::Statement delquery(db, u8"DELETE FROM goods WHERE " /*+ Params*/ 
                    + ParamsNames.at(1) + " = " + ParamsV.at(1)
                + " AND " + ParamsNames.at(2) + " = " + "\'" + ParamsV.at(2) + "\'" 
                + " AND " + ParamsNames.at(3) + " = " + "\'" + ParamsV.at(3) + "\'" 
                + " AND " + "\"" + ParamsNames.at(4) + "\"" + " = " + "\'" + ParamsV.at(4) + "\'"
                + " AND " + "\"" + ParamsNames.at(5) + "\"" + " = " + "\'" + ParamsV.at(5) + "\'"
                + " AND " + "\"" + ParamsNames.at(6) + "\"" + " = " + "\'" + ParamsV.at(6) + "\'"
                + " AND " + "\"" + ParamsNames.at(7) + "\"" + " = " + "\'" + ParamsV.at(7) + "\'"
                + " AND " + "\"" + ParamsNames.at(8) + "\"" + " = " + "\'" + ParamsV.at(8) + "\'"
                
                
                

                
                
                
                );
                int rowsAffected = delquery.exec();
                //delquery.reset();

                if (rowsAffected > 0) {
                    bot.getApi().sendMessage(message->chat->id, u8"Товар ликвидирован.");
                }
                else {
                    bot.getApi().sendMessage(message->chat->id, u8"Товар с таким ID не найден.");
                }
            }
            catch (std::exception& e) {
                std::cerr << e.what();
                bot.getApi().sendMessage(message->chat->id, u8"Неверный ввод, нужен числовой ID.");
            }
            goodsdeleteMode[message->chat->id] = false;
            // bot.getApi().sendMessage(message->chat->id, "Deletion mode exited.", false, 0, MainKeyboard);
        }
        //add goods
        if (goodsAddMode[message->chat->id])
        {
            goodsdata_toaddd.push_back(message->text);

         if (!MyPrefGoods.empty())
        {
             try {
                 ++addgoodscounter;
                 if (addgoodscounter > 3)
                 {
                     
                     for (char& el : message->text) {
                         if (!std::isdigit(el))
                         {
                             std::exception o;
                             
                             throw o;
                         }
                     }

                 }

                 TgBot::InlineKeyboardMarkup::Ptr keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();
                 AddGoodsFun(keyboard);
                 std::string temp;


                 SQLite::Statement goods_data(db, "SELECT * FROM goods");
                 while (goods_data.executeStep()) {
                     temp = goods_data.getColumnName(MyPrefGoods.at(0));

                 }
                 goods_data.reset();

                 bot.getApi().sendMessage(message->chat->id, temp, false, 0, keyboard);

                 MyPrefGoods.erase(MyPrefGoods.begin());
             }
             catch (std::exception& e) {
                 bot.getApi().sendMessage(user_id, u8"Неверный ввод, нужен числовой параметр для цен и количеств.");
                 goodsAddMode[user_id] = false;
             }
        }
            else if (MyPrefGoods.empty())
            {

             try {
                 std::string tcolums = "";
                 std::string tvalues = "";

                 if (addgoodscounter > 3)
                 {

                     for (char& el : message->text) {
                         if (!std::isdigit(el))
                         {
                             std::exception o;

                             throw o;
                         }
                     }

                 }

                 for (auto& el : goodsdata_toaddd)
                 {
                     tvalues += '\'' + el + "\', ";
                 }
                 tvalues += "\'0\', \'0\'";
                 //tvalues.pop_back();
                 //tvalues.pop_back();

                 int goodsarr[] = { 0, 1, 2, 3, 4, 5 , 6, 7, 8 };

                 for (auto& el : goodsarr) {

                     std::string ttempt;
                     SQLite::Statement goods_data(db, "SELECT * FROM goods");
                     while (goods_data.executeStep()) {
                         ttempt = goods_data.getColumnName(el);

                     }
                     goods_data.reset();

                     tcolums += '\'' + ttempt + "\', ";
                 }
                 tcolums.pop_back();
                 tcolums.pop_back();




                 SQLite::Statement ADDUSER(db, "INSERT INTO goods (" + tcolums + ") VALUES (" + tvalues + ");");
                 ADDUSER.exec();

                 goodsAddMode[message->chat->id] = false;
                 bot.getApi().sendMessage(message->chat->id, u8"Успешно добавлен.\nназад: /start");

             }
             catch (std::exception& e) {
                 bot.getApi().sendMessage(user_id, u8"Неверный ввод, нужен числовой параметр для цен и количеств.");
                 goodsAddMode[user_id] = false;
             }
            }
            












        }
        //change goods
        if (goodsChangeMode[message->chat->id]) {
            try {
                if (goodschangecounter == 0)
                {
                    std::string cid = message->text;
                    goodIdtochange = std::stoi(cid); // Convert input to integer
                    ++goodschangecounter;
                    bot.getApi().sendMessage(message->chat->id, u8"Выбери поле которое ты хочешь изменить");
                }
                else if (goodschangecounter == 1)
                {
                    goodchangecolumn = message->text;
                    bot.getApi().sendMessage(message->chat->id, u8"Напиши новые данные");
                    ++goodschangecounter;
                }
                else if (goodschangecounter == 2) {
                    std::string goodschangedata = message->text;


                    std::vector<std::string> ParamsV = {};
                    std::vector<std::string> ParamsNames = {};

                    SQLite::Database db("test.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

                    SQLite::Statement gchange_data(db, "SELECT * FROM sorted_table WHERE ID = " + std::to_string(goodIdtochange));

                    for (short int i = 0; i < 10; ++i)
                    {
                        while (gchange_data.executeStep())
                        {
                            ParamsV.push_back(gchange_data.getColumn(i).getString());
                            ParamsNames.push_back(gchange_data.getColumnName(i));
                        }
                        gchange_data.reset();
                    }

                    SQLite::Statement change_query(db, "UPDATE goods SET \"" + goodchangecolumn + "\" = \"" + goodschangedata + "\" WHERE " + ParamsNames.at(1) + " = " + ParamsV.at(1)
                        + " AND " + ParamsNames.at(2) + " = " + "\'" + ParamsV.at(2) + "\'"
                        + " AND " + ParamsNames.at(3) + " = " + "\'" + ParamsV.at(3) + "\'"
                        + " AND " + "\"" + ParamsNames.at(4) + "\"" + " = " + "\'" + ParamsV.at(4) + "\'"
                        + " AND " + "\"" + ParamsNames.at(5) + "\"" + " = " + "\'" + ParamsV.at(5) + "\'"
                        + " AND " + "\"" + ParamsNames.at(6) + "\"" + " = " + "\'" + ParamsV.at(6) + "\'"
                        + " AND " + "\"" + ParamsNames.at(7) + "\"" + " = " + "\'" + ParamsV.at(7) + "\'"
                        + " AND " + "\"" + ParamsNames.at(8) + "\"" + " = " + "\'" + ParamsV.at(8) + "\'"
                    );
                    change_query.exec();

                    goodschangecounter = 0;
                    ChangeMode[message->chat->id] = false;
                    bot.getApi().sendMessage(message->chat->id, u8"Изменен.\nназад: /start");
                }

            }
            catch (std::exception& e) {
                bot.getApi().sendMessage(message->chat->id, u8"Неверный ввод ID либо поля.");
            }

            // bot.getApi().sendMessage(message->chat->id, "Deletion mode exited.", false, 0, MainKeyboard);
        }


        //delete finished goods
        if (finishedgoodsdeleteMode[message->chat->id]) {
            try {
                std::string cid = message->text;
                int goodId = std::stoi(cid); // Convert input to integer
                SQLite::Database db("test.db", SQLite::OPEN_READWRITE);

                SQLite::Statement compare_data(db, u8"SELECT * FROM finishedgoods_sorted_table WHERE ID = " + cid);

                std::vector<std::string> ParamsV = {};
                std::vector<std::string> ParamsNames = {};

                for (short int i = 0; i < 9; ++i)
                {
                    while (compare_data.executeStep())
                    {
                        ParamsV.push_back(compare_data.getColumn(i).getString());
                        ParamsNames.push_back(compare_data.getColumnName(i));
                    }
                    compare_data.reset();
                }



                SQLite::Statement delquery(db, u8"DELETE FROM finishedgoods WHERE " /*+ Params*/
                    + ParamsNames.at(1) + " = " + ParamsV.at(1)
                    + " AND " + ParamsNames.at(2) + " = " + "\'" + ParamsV.at(2) + "\'"
                    + " AND " + ParamsNames.at(3) + " = " + "\'" + ParamsV.at(3) + "\'"
                    + " AND " + "\"" + ParamsNames.at(4) + "\"" + " = " + "\'" + ParamsV.at(4) + "\'"
                    + " AND " + "\"" + ParamsNames.at(5) + "\"" + " = " + "\'" + ParamsV.at(5) + "\'"
                    + " AND " + "\"" + ParamsNames.at(6) + "\"" + " = " + "\'" + ParamsV.at(6) + "\'"
                    + " AND " + "\"" + ParamsNames.at(7) + "\"" + " = " + "\'" + ParamsV.at(7) + "\'"
                    + " AND " + "\"" + ParamsNames.at(8) + "\"" + " = " + "\'" + ParamsV.at(8) + "\'"

                );
                int rowsAffected = delquery.exec();
                //delquery.reset();

                if (rowsAffected > 0) {
                    bot.getApi().sendMessage(message->chat->id, u8"Товар ликвидирован.");
                }
                else {
                    bot.getApi().sendMessage(message->chat->id, u8"Товар с таким ID не найден.");
                }
            }
            catch (std::exception& e) {
                std::cerr << e.what();
                bot.getApi().sendMessage(message->chat->id, u8"Неверный ввод, нужен числовой ID.");
            }
            goodsdeleteMode[message->chat->id] = false;
            // bot.getApi().sendMessage(message->chat->id, "Deletion mode exited.", false, 0, MainKeyboard);
        }


        //change profit period
        if (profitperiod[message->chat->id]) {
            try {
                for (auto& el : message->text)
                {
                    if (!std::isdigit(el))
                    {
                        std::exception ex;
                        throw ex;
                    }
                }
                profitdays = std::stoi(message->text);
                profitperiod[message->chat->id] = false;
                bot.getApi().sendMessage(message->chat->id, u8"Успешно изменено.");
                

            }
            catch (std::exception& ex) {
                bot.getApi().sendMessage(message->chat->id, u8"Неверный ввод, требуются только числа");
                soldonemoretime = false;
                sold[message->chat->id] = false;
            }

        }
        //add finished goods
        if (fingoodsAddMode[message->chat->id])
        {
            fingoodsdata_toaddd.push_back(message->text);

            if (!MyPrefFinGoods.empty())
            {
                try {
                    ++addfingoodscounter;
                    if (addfingoodscounter > 3)
                    {

                        for (char& el : message->text) {
                            if (!std::isdigit(el))
                            {
                                std::exception o;

                                throw o;
                            }
                        }

                    }

                    TgBot::InlineKeyboardMarkup::Ptr keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();
                    AddFinGoodsFun(keyboard);
                    std::string temp;


                    SQLite::Statement fingoods_data(db, "SELECT * FROM finishedgoods");
                    while (fingoods_data.executeStep()) {
                        temp = fingoods_data.getColumnName(MyPrefFinGoods.at(0));

                    }
                    fingoods_data.reset();

                    bot.getApi().sendMessage(message->chat->id, temp, false, 0, keyboard);

                    MyPrefFinGoods.erase(MyPrefFinGoods.begin());
                }
                catch (std::exception& e) {
                    bot.getApi().sendMessage(user_id, u8"Неверный ввод, нужен числовой параметр для цен и количеств.");
                    fingoodsAddMode[user_id] = false;
                }
            }
            else if (MyPrefFinGoods.empty())
            {

                try {
                    std::string tcolums = "";
                    std::string tvalues = "";

                    if (addfingoodscounter > 3)
                    {

                        for (char& el : message->text) {
                            if (!std::isdigit(el))
                            {
                                std::exception o;

                                throw o;
                            }
                        }

                    }

                    for (auto& el : fingoodsdata_toaddd)
                    {
                        tvalues += '\'' + el + "\', ";
                    }
                    tvalues += "\'0\'";
                    //tvalues.pop_back();
                    //tvalues.pop_back();

                    int goodsarr[] = { 0, 1, 2, 3, 4, 5 , 6, 7, 8 };

                    for (auto& el : goodsarr) {

                        std::string ttempt;
                        SQLite::Statement fingoods_data(db, "SELECT * FROM finishedgoods");
                        while (fingoods_data.executeStep()) {
                            ttempt = fingoods_data.getColumnName(el);

                        }
                        fingoods_data.reset();

                        tcolums += '\'' + ttempt + "\', ";
                    }
                    tcolums.pop_back();
                    tcolums.pop_back();




                    SQLite::Statement ADDUSER(db, "INSERT INTO finishedgoods (" + tcolums + ") VALUES (" + tvalues + ");");
                    ADDUSER.exec();

                    fingoodsAddMode[message->chat->id] = false;
                    bot.getApi().sendMessage(message->chat->id, u8"Успешно добавлен.\nназад: /start");

                }
                catch (std::exception& e) {
                    bot.getApi().sendMessage(user_id, u8"Неверный ввод, нужен числовой параметр для цен и количеств.");
                    fingoodsAddMode[user_id] = false;
                }
            }













        }
        //change goods

        });
    
        
    

    try {
        std::cout << "Bot is running..." << std::endl;
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            longPoll.start();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }


    
    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    }
    catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }

}

    catch (const std::exception& e) {
        std::cerr << "SQLite error: " << e.what() << std::endl;
    }
    return 0;
}
