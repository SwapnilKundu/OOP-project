#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// Room class
class Room {
private:
    int room_number;
    string room_type;
    int room_rate;
    string room_status;

public:
    Room(int number, string type, int rate, string status) {
        room_number = number;
        room_type = type;
        room_rate = rate;
        room_status = status;
    }

    int get_room_number() {
        return room_number;
    }

    string get_room_type() {
        return room_type;
    }

    int get_room_rate() {
        return room_rate;
    }

    string get_room_status() {
        return room_status;
    }

    void set_room_status(string status) {
        room_status = status;
    }
//polymorphism is used with virtual function
    virtual void display() {
        cout << "Room Number: " << room_number << endl;
        cout << "Room Type: " << room_type << endl;
        cout << "Room Rate: " << room_rate << endl;
        cout << "Room Status: " << room_status << endl;
    }
};

// Standard room class
class StandardRoom : public Room {
public:
    StandardRoom(int number, int rate, string status) : Room(number, "Standard", rate, status) {}

    void display() {
        cout << "Standard Room" << endl;
        Room::display();
    }
};

// Deluxe room class
class DeluxeRoom : public Room {
public:
    DeluxeRoom(int number, int rate, string status) : Room(number, "Deluxe", rate, status) {}

    void display() {
        cout << "Deluxe Room" << endl;
        Room::display();
    }
};

// FileManager class "static functions:write_data and read_data"
class FileManager {
public:
    static void write_data(string file_name, string data) {
        ofstream file(file_name); // Using constructor to open the file
        file << data;
        file.close();
    }

    static string read_data(string file_name) {
        ifstream file(file_name); // Using constructor to open the file
        stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        return buffer.str();
    }
};

// Hotel class
class Hotel {
public:
        string name;
    string location;
    int rating;
    vector<Room*> rooms;

public:
    Hotel(string name, string location, int rating) {
        this->name = name;
        this->location = location;
        this->rating = rating;
    }

    void add_room(Room* room) {
        rooms.push_back(room);
    }

    void display_rooms() {
        cout << "Rooms in " << name << endl;
        for (Room* room : rooms) {
            room->display();
            cout << endl;
        }
    }

    string to_string() {
        string data = name + "," + location + "," + std::to_string(rating) + "\n";
        for (Room* room : rooms) {
            data += std::to_string(room->get_room_number()) + ",";
            data += room->get_room_type() + ",";
            data += std::to_string(room->get_room_rate()) + ",";
            data += room->get_room_status() + "\n";
        }
        return data;
    }

    string get_name() {
        return name;
    }

    int get_rating() {
        return rating;
    }

    bool operator < (const Hotel& hotel) const {
        return rating < hotel.rating;
    }

    friend ostream& operator << (ostream& out, const Hotel& hotel);
};

ostream& operator << (ostream& out, const Hotel& hotel) {
    out << hotel.name << " (" << hotel.location << ")" << endl;
    out << "Rating: " << hotel.rating << endl;
    return out;
}

// HotelManagementSystem class
class HotelManagementSystem {
private:
    vector<Hotel*> hotels; // Added pointer to Hotel objects
    string file_name = "hotels.txt";

public:
    HotelManagementSystem() {
        string data = FileManager::read_data(file_name);
        if (!data.empty()) {
            vector<string> hotel_data = split_string(data, "\n");
            for (string hotel : hotel_data) {
                vector<string> fields = split_string(hotel, ",");
                string name = fields[0];
                string location = fields[1];
                int rating = stoi(fields[2]);
                Hotel* new_hotel = new Hotel(name, location, rating); // Creating new Hotel pointer
                for (int i = 3; i < fields.size(); i += 4) {
                    int room_number = stoi(fields[i]);
                    string room_type = fields[i + 1];
                    int room_rate = stoi(fields[i + 2]);
                    string room_status = fields[i + 3];
                    Room* new_room;
                    if (room_type == "Standard") {
                        new_room = new StandardRoom(room_number, room_rate, room_status); // Creating StandardRoom pointer
                    }
                    else if (room_type == "Deluxe") {
                        new_room = new DeluxeRoom(room_number, room_rate, room_status); // Creating DeluxeRoom pointer
                    }
                    new_hotel->add_room(new_room);
                }
                hotels.push_back(new_hotel); // Adding Hotel pointer to the vector
            }
        }
    }

    void add_hotel() {
        string name, location;
        int rating;
        cin.ignore();
        cout << "Enter Hotel Name: ";
        getline(cin, name);
        cout << "Enter Hotel Location: ";
        getline(cin, location);
        cout << "Enter Hotel Rating (1-5): ";
        cin >> rating;
        Hotel* new_hotel = new Hotel(name, location, rating);
        hotels.push_back(new_hotel);
        cout << "Hotel Added Successfully!" << endl;
    }

    void add_room(Hotel* hotel) {
        int room_number, room_rate;
        string room_type = "";
        while (room_type != "Standard" && room_type != "Deluxe") {
            cout << "Enter Room Type (Standard/Deluxe): ";
            cin >> room_type;
        }
        cout << "Enter Room Number: ";
        cin >> room_number;
        cout << "Enter Room Rate: ";
        cin >> room_rate;
        Room* new_room;
        if (room_type == "Standard") {
            new_room = new StandardRoom(room_number, room_rate, "Available");
        }
        else if (room_type == "Deluxe") {
            new_room = new DeluxeRoom(room_number, room_rate, "Available");
        }
        hotel->add_room(new_room);
        cout << "Room Added Successfully!" << endl;
    }

    void display_hotels() {
        cout << "Hotels in the System" << endl;
        for (Hotel* hotel : hotels) {
            cout << *hotel << endl;
        }
    }

    void display_rooms(Hotel* hotel) {
        hotel->display_rooms();
    }

    void book_room(Hotel* hotel) {
        int room_number;
        cout << "Enter Room Number to Book: ";
        cin >> room_number;
        for (Room* room : hotel->rooms) {
            if (room->get_room_number() == room_number && room->get_room_status() == "Available") {
                room->set_room_status("Booked");
                cout << "Room Booked Successfully!" << endl;
                return;
            }
        }
        cout << "Room is Either Not Available or Does Not Exist!" << endl;
    }

    void save_data() {
        string data = "";
        for (Hotel* hotel : hotels) {
            data += hotel->to_string();
        }
        FileManager::write_data(file_name, data);
        cout << "Data Saved Successfully!" << endl;
    }

    void search_hotels_by_rating() {
        int rating;
        cout << "Enter Rating: ";
        cin >> rating;
        vector<Hotel*> matched_hotels;
        for (Hotel* hotel : hotels) {
            if (hotel->get_rating() == rating) {
                matched_hotels.push_back(hotel);
            }
        }

        if (matched_hotels.empty()) {
            cout << "No Hotels Found!" << endl;
        }
        else {
            sort(matched_hotels.begin(), matched_hotels.end(), [](Hotel* a, Hotel* b) { // Added lambda function for custom sorting
                return *a < *b;
            });
            cout << "Hotels Matching Rating: " << rating << endl;
            for (Hotel* hotel : matched_hotels) {
                cout << *hotel << endl;
            }
        }
    }

    void manage_console() {
        while (true) {
            cout << "\nWelcome to Hotel Management System!" << endl;
            cout << "1. Add Hotel" << endl;
            cout << "2. Add Room to Hotel" << endl;
            cout << "3. Display All Hotels" << endl;
            cout << "4. Display Rooms of a Hotel" << endl;
            cout << "5. Book Room in a Hotel" << endl;
            cout << "6. Save Data" << endl;
            cout << "7. Search Hotels by Rating" << endl;
            cout << "8. Exit" << endl;
            int choice;
            cout << "Enter Choice: ";
            cin >> choice;
            switch (choice) {
            case 1: {
                add_hotel();
                break;
            }
            case 2: {
                if (hotels.empty()) {
                    cout << "No Hotels Found!" << endl;
                }
                else {
                    display_hotels();
                    int hotel_index;
                    cout << "Enter Hotel Index: ";
                    cin >> hotel_index;
                    if (hotel_index >= 0 && hotel_index < hotels.size()) {
                        add_room(hotels[hotel_index]);
                    }
                    else {
                        cout << "Invalid Hotel Index!" << endl;
                    }
                }
                break;
            }
            case 3: {
                if (hotels.empty()) {
                    cout << "No Hotels Found!" << endl;
                }
                else {
                    display_hotels();
                }
                break;
            }
            case 4: {
                if (hotels.empty()) {
                    cout << "No Hotels Found!" << endl;
                }
                else {
                    display_hotels();
                    int hotel_index;
                    cout << "Enter Hotel Index: ";
                    cin >> hotel_index;
                    if (hotel_index >= 0 && hotel_index < hotels.size()) {
                        display_rooms(hotels[hotel_index]);
                    }
                    else {
                        cout << "Invalid Hotel Index!" << endl;
                    }
                }
                break;
            }
            case 5: {
                if (hotels.empty()) {
                    cout << "No Hotels Found!" << endl;
                }
                else {
                    display_hotels();
                    int hotel_index;
                    cout << "Enter Hotel Index: ";
                    cin >> hotel_index;
                    if (hotel_index >= 0 && hotel_index < hotels.size()) {
                        book_room(hotels[hotel_index]);
                    }
                    else {
                        cout << "Invalid Hotel Index!" << endl;
                    }
                }
                break;
            }
            case 6: {
                save_data();
                break;
            }
            case 7: {
                search_hotels_by_rating();
                break;
            }
            case 8: {
                cout << "Thank you for using the Hotel Management System!" << endl;
                return;
            }
            default: {
                cout << "Invalid Choice!" << endl;
                break;
            }
            }
        }
    }

    vector<string> split_string(string str, string delimiter) {
        size_t pos = 0;
        string token;
        vector<string> tokens;
        while ((pos = str.find(delimiter)) != string::npos) {
            token = str.substr(0, pos);
            tokens.push_back(token);
            str.erase(0, pos + delimiter.length());
        }
        tokens.push_back(str);
        return tokens;
    }
};

int main() {
    HotelManagementSystem system;
    system.manage_console();
    return 0;
}
