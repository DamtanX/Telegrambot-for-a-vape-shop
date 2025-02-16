#pragma once
#include <iostream>
#include <stdio.h>
#include <tgbot/tgbot.h>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <chrono>
#include <ctime>

#define SQLITECPP_COMPILE_DLL
#define _CRT_SECURE_NO_WARNINGS
#include <SQLiteCpp/SQLiteCpp.h>



std::string ShouldBeIncluded(std::vector<int> FromEnum);
void DeleteClient(int ID);
int DetermineAction(std::string action);
void AddClientFun(TgBot::InlineKeyboardMarkup::Ptr keyboard);


std::string GoodsShouldBeIncluded(std::vector<int> FromEnum);
std::string FinishedGoodsShouldBeIncluded(std::vector<int> FromEnum, int SWprofitdays);
void AddGoodsFun(TgBot::InlineKeyboardMarkup::Ptr keyboard);
void AddFinGoodsFun(TgBot::InlineKeyboardMarkup::Ptr keyboard);

int SWprofitdaysFun(int profitdays, int monthvalues);
