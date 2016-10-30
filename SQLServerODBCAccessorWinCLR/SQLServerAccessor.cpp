#include "stdafx.h" // ✝

#include "SQLServerAccessor.h"

void WCharString::Assign(char * arg)
{
    value.assign(arg);
}

void WCharString::Assign(wchar_t * arg)
{
    wstring castedArg = arg;
    size_t argSize = castedArg.length() + 1;
    size_t cnvSize = 0;
    unique_ptr<char> mbArg(new char[argSize]);
    wcstombs_s(&cnvSize, mbArg.get(), argSize, arg, _TRUNCATE);
    value.assign(mbArg.get());
}

void WCharString::Assign(string arg)
{
    value.assign(arg);
}

void WCharString::Assign(string * arg)
{
    value.assign(*arg);
}

void WCharString::Assign(const char * arg)
{
    value.assign(arg);
}

void WCharString::Assign(const wchar_t * arg)
{
    wstring castedArg = arg;
    size_t argSize = castedArg.length() + 1;
    size_t cnvSize = 0;
    unique_ptr<char> mbArg(new char[argSize]);
    wcstombs_s(&cnvSize, mbArg.get(), argSize, arg, _TRUNCATE);
    value.assign(mbArg.get());
}

void WCharString::Assign(const string * arg)
{
    value.assign(*arg);
}

WCharString WCharString::Append(char * arg)
{
    value.append(arg);
    return *this;
}

WCharString WCharString::Append(wchar_t * arg)
{
    wstring castedArg = arg;
    size_t argSize = castedArg.length() + 1;
    size_t cnvSize = 0;
    unique_ptr<char> mbArg(new char[argSize]);
    wcstombs_s(&cnvSize, mbArg.get(), argSize, arg, _TRUNCATE);
    value.append(mbArg.get());
    return *this;
}

WCharString WCharString::Append(string arg)
{
    value.append(arg);
    return *this;
}

WCharString WCharString::Append(string * arg)
{
    value.append(*arg);
    return *this;
}

WCharString WCharString::Append(const char * arg)
{
    value.append(arg);
    return *this;
}

WCharString WCharString::Append(const wchar_t * arg)
{
    wstring castedArg = arg;
    size_t argSize = castedArg.length() + 1;
    size_t cnvSize = 0;
    unique_ptr<char> mbArg(new char[argSize]);
    wcstombs_s(&cnvSize, mbArg.get(), argSize, arg, _TRUNCATE);
    value.append(mbArg.get());
    return *this;
}

WCharString WCharString::Append(const string * arg)
{
    value.append(*arg);
    return *this;
}

WCharString WCharString::Value(char * arg)
{
    Assign(arg);
    return *this;
}

WCharString WCharString::Value(wchar_t * arg)
{
    Assign(arg);
    return *this;
}

WCharString WCharString::Value(string arg)
{
    Assign(arg);
    return *this;
}

WCharString WCharString::Value(string * arg)
{
    Assign(arg);
    return *this;
}

WCharString WCharString::Value(const char * arg)
{
    Assign(arg);
    return *this;
}

WCharString WCharString::Value(const wchar_t * arg)
{
    Assign(arg);
    return *this;
}

WCharString WCharString::Value(const string * arg)
{
    Assign(arg);
    return *this;
}

unique_ptr<wchar_t> WCharString::ToWChar()
{
    size_t retSize = value.length() + 1;
    size_t cnvSize = 0;
    unique_ptr<wchar_t> ret(new wchar_t[retSize]);
    mbstowcs_s(&cnvSize, ret.get(), retSize, value.c_str(), _TRUNCATE);
    return ret;
}

string WCharString::ToString()
{
    return value;
}

bool WCharString::WChar_tStartsWith(wchar_t * arg1eval, string arg2test)
{
    wstring castedArg1 = arg1eval;
    if (castedArg1.length() < arg2test.length()) {
        return false;
    }
    for (size_t i = 0; i < arg2test.length(); i++) {
        if (castedArg1.at(i) != arg2test.at(i)) {
            return false;
        }
    }
    return true;
}

void WCharString::SysErrWinAPI()
{
    LPVOID msg = NULL;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&msg,
        0,
        NULL);
    string out = this->Value((const wchar_t *)msg).ToString();
    if (out.substr(out.length() - 2, 2) == string("\r\n")) {
        out = out.substr(0, out.length() - 2);
    }
    cout << out << "\n";
}

WCharString::WCharString()
{
}

WCharString::~WCharString()
{
}

void NumericBinder::SetType(SQLSMALLINT arg)
{
    rawColumnType = arg;
}

IBinder::ColumnType NumericBinder::GetType()
{
    return IBinder::ColumnType::NUMERIC;
}

void NumericBinder::SetIndex(int arg)
{
    index = arg;
}

int NumericBinder::GetIndex()
{
    return index;
}

void NumericBinder::SetSize(SQLLEN arg)
{
    size = arg;
}

SQLLEN NumericBinder::GetSize()
{
    return size;
}

void NumericBinder::SetScale(SQLSMALLINT arg)
{
    scale = arg;
}

SQLSMALLINT NumericBinder::GetScale()
{
    return scale;
}

void NumericBinder::SetValue(double arg)
{
    switch (thisScale) {
    case ValueScale::SHORT:
        svalue = (short)arg;
        break;
    case ValueScale::INT:
        ivalue = (int)arg;
        break;
    case ValueScale::FLOAT:
        fvalue = (float)arg;
        break;
    default:
        dvalue = arg;
        break;
    }
}

double NumericBinder::GetValue()
{
    switch (thisScale) {
    case ValueScale::SHORT:
        return svalue;
    case ValueScale::INT:
        return ivalue;
    case ValueScale::FLOAT:
        return fvalue;
    default:
        return dvalue;
    }
}

void NumericBinder::Bind(SQLHANDLE statement)
{
    bindSuccess = false;
    if (scale == 0) {
        if (size < 10) {
            thisScale = ValueScale::SHORT;
            returnCode = SQLBindCol(
                statement,
                index + 1,
                SQL_C_SHORT,
                (SQLPOINTER)&svalue,
                size,
                &nullIndicator);
        }
        else {
            thisScale = ValueScale::INT;
            returnCode = SQLBindCol(
                statement,
                index + 1,
                SQL_C_SLONG,
                (SQLPOINTER)&ivalue,
                size,
                &nullIndicator);
        }
    }
    else {
        if (size < 10) {
            thisScale = ValueScale::FLOAT;
            returnCode = SQLBindCol(
                statement,
                index + 1,
                SQL_C_FLOAT,
                (SQLPOINTER)&fvalue,
                size,
                &nullIndicator);
        }
        else {
            thisScale = ValueScale::DOUBLE;
            returnCode = SQLBindCol(
                statement,
                index + 1,
                SQL_C_DOUBLE,
                (SQLPOINTER)&dvalue,
                size,
                &nullIndicator);
        }
    }
    if (returnCode != SQL_SUCCESS && returnCode != SQL_SUCCESS_WITH_INFO) {
        WCharString wcs;
        wcs.SysErrWinAPI();
    }
    else {
        bindSuccess = true;
    }
}

void NumericBinder::BindParam(SQLHANDLE statement)
{
    bindSuccess = false;
    SQLLEN cbTextSize = SQL_LEN_DATA_AT_EXEC(size);
    if (scale == 0) {
        if (size < 10) {
            thisScale = ValueScale::SHORT;
            returnCode = SQLBindParameter(
                statement,
                index + 1,
                SQL_PARAM_INPUT,
                SQL_C_SHORT,
                SQL_TINYINT,
                size,
                scale,
                (SQLPOINTER)&svalue,
                0,
                &cbTextSize);
        }
        else {
            thisScale = ValueScale::INT;
            returnCode = SQLBindParameter(
                statement,
                index + 1,
                SQL_PARAM_INPUT,
                SQL_C_SLONG,
                SQL_BIGINT,
                size,
                scale,
                (SQLPOINTER)&ivalue,
                0,
                &cbTextSize);
        }
    }
    else {
        if (size < 10) {
            thisScale = ValueScale::FLOAT;
            returnCode = SQLBindParameter(
                statement,
                index + 1,
                SQL_PARAM_INPUT,
                SQL_C_FLOAT,
                SQL_REAL,
                size,
                scale,
                (SQLPOINTER)&fvalue,
                0,
                &cbTextSize);
        }
        else {
            thisScale = ValueScale::DOUBLE;
            returnCode = SQLBindParameter(
                statement,
                index + 1,
                SQL_PARAM_INPUT,
                SQL_C_DOUBLE,
                SQL_DOUBLE,
                size,
                scale,
                (SQLPOINTER)&dvalue,
                0,
                &cbTextSize);
        }
    }
    if (returnCode != SQL_SUCCESS && returnCode != SQL_SUCCESS_WITH_INFO) {
        WCharString wcs;
        wcs.SysErrWinAPI();
    }
    else {
        bindSuccess = true;
    }
}

SQLRETURN NumericBinder::GetReturnCode()
{
    return returnCode;
}

string NumericBinder::GetErrorMessage()
{
    return errorMessage;
}

bool NumericBinder::GetBindSuccess()
{
    return bindSuccess;
}

NumericBinder::NumericBinder()
{
}

NumericBinder::~NumericBinder()
{
}

void StringBinder::SetType(SQLSMALLINT arg)
{
    rawColumnType = arg;
}

IBinder::ColumnType StringBinder::GetType()
{
    return IBinder::ColumnType::STRING;
}

void StringBinder::SetIndex(int arg)
{
    index = arg;
}

int StringBinder::GetIndex()
{
    return index;
}

void StringBinder::SetSize(SQLLEN arg)
{
    size = arg;
}

SQLLEN StringBinder::GetSize()
{
    return size;
}

void StringBinder::SetScale(SQLSMALLINT arg)
{
    scale = arg;
}

SQLSMALLINT StringBinder::GetScale()
{
    return scale;
}

void StringBinder::SetValue(wchar_t * arg)
{
    value = arg;
}

wchar_t * StringBinder::GetValue()
{
    return value;
}

void StringBinder::Bind(SQLHANDLE statement)
{
    bindSuccess = false;

    if (rawColumnType == 12) {      // SQL_VARCHAR のことを表す ( sql.h )
        rawColumnType = 1;          // SQL_CHAR ( = SQL_C_CHAR ) のことを表す ( sql.h )
    }                               // もう ODBC API 中マジックナンバーまみれや

    value = new wchar_t[size];
    returnCode = SQLBindCol(
        statement,
        index + 1,
        SQL_C_TCHAR,                // 実質 SQL_CHAR のこと ( sqlucode.h ) これもうわかんねえな・・・
        (SQLPOINTER)value,
        (size + 1) * sizeof(wchar_t),
        &nullIndicator);
    if (returnCode != SQL_SUCCESS && returnCode != SQL_SUCCESS_WITH_INFO) {
        WCharString wcs;
        wcs.SysErrWinAPI();
    }
    else {
        bindSuccess = true;
    }
}

void StringBinder::BindParam(SQLHANDLE statement)
{
    bindSuccess = false;

    SQLLEN cbTextSize = SQL_LEN_DATA_AT_EXEC(size);
    returnCode = SQLBindParameter(
        statement,
        index + 1,
        SQL_PARAM_INPUT,
        SQL_C_TCHAR,
        SQL_VARCHAR,
        size,
        0,
        (SQLPOINTER)value,
        0,
        &cbTextSize);
    if (returnCode != SQL_SUCCESS && returnCode != SQL_SUCCESS_WITH_INFO) {
        WCharString wcs;
        wcs.SysErrWinAPI();
    }
    else {
        bindSuccess = true;
    }
}

SQLRETURN StringBinder::GetReturnCode()
{
    return returnCode;
}

string StringBinder::GetErrorMessage()
{
    return errorMessage;
}

bool StringBinder::GetBindSuccess()
{
    return bindSuccess;
}

StringBinder::StringBinder()
{
}

StringBinder::~StringBinder()
{
    if (value != nullptr) {
        delete value;
    }
}

void TableBinder::AddBinder(IBinder * arg)
{
    columnVector->push_back(arg);
}

bool TableBinder::Bind(SQLHANDLE statement)
{
    for (size_t i = 0; i < columnVector->size(); i++) {
        columnVector->at(i)->Bind(statement);
        if (!columnVector->at(i)->GetBindSuccess()) {
            returnCode = columnVector->at(i)->GetReturnCode();
            errorMessage = columnVector->at(i)->GetErrorMessage();
            return false;
        }
    }
    return true;
}

bool TableBinder::BindParams(SQLHANDLE statement)
{
    for (size_t i = 0; i < columnVector->size(); i++) {
        columnVector->at(i)->BindParam(statement);
        if (!columnVector->at(i)->GetBindSuccess()) {
            returnCode = columnVector->at(i)->GetReturnCode();
            errorMessage = columnVector->at(i)->GetErrorMessage();
            return false;
        }
    }
    return true;
}

SQLRETURN TableBinder::GetReturnCode()
{
    return returnCode;
}

string TableBinder::GetErrorMessage()
{
    return errorMessage;
}

vector<IBinder *> * TableBinder::Get()
{
    return columnVector;
}

TableBinder::TableBinder()
{
    columnVector = new vector<IBinder *>();
}

TableBinder::~TableBinder()
{
    for (size_t i = 0; i < columnVector->size(); i++) {
        delete columnVector->at(i);
    }
    delete columnVector;
}

IData::DataType NumericData::GetType()
{
    return IData::DataType::NUMERIC;
}

void NumericData::SetData(short arg)
{
    thisScale = ValueScale::SHORT;
    svalue = arg;
}

void NumericData::SetData(int arg)
{
    thisScale = ValueScale::INT;
    ivalue = arg;
}

void NumericData::SetData(float arg)
{
    thisScale = ValueScale::FLOAT;
    fvalue = arg;
}

void NumericData::SetData(double arg)
{
    thisScale = ValueScale::DOUBLE;
    dvalue = arg;
}

double NumericData::GetData()
{
    switch (thisScale) {
    case ValueScale::SHORT:
        return svalue;
    case ValueScale::INT:
        return ivalue;
    case ValueScale::FLOAT:
        return fvalue;
    default:
        return dvalue;
    }
}

NumericData::NumericData()
{
}

NumericData::~NumericData()
{
}

IData::DataType StringData::GetType()
{
    return IData::DataType::STRING;
}

void StringData::SetData(wchar_t * arg)
{
    value = arg;
}

wchar_t * StringData::GetData()
{
    return value;
}

StringData::StringData()
{
}

StringData::~StringData()
{
}

bool SQLServerAccessor::DescribeTable()
{
    SQLSMALLINT columnsCount;
    SQLNumResultCols(statement, &columnsCount);

    for (SQLSMALLINT i = 0; i < columnsCount; i++) {
        SQLWCHAR columnName[128];
        SQLSMALLINT columnNameSize;
        SQLSMALLINT columnType;
        SQLULEN columnSize;
        SQLSMALLINT scale;
        SQLSMALLINT nullable;
        rc = SQLDescribeCol(
            statement,
            (SQLUSMALLINT)(i + 1),
            columnName,
            (SQLSMALLINT)sizeof(columnName),
            &columnNameSize,
            &columnType,
            &columnSize,
            &scale,
            &nullable);
        if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO) {
            wcharStr.SysErrWinAPI();
            return false;
        }
        IBinder * addBinder;
        switch (columnType) {
        case SQL_CHAR:
        case SQL_VARCHAR:
            addBinder = new StringBinder();
            addBinder->SetType(columnType);
            addBinder->SetIndex(i);
            addBinder->SetSize(columnSize);
            addBinder->SetScale(scale);
            break;
        case SQL_NUMERIC:
        case SQL_DECIMAL:
        case SQL_INTEGER:
        case SQL_SMALLINT:
        case SQL_FLOAT:
        case SQL_DOUBLE:
            addBinder = new NumericBinder();
            addBinder->SetType(columnType);
            addBinder->SetIndex(i);
            addBinder->SetSize(columnSize);
            addBinder->SetScale(scale);
            break;
        default:
            break;
        }
        table->AddBinder(addBinder);
    }

    return table->Bind(statement);
}

bool SQLServerAccessor::ExecDML(string arg)
{
    bool allDataProcessed = false;

    int rowPos = 0;
    do {
        unique_ptr<wchar_t> query = move(wcharStr.Value(arg).ToWChar());
        rc = SQLExecDirect(statement, query.get(), SQL_NTS);
        if (rc != SQL_SUCCESS && rc != SQL_NEED_DATA && rc != SQL_SUCCESS_WITH_INFO) {
            HandleDiagnosticRecord(statement, SQL_HANDLE_STMT, rc);
            wcharStr.SysErrWinAPI();
            return false;
        }

        PTR paramID;
        rc = SQLParamData(statement, &paramID);
        if (rc != SQL_NEED_DATA) {
            return true;
        }

        if (tableData == nullptr || tableData->size() == 0) {
            return true;
        }
        vector<IData *> oneRow = tableData->at(rowPos);
        for (size_t i = 0; i < oneRow.size(); i++) {
            if (oneRow.at(i)->GetType() == IData::DataType::NUMERIC) {
                double value = ((NumericData *)(oneRow.at(i)))->GetData();
                ((NumericBinder *)table->Get()->at(i))->SetValue(value);
            }
            else {
                wchar_t * value = ((StringData *)(oneRow.at(i)))->GetData();
                ((StringBinder *)table->Get()->at(i))->SetValue(value);
            }
        }
        rc = SQLPutData(statement, (SQLPOINTER)1, 1);
        if (rc != SQL_SUCCESS && rc != SQL_NEED_DATA && rc != SQL_SUCCESS_WITH_INFO) {
            HandleDiagnosticRecord(statement, SQL_HANDLE_STMT, rc);
            wcharStr.SysErrWinAPI();
            return false;
        }
        if ((size_t)rowPos >= tableData->size() - 1) {
            allDataProcessed = true;
        }
        rowPos++;
    } while (!allDataProcessed);

    return allDataProcessed;
}

void SQLServerAccessor::HandleDiagnosticRecord(SQLHANDLE handle, SQLSMALLINT handleType, SQLRETURN retCode)
{
    if (retCode == SQL_INVALID_HANDLE) {
        locale::global(locale("C"));
        cout << "Invalid handle" << "\n";
        locale::global(locale(""));
        return;
    }
    SQLSMALLINT iRec = 0;
    WCHAR szSQLState[SQL_SQLSTATE_SIZE + 1];
    SQLINTEGER error;
    WCHAR szErrorMessage[1000];
    while (SQLGetDiagRec(
        handleType,
        handle,
        ++iRec,
        szSQLState,
        &error,
        szErrorMessage,
        (SQLSMALLINT)(sizeof(szErrorMessage) / sizeof(WCHAR)),
        (SQLSMALLINT *)NULL) == SQL_SUCCESS
        ) {
        WCharString msgCnv;
        locale::global(locale("C"));
        cout << msgCnv.Value("Status = ").Append(szSQLState).Append(" ").Append("\n").Append("Message = ").Append(szErrorMessage).ToString() << "\n" << "\n";
        locale::global(locale(""));
    }
}

void SQLServerAccessor::Prepare()
{
    prepared = false;

    rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
    if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO) {
        HandleDiagnosticRecord(env, SQL_HANDLE_ENV, rc);
        wcharStr.SysErrWinAPI();
    }
    else {
        rc = SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
        if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO) {
            HandleDiagnosticRecord(env, SQL_HANDLE_ENV, rc);
            wcharStr.SysErrWinAPI();
        }
        else {
            rc = SQLAllocHandle(SQL_HANDLE_DBC, env, &connection);
            if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO) {
                HandleDiagnosticRecord(connection, SQL_HANDLE_DBC, rc);
                wcharStr.SysErrWinAPI();
            }
        }
    }

    prepared = true;
}

bool SQLServerAccessor::GetPrepared()
{
    return prepared;
}

void SQLServerAccessor::Connect(string arg)
{
    connected = false;

    unique_ptr<wchar_t> cs = move(wcharStr.Value(arg).ToWChar());
    rc = SQLDriverConnect(
        connection,
        NULL,
        cs.get(),
        SQL_NTS,
        connectionString,
        1024,
        &bufSize,
        SQL_DRIVER_NOPROMPT);

    if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
        connected = true;
        transactionBegun = false;
    }
    else {
        HandleDiagnosticRecord(connection, SQL_HANDLE_DBC, rc);
        wcharStr.SysErrWinAPI();
    }
}

bool SQLServerAccessor::GetConnected()
{
    return connected;
}

void SQLServerAccessor::BeginTransaction()
{
    transactionBegun = false;

    rc = SQLSetConnectAttr(
        connection,
        SQL_ATTR_AUTOCOMMIT,
        (SQLPOINTER)SQL_AUTOCOMMIT_OFF,
        SQL_NTS);
    if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
        transactionBegun = true;
    }
    else {
        HandleDiagnosticRecord(connection, SQL_HANDLE_DBC, rc);
        wcharStr.SysErrWinAPI();
    }
}

bool SQLServerAccessor::GetTransactionBegun()
{
    return transactionBegun;
}

void SQLServerAccessor::CommitTransaction()
{
    rc = SQLEndTran(
        SQL_HANDLE_ENV,
        (SQLHANDLE)env,
        SQL_COMMIT);
    if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
        transactionBegun = false;
    }
    else {
        HandleDiagnosticRecord(connection, SQL_HANDLE_DBC, rc);
        wcharStr.SysErrWinAPI();
    }
}

void SQLServerAccessor::RollbackTransaction()
{
    rc = SQLEndTran(
        SQL_HANDLE_ENV,
        (SQLHANDLE)env,
        SQL_ROLLBACK);
    if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
        transactionBegun = false;
    }
    else {
        HandleDiagnosticRecord(connection, SQL_HANDLE_DBC, rc);
        wcharStr.SysErrWinAPI();
    }
}

void SQLServerAccessor::SQLStatementPrepare()
{
    statementPrepared = false;

    // In ODBC 3.x, the ODBC 2.x function SQLAllocStmt has been replaced by SQLAllocHandle.
    // https://msdn.microsoft.com/ja-jp/library/ms709370(v=vs.85).aspx
    // ftp://public.dhe.ibm.com/software/data/db2/everyplace/infocenters/jpn/dbeapr1006.htm
    // https://mariadb.com/kb/en/sql-99/sqlallocstmt/
    //
    // 記事執筆時点で非推奨になってるメソッドを使うなんて恥ずかしくないの？✝悔い改めて✝
    // http://sato-si.at.webry.info/200505/article_1.html
    // SQLAllocStmt(env, &statement);

    rc = SQLAllocHandle(SQL_HANDLE_STMT, connection, &statement);

    if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
        statementPrepared = true;
    }
    else {
        HandleDiagnosticRecord(connection, SQL_HANDLE_STMT, rc);
        wcharStr.SysErrWinAPI();
    }
}

bool SQLServerAccessor::GetSQLStatementPrepared()
{
    return statementPrepared;
}

void SQLServerAccessor::SQLSelect(string arg)
{
    selQuerySuccess = false;

    unique_ptr<wchar_t> query = move(wcharStr.Value(arg).ToWChar());
    rc = SQLExecDirect(statement, query.get(), SQL_NTS);

    if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
        if (DescribeTable()) {
            selQuerySuccess = true;
        }
        else {
            HandleDiagnosticRecord(statement, SQL_HANDLE_STMT, rc);
            errorMessage = table->GetErrorMessage();
        }
    }
    else {
        HandleDiagnosticRecord(statement, SQL_HANDLE_STMT, rc);
        wcharStr.SysErrWinAPI();
    }
}

bool SQLServerAccessor::GetSelQuerySuccess()
{
    return selQuerySuccess;
}

void SQLServerAccessor::Fetch()
{
    fetchCompleted = false;

    do {
        rc = SQLFetch(statement);

        if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
            for (size_t i = 0; i < table->Get()->size(); i++) {
                locale::global(locale("C"));
                if (table->Get()->at(i)->GetType() == IBinder::ColumnType::NUMERIC) {
                    double outValue = (double)((NumericBinder *)table->Get()->at(i))->GetValue();
                    cout << outValue;
                }
                else {
                    string outValue = wcharStr.Value(((StringBinder *)table->Get()->at(i))->GetValue()).ToString();
                    cout << outValue;
                }
                if (i < table->Get()->size() - 1) {
                    cout << ", ";
                }
                locale::global(locale(""));
            }
        }
        else if (rc == SQL_NO_DATA_FOUND) {
            fetchCompleted = true;
        }
        else {
            HandleDiagnosticRecord(statement, SQL_HANDLE_STMT, rc);
            return;
        }
        cout << "\n";
    } while (!fetchCompleted);
}

bool SQLServerAccessor::GetFetchCompleted()
{
    return fetchCompleted;
}

void SQLServerAccessor::AddParamBindPos(IBinder::ColumnType type, SQLLEN size, SQLSMALLINT scale)
{
    paramBindPosAdded = false;

    IBinder * addBinder;
    if (type == IBinder::ColumnType::NUMERIC) {
        addBinder = new NumericBinder();
        addBinder->SetIndex(bindPos);
        addBinder->SetSize(size);
        addBinder->SetScale(scale);
    }
    else {
        addBinder = new StringBinder();
        addBinder->SetIndex(bindPos);
        addBinder->SetSize(size);
        addBinder->SetScale(scale);
    }
    table->AddBinder(addBinder);
    bindPos++;

    paramBindPosAdded = true;
}

bool SQLServerAccessor::GetParamBindPosAdded()
{
    return paramBindPosAdded;
}

void SQLServerAccessor::SetInsertData(vector<vector<IData *>> * arg)
{
    tableData = arg;
}

void SQLServerAccessor::SQLInsert(string arg)
{
    if (table->BindParams(statement)) {
        ExecDML(arg);
    }
    else {
        HandleDiagnosticRecord(statement, SQL_HANDLE_STMT, rc);
        wcharStr.SysErrWinAPI();
    }
}

bool SQLServerAccessor::GetInsQuerySuccess()
{
    return insQuerySuccess;
}

string SQLServerAccessor::GetErrorMessage()
{
    return errorMessage;
}

SQLServerAccessor::SQLServerAccessor()
{
    table = new TableBinder();
    bindPos = 0;
    disposed = false;
}

void SQLServerAccessor::Dispose()
{
    delete table;
    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    SQLFreeHandle(SQL_HANDLE_DBC, connection);
    SQLFreeHandle(SQL_HANDLE_ENV, env);
    disposed = true;
}

SQLServerAccessor::~SQLServerAccessor()
{
    if (!disposed) {
        Dispose();
    }
}