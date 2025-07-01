#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <limits>
using namespace std;
void WriteFileLines(string path, vector<string> my_str)
{
    auto status = ios::in | ios::out | ios::trunc;
    fstream My_File(path.c_str(), status);
    for (auto &line : my_str)
        My_File << line << endl;
    My_File.close();
}
vector<string> SplitString(string line, string delimeter = ",")
{
    vector<string> SubStr;
    int pos = 0;
    while ((pos = (int)line.find(delimeter)) != -1)
    {
        string word = line.substr(0, pos);
        SubStr.push_back(word);
        line.erase(0, pos + 1);
    }
    SubStr.push_back(line);
    return SubStr;
}
vector<string> ReadFileLines(string path)
{
    vector<string> lines;
    fstream Read(path.c_str());
    string text;
    while (getline(Read, text))
        lines.push_back(text);
    Read.close();
    return lines;
}
int convert_to_int(string word)
{
    stringstream s(word);
    int number;
    s >> number;
    return number;
}
int Read_int(int low, int high)
{
    cout << "enter choice:\n";
    int num;
    cin >> num;
    if (num >= low && num <= high)
        return num;
    return Read_int(low, high);
}
int Show_Read_Menu(vector<string> Menu)
{
    cout << "choices: \n";
    for (int i = 0; i < Menu.size(); i++)
    {
        cout << i + 1 << "." << Menu[i] << endl;
    }
    return Read_int(1, Menu.size());
}
struct User
{
    int user_id;
    string user_name;
    string password;
    string name;
    string email;
    int annonymous;
    vector<int> questions_from_me;
    map<int, vector<int>> questionid_questionidthread_to_map;
    User()
    {
        user_id = annonymous = -1;
    }
    User(string line)
    {
        vector<string> substr = SplitString(line);
        user_id = convert_to_int(substr[0]);
        user_name = substr[1];
        password = substr[2];
        name = substr[3];
        email = substr[4];
        annonymous = convert_to_int(substr[5]);
    }
    string to_string()
    {
        stringstream s;
        s << user_id << "," << user_name << "," << password << "," << name << "," << email << "," << annonymous;
        return s.str();
    }
    void print_user()
    {
        cout << user_id << " " << user_name << " " << password << " " << name << " " << email << " " << annonymous << endl;
    }
};
struct UserManager
{
    map<string, User> username_to_userobject;
    User current_user;
    int last_id;
    UserManager()
    {
        last_id = 0;
    }
    void Load_database()
    {
        last_id = 0;
        vector<string> lines = ReadFileLines("my_users.txt");
        for (auto &line : lines)
        {
            User user(line);
            username_to_userobject[user.user_name] = user;
            last_id = max(last_id, user.user_id);
        }
    }
    void Do_Login()
    {
        cout << "enter username && password:";
        cin >> current_user.user_name >> current_user.password;
        if (!username_to_userobject.count(current_user.user_name))
        {
            cout << "Invalid User_name or password " << endl;
            Do_Login();
        }
        if (username_to_userobject[current_user.user_name].password != current_user.password)
        {
            cout << "Invalid User_name or password " << endl;
            Do_Login();
        }
    }
    void Do_SignUp()
    {
        cout << "enter user_name:" << endl;
        cin >> current_user.user_name;
        if (username_to_userobject.count(current_user.user_name))
        {
            cout << "user already exist " << endl;
            Do_SignUp();
        }
        cout << "enter password:" << endl;
        cin >> current_user.password;
        cout << "enter name:" << endl;
        cin >> current_user.name;
        cout << "enter email:" << endl;
        cin >> current_user.email;
        cout << "enter annonymous:" << endl;
        cin >> current_user.annonymous;
        current_user.user_id = ++last_id;
        username_to_userobject[current_user.user_name] = current_user;
    }
    void Update_Database()
    {
        vector<string> my_str;
        for (auto &pair : username_to_userobject)
        {
            my_str.push_back(pair.second.to_string());
        }
        WriteFileLines("my_users.txt", my_str);
    }
    int Access_System()
    {
        int choice = Show_Read_Menu({"Login", "SignUp", "Cancel"});
        if (choice == 1)
        {
            Do_Login();
            return 1;
        }
        else if (choice == 2)
        {
            Do_SignUp();
            return 2;
        }
        else
            return 3;
    }
    void list_Users()
    {
        for (auto &pair : username_to_userobject)
            pair.second.print_user();
    }
};
struct Questions
{
    int question_id;
    int have_parent_question;
    int from_user;
    int to_user;
    int annonymous;
    string my_question;
    string my_answer;
    Questions()
    {
        question_id = have_parent_question = from_user = to_user = annonymous = -1;
    }
    Questions(string line)
    {
        vector<string> substr = SplitString(line);
        question_id = convert_to_int(substr[0]);
        have_parent_question = convert_to_int(substr[1]);
        from_user = convert_to_int(substr[2]);
        to_user = convert_to_int(substr[3]);
        annonymous = convert_to_int(substr[4]);
        my_question = substr[5];
        my_answer = substr[6];
    }

    string to_string()
    {
        stringstream s;
        s << question_id << "," << have_parent_question << "," << from_user << "," << to_user
          << "," << annonymous << "," << my_question << "," << my_answer;
        return s.str();
    }
    void Question_from_user(User &user)
    {

        cout << "question_id(" << question_id << ")";
        if (user.annonymous == 0)
            cout << " !AQ ";
        cout << " to user_id:" << to_user
             << " question is:" << my_question << " && answer is:" << my_answer << endl;
    }
    void Question_to_user()
    {
        string Prefix = "";
        if (have_parent_question != -1)
            Prefix = "Thread:";
        cout << Prefix << " question_id(" << question_id << ")";
        if (annonymous == 0)
            cout << " from user_id:" << from_user;
        cout << "  question is:" << my_question << " && answer is:" << my_answer << endl;
    }
};
struct QuestionManager
{
    map<int, vector<int>> questionid_questionidThread_map;
    map<int, Questions> questionid_questionsObj_map;
    int last_id;
    QuestionManager()
    {
        last_id = 0;
    }
    void Load_database()
    {
        questionid_questionidThread_map.clear();
        last_id = 0;
        vector<string> lines = ReadFileLines("my_questions.txt");
        for (auto &line : lines)
        {
            Questions question(line);
            questionid_questionsObj_map[question.question_id] = question;
            last_id = max(last_id, question.question_id);
        }
    }
    void Match_questions_with_user(User &user)
    {
        for (auto &pair : questionid_questionsObj_map)
        {
            if (pair.second.from_user == user.user_id)
            {
                user.questions_from_me.push_back(pair.second.question_id);
            }
            else if (pair.second.to_user == user.user_id)
            {
                if (pair.second.have_parent_question == -1)
                {
                    user.questionid_questionidthread_to_map[pair.second.question_id].push_back(pair.second.question_id);
                    questionid_questionidThread_map[pair.second.question_id].push_back(pair.second.question_id);
                }
                else
                {
                    user.questionid_questionidthread_to_map[pair.second.have_parent_question].push_back(pair.second.question_id);
                    questionid_questionidThread_map[pair.second.have_parent_question].push_back(pair.second.question_id);
                }
            }
        }
    }
    void Questions_from_users(UserManager &user_manager)
    {
        User &my_user = user_manager.username_to_userobject[user_manager.current_user.user_name];
        if (my_user.questions_from_me.size() == 0)
        {
            cout << "no questions from me " << endl;
            return;
        }
        for (int i = 0; i < (int)my_user.questions_from_me.size(); i++)
        {
            Questions &question = questionid_questionsObj_map[my_user.questions_from_me[i]];
            question.Question_from_user(my_user);
        }
    }
    void Questions_to_users(UserManager &user_manager)
    {
        User &my_user = user_manager.username_to_userobject[user_manager.current_user.user_name];
        if (my_user.questionid_questionidthread_to_map.size() == 0)
        {
            cout << "no questions to others " << endl;
            return;
        }
        for (auto &pair : my_user.questionid_questionidthread_to_map)
        {
            for (auto question_ids : pair.second)
            {
                Questions &question = questionid_questionsObj_map[question_ids];
                question.Question_to_user();
            }
            cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl;
        }
    }
    void Ask_question(UserManager &user_manager)
    {

        User &my_user = user_manager.username_to_userobject[user_manager.current_user.user_name];
        cout << "do you want to ask new question or give it to parent question?: " << endl;
        int choice = 0;
        Questions my_quest;
        while (true)
        {
            cout << "for new  -1   OR   for parent  enter question" << endl;
            cin >> choice;
            
            if (choice == -1)
            {
                my_quest.have_parent_question = -1;
                break;
            }
            else
            {

                if (questionid_questionsObj_map.count(choice))
                {
                    my_quest.have_parent_question = choice;
                    break;
                }
                else
                    continue;
            }
        }
        my_quest.question_id = ++last_id;
        my_quest.from_user = my_user.user_id;
        cout << "enter user id you want to send your question " << endl;
        cin >> my_quest.to_user;
        cout << "enter question " << endl;
        cin.ignore();
        getline(cin, my_quest.my_question);
        my_quest.annonymous = 0;
        my_quest.my_answer = "";
        questionid_questionsObj_map[my_quest.question_id] = my_quest;
        Update_Database();
    }
    void Answer_question(UserManager &user_manager)
    {
        User &my_user = user_manager.username_to_userobject[user_manager.current_user.user_name];
        int my_question_id = 0;
        bool is_found = false;
        cout << "enter question you want to answer " << endl;
        cin >> my_question_id;
        for (auto &pair : my_user.questionid_questionidthread_to_map)
        {
            for (auto &quest_id : pair.second)
            {
                if (my_question_id == quest_id)
                {
                    Questions &question = questionid_questionsObj_map[quest_id];
                    if (question.my_answer != "")
                    {
                        cout << "answer already exist,Do yo want to Edit No(-1) yes(ANY) " << endl;
                        int choice = 0;
                        cin >> choice;
                        if (choice == -1)
                            return;
                    }
                    cin.ignore();
                    getline(cin, question.my_answer);
                    is_found = true;
                    Update_Database();
                    return;
                }
            }
        }
        if (!is_found)
            Answer_question(user_manager);
    }
    void Delete_question(UserManager &user_manager)
    {
        User &my_user = user_manager.username_to_userobject[user_manager.current_user.user_name];
        cout << "enter question you want to Delete " << endl;
        int my_quest_id = 0;

        cin >> my_quest_id;
        for (auto quest_id : my_user.questions_from_me)
        {
            if (my_quest_id == quest_id)
            {
                for (auto &pair : questionid_questionidThread_map)
                {
                    if (quest_id == pair.first)
                    {
                        for (auto questionId : pair.second)
                        {
                            Questions &question = questionid_questionsObj_map[questionId];
                            questionid_questionsObj_map.erase(questionId);
                        }
                        Update_Database();
                        return;
                    }
                    else
                    {
                        for (auto question_id : pair.second)
                        {
                            if (my_quest_id == question_id)
                            {
                                Questions &question = questionid_questionsObj_map[question_id];
                                questionid_questionsObj_map.erase(question_id);
                                Update_Database();
                                return;
                            }
                        }
                    }
                }
            }
        }
        cout << "question not Found in list for User id you entered " << endl;
    }
    void Update_Database()
    {
        vector<string> my_str;
        for (auto &pair : questionid_questionsObj_map)
        {
            my_str.push_back(pair.second.to_string());
        }
        WriteFileLines("my_questions.txt", my_str);
    }
    void Feed()
    {

        for (auto &pair : questionid_questionidThread_map)
        {
            for (auto quest_id : pair.second)
            {
                Questions &question = questionid_questionsObj_map[quest_id];
                question.Question_to_user();
            }
            cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< " << endl;
        }
    }
};
struct Askme
{
    UserManager um;
    QuestionManager qm;
    void Run()
    {
        Load_database();
        int choices;
        choices = um.Access_System();
        if (choices == 3)
            return;
        while (true)
        {
            Load_database();
            for (auto &pair : um.username_to_userobject)
            {
                qm.Match_questions_with_user(pair.second);
            }
            cout << "\n Menu :\n";
            cout << "1.print question To Me " << endl;
            cout << "2.print questions From me" << endl;
            cout << "3.Answer Question" << endl;
            cout << "4.Delete Question" << endl;
            cout << "5.Ask Question" << endl;
            cout << "6.List System Users" << endl;
            cout << "7.Feed" << endl;
            cout << "8.Logout" << endl;
            cout << "enter choice:" << endl;
            int choice;
            cin >> choice;
            if (choice == 1)
            {
                qm.Questions_to_users(um);
            }
            else if (choice == 2)
            {
                qm.Questions_from_users(um);
            }
            else if (choice == 3)
            {
                qm.Answer_question(um);
            }
            else if (choice == 4)
            {
                qm.Delete_question(um);
            }
            else if (choice == 5)
            {
                qm.Ask_question(um);
            }
            else if (choice == 6)
            {
                um.list_Users();
            }
            else if (choice == 7)
            {
                qm.Feed();
            }
            else if (choice == 8)
            {
                break;
            }
        }
        Run();
    }
    void Load_database()
    {
        um.Load_database();
        qm.Load_database();
    }
    void Update_database()
    {
        um.Update_Database();
        qm.Update_Database();
    }
};
int main()
{
    Askme ask1;
    ask1.Run();
    return 0;
}