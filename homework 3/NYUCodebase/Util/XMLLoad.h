// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#ifndef XMLLoad_h
#define XMLLoad_h

#include <iostream>
#include <fstream>
#include <map>
#include <string>


#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"

class XMLLoad{
public:
    class Data {
    public:
        friend std::ostream& operator<<(std::ostream& os, const Data& data){
            os << "x: " << data.x << " y: " << data.y << " width: " << data.width << " height: " << data.height << std::endl;
            return os;
        }
        Data(){}
        Data(float x, float y, float width, float height): x(x), y(y), width(width), height(height){}
        
        float x;
        float y;
        float width;
        float height;
    };
    
    XMLLoad(const std::string& filepath): name(filepath){
        std::ifstream ifs (RESOURCE_FOLDER + filepath);
        if (!ifs) {
            std::cout << "Unable to load XML sheet in the path " << filepath << ". Make sure the path is correct\n";
            exit(1);
        }
        
        std::string first, subtext, name, x, y, width, height;
        
        // first line contains the information about the entire sheet
        getline(ifs, first);
        
        while (ifs >> subtext >> name >> x >> y >> width >> height){
            // assume the pattern is name="beam0.png" x="143" y="377" width="43" height="31"/>
            // substr: start position, length of the substring
//            name = name.substr(name.find_first_of("\"") + 1, name.size() - name.find_first_of("\"") - 2);
            
            table[name.substr(6, name.size() - 6 - 1)] = Data(stoi(x.substr(3, x.size() - 3 - 1)), stoi(y.substr(3, y.size() - 3 - 1)), stoi(width.substr(7, width.size() - 7 - 1)), stoi(height.substr(7, height.size() - 7 - 1)));
            
        }
        
        
        
        ifs.close();
    }
    
    
    
    void display(std::ostream& os = std::cout){
        std::cout << "XML sheet: " << name << std::endl;
        for (auto i = table.begin(); i != table.end(); i++){
            std::cout << i->first << " " << i->second << std::endl;
        }
    }
    
    void getKeys() const {
        
    }
    
    Data getData(const std::string& name) const {
        return Data(0,0,0,0);
    }
    
    
    
    
    
private:
    std::string name;
    std::map<std::string, Data> table;
    
};

#endif /* XMLLoad_h */
