#ifndef _MAP_H
#define _MAP_H

#include <map>
#include <vector>

#include "stdafx.h"
#include "Object.h"
#include "AIMesh.h"
#include "Champion.h"
#include "CollisionHandler.h"

class Game;

class Map {

protected:
   std::map<uint32, Object*> objects;
   std::vector<uint32> expToLevelUp;
   int waveNumber;
   const uint64 firstSpawnTime;
   const uint64 spawnInterval;
   uint64 gameTime;
   uint64 nextSpawnTime;
   uint64 firstGoldTime; // Time that gold should begin to generate
   Game* game;
   bool firstBlood;
   bool killReduction;
   AIMesh mesh;

   CollisionHandler *collisionHandler;
   
public:
   Map(Game* game, uint64 firstSpawnTime, uint64 spawnInterval, uint64 firstGoldTime);
   virtual ~Map() { delete collisionHandler; }
   virtual void update(long long diff);
   virtual float getGoldPerSecond() = 0;
   virtual bool spawn() = 0;
   
   Object* getObjectById(uint32 id);
   void addObject(Object* o);
   void removeObject(Object* o);
   const std::vector<uint32>& getExpToLevelUp() { return expToLevelUp; }
   uint64 getGameTime() { return gameTime; }
   uint64 getFirstGoldTime() { return firstGoldTime; }
   virtual const Target getRespawnLoc(int side) const = 0;
   virtual float getGoldFor(Unit* u) const = 0;
   virtual float getExpFor(Unit* u) const = 0 ;
   
   Game* getGame() const { return game; }

   CollisionHandler* getPathFinder() { return collisionHandler; }
   
   const std::map<uint32, Object*>& getObjects() { return objects; }
   void stopTargeting(Unit* target);

   std::vector<Champion*> getChampionsInRange(Target* t, float range);
   
   bool getFirstBlood() { return firstBlood; }
   bool setFirstBlood(bool state) { firstBlood = state; }

   AIMesh *getAIMesh() { return &mesh; }
   
   bool getKillReduction() { return killReduction; }
   bool setKillReduction(bool state) { killReduction = state; }
};

#endif