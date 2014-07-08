#include "commandparser.h"

CommandParser::CommandParser(QString line, QHash<QString, QString> context)
{
    bufStream.setString(&bufString);
    varStream.setString(&varString);
    state = Normal;
    isProgram = true;
    for (int i = 0; i < line.size(); i += 1) {
        QChar c = line.at(i);
        switch (state) {
        case Normal:
            if (c == '"' || c == '\'') {
                state = InsideQuote;
                quoteType = c;
            } else if (c.isSpace()) {
                handleEndToken();
            } else if (c == '$') {
                oldState = state;
                state = Dollar;
            } else {
                bufStream << c;
            }
            break;
        case InsideQuote:
            if (c == quoteType) {
                state = QuoteEnd;
            } else if (c == '$') {
                oldState = state;
                state = Dollar;
            } else {
                bufStream << c;
            }
            break;
        case QuoteEnd:
            if (c == quoteType) {
                bufStream << c;
                state = Normal;
            } else if (c.isSpace()) {
                handleEndToken();
                state = Normal;
            } else {
                error = QString("Column %1: Expected space or end").arg(i);
                return;
            }
            break;
        case Dollar:
            if (c == '{') {
                state = InsideBrace;
            } else if (c == '$') {
                bufStream << c;
                state = oldState;
            } else {
                error = QString("Column %1: Expected '{' or '$'").arg(i);
            }
            break;
        case InsideBrace:
            if (c == '}') {
                state = BraceEnd;
            } else {
                varStream << c;
            }
            break;
        case BraceEnd:
            if (c == '}') {
                varStream << c;
                state = InsideBrace;
            } else {
                handleBraceEnd(context, i);

                state = oldState;
                i -= 1;
                continue;
            }
            break;
        }
    }
    switch (state) {
    case Normal:
        handleEndToken();
        break;
    case Dollar:
        error = QString("Column %1: Expected '{' or '$'").arg(line.size());
        break;
    case QuoteEnd:
        handleEndToken();
        break;
    case InsideQuote:
        error = QString("Column %1: Expected end quote").arg(line.size());
        break;
    case InsideBrace:
        error = QString("Column %1: Expected '}'").arg(line.size());
        break;
    case BraceEnd:
        handleBraceEnd(context, line.size());
        if (error.isNull())
            handleEndToken();
        break;
    }
}

void CommandParser::handleEndToken()
{
    // ignore multiple end tokens in a row
    if (bufString.size() == 0)
        return;

    QString value = bufStream.readAll();
    if (isProgram) {
        exe = value;
        isProgram = false;
    } else {
        args << value;
    }
    bufString.clear();
    bufStream.setString(&bufString);
}

void CommandParser::handleBraceEnd(QHash<QString, QString> &context, int col)
{
    QString varName = varStream.readAll();
    if (!context.contains(varName)) {
        error = QString("Column %1: Variable %2 not found").arg(QString::number(col), varName);
        return;
    }
    varString.clear();
    varStream.setString(&varString);
    bufStream << context.value(varName);
}

