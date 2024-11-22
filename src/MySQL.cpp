#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>

using namespace std;

int main() {
    try {
        // 初始化 MySQL 驱动
        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con;

        // 连接到 MySQL 数据库
        con = driver->connect("tcp://127.0.0.1:3306", "root", "123456");

        // 选择数据库
        con->setSchema("my_database");

        // 创建 SQL 语句对象
        sql::Statement *stmt = con->createStatement();

        // 创建一个表
        stmt->execute("DROP TABLE IF EXISTS Employee");
        stmt->execute("CREATE TABLE Employee ("
                      "id INT AUTO_INCREMENT PRIMARY KEY, "
                      "name VARCHAR(100), "
                      "age INT, "
                      "position VARCHAR(100))");

        cout << "Table 'Employee' created successfully!" << endl;

        // 插入数据
        stmt->execute("INSERT INTO Employee (name, age, position) VALUES "
                      "('Alice', 30, 'Engineer'), "
                      "('Bob', 25, 'Technician'), "
                      "('Charlie', 35, 'Manager')");

        cout << "Data inserted successfully!" << endl;

        // 查询表内容
        sql::ResultSet *res = stmt->executeQuery("SELECT * FROM Employee");

        // 打印表内容
        cout << "Employee Table:" << endl;
        while (res->next()) {
            cout << "ID: " << res->getInt("id")
                 << ", Name: " << res->getString("name")
                 << ", Age: " << res->getInt("age")
                 << ", Position: " << res->getString("position") << endl;
        }

        // 清理
        delete res;
        delete stmt;
        delete con;
    } catch (sql::SQLException &e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}

