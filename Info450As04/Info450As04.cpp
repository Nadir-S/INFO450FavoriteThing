// Info450As04.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

const int READERROR = 100;
const int WRITEERROR = 200;
const int ARRAYSIZE = 100;

class Game
{
    string title;
    string developer;
    string genre;
    double price;
public:
    Game();
    Game(string ttl, string dev, string gen, double p);
    void CaptureGameItem();
    void ShowGameItem();
    int SaveGameItem(ofstream &outfile);    
};

class GameList
{
    Game **list;
    int numrecords;
    int listsize;
    int ReallocateArray();
public:
    GameList();
    ~GameList();
    void GetUserInput();
    void ShowGameList();
    int SaveGameList(string filename);
    int ReadGameList(string filename);
};

//default constructor
Game::Game()
{
    title = "";
    developer = "";
    genre = "";
    price = 0;
}

//overload constructor
Game::Game(string ttl, string dev, string gen, double p)
{
    title = ttl;
    developer = dev;
    genre = gen;
    price = p;
}

//capture item from user
void Game::CaptureGameItem()
{
    cout << "Game Title:" << endl;
    getline(cin, title);
    cout << "Game Developer:" << endl;
    getline(cin, developer);
    cout << "Game Genre:" << endl;
    getline(cin, genre);
    cout << "Game Price:" << endl;
    cin >> price;
    cin.ignore();
    cin.clear();
}

//display item to console
void Game::ShowGameItem()
{
    cout << "Title : " << title << "Developer : " << developer << "Genre : " << genre << "Price : " << price << endl;
}

//save game item to file
int Game::SaveGameItem(ofstream &outfile)
{
    if (outfile.is_open())
    {
        outfile << title << "|" << developer << "|" << genre << "|" << price << endl;
        return 0;
    }
    else
    {
        return WRITEERROR;
    }
}

//GameList constructor
GameList::GameList()
{
    list = new Game*[ARRAYSIZE];
    numrecords = 0;
    listsize = ARRAYSIZE;
}

GameList::~GameList()
{
    for (int i = 0; i < numrecords; i++)
    {
        delete list[i];
    }
    delete []list;
}

//Reallocate memory
int GameList::ReallocateArray()
{
    Game **temp;
    temp = new Game*[listsize + ARRAYSIZE];
    listsize = listsize + ARRAYSIZE;

    for (int i = 0; i < numrecords; i++)
    {
        temp[i] = list[i];
    }

    delete []list;
    list = temp;
    return 0;
}

//get user input
void GameList::GetUserInput()
{
    string answer = "Y";
    //check to see if there is space
    cout << "Enter game Y/N?" << endl;
    getline(cin, answer);
    while (answer == "Y" || answer == "y")
    {
        list[numrecords] = new Game();
        list[numrecords]->CaptureGameItem();
        numrecords++;
        cout << "Enter another item Y/N?" << endl;
        getline(cin, answer);
    }
}

//show list to the console
void GameList::ShowGameList()
{
    for (int i = 0; i < numrecords; i++)
    {
        list[i]->ShowGameItem();
    }
}

//save entire list to file
int GameList::SaveGameList(string filename)
{
    ofstream output(filename, ios::trunc);
    if (output)
    {
        for (int i = 0; i < numrecords; i++)
        {
            list[i]->SaveGameItem(output);
        }
        output.close();
    }
    else 
    {
        return WRITEERROR;
    }
    return 0;
}

//read inventory list from a file
int GameList::ReadGameList(string filename)
{
    string gtit, gdev, ggen, gp;
    ifstream infile(filename, ios::in);
    if (!infile)
    {
        cout << "File could not be opened for reading" << endl;
        return READERROR;
    }

    while (!infile.eof())
    {
        if (numrecords == listsize)
        {
            ReallocateArray();
        }
        getline(infile, gtit, '|');
        if (!gtit.empty())
        {
            getline(infile, gdev, '|');
            getline(infile, ggen, '|');
            getline(infile, gp);
            double price = stoi(gp);
            list[numrecords] = new Game(gtit, gdev, ggen, price);
            numrecords++;
        }
    }
    infile.close();
    return 0;
}

int main()
{
    string filename;
    cout << "Enter file path:" << endl;
    getline(cin, filename);

    GameList myGames;
    string answer;
    int error;
    error = myGames.ReadGameList(filename);

    if (error)
    {
        cout << "Cannot read inventory, create new list? Y/N" << endl;
        getline(cin, answer);
        if (answer != "Y" && answer != "y")
        {
            return 1;
        }
    }

    myGames.GetUserInput();
    myGames.SaveGameList(filename);
    myGames.ShowGameList();
    
    return 0;
}

