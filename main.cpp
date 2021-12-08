#include <iostream>

#include "Table/Table.h"

using namespace std;

int main() {
    string table_name;

    while (cin >> table_name && table_name != "exit") {
        string command;
        cin >> command;

        if (command == "create") {
            create_table(table_name, cin);
            continue;
        }

        // TODO: verific daca tabelul exista.
        // todo: as putea face coversia de la Table la bool - true doar daca exista tabelul
        Table table(table_name);

        vector<string> column_names = table.get_column_names();
        vector<string> column_types = table.get_column_types();
        vector<int> column_sizes = table.get_column_sizes();
        cout << table.get_column_count() << endl;
        for (int i = 0; i < table.get_column_count(); i++) {
            cout << column_names[i] << "\t" << column_types[i] << "\t" << column_sizes[i] << endl;
        }

        if (command == "select")        table.select_rows(cout, cin);
        else if (command == "insert")   table.insert_row(cin);
        else if (command == "update")   table.update_rows(cin);
        else if (command == "delete")   table.delete_rows(cin);
        else                            cout << "Unrecognized command\n";
    }
}
