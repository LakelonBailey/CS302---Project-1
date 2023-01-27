#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

/***********************************
********* HELPER FUNCTIONS *********
************************************/


// Return a vector of strings split on the provided separator
vector<string> split(string line, char on) {
    vector<string> strings;
    string item = "";
    for (size_t i = 0; i <= line.size(); i++) {
        if (i == line.size() || line[i] == on) {
            strings.push_back(item);
            item = "";
        }
        else {
            item += (char)line[i];
        }
    }
    return strings;
}

// Join a vector of strings on the provided separator
string join(vector<string> strings, char separator) {
    string result = "";
    for (size_t i = 0; i < strings.size(); i++) {
        if (i) result += separator;
        result += strings[i];
    }
    return result;
}

// Replace every instance of a char in a string with a different char
string replace(string item, char toReplace, char replaceWith) {
    return join(split(item, toReplace), replaceWith);
}

// Convert numeric string to integer
int intString(string num) {
    istringstream ss;
    int val;
    ss.str(num);
    ss>>val;
    return val;
}

// Convert integer to string
string stringInt(int num) {
    ostringstream ss;
    ss << num;
    return ss.str();
}

// Convert time in format MM:SS to total seconds as an integer
int calculateTime(string timeString) {
    vector <string> timeStrings = split(timeString, ':');
    int time = 0;
    time += intString(timeStrings[0]) * 60;
    time += intString(timeStrings[1]);
    return time;
}

// Convert time in total seconds to MM:SS format
string formatTime(int time) {
    int minutes = time / 60;
    int seconds = time % 60;
    return stringInt(minutes) + ":" + stringInt(seconds);
}


/***********************************
************ STRUCTURES ************
************************************/


struct Song {
    string title;
    int time;  // could also be a string
    int track;

    Song(string title, string timeString, string trackString) {
        this->title = title;
        this->track = intString(trackString);
        this->time = calculateTime(timeString);
    }

    void print() {
        printf("                %d. %s: %s\n", track, title.c_str(), formatTime(time).c_str());
    }
};

struct Album {
    vector<Song> songs;
    string name;

    Album(string name) {
        this->name = name;
    }

    int time() {
        int total = 0;
        for (size_t i = 0; i < songs.size(); i++) {
            total += songs[i].time;
        }
        return total;
    }

    int songCount() {
        return (int)songs.size();
    }

    void print() {
        printf("        %s, %d, %s\n", name.c_str(), songCount(), formatTime(time()).c_str());
        for (size_t i = 0; i < songs.size(); i++) {
            songs[i].print();
        }
    }
};

struct Artist {
    vector<Album *> albums;
    string name;

    Artist(string name) {
        this->name = name;
    }

    ~Artist() {
        for (size_t i = 0; i < albums.size(); i++) {
            delete albums[i];
        }
    }

    Album * getAlbum(string albumName) {
        for (size_t i = 0; i < albums.size(); i++) {
            if (albums[i]->name == albumName) {
                return albums[i];
            }
        }
        Album * album = new Album(albumName);
        albums.push_back(album);
        return album;
    }

    int songCount() {
        int sum = 0;
        for (size_t i = 0; i < albums.size(); i++) sum += albums[i]->songCount();
        return sum;
    }

    string getTime() {
        int timeSum = 0;
        for (size_t i = 0; i < albums.size(); i++) {
            timeSum += albums[i]->time();
        }
        return formatTime(timeSum);
    }

    void print() {
        printf("%s: %d, %s\n", name.c_str(), songCount(), getTime().c_str());
        for (size_t i = 0; i < albums.size(); i++) {
            albums[i]->print();
        }
    }
};


/***********************************
*************** MAIN ***************
************************************/

int main(int argc, char *argv[]) {
    ifstream fin;
    fin.open(argv[1]);
    string line;
    vector<Artist *> artists;

    while (getline(fin, line)) {
        vector <string> songInfo = split(line, ' ');
        Artist * artist = nullptr;
        bool newArtist = true;

        string artistName = replace(songInfo[2], '_', ' ');
        for (size_t i = 0; i < artists.size(); i++) {
            if (artists[i]->name == artistName) {
                artist = artists[i];
                newArtist = false;
                break;
            }
        }

        if (newArtist) {
            artist = new Artist(artistName);
            artists.push_back(artist);
        }

        string albumName = replace(songInfo[3], '_', ' ');
        Album * album = artist->getAlbum(albumName);

        string songTitle = replace(songInfo[0], '_', ' ');
        string timeString = songInfo[1];
        string trackString = songInfo[5];
        Song song = Song(songTitle, timeString, trackString);
        album->songs.push_back(song);

    }

    for (size_t i = 0; i < artists.size(); i++) {
        artists[i]->print();
        delete artists[i];
    }

    fin.close();
}