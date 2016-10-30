#pragma once

#include "stdafx.h" // ✝

class WCharString
{
private:

    string value;

    void Assign(char * arg);

    void Assign(wchar_t * arg);

    void Assign(string arg);

    void Assign(string * arg);

    void Assign(const char * arg);

    void Assign(const wchar_t * arg);

    void Assign(const string * arg);

public:

    WCharString Append(char * arg);

    WCharString Append(wchar_t * arg);

    WCharString Append(string arg);

    WCharString Append(string * arg);

    WCharString Append(const char * arg);

    WCharString Append(const wchar_t * arg);

    WCharString Append(const string * arg);

    WCharString Value(char * arg);

    WCharString Value(wchar_t * arg);

    WCharString Value(string arg);

    WCharString Value(string * arg);

    WCharString Value(const char * arg);

    WCharString Value(const wchar_t * arg);

    WCharString Value(const string * arg);

    unique_ptr<wchar_t> ToWChar();

    string ToString();

    bool WChar_tStartsWith(wchar_t * arg1eval, string arg2test);

    void SysErrWinAPI();

    WCharString();

    ~WCharString();
};

class IBinder
{
public:

    enum ColumnType
    {
        STRING, NUMERIC,
    };

    virtual ColumnType GetType() = 0;

    virtual void SetIndex(int arg) = 0;

    virtual void SetType(SQLSMALLINT arg) = 0;

    virtual void SetSize(SQLLEN arg) = 0;

    virtual void SetScale(SQLSMALLINT arg) = 0;

    virtual void Bind(SQLHANDLE statement) = 0;

    virtual void BindParam(SQLHANDLE statement) = 0;

    virtual SQLRETURN GetReturnCode() = 0;

    virtual string GetErrorMessage() = 0;

    virtual bool GetBindSuccess() = 0;
};

class NumericBinder : public IBinder
{
private:

    enum ValueScale
    {
        SHORT, INT, FLOAT, DOUBLE,
    };

    ValueScale thisScale;

    SQLSMALLINT rawColumnType;

    int index;

    SQLLEN size;

    SQLSMALLINT scale;

    short svalue;

    int ivalue;

    float fvalue;

    double dvalue;

    SQLLEN nullIndicator;

    SQLRETURN returnCode;

    string errorMessage;

    bool bindSuccess;

public:

    void SetType(SQLSMALLINT arg);

    IBinder::ColumnType GetType();

    void SetIndex(int arg);

    int GetIndex();

    void SetSize(SQLLEN arg);

    SQLLEN GetSize();

    void SetScale(SQLSMALLINT arg);

    SQLSMALLINT GetScale();

    void SetValue(double arg);

    double GetValue();

    void Bind(SQLHANDLE statement);

    void BindParam(SQLHANDLE statement);

    SQLRETURN GetReturnCode();

    string GetErrorMessage();

    bool GetBindSuccess();

    NumericBinder();

    ~NumericBinder();
};

class StringBinder : public IBinder
{
private:

    SQLSMALLINT rawColumnType;

    int index;

    SQLLEN size;

    SQLSMALLINT scale;

    wchar_t * value;

    SQLLEN nullIndicator;

    SQLRETURN returnCode;

    string errorMessage;

    bool bindSuccess;

public:

    void SetType(SQLSMALLINT arg);

    IBinder::ColumnType GetType();

    void SetIndex(int arg);

    int GetIndex();

    void SetSize(SQLLEN arg);

    SQLLEN GetSize();

    void SetScale(SQLSMALLINT arg);

    SQLSMALLINT GetScale();

    void SetValue(wchar_t * arg);

    wchar_t * GetValue();

    void Bind(SQLHANDLE statement);

    void BindParam(SQLHANDLE statement);

    SQLRETURN GetReturnCode();

    string GetErrorMessage();

    bool GetBindSuccess();

    StringBinder();

    ~StringBinder();
};

class TableBinder
{
private:

    vector<IBinder *> * columnVector;

    SQLRETURN returnCode;

    string errorMessage;

public:

    void AddBinder(IBinder * arg);

    bool Bind(SQLHANDLE statement);

    bool BindParams(SQLHANDLE statement);

    SQLRETURN GetReturnCode();

    string GetErrorMessage();

    vector<IBinder *> * Get();

    TableBinder();

    ~TableBinder();
};

class IData
{
public:

    enum DataType
    {
        STRING, NUMERIC,
    };

    virtual DataType GetType() = 0;
};

class NumericData : public IData
{
private:

    enum ValueScale
    {
        SHORT, INT, FLOAT, DOUBLE,
    };

    ValueScale thisScale;

    short svalue;

    int ivalue;

    float fvalue;

    double dvalue;

public:

    IData::DataType GetType();

    void SetData(short arg);

    void SetData(int arg);

    void SetData(float arg);

    void SetData(double arg);

    double GetData();

    NumericData();

    ~NumericData();
};

class StringData : public IData
{
private:

    wchar_t * value;

public:

    IData::DataType GetType();

    void SetData(wchar_t * arg);

    wchar_t * GetData();

    StringData();

    ~StringData();
};

class SQLServerAccessor
{
private:

    SQLHANDLE env;

    SQLHANDLE connection;

    SQLWCHAR connectionString[1024];

    SQLSMALLINT bufSize;

    SQLHANDLE statement;

    SQLRETURN rc;

    TableBinder * table;

    vector<vector<IData *>> * tableData;

    WCharString wcharStr;

    string errorMessage;

    bool prepared;

    bool connected;

    bool transactionBegun;

    bool statementPrepared;

    bool selQuerySuccess;

    bool fetchCompleted;

    int bindPos;

    bool paramBindPosAdded;

    bool insQuerySuccess;

    bool disposed;

    bool DescribeTable();

    bool ExecDML(string arg);

public:

    void Prepare();

    bool GetPrepared();

    void Connect(string arg);

    bool GetConnected();

    void BeginTransaction();

    bool GetTransactionBegun();

    void CommitTransaction();

    void RollbackTransaction();

    void SQLStatementPrepare();

    bool GetSQLStatementPrepared();

    void SQLSelect(string arg);

    bool GetSelQuerySuccess();

    void Fetch();

    void AddParamBindPos(IBinder::ColumnType type, SQLLEN size, SQLSMALLINT scale);

    bool GetParamBindPosAdded();

    void SetInsertData(vector<vector<IData *>> * arg);

    void SQLInsert(string arg);

    bool GetInsQuerySuccess();

    bool GetFetchCompleted();

    string GetErrorMessage();

    void HandleDiagnosticRecord(SQLHANDLE handle, SQLSMALLINT handleType, SQLRETURN retCode);

    SQLServerAccessor();

    void Dispose();

    ~SQLServerAccessor();
};
