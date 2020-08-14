#include "FolderConfig.h"
#include <QFile>
#include <QDebug>
#include<QDir>

namespace QtDBMigrationNS {
FolderConfig::FolderConfig(const QString &path): AbstractConfig(path)
{
}

QString getOperation(QString line, QString operation)
{
    QRegularExpression rxOperation("^-- \\+migrate (?<operation>(Up|Down))");
    QRegularExpressionMatch operationMatch = rxOperation.match(line);
    if (operationMatch.hasMatch())
    {
        operation = operationMatch.captured("operation");
    }
    return operation;
}


bool FolderConfig::load()
{
    QRegExp rx("^\\d{4}\\.sql$");
    QDir dir(m_path);
    QStringList nameFilter("*.sql");
    QStringList sqlFiles = dir.entryList(nameFilter);

    int totalFiles = sqlFiles.count();

    if (!dir.exists() || totalFiles < 1) {
        return false;
    }

    for(auto fileName : sqlFiles) {
        QString operation = "";
        QString filePath = dir.filePath(fileName);
        QFile file(filePath);
        if(!file.exists())
        {
            return false;
        }
        if (!rx.exactMatch(fileName)) {
            continue;
        }

        if(!file.open(QFile::ReadOnly | QIODevice::Text)) {
            return false;
        }
        QTextStream in(&file);
        QString line = "";
        QStringList applySql;
        QStringList revertSql;
        QString compoundStatement;
        do {
            line = in.readLine();
            if (line.size()==0) {
                continue;
            }
            if (operation == "Up" && !line.contains("-- +migrate")) {
                compoundStatement.append(line);
                if(compoundStatement.contains(";")) {
                    applySql.append(compoundStatement);
                    compoundStatement = "";
                }
            } else if (operation == "Down" && !line.contains("-- +migrate")) {
                compoundStatement.append(line);
                if(compoundStatement.contains(";")) {
                    revertSql.append(compoundStatement);
                    compoundStatement="";
                }
            } else {
                operation = getOperation(line,operation);
                continue;
            }
        }
        while(!in.atEnd());

        QString version = rx.capturedTexts()[0].split(".")[0];
        if (version=="")
            continue;

        m_schemaVersions << new SchemaVersion(
                            version.toInt(), applySql, revertSql);
    }
    return true;
   }
}


