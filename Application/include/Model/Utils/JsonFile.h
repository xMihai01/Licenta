#pragma once
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>

#include <vector>
//!  JsonFile class
/*!
	JsonFile is used for simple writing and reading to a Json file.
*/
class JsonFile
{
public:

	JsonFile(const QString& fileName, const QString& keyName, const std::vector<QString>& cellNames);

	/*!
		@brief Writes data to file

		Writes data to file. It may only add one entry at a time and the data vector must be equal to the number of cells (excluding the key)
		\n A key is just another cell that will be used in other functions for updating or removing specific elements with a given key.
		\n If the key already exists, the corresponding data will be updated.
		
		@param data Data to write.
		@param key A QString to look for in all cells named after given keyName
	*/
	void WriteToFile(const std::vector<QString>& data, const QString& key);

	/*!
		@brief Removes a specific element found by a given key

		@param key A QString to look for in all cells named after given keyName
	*/
	void RemoveElement(const QString& key);

	/*!
		@brief Updates a specific element found by a given key with new data

		Updates data in file. It may only update one entry at a time and the data vector must be equal to the number of cells (excluding the key)
		\n A key is just another cell that will be used in other functions for updating or removing specific elements with a given key.

		@param data New data to write.
		@param key A QString to look for in all cells named after given keyName
	*/
	void UpdateElement(const std::vector<QString>& data, const QString& key);

	/*!
		@brief Reads everything from the file.

		Reads everything from the file into a QString vector. 
		\n The data in the vector will be in the following order: Value of key + the rest of the data in the same order the cells have been declared in the constructor

		@return Returns all data in a QString vector.
	*/
	std::vector<QString> ReadAllFromFile();

	/*!
		@brief Reads a specific item using it's key from the file.

		Reads a specific item using it's key from the file into a QString vector.
		\n The data in the vector will be in the following order: Value of key + the rest of the data in the same order the cells have been declared in the constructor

		@param key A Qstring to look for in all cells named after given keyName

		@return Returns item's data in a QString vector.
	*/
	std::vector<QString> ReadFromFile(const QString& key);

	~JsonFile();

private:

	/*!
		@brief Gets all data from the QFile

		@return A QJsonArray containing all data from the file
	*/
	QJsonArray GetFileData();

private:

	QFile* m_file;

	std::vector<QString> m_cellNames; /*!<All cell names*/
	QString m_keyName; /*!<Special cell, used for updating or removing specific elements*/

};

