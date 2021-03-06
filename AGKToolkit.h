#pragma once
#include <agk.h>
#include <string>
#include <vector>
#include <math.h>
#include <sstream>
#include <functional>

#include "rapidxml.hpp"

using namespace std;
using namespace rapidxml;


class Animation {
private:
	string name;
	int height;
	int width;
	int framecount;
	float fps;
	bool loop;
	vector<int> imageIDs;
public:
	Animation() {
		name = "";
		height = -1;
		width = -1;
		framecount = -1;
		fps = 10;
		loop = true;
	}
	Animation(string animationName) {
		setName(animationName);
		height = -1;
		width = -1;
		framecount = -1;
		fps = 10;
		loop = true;
	}
	void setName(string animationName) {
		name = animationName;
	}
	string getName() {
		return name;
	}
	void setLoop(bool value) {
		loop = value;
	}
	bool getLoop() {
		return loop;
	}
	int getWidth() {
		return width;
	}
	int getHeight() {
		return height;
	}
	int getFrameCount() {
		return framecount;
	}
	float getFPS() {
		return fps;
	}
	vector<int> getImageIDs() {
		return imageIDs;
	}
	void destroy() {
		for (int i = 0; i < imageIDs.size(); i++) {
			imageIDs.erase(imageIDs.begin() + i);
			i--;
		}
	}
	void addFrame(string filename) {
		imageIDs.push_back(agk::LoadImage(filename.c_str()));
	}
	void setupSingleFrameAnimation(string filename, int frameWidth, int frameHeight, float newFPS = 10) {
		addFrame(filename);
		height = frameHeight;
		width = frameWidth;
		setFPS(newFPS);
		framecount = (agk::GetImageWidth(imageIDs[0]) / width)*(agk::GetImageHeight(imageIDs[0]) / height);
	}
	void setFPS(float newFPS) {
		fps = newFPS;
	}
};

class Entity {
private:
	int spriteID;
	string currentAnimation;
	bool loopingAnimation;
	vector<Animation> animations;
public:
	Entity() {
		spriteID = -1;
		currentAnimation = "";
		loopingAnimation = false;
	}
	Entity(string imageFilename) {
		spriteID = -1;
		currentAnimation = "";
		loopingAnimation = false;
		loadImage(imageFilename);
	}
	Entity(string imageFilename, float x, float y) {
		spriteID = -1;
		currentAnimation = "";
		loopingAnimation = false;
		loadImage(imageFilename);
		setPos(x, y);
	}
	~Entity() {

	}
	void setPhysics(bool value, int mode=1/*1=static, 2=dynamic, 3=kinematic*/) {
		if (value) {
			agk::SetSpritePhysicsOn(spriteID, mode);
		}
		else {
			agk::SetSpritePhysicsOff(spriteID);
		}
	}
	void setSpriteShapeCircle() {
		agk::SetSpriteShape(spriteID, 1);
	}
	void setSpriteShapeSquare() {
		agk::SetSpriteShape(spriteID, 2);
	}
	void setSpriteShapePolygon() {
		agk::SetSpriteShape(spriteID, 3);
	}
	void setSpriteShapeSquare(int x, int y, int x2, int y2) {
		agk::SetSpriteShapeBox(spriteID, x, y, x2, y2);
	}
	void addAnimation(Animation anim) {
		animations.push_back(anim);
	}
	void setFixed(bool value) {
		agk::FixSpriteToScreen(spriteID, (int)value);
	}
	void setDepth(int value) {
		agk::SetSpriteDepth(spriteID, value);
	}
	void setAnimation(string name) {
		for (int i = 0; i < animations.size(); i++) {
			if (name == animations[i].getName()) {
				agk::ClearSpriteAnimationFrames(spriteID);
				if (animations[i].getImageIDs().size() == 1) {
					agk::SetSpriteImage(spriteID, animations[i].getImageIDs()[0]);
					agk::SetSpriteAnimation(spriteID, animations[i].getWidth(), animations[i].getHeight(), animations[i].getFrameCount());
				}
				else {
					for (int x = 0; x < animations[i].getImageIDs().size(); x++) {
						agk::AddSpriteAnimationFrame(spriteID, animations[i].getImageIDs()[x]);
					}
				}
				currentAnimation = animations[i].getName();
				loopingAnimation = animations[i].getLoop();
				agk::PlaySprite(spriteID, animations[i].getFPS(), animations[i].getLoop());
				break;
			}
		}
	}
	void stopAnimation() {
		agk::StopSprite(spriteID);
	}
	void resumeAnimation() {
		agk::ResumeSprite(spriteID);
	}
	bool getAnimationLooping() {
		return loopingAnimation;
	}
	string getAnimationPlaying() {
		return currentAnimation;
	}
	bool isAnimationPlaying() {
		return (bool)agk::GetSpritePlaying(spriteID);
	}
	void destroy() {
		agk::DeleteSprite(spriteID);
	}
	void loadImage(string imageFilename) {
		if (spriteID != -1) {
			agk::SetSpriteImage(spriteID, agk::LoadImage(imageFilename.c_str()));
		}
		else {
			spriteID = agk::CreateSprite(agk::LoadImage(imageFilename.c_str()));
		}
	}
	void setImage(int imageID) {
		if (spriteID != -1) {
			agk::SetSpriteImage(spriteID, imageID);
		}
		else {
			spriteID = agk::CreateSprite(imageID);
		}
	}
	void setColor(int r, int g, int b, int a) {
		agk::SetSpriteColor(spriteID, r, g, b, a);
	}
	int getColorRed() {
		return agk::GetSpriteColorRed(spriteID);
	}
	int getColorGreen() {
		return agk::GetSpriteColorGreen(spriteID);
	}
	int getColorBlue() {
		return agk::GetSpriteColorBlue(spriteID);
	}
	int getColorAlpha() {
		return agk::GetSpriteColorAlpha(spriteID);
	}
	bool compare(Entity other) {
		if (spriteID == other.getID()) {
			return true;
		}
		else {
			return false;
		}
	}
	int getID() {
		return spriteID;
	}
	void setX(float x) {
		agk::SetSpriteX(spriteID, x);
	}
	void setY(float y) {
		agk::SetSpriteY(spriteID, y);
	}
	void setPos(float x, float y) {
		setX(x);
		setY(y);
	}
	float getX() {
		return agk::GetSpriteX(spriteID);
	}
	float getY() {
		return agk::GetSpriteY(spriteID);
	}
	void setSize(float w, float h) {
		agk::SetSpriteSize(spriteID, w, h);
	}
	float getW() {
		return agk::GetSpriteWidth(spriteID);
	}
	float getH() {
		return agk::GetSpriteHeight(spriteID);
	}
	bool isVisible() {
		return agk::GetSpriteVisible(spriteID);
	}
	void setVisible(bool value) {
		if (value) {
			agk::SetSpriteVisible(spriteID, 1);
		}
		else {
			agk::SetSpriteVisible(spriteID, 0);
		}
	}
	bool isActive() {
		return agk::GetSpriteActive(spriteID);
	}
	void setActive(bool value) {
		if (value) {
			agk::SetSpriteActive(spriteID, 1);
		}
		else {
			agk::SetSpriteActive(spriteID, 0);
		}
	}
	float getAngle() {
		return agk::GetSpriteAngle(spriteID);
	}
	void setAngle(float value) {
		agk::SetSpriteAngle(spriteID, value);
	}
	bool collision(Entity other) {
		return agk::GetSpriteCollision(spriteID, other.getID());
	}
	bool physicsCollision(Entity other) {
		return agk::GetPhysicsCollision(spriteID, other.getID());
	}

};

class Movement {
private:
	bool remove;
	int style; //0=move to point, 1=move towards point
	float speedx;
	float speedy;
	float speed;
	float endx;
	float endy;
	int tweentype;
	int tweenID;
public:
	Movement() {
		remove = true;
		style = 0; //0=move to point, 1=move towards point
		speedx = 0;
		speedy = 0;
		speed = 0;
		endx = 0;
		endy = 0;
		tweentype = 0;
		tweenID = -1;
	}
	~Movement() {

	}
	void set(bool shouldRemove, int objectStyle, float ispeedx, float ispeedy, float ispeed, float iendx, float iendy, int itweentype, int itweenID) {
		remove = shouldRemove;
		style = objectStyle;
		speedx = ispeedx;
		speedy = ispeedy;
		speed = ispeed;
		endx = iendx;
		endy = iendy;
		tweentype = itweentype;
		tweenID = itweenID;
	}
	bool getRemove() {
		return remove;
	}
	int getStyle() {
		return style;
	}
	float getSpeedX() {
		return speedx;
	}
	void setSpeedX(float value) {
		speedx = value;
	}
	void setSpeedY(float value) {
		speedy = value;
	}
	float getSpeedY() {
		return speedy;
	}
	float getSpeed() {
		return speed;
	}
	float getEndX() {
		return endx;
	}
	float getEndY() {
		return endy;
	}
	int getTweenType() {
		return tweentype;
	}
	int getTweenID() {
		return tweenID;
	}
	void setTweenID(int value) {
		tweenID = value;
	}

};

class DynamicEntity : public Entity {
private:
	vector<Movement> movementList;
	vector<Entity> collisionList;
	bool pause;
	int rampmove;
public:
	DynamicEntity() : Entity() {
		pause = false;
		rampmove = 0;
	}
	DynamicEntity(string imageFilename) : Entity(imageFilename) {
		pause = false;
		rampmove = 0;
	}
	DynamicEntity(string imageFilename, float x, float y) : Entity(imageFilename, x, y) {
		pause = false;
		rampmove = 0;
	}
	void addToEntityCollisionList(vector<Entity> newEntries) {
		for (int i = 0; i < newEntries.size(); i++) {
			collisionList.push_back(newEntries[i]);
		}
	}
	void eraseEntityCollisionList() {
		collisionList.erase(collisionList.begin(), collisionList.end());
	}
	void removeFromEntityCollisionList(vector<Entity> entriesToRemove) {
		for (int i = 0; i < collisionList.size(); i++) {
			for (int x = 0; x < entriesToRemove.size(); x++) {
				if (collisionList[i].compare(entriesToRemove[x])) {
					collisionList.erase(collisionList.begin() + i);
					i--;
					entriesToRemove.erase(entriesToRemove.begin() + x);
					break;
				}
			}
		}
	}
	void update(float delta) {
		if (movementList.size() > 0) {
			if (!pause) {
				if (movementList.at(0).getStyle() == 0) {
					if (movementList.at(0).getTweenID() == -1) {
						float deltax = movementList.at(0).getEndX() - getX();
						float deltay = movementList.at(0).getEndY() - getY();
						float distance = agk::Sqrt(agk::Pow(deltax, 2) + agk::Pow(deltay, 2));
						float time = distance / movementList.at(0).getSpeed();
						movementList.at(0).setTweenID(agk::CreateTweenCustom(time));
						agk::SetTweenCustomFloat1(movementList.at(0).getTweenID(), getX(), movementList.at(0).getEndX(), movementList.at(0).getTweenType());
						agk::SetTweenCustomFloat2(movementList.at(0).getTweenID(), getY(), movementList.at(0).getEndY(), movementList.at(0).getTweenType());
						agk::PlayTweenCustom(movementList.at(0).getTweenID(), 0);
					}
					else {
						if (agk::GetTweenCustomPlaying(movementList.at(0).getTweenID()) == 1) {
							agk::UpdateTweenCustom(movementList.at(0).getTweenID(), delta);
							setPos(agk::GetTweenCustomFloat1(movementList.at(0).getTweenID()), agk::GetTweenCustomFloat2(movementList.at(0).getTweenID()));
						}
						else {
							agk::DeleteTween(movementList.at(0).getTweenID());
							movementList.at(0).setTweenID(-1);
							if (!movementList.at(0).getRemove()) {
								Movement current = movementList[0];
								movementList.push_back(current);
							}
							
							for (int i = 0; i < movementList.size() - 1; i++) {
								movementList[i] = movementList[i + 1];
							}
							movementList.pop_back();

						}
					}
				}
				else if (movementList.at(0).getStyle() == 1) {
					if (movementList.at(0).getSpeedX() == -1) {
						float deltax = movementList.at(0).getEndX() - getX();
						float deltay = movementList.at(0).getEndY() - getY();
						float distance = agk::Sqrt(agk::Pow(deltax, 2) + agk::Pow(deltay, 2));
						float time = distance / movementList.at(0).getSpeed();
						movementList.at(0).setSpeedX(deltax / time);
						movementList.at(0).setSpeedY(deltay / time);
					}
					else {
						moveT(movementList.at(0).getSpeedX(), movementList.at(0).getSpeedY(), delta);
					}
				}
			}
		}
	}
	bool isMoving() {
		return movementList.size() > 0;
	}
	bool isPaused() {
		return pause;
	}
	void setPause(bool value) {
		pause = value;
	}

	void move(float dx, float dy) {
		setPos(getX() + dx, getY() + dy);
		if (collisionList.size() > 0) {
			for (int i = 0; i < collisionList.size(); i++) {
				if (collision(collisionList[i])) {
					setPos(getX() - dx, getY() - dy);
					break;
				}
			}
		}
	}
	void moveT(float xPixelsPerSecond, float yPixelsPerSecond, float delta) {
		setPos(getX() + (xPixelsPerSecond*delta), getY() + (yPixelsPerSecond*delta));
		if (collisionList.size() > 0) {
			for (int i = 0; i < collisionList.size(); i++) {
				if (collision(collisionList[i])) {
					setPos(getX() - (xPixelsPerSecond*delta), getY() - (yPixelsPerSecond*delta));
					break;
				}
			}
		}
	}
	void moveAtAngle(float distance, float angle) {
		setPos(getX() + (agk::Cos(angle)*distance), getY() + (agk::Sin(angle)*distance));
		if (collisionList.size() > 0) {
			for (int i = 0; i < collisionList.size(); i++) {
				if (collision(collisionList[i])) {
					setPos(getX() - (agk::Cos(angle)*distance), getY() - (agk::Sin(angle)*distance));
					break;
				}
			}
		}
	}
	void moveAtAngleT(float distancePerSecond, float angle, float delta) {
		setPos(getX() + (agk::Cos(angle)*(distancePerSecond*delta)), getY() + (agk::Sin(angle)*(distancePerSecond*delta)));
		if (collisionList.size() > 0) {
			for (int i = 0; i < collisionList.size(); i++) {
				if (collision(collisionList[i])) {
					setPos(getX() - (agk::Cos(angle)*(distancePerSecond*delta)), getY() - (agk::Sin(angle)*(distancePerSecond*delta)));
					break;
				}
			}
		}
	}
	void rotate(float da) {
		setAngle(getAngle() + da);
	}
	void rotateT(float degreesPerSecond, float delta) {
		setAngle(getAngle() + (degreesPerSecond*delta));
	}
	void moveToPoint(float x, float y, float pixelsPerSecond, bool remove = true, int tween = 0) {
		movementList.push_back(Movement());
		movementList.at(movementList.size() - 1).set(remove, 0, -1, -1, pixelsPerSecond, x, y, tween, -1);
	}
	void moveTowardsPoint(float x, float y, float pixelsPerSecond) {
		movementList.push_back(Movement());
		movementList.at(movementList.size() - 1).set(false, 1, -1, -1, pixelsPerSecond, x, y, -1, -1);
	}
	void setRampMovePixels(int amount) {

	}
};

class Timer {
private:
	float seconds;
	bool running;
	bool countDown;
	float startTime;
	string intToString(int value) {
		stringstream buffer;
		buffer << value;
		string result = "";
		buffer >> result;
		return result;
	}
public:
	Timer() {
		seconds = 0;
		startTime = 0;
		running = false;
		countDown = true;
	}
	Timer(float time, bool shouldCountDown=true) {
		seconds = time;
		startTime = time;
		running = false;
		countDown = shouldCountDown;
	}
	void set(float time) {
		seconds = time;
		startTime = time;
	}
	void start() {
		running = true;
	}
	void stop() {
		running = false;
	}
	void reset() {
		seconds = startTime;
	}
	void update(float delta) {
		if (running) {
			if (countDown) {
				seconds -= delta;
			}
			else {
				seconds += delta;
			}
		}
	}
	float getTime() {
		return seconds;
	}
	string getFormatedTime() {
		string result = "";
		float remainingTime = seconds;
		int hours = remainingTime / 3600;
		remainingTime -= hours * 3600;
		int minutes = remainingTime / 60;
		remainingTime -= minutes * 60;
		int seconds = remainingTime;
		remainingTime -= seconds;
		int decimal = remainingTime * 10;

		if (hours > 0) {
			if (hours < 10) {
				result += "0" + intToString(hours) + ":";
			}
			else {
				result += intToString(hours) + ":";
			}
		}
		if (minutes > 0 || hours > 0) {
			if (minutes == 0) {
				result += "00:";
			}
			else if (minutes < 10) {
				result += "0" + intToString(minutes) + ":";
			}
			else {
				result += intToString(minutes) + ":";
			}
		}
		if (seconds == 0) {
			result += "00.";
		}
		else if (seconds < 10) {
			result += "0" + intToString(seconds) + ".";
		}
		else {
			result += intToString(seconds) + ".";
		}
		result += intToString(decimal);

		return result;
	}
	bool check() {
		return !(seconds > 0);
	}

};

class Property {
private:
	string name;
	string stringValue;
	int intValue;
	float floatValue;
	bool boolValue;
public:
	Property() {
		name = "";
		intValue = 0;
		boolValue = false;
		floatValue = 0;
		stringValue = "";
	}
	Property(string newName, int setValue) {
		name = newName;
		intValue = setValue;
		floatValue = 0;
		stringValue = "";
		boolValue = false;
	}
	Property(string newName, float value) {
		name = newName;
		intValue = 0;
		floatValue = value;
		stringValue = "";
		boolValue = false;
	}
	Property(string newName, string value) {
		name = newName;
		intValue = 0;
		floatValue = 0;
		boolValue = false;
		stringValue = value;
	}
	Property(string newName, bool value) {
		name = newName;
		intValue = 0;
		floatValue = 0;
		boolValue = value;
		stringValue = "";
	}
	string getName() {
		return name;
	}
	int getI() {
		return intValue;
	}
	float getF() {
		return floatValue;
	}
	bool getB() {
		return boolValue;
	}
	string getS() {
		return stringValue;
	}
};

class PropertyGroup {
private:
	vector<Property> values;
public:
	void addBool(string name, bool value) {
		values.emplace_back(name, value);
	}
	void addString(string name, string value) {
		values.emplace_back(name, value);
	}
	void addInteger(string name, int value) {
		values.emplace_back(name, value);
	}
	void addFloat(string name, float value) {
		values.emplace_back(name, value);
	}
	int getIndex(string name) {
		int value=0;
		for (int i = 0; i < values.size(); i++) {
			if (values[i].getName() == name) {
				value = i;
				break;
			}
		}
		return value;
	}
	int getI(string name) {
		return values[getIndex(name)].getI();
	}
	bool getB(string name) {
		return values[getIndex(name)].getB();
	}
	float getF(string name) {
		return values[getIndex(name)].getF();
	}
	string getS(string name) {
		return values[getIndex(name)].getS();
	}
};

struct Tileset {
	int firstgid;
	int lastgid;
	string name;
	float tilewidth;
	float tileheight;
	int tilecount;
	int columns;
	int imageID;
	PropertyGroup properties;
};

struct Layer {
	string name;
	int width;
	int height;
	vector<Entity> sprites;
	PropertyGroup properties;
};

struct ImageLayer {
	int imageID;
	string name;
	float width;
	float height;
	float offsetx;
	float offsety;
	PropertyGroup properties;
};

struct Point {
	float x;
	float y;
};

struct MapObject {
	int id;
	string name;
	string otype;
	float x;
	float y;
	float width;
	float height;
	vector<Point> points;
	PropertyGroup properties;
};

class TmxMap {
private:
	vector<Layer> layers;
	vector<Tileset> tilesets;
	vector<ImageLayer> imageLayers;
	vector<MapObject> mapobjects;
	PropertyGroup properties;
	bool visible;
	bool active;
	float mapX;
	float mapY;
public:
	TmxMap() {
		visible = false;
		active = false;
		mapX = 0;
		mapY = 0;
	}
	TmxMap(string filename) {
		loadTmxMap(filename);
	}
	PropertyGroup getProperties() {
		return properties;
	}
	PropertyGroup getLayerProperties(string name) {
		PropertyGroup result = PropertyGroup();
		for (int i = 0; i < layers.size(); i++) {
			if (layers[i].name == name) {
				result = layers[i].properties;
				break;
			}
		}
		return result;
	}
	PropertyGroup getTilesetProperties(string name) {
		PropertyGroup result = PropertyGroup();
		for (int i = 0; i < tilesets.size(); i++) {
			if (tilesets[i].name == name) {
				result = tilesets[i].properties;
				break;
			}
		}
		return result;
	}
	PropertyGroup getImageLayerProperties(string name) {
		PropertyGroup result = PropertyGroup();
		for (int i = 0; i < imageLayers.size(); i++) {
			if (imageLayers[i].name == name) {
				result = imageLayers[i].properties;
				break;
			}
		}
		return result;
	}
	PropertyGroup getObjectProperties(string name) {
		PropertyGroup result = PropertyGroup();
		for (int i = 0; i < mapobjects.size(); i++) {
			if (mapobjects[i].name == name) {
				result = mapobjects[i].properties;
				break;
			}
		}
		return result;
	}
	void setLayerOpacity(string name, int value) {
		vector<Entity> tiles = getTilesinLayer(name);
		if (value >= 0 && value<=255) {
			for (int i = 0; i < tiles.size(); i++) {
				agk::SetSpriteColorAlpha(tiles[i].getID(), value);
			}
		}
	}
	void processProperties(string dir, xml_node<> *parent, PropertyGroup &object) {
		if (parent->first_node("properties") != 0) {
			xml_node<> *child = parent->first_node("properties");
			if (child->first_node("property") != 0) {
				for (xml_node<> *propertyNode = child->first_node("property"); propertyNode; propertyNode = propertyNode->next_sibling("property")) {
					string name = propertyNode->first_attribute("name")->value();
					string type = "string";
					if (propertyNode->first_attribute("type") != 0) {
						type=propertyNode->first_attribute("type")->value();
					}
					if (type == "string") {
						string value = propertyNode->first_attribute("value")->value();
						object.addString(name, value);
					}
					else if (type == "int") {
						int value = atoi(propertyNode->first_attribute("value")->value());
						object.addInteger(name, value);
					}
					else if (type == "float") {
						float value = stof(propertyNode->first_attribute("value")->value());
						object.addFloat(name, value);
					}
					else if (type == "bool") {
						if (string(propertyNode->first_attribute("value")->value()) == "true") {
							object.addBool(name, true);
						}
						else {
							object.addBool(name, false);
						}
					}
					else if (type == "file") {
						string rel = propertyNode->first_attribute("value")->value();
						object.addString(name, getPathFromRoot(dir, rel));
					}
				}
			}
		}
	}
	bool getVisible() {
		return visible;
	}
	bool getActive() {
		return active;
	}
	float getMapX() {
		return mapX;
	}
	float getMapY() {
		return mapY;
	}
	void setVisible(bool value) {
		for (int i = 0; i < layers.size(); i++) {
			for (int x = 0; x < layers[i].sprites.size(); x++) {
				layers[i].sprites[x].setVisible(value);
			}
		}
		for (int i = 0; i < imageLayers.size(); i++) {
			agk::SetSpriteVisible(imageLayers[i].imageID, (int)value);
		}
	}
	void setActive(bool value) {
		for (int i = 0; i < layers.size(); i++) {
			for (int x = 0; x < layers[i].sprites.size(); x++) {
				layers[i].sprites[x].setActive(value);
			}
		}
		for (int i = 0; i < imageLayers.size(); i++) {
			agk::SetSpriteActive(imageLayers[i].imageID, (int)value);
		}
	}
	void moveMap(float newX, float newY) {
		for (int i = 0; i < layers.size(); i++) {
			for (int x = 0; x < layers[i].sprites.size(); x++) {
				float offsetX = layers[i].sprites[x].getX() - mapX;
				float offsetY = layers[i].sprites[x].getY() - mapY;
				layers[i].sprites[x].setPos(newX + offsetX, newY + offsetY);
			}
		}
		for (int i = 0; i < imageLayers.size(); i++) {
			float offsetX = agk::GetSpriteX(imageLayers[i].imageID) - mapX;
			float offsetY = agk::GetSpriteY(imageLayers[i].imageID) - mapY;
			agk::SetSpriteX(imageLayers[i].imageID, newX + offsetX);
			agk::SetSpriteY(imageLayers[i].imageID, newY + offsetY);
		}
		for (int i = 0; i < mapobjects.size(); i++) {
			float offsetX = mapobjects[i].x - mapX;
			float offsetY = mapobjects[i].y - mapY;
			mapobjects[i].x = newX + offsetX;
			mapobjects[i].y = newY + offsetY;
			for (int z = 0; z < mapobjects[i].points.size(); z++) {
				float offX = mapobjects[i].points[z].x - mapX;
				float offY = mapobjects[i].points[z].y - mapY;
				mapobjects[i].points[z].x = newX + offX;
				mapobjects[i].points[z].y = newY + offY;
			}
		}

		mapX = newX;
		mapY = newY;
	}
	string getPathFromRoot(string pathTo, string pathFrom) {
		string pathToFile = pathTo;
		string pathFromFile = pathFrom;
		size_t begin = 0;
		size_t length = 3;
		while (pathFromFile.find("../")!=string::npos) {
			pathToFile.erase(pathToFile.begin() + pathToFile.find_last_of('/'), pathToFile.end());
			pathFromFile.erase(begin, length);
		}
		return pathToFile + "/" + pathFromFile;
	}
	void loadTmxMap(string filename, float offsetX = 0, float offsetY = 0) {
		mapX = offsetX;
		mapY = offsetY;


		//Load the file into memory
		string data = "";
		string line = "";
		int tmxDoc = agk::OpenToRead(filename.c_str());
		while (agk::FileEOF(tmxDoc) == 0) {
			line=agk::ReadLine(tmxDoc);
			data += line;
		}
		agk::CloseFile(tmxDoc);
		//End loading file into memory
		
		//Begin parsing file
		string path = "";
		size_t n = count(filename.begin(), filename.end(), '/');
		if (n > 0) {
			path = filename;
			path.erase(path.begin() + path.find_last_of('/'), path.end());
		}


		xml_document<> doc;
		doc.parse<0>(&data[0]);
		xml_node<> *root = doc.first_node("map");

		if (root != 0) {

			processProperties(path, root, properties);

			if (root->first_node("tileset") != 0) {

				for (xml_node<> *tilesetNode = root->first_node("tileset"); tilesetNode; tilesetNode = tilesetNode->next_sibling("tileset")) {

					tilesets.emplace_back();

					tilesets[tilesets.size() - 1].firstgid = atoi(tilesetNode->first_attribute("firstgid")->value());
					tilesets[tilesets.size() - 1].name = tilesetNode->first_attribute("name")->value();
					tilesets[tilesets.size() - 1].tilewidth = atoi(tilesetNode->first_attribute("tilewidth")->value());
					tilesets[tilesets.size() - 1].tileheight = atoi(tilesetNode->first_attribute("tileheight")->value());
					tilesets[tilesets.size() - 1].tilecount = atoi(tilesetNode->first_attribute("tilecount")->value());
					tilesets[tilesets.size() - 1].columns = atoi(tilesetNode->first_attribute("columns")->value());

					tilesets[tilesets.size() - 1].imageID = agk::LoadImage(getPathFromRoot(path,tilesetNode->first_node("image")->first_attribute("source")->value()).c_str());
					tilesets[tilesets.size() - 1].lastgid = tilesets[tilesets.size() - 1].firstgid + (tilesets[tilesets.size() - 1].tilecount - 1);

					processProperties(path, tilesetNode, tilesets[tilesets.size() - 1].properties);

				}

			}

			if (root->first_node("imagelayer") != 0) {

				for (xml_node<> *imageNode = root->first_node("imagelayer"); imageNode; imageNode = imageNode->next_sibling("imagelayer")) {

					imageLayers.emplace_back();

					imageLayers[imageLayers.size() - 1].name = imageNode->first_attribute("name")->value();
					imageLayers[imageLayers.size() - 1].offsetx = atoi(imageNode->first_attribute("offsetx")->value());
					imageLayers[imageLayers.size() - 1].offsety = atoi(imageNode->first_attribute("offsety")->value());

					imageLayers[imageLayers.size() - 1].imageID = agk::CreateSprite(agk::LoadImage(getPathFromRoot(path,imageNode->first_node("image")->first_attribute("source")->value()).c_str()));
					imageLayers[imageLayers.size() - 1].width = atoi(imageNode->first_node("image")->first_attribute("width")->value());
					imageLayers[imageLayers.size() - 1].height = atoi(imageNode->first_node("image")->first_attribute("height")->value());
					agk::SetSpriteSize(imageLayers[imageLayers.size() - 1].imageID, imageLayers[imageLayers.size() - 1].width, imageLayers[imageLayers.size() - 1].height);
					agk::SetSpritePosition(imageLayers[imageLayers.size() - 1].imageID, imageLayers[imageLayers.size() - 1].offsetx + offsetX, imageLayers[imageLayers.size() - 1].offsety + offsetY);

					processProperties(path, imageNode, imageLayers[imageLayers.size() - 1].properties);

				}

			}

			if (root->first_node("layer") != 0) {

				for (xml_node<> *layerNode = root->first_node("layer"); layerNode; layerNode = layerNode->next_sibling("layer")) {

					layers.emplace_back();

					layers[layers.size() - 1].name = layerNode->first_attribute("name")->value();
					layers[layers.size() - 1].width = atoi(layerNode->first_attribute("width")->value());
					layers[layers.size() - 1].height = atoi(layerNode->first_attribute("height")->value());

					processProperties(path, layerNode, layers[layers.size() - 1].properties);

					stringstream rawData;
					rawData.str(layerNode->first_node("data")->value());

					int gid = 0;
					string token = "";

					for (int h = 1; h <= layers[layers.size() - 1].height; h++) {
						for (int w = 1; w <= layers[layers.size() - 1].width; w++) {
							getline(rawData, token, ',');
							gid = atoi(token.c_str());
							if (gid != 0) {
								for (int i = 0; i < tilesets.size(); i++) {
									if (gid > tilesets[i].firstgid - 1 && gid < tilesets[i].lastgid + 1) {

										layers[layers.size() - 1].sprites.emplace_back();

										layers[layers.size() - 1].sprites[layers[layers.size() - 1].sprites.size() - 1].setImage(tilesets[i].imageID);
										agk::SetSpriteAnimation(layers[layers.size() - 1].sprites[layers[layers.size() - 1].sprites.size() - 1].getID(), tilesets[i].tilewidth, tilesets[i].tileheight, tilesets[i].tilecount);
										agk::SetSpriteFrame(layers[layers.size() - 1].sprites[layers[layers.size() - 1].sprites.size() - 1].getID(), (gid - tilesets[i].firstgid) + 1);
										layers[layers.size() - 1].sprites[layers[layers.size() - 1].sprites.size() - 1].setPos(((w - 1)*tilesets[i].tilewidth) + offsetX, ((h - 1)*tilesets[i].tileheight) + offsetY);
										agk::SetSpriteUVBorder(layers[layers.size() - 1].sprites[layers[layers.size() - 1].sprites.size() - 1].getID(), 1);

									}
								}
							}
						}
					}

					if (layerNode->first_attribute("opacity") != 0) {
						setLayerOpacity(layers[layers.size() - 1].name, (int)(atof(layerNode->first_attribute("opacity")->value()) * 255));
					}

				}

			}

			if (root->first_node("objectgroup") != 0) {

				for (xml_node<> *objectGroupNode = root->first_node("objectgroup"); objectGroupNode; objectGroupNode = objectGroupNode->next_sibling("objectgroup")) {

					if (objectGroupNode->first_node("object") != 0) {

						for (xml_node<> *objectNode = objectGroupNode->first_node("object"); objectNode; objectNode = objectNode->next_sibling("object")) {

							mapobjects.emplace_back();

							mapobjects[mapobjects.size() - 1].id = atoi(objectNode->first_attribute("id")->value());
							mapobjects[mapobjects.size() - 1].name = objectNode->first_attribute("name")->value();
							mapobjects[mapobjects.size() - 1].otype = objectNode->first_attribute("type")->value();
							mapobjects[mapobjects.size() - 1].x = atoi(objectNode->first_attribute("x")->value()) + offsetX;
							mapobjects[mapobjects.size() - 1].y = atoi(objectNode->first_attribute("y")->value()) + offsetY;
							mapobjects[mapobjects.size() - 1].width = atoi(objectNode->first_attribute("width")->value());
							mapobjects[mapobjects.size() - 1].height = atoi(objectNode->first_attribute("height")->value());

							processProperties(path, objectNode, mapobjects[mapobjects.size() - 1].properties);


							if (objectNode->first_node("polyline") != 0) {

								for (xml_node<> *polylineNode = objectNode->first_node("polyline"); polylineNode; polylineNode = polylineNode->next_sibling("polyline")) {
									stringstream rawData;
									string corToken="";
									stringstream corPair;
									string token="";
									rawData.str(polylineNode->first_attribute("points")->value());
									int relX = 0;
									int relY = 0;
									while (!rawData.eof()) {

										Point p = Point();

										getline(rawData, corToken, ' ');
										corPair.str(corToken);

										getline(corPair, token, ',');
										relX=atoi(token.c_str());

										getline(corPair, token, ',');
										relY=atoi(token.c_str());

										p.x = mapobjects[mapobjects.size() - 1].x + relX;
										p.y = mapobjects[mapobjects.size() - 1].y + relY;

										mapobjects[mapobjects.size() - 1].points.push_back(p);
									}
								}

							}

						}

					}
				}

			}

		}

		

		//End parsing file

		visible = true;
		active = true;
	}
	vector<Entity> getTilesinLayer(string name) {
		for (int i = 0; i < layers.size(); i++) {
			if (name == layers[i].name) {
				return layers[i].sprites;
			}
		}
		return vector<Entity>();
	}
	vector<MapObject> getMapObjects(string name) {
		vector<MapObject> result;
		for (int i = 0; i < mapobjects.size(); i++) {
			if (name == mapobjects[i].name) {
				result.push_back(mapobjects[i]);
			}
		}
		return result;
	}

};

class MindMap { //Class for loading and using .mm files created in FreeMind
private:
	vector<MindMap> children;
	string text;
	string style;
	string name;
	string value;
public:
	MindMap(string newText, string newStyle) {
		text = newText;
		style = newStyle;
		name = "";
		value = "";
	}
	string getText() {
		return text;
	}
	string getStyle() {
		return style;
	}
	vector<MindMap> getChildren() {
		return children;
	}
	string getName() {
		return name;
	}
	string getValue() {
		return value;
	}
	vector<MindMap> getChildrenByStyle(string value) {
		vector<MindMap> result;
		for (int i = 0; i < children.size(); i++) {
			if (value == children[i].getStyle()) {
				result.push_back(children[i]);
			}
		}
		return result;
	}
	vector<MindMap> getChildrenByText(string value) {
		vector<MindMap> result;
		for (int i = 0; i < children.size(); i++) {
			if (value == children[i].getText()) {
				result.push_back(children[i]);
			}
		}
		return result;
	}
	vector<MindMap> getChildrenByName(string value) {
		vector<MindMap> result;
		for (int i = 0; i < children.size(); i++) {
			if (value == children[i].getName()) {
				result.push_back(children[i]);
			}
		}
		return result;
	}
	vector<MindMap> getChildrenByValue(string value) {
		vector<MindMap> result;
		for (int i = 0; i < children.size(); i++) {
			if (value == children[i].getValue()) {
				result.push_back(children[i]);
			}
		}
		return result;
	}
	void parseChildren(xml_node<> *root) {
		if (root->first_node("attribute") != 0) {
			name = root->first_node("attribute")->first_attribute("NAME")->value();
			value = root->first_node("attribute")->first_attribute("VALUE")->value();
		}
		if (root->first_node("node") != 0) {
			for (xml_node<> *child = root->first_node("node"); child; child = child->next_sibling("node")) {
				children.emplace_back(child->first_attribute("TEXT")->value(), child->first_attribute("STYLE")->value());
				children.back().parseChildren(child);
			}
		}
	}
	void loadMM(string filename) {

		//Load from file into memory

		string data = "";
		string line = "";
		int mmDoc = agk::OpenToRead(filename.c_str());
		while (agk::FileEOF(mmDoc) == 0) {
			line = agk::ReadLine(mmDoc);
			data += line;
		}
		agk::CloseFile(mmDoc);

		//End loading into memory



		//Parse document

		xml_document<> doc;
		doc.parse<0>(&data[0]);
		xml_node<> *root = doc.first_node("map");

		parseChildren(root);

		//End parsing document
	}

};

class Entry {
private:
	string ss(static char* value) {
		string result = "";
		result = value;
		delete value;
		return result;
	}
	string name;
	int intValue;
	float floatValue;
	string stringValue;
public:
	Entry() {
		name = "";
		intValue = 0;
		floatValue = 0;
		stringValue = "";
	}
	Entry(string newName, int setValue) {
		name = newName;
		intValue = setValue;
		floatValue = 0;
		stringValue = "";
	}
	Entry(string newName, float value) {
		name = newName;
		intValue = 0;
		floatValue = value;
		stringValue = "";
	}
	Entry(string newName, string value) {
		name = newName;
		intValue = 0;
		floatValue = 0;
		stringValue = value;
	}
	void read(int file) {
		name = agk::ReadString(file);
		intValue = agk::ReadInteger(file);
		floatValue = agk::ReadFloat(file);
		stringValue = ss(agk::ReadString(file));
	}
	void write(int file) {
		agk::WriteString(file, name.c_str());
		agk::WriteInteger(file, intValue);
		agk::WriteFloat(file, floatValue);
		agk::WriteString(file, stringValue.c_str());
	}
	string getName() {
		return name;
	}
	int getInt() {
		return intValue;
	}
	float getFloat() {
		return floatValue;
	}
	string getString() {
		return stringValue;
	}
	void setInt(int value) {
		intValue = value;
	}
	void setFloat(float value) {
		floatValue = value;
	}
	void setString(string value) {
		stringValue = value;
	}
};

class Save {
private:
	vector<Entry> memory;
public:
	Entry getEntry(string name) {
		Entry result = Entry();
		for (int i = 0; i < memory.size(); i++) {
			if (name == memory[i].getName()) {
				result = memory[i];
				break;
			}
		}
		return result;
	}
	bool read(string filename) {
		memory.erase(memory.begin(), memory.end());
		if (agk::GetFileExists(filename.c_str()) == 1) {
			int file = agk::OpenToRead(filename.c_str());
			int num = agk::ReadInteger(file);
			for (int i = 0; i < num; i++) {
				memory.emplace_back();
				memory.back().read(file);
			}
			agk::CloseFile(file);
			return true;
		}
		else {
			return false;
		}
	}
	void write(string filename) {
		int file = agk::OpenToWrite(filename.c_str());
		agk::WriteInteger(file, memory.size());
		for (int i = 0; i < memory.size(); i++) {
			memory[i].write(file);
		}
		agk::CloseFile(file);
	}
	void setI(string name, int value) {
		bool found = false;
		for (int i = 0; i < memory.size(); i++) {
			if (name == memory[i].getName()) {
				found = true;
				memory[i].setInt(value);
				break;
			}
		}
		if (!found) {
			memory.emplace_back(name, value);
		}
	}
	void setF(string name, float value) {
		bool found = false;
		for (int i = 0; i < memory.size(); i++) {
			if (name == memory[i].getName()) {
				found = true;
				memory[i].setFloat(value);
				break;
			}
		}
		if (!found) {
			memory.emplace_back(name, value);
		}
	}
	void setS(string name, string value) {
		bool found = false;
		for (int i = 0; i < memory.size(); i++) {
			if (name == memory[i].getName()) {
				found = true;
				memory[i].setString(value);
				break;
			}
		}
		if (!found) {
			memory.emplace_back(name, value);
		}
	}
	int getI(string name) {
		return getEntry(name).getInt();
	}
	float getF(string name) {
		return getEntry(name).getFloat();
	}
	string getS(string name) {
		return getEntry(name).getString();
	}
};

class Engine {
private:
	float offsetX;
	float offsetY;
public:
	Engine() {
		offsetX = 0;
		offsetY = 0;
	}
	Engine(string windowTitle, int width, int height, float fps = 60, bool fullscreen = false) {
		agk::SetVirtualResolution(width, height);
		agk::SetWindowSize(width, height, (int)fullscreen);
		agk::SetClearColor(0, 0, 0);
		agk::SetWindowTitle(windowTitle.c_str());
		agk::SetSyncRate(fps, 0);
		agk::SetScissor(0, 0, width, height);
		offsetX = 0;
		offsetY = 0;
	}
	void update() {
		agk::Sync();
	}
	float delta() {
		return agk::GetFrameTime();
	}
	float mouseX() {
		return agk::GetPointerX();
	}
	float mouseY() {
		return agk::GetPointerY();
	}
	float joyX() {
		return agk::GetJoystickX();
	}
	float joyY() {
		return agk::GetJoystickY();
	}
	bool click() {
		return (bool)agk::GetPointerPressed();
	}
	bool keyDown(int key) {
		return (bool)agk::GetRawKeyPressed(key);
	}
	void updateOffset() {
		offsetX = getViewOffsetX();
		offsetY = getViewOffsetY();
	}
	float getViewOffsetX() {
		return agk::GetViewOffsetX();
	}
	float getViewOffsetY() {
		return agk::GetViewOffsetY();
	}
	void setViewOffsetX(float value) {
		agk::SetViewOffset(value, offsetY);
		updateOffset();
	}
	void setViewOffsetRelX(float value) {
		agk::SetViewOffset(offsetX + value, offsetY);
		updateOffset();
	}
	void setViewOffsetY(float value) {
		agk::SetViewOffset(offsetX, value);
		updateOffset();
	}
	void setViewOffsetRelY(float value) {
		agk::SetViewOffset(offsetX, offsetY + value);
		updateOffset();
	}

};

class Sound {
private:
	int soundID;
	int instanceID;
public:
	Sound() {
		soundID = -1;
		instanceID = -1;
	}
	Sound(string filename) {
		soundID = -1;
		instanceID = -1;
		loadSound(filename);
	}
	void loadSound(string filename) {
		if (soundID == -1) {
			soundID = agk::LoadSound(filename.c_str());
		}
		else {
			agk::DeleteSound(soundID);
			soundID = agk::LoadSound(filename.c_str());
		}
	}
	int getID() {
		return soundID;
	}
	void play(int loop = 0) {
		instanceID = agk::PlaySound(soundID, 100, loop);
	}
	bool isPlaying() {
		return (bool)agk::GetSoundInstancePlaying(instanceID);
	}
	void stop() {
		agk::StopSoundInstance(instanceID);
	}
	void destroy() {
		agk::DeleteSound(soundID);
		soundID = -1;
	}
};

class MusicOGG {
private:
	int musicID;
	int volume = 100;
public:
	MusicOGG() {
		musicID = -1;
		volume = 100;
	}
	MusicOGG(string filename) {
		musicID = -1;
		volume = 100;
		loadMusic(filename);
	}
	void loadMusic(string filename) {
		if (musicID == -1) {
			musicID = agk::LoadMusicOGG(filename.c_str());
		}
		else {
			agk::DeleteMusic(musicID);
			musicID = agk::LoadMusicOGG(filename.c_str());
		}
	}
	int getID() {
		return musicID;
	}
	void play(int loop = 0) {
		agk::PlayMusicOGG(musicID, loop);
	}
	bool isPlaying() {
		return (bool)agk::GetMusicPlayingOGG(musicID);
	}
	void stop() {
		agk::StopMusicOGG(musicID);
	}
	void pause() {
		agk::PauseMusicOGG(musicID);
	}
	void resume() {
		agk::ResumeMusicOGG(musicID);
	}
	void destroy() {
		agk::DeleteMusicOGG(musicID);
		musicID = -1;
	}
	void setVolume(int value) {
		volume = value;
		agk::SetMusicVolumeOGG(musicID, value);
	}
	int getVolume() {
		return volume;
	}
};

class Music {
private:
	int musicID;
public:
	Music() {
		musicID = -1;
	}
	Music(string filename) {
		musicID = -1;
		loadMusic(filename);
	}
	void loadMusic(string filename) {
		if (musicID == -1) {
			musicID = agk::LoadMusic(filename.c_str());
		}
		else {
			agk::DeleteMusic(musicID);
			musicID = agk::LoadMusic(filename.c_str());
		}
	}
	int getID() {
		return musicID;
	}
	void play(int loop = 0) {
		agk::PlayMusic(musicID, loop);
	}
	bool isPlaying() {
		return (bool)agk::GetMusicPlaying();
	}
	void stop() {
		agk::StopMusic();
	}
	void destroy() {
		agk::DeleteMusic(musicID);
		musicID = -1;
	}
};

class Font {
private:
	int fontID;
public:
	Font() {
		fontID = -1;
	}
	Font(string filename) {
		fontID = -1;
		loadFont(filename);
	}
	void loadFont(string filename) {
		fontID = agk::LoadFont(filename.c_str());
	}
	void destroy() {
		agk::DeleteFont(fontID);
		fontID = -1;
	}
	int getID() {
		return fontID;
	}

};

class Text {
private:
	Font ttFont;
	int textID;
	string ss(static char* value) {
		string result = "";
		result = value;
		delete value;
		return result;
	}
public:
	Text() {
		ttFont = Font();
		textID = -1;
	}
	Text(Font ttf, string content, float size, float x, float y, int alignment) {

		textID = -1;
		createString(content);
		setFont(ttf);
		setSize(size);
		setX(x);
		setY(y);
		setAlignment(alignment);
	}
	void createString(string content) {
		if (textID != -1) {
			agk::DeleteText(textID);
		}
		textID = agk::CreateText(content.c_str());
	}
	void setFont(Font ttf) {
		ttFont = ttf;
		agk::SetTextFont(textID, ttFont.getID());
	}
	void setSize(float size) {
		agk::SetTextSize(textID, size);
	}
	void setX(float x) {
		agk::SetTextX(textID, x);
	}
	void setVisible(bool value) {
		agk::SetTextVisible(textID, (int)value);
	}
	void setY(float y) {
		agk::SetTextY(textID, y);
	}
	float getX() {
		return agk::GetTextX(textID);
	}
	float getY() {
		return agk::GetTextY(textID);
	}
	string getString() {
		return ss(agk::GetTextString(textID));
	}
	void setString(string content) {
		agk::SetTextString(textID, content.c_str());
	}
	void setColor(int r, int g, int b, int a=255) {
		agk::SetTextColor(textID, r, g, b, a);
	}
	int getColorRed() {
		return agk::GetTextColorRed(textID);
	}
	int getColorGreen() {
		return agk::GetTextColorGreen(textID);
	}
	int getColorBlue() {
		return agk::GetTextColorBlue(textID);
	}
	int getColorAlpha() {
		return agk::GetTextColorAlpha(textID);
	}
	int getID() {
		return textID;
	}
	void fixTextToScreen(bool value) {
		agk::FixTextToScreen(textID, (int)value);
	}
	void setAlignment(int align) {
		agk::SetTextAlignment(textID, align);
	}
	void destroy() {
		agk::DeleteText(textID);
		textID = -1;
	}

};

template<class T> class Moment {
private:
	T *obj;
	function<void(T&)> actionf;
	function<bool(T&)> questionf;
	float delay;
	bool isQuestion;
	bool remove;
	bool active;
public:
	Moment(T *nobj, function<void(T&)> nAction, float nDelay, bool nRemove) {
		obj = nobj;
		actionf = nAction;
		delay = nDelay;
		remove = nRemove;
		active = true;
		isQuestion = false;
	}
	Moment(T *nobj, function<bool(T&)> nQuestion, bool nRemove, float nDelay) {
		obj = nobj;
		questionf = nQuestion;
		remove = nRemove;
		isQuestion = true;
		delay = nDelay;
		active = true;
	}
	bool getIsQuestion() {
		return isQuestion;
	}
	bool getActive() {
		return active;
	}
	void setActive(bool value) {
		active = value;
	}
	float getDelay() {
		return delay;
	}
	bool shouldRemove() {
		return remove;
	}
	void action() {
		actionf(*obj);
	}
	bool question() {
		return questionf(*obj);
	}
};

template<class T> class Timeline {
private:
	vector<Moment<T>> events;
	Timer clock;
	int index;
	bool running;
public:
	Timeline() {
		clock = timer();
		index = 0;
		running = false;
	}
	bool atLeastOne() {
		bool result = false;
		for (int i = 0; i < events.size(); i++) {
			if (events[i].getActive()) {
				result = true;
				break;
			}
		}
		return result;
	}
	void nextEvent() {
		index++;
		if (index < events.size()) {
			if (atLeastOne()) {
				if (events[index].getActive()) {
					clock.set(events[index].getDelay());
				}
				else {
					nextEvent();
				}
			}
			else {
				running = false;
			}
		}
		else {
			index = -1;
			nextEvent();
		}
	}
	void update(float delta) {
		if (running) {
			clock.update(delta);
			if (clock.check()) {
				if (events[index].getIsQuestion()) {
					if (events[index].question()) {
						if (events[index].shouldRemove()) {
							events[index].setActive(false);
						}
						nextEvent();
					}
				}
				else {
					events[index].action();
					if (events[index].shouldRemove()) {
						events[index].setActive(false);
					}
					nextEvent();
				}
			}
		}
	}
	void addAction(T *instance, function<void(T&)> func, float delay = 0, bool remove = true) {
		events.emplace_back(instance, func, delay, remove);
	}
	void addQuestion(T *instance, function<bool(T&)> func, bool remove = true, float delay=0) {
		events.emplace_back(instance, func, remove, delay);
	}
	void start() {
		if (events.size() > 0) {
			index = 0;
			nextEvent();
			running = true;
			clock.start();
		}
	}
	void stop() {
		running = false;
	}
	void reset() {
		for_each(events.begin(), events.end(), [&](Moment i) { i.setActive(true); })
	}
};

class GuiButton {
private:
	DynamicEntity object;
	Text text;
	Sound hoverSound;
	bool hasHoverSound;
	Sound clickSound;
	bool hasClickSound;
	string idleAni;
	string hoverAni;
	string clickAni;
	bool isClicked;
	bool hasText;
	bool active;
	float startX;
	float startY;
	float preX;
	float preY;
public:
	GuiButton() {
		startX = 0;
		startY = 0;
		preX = 0;
		preY = 0;
		idleAni = "";
		hoverAni = "";
		clickAni = "";
		hasText = false;
		hasHoverSound = false;
		hasClickSound = false;
		isClicked = false;
		active = false;
	}
	GuiButton(string filename, float x, float y, float w, float h) {
		object = DynamicEntity(filename, x, y);
		startX = x;
		startY = y;
		preX = x;
		preY = y;
		object.setSize(w, h);
		idleAni = "";
		hoverAni = "";
		clickAni = "";
		hasText = false;
		hasHoverSound = false;
		hasClickSound = false;
		isClicked = false;
		active = true;
	}
	GuiButton(int imageID, float x, float y, float w, float h) {
		object = DynamicEntity();
		object.setImage(imageID);
		object.setPos(x, y);
		startX = x;
		startY = y;
		preX = x;
		preY = y;
		object.setSize(w, h);
		idleAni = "";
		hoverAni = "";
		clickAni = "";
		hasText = false;
		hasHoverSound = false;
		hasClickSound = false;
		isClicked = false;
		active = true;
	}
	DynamicEntity getEntity() {
		return object;
	}
	void returnToStartPos(float duration, int tween) {
		if (startX != object.getX() || startY != object.getY()) {
			float distance = sqrt(pow(startX - object.getX(), 2) + pow(startY - object.getY(), 2));
			object.moveToPoint(startX, startY, distance / duration, true, tween);
		}
	}
	void setupText(string ttfFilename, string textValue, float size, int r = 255, int g = 255, int b = 255) {
		hasText = true;
		text = Text(Font(ttfFilename), textValue, size, object.getX() + (object.getW() / 2.0), object.getY() + (object.getH() / 3.0), 1);
		text.setColor(r, g, b);
		preX = text.getX();
		preY = text.getY();
	}
	void setupText(Font ttf, string textValue, float size, int r = 255, int g = 255, int b = 255) {
		hasText = true;
		text = Text(ttf, textValue, size, object.getX() + (object.getW() / 2.0), object.getY() + (object.getH() / 3.0), 1);
		text.setColor(r, g, b);
		preX = text.getX();
		preY = text.getY();
	}
	void setIdleAnimation(Animation idle) {
		object.addAnimation(idle);
		idleAni = idle.getName();
	}
	void setHoverAnimation(Animation hover) {
		object.addAnimation(hover);
		hoverAni = hover.getName();
	}
	void setClickAnimation(Animation click) {
		object.addAnimation(click);
		clickAni = click.getName();
	}
	void addHoverSound(string filename) {
		hasHoverSound = true;
		hoverSound = Sound(filename);
	}
	void addClickSound(string filename) {
		hasClickSound = true;
		clickSound = Sound(filename);
	}
	void setFixed(bool value) {
		object.setFixed(value);
		if (hasText) {
			text.fixTextToScreen(value);
		}
	}
	bool clicked() {
		if (isClicked) {
			isClicked = false;
			return true;
		}
		else return false;
	}
	void update(Engine &e) {
		if (active) {

			object.update(e.delta());
			if (hasText) {
				text.setX(object.getX() + (object.getW() / 2.0));
				text.setY(object.getY() + (object.getH() / 3.0));
			}

			if (isClicked) isClicked = false;
			if ((e.mouseX()>object.getX()&&e.mouseX()<object.getX()+object.getW())&&(e.mouseY()>object.getY()&&e.mouseY()<object.getY()+object.getH())) {
				if (e.click()) {
					if (clickAni != "") {
						if (object.getAnimationPlaying() != clickAni) {
							object.setAnimation(clickAni);
							if (hasClickSound) {
								clickSound.play();
							}
						}
					}
					else {
						if (hasClickSound) {
							clickSound.play();
						}
						isClicked = true;
					}
				}
				else {
					if (hoverAni != "" && object.getAnimationPlaying() != hoverAni) {
						object.setAnimation(hoverAni);
						if (hasHoverSound) {
							hoverSound.play();
						}
					}
				}
			}
			else {
				if (clickAni != "" && object.getAnimationPlaying() == clickAni) {
					if (!object.getAnimationLooping()) {
						isClicked = true;
						if (idleAni != "") {
							object.setAnimation(idleAni);
						}
					}
					else if (!object.isAnimationPlaying()) {
						isClicked = true;
						if (idleAni != "") {
							object.setAnimation(idleAni);
						}
					}
				}
				else {
					if (idleAni != "") {
						object.setAnimation(idleAni);
					}
				}
			}
		}
	}
	void setVisible(bool value) {
		object.setVisible(value);
		if (hasText) {
			text.setVisible(value);
		}
	}
	void setActive(bool value) {
		object.setActive(value);
		if (hasText) {
			text.setVisible(value);
		}
		active = value;
	}
};

struct NamedButton {
	GuiButton button;
	string name;
	NamedButton(GuiButton newButton, string newName) {
		button = newButton;
		name = newName;
	}
};

class ButtonGroup {
private:
	vector<NamedButton> buttons;
public:
	void addButton(GuiButton button, string name) {
		buttons.emplace_back(button, name);
	}
	void update(Engine &e) {
		for (int i = 0; i < buttons.size(); i++) {
			buttons[i].button.update(e);
		}
	}
	GuiButton getButton(string name) {
		GuiButton result = GuiButton();
		for (int i = 0; i < buttons.size(); i++) {
			if (buttons[i].name == name) {
				result = buttons[i].button;
				break;
			}
		}
		return result;
	}
	bool isClicked(string name) {
		bool result = false;
		for (int i = 0; i < buttons.size(); i++) {
			if (buttons[i].name == name) {
				result = buttons[i].button.clicked();
				break;
			}
		}
		return result;
	}
	void setVisible(bool value) {
		for (int i = 0; i < buttons.size(); i++) {
			buttons[i].button.setVisible(value);
		}
	}
	void setActive(bool value) {
		for (int i = 0; i < buttons.size(); i++) {
			buttons[i].button.setActive(value);
		}
	}
};

class Skeleton {
private:
	int skeletonID;
	int spriteID;
	float speed;
	string current;
	float offsetX;
	float offsetY;
public:
	Skeleton() {
		skeletonID = -1;
		spriteID = -1;
		speed = 1.0;
		current = "";
		offsetX = 0;
		offsetY = 0;
	}
	Skeleton(string filename, float scale, float x = 0, float y = 0) {
		skeletonID = agk::LoadSkeleton2DFromSpriterFile(filename.c_str(), scale, 0);
		spriteID = -1;
		setPos(x, y);
		speed = 1.0;
		current = "";
		offsetX = 0;
		offsetY = 0;
	}
	void setPos(float x, float y) {
		agk::SetSkeleton2DPosition(skeletonID, x, y);
	}
	void lockToSprite(int id, float relX, float relY) {
		spriteID = id;
		offsetX = relX;
		offsetY = relY;
	}
	float getSpeed() {
		return speed;
	}
	void setSpeed(float value) {
		speed = value;
		agk::SetSkeleton2DAnimationSpeed(skeletonID, speed);
	}
	string getAnimationName() {
		return current;
	}
	bool isAnimating() {
		return (bool)agk::GetSkeleton2DIsAnimating(skeletonID);
	}
	void playAnimation(string animationName, float tweenTime, int loop=1) {
		agk::PlaySkeleton2DAnimation(skeletonID, animationName.c_str(), 0, loop, tweenTime);
		current = animationName;
	}
	void setVisible(bool value) {
		agk::SetSkeleton2DVisible(skeletonID, (int)value);
	}
	void setFlip(bool value) {
		agk::SetSkeleton2DFlip(skeletonID, (int)value, 0);
	}
	void update(bool fixed=false) {
		if (spriteID != -1) {
			if (fixed) {
				agk::SetSkeleton2DPosition(skeletonID, (agk::ScreenToWorldX(agk::GetSpriteX(spriteID) + (agk::GetSpriteWidth(spriteID) / 2.0)))+offsetX, (agk::ScreenToWorldY(agk::GetSpriteY(spriteID) + (agk::GetSpriteHeight(spriteID) / 2.0)))+offsetY);
			}
			else {
				agk::SetSkeleton2DPosition(skeletonID, (agk::GetSpriteX(spriteID) + (agk::GetSpriteWidth(spriteID) / 2.0))+offsetX, (agk::GetSpriteY(spriteID) + (agk::GetSpriteHeight(spriteID) / 2.0))+offsetY);
			}
			agk::SetSkeleton2DRotation(skeletonID, agk::GetSpriteAngle(spriteID));
		}
	}
};

struct ParticleColorKeyFrame {
	float time;
	int r;
	int g;
	int b;
	int a;
	ParticleColorKeyFrame(float nTime, int nr, int ng, int nb, int na) {
		time = nTime;
		r = nr;
		g = ng;
		b = nb;
		a = na;
	}
};

struct ParticleForceKeyFrame {
	float startTime;
	float endTime;
	float x;
	float y;
	ParticleForceKeyFrame(float start, float end, float nx, float ny) {
		startTime = start;
		endTime = end;
		x = nx;
		y = ny;
	}
};

struct ParticleScaleKeyFrame {
	float time;
	float scale;
	ParticleScaleKeyFrame(float atTime, float nScale) {
		time = atTime;
		scale = nScale;
	}
};

struct Emitter {
	int ID=-1;
	int offsetX=0;
	int offsetY=0;
	int spriteToFollow = -1;
	Emitter(int nID, int nOffX, int nOffY, int nSprite) {
		ID = nID;
		offsetX = nOffX;
		offsetY = nOffY;
		spriteToFollow = nSprite;
	}
};

class ParticleEmitter {
private:
	vector<ParticleColorKeyFrame> colorFrames;
	vector<ParticleForceKeyFrame> forceFrames;
	vector<ParticleScaleKeyFrame> scaleFrames;
	vector<Emitter> particles;
	bool fixToScreen = false;
	bool colorInterpolation = true;
	int depth = 0;
	int imageID = -1;
	float vx = 0;
	float vy = 0;
	bool faceDirection = false;
	float freq = 1;
	float life = 5;
	int max = 50;
	float rotationmin = 0;
	float rotationmax = 0;
	float size = 1;
	float x1 = 0;
	float x2 = 0;
	float y1 = 0;
	float y2 = 0;
	int blendMode = 1;
	float minVel = 1;
	float maxVel = 1;
public:
	ParticleEmitter() {

	}
	ParticleEmitter(string filename, float nLife, int nMax=-1) {
		imageID = agk::LoadImage(filename.c_str());
		life = nLife;
		max = nMax;
	}
	void addColorKeyFrame(float time, int r, int g, int b, int a) {
		colorFrames.emplace_back(time, r, g, b, a);
	}
	void addForceKeyFrame(float start, float end, float x, float y) {
		forceFrames.emplace_back(start, end, x, y);
	}
	void addScaleKeyFrame(float time, float scale) {
		scaleFrames.emplace_back(time, scale);
	}
	void setFixed(bool value) {
		fixToScreen = value;
	}
	void setColorInterpolation(bool value) {
		colorInterpolation = value;
	}
	void setDepth(int value) {
		depth = value;
	}
	void setDirection(float nvx, float nvy) {
		vx = nvx;
		vy = nvy;
	}
	void setFaceDirection(bool value) {
		faceDirection = value;
	}
	void setFrequency(float value) {
		freq = value;
	}
	void setLife(float value) {
		life = value;
	}
	void setMax(int value) {
		max = value;
	}
	void setRotationRange(float min, float max) {
		rotationmin = min;
		rotationmax = max;
	}
	void setSize(float value) {
		size = value;
	}
	void setStartZone(float nx1, float ny1, float nx2, float ny2) {
		x1 = nx1;
		y1 = ny1;
		x2 = nx2;
		y2 = ny2;
	}
	void setBlendMode(int value = 1) {
		blendMode = value;
	}
	void setVelRange(float min, float max) {
		minVel = min;
		maxVel = max;
	}
	void create(float x, float y, int sprite = -1, int offx = 0, int offY = 0) {
		int id = agk::CreateParticles(x, y);
		for (int i = 0; i < colorFrames.size(); i++) {
			agk::AddParticlesColorKeyFrame(id, colorFrames[i].time, colorFrames[i].r, colorFrames[i].g, colorFrames[i].b, colorFrames[i].a);
		}
		for (int i = 0; i < forceFrames.size(); i++) {
			agk::AddParticlesForce(id, forceFrames[i].startTime, forceFrames[i].endTime, forceFrames[i].x, forceFrames[i].y);
		}
		for (int i = 0; i < scaleFrames.size(); i++) {
			agk::AddParticlesScaleKeyFrame(id, scaleFrames[i].time, scaleFrames[i].scale);
		}
		agk::FixParticlesToScreen(id, (int)fixToScreen);
		agk::SetParticlesColorInterpolation(id, (int)colorInterpolation);
		agk::SetParticlesDepth(id, depth);
		agk::SetParticlesDirection(id, vx, vy);
		agk::SetParticlesFaceDirection(id, (int)faceDirection);
		agk::SetParticlesFrequency(id, freq);
		agk::SetParticlesImage(id, imageID);
		agk::SetParticlesLife(id, life);
		agk::SetParticlesMax(id, max);
		agk::SetParticlesRotationRange(id, rotationmin, rotationmax);
		agk::SetParticlesSize(id, size);
		agk::SetParticlesStartZone(id, x1, y1, x2, y2);
		agk::SetParticlesTransparency(id, blendMode);
		agk::SetParticlesVelocityRange(id, minVel, maxVel);

		particles.emplace_back(id, offx, offY, sprite);
	}
	void update() {
		for (int i = 0; i < particles.size(); i++) {
			if (agk::GetParticlesMaxReached(particles[i].ID) == 1) {
				agk::DeleteParticles(particles[i].ID);
				particles.erase(particles.begin() + i);
				i--;
			}
			else if(particles[i].spriteToFollow!=-1) {
				agk::SetParticlesPosition(particles[i].ID, agk::GetSpriteX(particles[i].spriteToFollow) + particles[i].offsetX, agk::GetSpriteY(particles[i].spriteToFollow) + particles[i].offsetY);
			}
		}
	}
	void setActive(bool value) {
		for (int i = 0; i < particles.size(); i++) {
			agk::SetParticlesActive(particles[i].ID, (int)value);
		}
	}
	void destroyAll() {
		for (int i = 0; i < particles.size(); i++) {
			agk::DeleteParticles(particles[i].ID);
			particles.erase(particles.begin() + i);
			i--;
		}
	}
};