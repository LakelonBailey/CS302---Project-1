#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

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

string join(vector<string> strings, char separator) {
    string result = "";
    for (size_t i = 0; i < strings.size(); i++) {
        if (i) result += separator;
        result += strings[i];
    }
    return result;
}

string replace(string item, char toReplace, char replaceWith) {
    return join(split(item, toReplace), replaceWith);
}

int intString(string num) {
    istringstream ss;
    int val;
    ss.str(num);
    ss>>val;
    return val;
}

int calculateTime(string timeString) {
    vector <string> timeStrings = split(timeString, ':');
    int time = 0;
    time += intString(timeStrings[0]) * 60;
    time += intString(timeStrings[1]);
    return time;
}

struct Song {
    string title;
    int time;  // could also be a string
    int track;

    Song(string title, string timeString, string trackString) {
        this->title = title;
        this->track = intString(trackString);
        this->time = calculateTime(timeString);
    }
};

struct Album {
    vector<Song> songs;
    string name;
    int nsongs = 1;  // optional variable but makes it easier

    int time() {
        int total = 0;
        for (size_t i = 0; i < songs.size(); i++) {
            total += songs[i].time;
        }
        return total;
    }
};

struct Artist {
    vector<Album> albums;
    string name;
    int time = 0;
    int nsongs = 0;

    Artist(string name) {
        this->name = name;
    }

    void addAlbum(Album album) {
        this->albums.push_back(album);
        this->time += album.time();
    }
};

int main(int argc, char *argv[]) {
    ifstream fin;
    fin.open(argv[1]);
    string line;
    vector<Artist *> artists;

    while (getline(fin, line)) {
        vector <string> items = split(line, ' ');
        Artist * artist = nullptr;
        bool newArtist = true;

        string artistName = replace(items[2], '_', ' ');
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

    }

    for (size_t i = 0; i < artists.size(); i++) {
        cout<<artists[i]->name<<endl;
        delete artists[i];
    }

    fin.close();
}