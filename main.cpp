#include <iostream>

#include "Table/Table.h"
#include "TableException.h"

using namespace std;

int main() {
    string table_name;

    cout << "Table Name:";
    while (cin >> table_name && table_name != "exit") {
        cout << "Command:";
        string command;
        cin >> command;

        try {
            if (command == "create")
                Table::create_table(table_name, cin);
            else if (command == "drop")
                Table::delete_table(table_name);
        } catch (TableCreateException& e) {
            cout << e.what() << endl;
        }

        try {
            Table table(table_name);

            vector<string> column_names = table.get_column_names();
            vector<string> column_types = table.get_column_types();
            vector<int> column_sizes = table.get_column_sizes();

            if (command == "select")        table.select_rows(cout, cin);
            else if (command == "insert")   table.insert_row(cin);
            else if (command == "update")   table.update_rows(cin);
            else if (command == "delete")   table.delete_rows(cin);
            else if (command != "create" && command != "drop")   cout << "Unrecognized command\n";
        } catch (TableException& e) {
            cout << e.what() << endl;
        } catch (runtime_error& e) {
            // eroare cauzata de lipsa unor feature-uri
            cout << e.what() << endl;
        }

        cout << "Table Name:";
    }
}
