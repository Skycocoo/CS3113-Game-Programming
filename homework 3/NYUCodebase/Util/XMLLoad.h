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
        
        // skip the first line (not useful)
        getline(ifs, first);
        
        
        // assume the pattern is name="beam0.png" x="143" y="377" width="43" height="31"/>
        // substr: start position, length of the substring
        while (ifs >> subtext >> name >> x >> y >> width >> height){
            name = name.substr(6, name.size() - 6 - 1);
            x = x.substr(3, x.size() - 3 - 1);
            y = y.substr(3, y.size() - 3 - 1);
            width = width.substr(7, width.size() - 7 - 1);
            height = height.substr(8, height.size() - 8 - 3);
            
            table[name] = Data(stoi(x), stoi(y), stoi(width), stoi(height));
        }
        ifs.close();
    }
    
    
    
    void display(std::ostream& os = std::cout) const {
        std::cout << "XML sheet: " << name << std::endl;
        for (auto i = table.begin(); i != table.end(); i++){
            std::cout << i->first << " " << i->second;
        }
    }
    
    void getKeys() const {
        int style = 0;
        for (auto i = table.begin(); i != table.end(); i++){
            std::cout << i->first << " ";
            if (style % 3 == 0) std::cout << std::endl;
            ++style;
        }
    }
    
    Data getData(const std::string& name) {
        return table[name];
    }
    
    
    
    
    
private:
    std::string name;
    std::map<std::string, Data> table;
    
};

#endif /* XMLLoad_h */
