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
        
        Data(float x, float y, float width, float height): x(x), y(y), width(width), height(height){}
        
        float x;
        float y;
        float width;
        float height;
    };
    
    XMLLoad(const std::string& filepath){
        std::ifstream ifs (RESOURCE_FOLDER + filepath);
        if (!ifs) {
            std::cout << "Unable to load XML sheet in the path " << filepath << ". Make sure the path is correct\n";
            exit(1);
        }
        
        
        
        
        
        ifs.close();
    }
    

    void displayKeys() const {
        
    }
    
    
    Data getData(const std::string& name) const {
        return Data(0,0,0,0);
    }
    
    
    
    
    
private:
    std::map<std::string, Data> table;
    
};

#endif /* XMLLoad_h */
