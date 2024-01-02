#pragma once

#include <QSql>
#include <QSqlDriver>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <iostream>

class Database {

public:

	void Connect(const std::string& serverName, const std::string& username, const std::string& password);

	void QueryTest() {
        QSqlQuery query("SELECT * FROM test");

        if (!query.isActive()) {
            std::cerr << "Query error: " << query.lastError().text().toStdString() << std::endl;
        }

        while (query.next()) {
            QString column1 = query.value("id").toString();
            QString column2 = query.value("name").toString();

            std::cout << "Column1: " << column1.toStdString() << ", Column2: " << column2.toStdString() << std::endl;
        }

	}

	QSqlDatabase GetDatabase() const;

private:

	QSqlDatabase m_database;

	bool isConncted = false;
};