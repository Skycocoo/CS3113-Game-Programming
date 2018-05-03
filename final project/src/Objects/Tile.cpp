// Yuxi Luo (yl4217), March 27, 2018
// CS3113 Game Programming

#include "Tile.hpp"
#include "Player.hpp"
#include <fstream>

extern glm::vec3 center;
extern ShaderProgram textured;
// extern Mix_Chunk* hit;


#define RESOURCE_FOLDER "../src/"

// assume tiles start drawing at (0,0)
Tile::Tile(): Object(){}

// move copy constructor
Tile::Tile(Tile&& rhs): map(rhs.map){
    rhs.map = FlareMap();
}

// move assignment operator
Tile& Tile::operator=(Tile&& rhs){
    map = rhs.map;
    rhs.map = FlareMap();
    return *this;
}


Tile::Tile(const std::string& tex, const std::string& txt, float tilesize): Object(&textured, 0), tilesize(tilesize){
    texture = LoadTexture((RESOURCE_FOLDER + tex + ".png").c_str());
    loadMap(txt);
}

// should take care of block types
void Tile::loadMap(const std::string& txt){
    // destructing previous map
    map.~FlareMap();
    map.Load(txt);

    vertices.clear();
    texCoords.clear();
    deco.clear();
    trap.clear();

    // the data of sprite sheet for tilemap
    int spritex = 30, spritey = 30;

    for (int y = 0; y < map.mapHeight; y++){
        for (int x = 0; x < map.mapWidth; x++){
            // std::cout << map.mapData[y][x] << " ";
            if (map.mapData[y][x] != -1){
                float u = (float)(((int) map.mapData[y][x]) % spritex) / (float) spritex;
                float v = (float)(((int) map.mapData[y][x]) / spritex) / (float) spritey;
                float spriteWidth = 1.0f / (float) spritex;
                float spriteHeight = 1.0f / (float) spritey;
                vertices.insert(vertices.end(), {
                    tilesize * x, -tilesize * y,
                    tilesize * x, (-tilesize * y)-tilesize,
                    (tilesize * x)+tilesize, (-tilesize * y)-tilesize,
                    tilesize * x, -tilesize * y,
                    (tilesize * x)+tilesize, (-tilesize * y)-tilesize,
                    (tilesize * x)+tilesize, -tilesize * y
                });

                texCoords.insert(texCoords.end(), {
                    u, v,
                    u, v+(spriteHeight),
                    u+spriteWidth, v+(spriteHeight),
                    u, v,
                    u+spriteWidth, v+(spriteHeight),
                    u+spriteWidth, v
                });
            }
        }
        // std::cout << std::endl;
    }
    shape = glm::vec3(map.mapWidth * tilesize, map.mapHeight * tilesize, 0);

    loadType(txt);
    // pos = glm::vec3((map.mapWidth * tilesize) / 2, (-map.mapHeight * tilesize) / 2, 0);

}


void Tile::loadType(const std::string& txt){
    std::ifstream ifs(RESOURCE_FOLDER + txt + "_block_types.txt");
    if(!ifs){
        std::cout << "Unable to load FlareMap in the path " << txt << ". Make sure the path is correct\n";
        exit(1);
    }

    std::string line;
    int index, index2;
    char comma = 0;

    while (ifs >> line){
        std::string temp = comma + line;
        if (temp == "decoration" || line == "decoration"){
            while (ifs >> index >> comma){
                deco.insert(index - 1);
                if (comma != ',') break;
            }
        } else if (temp == "trap"){
            while (ifs >> index >> comma){
                trap.insert(index - 1);
                if (comma != ',') break;
            }
            trap.insert(index);
        } else if (temp == "start"){
            ifs >> index >> comma >> index2 >> comma;
            pos = glm::vec3((index * tilesize), (-index2 * tilesize), 0);
        } else if (temp == "end"){
            while (ifs >> index >> comma){
                end.push_back(index);
                if (comma != ',') break;
            }
            end.push_back(index);
        }
    }
}



void Tile::render(const Matrix& view){
    glUseProgram(program->programID);

    program->SetModelMatrix(modelMatrix);
    program->SetProjectionMatrix(projectionMatrix);
    program->SetViewMatrix(view);

    glBindTexture(GL_TEXTURE_2D, texture);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 2);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}


bool Tile::collide(Object& rhs) const {
    rhs.coll.reset();
    bool collide = false;

    int tileX = int(rhs.pos.x / tilesize), tileY = int(-rhs.pos.y / tilesize);

    // float enUp = -FLT_MAX, enDown = FLT_MAX, enLeft = FLT_MAX, enRight = -FLT_MAX;
    // for (size_t i = 0; i < rhs.points.size(); i++){
    //     glm::vec3 point = rhs.modelMatrix * rhs.points[i];
    //     if (point.x < enLeft) enLeft = point.x;
    //     if (point.x > enRight) enRight = point.x;
    //     if (point.y > enUp) enUp = point.y;
    //     if (point.y < enDown) enDown = point.y;
    // }

    float enUp = rhs.pos.y + rhs.shape.y / 2,
    enDown = rhs.pos.y - rhs.shape.y / 2,
    enLeft = rhs.pos.x - rhs.shape.x / 2,
    enRight = rhs.pos.x + rhs.shape.x / 2;

    int tileUp = int(-enUp / tilesize),
    tileDown = int(-enDown / tilesize),
    tileLeft = int(enLeft/ tilesize),
    tileRight = int(enRight/ tilesize);

    if ((tileUp > 0) && (tileDown < map.mapHeight) && (tileLeft > 0) && (tileRight < map.mapWidth)){

        if ((map.mapData[tileUp][tileX] != -1) || (map.mapData[tileDown][tileX] != -1) ||
            (map.mapData[tileY][tileLeft] != -1) || (map.mapData[tileY][tileRight] != -1))
            collide = true;

        if (collide){
            if (map.mapData[tileUp][tileX] != -1 && deco.find(map.mapData[tileUp][tileX]) == deco.end()) {
                rhs.coll.top = true;
                rhs.pos.y -= fabs((-tilesize * tileUp - tilesize) - enUp) + 0.0001;
            }
            if (map.mapData[tileDown][tileX] != -1 && deco.find(map.mapData[tileDown][tileX]) == deco.end()) {
                rhs.coll.bottom = true;
                rhs.pos.y += fabs(enDown - (-tilesize * tileDown)) + 0.0001;
            }
            if (map.mapData[tileY][tileLeft] != -1 && deco.find(map.mapData[tileY][tileLeft]) == deco.end()) {
                rhs.coll.left = true;
                rhs.pos.x += fabs(enLeft - (tilesize * tileLeft + tilesize)) + 0.0001;
            }
            if (map.mapData[tileY][tileRight] != -1 && deco.find(map.mapData[tileY][tileRight]) == deco.end()) {
                rhs.coll.right = true;
                rhs.pos.x -= fabs((tilesize * tileRight) - enRight) + 0.0001;
            }
        }
    } else {
        collide = true;

        if (tileUp <= 0) {
            rhs.coll.top = true;
            rhs.pos.y -= fabs((-tilesize * tileUp - tilesize) - enUp) + 0.0001;
        } else if (tileDown >= map.mapHeight) {
            rhs.coll.bottom = true;
            rhs.pos.y += fabs(enDown - (-tilesize * tileDown)) + 0.0001;
        } else if (tileLeft <= 0) {
            rhs.coll.left = true;
            rhs.pos.x += fabs(enLeft - (tilesize * tileLeft + tilesize)) + 0.0001;
        } else if (tileRight >= map.mapWidth) {
            rhs.coll.right = true;
            rhs.pos.x -= fabs((tilesize * tileRight) - enRight) + 0.0001;
        }
    }
    // if (rhs.coll.left || rhs.coll.right) Mix_PlayChannel(-1, hit, 0);
    // std::cout << "original: " << enLeft << " " << enRight << " " << enUp << " " << enDown << std::endl;

    return collide;
}

// manage collision with end & traps for player only
bool Tile::collide(Player& rhs) const {
    rhs.coll.reset();
    bool collide = false;

    int tileX = int(rhs.pos.x / tilesize), tileY = int(-rhs.pos.y / tilesize);

    float enUp = rhs.pos.y + rhs.shape.y / 2,
    enDown = rhs.pos.y - rhs.shape.y / 2,
    enLeft = rhs.pos.x - rhs.shape.x / 2,
    enRight = rhs.pos.x + rhs.shape.x / 2;

    int tileUp = int(-enUp / tilesize),
    tileDown = int(-enDown / tilesize),
    tileLeft = int(enLeft/ tilesize),
    tileRight = int(enRight/ tilesize);

    if ((tileUp > 0) && (tileDown < map.mapHeight) && (tileLeft > 0) && (tileRight < map.mapWidth)){
        if ((map.mapData[tileUp][tileX] != -1) || (map.mapData[tileDown][tileX] != -1) ||
            (map.mapData[tileY][tileLeft] != -1) || (map.mapData[tileY][tileRight] != -1))
            collide = true;

        if (collide){
            // std::cout << tileY << " " << tileLeft << std::endl;;
            if (map.mapData[tileUp][tileX] != -1) {
                if (deco.find(map.mapData[tileUp][tileX]) == deco.end()){
                    rhs.coll.top = true;
                    rhs.pos.y -= fabs((-tilesize * tileUp - tilesize) - enUp) + 0.0001;

                    if (trap.find(map.mapData[tileUp][tileX]) != trap.end()){
                        rhs.dead();
                        return true;
                    }
                } else {
                    for (size_t i = 0; i < end.size()/2; i++){
                        if (end[2 * i] == tileX && end[2 * i + 1] == tileUp) rhs.end = true;
                    }
                }
            }
            if (map.mapData[tileDown][tileX] != -1) {
                if (deco.find(map.mapData[tileDown][tileX]) == deco.end()){
                    rhs.coll.bottom = true;
                    rhs.pos.y += fabs(enDown - (-tilesize * tileDown)) + 0.0001;

                    if (trap.find(map.mapData[tileDown][tileX]) != trap.end()){
                        rhs.dead();
                        return true;
                    }
                } else {
                    for (size_t i = 0; i < end.size()/2; i++){
                        if (end[2 * i] == tileX && end[2 * i + 1] == tileDown) rhs.end = true;
                    }
                }
            }
            if (map.mapData[tileY][tileLeft] != -1) {
                if (deco.find(map.mapData[tileY][tileLeft]) == deco.end()){
                    rhs.coll.left = true;
                    rhs.pos.x += fabs(enLeft - (tilesize * tileLeft + tilesize)) + 0.0001;

                    if (trap.find(map.mapData[tileY][tileLeft]) != trap.end()){
                        rhs.dead();
                        return true;
                    }
                } else {
                    for (size_t i = 0; i < end.size()/2; i++){
                        // first x then y for the location of end
                        if (end[2 * i] == tileLeft && end[2 * i + 1] == tileY) {
                            rhs.end = true;
                            return true;
                        }
                    }
                }
            }
            if (map.mapData[tileY][tileRight] != -1) {
                if (deco.find(map.mapData[tileY][tileRight]) == deco.end()){
                    rhs.coll.right = true;
                    rhs.pos.x -= fabs((tilesize * tileRight) - enRight) + 0.0001;

                    if (trap.find(map.mapData[tileY][tileRight]) != trap.end()){
                        rhs.dead();
                        return true;
                    }
                } else {
                    for (size_t i = 0; i < end.size()/2; i++){
                        if (end[2 * i] == tileRight && end[2 * i + 1] == tileY) rhs.end = true;
                    }
                }
            }
        }
    } else {
        collide = true;

        if (tileUp <= 0) {
            rhs.coll.top = true;
            rhs.pos.y -= fabs((-tilesize * tileUp - tilesize) - enUp) + 0.0001;
        } else if (tileDown >= map.mapHeight) {
            rhs.coll.bottom = true;
            rhs.pos.y += fabs(enDown - (-tilesize * tileDown)) + 0.0001;
        } else if (tileLeft <= 0) {
            rhs.coll.left = true;
            rhs.pos.x += fabs(enLeft - (tilesize * tileLeft + tilesize)) + 0.0001;
        } else if (tileRight >= map.mapWidth) {
            rhs.coll.right = true;
            rhs.pos.x -= fabs((tilesize * tileRight) - enRight) + 0.0001;
        }
    }
    // if (rhs.coll.left || rhs.coll.right) Mix_PlayChannel(-1, hit, 0);
    // std::cout << "original: " << enLeft << " " << enRight << " " << enUp << " " << enDown << std::endl;

    return collide;
}
