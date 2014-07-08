#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QChar>
#include <QHash>

class CommandParser
{
public:
    CommandParser(QString line, QHash<QString, QString> context);

    QString exe;
    QStringList args;
    QString error;

private:
    QString bufString;
    QString varString;
    QTextStream bufStream;
    QTextStream varStream;
    enum State {Normal, InsideQuote, QuoteEnd, Dollar, InsideBrace, BraceEnd};
    State state;
    State oldState;
    QChar quoteType;
    bool isProgram;

    void handleEndToken();
    void handleBraceEnd(QHash<QString, QString> &context, int col);
};

#endif // COMMANDPARSER_H
