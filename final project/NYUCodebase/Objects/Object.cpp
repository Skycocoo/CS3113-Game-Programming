// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#include "Object.hpp"
#include <math.h>
#include <utility>

using namespace std;

extern float screenWidth;
extern float screenHeight;
extern float splitScale;

std::ostream& operator<<(std::ostream& os, const Coll& c){
    os << std::boolalpha << "top: " << c.top << " bottom: " << c.bottom << " left: " << c.left << " right: " << c.right << "\n";
    return os;
}


Object::Object(){}

Object::Object(ShaderProgram* program, GLuint texture, const glm::vec3& pos): program(program), texture(texture), pos(pos), shape(1, 1, 1){
    projectionMatrix.SetOrthoProjection(-screenWidth, screenWidth, -screenHeight, screenHeight, -1.0f, 1.0f);
    satPoints();
}


void Object::update(float elapsed){
    modelMatrix.Identity();

    modelMatrix.Translate(pos.x, pos.y, pos.z);
    modelMatrix.Rotate(rotate);
    modelMatrix.Scale(scale, scale, scale);
}


void Object::render(const Matrix& view){
    // viewMatrix = view;

    program->SetModelMatrix(modelMatrix);
    program->SetProjectionMatrix(projectionMatrix);
    program->SetViewMatrix(view);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);

    if (glIsTexture(texture)){
        glBindTexture(GL_TEXTURE_2D, texture);
        glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
        glEnableVertexAttribArray(program->texCoordAttribute);
    }
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);

    if (glIsTexture(texture)) glDisableVertexAttribArray(program->texCoordAttribute);
}

// collision detection
// separate axis for collision handling
bool Object::collide(const Object& rhs) {
    coll.reset();
    bool collide = false;

    float objUp = pos.y + shape.y / 2,
    objDown = pos.y - shape.y / 2,
    objLeft = pos.x - shape.x / 2,
    objRight = pos.x + shape.x / 2;

    float enUp = rhs.pos.y + rhs.shape.y / 2,
    enDown = rhs.pos.y - rhs.shape.y / 2,
    enLeft = rhs.pos.x - rhs.shape.x / 2,
    enRight = rhs.pos.x + rhs.shape.x / 2;

    // intersecting
    if (!(objUp < enDown || objLeft > enRight || objDown > enUp || objRight < enLeft)) collide = true;

    if (collide){
        if (objUp >= enDown && pos.y < rhs.pos.y) coll.top = true;
        if (objLeft <= enRight && pos.x > rhs.pos.x) coll.left = true;
        if (objDown <= enUp && pos.y > rhs.pos.y) coll.bottom = true;
        if (objRight >= enLeft && pos.x < rhs.pos.x) coll.right = true;
    } else coll.reset();

    if (coll.left || coll.right){
        float pen = fabs(fabs(pos.x - rhs.pos.x) - shape.x / 2 - rhs.shape.x / 2);
        if (coll.left) pos.x += pen + 0.0001;
        else pos.x -= pen - 0.0001;
    } else if (coll.top || coll.bottom){
        float pen = fabs(fabs(pos.y - rhs.pos.y) - shape.y / 2 - rhs.shape.y / 2);
        if (coll.bottom) pos.y += pen + 0.0001;
        else pos.y -= pen - 0.0001;
    }

    return collide;
}

void Object::satPoints(){
    points.clear();

    // shape is taken care of in modelMatrix (?)
    points.push_back(glm::vec3(-0.5 * shape.x, -0.5 * shape.y, 0));
    points.push_back(glm::vec3(0.5 * shape.x, -0.5 * shape.y, 0));
    points.push_back(glm::vec3(0.5 * shape.x, 0.5 * shape.y, 0));
    points.push_back(glm::vec3(-0.5 * shape.x, 0.5 * shape.y, 0));
}


bool Object::satCollide(const Object& rhs){
    coll.reset();

    std::pair<float,float> penetration;
    std::vector<std::pair<float,float>> e1Points;
    std::vector<std::pair<float,float>> e2Points;

    for (size_t i = 0; i < points.size(); i++){
        glm::vec3 point = modelMatrix * points[i];
        e1Points.push_back(std::make_pair(point.x, point.y));
    }

    for (size_t i = 0; i < rhs.points.size(); i++){
        glm::vec3 point = rhs.modelMatrix * rhs.points[i];
        e2Points.push_back(std::make_pair(point.x, point.y));
    }

    // std::cout << coll;

    if (CheckSATCollision(e1Points, e2Points, penetration)){
        // std::cout << penetration.first << " " << penetration.second << std::endl;
        if (penetration.first == 0 && penetration.second == 0) {
            return false;
        }

        if (penetration.first > 0) coll.left = true;
        if (penetration.first < 0) coll.right = true;
        if (penetration.second > 0) coll.bottom = true;
        if (penetration.second < 0) coll.top = true;

        // std::cout << coll;

        pos.x += penetration.first;
        pos.y += penetration.second;

        return true;
    }

    return false;
}


void Object::setScale(float size){
    this->scale = size;
    this->shape *= size;
}

void Object::setShape(const glm::vec3& shape){
    this->scale = 1.0;

    float w = shape.x / shape.y;
    float h = 1.0;
    vertices = {
        -0.5f * w, -0.5f * h,
        0.5f * w, -0.5f * h,
        0.5f * w, 0.5f * h,
        -0.5f * w, -0.5f * h,
        0.5f * w, 0.5f * h,
        -0.5f * w, 0.5f * h ,
    };
    this->shape.x = w;
    this->shape.y = h;
}

void Object::setRotate(float rot){
    this->rotate = rot;
}

void Object::setPos(const glm::vec3& pos){
    this->pos = pos;
}

void Object::setPos(float x, float y){
    this->pos.x = x;
    this->pos.y = y;
}

void Object::setProject(float proj){
    projectionMatrix.SetOrthoProjection(-screenWidth * proj, screenWidth * proj, -screenHeight * proj, screenHeight * proj, -1.0f, 1.0f);
}



void Object::setData(const XMLData& data){
    // assume the shape of sheetsprite is 1024 * 512
    float u = data.x / 1024.0,
          v = data.y / 512.0,
          width = data.width / 1024.0,
          height = data.height / 512.0;

    // rescale the image so that the max edge length is 1
    float w = (data.width / data.height < 1) ? data.width / data.height : 1.0,
          h = (data.width / data.height < 1) ? 1.0 : data.height / data.width;

    vertices = {
      -0.5f * w, -0.5f * h,
      0.5f * w, -0.5f * h,
      0.5f * w, 0.5f * h,
      -0.5f * w, -0.5f * h,
      0.5f * w, 0.5f * h,
      -0.5f * w, 0.5f * h ,
    };


    texCoords = {
        u, v+height,
        u+width, v+height,
        u+width, v,
        u, v+height,
        u+width, v,
        u, v
    };

    shape.x = w;
    shape.y = h;

    satPoints();
}

const glm::vec3& Object::getCenter() const {
    return pos;
}







bool TestSATSeparationForEdge(float edgeX, float edgeY, const std::vector<std::pair<float,float>> &points1, const std::vector<std::pair<float,float>> &points2, std::pair<float,float> &penetration) {
	float normalX = -edgeY;
	float normalY = edgeX;
	float len = sqrtf(normalX*normalX + normalY*normalY);
	normalX /= len;
	normalY /= len;

	std::vector<float> e1Projected;
	std::vector<float> e2Projected;

	for(int i=0; i < points1.size(); i++) {
		e1Projected.push_back(points1[i].first * normalX + points1[i].second * normalY);
	}
	for(int i=0; i < points2.size(); i++) {
		e2Projected.push_back(points2[i].first * normalX + points2[i].second * normalY);
	}

	std::sort(e1Projected.begin(), e1Projected.end());
	std::sort(e2Projected.begin(), e2Projected.end());

	float e1Min = e1Projected[0];
	float e1Max = e1Projected[e1Projected.size()-1];
	float e2Min = e2Projected[0];
	float e2Max = e2Projected[e2Projected.size()-1];

	float e1Width = fabs(e1Max-e1Min);
	float e2Width = fabs(e2Max-e2Min);
	float e1Center = e1Min + (e1Width/2.0);
	float e2Center = e2Min + (e2Width/2.0);
	float dist = fabs(e1Center-e2Center);
	float p = dist - ((e1Width+e2Width)/2.0);

	if(p >= 0) {
		return false;
	}

	float penetrationMin1 = e1Max - e2Min;
	float penetrationMin2 = e2Max - e1Min;

	float penetrationAmount = penetrationMin1;
	if(penetrationMin2 < penetrationAmount) {
		penetrationAmount = penetrationMin2;
	}

	penetration.first = normalX * penetrationAmount;
	penetration.second = normalY * penetrationAmount;

	return true;
}

bool PenetrationSort(const std::pair<float,float> &p1, const std::pair<float,float> &p2) {
	return sqrtf(p1.first*p1.first + p1.second*p1.second) < sqrtf(p2.first*p2.first + p2.second*p2.second);
}

bool CheckSATCollision(const std::vector<std::pair<float,float>> &e1Points, const std::vector<std::pair<float,float>> &e2Points, std::pair<float,float> &penetration) {
	std::vector<std::pair<float,float>> penetrations;
	for(int i=0; i < e1Points.size(); i++) {
		float edgeX, edgeY;

		if(i == e1Points.size()-1) {
			edgeX = e1Points[0].first - e1Points[i].first;
			edgeY = e1Points[0].second - e1Points[i].second;
		} else {
			edgeX = e1Points[i+1].first - e1Points[i].first;
			edgeY = e1Points[i+1].second - e1Points[i].second;
		}
		std::pair<float,float> penetration;
		bool result = TestSATSeparationForEdge(edgeX, edgeY, e1Points, e2Points, penetration);
		if(!result) {
			return false;
		}
		penetrations.push_back(penetration);
	}
	for(int i=0; i < e2Points.size(); i++) {
		float edgeX, edgeY;

		if(i == e2Points.size()-1) {
			edgeX = e2Points[0].first - e2Points[i].first;
			edgeY = e2Points[0].second - e2Points[i].second;
		} else {
			edgeX = e2Points[i+1].first - e2Points[i].first;
			edgeY = e2Points[i+1].second - e2Points[i].second;
		}
		std::pair<float,float> penetration;
		bool result = TestSATSeparationForEdge(edgeX, edgeY, e1Points, e2Points, penetration);

		if(!result) {
			return false;
		}
		penetrations.push_back(penetration);
	}

	std::sort(penetrations.begin(), penetrations.end(), PenetrationSort);
	penetration = penetrations[0];

	std::pair<float,float> e1Center;
	for(int i=0; i < e1Points.size(); i++) {
		e1Center.first += e1Points[i].first;
		e1Center.second += e1Points[i].second;
	}
	e1Center.first /= (float)e1Points.size();
	e1Center.second /= (float)e1Points.size();

	std::pair<float,float> e2Center;
	for(int i=0; i < e2Points.size(); i++) {
		e2Center.first += e2Points[i].first;
		e2Center.second += e2Points[i].second;
	}
	e2Center.first /= (float)e2Points.size();
	e2Center.second /= (float)e2Points.size();

	std::pair<float,float> ba;
	ba.first = e1Center.first - e2Center.first;
	ba.second = e1Center.second - e2Center.second;

	if( (penetration.first * ba.first) + (penetration.second * ba.second) < 0.0f) {
		penetration.first *= -1.0f;
		penetration.second *= -1.0f;
	}

	return true;
}
