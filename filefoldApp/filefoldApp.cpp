// filefoldApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

//The of the classes is as per "Composite Design Pattern"
//Three parts of this design pattern
//1. Component eg: dataContainer
//2. Leaf eg: file
//3. Composite eg:folder

//Future plans for making the code more modern
//Use smart pointers instead of raw pointers

class folder;
class dataContainer
{
public:
    virtual ~dataContainer()//destructor must be virtual to ensure that the destructor of the derived class is called
    {

    }
    virtual bool isFile()
    {
        return false;
    }
    virtual bool isFolder()
    {
        return false;
    }
    dataContainer* getParent()
    {
        return m_parent;
    }
    dataContainer* getRoot()
    {
        dataContainer* root = nullptr;  
        root = this;
        while (root->getParent() != nullptr)
        {
            root = root->getParent();
        }
        m_rootCache = root;
        return root;
    }
    std::string getName()
    {
        return m_name;
    }
    virtual void printName()
    {
        using namespace std;
        cout << m_name << endl;
    }
    virtual folder* getFolder()
    {
        //This is implemented to avoid the typecasting from "dataContainer" class to "folder" class
        //refer composite pattern in Gang Of Four book.
        //alternately we can use dynamic_cast which is type safe
        return nullptr;
    }
protected:
    dataContainer(std::string &name, dataContainer* parent):m_name(name), m_parent(parent), m_rootCache(nullptr)
    {

    }
private:
    int getLevel()
    {
        getRoot();
    }
private:
    std::string m_name;
    dataContainer* m_parent;
    dataContainer* m_rootCache;
};

class file : public dataContainer
{
public:
    file(std::string& name, dataContainer* parent) :dataContainer(name, parent)
    {

    }
    ~file()
    {

    }
    bool isFile()
    {
        return true;
    }
    
};

class folder : public dataContainer
{
public:
    void addFile(std::string& name)
    {
        file* neoFile = new file(name, this);
        m_vecDataContainers.push_back(neoFile);
    }
    void addFolder(std::string& name)
    {
        folder* neoFolder = new folder(name, this);
        m_vecDataContainers.push_back(neoFolder);

    }
    void printList()
    {
        using namespace std;
        for (auto item : m_vecDataContainers)
        {
            item->printName();
            if (item->isFolder())
            {
                item->getFolder()->printList();
            }
        }
    }
    folder* getSubFolder(std::string name)
    {
        using namespace std;
        folder* subFolder = nullptr;
        for (auto item : m_vecDataContainers)
        {
            if (item->isFolder() && item->getName() == name)
            {
                subFolder = item->getFolder();
            }
        }

        return subFolder;
    }
    bool isFolder()
    {
        return true;
    }
    virtual folder* getFolder()
    {
        return this;
    }    
    folder(std::string& name, dataContainer* parent) :dataContainer(name, parent)
    {

    }
    ~folder()
    {
        for (auto data : m_vecDataContainers)
        {
            delete data;
        }
    }
private:
    std::vector<dataContainer*> m_vecDataContainers;
};

/// <summary>
///Command Pattern
/// Overloading the () operator instead of implementing an "Execute" fuction
/// </summary>
class command
{
public:
    void operator()()
    {

    }
protected:
    dataContainer* currentFolder;
};
class addFileCmd : public command
{
public:
    void operator ()(folder* current)
    {
        using namespace std;
        string name;
        cout << "Enter file name:";
        cin >> name;        
        current->addFile(name);
        
    }
};
class addFolderCmd : public command
{
public:
    void operator ()(folder* current)
    {
        using namespace std;
        string name;
        cout << "Enter folder name:";
        cin >> name;
        current->addFolder(name);
    }
};
class listCmd : public command
{
public:
    void operator ()(folder* current)
    {
        current->printList();
        system("pause");
    }
};
class changeFolderCmd : public command
{
public:
    void operator ()(folder* &current)
    {
        using namespace std;
        string name;
        cout << "Enter folder name:";
        cin >> name;
        folder *subFolder = current->getSubFolder(name);
        if (subFolder)
            current = subFolder;
    }
};
class upToCmd : public command
{
public:
    void operator ()(folder*& current)
    {
        folder* parentFolder = current->getParent()->getFolder();
		if (parentFolder)
			current = parentFolder;
    }
};
class treeCmd : public command
{
public:
    void operator ()(folder* current)
    {
        folder* root = current->getRoot()->getFolder();
        root->printList();
        system("pause");
    }
};
class exitCmd : public command
{
public:
    void operator ()()
    {
        system("exit");
    }
};

class commandMgr
{
public:
    commandMgr(folder* currentFolder) : m_current (currentFolder), m_option(0), m_keepGoing(true)
    {
       
    }

    void runLoop()
    {
        while (m_keepGoing)
        {
            getUserOption();
            execute();
        }
    }
    void runAddFile()
    {
        addfile(m_current);
    }
    void runAddFolder()
    {
        addfolder(m_current);
    }
    void runList()
    {
        list(m_current);
    }
    void runChangeFolder()
    {
        changefolder(m_current);
    }
    void runUpto()
    {
        upto(m_current);
    }
    void runTree()
    {
        tree(m_current);
    }
    void runExit()
    {
        exit();
    }

    void getUserOption()
    {
        using namespace std;
        int option = 0;
        system("CLS");
        cout << "---------------App--------------" << endl;
        cout << "1. Add File" << endl;
        cout << "2. Add Folder" << endl;
        cout << "3. List" << endl;
        cout << "4. Change Folder" << endl;
        cout << "5. Up to" << endl;
        cout << "6. Tree" << endl;
        cout << "7. Exit" << endl;
        cout << "Select a suitable option for the " << m_current->getName() << " folder :";
        cin >> m_option;
        
    }

    void execute()
    {
        switch (m_option)
        {
        case 1:
            runAddFile();
            break;
        case 2:
            runAddFolder();
            break;
        case 3:
            runList();
            break;
        case 4:
            runChangeFolder();
            break;
        case 5:
            runUpto();
            break;
        case 6:
            runTree();
            break;
        case 7:
        {
            m_keepGoing = false;
            runExit();
        }            
            break;
        default:
            break;
        }
    }

private:
    addFileCmd addfile;
    addFolderCmd addfolder;
    listCmd list;
    changeFolderCmd changefolder;
    upToCmd upto;
    treeCmd tree;
    exitCmd exit;

    folder* m_current;
    int m_option;
    bool m_keepGoing;
};

int runApp()
{
    std::string rootFolderName = "root";
    folder* root = new folder(rootFolderName,nullptr);

    folder* current = nullptr;
    current = root;    

    //command interface loop
    commandMgr cmdMgr(current);
    cmdMgr.runLoop();

    return 0;
}

int main()
{
    runApp();
    std::cout << "Bye bye!!!\n";
}

