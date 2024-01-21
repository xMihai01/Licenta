#include "Model/Utils/JsonFile.h"

JsonFile::JsonFile(const QString& fileName, const QString& keyName, const std::vector<QString>& cellNames)
	: m_cellNames(cellNames)
    , m_keyName(keyName)
{
	m_file = new QFile(fileName);
}

void JsonFile::WriteToFile(const std::vector<QString>& data, const QString& key)
{
	QJsonArray jsonArray = GetFileData();
    if (data.size() != m_cellNames.size()) {
        qDebug() << "Given data size doesn't match number of cells given by the vector of cell names in the constructor!";
        return;
    }
    if (ReadFromFile(key).size() > 0) {
        UpdateElement(data, key);
        return;
    }
    QJsonObject jsonData;
    jsonData[m_keyName] = key;
    for (size_t index = 0; index < data.size(); index++) 
        jsonData[m_cellNames[index]] = data[index];
    
    jsonArray.append(jsonData);

    QJsonDocument jsonDocument(jsonArray);

    if (m_file->open(QIODevice::WriteOnly)) {
        m_file->write(jsonDocument.toJson());
        m_file->close();
    }
    else {
        qDebug() << "File couldn't open!";
    }
}

void JsonFile::RemoveElement(const QString& key)
{
    if (!m_file->open(QIODevice::ReadWrite | QIODevice::Text)) {
        return;
    }

    QByteArray fileData = m_file->readAll();
    m_file->close();

    QJsonParseError parseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(fileData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "JSON parsing error: " << parseError.errorString();
        return;
    }

    if (jsonDocument.isArray()) {
        QJsonArray jsonArray = jsonDocument.array();

        for (size_t index = 0; index < jsonArray.size(); ++index) {
            QJsonValue jsonValue = jsonArray.at(index);
            if (jsonValue.isObject()) {
                QJsonObject jsonObject = jsonValue.toObject();
                if (jsonObject.contains(m_keyName) && jsonObject[m_keyName].toString() == key) {
                    jsonArray.removeAt(index);
                }
            }
        }
        jsonDocument.setArray(jsonArray);
    }
    if (m_file->open(QIODevice::WriteOnly)) {
        m_file->write(jsonDocument.toJson());
        m_file->close();
    }
    else {
        qDebug() << "File couldn't open!";
    }
}

void JsonFile::UpdateElement(const std::vector<QString>& data, const QString& key)
{
    if (!m_file->open(QIODevice::ReadOnly) && data.size() != m_cellNames.size()) {
        return;
    }

    QByteArray fileData = m_file->readAll();
    m_file->close();

    QJsonParseError parseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(fileData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "JSON parsing error: " << parseError.errorString();
        return;
    }

    if (jsonDocument.isArray()) {
        QJsonArray jsonArray = jsonDocument.array();

        for (size_t index = 0; index < jsonArray.size(); ++index) {
            QJsonValue jsonValue = jsonArray.at(index);
            if (jsonValue.isObject()) {
                QJsonObject jsonObject = jsonValue.toObject();
                if (jsonObject.contains(m_keyName) && jsonObject[m_keyName].toString() == key) {
                    for (size_t index2 = 0; index2 < m_cellNames.size(); index2++) 
                        jsonObject[m_cellNames[index2]] = data[index2];
                    jsonArray.replace(index, jsonObject);
                }
            }
        }
        jsonDocument.setArray(jsonArray);
    }
    if (m_file->open(QIODevice::WriteOnly)) {
        m_file->write(jsonDocument.toJson());
        m_file->close();
    }
    else {
        qDebug() << "File couldn't open!";
    }
}

std::vector<QString> JsonFile::ReadAllFromFile()
{
    std::vector<QString> readData;
    if (!m_file->open(QIODevice::ReadOnly)) {
        return readData;
    }

    QByteArray fileData = m_file->readAll();
    m_file->close();

    QJsonParseError parseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(fileData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "JSON parsing error: " << parseError.errorString();
        return readData;
    }
    if (jsonDocument.isArray()) {
        QJsonArray jsonArray = jsonDocument.array();
        for (const QJsonValue& jsonValue : jsonArray) {
            if (jsonValue.isObject()) {
                QJsonObject jsonObject = jsonValue.toObject();
                readData.push_back(jsonObject[m_keyName].toString());
                for (size_t index = 0; index < m_cellNames.size(); index++)
                    readData.push_back(jsonObject[m_cellNames[index]].toString());
            }
        }
    }
    return readData;
}

QJsonArray JsonFile::GetFileData()
{

    if (!m_file->open(QIODevice::ReadOnly)) {
        return QJsonArray();
    }

    QByteArray fileData = m_file->readAll();
    m_file->close();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "JSON parsing error: " << parseError.errorString();
        return QJsonArray();
    }

    if (jsonDoc.isArray())
        return jsonDoc.array();
    else
        return QJsonArray();
}

std::vector<QString> JsonFile::ReadFromFile(const QString& key)
{
    std::vector<QString> readData;
    if (!m_file->open(QIODevice::ReadOnly)) {
        return readData;
    }

    QByteArray fileData = m_file->readAll();
    m_file->close();

    QJsonParseError parseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(fileData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "JSON parsing error: " << parseError.errorString();
        return readData;
    }
    if (jsonDocument.isArray()) {
        QJsonArray jsonArray = jsonDocument.array();
        for (const QJsonValue& jsonValue : jsonArray) {
            if (jsonValue.isObject()) {
                QJsonObject jsonObject = jsonValue.toObject();
                if (jsonObject.contains(m_keyName) && jsonObject[m_keyName].toString() == key) {
                    readData.push_back(jsonObject[m_keyName].toString());
                    for (size_t index = 0; index < m_cellNames.size(); index++)
                        readData.push_back(jsonObject[m_cellNames[index]].toString());
                }
            }
        }
    }
    return readData;
}

JsonFile::~JsonFile()
{
	delete m_file;
}

