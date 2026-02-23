# Project Name: Key–Value Store Server

&nbsp;

##### Project Description



This is a database that stores data as key-value pairs. The value to be stored can be in any format, e.g., words, letters, numbers, and symbols. It maintains data durability through an append-only log, ensuring that the PUT(write operations) are recorded in permanent storage and can be recovered in case of a system restart.





###### The core operations supported by the system:



PUT: insert a key value pair

GET: retrieve a value by the key

DELETE: Erase the key from the store 



###### Functional Requirement

The server provides the following functionality:



**Supported Commands**

1. **PUT**

It is used to insert a new key–value pair or update the value of an existing key.

When a client sends PUT <key> <value>, the server first records the operation in the append-only log for persistence, then stores or updates the key in memory, and finally responds with OK to confirm success.



2\. **GET** 

It retrieves the value associated with a given key. When a client sends GET <key>, the server searches the in-memory store. If the key exists, the corresponding value is returned. If the key does not exist, the server responds with NOT\_FOUND.



3\. **DELETE** 

Removes a key and its associated value from the store. When DELETE <key> is received, the server logs the operation, removes the key from memory if it exists, and returns OK. If the key does not exist, the response is NOT\_FOUND.



Together, these three commands provide full basic data management functionality: create/update, read, and delete operations.

The server is robust against invalid client input and does not terminate unexpectedly.



##### System Architecture

The system is structured into independent modules:





**1. Command Processor**

Parses incoming commands

* Validates syntax
* Dispatch operations to the store
* Ensures persistence before modification



**2. Key–Value Store**

* Implemented using `std::unordered\_map`
* Provides constant-time average lookup
* Thread-safe access using mutex protection



**3. Append-Only Log**

* Records every `PUT` and `DELETE` operation
* Guarantees durability
* Used for rebuilding the state during restart





4**. Persistence Mechanism**

For every write operation:

* Command is appended to store.log
* Log is flushed to disk
* In-memory store is updated
* The response is returned to the client



This guarantees that no committed write is lost.



**5. Recovery Process**

On server startup:

* Open store.log
* Read each logged command sequentially
* Reapply operations to the in-memory store
* Resume normal operation



**6. Build and Compilation**

&nbsp;Requirements

* C++17 or later
* g++ compiler



#### Conclusion



This project successfully implements a persistent multi-client Key–Value Store Server in C++. The system ensures durability through append-only logging and full recovery on restart. The modular architecture and concurrency handling reflect practical system design principles used in real-world distributed systems.



