#include "csv.h"

#include <QFile>
#include <QTextStream>
#include <QTextCodec>

static QList<QStringList> parse(const QString &string)
{
    enum State {Normal, Quote} state = Normal;
    QList<QStringList> data;
    QStringList line;
    QString value;

    for (int i = 0; i < string.size(); i++) {
        QChar current = string.at(i);

        // Normal
        if (state == Normal) {
            // newline
            if (current == '\n') {
                // add value
                line.append(value);
                value.clear();
                // add line
                data.append(line);
                line.clear();
            }
            // comma
            else if (current == ',') {
                // add line
                line.append(value);
                value.clear();
            }
            // double quote
            else if (current == '"') {
                state = Quote;
            }
            // character
            else {
                value += current;
            }
        }
        // Quote
        else if (state == Quote) {
            // double quote
            if (current == '"') {
                int index = (i+1 < string.size()) ? i+1 : string.size();
                QChar next = string.at(index);
                if (next == '"') {
                    value += '"';
                    i++;
                } else {
                    state = Normal;
                }
            }
            // other
            else {
                value += current;
            }
        }
    }

    return data;
}

static QString initString(const QString &string)
{
    QString result = string;
    result.replace("\r\n", "\n");
    if (result.at(result.size()-1) != '\n') {
        result += '\n';
    }
    return result;
}

QList<QStringList> CSV::parseFromString(const QString &string)
{
    return parse(initString(string));
}

QList<QStringList> CSV::parseFromFile(const QString &filename)
{
    QString string;
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        in.setCodec(QTextCodec::codecForName("UTF-8"));
        string = in.readAll();
        file.close();
    }
    return parse(initString(string));
}
