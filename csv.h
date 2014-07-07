#ifndef CSV_H
#define CSV_H

#include <QStringList>

namespace CSV {
    QList<QStringList> parseFromString(const QString &string);
    QList<QStringList> parseFromFile(const QString &filename);
}

#endif // CSV_H
