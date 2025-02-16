#include "functionality.h"



std::string ShouldBeIncluded(std::vector<int> FromEnum) {
	SQLite::Database db("test.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
	SQLite::Statement clients_data(db, "SELECT * FROM clients");
	std::string include = "";
	while (clients_data.executeStep()) {
		for (const auto& el : FromEnum)
		{
			include += clients_data.getColumnName(el);
			include += ": " + clients_data.getColumn(el).getString() + '\t';
		}
		include += '\n';
	}
	include += u8"назад: /start";
	
	clients_data.reset();
	return include; 
}

std::string GoodsShouldBeIncluded(std::vector<int> FromEnum) {
	SQLite::Database db("test.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
	SQLite::Statement temp_data(db, "SELECT * FROM sorted_table");
	std::string costilcounter = "";
	//std::string costilbrand = "";

	while (temp_data.executeStep()) {
		costilcounter = temp_data.getColumnName(7);
		//costilbrand = temp_data.getColumnName(3);
	}
	temp_data.reset();

	std::string include = u8"Вейпы:\n";
	try {
		SQLite::Statement vape_data(db, u8"SELECT * FROM sorted_table WHERE type = 1 AND " + costilcounter + " > 0");
		
		while (vape_data.executeStep()) {
			for (const auto& el : FromEnum)
			{
				include += vape_data.getColumnName(el);
				include += ": " + vape_data.getColumn(el).getString() + '\t';
			}

			include += '\n';
			include += '\n';
		}
		vape_data.reset();

	}
	catch (std::exception& e) {
		std::cerr << e.what();
		std::cerr << "no vapes\n";
	}
	include += u8"Жижки:\n";
	try {
		SQLite::Statement liquid_data(db, u8"SELECT * FROM sorted_table WHERE type = 2 AND " + costilcounter + " > 0");
		while (liquid_data.executeStep()) {
			for (const auto& el : FromEnum)
			{
				include += liquid_data.getColumnName(el);
				include += ": " + liquid_data.getColumn(el).getString() + '\t';
			}
			include += '\n';
		}
		liquid_data.reset();
	}
	catch (std::exception& e) {
		std::cerr << e.what();
		std::cerr << "no liquids\n";
	}
	include += u8"назад: /start";

	
	
	return include;
}

std::string FinishedGoodsShouldBeIncluded(std::vector<int> FromEnum, int SWprofitdays) {
	SQLite::Database db("test.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

	int date_period =  SWprofitdays - 1;

	std::string column_date = "";
	SQLite::Statement costil_query(db, u8"SELECT * FROM finishedgoods_sorted_table");
	while (costil_query.executeStep()) {
		column_date = costil_query.getColumnName(8);
	}
	costil_query.reset();




	SQLite::Statement vape_data(db, u8"SELECT * FROM finishedgoods_sorted_table WHERE \"" + column_date + "\" > " + std::to_string(date_period));
	std::string include = u8"Проданный товар:\n";
	while (vape_data.executeStep()) {
		for (const auto& el : FromEnum)
		{
			include += vape_data.getColumnName(el);
			include += ": " + vape_data.getColumn(el).getString() + '\t';
		}

		include += '\n';
		include += '\n';
	}
	include += u8"назад: /start";

	vape_data.reset();
	return include;
}

void DeleteClient(int ID) {
	SQLite::Database db("test.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
	SQLite::Statement delete_client(db, "DELETE FROM clients WHERE ID = ?;");
	delete_client.bind(1 , ID);
	delete_client.exec();
}


enum
{
	Clients = 0,
Goods= 1,
preferences= 2,
Add_Client = 3,
Show_Clients = 4,
Change_Client = 5,
Delete_Client = 6
};

int DetermineAction(std::string action) {
	if (action == u8"Клиенты")
	{
		return Clients;
	}
	else if (action == u8"Товар")
	{
		return Goods;
	}
	else if (action == u8"Настройки")
	{
		return preferences;
	}
	else if (action == u8"Добавить клиента")
	{
		return Add_Client;
	}
	else if (action == u8"Показать список клиентов")
	{
		return Show_Clients;
	}
	else if (action == u8"Изменить клиента")
	{
		return Change_Client;
	}
	else if (action == u8"Удалить клиента")
	{
		return Delete_Client;
	}
	else
	{
		return -1;
	}
}


void AddClientFun(TgBot::InlineKeyboardMarkup::Ptr keyboard) {
    

    // Create buttons
    std::vector<TgBot::InlineKeyboardButton::Ptr> row;
    TgBot::InlineKeyboardButton::Ptr button1 = std::make_shared<TgBot::InlineKeyboardButton>();
    button1->text = " - ";
    button1->callbackData = "button1_pressed";
    row.push_back(button1);

    keyboard->inlineKeyboard.push_back(row);
    
}

void AddGoodsFun(TgBot::InlineKeyboardMarkup::Ptr keyboard) {


	// Create buttons
	std::vector<TgBot::InlineKeyboardButton::Ptr> goodsrow;
	TgBot::InlineKeyboardButton::Ptr button1 = std::make_shared<TgBot::InlineKeyboardButton>();
	button1->text = " - ";
	button1->callbackData = "button2_pressed";
	goodsrow.push_back(button1);

	keyboard->inlineKeyboard.push_back(goodsrow);

}

void AddFinGoodsFun(TgBot::InlineKeyboardMarkup::Ptr keyboard) {


	// Create buttons
	std::vector<TgBot::InlineKeyboardButton::Ptr> goodsrow;
	TgBot::InlineKeyboardButton::Ptr button1 = std::make_shared<TgBot::InlineKeyboardButton>();
	button1->text = " - ";
	button1->callbackData = "button2fin_pressed";
	goodsrow.push_back(button1);

	keyboard->inlineKeyboard.push_back(goodsrow);

}

int SWprofitdaysFun(int profitdays, int monthvalues) {

	auto now = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);
	std::tm local_tm;
	localtime_s(&local_tm, &now_c);

	int year = local_tm.tm_year + 1900;
	int month = local_tm.tm_mon + 1 - monthvalues;
	if (month == 0)
	{
		month = 12;
		--year;
	}
	int day = local_tm.tm_mday;
	if (monthvalues>0)
	{
		day = 30;
	}
	
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

	int checker = profitdays;
	if (day - checker <0)
	{
		int need_to_return = SWprofitdaysFun(checker - day, ++monthvalues);
		return need_to_return;
	}
	if (month < 10)
	{
		oformonth = "0";
	}

	day -= checker;
	if (day < 10)
	{
		oforday = "0";
	}



	std::string currentDate = std::to_string(year) + oformonth + std::to_string(month) + oforday + std::to_string(day);

	return std::stoi(currentDate);
}