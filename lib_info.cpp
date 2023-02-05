#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>

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
    ostringstream timeString;
    timeString << stringInt(minutes) << ':' << setw(2) << setfill('0') << stringInt(seconds);
    return timeString.str();
}


/***********************************
************ STRUCTURES ************
************************************/


struct Song {
    string title;
    int time;
    int track;

    Song(string title, string timeString, string trackString) {
        this->title = title;
        this->track = intString(trackString);
        this->time = calculateTime(timeString);
    }

    // Print formatted values
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

    // Sum total time of all songs
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
        printf("        %s: %d, %s\n", name.c_str(), songCount(), formatTime(time()).c_str());
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

    // Add or retrieve album
    Album * getAlbum(string albumName) {
        for (size_t i = 0; i < albums.size(); i++) {
            if (albums[i]->name == albumName) {
                return albums[i];
            }
        }

        // Add in correct spot based on lexicographical ordering
        size_t pos;
        for (pos = 0; pos < albums.size(); pos++) {
            if (albumName < albums[pos]->name) {
                break;
            }
        }
        Album * album = new Album(albumName);
        albums.insert(albums.begin() + pos, album);
        return album;
    }

    // Get total song count
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

    // Open and begin reading file
    ifstream fin;
    fin.open(argv[1]);
    string line;
    vector<Artist *> artists;
    while (getline(fin, line)) {

        // Get song info
        vector <string> songInfo = split(line, ' ');

        // Find or create artist
        Artist * artist = nullptr;
        bool newArtist = true;
        string artistName = songInfo[2];
        replace(artistName.begin(), artistName.end(), '_', ' ');

        for (size_t i = 0; i < artists.size(); i++) {
            if (artists[i]->name == artistName) {
                artist = artists[i];
                newArtist = false;
                break;
            }
        }

        if (newArtist) {
            artist = new Artist(artistName);
            size_t pos;

            // Insert artist in correct position based on name
            for (pos = 0; pos < artists.size(); pos++) {
                if (artistName < artists[pos]->name) {
                    break;
                }
            }
            artists.insert(artists.begin() + pos, artist);
        }

        // Get album
        string albumName = songInfo[3];
        replace(albumName.begin(), albumName.end(), '_', ' ');

        Album * album = artist->getAlbum(albumName);

        // Create song and add to album based on track number
        string songTitle = songInfo[0];
        replace(songTitle.begin(), songTitle.end(), '_', ' ');

        string timeString = songInfo[1];
        string trackString = songInfo[5];
        Song song = Song(songTitle, timeString, trackString);

        size_t song_pos;
        for (song_pos = 0; song_pos < album->songs.size(); song_pos++) {
            if (song.track < album->songs[song_pos].track) {
                break;
            }
        }
        album->songs.insert(album->songs.begin() + song_pos, song);

    }

    // Print artists then deallocate
    for (size_t i = 0; i < artists.size(); i++) {
        artists[i]->print();
        delete artists[i];
    }

    fin.close();
}