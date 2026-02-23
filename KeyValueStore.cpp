#include <iostream>//input output operations  
#include <fstream>//for reading and writing files
#include <unordered_map>//hash table for fast key-value storage
#include <sstream>//to break the user input into parts
#include <string>// to use the strings

//   Command Representation
enum class CommandType { 
//Defining only the allowed type of operations.
    PUT,GET,DELETE,INVALID
};

struct Command {
    CommandType type = CommandType::INVALID;//default type if no command is pressed 
    std::string key;//to store the key
    std::string value;// to store the value
};

//converts a text line into a command that can be executed
Command breakInputCommand(const std::string& line) {
    std::istringstream iss(line);
 // creating a variable to store the input from the user
    std::string function;
    iss >> function;//reads the first letter as the command type.

if (function == "PUT") {
        Command c;
        c.type = CommandType::PUT;
        //read the next word after the command(PUT) as the key
        iss >> c.key;
        // reads the value after the key, from the space to the last letter.
        std::getline(iss, c.value);
        //delete the space after the key, if the conditions are met
        if (!c.value.empty() && c.value[0] == ' ')
            c.value.erase(0, 1);
        return c;
    }
if (function == "GET") {
        Command c;
        c.type = CommandType::GET;
        //read the key to retrieve the value if present
        iss >> c.key;
        return c;
    }
if (function == "DELETE") {
        Command c;
        c.type = CommandType::DELETE;
        //read the key to delete from the database
        iss >> c.key;
        return c;
    }
   //return the command error(invalid) is the commands are not PUT/GET/DELETE
    return {};
}
//The key value store
class KeyValueStore {
public:
  //insert the key and the value pair in the map
    void put(const std::string& key, const std::string& value) {
        store_[key] = value;
    }

bool get(const std::string& key, std::string& value) const {
        //searches for the key in the map
        auto it = store_.find(key);
        //return false if the key is not found ie reaches the end
        if (it == store_.end()) return false;
        //retrieve the value
        value = it->second;
        return true;
    }
    //function to delete a key-value pair and return true if it exsted.
bool erase(const std::string& key) {
        return store_.erase(key) > 0;
    }

private:
    //hash map used by the KeyValueStore
    std::unordered_map<std::string, std::string> store_;
};
   //Append-Only Log
   //function that make the database survive a program restart 
   // used to save operations permanently
class AppendOnlyLog {
public:
    //opens the file in append mode ie additions to the end
    explicit AppendOnlyLog(const std::string& filename)
        : file_(filename, std::ios::app) {}

    void append(const Command& function) {
        //PUT command
        if (function.type == CommandType::PUT) {
            file_ << "PUT " << function.key << " " << function.value << "\n";
        } else if
        //DELETE command 
        (function.type == CommandType::DELETE) {
            file_ << "DELETE " << function.key << "\n";
        }
        // ensure durability or data is written immediately to the database after editing
        file_.flush();  
    }

private:
    //output the file stream
    std::ofstream file_;
};
 // Recovery Logic(requires PUT and DELETE operations only)
 //opens the log file, reads line by line and re-run each command
void logRecovery(const std::string& filename, KeyValueStore& store) {
    std::ifstream in(filename);
    std::string line;

    while (std::getline(in, line)) {
        Command function = breakInputCommand(line);
//use the store.put if the command is PUT
        if (function.type == CommandType::PUT) {
            store.put(function.key, function.value);
        }
//use store.erase if the command is DELETE            
        else if (function.type == CommandType::DELETE) {
            store.erase(function.key);
        }
     }
      }
//function to receive a command and execute it
class CommandProcessor {
public:
    CommandProcessor(KeyValueStore& store, AppendOnlyLog& log)
        : store_(store), log_(log) {}
//function to decide what to do with the command
    std::string execute(const Command& function) {
//conditional statement to decide the operation to perform using the instructions.
switch (function.type) {
  case CommandType::PUT:
    //persist to save in the log to be replayed duiring recovery
    log_.append(function); 
    //apply the saved changes to the memory              
    store_.put(function.key, function.value); 
     return "--OK--";

  case CommandType::GET: {
    // creating a temporary storage to store hold the results
     std::string value;
     //to search the store and put result in value
    if (store_.get(function.key, value))
        return value;
        return "--KEY NOT FOUND--";
            }
    
    case CommandType::DELETE:
    //persistence to save changes.    
    log_.append(function);
    //To erase the Key
      if (store_.erase(function.key))
       return "--OK--";
       return "--kEY NOT FOUND--";
     default:
         return "--ERROR--";
        }
    }

private:
    //References stored inside the class and used by the CommandProcessor
    KeyValueStore& store_;
    AppendOnlyLog& log_;
};
 //The main function.
    
    int main() {
    //create a log file name
    const std::string log_file = "store.log";
    //creating a store
    KeyValueStore store;

    // Recovery to rebuild database from the previous runs(recover progress)
    logRecovery(log_file, store);
    
    //create  a log systems
    AppendOnlyLog log(log_file);
    CommandProcessor processor(store, log);

    std::cout << "KEY-VALUE STORE:(PUT / GET / DELETE) ONLY!!\n";
   //create a string to store the user input
    std::string line;
    while (std::getline(std::cin, line)) {
        Command function = breakInputCommand(line);
        std::cout << processor.execute(function) << std::endl;
    }

    return 0;
}
