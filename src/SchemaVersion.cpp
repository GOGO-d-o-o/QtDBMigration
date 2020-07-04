// Copyright (c) 2014 Archangel.SDY@gmail.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.


#include "SchemaVersion.h"

namespace QtDBMigrationNS {

SchemaVersion::SchemaVersion(int version,
                             const QStringList &applySql,
                             const QStringList &revertSql) :
    m_version(version) ,
    m_applySql(applySql) ,
    m_revertSql(revertSql)
{
}

bool SchemaVersion::apply(const QSqlDatabase &db)
{
    QSqlQuery beginTransaction(db);
    QSqlQuery q(db);
    QSqlQuery commitTransaction(db);

    beginTransaction.prepare("begin transaction;");
    beginTransaction.exec();
    for (QString query : m_applySql)
    {
        if(!q.prepare(query))
        {
            qDebug() << "Database engine error:" << q.lastError().text() << ", Database engine error code:" << q.lastError().nativeErrorCode();
        }

        if (!q.exec()) {
            qDebug() << "Database engine error:" << q.lastError().text() << ", Database engine error code:" << q.lastError().nativeErrorCode();
            return false;
        }
    }

    QSqlQuery upgradeVer(db);
    upgradeVer.prepare("update qtdbmigration_meta set cur_ver = cur_ver + 1;");
    upgradeVer.exec();

    commitTransaction.prepare("commit;");
    return commitTransaction.exec();
}

bool SchemaVersion::revert(const QSqlDatabase &db)
{
    QSqlQuery beginTransaction(db);
    QSqlQuery q(db);
    QSqlQuery commitTransaction(db);

    beginTransaction.prepare("begin transaction;");
    beginTransaction.exec();

    for (QString query : m_revertSql)
    {
        q.prepare(query);
        if (!q.exec()) {
            qDebug() << "Database engine error:" << q.lastError().text() << ", Database engine error code:" << q.lastError().nativeErrorCode();
            return false;
        }
    }

    QSqlQuery downgradeVer(db);
    downgradeVer.prepare("update qtdbmigration_meta set cur_ver = cur_ver - 1");
    downgradeVer.exec();

    commitTransaction.prepare("commit;");
    return commitTransaction.exec();
  }
}
