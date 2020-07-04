#ifndef FOLDERCONFIG_H
#define FOLDERCONFIG_H

#include <QString>
#include "AbstractConfig.h"

namespace QtDBMigrationNS {

class FolderConfig : public AbstractConfig
{
public:
    FolderConfig(const QString &path);

    bool load();
    bool filterFiles(QFile &file, QRegExp rx);
};
}

#endif // FOLDERCONFIG_H
