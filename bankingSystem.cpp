#include <iostream>
#include <stdio.h>
#include <fstream>
#include <filesystem>
#include <string>
#include <ctime>
#include <chrono>
#include <sstream>
#include <Windows.h>
#include <conio.h>
#include <vector>

using namespace std;

int main();

string attemptLogin()
{
    hash<string> hashString;
    string username, password, actualPassHash;
    cout << "ENTER YOUR USERNAME: ";
    cin >> username;
    cout << "ENTER YOUR PASSWORD: ";
    cin >> password;

    string attemptNameHash = to_string(hashString(username));
    string attemptPassHash = to_string(hashString(password));
    string directory = "data\\" + attemptNameHash;

    try
    {
        ifstream read(directory + "\\" + "password.txt");
        getline(read, actualPassHash);

        if (attemptPassHash == actualPassHash)
        {
            return directory;
        }
        else
        {
            return "NOLOGIN";
        }
    }
    catch (exception e)
    {
        return "NOLOGIN";
    }
    return "NOLOGIN";
}

string current_date()
{
    time_t now = time(NULL);
    struct tm tstruct;
    char buf[40];
    tstruct = *localtime(&now);
    //format: day DD-MM-YYYY
    strftime(buf, sizeof(buf), "%A %d/%m/%Y", &tstruct);
    return buf;
}

string current_time()
{
    time_t now = time(NULL);
    struct tm tstruct;
    char buf[40];
    tstruct = *localtime(&now);
    //format: HH:mm:ss
    strftime(buf, sizeof(buf), "%X", &tstruct);
    return buf;
}

string getDateTime()
{
    string date = current_date();
    string time = current_time();
    string both = date + " " + time;
    return both;
}

void createAccount()
{
    hash<string> hashString;
    string name1, name2, username, password;

    cout << "Enter your first name: ";
    cin >> name1;
    cout << "Enter your last name: ";
    cin >> name2;
    cout << "Select a username: ";
    cin >> username;
    cout << "Select a password: ";
    cin >> password;

    try
    {

        string nameHash = to_string(hashString(username));
        string passHash = to_string(hashString(password));

        string directory = "data\\" + nameHash;

        filesystem::create_directory(directory);

        ofstream passFile(directory + "\\" + "password.txt");
        passFile << passHash << endl;
        passFile.close();

        ofstream moneyFile(directory + "\\" + "money.txt");
        moneyFile << "0" << endl;
        moneyFile.close();

        ofstream transacFile(directory + "\\" + "transactions.txt");
        string datetime = getDateTime();
        transacFile << "Account Created --- " + datetime << endl;
        transacFile.close();

        ofstream nameFile(directory + "\\" + "name.txt");
        string fullname = name1 + " " + name2;
        nameFile << fullname << endl;
        nameFile.close();
    }
    catch (exception e)
    {
        cout << "ERROR WHEN CREATING NEW ACCOUNT\nPLEASE CHECK THAT CREDENTIALS ARE IN A VALID FORMAT AND TRY AGAIN" << endl;
        main();
    }
    cout << "ACCOUNT SUCCESSFULLY CREATED, PLEASE LOG-IN FROM THE MAIN MENU" << endl;
}

string getAccountBalance(string loginResult)
{
    string cash;
    string moneyDirectory = loginResult + "\\" + "money.txt";
    ifstream read(moneyDirectory);
    getline(read, cash);
    read.close();
    return cash;
}

void DorWamount(int type)
{
    string currentCash;
    string loginResult = attemptLogin(); //If login is successful, this contains the directory
    if (loginResult != "NOLOGIN")
    {
        cout << "LOGIN ATTEMPT SUCCESSFUL" << endl;
    }
    else
    {
        cout << "LOGIN ATTEMPT UNSUCCESSFUL: INVALID CREDENTIALS\nPRESS ANY KEY TO RETURN TO THE MAIN MENU" << endl;
        _getch();
        main();
    }

    string moneyDirectory = loginResult + "\\" + "money.txt";
    string transacDirectory = loginResult + "\\" + "transactions.txt";

    if (type == 1)
    {
        int addCash, total;
        currentCash = getAccountBalance(loginResult);
        cout << "ACCOUNT BALANCE: £" + currentCash << endl;
        bool cont = true;
        while (cont)
        {
            cout << "ENTER AMOUNT TO DEPOSIT: " << endl;
            cin >> addCash;
            if (addCash <= 0)
            {
                cout << "PLEASE ENTER A VALID (POSITIVE) AMOUNT" << endl;
                cont = true;
            }
            else
            {
                cont = false;
            }
        }
        filesystem::remove(moneyDirectory);
        stringstream ss;
        ss << currentCash;
        ss >> total;
        total += addCash;
        ofstream moneyFile(moneyDirectory);
        moneyFile << total << endl;
        moneyFile.close();
        ofstream transacFile(transacDirectory, ios_base::app);
        string datetime = getDateTime();
        transacFile << "-\nDeposit: £" + to_string(addCash) + " --- " + datetime << endl;
        transacFile.close();
        cout << "TRANSACTION SUCCESSFUL" << endl;
    }
    else
    {
        int subCash, total;
        currentCash = getAccountBalance(loginResult);
        cout << "ACCOUNT BALANCE: £" + currentCash << endl;
        bool cont = true;
        while (cont)
        {
            cout << "ENTER AMOUNT TO WITHDRAW: " << endl;
            cin >> subCash;
            if (subCash <= 0)
            {
                cout << "PLEASE ENTER A VALID (POSITIVE) AMOUNT" << endl;
                cont = true;
            }
            else
            {
                cont = false;
            }
        }
        filesystem::remove(moneyDirectory);
        stringstream ss;
        ss << currentCash;
        ss >> total;
        total -= subCash;
        ofstream moneyFile(moneyDirectory);
        moneyFile << total << endl;
        moneyFile.close();
        ofstream transacFile(transacDirectory, ios_base::app);
        string datetime = getDateTime();
        transacFile << "-\nWithdrawal: £" + to_string(subCash) + " --- " + datetime << endl;
        transacFile.close();
        cout << "TRANSACTION SUCCESSFUL" << endl;
    }
}

void getAllAccounts()
{
    string path = "data";
    vector<string> folders;
    vector<string> names;
    for (const auto &entry : filesystem::directory_iterator(path))
    {
        string stringVersion = entry.path().string();
        int length = stringVersion.length() - 5;
        string folderName = stringVersion.substr(5, length);
        folders.push_back(folderName);
    }

    for (int i = 0; i < folders.size(); i++)
    {
        string name;
        string directory = "data\\" + folders[i] + "\\" + "name.txt";
        ifstream read(directory);
        getline(read, name);
        read.close();
        names.push_back(name);
    }

    cout << "ACCOUNT NAMES:\n"
         << endl;

    for (int i = 0; i < names.size(); i++)
    {
        if (i == 0)
        {
            cout << names[i] << endl;
        }
        else if (i == names.size() - 1)
        {
            cout << "---" << endl;
            cout << names[i] << endl;
            cout << "\n"
                 << endl;
        }
        else
        {
            cout << "---" << endl;
            cout << names[i] << endl;
        }
    }

    cout << "PRESS ANY KEY TO RETURN TO MAIN MENU" << endl;
    _getch();
}

void closeAccount()
{
    string confirmation;
    string directory = attemptLogin();
    if (directory != "NOLOGIN")
    {
        cout << "\nTYPE 'CONFIRM' TO CONFIRM ACCOUNT REMOVAL\nTHIS CANNOT BE UNDONE" << endl;
        cin >> confirmation;
        transform(confirmation.begin(), confirmation.end(), confirmation.begin(), ::toupper);
        if (confirmation == "CONFIRM")
        {
            filesystem::remove_all(directory);
            cout << "ACCOUNT SUCCESSFULLY REMOVED" << endl;
            cout << "PRESS ANY KEY TO RETURN TO MAIN MENU" << endl;
            _getch();
        }
        else
        {
            cout << "ACCOUNT DELETION CONFIRMATION NOT PROVIDED" << endl;
            Sleep(2000);
        }
    }
    else
    {
        cout << "LOGIN FAILED" << endl;
        Sleep(2000);
    }
}

void modifyAccount()
{
    hash<string> hashString;
    string directory = attemptLogin();
    if (directory != "NOLOGIN")
    {
        string selection;
        bool cont = true;
        while (cont)
        {
            cout << "CHANGE NAME, USERNAME OR PASSWORD?" << endl;
            cin >> selection;
            transform(selection.begin(), selection.end(), selection.begin(), ::toupper);

            if (selection == "NAME")
            {
                cont = false;
                string name;
                string newFirst, newLast;
                string nameDirectory = directory + "\\" + "name.txt";
                ifstream read(nameDirectory);
                getline(read, name);
                read.close();

                cout << "CURRENT ACCOUNT NAME: " + name << endl;
                cout << "ENTER NEW FIRST NAME: " << endl;
                cin >> newFirst;
                cout << "ENTER NEW LAST NAME: " << endl;
                cin >> newLast;

                ofstream nameFile(directory + "\\" + "name.txt");
                string fullname = newFirst + " " + newLast;
                nameFile << fullname << endl;
                nameFile.close();

                cout << "ACCOUNT NAME SUCCESSFULLY UPDATED" << endl;
            }
            else if (selection == "USERNAME")
            {
                cont = false;
                string newUser;

                cout << "ENTER NEW USERNAME: " << endl;
                cin >> newUser;

                string oldUserFolder = directory;
                string newUserFolder = "data\\" + to_string(hashString(newUser));

                char oldName[1024];
                char newName[1024];

                strcpy(oldName, oldUserFolder.c_str());
                strcpy(newName, newUserFolder.c_str());

                int check = rename(oldName, newName);
                if (check == 0)
                {
                    cout << "ACCOUNT USERNAME SUCCESSFULLY UPDATED" << endl;
                    cout << "NEW USERNAME: " + newUser + "\nPRESS ANY KEY TO RETURN TO MAIN MENU" << endl;
                    _getch(); 
                }
                else
                {
                    cout << "USERNAME UPDATE FAILED. PLEASE TRY AGAIN LATER" << endl;
                }
            }
            else if (selection == "PASSWORD")
            {
                cont = false;
                string newPassword, confirmNewPass;
                bool passAsk = true;

                while (passAsk)
                {
                    cout << "ENTER NEW PASSWORD: " << endl;
                    cin >> newPassword;
                    cout << "ENTER NEW PASSWORD AGAIN TO CONFIRM: " << endl;
                    cin >> confirmNewPass;

                    if (newPassword == confirmNewPass)
                    {
                        passAsk = false;
                    }
                    else
                    {
                        passAsk = true;
                        cout << "PASSWORDS DO NOT MATCH, PLEASE TRY AGAIN" << endl;
                    }
                }

                string passHash = to_string(hashString(newPassword));

                ofstream passFile(directory + "\\" + "password.txt");
                passFile << passHash << endl;
                passFile.close();

                cout << "ACCOUNT PASSWORD SUCCESSFULLY UPDATED" << endl;
            }
            else
            {
                cont = true;
                cout << "INVALID OPTION, PLEASE TRY AGAIN" << endl;
                Sleep(2000);
            }
        }
    }
    else
    {
        cout << "LOGIN FAILED" << endl;
        Sleep(2000);
    }
}

int main()
{
    string loginResult, amount;
    int choice;

    cout << "01: NEW ACCOUNT\n02: DEPOSIT AMOUNT\n03: WITHDRAW AMOUNT\n04: BALANCE ENQUIRY\n05: ALL ACCOUNT HOLDER LIST\n06: CLOSE AN ACCOUNT\n07: MODIFY AN ACCOUNT\n08: EXIT\nSelect Your Option <1-8>: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
        createAccount();
        break;

    case 2:
        DorWamount(1);
        break;

    case 3:
        DorWamount(2);
        break;

    case 4:

        loginResult = attemptLogin(); //If login is successful, this contains the directory
        if (loginResult != "NOLOGIN")
        {
            cout << "LOGIN ATTEMPT SUCCESSFUL" << endl;
        }
        else
        {
            cout << "LOGIN ATTEMPT UNSUCCESSFUL: INVALID CREDENTIALS\nPRESS ANY KEY TO RETURN TO THE MAIN MENU" << endl;
            _getch();
            main();
        }
        amount = getAccountBalance(loginResult);
        cout << "ACCOUNT BALANCE: £" + amount << endl;
        cout << "PRESS ANY KEY TO RETURN TO MAIN MENU" << endl;
        _getch();
        break;

    case 5:
        getAllAccounts();
        break;

    case 6:
        closeAccount();
        break;

    case 7:
        modifyAccount();
        break;

    case 8:
        return 0;
        break;

    default:
        break;
    }

    cout << "RETURNING TO MAIN MENU..." << endl;
    Sleep(2000);
    main();
}