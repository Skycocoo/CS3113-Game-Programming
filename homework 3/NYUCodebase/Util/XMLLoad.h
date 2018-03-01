// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#ifndef XMLLoad_h
#define XMLLoad_h

#include <iostream>
#include <fstream>
#include <map>
#include <string>


#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"


class Data {
public:
    friend std::ostream& operator<<(std::ostream& os, const Data& data);
    
    Data();
    Data(float x, float y, float width, float height);
    
    float x;
    float y;
    float width;
    float height;
};

class XMLLoad{
public:
    XMLLoad(const std::string& filepath);
    
    void display(std::ostream& os = std::cout) const;
    void getKeys() const;
    Data getData(const std::string& name);

private:
    std::string name;
    std::map<std::string, Data> table;
};

#endif /* XMLLoad_h */
