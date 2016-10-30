// SQLServerODBCAccessorWin32.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include "SQLServerAccessor.h"

string GetConnectionString(string DBName, string userID, string password)
{
    WCharString cStr;
    cStr = cStr.Value("DRIVER={SQL Server Native Client 11.0}; ").Append("DSN=localhost; ").Append("SERVER=").Append(DBName).Append("; ");
    cStr = cStr.Append("UID=").Append(userID).Append("; ").Append("PWD=").Append(password).Append("; ");
    return cStr.ToString();
}

string GetQuery()
{
    string query;
    bool queryEnd = false;

    do {
        string subquery;
        cin >> subquery;

        if (subquery.substr(subquery.length() - 1, 1)[0] == char(';')) {
            subquery = subquery.substr(0, subquery.length() - 1);
            queryEnd = true;
        }
        query.append(subquery);
        query.append(" ");
    } while (!queryEnd);

    return query;
}

void Test01()
{
    unique_ptr<SQLServerAccessor> accessor(new SQLServerAccessor());

    accessor->Prepare();
    if (!accessor->GetPrepared()) {
        string prompt;
        cin >> prompt;
        return;
    }

    cout << "接続先データベース名を指定してください。" << "\n";

    string DBName;
    cin >> DBName;

    cout << "接続ユーザー ID を入力してください。" << "\n";

    string userID;
    cin >> userID;

    cout << "パスワードを入力してください。" << "\n";

    string password;
    cin >> password;

    string connectionString;
    connectionString = GetConnectionString(DBName, userID, password);
    accessor->Connect(connectionString);
    if (!accessor->GetConnected()) {
        string prompt;
        cin >> prompt;
        return;
    }

    accessor->SQLStatementPrepare();
    if (!accessor->GetSQLStatementPrepared()) {
        string prompt;
        cin >> prompt;
        return;
    }

    cout << "実行する SQL を入力してください。" << "\n";

    string query;
    query = GetQuery();

    accessor->SQLSelect(query);
    if (!accessor->GetSelQuerySuccess()) {
        string prompt;
        cin >> prompt;
        return;
    }

    accessor->Fetch();
    if (!accessor->GetFetchCompleted()) {
        string prompt;
        cin >> prompt;
        return;
    }

    string prompt;
    cin >> prompt;
}

void Test02()
{
    unique_ptr<SQLServerAccessor> accessor(new SQLServerAccessor());

    accessor->Prepare();
    if (!accessor->GetPrepared()) {
        string prompt;
        cin >> prompt;
        return;
    }

    cout << "接続先データベース名を指定してください。" << "\n";

    string DBName;
    cin >> DBName;

    cout << "接続ユーザー ID を入力してください。" << "\n";

    string userID;
    cin >> userID;

    cout << "パスワードを入力してください。" << "\n";

    string password;
    cin >> password;

    string connectionString;
    connectionString = GetConnectionString(DBName, userID, password);
    accessor->Connect(connectionString);
    if (!accessor->GetConnected()) {
        string prompt;
        cin >> prompt;
        return;
    }

    accessor->SQLStatementPrepare();
    if (!accessor->GetSQLStatementPrepared()) {
        string prompt;
        cin >> prompt;
        return;
    }

    accessor->AddParamBindPos(IBinder::ColumnType::NUMERIC, 5, 0);
    accessor->AddParamBindPos(IBinder::ColumnType::NUMERIC, 2, 0);
    accessor->AddParamBindPos(IBinder::ColumnType::NUMERIC, 2, 0);
    accessor->AddParamBindPos(IBinder::ColumnType::NUMERIC, 2, 0);
    accessor->AddParamBindPos(IBinder::ColumnType::NUMERIC, 2, 0);
    accessor->AddParamBindPos(IBinder::ColumnType::NUMERIC, 2, 0);
    accessor->AddParamBindPos(IBinder::ColumnType::STRING, 8, 0);

    NumericData * c1 = new NumericData();
    c1->SetData(1234);
    NumericData * c2 = new NumericData();
    c2->SetData(3);
    NumericData * c3 = new NumericData();
    c3->SetData(3);
    NumericData * c4 = new NumericData();
    c4->SetData(4);
    NumericData * c5 = new NumericData();
    c5->SetData(3);
    NumericData * c6 = new NumericData();
    c6->SetData(3);
    StringData * c7 = new StringData();
    c7->SetData(L"4 TEST");
    vector<IData *> oneRow;
    oneRow.push_back(c1);
    oneRow.push_back(c2);
    oneRow.push_back(c3);
    oneRow.push_back(c4);
    oneRow.push_back(c5);
    oneRow.push_back(c6);
    oneRow.push_back(c7);
    vector<vector<IData *>> * tableData = new vector<vector<IData *>>();
    tableData->push_back(oneRow);
    accessor->SetInsertData(tableData);

    string query = "";

    accessor->SQLInsert(query);
    if (!accessor->GetInsQuerySuccess()) {
        string prompt;
        cin >> prompt;
        return;
    }

    string prompt;
    cin >> prompt;
}

int main()
{
    /*
    Test01();
    */
    Test02();

    return 0;
}