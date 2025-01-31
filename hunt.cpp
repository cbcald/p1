

#include <vector>
#include <iostream>
#include <algorithm> // std::sort
#include <getopt.h>
#include <string>    // needed for VS
#include <stack>
#include <queue>
#include "coordinate.h"
#include "pair.h"
#include <cstdlib> // For atoi

//Project Identifier: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

// 
// ----------------------------------------------------------------------------
//                    TreasureHunt Declarations
// ----------------------------------------------------------------------------


class TreasureHunt {
    
public:
    
    // Read in the file 
    void read_data();

    // Read and process command line arguments.
    void get_options(int argc, char** argv);
    
    // to be used in the driver
    void hunt();

    void land_hunt(Pair &land_coordinates); 

    void sail_north(); 
    void sail_south(); 
    void sail_east(); 
    void sail_west(); 
    //change back to a public for deque    

    void print_stats(); 

    void find_path_map(); 
    void find_path_list(); 
    void count_path(); 

    void print_path_list(); 
    void print_path_map();

private:

    Pair search_location; // hold the currect search location
    Pair sail_location = {0,0};  // hold the current sail location 

    char filetype; // this is either Map or List
    size_t dimention = 0; // will determine how large the vector is to hold the whole map 
    int num_water_locations = 0; // to print out
    int num_land_locations = 0; // to print out 
    int times_ashore = 0; // to print out 
    Pair starting_location; 
    Pair treasure_location; 
    bool treasure_found = false; 
    // all of these are to keep track of what is specificed on the options on command line 
    string hunt_order = "NESW"; 
    string c_option = "STACK"; 
    string fm_option = "QUEUE"; 
    bool verbose_option = false; 
    bool stats_option = false; 
    bool p_option = false; 
    char type_path = 'L'; 

    int path_length =0; 

    deque<Pair> captian; 
    vector<Pair> land_locs; 
    vector<Pair> water_locs; 


    vector<vector<Coordinate>> map; // the vector that will act as a map and will be filled when read_data is called 

};




// ----------------------------------------------------------------------------
//                               Driver
// ----------------------------------------------------------------------------


int main(int argc, char** argv) {
    std::ios_base::sync_with_stdio(false);

    TreasureHunt treasure; 

    treasure.get_options(argc,argv); 

    treasure.read_data(); 

    treasure.hunt(); 

    //call functions read data

    // read data
}

// ----------------------------------------------------------------------------
//                    TreasureHunt Definitions
// ----------------------------------------------------------------------------



// Read and process command line options.
void TreasureHunt::get_options(int argc, char** argv) {
    int option_index = 0, option = 0;
    
    // Don't display getopt error messages about options
    opterr = false;

    // use getopt to find command line options
    struct option longOpts[] = {{ "help", no_argument, nullptr, 'h' },
                                { "captain", required_argument, nullptr, 'c'}, 
                                { "first-mate", required_argument, nullptr, 'f'}, 
                                { "hunt-order", required_argument, nullptr, 'o'},
                                { "verbose", no_argument, nullptr, 'v'},
                                { "stats", no_argument, nullptr, 's'},
                                {"show-path", required_argument, nullptr, 'p'},
                                { nullptr, 0, nullptr, '\0' }};
    
    while ((option = getopt_long(argc, argv, "hc:f:o:vsp:", longOpts, &option_index)) != -1) {
        switch (option) {
            case 'c' :{
                string arg {optarg}; 
                if (arg != "QUEUE" && arg != "STACK"){
                    cerr << "Invalid argument to --captain" << '\n'; 
                    exit(1); 
                }

                if (arg == "QUEUE"){
                    c_option = "QUEUE"; 
                }

                break; 
            }

            case 'f': {
                string arg {optarg}; 
                if (arg != "QUEUE" && arg != "STACK"){
                    cerr << "Invalid argument to --first-mate" << '\n'; 
                    cerr << "I dont recognize: " << arg << '\n'; 
                    exit(1); 
                }

                if (arg == "STACK"){
                    fm_option = "STACK"; 
                }

                break; 
            }

            case 'o': {
                string arg {optarg}; 
                if (arg != "NESW" && arg != "NEWS" && arg != "NSEW" && arg != "NSWE" && 
                    arg != "NWES" && arg != "NWSE" && arg != "ENSW" && arg != "ENWS" &&
                    arg != "ESNW" && arg != "ESWN" && arg != "EWNS" && arg != "EWSN" && 
                    arg != "SNEW" && arg != "SNWE" && arg != "SENW" && arg != "SWEN" && 
                    arg != "SWNE" && arg != "SEWN" && arg != "WNES" && arg != "WNSE" && 
                    arg != "WENS" && arg != "WESN" && arg != "WSEN" && arg != "WSNE"){
                    cerr << "Invalid argument to --hunt-order" << '\n'; 
                    cerr << "I dont recognize: " << arg << '\n'; 
                    exit(1); 
                }

                if(arg.size() != 4){
                    cerr << "Invalid argument to --hunt-order" << '\n'; 
                    cerr << "I dont recognize: " << arg << '\n'; 
                    exit(1); 
                }

                if (arg.length() != 4 || 
                    count(arg.begin(), arg.end(), 'N') != 1 || 
                    count(arg.begin(), arg.end(), 'E') != 1 ||
                    count(arg.begin(), arg.end(), 'S') != 1 ||
                    count(arg.begin(), arg.end(), 'W') != 1) {
                        cerr << "Invalid argument to --hunt-order" << '\n';
                        exit(1);
                }

                hunt_order = arg; 

                break; 
            }


            case 'v': 
                verbose_option = true; 
                break; 


            case 's': 
                stats_option = true; 
                break; 

            case 'p': {
                string arg {optarg}; 
                if ((arg != "M" && arg != "L")){
                    cerr << "Invalid argument to --show-path" << '\n'; 
                    cerr << "I dont recognize: " << arg << '\n'; 
                    exit(1); 
                }
                else if (p_option == true){
                    cerr<< "Specify --show-path only once" << '\n'; 
                    exit(1); 
                }
                p_option = true; 


                if (arg == "M"){
                    type_path = 'M'; 
                }
                break; 
            }

            case 'h':
                std::cout << "some sort of help message";
                exit(0);

            default:
                cerr << "Unknown option" << '\n';
                 exit(1);
        }
    }
}




// Read data into the program through stdin.
void TreasureHunt::read_data() {
    string junk;
    char word; 

    while (cin >> word){
        if (word == '#'){
            getline(cin , junk); 
        }

        if (word == 'L' || word == 'M'){
            filetype = word; 
            cin >> dimention; 
            break; 
        }
    }

    if (filetype == 'M'){
            Coordinate c; 
            map.resize(dimention, vector<Coordinate>(dimention, c)); 
                char type; 
                for (size_t r = 0; r < dimention; r++){
                    for (size_t c = 0; c < dimention; c++){
                        cin >> type; 
                        Pair coordinates = {r,c}; 
                        Coordinate new_coordinate = {type ,'1'}; 
                        map[r][c] =  new_coordinate; 
                        if (type == '@'){
                            starting_location = coordinates; 
                            map[r][c].direction = 'K'; 

                        }
                    }
                }
    }
    
    else if (filetype == 'L'){
        
        Coordinate c = {'.', '1'}; 
        map.resize(dimention, vector<Coordinate>(dimention, c)); 

        size_t row, col;
        char type; 
        char word; 

        while (cin >> word){
            row = static_cast<size_t>(word) - '0'; 
            cin >> col; 
            cin >> type;
            Pair coordinates = {row, col}; 
            Coordinate c = {type, '1'}; 
            map[row][col] = c; 
            if (type == '@'){
                starting_location = coordinates; 
                map[row][col].direction = 'K'; 
            }
        }
    }

}


// Sort and print the data.
void TreasureHunt::hunt() {

    captian.push_back(starting_location); 

    //map[starting_location.first]
        //[starting_location.second]; 
    
    sail_location = {starting_location.first, starting_location.second}; 

    if (verbose_option){
        cout << "Treasure hunt started at: " << 
        starting_location.first << 
        "," << starting_location.second << '\n'; 
    }
    
    while (!captian.empty() && !treasure_found){
        // if stack pop back if queue pop front? 
        if (c_option == "STACK"){
            sail_location = captian.back(); 
            captian.pop_back(); 
            num_water_locations++; 
        }

        else if (c_option == "QUEUE"){
            sail_location = captian.front(); 
            captian.pop_front(); 
            num_water_locations++; 
        }
            size_t current_row = sail_location.first;
            size_t current_col = sail_location.second;

        for (char d: hunt_order){

            if (d == 'N' && current_row != 0 && map[current_row-1][current_col].direction == '1'){
                sail_north(); 
                if (treasure_found){
                    break; 
                }
            } //end if for north direction

            else if (d == 'S' && current_row != dimention -1 && 
                map[current_row+1][current_col].direction == '1'){
                sail_south(); 
                if (treasure_found){
                    break; 
                }
            }

            else if (d == 'E' && current_col != dimention -1 && 
                map[current_row][current_col+1].direction == '1'){
                    sail_east();
                    if (treasure_found){
                        break; 
                    } 
            }    

            else if (d == 'W' && current_col != 0 && 
                    map[current_row][current_col-1].direction == '1'){
                    sail_west(); 
                    if (treasure_found){
                        break;
                    }
            } 
        }
    }

    if (!treasure_found && verbose_option){
        cout << "Treasure hunt failed" << '\n'; 
    }


    if( p_option && type_path =='L' && treasure_found){
        find_path_list(); 
    }
    
    if (treasure_found){
        find_path_map(); 
    }

    if (stats_option){
        print_stats();
    }

    if (p_option && treasure_found && type_path == 'M'){
        print_path_map(); 
    }
    else if ( p_option && type_path =='L' && treasure_found){
        print_path_list(); 
    }

    if (treasure_found){
        cout 
            << "Treasure found at " 
            << treasure_location.first << "," 
            << treasure_location.second
            << " with path length " << path_length << '.' <<'\n';
    }

    else {

        cout 
            << "No treasure found after investigating " 
            << num_water_locations + num_land_locations 
            << " locations." << '\n'; 
    }

} 

void TreasureHunt::sail_north(){
     //so we can easily access the locations on the other side of this location 
    size_t current_row = sail_location.first;
    size_t current_col = sail_location.second;

    if (map[current_row-1][current_col].type == '.'){
        map[current_row-1][current_col].direction = 'N'; 
        Pair c = {current_row-1, current_col}; 
        captian.push_back(c);
    }

    else if (map[current_row-1][current_col].type == 'o' ||
        map[current_row-1][current_col].type == '$'){
            map[current_row-1][current_col].direction = 'N';
            Pair c = {current_row-1, current_col}; 
            land_hunt(c);
            times_ashore ++; 
            if (verbose_option){
                cout << "Went ashore at: " << current_row-1 << 
                "," << 
                current_col << '\n';  
            }

            if (treasure_found && verbose_option){
                cout
                << "Searching island... party found treasure at "
                << treasure_location.first
                << "," << treasure_location.second 
                << "." << '\n'; 

                // do things here for what happens when you find treasure 
            }

            else if (verbose_option){
                cout 
                << "Searching island... party returned with no treasure. " << '\n'; 
                //the treasure was not found 
            }

    }

}

void TreasureHunt::sail_south(){

    size_t current_row = sail_location.first;
    size_t current_col = sail_location.second;

    if (map[current_row+1][current_col].type == '.'){

                    map[current_row+1][current_col].direction = 'S'; 
                    Pair c = {current_row+1, current_col}; 
                    captian.push_back(c); 

    }

    else if (map[current_row+1][current_col].type == 'o' ||
         map[current_row+1][current_col].type == '$'){
                            map[current_row+1][current_col].direction = 'S';
                            Pair south = {current_row+1,current_col}; 
                            land_hunt(south); 
                            times_ashore++;
                            if (verbose_option){
                                cout << "Went ashore at: " << current_row +1 << 
                                "," << 
                                current_col << '\n'; 
                            }

                            if (treasure_found && verbose_option){
                                cout
                                    << "Searching island... party found treasure at "
                                    << treasure_location.first
                                    << "," << treasure_location.second 
                                    << "." << '\n'; 

                                // do things here for what happens when you find treasure 
                            }

                            else if (verbose_option){

                                cout 
                                    << "Searching island... party returned with no treasure. " << '\n'; 
                                //the treasure was not found 
                            }

                }

}

void TreasureHunt::sail_east(){
    size_t current_row = sail_location.first;
    size_t current_col = sail_location.second;

    if (map[current_row][current_col+1].type == '.'){

        map[current_row][current_col+1].direction = 'E'; 
        Pair c = {current_row, current_col+1};
        captian.push_back(c); 

    }

    else if (map[current_row][current_col+1].type == 'o' ||
            map[current_row][current_col+1].type == '$'){
            map[current_row][current_col+1].direction = 'E'; 
            Pair east = {current_row,current_col+1}; 
            land_hunt(east); 
            times_ashore++;
        if (verbose_option){
            cout << "Went ashore at: " << current_row << 
            "," << 
            current_col +1<< '\n'; 
            }

        if (treasure_found && verbose_option){
            cout
            << "Searching island... party found treasure at "
            << treasure_location.first
            << "," << treasure_location.second 
            << "." << '\n'; 
            // do things here for what happens when you find treasure 
        }

        else if (verbose_option){

            cout 
            << "Searching island... party returned with no treasure. " << '\n'; 
            //the treasure was not found 
            }

    }
}

void TreasureHunt::sail_west(){
    size_t current_row = sail_location.first;
    size_t current_col = sail_location.second;

    if (map[current_row][current_col-1].type == '.'){

        map[current_row][current_col-1].direction = 'W'; 
        Pair c = {current_row, current_col-1}; 
        captian.push_back(c); 
    }

    else if (map[current_row][current_col-1].type == 'o' ||
            map[current_row][current_col-1].type == '$'){
                map[current_row][current_col-1].direction = 'W'; 
                Pair west = {current_row,current_col-1}; 
                land_hunt(west); 
                times_ashore++; 
        if (verbose_option){
            cout << "Went ashore at: " << current_row << 
            "," << 
            current_col-1 << '\n'; 
            }

        if (treasure_found && verbose_option){
            cout
            << "Searching island... party found treasure at "
            << treasure_location.first
            << "," << treasure_location.second 
            << "." << '\n'; 
            // do things here for what happens when you find treasure 
        }

        else if (verbose_option){

            cout 
            << "Searching island... party returned with no treasure. " << '\n'; 
            //the treasure was not found 
            }

    }
}

void TreasureHunt::land_hunt( Pair &cor){

    deque <Pair> first_mate; 
    
    Pair m = {cor.first,cor.second}; 
    first_mate.push_back(m);

    while (!first_mate.empty() && !treasure_found){
        if (fm_option == "STACK"){
            search_location =first_mate.back(); 
            first_mate.pop_back(); 
            num_land_locations++; 
        }

        else if (fm_option == "QUEUE"){
            search_location = first_mate.front();
            first_mate.pop_front(); 
            num_land_locations ++; 
        }
            size_t current_row = search_location.first; 
            size_t current_col = search_location.second; //dont need to do this everytime put it outside the for loops

            if (map[current_row][current_col].type == '$'){
                treasure_found = true; 
                treasure_location = {current_row,current_col};
            }


        for (char d: hunt_order){

            if (treasure_found){
                break; 
            }
    
            if (d == 'N' && current_row != 0 && map[current_row-1][current_col].type != '#' &&
                map[current_row-1][current_col].type != '.'  && map[current_row-1][current_col].direction == '1'){
                    if (map[current_row-1][current_col].type == '$'){
                        treasure_found = true; 
                        treasure_location = {current_row-1, current_col}; 
                        map[current_row-1][current_col].direction = 'N'; 
                        num_land_locations++; 
                        break; 
                    }

                    else if (map[current_row-1][current_col].type == 'o'){
                        map[current_row-1][current_col].direction = 'N'; 
                        Pair p = {current_row-1,current_col}; 
                        first_mate.push_back(p); 
                    }
                }

            else if (d == 'S' && current_row != dimention-1 && 
                map[current_row+1][current_col].type != '.'  && map[current_row+1][current_col].direction == '1'){
                    if (map[current_row+1][current_col].type == '$'){
                        treasure_found = true; 
                        treasure_location = {current_row+1,current_col}; 
                        map[current_row+1][current_col].direction = 'S'; 
                        num_land_locations++; 
                        break;
                    }
                    else if (map[current_row+1][current_col].type == 'o'){
                        map[current_row+1][current_col].direction = 'S';  
                        Pair p = {current_row+1, current_col};
                        first_mate.push_back(p); 
                    }
                }

            else if (d == 'E' && current_col != dimention-1 &&
                map[current_row][current_col+1].type != '.'  && map[current_row][current_col+1].direction == '1'){
                    if (map[current_row][current_col+1].type == '$'){
                        treasure_found = true; 
                        treasure_location = {current_row,current_col+1}; 
                        map[current_row][current_col+1].direction = 'E'; 
                        num_land_locations++; 
                        break; 

                    }
                    else if (map[current_row][current_col+1].type == 'o'){
                        map[current_row][current_col+1].direction = 'E'; 
                        Pair p = {current_row,current_col+1}; 
                        first_mate.push_back(p);
                    }
                }

            else if (d == 'W' && current_col != 0 && 
                map[current_row][current_col-1].type != '.' && map[current_row][current_col-1].direction == '1'){
                    if (map[current_row][current_col-1].type == '$'){
                        treasure_found = true; 
                        treasure_location = {current_row,current_col-1};
                        map[current_row][current_col-1].direction = 'W'; 
                         num_land_locations++; 
                         break; 


                    }
                    else if (map[current_row][current_col-1].type == 'o'){
                        map[current_row][current_col-1].direction = 'W'; 
                        Pair p = {current_row,current_col-1}; 
                        first_mate.push_back(p); 

                    }
                }


        }



    }

}


void TreasureHunt::print_stats(){
    cout
        << "--- STATS ---" << '\n'
        << "Starting location: " << starting_location.first
        << "," << starting_location.second << '\n'
        << "Water locations investigated: " << num_water_locations << '\n'
        << "Land locations investigated: " << num_land_locations << '\n'
        << "Went ashore: " << times_ashore << '\n'; 

    if (treasure_found){ 
        cout
            << "Path length: " << path_length << '\n'
            << "Treasure location: " << treasure_location.first
            << "," << treasure_location.second << '\n'; 
    }

    cout
        << "--- STATS ---" << '\n';
}

void TreasureHunt::find_path_map(){

    
    Pair current = treasure_location; 
    map[treasure_location.first][treasure_location.second].type = 'X'; 

    Pair next = {current.first,current.second}; 

        while (map[next.first][next.second].direction != 'K') {

            if (map[current.first][current.second].direction == 'N'){
                next = {current.first+1 ,current.second}; 
            }
            else if (map[current.first][current.second].direction == 'S'){
                next = {current.first-1,current.second}; 
            }
            else if (map[current.first][current.second].direction == 'W'){
                next = {current.first, current.second +1}; 
            }
            else if (map[current.first][current.second].direction == 'E'){
                next = {current.first,current.second -1}; 
            }

            if ((map[current.first][current.second].direction == 'S' || 
                map[current.first][current.second].direction == 'N')
                && (map[next.first][next.second].direction == 'E' ||
                    map[next.first][next.second].direction == 'W') ){
                    map[next.first][next.second].type = '+'; 
                }

            else if ((map[next.first][next.second].direction == 'S' || 
                    map[next.first][next.second].direction == 'N')
                    && (map[current.first][current.second].direction == 'E' || 
                    map[current.first][current.second].direction == 'W') ){
                    map[next.first][next.second].type = '+'; 
                }

            else if (map[next.first][next.second]. direction == 'S' || 
                   map[next.first][next.second].direction == 'N'){
                    map[next.first][next.second].type = '|'; 
            }

            else if (map[next.first][next.second].direction == 'W' || 
                    map[next.first][next.second].direction == 'E'){
                    map[next.first][next.second].type = '-'; 
            }

            current = next;   
            path_length++;           
        }

        

}


void TreasureHunt::find_path_list(){

    Pair current = treasure_location;

    Pair next; 

    land_locs.push_back(treasure_location); 

    while (map[current.first][current.second].direction != 'K'){

        if (map[current.first][current.second].direction == 'N'){
            next = {current.first+1,current.second}; 
        }
        else if (map[current.first][current.second].direction == 'S'){
            next = {current.first-1,current.second}; 
        }
        else if (map[current.first][current.second].direction == 'W'){
            next = {current.first,current.second+1}; 
        }
        else if (map[current.first][current.second].direction == 'E'){
            next = {current.first,current.second-1};
        }

        if (map[next.first][next.second].type == '.'){
            water_locs.push_back(next); 
        }
        else if (map[next.first][next.second].type == 'o' || 
                map[next.first][next.second].type == '$'){
            land_locs.push_back(next); 
        }

        current = next; 

    }   
    water_locs.push_back(starting_location); 
}


void TreasureHunt::print_path_list(){
    
    cout << "Sail:" << '\n'; 
        for (size_t i = water_locs.size(); i > 0; i--){
            cout << water_locs[i-1].first << ',' << water_locs[i-1].second << '\n'; 
        }
        
    cout << "Search:" << '\n'; 

        for (size_t i = land_locs.size(); i >0; i--){
            cout << land_locs[i-1].first << ',' << land_locs[i-1].second << '\n'; 
        }

}

void TreasureHunt::print_path_map(){
    for (size_t r = 0; r < dimention; r++){
            for (size_t c = 0; c < dimention; c++){
                cout << map[r][c].type; 
            }
            cout << '\n'; 
        }
}
